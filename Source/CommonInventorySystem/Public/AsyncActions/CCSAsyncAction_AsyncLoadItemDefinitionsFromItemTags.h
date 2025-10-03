// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/CISInventoryTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags.generated.h"
class UCISInventoryItemDefinition;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCCSOnAsyncLoadItemDefinitionsFromItemTagsFinishedOutputPin,
	const FCISAsyncInventoryItemDefinitionRequest&, Request,
	const TArray<UCISInventoryItemDefinition*>&, LoadedItemDefinitions
);


/**
 * 
 */
UCLASS()
class COMMONINVENTORYSYSTEM_API UCCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Async Load Item Definitions From Item Tags", meta=(BlueprintInternalUseOnly="true", Category="CommonCraftingSystem", WorldContext = "WorldContextObject"))
	static UCCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags* AsyncLoadItemDefinitionsFromItemTags(UObject* WorldContextObject,
		FCISAsyncInventoryItemDefinitionRequest InRequest, FGameplayTagContainer ItemTags);
	
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable, Category="CommonInventorySystem", DisplayName="Finished")
	FCCSOnAsyncLoadItemDefinitionsFromItemTagsFinishedOutputPin OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate;

	FCISAsyncInventoryItemDefinitionRequest Request;
	FGameplayTagContainer ItemTags;
	TWeakObjectPtr<UObject> WeakWorldContextObject;
};
