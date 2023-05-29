// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#if !defined(WITH_MLSDK) || WITH_MLSDK

#include "Lumin/CAPIShims/LuminAPI.h"

LUMIN_THIRD_PARTY_INCLUDES_START
#include <ml_found_object.h>
LUMIN_THIRD_PARTY_INCLUDES_END

namespace MLSDK_API
{

CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectTrackerCreate)
#define MLFoundObjectTrackerCreate ::MLSDK_API::MLFoundObjectTrackerCreateShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectTrackerInitSettings)
#define MLFoundObjectTrackerInitSettings ::MLSDK_API::MLFoundObjectTrackerInitSettingsShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectTrackerUpdateSettings)
#define MLFoundObjectTrackerUpdateSettings ::MLSDK_API::MLFoundObjectTrackerUpdateSettingsShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectQuery)
#define MLFoundObjectQuery ::MLSDK_API::MLFoundObjectQueryShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetResultCount)
#define MLFoundObjectGetResultCount ::MLSDK_API::MLFoundObjectGetResultCountShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetResult)
#define MLFoundObjectGetResult ::MLSDK_API::MLFoundObjectGetResultShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetProperty)
#define MLFoundObjectGetProperty ::MLSDK_API::MLFoundObjectGetPropertyShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectRequestPropertyData)
#define MLFoundObjectRequestPropertyData ::MLSDK_API::MLFoundObjectRequestPropertyDataShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetPropertyData)
#define MLFoundObjectGetPropertyData ::MLSDK_API::MLFoundObjectGetPropertyDataShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectSetBinding)
#define MLFoundObjectSetBinding ::MLSDK_API::MLFoundObjectSetBindingShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectRemoveBinding)
#define MLFoundObjectRemoveBinding ::MLSDK_API::MLFoundObjectRemoveBindingShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetBindingCount)
#define MLFoundObjectGetBindingCount ::MLSDK_API::MLFoundObjectGetBindingCountShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetFoundObjectBindingCount)
#define MLFoundObjectGetFoundObjectBindingCount ::MLSDK_API::MLFoundObjectGetFoundObjectBindingCountShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetBinding)
#define MLFoundObjectGetBinding ::MLSDK_API::MLFoundObjectGetBindingShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetBindingByVirtualId)
#define MLFoundObjectGetBindingByVirtualId ::MLSDK_API::MLFoundObjectGetBindingByVirtualIdShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetAvailableLabelsCount)
#define MLFoundObjectGetAvailableLabelsCount ::MLSDK_API::MLFoundObjectGetAvailableLabelsCountShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetUniqueLabel)
#define MLFoundObjectGetUniqueLabel ::MLSDK_API::MLFoundObjectGetUniqueLabelShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectSubscribe)
#define MLFoundObjectSubscribe ::MLSDK_API::MLFoundObjectSubscribeShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectUnsubscribe)
#define MLFoundObjectUnsubscribe ::MLSDK_API::MLFoundObjectUnsubscribeShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetSubscriptionList)
#define MLFoundObjectGetSubscriptionList ::MLSDK_API::MLFoundObjectGetSubscriptionListShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetSubscriptionIdAtIndex)
#define MLFoundObjectGetSubscriptionIdAtIndex ::MLSDK_API::MLFoundObjectGetSubscriptionIdAtIndexShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetStateChangesCount)
#define MLFoundObjectGetStateChangesCount ::MLSDK_API::MLFoundObjectGetStateChangesCountShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetStateChangesCountForIndex)
#define MLFoundObjectGetStateChangesCountForIndex ::MLSDK_API::MLFoundObjectGetStateChangesCountForIndexShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectGetNextStateChange)
#define MLFoundObjectGetNextStateChange ::MLSDK_API::MLFoundObjectGetNextStateChangeShim
CREATE_FUNCTION_SHIM(ml_perception_client, MLResult, MLFoundObjectTrackerDestroy)
#define MLFoundObjectTrackerDestroy ::MLSDK_API::MLFoundObjectTrackerDestroyShim

}

#endif // !defined(WITH_MLSDK) || WITH_MLSDK
