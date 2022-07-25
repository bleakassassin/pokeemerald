//Credits: TheXaman
#include "global.h"
#include "constants/songs.h"
#include "bg.h"
#include "decoration.h"
#include "event_scripts.h"
#include "event_object_lock.h"
#include "event_object_movement.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "international_string_util.h"
#include "item.h"
#include "item_icon.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "constants/items.h"
#include "list_menu.h"
#include "mail.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "player_pc.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "window.h"
#include "menu_specialized.h"
#include "registered_items_menu.h"

struct TxRegItemsMenu_Struct
{
    struct ListMenuItem listItems[REGISTERED_ITEMS_MAX + 1];
    u8 itemNames[REGISTERED_ITEMS_MAX + 1][ITEM_NAME_LENGTH + 10];
    u8 windowIds[1];
    u8 toSwapPos;
    u8 spriteId[2];
    u8 swapLineSpriteIds[5];
    u8 iconSlot;
};

#define TAG_SWAP_LINE     109
#define TAG_ITEM_ICON    5110
#define TAG_SCROLL_ARROW 5112

#define NOT_SWAPPING 0xFF

static void TxRegItemsMenu_InitMenuFunctions(u8 taskId);
static void TxRegItemsMenu_InitDataAndCreateListMenu(u8 taskId);
static void TxRegItemsMenu_ProcessInput(u8 taskId);
static void TxRegItemsMenu_DoItemAction(u8 taskId);
static void TxRegItemsMenu_CloseMenu(u8 taskId);
static void TxRegItemsMenu_ItemSwapChoosePrompt(u8 taskId);
static void TxRegItemsMenu_HandleSwapInput(u8 taskId);
//helper
static u8 TxRegItemsMenu_InitWindow(void);
static void TxRegItemsMenu_RefreshListMenu(void);
static void TxRegItemsMenu_MoveCursor(s32 id, bool8 onInit, struct ListMenu *thisMenu);
static void TxRegItemsMenu_PrintFunc(u8 windowId, u32 id, u8 yOffset);
static void TxRegItemsMenu_PrintItemIcon(u16 itemId, u8 iconSlot);
static void TxRegItemsMenu_DoItemSwap(u8 taskId, bool8 a);
static void TxRegItemsMenu_PrintSwappingCursor(u8 y, u8 b, u8 speed);
static void TxRegItemsMenu_MoveItemSlotInList(u16 *registeredItemSlots_, u32 from, u32 to_);
static void TxRegItemsMenu_CalcAndSetUsedSlotsCount(u16 *slots, u8 count, u8 *arg2, u8 *usedSlotsCount, u8 maxUsedSlotsCount);
//helper cleanup
static void TxRegItemsMenu_RemoveItemIcon(u8 iconSlot);
static void TxRegItemsMenu_RemoveScrollIndicator(void);


static const struct WindowTemplate TxRegItemsMenu_WindowTemplates[1] =
{
    {//item list window
        .bg = 0,
        .tilemapLeft = 8, //0
        .tilemapTop = 13,
        .width = 14, //30
        .height = 6, //7
        .paletteNum = 15,
        .baseBlock = 0x0001
    },
};

static const struct ListMenuTemplate gTxRegItemsMenu_List = //item storage list
{
    .items = NULL,
    .moveCursorFunc = TxRegItemsMenu_MoveCursor,
    .itemPrintFunc = TxRegItemsMenu_PrintFunc,
    .totalItems = 0,
    .maxShowed = 0,
    .windowId = 0,
    .header_X = 0,
    .item_X = 48,
    .cursor_X = 40,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = FALSE,
    .itemVerticalPadding = 0,
    .scrollMultiple = FALSE,
    .fontId = FONT_NARROW
};


// EWRAM
static EWRAM_DATA struct TxRegItemsMenu_Struct *gTxRegItemsMenu = NULL;
static EWRAM_DATA struct TxRegItemsMenu_ItemPageStruct TxRegItemsMenuItemPageInfo = {0, 0, 0, 0, {0, 0, 0}, 0};


// functions
void TxRegItemsMenu_OpenMenu(void)
{
    u8 taskId = CreateTask(TaskDummy, 0);
    FreezeObjects_WaitForPlayer();
    gTasks[taskId].func = TxRegItemsMenu_InitMenuFunctions;
}

