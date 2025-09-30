// By hzFishy - 2025 - Do whatever you want with it.


#include "AsyncActions/CCSAsyncAction_AsyncLoadAllCraftingRecipes.h"

#include "Core/CCSCraftingSubsystem.h"


UCCSAsyncAction_AsyncLoadAllCraftingRecipes* UCCSAsyncAction_AsyncLoadAllCraftingRecipes::AsyncLoadAllCraftingRecipes(UObject* WorldContextObject)
{
	auto* Action = NewObject<UCCSAsyncAction_AsyncLoadAllCraftingRecipes>();
	Action->WeakWorldContextObject = WorldContextObject;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UCCSAsyncAction_AsyncLoadAllCraftingRecipes::Activate()
{
	Super::Activate();

	auto* FoundWorld = GEngine->GetWorldFromContextObject(WeakWorldContextObject.Get(), EGetWorldErrorMode::LogAndReturnNull);
	if (auto* CraftingSubsystem = FoundWorld->GetGameInstance()->GetSubsystem<UCCSCraftingSubsystem>())
	{
		CraftingSubsystem->OnAllCraftingRecipesLoadedDelegate.AddWeakLambda(this, [this](const auto& SearchResult)
		{
			OnAllCraftingRecipesLoaded.Broadcast(SearchResult);
			SetReadyToDestroy();
		});
		CraftingSubsystem->AsyncLoadAllCraftingRecipes();
	}
}
