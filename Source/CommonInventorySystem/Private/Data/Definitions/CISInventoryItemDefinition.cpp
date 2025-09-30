// By hzFishy - 2025 - Do whatever you want with it.


#include "Data/Definitions/CISInventoryItemDefinition.h"
#include "Core/CISInventoryItem.h"
#include "Data/CISInventoryTypes.h"


UCISInventoryItemDefinition::UCISInventoryItemDefinition():
	ItemClass(UCISInventoryItem::StaticClass())
{}

FPrimaryAssetId UCISInventoryItemDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(CIS::Core::NAME_InventoryItemDefinitionType, FName(Tag.ToString()));
}

bool UCISInventoryItemDefinition::GetFragmentFromType(UScriptStruct* Type, bool bAllowChilds,
	TInstancedStruct<FCISBaseInventoryItemFragment>& FoundStruct) const
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
