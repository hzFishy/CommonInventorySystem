// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "GameplayTagContainer.h"
// #include "CTTypes.generated.h"


struct COMMONTYPES_API FCTItemProviderQueryEntry
{
	FCTItemProviderQueryEntry();

	FGameplayTag ItemTag;
	int32 Amount;
};

struct COMMONTYPES_API FCTItemProviderQuery
{
	FCTItemProviderQuery();

	TArray<FCTItemProviderQueryEntry> Entries;
};
