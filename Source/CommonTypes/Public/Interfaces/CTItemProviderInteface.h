// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "UObject/Interface.h"
#include "Data/CTTypes.h"
#include "CTItemProviderInteface.generated.h"


UINTERFACE()
class UCTItemProviderInteface : public UInterface
{
	GENERATED_BODY()
};


/**
 * Interface used on objects which contains items.
 * For example, the player inventory.
 */
class COMMONTYPES_API ICTItemProviderInteface
{
	GENERATED_BODY()

public:
	virtual bool ProviderHasItems(FCTItemProviderQuery Query) = 0;
};
