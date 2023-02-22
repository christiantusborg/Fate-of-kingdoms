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
#include "arena.h"
#include "fok_struct.h"

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

if (IS_AFFECTED2(ch,AFF2_HOLD))
    {
        send_to_char( "You are esped.\n\r", ch );
        act( "$n struggles vainly but cannot seem to move.", ch, NULL, NULL, TO_ROOM );
        return;
    }
    
if (IS_AFFECTED2(ch,AFF2_PARALYZE))
    {
        send_to_char( "You are completely paralyzed and cannot move.\n\r", ch );
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

    /*
     * Exit trigger, if activated, bail out. Only PCs are triggered.
     */
//    if ( !IS_NPC(ch) && mp_exit_trigger( ch, door ) )
//    if ( !IS_NPC(ch) && p_exit_trigger( ch, door, PRG_MPROG ) )
  //     return;
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
	    	if ( TRUE
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
  	&&    !IS_AFFECTED(ch,AFF_FLYING))
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

    if ( !IS_AFFECTED(ch, AFF_SNEAK) && (!is_affected(ch,skill_lookup("wraithform")))
    &&   ch->invis_level < LEVEL_HERO)
	{
        if (!IS_NPC(ch) && ch->in_room->sector_type != SECT_INSIDE &&
            ch->in_room->sector_type != SECT_CITY &&
            number_percent() < ch->pcdata->learned[gsn_quiet_movement])
          {
  	    if (MOUNTED(ch))
                sprintf(buf,"$n leaves, riding on %s.",
                        MOUNTED(ch)->short_descr );
            else sprintf(buf,"$n leaves.");
            check_improve(ch,gsn_quiet_movement,TRUE,1);

          }
	else 
  	{
           if (MOUNTED(ch))
                sprintf(buf,"$n leaves $T, riding on %s.",
                        MOUNTED(ch)->short_descr );
           else sprintf(buf,"$n leaves $T." );
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
//       mp_percent_trigger( ch, NULL, NULL, NULL, TRIG_ENTRY );
         p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_ENTRY );
//    if ( !IS_NPC( ch ) )
//       mp_greet_trigger( ch );

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
    move_char( ch, DIR_NORTH, FALSE );
    return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_EAST, FALSE );
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_SOUTH, FALSE );
    return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_WEST, FALSE );
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_UP, FALSE );
    return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
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



//    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    if ( ( obj = get_obj_here( ch,NULL, arg ) ) != NULL )
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



//    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    if ( ( obj = get_obj_here( ch,NULL, arg ) ) != NULL )
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


//    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    if ( ( obj = get_obj_here( ch,NULL, arg ) ) != NULL )
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


//    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    if ( ( obj = get_obj_here( ch,NULL, arg ) ) != NULL )
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
    else obj = ch->on;



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
    else obj = ch->on;

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

//    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
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

