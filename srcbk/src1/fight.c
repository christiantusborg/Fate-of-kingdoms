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
*     Fate license.  The use off this code without the permission of the    *
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
#include "fight.h"



DECLARE_DO_FUN(	do_quit_count	);
DECLARE_DO_FUN( do_yell		);
DECLARE_DO_FUN( do_emote	);
DECLARE_DO_FUN( do_zap		);
DECLARE_DO_FUN( do_eat		);
DECLARE_DO_FUN( do_bash		);
DECLARE_DO_FUN( do_berserk	);
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
void	death_cry	args( ( CHAR_DATA *ch ) );
void	death_cry_org	args( ( CHAR_DATA *ch, int part) );
void	group_gain	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int	xp_compute	args( ( CHAR_DATA *gch, CHAR_DATA *victim,int total_levels,int members ) );
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

int skill_mod(CHAR_DATA *ch)
{
    //values need to be set here with a switch statement, higher values are picked last
    //remember competing with hitpoints so values need to have some size to them and should
    //be based on level of class and wheather if at that level they have access to summon,
    //parry, dodge, shield block(with a shield out)
    return 0;
}

int armor_mod(CHAR_DATA *ch)
{
    return (GET_AC(ch, AC_PIERCE)/10 + GET_AC(ch, AC_BASH)/10 + GET_AC(ch, AC_SLASH)/10  + GET_AC(ch, AC_EXOTIC)/10)*10;
}


int get_thac(CHAR_DATA *ch, CHAR_DATA *victim,int dt,int skill)
{
    int THAC_HITROLL_AFFECT 			= 10; // Div This
    int THAC_LEVEL_AFFECT 			= 1;  // Div This
    int THAC_CANSEE_AFFECT 			= 15;  // Affect This
    int THAC_LESS_POS_FIGHTING_AFFECT 	= 4;  // Affect This
    int THAC_LESS_POS_RESTING_AFFECT 		= 15;  // Affect This
    int THAC_AFFECT_HASTE	 		= 3;  // Affect This
    int THAC_SUN_AFFECT			= 20;
    int thac;
    int race;
    
	thac = 30;
	thac += ch->level / THAC_LEVEL_AFFECT;
	thac -= victim->level / THAC_LEVEL_AFFECT;
	thac += GET_HITROLL(ch) / THAC_HITROLL_AFFECT;

    if ( IS_NPC(ch) )
    {
	if (IS_SET(ch->act,ACT_WARRIOR))
	    thac = 30;
	else if (IS_SET(ch->act,ACT_THIEF))
	    thac += 20;
	else if (IS_SET(ch->act,ACT_CLERIC))
	    thac += 15;
	else if (IS_SET(ch->act,ACT_MAGE))
	    thac += 5;
	else
	  thac += 20;   
    }
    else
	thac += class_table[ch->class].thac0_00; //(1- 30)

	if ( !can_see( ch, victim ) )
		thac -= THAC_CANSEE_AFFECT;

	if ( victim->position < POS_FIGHTING)
		thac += THAC_LESS_POS_FIGHTING_AFFECT;

	if (victim->position < POS_RESTING)
		thac += THAC_LESS_POS_RESTING_AFFECT;

	if (IS_AFFECTED(ch,AFF_HASTE))
		thac += THAC_AFFECT_HASTE;
	
	if (IS_AFFECTED(victim,AFF_HASTE))
		thac -= THAC_AFFECT_HASTE;

//	thac += agi_app[get_curr_stat(ch,STAT_AGI)].tohit;
//	thac -= agi_app[get_curr_stat(victim,STAT_AGI)].tohit;
//	thac += get_tohit_part(part);		

	if (!IS_NPC(ch))	
		thac -=	ch->pcdata->condition[COND_DRUNK] / 5;

	if ((weather_info.sunlight == SUN_SET)
	 || (weather_info.sunlight == SUN_DARK)
	 || (!IS_OUTSIDE(victim)))
	  {
	  	race = race_lookup("drow");
	  	if (victim->race == race)
	  		thac -= THAC_SUN_AFFECT;
	  	if (ch->race == race)
	  		thac += THAC_SUN_AFFECT;
	  }
	else
	  {
	  	race = race_lookup("drow");
	  	if (victim->race == race)
	  		thac += THAC_SUN_AFFECT - 5;
	  	if (ch->race == race)
	  		thac -= THAC_SUN_AFFECT;
	  }	

return thac;
}

