/****************************************************************************
*     Fate of Kingdom 4.0 is copyright 2000-???? Christian Tusborg (Kazmir) *
*     By using this code, you have agreed to follow the terms of the        *
*     Fate license.  The use of this code without the permission of the     *
*     author is prohibited.  You will be punished to the fullest extend of  *
*     the law.								    *
*     									    *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		    *
****************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "act.h"
#include "magic.h"

void  	do_act_warrior	args( ( CHAR_DATA *ch) );
void  	do_act_cleric	args( ( CHAR_DATA *ch) );
void  	do_act_mage	args( ( CHAR_DATA *ch) );
//void  	do_act_thief	args( ( CHAR_DATA *ch) );

const	struct	act_table_type	act_table	[] =
{
	{ "warrior",	ACT_WARRIOR,	do_act_warrior },
	{ "cleric",	ACT_CLERIC,	do_act_cleric },
	{ "mage",	ACT_MAGE,	do_act_mage },
//	{ "thief",	ACT_THIEF,	do_act_thief },
//	{ "quaff",	ACT_QUAFF,	do_act_quaff },
	{ NULL,		0,		0 }
};



CHAR_DATA * get_friend(CHAR_DATA *ch)
{
return NULL;
}


void violence_update_act(void)
{
  int	act = 0;
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
//bug("I win you lose",0);
 	for ( ch = char_list; ch != NULL; ch = ch_next )
 	  {
		ch_next = ch->next;
		
		for (act = 0;act_table[act].name != NULL; act++)
	  	 {
			if (IS_SET(ch->act,act_table[act].class_act))
				(*act_table[act].do_act) (ch);
		  }
	  }
} 

void	do_act_warrior(CHAR_DATA *ch)
{
	
return;
	CHAR_DATA *victim;
//	CHAR_DATA *friend = get_friend(ch);
	OBJ_DATA *obj;
	OBJ_DATA *weapon;
	OBJ_DATA *weapon_next;
	char weaponname[MAX_STRING_LENGTH];
	
	  victim = ch->last_fought;
	  obj = get_eq_char(ch, WEAR_WIELD);
	  
  if (!IS_NPC(ch))
  return;
	  
  if (ch->position < POS_FIGHTING)
   do_stand(ch, "");
   
   
   for ( weapon = ch->in_room->contents; weapon != NULL; weapon = weapon_next )
    {
	weapon_next = weapon->next_content;
	if (IS_SET( weapon->wear_flags, ITEM_WIELD))
	{
	     xprintf( weaponname, "%s", weapon->name );
	    one_argument(weaponname, weaponname);
	    do_get( ch, weaponname );
	}
	    if (get_eq_char(ch, WEAR_WIELD) == NULL)
	    do_wear(ch, weaponname);
	    continue;
    }
  
  if (victim == NULL)
     return;
     
     
  if (ch->position == POS_STANDING && (victim->in_room == ch->in_room))
  {
  	if ((obj != NULL) && ((obj->value[0] == WEAPON_POLEARM) || (obj->value[0] == WEAPON_LANCE)))
    	{    
    	    do_charge( ch, victim->name );
    	    return;
    	}
    	else if ((victim->size-1 <= ch->size <= victim->size+2)
    	 	&& (victim->position >= POS_FIGHTING))
  	{
  	    do_bash( ch, victim->name );
  	    return;
  	}
  /*	else if ((ch->hit > ch->max_hit/2) && (friend->position == POS_FIGHTING)) 
  	{
  	    do_rescue( ch, friend->name );
  	    return;
  	}*/
  	else
  	{
  	    multi_hit( ch, victim, TYPE_UNDEFINED );
  	    return;
  	}
  }
 
   if (ch->position == POS_FIGHTING)
   {
   switch (number_range(0,7))
   {
   case 0:
   if (!IS_AFFECTED(victim,AFF_BLIND))
     {
   	if (ch->level > 75)
       	do_gouge( ch, victim->name );
   	else
       	do_dirt( ch, victim->name );
      }
   else
      if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
      do_disarm( ch, victim->name );
   break;
   case 1:
   if ((victim->size-1 <= ch->size <= victim->size+2)
      && (victim->position >= POS_FIGHTING))
      do_bash( ch, victim->name );
   else
      do_trip( ch, victim->name );
   break;
   case 3:
//   if ((ch->hit > ch->max_hit/2) && (friend->position == POS_FIGHTING))
 //     do_rescue( ch, friend->name );
   break;
   case 4:
   if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
      do_disarm( ch, victim->name );
   break;
   case 5:
   break;
   case 6:
   break;
   case 7:
   break;
   } 
  }    