static void TxRegItemsMenu_InitMenuFunctions(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 offset = 0;
    u8 cursorStart = gSaveBlock1Ptr->registeredItemLastSelected;
    u8 count = TxRegItemsMenu_CountUsedRegisteredItemSlots();

    tUsedSlots = TxRegItemsMenu_CountUsedRegisteredItemSlots();
    //calculate offset from list top
    if (cursorStart > 1 && count > 3)
    {
        if (cursorStart == count - 1)
        {
            offset = cursorStart - 2;
            cursorStart = 2;
        }
        else
        {
            offset = cursorStart - 1;
            cursorStart = 1;
        }
    }

    TxRegItemsMenuItemPageInfo.cursorPos = cursorStart;
    TxRegItemsMenuItemPageInfo.itemsAbove = offset;
    TxRegItemsMenuItemPageInfo.scrollIndicatorTaskId = TASK_NONE;
    TxRegItemsMenuItemPageInfo.pageItems = 3; //ItemStorage_SetItemAndMailCount(taskId);
    gTxRegItemsMenu = AllocZeroed(sizeof(struct TxRegItemsMenu_Struct));
    memset(gTxRegItemsMenu->windowIds, WINDOW_NONE, 0x1);
    gTxRegItemsMenu->toSwapPos = NOT_SWAPPING;
    gTxRegItemsMenu->spriteId[0] = SPRITE_NONE;
    gTxRegItemsMenu->spriteId[1] = SPRITE_NONE;
    LoadListMenuSwapLineGfx();
    CreateSwapLineSprites(gTxRegItemsMenu->swapLineSpriteIds, 5);
    gTasks[taskId].func = TxRegItemsMenu_InitDataAndCreateListMenu;
}

static void TxRegItemsMenu_InitDataAndCreateListMenu(u8 taskId)
{
    s16 *data;
    u32 i, x;
    const u8* text;

    data = gTasks[taskId].data;
    TxRegItemsMenu_CompactRegisteredItems();
    TxRegItemsMenu_CalcAndSetUsedSlotsCount(gSaveBlock1Ptr->registeredItems, REGISTERED_ITEMS_MAX, &(TxRegItemsMenuItemPageInfo.pageItems), &(TxRegItemsMenuItemPageInfo.count), 3);
    SetCursorWithinListBounds(&(TxRegItemsMenuItemPageInfo.itemsAbove), &(TxRegItemsMenuItemPageInfo.cursorPos), TxRegItemsMenuItemPageInfo.pageItems, TxRegItemsMenuItemPageInfo.count); //fine
    TxRegItemsMenu_RefreshListMenu();
    data[5] = ListMenuInit(&gMultiuseListMenuTemplate, TxRegItemsMenuItemPageInfo.itemsAbove, TxRegItemsMenuItemPageInfo.cursorPos);
    if (TxRegItemsMenuItemPageInfo.scrollIndicatorTaskId == TASK_NONE)
    TxRegItemsMenuItemPageInfo.scrollIndicatorTaskId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 84, 110, 148, TxRegItemsMenuItemPageInfo.count - TxRegItemsMenuItemPageInfo.pageItems, TAG_SCROLL_ARROW, TAG_SWAP_LINE, &(TxRegItemsMenuItemPageInfo.itemsAbove)); //176, 12, 148 x, y1, y2
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].func = TxRegItemsMenu_ProcessInput;
}

static void TxRegItemsMenu_ProcessInput(u8 taskId)
{
    s16 *data;
    s32 id;

    data = gTasks[taskId].data;
    if (JOY_NEW(SELECT_BUTTON))
    {
        ListMenuGetScrollAndRow(data[5], &(TxRegItemsMenuItemPageInfo.itemsAbove), &(TxRegItemsMenuItemPageInfo.cursorPos)); //fine
        if ((TxRegItemsMenuItemPageInfo.itemsAbove + TxRegItemsMenuItemPageInfo.cursorPos) != (TxRegItemsMenuItemPageInfo.count - 1))
        {
            PlaySE(SE_SELECT);
            TxRegItemsMenu_ItemSwapChoosePrompt(taskId);
        }
    }
    else
    {
        id = ListMenu_ProcessInput(data[5]); //fine
        ListMenuGetScrollAndRow(data[5], &(TxRegItemsMenuItemPageInfo.itemsAbove), &(TxRegItemsMenuItemPageInfo.cursorPos)); //fine
        switch(id)
        {
        case LIST_NOTHING_CHOSEN:
            break;
        case LIST_CANCEL:
            PlaySE(SE_SELECT);
            EnableBothScriptContexts();
            TxRegItemsMenu_CloseMenu(taskId);
            break;
        default:
            PlaySE(SE_SELECT);
            TxRegItemsMenu_DoItemAction(taskId);
            break;
        }
    }
}

