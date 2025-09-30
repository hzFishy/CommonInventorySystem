// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "Data/Core/CCSCraftTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CCSAsyncAction_AsyncLoadAllCraftingRecipes.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCCSOnAllCraftingRecipesLoadedOutputPin,
	const FCCSCraftingRecipesSearchResult&, SearchResult
);


/**
 * 
 */
UCLASS()
class COMMONCRAFTINGSYSTEM_API UCCSAsyncAction_AsyncLoadAllCraftingRecipes : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Async Load All Crafting Recipes", meta=(BlueprintInternalUseOnly="true", Category="CommonCraftingSystem", WorldContext = "WorldContextObject"))
	static UCCSAsyncAction_AsyncLoadAllCraftingRecipes* AsyncLoadAllCraftingRecipes(UObject* WorldContextObject);
	
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable, Category="CommonCraftingSystem", DisplayName="On All Crafting Recipes Loaded")
	FCCSOnAllCraftingRecipesLoadedOutputPin OnAllCraftingRecipesLoaded;

	TWeakObjectPtr<UObject> WeakWorldContextObject;
};
