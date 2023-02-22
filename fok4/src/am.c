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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "merc.h"


DECLARE_DO_FUN(	do_look 	);
DECLARE_DO_FUN( do_stand	); 
DECLARE_DO_FUN( do_hide_obj	);

char *	const	dir_name	[]		=
{    "{Bn{borth{w", "{Re{rast{w", "{Cs{couth{w", "{Yw{yest{w", "{Wu{wp",
"{Wd{wown" };

const	sh_int	rev_dir		[]		=
{    2, 3, 0, 1, 5, 4    };

const	sh_int	movement_loss	[SECT_MAX]	=
{    
1, 	/* 0  inside		*/
2, 	/* 1  city		*/
3,	/* 2  field		*/ 	
4, 	/* 3  forest		*/
4, 	/* 4  hills		*/
6, 	/* 5  mountain		*/
4, 	/* 6  water_swim	*/
1,	/* 7  water_noswim	*/ 
6,	/* 8  unused		*/ 
10, 	/* 9  air		*/
6, 	/* 10 desert		*/
1,	/* 11 road		*/
2,	/* 12 enter		*/
12,	/* 13 rock_mountain	*/
18,	/* 14 snow_mountain	*/
6,	/* 15 swamp		*/
6,	/* 16 jungle		*/
2,	/* 17 ruins		*/     
4	/* 18  cavern		*/
};


/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
int     find_exit       args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );
int mount_success 	args( ( CHAR_DATA *ch, CHAR_DATA *mount, int canattack) );
int	check_obj_dodge	args( ( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, int bonus) ); 

void move_char( CHAR_DATA *ch, int door, bool follow )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    CHAR_DATA *mount;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    AREA_DATA* pArea;
    EXIT_DATA *pexit;
    bool room_has_pc;
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];
    int chance;
 if (IS_AFFECTED2(ch,AFF2_WEBS))
    {
        send_to_char( "You attempt to leave the room, but the webs hold you tight.\n\r", ch );
        act( "$n struggles vainly against the webs which hold $m in place.", ch, NULL, NULL, TO_ROOM );
        return;
    }
    
    if (ch->submission > 0)
    {
        send_to_char( "How? By leaving your arm behind?.\n\r", ch );
        act( "$n attempts to flee but a firm grip in his arm prevents it.", ch, NULL, NULL, TO_ROOM );
        return;
    }
    
 if (IS_AFFECTED2(ch,AFF2_STONESHAPE))
    {
        send_to_char( "You cannot move while in stoneform.\n\r", ch );
        return;
    }

if (IS_AFFECTED2(ch,AFF2_HOLD))
    {
        send_to_char( "You are esped.\n\r", ch );
        act( "$n struggles vainly but cannot seem to move.", ch, NULL, NULL, TO_ROOM );
        return;
    }    
  
  if (IS_AFFECTED2(ch,AFF2_PARALYZE))
    {
        send_to_char( "You attempt to leave the room, but a part of your mind refuses.\n\r", ch );
        act( "$n struggles to move but fails miserably.", ch, NULL, NULL, TO_ROOM );
        return;
    }
    
/*if (IS_AFFECTED2(ch,AFF_STUN))
    {
        send_to_char( "You are completely paralyzed and cannot move.\n\r", ch );
        return;
    }*/
     if ( !IS_NPC(ch) && (ch->pcdata->condition[COND_DRUNK]/8) > number_percent( ) )
       {
    	send_to_char( "You lose your balance and fall flat on your rear.\n\r", ch );
    	act( "{C$n trips in his own feet and falls flat on his ass.{c", ch, NULL, NULL, TO_ROOM);
    	ch->position = POS_RESTING;
    	return;
	}
         	
  if ( IS_SET(ch->tag_flags, TAG_FROZEN)
    &&   IS_SET(ch->tag_flags, TAG_PLAYING))
    {
        send_to_char( "You've been frozen, you can't move!\n\r", ch );
        return;
    }

    if ( door < 0 || door > 5 )
    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

/*  adds trap stuff */
/* if ( checkmovetrap(ch) )    return; */
	checkmovetrap(ch);

    if ( IS_AFFECTED( ch, AFF_HIDE ) && !IS_AFFECTED(ch, AFF_SNEAK) )  {
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	send_to_char( "{CYou step out of shadows.{x\n\r", ch );
	act( "{C$n steps out of shadows.{c", ch, NULL, NULL, TO_ROOM);
    }/*
    if ( IS_AFFECTED( ch, AFF_FADE ) && !IS_AFFECTED(ch, AFF_SNEAK) )  {
	REMOVE_BIT(ch->affected_by, AFF_FADE);
	send_to_char( "{CYou step out of shadows.{x\n\r", ch );
	act( "{C$n steps out of shadows.{x", ch, NULL, NULL, TO_ROOM);
    }*/
    if ( IS_AFFECTED( ch, AFF_CAMOUFLAGE ) )  {
	REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	send_to_char( "{CYou step out from your cover.{x\n\r", ch );
	act( "{C$n steps out from $m's cover.{x", ch, NULL, NULL,TO_ROOM);
    }
    
    if ( IS_SET(ch->affected_by, AFF_FADE))
    REMOVE_BIT(ch->affected_by, AFF_FADE);

    /*
     * Exit trigger, if activated, bail out. Only PCs are triggered.
     */
    if ( !IS_NPC(ch) 
      && (p_exit_trigger( ch, door, PRG_MPROG ) 
      ||  p_exit_trigger( ch, door, PRG_OPROG )
      ||  p_exit_trigger( ch, door, PRG_RPROG )) )
	return;


    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||	 !can_see_room(ch,pexit->u1.to_room))
    {
	send_to_char( "{CAlas, you cannot go that way.{x\n\r", ch );
	return;
    }

if (!IS_IMMORTAL(ch))
{
   if (pexit->u1.to_room->sector_type==SECT_ROCK_MOUNTAIN)
   {
	send_to_char("{cThose mountains are too high to climb or even fly over!{x\n\r",ch);
	return;	
   }
   else if (pexit->u1.to_room->sector_type==SECT_SNOW_MOUNTAIN)
   {
	send_to_char("{cThose mountains are too cold for survival.{x\n\r",ch);
	return;	
   }
} /* if mortal */
    if ( IS_AFFECTED2(ch, AFF2_SUBMERGED))
     if ((pexit->u1.to_room->sector_type!=SECT_WATER_SWIM) && (pexit->u1.to_room->sector_type!=SECT_WATER_NOSWIM))
     	{
	send_to_char("{cYou should swim to the surface before you make landfall{x\n\r",ch);
	return;	
        }

    if (IS_SET(pexit->exit_info, EX_CLOSED) 
      && (!IS_AFFECTED(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS))
      &&   !IS_TRUSTED(ch,ANGEL) )
    {
	if ( IS_AFFECTED( ch, AFF_PASS_DOOR ) && IS_SET(pexit->exit_info, EX_NOPASS))  {
  	  act( "{CYou failed to pass through the $d.{x", ch, NULL,pexit->keyword, TO_CHAR);
	  act( "{C$n tries to pass through the $d, but $e fails{x", ch,NULL, pexit->keyword, TO_ROOM);
	}
	else
	  act( "{CThe $d is closed.{x", ch, NULL, pexit->keyword,TO_CHAR);
        return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
	send_to_char( "{CWhat?  And leave your beloved master?{x\n\r",ch);
	return;
    }

//    if ( !is_room_owner(ch,to_room) && room_is_private( to_room ) )

    if ( room_is_private( to_room ) )
    {
	send_to_char( "{CThat room is private right now.{x\n\r", ch );
	return;
    }

 if (MOUNTED(ch))
    {
/*        if (MOUNTED(ch)->position == POS_DEAD)
	{
	send_to_char("Your mount has just died.\n\r",ch);
	ch->riding = FALSE;
	ch->mount = NULL;
	} */
        if (MOUNTED(ch)->position < POS_FIGHTING)
        {
         send_to_char("Your mount must be standing.\n\r", ch);
         return;
        }
        if (!mount_success(ch, MOUNTED(ch), FALSE))
        {
         send_to_char("Your mount subbornly refuses to go that way.\n\r",ch);
         return;
        }
    }


    if ( !IS_NPC(ch) )
    {
	int iClass, iGuild;
	int move;


	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	    for ( iGuild = 0; iGuild < MAX_GUILD; iGuild ++)	
	    	if ( to_room->vnum == class_table[iClass].guild[iGuild] 
		     && !IS_IMMORTAL(ch) )
	    	{
		    if ( iClass != ch->class )  {
		      send_to_char( "{CYou aren't allowed in there.{x\n\r", ch );
		      return;
		    }
                    if (ch->last_fight_time != -1 &&
                      current_time - ch->last_fight_time < FIGHT_DELAY_TIME)
                    {
                      send_to_char(
                           "{CYou feel too {rbloody{C to go in there now.{x\n\r",ch);
                      return;
                    }
	        }

	if ( in_room->sector_type == SECT_AIR
	||   to_room->sector_type == SECT_AIR )
	{
   if ( MOUNTED(ch) )
            {
                if( !IS_AFFECTED(MOUNTED(ch), AFF_FLYING) )
                {
                    send_to_char( "Your mount can't fly.\n\r", ch );
                    return;
                }
	}
	else if ( !IS_AFFECTED(ch, AFF_FLYING) && !IS_IMMORTAL(ch))
	    {
		send_to_char( "{CYou can't fly.{x\n\r", ch );
		return;
	    }
	}

	  if (( in_room->sector_type == SECT_WATER_NOSWIM
        ||    to_room->sector_type == SECT_WATER_NOSWIM )
        &&    (MOUNTED(ch) && !IS_AFFECTED(MOUNTED(ch),AFF_FLYING)) )
        {
            send_to_char("You can't take your mount there.\n\r",ch);
            return;
        }

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
  	&&    !IS_AFFECTED(ch,AFF_FLYING) && (ch->race != race_lookup("merma")))
	{
	    OBJ_DATA *obj;
	    bool found;

	    /*
	     * Look for a boat.
	     */
	    found = FALSE;

	    if (IS_IMMORTAL(ch))
		found = TRUE;

        if(number_percent() < get_skill(ch, gsn_swim))
                found = TRUE;

	    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->item_type == ITEM_BOAT )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if ( !found )
	    {
		send_to_char( "{CYou need a boat to go there.{x\n\r", ch);
		return;
	    }
	}

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;

        move /= 2;  /* i.e. the average */

