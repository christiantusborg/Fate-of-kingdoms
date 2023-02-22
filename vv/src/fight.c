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
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "magic.h"
#include "arena.h"
#include "fok_struct.h"
#include "declaration.h"

#define MAX_DAMAGE_MESSAGE 45

DECLARE_DO_FUN(	do_quit_count	);
DECLARE_DO_FUN( do_yell		);
DECLARE_DO_FUN( do_emote	);
DECLARE_DO_FUN( do_zap		);
DECLARE_DO_FUN( do_eat		);
DECLARE_DO_FUN( do_bash		);
DECLARE_DO_FUN( do_disarm	);
DECLARE_DO_FUN( do_kick		);
DECLARE_DO_FUN( do_dirt		);
DECLARE_DO_FUN( do_trip		);
DECLARE_DO_FUN( do_look		);
DECLARE_DO_FUN( do_get		);
DECLARE_DO_FUN( do_sacrifice	);
DECLARE_DO_FUN( do_recall	);
DECLARE_DO_FUN( do_flee		);
DECLARE_DO_FUN( do_recite	);
DECLARE_DO_FUN( do_brandish	);
/* command procedures needed */
DECLARE_SPELL_FUN(spell_cancellation );
DECLARE_SPELL_FUN(	spell_blindness	);
DECLARE_SPELL_FUN(	spell_poison	);

extern void do_visible( CHAR_DATA *ch, char *argument );

/*
 * Local functions.
 */
void	check_assist	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	check_dodge	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	check_killer	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	check_parry	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	check_block	args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
bool	check_lightning_reflexes args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
void	dam_message	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
			    int dt, bool immune ) );
void	death_cry	args( ( CHAR_DATA *ch ) );
void	death_cry_org	args( ( CHAR_DATA *ch, int part) );
void	group_gain	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int	xp_compute	args( ( CHAR_DATA *gch, CHAR_DATA *victim,
			    int total_levels,int members ) );
bool	is_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	make_corpse	args( ( CHAR_DATA *ch ) );
/*  changes for dual wield */
void    one_hit     args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool secondary ) );
void    mob_hit		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void	raw_kill_org	args( ( CHAR_DATA *victim, int part ) );
void	set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	disarm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
/*  adds use_magical_item for mo */
void    use_magical_item     args( ( CHAR_DATA *ch ) );
void    damage_to_object        args( (CHAR_DATA *ch, OBJ_DATA *wield,OBJ_DATA *worn, int damage) );
int     critical_strike args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam ) );
void	check_weapon_destroy	args( (CHAR_DATA *ch, CHAR_DATA *victim, bool second) );
void do_fleealways args( ( CHAR_DATA *ch, char *argument ) );

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */

void do_autofreeze( CHAR_DATA *ch )
{
//    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( !IS_SET(ch->act, PLR_FREEZE) )
    {
	SET_BIT(ch->act, PLR_FREEZE);
	send_to_char("Your fucked, you may have cheated and is frozen.\n\r",ch);	
	sprintf(buf,"[%s] is fucked, he may have cheated and is frozen",ch->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    log_string( buf );
    save_char_obj( ch );
    ch->last_fight_time = 0;
    stop_fighting(ch,TRUE);
    sprintf(buf,"%s 1",ch->name);
    do_transfer(ch, buf );
    send_to_char("All you do now is quit.\n\r",ch);	

    return;
}



void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
     OBJ_DATA *obj_next;
     bool room_trig = FALSE;    
    OBJ_DATA *obj;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next	= ch->next;

	if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
	    continue;


	if ( IS_AWAKE(ch) && ch->in_room == victim->in_room )
	    multi_hit( ch, victim, TYPE_UNDEFINED );
	else
	    stop_fighting( ch, FALSE );

	if ( ( victim = ch->fighting ) == NULL )
	    continue;

	if (!IS_NPC(victim))
	  ch->last_fought = victim;
	ch->last_fight_time = current_time;


	for (obj = ch->carrying;obj != NULL; obj = obj->next_content)
	  {
	    if (IS_SET(obj->progtypes,IPROG_FIGHT))
	      (obj->pIndexData->iprogs->fight_prog) (obj,ch);
	  }

	if (IS_SET(ch->progtypes,MPROG_FIGHT) && (ch->wait <= 0))
	  (ch->pIndexData->moprogs->fight_prog) (ch,victim);


	/*
	 * Fun for the whole family!
	 */
	check_assist(ch,victim);
       if ( IS_NPC( ch ) )
       {
           if ( HAS_TRIGGER_MOB( ch, TRIG_FIGHT ) )
                p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_FIGHT );
//               mp_percent_trigger( ch, victim, NULL, NULL, TRIG_FIGHT );
           if ( HAS_TRIGGER_MOB( ch, TRIG_HPCNT ) )
//               mp_hprct_trigger( ch, victim );
                 p_hprct_trigger( ch, victim );

       }
	for ( obj = ch->carrying; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( obj->wear_loc != WEAR_NONE && HAS_TRIGGER_OBJ( obj, TRIG_FIGHT ) )
		p_percent_trigger( NULL, obj, NULL, victim, NULL, NULL, TRIG_FIGHT );
	}

	if ( HAS_TRIGGER_ROOM( ch->in_room, TRIG_FIGHT ) && room_trig == FALSE )
	{
	    room_trig = TRUE;
	    p_percent_trigger( NULL, NULL, ch->in_room, victim, NULL, NULL, TRIG_FIGHT );
	}
    }
    

    return;
}

/* for auto assisting */
void check_assist(CHAR_DATA *ch,CHAR_DATA *victim)
{
    CHAR_DATA *rch, *rch_next;

    for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
    {
	rch_next = rch->next_in_room;

	if (IS_AWAKE(rch) && rch->fighting == NULL)
	{

	    /* quick check for ASSIST_PLAYER */
	    if (!IS_NPC(ch) && IS_NPC(rch)
	    && IS_SET(rch->off_flags,ASSIST_PLAYERS)
	    && (rch->level + 6 > victim->level)
	    && can_see(rch,ch))
	    {
		do_emote(rch,"screams and attacks!");
		multi_hit(rch,victim,TYPE_UNDEFINED);
		continue;
	    }

	    /* quick check for ASSIST_CLAN */
	    if (!IS_NPC(ch) && IS_NPC(rch)
	    && IS_SET(rch->off_flags,ASSIST_CLAN)
	    &&  (((rch->pIndexData->vnum-100)/10) != victim->kingdom)
	    && can_see(rch,ch))
	    {
		do_emote(rch,"raises an enormous arm and attacks!");
		multi_hit(rch,victim,TYPE_UNDEFINED);
		continue;
	    }


	    /* PCs next */
	    if (!IS_NPC(rch) || IS_AFFECTED(rch,AFF_CHARM))
	    {
		if (  ((!IS_NPC(rch) && IS_SET(rch->act,PLR_AUTOASSIST)
		&& (rch->desc ))
		||     IS_AFFECTED(rch,AFF_CHARM))
		&&   is_same_group(ch,rch) 
		&& (ch->desc )
		&& can_see(rch,ch))
		    multi_hit (rch,victim,TYPE_UNDEFINED);

		continue;
	    }

	    /* now check the NPC cases */

 	    if (IS_NPC(ch))

	    {
		if ( (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALL))

		||   (IS_NPC(rch) && rch->race == ch->race
		   && IS_SET(rch->off_flags,ASSIST_RACE))

		||   (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALIGN)
		   &&   ((IS_GOOD(rch)    && IS_GOOD(ch))
		     ||  (IS_EVIL(rch)    && IS_EVIL(ch))
		     ||  (IS_NEUTRAL(rch) && IS_NEUTRAL(ch))))

		||   (rch->pIndexData == ch->pIndexData
		   && IS_SET(rch->off_flags,ASSIST_VNUM)) )
	   	{

		    CHAR_DATA *vch;
		    CHAR_DATA *target;
		    int number;

		if(!can_see(rch,ch))
			return;

		    if (number_bits(1) == 0)
			continue;

		    target = NULL;
		    number = 0;
		    for (vch = ch->in_room->people; vch; vch = vch->next)
		    {
			if (can_see(rch,vch)
			&&  is_same_group(vch,victim)
			&&  number_range(0,number) == 0)
			{
			    target = vch;
			    number++;
			}
		    }

		    if (target != NULL)
		    {
		do_emote(rch,"screams and attacks!");
			multi_hit(rch,target,TYPE_UNDEFINED);
		    }
		}
	    }
	}
    }
}


/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int     chance;

    /* decrement the wait */
    if (ch->desc == NULL)
	ch->wait = UMAX(0,ch->wait - PULSE_VIOLENCE);


    /* no attacks for stunnies -- just a check */
    if (ch->position < POS_RESTING)
	return;
    if (victim->lagstand > 0)
     --victim->lagstand;

      if (victim->lagflee > 0)
       --victim->lagflee;
     
      if (victim->lagspell > 0)
       --victim->lagspell;
     
      if (victim->lagattack > 0)
       --victim->lagattack;

    
    /* no attacks on ghosts or attacks by ghosts */
    if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST)) ||
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_GHOST)))
	return;

  if (IS_AFFECTED(ch,AFF_STUN) )
        {
 act_new("You are stunned to response $N's attack.",
                ch,NULL,victim,TO_CHAR,POS_FIGHTING);
 act_new("$n is stunned to response your attack.",
                ch,NULL,victim,TO_VICT,POS_FIGHTING);
 act_new("$n seems stunned to response $N's attack.",
                ch,NULL,victim,TO_NOTVICT,POS_FIGHTING);
/*         affect_strip(ch,gsn_power_stun);
         SET_BIT(ch->affected_by,AFF_WEAK_STUN); */
         return;
        }

    if (IS_NPC(ch))
    {
	mob_hit(ch,victim,dt);
	return;
    }

    one_hit( ch, victim, dt ,FALSE);

/*  adds dual */


    if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
    {
    chance = get_skill(ch,gsn_dual_wield);
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, TRUE );
	check_improve(ch,gsn_dual_wield,TRUE,3);
	if ( ch->fighting != victim )
	    return;
    }
    }

    /*don't you get faster with both hands?*/

   if (IS_AFFECTED(ch,AFF_HASTE))
    if ((dt != gsn_backstab) && (ch->level >= 50) )
     if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
      {
       chance = get_skill(ch,gsn_dual_wield);
       if ( number_percent( ) < chance )
        {
	 one_hit( ch, victim, dt, TRUE );
	 check_improve(ch,gsn_dual_wield,TRUE,3);
	
	if ( ch->fighting != victim )
	 return;
       }
     }


    if (ch->fighting != victim)
	return;

    if (IS_AFFECTED(ch,AFF_HASTE))
	one_hit(ch,victim,dt,FALSE);

    if ( ch->fighting != victim || dt == gsn_backstab || dt == gsn_cleave
	|| dt == gsn_ambush || dt == gsn_dual_backstab || dt == gsn_circle
	|| dt == gsn_assassinate)
	return;


   if ((get_eq_char (ch, WEAR_SECONDARY) != NULL) 
    && (get_skill(ch,gsn_dual_wield) < 1 
    && (ch->kingdom == KINGDOM_ARKADIA) ) )
    {
    chance = get_skill(ch,gsn_battle_prowess);
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, TRUE );
//	check_improve(ch,gsn_dual_wield,TRUE,3);
	if ( ch->fighting != victim )
	    return;
    }
    }

   /*don't monk have two hands faster with both hands?*/

   /*don't monk have two hands?*/
    if ( (ch->class == CLASS_MNK) && (get_eq_char (ch, WEAR_HOLD) == NULL) && (get_eq_char (ch, WEAR_SECONDARY) == NULL) && (ch->level > 30))
     {
      one_hit( ch, victim, dt ,FALSE);
     }


   if (IS_AFFECTED(ch,AFF_HASTE))
    if ( (ch->class == CLASS_MNK) && (get_eq_char (ch, WEAR_SECONDARY) == NULL) && (ch->level > 50))
     {
      one_hit( ch, victim, dt ,FALSE);
     }
/*  end */



    chance = get_skill(ch,gsn_extra_attack);

    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_extra_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return; 
    }
    
    chance = get_skill(ch,gsn_blade_dance);
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_blade_dance,TRUE,5);
	if ( ch->fighting != victim )
	    return;	    
    }
  /* Monk should have more chance to hit more by Takeda 3-3-00        */
        if (ch->class == CLASS_MNK)
    chance = get_skill(ch,gsn_second_attack);
        else  
    chance = get_skill(ch,gsn_second_attack)/2;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_second_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return;
    }
  /* Monk should have more chance to hit more by Takeda 3-3-00        */
        if (ch->class == CLASS_MNK)
    chance = get_skill(ch,gsn_third_attack)/2;
        else     
 chance = get_skill(ch,gsn_third_attack)/4;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt,FALSE );
	check_improve(ch,gsn_third_attack,TRUE,6);
	if ( ch->fighting != victim )
	    return;
    }
     /* Monk should have more chance to hit more by Takeda 3-3-00 	*/
	if (ch->class == CLASS_MNK) 
    chance = get_skill(ch,gsn_fourth_attack)/4;
	else
    chance = get_skill(ch,gsn_fourth_attack)/8;
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt ,FALSE);
	check_improve(ch,gsn_fourth_attack,TRUE,7);
	if ( ch->fighting != victim )
	    return;
    }

     /* Monk should have more chance to hit more by Takeda 3-3-00 	*/
	if (ch->class == CLASS_MNK) 
    chance = get_skill(ch,gsn_fifth_attack)/5;
	else
    chance = get_skill(ch,gsn_fifth_attack)/10;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt ,FALSE);
        check_improve(ch,gsn_fifth_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }


    /* Added sixth attack by Takeda 3-3-00 				*/
    /* Monk should have more chance to hit more by Takeda 3-3-00 	*/
	if (ch->class == CLASS_MNK) 
    chance = get_skill(ch,gsn_sixth_attack)/6;
	else
    chance = get_skill(ch,gsn_sixth_attack)/12;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt ,FALSE);
        check_improve(ch,gsn_sixth_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }

   if (ch->class == CLASS_MNK) 
    chance = get_skill(ch,gsn_7_attack)/6;
	else
    chance = get_skill(ch,gsn_7_attack)/12;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt ,FALSE);
        check_improve(ch,gsn_7_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }

   if (ch->class == CLASS_MNK) 
    chance = get_skill(ch,gsn_8_attack)/6;
	else
    chance = get_skill(ch,gsn_8_attack)/12;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt ,FALSE);
        check_improve(ch,gsn_8_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }

   if (ch->class == CLASS_MNK) 
    chance = get_skill(ch,gsn_9_attack)/6;
	else
    chance = get_skill(ch,gsn_9_attack)/12;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt ,FALSE);
        check_improve(ch,gsn_9_attack,TRUE,8);
        if ( ch->fighting != victim )
            return;
    }
 return;
}

/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    int chance,number;
    CHAR_DATA *vch, *vch_next;

    /* no attacks on ghosts, just to make sure :) */
    if (!IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST))
	return;

    one_hit(ch,victim,dt,FALSE);
/*  adds dual */
    if (get_eq_char (ch, WEAR_SECONDARY))
    {
    chance = get_skill(ch,gsn_dual_wield);
    if ( number_percent( ) < chance )
    {
	one_hit( ch, victim, dt, TRUE );
	check_improve(ch,gsn_dual_wield,TRUE,3);
	if ( ch->fighting != victim )
	    return;
    }
    }
/*  end */

    if (ch->fighting != victim)
	return;


    /* Area attack -- BALLS nasty! */

    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
	    vch_next = vch->next;
	    if ((vch != victim && vch->fighting == ch))
		one_hit(ch,vch,dt,FALSE);
	}
    }

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_SET(ch->off_flags,OFF_FAST))
	one_hit(ch,victim,dt,FALSE);

    if (ch->fighting != victim || dt == gsn_backstab || dt == gsn_circle ||
	dt == gsn_dual_backstab || dt == gsn_cleave || dt == gsn_ambush)
	return;

    chance = get_skill(ch,gsn_extra_attack)/2;
    if (number_percent() < chance)
    {
	one_hit(ch,victim,dt,FALSE);
	if (ch->fighting != victim)
	    return;
    }

    chance = get_skill(ch,gsn_second_attack)/2;
    if (number_percent() < chance)
    {
	one_hit(ch,victim,dt,FALSE);
	if (ch->fighting != victim)
	    return;
    }

    chance = get_skill(ch,gsn_third_attack)/4;
    if (number_percent() < chance)
    {
	one_hit(ch,victim,dt,FALSE);
	if (ch->fighting != victim)
	    return;
    }
    /* oh boy!  Fun stuff! */

    if (ch->wait > 0)
	return;

    number = number_range(0,2);

