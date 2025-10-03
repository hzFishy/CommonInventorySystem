// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "Data/CISInventoryTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Data/Fragments/CISBaseInventoryItemFragment.h"
#include "StructUtils/InstancedStruct.h"
#include "CCSAsyncAction_AsyncLoadItemDefinitionForFragmentType.generated.h"
class UCISInventoryItemDefinition;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCCSOnAsyncLoadItemDefinitionForFragmentTypeFinishedOutputPin,
	const FCISAsyncInventoryItemDefinitionRequest&, Request,
	TInstancedStruct<FCISBaseInventoryItemFragment>, FoundStruct
);


/**
 * 
 */
UCLASS()
class COMMONINVENTORYSYSTEM_API UCCSAsyncAction_AsyncLoadItemDefinitionForFragmentType : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Async Load Item Definitions From Item Tags", meta=(BlueprintInternalUseOnly="true", Category="CommonCraftingSystem", WorldContext = "WorldContextObject"))
	static UCCSAsyncAction_AsyncLoadItemDefinitionForFragmentType* AsyncLoadItemDefinitionForFragmentType(UObject* WorldContextObject,
		const FCISAsyncInventoryItemDefinitionRequest& InRequestId,
		TSoftObjectPtr<UCISInventoryItemDefinition> InItemDefinition,
		UPARAM(meta=(MetaStruct="/Script/CommonInventorySystem.CISBaseInventoryItemFragment")) UScriptStruct* InFragmentClass);
	
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable, Category="CommonInventorySystem", DisplayName="Finished")
	FCCSOnAsyncLoadItemDefinitionForFragmentTypeFinishedOutputPin OnAsyncLoadItemDefinitionForFragmentTypeFinishedDelegate;

	FCISAsyncInventoryItemDefinitionRequest Request;
	TSoftObjectPtr<UCISInventoryItemDefinition> ItemDefinition;
	TWeakObjectPtr<UScriptStruct> FragmentClass;
	TWeakObjectPtr<UObject> WeakWorldContextObject;
};
