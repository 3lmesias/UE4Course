// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConcertLocalFileSharingService.h"
#include "HAL/FileManager.h"
#include "ConcertUtil.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"


FConcertLocalFileSharingService::FConcertLocalFileSharingService(const FString& Role)
	: SharedRootPathname(FPaths::EngineIntermediateDir() / Role / TEXT("TempFileShare")) // The EngineIntermediateDir is expected to be know of both the client and the server.
	, SystemMutexName(TEXT("ConcertFileSharingMutex_777221B1ZZ")) // Arbitrary name, but unique enough to avoid collision with other global mutexes from other apps.
	, ActiveServicesRepositoryPathname(SharedRootPathname / TEXT("ActiveProcesses.json"))
{
	FSystemWideCriticalSection ScopedSysWideMutex(SystemMutexName);

	TArray<uint32> ActivesPids;
	LoadActiveServices(ActivesPids);
	RemoveDeadProcessesAndFiles(ActivesPids);
	ActivesPids.Add(FPlatformProcess::GetCurrentProcessId());
	SaveActiveServices(ActivesPids);
}

FConcertLocalFileSharingService::~FConcertLocalFileSharingService()
{
	FSystemWideCriticalSection ScopedSysWideMutex(SystemMutexName);

	TArray<uint32> ActivePids;
	LoadActiveServices(ActivePids);
	ActivePids.Remove(FPlatformProcess::GetCurrentProcessId());
	RemoveDeadProcessesAndFiles(ActivePids);
	if (ActivePids.Num()) // If no pids are left, the sharing directory was deleted, don't recreate it to save an empty file.
	{
		SaveActiveServices(ActivePids);
	}
}

void FConcertLocalFileSharingService::RemoveDeadProcessesAndFiles(TArray<uint32>& InOutPids)
{
	// Remove all dead process.
	InOutPids.RemoveAll([](uint32 Pid)
	{
		return !FPlatformProcess::IsApplicationRunning(Pid);
	});

	// If no processes are left using the shared directory, delete it with its content.
	if (InOutPids.Num() == 0)
	{
		IFileManager::Get().DeleteDirectory(*SharedRootPathname, /*RequireExists*/false, /*Tree*/true);
	}
}

bool FConcertLocalFileSharingService::Publish(const FString& Pathname, FString& OutFileId)
{
	// Create a unique name for the file.
	OutFileId = SharedRootPathname / FGuid::NewGuid().ToString();
	return IFileManager::Get().Copy(*OutFileId, *Pathname) == COPY_OK;
}

bool FConcertLocalFileSharingService::Publish(FArchive& SrcAr, int64 Size, FString& OutFileId)
{
	// Create a unique name for the file.
	FString SharedFilePathname = SharedRootPathname / FGuid::NewGuid().ToString();
	TUniquePtr<FArchive> SharedFileWriter(IFileManager::Get().CreateFileWriter(*SharedFilePathname));
	if (SharedFileWriter)
	{
		ConcertUtil::Copy(*SharedFileWriter, SrcAr, Size);
		OutFileId = FPaths::ConvertRelativePathToFull(SharedFilePathname);
		return true;
	}
	return false;
}

TSharedPtr<FArchive> FConcertLocalFileSharingService::CreateReader(const FString& InFileId)
{
	if (IFileManager::Get().FileExists(*InFileId))
	{
		// Delete the files when the archive goes out of scope, so that consumed files don't use too much space.
		auto Deleter = [InFileId](FArchive* Ar)
		{
			delete Ar;
			IFileManager::Get().Delete(*InFileId);
		};

		return MakeShareable(IFileManager::Get().CreateFileReader(*InFileId), MoveTemp(Deleter));
	}

	return nullptr; // File not found.
}

void FConcertLocalFileSharingService::LoadActiveServices(TArray<uint32>& OutPids)
{
	if (IFileManager::Get().FileExists(*ActiveServicesRepositoryPathname))
	{
		TUniquePtr<FArchive> Ar(IFileManager::Get().CreateFileReader(*ActiveServicesRepositoryPathname));
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Ar.Get());

		TArray< TSharedPtr<FJsonValue> > Pids;
		if (FJsonSerializer::Deserialize(JsonReader, Pids))
		{
			for (const TSharedPtr<FJsonValue>& PidVal : Pids)
			{
				uint32 Pid;
				if (PidVal->TryGetNumber(Pid))
				{
					OutPids.Add(Pid);
				}
			}
		}
	}
}

void FConcertLocalFileSharingService::SaveActiveServices(const TArray<uint32>& InPids)
{
	if (IFileManager::Get().DirectoryExists(*SharedRootPathname) || IFileManager::Get().MakeDirectory(*SharedRootPathname, /*Tree*/true))
	{
		TUniquePtr<FArchive> Ar(IFileManager::Get().CreateFileWriter(*ActiveServicesRepositoryPathname));
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(Ar.Get());

		JsonWriter->WriteArrayStart();
		for (uint32 Pid : InPids)
		{
			JsonWriter->WriteValue(static_cast<int64>(Pid));
		}
		JsonWriter->WriteArrayEnd();
	}
}