/*
    if (number == 1 && IS_SET(ch->act,ACT_MAGE))
      { mob_cast_mage(ch,victim); return; }


    if (number == 2 && IS_SET(ch->act,ACT_CLERIC))
      { mob_cast_cleric(ch,victim); return; }
 */
    /* now for the skills */

    number = number_range(0,7);

    switch(number)
    {
    case (0) :
	if (IS_SET(ch->off_flags,OFF_BASH))
	do_bash(ch,"");
	break;

    case (1) :
	if (IS_SET(ch->off_flags,OFF_BERSERK) && !IS_AFFECTED(ch,AFF_BERSERK))
	do_berserk(ch,"");
	break;


    case (2) :
	if (IS_SET(ch->off_flags,OFF_DISARM)
	|| (get_weapon_sn(ch) != gsn_hand_to_hand
	&& (IS_SET(ch->act,ACT_WARRIOR)
   	||  IS_SET(ch->act,ACT_THIEF))))
	do_disarm(ch,"");
	break;

    case (3) :
	if (IS_SET(ch->off_flags,OFF_KICK))
	do_kick(ch,"");
	break;

    case (4) :
	if (IS_SET(ch->off_flags,OFF_KICK_DIRT))
	do_dirt(ch,"");
	break;

    case (5) :
	if (IS_SET(ch->off_flags,OFF_TAIL))
	    one_hit( ch, victim, dt ,FALSE);
	/*  do_tail(ch,""); */
	break;

    case (6) :
	if (IS_SET(ch->off_flags,OFF_TRIP))
	do_trip(ch,"");
	break;
/*
    case (7) :
	if (IS_SET(ch->off_flags,OFF_CRUSH))
	do_crush(ch,"");
	break;
 */
    }
}

/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool secondary )
{
    OBJ_DATA *wield;
    int victim_ac;
    int thac0;
    int thac0_00;
    int thac0_32;
    int dam;
    //inti;
    int diceroll;
    int sn,skill;
    int dam_type;
    bool counter;
    bool yell;
    bool result;
    OBJ_DATA *corpse;
    char buf[MAX_STRING_LENGTH];
    sn = -1;
    counter = FALSE;




    if (victim->fighting == ch)
      yell = FALSE;
    else yell = TRUE;

    /* just in case */
    if (victim == ch || ch == NULL || victim == NULL)
	return;

    /* ghosts can't fight */
    if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST)) ||
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_GHOST)))
	return;


     if(is_affected(ch,skill_lookup("divine deflection aura")))
	{
         send_to_char("{Your divine deflection prevent you from attcking!!{x\n\r",ch);
	 return;
	}    
    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
	return;

    /*
     * Figure out the type of damage message.
     * if secondary == true, use the second weapon.
     */
    if (!secondary)
        wield = get_eq_char( ch, WEAR_WIELD );
    else
        wield = get_eq_char( ch, WEAR_SECONDARY );
/*  replaces one line with 4 above
 *  wield = get_eq_char( ch, WEAR_WIELD );
 */
 /*Kazmir ADD 15/03/00 to make hand wear to monk class*/
/*    if ((ch->class == CLASS_MNK) && ( get_eq_char( ch, WEAR_HANDS ) != NULL ))
     {
       mnkdam = get_eq_char( ch, WEAR_HANDS );
      if (mnkdam->item_type == ITEM_WEAPON)
       wield = get_eq_char( ch, WEAR_HANDS );
     }
*/
    if ( dt == TYPE_UNDEFINED )
    {
	dt = TYPE_HIT;
	if ( wield != NULL && wield->item_type == ITEM_WEAPON )
	    dt += wield->value[3];
	else
	    dt += ch->dam_type;
    }

    if (dt < TYPE_HIT)
    	if (wield != NULL)
    	    dam_type = attack_table[wield->value[3]].damage;
    	else
    	    dam_type = attack_table[ch->dam_type].damage;
    else
    	dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
	dam_type = DAM_BASH;

    /* get the weapon skill */
    sn = get_weapon_sn(ch);
    skill = 20 + get_weapon_skill(ch,sn);

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
    if ( IS_NPC(ch) )
    {
	thac0_00 = 20;
	thac0_32 = -4;   /* as good as a thief */
	if (IS_SET(ch->act,ACT_WARRIOR))
	    thac0_32 = -10;
	else if (IS_SET(ch->act,ACT_THIEF))
	    thac0_32 = -4;
	else if (IS_SET(ch->act,ACT_CLERIC))
	    thac0_32 = 2;
	else if (IS_SET(ch->act,ACT_MAGE))
	    thac0_32 = 6;
    }
    else
    {
	thac0_00 = class_table[ch->class].thac0;
	thac0_32 = class_table[ch->class].thac0;
    }

    thac0  = interpolate( ch->level, thac0_00, thac0_32 );

    if (thac0 < 0)
        thac0 = thac0/2;

    if (thac0 < -5)
        thac0 = -5 + (thac0 + 5) / 2;
    
// if (ch->class == CLASS_MNK)
//  {  
//   thac0 -= GET_HITROLL(ch) * skill/50;
//   thac0 -= ch->level;
//  }
//  else
//    thac0 -= GET_HITROLL(ch) /30; //ALARK
    thac0 += 5 * (100 - skill) / 100;
    thac0 -= UMAX((sqrt(GET_HITROLL(ch))-(ch->level/20)),0);
    if (dt == gsn_backstab)
	thac0 -= 10 * (100 - get_skill(ch,gsn_backstab));

    if (dt == gsn_dual_backstab)
        thac0 -= 10 * (100 - get_skill(ch,gsn_dual_backstab));

    if (dt == gsn_cleave)
        thac0 -= 10 * (100 - get_skill(ch,gsn_cleave));

    if (dt == gsn_ambush)
        thac0 -= 10 * (100 - get_skill(ch,gsn_ambush));


    switch(dam_type)
    {
	case(DAM_PIERCE):victim_ac = GET_AC(victim,AC_PIERCE)/10;	break;
	case(DAM_BASH):	 victim_ac = GET_AC(victim,AC_BASH)/10;		break;
	case(DAM_SLASH): victim_ac = GET_AC(victim,AC_SLASH)/10;	break;
	default:	 victim_ac = GET_AC(victim,AC_EXOTIC)/10;	break;
    };

//    if (victim_ac < -15)
//	victim_ac = (victim_ac + 15) / 5 - 15;
//victim_ac = (victim_ac-10) * 15 / 50 + 10;
victim_ac = (((victim_ac-10)*20)/43)+10;//modifed by alark

//    sprintf(buf, "%d %d %d %d\n\r", thac0_00, thac0_32, thac0, victim_ac);
//    send_to_char (buf, ch);

    if ( !can_see( ch, victim ) )
	victim_ac -= 4;

    if ( victim->position < POS_FIGHTING)
	victim_ac += 4;

    if (victim->position < POS_RESTING)
	victim_ac += 6;

    /*
     * The moment of excitement!
     */
    while ( ( diceroll = number_bits( 5 ) ) >= 20 )
	;

    if ( diceroll == 0
    || ( diceroll != 19 && diceroll < thac0 - victim_ac ) )
    {
	/* Miss. */
	damage( ch, victim, 0, dt, dam_type, TRUE );
	tail_chain( );
	return;
    }

    /*
     * Hit.
     * Calc damage.
     */


    if ( IS_NPC(ch) && (!ch->pIndexData->new_format || wield == NULL))
	if (!ch->pIndexData->new_format)
	{
	    dam = number_range( ch->level / 2, ch->level * 3 / 2 );
	    if ( wield != NULL )
	    	dam += dam / 2;
	}
	else
	    dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);

    else
    {
	if (sn != -1)
	    check_improve(ch,sn,TRUE,5);
	if ( wield != NULL )
	{
	    if (wield->pIndexData->new_format)
		dam = dice(wield->value[1],wield->value[2]) * skill/100;
	    else
	    	dam = number_range( wield->value[1] * skill/100,
				wield->value[2] * skill/100);

	    if (get_eq_char(ch,WEAR_SHIELD) == NULL)  /* no shield = more */
		dam = dam * 42/40; 

             if (IS_WEAPON_STAT(wield,WEAPON_TWO_HANDS))
              dam += number_range (GET_DAMROLL(ch) /3, (GET_DAMROLL(ch)));

            /* sharpness! */
          
            if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
            {
                int percent;

                if ((percent = number_percent()) <= (skill / 8))
                    dam = 2 * dam + (dam * 2 * percent / 100);
            }
	}
	else
	    dam = number_range( 1 + 4 * skill/100, 2 * ch->level/3 * skill/100);
    }

    /*
     * Bonuses.
     */
    if ( get_skill(ch,gsn_enhanced_damage) > 0 )
    {
        diceroll = number_percent();
        if (diceroll <= get_skill(ch,gsn_enhanced_damage))
        {
            check_improve(ch,gsn_enhanced_damage,TRUE,6);
            dam += dam * diceroll/100;
        }
    }
/*Kazmir str do make more dam*/
  if (!IS_NPC(ch)) 
   {
    dam += dam + ((ch->level/10) *
str_app[get_curr_stat(ch,STAT_STR)].todam);
   }
    if ( !IS_AWAKE(victim) )
	dam *= 2;
     else if (victim->position < POS_FIGHTING)
	dam = dam * 3 / 2;