void find_weakest_in_group(CHAR_DATA *ch, CHAR_DATA *victim)
{
    CHAR_DATA *weakest = victim;
    CHAR_DATA *gch;
    sh_int weakpoint,point;


    weakpoint = weakest->hit;
    if (IS_AFFECTED(weakest, AFF_SANCTUARY))
     weakpoint += weakest->hit;
    
    weakpoint += skill_mod(weakest);
    weakpoint += armor_mod(weakest);
    if (IS_NPC(weakest))
     weakpoint += 500;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
     if ( gch != ch 
      && is_same_group(gch,victim) )
         { 
          point = gch->hit;
          if (IS_AFFECTED(gch, AFF_SANCTUARY))
           point += gch->hit;
          point += skill_mod(gch);
          point += armor_mod(gch);
          if (IS_NPC(gch))
           point += 500;
          if (point < weakpoint)
           {
            weakest = gch;
            weakpoint = point;

         if (weakest!=victim && number_range(1,100)>75) 
           if (weakest->guarded_by && (number_range(1,100) > get_skill(weakest->guarded_by, gsn_guard)/3))
            {
             ch->fighting=weakest;
            }
           } 
        }
    }

}

void do_autofreeze( CHAR_DATA *ch )
{
//    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( !IS_SET(ch->act, PLR_FREEZE) )
    {
	SET_BIT(ch->act, PLR_FREEZE);
	send_to_char("You're fucked, you may have cheated and are frozen.\n\r",ch);	
	xprintf(buf,"[%s] is fucked, he may have cheated and is frozen",ch->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    log_string( buf );
    save_char_obj( ch );
    ch->last_fight_time = 0;
    stop_fighting(ch,TRUE);
    xprintf(buf,"%s 1",ch->name);
    do_transfer(ch, buf );
    send_to_char("All you can do now, is quit.\n\r",ch);	

    return;
}



void violence_update(int size)
//void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
     OBJ_DATA *obj_next;
     bool room_trig = FALSE;
     
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next	= ch->next;

	if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
	    continue;
         
         if (ch->size != size)
         continue;

        
         find_weakest_in_group(ch, victim);
         victim = ch->fighting;
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
               p_percent_trigger( ch, obj, NULL, victim, NULL, NULL, TRIG_FIGHT );
           if ( HAS_TRIGGER_MOB( ch, TRIG_HPCNT ) )
               p_hprct_trigger( ch, victim );


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
    }
    return;
}

/* for auto assisting */



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
 act_new("You are paralyzed and unable to attack.",ch,NULL,victim,TO_CHAR,POS_FIGHTING);
 act_new("$n is paralyzed and can't attack.",ch,NULL,victim,TO_VICT,POS_FIGHTING);
 act_new("$n is paralyzed and can't attack.",ch,NULL,victim,TO_NOTVICT,POS_FIGHTING);
/*         affect_strip(ch,gsn_power_stun);
         SET_BIT(ch->affected_by,AFF_WEAK_STUN); */
         return;
        }
        
  chance = number_range(1,50) + (get_curr_stat(victim,STAT_STR) - get_curr_stat(ch,STAT_STR));
        if (ch->submission > 0)
        {
         if (chance > 40)
         {
         	act("{c$n struggles, but you hold him to tightly!{x",ch,NULL,victim,TO_VICT);
		act("{cYour attempt to make $N let go of your arm is futile{x",ch,NULL,victim,TO_CHAR);
		act("{c$n struggles against $N's armlock without any luck{x",ch,NULL,victim,TO_NOTVICT);
         }
         else if ((chance < 10 ) || (ch->submission == 1))
         {
         	act("{cYour inferior strength is not sufficient to control $n's arm !{x",ch,NULL,victim,TO_VICT);
		act("{cYour superior strength breaks $N's armlock{x",ch,NULL,victim,TO_CHAR);
		act("{c$n regains control of his arm as he shakes off $N.{x",ch,NULL,victim,TO_NOTVICT);
         	ch->submission = 0;
         }
         else
         {
         	act("{c$n struggles against your grip, and you feel your fingers slip{x",ch,NULL,victim,TO_VICT);
		act("{cAs you struggle you feel $N's lock loosen slightly{x",ch,NULL,victim,TO_CHAR);
        	--ch->submission;
	 }
	}
	
    if (IS_NPC(ch))
    {
	mob_hit(ch,victim,dt);
	return;
    }

