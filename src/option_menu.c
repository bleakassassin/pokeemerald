#include "global.h"
#include "option_menu.h"
#include "main.h"
#include "menu.h"
#include "scanline_effect.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "malloc.h"
#include "bg.h"
#include "gpu_regs.h"
#include "window.h"
#include "text.h"
#include "text_window.h"
#include "international_string_util.h"
#include "strings.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"

// Menu items
enum
{
    MENUITEM_TEXTSPEED,
    MENUITEM_BATTLESCENE,
    MENUITEM_BATTLESTYLE,
    MENUITEM_ATTACKSTYLE,
    MENUITEM_MATCHCALL,
    MENUITEM_SOUND,
    MENUITEM_BUTTONMODE,
    MENUITEM_UNITSYSTEM,
    MENUITEM_FRAMETYPE,
    MENUITEM_FONT,
    MENUITEM_CANCEL,
    MENUITEM_COUNT,
};

// Window Ids
enum
{
    WIN_TEXT_OPTION,
    WIN_OPTIONS,
    WIN_DESCRIPTIONS
};

#define Y_DIFF 16 // Difference in pixels between items.
#define OPTIONS_ON_SCREEN 6
#define NUM_OPTIONS_FROM_BORDER 1

struct OptionMenu
{
    u8 sel[MENUITEM_COUNT];
    int menuCursor;
    int visibleCursor;
};

// this file's functions
static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(int cursor);
static int ProcessInput_Sound(int selection);
static int ProcessInput_FrameType(int selection);
static int ProcessInput_FontType(int selection);
static int ProcessInput_Options_Two(int selection);
static int ProcessInput_Options_Three(int selection);
static void DrawChoices_TextSpeed(int selection, int y);
static void DrawChoices_BattleScene(int selection, int y);
static void DrawChoices_BattleStyle(int selection, int y);
static void DrawChoices_AttackStyle(int selection, int y);
static void DrawChoices_Sound(int selection, int y);
static void DrawChoices_ButtonMode(int selection, int y);
static void DrawChoices_UnitSystem(int selection, int y);
static void DrawChoices_FrameType(int selection, int y);
static void DrawChoices_Font(int selection, int y);
static void DrawTextOption(void);
static void DrawOptionMenuTexts(void);
static void DrawDescriptionText(int cursor);
static void DrawBgWindowFrames(void);

struct
{
    void (*drawChoices)(int selection, int y);
    int (*processInput)(int selection);
}

static const sItemFunctions[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]    = {DrawChoices_TextSpeed,   ProcessInput_Options_Three},
    [MENUITEM_BATTLESCENE]  = {DrawChoices_BattleScene, ProcessInput_Options_Two},
    [MENUITEM_BATTLESTYLE]  = {DrawChoices_BattleStyle, ProcessInput_Options_Three},
    [MENUITEM_ATTACKSTYLE]  = {DrawChoices_AttackStyle, ProcessInput_Options_Two},
    [MENUITEM_SOUND]        = {DrawChoices_Sound,       ProcessInput_Sound},
    [MENUITEM_BUTTONMODE]   = {DrawChoices_ButtonMode,  ProcessInput_Options_Two},
    [MENUITEM_UNITSYSTEM]   = {DrawChoices_UnitSystem,  ProcessInput_Options_Two},
    [MENUITEM_FRAMETYPE]    = {DrawChoices_FrameType,   ProcessInput_FrameType},
    [MENUITEM_FONT]         = {DrawChoices_FrameType,   ProcessInput_FontType}, 
    [MENUITEM_MATCHCALL]    = {DrawChoices_BattleScene, ProcessInput_Options_Two},
    [MENUITEM_CANCEL]       = {NULL, NULL},
};

// EWRAM vars
EWRAM_DATA static struct OptionMenu *sOptions = NULL;

static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text.gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/interface/option_menu_equals_sign.4bpp");

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = gText_TextSpeed,
    [MENUITEM_BATTLESCENE] = gText_BattleScene,
    [MENUITEM_BATTLESTYLE] = gText_BattleStyle,
    [MENUITEM_ATTACKSTYLE] = gText_AttackStyle,
    [MENUITEM_MATCHCALL]   = gText_MatchCalls,
    [MENUITEM_SOUND]       = gText_Sound,
    [MENUITEM_BUTTONMODE]  = gText_ButtonMode,
    [MENUITEM_UNITSYSTEM]  = gText_UnitSystem,
    [MENUITEM_FRAMETYPE]   = gText_Frame,
    [MENUITEM_FONT]        = gText_Font,
    [MENUITEM_CANCEL]      = gText_Cancel,
};