/*
    if (IS_SET(ch->affected_by, AFF_FADE))
      {
	send_to_char("You step out of the shadows.\n\r",ch);
	REMOVE_BIT(ch->affected_by, AFF_FADE);
	act("$n steps out of the shadows.", ch, NULL, NULL, TO_ROOM);
      }    */
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
	    sprintf( buf, "You failed!.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	lose = 100 ;
	gain_exp( ch, 0 - lose );
	check_improve(ch,gsn_recall,TRUE,4);
	sprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
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
send_to_char("Fok4 don't use this",ch);
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
                 sprintf(buf,"$p sails into the room from the %s!",dir_name[rev_dir[door]]);
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
//  else if (!(mount = get_char_room(ch, arg)))
  else if (!(mount = get_char_room(ch,NULL, arg)))
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
    sprintf(buf, "%s belongs to %s, not you.\n\r", mount->short_descr,
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
  struct char_data *mount;

  if(MOUNTED(ch)) {
   mount = MOUNTED(ch);

    act("You dismount from $N.", ch, NULL, mount, TO_CHAR);
    act("$n dismounts from $N.", ch, NULL, mount, TO_NOTVICT);
    act("$n dismounts from you.", ch, NULL, mount, TO_VICT);

    ch->riding = FALSE;
    mount->riding = FALSE;
  }
  else
  {
    send_to_char("You aren't mounted.\n\r", ch);
    return;
  }
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
    int door;

 char * const rev_name [] =
    {
        "the south", "the west", "the north", "the east", "below", "above"
    };

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
//    victim = get_char_room(ch,arg1);
    victim = get_char_room(ch,NULL,arg1);
    obj = get_obj_list( ch, arg1, ch->in_room->contents );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        sprintf( buf, "%s whom or what where?\n\r", capitalize(verb));
        send_to_char( buf, ch );
        return;
    }

  if (MOUNTED(ch))
    {
	sprintf(buf, "You can't %s while mounted.\n\r",capitalize(verb));
        send_to_char( buf, ch );
        return;
    }
    if (RIDDEN(ch))
    {
       sprintf(buf,"You can't %s while being ridden.\n\r",capitalize(verb));
        send_to_char( buf, ch );
        return;
    }

    if ( (!victim || !can_see(ch,victim))
    && (!obj || !can_see_obj(ch,obj)) )
    {
        sprintf(buf,"%s whom or what where?\n\r", capitalize(verb));
        send_to_char( buf, ch );
        return;
    }

         if ( !str_cmp( arg2, "n" ) || !str_cmp( arg2, "north" ) ) door =0;
    else if ( !str_cmp( arg2, "e" ) || !str_cmp( arg2, "east"  ) ) door =1;
    else if ( !str_cmp( arg2, "s" ) || !str_cmp( arg2, "south" ) ) door =2;
    else if ( !str_cmp( arg2, "w" ) || !str_cmp( arg2, "west"  ) ) door =3;
    else if ( !str_cmp( arg2, "u" ) || !str_cmp( arg2, "up"    ) ) door =4;
    else if ( !str_cmp( arg2, "d" ) || !str_cmp( arg2, "down"  ) ) door =5;
    else
    {
      sprintf( buf, "Alas, you cannot %s in that direction.\n\r", verb );
      send_to_char( buf, ch );
      return;
    }

  if ( obj )
  {
    in_room = obj->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||   !can_see_room(ch,pexit->u1.to_room))
    {
        sprintf( buf, "Alas, you cannot %s in that direction.\n\r", verb);
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

    if ( obj->weight >  (2 * can_carry_w (ch)) )
    {
      act( "$p is too heavy to $T.\n\r", ch, obj, verb, TO_CHAR);
      act( "$n attempts to $T $p, but it is too heavy.\n\r", ch, obj,verb, TO_ROOM);
      return;
    }
    if   ( !IS_IMMORTAL(ch)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SOLITARY) )
    {
        send_to_char( "It won't budge.\n\r", ch );
        return;
    }

    if ( ch->move > 10 )
    {
        ch->move -= 10;
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
      sprintf( buf, "You are too tired to %s anything around!\n\r", verb);
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

    in_room = victim->in_room;

    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||   !can_see_room(victim,pexit->u1.to_room))
    {
        sprintf( buf, "Alas, you cannot %s them that way.\n\r", verb );
        send_to_char( buf, ch );
        return;
    }
    if(IS_AFFECTED2(ch,AFF2_WEBS))
	{
        send_to_char( "You're webbed, and want to do WHAT?!?\n\r", ch );
	sprintf(buf,"$n stupidly tries to %s $N while webbed.",
		capitalize(verb));
        act(buf, ch, NULL,victim,TO_ROOM );
        return;
    }

    if(IS_AFFECTED2(victim,AFF2_WEBS))
    {
	sprintf(buf,"You attempt to %s $N, but the webs hold $m in place.",
		 capitalize(verb));
        act( buf, ch, NULL, victim, TO_CHAR );
	sprintf(buf,"$n attempts to %s $n, but fails as the webs hold $n in place.", capitalize(verb));
     act( buf,ch, NULL, victim, TO_ROOM );
        return;
    }

 if(IS_AFFECTED2(ch,AFF2_HOLD))
	{
        send_to_char( "You're esped, and want to do WHAT?!?\n\r", ch );
	sprintf(buf,"$n stupidly tries to %s $N while esped.",
		capitalize(verb));
        act(buf, ch, NULL,victim,TO_ROOM );
        return;
    }

    if(IS_AFFECTED2(victim,AFF2_HOLD))
    {
	sprintf(buf,"You attempt to %s $N, but $N is frozen.",
		 capitalize(verb));
        act( buf, ch, NULL, victim, TO_CHAR );
	sprintf(buf,"$n attempts to %s $n, but fails.", capitalize(verb));
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
      sprintf( buf, "You are too tired to %s anybody around!\n\r", verb );
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

void do_war(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;

  if (iswar != TRUE) 
  {
    send_to_char("There is no arena going on!\n\r", ch);
    return;
  }

  if (ch->level < min_level || ch->level > max_level)
  {
    send_to_char("Sorry, you can't join this arena battle.\n\r", ch);
    return;
  }
  if (inmiddle == TRUE)
  {
    send_to_char("The ARENA already started!!\n\r",ch);
    return;
  }
  if (ch->pking > 0)
  {
    send_to_char("No way!! you are escaping from your pk!!",ch);
    return;
  }

    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   ch->in_room == ch->master->in_room )
    {
	send_to_char( "{CWhat?  And leave your beloved master?{x\n\r",ch);
	return;
    }

  if (IS_SET(ch->act, PLR_ARENA))
  {
    send_to_char("I don't think so.\n\r", ch);
    return;
  }

  if(IS_NPC(ch)) {
	send_to_char("Sorry Player can only play.\n\r",ch);
	return;
  }

  if(!IS_NPC(ch)) {
  ch->pcdata->room_number = (ch->in_room == NULL ? 3001 : ch->in_room->vnum);
  ch->pcdata->arenahp = ch->hit;
  ch->pcdata->arenamp = ch->mana;
  ch->pcdata->arenamove = ch->move;
  }
  ch->hit = ch->max_hit;
  ch->mana = ch->max_mana;
  ch->move = ch->max_move;

  if (wartype == 1)
  {
    if ((location = get_room_index(ARENA_WAR_ROOM)) == NULL)
    {
      send_to_char("Arena is not yet completed, sorry.\n\r", ch);
      return;
    }
    else
    {
      act("$n goes to the battlefield of war!", ch, NULL, NULL,TO_ROOM); 
      char_from_room(ch);
      char_to_room(ch, location);
      SET_BIT(ch->act, PLR_ARENA);
      sprintf(buf, "{R[{YARENA{R] {c%s joins the blood bath!",ch->name);
      info(buf,NULL,NULL,INFO_ARENA,0,0);
      act("$n arrives into the battlefields!", ch, NULL, NULL,TO_ROOM);
      inwar++;
	do_look(ch,"auto");
      return;
    }
  }
  if (wartype == 2)
  {
	send_to_char("Team Arena is not yet completed!",ch);
	return;
	}
}

void do_startwar(CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
  char arg3[MAX_INPUT_LENGTH];
  DESCRIPTOR_DATA *d;

  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);
  argument = one_argument(argument, arg3);
  if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0')
  {
    send_to_char("Syntax: startarena <type> <min_level> <max_level>\n\r",ch);
    return;
  }

  if (atoi(arg1) < 1 || atoi(arg1) > 2)
  {
    send_to_char("The type either has to be 1, or 2.\n\r", ch);
    return;
  }

  if (atoi(arg2) <= 0 || atoi(arg2) > 83)
  {
    send_to_char("Level must be between 1 and 83.\n\r", ch);
    return;
  }

  if (atoi(arg3) <= 0 || atoi(arg3) > 83)
  {
    send_to_char("Level must be between 1 and 83.\n\r", ch);
    return;
  }

  if (atoi(arg3) < atoi(arg2))
  {
    send_to_char("Max level must be greater than the min level.\n\r", ch);
    return;
  }

  if (arena_counter > 0)
  {
    sprintf(buf,"{WArena janitor tells you 'Give me %d minutes to clean up.'{x\n\r",
	arena_counter);
    send_to_char(buf,ch);
    return;
  }

  if (iswar == TRUE)
  {
    send_to_char("There is already a arena going!\n\r", ch);
    return;
  }

  iswar = TRUE;
  inwar = 0;
  wartype = atoi(arg1);
  min_level = atoi(arg2);
  max_level = atoi(arg3);
  sprintf(buf, "{R[{YARENA{R] %s type arena for levels {Y%d {Rto {Y%d{R.\n\rType 'arena' to join!\n", wartype == 1 ? "Single" : "Team", min_level, max_level);
      info(buf,NULL,NULL,INFO_ARENA,0,0);
  wartimeleft = 3;

  for (d = descriptor_list; d != NULL; d = d->next)
  {
	CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;

        if ( d->connected == CON_PLAYING && !IS_NPC(vch))
      	if (IS_SET(vch->act, PLR_ARENA))
        REMOVE_BIT(vch->act, PLR_ARENA);
  }
}

