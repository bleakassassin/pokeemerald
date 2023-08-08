#ifndef GUARD_REGISTERED_ITEMS_MENU
#define GUARD_REGISTERED_ITEMS_MENU

struct TxRegItemsMenu_ItemPageStruct
{
    u16 cursorPos;
    u16 itemsAbove;
    u8 pageItems;
    u8 count;
    u8 filler[2];
    u8 scrollIndicatorTaskId;
    u8 flashTaskId;
};

extern const u16 gRegisteredItems[];

void TxRegItemsMenu_RegisteredItemsMenuNewGame(void);
void TxRegItemsMenu_OpenMenu(void);
u8 TxRegItemsMenu_CountUsedRegisteredItemSlots(void);
u8 MapRegisteredItem(u16 itemId);
void TxRegItemsMenu_RemoveRegisteredItem(u16 itemId);
void TxRegItemsMenu_CompactRegisteredItems(void);
bool8 TxRegItemsMenu_AddRegisteredItem(u16 itemId);
bool8 TxRegItemsMenu_CheckRegisteredHasItem(u16 itemId);
u8 TxRegItemsMenu_GetRegisteredItemIndex(u16 itemId);

#endif