static const u8 *const sTextSpeedStrings[] = {gText_TextSpeedSlow, gText_TextSpeedMid, gText_TextSpeedFast};

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = 28,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 3,
        .width = 28,
        .height = 12,
        .paletteNum = 1,
        .baseBlock = 0x3A
    },
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 17,
        .width = 28,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 427
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
   {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
       .baseTile = 0
   },
   {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
       .baseTile = 0
   }
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};// Descriptions
static const u8 sText_TextSpeed[]   = _("Choose text-scrolling speed.");
static const u8 sText_BattleScene[] = _("Toggle in-battle animations.");
static const u8 sText_Difficulty[]  = _("Set battle difficulty and Exp. gain.");
static const u8 sText_AttackStyle[] = _("Set what determines attack power.");
static const u8 sText_MatchCall[]   = _("Toggle calls from other Trainers.");
static const u8 sText_Sound[]       = _("Set audio output.");
static const u8 sText_ButtonMode[]  = _("Set function of L/R Buttons.");
static const u8 sText_UnitSystem[]  = _("Toggle between measuring systems.");
static const u8 sText_FrameType[]   = _("Choose window frame style.");
static const u8 sText_Font[]        = _("POKéMON EMERALD");
static const u8 sText_Cancel[]      = _("Exit the Options menu.");
static const u8 *const sOptionMenuItemDescriptions[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]   = sText_TextSpeed,
    [MENUITEM_BATTLESCENE] = sText_BattleScene,
    [MENUITEM_BATTLESTYLE] = sText_Difficulty,
    [MENUITEM_ATTACKSTYLE] = sText_AttackStyle,
    [MENUITEM_MATCHCALL]   = sText_MatchCall,
    [MENUITEM_SOUND]       = sText_Sound,
    [MENUITEM_BUTTONMODE]  = sText_ButtonMode,
    [MENUITEM_UNITSYSTEM]  = sText_UnitSystem,
    [MENUITEM_FRAMETYPE]   = sText_FrameType,
    [MENUITEM_FONT]        = sText_Font,
    [MENUITEM_CANCEL]      = sText_Cancel,
};

// code
static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void DrawChoices(u32 id, int y)
{
    if (sItemFunctions[id].drawChoices != NULL)
        sItemFunctions[id].drawChoices(sOptions->sel[id], y);
}