void do_arenaout(CHAR_DATA *ch, char *argument)
{
//  ROOM_INDEX_DATA *location;

	if(iswar == TRUE)
	{
	send_to_char("There is an arena going on!\n\r",ch);
	return;
	}
	 if (inmiddle == TRUE)
	  {
    send_to_char("The ARENA already started!!\n\r",ch);
    return;
	  }

if ( !strstr( ch->in_room->area->builders, "Arena" ) ) {
	send_to_char("You are not in the Arena.\n\r",ch);
	return;
	}

    if(!IS_NPC(ch)) 
     {
      if(ch->pcdata->arenahp != 0)
       ch->hit = ch->pcdata->arenahp;
      else
       ch->hit = ch->max_hit;
    
      if(ch->pcdata->arenamp != 0)
        ch->mana = ch->pcdata->arenamp;
      else
        ch->mana = ch->max_mana;

      if(ch->pcdata->arenamove !=0)
       ch->move = ch->pcdata->arenamove;
      else
        ch->move = ch->max_move;


       ch->last_fight_time = -1;
       ch->pking = 0;
       update_pos( ch );
     
      if ( ch->pcdata->room_number == 0 )
         char_to_room(ch, (get_room_index(ROOM_VNUM_ARENA_SAFE))); 
	
	  else
         char_to_room(ch,get_room_index(ch->pcdata->room_number));
      do_look(ch,"auto");
        stop_fighting( ch, TRUE );
     }      
     
/*
  if ((location = get_room_index(ROOM_VNUM_ALTAR)) == NULL)
    {
      send_to_char("Please report to Takeda about this bug.\n\r", ch);
      return;
    }
    else
    {
      char_from_room(ch);

*/


//      char_to_room(ch, location);
//	send_to_char("You been transfer out by the janitor.\n\r",ch);
//	return;
//    }
}