static void TxRegItemsMenu_DoItemAction(u8 taskId)
{
    s16 *data;
    u16 pos;

    data = gTasks[taskId].data;
    pos = (TxRegItemsMenuItemPageInfo.cursorPos + TxRegItemsMenuItemPageInfo.itemsAbove);
    TxRegItemsMenu_RemoveScrollIndicator();

    gSaveBlock1Ptr->registeredItemLastSelected = pos;
    TxRegItemsMenu_CloseMenu(taskId);
    UseRegisteredKeyItemOnField(pos+2);
}

static void TxRegItemsMenu_CloseMenu(u8 taskId)
{
    s16 *data;
    u8 *windowIdLoc = &(gTxRegItemsMenu->windowIds[0]);

    data = gTasks[taskId].data;
    TxRegItemsMenu_RemoveItemIcon(gTxRegItemsMenu->iconSlot ^ 1);
    TxRegItemsMenu_RemoveScrollIndicator();
    DestroyListMenuTask(data[5], NULL, NULL);
    DestroySwapLineSprites(gTxRegItemsMenu->swapLineSpriteIds, 5);
    if (*windowIdLoc != WINDOW_NONE)
    {
        ClearStdWindowAndFrameToTransparent(*windowIdLoc, FALSE);
        ClearWindowTilemap(*windowIdLoc);
        ScheduleBgCopyTilemapToVram(0);
        RemoveWindow(*windowIdLoc);
        *windowIdLoc = WINDOW_NONE;
    }
    Free(gTxRegItemsMenu);
    ScriptUnfreezeObjectEvents();
    DestroyTask(taskId);
}

static void TxRegItemsMenu_ItemSwapChoosePrompt(u8 taskId)
{
    s16 *data;

    data = gTasks[taskId].data;
    ListMenuSetUnkIndicatorsStructField(data[5], 16, 1);
    gTxRegItemsMenu->toSwapPos = (TxRegItemsMenuItemPageInfo.itemsAbove + TxRegItemsMenuItemPageInfo.cursorPos);
    TxRegItemsMenu_PrintSwappingCursor(ListMenuGetYCoordForPrintingArrowCursor(data[5]), 0, 0);
    UpdateSwapLineSpritesPos(gTxRegItemsMenu->swapLineSpriteIds, 5, 104, ((gTxRegItemsMenu->toSwapPos + 1) * 16 + 90));
    gTasks[taskId].func = TxRegItemsMenu_HandleSwapInput;
}

static void TxRegItemsMenu_HandleSwapInput(u8 taskId)
{
    s16 *data;
    s32 id;

    data = gTasks[taskId].data;
    if (JOY_NEW(SELECT_BUTTON))
    {
        ListMenuGetScrollAndRow(data[5], &(TxRegItemsMenuItemPageInfo.itemsAbove), &(TxRegItemsMenuItemPageInfo.cursorPos));
        TxRegItemsMenu_DoItemSwap(taskId, FALSE);
        return;
    }
    id = ListMenu_ProcessInput(data[5]);
    ListMenuGetScrollAndRow(data[5], &(TxRegItemsMenuItemPageInfo.itemsAbove), &(TxRegItemsMenuItemPageInfo.cursorPos));
    SetSwapLineSpritesInvisibility(gTxRegItemsMenu->swapLineSpriteIds, 5, FALSE); //fine
    UpdateSwapLineSpritesPos(gTxRegItemsMenu->swapLineSpriteIds, 5, 104, ((TxRegItemsMenuItemPageInfo.cursorPos + 1) * 16 + 90));
    switch(id)
    {
    case LIST_NOTHING_CHOSEN:
        break;
    case LIST_CANCEL:
        if (JOY_NEW(A_BUTTON))
            TxRegItemsMenu_DoItemSwap(taskId, FALSE);
        else
            TxRegItemsMenu_DoItemSwap(taskId, TRUE);
        break;
    default:
        TxRegItemsMenu_DoItemSwap(taskId, FALSE);
        break;
    }
}