if ( IS_SET(pexit->u1.to_room->room_flags, ROOM_WILDERNESS) ) move *= 2; 

	/* conditional effects */
	if (IS_AFFECTED(ch,AFF_FLYING) || IS_AFFECTED(ch,AFF_HASTE))
	    move /= 2;

	if (IS_AFFECTED(ch,AFF_SLOW))
	    move *= 2;

	  if ( !MOUNTED(ch) && ch->move < move )
        {
            send_to_char( "You are too exhausted.\n\r", ch );
            return;
        }

	/* 
	 * Forestry Skill Coded by Takeda 3-3-00 	
	 * Part of Imrahil's Idea			
	 */

	if ((ch->in_room->sector_type == SECT_FOREST) ||
	    (ch->in_room->sector_type == SECT_JUNGLE)) {
		 chance = get_skill(ch,gsn_forestry);
			if ( number_percent( ) < chance )
        		{	  
				if(move <= 1)
				  move = 1;
				else
				  move /= 2;
				check_improve(ch,gsn_forestry,TRUE,5);     
		  	} /* if */
	} /* if */

	/* 
	 * Mountaineer Skill Coded by Takeda 3-3-00 	
	 * Part of Imrahil's Idea			
	 */

	if ((ch->in_room->sector_type == SECT_HILLS) ||
	    (ch->in_room->sector_type == SECT_MOUNTAIN) ||
	    (ch->in_room->sector_type == SECT_SNOW_MOUNTAIN) ||
	    (ch->in_room->sector_type == SECT_CAVERN) ||
	    (ch->in_room->sector_type == SECT_ROCK_MOUNTAIN)) {
	            chance = get_skill(ch,gsn_mountaineer);  
			if ( number_percent( ) < chance )
        		{	  
				if(move <= 1)
				  move = 1;
				else
				  move /= 2;
				check_improve(ch,gsn_mountaineer,TRUE,5);     
		  	} /* if */
	} /* if */


     if (!IS_IMMORTAL(ch)) /* so i can build wilderness faster */
      {
       if ( ch->in_room->sector_type == SECT_DESERT || IS_WATER(ch->in_room) )
        {
          WAIT_STATE( ch, 2 );
        }  
       else
        {
         WAIT_STATE( ch, 1 );
        } 
       }

    if (IS_AFFECTED2(ch,AFF2_PANIC))
     {
      move *= 5;
     }
        if (!MOUNTED(ch))
          ch->move -= move;
    }
    
    if (IS_AFFECTED2(ch,AFF2_WITHER))
      WAIT_STATE(ch,PULSE_VIOLENCE/2);

    if ( !IS_AFFECTED(ch, AFF_SNEAK) && (!is_affected(ch,skill_lookup("wraithform")))
    &&   ch->invis_level < LEVEL_HERO)
	{
        if (!IS_NPC(ch) && ch->in_room->sector_type != SECT_INSIDE &&
            ch->in_room->sector_type != SECT_CITY &&
            number_percent() < ch->pcdata->learned[gsn_quiet_movement])
          {
  	    if (MOUNTED(ch))
                xprintf(buf,"$n leaves, riding on %s.",
                        MOUNTED(ch)->short_descr );
            else xprintf(buf,"$n leaves.");
            check_improve(ch,gsn_quiet_movement,TRUE,1);

          }
	else 
  	{
           if (MOUNTED(ch))
                xprintf(buf,"$n leaves $T, riding on %s.",
                        MOUNTED(ch)->short_descr );
           else xprintf(buf,"$n leaves $T." );
          }
          act( buf, ch, NULL, dir_name[door], TO_ROOM );
     }
       mount = MOUNTED(ch);
    char_from_room( ch );
    char_to_room( ch, to_room );

    /* Room history -- for tracking */
    if (!IS_NPC(ch) && ch->in_room)
      room_record(ch->name,in_room, door);


    if ( !IS_AFFECTED(ch, AFF_SNEAK) && (!is_affected(ch,skill_lookup("wraithform")))
    &&   ch->invis_level < LEVEL_HERO)
	 {
        if (mount)
             act( "$n has arrived, riding $N.", ch, NULL, mount,TO_ROOM );
        else act( "$n has arrived.", ch, NULL, NULL, TO_ROOM );
      }

	do_look(ch,"auto");

    pArea = ch->in_room->area;

 /* MSP */
      if(ch->msp == 1)
       {
	if(!is_full_name(pArea->owner,"NONE")) 
	 {
	  if(is_full_name(pArea->owner,kingdom_table[KINGDOM_ALDON].short_name)) 
		send_to_char("!!MUSIC(music/fokkujikiri.mid T=kingdom)",ch);
          if(is_full_name(pArea->owner,kingdom_table[KINGDOM_ARKADIA].short_name)) 
		send_to_char("!!MUSIC(music/fokasper.mid T=kingdom)",ch);
	  if(is_full_name(pArea->owner,kingdom_table[KINGDOM_GONDOR].short_name)) 
		send_to_char("!!MUSIC(music/foksirius.mid T=kingdom)",ch);	
	  if(is_full_name(pArea->owner,kingdom_table[KINGDOM_DRAKKON].short_name)) 
		send_to_char("!!MUSIC(music/fokgemma.mid T=kingdom)",ch);	
	 }
	else 
	 {
	  if(is_full_name(pArea->name,"Sanaria"))
		send_to_char("!!MUSIC(music/foksanaria.mid T=town)",ch);
	  if(is_full_name(pArea->name,"Fate"))
		send_to_char("!!MUSIC(music/fokworld.mid T=town)",ch);
	  if(is_full_name(pArea->name,"Emerald Forest"))
		send_to_char("!!MUSIC(music/fokeforest.mid T=town)",ch);
	  if(is_full_name(pArea->name,"Lankford Ruins"))
		send_to_char("!!MUSIC(music/foklanford.mid T=town)",ch);
	  if(is_full_name(pArea->name,"New Thalos"))
		send_to_char("!!MUSIC(music/foknewthalos.mid T=town)",ch);
	  if(is_full_name(pArea->name,"Elvandar"))
		send_to_char("!!MUSIC(music/fokelvander.mid T=town)",ch);
	  if(is_full_name(pArea->name,"Old Reactor"))
		send_to_char("!!MUSIC(music/fokreactor.mid T=town)",ch);
	  }
	}  

    if (mount)
        {
         char_from_room( mount );
	 char_to_room( mount, to_room);
         ch->riding = TRUE;
         mount->riding = TRUE;
        }

    if (in_room == to_room) /* no circular follows */
	return;


    for (fch = to_room->people,room_has_pc = FALSE;fch != NULL; fch = fch_next)
      {
        fch_next = fch->next_in_room;
        if (!IS_NPC(fch))
          room_has_pc = TRUE;
      }

    for (fch = to_room->people;fch!=NULL;fch = fch_next) {
      fch_next = fch->next_in_room;

      /* greet trigger for items carried by people in room */
      for (obj = fch->carrying;room_has_pc && obj != NULL;
           obj = obj->next_content)
        {
          if (IS_SET(obj->progtypes,IPROG_GREET))
            (obj->pIndexData->iprogs->greet_prog) (obj,ch);
        }

      /* Greet trigger for mobs  */
      if (room_has_pc && IS_SET(fch->progtypes,MPROG_GREET))
        (fch->pIndexData->moprogs->greet_prog) (fch,ch);
    }

    /* Entry trigger for items */
    for (obj = ch->carrying;room_has_pc && obj!=NULL;obj=obj->next_content)
      {
        if (IS_SET(obj->progtypes,IPROG_ENTRY))
          (obj->pIndexData->iprogs->entry_prog) (obj);
      }


    for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next_in_room;

	if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM) 
	&&   fch->position < POS_STANDING)
	do_stand(fch,"");

	if ( fch->master == ch && fch->position == POS_STANDING 
	&&   can_see_room(fch,to_room))
	{

	    if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
	    &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
	    {
		act("{CYou can't bring $N into the city.{x",
		    ch,NULL,fch,TO_CHAR);
		act("{CYou aren't allowed in the city.{x",
		    fch,NULL,NULL,TO_CHAR);
		continue;
	    }

	    act( "{cYou follow $N.{x", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door, TRUE );
	}
    }

    /*
     * If someone is following the char, these triggers get activated
     * for the followers before the char, but it's safer this way...
     */

    if ( IS_NPC( ch ) && HAS_TRIGGER_MOB( ch, TRIG_ENTRY ) )
       p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_ENTRY );
    
    if ( !IS_NPC( ch ) )
    {
    	p_greet_trigger( ch, PRG_MPROG );
	p_greet_trigger( ch, PRG_OPROG );
	p_greet_trigger( ch, PRG_RPROG );
    }

    /* greet trigger for items on the ground */
    for (obj = ch->in_room->contents;room_has_pc && obj != NULL;
         obj = obj->next_content)
      {
        if (IS_SET(obj->progtypes,IPROG_GREET))
          (obj->pIndexData->iprogs->greet_prog) (obj,ch);
      }

    if ( /* room_has_pc && */ IS_SET(ch->progtypes,MPROG_ENTRY))
      (ch->pIndexData->moprogs->entry_prog) (ch);

    return;
}



