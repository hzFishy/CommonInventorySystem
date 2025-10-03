// By hzFishy - 2025 - Do whatever you want with it.


#include "AsyncActions/CCSAsyncAction_AsyncLoadItemDefinitionForFragmentType.h"
#include "Core/CISInventorySubsystem.h"


UCCSAsyncAction_AsyncLoadItemDefinitionForFragmentType* UCCSAsyncAction_AsyncLoadItemDefinitionForFragmentType::AsyncLoadItemDefinitionForFragmentType(
	UObject* WorldContextObject,
	const FCISAsyncInventoryItemDefinitionRequest& InRequestId,
	TSoftObjectPtr<UCISInventoryItemDefinition> InItemDefinition,
	UScriptStruct* InFragmentClass)
{
	auto* Action = NewObject<UCCSAsyncAction_AsyncLoadItemDefinitionForFragmentType>();
	Action->WeakWorldContextObject = WorldContextObject;
	Action->ItemDefinition = InItemDefinition;
	Action->FragmentClass = InFragmentClass;
	Action->Request = InRequestId;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UCCSAsyncAction_AsyncLoadItemDefinitionForFragmentType::Activate()
{
	Super::Activate();

	if (!FragmentClass.IsValid() || !ItemDefinition.IsValid())
	{
		OnAsyncLoadItemDefinitionForFragmentTypeFinishedDelegate.Broadcast(
			Request,
			TInstancedStruct<FCISBaseInventoryItemFragment>::Make()
		);
		SetReadyToDestroy();
	}
	
	if (auto* AM = UAssetManager::GetIfInitialized())
	{
		AM->GetStreamableManager().RequestAsyncLoad(
			ItemDefinition.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(this, [this]()
			{
				auto* LoadedDefinition = ItemDefinition.Get();

				TInstancedStruct<FCISBaseInventoryItemFragment> FoundStructInstance;
				if (LoadedDefinition->GetFragmentFromType(FragmentClass.Get(), false, FoundStructInstance))
				{
					OnAsyncLoadItemDefinitionForFragmentTypeFinishedDelegate.Broadcast(Request, FoundStructInstance);
				}
				else
				{	
					OnAsyncLoadItemDefinitionForFragmentTypeFinishedDelegate.Broadcast(
						Request,
						TInstancedStruct<FCISBaseInventoryItemFragment>::Make()
					);
				}
			})
		);
	}
}