/*monk*/
   if (ch->class == CLASS_MNK) 
     dam += ((get_skill(ch,gsn_hand_to_hand)* 0.8) / (101 - ch->level));
  
  if (victim->fighting == NULL && !IS_NPC(victim) &&
	 !is_safe_nomessage(victim, ch) && !is_safe_nomessage(ch,victim) &&
	(victim->position == POS_SITTING || victim->position == POS_STANDING)
	&& dt != gsn_assassinate &&
	((number_percent()+(dt==gsn_backstab)?40:0) <
	(victim->pcdata->learned[gsn_counter]/2)))

      {
	counter = TRUE;
	check_improve(victim,gsn_counter,TRUE,3);
	act("{C$N counters, turning your attack against you!{x",ch,NULL,victim,TO_CHAR);
	act("{CYou counter $n's attack, turning it against $m!{x",ch,NULL,victim,TO_VICT);
	act("{C$N counters $n's attack, turning it against $m!{x",ch,NULL,victim,TO_NOTVICT);
	ch->fighting = victim;
      }
    else check_improve(victim,gsn_counter,FALSE,1);

    if ( dt == gsn_backstab && wield != NULL)
      dam = (ch->level/50 + 1) * dam + (ch->level * 3);

    else if ( dt == gsn_dual_backstab && wield != NULL)
      dam = (ch->level/50 + 1) * dam + (ch->level * 3);

    else if (dt == gsn_circle)
      dam = (ch->level/50 + 2) * dam + (ch->level * 3);

    else if ( dt == gsn_cleave && wield != NULL)
      {
	if (number_percent() <= 2 && !counter &&((ch->level-10)>victim->level))
	  {
	    act("{RYou swing with full force and cleave $N IN HALF!{x",
		      ch,NULL,victim,TO_CHAR);
	    act("{R$n's swings with full force, chopping you IN HALF!{x",
		      ch,NULL,victim,TO_VICT);
	    act("{R$n's swings with full force, chopping $N IN HALF!{x",
		      ch,NULL,victim,TO_NOTVICT);

    if ( !IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
    {
	    send_to_char("{rYou have been KILLED!!{x\n\r",victim);
	    act("{r$n is DEAD!{x",victim,NULL,NULL,TO_ROOM);
    }
    else
    {
	    send_to_char("{rYou lost the fight!!{x\n\r",victim);
	    send_to_char("{rYou wake up with a headache some time later.{x\n\r",victim);
	    act("{r$n lost the fight!{x",victim,NULL,NULL,TO_ROOM);
	    act("{r$n's mangled body is dragged away by the groundskeeper.{x",victim,NULL,NULL,TO_ROOM);
    }
	    WAIT_STATE( ch, 2 );
	 if (IS_SET(victim->in_room->room_flags, ROOM_ARENA))

if (IS_SET(victim->act, PLR_ARENA) && iswar == TRUE && inmiddle == TRUE)
{
  REMOVE_BIT(victim->act, PLR_ARENA);

  if(!IS_NPC(victim))
  inwar--;

  if (inwar == 1)
 {
/*        if(!IS_NPC(ch)) {
         ch->pcdata->awins += 1; } */
  sprintf(buf, "{R[{YARENA{R] {c%s {Ris victorious in the arena!{w",ch->name);
  info(buf,NULL,NULL,INFO_ARENA,0,0);         
//    while ( victim->affected )
//    affect_remove( victim, victim->affected );
//    victim->affected_by = race_table[victim->race].aff;
//    victim->affected_by2 = race_table[victim->race].aff2;
//    victim->detection = race_table[victim->race].detect;
	    stop_fighting(victim,TRUE);
            spell_cure_corruption(skill_lookup("corruption"),150,ch,ch,0);
	    affect_strip(ch,gsn_plague);
            affect_strip(ch,gsn_poison);
            affect_strip(ch,gsn_blindness);
            affect_strip(ch,gsn_sleep);
            affect_strip(ch,gsn_curse);
            affect_strip(ch,skill_lookup("ice ray"));
            affect_strip(ch,skill_lookup("divine word"));
            affect_strip(ch,skill_lookup("faerie fog")); 
            check_dispel( 150, ch,skill_lookup("deafen"));
            check_dispel( 150, ch,skill_lookup("insanity"));
            check_dispel( 150, ch,skill_lookup("corruption"));
	 spell_cancellation(0, 150, ch, ch, 0); 
    if(!IS_NPC(ch)) {
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
    } else {
    ch->hit = ch->max_hit;
    ch->mana = ch->max_mana;
    }
    ch->pking = 0;
    ch->last_fight_time = -1;

    /* Reset Arena Counters */
    wartype = 0;
    min_level = 0;
    max_level = 0;
    iswar = FALSE;
    inmiddle = FALSE;
    inwar = 0;
    wartimer = 0;
    wartimeleft = 0;
    arena_counter = 15;
    REMOVE_BIT(ch->act, PLR_ARENA);
    char_from_room(ch);
	if(!IS_NPC(ch)) {
                if ( ch->pcdata->room_number == 0 ) 
     char_to_room(ch, (get_room_index(ROOM_VNUM_ARENA_SAFE))); 
	else
     char_to_room(ch,get_room_index(ch->pcdata->room_number));
	} else 
     char_to_room(ch, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
	do_look(ch,"auto");
        stop_fighting( ch, TRUE );
  }
}
	 if (IS_SET(victim->in_room->room_flags, ROOM_ARENA)) {
/*	 if(!IS_NPC(ch)) {
     victim->pcdata->alosses += 1; } */
    sprintf(buf, "{R[{YARENA{R] {c%s {Rhas just defeated %s!{w",ch->name,victim->name);
    info(buf,NULL,NULL,INFO_ARENA,0,0);         
     char_from_room(victim);
	if(!IS_NPC(victim)) {
                if ( victim->pcdata->room_number == 0 )
     char_to_room(victim, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
	else
    char_to_room(victim,get_room_index(victim->pcdata->room_number));
	} else 
     char_to_room(victim, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
	do_look(victim,"auto");
	stop_fighting(victim,TRUE);
	    affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blindness);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);
            affect_strip(victim,skill_lookup("ice ray"));
            affect_strip(victim,skill_lookup("divine word"));  
            affect_strip(victim,skill_lookup("faerie fog"));
            check_dispel( 150, ch,skill_lookup("deafen"));
            check_dispel( 150, ch,skill_lookup("insanity"));
            check_dispel( 150, ch,skill_lookup("corruption"));
	    spell_cure_corruption(skill_lookup("corruption"),150,ch,ch,0);
	 spell_cancellation(0, 150, ch, ch, 0);

    if(!IS_NPC(victim)) {
    if(victim->pcdata->arenahp != 0)
    victim->hit = victim->pcdata->arenahp;
    else
    victim->hit = victim->max_hit;
    if(victim->pcdata->arenamp != 0)
    victim->mana = victim->pcdata->arenamp;
    else
    victim->mana = victim->max_mana;
    if(victim->pcdata->arenamove !=0)
    victim->move = victim->pcdata->arenamove;
    else
    victim->move = victim->max_move;
    } else {
    victim->hit = victim->max_hit;
    victim->mana = victim->max_mana;
    }
    victim->last_fight_time = -1;
    victim->pking = 0;
      update_pos( victim );
     }
	else {
	    if(ch->msp == 1)
		send_to_char("!!MUSIC(off)",ch);
       /*
        * Death trigger
        */
       if ( IS_NPC( victim ) && HAS_TRIGGER_MOB( victim, TRIG_DEATH) )
       {
           victim->position = POS_STANDING;

           p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_DEATH );
//           mp_percent_trigger( victim, ch, NULL, NULL, TRIG_DEATH );
       }

       if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) ) {
      sprintf( log_buf, "%s got killed by %s at %s [room %d]",
            (IS_NPC(victim) ? victim->short_descr : victim->name),
            (IS_NPC(ch) ? ch->short_descr : ch->name),
            ch->in_room->name, ch->in_room->vnum);
 
        if (IS_NPC(victim))
            wiznet(log_buf,NULL,NULL,WIZ_MOBDEATHS,0,0);
        else
            wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);

	if(!IS_NPC(victim) && !IS_NPC(ch))
	   if(!warring(ch->kingdom,victim->kingdom)) 
		ch->lastpk = current_time;
	}
       if(IS_SET(victim->act,PLR_WANTED) && IS_SET(ch->act,PLR_WANTED) && (ch !=victim))
         {
            send_to_char("Your lucky, You don't lose and eq in orb quest.\n\r",victim);	
            death_cry(victim);
            char_from_room( victim );
            victim->hit = 1;
            update_pos( victim );
            char_to_room( victim, get_room_index(3370) );
         }   
     	 else
     	   raw_kill(victim);
	}

	if (!IS_NPC(ch)
	&&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
	&&  corpse->item_type == ITEM_CORPSE_NPC &&
	can_see_obj(ch,corpse))
	{
	    OBJ_DATA *coins;

	    corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 

	    if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		 corpse && corpse->contains) /* exists and not empty */
		do_get(ch,"all corpse");

   		if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                    corpse && corpse->contains  && /* exists and not empty */
                    !IS_SET(ch->act,PLR_AUTOLOOT))  
		if ((coins = get_obj_list(ch,"gcash",corpse->contains))
		     != NULL)
		do_get(ch,"all.gcash corpse");
            
	    if ( IS_SET(ch->act, PLR_AUTOSAC) )
	     {
       	      if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
		return;  /* leave if corpse has treasure */
	      else
		do_sacrifice(ch,"corpse");
	    }	
	}

	    return;
	  }
	else dam = (dam * 2 + ch->level);
      }

    if (dt == gsn_assassinate)
      {
//	if (number_percent() <= URANGE(10,20+(ch->level-victim->level)*2,50) && !counter && (ch->level/2 > victim->level))
	if (number_percent() <= URANGE(10,20+(ch->level-victim->level)*2,50) && !counter && (ch->level - 4 > victim->level))
	  {
	    act("{rYou {R+++ASSASSINATE+++{r $N!{x",ch,NULL,victim,TO_CHAR);
	    act("{r$n {R+++ASSASSINATES+++{r $N!{x",ch,NULL,victim,TO_NOTVICT);
	    act("{r$n {R+++ASSASSINATES+++{r you!{x",ch,NULL,victim,TO_VICT);
    if ( !IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
    {
	    send_to_char("{rYou have been KILLED!!{x\n\r",victim);
	    act("{r$n is DEAD!{x",victim,NULL,NULL,TO_ROOM);
  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Executioner");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

    }
    else
    {
	    send_to_char("{rYou lost the fight!!{x\n\r",victim);
	    send_to_char("{rYou wake up with a headache some time later.{x\n\r",victim);
	    send_to_char("{rThat assassin must have held back.{x\n\r",victim);
	    act("{r$n lost the fight!{x",victim,NULL,NULL,TO_ROOM);
	    act("{r$n's twisted body is dragged away by the groundskeeper near dead.{x",victim,NULL,NULL,TO_ROOM);
    }

		    if(ch->msp == 1)
		send_to_char("!!MUSIC(off)",ch);
       /*
        * Death trigger
        */
       if ( IS_NPC( victim ) && HAS_TRIGGER_MOB( victim, TRIG_DEATH) )
       {
           victim->position = POS_STANDING;
           p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_DEATH );
//           mp_percent_trigger( victim, ch, NULL, NULL, TRIG_DEATH );
       }

       if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) ) {
      sprintf( log_buf, "%s got killed by %s at %s [room %d]",
            (IS_NPC(victim) ? victim->short_descr : victim->name),
            (IS_NPC(ch) ? ch->short_descr : ch->name),
            ch->in_room->name, ch->in_room->vnum);
 
        if (IS_NPC(victim))
            wiznet(log_buf,NULL,NULL,WIZ_MOBDEATHS,0,0);
        else
            wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);

	if(!IS_NPC(victim) && !IS_NPC(ch))
	   if(!warring(ch->kingdom,victim->kingdom)) 
		ch->lastpk = current_time;
	}
       if(IS_SET(victim->act,PLR_WANTED) && IS_SET(ch->act,PLR_WANTED) && (ch !=victim))
         {
            send_to_char("Your lucky, You don't lose and eq in orb quest.\n\r",victim);	
            death_cry(victim);
            char_from_room( victim );
            victim->hit = 1;
            update_pos( victim );
            char_to_room( victim, get_room_index(3370) );
         }   
     	 else
     	   raw_kill(victim);
	if (!IS_NPC(ch)
	&&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
	&&  corpse->item_type == ITEM_CORPSE_NPC &&
	can_see_obj(ch,corpse))
	{
		OBJ_DATA *coins;

	    corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 

	    if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		 corpse && corpse->contains) /* exists and not empty */
		do_get(ch,"all corpse");

		   if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                    corpse && corpse->contains  && /* exists and not empty */
                    !IS_SET(ch->act,PLR_AUTOLOOT))  
		if ((coins = get_obj_list(ch,"gcash",corpse->contains))
		     != NULL)
		do_get(ch,"all.gcash corpse");
                            
	    if ( IS_SET(ch->act, PLR_AUTOSAC) )
	     {
       	      if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
		return;  /* leave if corpse has treasure */
	      else
		do_sacrifice(ch,"corpse");
	    }
	}

	    return;

	  }
	else
	  {
	    check_improve(ch,gsn_assassinate,FALSE,1);
	    dam *= 2;
	  }
      }


    dam += GET_DAMROLL(ch) * UMIN(100,skill) /100;

    if (dt == gsn_ambush)
      dam *= 3;

    if (!IS_NPC(ch) && ch->pcdata->learned[gsn_deathblow] > 1 &&
	ch->level >= skill_table[gsn_deathblow].abiliti_level[ch->class] )
      {
	if (number_percent() < 0.125 * ch->pcdata->learned[gsn_deathblow])
	  {
	    if (kingdom_ok(ch,gsn_deathblow)) {
	    act("{CYou deliver a blow of deadly force!{x",ch,NULL,NULL,TO_CHAR);
	    act("{C$n delivers a blow of deadly force!{x",ch,NULL,NULL,TO_ROOM);
  	 if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY))
		dam *= 2;
		dam *= number_range(1.5,(float)ch->level/40);
	      check_improve(ch,gsn_deathblow,TRUE,1);
	    }
	  }
	else check_improve(ch,gsn_deathblow,FALSE,3);
      }

    if ( dam <= 0 )
	dam = 1;

    if (dt == gsn_jab)
      dam /= 3;

    if (counter)
      {
	result = damage(ch,ch,2*dam,dt,dam_type,TRUE);
	multi_hit(victim,ch,TYPE_UNDEFINED);
      }
    else result = damage( ch, victim, dam, dt, dam_type, TRUE );


    /* but do we have a funky weapon? */
    if (result && wield != NULL)
    {
        int dam;

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_POISON))
        {
            int level;
            AFFECT_DATA *poison, af;

            if ((poison = affect_find(wield->affected,gsn_poison)) == NULL)
                level = wield->level;
            else
                level = poison->level;
            if (!saves_spell(level / 2,victim,DAM_POISON))
            {
                send_to_char("{gYou feel poison coursing through your veins.{x",
                    victim);
                act("{g$n is poisoned by the venom on $p.{x",
                    victim,wield,NULL,TO_ROOM);

                af.where     = TO_AFFECTS;
                af.type      = gsn_poison;
                af.level     = level * 3/4;
                af.duration  = level / 2;
                af.location  = APPLY_STR;
                af.modifier  = -1;
                af.bitvector = AFF_POISON;
                affect_join( victim, &af );
            }

            /* weaken the poison if it's temporary */
            if (poison != NULL)
            {
                poison->level = UMAX(0,poison->level - 2);
                poison->duration = UMAX(0,poison->duration - 1);
                if (poison->level == 0 || poison->duration == 0)
                    act("{cThe poison on $p has worn off.{x",ch,wield,NULL,TO_CHAR);
            }
        }
        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_VAMPIRIC))
        {
            dam = number_range(1, wield->level / 5 + 1);
            act("{C$p draws life from $n.{x",victim,wield,NULL,TO_ROOM);
            act("{CYou feel $p drawing your life away.{x",
                victim,wield,NULL,TO_CHAR);
            damage(ch,victim,dam,0,DAM_NEGATIVE,FALSE);
            ch->hit += dam/2;
        }
        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FLAMING))
        {
            dam = number_range(1,wield->level / 4 + 1);
            act("{C$n is burned by $p.{x",victim,wield,NULL,TO_ROOM);
            act("{C$p sears your flesh.{x",victim,wield,NULL,TO_CHAR);
	 if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
            fire_effect( (void *) victim,wield->level/2,dam,TARGET_CHAR);
            damage(ch,victim,dam,0,DAM_FIRE,FALSE);
        }
        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FROST))
        {
            dam = number_range(1,wield->level / 6 + 2);
            act("{C$p freezes $n.{x",victim,wield,NULL,TO_ROOM);
            act("{CThe cold touch of $p surrounds you with ice.{x",
                victim,wield,NULL,TO_CHAR);
	 if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
            cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
            damage(ch,victim,dam,0,DAM_COLD,FALSE);
        }
        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SHOCKING))
        {
            dam = number_range(1,wield->level/5 + 2);
            act("{C$n is struck by lightning from $p.{x",victim,wield,NULL,TO_ROOM);
            act("{CYou are shocked by $p.{x",victim,wield,NULL,TO_CHAR);
	 if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
            shock_effect(victim,wield->level/2,dam,TARGET_CHAR);
            damage(ch,victim,dam,0,DAM_LIGHTNING,FALSE);
        }
        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_MANADRAIN))
        {
            dam = number_range(1,wield->level / 3 + 1);
            act("{C$n is being absorbed by $p.{x",victim,wield,NULL,TO_ROOM);
            act("{C$p absorbs your magic.{x",victim,wield,NULL,TO_CHAR);
            damage(ch,victim,dam/2,0,DAM_ENERGY,FALSE);
            ch->mana += dam/4;
	    victim->mana -= dam/2;
        }

    }

    tail_chain( );
if (victim->lagstand > 0)
 victim->position = POS_RESTING;
    return;
}



/*
 * Inflict damage from a hit.
 */
bool damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show ) {

    char buf[MAX_STRING_LENGTH];
    char log_buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    bool immune;
    int dam2;
//    int i;
//    AFFECT_DATA * paf;
//    AFFECT_DATA * paf_next;
    
    if ( victim->position == POS_DEAD )
	return FALSE;

    if ( (ch == victim) && dt != gsn_poison && dt != gsn_plague && dt != gsn_corruption &&
	IS_SET (victim->in_room->room_flags, ROOM_ARENA )) {
	send_to_char("You can't kill yourself in the arena!!\n\r",ch);
	return FALSE;
	}

    if (ch->msp == 1)
	send_to_char("!!MUSIC(music/fokfight1.mid T=fight)",ch);
  
    /* damage reduction */
    if ( dam > 30)
	dam = (dam - 30)/2 + 30;
    if ( dam > 75)
	dam = (dam - 75)/2 + 75;

  /*
     * Stop up any residual loopholes.
     */
    if ( dam > 1500 && !IS_IMMORTAL(ch) && !IS_NPC(ch))
    {
	bug( "Damage: %d: more than 1500 points!{x", dam );
	dam = 1500;

	if (!IS_IMMORTAL(ch))
	{
	    OBJ_DATA *obj;
	    obj = get_eq_char( ch, WEAR_WIELD );
	sprintf(buf,"%s is doing %d damage with %s.",ch->name,dam,obj->name);
	bug(buf,0);
	    send_to_char("{cYour weapon is vaporized with that extraordinary hit.{x\n\r",ch);
	    if (obj)
	      extract_obj(obj);
	}
    }

    if ( victim != ch )
    {
	/*
	 * Certain attacks are forbidden.
	 * Most other attacks are returned.
	 */
	if ( is_safe( ch, victim ) )
	    return FALSE;
	check_killer( ch, victim );

	if ( victim->position > POS_STUNNED )
	{
	    if ( victim->fighting == NULL )
	   {
		set_fighting( victim, ch );
            if ( IS_NPC( victim ) && HAS_TRIGGER_MOB( victim, TRIG_KILL ) )
                   p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_KILL );
//                   mp_percent_trigger( victim, ch, NULL, NULL, TRIG_KILL);
           }
	    if ((victim->timer <= 4) || (victim->lagattack < 1))
	    	victim->position = POS_FIGHTING;
	}

	if ( victim->position > POS_STUNNED )
	{
	    if ( ch->fighting == NULL )
		set_fighting( ch, victim );

	    /*
	     * If victim is charmed, ch might attack victim's master.
	     */
	    if ( IS_NPC(ch)
	    &&   IS_NPC(victim)
	    &&   IS_AFFECTED(victim, AFF_CHARM)
	    &&   victim->master != NULL
	    &&   victim->master->in_room == ch->in_room
	    &&   number_bits( 3 ) == 0 )
	    {
		stop_fighting( ch, FALSE );
		multi_hit( ch, victim->master, TYPE_UNDEFINED );
		return FALSE;
	    }
	}

	   if (IS_AFFECTED2(ch, AFF2_HOLD)) {
	act( "{c$n tries to attack but is held by ESP!!{x",ch, NULL, NULL, TO_ROOM );
		return FALSE;
		}
	/*
	 * More charm stuff.
	 */
	if ( victim->master == ch )
	    stop_follower( victim );
    }

	if(IS_NPC(ch) && ch->master != NULL
    	&& ch->in_room != ch->master->in_room ) {
	do_fleealways(ch,"");
	}
    /*
     * No one in combat can sneak, hide, or be invis or camoed.
     */
    if ( is_affected(ch, gsn_duo_dimension))
        {
         affect_strip(ch,gsn_duo_dimension);
         send_to_char("You turn back into three dimension.\n\r", ch);
         act("$n turns back into three dimension.", ch, NULL, NULL, TO_ROOM);
        }


    if ( IS_SET(ch->affected_by, AFF_HIDE)
	|| IS_SET(ch->affected_by, AFF_INVISIBLE)
	|| IS_SET(ch->affected_by, AFF_FADE)
	|| IS_SET(ch->affected_by, AFF_SNEAK)
	|| IS_SET(ch->affected_by, AFF_CAMOUFLAGE))
      do_visible(ch, "");

    /*
     * Damage modifiers.
     */
    if ( IS_AFFECTED(victim, AFF_SANCTUARY) &&
	 !( (dt == gsn_cleave)&& (number_percent() < 50) ) && !(dt == gsn_jab) )
	dam /= 2;           /* cleave passes through sanc */

    
    if ( IS_AFFECTED2(victim, AFF2_INVUL)) {
	act( "{c$n's aura of invulnerability absorbs attack stricking him!{x",victim, NULL, NULL, TO_ROOM );
	sprintf(buf,"{cYour aura of invulnerability absorbs %s's attack!{x",PERS(victim,ch));
	send_to_char(buf,victim);
	return FALSE;
	}

   if (ch != victim)
       if(is_affected(victim,skill_lookup("divine deflection aura") ) ) 
	{
	 act( "{c$n's a divine aura reflection your attacks!{x\n\r",victim, NULL, NULL, TO_ROOM );
	 sprintf(buf,"{cYour divine aura reflection %s's attack!{x\n\r",PERS(victim,ch));
	 send_to_char(buf,victim);
    if (!is_affected(ch,skill_lookup("divine deflection aura"))
     && !is_affected(ch,skill_lookup("chill shield"))
     && !is_affected(ch,skill_lookup("fire shield")) )
      dam += dam;
      damage(victim,ch,dam,gsn_aura_divine_deflection,DAM_FIRE,TRUE);
	 return FALSE;
	}

    if ( IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
	dam -= dam / 4;

    if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )
	dam -= dam / 4;

    if ( is_affected(victim,gsn_resistance) )
        dam -= dam /3;

    if ( is_affected(ch,gsn_spell_force_nature) )
      if  (dt != gsn_backstab && dt != gsn_cleave
	&& dt != gsn_ambush && dt != gsn_dual_backstab && dt != gsn_circle
	&& dt != gsn_assassinate)      
        dam += dam *.7 ;


    if (is_affected(victim, gsn_protection_heat_cold) &&
	((dam_type == DAM_FIRE) || (dam_type == DAM_COLD)))
       dam -= dam / 4;

    immune = FALSE;


    /*
     * Check for parry, and dodge.
     */
    if ( dt >= TYPE_HIT && ch != victim)
    {
        if (is_affected(victim,gsn_mirror)) 
	{
	  act("{c$n shatters into tiny fragments of glass.{x",
	      victim,NULL,NULL,TO_ROOM);
	  extract_char(victim,TRUE);

	  return FALSE;
	}

/*  adds codes for mobs to use magical items */
          if ( IS_NPC( ch )
              && number_percent() < UMIN( 25, UMAX( 10, ch->level ) )
              && !IS_NPC( victim ) )
              use_magical_item( ch );

        if ( check_parry( ch, victim ) )
	    return FALSE;
	if ( check_block( ch, victim) )
	    return FALSE;
	if ( check_dodge( ch, victim ) )
	    return FALSE;
        if ( check_battle_prowess( ch, victim ) )
	    return FALSE;	    
    }

    switch(check_immune(victim,dam_type))
    {
	case(IS_IMMUNE):
	    immune = TRUE;
	    dam = 0;
	    break;
	case(IS_RESISTANT):
	    dam -= dam/3;
	    break;
	case(IS_VULNERABLE):
	    dam += dam/2;
	    break;
    }

 if ( dt != TYPE_HUNGER &&  dt >= TYPE_HIT && ch != victim )
        {
	check_weapon_destroy( ch, victim , FALSE);

         if ( (dam2 = critical_strike(ch,victim,dam)) != 0 )
         dam = dam2;
        }

    if ( show )
      dam_message( ch, victim, dam, dt, immune );

    if (dam == 0)
	return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
