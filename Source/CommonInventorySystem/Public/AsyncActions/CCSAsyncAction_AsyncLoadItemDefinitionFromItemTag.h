// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CCSAsyncAction_AsyncLoadItemDefinitionFromItemTag.generated.h"
class UCISInventoryItemDefinition;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCCSOnAsyncLoadItemDefinitionsFromItemTagsFinishedOutputPin,
	const TArray<UCISInventoryItemDefinition*>&, LoadedItemDefinitions
);


/**
 * 
 */
UCLASS()
class COMMONINVENTORYSYSTEM_API UCCSAsyncAction_AsyncLoadItemDefinitionFromItemTag : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Async Load Item Definitions From Item Tags", meta=(BlueprintInternalUseOnly="true", Category="CommonCraftingSystem", WorldContext = "WorldContextObject"))
	static UCCSAsyncAction_AsyncLoadItemDefinitionFromItemTag* AsyncLoadItemDefinitionsFromItemTags(UObject* WorldContextObject, FGameplayTagContainer ItemTags);
	
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable, Category="CommonCraftingSystem", DisplayName="Finished")
	FCCSOnAsyncLoadItemDefinitionsFromItemTagsFinishedOutputPin OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate;

	FGameplayTagContainer ItemTags;
	TWeakObjectPtr<UObject> WeakWorldContextObject;
};
