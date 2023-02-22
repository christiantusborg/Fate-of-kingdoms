/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*     Fate 1.0 is copyright 1997-1998 Hui Kwong Tin (Takeda)               *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.										   *
*     											   *
*     Original Fate of Kingdom Mud (fate.gator.net 2000)			   *
***************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

DECLARE_DO_FUN(do_look);
void    raw_kill        args( ( CHAR_DATA *victim ) );

/* local function */
void  trapdamage args( ( CHAR_DATA *ch, OBJ_DATA *obj) );

bool checkmovetrap(CHAR_DATA *ch)
{
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  bool found;

     if (IS_NPC(ch) )
      return FALSE;

    for (obj = ch->in_room->contents; obj != NULL; obj = obj_next)
    {
    obj_next = obj->next_content;

     if(( obj->item_type==ITEM_TRAP)
     && IS_SET(obj->value[0], TRAP_EFF_MOVE)
     && obj->value[2] > 0)
      found = TRUE;
     else
      found = FALSE;

       if (found == TRUE)
       {
         trapdamage(ch, obj);
         return TRUE;
       }
     }
return FALSE;
}

bool checkgetput(CHAR_DATA *ch, OBJ_DATA *obj)
{
     if (IS_NPC(ch) )
      return FALSE;

      if (obj->item_type!=ITEM_TRAP )
         return FALSE;

       if (IS_SET(obj->value[0], TRAP_EFF_OBJECT)
       && obj->value[2] > 0)
        {
        trapdamage(ch, obj);
        return TRUE;
         }
    return FALSE;
}

void do_traplist(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
 
    found = FALSE;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( !can_see_obj( ch, obj )  
         || (obj->item_type!=ITEM_TRAP) )
            continue;

        found = TRUE;

        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
            ;

        if ( in_obj->carried_by != NULL )
        {
            sprintf( buf, "%s carried by %s.\n\r",
                obj->short_descr, PERS(in_obj->carried_by, ch) );
        }
        else
        {
            sprintf( buf, "%s in %s.\n\r",
                obj->short_descr, in_obj->in_room == NULL
                    ? "somewhere" : in_obj->in_room->name );
        }

        buf[0] = UPPER(buf[0]);
        send_to_char( buf, ch );
    }

    if ( !found )
        send_to_char( "No traps found.\n\r", ch );

    return;
}

