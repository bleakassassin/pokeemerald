#ifndef GUARD_PLAYER_PC_H
#define GUARD_PLAYER_PC_H

#include "menu.h"

#define tUsedSlots  data[1]

struct PlayerPCItemPageStruct
{
    u16 cursorPos;
    u16 itemsAbove;
    u8 pageItems;
    u8 count;
    u8 filler[3];
    u8 scrollIndicatorTaskId;
};

extern struct PlayerPCItemPageStruct gPlayerPCItemPageInfo;

extern const struct MenuAction gMailboxMailOptions[];

void ReshowPlayerPC(u8 taskId);
void CB2_PlayerPCExitBagMenu(void);
void Mailbox_ReturnToMailListAfterDeposit(void);

// Message IDs for Item Storage
enum {
    MSG_SWITCH_WHICH_ITEM = 0xFFF7,
    MSG_OKAY_TO_THROW_AWAY,
    MSG_TOO_IMPORTANT,
    MSG_NO_MORE_ROOM,
    MSG_THREW_AWAY_ITEM,
    MSG_HOW_MANY_TO_TOSS,
    MSG_WITHDREW_ITEM,
    MSG_HOW_MANY_TO_WITHDRAW,
    MSG_GO_BACK_TO_PREV
};


#endif //GUARD_PLAYER_PC_H