//    if (ch->pcdata->condition[COND_DRUNK] > 10)
     one_hit( ch, victim, dt ,FALSE);

/*  adds dual */


    if (get_eq_char (ch, WEAR_SECONDARY) != NULL && !IS_AFFECTED2(ch, AFF2_SUBMISSION) && ch->submission == 0)
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

   if (IS_AFFECTED2(ch, AFF2_SUBMISSION) && victim->submission > 0)
   damage( ch,victim,number_range(100,500),gsn_submission,DAM_HARM, TRUE);
   

    /*don't you get faster with both hands?*/

   if (IS_AFFECTED(ch,AFF_HASTE))
    if ((dt != gsn_backstab) && (ch->level >= 50) )
     if (get_eq_char (ch, WEAR_SECONDARY) != NULL && !IS_AFFECTED2(ch, AFF2_SUBMISSION) && ch->submission == 0)
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
    && (get_skill(ch,gsn_dual_wield) < 1))
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
    
    if (ch->race == race_lookup("dwarf") && !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 50 )
    if ( number_percent( ) < get_skill(ch,gsn_second_attack) )
    {
	one_hit( ch, victim, dt, FALSE );
	check_improve(ch,gsn_second_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return;
    }
    if (ch->race == race_lookup("dwarf") && !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 70 )
    if ( number_percent( ) < get_skill(ch,gsn_second_attack) )
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
    int chance,number,extra;
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

//		if extra


    if (IS_SET(ch->off_flags,OFF_XATTACK))
    one_hit(ch,victim,dt,FALSE);
    
    if (IS_SET(ch->off_flags,OFF_XATTACK2))
    one_hit(ch,victim,dt,FALSE);
    
    if (IS_SET(ch->off_flags,OFF_XATTACK3))
    one_hit(ch,victim,dt,FALSE);
    
    if (IS_SET(ch->off_flags,OFF_XATTACK4))
    one_hit(ch,victim,dt,FALSE);
    
    if (IS_SET(ch->off_flags,OFF_XATTACK5))
    one_hit(ch,victim,dt,FALSE);
    
    if (IS_SET(ch->off_flags,OFF_XATTACK6))
    one_hit(ch,victim,dt,FALSE);


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
    int dam = 0;
    int thac;
    //inti;
    int diceroll;
    int sn,skill;
    int dam_type;
    bool counter;
    bool yell;
 int body_part;

    sn = -1;
    counter = FALSE;




	//Can we do the fighting?
	if (victim == ch || ch == NULL || victim == NULL)
		return;

	if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST)) || (!IS_NPC(ch) && IS_SET(ch->act, PLR_GHOST)))
		return;

	if ( IS_AFFECTED2(ch, AFF2_STONESHAPE))
	  {
		send_to_char("You cannot attack in stoneform!!!{x\n\r",ch);
		return;
	  } 

	if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
		return;
    

	//Yell Yes or No
	if (victim->fighting == ch)
		yell = FALSE;
	else 
		yell = TRUE;

	set_fighting( ch, victim );
	
	if (check_aura_of_invulnerability(ch,victim))
		return;


	if (check_pixie_quickly(victim))
		return;
	
	if (check_esp(ch))
		return;	

	if (check_phases(victim))
		return;		


        if ( check_parry( ch, victim ) )
	    return;
	
	if ( check_block( ch, victim) )
	    return;
	
	if ( check_dodge( ch, victim ) )
	    return;
        
        if ( check_battle_prowess( ch, victim ) )
	    return;	    

	if ( check_mirror(ch,victim))
	    return;	


	//Do secondary weapon or not
	if (!secondary)
		wield = get_eq_char( ch, WEAR_WIELD );
	else
		wield = get_eq_char( ch, WEAR_SECONDARY );

	//Damage Type
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

	//Weapon skill
	sn = get_weapon_sn(ch);
	skill = 20 + get_weapon_skill(ch,sn);

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
	thac = get_thac(ch,victim,dt,skill);

    diceroll = number_range(1,100);
    
    
      	if  (diceroll >= 95 || ( ( diceroll >= thac) && ( diceroll >= 5) ) )
    		{
			// Miss.
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
		else {
			dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
		}

	else
	 {
		if (sn != -1)
			check_improve(ch,sn,TRUE,5);
	
		if ( wield != NULL )
		  {
			if (wield->pIndexData->new_format)
				dam = dice(wield->value[1],wield->value[2]) * skill/100;
			else
				dam = number_range( wield->value[1] * skill/100,wield->value[2] * skill/100);

	if (IS_WEAPON_STAT(wield,WEAPON_TWO_HANDS))
		dam += (GET_DAMROLL(ch) / 3 ) *  UMIN(100,skill) /100;

			// sharpness! 

			if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
			  {
				int percent;

				if ((percent = number_percent()) <= (skill / 8))
					dam = dam + (dam * 2 * percent / 100);
			 }
		  }
		else
			dam = number_range( 1 + 4 * skill/100, 2 * ch->level/3 * skill/100);
	  }

    /*
     * Bonuses.
     */
	dam = get_dwarf_drunk_damage_affect(ch,dam);

	dam = get_affect_enhanced_damage(ch,dam); 

	dam = get_affect_brutal_damage(ch,dam);
	
	if (!IS_NPC(ch)) 
	  {
		dam += ((ch->level/10) *
		str_app[get_curr_stat(ch,STAT_STR)].todam);
	  }
	if ( !IS_AWAKE(victim) )
		dam *= 2;
	else if (victim->position < POS_FIGHTING)
		dam = dam * 3 / 2;

	if (ch->class == CLASS_MNK) 
		dam += ((get_skill(ch,gsn_hand_to_hand)* 0.8) / (101 - ch->level));


	if (check_counter(ch,victim))
	  {
		damage(ch,ch,2*dam,dt,dam_type,TRUE);
		multi_hit(victim,ch,TYPE_UNDEFINED);
		return;
	  }
  

	if ( dt == gsn_backstab && wield != NULL)
		dam = (ch->level/50 + 1) * dam + (ch->level * 3);
	else if ( dt == gsn_dual_backstab && wield != NULL)
		dam = (ch->level/50 + 1) * dam + (ch->level * 3);
	else if (dt == gsn_circle)
		dam = (ch->level/50 + 2) * dam + (ch->level * 3);
	else if (dt == gsn_ambush)
		dam *= 3;
	else if (dt == gsn_jab)
		dam /= 3;


	WAIT_STATE( ch, 2 );


	dam += GET_DAMROLL(ch) * UMIN(100,skill) /100;

	body_part = number_range(1,4);
	dam = affect_armor_damage(victim,dam_type,dam,body_part);
	dam = affect_size_damage(ch,dam);


	if ( dam <= 0 )
		dam = 1;

	damage(ch,victim,dam,dt,dam_type,TRUE);

    /* but do we have a funky weapon? */
	if (wield != NULL)
	  {
		affect_poison_weapon(ch,victim,wield);
		affect_vampiric_weapon(ch,victim,wield);
		affect_flaming_weapon(ch,victim,wield);
		affect_frost_weapon(ch,victim,wield);   
		affect_shocking_weapon(ch,victim,wield);   
		affect_manadrain_weapon(ch,victim,wield);   
		affect_lightning_weapon(ch,victim,wield);
		affect_negative_weapon(ch,victim,wield);
		affect_harm_weapon(ch,victim,wield);
		affect_energy_weapon(ch,victim,wield);
		affect_disease_weapon(ch,victim,wield);
		affect_sound_weapon(ch,victim,wield);
		
	  }

    tail_chain( );
if (victim->lagstand > 0)
 victim->position = POS_RESTING;
    return;
}


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
      case ITEM_PILL:   xprintf( buf, "%s", cobj->name );
	do_eat(ch,buf);
                        break;
  }
  return;
}



bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
  if (is_safe_nomessage(ch,victim))
    {
      act("{cYou are unable to attack $N.{x",ch,NULL,victim,TO_CHAR);
      act("{c$n seems unable to attack $N.{x",ch,NULL,victim,TO_ROOM);
      return TRUE;
    }
  else return FALSE;   
}



bool is_safe_nomessage(CHAR_DATA *ch, CHAR_DATA *victim )
{
//  int chexp, vchexp;
//  long time;
//  int timeleft;
//  int level_diff = 0;

	if (ch == NULL)
		{
			stop_fighting( ch, TRUE );
			return TRUE;
		}
		
	if (victim == NULL)
		{
			stop_fighting( victim, TRUE );
			return TRUE;
		}
	

  if (victim->fighting == ch || ch==victim)
    return FALSE;


  
  /* ghosts are safe */
  if ((!IS_NPC(victim) && IS_SET(victim->act, PLR_GHOST)) ||
      (!IS_NPC(ch) && IS_SET(ch->act, PLR_GHOST)))
    return TRUE;

      if( !IS_NPC(victim) && victim->pcdata->quest && victim->pcdata->quest->protection)
        return TRUE;

  if(ch->in_room->vnum == 11300)
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
//      send_to_char( "Sorry, you are banned from pk.\n\r", ch );      	
      return TRUE;	
     }
     
    if ( IS_AFFECTED2(victim, AFF2_SUBMERGED) && !IS_AFFECTED2(ch, AFF2_SUBMERGED)){
    return TRUE;
    }

    if ( IS_AFFECTED2(ch, AFF2_SUBMERGED) && !IS_AFFECTED2(victim, AFF2_SUBMERGED)){
    return TRUE;
    } 

    if (IS_SET(victim->comm,COMM_AFK))
       	return TRUE;