//    dam = dam / .5;
  if (!IS_NPC(ch))
    dam = dam  / 2;
    
    victim->hit -= dam;
    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit < 1 )
	victim->hit = 1;
    update_pos( victim );
  if(dam>1)
         {
        
        if (ch != victim)
         {
          if(is_affected(victim,skill_lookup("fire shield")) 
           && !is_affected(ch,skill_lookup("fire shield"))
           && !is_affected(ch,skill_lookup("divine deflection aura")) )
            {
             damage(victim,ch,dam/2,gsn_fire_shield,DAM_FIRE,TRUE);
             dam = 0;
            } 
          if(is_affected(victim,skill_lookup("chill shield"))
           && !is_affected(ch,skill_lookup("chill shield"))
           && !is_affected(ch,skill_lookup("divine deflection aura")) )
           {
            damage(victim,ch,dam/2,gsn_chill_shield,DAM_COLD,TRUE);
            dam = 0;
           }
         }
        }


   if(is_affected(victim,skill_lookup("ironwill")))
      if(!is_affected(victim,skill_lookup("triggered ironwill")))
       if (victim->hit < victim->max_hit * .2)
        {
         AFFECT_DATA af;
	 act("{D$n's eyes flash red as he is caught up in a frenzy.{x",victim,NULL,NULL,TO_ROOM); 	
         send_to_char("{YYou are caught up in a frenzy..{x\n\r",victim );
         victim->hit += ((victim->max_hit / 100) * victim->level/3) +100;
         af.type	= gsn_triggered_ironwill;
      	 af.level	= victim->level;
      	 af.duration	= 25;
      	 af.modifier	= 0;
      	 af.bitvector = 0;
      	 af.location	= 0;
      	 affect_to_char(victim,&af);
        }     
         update_pos(ch);

    switch( victim->position )
    {
    case POS_MORTAL:
	if ( dam_type == DAM_HUNGER || dam_type == DAM_THIRST) break;
	act( "{c$n is mortally wounded, and will die soon, if not aided.{x",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char(
	    "{cYou are mortally wounded, and will die soon, if not aided.{x\n\r",
	    victim );
	break;

    case POS_INCAP:
	if ( dam_type == DAM_HUNGER || dam_type == DAM_THIRST) break;
	act( "{c$n is incapacitated and will slowly die, if not aided.{x",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char(
	    "{cYou are incapacitated and will slowly die, if not aided.{x\n\r",
	    victim );
	break;

    case POS_STUNNED:
	if ( dam_type == DAM_HUNGER || dam_type == DAM_THIRST) break;
	act( "{c$n is stunned, but just might recover.{x",
	    victim, NULL, NULL, TO_ROOM );
	send_to_char("{cYou are stunned, but just might recover.{x\n\r",
	    victim );
	break;

    case POS_DEAD:
    if ( !IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
    {
	    send_to_char("{rYou have been KILLED!!{x\n\r",victim);
	    act("{r$n is DEAD!{x",victim,NULL,NULL,TO_ROOM);
    }
    else
    {
	    send_to_char("{rYou lost the fight!!{x\n\r",victim);
	    send_to_char("{rYou wake up with a headache some time later.{x\n\r",victim);
	    act("{r$n lost the fight!{x",victim,NULL,NULL,TO_ROOM);
	    act("{r$n's body is dragged away by the groundskeeper nearly dead.{x",victim,NULL,NULL,TO_ROOM);
    }

	break;

    default:
	if ( dam_type == DAM_HUNGER || dam_type == DAM_THIRST) break;
	if ( dam > victim->max_hit / 4 )
	    send_to_char( "{rThat really did {RHURT{r!{x\n\r", victim );
	if ( victim->hit < victim->max_hit / 4 )
	    send_to_char( "{rYou sure are BLEEDING!{x\n\r", victim );
	break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) )
	stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
	group_gain( ch, victim );

	if ( !IS_NPC(victim) )
	{
            sprintf( log_buf, "%s killed by %s at %d",
                victim->name,
                (IS_NPC(ch) ? ch->short_descr : ch->name),
                victim->in_room->vnum );
            log_string( log_buf );

            /*
	     * Dying penalty:
	     * 8% of total exp.
	     */
/*  thinsk this is fucked and people didnt lose exp on deaths
	    if (victim == ch || (IS_NPC(ch) && (ch->master == NULL &&
		 ch->leader == NULL)) || IS_SET(victim->act,PLR_WANTED))
*/

if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) )
if ( !IS_NPC(ch) )
if ( ch!=victim )
{ /*pk counting*/
     /* increment ch pk count */

       if(IS_SET(victim->act,PLR_WANTED) && IS_SET(ch->act,PLR_WANTED) && (ch !=victim))
         {
            send_to_char("Too bad, pk don't count in orb quest.\n\r",ch);	
         }   
     	 else
         ch->pcdata->pkills += 1;
     send_to_char("{CThe spirit of another enemy haunts you, waiting for your death.{x\n\r",ch);     

     /* see if the victim needs some haunting */
     if (victim->pcdata->pkills > 0)
     {
     /* lose 2.5% of normal exp loss for every person you have killed */
 if ( victim->exp > exp_per_level(victim,victim->pcdata->points)
                                * victim->level )
          {
     gain_exp( victim, -(UMIN(1000,victim->exp * 0.08))
                       *(0.025*victim->pcdata->pkills) );
/*
	   lost_exp = (2 * ( exp_per_level(victim,victim->pcdata->points)
			* victim->level - victim->exp) / 3 ) + 50;
	   gain_exp( victim , lost_exp);
*/
	}
     }


/* haunting messages in here */
if (victim->pcdata->pkills == 1 )
if (number_percent() < 80 )
send_to_char("{CThe spirit of a single enemy taunts you, fulfilling long awaited revenge.{x\n\r",victim);

if (victim->pcdata->pkills > 1 && victim->pcdata->pkills < 11 )
{
if (number_percent() < 60 )
send_to_char("{CThe spirits of past enemies seek you out, fulfilling long awaited revenge.{x\n\r",victim);
  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Nefarious %s", capitalize(class_table[ch->class].name ));
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

}

if (victim->pcdata->pkills > 10 )
{
if (number_percent() < 40 ) {
send_to_char("{CThe spirits of past enemies lash out at you, fulfilling long awaited revenge.{x\n\r",victim);
act( "{CCrazed spirits of previous enemies lash out at $n violently.{x\n\r",victim,NULL,NULL,TO_ROOM); 
}
  if ((!IS_NPC(ch)) && (number_percent() < 10)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Serial Killer");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}
}

} /* pk counting */
/*  end */

/*  adds bounty coding */
if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) )
if (!IS_NPC(ch) && !IS_IMMORTAL(ch))
{
  if (ch!=victim)
  {
         if ( victim->pcdata->bounty == 0 )
            {
            ch->pcdata->bounty += (3 * victim->level) + (dice(1,ch->level));
                  sprintf( log_buf, "%s Bounty is now %d after killing  %s",
                  ch->name,ch->pcdata->bounty, victim->name);
            wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);      
            log_string(log_buf);
            sprintf(log_buf, "%s Collected a bounty of %d gold from the murder of %s.",
            ch->name,victim->pcdata->bounty,victim->name);
            log_string(log_buf);    
            }
          else 
           {
	    send_to_char("{CWith this kill, you are due a gold reward.\n\rYou can collect it at any bounty office.\n\r{x",ch);
            ch->pcdata->owed   += victim->pcdata->bounty;
            sprintf(log_buf, "%s Collected a bounty of %d gold from the murder of %s.",
            ch->name,victim->pcdata->bounty,victim->name);
            victim->pcdata->bounty = 0;
            log_string(log_buf);    
           }
  }
}
/*  end bounty code */

