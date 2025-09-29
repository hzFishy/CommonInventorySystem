// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CISInventoryItem.h"
#include "Data/Definitions/CISInventoryItemDefinition.h"

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UCISInventoryItem::UCISInventoryItem()
{
	
}

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
void UCISInventoryItem::InitFromDefinition(UCISInventoryItemDefinition* Definition)
{
	Tag = Definition->Tag;
	TraitsTags = Definition->TraitsTags;
	Fragments = Definition->Fragments;
}

bool UCISInventoryItem::HasTrait(FGameplayTag TraitToCheck) const
{
	return TraitsTags.HasAll(TraitsTags);
}

bool UCISInventoryItem::HasTraits(FGameplayTagContainer TraitsToCheck) const
{
	return TraitsTags.HasAll(TraitsTags);
}

FString UCISInventoryItem::GetFragmentsDebugString() const
{
	FString Output = "";
	
	for (auto& Fragment : Fragments)
	{
		if (!Output.IsEmpty())
		{
			Output += ", ";
		}

		Output += Fragment.GetScriptStruct()->GetName();
	}

	return Output;
}

bool UCISInventoryItem::GetFragmentFromType(UScriptStruct* Type, bool bAllowChilds, TInstancedStruct<FCISBaseInventoryItemFragment>& FoundStruct) const
{
	for (auto& Fragment : Fragments)
	{
		if (Fragment.GetScriptStruct() == Type)
		{
			FoundStruct = Fragment;
			return true;
		}
		else if (bAllowChilds && Type->IsChildOf(Fragment.GetScriptStruct()))
		{
			FoundStruct = Fragment;
			return true;
		}
	}
	
	return false;
}