void do_north( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) && ((ch->pcdata->condition[COND_DRUNK]/8) > number_percent( )
	|| (is_affected(ch,gsn_confuse) && number_percent( ) > 30)) )
       {
        if ( number_percent( ) > 70)
        {
        switch(number_range(1,5)) {
        case 1:
        move_char( ch, DIR_SOUTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 2:
        move_char( ch, DIR_WEST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 3:
        move_char( ch, DIR_EAST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 4:
        move_char( ch, DIR_DOWN, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 5:
        move_char( ch, DIR_UP, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
	}
	return;
	}
      }
        move_char( ch, DIR_NORTH, FALSE );
        return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
	if ( !IS_NPC(ch) && ((ch->pcdata->condition[COND_DRUNK]/8) > number_percent( )
	|| (is_affected(ch,gsn_confuse) && number_percent( ) > 30)) )
       {
        switch(number_range(1,5)) {
        case 1:
        move_char( ch, DIR_SOUTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 2:
        move_char( ch, DIR_WEST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 3:
        move_char( ch, DIR_NORTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 4:
        move_char( ch, DIR_DOWN, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 5:
        move_char( ch, DIR_UP, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
	}
	return;
	}
    move_char( ch, DIR_EAST, FALSE );
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
	if ( !IS_NPC(ch) && ((ch->pcdata->condition[COND_DRUNK]/8) > number_percent( )
	|| (is_affected(ch,gsn_confuse) && number_percent( ) > 30)) )
        {
        switch(number_range(1,5)) {
        case 1:
        move_char( ch, DIR_NORTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 2:
        move_char( ch, DIR_WEST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 3:
        move_char( ch, DIR_EAST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 4:
        move_char( ch, DIR_DOWN, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 5:
        move_char( ch, DIR_UP, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
	}
	return;
	}
    move_char( ch, DIR_SOUTH, FALSE );
    return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
	if ( !IS_NPC(ch) && ((ch->pcdata->condition[COND_DRUNK]/8) > number_percent( )
	|| (is_affected(ch,gsn_confuse) && number_percent( ) > 30)) )
        {
        switch(number_range(1,5)) {
        case 1:
        move_char( ch, DIR_SOUTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 2:
        move_char( ch, DIR_NORTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 3:
        move_char( ch, DIR_EAST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 4:
        move_char( ch, DIR_DOWN, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 5:
        move_char( ch, DIR_UP, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
         break;
	}
	return;
      }
    move_char( ch, DIR_WEST, FALSE );
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
	if ( !IS_NPC(ch) && ((ch->pcdata->condition[COND_DRUNK]/8) > number_percent( )
	|| (is_affected(ch,gsn_confuse) && number_percent( ) > 30)) )
        {
        switch(number_range(1,5)) {
        case 1:
        move_char( ch, DIR_SOUTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 2:
        move_char( ch, DIR_WEST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 3:
        move_char( ch, DIR_EAST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 4:
        move_char( ch, DIR_DOWN, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 5:
        move_char( ch, DIR_NORTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
	}
	return;
	}
    move_char( ch, DIR_UP, FALSE );
    return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
	if ( !IS_NPC(ch) && ((ch->pcdata->condition[COND_DRUNK]/8) > number_percent( )
	|| (is_affected(ch,gsn_confuse) && number_percent( ) > 30)) )
        {
        switch(number_range(1,5)) {
        case 1:
        move_char( ch, DIR_SOUTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 2:
        move_char( ch, DIR_WEST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 3:
        move_char( ch, DIR_EAST, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
        case 4:
        move_char( ch, DIR_NORTH, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        return;
        case 5:
        move_char( ch, DIR_UP, FALSE );
        send_to_char("You accidently stumple the wrong direction.\n\r",ch);
        break;
	}
	return;
	}
    move_char( ch, DIR_DOWN, FALSE );
    return;
}

int find_exit( CHAR_DATA *ch, char *arg )
{
    int door;

        if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
        act( "I see no exit $T here.", ch, NULL, arg, TO_CHAR );
        return -1;
    }

    return door;
}

int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "{cI see no $T here.{x", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "{cThere is no door $T here.{x", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "{cYou can't do that.{x\n\r", ch );
	return -1;
    }

    return door;
}



void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cOpen what?{x\n\r", ch );
	return;
    }



    if ( ( obj = get_obj_here( ch, NULL,  arg ) ) != NULL )
    {
 	/* open portal */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1], EX_ISDOOR))
	    {
		send_to_char("{cYou can't do that.{x\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1], EX_CLOSED))
	    {
		send_to_char("{cIt's already open.{x\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1], EX_LOCKED))
	    {
		send_to_char("{cIt's locked. Try to unlock first.{x\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1], EX_CLOSED);
	    act("{CYou open $p.{x",ch,obj,NULL,TO_CHAR);
	    act("{C$n opens $p.{x",ch,obj,NULL,TO_ROOM);
	    return;
 	}

	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER)
	    { send_to_char( "{cThat's not a container.{x\n\r", ch );return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "{cIt's already open.{x\n\r",      ch );return;}
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "{cYou can't do that.{x\n\r", ch ); return;}
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "{cIt's locked.{x\n\r",ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	act("{CYou open $p.{x",ch,obj,NULL,TO_CHAR);
	act( "{C$n opens $p.{x", ch, obj, NULL, TO_ROOM );
	return;
    }
    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "{cIt's already open.{x\n\r",      ch );return;}
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "{cIt's locked.{x\n\r",            ch );return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "{C$n opens the $d.{x", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "{CYou open it.{x\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "{CThe $d opens.{x", rch, NULL, pexit_rev->keyword,TO_CHAR );
	}
	return;
    }
    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cClose what?{x\n\r", ch );
	return;
    }



    if ( ( obj = get_obj_here( ch, NULL,  arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{

	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||   IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char("{cYou can't do that.{x\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("{cIt is already closed.{x\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_CLOSED);
	    act("{CYou close $p.{x",ch,obj,NULL,TO_CHAR);
	    act("{C$n closes $p.{x",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "{cThat's not a container.{x\n\r", ch );return;}
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "{cIt is already closed.{x\n\r",    ch );return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "{cYou can't do that.{x\n\r",      ch );return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	act("{CYou close $p.{x",ch,obj,NULL,TO_CHAR);
	act("{C$n closes $p.{x", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "{cIt's already closed.{x\n\r",    ch );return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "{C$n closes the $d.{x", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "{CYou close it.{x\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "{CThe $d closes.{x", rch, NULL, pexit_rev->keyword,
TO_CHAR );
	}
	return;
    }

    return;
}


bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	  if ( can_see_obj( ch, obj ) )
	    return TRUE;
    }

    return FALSE;
}

bool has_key_ground( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
          if ( can_see_obj( ch, obj ) )
	    return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cLock what?{x\n\r", ch );
	return;
    }


    if ( ( obj = get_obj_here( ch, NULL,  arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||  IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char("{cYou can't do that.{x\n\r",ch);
		return;
	    }
	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("{cIt is not closed.{x\n\r",ch);
	 	return;
	    }

	    if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
	    {
		send_to_char("{cIt can not be locked.{x\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("{cYou lack the key.{x\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("{cIt is already locked.{x\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_LOCKED);
	    act("{CYou lock $p.{x",ch,obj,NULL,TO_CHAR);
	    act("{C$n locks $p.{x",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "{cThat's not a container.{x\n\r", ch );return;}
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "{cIt is not closed.{x\n\r",        ch);return;}
	if ( obj->value[2] < 0 )
	    { send_to_char( "{cIt can not be locked.{x\n\r",     ch );return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "{cYou lack the key.{x\n\r",       ch );return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "{cIt is already locked.{x\n\r",    ch );return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	act("{CYou lock $p.{x",ch,obj,NULL,TO_CHAR);
	act("{C$n locks $p.{x", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "{cIt is not closed.{x\n\r",        ch );return; }
	if ( pexit->key < 0 )
	    { send_to_char( "{cIt can not be locked.{x\n\r",     ch );return; }
	if ( !has_key( ch, pexit->key) && 
             !has_key_ground( ch, pexit->key) )
	    { send_to_char( "{cYou lack the key.{x\n\r",       ch );return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "{cIt is already locked.{x\n\r",    ch );return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "{C*Click*{x\n\r", ch );
	act( "{C$n locks the $d.{x", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "{CThe $d clicks.{x", rch, NULL, pexit_rev->keyword,
TO_CHAR );

	}
        return;
    }
    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cUnlock what?{x\n\r", ch );
	return;
    }


    if ( ( obj = get_obj_here( ch, NULL,  arg ) ) != NULL )
    {
 	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char("{cYou can not do that.{x\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("{cIt is not closed.{x\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("{cIt can not be unlocked.{x\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("{cYou lack the key.{x\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("{cIt is already unlocked.{x\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("{CYou unlock $p.{x",ch,obj,NULL,TO_CHAR);
	    act("{C$n unlocks $p.{x",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "{cThat is not a container.{x\n\r", ch );return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "{cIt is not closed.{x\n\r",        ch );return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "{cIt can't be unlocked.{x\n\r",   ch );return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "{cYou lack the key.{x\n\r",       ch );return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "{cIt is already unlocked.{x\n\r",  ch );return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	act("{CYou unlock $p.{x",ch,obj,NULL,TO_CHAR);
	act("{C$n unlocks $p.{x", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "{cIt is not closed.{x\n\r",        ch );return; }
	if ( pexit->key < 0 )
	    { send_to_char( "{cIt can not be unlocked.{x\n\r",   ch );return; }
	if ( !has_key( ch, pexit->key) &&
	     !has_key_ground( ch, pexit->key) )
	    { send_to_char( "{cYou lack the key.{x\n\r",       ch );return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "{cIt is already unlocked.{x\n\r",  ch );return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "{C*Click*{x\n\r", ch );
	act( "{C$n unlocks the $d.{x", ch, NULL, pexit->keyword, TO_ROOM);

	/* unlock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "{CThe $d clicks.{x", rch, NULL, pexit_rev->keyword,TO_CHAR );
	}
        return;
    }
    return;
}
/*  stopped color editing here */


void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }
 if (MOUNTED(ch))
    {
        send_to_char("You can't pick while mounted.\n\r", ch);
        return;
    }


    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > get_skill(ch,gsn_pick_lock))
    {
	send_to_char( "You failed.\n\r", ch);
	check_improve(ch,gsn_pick_lock,FALSE,2);
	return;
    }

    if ( ( obj = get_obj_here( ch, NULL,  arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {	
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_PICKPROOF))
	    {
		send_to_char("You failed.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	    check_improve(ch,gsn_pick_lock,TRUE,2);
	    return;
	}

	
	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
        act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
        act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	check_improve(ch,gsn_pick_lock,TRUE,2);
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL(ch))
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 && !IS_IMMORTAL(ch))
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	check_improve(ch,gsn_pick_lock,TRUE,2);

	/* pick the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;
    int chance =0;
    
    if ( (ch->lagstand > 0) && (ch->fighting != NULL) && (ch->position == POS_RESTING) )
     {
      chance = 100 - ch->lagstand * 9;
     if (number_percent() < chance)
      {
       ch->lagstand = 0;
      }
     else
      {
       send_to_char("Youve fallen and cant get up.\n\r",ch);
       return;      
      }
     }
    if (argument[0] != '\0')
    {
	if (ch->position == POS_FIGHTING)
	{
	    send_to_char("Maybe you should finish fighting first?\n\r",ch);
	    return;
	}
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	if (obj->item_type != ITEM_FURNITURE
	||  (!IS_SET(obj->value[2],STAND_AT)
	&&   !IS_SET(obj->value[2],STAND_ON)
	&&   !IS_SET(obj->value[2],STAND_IN)))
	{
	    send_to_char("You can't seem to find a place to stand.\n\r",ch);
	    return;
	}
	if (ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
			p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );
	    act_new("There's no room to stand on $p.",
		ch,obj,NULL,TO_ROOM,POS_DEAD);
	    return;
	}
    }
    
    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }
	
	if (obj == NULL)
	{
	    send_to_char( "You wake and stand up.\n\r", ch );
	    act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
  if(ch->msp == 1)
	send_to_char("!!MUSIC(music/fokscore.mid T=sleep)\n!!MUSIC(off)",ch);
	DAZE_STATE(ch,60);
	send_to_char("{cYou stretch and shake off your fatigue.{x\n\r",ch);
	WAIT_STATE(ch,6);
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	   act_new("You wake and stand at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	   act("$n wakes and stands at $p.",ch,obj,NULL,TO_ROOM);
	DAZE_STATE(ch,60);
	send_to_char("{cYou stretch and shake off your fatigue.{x\n\r",ch);
	WAIT_STATE(ch,6);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act_new("You wake and stand on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands on $p.",ch,obj,NULL,TO_ROOM);
	DAZE_STATE(ch,60);
	send_to_char("{cYou stretch and shake off your fatigue.{x\n\r",ch);
	WAIT_STATE(ch,6);
	}
	else 
	{
	    act_new("You wake and stand in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands in $p.",ch,obj,NULL,TO_ROOM);
	DAZE_STATE(ch,60);
	send_to_char("{cYou stretch and shake off your fatigue.{x\n\r",ch);
 	WAIT_STATE(ch,6);	
	}
     if (ch->fighting == NULL)
	ch->position = POS_STANDING;
      else
       ch->position = POS_FIGHTING;	
	do_look(ch,"auto");
	break;

    case POS_RESTING: case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char( "You stand up.\n\r", ch );
	    act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	    act("You stand at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act("You stand on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
	    act("You stand in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
     if (ch->fighting == NULL)
	ch->position = POS_STANDING;
      else
       ch->position = POS_FIGHTING;	
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("You are already fighting!\n\r",ch);
	return;
    }
	 if (MOUNTED(ch))
    {
        send_to_char("You can't rest while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char("You can't rest while being ridden.\n\r", ch);
        return;
    }

    /* okay, now that we know we can rest, find an object to rest on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }


    if (obj != NULL)
    {
        if (!IS_SET(obj->item_type,ITEM_FURNITURE) 
    	||  (!IS_SET(obj->value[2],REST_ON)
    	&&   !IS_SET(obj->value[2],REST_IN)
    	&&   !IS_SET(obj->value[2],REST_AT)))
    	{
	    send_to_char("You can't rest on that.\n\r",ch);
	    return;
    	}

        if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
        {
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
    	}
	
	ch->on = obj;

	    if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
		p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );

    }

    	


    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }
	if (obj == NULL)
	{
	    send_to_char( "You wake up and start resting.\n\r", ch );
	    act ("$n wakes up and starts resting.",ch,NULL,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],REST_AT))
	{
	    act_new("You wake up and rest at $p.",
		    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
	    act("$n wakes up and rests at $p.",ch,obj,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_ON))
        {
            act_new("You wake up and rest on $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
            act_new("You wake up and rest in $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_STANDING:
	if (obj == NULL)
	{
	    send_to_char( "You rest.\n\r", ch );
	    act( "$n sits down and rests.", ch, NULL, NULL, TO_ROOM );
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You sit down at $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits down at $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char("You rest.\n\r",ch);
	    act("$n rests.",ch,NULL,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You rest at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests at $p.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You rest on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_RESTING;
	break;
    }


    return;
}


void do_sit (CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("Maybe you should finish this fight first?\n\r",ch);
	return;
    }
 if (MOUNTED(ch))
    {
        send_to_char("You can't sit while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char("You can't sit while being ridden.\n\r", ch);
        return;
    }

    /* okay, now that we know we can sit, find an object to sit on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else 

	
    if (obj != NULL)
    {
	if (!IS_SET(obj->item_type,ITEM_FURNITURE)
	||  (!IS_SET(obj->value[2],SIT_ON)
	&&   !IS_SET(obj->value[2],SIT_IN)
	&&   !IS_SET(obj->value[2],SIT_AT)))
	{
	    send_to_char("You can't sit on that.\n\r",ch);
	    return;
	}

	if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}

	ch->on = obj;

      obj = ch->on;

	if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
	 	p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );

    }
    switch (ch->position)
    {
	case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }
            if (obj == NULL)
            {
            	send_to_char( "You wake and sit up.\n\r", ch );
            	act( "$n wakes and sits up.", ch, NULL, NULL, TO_ROOM );
            }
            else if (IS_SET(obj->value[2],SIT_AT))
            {
            	act_new("You wake and sit at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits at $p.",ch,obj,NULL,TO_ROOM);
            }
            else if (IS_SET(obj->value[2],SIT_ON))
            {
            	act_new("You wake and sit on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits at $p.",ch,obj,NULL,TO_ROOM);
            }
            else
            {
            	act_new("You wake and sit in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits in $p.",ch,obj,NULL,TO_ROOM);
            }

	    ch->position = POS_SITTING;
	    break;
	case POS_RESTING:
	    if (obj == NULL)
		send_to_char("You stop resting.\n\r",ch);
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits at $p.",ch,obj,NULL,TO_ROOM);
	    }

	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SITTING;
	    break;
	case POS_SITTING:
	    send_to_char("You are already sitting down.\n\r",ch);
	    break;
	case POS_STANDING:
	    if (obj == NULL)
    	    {
		send_to_char("You sit down.\n\r",ch);
    	        act("$n sits down on the ground.",ch,NULL,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit down at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
		act("You sit on $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act("You sit down in $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down in $p.",ch,obj,NULL,TO_ROOM);
	    }
    	    ch->position = POS_SITTING;
    	    break;
    }
    return;
}


void do_sleep( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;



 if (MOUNTED(ch))
    {
        send_to_char("You can't sleep while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char("You can't sleep while being ridden.\n\r", ch);
        return;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING: 
	if (argument[0] == '\0' && ch->on == NULL)
	{
	    send_to_char( "You go to sleep.\n\r", ch );
	    act( "$n goes to sleep.", ch, NULL, NULL, TO_ROOM );
          if(ch->msp == 1)
        send_to_char("!!MUSIC(music/fokscore.mid L=-1 T=sleep)",ch);

	    ch->position = POS_SLEEPING;
	}
	else  /* find an object and sleep on it */
	{
	    if (argument[0] == '\0')
		obj = ch->on;
	    else
	    	obj = get_obj_list( ch, argument,  ch->in_room->contents );

	    if (obj == NULL)
	    {
		send_to_char("You don't see that here.\n\r",ch);
		return;
	    }
	    if (obj->item_type != ITEM_FURNITURE
	    ||  (!IS_SET(obj->value[2],SLEEP_ON) 
	    &&   !IS_SET(obj->value[2],SLEEP_IN)
	    &&	 !IS_SET(obj->value[2],SLEEP_AT)))
	    {
		send_to_char("You can't sleep on that!\n\r",ch);
		return;
	    }

	    if (ch->on != obj && count_users(obj) >= obj->value[0])
	    {
		act_new("There is no room on $p for you.",
		    ch,obj,NULL,TO_CHAR,POS_DEAD);
		return;
	    }

	    ch->on = obj;

	    if ( HAS_TRIGGER_OBJ( obj, TRIG_SIT ) )
		p_percent_trigger( NULL, obj, NULL, ch, NULL, NULL, TRIG_SIT );
	    if (IS_SET(obj->value[2],SLEEP_AT))
	    {
		act("You go to sleep at $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep at $p.",ch,obj,NULL,TO_ROOM);
  if(ch->msp == 1)
        send_to_char("!!MUSIC(music/fokscore.mid L=-1 T=sleep)",ch);

	    }
	    else if (IS_SET(obj->value[2],SLEEP_ON))
	    {
	        act("You go to sleep on $p.",ch,obj,NULL,TO_CHAR);
	        act("$n goes to sleep on $p.",ch,obj,NULL,TO_ROOM);
  if(ch->msp == 1)
        send_to_char("!!MUSIC(music/fokscore.mid L=-1 T=sleep)",ch);
	    }
	    else
	    {
		act("You go to sleep in $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep in $p.",ch,obj,NULL,TO_ROOM);
  if(ch->msp == 1)
        send_to_char("!!MUSIC(music/fokscore.mid L=-1 T=sleep)",ch);
	    }
	    ch->position = POS_SLEEPING;
	}
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ 	
	do_stand(ch,argument);
		return; 	}

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }

    act_new( "$n wakes you.", ch, NULL, victim, TO_VICT,POS_SLEEPING );
	do_stand(victim,"");
    return;
}



void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    if (MOUNTED(ch))
    {
        send_to_char("You can't sneak while mounted.\n\r", ch);
        return;
    }

    send_to_char( "You attempt to move silently.\n\r", ch );
    affect_strip( ch, gsn_sneak );

    if (IS_AFFECTED(ch,AFF_SNEAK))
	return;

    if ( number_percent( ) < get_skill(ch,gsn_sneak))
    {
	check_improve(ch,gsn_sneak,TRUE,3);
	af.where     = TO_AFFECTS;
	af.type      = gsn_sneak;
	af.level     = ch->level; 
	af.duration  = ch->level;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	affect_to_char( ch, &af );
    }
    else
	check_improve(ch,gsn_sneak,FALSE,3);

    return;
}

/*
void do_fade( CHAR_DATA *ch, char *argument )
{
  if (!kingdom_ok(ch,gsn_fade))
    return;
 if (MOUNTED(ch))
    {
        send_to_char("You can't fade while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char("You can't fade while being ridden.\n\r", ch);
        return;
    }

      if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE )  )  {
      send_to_char( "You can not fade while glowing.\n\r", ch );
      return;
    }
    if ( IS_AFFECTED( ch, AFF_FAERIE_FOG )  )  {
      send_to_char( "You can not fade while glowing.\n\r", ch );
      return;
    }

  if ((weather_info.sunlight == SUN_LIGHT 
  || weather_info.sunlight == SUN_RISE))
 {
    send_to_char( "The sunlight prevents you to fade into shadows.\n\r", ch );
    return;	
 } 

  if (!(room_is_dark (ch->in_room,NULL)))
  {
    send_to_char( "The light in the room does not allow you to fade.\n\r", ch );
    return;	
  }

    send_to_char( "You attempt to fade into the shadows.\n\r", ch );

    if (!IS_NPC(ch) && 
	ch->level < skill_table[gsn_fade].skill_level[ch->class] )
      return;

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

    if ( IS_AFFECTED(ch, AFF_FADE) )
	REMOVE_BIT(ch->affected_by, AFF_FADE);

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_fade] )
    {
	SET_BIT(ch->affected_by, AFF_FADE);
	check_improve(ch,gsn_fade,TRUE,3);
    }
    else
	check_improve(ch,gsn_fade,FALSE,3);

    return;
}

*/

void do_hide( CHAR_DATA *ch, char *argument )
{
int forest;
/*  adds hidden obj code */
    char       arg [ MAX_INPUT_LENGTH ];
    one_argument( argument, arg );

   if (MOUNTED(ch))
    {
        send_to_char("You can't hide while mounted.\n\r", ch);
        return;
    }

    if (RIDDEN(ch))
    {
        send_to_char("You can't hide while being ridden.\n\r", ch);
        return;
    }

    if ( arg[0] != '\0' )
    {
	do_hide_obj(ch,argument);
        return;
    }
/*  end */

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_hide].skill_level[ch->class] )
    {
	send_to_char(
	    "You don't know how to hide.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE )  )  {
      send_to_char( "You can not hide while glowing.\n\r", ch );
      return;
    }
    if ( IS_AFFECTED( ch, AFF_FAERIE_FOG )  )  {
      send_to_char( "You can not hide while glowing.\n\r", ch );
      return;
    }
    forest = 0;
    forest += ch->in_room->sector_type == SECT_FOREST?60:0;
    forest += ch->in_room->sector_type == SECT_FIELD?60:0;

    send_to_char( "You attempt to hide.\n\r", ch );

    if ( number_percent( ) < get_skill(ch,gsn_hide)-forest)
    {
	SET_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,TRUE,3);
    }
    else  {
      if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,FALSE,3);
    }

    return;
}



void do_camouflage( CHAR_DATA *ch, char *argument )
{
  if (MOUNTED(ch))
    {
        send_to_char("You can't camouflage while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char("You can't camouflage while being ridden.\n\r", ch);
        return;
    }


    if ( IS_NPC(ch) || 
         ch->level < skill_table[gsn_camouflage].skill_level[ch->class] )
      {
	send_to_char("You don't know how to camouflage yourself.\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE ) )  {
      send_to_char( "You can't camouflage yourself while glowing.\n\r", ch );
      return; 
    }
   if ( IS_AFFECTED( ch, AFF_FAERIE_FOG ) )  {
      send_to_char( "You can't camouflage yourself while glowing.\n\r", ch );
      return; 
    }

    if (ch->in_room->sector_type != SECT_FOREST &&
	ch->in_room->sector_type != SECT_HILLS  &&
	ch->in_room->sector_type != SECT_MOUNTAIN)
      {
	send_to_char("There is no cover here.\n\r",ch);
	act("$n tries to camouflage $mself against the lone leaf on the ground.",ch,NULL,NULL,TO_ROOM);
	return;
      }
    send_to_char( "You attempt to camouflage yourself.\n\r", ch );
    WAIT_STATE( ch, skill_table[gsn_camouflage].beats );

    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) )
	REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);


    if ( IS_NPC(ch) || 
	number_percent( ) < ch->pcdata->learned[gsn_camouflage] )
    {
	SET_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	check_improve(ch,gsn_camouflage,TRUE,1);
    }
    else
	check_improve(ch,gsn_camouflage,FALSE,1);

    return;
}

/*
 * Contributed by Alander
 */
void do_visible( CHAR_DATA *ch, char *argument )
{

    if (IS_SET(ch->affected_by, AFF_FADE))
      {
	send_to_char("You fade into existence.\n\r", ch);
	affect_strip(ch, gsn_fade);
	REMOVE_BIT(ch->affected_by, AFF_FADE);
	act("$n fades into existence.", ch, NULL, NULL, TO_ROOM);
      }
     
	if (IS_SET(ch->affected_by, AFF_HIDE))
      {
	send_to_char("You step out of the shadows.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	act("$n steps out of the shadows.", ch, NULL, NULL, TO_ROOM);
      }
    if (IS_SET(ch->affected_by, AFF_CAMOUFLAGE))
      {
	send_to_char("You step out from your cover.\n\r",ch);
	REMOVE_BIT(ch->affected_by,AFF_CAMOUFLAGE);
	act("$n steps out from $s cover.",ch,NULL,NULL,TO_ROOM);
      }
    if (IS_SET(ch->affected_by, AFF_INVISIBLE))
      {
	send_to_char("You fade into existence.\n\r", ch);
	affect_strip(ch, gsn_invis);
	affect_strip(ch, gsn_mass_invis);
	affect_strip(ch, gsn_shadow_door);
	REMOVE_BIT(ch->affected_by, AFF_INVISIBLE);
	act("$n fades into existence.", ch, NULL, NULL, TO_ROOM);
      }
     
    if (IS_SET(ch->affected_by, AFF_SNEAK) 
	&& !IS_NPC(ch) && !IS_SET(race_table[ch->race].aff,AFF_SNEAK) )
      {
	send_to_char("You trample around loudly again.\n\r", ch);
	affect_strip(ch, gsn_sneak);
	REMOVE_BIT(ch->affected_by, AFF_SNEAK);
      }

    affect_strip ( ch, gsn_mass_invis			);
    affect_strip ( ch, gsn_spell_illusion_fields	);
    return;
}

void do_recall( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;
    int point;
 
    if (IS_GOOD(ch))      /* set values for hometown recall table */
      point = hometown_table[ch->hometown].recall[0];
    else if (IS_EVIL(ch))
      point = hometown_table[ch->hometown].recall[2];
    else
      point = hometown_table[ch->hometown].recall[1];

    if (IS_NPC(ch) && !IS_SET(ch->act,ACT_PET))
    {
	send_to_char("Only players can recall.\n\r",ch);
	return;
    }

    if (ch->level >= 11)
      {
	send_to_char("Only those under level 10 get free recalls.\n\r",ch);
	return;
      }

    if (ch->desc != NULL && current_time - ch->last_fight_time
	< FIGHT_DELAY_TIME)
      {
	send_to_char("You are too pumped to pray now.\n\r",ch);
	return;
      }

    if (ch->desc == NULL && !IS_NPC(ch))
      {
	point =
	hometown_table[number_range(0, 4)].recall[number_range(0,2)];
      }

    act( "$n prays for transportation!", ch, 0, 0, TO_ROOM );
    
    if ( ( location = get_room_index(point ) )
	== NULL )
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "The gods have forsaken you.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
	send_to_char( "You are still fighting!\n\r", ch );
/*
	int lose,skill;

	if (IS_NPC(ch))
	    skill = 40 + ch->level;
	else
	    skill = ch->pcdata->learned[gsn_recall];

	if ( number_percent() < 80 * skill / 100 )
	{
	    check_improve(ch,gsn_recall,FALSE,6);
	    WAIT_STATE( ch, 4 );
	    xprintf( buf, "You failed!.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	lose = 100 ;
	gain_exp( ch, 0 - lose );
	check_improve(ch,gsn_recall,TRUE,4);
	xprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );
*/	
    }

    ch->move /= 2;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
	do_look(ch,"auto");
    
    if (ch->pet != NULL)
      {
 	char_from_room( ch->pet );
	char_to_room( ch->pet, location );
	do_look(ch->pet,"auto");
      }

    return;
}


void do_train( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    sh_int stat = - 1;
    char *pOutput = NULL;
    int cost,swapgain;

    if ( IS_NPC(ch) )
	return;

    /*
     * Check for trainer.
     */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
	if ( IS_NPC(mob) && IS_SET(mob->act, ACT_TRAIN) )
	    break;
    }

    if ( mob == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return;
    }


    if ( argument[0] == '\0' )
    {
	xprintf( buf, "You have %d training sessions.\n\r", ch->train );
	send_to_char( buf, ch );
	argument = "foo";
    }

    cost = 1;

    if ( !str_cmp( argument, "str" ) )
    {
	if ( class_table[ch->class].attr_prime == STAT_STR )
	    cost    = 1;
	stat        = STAT_STR;
	pOutput     = "strength";
    }

    else if ( !str_cmp( argument, "int" ) )
    {
	if ( class_table[ch->class].attr_prime == STAT_INT )
	    cost    = 1;
	stat	    = STAT_INT;
	pOutput     = "intelligence";
    }

    else if ( !str_cmp( argument, "wis" ) )
    {
	if ( class_table[ch->class].attr_prime == STAT_WIS )
	    cost    = 1;
	stat	    = STAT_WIS;
	pOutput     = "wisdom";
    }

    else if ( !str_cmp( argument, "dex" ) )
    {
	if ( class_table[ch->class].attr_prime == STAT_DEX )
	    cost    = 1;
	stat  	    = STAT_DEX;
	pOutput     = "dexterity";
    }

    else if ( !str_cmp( argument, "con" ) )
    {
	if ( class_table[ch->class].attr_prime == STAT_CON )
	    cost    = 1;
	stat	    = STAT_CON;
	pOutput     = "constitution";
    }

    else if ( !str_cmp(argument, "hp" ) )
	cost = 1;

    else if ( !str_cmp(argument, "mana" ) )
	cost = 1;

  else if (!str_cmp("swap",argument))
    {

    //if IS_NPC(ch)
    send_to_char( "Kazmir did remove this function 17/10/2002.\n\r", ch );
     return;
  /*  
    if ( ch->pcdata->gainswap >= ch->level )
    {
	send_to_char( "You can't swap any more you level.\n\r", ch );
	return;
    }
*/
    if ( ch->pcdata->perm_mana <= 11 )
    {
	send_to_char( "You can't swap if don't have mana!!!.\n\r", ch );
	return;
    }
/*
   if (ch->class == CLASS_WAR)      // Rangers are better
       swapgain = 15;
    else if (ch->class == CLASS_PDN
	|| ch->class == CLASS_BKT
	|| ch->class == CLASS_THI
	|| ch->class == CLASS_MNK
	|| ch->class == CLASS_RAN
	|| ch->class == CLASS_ASN)
      swapgain = 10;
   else if (ch->class == CLASS_BRD
	|| ch->class == CLASS_INQ
	|| ch->class == CLASS_SPE)
      swapgain = 7;
  else
    swapgain = 5;
    */
    swapgain = (ch->perm_stat[STAT_CON]) - (ch->perm_stat[STAT_INT] / 2);
    
        ch->pcdata->perm_mana -= 10;
        ch->pcdata->gainswap += 1;
        ch->max_mana -= 10;
        ch->mana -= 10;
        ch->pcdata->perm_hit += swapgain;
        ch->max_hit += swapgain;
        ch->hit += swapgain;
        act( "Your power increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's power increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    else
    {
	strcpy( buf, "You can train:" );
	if ( ch->perm_stat[STAT_STR] < get_max_train(ch,STAT_STR)) 
	    strcat( buf, " str" );
	if ( ch->perm_stat[STAT_INT] < get_max_train(ch,STAT_INT))  
	    strcat( buf, " int" );
	if ( ch->perm_stat[STAT_WIS] < get_max_train(ch,STAT_WIS)) 
	    strcat( buf, " wis" );
	if ( ch->perm_stat[STAT_DEX] < get_max_train(ch,STAT_DEX))  
	    strcat( buf, " dex" );
	if ( ch->perm_stat[STAT_CON] < get_max_train(ch,STAT_CON))  
	    strcat( buf, " con" );
	strcat( buf, " hp mana");

	if ( buf[strlen(buf)-1] != ':' )
	{
	    strcat( buf, ".\n\r" );
	    send_to_char( buf, ch );
	}
	else
	{
	    /*
	     * This message dedicated to Jordan ... you big stud!
	     */
	    act( "You have nothing left to train, you $T!",
		ch, NULL,
		ch->sex == SEX_MALE   ? "big stud" :
		ch->sex == SEX_FEMALE ? "hot babe" :
					"wild thing",
		TO_CHAR );
	}

	return;
    }

    if (!str_cmp("hp",argument))
    {
    	if ( cost > ch->train )
    	{
       	    send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }
 
	ch->train -= cost;
        ch->pcdata->perm_hit += 10;
        ch->max_hit += 10;
        ch->hit +=10;
        act( "Your durability increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's durability increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }
 
    if (!str_cmp("mana",argument))
    {
        if ( cost > ch->train )
        {
            send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }

	ch->train -= cost;
        ch->pcdata->perm_mana += 10;
        ch->max_mana += 10;
        ch->mana += 10;
        act( "Your power increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's power increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }



    if ( ch->perm_stat[stat]  >= get_max_train(ch,stat) )
    {
        act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
        return;
    }

    if ( cost > ch->train )
    {
        send_to_char( "You don't have enough training sessions.\n\r", ch );
        return;
    }

    ch->train           -= cost;
  
    ch->perm_stat[stat]         += 1;
    act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
    return;
}



void do_track(CHAR_DATA *ch, char *argument)
{
  ROOM_HISTORY_DATA *rh;
  char buf[MAX_STRING_LENGTH];
  static char *door[] = { "north","east","south","west","up","down",
                            "that way" };

  if (!IS_NPC(ch) && get_skill(ch,gsn_track) < 2) {
    send_to_char("There are no train tracks here.\n\r",ch);
    return;
  }

  WAIT_STATE(ch,skill_table[gsn_track].beats);
  act("$n checks the ground for tracks.",ch,NULL,NULL,TO_ROOM);

  if (IS_NPC(ch) || number_percent() < get_skill(ch,gsn_track) )
    {
   if (IS_NPC(ch))
        ch->status = 0;
      for (rh = ch->in_room->history;rh != NULL;rh = rh->next)
        if (is_name(argument,rh->name)) {
          check_improve(ch,gsn_track,TRUE,1);
          xprintf(buf,"%s's tracks lead %s.\n\r",capitalize(rh->name),
                  door[URANGE(0,rh->went,7)]);
          send_to_char(buf, ch);
          move_char(ch,rh->went,FALSE);
          return;
      }
    }
  send_to_char("You don't see any tracks.\n\r",ch);
  if (IS_NPC(ch))
    ch->status = 5;
  check_improve(ch,gsn_track,FALSE,1);
}

/* Relock, Coded by Andeus in 3 mins, idea from CF */
void do_relock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Which door or container?\n\r", ch );
        return;
    }


    if ( !IS_NPC(ch) && number_percent( ) > get_skill(ch,gsn_relock))
    {
        send_to_char( "You failed.\n\r", ch);
        check_improve(ch,gsn_relock,FALSE,2);
        return;
    }



    if ( ( obj = get_obj_here( ch, NULL, arg ) ) != NULL )
    {
        /* portal stuff */
        if (obj->item_type == ITEM_PORTAL)
        {
            if (!IS_SET(obj->value[1],EX_ISDOOR)
            ||  IS_SET(obj->value[1],EX_NOCLOSE))
            {
                send_to_char("You can't do that.\n\r",ch);
                return;
            }
            if (!IS_SET(obj->value[1],EX_CLOSED))
            {
                send_to_char("It's not closed.\n\r",ch);
                return;
            }

            if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
            {
                send_to_char("It can't be relocked.\n\r",ch);
                return;
            }
/*
            if (!has_key(ch,obj->value[4]))
            {
                send_to_char("You lack the key.\n\r",ch);
                return;
            }
*/
            if (IS_SET(obj->value[1],EX_LOCKED))
            {
                send_to_char("It's already locked.\n\r",ch);
                return;
            }

            SET_BIT(obj->value[1],EX_LOCKED);
            act("You lock $p.",ch,obj,NULL,TO_CHAR);
            act("$n locks $p.",ch,obj,NULL,TO_ROOM);
            return;
        }

        /* 'lock object' */
        if ( obj->item_type != ITEM_CONTAINER )
            { send_to_char( "That's not a container.\n\r", ch ); return; }
        if ( !IS_SET(obj->value[1], CONT_CLOSED) )
            { send_to_char( "It's not closed.\n\r",        ch ); return; }
        if ( obj->value[2] < 0 )
            { send_to_char( "It can't be relocked.\n\r",     ch ); return; }
/*      if ( !has_key( ch, obj->value[2] ) )
            { send_to_char( "You lack the key.\n\r",       ch ); return; }*/
        if ( IS_SET(obj->value[1], CONT_LOCKED) )
            { send_to_char( "It's already locked.\n\r",    ch ); return; }

        SET_BIT(obj->value[1], CONT_LOCKED);
        act("You lock $p.",ch,obj,NULL,TO_CHAR);
        act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
        return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
        /* 'Relock door' */
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;

        pexit   = ch->in_room->exit[door];
        if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
            { send_to_char( "It's not closed.\n\r",        ch ); return; }
        if ( pexit->key < 0 )
            { send_to_char( "It can't be relocked.\n\r",     ch ); return; }
/*      if ( !has_key( ch, pexit->key) && 
             !has_key_ground( ch, pexit->key) )
            { send_to_char( "You lack the key.\n\r",       ch ); return; }*/
        if ( IS_SET(pexit->exit_info, EX_LOCKED) )
            { send_to_char( "It's already locked.\n\r",    ch ); return; }

        SET_BIT(pexit->exit_info, EX_LOCKED);
        send_to_char( "*Click*\n\r", ch );
        act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

        /* lock the other side */
        if ( ( to_room   = pexit->u1.to_room            ) != NULL
        &&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
        &&   pexit_rev->u1.to_room == ch->in_room )
        {
            SET_BIT( pexit_rev->exit_info, EX_LOCKED );
            for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
                act( "The $d clicks.", rch, NULL, pexit_rev->keyword, TO_CHAR );

        }
        return;
    }
    return;
}

int send_arrow( CHAR_DATA *ch, CHAR_DATA *victim,OBJ_DATA *arrow , int door, int chance ,int bonus) 
{
        EXIT_DATA *pExit;
        ROOM_INDEX_DATA *dest_room;
        char buf[512];
        AFFECT_DATA *paf;
        int damroll=0,hitroll=0,sn,Tdam;
    int dam;
        AFFECT_DATA af;

        if (arrow->value[0] == WEAPON_SPEAR)  sn = gsn_spear;
        else sn = gsn_arrow;
        
        for ( paf = arrow->affected; paf != NULL; paf = paf->next )
         {
           if ( paf->location == APPLY_DAMROLL )
              damroll += paf->modifier;
           if ( paf->location == APPLY_HITROLL )
              hitroll += paf->modifier;
         }

        dest_room = ch->in_room;
        chance += (hitroll + str_app[get_curr_stat(ch,STAT_STR)].tohit
                        + (get_curr_stat(ch,STAT_DEX) - 18)) * 2;
        damroll *= 10;
        while (1)
        {
         chance -= 5;
         if ( victim->in_room == dest_room ) 
         {
          if (number_percent() < chance)
          { 
           if ( check_obj_dodge(ch,victim,arrow,chance))
                return 0;
           act("$p strikes you!", victim, arrow, NULL, TO_CHAR );
           act("Your $p strikes $N!", ch, arrow, victim, TO_CHAR );
           if (ch->in_room == victim->in_room)
              act("$n's $p strikes $N!", ch, arrow, victim, TO_NOTVICT );
           else 
           {
             act("$n's $p strikes $N!", ch, arrow, victim, TO_ROOM );
             act("$p strikes $n!", victim, arrow, NULL, TO_ROOM );
           }
           if (is_safe(ch,victim) || 
                (IS_NPC(victim) && IS_SET(victim->act,ACT_NOTRACK)) )
            {
                act("$p falls from $n doing no visible damage...",victim,arrow,NULL,TO_ALL);
                act("$p falls from $n doing no visible damage...",ch,arrow,NULL,TO_CHAR);
                obj_to_room(arrow,victim->in_room);
            }
           else if (victim->race == race_lookup("orc"))
            {
                act("$p falls from $n doing no visible damage...",victim,arrow,NULL,TO_ALL);
                act("$p falls from $n doing no visible damage...",ch,arrow,NULL,TO_CHAR);
                obj_to_room(arrow,victim->in_room);
            }
           else
            {
                

                dam = dice(arrow->value[1],arrow->value[2]);
                dam += bonus + 
                        (10 * str_app[get_curr_stat(ch,STAT_STR)].todam);
		dam = number_range(dam*1.5,3.5*dam);
                if (IS_WEAPON_STAT(arrow,WEAPON_POISON))
                {
                 int level;
                 AFFECT_DATA *poison, af;

                 if ((poison = affect_find(arrow->affected,gsn_poison)) == NULL)
                        level = arrow->level;
                 else
                        level = poison->level;
                 if (!saves_spell(level,victim,DAM_POISON))
                 {
                  send_to_char("You feel poison coursing through your veins.",
                    victim);
                  act("$n is poisoned by the venom on $p.",
                        victim,arrow,NULL,TO_ROOM);

                  af.where     = TO_AFFECTS;
                  af.type      = gsn_poison;
                  af.level     = level * 3/4;
                  af.duration  = level / 2;
                  af.location  = APPLY_STR;
                  af.modifier  = -1;
                  af.bitvector = AFF_POISON;
                  affect_join( victim, &af );
                 }

                }
                if (IS_WEAPON_STAT(arrow,WEAPON_FLAMING))
                {
                 act("$n is burned by $p.",victim,arrow,NULL,TO_ROOM);
                 act("$p sears your flesh.",victim,arrow,NULL,TO_CHAR);
                 fire_effect( (void *) victim,arrow->level,dam,TARGET_CHAR);
                }
                if (IS_WEAPON_STAT(arrow,WEAPON_FROST))
                {
                    act("$p freezes $n.",victim,arrow,NULL,TO_ROOM);
                    act("The cold touch of $p surrounds you with ice.",
                        victim,arrow,NULL,TO_CHAR);
                    cold_effect(victim,arrow->level,dam,TARGET_CHAR);
                }
                if (IS_WEAPON_STAT(arrow,WEAPON_SHOCKING))
                {
                    act("$n is struck by lightning from $p.",victim,arrow,NULL,TO_ROOM);
                    act("You are shocked by $p.",victim,arrow,NULL,TO_CHAR);
                    shock_effect(victim,arrow->level,dam,TARGET_CHAR);
                }
                if (IS_WEAPON_STAT(arrow, WEAPON_MANADRAIN))
                {
            act("{C$n is being absorbed by $p.{x",victim,arrow,NULL,TO_ROOM);
            act("{C$p absorbs your magic.{x",victim,arrow,NULL,TO_CHAR);
            victim->mana -= dam/2;
        }
                Tdam = dam;
                if ( dam > victim->max_hit / 8
                        && number_percent() < 50 )
                {
                  af.where     = TO_AFFECTS;
                  af.type      = sn;
                  af.level     = ch->level; 
                  af.duration  = -1;
                  af.location  = APPLY_HITROLL;
                  af.modifier  = - (dam / 20);
                  if (IS_NPC(victim)) af.bitvector = 0;
                        else af.bitvector = AFF_CORRUPTION;

                  affect_join( victim, &af );

//                obj_to_char(arrow,victim);
//                equip_char(victim,arrow,WEAR_STUCK_IN);
                }
                else obj_to_room(arrow,victim->in_room); 

                damage( ch, victim,dam,sn,DAM_PIERCE,TRUE );
                path_to_track(ch,victim,door);

        }
            return 1;
          }
          else {
                  obj_to_room(arrow,victim->in_room);
                  act("$p sticks in the ground at your feet!",victim,arrow,NULL, TO_ALL );
                  return 0;
                }
         }
         pExit = dest_room->exit[ door ];
         if ( !pExit ) break;
         else 
         {
          dest_room = pExit->u1.to_room;
          if ( dest_room->people )
                {
                 xprintf(buf,"$p sails into the room from the %s!",dir_name[rev_dir[door]]);
                 act(buf, dest_room->people, arrow, NULL, TO_ALL );
                }
         }
        }
 return 0;
}

CHAR_DATA * find_char( CHAR_DATA *ch, char *argument,int door, int range )
{
 EXIT_DATA *pExit,*bExit;
 ROOM_INDEX_DATA *dest_room,*back_room;
 CHAR_DATA *target;
 int number = 0,opdoor;
 char arg[MAX_INPUT_LENGTH];

 number = number_argument(argument,arg);
 dest_room = ch->in_room;
 if ( (target = get_char_room2(ch,dest_room,arg,&number)) != NULL)
        return target;

 if ( (opdoor = opposite_door( door )) == -1)
  {
   bug("In find_char wrong door: %d",door);
   send_to_char("You don't see that there.\n\r",ch);
   return NULL;
 }
 while (range > 0)
 {
  range--;
  /* find target room */
  back_room = dest_room;
  if ( (pExit = dest_room->exit[door]) == NULL
      || (dest_room = pExit->u1.to_room) == NULL
      || IS_SET(pExit->exit_info,EX_CLOSED) )
 break;
  if ( (bExit = dest_room->exit[opdoor]) == NULL
      || bExit->u1.to_room != back_room)
   {
    send_to_char("The path you choose prevents your power to pass.\n\r",ch);
    return NULL;
   }
  if ((target = get_char_room2(ch,dest_room,arg,&number)) != NULL )
        return target;
 }

 send_to_char("You don't see that there.\n\r",ch);
 return NULL;
}

void do_shoot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *wield;
    OBJ_DATA *arrow; 
    char arg1[512],arg2[512],buf[512];
    bool success;
    int chance,direction;
    int range = (ch->level / 10) + 1;
    
   if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_bow].skill_level[ch->class] )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

   argument=one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "Shoot what direction and whom?\n\r", ch );
	return;
    }

    if (ch->fighting)
    {
	send_to_char("You cannot concentrate on shooting arrows.\n\r",ch);
	return;
    }

   direction = find_exit( ch, arg1 );

   if (direction<0 || direction > 5) 
	{
	 send_to_char("Shoot which direction and whom?\n\r",ch);
	 return;
	}
	
    if ( ( victim = find_char( ch, arg2, direction, range) ) == NULL )
	return;

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
	send_to_char("You can't do that.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	xprintf(buf,"Gods protect %s.\n\r",victim->name);
	send_to_char(buf,ch);
	return;
    }

   wield = get_eq_char(ch, WEAR_WIELD);
   arrow = get_eq_char(ch, WEAR_HOLD);    

   if (!wield || wield->item_type!=ITEM_WEAPON || wield->value[0]!=WEAPON_BOW)
    	{
	 send_to_char("You need a bow to shoot!\n\r",ch);
	 return;    	
	}

   if (get_eq_char(ch,WEAR_SECONDARY) || get_eq_char(ch,WEAR_SHIELD) )
    	{
	 send_to_char("Your second hand should be free!\n\r",ch);
	 return;    	
	}

   if (!arrow)
    	{
	 send_to_char("You need an arrow holding for your ammunition!\n\r",ch);
	 return;    	
	}	
	
    if (arrow->item_type!=ITEM_WEAPON || arrow->value[0] != WEAPON_ARROW) 
	{
	 send_to_char("That's not the right kind of arrow!\n\r",ch);
	 return;
	}
	
    
    WAIT_STATE( ch, skill_table[gsn_bow].beats );
   
    chance = (get_skill(ch,gsn_bow) - 50) * 2;
    if (ch->position == POS_SLEEPING)
	chance += 30;
    if (ch->position == POS_RESTING)
	chance += 10;
    if (victim->position == POS_FIGHTING)
	chance -= 40;
    chance += GET_HITROLL(ch)/ 7;

    xprintf( buf, "You shoot $p to %s.", dir_name[ direction ] );
    act( buf, ch, arrow, NULL, TO_CHAR );
    xprintf( buf, "$n shoots $p to %s.", dir_name[ direction ] );
    act( buf, ch, arrow, NULL, TO_ROOM );

    obj_from_char(arrow);
    success = send_arrow(ch,victim,arrow,direction,chance,
		dice(wield->value[1],wield->value[2]) );
    check_improve(ch,gsn_bow,TRUE,1);
}


void do_camp( CHAR_DATA *ch, char *argument )
{
  AFFECT_DATA af,af2;

  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_camp].skill_level[ch->class] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }
 if (is_affected(ch, gsn_camp))
    {
      send_to_char("You don't have enough power to handle more camp areas.\n\r",ch);
      return;
    }


  if ( number_percent( ) > get_skill( ch, gsn_camp) )
  {
        send_to_char( "You failed to make your camp.\n\r", ch );
        check_improve(ch,gsn_camp,TRUE,4);
        return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||

    IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  ||
         ( ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS ) )
  {
    send_to_char( "There are not enough leaves to camp here.\n\r", ch );
    return;
  }

  if ( ch->mana < 150 )
  {
     send_to_char( "You don't have enough mana to make a camp.\n\r", ch );
     return;
  }

  check_improve(ch,gsn_camp,TRUE,4);
 ch->mana -= 150;

  WAIT_STATE(ch,skill_table[gsn_camp].beats);

  send_to_char("You succeeded to make your camp.\n\r",ch);
  act("$n succeeded to make $s camp.",ch,NULL,NULL,TO_ROOM);

  af.where              = TO_AFFECTS;
  af.type               = gsn_camp;
  af.level              = ch->level;
  af.duration           = 12;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

  af2.where             = TO_ROOM_CONST;
 af2.type               = gsn_camp;
  af2.level              = ch->level;
  af2.duration           = ch->level / 20;
  af2.bitvector          = 0;
  af2.modifier           = 2 * ch->level;
  af2.location           = APPLY_ROOM_HEAL;
  affect_to_room(ch->in_room, &af2);

  af2.modifier           = ch->level;
  af2.location           = APPLY_ROOM_MANA;
  affect_to_room(ch->in_room, &af2);

}

void do_layhands(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_layhands].skill_level[ch->class] )
    {
        send_to_char("You lack the skill to heal others with touching.\n\r",ch);
        return;
    }

    if ( (victim = get_char_room( ch, NULL, argument)) == NULL)
      {
        send_to_char("You do not see that person here.\n\r",ch);
        return;
      }

 if ( is_affected(ch, gsn_layhands))
        {
         send_to_char("You can't concentrate enough.\n\r",ch);
         return;
        }

    WAIT_STATE(ch,skill_table[gsn_layhands].beats);

 if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_layhands])
  {
    af.type = gsn_layhands;
    af.where = TO_AFFECTS;
    af.level = ch->level;
//    af.duration = 21 - (ch->level/4);
    af.duration = 15;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = 0;
    affect_to_char (ch,&af );

    victim->hit += ch->max_hit/3;

//    victim->hit = UMIN( victim->hit + 100 , victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    check_improve(ch,gsn_layhands,TRUE,1);
	}
	else
	send_to_char("You fail miserablely.\n\r",ch);
}

int mount_success ( CHAR_DATA *ch, CHAR_DATA *mount, int canattack)
{
  int percent;
  int success;

  percent = number_percent() + (ch->level < mount->level ?
            (mount->level - ch->level) * 3 :
            (mount->level - ch->level) * 2);

  if (!ch->fighting)
  percent -= 25;

  if (!IS_NPC(ch) && IS_DRUNK(ch)) {
    percent += get_skill(ch,gsn_riding) / 2;
    send_to_char("Due to your being under the influence, riding seems a bit harder...\n\r",ch);
  }

  success = percent - get_skill(ch,gsn_riding);
  if( success <= 0 ) { /* Success */
    check_improve(ch, gsn_riding, TRUE, 1);
    return(1);
  } else {
    check_improve(ch, gsn_riding, FALSE, 1);
    if ( success >= 10 && MOUNTED(ch) == mount) {
      act("You lose control and fall off of $N.", ch, NULL, mount,TO_CHAR);
      act("$n loses control and falls off of $N.", ch, NULL, mount,TO_ROOM);
      act("$n loses control and falls off of you.", ch, NULL, mount,TO_VICT);

      ch->riding = FALSE;
      mount->riding = FALSE;
      if (ch->position > POS_STUNNED)
                ch->position=POS_SITTING;

    /*  if (ch->hit > 2) { */
        ch->hit -= 5;
        update_pos(ch);

    }
    if ( success >= 40 && canattack) {
      act("$N doesn't like the way you've been treating $M.", ch, NULL, mount,  TO_CHAR);
      act("$N doesn't like the way $n has been treating $M.", ch, NULL, mount,  TO_ROOM);
      act("You don't like the way $n has been treating you.", ch, NULL, mount, TO_VICT);

      act("$N snarls and attacks you!", ch, NULL, mount, TO_CHAR);

      act("$N snarls and attacks $n!", ch, NULL, mount, TO_ROOM);
      act("You snarl and attack $n!", ch, NULL, mount, TO_VICT);

      damage(mount, ch, number_range(1, mount->level),gsn_kick,DAM_BASH,TRUE );

/*      multi_hit( mount, ch, TYPE_UNDEFINED ); */
    }
  }
  return(0);
}

/*
 * It is not finished yet to implement all.
 */
void do_mount( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];

 char buf[MAX_STRING_LENGTH];
  struct char_data *mount;

  argument = one_argument(argument, arg);

  if (IS_NPC(ch)) return;

  if (arg[0] == '\0' && ch->mount && ch->mount->in_room == ch->in_room)
  {
    mount = ch->mount;
  }
  else if (!(mount = get_char_room( ch, NULL,  arg)))
 {
    send_to_char("Mount what?\n\r", ch);
    return;
  }
 if(get_skill(ch,gsn_riding) < 1)
  {
    send_to_char("You don't know how to ride!\n\r", ch);
    return;
  }
/*
  if (!IS_NPC(mount) || strcmp(race_table[RACE(mount)].name,"dragon")
       || IS_SET(mount->act,ACT_NOTRACK) )
  {
    send_to_char("You can't ride that.\n\r",ch);
    return;
  }
*/
 if ( !IS_NPC( mount ) ||
         !IS_SET( mount->act, ACT_MOUNTABLE ) || IS_IMMORTAL(mount) )
    {
        send_to_char( "You can't ride that.\n\r", ch );
        return;
    }

  if (mount->level - 5 > ch->level) {
    send_to_char("That beast is too powerful for you to ride.", ch);
    return;
  }
 if( (mount->mount) && (!mount->riding) && (mount->mount != ch)) {
    xprintf(buf, "%s belongs to %s, not you.\n\r", mount->short_descr,
                   mount->mount->name);
    send_to_char(buf,ch);
    return;
  }

  if (mount->position < POS_STANDING) {
    send_to_char("Your mount must be standing.\n\r", ch);
    return;
  }

  if (RIDDEN(mount)) {
    send_to_char("This beast is already ridden.\n\r", ch);
    return;
  } else if (MOUNTED(ch)) {
   send_to_char("You are already riding.\n\r", ch);
    return;
  }

  if( !mount_success(ch, mount, TRUE) ) {
    send_to_char("You fail to mount the beast.\n\r", ch);
    return;
  }

  act("You hop on $N's back.", ch, NULL, mount, TO_CHAR);
  act("$n hops on $N's back.", ch, NULL, mount, TO_NOTVICT);
  act("$n hops on your back!", ch, NULL, mount, TO_VICT);

  ch->mount = mount;
  ch->riding = TRUE;
  mount->mount = ch;
  mount->riding = TRUE;

  /* No sneaky people on mounts */
  affect_strip(ch, gsn_sneak);
  REMOVE_BIT(ch->affected_by, AFF_SNEAK);
  affect_strip(ch, gsn_hide);
  REMOVE_BIT(ch->affected_by, AFF_HIDE);
  affect_strip(ch, gsn_fade);
  REMOVE_BIT(ch->affected_by, AFF_FADE);
}

void do_dismount( CHAR_DATA *ch, char *argument )
{
//Dismounting you pet	
	struct char_data *mount;

	//Are we mountes
	if(MOUNTED(ch))
	  {
		//Doing the dismount
		mount = MOUNTED(ch);

		act("You dismount from $N.", ch, NULL, mount, TO_CHAR);
		act("$n dismounts from $N.", ch, NULL, mount, TO_NOTVICT);
		act("$n dismounts from you.", ch, NULL, mount, TO_VICT);

		ch->riding = FALSE;
		mount->riding = FALSE;
	  }
	//Nope not mounted  
	else
	  {
		send_to_char("You aren't mounted.\n\r", ch);
		return;
	  }
return;
}

void do_push_drag( CHAR_DATA *ch, char *argument, char *verb )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    CHAR_DATA *victim;
    EXIT_DATA *pexit;
    OBJ_DATA *obj;
//    OBJ_DATA *portal;
    int door;



 char * const rev_name [] =
    {
        "the south", "the west", "the north", "the east", "below", "above"
    };

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    victim = get_char_room( ch, NULL, arg1);
    obj = get_obj_list( ch, arg1, ch->in_room->contents );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        xprintf( buf, "%s whom or what where?\n\r", capitalize(verb));
        send_to_char( buf, ch );
        return;
    }

  if (MOUNTED(ch))
    {
	xprintf(buf, "You can't %s while mounted.\n\r",capitalize(verb));
        send_to_char( buf, ch );
        return;
    }
    if (RIDDEN(ch))
    {
       xprintf(buf,"You can't %s while being ridden.\n\r",capitalize(verb));
        send_to_char( buf, ch );
        return;
    }

    if ( (!victim || !can_see(ch,victim))
    && (!obj || !can_see_obj(ch,obj)) )
    {
        xprintf(buf,"%s whom or what where?\n\r", capitalize(verb));
        send_to_char( buf, ch );
        return;
    }

         if ( !str_cmp( arg2, "n" ) || !str_cmp( arg2, "north" ) ) door =0;
    else if ( !str_cmp( arg2, "e" ) || !str_cmp( arg2, "east"  ) ) door =1;
    else if ( !str_cmp( arg2, "s" ) || !str_cmp( arg2, "south" ) ) door =2;
    else if ( !str_cmp( arg2, "w" ) || !str_cmp( arg2, "west"  ) ) door =3;
    else if ( !str_cmp( arg2, "u" ) || !str_cmp( arg2, "up"    ) ) door =4;
    else if ( !str_cmp( arg2, "d" ) || !str_cmp( arg2, "down"  ) ) door =5;
//    else
//    {
//      portal = get_obj_list( ch, arg2,  ch->in_room->contents );
 //     if (obj != NULL)
  //    if (portal != NULL)
  //     {
//
//    	if ( obj->weight >  (2 * can_carry_w (ch)) && (obj->item_type != ITEM_CORPSE_PC))
 //   	  {
   //   		act( "$p is too heavy to $T.\n\r", ch, obj, verb, TO_CHAR);
     // 		act( "$n attempts to $T $p, but it is too heavy.\n\r", ch, obj,verb, TO_ROOM);
      	//	return;
   // 	  }
    //    obj_from_room(obj);
    //    do_enter(ch,arg2);
//	obj_to_room(obj,ch->in_room);
  //      return;
   //    }
      else
       {
      	xprintf( buf, "Alas, you cannot %s in that direction.\n\r", verb );
      	send_to_char( buf, ch );
      	return;
      }	
 //   }

  if ( obj )
  {
    in_room = obj->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||   !can_see_room(ch,pexit->u1.to_room))
    {
        xprintf( buf, "Alas, you cannot %s in that direction.\n\r", verb);
        send_to_char( buf, ch );
        return;
    }

    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    || IS_SET(pexit->exit_info,EX_NOPASS) )
    {
        act( "You cannot $t it through the $d.", ch, verb, pexit->keyword,TO_CHAR );
        act( "$n decides to $t $P around!", ch, verb, obj, TO_ROOM );
        return;
    }

    act( "You attempt to $T $p out of the room.", ch, obj, verb, TO_CHAR);
    act( "$n is attempting to $T $p out of the room.", ch, obj, verb,TO_ROOM );

    if ( obj->weight >  (2 * can_carry_w (ch)) && (obj->item_type != ITEM_CORPSE_PC))
    {
      act( "$p is too heavy to $T.\n\r", ch, obj, verb, TO_CHAR);
      act( "$n attempts to $T $p, but it is too heavy.\n\r", ch, obj,verb, TO_ROOM);
      return;
    }
    if   (( !IS_IMMORTAL(ch)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY) )
    && (obj->item_type != ITEM_CORPSE_PC))
    {
        send_to_char( "It won't budge.\n\r", ch );
        return;
    }

    if ( ch->move > 10 )
    {
        ch->move -= 10;
        WAIT_STATE(ch,PULSE_VIOLENCE/4);
        send_to_char( "You succeed!\n\r", ch );
        act( "$n succeeds!", ch, NULL, NULL, TO_ROOM );
        if (!str_cmp( verb, "drag" ))
        {
        act( "$n drags $p $T!", ch, obj, dir_name[door], TO_ROOM );
        char_from_room( ch );
        char_to_room( ch, pexit->u1.to_room );
	do_look(ch,"auto");
        obj_from_room( obj );
        obj_to_room( obj, to_room );
        act( "$n drags $p into the room.", ch, obj, dir_name[door],TO_ROOM );
        }
        else if (!str_cmp( verb, "push" ))
        {
        act( "$p {Wflies{x $T!", ch, obj, dir_name[door], TO_ROOM );
        act( "$p {Wflies{x $T!", ch, obj, dir_name[door], TO_CHAR );
        char_from_room( ch );
        char_to_room( ch, pexit->u1.to_room );
        act( "You notice movement from nearby to the $T.",
                ch, NULL, rev_name[door], TO_ROOM );
        act( "$p {Wflies{x into the room!", ch, obj, dir_name[door], TO_ROOM );
        char_from_room( ch );
        char_to_room( ch, in_room );
        obj_from_room( obj );
        obj_to_room( obj, to_room );
        }
    }
    else
    {
      xprintf( buf, "You are too tired to %s anything around!\n\r", verb);
      send_to_char( buf, ch );
    }
  }
  else
  {
    if ( ch == victim )
    {
        act( "You $t yourself about the room and look very silly.", ch,verb, NULL, TO_CHAR );
        act( "$n decides to be silly and $t $mself about the room.", ch,verb, NULL, TO_ROOM );
        return;
    }

	if (victim == NULL )
		return;
    in_room = victim->in_room;

    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||   !can_see_room(victim,pexit->u1.to_room))
    {
        xprintf( buf, "Alas, you cannot %s them that way.\n\r", verb );
        send_to_char( buf, ch );
        return;
    }
    if(IS_AFFECTED2(ch,AFF2_WEBS))
	{
        send_to_char( "You're webbed, and want to do WHAT?!?\n\r", ch );
	xprintf(buf,"$n stupidly tries to %s $N while webbed.",
		capitalize(verb));
        act(buf, ch, NULL,victim,TO_ROOM );
        return;
    }
    
    if(IS_AFFECTED2(ch,AFF2_STONESHAPE))
	{
        send_to_char( "You can't do that while in stoneform.\n\r", ch );
        return;
    }
    
    
    if(IS_AFFECTED2(victim,AFF2_PARALYZE))
    {
	xprintf(buf,"You attempt to %s $N, but an unseen force hold $m in place.",
		 capitalize(verb));
        act( buf, ch, NULL, victim, TO_CHAR );
	xprintf(buf,"$n attempts to %s $n, but fails as an unseen force hold $n in place.", capitalize(verb));
     act( buf,ch, NULL, victim, TO_ROOM );
        return;
    }

    if(IS_AFFECTED2(victim,AFF2_WEBS))
    {
	xprintf(buf,"You attempt to %s $N, but the webs hold $m in place.",
		 capitalize(verb));
        act( buf, ch, NULL, victim, TO_CHAR );
	xprintf(buf,"$n attempts to %s $n, but fails as the webs hold $n in place.", capitalize(verb));
     act( buf,ch, NULL, victim, TO_ROOM );
        return;
    }
     
 if(IS_AFFECTED2(ch,AFF2_HOLD))
	{
        send_to_char( "You're esped, and want to do WHAT?!?\n\r", ch );
	xprintf(buf,"$n stupidly tries to %s $N while esped.",
		capitalize(verb));
        act(buf, ch, NULL,victim,TO_ROOM );
        return;
    }

    if(IS_AFFECTED2(victim,AFF2_HOLD))
    {
	xprintf(buf,"You attempt to %s $N, but $N is frozen.",
		 capitalize(verb));
        act( buf, ch, NULL, victim, TO_CHAR );
	xprintf(buf,"$n attempts to %s $n, but fails.", capitalize(verb));
     act( buf,ch, NULL, victim, TO_ROOM );
        return;
    }

 if (IS_NPC(victim))
        if(victim->pIndexData->vnum == 110 ||
           victim->pIndexData->vnum == 120 ||
           victim->pIndexData->vnum == 130 ||
           victim->pIndexData->vnum == 140 ||
           victim->pIndexData->vnum == 150 ||
           victim->pIndexData->vnum == 160)
        send_to_char("You can't seem to move the guardian.\n",ch);

    if (IS_SET(pexit->exit_info, EX_CLOSED)
    &&  (!IS_AFFECTED(victim, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS)))
    {
        act( "You try to $t them through the $d.", ch, verb,pexit->keyword, TO_CHAR );
        act( "$n decides to $t you around!", ch, verb, victim, TO_VICT );
        act( "$n decides to $t $N around!", ch, verb, victim, TO_NOTVICT);
        return;
    }

    act( "You attempt to $t $N out of the room.", ch, verb, victim,TO_CHAR );
    act( "$n is attempting to $t you out of the room!", ch, verb, victim,TO_VICT );
    act( "$n is attempting to $t $N out of the room.", ch, verb, victim,TO_NOTVICT );

    if   ( !IS_IMMORTAL(ch)
    ||   (IS_NPC(victim)
    &&   (IS_SET(victim->act,ACT_TRAIN)
    ||   IS_SET(victim->act,ACT_PRACTICE)
    ||   IS_SET(victim->act,ACT_IS_HEALER)
    ||   IS_SET(victim->act,ACT_IS_CHANGER)
    ||   IS_SET(victim->imm_flags,IMM_SUMMON)
    ||   victim->pIndexData->pShop ))
    ||   victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   (!str_cmp( verb, "push" ) && victim->position != POS_STANDING)
    ||   is_safe(ch,victim)
    ||   (number_percent() > 50))