//helper functions
static u8 TxRegItemsMenu_InitWindow(void)
{
    u8 *windowIdLoc = &(gTxRegItemsMenu->windowIds[0]);
    if (*windowIdLoc == WINDOW_NONE)
    {
        *windowIdLoc = AddWindow(&TxRegItemsMenu_WindowTemplates[0]);
        DrawStdFrameWithCustomTileAndPalette(*windowIdLoc, FALSE, 0x214, 0xE);
        ScheduleBgCopyTilemapToVram(0);
    }
    return *windowIdLoc;
}

static void TxRegItemsMenu_RefreshListMenu(void)
{
    u16 i;
    u8 windowId = TxRegItemsMenu_InitWindow();
    LoadMessageBoxAndBorderGfx();
    SetStandardWindowBorderStyle(windowId , 0);

    for(i = 0; i < TxRegItemsMenuItemPageInfo.count - 1; i++)
    {
        CopyItemName(gSaveBlock1Ptr->registeredItems[i], &gTxRegItemsMenu->itemNames[i][0]);
        gTxRegItemsMenu->listItems[i].name = &(gTxRegItemsMenu->itemNames[i][0]);
        gTxRegItemsMenu->listItems[i].id = i;
    }
    StringCopy(&(gTxRegItemsMenu->itemNames[i][0]) ,gText_Cancel);
    gTxRegItemsMenu->listItems[i].name = &(gTxRegItemsMenu->itemNames[i][0]);
    gTxRegItemsMenu->listItems[i].id = -2;
    gMultiuseListMenuTemplate = gTxRegItemsMenu_List;
    gMultiuseListMenuTemplate.windowId = windowId;
    gMultiuseListMenuTemplate.totalItems = TxRegItemsMenuItemPageInfo.count;
    gMultiuseListMenuTemplate.items = gTxRegItemsMenu->listItems;
    gMultiuseListMenuTemplate.maxShowed = 3;//TxRegItemsMenuItemPageInfo.pageItems;
}

static void TxRegItemsMenu_MoveCursor(s32 id, bool8 onInit, struct ListMenu *thisMenu)
{
    if (onInit != TRUE)
        PlaySE(SE_SELECT);
    if (gTxRegItemsMenu->toSwapPos == NOT_SWAPPING)
    {
        if (id != LIST_CANCEL)
            TxRegItemsMenu_PrintItemIcon(gSaveBlock1Ptr->registeredItems[id], gTxRegItemsMenu->iconSlot);
        else
            TxRegItemsMenu_PrintItemIcon(MSG_GO_BACK_TO_PREV, gTxRegItemsMenu->iconSlot);
        TxRegItemsMenu_RemoveItemIcon(gTxRegItemsMenu->iconSlot ^ 1);
        gTxRegItemsMenu->iconSlot ^= 1;
    }
}

static void TxRegItemsMenu_PrintFunc(u8 windowId, u32 id, u8 yOffset)
{
    if (id != LIST_CANCEL && gTxRegItemsMenu->toSwapPos != NOT_SWAPPING)
    {
        if (gTxRegItemsMenu->toSwapPos == (u8)id)
            TxRegItemsMenu_PrintSwappingCursor(yOffset, 0, TEXT_SKIP_DRAW);
        else
            TxRegItemsMenu_PrintSwappingCursor(yOffset, 0xFF, TEXT_SKIP_DRAW);
    }
}

static void TxRegItemsMenu_PrintItemIcon(u16 itemId, u8 iconSlot)
{
    u8 spriteId;
    u8* spriteIdLoc = &gTxRegItemsMenu->spriteId[iconSlot];

    if (*spriteIdLoc == SPRITE_NONE)
    {
        spriteId = AddItemIconSprite(iconSlot + TAG_ITEM_ICON, iconSlot + TAG_ITEM_ICON, itemId);
        if (spriteId != MAX_SPRITES)
        {
            *spriteIdLoc = spriteId;
            gSprites[spriteId].oam.priority = 0;
            gSprites[spriteId].x = 88;
            gSprites[spriteId].y = 132;
        }
    }
}