if ( ch==victim )
{
  if ((( dam_type == DAM_HUNGER || dam_type == DAM_THIRST)) && (!IS_NPC(ch))){
    if (number_percent() < 50) {    
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Starving Beggar");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}
    }
  else if ((( dam_type == DAM_DISEASE || dam_type == DAM_POISON)) 
	&& (!IS_NPC(ch))){
    if (number_percent() < 50) {
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Besmirched");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}
    } 
  else if (!IS_NPC(ch)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Suicidal Maniac");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}
}

	/* regular exp loss, disabled in room_arena */
	  if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) )
	  {
	 if ( victim->exp > exp_per_level(victim,victim->pcdata->points)
                                * victim->level )
          {
	   gain_exp( victim, - UMIN(1000,victim->exp * 0.08));
	     victim->pcdata->total_death += 1;
	  }
	}
	/*
	 *  Die too much expect PK, and get deleted..

	      if ( !IS_NPC(victim) && ( victim == ch || (IS_NPC(ch) &&
                   (ch->master == NULL && ch->leader == NULL))
                   || IS_SET(victim->act,PLR_WANTED) ) )
        old one
	*/
/*  comments out more code that didnt let people lose con or exp
      if ( ( victim == ch || (IS_NPC(ch) &&
                   (ch->master == NULL && ch->leader == NULL))
                   || IS_SET(victim->act,PLR_WANTED) ) )
*/

if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) )
if (1)
              {
		victim->pcdata->death--;
                if ( victim->pcdata->death < 1 )  {
//		int loss=number_range(0,7);
		
		victim->pcdata->death = 5;	      
/*
		if (loss > 4)
                  victim->perm_stat[STAT_CON]--;
		else
	         victim->perm_stat[loss]--;
*/
                  if 
		(
		  ( victim->perm_stat[STAT_CON] < 3 )  
		  &&
		  ( 
          number_percent() < 
	 (9-victim->perm_stat[STAT_CON])*10-
	 (25-(pc_race_table[victim->race].max_stats[STAT_CON]))*5
		  )
		)	
		 {
		  char strsave[160];
		    send_to_char(
"{GYou don't have the life power enough to reincarnate again.{x\n\r"
"{GYou became a ghost permenantly and leave the earth realm.{x\n\r"
"\n\r{GIt was pleasure to see you in the land...{x\n\r",
			victim );
		    act( "{G$n is dead, and will not rise again...{x\n\r",victim,NULL,NULL,TO_ROOM);
		    victim->last_fight_time = -1;
		    victim->hit = 1;
		    victim->position = POS_STANDING;
                    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );
                    wiznet("$N is deleted due to lack of CON.",ch,NULL,0,0,0);
                    do_quit_count(victim,"");
                    unlink(strsave);
                    return TRUE;
		  }
		  else
		  send_to_char("{CWith this death you suffer exceedingly...{x\n\r", victim);
		}
	      }
      }
	 if (IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
if (IS_SET(victim->act, PLR_ARENA) && iswar == TRUE && inmiddle == TRUE)
{
  REMOVE_BIT(victim->act, PLR_ARENA);

  if(!IS_NPC(victim))
  inwar--;

  if (inwar == 1)
 {
/*        if(!IS_NPC(ch)) {
         ch->pcdata->awins += 1; } */
    sprintf(buf, "{R[{YARENA{R] {c%s {Ris victorious in the arena!{w", ch->name);
    info(buf,NULL,NULL,INFO_ARENA,0,0);         

	    stop_fighting(victim,TRUE);
	    affect_strip(ch,gsn_plague);
            affect_strip(ch,gsn_poison);
            affect_strip(ch,gsn_blindness);
            affect_strip(ch,gsn_sleep);
            affect_strip(ch,gsn_curse);
            affect_strip(ch,skill_lookup("ice ray"));
            affect_strip(ch,skill_lookup("divine word"));              
            affect_strip(ch,skill_lookup("faerie fog")); 
            check_dispel( 150, ch,skill_lookup("deafen"));
            check_dispel( 150, ch,skill_lookup("insanity"));
            check_dispel( 150, ch,skill_lookup("corruption"));
            spell_cure_corruption(skill_lookup("corruption"),150,ch,ch,0);
	 spell_cancellation(0, 150, ch, ch, 0); 
    if(!IS_NPC(ch)) {
    if(ch->pcdata->arenahp != 0)
    ch->hit = ch->pcdata->arenahp;
    else
    ch->hit = ch->max_hit;
    if(ch->pcdata->arenamp != 0)
    ch->mana = ch->pcdata->arenamp;
    else
    ch->mana = ch->max_mana;
    if(ch->pcdata->arenamove != 0)
    ch->move = ch->pcdata->arenamove;
    else
    ch->move = ch->max_move;
    } else {
    ch->hit = ch->max_hit;
    ch->mana = ch->max_mana;
    }
    ch->last_fight_time = -1;
    ch->pking = 0;
    wartype = 0;
    min_level = 0;
    max_level = 0;
    iswar = FALSE;
    inmiddle = FALSE;
    inwar = 0;
    wartimer = 0;
    wartimeleft = 0;
    arena_counter = 15;
    REMOVE_BIT(ch->act, PLR_ARENA);
    char_from_room(ch);
	if(!IS_NPC(ch)) {
                if ( ch->pcdata->room_number == 0 )
     char_to_room(ch, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
	else
    char_to_room(ch,get_room_index(ch->pcdata->room_number));
	} else 
     char_to_room(ch, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
	do_look(ch,"auto");
    stop_fighting(ch,TRUE);
  }
}
	 if (IS_SET(victim->in_room->room_flags, ROOM_ARENA)) {
/*	 if(!IS_NPC(ch)) {
     victim->pcdata->alosses += 1; } */
    sprintf(buf, "{R[{YARENA{R] {c%s {Rhas just defeated %s!{w",ch->name,victim->name);
    info(buf,NULL,NULL,INFO_ARENA,0,0);         
     char_from_room(victim);
	if(!IS_NPC(victim)) {
                if ( victim->pcdata->room_number == 0 )
    char_to_room(victim,get_room_index(ROOM_VNUM_ARENA_SAFE));
	else
    char_to_room(victim,get_room_index(victim->pcdata->room_number));
	} else
    char_to_room(victim,get_room_index(ROOM_VNUM_ARENA_SAFE));
	do_look(victim,"auto");
    stop_fighting(victim,TRUE);
	    affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blindness);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);
            affect_strip(ch,skill_lookup("faerie fog"));
	 spell_cancellation(0, 150, ch, ch, 0);
    if(!IS_NPC(victim)) {
    if(victim->pcdata->arenahp != 0)
    victim->hit = victim->pcdata->arenahp;
    else
    victim->hit = victim->max_hit;
    if(victim->pcdata->arenamp != 0)
    victim->mana = victim->pcdata->arenamp;
    else
    victim->mana = victim->max_mana;
    if(victim->pcdata->arenamove != 0)
    victim->move = victim->pcdata->arenamove;
    else
    victim->move = victim->max_move;
    } else {
    victim->hit = victim->max_hit;
    victim->mana = victim->max_mana;
    }
    victim->last_fight_time = -1;
    victim->pking = 0;
      update_pos( victim );
     }
	else {
	    if(ch->msp == 1)
		send_to_char("!!MUSIC(off)",ch);
       /*
        * Death trigger
        */
       if ( IS_NPC( victim ) && HAS_TRIGGER_MOB( victim, TRIG_DEATH) )
       {
           victim->position = POS_STANDING;
           p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_KILL );
//           mp_percent_trigger( victim, ch, NULL, NULL, TRIG_DEATH );
       }

       if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) ) {
      sprintf( log_buf, "%s got killed by %s at %s [room %d]",
            (IS_NPC(victim) ? victim->short_descr : victim->name),
            (IS_NPC(ch) ? ch->short_descr : ch->name),
            ch->in_room->name, ch->in_room->vnum);
 
        if (IS_NPC(victim))
            wiznet(log_buf,NULL,NULL,WIZ_MOBDEATHS,0,0);
        else
            wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);

  	if(!IS_NPC(victim) && !IS_NPC(ch))
	   if(!warring(ch->kingdom,victim->kingdom)) 
	ch->lastpk = current_time;
	}
       if(IS_SET(victim->act,PLR_WANTED) && IS_SET(ch->act,PLR_WANTED) && (ch !=victim))
         {
            send_to_char("Your lucky, You don't lose and eq in orb quest.\n\r",victim);	
            death_cry(victim);
            char_from_room( victim );
            victim->hit = 1;
            update_pos( victim );
            char_to_room( victim, get_room_index(3370) );
         }   
     	 else
     	   raw_kill(victim);
	}
	
        /* RT new auto commands */

	if (!IS_NPC(ch)
	&&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
	&&  corpse->item_type == ITEM_CORPSE_NPC &&
	can_see_obj(ch,corpse))
	{
	    corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 

	    if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		 corpse && corpse->contains) /* exists and not empty */
		do_get(ch,"all corpse");

	   if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                    corpse && corpse->contains  && /* exists and not empty */
                    !IS_SET(ch->act,PLR_AUTOLOOT))  {
		do_get(ch,"gold corpse");
		do_get(ch,"silver corpse");
                }
            
	    if ( IS_SET(ch->act, PLR_AUTOSAC) )
	     {
       	      if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
		return TRUE;  /* leave if corpse has treasure */
	      else
		do_sacrifice(ch,"corpse");
	     }
	}

	return TRUE;
    }

    if ( victim == ch )
	return TRUE;

    /*
     * Take care of link dead people.
     */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	if ( number_range( 0, victim->wait ) == 0 )
	{
		do_recall(victim,"");
	    return TRUE;
	}
    }

    /*
     * Wimp out?
     */
    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
	if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
	&&   victim->hit < victim->max_hit / 5)
	||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
	&&     victim->master->in_room != victim->in_room ) )
		do_flee(victim,"");
	if ( IS_NPC(victim) ) victim->last_fought = NULL;
    }

    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2 )
	do_flee(victim,"");

    tail_chain( );
    return TRUE;
}
/*  :: use_magical_item function */
void use_magical_item( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *cobj     = NULL;
    int       number   = 0;
    char      buf[ MAX_INPUT_LENGTH ];

    for ( obj = ch->carrying; obj; obj = obj->next_content )
    {
        if ( (   obj->item_type == ITEM_SCROLL
              || obj->item_type == ITEM_WAND
              || obj->item_type == ITEM_STAFF
              || obj->item_type == ITEM_PILL )
            && number_range( 0, number ) == 0 )
        {
            cobj = obj;
            number++;
        }
    }

    if ( !cobj )
        return;

    switch( cobj->item_type )
    {
      case ITEM_SCROLL: 
	do_recite(ch,"scroll");
                         break;
      case ITEM_WAND:   if ( cobj->wear_loc == WEAR_HOLD )
	do_zap(ch,"");
                          break;
      case ITEM_STAFF:  if ( cobj->wear_loc == WEAR_HOLD )
	do_brandish(ch,"");
                        break;
      case ITEM_PILL:   sprintf( buf, "%s", cobj->name );
	do_eat(ch,buf);
                        break;
  }
  return;
}



bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
  if (is_safe_nomessage(ch,victim))
    {
      act("{cThe gods protect $N.{x",ch,NULL,victim,TO_CHAR);
      act("{cThe gods protect $N from $n.{x",ch,NULL,victim,TO_ROOM);
      return TRUE;
    }
  else return FALSE;
}



bool is_safe_nomessage(CHAR_DATA *ch, CHAR_DATA *victim )
{
  int chexp, vchexp;
//  long time;
//  int timeleft;



  if (victim->fighting == ch || ch==victim)
    return FALSE;

  /* ghosts are safe */
  if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST)) ||
      (!IS_NPC(ch) && IS_SET(ch->act, PLR_GHOST)))
    return TRUE;

   if (!str_cmp( ch->name, "Salamon") && (victim->level >  ch->level))
     return FALSE;

   if (!str_cmp( victim->name, "Salamon") && (victim->level <  ch->level))
     return FALSE;

   if( !IS_NPC(victim) && victim->pcdata->quest && victim->pcdata->quest->protection)
        return TRUE;

   if ( IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
       return FALSE;

  /*Kazmir out 14/03/00 */ 
/*  time = current_time - ch->lastpk;
  timeleft = time/86400;

   if((victim != ch)) 
   if((timeleft < 1) && !IS_IMMORTAL(ch) && !IS_NPC(ch) && !IS_NPC(victim))
	return TRUE;

*/ /*Kazmir Add 14/03/00*/
/* Takeda: don't waste my ACT flags use that instead */
   if((victim->pkban == 1) && !IS_NPC(victim))
     {
//      send_to_char( "Sorry, you victim PKban.\n\r", ch );      	
      return TRUE;	
     }

    if (IS_SET(victim->comm,COMM_AFK))
       	return TRUE;
/* Prevent someone writing note to be pked */
  if ((victim->notewrite) || (ch->notewrite))
	return TRUE;
   if ( victim->level <= 15 && !IS_NPC(victim))
       return TRUE;
  /* Make Wanted pked able by all level */
 /*Change to work in a rp thing*/
  if(IS_SET(victim->act,PLR_WANTED)&& IS_SET(ch->act,PLR_WANTED))
	return FALSE;

if ( strstr( ch->in_room->area->builders, "Arena" ) &&
	!IS_SET(ch->in_room->room_flags, ROOM_ARENA) ) 
	return TRUE;
/* will be used sune Kazmir
if ( strstr( ch->in_room->area->builders, "FreezeTag" ) )
       return TRUE;
*/
  if (((!IS_NPC(victim) && !IS_NPC(ch)) &&
	((strlen(ch->description) < 250 ))) ||
      ((!IS_NPC(victim) && !IS_NPC(ch)) &&
	 ((strlen(ch->pcdata->background) < 500 ))))
	return TRUE;

  /* link dead players who do not have rushing adrenalin are safe */
  if (!IS_NPC(victim) && ((victim->last_fight_time == -1) ||
	((current_time - victim->last_fight_time) > FIGHT_DELAY_TIME)) &&
	!victim->desc)
    return TRUE;

  /* Player does not fall below the level's minimum pk range */
  chexp = IS_NPC(ch) ? ch->exp :
    UMAX(ch->exp, ch->exp - exp_this_level(ch,ch->level,ch->pcdata->points));
  vchexp = IS_NPC(victim) ? victim->exp :
    UMAX(victim->exp, victim->exp -
	 exp_this_level(victim,victim->level,victim->pcdata->points));

  if (victim == NULL)
   return TRUE;

  if ( IS_SET(victim->in_room->room_flags,ROOM_SAFE) && !IS_IMMORTAL(ch) &&
      ( (IS_NPC(ch) && IS_NPC(victim)) ||
       (!IS_NPC(victim) && vchexp < PK_MIN) ||
       (!IS_NPC(ch) && chexp < PK_MIN)) )
    return TRUE;

  if (!IS_IMMORTAL(ch) && !IS_NPC(victim) &&
      ((ch->last_death_time != -1 && current_time - ch->last_death_time < 600 )
      || (victim->last_death_time != -1 &&
            current_time - victim->last_death_time < 600)) )
    return TRUE;


  if ( ch != victim && !IS_IMMORTAL(ch) && !IS_NPC(ch) && !IS_NPC(victim) &&
      ( (chexp < PK_MIN) || (vchexp < PK_MIN) ||
        ( ((float) (chexp - vchexp)) / (float) chexp > 0.3) ||
	( ((float) (vchexp - chexp)) / (float) vchexp > 0.3) ) )
    return TRUE;

  if ( ch != victim && !IS_IMMORTAL(ch) && !IS_NPC(ch) && !IS_NPC(victim) &&
      ((ch->level >= victim->level + 7) || (victim->level >= ch->level + 7)))
    return TRUE;

  return FALSE;
}





bool is_safe_spell(CHAR_DATA *ch, CHAR_DATA *victim, bool area )
{

    /* can't zap self (crash bug) */
    if (ch == victim && !area)
	return TRUE;

    /* immortals not hurt in area attacks */
    if (IS_IMMORTAL(victim) &&  area)
	return TRUE;

    if (is_same_group(ch,victim) && area)
      return TRUE;

    if (ch == victim && area && ch->in_room->sector_type == SECT_INSIDE)
      return TRUE;

    return is_safe(ch,victim);
}

/*
 * See if an attack justifies a KILLER flag.....NEVER!!!
 */
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
	char buf[MAX_STRING_LENGTH];

    if ( IS_SET( ch->in_room->room_flags, ROOM_ARENA ) )      /* ARENA */
    return;
#if 0
    /*
     * Follow charm thread to responsible character.
     * Attacking someone's charmed char is hostile!
     */
 
    while ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )
	victim = victim->master;

    /*
     * NPC's are fair game.
     * So are killers and thieves.
     */
    if ( IS_NPC(victim)
    ||   IS_SET(victim->act, PLR_KILLER)
    ||   IS_SET(victim->act, PLR_THIEF) )
	return;

    /*
     * Charm-o-rama.
     */
    if ( IS_SET(ch->affected_by, AFF_CHARM) )
    {
	if ( ch->master == NULL )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "Check_killer: %s bad AFF_CHARM",
		IS_NPC(ch) ? ch->short_descr : ch->name );
	    bug( buf, 0 );
	    affect_strip( ch, gsn_charm_person );
	    REMOVE_BIT( ch->affected_by, AFF_CHARM );
	    return;
	}
/*
	send_to_char( "*** You are now a KILLER!! ***\n\r", ch->master );
  	SET_BIT(ch->master->act, PLR_KILLER);
*/
	stop_follower( ch );
	return;
    }

    /*
     * NPC's are cool of course (as long as not charmed).
     * Hitting yourself is cool too (bleeding).
     * So is being immortal (Alander's idea).
     * And current killers stay as they are.
     */
    if ( IS_NPC(ch)
    ||   ch == victim
    ||   ch->level >= LEVEL_IMMORTAL
    ||   IS_SET(ch->act, PLR_KILLER) )
	return;

    send_to_char( "*** You are now a KILLER!! ***\n\r", ch );
    SET_BIT(ch->act, PLR_KILLER);
    save_char_obj( ch );
#endif
	if(!IS_NPC(ch) && !IS_NPC(victim)) {
	ch->pking = 5;
	victim->pking = 5;	
sprintf(buf,"%s is attempting to murder %s",ch->name,victim->name);
 wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
log_string( buf );
	}
    return;
}



/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;
    int dex_effect;  // to modify chance with regards to dexterity -Derkec    
    int atk_str; 
    int str_effect;  // to modify chance with regards to dexterity and str - Kazmir    
    int atk_dex; 
    OBJ_DATA *weapon;  // the defender's weapons effect his ability to parry
    OBJ_DATA *sec;	    // the secondary weapon.
   	

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( IS_NPC(victim) )
    {
	chance	= UMIN( 30, victim->level );
    }
    else
    {
	if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
	    return FALSE;
	chance	= victim->pcdata->learned[gsn_parry] / 4;
	if(chance == 0)
		return FALSE;
      /* SPEED AND DEXTERITY EFFECTS ADDED BY DERKEC 1/22/00 */

	dex_effect= get_curr_stat(victim,STAT_DEX) /3;
	str_effect= get_curr_stat(victim,STAT_STR) /3;
	
	if ( IS_NPC(ch) )
	{
		atk_dex = 9;
		atk_str = 9;
	}	
	else{
		atk_dex = get_curr_stat(ch,STAT_DEX) /3;
		atk_str = get_curr_stat(ch,STAT_STR) /3;
	}	
    	chance += dex_effect*dex_effect/21;
	chance += str_effect*str_effect/21;
	chance -= atk_dex*atk_dex/21;
        chance -= atk_str*atk_str/21;
        chance -= GET_HITROLL(ch) /6;

 if (is_affected(ch,gsn_aim))
    chance += (ch->pcdata->learned[gsn_aim] / (100 - ch->level)) ;
        
    	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
			chance -= 3;
    	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,OFF_FAST))
		chance += 3;

	/**************** Effect of Weapons - Derkec *******************/

	 weapon = get_eq_char( victim, WEAR_WIELD );
	 sec = get_eq_char( victim, WEAR_SECONDARY );  
	 switch (weapon->value[0])  // type of weapon

	 {
		case WEAPON_SWORD: 
			chance +=8;
			break;
		case WEAPON_DAGGER:
			chance += 3;
			break;
		case WEAPON_AXE:
			chance -= 20;
			break;
		case WEAPON_FLAIL:
			chance -= 7;
			break;
		case WEAPON_WHIP:
			chance -= 7;
			break;
		case WEAPON_POLEARM:
			chance -= 10;
			break;
		case WEAPON_BOW:
			chance += 6;
			break;
		case WEAPON_LANCE:
			chance -= 12;
			break;
		default: break;
	 }
	 if (sec != NULL){
	 	if((sec->value[0] == WEAPON_SWORD) ||  (sec->value[0] == WEAPON_DAGGER))
			chance += 3;
	 }

	
    }
    if (chance >= 60)
	chance = 60;	
    if ( number_percent( ) >= chance + (victim->level/2) - (ch->level/2) )
	return FALSE;

    act( "{CYou parry $n's attack.{x",  ch, NULL, victim, TO_VICT    );
    act( "{C$N parries your attack.{x", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_parry,TRUE,6);
    return TRUE;
}
/*
 * Check for shield block.
 */

