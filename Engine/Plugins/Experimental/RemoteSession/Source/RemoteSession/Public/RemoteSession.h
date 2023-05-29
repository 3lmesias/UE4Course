// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRemoteSessionRole.h"
#include "Modules/ModuleManager.h"

#define REMOTE_SESSION_VERSION_STRING TEXT("1.0.5")

REMOTESESSION_API DECLARE_LOG_CATEGORY_EXTERN(LogRemoteSession, Log, All);

class IRemoteSessionChannelFactoryWorker;

class REMOTESESSION_API IRemoteSessionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override = 0;
	virtual void ShutdownModule() override = 0;
	/** End IModuleInterface implementation */

public:

	enum
	{
		kDefaultPort = 2049,
	};

public:

	/** Register a third party channel */
	virtual void AddChannelFactory(TWeakPtr<IRemoteSessionChannelFactoryWorker> Worker) = 0;

	/** Unregister a channel factory */
	virtual void RemoveChannelFactory(TWeakPtr<IRemoteSessionChannelFactoryWorker> Worker) = 0;

	/** Find a registered party channel factory */
	virtual TSharedPtr<IRemoteSessionChannelFactoryWorker> FindChannelFactoryWorker(const TCHAR* Type) = 0;

public:
	/** Client implementation */
	
	/** Returns a reference to the client role (if any) */
	virtual TSharedPtr<IRemoteSessionRole> CreateClient(const TCHAR* RemoteAddress) = 0;

	/** Stops the client. After this CreateClient() must be called if a new connection is desired */
	virtual void StopClient(TSharedPtr<IRemoteSessionRole> Client) = 0;

    
public:
	/** Server implementation */

	/** Starts a RemoteSession server that listens for clients on the provided port */
	virtual void InitHost(const int16 Port=0) = 0;

	/** Returns true/false based on the running state of the host server */
	virtual bool IsHostRunning() const = 0;

	/** Returns true/false if a client is connected */
	virtual bool IsHostConnected() const = 0;

	/** Stops the server, after this InitHost() must be called if a new connection is desired */
	virtual void StopHost() = 0;

	/** Programatically sets the desired channels. Defaults are Input=Receive and Framebuffer=Send. Unioned with values from ini file */
	virtual void SetSupportedChannels(TMap<FString, ERemoteSessionChannelMode>& SupportedChannels) = 0;

	/** Programatically add the desired channels. Defaults are Input=Receive and Framebuffer=Send. Unioned with values from ini file */
	virtual void AddSupportedChannel(FString Type, ERemoteSessionChannelMode Mode) = 0;

	/** Programatically add the desired channels. Defaults are Input=Receive and Framebuffer=Send. Unioned with values from ini file */
	virtual void AddSupportedChannel(FString Type, ERemoteSessionChannelMode Mode, FOnRemoteSessionChannelCreated OnCreated) = 0;

	/** Returns a reference to the server role (if any) */
	virtual TSharedPtr<IRemoteSessionRole> GetHost() const = 0;

	/** Returns host that is not controlled by the module */
	virtual TSharedPtr<IRemoteSessionUnmanagedRole> CreateHost(TArray<FRemoteSessionChannelInfo> SupportedChannels, int32 Port) const = 0;
};