void CB2_InitOptionMenu(void)
{
    u32 i, taskId;
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void*)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ResetBgPositions();
        InitWindows(sOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN1_BG0 | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_BG0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, 0, sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, 0x70, 0x20);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, 16, sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        sOptions = AllocZeroed(sizeof(*sOptions));
        sOptions->sel[MENUITEM_TEXTSPEED]   = gSaveBlock2Ptr->optionsTextSpeed;
        sOptions->sel[MENUITEM_BATTLESCENE] = gSaveBlock2Ptr->optionsBattleSceneOff;
        sOptions->sel[MENUITEM_BATTLESTYLE] = gSaveBlock2Ptr->optionsDifficulty;
        sOptions->sel[MENUITEM_ATTACKSTYLE] = gSaveBlock2Ptr->optionsAttackStyle;
        sOptions->sel[MENUITEM_MATCHCALL]   = gSaveBlock2Ptr->optionsDisableMatchCall;
        sOptions->sel[MENUITEM_SOUND]       = gSaveBlock2Ptr->optionsSound;
        sOptions->sel[MENUITEM_BUTTONMODE]  = gSaveBlock2Ptr->optionsButtonMode;
        sOptions->sel[MENUITEM_UNITSYSTEM]  = gSaveBlock2Ptr->optionsUnitSystem;
        sOptions->sel[MENUITEM_FRAMETYPE]   = gSaveBlock2Ptr->optionsWindowFrameType;
        sOptions->sel[MENUITEM_FONT]        = gSaveBlock2Ptr->optionsCurrentFont;
        gMain.state++;
        break;
    case 7:
        PutWindowTilemap(WIN_TEXT_OPTION);
        DrawTextOption();
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_DESCRIPTIONS);
        DrawDescriptionText(0);
        gMain.state++;
        break;
    case 9:
        PutWindowTilemap(WIN_OPTIONS);
        DrawOptionMenuTexts();
        gMain.state++;
    case 10:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 11:
        taskId = CreateTask(Task_OptionMenuFadeIn, 0);
        
        AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 240 / 2, 20, 124,
          MENUITEM_COUNT - 1, 110, 110, 0);

        for (i = 0; i < OPTIONS_ON_SCREEN; i++)
            DrawChoices(i, i * Y_DIFF);

        HighlightOptionMenuItem(sOptions->menuCursor);

        CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
        gMain.state++;
        break;
    case 12:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void ScrollMenu(int direction)
{
    int menuItem, pos;
    if (direction == 0) // scroll down
        menuItem = sOptions->menuCursor + NUM_OPTIONS_FROM_BORDER, pos = OPTIONS_ON_SCREEN - 1;
    else
        menuItem = sOptions->menuCursor - NUM_OPTIONS_FROM_BORDER, pos = 0;

    // Hide one
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF, PIXEL_FILL(0));
    // Show one
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, Y_DIFF * pos, 28 * 8, Y_DIFF);
    // Print
    DrawChoices(menuItem, pos * Y_DIFF);
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenuItemsNames[menuItem], 8, (pos * Y_DIFF), TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}
static void ScrollAll(int direction) // to bottom or top
{
    int i, y, menuItem, pos;
    int scrollCount = MENUITEM_COUNT - OPTIONS_ON_SCREEN;
    // Move items up/down
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF * scrollCount, PIXEL_FILL(1));

    // Clear moved items
    if (direction == 0)
    {
        y = OPTIONS_ON_SCREEN - scrollCount;
        if (y < 0)
            y = OPTIONS_ON_SCREEN;
        y *= Y_DIFF;
    }
    else
    {
        y = 0;
    }

    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(1), 0, y, 28 * 8, Y_DIFF * scrollCount);
    // Print new texts
    for (i = 0; i < scrollCount; i++)
    {
        if (direction == 0) // From top to bottom
            menuItem = MENUITEM_COUNT - 1 - i, pos = OPTIONS_ON_SCREEN - 1 - i;
        else // From bottom to top
            menuItem = i, pos = i;
        DrawChoices(menuItem, pos * Y_DIFF);
        AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenuItemsNames[menuItem], 8, (pos * Y_DIFF), TEXT_SKIP_DRAW, NULL);
    }
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput;
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    int i, scrollCount = 0, itemsToRedraw;
    if (JOY_NEW(A_BUTTON))
    {
        if (sOptions->menuCursor == MENUITEM_CANCEL)
            gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sOptions->visibleCursor == NUM_OPTIONS_FROM_BORDER) // don't advance visible cursor until scrolled to the bottom
        {
            if (--sOptions->menuCursor == 0)
                sOptions->visibleCursor--;
            else
                ScrollMenu(1);
        }
        else
        {
            if (--sOptions->menuCursor < 0) // Scroll all the way to the bottom.
            {
                sOptions->visibleCursor = sOptions->menuCursor = 3;
                ScrollAll(0);
                sOptions->visibleCursor = 5;
                sOptions->menuCursor = MENUITEM_COUNT - 1;
            }
            else
            {
                sOptions->visibleCursor--;
            }
        }
        HighlightOptionMenuItem(sOptions->visibleCursor);
        DrawDescriptionText(sOptions->menuCursor);
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sOptions->visibleCursor == 4) // don't advance visible cursor until scrolled to the bottom
        {
            if (++sOptions->menuCursor == MENUITEM_COUNT - 1)
                sOptions->visibleCursor++;
            else
                ScrollMenu(0);
        }
        else
        {
            if (++sOptions->menuCursor >= MENUITEM_COUNT) // Scroll all the way to the top.
            {
                sOptions->visibleCursor = 3;
                sOptions->menuCursor = MENUITEM_COUNT - 4;
                ScrollAll(1);
                sOptions->visibleCursor = sOptions->menuCursor = 0;
            }
            else
            {
                sOptions->visibleCursor++;
            }
        }
        HighlightOptionMenuItem(sOptions->visibleCursor);
        DrawDescriptionText(sOptions->menuCursor);
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        int cursor = sOptions->menuCursor;
        u8 previousOption = sOptions->sel[cursor];
        if (sItemFunctions[cursor].processInput != NULL)
            sOptions->sel[cursor] = sItemFunctions[cursor].processInput(previousOption);

        if (previousOption != sOptions->sel[cursor])
            DrawChoices(cursor, sOptions->visibleCursor * Y_DIFF);
    }
}