bool check_battle_prowess( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;
    OBJ_DATA *weapon;  // the defender's weapons effect his ability to parry
   	
    if(victim->kingdom != KINGDOM_ARKADIA)
       return FALSE;

    if ( get_eq_char( victim, WEAR_SECONDARY ) == NULL )
	    return FALSE;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( IS_NPC(victim) )
    {
	chance	= UMIN( 30, victim->level );
    }
    else
    {
	if ( get_eq_char( victim, WEAR_SECONDARY ) == NULL )
	    return FALSE;
	chance	= victim->pcdata->learned[gsn_battle_prowess] / 4;
	if(chance == 0)
		return FALSE;

	 weapon = get_eq_char( victim, WEAR_SECONDARY );
	 switch (weapon->value[0])  // type of weapon

	 {
		case WEAPON_SWORD: 
			chance +=8;
			break;
		case WEAPON_DAGGER:
			chance += 3;
			break;
		case WEAPON_AXE:
			chance -= 20;
			break;
		case WEAPON_FLAIL:
			chance -= 7;
			break;
		case WEAPON_WHIP:
			chance -= 7;
			break;
		case WEAPON_POLEARM:
			chance -= 10;
			break;
		case WEAPON_BOW:
			chance += 6;
			break;
		case WEAPON_LANCE:
			chance -= 12;
			break;
		default: break;
	 }
	
    }
    if (chance >= 60)
	chance = 60;	
    if ( number_percent( ) >= chance + (victim->level/2) - (ch->level/2) )
	return FALSE;

    act( "{CYou parry $n's attack with you off-hand.{x",  ch, NULL, victim, TO_VICT    );
    act( "{C$N parries your attack with his off-hand..{x", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_battle_prowess,TRUE,6);
    return TRUE;
}


bool check_block( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;
    int split;
    OBJ_DATA *shield;
    OBJ_DATA *axe;
    OBJ_DATA *broken;
    char buf[160];

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( get_eq_char( victim, WEAR_SHIELD ) == NULL )
      return FALSE;

  if (IS_NPC(victim))
    {
      chance = 10;
    }
  else
    {
    if (victim->pcdata->learned[gsn_shield_block] <= 1)
      return FALSE;
    chance -= GET_HITROLL(ch) /6;
    chance	= victim->pcdata->learned[gsn_shield_block] / 3;
    }

 if (is_affected(ch,gsn_aim))
    chance += (ch->pcdata->learned[gsn_aim] / (100 - ch->level));
    if ( number_percent( ) >= chance + (victim->level/2) - (ch->level/2) )
	return FALSE;

    /*
     *  Check for heavy axes split shields in two.
     */
    split = 0;
    if ( (shield = get_eq_char( victim, WEAR_SHIELD)) != NULL )
    {
      axe = get_eq_char( ch, WEAR_WIELD );
      if ( axe != NULL && axe->value[0] == WEAPON_AXE )
      {
        if ( axe->weight > shield->weight)
          split += (axe->weight-shield->weight) / 2;
        if ( !str_cmp( shield->material, "wood" )  &&
             str_cmp( axe->material, "wood" ) )
          split += 5;
        if ( number_percent() < 70 )
          split /= 2;

        if ( number_percent() < split )  {
          broken = create_object( get_obj_index( OBJ_VNUM_BROKEN ), 0 );
          sprintf( buf, broken->name, shield->name );
          free_string( broken->name );
          broken->name = str_dup( buf );

          sprintf( buf, broken->short_descr, shield->short_descr );
          free_string( broken->short_descr );
          broken->short_descr = str_dup( buf );

          sprintf( buf, broken->description, shield->short_descr );
          buf[0] = UPPER(buf[0]);
          free_string( broken->description );
          broken->description = str_dup( buf );

          sprintf( buf, broken->material, shield->material );
          free_string( broken->material );
          broken->material = str_dup( buf );
	  broken->weight = shield->weight;

	  obj_to_room( broken, victim->in_room );
          extract_obj( shield );
          act( "{y$N's attack splits your shield in two!{x", victim, NULL,ch, TO_CHAR);
	  act( "{yYou split $N's shield in two!{x", ch, NULL, victim,TO_CHAR);
          act( "{y$n's attack splits $N's shield in two.{x", ch, NULL,victim, TO_NOTVICT);
          return FALSE;
	}
      }
    }

    act( "{yYour shield blocks $n's attack.{x",  ch, NULL, victim, TO_VICT);
    act( "{y$N deflects your attack with $S shield.{x",ch,NULL,victim,TO_CHAR);
    check_improve(victim,gsn_shield_block,TRUE,6);
    return TRUE;
}



/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;
    int dex_effect=0;       // adjust the chance of dodge based on dexterity.

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( IS_NPC(victim) )
        chance  = UMIN( 30, victim->level );
    else
        chance  = victim->pcdata->learned[gsn_dodge] / 4;
        chance -= GET_HITROLL(ch) /6;
    //if(chance == 0)          // Prevents dexterity from creating false chances.
    //	  return FALSE;	
 if (is_affected(ch,gsn_aim))
    chance += (ch->pcdata->learned[gsn_aim] / (100 - ch->level));
     /* dexterity - added 11/20/99 by Derkec*/
    if(chance > 20)
    {
    	dex_effect= get_curr_stat(victim,STAT_DEX);
    	chance += dex_effect*dex_effect/20-18; //modified from -11 to -18 6/1/00 derk.

    /* speed  - added 11/20/99 by Derkec*/
    	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
			chance += 3;
    	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,OFF_FAST))
		chance -= 3;
     /* pixie should be harder to hit 3/3/00 by Takeda*/
	 if((victim->race == race_lookup("pixie")) && victim->level < 10)
	  {
		if(victim->pcdata->learned[gsn_dodge] < 50)
			victim->pcdata->learned[gsn_dodge] = 50; 	
         	chance += 20;
        }
    }

    if ( number_percent( ) >= chance + (victim->level/2) - (ch->level/2) )
        return FALSE;

    act( "{DYou dodge $n's attack.{x", ch, NULL, victim, TO_VICT    );
    act( "{D$N dodges your attack.{x", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_dodge,TRUE,6);
    return TRUE;
}

bool check_lightning_reflexes( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;
return FALSE; //skill disabled

    if (!kingdom_ok(victim,gsn_lightning_reflexes))
      return FALSE;

  if (victim->level < skill_table[gsn_lightning_reflexes].abiliti_level[victim->class] )
      return FALSE;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( IS_NPC(victim) )
        chance  = UMIN( 30, victim->level );
    else
        chance  = victim->pcdata->learned[gsn_lightning_reflexes] / 2;


    if ( number_percent( ) >= chance + (victim->level/2) - (ch->level/2) )
        return FALSE;

    act( "{DWith dextreous ease you evade $n's attack.{x", ch, NULL, victim, TO_VICT    );
    act( "{D$N evades your attack with dextreous ease.{x", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_lightning_reflexes,TRUE,6);
    return TRUE;
}

bool check_draggon_speed( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance = 0;

     if(!is_affected(victim,skill_lookup("dragon speed")))
	{
         send_to_char("{You are not affected by the speed of the dragons!!{x\n\r",ch);
	 return FALSE;
	}    



  if (victim->level < skill_table[gsn_spell_dragon_speed].abiliti_level[victim->class] )
      return FALSE;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( IS_NPC(victim) )
        chance  = UMIN( 30, victim->level );
    else
        chance  += victim->pcdata->learned[gsn_spell_dragon_speed] * 0.66;


    if ( number_percent( ) >= chance + (victim->level/2) - (ch->level/2) )
        return FALSE;

    act( "{D$n knocks you to the ground, but you get back up with supernatural speed.{x", ch, NULL, victim, TO_VICT    );
    act( "{DYou knock $N to the ground, but $n it gets back up with supernatural speed..{x", ch, NULL, victim, TO_CHAR    );
    check_improve(victim,gsn_spell_dragon_speed,TRUE,6);
    return TRUE;
}


/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{

if (victim->fighting != NULL)
 return;
 
    if ( victim->hit > 0 )
    {
    	if ( victim->position <= POS_STUNNED )
	    victim->position = POS_STANDING;
	return;
    }

    if ( IS_NPC(victim) && victim->hit < 1 )
    {
	victim->position = POS_DEAD;
	return;
    }

    if ( victim->hit <= -11 )
    {
	victim->position = POS_DEAD;
	return;
    }

         if ( victim->hit <= -6 ) victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 ) victim->position = POS_INCAP;
    else                          victim->position = POS_STUNNED;

    return;
}



/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
/*    if ( ch->fighting != NULL )
    {
	bug( "Set_fighting: already fighting", 0 );
	return;
    }
 */

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
	affect_strip( ch, gsn_sleep );

    ch->fighting = victim;
    ch->position = POS_FIGHTING;

    return;
}



/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

	if(ch->msp == 1)
	send_to_char("!!MUSIC(off)",ch);

    	    ch->lagstand = 0;
    	    ch->lagflee = 0;
    	    ch->lagspell = 0;
	    ch->lagattack = 0;
    
    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch == ch || ( fBoth && fch->fighting == ch ) )
	{
	    fch->fighting	= NULL;
	    fch->position	= IS_NPC(fch) ? ch->default_pos : POS_STANDING;
	    update_pos( fch );
    	    fch->lagstand = 0;
    	    fch->lagflee = 0;
    	    fch->lagspell = 0;
	    fch->lagattack = 0;
	    
	}
    }

    return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char log_buf[MAX_STRING_LENGTH];
    char pfile[MAX_STRING_LENGTH];
    char strsave[MAX_INPUT_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    SHOP_DATA *pShop;
    char *name;
    
    pShop = NULL;

   if (!IS_NPC(ch))
    {
     
     save_char_obj(ch);
     sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
     sprintf(pfile,"/home/fate/.ftpbak/ss/Backup/death/%s.bak",capitalize( ch->name ) );
     rename(strsave,pfile);
     save_char_obj(ch);
    }



    if ( IS_NPC(ch) )
    {
        name            = ch->short_descr;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
        corpse->timer   = number_range( 3, 6 );

     if((pShop = ch->pIndexData->pShop) != NULL) {
          ch->gold = ch->level*4;
          ch->silver = ch->level*3;
        }

        if ( ch->gold > 0 )
     {
            obj_to_obj( create_money( ch->gold, ch->silver ), corpse );
            ch->gold = 0;
            ch->silver = 0;
        }
        corpse->cost = 0;
    }
    else
    {
        name            = ch->name;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC),0);
        corpse->timer   = number_range( 200, 300 );
        corpse->lootamount = 0;
        corpse->weight = 1000;
        REMOVE_BIT(ch->act,PLR_CANLOOT);
        corpse->owner = str_dup(ch->name);

            if (ch->gold > 1 || ch->silver > 1)
            {
                obj_to_obj(create_money(ch->gold / 2, ch->silver/2),corpse);
                ch->gold -= ch->gold/2;
                ch->silver -= ch->silver/2;
            }

        corpse->cost = 0;
    }

    corpse->level = ch->level;

    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        bool floating = FALSE;
     

        obj_next = obj->next_content;

    if ( !IS_NPC( ch ) )
        {
         sprintf( log_buf, "DEATH LOG EQ: player: %s obj [vnum: %d]NAME: %s SHORTDESC: %s",ch->name,obj->pIndexData->vnum,obj->name,obj->short_descr);
         log_string( log_buf );
        }
        
        if (obj->wear_loc == WEAR_FLOAT)
            floating = TRUE;
        obj_from_char( obj );
        if (obj->item_type == ITEM_POTION)
            obj->timer = number_range(500,1000);
        if (obj->item_type == ITEM_SCROLL)
            obj->timer = number_range(1000,2500);
      if (IS_SET(obj->extra_flags,ITEM_ROT_DEATH) && !floating)
        {
            obj->timer = number_range(5,10);
            REMOVE_BIT(obj->extra_flags,ITEM_ROT_DEATH);
        }
        REMOVE_BIT(obj->extra_flags,ITEM_VIS_DEATH);

      if ( IS_SET( obj->extra_flags, ITEM_INVENTORY )  
          || ( (obj->pIndexData->limit != -1 
          && (obj->pIndexData->count >= obj->pIndexData->limit) ) 
          && (!IS_NPC( ch ) ) ) )
     
          {
            extract_obj( obj );
          }
        else if (floating)
        {
            if (IS_OBJ_STAT(obj,ITEM_ROT_DEATH)) /* get rid of it! */
            {
                if (obj->contains != NULL)
                {
                    OBJ_DATA *in, *in_next;

                    act("$p evaporates,scattering its contents.",
                        ch,obj,NULL,TO_ROOM);
                    for (in = obj->contains; in != NULL; in = in_next)
                    {
                        in_next = in->next_content;
                        obj_from_obj(in);
                        obj_to_room(in,ch->in_room);
                    }
                 }
                 else
                    act("$p evaporates.",
                        ch,obj,NULL,TO_ROOM);
                 extract_obj(obj);
            }
            else
            {
                act("$p falls to the floor.",ch,obj,NULL,TO_ROOM);
          obj_to_room(obj,ch->in_room);
            }
        }
        else
            obj_to_obj( obj, corpse );
    }

    obj_to_room( corpse, ch->in_room );
    return;
}

void death_cry( CHAR_DATA *ch )
{
  death_cry_org( ch, -1 );
}

/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry_org( CHAR_DATA *ch, int part )
{
    ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;
    int vnum;
  
    vnum = 0;
    msg = "{cYou hear $n's death cry.{x";

    if ( part == -1 )
      part = number_bits(4);

    switch ( part )
    {

    case  0: msg  = "{c$n hits the ground ... DEAD.{x";			break;
    case  1:
	if (ch->material == 0)
	{
	    msg  = "{c$n splatters blood on your armor.{x";
	    break;
	}
    case  2:
	if (IS_SET(ch->parts,PART_GUTS))
	{
	    msg = "{c$n spills $s guts all over the floor.{x";
	    vnum = OBJ_VNUM_GUTS;
	}
	break;
    case  3:
	if (IS_SET(ch->parts,PART_HEAD))
	{
	    msg  = "{c$n's severed head plops on the ground.{x";
	    vnum = OBJ_VNUM_SEVERED_HEAD;
	}
	break;
    case  4:
	if (IS_SET(ch->parts,PART_HEART))
	{
	    msg  = "{c$n's heart is torn from $s chest.{x";
	    vnum = OBJ_VNUM_TORN_HEART;
	}
	break;
    case  5:
	if (IS_SET(ch->parts,PART_ARMS))
	{
	    msg  = "{c$n's arm is sliced from $s dead body.{x";
	    vnum = OBJ_VNUM_SLICED_ARM;
	}
	break;
    case  6:
	if (IS_SET(ch->parts,PART_LEGS))
	{
	    msg  = "{c$n's leg is sliced from $s dead body.{x";
	    vnum = OBJ_VNUM_SLICED_LEG;
	}
	break;
    case 7:
	if (IS_SET(ch->parts,PART_BRAINS))
	{
	    msg = "{c$n's head is shattered, and $s brains splash all over you.{x";
	    vnum = OBJ_VNUM_BRAINS;
	}
    }

    act( msg, ch, NULL, NULL, TO_ROOM );

    if ( vnum != 0 )
    {
	char buf[MAX_STRING_LENGTH];
	OBJ_DATA *obj;
	char *name;

	name		= IS_NPC(ch) ? ch->short_descr : ch->name;
	obj		= create_object( get_obj_index( vnum ), 0 );
	obj->timer	= number_range( 4, 7 );

	sprintf( buf, obj->short_descr, name );
	free_string( obj->short_descr );
	obj->short_descr = str_dup( buf );

	sprintf( buf, obj->description, name );
	free_string( obj->description );
	obj->description = str_dup( buf );

	obj->from = str_dup(name);

	if (obj->item_type == ITEM_FOOD)
	{
	    if (IS_SET(ch->form,FORM_POISON))
		obj->value[3] = 1;
	    else if (!IS_SET(ch->form,FORM_EDIBLE))
		obj->item_type = ITEM_TRASH;
	}

	obj_to_room( obj, ch->in_room );
    }

    if ( IS_NPC(ch) )
	msg = "{cYou hear something's death cry.{x";
    else
	msg = "{cYou hear someone's death cry.{x";

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = was_in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   pexit->u1.to_room != was_in_room )
	{
	    ch->in_room = pexit->u1.to_room;
	    act( msg, ch, NULL, NULL, TO_ROOM );
	}
    }
    ch->in_room = was_in_room;

    return;
}

