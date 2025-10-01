// By hzFishy - 2025 - Do whatever you want with it.


#include "AsyncActions/CCSAsyncAction_AsyncLoadItemDefinitionFromItemTag.h"
#include "Core/CISInventorySubsystem.h"


UCCSAsyncAction_AsyncLoadItemDefinitionFromItemTag* UCCSAsyncAction_AsyncLoadItemDefinitionFromItemTag::AsyncLoadItemDefinitionsFromItemTags(
	UObject* WorldContextObject, FGameplayTagContainer ItemTags)
{
	auto* Action = NewObject<UCCSAsyncAction_AsyncLoadItemDefinitionFromItemTag>();
	Action->WeakWorldContextObject = WorldContextObject;
	Action->ItemTags = ItemTags;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UCCSAsyncAction_AsyncLoadItemDefinitionFromItemTag::Activate()
{
	Super::Activate();

	auto* FoundWorld = GEngine->GetWorldFromContextObject(WeakWorldContextObject.Get(), EGetWorldErrorMode::LogAndReturnNull);
	if (auto* InventorySubsystem = FoundWorld->GetGameInstance()->GetSubsystem<UCISInventorySubsystem>())
	{
		InventorySubsystem->AsyncLoadItemDefinitionsFromItemTags(ItemTags, [this](const TArray<UCISInventoryItemDefinition*>& LoadedItemDefinitions)
		{
			OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate.Broadcast(LoadedItemDefinitions);
			SetReadyToDestroy();
		});
	}
}