//    ||   (victim->max_hit > (ch->max_hit + (get_curr_stat(ch,STAT_STR)*20))) )
    {
        send_to_char( "They won't budge.\n\r", ch );
        return;
    }

    if ( ch->move > 10 )
    {
        ch->move -= 10;
        send_to_char( "You succeed!\n\r", ch );
        act( "$n succeeds!", ch, NULL, NULL, TO_ROOM );
        if (!str_cmp( verb, "drag" ))
        {
        move_char( ch, door, FALSE );
        act( "$n is dragged $T!", victim, NULL, dir_name[door], TO_ROOM );
        act( "You are dragged $T!\n\r", victim, NULL, dir_name[door], TO_CHAR );
        char_from_room( victim );
        char_to_room( victim, pexit->u1.to_room );
	do_look(victim,"auto");
        act( "$N drags $n into the room.", victim, NULL, ch, TO_NOTVICT );
        }
        else if (!str_cmp( verb, "push" ))
        {
        act( "$n {Wflies{x $T!", victim, NULL, dir_name[door], TO_ROOM );
        act( "You {Wfly{x $T!\n\r", victim, NULL, dir_name[door], TO_CHAR);
        char_from_room( victim );
        char_to_room( victim, pexit->u1.to_room );
	do_look(victim,"auto");
        act( "You notice movement from nearby to the $T.",
                victim, NULL, rev_name[door], TO_ROOM );
        act( "$n {Wflies{x into the room!", victim, NULL, NULL, TO_ROOM );
        }
    }
    else
    {
      xprintf( buf, "You are too tired to %s anybody around!\n\r", verb );
      send_to_char( buf, ch );
    }
  }

  return;
}
               