static void Task_OptionMenuSave(u8 taskId)
{
    gSaveBlock2Ptr->optionsTextSpeed        = sOptions->sel[MENUITEM_TEXTSPEED];
    gSaveBlock2Ptr->optionsBattleSceneOff   = sOptions->sel[MENUITEM_BATTLESCENE];
    gSaveBlock2Ptr->optionsDifficulty      = sOptions->sel[MENUITEM_BATTLESTYLE];
    gSaveBlock2Ptr->optionsAttackStyle      = sOptions->sel[MENUITEM_ATTACKSTYLE];
    gSaveBlock2Ptr->optionsDisableMatchCall = sOptions->sel[MENUITEM_MATCHCALL];
    gSaveBlock2Ptr->optionsSound            = sOptions->sel[MENUITEM_SOUND];
    gSaveBlock2Ptr->optionsButtonMode       = sOptions->sel[MENUITEM_BUTTONMODE];
    gSaveBlock2Ptr->optionsUnitSystem       = sOptions->sel[MENUITEM_UNITSYSTEM];
    gSaveBlock2Ptr->optionsWindowFrameType  = sOptions->sel[MENUITEM_FRAMETYPE];
    gSaveBlock2Ptr->optionsCurrentFont      = sOptions->sel[MENUITEM_FONT];

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        FREE_AND_SET_NULL(sOptions);
        SetMainCallback2(gMain.savedCallback);
    }
}

static void HighlightOptionMenuItem(int cursor)
{
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(8, 232));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(cursor * Y_DIFF + 24, cursor * Y_DIFF + 40));
}

static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3)
{
    int xMid;
    int widthLeft = GetStringWidth(1, txt1, 0);
    int widthMid = GetStringWidth(1, txt2, 0);
    int widthRight = GetStringWidth(1, txt3, 0);

    widthMid -= (198 - 104);
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    return xMid;
}

// Process Input functions
static int ProcessInput_Options_Two(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;

    return selection;
}

static int ProcessInput_Options_Three(int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (++selection > 2)
            selection = 0;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (--selection < 0)
            selection = 2;
    }
    return selection;
}

static int ProcessInput_Sound(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        SetPokemonCryStereo(selection);
    }

    return selection;
}

static int ProcessInput_FrameType(int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < WINDOW_FRAMES_COUNT - 1)
            selection++;
        else
            selection = 0;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, 0x70, 0x20);
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = WINDOW_FRAMES_COUNT - 1;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, 0x70, 0x20);
    }
    return selection;
}

static int ProcessInput_FontType(int selection)
{
    u8 color_gray[3];

    color_gray[0] = TEXT_COLOR_TRANSPARENT;
    color_gray[1] = 6;
    color_gray[2] = 7;

    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < FONT_TYPES_COUNT - 1)
            selection++;
        else
            selection = 0;
        FillWindowPixelBuffer(WIN_DESCRIPTIONS, PIXEL_FILL(1));
        AddTextPrinterParameterized4(WIN_DESCRIPTIONS, selection, 8, 0, 0, 0, color_gray, TEXT_SKIP_DRAW, sText_Font);
        CopyWindowToVram(WIN_DESCRIPTIONS, COPYWIN_FULL);
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = FONT_TYPES_COUNT - 1;
        FillWindowPixelBuffer(WIN_DESCRIPTIONS, PIXEL_FILL(1));
        AddTextPrinterParameterized4(WIN_DESCRIPTIONS, selection, 8, 0, 0, 0, color_gray, TEXT_SKIP_DRAW, sText_Font);
        CopyWindowToVram(WIN_DESCRIPTIONS, COPYWIN_FULL);
    }
    return selection;
}

// Draw Choices functions
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[16];
    u16 i;

    for (i = 0; *text != EOS && i <= 14; i++)
        dst[i] = *(text++);

    if (style != 0)
    {
        dst[2] = 4;
        dst[5] = 5;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, dst, x, y, 0x00, NULL);
}

static void DrawChoices_TextSpeed(int selection, int y)
{
    u8 styles[3] = {0};
    s32 widthSlow, widthMid, widthFast, xMid;

    styles[selection] = 1;

    DrawOptionMenuChoice(gText_TextSpeedSlow, 104, y, styles[0]);

    widthSlow = GetStringWidth(FONT_NORMAL, gText_TextSpeedSlow, 0);
    widthMid = GetStringWidth(FONT_NORMAL, gText_TextSpeedMid, 0);
    widthFast = GetStringWidth(FONT_NORMAL, gText_TextSpeedFast, 0);

    widthMid -= 94;
    xMid = (widthSlow - widthMid - widthFast) / 2 + 112;
    DrawOptionMenuChoice(gText_TextSpeedMid, xMid, y, styles[1]);

    DrawOptionMenuChoice(gText_TextSpeedFast, GetStringRightAlignXOffset(FONT_NORMAL, gText_TextSpeedFast, 214), y, styles[2]);
}

