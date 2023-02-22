/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/****************************************************************************
				Freeze Tag
****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"

#define FTAG_MIN_VNUM           1051
#define FTAG_MAX_VNUM           1099

DECLARE_DO_FUN( do_transfer );

void do_red( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf [MAX_STRING_LENGTH];

#ifdef DEBUG
        Debug ("do_red");
#endif

    if ( argument[0] == '\0' )
    {
        send_to_char( "Syntax:  red <message>\n\r", ch );
        return;
    }

    if (!IS_IMMORTAL(ch) && !IS_SET(ch->tag_flags,TAG_PLAYING))
    {
        send_to_char( "You must be a freeze tag player to use this channel.\n\r",ch);
        return;
    }

    if (IS_SET(ch->tag_flags,TAG_BLUE))
    {
        sprintf(buf,"{R{{RED}{B %s{x: %s\n\r",ch->name,argument);
        send_to_char(buf, ch);
    }
    else
    {
        sprintf(buf,"{R{{RED}{R %s{x: %s\n\r",ch->name,argument);
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( (d->connected == CON_PLAYING)
      &&   IS_SET(d->character->tag_flags,TAG_RED))
        {
          send_to_char(buf, d->character);
        }
    }
}

void do_blue( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf [MAX_STRING_LENGTH];

#ifdef DEBUG
        Debug ("do_blue");
#endif

    if ( argument[0] == '\0' )
    {
        send_to_char( "Syntax:  blue <message>\n\r", ch );
        return;
    }

    if (!IS_IMMORTAL(ch) && !IS_SET(ch->tag_flags,TAG_PLAYING))
    {
        send_to_char( "You must be a freeze tag player to use this channel.\n\r",ch);
        return;
    }

    if (IS_SET(ch->tag_flags,TAG_RED))
    {
        sprintf(buf,"{B{{BLUE}{R %s{x: %s\n\r",ch->name,argument);
        send_to_char(buf, ch);
    }
    else
    {
        sprintf(buf,"{B{{BLUE}{B %s{x: %s\n\r",ch->name,argument);
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( (d->connected == CON_PLAYING)
      &&   IS_SET(d->character->tag_flags,TAG_BLUE))
        {
          send_to_char(buf, d->character);
        }
    }
}

void check_team_frozen ( CHAR_DATA *ch )
{
    DESCRIPTOR_DATA *d;

#ifdef DEBUG
        Debug ("check_team_frozen");
#endif

    if (IS_SET(ch->tag_flags,TAG_BLUE))
    {
        for ( d = descriptor_list; d != NULL; d = d->next )
        {
          if ( (d->connected == CON_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_BLUE)
          &&   !IS_NPC(d->character)
        &&   !IS_SET(d->character->tag_flags,TAG_FROZEN))
          {
            return;
         }
        }
info("{C[{cFTAG{C] {wThe {RRED{g team has won FREEZE TAG!!!",NULL,NULL,INFO_FTAG,0,0);
    return;
    }

    if (IS_SET(ch->tag_flags,TAG_RED))
    {
        for ( d = descriptor_list; d != NULL; d = d->next )
        {
          if ( (d->connected == CON_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_PLAYING)
        &&   IS_SET(d->character->tag_flags,TAG_RED)
          &&   !IS_NPC(d->character)
        &&   !IS_SET(d->character->tag_flags,TAG_FROZEN))
          {
            return;
         }
        }

info("{C[{cFTAG{C] The {BBLUE{w team has won FREEZE TAG!!!",NULL,NULL,INFO_FTAG,0,0);
    return;
    }
}

void do_ftag( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    bool fRed = FALSE;

#ifdef DEBUG
      Debug ("do_ftag");
#endif

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Syntax:  ftag reset\n\r", ch );
        send_to_char( "         ftag start\n\r", ch );
        send_to_char( "         ftag auto (splits everyone in room onto teams)\n\r",ch);
        send_to_char( "         ftag red <player>\n\r", ch );
        send_to_char( "         ftag blue <player>\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "reset" ) )
    {
      for ( d = descriptor_list; d != NULL; d = d->next )
        {
          if (d->connected != CON_PLAYING)
            continue;

          if ( IS_SET(d->character->tag_flags,TAG_PLAYING))
          {
            REMOVE_BIT(d->character->tag_flags,TAG_PLAYING);
            if (!IS_NPC(d->character))
            {
                sprintf(buf,"%s %d",d->character->name,ROOM_VNUM_TEMPLE);
                do_transfer(ch,buf);
            }
          }
          if ( IS_SET(d->character->tag_flags,TAG_FROZEN))
            REMOVE_BIT(d->character->tag_flags,TAG_FROZEN);
          if ( IS_SET(d->character->tag_flags,TAG_RED))
            REMOVE_BIT(d->character->tag_flags,TAG_RED);
          if ( IS_SET(d->character->tag_flags,TAG_BLUE))
            REMOVE_BIT(d->character->tag_flags,TAG_BLUE);
        send_to_char( "Freeze tag has been stopped!\n\r", d->character );
        }
        send_to_char( "All players reset.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "start" ) )
    {
        for ( d = descriptor_list; d != NULL; d = d->next )
        {
          if (d->connected != CON_PLAYING)
            continue;

          if ( IS_SET(d->character->tag_flags,TAG_PLAYING)
                 && !IS_NPC(d->character))
          {
            sprintf(buf,"%s %d",d->character->name,
                number_range(FTAG_MIN_VNUM,FTAG_MAX_VNUM));
            REMOVE_BIT(d->character->tag_flags,TAG_FROZEN);
            do_transfer(ch,buf);
            send_to_char( "Freeze Tag has started!\n\r", d->character );
          }
        }
        send_to_char( "You have started a game of Freeze Tag.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "auto" ) )
    {
        for ( victim = ch->in_room->people; victim != NULL;
                        victim = victim->next_in_room )
       {
          if ( victim == ch || IS_NPC(victim))
            continue;

          if ((fRed = !fRed))
          {
            sprintf(buf,"red %s",victim->name);
          }
          else
          {
            sprintf(buf,"blue %s",victim->name);
          }

          do_ftag(ch,buf);
        }
        return;
    }

    argument = one_argument( argument, arg2 );
    if ( arg2[0] == '\0' ||
           (str_cmp( arg1, "red" ) &&
            str_cmp( arg1, "blue" )))
    {
        send_to_char( "Syntax:  ftag reset\n\r", ch );
        send_to_char( "         ftag start\n\r", ch );
        send_to_char( "         ftag auto (splits everyone in room onto teams)\n\r",ch);
        send_to_char( "         ftag red <player>\n\r", ch );
        send_to_char( "         ftag blue <player>\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "red" ) )
    {
        SET_BIT(victim->tag_flags,TAG_PLAYING);
        REMOVE_BIT(victim->tag_flags,TAG_FROZEN);
        SET_BIT(victim->tag_flags,TAG_RED);
        REMOVE_BIT(victim->tag_flags,TAG_BLUE);
        act( "You are on the {RRED{x team!", ch, NULL, victim, TO_VICT );
        act( "$N is on the {RRED{x team!",   ch, NULL, victim, TO_NOTVICT);
        act( "$N is on the {RRED{x team!",   ch, NULL, victim, TO_CHAR);
    }
    else
    if ( !str_cmp( arg1, "blue" ) )
    {
        SET_BIT(victim->tag_flags,TAG_PLAYING);
       REMOVE_BIT(victim->tag_flags,TAG_FROZEN);
        SET_BIT(victim->tag_flags,TAG_BLUE);
        REMOVE_BIT(victim->tag_flags,TAG_RED);
        act( "You are on the {BBLUE{x team!", ch, NULL, victim, TO_VICT );
        act( "$N is on the {BBLUE{x team!",   ch, NULL, victim, TO_NOTVICT);
        act( "$N is on the {BBLUE{x team!",   ch, NULL, victim, TO_CHAR);
    }

    return;
}

void do_tag( CHAR_DATA *ch, char *argument )
{
    char arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

#ifdef DEBUG
        Debug ("do_tag");
#endif

    argument = one_argument( argument, arg );

    if (!IS_SET(ch->tag_flags,TAG_PLAYING))
    {
        send_to_char( "You're not playing freeze tag.\n\r", ch );
        return;
    }

    if ( arg[0] == '\0' )
    {
        send_to_char( "Tag whom?\n\r", ch );
        return;
    }
   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "You tag yourself.  How amusing.\n\r", ch );
        return;
    }

    if (!IS_SET(victim->tag_flags,TAG_PLAYING))
    {
        send_to_char( "They're not playing freeze tag.\n\r", ch );
        return;
    }
   if (IS_SET(ch->tag_flags,TAG_FROZEN))
    {
        send_to_char( "You can't tag, you're frozen!\n\r", ch );
        return;
    }

    act( "$n tags you.", ch, NULL, victim, TO_VICT );
    act( "$n tags $N.",  ch, NULL, victim, TO_NOTVICT );
    act( "You tag $N.",  ch, NULL, victim, TO_CHAR    );

    if ((IS_SET(ch->tag_flags,TAG_RED) &&
         IS_SET(victim->tag_flags,TAG_RED))
        ||(IS_SET(ch->tag_flags,TAG_BLUE) &&
         IS_SET(victim->tag_flags,TAG_BLUE)))
    {
        if (IS_SET(victim->tag_flags,TAG_FROZEN))
       {
          REMOVE_BIT(victim->tag_flags,TAG_FROZEN);
          act( "You are no longer frozen!", ch, NULL, victim, TO_VICT );
          act( "$N is no longer frozen!",   ch, NULL, victim, TO_NOTVICT);
          act( "$N is no longer frozen!",   ch, NULL, victim, TO_CHAR);
        }
        else
        {
          act( "$N is not frozen!",   ch, NULL, victim, TO_CHAR    );
        }
    }
    else
    {
        if (IS_SET(victim->tag_flags,TAG_FROZEN))
        {
          act( "$N is already frozen!",   ch, NULL, victim, TO_CHAR    );
        }
     else
        {
          SET_BIT(victim->tag_flags,TAG_FROZEN);
          act( "You are frozen!", ch, NULL, victim, TO_VICT );
          act( "$N is frozen!",   ch, NULL, victim, TO_NOTVICT );
          act( "$N is frozen!",   ch, NULL, victim, TO_CHAR    );
          check_team_frozen( victim );
        }
    }

    return;
}
