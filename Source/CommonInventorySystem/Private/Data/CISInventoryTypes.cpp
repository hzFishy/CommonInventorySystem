// By hzFishy - 2025 - Do whatever you want with it.


#include "Data/CISInventoryTypes.h"


FCISAsyncInventoryItemDefinitionRequest::FCISAsyncInventoryItemDefinitionRequest():
	Id(0)
{}

FCISAsyncInventoryItemDefinitionRequest::FCISAsyncInventoryItemDefinitionRequest(uint32 InId):
	Id(InId)
{}


FCISHotbarChangedEvent::FCISHotbarChangedEvent():
	OldIndex(-1),
	NewIndex(-1)
{}

FCISHotbarChangedEvent::FCISHotbarChangedEvent(int32 InOldIndex, int32 InNewIndex):
	OldIndex(InOldIndex),
	NewIndex(InNewIndex)
{}