//  if ((victim != NULL) && (victim->hit < 20))
//  ch->last_fought = NULL;
  
  return;
}

/*void	do_act_quaff(CHAR_DATA *ch)
{
	OBJ_DATA *obj;
	
	for(obj=ch->carrying;obj!=NULL;obj=obj->next_content)
		 if ( obj->item_type == ITEM_POTION )
		 {
		   if ( ch->hit < ch->max_hit*0.9 )  // hp curies 
		   {
		   int cl;	
		     cl=potion_cure_level( obj );
		     if (cl > 0)
		     {
			if ( ch->hit<ch->max_hit*0.5 && cl > 3 )  {
			  do_quaff( ch, obj->name );
			  continue;
			}
			else if ( ch->hit<ch->max_hit*0.7 )  {
			  do_quaff( ch, obj->name );
			  continue;
			}
		     }
		   }
	     	   if ( IS_AFFECTED(ch,AFF_POISON) && potion_cure_poison(obj) )
		   {
		     do_quaff( ch, obj->name );
		     continue;
		   }
	     	   if ( IS_AFFECTED(ch,AFF_PLAGUE) && potion_cure_disease(obj) )
		   {
		     do_quaff( ch, obj->name );
		     continue;
		   }
	     	   if ( IS_AFFECTED(ch,AFF_BLIND) && potion_cure_blind(obj) )
		   {
		     do_quaff( ch, obj->name );
		     continue;
		   }
	     	   if ( ch->fighting != NULL )
		   {
		   int al;
		     al = potion_arm_level( obj );
		     if ( ch->level-ch->fighting->level < 7 && al>3)
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     if ( ch->level-ch->fighting->level < 8 && al>2 )
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     if ( ch->level-ch->fighting->level < 9 && al>1 )
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     if ( ch->level-ch->fighting->level < 10 && al>0 )
		     {
		     	do_quaff( ch, obj->name );
			continue;
		     }
		     else 
		        continue;
		   }
		 }  */