/*
 *  For death cry, improved by GG(). part = deathcry()
 */

void raw_kill( CHAR_DATA *victim )
{
  raw_kill_org( victim, -1 );
  return;
}

 void raw_kill_org( CHAR_DATA *victim, int part )
{

  CHAR_DATA *tmp_ch;
  OBJ_DATA *obj;
  int i;
  OBJ_DATA *tattoo; 

  if(victim->msp == 1)
  send_to_char("!!MUSIC(music/fokdeath.mid T=death)",victim);

  stop_fighting( victim, TRUE );

/* work on m/i progs  */
  for (obj = victim->carrying;obj != NULL;obj = obj->next_content)
    {
      if (IS_SET(obj->progtypes,IPROG_DEATH))
	if ((obj->pIndexData->iprogs->death_prog) (obj,victim))
	  {
	    victim->position = POS_STANDING;
	    return;
	  }
    }
  victim->last_fight_time = -1;
  if ( IS_SET(victim->progtypes,MPROG_DEATH))
    if ((victim->pIndexData->moprogs->death_prog) (victim))
      {
	victim->position = POS_STANDING;
	return;
      }
/* Okay enough of that i/m prog work */

  victim->last_death_time = current_time;



  tattoo = get_eq_char(victim, WEAR_TATTOO);
  if (tattoo != NULL)
    obj_from_char(tattoo);

if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) ) { 

    death_cry_org( victim, part );
    make_corpse( victim );
}

  if ( IS_NPC(victim) )
    {
      victim->pIndexData->killed++;
      kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
      extract_char( victim, TRUE );

      return;
    }

if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) )
{
  send_to_char("{cYou turn into an invincible ghost for a few minutes.{x\n\r",victim);
  send_to_char("{cAs long as you do not attack anything.{x\n\r", victim);
    extract_char( victim, FALSE );
}
else
{
  send_to_char("{cYou are very badly injured, but relieved to find all of your gear intact.{x\n\r",victim);
}

  while ( victim->affected )
    affect_remove( victim, victim->affected );
victim->affected_by = race_table[victim->race].aff;
victim->affected_by2 = race_table[victim->race].aff2;
victim->detection = race_table[victim->race].detect;
  for (i = 0; i < 4; i++)
    victim->armor[i]= 100;
  victim->position	= POS_RESTING;
  victim->hit		= 1;
  victim->mana	= 1;
  victim->move	= victim->level>25?UMAX( 1, victim->move): victim->max_move;

  /* RT added to prevent infinite deaths */
  REMOVE_BIT(victim->act, PLR_KILLER);
  REMOVE_BIT(victim->act, PLR_THIEF);
//  REMOVE_BIT(victim->act, PLR_WANTED);
  REMOVE_BIT(victim->act, PLR_BOUGHT_PET);

  victim->pcdata->condition[COND_THIRST] = 40;
  victim->pcdata->condition[COND_HUNGER] = 40;
  victim->pcdata->condition[COND_FULL] = 40;



  if (tattoo != NULL)
    {
      obj_to_char(tattoo, victim);
      equip_char(victim, tattoo, WEAR_TATTOO);
    }
    save_char_obj( victim ); 

  /* Calm the mad mobs down */
  for (tmp_ch = char_list; tmp_ch != NULL; tmp_ch = tmp_ch->next)
    if (tmp_ch->last_fought == victim)
      tmp_ch->last_fought = NULL;

  return;
}



void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    int xp;
    int members;
    int group_levels;
    int highestlevel = 0;
	/*  :: no gains in arena */
    if ( IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
        return;

/* : quest code added */
        if (IS_SET(ch->act, PLR_QUESTOR) && IS_NPC(victim))
        {if (ch->questmob == victim->pIndexData->vnum)
             {
		send_to_char("{cYou have almost completed your job!{x\n\r",ch);
                send_to_char("{cReturn to the whoever assigned you this job before your time runs out!{x\n\r",ch);
                ch->questmob = -1;
	     }
	}
/* : end */
    /*
     * Monsters don't get kill xp's or alignment changes.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     * or killing grouped mobs (since you don't lose xp to them)
     */
    if ( victim == ch )
	return;


	if (IS_NPC(ch))
	return;
        if (!IS_NPC(victim))
        return;

    if (IS_NPC(victim) && (victim->master != NULL || victim->leader != NULL))
      return;


    members = 1;
    group_levels = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) )
        {
	    if (!IS_NPC(gch) && gch != ch)
	      members++;
	    group_levels += gch->level;
	}
    }

   for (lch = ch->in_room->people; lch != NULL; lch = lch->next_in_room)
	{
        if ( !is_same_group( lch, ch ) || IS_NPC(lch))
		continue;
        if (lch->level > highestlevel)            
		highestlevel = lch->level;
    }

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	if ( !is_same_group( gch, ch ) || IS_NPC(gch))
	    continue;

	if ( gch->level - highestlevel <= -8 )    {
       send_to_char( "Your powers would be useless to such an advanced group of adventurers\n\r",ch);        
		continue;    }

	xp = xp_compute( gch, victim, group_levels,members );
	sprintf( buf, "{cYou receive %d experience points.{x\n\r", xp );
	send_to_char( buf, gch );

	gain_exp( gch, xp );

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_loc == WEAR_NONE )
		continue;

	    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ) )
	    {
		act( "{cYou are zapped by $p.{x", ch, obj, NULL, TO_CHAR );
		act( "{c$n is zapped by $p.{x",   ch, obj, NULL, TO_ROOM );
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );
	    }
	}
    }

    return;
}



/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute(CHAR_DATA *gch, CHAR_DATA *victim, int total_levels,int members)
{
  char buf[MAX_STRING_LENGTH];
  int xp;
  int base_exp;
  int level_range;

  level_range = victim->level - gch->level;

  if(gch->kingdom == KINGDOM_ALDON)
    level_range -= 5;

  /* compute the base exp */
  switch (level_range)
    {
    default : 	base_exp =   0;		break;
  case -9 :	base_exp =   1;		break;
  case -8 :	base_exp =   2;		break;
  case -7 :	base_exp =   5;		break;
  case -6 : 	base_exp =   9;		break;
  case -5 :	base_exp =  11;		break;
  case -4 :	base_exp =  22;		break;
  case -3 :	base_exp =  33;		break;
  case -2 :	base_exp =  50;		break;
  case -1 :	base_exp =  66;		break;
  case  0 :	base_exp =  83;		break;
  case  1 :	base_exp =  99;		break;
  case  2 :	base_exp = 121;		break;
  case  3 :	base_exp = 143;		break;
  case  4 :	base_exp = 165;		break;
  }

  if (level_range > 4)
    base_exp = 160 + 20 * (level_range - 4);

  /* calculate exp multiplier */
  if (IS_SET(victim->act,ACT_NOALIGN))
    xp = base_exp;

  /* New alignment computations for xp */

  else if ((IS_EVIL(gch) && IS_GOOD(victim)) || (IS_EVIL(victim) && IS_GOOD(gch)))
    xp = base_exp * 8/5;

  else if ( IS_GOOD(gch) && IS_GOOD(victim) )
    xp = 0;

  else if ( !IS_NEUTRAL(gch) && IS_NEUTRAL(victim) )
    xp = base_exp * 1.1;

  else if ( IS_NEUTRAL(gch) && !IS_NEUTRAL(victim) )
    xp = base_exp * 1.3;

  else xp = base_exp;

    /* randomize the rewards */
    xp = number_range (xp * 3/4, xp * 5/4);

    /* adjust for grouping */
    xp = xp * gch->level/total_levels;

    if (members == 2)
      xp *= 2;
    else if (members == 3)
      xp *= 4;
    else if (members >= 4)
    	xp *= 2;

    if(gch->level < 11 ) 
	xp *= 2;
    else if(gch->level < 21 )
	xp *= 1.5;
    else if(gch->level < 40 )
	xp *= 1;
    else if(gch->level < 50 )
   xp *= .09;
    else if(gch->level < 60)
	xp *= .8;
    else if(gch->level < 70)
	xp *= .7;
    else if(gch->level < 76 )
	xp *= .6;
    else if(gch->level >= 77 )
	xp *= .4;
    
if(IS_NPC(victim)) {
if (strstr(victim->pIndexData->area->builders, "Unlinked") &&
	!IS_IMMORTAL(gch)) {
	sprintf(buf,"%s is not approved yet.\n\r",victim->short_descr);
	send_to_char(buf,gch);
	xp = 0;
    }
}

	if(IS_IMMORTAL(gch))
	xp = 0;

	if(IS_NPC(victim)) {
	if(victim->pIndexData->vnum ==MOB_VNUM_SPECIAL_GUARD)
	xp = 0;
	if(victim->pIndexData->vnum == MOB_VNUM_SQUIRE)
	xp = 0;
	if(victim->pIndexData->vnum == 14)
	xp = 0;
	if(victim->pIndexData->vnum == 15)
	xp = 0;
	if(victim->pIndexData->vnum == 232)
	xp = 0;
	}
/*  Mandatory Quests? ... Nah.
   if(!IS_SET(gch->quest, QUEST_WEAPONSMITH ) && gch->level >= 40) {
   send_to_char("You must solve the quest of the weaponsmith to level!\n",gch);
	xp = 0;
        }
   if(!IS_SET(gch->quest, QUEST_GOLEM ) && gch->level >= 60) {
send_to_char("You must solve the quest of the queen of new thalos to level!\n",gch);
	xp = 0;
        }
*/
/*
 if (!str_cmp (gch->name, "Asmodai")) {
  send_to_char("You have revealed quest information to other player.\n",gch);
  send_to_char("There you been punished for a month of non xp gain.\n",gch);
  send_to_char("April 14, 1999 2:28, you will be free on May 14, 1999 2:28pm\n",gch);
	xp = 0;
	}
*/
	/* DOUBLE EXP DAY */
	/*	xp *= 2; */
     if (xp > 750) 
      {
       sprintf( log_buf, "%s just gained %d from killing [%d] %s (Level Difference %d )",gch->name,xp,victim->pIndexData->vnum,victim->name,level_range);
       log_string( log_buf );
       wiznet(log_buf,gch,NULL,WIZ_CHEAT,0,0);
       xp = 750;
      }      

   if (level_range > 50)
    {
     do_autofreeze(gch);
    }

    return xp;
}


void dam_message( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune )
{
    char buf1[256], buf2[256], buf3[256], buf4[256], buf5[256], pbuf1[256], pbuf2[256], pbuf3[256];
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;
    int pblood1, pblood2;
    int blood_vnum;
    OBJ_DATA *blood;
    int battleskill,damagetype,part;
    
  if (ch == NULL || victim == NULL)
        return;

battleskill = number_range(0,7);
part  = number_range(0,3);

damage_message(ch,victim,dam,battleskill,dt,part);
 return;
/*  adds his own damage messages */

if ( dam == 0 ) { vs = "miss{c"; vp = "misses";}
 else if ( dam <= 3 ) { vs = "{gnick{w"; vp = "{gnicks{w"; }
 else if ( dam <= 5 ) { vs = "{gscratch{w";vp = "{gscratches{w"; }
 else if ( dam <= 8 ) { vs = "{ggraze{w";vp = "grazes{w";}
 else if ( dam <=12 ) { vs = "{gbruise{w"; vp = "bruises{w"; }
 else if ( dam <=16 ) { vs = "{Ghit{w";vp = "{Ghits{w";}
 else if ( dam <=20 ) { vs = "{Ginjure{w"; vp = "{Ginjures{w"; }
 else if ( dam <=24 ) { vs = "{Gwound{w";vp = "{Gwounds{w";}
 else if ( dam <=28 ) { vs = "{Gmaul{w"; vp = "{Gmauls{w"; }
 else if ( dam <=32 ) { vs = "{Gdecimate{w"; vp = "{Gdecimates{w"; }
 else if ( dam <=36 ) { vs = "{Gdevastate{w";vp = "{Gdevastates{w";}
 else if ( dam <=40 ) { vs = "{rMUTILATE{w"; vp = "{rMUTILATES{w"; }
 else if ( dam <=44 ) { vs = "{rDISEMBOWEL{w"; vp = "{rDISEMBOWELS{w"; }
 else if ( dam <=48 ) { vs = "{rEVISCERATE{w";vp = "{rEVISCERATES{w";}
 else if ( dam <=52 ) { vs = "{rDISMEMBER{w";vp = "{rDISMEMBERS{w";}
 else if ( dam <= 56 ) { vs = "{rMASSACRE{w"; vp = "{rMASSACRES{w"; }
 else if ( dam <= 60 ) { vs = "{rMANGLE{w";vp = "{rMANGLES{w";}
 else if ( dam <= 75 ) { vs = "{R*DEMOLISH*{w";
vp = "{R*DEMOLISHES*{w"; }
 else if ( dam <= 90 ) { vs = "{R**SLAUGHTER**{w";
vp = "{R**SLAUGHTERS**{w"; }
 else if ( dam <= 105 ) { vs = "{R***OBLITERATE***{w";
vp = "{R***OBLITERATES***{w";}
 else if ( dam <= 125 ) { vs = "{r<={RCREMATE{r=>{w";
vp = "{r<={RCREMATES{r=>{w";}
 else if ( dam <= 150 ) { vs = "{r<=<={RANNIHILATE{r=>=>{w";
 vp = "{r<=<={RANNIHILATES{r=>=>{w"; }
 else if ( dam <= 175 ) { vs = "{r<<=<={RERADICATE{r=>=>>{w";
 vp = "{r<<=<={RERADICATES{r=>=>>{w"; }
 else if ( dam <= 200 ) { vs = "{r<=><=>{RPULVERIZE{r<=><=>{w";
 vp = "{r<=><=>{RPULVERIZES{r<=><=>{w"; }
 else if ( dam <= 225 ) { vs = "{r<=><=><=>{RVAPORIZE{r<=><=><=>{w";
 vp = "{r<=><=><=>{RVAPORIZES{r<=><=><=>{w"; }
 else if ( dam <= 250 ) { vs = "{r<==><==><==>{RNUKE{r<==><==><==>{w";
 vp = "{r<==><==><==>{RNUKES{r<==><==><==>{w"; }
 else { vs = "do {WUNSPEAKABLE{w things to";
 vp = "does {WUNSPEAKABLE{w things to"; }

     punct   = (dam <= 24) ? '.' : '!';
    sprintf(buf4, " {R({Y%d{R){w", dam);

    if ( dt == TYPE_HIT )
    {
	if (ch  == victim)
	{
	sprintf( buf1, "$n %s $melf%c",vp,punct);
	sprintf( buf2, "You %s yourself%c%s",vs,punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : ""));
	sprintf( buf5, "%s %s %s",ch->name,vp,victim->name);
	}
	else
	{
   	sprintf( buf1, "$n %s $N%c",  vp, punct );
        sprintf( buf2, "You %s $N%c%s", vs, punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ));
        sprintf( buf3, "$n %s you%c%s", vp, punct,
	((IS_SET(victim->act,PLR_AUTODAMAGE) && 
	IS_SET(ch->act,PLR_AUTODAMAGE))? buf4 : "" ));
        sprintf( buf5, "%s %s %s",ch->name,vp,victim->name);
	}
    }
    else
    {
	if ( dt >= 0 && dt < MAX_SKILL )
	    attack	= skill_table[dt].abiliti_noun_damage;
	else if ( dt >= TYPE_HIT
	&& dt <= TYPE_HIT + MAX_DAMAGE_MESSAGE) 
	    attack	= attack_table[dt - TYPE_HIT].noun;
	else
	{
	    bug( "Dam_message: bad dt %d.{x", dt );
	    dt  = TYPE_HIT;
	    attack  = attack_table[0].name;
	}

	if (immune)
	{
	    if (ch == victim)
	    {
		sprintf(buf1,"$n is unaffected by $s own %s.",attack);
		sprintf(buf2,"Luckily, you are immune to that.");
		sprintf(buf5,"%s is unaffected by %s.",ch->name,attack);
	    }
	    else
	    {
	    	sprintf(buf1,"$N is unaffected by $n's %s!",attack);
	    	sprintf(buf2,"$N is unaffected by your %s!",attack);
	    	sprintf(buf3,"$n's %s is powerless against you.",attack);
		sprintf(buf5,"%s is unaffected by %s's %s!",victim->name,ch->name,attack);
	    }
	}
	else
	{
	    if (ch == victim)
	    {
		sprintf( buf1, "$n's %s %s $m%c",attack,vp,punct);
                sprintf( buf2, "Your %s %s you%c%s",attack,vp,punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ));
                sprintf( buf5, "%s's %s %s %s",ch->name,attack,vp,victim->name);
	    }
	    else
	    {
         sprintf( buf1, "$n's %s %s $N%c%s",  attack, vp, punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ) );
                sprintf( buf2, "Your %s %s $N%c%s",  attack, vp, punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ));
                sprintf( buf3, "$n's %s %s you%c%s", attack, vp, punct,
		((IS_SET(victim->act,PLR_AUTODAMAGE) &&
		IS_SET(ch->act,PLR_AUTODAMAGE))? buf4 : "" ));
                sprintf( buf5, "%s's %s %s %s",ch->name,attack,vp,victim->name);
	    }
	}
    }

  if (!IS_NPC( victim ))
   {
    pblood1 = ((victim->max_hit / 100) * 2 );
    pblood2 = ((victim->max_hit / 100) * 35);

    if ( (dam > pblood1) && (victim->hit < pblood2) )
     {
      blood_vnum = OBJ_VNUM_BLOOD;
      blood = create_object( get_obj_index(blood_vnum), victim->level);
      blood->timer = number_range(1,10);
      blood->cost  = 0;
      obj_to_room(blood, victim->in_room);
      
      sprintf( pbuf1, "{R%s's blood flies everywhere{X",victim->name);
      sprintf( pbuf2, "{R%s's blood flies everywhere{X",victim->name);
      sprintf( pbuf3, "{RYour blood flies everywhere{X");
      
      if (ch == victim)
       {
	act(pbuf1,ch,NULL,NULL,TO_ROOM);
	act(pbuf2,ch,NULL,NULL,TO_CHAR);
       }
      else
      {
       act( pbuf1, ch, NULL, victim, TO_NOTVICT);
       act( pbuf2, ch, NULL, victim, TO_CHAR);
       act( pbuf3, ch, NULL, victim, TO_VICT);
      }
       
     }
   }
    if (ch == victim)
    {
	act(buf1,ch,NULL,NULL,TO_ROOM);
	act(buf2,ch,NULL,NULL,TO_CHAR);
    }
    else
    {
    	act( buf1, ch, NULL, victim, TO_NOTVICT);
    	act( buf2, ch, NULL, victim, TO_CHAR);
    	act( buf3, ch, NULL, victim, TO_VICT);
    }
    return;
}



