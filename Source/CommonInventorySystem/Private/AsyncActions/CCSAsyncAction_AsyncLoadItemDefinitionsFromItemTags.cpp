// By hzFishy - 2025 - Do whatever you want with it.


#include "AsyncActions/CCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags.h"
#include "Core/CISInventorySubsystem.h"


UCCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags* UCCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags::AsyncLoadItemDefinitionsFromItemTags(
	UObject* WorldContextObject, FCISAsyncInventoryItemDefinitionRequest InRequest, FGameplayTagContainer ItemTags)
{
	auto* Action = NewObject<UCCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags>();
	Action->WeakWorldContextObject = WorldContextObject;
	Action->ItemTags = ItemTags;
	Action->Request = InRequest;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UCCSAsyncAction_AsyncLoadItemDefinitionsFromItemTags::Activate()
{
	Super::Activate();

	if (ItemTags.IsEmpty())
	{
		OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate.Broadcast(Request, {});
		SetReadyToDestroy();
	}
	
	auto* FoundWorld = GEngine->GetWorldFromContextObject(WeakWorldContextObject.Get(), EGetWorldErrorMode::LogAndReturnNull);
	if (auto* InventorySubsystem = FoundWorld->GetGameInstance()->GetSubsystem<UCISInventorySubsystem>())
	{
		InventorySubsystem->AsyncLoadItemDefinitionsFromItemTags(ItemTags, [this, CachedRequest = Request](const TArray<UCISInventoryItemDefinition*>& LoadedItemDefinitions)
		{
			OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate.Broadcast(CachedRequest, LoadedItemDefinitions);
			SetReadyToDestroy();
		});
	}
}