void trapdamage(CHAR_DATA *ch, OBJ_DATA *obj)
{
    ROOM_INDEX_DATA *pRoomIndex;
    AFFECT_DATA af; 
    CHAR_DATA *wch;
    int dam = 0;

    if (obj->value[2] <= 0)
       return;
     
     act("{CYou hear a strange noise...{x", ch, NULL, NULL, TO_ROOM);
     act("{CYou hear a strange noise...{x", ch, NULL, NULL, TO_CHAR);
     obj->value[2] -= 1;

     switch(obj->value[1])
     {
     case TRAP_DAM_SLEEP:
     if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
     {
        if ( IS_AFFECTED(ch, AFF_SLEEP) )
        return;
    
    af.type      = 0;
    af.duration  = 4 + obj->level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( ch, &af );

    if ( IS_AWAKE(ch) )
    {
        send_to_char( "{CYou feel very sleepy ..... zzzzzz.{x\n\r", ch );
        act( "{g$n goes to sleep.{x", ch, NULL, NULL, TO_ROOM );
        ch->position = POS_SLEEPING;
    }

     }
     else
     {
      for (wch = ch->in_room->people; wch != NULL; wch = wch->next_in_room)
      {
       
     if ( IS_AFFECTED(wch, AFF_SLEEP) )
        continue;
    
    af.type      = 0;
    af.duration  = 4 + obj->level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( wch, &af );

    if ( IS_AWAKE(wch) )
    {
        send_to_char( "{CYou feel very sleepy ..... zzzzzz.{x\n\r", wch );
        act( "{g$n goes to sleep.{x", wch, NULL, NULL, TO_ROOM );
        wch->position = POS_SLEEPING;
    }
 
      } 
      }
     break;
    
     case TRAP_DAM_TELEPORT:
     if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
     {
     if ( ch->in_room == NULL
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    || ( !IS_NPC(ch) && ch->fighting != NULL ) )
    {
        send_to_char( "{CThat was close...you set off a trap and it malfunctioned!{x\n\r", ch );
        return;
    }

    for ( ; ; )
    {
        pRoomIndex = get_room_index( number_range( 0, 19999 ) );
        if ( pRoomIndex != NULL )
        if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) )
            break;
    }
    act( "{g$n disappears suddenly!{x", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, pRoomIndex );
    act( "{g$n arrives suddenly!{x", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    return;
 
     }
     else
     {
        for (wch = ch->in_room->people; wch != NULL; wch = wch->next_in_room)
       {
        if ( wch->in_room == NULL
       ||   IS_SET(wch->in_room->room_flags, ROOM_NO_RECALL)
       || ( !IS_NPC(wch) && wch->fighting != NULL ) )
       {
        send_to_char( "{CA trap was set off and malfunctioned!{x\n\r",wch);
        continue;
      }

       for ( ; ; )
       {
        pRoomIndex = get_room_index( number_range( 0, 19999 ) );
        if ( pRoomIndex != NULL )
        if ( !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) )
            break;
       }
     act( "{g$n disappears suddenly!{x", wch, NULL, NULL, TO_ROOM );
     char_from_room( wch );
     char_to_room( wch, pRoomIndex );
     act( "{g$n arrives suddenly.{x", wch, NULL, NULL, TO_ROOM );
     do_look( ch, "auto" );
      } 
      }
      break;


      case TRAP_DAM_FIRE:
      if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
     {
      act("{rA flame shoots out of $p and hits $n!{x", ch, obj, NULL,TO_ROOM);
      act("{rA flame shoots out of $p and hits you!{x", ch, obj, NULL,TO_CHAR);
      dam = obj->level * 4; 
     }
     else
     {
      act("{rA fireball shoots out of $p and hits everyone in the room!{x", ch, obj, NULL, TO_ROOM);
      act("{rA fireball shoots out of $p and hits everyone in the room!{x",ch, obj, NULL, TO_CHAR);
      dam = obj->level * 4;
      }
      break;


      case TRAP_DAM_COLD:
      if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
       {
        act("{BA blast of frost from $p hits $n!{x", ch, obj, NULL,TO_ROOM);
        act("{BA blast of frost from $p hits you!{x",ch, obj, NULL,TO_CHAR);
        dam = obj->level * 5;
       }
      else
       {
       act("{BA blast of frost from $p fills the room freezing you!{x",ch, obj, NULL, TO_ROOM);
       act("{BA blast of frost from $p fills the room freezing you!{x",ch,obj, NULL, TO_CHAR);
       dam = obj->level * 5;
       }
      break;
      

       case TRAP_DAM_ACID:
       if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
     {
     act("{YA blast of acid erupts from $p, burning your skin!{x", ch,obj,NULL, TO_CHAR);
     act("{YA blast of acid erupts from $p, burning $n's skin!{x", ch,obj,NULL, TO_ROOM);
     dam = obj->level * 6;
     }
     else
     {
      act("{YA blast of acid erupts from $p, burning your skin!{x", ch,obj,NULL, TO_ROOM);
      act("{YA blast of acid erupts from $p, burning your skin!{x", ch,obj,NULL, TO_CHAR);
      dam = obj->level * 6;
      }

        break;

       case TRAP_DAM_ENERGY:
       if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
     {
     act("{MA pulse of energy from $p zaps $n!{x", ch, obj, NULL,TO_ROOM);
     act("{MA pulse of energy from $p zaps you!{x", ch, obj, NULL,TO_CHAR);
     dam = obj->level * 3;
     }
     else
     {
      act("{MA pulse of energy from $p zaps you!{x", ch, obj, NULL,TO_ROOM);
      act("{MA pulse of energy from $p zaps you!{x", ch, obj,NULL,TO_CHAR);
      dam = obj->level * 3;
      }
       break;


       case TRAP_DAM_BLUNT:
       dam = (10* obj->level) + GET_AC(ch,AC_BASH);
       break;
 
       case TRAP_DAM_PIERCE:
       dam = (10* obj->level) + GET_AC(ch,AC_PIERCE);
       break;
 


       case TRAP_DAM_SLASH:
       dam = (10* obj->level) + GET_AC(ch,AC_SLASH);
       break;
      }
     if (obj->value[1] == TRAP_DAM_BLUNT)
     {
       if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
       {
        act("{g$n sets off a trap on $p and is hit by a blunt object{x",ch, obj, NULL, TO_ROOM);
        act("{gYou are hit by a blunt object from $p!{x", ch, obj, NULL,TO_CHAR);
       }
       else
       {
       act("{g$n sets off a trap on $p and you are hit by a flying object!{x", ch, obj, NULL, TO_ROOM);
       act("{gYou are hit by a blunt object from $p!{x", ch, obj, NULL,TO_CHAR);
       }
     }

     if (obj->value[1] == TRAP_DAM_PIERCE)
      {
       if (!IS_SET( obj->value[0], TRAP_EFF_ROOM) )
       {
        act("{R$n sets of a trap on $p and is pierced in the chest!{x",ch,obj, NULL, TO_ROOM);
        act("{RYou set off a trap on $p and are pierced through the chest!{x", ch, obj, NULL, TO_CHAR);
        }
        else
        {
        act("{R$n sets off a trap on $p and you are hit by a piercing object!{x", ch, obj, NULL, TO_ROOM); 
        act("{RYou set off a trap on $p and are pierced through the chest!{x", ch, obj, NULL, TO_CHAR);
        } 
      }
    
      if (obj->value[1] == TRAP_DAM_SLASH)
        {
        if (!IS_SET(obj->value[0], TRAP_EFF_ROOM) )
        {
         act("{R$n just got slashed by a trap on $p.{x", ch, obj, NULL,TO_ROOM);
         act("{RYou just got slashed by a trap on $p!{x", ch, obj, NULL,TO_CHAR);
        }
        else
        {
         act("{R$n set off a trap releasing a blade that slashes you!{x",ch, obj, NULL, TO_ROOM);
         act("{RYou set off a trap releasing blades around the room...{x",ch, obj, NULL, TO_CHAR);
         act("{ROne of the blades slashes you in the chest!{x", ch,obj,NULL, TO_CHAR);
        }
        }

     /*
      * Do the damage
      */
    if (!IS_SET(obj->value[0], TRAP_EFF_ROOM))
     {            
    if ( ch->position == POS_DEAD )
        return;

dam += 1; /* so dimwits can see they forgot to set the objects level :p */

    if ( dam > 1000 )
    {
        bug( "Damage: %d: more than 1000 points in trap!", dam );
        dam = 1000;
    }

        if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
        {
            affect_strip( ch, gsn_invis );
            affect_strip( ch, gsn_mass_invis );
            REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
            act( "{g$n fades into existence.{x", ch, NULL, NULL, TO_ROOM);
        }

        /*
         * Damage modifiers.
         */
        if ( IS_AFFECTED(ch, AFF_SANCTUARY) )
            dam /= 2;

        if ( dam < 0 )
            dam = 0;

    ch->hit -= dam;
    if ( !IS_NPC(ch)
    &&   ch->level >= LEVEL_IMMORTAL
    &&   ch->hit < 1 )
      ch->hit = 1;
    update_pos( ch );

    switch( ch->position )
    {
    case POS_MORTAL:
         act( "{r$n is mortally wounded by a trap and will die soon if not aided.{x",ch, NULL, NULL, TO_ROOM );
        send_to_char( 
            "{rYou are mortally wounded, and will die soon, if not aided.{x\n\r", ch );
        break;

    case POS_INCAP:
        act( "{r$n is incapacitated and will slowly die, if not aided.{x",ch, NULL, NULL, TO_ROOM );
        send_to_char(
       "{rYou are incapacitated and will slowly die, if not aided.{x\n\r", ch );
        break;

    case POS_STUNNED:
        act( "{C$n has been stunned by a trap, but will probably recover.{x",ch, NULL, NULL, TO_ROOM);
        send_to_char("{CYou are stunned, but will probably recover.{x\n\r",
            ch );
        break;

    case POS_DEAD:
        act( "{r$n died from the trap!{x", ch, 0, 0, TO_ROOM );

 send_to_char( "{rYou have been KILLED!!{x\n\r\n\r", ch );
        break;

    default:
        if ( dam > ch->max_hit / 4 )
            send_to_char( "{rThat trap really did HURT!{x\n\r", ch );
        if ( ch->hit < ch->max_hit / 4 )
            send_to_char( "{rYou sure are BLEEDING!{x\n\r", ch );
        break;
    }
   if (ch->position == POS_DEAD)
   {
            sprintf( log_buf, "%s killed by a trap at %d",
                ch->name,
                ch->in_room->vnum );

            log_string( log_buf );
            
            /* Insert wizinfo stuff here */

            /*
             * Dying penalty:
             * 1/2 way back to previous level.
             */
	if(!IS_NPC(ch)) {
     if ( ch->exp > exp_per_level(ch,ch->pcdata->points)
                                * ch->level )
     gain_exp( ch, -(UMIN(1000,ch->exp * 0.08))
                       *(0.025*ch->pcdata->pkills) );
	}
        raw_kill( ch );
        }
     }
     else
     {
      for (wch = ch->in_room->people; wch != NULL; wch = wch->next_in_room)
       {
         if (wch == NULL)
          break;
        
          if (obj->value[1] == TRAP_DAM_BLUNT)
           dam = (10* obj->level) + GET_AC(wch,AC_BASH);

          if (obj->value[1] == TRAP_DAM_PIERCE)
           dam = (10* obj->level) + GET_AC(wch,AC_PIERCE);

          if (obj->value[1] == TRAP_DAM_SLASH)
           dam = (10* obj->level) + GET_AC(wch,AC_SLASH);

         if ( wch->position == POS_DEAD )
        return;

    /*
     * Stop up any residual loopholes.
     */
    if ( dam > 1000 )
    {
        bug( "Damage: %d: more than 1000 points!", dam );
        dam = 1000;
    }

        if ( IS_AFFECTED(wch, AFF_INVISIBLE) )
        {
            affect_strip( wch, gsn_invis );
            affect_strip( wch, gsn_mass_invis );
  REMOVE_BIT( wch->affected_by, AFF_INVISIBLE );
            act( "$n fades into existence.", wch, NULL, NULL, TO_ROOM );
        }

        /*
         * Damage modifiers.
         */
        if ( IS_AFFECTED(wch, AFF_SANCTUARY) )
            dam /= 2;

        if ( dam < 0 )
            dam = 0;

    wch->hit -= dam;
    if ( !IS_NPC(wch)
    &&   wch->level >= LEVEL_IMMORTAL
    &&   wch->hit < 1 )
      wch->hit = 1;
    update_pos( wch );

    switch( wch->position )
    {
    case POS_MORTAL:

   act( "$n is mortally wounded, and will die soon, if not aided.",
            wch, NULL, NULL, TO_ROOM );
        send_to_char( 
            "You are mortally wounded, and will die soon, if not aided.\n\r",
            wch );
        break;

    case POS_INCAP:
        act( "$n is incapacitated and will slowly die, if not aided.",
            wch, NULL, NULL, TO_ROOM );
        send_to_char(
            "You are incapacitated and will slowly die, if not aided.\n\r",
            wch );
        break;

    case POS_STUNNED:
        act( "$n is stunned, but will probably recover.",wch, NULL, NULL, TO_ROOM);
        send_to_char("You are stunned, but will probably recover.\n\r",
            wch );
        break;

    case POS_DEAD:
        act( "$n is DEAD!!", wch, 0, 0, TO_ROOM );

   send_to_char( "You have been KILLED!!\n\r\n\r", wch );
        break;

    default:
        if ( dam > wch->max_hit / 4 )
            send_to_char( "That really did HURT!\n\r", wch );
        if ( wch->hit < wch->max_hit / 4 )
            send_to_char( "You sure are BLEEDING!\n\r", wch );
        break;
    }
   if (wch->position == POS_DEAD)
   {
            sprintf( log_buf, "%s killed by a trap at %d",
                wch->name,
                wch->in_room->vnum );

            log_string( log_buf );
            
            /* Insert wizinfo stuff here */
 

         if ( !IS_SET ( wch->in_room->room_flags, ROOM_ARENA ) )
         {
          gain_exp( wch, - UMIN(1000,wch->exp * 0.08));
            wch->pcdata->total_death += 1;
         }
 
        raw_kill( wch );
        }
       }
     }
return;
}



