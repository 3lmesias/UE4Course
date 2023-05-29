// Copyright Epic Games, Inc. All Rights Reserved.

#include "Drawing/ControlRigDrawContainer.h"

int32 FControlRigDrawContainer::GetIndex(const FName& InName) const
{
	for (int32 Index = 0; Index < Instructions.Num(); ++Index)
	{
		if (Instructions[Index].Name == InName)
		{
			return Index;
		}
	}
	return INDEX_NONE;
}

void FControlRigDrawContainer::Reset()
{
	Instructions.Reset();
}