void war_update(void)
{
  char buf[MAX_STRING_LENGTH];
  DESCRIPTOR_DATA *d;
  ROOM_INDEX_DATA *random;
/*  int x;
  CHAR_DATA *ch; */
  
  if (wartimeleft > 0)
  {
    sprintf(buf, "{R[{YARENA{R] %d tick%s left to join the war.", wartimeleft,wartimeleft == 1 ? "" : "s");
      info(buf,NULL,NULL,INFO_ARENA,0,0);
    sprintf(buf, "{R[{YARENA{R] %d %s %s fighting in the war, so far.", inwar, inwar ==1 ? "person" : "people", inwar == 1 ? "is" : "are");
      info(buf,NULL,NULL,INFO_ARENA,0,0);
    sprintf(buf, "{R[{YARENA{R] Type of war: {Y%d {R- {Y%d{R, {C%s war{R.{w", min_level,max_level, wartype == 1 ? "Single" : "Team");
      info(buf,NULL,NULL,INFO_ARENA,0,0);
    wartimeleft--;
    return;
  }

  if (wartimeleft == 0 && iswar == TRUE && wartimer == 0)
  {  
    if (inwar == 0 || inwar == 1)
    {
      sprintf(buf, "{R[{YARENA{R] Not enough people for war.  War reset.");
      info(buf,NULL,NULL,INFO_ARENA,0,0);
      iswar = FALSE;
      inmiddle = FALSE;
      inwar = 0;
      wartimeleft = 0;
      wartimer = 0;
      min_level = 0;
      max_level = 0;
      wartype = 0;
      for(d = descriptor_list; d != NULL; d = d->next)
      {
	   CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
	 if ( d->connected == CON_PLAYING && !IS_NPC(vch))
        if (IS_SET(vch->act, PLR_ARENA))
        {
     /*Arena Fix Zardic*/
   if(!IS_NPC(vch)) 
   {
    if(vch->pcdata->arenahp != 0)
     vch->hit = vch->pcdata->arenahp;
    else
     vch->hit = vch->max_hit;
   
    if(vch->pcdata->arenamp != 0)
     vch->mana = vch->pcdata->arenamp;
    else
     vch->mana = vch->max_mana;
   
    if(vch->pcdata->arenamove !=0)
     vch->move = vch->pcdata->arenamove;
    else
     vch->move = vch->max_move;
   }
/* 
   else
    vch->hit = vch->max_hit;
    vch->mana = vch->max_mana
*/
          char_from_room(vch);
    if(!IS_NPC(vch)) {
     if ( vch->pcdata->room_number == 0 )
     char_to_room(vch, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
        else
     char_to_room(vch,get_room_index(vch->pcdata->room_number));
        } else
     char_to_room(vch, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
        do_look(vch,"auto");
        }
      }
    }
    else
    {

      wartimer = 20;
      inmiddle = TRUE;
      inwar = 0;
      for(d = descriptor_list; d != NULL; d = d->next)
      {
	   CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
	 if ( d->connected == CON_PLAYING && !IS_NPC(vch))
        if (IS_SET(vch->act, PLR_ARENA))
        {
	  inwar += 1;
	  random = get_room_index(number_range(MIN_ARENA_ROOM,MAX_ARENA_ROOM));
          char_from_room(vch);
          char_to_room(vch, random);
	do_look(vch,"auto");
        }
      }
      sprintf(buf, "{R[{YARENA{R] The battle begins! %d players are fighting!", inwar);
      info(buf,NULL,NULL,INFO_ARENA,0,0);
    }
  }
  return;
}

/****************************************************************************
				Freeze Tag
****************************************************************************/

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

//    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg2 ) ) == NULL )
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
//   if ( ( victim = get_char_room( ch, arg ) ) == NULL )
   if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
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