static void TxRegItemsMenu_DoItemSwap(u8 taskId, bool8 a)
{
    s16 *data;
    u16 b;
    u8 lastSelected = gSaveBlock1Ptr->registeredItemLastSelected;
    u16 lastSelectedItemId = gSaveBlock1Ptr->registeredItems[lastSelected];

    data = gTasks[taskId].data;
    b = (TxRegItemsMenuItemPageInfo.itemsAbove + TxRegItemsMenuItemPageInfo.cursorPos);
    PlaySE(SE_SELECT);
    DestroyListMenuTask(data[5], &(TxRegItemsMenuItemPageInfo.itemsAbove), &(TxRegItemsMenuItemPageInfo.cursorPos));
    if (!a && gTxRegItemsMenu->toSwapPos != b && gTxRegItemsMenu->toSwapPos != b - 1)
    {
        TxRegItemsMenu_MoveItemSlotInList(gSaveBlock1Ptr->registeredItems, gTxRegItemsMenu->toSwapPos, b);
        gSaveBlock1Ptr->registeredItemLastSelected = TxRegItemsMenu_GetRegisteredItemIndex(lastSelectedItemId);
        TxRegItemsMenu_RefreshListMenu();
    }
    if (gTxRegItemsMenu->toSwapPos < b)
        TxRegItemsMenuItemPageInfo.cursorPos--;
    SetSwapLineSpritesInvisibility(gTxRegItemsMenu->swapLineSpriteIds, 5, TRUE);
    gTxRegItemsMenu->toSwapPos = NOT_SWAPPING;
    data[5] = ListMenuInit(&gMultiuseListMenuTemplate, TxRegItemsMenuItemPageInfo.itemsAbove, TxRegItemsMenuItemPageInfo.cursorPos);
    ScheduleBgCopyTilemapToVram(0);
    gTasks[taskId].func = TxRegItemsMenu_ProcessInput;
}

static const u8 sSwapArrowTextColors[] = {TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_DARK_GRAY};
static void TxRegItemsMenu_PrintSwappingCursor(u8 y, u8 b, u8 speed)
{
    u8 x = 40;
    u8 windowId = gTxRegItemsMenu->windowIds[0];
    if (b == 0xFF)
        FillWindowPixelRect(windowId, PIXEL_FILL(1), x, y, GetMenuCursorDimensionByFont(1, 0), GetMenuCursorDimensionByFont(1, 1));
    else
        AddTextPrinterParameterized4(windowId, 1, x, y, 0, 0, sSwapArrowTextColors, speed, gText_SelectorArrow);
}

//registeredItems struct helper functions
static void TxRegItemsMenu_MoveItemSlotInList(u16 *registeredItemSlots_, u32 from, u32 to_)
{
    // dumb assignments needed to match
    u16 *registeredItemSlots = registeredItemSlots_;
    u32 to = to_;

    if (from != to)
    {
        s16 i, count;
        u16 firstSlot = registeredItemSlots[from];

        if (to > from)
        {
            to--;
            for (i = from, count = to; i < count; i++)
                registeredItemSlots[i] = registeredItemSlots[i + 1];
        }
        else
        {
            for (i = from, count = to; i > count; i--)
                registeredItemSlots[i] = registeredItemSlots[i - 1];
        }
        registeredItemSlots[to] = firstSlot;
    }
}

u8 TxRegItemsMenu_CountUsedRegisteredItemSlots(void)
{
    u8 usedSlots = 0;
    u8 i;

    for (i = 0; i < PC_ITEMS_COUNT; i++)
    {
        if (gSaveBlock1Ptr->registeredItems[i] != ITEM_NONE)
            usedSlots++;
    }
    return usedSlots;
}

static void TxRegItemsMenu_ChangeLastSelectedItemIndex(u8 index)
{
    if (gSaveBlock1Ptr->registeredItemLastSelected == index)
        gSaveBlock1Ptr->registeredItemLastSelected = 0;
    else if (index < gSaveBlock1Ptr->registeredItemLastSelected && index != 0)
        gSaveBlock1Ptr->registeredItemLastSelected--;
}

void TxRegItemsMenu_RemoveRegisteredItem(u16 itemId)
{
    u8 i;
    for (i = i ; i < REGISTERED_ITEMS_MAX; i++)
    {
        if (gSaveBlock1Ptr->registeredItems[i] == itemId)
        {
            gSaveBlock1Ptr->registeredItems[i] = ITEM_NONE;
            gSaveBlock1Ptr->registeredItemListCount--;
            TxRegItemsMenu_ChangeLastSelectedItemIndex(i);
            TxRegItemsMenu_CompactRegisteredItems();
        }
    }
}