/* Prevent someone writing note to be pked */
  if ((victim->notewrite) || (ch->notewrite))
	return TRUE;

   if ( victim->level <= 15 && !IS_NPC(victim))
       return TRUE;

/*
   if(IS_SET(victim->act,PLR_WANTED)&& IS_SET(ch->act,PLR_WANTED))
	return FALSE;
*/
    if ( strstr( ch->in_room->area->builders, "Arena" ) &&
	!IS_SET(ch->in_room->room_flags, ROOM_ARENA) ) 
	return TRUE;

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


  if (!IS_IMMORTAL(ch) && !IS_NPC(victim) &&
      ((ch->last_death_time != -1 && current_time - ch->last_death_time < 600 )
      || (victim->last_death_time != -1 &&
            current_time - victim->last_death_time < 600)) )
    return TRUE;

  if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
   return TRUE;

  if (victim == NULL)
   return TRUE;


  if ( ch != victim && !IS_IMMORTAL(ch) && !IS_NPC(ch) && !IS_NPC(victim) &&
      ((ch->level >= victim->level + (victim->crime +7 )) || (victim->level >= ch->level + (ch->crime + 7))))
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

	    xprintf( buf, "Check_killer: %s bad AFF_CHARM",
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

xprintf(buf,"%s is attempting to murder %s",ch->name,victim->name);
 wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
log_string( buf );
	}
    return;
}

/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
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

    if ( victim->hit <= -10 )
    {
	victim->position = POS_DEAD;
	return;
    }

         if ( victim->hit <= -5 ) victim->position = POS_MORTAL;
    else if ( victim->hit <= 0 ) victim->position = POS_INCAP;
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
                send_to_char("{cReturn to whoever assigned you this job before your time runs out!{x\n\r",ch);
                ch->questmob = -1;
	     }
	}
    if ( victim == ch )
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
	    if (gch != ch)
	      members++;
	    group_levels += gch->level;
	}
    }

   for (lch = ch->in_room->people; lch != NULL; lch = lch->next_in_room)
	{
        if ( !is_same_group( lch, ch ))
		continue;
        if (lch->level > highestlevel)            
		highestlevel = lch->level;
    }

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	if ( !is_same_group( gch, ch ))
	    continue;

	if ( gch->level - highestlevel <= -8 )    {
       send_to_char( "Your powers would be useless to such an advanced group of adventurers\n\r",ch);        
		continue;    }

	xp = xp_compute( gch, victim, group_levels,members );
	xprintf( buf, "{cYou receive %d experience points.{x\n\r", xp );
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
  int gain = 0;
  int level =0;
  int vmlevel=0;
  int i =0;
  int count = 0;
  bool vnum_found = FALSE;
  
	if (IS_NPC(gch))
		return FALSE;
  
	if(IS_IMMORTAL(gch))
		return FALSE;

	
	vmlevel = victim->level;

