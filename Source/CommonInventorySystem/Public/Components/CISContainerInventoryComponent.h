// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "CISBaseInventoryComponent.h"
#include "CISContainerInventoryComponent.generated.h"


UCLASS(ClassGroup=(CommonInventorySystem), DisplayName="Container Inventory Component", meta=(BlueprintSpawnableComponent))
class COMMONINVENTORYSYSTEM_API UCISContainerInventoryComponent : public UCISBaseInventoryComponent
{
	GENERATED_BODY()


	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISContainerInventoryComponent();
};
