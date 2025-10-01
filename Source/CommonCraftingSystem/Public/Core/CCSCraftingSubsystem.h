// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "Data/Core/CCSCraftTypes.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CCSCraftingSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FCCSOnAsyncLoadAllCraftingRecipesFinishedSignature,
	const FCCSCraftingRecipesSearchResult& /* SearchResult */
);


/**
 * 
 */
UCLASS(DisplayName = "Crafting Subsystem")
class COMMONCRAFTINGSYSTEM_API UCCSCraftingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	FCCSOnAsyncLoadAllCraftingRecipesFinishedSignature OnAllCraftingRecipesLoadedDelegate;

protected:
	TSharedPtr<FStreamableHandle> AsyncLoadAllCraftingRecipesHandle;

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCCSCraftingSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
public:
	UFUNCTION(BlueprintCallable, Category="CommonCraftingSystem")
	void AsyncLoadAllCraftingRecipes();
	
	/**
	 *	Ask the ItemProvider object to craft a specific recipe.
	 *	This means consuming the input items and producing the output items.
	 */
	UFUNCTION(BlueprintCallable, DisplayName="CommonCraftingSystem")
	bool RequestCraftingRecipe(
		UPARAM(meta=(AllowedClasses="/Script/CommonTypes.CTItemProviderInterface")) UObject* ItemProvider,
		const FCCSCraftingRecipeInput& Input, const FCCSCraftingRecipeOutput& Output);
};