void	do_act_cleric(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
//	CHAR_DATA *friend;

  if (!IS_NPC(ch))
  return;
  
  if (is_affected(ch, gsn_deafen))
    return;  
  
  victim = ch->last_fought;
  if (ch->position < POS_FIGHTING)
   {
   do_stand(ch, "");
   if (1 == number_range (1,3))
    return;
  }
  if (ch->position == POS_STANDING) 
  {    
   if (ch->level < 40) 
   {
     if ((ch->hit < ch->max_hit/1.5) && (ch->position == POS_STANDING))
     {
      act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
      spell_cure_critical( skill_lookup( "cure critical" ), ch->level,ch,ch,TARGET_CHAR);
      return;
     }
   }
   else
   {
   	if ((ch->hit < ch->max_hit/1.5) && (ch->position == POS_STANDING) && (1 == number_range (1,50)))
   	{
   	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   	spell_rejuvinate( skill_lookup( "rejuvinate" ), ch->level,ch,ch,TARGET_CHAR);
        return;
        }
   }
   
   if (!IS_AFFECTED(ch,AFF_SANCTUARY))
   {
  	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
  	spell_sanctuary( skill_lookup( "sanctuary" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
   if (!is_affected(ch,gsn_blade_barrier) && (ch->level >= 77))
    {
  	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
  	spell_bless( skill_lookup( "blade barrier" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
   if (!is_affected(ch,gsn_bless))
    {
  	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
  	spell_bless( skill_lookup( "bless" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
     if (IS_AFFECTED(ch,AFF_POISON)){
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_cure_poison( skill_lookup( "cure poison" ), ch->level*1.5,ch,ch,TARGET_CHAR); 
   return;
   }
     if (IS_AFFECTED(ch,AFF_PLAGUE)){
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_cure_disease( skill_lookup( "cure disease" ), ch->level*1.5,ch,ch,TARGET_CHAR); 
   return;
   }
 }
   if (victim == NULL)
      return;
   if (ch->position == POS_STANDING) 
  {      
   if ((!IS_AFFECTED(ch,AFF_PROTECT_EVIL)) && (!IS_AFFECTED(ch,AFF_PROTECT_GOOD)) && IS_EVIL(victim))
   {
  	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
  	spell_protection_evil( skill_lookup( "protection evil" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
   
   if ((!IS_AFFECTED(ch,AFF_PROTECT_EVIL)) && (!IS_AFFECTED(ch,AFF_PROTECT_GOOD)))
   {
  	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
  	spell_protection_good( skill_lookup( "protection good" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
   
     if ((victim->in_room->area == ch->in_room->area) && ( !IS_AFFECTED2(victim, AFF2_INVUL))
        && (!is_affected(ch, gsn_invul)) && (ch->level > 50) && (victim->level < ch->level +10))
   {
  	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
  	spell_invul( skill_lookup( "aura of invulnerabilty" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
 }      
        
     if (victim->in_room != ch->in_room && (ch->position == POS_STANDING) && (victim->position != POS_FIGHTING)
	&&  (victim->in_room->area == ch->in_room->area) && (!IS_SET(victim->in_room->room_flags, ROOM_SAFE))
	&& (!IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)) && (!IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON))
	&& (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) && (ch->level > victim->level-5) && can_see( ch, victim ))
	{
	act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
	act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
    	char_from_room( victim );
    	char_to_room( victim, ch->in_room );
    	act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
    	act( "$n has summoned you!", ch, NULL, victim,   TO_VICT );
    	do_look( victim, "auto" );
    	return;
       	}

 if ((ch->position == POS_STANDING) && (victim->in_room == ch->in_room))
 {
  act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
  spell_demonfire( skill_lookup( "demonfire" ), ch->level,ch,victim,TARGET_CHAR); 
  return;
 }
 
 if (ch->position == POS_FIGHTING)
  {
   if ((ch->hit < ch->max_hit/4) && (number_percent() > 95)){
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_calm( skill_lookup( "calm" ), ch->level*1.5,ch,victim,TARGET_CHAR); 
   return;
   }
   if (IS_AFFECTED(ch,AFF_BLIND)){
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_cure_blindness( skill_lookup( "cure blindness" ), ch->level*1.5,ch,ch,TARGET_CHAR); 
   return;
   }
   if (ch->level < 40)
   { 
   switch (number_range(0,7))
   {
   case 0:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   if (IS_EVIL(victim))
   spell_wrath( skill_lookup( "wrath" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_demonfire( skill_lookup( "demonfire" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 1:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 3:
   if (ch->hit < ch->max_hit)
   {
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_heal( skill_lookup( "heal" ), ch->level,ch,ch,TARGET_CHAR); 
   }
   break;
   case 4:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_flamestrike( skill_lookup( "flamestrike" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 5:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_curse( skill_lookup( "curse" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 6:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   if (IS_EVIL(victim))
   spell_wrath( skill_lookup( "wrath" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_demonfire( skill_lookup( "demonfire" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 7:
   if (ch->hit < ch->max_hit)
   {
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_heal( skill_lookup( "heal" ), ch->level,ch,ch,TARGET_CHAR); 
   }
   break;
   }
   }
   else
   {
   switch (number_range(0,7))
   {
   case 0:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   if (IS_EVIL(victim))
   spell_ray_of_truth( skill_lookup( "ray of truth" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_hellfire( skill_lookup( "hellfire" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 1:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 3:
   if (ch->hit < ch->max_hit)
   {
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_restoration( skill_lookup( "restoration" ), ch->level,ch,ch,TARGET_CHAR); 
   }
   break;
   case 4:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   if (IS_EVIL(victim))
   spell_ray_of_truth( skill_lookup( "ray of truth" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_hellfire( skill_lookup( "hellfire" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 5:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_curse( skill_lookup( "curse" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 6:
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   if (IS_EVIL(victim))
   spell_ray_of_truth( skill_lookup( "ray of truth" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_hellfire( skill_lookup( "hellfire" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 7:
   if (ch->hit < ch->max_hit)
   {
   act( "$n breathes deeply as he closes his eyes in communion.", ch, NULL, NULL, TO_ROOM );
   spell_restoration( skill_lookup( "restoration" ), ch->level,ch,ch,TARGET_CHAR); 
   }
   break;
   }
  }
}  
  if ((victim != NULL) && (victim->hit < 20))
  ch->last_fought = NULL;
  
  return;
}

void	do_act_mage(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *vch;
    	CHAR_DATA *vch_next;
//	CHAR_DATA *zonemob = NULL;
//	CHAR_DATA *friend;
//	long mcounter;
//	int mob_vnum;
	int room_vnum;
//	MOB_INDEX_DATA *vsearch = NULL;
//	bool found = FALSE;
//	int looptimes;
	

  
  
  if (!IS_NPC(ch))
  return;
  
  if (is_affected(ch, gsn_deafen))
    return;

  
  victim = ch->last_fought;
  if (ch->position < POS_FIGHTING)
   do_stand(ch, "");
 
  if (ch->position == POS_STANDING) 
  {    
  
   if (!is_affected(ch,gsn_fire_shield) && (ch->level >= 40))
   {
  	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
  	spell_fire_shield( skill_lookup( "fire shield" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
   if (!is_affected(ch,gsn_haste) && (ch->level >= 30))
    {
  	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
  	spell_haste( skill_lookup( "haste" ), ch->level,ch,ch,TARGET_CHAR); 
  	return;
   }
     if (!is_affected(ch,gsn_magic_resistance))
   {
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_magic_resistance( skill_lookup( "magic resistance" ), ch->level*1.5,ch,ch,TARGET_CHAR); 
   	return;
   }
     if (!is_affected(ch,gsn_protective_shield))
   {
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_protective_shield( skill_lookup( "protective shield" ), ch->level,ch,ch,TARGET_CHAR); 
   	return;
   }
   if (!IS_AFFECTED(ch,AFF_FLYING))
   {
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_fly( skill_lookup( "fly" ), ch->level,ch,ch,TARGET_CHAR); 
   	return;
   }
   
   if (!is_affected(ch,gsn_master_sight) && (ch->level >= 40))
   {
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_master_sight( skill_lookup( "master sight" ), ch->level,ch,ch,TARGET_CHAR); 
   	return;
   }
   else if ( !CAN_DETECT(ch, DETECT_INVIS))
   {
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_detect_invis( skill_lookup( "detect invis" ), ch->level,ch,ch,TARGET_CHAR); 
   	return;
   }
   if (!is_affected(ch,gsn_giant_strength))
   {
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_giant_strength( skill_lookup( "giant strength" ), ch->level,ch,ch,TARGET_CHAR); 
   	return;
   }
   
 
}
 if (victim == NULL)
    if (ch->fighting != NULL)
    victim = ch->fighting;
    
 if (victim == NULL)
 return;
 
 
 if ((ch->position == POS_STANDING) && (victim->in_room != ch->in_room)
    && can_gate(ch,victim))
    {  
	act("$n steps through a gate and vanishes.",ch,NULL,NULL,TO_ROOM);
	char_from_room( ch );
	char_to_room( ch, victim->in_room );
	act("$n has arrived through a gate.",ch,NULL,NULL,TO_ROOM);
	return;
    }

 if ((ch->position == POS_STANDING) && (victim->in_room == ch->in_room))
 {
 	if (!is_affected(ch,gsn_blindness) && !can_see(ch, victim)
 	   && !is_affected(ch,gsn_dirt) && !is_affected(ch,gsn_gouge))
 	{
 		spell_faerie_fog( skill_lookup( "faerie fog" ), ch->level,ch,ch,TARGET_CHAR);
 		return;
 	}
 	

 	if (victim->position == POS_SLEEPING)
 	{
 		switch (number_range(0,11))
 		{
 			case 0:
 			if (!is_affected(ch,gsn_weaken))
			{
   			act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   			spell_weaken( skill_lookup( "weaken" ), ch->level,ch,victim,TARGET_CHAR); 
   			} 
  			break;
  			case 1:
  			if (!is_affected(ch,gsn_blindness))
			{
   			act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   			spell_blindness( skill_lookup( "blindness" ), ch->level,ch,victim,TARGET_CHAR); 
   			}
  			break;
  			case 2:
  			if (!is_affected(ch,gsn_faerie_fire))
			{
   			act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   			spell_faerie_fire( skill_lookup( "faerie_fire" ), ch->level,ch,victim,TARGET_CHAR); 
   			}
  			break;
  			case 3:
  			if (!is_affected(ch,gsn_curse))
			{
   			act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   			spell_curse( skill_lookup( "curse" ), ch->level,ch,victim,TARGET_CHAR); 
   			}
  			break;
  			case 4:
  			if (!is_affected(ch,gsn_slow))
			{
   			act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   			spell_slow( skill_lookup( "slow" ), ch->level,ch,victim,TARGET_CHAR); 
   			}
  			break;
  			case 5:
  			if (number_range(0,7) <= 2 && ch->level >= 100)
			{
   			act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   			spell_amnesia( skill_lookup( "amnesia" ), ch->level,ch,victim,TARGET_CHAR); 
   			}
  			break;
  			case 6:
  			act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
  			if (ch->level <= 9)
   			spell_magic_missile( skill_lookup( "magic missile" ), ch->level,ch,victim,TARGET_CHAR); 
   			else if (ch->level <= 15)
   			spell_shocking_grasp( skill_lookup( "shocking grasp" ), ch->level,ch,victim,TARGET_CHAR); 
   			else if (ch->level <= 21)
   			spell_colour_spray( skill_lookup( "colour spray" ), ch->level,ch,victim,TARGET_CHAR); 
   			else if (ch->level <= 29)
   			spell_fireball( skill_lookup( "fireball" ), ch->level,ch,victim,TARGET_CHAR); 
   			else if (ch->level <= 39)
   			spell_acid_blast( skill_lookup( "acid blast" ), ch->level,ch,victim,TARGET_CHAR); 
   			else if (ch->level <= 63)
   			spell_phantasmal_force( skill_lookup( "phantasmal force" ), ch->level,ch,victim,TARGET_CHAR); 
   			else if (ch->level <= 80)
   			spell_firewind( skill_lookup( "firewind" ), ch->level,ch,victim,TARGET_CHAR); 
   			else
   			spell_multi_missile( skill_lookup( "multi missile" ), ch->level,ch,victim,TARGET_CHAR); 
  			break;
  			case 7:		
  			break;
  			case 8:		
  			break;
  			case 9:		
  			break;
  			case 10:		
  			break;
  			case 11:		
  			break;	
  		}
  	}
  	else if ((ch->level >= victim->level + 5) && (number_range(1,2) == 2))
    	{
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_sleep( skill_lookup( "sleep" ), ch->level,ch,victim,TARGET_CHAR); 
   	return;
	}
	else
	{
		act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
  		if (ch->level <= 9)
   		spell_magic_missile( skill_lookup( "magic missile" ), ch->level,ch,victim,TARGET_CHAR); 
   		else if (ch->level <= 15)
   		spell_shocking_grasp( skill_lookup( "shocking grasp" ), ch->level,ch,victim,TARGET_CHAR); 
   		else if (ch->level <= 21)
   		spell_colour_spray( skill_lookup( "colour spray" ), ch->level,ch,victim,TARGET_CHAR); 
   		else if (ch->level <= 29)
   		spell_fireball( skill_lookup( "fireball" ), ch->level,ch,victim,TARGET_CHAR); 
   		else if (ch->level <= 39)
   		spell_acid_blast( skill_lookup( "acid blast" ), ch->level,ch,victim,TARGET_CHAR); 
   		else if (ch->level <= 63)
   		spell_phantasmal_force( skill_lookup( "phantasmal force" ), ch->level,ch,victim,TARGET_CHAR); 
   		else if (ch->level <= 80)
   		spell_firewind( skill_lookup( "firewind" ), ch->level,ch,victim,TARGET_CHAR); 
   		else
   		spell_multi_missile( skill_lookup( "multi missile" ), ch->level,ch,victim,TARGET_CHAR); 
   	return;
	}
  }

 
 if (ch->position == POS_FIGHTING)
  {
  	
  	if ((ch->hit < ch->max_hit/10)
    	&& (number_range(1,6) < 3 ) )
    	{
    	room_vnum = find_gate_mobile_in_zone(ch);
    	if (room_vnum > 0)
		{
	//		act("$n steps through a gate and vanishes.",ch,NULL,NULL,TO_ROOM);
  //  			char_from_room( ch );
  //  			char_to_room( ch, get_room_index(room_vnum) );
  //  			act("$n has arrived through a gate.",ch,NULL,NULL,TO_ROOM);
    			return;
    		}
    	}
	else if ((ch->hit < ch->max_hit/5) && ( !CAN_DETECT(victim, DETECT_INVIS))
      	&& (number_range(1,6) < 3) )
     	{
      	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
      	spell_shadow_door( skill_lookup( "shadow door" ), ch->level,ch,ch,TARGET_CHAR);
      	ch->last_fought = NULL;
      	return;
     	}
   if (IS_NPC(ch->fighting) && number_range(1,2) == 1)
   {
   	act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   	spell_teleport( skill_lookup( "teleport" ), ch->level,ch,ch->fighting,TARGET_CHAR);
   	return; 
   }
   
   ch->fighting = victim;
   
   if (ch->level < 40)
   {
   act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   switch (number_range(0,8))
   {
   case 0:
   spell_colour_spray( skill_lookup( "colour spray" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 1:
   spell_magic_missile( skill_lookup( "magic_missile" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 3:
   spell_shocking_grasp( skill_lookup( "shocking grasp" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 4:
   spell_acid_blast( skill_lookup( "acid blast" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 5:
   spell_fireball( skill_lookup( "fireball" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 6:
   spell_phantasmal_force( skill_lookup( "phantasmal force" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 7:
   if ((is_affected(victim,gsn_sanctuary)) || (is_affected(ch,gsn_haste)))
   spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_poison( skill_lookup( "poison" ), ch->level,ch,victim,TARGET_CHAR);
   break;
   case 8:
   spell_lightning_bolt( skill_lookup( "lightning bolt" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   }
   }
   else
   {
   act( "$n traces symbols in the air with $s fingers.", ch, NULL, NULL, TO_ROOM );
   switch (number_range(0,15))
   {
   case 0:
   spell_firewind( skill_lookup( "firewind" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 1:
   if ((is_affected(victim,gsn_sanctuary)) || (is_affected(ch,gsn_haste)))
   spell_dispel_magic( skill_lookup( "dispel magic" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_blindness( skill_lookup( "blindness" ), ch->level,ch,victim,TARGET_CHAR);
   break;
   case 3:
   if (!is_affected(victim,gsn_faerie_fire))
   spell_faerie_fire( skill_lookup( "faerie fire" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_slow( skill_lookup( "slow" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 4:
   spell_multi_missile( skill_lookup( "multi missile" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 5:
   for ( vch = char_list; vch != NULL; vch = vch_next )
    {
    	vch_next	= vch->next;
	if ( vch->in_room == ch->in_room )
	{
	    if (vch->position != POS_FIGHTING)
	    {
	    	spell_teleport( skill_lookup( "teleport" ), ch->level,ch,victim,TARGET_CHAR);
	    	break;
	    }
	     else
	     {
	     if (number_range(1,3) == 1)
	     spell_gas_breath( skill_lookup( "gas breath" ), ch->level,ch,victim,TARGET_CHAR);
	     else if (number_range(1,3) == 2)
	     spell_chain_lightning( skill_lookup( "chain lightning" ), ch->level,ch,victim,TARGET_CHAR);
	     else
	     spell_meteor_swarm( skill_lookup( "meteor swarm" ), ch->level,ch,victim,TARGET_CHAR);
	     break;
	     }
	}
	
    } 
   break;
   case 6:
   if (!is_affected(victim,gsn_blindness))
   spell_blindness( skill_lookup( "blindness" ), ch->level,ch,victim,TARGET_CHAR); 
   else
   spell_plague( skill_lookup( "plague" ), ch->level,ch,victim,TARGET_CHAR); 
   break;
   case 7:
   if (!is_affected(victim,gsn_weaken))
   spell_weaken( skill_lookup( "weaken" ), ch->level,ch,victim,TARGET_CHAR);
   else
   spell_firewind( skill_lookup( "firewind" ), ch->level,ch,victim,TARGET_CHAR);
   break;
   }
  }
}  
  if ((victim != NULL) && (victim->hit < 20))
  ch->last_fought = NULL;
  
  return;
}