/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
	return;

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE))
    {
	act("{C$S weapon won't budge!{x",ch,NULL,victim,TO_CHAR);
	act("{C$n tries to disarm you, but your weapon won't budge!{x",
	    ch,NULL,victim,TO_VICT);
	act("{c$n tries to disarm $N, but fails.{x",ch,NULL,victim,TO_NOTVICT);
	return;
    }

    act( "{C$n disarms you and sends your weapon flying!{x",
	 ch, NULL, victim, TO_VICT    );
    act( "{CYou disarm $N!{x",  ch, NULL, victim, TO_CHAR    );
    act( "{c$n disarms $N!{x",  ch, NULL, victim, TO_NOTVICT );

    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_INVENTORY) )
	obj_to_char( obj, victim );
    else
    {
	obj_to_room( obj, victim->in_room );
	if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
	    get_obj(victim,obj,NULL);
    }

    return;
}
void do_surrender( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *mob;
    if ( (mob = ch->fighting) == NULL )
    {
       send_to_char( "But you're not fighting!\n\r", ch );
       return;
    }
    act( "You surrender to $N!", ch, NULL, mob, TO_CHAR );
    act( "$n surrenders to you!", ch, NULL, mob, TO_VICT );
    act( "$n tries to surrender to $N!", ch, NULL, mob, TO_NOTVICT );
    stop_fighting( ch, TRUE );
    if ( !IS_NPC( ch ) && IS_NPC( mob )
    &&   ( !HAS_TRIGGER_MOB( mob, TRIG_SURR )
        || !p_percent_trigger( mob, NULL, NULL, NULL, NULL, NULL, TRIG_SURR ) ))
//        || !mp_percent_trigger( mob, ch, NULL, NULL, TRIG_SURR ) ) )
    {
       act( "$N seems to ignore your cowardly act!", ch, NULL, mob, TO_CHAR );
       multi_hit( mob, ch, TYPE_UNDEFINED );
    }
}




void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cKill whom?{x\n\r", ch );
	return;
    }

//    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
	return;

    if ( ch->position == POS_FIGHTING)
    {
      if (victim == ch->fighting)
	send_to_char( "{cYou do the best you can!{x\n\r", ch );
      else if (victim->fighting != ch)
	send_to_char("{cOne battle at a time, please.{x\n\r",ch);
      else
	{
	  act( "{cYou start aiming at $N.{x",ch,NULL,victim,TO_CHAR);
	  ch->fighting = victim;
	}
	return;
    }
    if ( !IS_NPC(victim) )
    {
        if ( !IS_SET(victim->act, PLR_KILLER)
        &&   !IS_SET(victim->act, PLR_THIEF) )
        {
            send_to_char( "{cYou must MURDER a player.{x\n\r", ch );
            return;
        }
    }
    if ( victim == ch )
    {
	send_to_char( "{cYou hit yourself.  Ouch!{x\n\r", ch );
	multi_hit( ch, ch, TYPE_UNDEFINED );
	return;
    }


#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif
    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
	act( "{c$N is your beloved master.{x", ch, NULL, victim, TO_CHAR );
	return;
    }


    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    check_killer( ch, victim );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}

void do_murde( CHAR_DATA *ch, char *argument )
{
    send_to_char( "{cIf you want to MURDER, spell it out.{x\n\r", ch );
    return;
}



void do_murder( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cMurder whom?{x\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) || (IS_NPC(ch) && IS_SET(ch->act,ACT_PET)))
	return;

//    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cSuicide is a mortal sin.{x\n\r", ch );
	return;
    }

  /*   if (IS_AFFECTED(victim,AFF_FLYING) && !(IS_AFFECTED(ch,AFF_FLYING)))    // Modified by Derkec 11/21/99
	{
	send_to_char("You swing at the FLYING person and miss.\n\r",ch);
	send_to_char("You notice some person on the ground swinging a weapon at you.\n\r",victim);
	return;
	}
*/

    if ( is_safe( ch, victim ) )
	return;

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
	act( "{c$N is your beloved master.{x", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "{cYou do the best you can!{x\n\r", ch );
	return;
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    if (!can_see(victim, ch))
	do_yell(victim,"Help! I am being attacked by someone!");
    else
      {
	if (IS_NPC(ch))
	  sprintf(buf, "Help! I am being attacked by %s!{x",ch->short_descr);
	else
	  sprintf( buf, "Help!  I am being attacked by %s!{x",
		  (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		  ch->doppel->name : ch->name );
	do_yell(victim,buf);
      }
    check_killer( ch, victim );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}





void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int attempt;
    int chance =0;
    int sn;
    
    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
            ch->position = POS_STANDING;
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
    }


 sn = skill_lookup("panic");
 if (!is_affected(ch, gsn_panic))
    {
     af.type      = sn;
     af.level     = ch->level;
     af.duration  = 1;
     af.where     = TO_AFFECTS2;
     af.bitvector = AFF2_PANIC;
     af.modifier	= 0;
     af.location	= 0;
     affect_to_char( ch, &af );
    }

    if ( (ch->lagflee > 0) )
     {

      chance = 100 - (ch->lagflee * 9);
     if (number_percent() < chance)
      {
       ch->lagflee = 0;
      }
     else
      {
       send_to_char( "{CPANIC! You couldn't escape!{x\n\r", ch );
       return;      
      }
     }

 if (ch->move < (ch->max_move * .05))
  {
   send_to_char( "{CPANIC! You're too exausted to escape.!{x\n\r", ch );
   return;       
  }
    was_in = ch->in_room;
    for ( attempt = 0; attempt < 6; attempt++ )
    {
	EXIT_DATA *pexit;
	int door;

	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->u1.to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
	    continue;

	move_char( ch, door, FALSE );
	if ( ( now_in = ch->in_room ) == was_in )
	    continue;

	ch->in_room = was_in;
	act( "{c$n has fled!{x", ch, NULL, NULL, TO_ROOM );
	ch->in_room = now_in;


	if ( !IS_NPC(ch) )
	{
/*	 send_to_char( "{cYou flee from combat!  You lose 10 exps.{x\n\r", ch );
	 if ( ch->exp > exp_per_level(ch,ch->pcdata->points)      
                          * ch->level )
          {
	    gain_exp( ch, -10 );
	} */
	ch->move = ch->move * 0.9;
	}
	else
	  ch->last_fought = NULL;  /* Once fled, the mob will not go after */
				   /* the winner */
	stop_fighting( ch, TRUE );
	return;
    }

    send_to_char( "{CPANIC! You couldn't escape!{x\n\r", ch );
    return;
}
void do_fleealways( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    EXIT_DATA *pexit;
    int door;

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
            ch->position = POS_STANDING;
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    was_in = ch->in_room;

	door = number_door( );
	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->u1.to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
	return;

	move_char( ch, door, FALSE );
	if ( ( now_in = ch->in_room ) == was_in )
	return;

	ch->in_room = was_in;
	act( "{c$n has fled!{x", ch, NULL, NULL, TO_ROOM );
	ch->in_room = now_in;


	if ( !IS_NPC(ch) )
	{
	 send_to_char( "{cYou flee from combat!  You lose 10 exps.{x\n\r", ch );
	 if ( ch->exp > exp_per_level(ch,ch->pcdata->points)      
                          * ch->level )
          {
	    gain_exp( ch, -10 );
	}
	}
	else
	  ch->last_fought = NULL;  /* Once fled, the mob will not go after */
				   /* the winner */

	stop_fighting( ch, TRUE );
	return;

}







void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "{cIf you want to SLAY, spell it out.{x\n\r", ch );
    return;
}



void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "{cSlay whom?{x\n\r", ch );
	return;
    }

//    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x{x\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "Suicide is a mortal sin.{x\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && victim->level >= get_trust(ch) )
    {
	send_to_char( "{cYou failed.{x\n\r", ch );
	return;
    }

    act( "{cYou slay $M in cold blood!{x",  ch, NULL, victim, TO_CHAR    );
    act( "{c$n slays you in cold blood!{x", ch, NULL, victim, TO_VICT    );
    act( "{c$n slays $N in cold blood!{x",  ch, NULL, victim, TO_NOTVICT );
    raw_kill( victim );
    return;
}



/*
 * Check for obj dodge.
 */
bool check_obj_dodge( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, int bonus )
{
    int chance;


    if (IS_NPC(victim))
     return FALSE;
 
    if ( !IS_AWAKE(victim) || MOUNTED(victim) )
        return FALSE;

    chance  = get_skill(victim,gsn_dodge) / 3;
                   /*  chance for high dex. */
    chance += 2 * (get_curr_stat(victim,STAT_DEX) - 20);
 
    chance -= (bonus - 90);
    chance /= 2;

/*    if ( number_percent( ) >= chance &&
         (IS_NPC(victim) || victim->kingdom != KINGDOM_ASPER ))
        return FALSE; 
*/

   if ( number_percent( ) <= chance)
   {
/*
    if (!IS_NPC(victim) && victim->cabal == KINGDOM_BATTLE
        && IS_SET(victim->act,PLR_CANINDUCT))
    {
     act("You catch $p that had been shot to you.",ch,obj,victim,TO_VICT);
     act("$N catches $p that had been shot to $M.",ch,obj,victim,TO_CHAR);
     act("$n catches $p that had been shot to $m.",victim,obj,ch,TO_NOTVICT);
     obj_to_char(obj,victim);
     return TRUE;
    }
*/
    act("You dodge $p that had been shot to you.",ch,obj,victim,TO_VICT);
    act("$N dodges $p that had been shot to $M.",ch,obj,victim,TO_CHAR);
    act("$n dodges $p that had been shot to $m.",victim,obj,ch,TO_NOTVICT);
    obj_to_room(obj,victim->in_room);
    check_improve(victim,gsn_dodge,TRUE,6);

    return TRUE;
   } 
return FALSE;
}

void do_multiburst( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim = ch->fighting;
  char  arg1 [ MAX_INPUT_LENGTH ];
  char  arg2 [ MAX_INPUT_LENGTH ];
  int   sn1;
  int   sn2;
  bool  legal1 = FALSE;
  bool  legal2 = FALSE;
  int   cost = 0;
  int   chance;
  int   cost1;
  int   cost2;

  if ( IS_NPC( ch ) )
        return;

    if ( (chance = get_skill(ch,gsn_multiburst)) == 0)
   {
        send_to_char("{CHuh?.\n\r",ch);
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if ( get_skill(ch,gsn_multiburst) < number_percent())
    {
        send_to_char( "You failed.\n\r", ch);
        return;
    }

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  sn1 = skill_lookup( arg1 );
  sn2 = skill_lookup( arg2 );

  if ( sn1 != -1
  && get_skill( ch, sn1 )
  && skill_table[sn1].abiliti_target == TAR_CHAR_OFFENSIVE
  && (*skill_table[sn1].spell_fun) != (*spell_null) )
        legal1 = TRUE;

  if ( sn2 != -1
  && get_skill( ch, sn2 )
  && skill_table[sn2].abiliti_target == TAR_CHAR_OFFENSIVE
  && (*skill_table[sn2].spell_fun) != (*spell_null) )
        legal2 = TRUE;

  if ( !legal1 && !legal2 )
        {
        WAIT_STATE( ch, skill_table[gsn_multiburst].abiliti_beats[1] );
        send_to_char( "Your multiburst fails.\n\r", ch );
        check_improve(ch,gsn_multiburst,FALSE,0);
        return;
        }
  cost1 = skill_table[sn1].abiliti_manacost;
  cost2 = skill_table[sn2].abiliti_manacost;

  cost += cost1;
  cost += cost1 * 0.2;
  cost += cost2;
  cost += cost2 * 0.2;;

  if ( ch->mana < cost && ch->level < LEVEL_IMMORTAL )
        {
	WAIT_STATE( ch, skill_table[gsn_multiburst].abiliti_beats[1] );
        send_to_char( "You don't have enough mana to multiburst these spells.\n\r",ch);
        return;
        }

  WAIT_STATE( ch, skill_table[gsn_multiburst].abiliti_beats[1] );

  send_to_char(  "You release a burst of energy!\n\r", ch );
  act(  "$n releases a burst of energy.", ch, NULL, NULL, TO_ROOM );


  if ( legal1 )
        (*skill_table[sn1].spell_fun) ( sn1,
                                        URANGE( 1, ch->level, 141 ),
                                        ch, victim, TARGET_CHAR );

  if ( victim->position != POS_DEAD && ch->in_room == victim->in_room )
  {
  if ( legal2 )
        (*skill_table[sn2].spell_fun) ( sn2,
                                        URANGE( 1, ch->level, 141 ),
                                        ch, victim, TARGET_CHAR );
  }

  check_improve(ch,gsn_multiburst,TRUE,1);

  if ( ch->level < LEVEL_IMMORTAL )
        ch->mana -= cost;
  return;
}
 
