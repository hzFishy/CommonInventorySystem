// By hzFishy - 2025 - Do whatever you want with it.

#pragma once
#include "Components/WidgetComponent.h"

#include "CISInventoryTypes.generated.h"


namespace CIS::Core
{
	inline FName NAME_InventoryItemDefinitionType = "InventoryItemDefinition";
}


USTRUCT(BlueprintType, DisplayName = "Async Inventory Item Definition Request")
struct COMMONINVENTORYSYSTEM_API FCISAsyncInventoryItemDefinitionRequest
{
	GENERATED_BODY()
	
	FCISAsyncInventoryItemDefinitionRequest();
	FCISAsyncInventoryItemDefinitionRequest(uint32 InId);

	bool operator==(const FCISAsyncInventoryItemDefinitionRequest& Other) const
	{
		return Id == Other.Id;
	};
	
	friend uint32 GetTypeHash(const FCISAsyncInventoryItemDefinitionRequest& Self)
	{
		return GetTypeHash(Self.Id);
	}
	
	uint32 Id;
};

template<>
struct TStructOpsTypeTraits<FCISAsyncInventoryItemDefinitionRequest> : public TStructOpsTypeTraitsBase2<FCISAsyncInventoryItemDefinitionRequest>
{
	enum
	{
		WithIdenticalViaEquality = true,
	};
};


USTRUCT(BlueprintType, DisplayName="Hotbar Changed Event")
struct COMMONINVENTORYSYSTEM_API FCISHotbarChangedEvent
{
	GENERATED_BODY()

	FCISHotbarChangedEvent();
	FCISHotbarChangedEvent(int32 InOldIndex, int32 InNewIndex);

	UPROPERTY(BlueprintReadWrite)
	int32 OldIndex;
	
	UPROPERTY(BlueprintReadWrite)
	int32 NewIndex;
};


struct COMMONINVENTORYSYSTEM_API FCISInventoryHotbarSelection
{
	FCISInventoryHotbarSelection();
	
	void SetNewChange(float InGameTime);
	
	void SetActor(AActor* InItemActor);

	bool IsActorSet() const { return ItemActor.IsValid(); };
	
	AActor* GetActor() const { return ItemActor.Get(); };

	float GetSetRealTime() const { return RealGameTime; }
	
	void Clear();

protected:
	/** The item actor we spawned in the world */
	TWeakObjectPtr<AActor> ItemActor;
	/** Used to detect race conditions between deferred spawn request */
	float RealGameTime;
};