//	if (gch->level >= 55)
//		return FALSE;

	
	if (vmlevel > 75)
		vmlevel = 75;

	if ( !IS_SET( gch->quest, QUEST_REDBLUERING ) && gch->level >= 45) 
	  {
	  	send_to_char( "Completed eyes of the knight quest to gain more levels.\n\r", gch );
		return 0;
	  }


	if(!IS_SET(gch->quest, QUEST_WEAPONSMITH ) && gch->level >= 50) 
	  {
	  	send_to_char( "Completed the weapon smith quest to gain more levels.\n\r", gch );
		return 0;
	  } 

	if ( !IS_SET( gch->quest, QUEST_GOLEM ) && gch->level >= 55) 
	  {
	  	send_to_char( "Completed the dressmake quest to gain more levels.\n\r", gch );
		return 0;
	  }


	if (gch->level >= 75)
	 return 0;

	level_range = vmlevel - gch->level;

  // compute the base exp
	if (level_range <= -10)
		base_exp = 0;
	else if (level_range <= -14) 
		base_exp = number_range(1,3);
	else if (level_range <= -13)
		base_exp = number_range(1,3);
	else if (level_range <= -12)
		base_exp = number_range(1,3);
	else if (level_range <= -11) 
		base_exp = number_range(1,3);
	else if (level_range <= -10) 
		base_exp = number_range(1,3);
	else if (level_range <= -9) 
		base_exp = number_range(1,3);
	else if (level_range <= -8)
		base_exp = number_range(2,7);
	else if (level_range <= -7)
		base_exp = number_range(5,11);
	else if (level_range <= -6)
		base_exp = number_range(9,17);
	else if (level_range <= -5)
		base_exp = number_range(13,26);
	else if (level_range <= -4)
		base_exp = number_range(20,38);
	else if (level_range <= -3)
		base_exp = number_range(32,50);
	else if (level_range <= -3)
		base_exp = number_range(45,68);
	else if (level_range <= -2)
		base_exp = number_range(58,86);
	else if (level_range <= -1)
		base_exp = number_range(76,100);
	else if (level_range <= 0)
		base_exp = number_range(90,113);
	else if (level_range < 1)
		base_exp = number_range(103,126);
	else if (level_range < 2)
		base_exp = number_range(116,141);
	else if (level_range < 3)
		base_exp = number_range(131,157);
	else if (level_range < 4)
		base_exp = number_range(147,173);
	else if (level_range < 5)
		base_exp = number_range(163,187);
	else if (level_range < 6)
		base_exp = number_range(177,212);
	else if (level_range < 7)
		base_exp = number_range(202,232);
	else if (level_range < 8)
		base_exp = number_range(222,250);
	else 
		base_exp = 250;

		
//	base_exp *= 1.25;
// base_exp *= 2;

if (double_xp)
	 base_exp *= 2; 
	
	if (members >= 4)
		base_exp /= ( members * 2 );

  // New alignment computations for xp 
	if ( IS_GOOD(gch) && IS_EVIL(victim) )
		xp = base_exp * 2;
	else if ( IS_GOOD(gch) && IS_GOOD(victim) )
		xp = 0;
  else xp = base_exp;

	if(IS_NPC(victim)) 
	  {
		if (strstr(victim->pIndexData->area->builders, "Unlinked") && !IS_IMMORTAL(gch)) 
		  {
			xprintf(buf,"%s is not approved yet.\n\r",victim->short_descr);
			send_to_char(buf,gch);
			xp = 0;
		  }
	  }

