// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "Data/CISInventoryTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CISInventoryLibrary.generated.h"


/**
 * 
 */
UCLASS(DisplayName="Inventory Library")
class COMMONINVENTORYSYSTEM_API UCISInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static FCISAsyncInventoryItemDefinitionRequest MakeAsyncItemDefinitionRequest(const UObject* WorldContextObject);

};
