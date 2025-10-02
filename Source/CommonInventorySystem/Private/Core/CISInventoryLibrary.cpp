// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CISInventoryLibrary.h"
#include "Core/CISInventorySubsystem.h"


FCISAsyncInventoryItemDefinitionRequest UCISInventoryLibrary::MakeAsyncItemDefinitionRequest(
	const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (auto* InventorySubsystem = World->GetGameInstance()->GetSubsystem<UCISInventorySubsystem>())
		{
			return  InventorySubsystem->MakeAsyncItemDefinitionRequest();
		}
	}

	return FCISAsyncInventoryItemDefinitionRequest(0);
}