/*	
	if(IS_NPC(victim)) 
	  {
		for ( i = 4; i > 0; i-- )
		  {
			if (gch->pcdata->trophyvnum[i] == victim->pIndexData->vnum)
			  {
				vnum_found = TRUE;
				count = gch->pcdata->trophycount[i];
			  }
		  }
		if (vnum_found)
		  
			if (count < 5)
				xp *= 1;
			else if (count < 7)
				xp *= 0.80;		
			else if (count < 10)
				xp *= 0.60;
			else if (count < 13)
				xp *= 0.40;
			else if (count < 16)
				xp *= 0.20;
			else 
				xp = 0;
		   }
		 else
		   {
				xprintf_2( log_buf, "xp Compute BAD vnum %s )",gch->name);
				log_string( log_buf );
		  }
		 
	  }
*/
    if (level_range > 15) 
      {
		xprintf_2( log_buf, "%s just gained %d from killing [%d] %s (Level Difference %d )",gch->name,xp,victim->pIndexData->vnum,victim->name,level_range);
		log_string( log_buf );
		wiznet(log_buf,gch,NULL,WIZ_CHEAT,0,0);
      }      

	if (level_range > 50)
		do_autofreeze(gch);

  return xp;
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

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
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

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cSuicide is a mortal sin.{x\n\r", ch );
	return;
    }

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
	  xprintf(buf, "Help! I am being attacked by %s!{x",ch->short_descr);
	else
	  xprintf( buf, "Help!  I am being attacked by %s!{x",
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
    
    if (ch->submission > 0)
    {
        send_to_char( "How? By leaving your arm behind?.\n\r", ch );
        act( "$n attempts to flee but a firm grip in his arm prevents it.", ch, NULL, NULL, TO_ROOM );
        return;
    }
    
    if (IS_AFFECTED2(ch, AFF2_SUBMISSION))
    {
    	send_to_char( "You let go of your opponents arm as you attempt to flee.\n\r", ch );
    	act( "$n lets go of your arm and heads for the nearest exit.", ch, NULL, victim, TO_VICT );
    	affect_strip(ch,gsn_submission);
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

	if (was_in == NULL)
	  continue;
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

	ch->move = ch->move * 0.9;
	}

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

    if (IS_NPC(ch))
      return;

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


	stop_fighting( ch, TRUE );
	return;

}