void do_push( CHAR_DATA *ch, char *argument )
{
    do_push_drag( ch, argument, "push" );
    return;
}

void do_drag( CHAR_DATA *ch, char *argument )
{
    do_push_drag( ch, argument, "drag" );
    return;
}

void do_arise(CHAR_DATA *ch, char *argument)
{
   OBJ_DATA    *obj;
   int i;
   char       arg [ MAX_INPUT_LENGTH ];
   char       buf  [ MAX_STRING_LENGTH ];
   
   argument = one_argument( argument, arg );
    
         
    if(ch->in_room != get_room_index(ROOM_VNUM_AFTERLIFE))
    {
    	send_to_char( "You are not dead.\n\r", ch );
    	return;
    }
    if (IS_GOOD(ch))
          i = 0;
    else if (IS_EVIL(ch))
          i = 2;
    else
          i = 1;
          
    if(!str_cmp(arg, "rp") )
     {
      if (ch->rptoken > ch->level/5)
      {
      ch->rptoken -= ch->level/5;
      act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
      char_from_room( ch );
      char_to_room(ch, get_room_index(hometown_table[ch->hometown].altar[i]));
      act( "$n returns from the dead.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You are brought back to life.\n\r", ch );
      xprintf( buf,"You have been deducted %d rptokens.\n\r",ch->level/5);
      send_to_char(buf, ch);
      do_look(ch,"auto");
      }
      else
      send_to_char( "You can't afford to get revived with rptokens.\n\r", ch );
      return;
     }
   if(!str_cmp(arg, "glory") )
     {
      if (ch->quest_curr > ch->level*4)
      {
      ch->quest_curr -= ch->level*4;
      act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
      char_from_room( ch );
      char_to_room(ch, get_room_index(hometown_table[ch->hometown].altar[i]));
      act( "$n returns from the dead.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You are brought back to life.\n\r", ch );
      xprintf( buf,"You have been deducted %d glorypointst.\n\r",ch->level*4);
      send_to_char(buf, ch);
      do_look(ch,"auto");
      }
      else
      send_to_char( "You haven't accumulated enough glorypoints to do this.\n\r", ch );
      return;
     }
   if(!str_cmp(arg, "gold") )
     {
      if (ch->pcdata->bank_g > ch->level*250)
      {
      ch->pcdata->bank_g -= ch->level*250;
      act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
      char_from_room( ch );
      char_to_room(ch, get_room_index(hometown_table[ch->hometown].altar[i]));
      act( "$n returns from the dead.", ch, NULL, NULL, TO_ROOM );
      send_to_char( "You are brought back to life.\n\r", ch );
      xprintf( buf,"%d gold has been subtracted from your account.\n\r",ch->level*250);
      send_to_char(buf, ch);
      do_look(ch,"auto");
      }
      else
      send_to_char( "You do not have enough gold in the bank to get raised this way.\n\r", ch );
      return;
     }
    
    if ( ( obj = get_obj_world( ch, ch->name ) ) == NULL )
    {          
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room(ch, get_room_index(hometown_table[ch->hometown].altar[i]));
    act( "$n returns from the dead.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You are brought back to life.\n\r", ch );
    do_look(ch,"auto");
    return;
    }
    else
    {
	do_owhere(ch, ch->name );
	return;
    }
  
}