void TxRegItemsMenu_CompactRegisteredItems(void)
{
    u16 i;
    u16 j;

    for (i = 0; i < REGISTERED_ITEMS_MAX - 1; i++)
    {
        for (j = i + 1; j < REGISTERED_ITEMS_MAX; j++)
        {
            if (gSaveBlock1Ptr->registeredItems[i] == ITEM_NONE)
            {
                u16 temp = gSaveBlock1Ptr->registeredItems[i];
                gSaveBlock1Ptr->registeredItems[i] = gSaveBlock1Ptr->registeredItems[j];
                gSaveBlock1Ptr->registeredItems[j] = temp;
            }
        }
    }
    gSaveBlock1Ptr->registeredItemListCount = TxRegItemsMenu_CountUsedRegisteredItemSlots();
}

static void TxRegItemsMenu_CalcAndSetUsedSlotsCount(u16 *slots, u8 count, u8 *arg2, u8 *usedSlotsCount, u8 maxUsedSlotsCount)
{
    u16 i;
    u16 *slots_ = slots;

    (*usedSlotsCount) = 0;
    for (i = 0; i < count; i++)
    {
        if (slots_[i] != ITEM_NONE)
            (*usedSlotsCount)++;
    }

    (*usedSlotsCount)++;
    if ((*usedSlotsCount) > maxUsedSlotsCount)
        *arg2 = maxUsedSlotsCount;
    else
        *arg2 = (*usedSlotsCount);
}

bool8 TxRegItemsMenu_CheckRegisteredHasItem(u16 itemId)
{
    u8 i;

    for (i = 0; i < REGISTERED_ITEMS_MAX; i++)
    {
        if (gSaveBlock1Ptr->registeredItems[i] == itemId)
            return TRUE;
    }
    return FALSE;
}

u8 TxRegItemsMenu_GetRegisteredItemIndex(u16 itemId)
{
    u8 i;

    for (i = 0; i < REGISTERED_ITEMS_MAX; i++)
    {
        if (gSaveBlock1Ptr->registeredItems[i] == itemId)
            return i;
    }
    return 0xFF;
}

bool8 TxRegItemsMenu_AddRegisteredItem(u16 itemId)
{
    u8 i;
    s8 freeSlot;
    u16 *newItems;

    // Copy PC items
    newItems = AllocZeroed(sizeof(gSaveBlock1Ptr->registeredItems));
    memcpy(newItems, gSaveBlock1Ptr->registeredItems, sizeof(gSaveBlock1Ptr->registeredItems));

    //check for a free slot
    
    for (i = 0; i < REGISTERED_ITEMS_MAX; i++)
    {
        if (gSaveBlock1Ptr->registeredItems[i] == ITEM_NONE)
        {
            newItems[i] = itemId;
            gSaveBlock1Ptr->registeredItemListCount++;
            // Copy items back to the PC
            memcpy(gSaveBlock1Ptr->registeredItems, newItems, sizeof(gSaveBlock1Ptr->registeredItems));
            Free(newItems);
            return;
        }
    }
    Free(newItems);
}

void TxRegItemsMenu_RegisteredItemsMenuNewGame(void)
{
    u8 i;
    for (i = i ; i < REGISTERED_ITEMS_MAX; i++)
    {
        gSaveBlock1Ptr->registeredItems[i] = ITEM_NONE;
    }
    gSaveBlock1Ptr->registeredItemLastSelected = 0;
    gSaveBlock1Ptr->registeredItemListCount = 0;
}


//helper cleanup
static void TxRegItemsMenu_RemoveItemIcon(u8 iconSlot) //remove item storage selected item icon
{
    u8* spriteIdLoc = &gTxRegItemsMenu->spriteId[iconSlot];
    if (*spriteIdLoc != SPRITE_NONE)
    {
        FreeSpriteTilesByTag(iconSlot + TAG_ITEM_ICON);
        FreeSpritePaletteByTag(iconSlot + TAG_ITEM_ICON);
        DestroySprite(&(gSprites[*spriteIdLoc]));
        *spriteIdLoc = SPRITE_NONE;
    }
}
static void TxRegItemsMenu_RemoveScrollIndicator(void)
{
    if (TxRegItemsMenuItemPageInfo.scrollIndicatorTaskId != TASK_NONE)
    {
        RemoveScrollIndicatorArrowPair(TxRegItemsMenuItemPageInfo.scrollIndicatorTaskId);
        TxRegItemsMenuItemPageInfo.scrollIndicatorTaskId = TASK_NONE;
    }
}