void do_guard(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  one_argument(argument, arg);

  if (!kingdom_ok(ch,gsn_guard))
    return;

  if (IS_NPC(ch) || ch->pcdata->learned[gsn_guard] < 40 ||
        ch->level < skill_table[gsn_guard].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if ( arg[0] == '\0' )
    {
      send_to_char("{cGuard whom?{x\n\r", ch);
      return;
    }

  if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
      send_to_char( "{cThey aren't here.{x{x\n\r", ch );
      return;
    }

  if (IS_NPC(victim))
    {
      act("{c$N doesn't want your pity!{x", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (!str_cmp(arg, "none") || !str_cmp(arg, "self") || victim == ch)
    {
	if (ch->guarding == NULL)
	  {
	    send_to_char("{cYou can't guard yourself!{x\n\r", ch);
	    return;
	  }
	else
	{
      act("{cYou stop guarding $N.{x", ch, NULL, ch->guarding, TO_CHAR);
      act("{c$n stops guarding you.{x", ch, NULL, ch->guarding, TO_VICT);
      act("{c$n stops guarding $N.{x", ch, NULL, ch->guarding, TO_NOTVICT);
      ch->guarding->guarded_by = NULL;
      ch->guarding             = NULL;
      return;
        }
    }

  if (ch->guarding == victim)
    {
      act("{cYou're already guarding $N!{x", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (ch->guarding != NULL)
    {
      send_to_char("{cBut you're already guarding someone else!{x\n\r",ch);
      return;
    }

  if (victim->guarded_by != NULL)
    {
      act("{c$N is already being guarded by someone.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (victim->guarding == ch)
    {
      act("{cBut $N is guarding you!{x", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (!is_same_group(victim, ch))
    {
      act("{cBut you aren't in the same group as $N.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_AFFECTED(ch,AFF_CHARM))
    {
      act("{cYou like your master too much to bother guarding $N!{x",
	  ch,NULL,victim,TO_VICT);
      return;
    }

 if (victim->fighting != NULL)
   {
     send_to_char("{cWhy don't you let them stop fighting first?{c\n\r", ch);
     return;
   }

  if (ch->fighting != NULL)
    {
      send_to_char
    ("{cBetter finish your own battle before you worry about guarding someone else.{x\n\r",
     ch);
      return;
    }

  act("{cYou are now guarding $N.{x", ch, NULL, victim, TO_CHAR);
  act("{cYou are being guarded by $n.{x", ch, NULL, victim, TO_VICT);
  act("{c$n is now guarding $N.{x", ch, NULL, victim, TO_NOTVICT);

  ch->guarding = victim;
  victim->guarded_by = ch;

}








/*
 * critical strike
*/
int critical_strike( CHAR_DATA *ch, CHAR_DATA *victim,   int dam )
{
  int diceroll;
  AFFECT_DATA baf;

/*
  if ( get_eq_char(ch,WEAR_WIELD) == NULL
        && get_eq_char(ch,WEAR_SECONDARY) == NULL
      && number_percent() > ( (ch->hit * 100) / ch->max_hit ) )
      return 0;
*/

  if ( !IS_NPC(ch) && /* IS_SET( ch->off_flags, OFF_CRIT ) ) ||*/
         get_skill( ch, gsn_critical ) > 0 )
  {

    diceroll = number_range( 0, 100 );
    if( victim -> level > ch -> level )
      diceroll += ( victim->level - ch->level ) * 2;
    if( victim -> level < ch -> level )
      diceroll -= ( ch->level - victim->level );

    if ( (!IS_NPC(ch)) && (diceroll <= (get_skill( ch, gsn_critical )/2))
)
    {
      check_improve( ch, gsn_critical, TRUE, 2 );
      dam += dam * diceroll/200;
  }

    if ( (IS_NPC(ch) && (diceroll <= (ch->level / 13))) ||
         (!IS_NPC(ch) && (diceroll <= (get_skill(ch, gsn_critical)/13)) )
)
    {
      diceroll = number_percent( );
      if( diceroll < 75 )
      {
        act_new( "$n takes you down with a weird judo move!",
             ch, NULL, victim, TO_VICT,POS_RESTING);
        act_new( "You take $N down with a weird judo move!",
             ch, NULL, victim, TO_CHAR,POS_RESTING);
        if (!IS_NPC(ch))
          check_improve( ch, gsn_critical, TRUE, 3 );
       // WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
       
       	victim->position = POS_RESTING;
	/*setting lags*/
        if (victim->lagattack < 6)
	  victim->lagattack = 6;
	if (victim->lagflee   < 6)
	 victim->lagflee   = 6;
	if (victim->lagstand   <6)
	  victim->lagstand  = 6;
	if (victim->lagspell   < 6)
	  victim->lagspell = 6;
        dam += (dam * number_range( 2, 5 )) / 5;
        return(dam);
      }
   else if( diceroll > 75 && diceroll < 95 )
      {
        act_new( "You are blinded by $n's attack!", ch, NULL, victim,
                        TO_VICT ,POS_RESTING);
        act_new( "You blind $N with your attack!", ch, NULL, victim,
                        TO_CHAR,POS_RESTING);
        if (!IS_NPC(ch))
          check_improve( ch, gsn_critical, TRUE, 4 );
        if (!IS_AFFECTED(victim,AFF_BLIND))
        {
          baf.type = gsn_dirt;
          baf.level = ch->level;
          baf.location = APPLY_HITROLL;
          baf.modifier = -4;
          baf.duration = number_range(1,5);
          baf.bitvector = AFF_BLIND;
          affect_to_char( victim, &baf );
    }
        dam += dam * number_range( 1, 2 );
        return(dam);
      }
      else if( diceroll > 95 )
      {
        act_new( "$n does a flying jump kick.  OUCH!!",
             ch, NULL, victim, TO_VICT ,POS_RESTING);
        act_new( "You does a flying jump kick at $N!  I bet that hurt!",
             ch, NULL, victim, TO_CHAR ,POS_RESTING);
        if (!IS_NPC(ch))
          check_improve( ch, gsn_critical, TRUE, 5 );
        dam += dam * number_range( 2, 5 );
        return(dam);
      }
    }
  }

  return dam;
}