static void DrawChoices_BattleScene(int selection, int y)
{
    u8 styles[2] = {0};

    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOff, 214), y, styles[1]);
}

static void DrawChoices_BattleStyle(int selection, int y)
{
    u8 styles[3] = {0};
    s32 widthEasy, widthNormal, widthHard, xMid;

    styles[selection] = 1;

    DrawOptionMenuChoice(gText_DifficultyEasy, 104, y, styles[0]);

    widthEasy = GetStringWidth(FONT_NORMAL, gText_DifficultyEasy, 0);
    widthNormal = GetStringWidth(FONT_NORMAL, gText_DifficultyNormal, 0);
    widthHard = GetStringWidth(FONT_NORMAL, gText_DifficultyHard, 0);

    widthNormal -= 94;
    xMid = (widthEasy - widthNormal - widthHard) / 2 + 112;

    DrawOptionMenuChoice(gText_DifficultyNormal, xMid, y, styles[1]);
    DrawOptionMenuChoice(gText_DifficultyHard, GetStringRightAlignXOffset(FONT_NORMAL, gText_DifficultyHard, 214), y, styles[2]);
}

static void DrawChoices_AttackStyle(int selection, int y)
{
    u8 styles[2] = {0};

    styles[selection] = 1;

    DrawOptionMenuChoice(gText_AttackStyleType, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_AttackStyleCategory, GetStringRightAlignXOffset(FONT_NORMAL, gText_AttackStyleCategory, 214), y, styles[1]);
}

static void DrawChoices_Sound(int selection, int y)
{
    u8 styles[2] = {0};

    styles[selection] = 1;

    DrawOptionMenuChoice(gText_SoundMono, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_SoundStereo, GetStringRightAlignXOffset(FONT_NORMAL, gText_SoundStereo, 214), y, styles[1]);
}

static void DrawChoices_ButtonMode(int selection, int y)
{
    u8 styles[2] = {0};

    styles[selection] = 1;

    DrawOptionMenuChoice(gText_ButtonTypeDefault, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(FONT_NORMAL, gText_ButtonTypeLEqualsA, 214), y, styles[1]);
}

static void DrawChoices_UnitSystem(int selection, int y)
{
    u8 styles[2] = {0};

    styles[selection] = 1;

    DrawOptionMenuChoice(gText_UnitSystemImperial, 104, y, styles[0]);
    DrawOptionMenuChoice(gText_UnitSystemMetric, GetStringRightAlignXOffset(FONT_NORMAL, gText_UnitSystemMetric, 214), y, styles[1]);
}

static void DrawChoices_FrameType(int selection, int y)
{
    u8 text[16];
    u8 n = selection + 1;
    u16 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = 0x77;
        i++;
    }

    text[i] = EOS;

    DrawOptionMenuChoice(gText_FrameType, 104, y, 0);
    DrawOptionMenuChoice(text, 131, y, 1);
}

static void DrawTextOption(void)
{   
    u8 color_white[3];

    color_white[0] = TEXT_COLOR_TRANSPARENT;
    color_white[1] = 1;
    color_white[2] = 6;

    AddTextPrinterParameterized4(WIN_TEXT_OPTION, FONT_NORMAL, 8, 1, 0, 0, color_white, TEXT_SKIP_DRAW, gText_Option);
    CopyWindowToVram(WIN_TEXT_OPTION, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(void)
{
    u8 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < MENUITEM_COUNT; i++)
        AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, sOptionMenuItemsNames[i], 8, (i * Y_DIFF), TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawDescriptionText(int cursor)
{
    u8 color_gray[3];

    color_gray[0] = TEXT_COLOR_TRANSPARENT;
    color_gray[1] = 6;
    color_gray[2] = 7;

    FillWindowPixelBuffer(WIN_DESCRIPTIONS, PIXEL_FILL(1));
    AddTextPrinterParameterized4(WIN_DESCRIPTIONS, sOptions->sel[MENUITEM_FONT], 8, 0, 0, 0, color_gray, TEXT_SKIP_DRAW, sOptionMenuItemDescriptions[cursor]);
    CopyWindowToVram(WIN_DESCRIPTIONS, COPYWIN_FULL);
}

// Background tilemap
#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  0,  2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      1,  2, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 29,  2,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     0,  3,  1, 16,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   29,  3,  1, 16,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  0, 15,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      1, 15, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 29, 15,  1,  1,  7);

    // Draw description window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  0, 16,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      1, 16, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 29, 16,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     0, 17,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   29, 17,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  0, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      1, 19, 28,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 29, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
