/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/***************************************************************************
                         Damage/Fight Check
****************************************************************************/

//bool check_mirror(CHAR_DATA *ch, CHAR_DATA *victim)
//bool check_phases(CHAR_DATA *victim)
//bool check_esp(CHAR_DATA *ch,CHAR_DATA *victim)
//bool check_pixie_quickly(CHAR_DATA *victim)
//bool check_aura_of_invulnerability(CHAR_DATA *ch, CHAR_DATA *victim)
//bool check_sanctuary(CHAR_DATA *victim,int dt)
//bool check_cheat_damage(CHAR_DATA *ch,int dam)
//bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim )
//bool check_block( CHAR_DATA *ch, CHAR_DATA *victim )
//bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim )
//bool check_lightning_reflexes( CHAR_DATA *ch, CHAR_DATA *victim )
//bool check_draggon_speed( CHAR_DATA *ch, CHAR_DATA *victim )
//bool check_battle_prowess( CHAR_DATA *ch, CHAR_DATA *victim )
//bool check_obj_dodge( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, int bonus )
//CHAR_DATA *check_guard(CHAR_DATA *ch, CHAR_DATA *mob)
//void check_assist(CHAR_DATA *ch,CHAR_DATA *victim)
//bool check_counter(CHAR_DATA *ch,CHAR_DATA *victim) 

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "declaration.h"


bool check_mirror(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if ( ch != victim)
	  {
		if (is_affected(victim,gsn_mirror)) 
		  {
			act("{c$n shatters into tiny fragments of glass.{x",victim,NULL,NULL,TO_ROOM);
			extract_char(victim,TRUE);
			return TRUE;
	  }		  }
return FALSE;
}

bool check_phases(CHAR_DATA *victim)
{
	if ( IS_AFFECTED2(victim, AFF2_PHASING))
	  {
		if (number_percent( ) < 40)
		  {
			act( "{c$n phases out of existance and the attack passes directly through!{x",victim, NULL, NULL, TO_ROOM );
			send_to_char("You avoid the attack by phasing.\n\r",victim);
			return TRUE;
		  }
	  }
return FALSE;
}


bool check_esp(CHAR_DATA *ch)
{
	if (IS_AFFECTED2(ch, AFF2_HOLD))
	  {
		act( "{c$n tries to attack but is held by ESP!!{x",ch, NULL, NULL, TO_ROOM );
		return TRUE;
	  }
return FALSE;
}


bool check_pixie_quickly(CHAR_DATA *victim)
{
	if ((victim->race == race_lookup("pixie")) && (number_percent() < 15 ))
	  {
		act( "{c$n flitters out of harms way.{x",victim, NULL, NULL, TO_ROOM );
		send_to_char("You react quickly and fly out of harms way.\n\r", victim);
		return TRUE;
	  }

return FALSE;
}

bool check_aura_of_invulnerability(CHAR_DATA *ch, CHAR_DATA *victim)
{
  char buf[MAX_STRING_LENGTH];

	if ( IS_AFFECTED2(victim, AFF2_INVUL))
	  {
		act( "{c$n's aura of invulnerability absorb attacks striking him!{x",victim, NULL, NULL, TO_ROOM );
		xprintf(buf,"{cYour aura of invulnerability absorbs %s's attack!{x\n\r",PERS(victim,ch));
		send_to_char(buf,victim);
		return TRUE;
	  }
return FALSE;
}

bool check_sanctuary(CHAR_DATA *victim,int dt)
{
	if ( IS_AFFECTED(victim, AFF_SANCTUARY)
	 && !( (dt == gsn_cleave)&& (number_percent() < 50) ) 
	 && !(dt == gsn_jab) )
		return TRUE;
		
		
return FALSE;
}


bool check_cheat_damage(CHAR_DATA *ch,int dam)
{

  char buf[MAX_STRING_LENGTH];

    if ( dam > 2000 && !IS_IMMORTAL(ch) && !IS_NPC(ch))
    {
	bug( "Damage: %d: more than 2000 points!{x", dam );
	dam = 2000;

	if (!IS_IMMORTAL(ch))
	{
	    OBJ_DATA *obj;
	    obj = get_eq_char( ch, WEAR_WIELD );
	xprintf(buf,"%s is doing %d damage with %s.",ch->name,dam,obj->name);
	bug(buf,0);
	    send_to_char("{cYour weapon is vaporized with that extraordinary hit.{x\n\r",ch);
	    if (obj)
	      extract_obj(obj);
	  return TRUE;    
	}
    }
return FALSE;
}

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
  //      chance -= GET_HITROLL(ch) /6;
        
   
    if (((weather_info.sunlight == SUN_SET) || (weather_info.sunlight == SUN_DARK)
    || (IS_OUTSIDE(ch))) && (ch->race == race_lookup("drow")))
    chance -= GET_HITROLL(ch) /2;
    else
    chance -= GET_HITROLL(ch) /6;
    
    
      

 if (is_affected(ch,gsn_aim))
    chance += (ch->pcdata->learned[gsn_aim] / (100 - ch->level)) ;
        
    	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
			chance -= 3;
    	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,OFF_FAST))
		chance += 3;

    if (((weather_info.sunlight == SUN_SET) || (weather_info.sunlight == SUN_DARK)
    || (IS_OUTSIDE(victim))) && (victim->race == race_lookup("drow")))
    chance += victim->level/4;
    
    if (((weather_info.sunlight == SUN_RISE) || (weather_info.sunlight == SUN_LIGHT)
	||(IS_OUTSIDE(victim))) && (victim->race == race_lookup("drow")))
    chance -= ch->level/8;
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
			chance -= 15;
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

  if (IS_AFFECTED2(ch, AFF2_SUBMISSION) || ch->submission > 0)
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

        if (( number_percent() < split ) 
        && !IS_SET(shield->extra_flags,ITEM_INDESTRUCT) 
        && !IS_SET(shield->extra_flags,ITEM_INDESTRUCT)){
          broken = create_object( get_obj_index( OBJ_VNUM_BROKEN ), 0 );
          xprintf( buf, broken->name, shield->name );
          free_string( broken->name );
          broken->name = str_dup( buf );

          xprintf( buf, broken->short_descr, shield->short_descr );
          free_string( broken->short_descr );
          broken->short_descr = str_dup( buf );

          xprintf( buf, broken->description, shield->short_descr );
          buf[0] = UPPER(buf[0]);
          free_string( broken->description );
          broken->description = str_dup( buf );

          xprintf( buf, broken->material, shield->material );
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
    //    chance -= GET_HITROLL(ch) /6;
    
    if (((weather_info.sunlight == SUN_SET) || (weather_info.sunlight == SUN_DARK)
    || (IS_OUTSIDE(ch))) && (ch->race == race_lookup("drow")))
    chance -= GET_HITROLL(ch) /2;
    else
    chance -= GET_HITROLL(ch) /6;
    
   if (((weather_info.sunlight == SUN_SET) || (weather_info.sunlight == SUN_DARK)
    || (IS_OUTSIDE(victim))) && (victim->race == race_lookup("drow")))
    chance += victim->level/4;
    
    if (((weather_info.sunlight == SUN_RISE) || (weather_info.sunlight == SUN_LIGHT)
	||(IS_OUTSIDE(victim))) && (victim->race == race_lookup("drow")))
    chance -= ch->level/8;
    
    if (ch->submission > 0)
        chance -= 10;
    
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

  if (victim->level < skill_table[gsn_lightning_reflexes].skill_level[victim->class] )
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



  if (victim->level < skill_table[gsn_spell_dragon_speed].skill_level[victim->class] )
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


bool check_battle_prowess( CHAR_DATA *ch, CHAR_DATA *victim )
{
    int chance;
    OBJ_DATA *weapon;  // the defender's weapons effect his ability to parry

/*   	
    if(victim->kingdom != KINGDOM_ARKADIA)
       return FALSE;
*/
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

CHAR_DATA *check_guard(CHAR_DATA *ch, CHAR_DATA *mob)
{
  int chance;

  if (ch->guarded_by == NULL ||
      get_char_room( ch, NULL, ch->guarded_by->name) == NULL)
      return ch;
  else
    {
      chance = (get_skill(ch->guarded_by,gsn_guard) -
		(1.5 * (ch->level - mob->level)));
      if (number_percent() < UMIN(100,chance))
	{
	  act("{c$n jumps in front of $N!{x",ch->guarded_by,NULL,ch,TO_NOTVICT);
	  act("{c$n jumps in front of you!{x",ch->guarded_by,NULL,ch,TO_VICT);
	  act("{cYou jump in front of $N!{x",ch->guarded_by,NULL,ch,TO_CHAR);
	  check_improve(ch->guarded_by,gsn_guard,TRUE,3);
	  return ch->guarded_by;
	}
      else
	{
	  check_improve(ch->guarded_by, gsn_guard, FALSE, 3);
	  return ch;
	}

    }
}

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
	    if (!IS_NPC(rch) || (IS_AFFECTED(rch,AFF_CHARM) && (rch->spec_fun != spec_lookup("spec_lich"))))
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

bool check_counter(CHAR_DATA *ch,CHAR_DATA *victim)
{

	if (!IS_NPC(victim) && (victim->position != POS_FIGHTING)
	 && number_percent() < victim->pcdata->learned[gsn_counter]/2
	 && victim->level >= skill_table[gsn_counter].skill_level[victim->class] )
	  {
		check_improve(victim,gsn_counter,TRUE,3);
		act("{C$N counters, turning your attack against you!{x",ch,NULL,victim,TO_CHAR);
		act("{CYou counter $n's attack, turning it against $m!{x",ch,NULL,victim,TO_VICT);
		act("{C$N counters $n's attack, turning it against $m!{x",ch,NULL,victim,TO_NOTVICT);
		return TRUE;
	  }
    	else
    	  {
		check_improve(victim,gsn_counter,FALSE,1);
		return FALSE;
	  }
}

bool check_death_door(CHAR_DATA *victim)
{
	if (is_affected(victim,gsn_death_door_protect))
    	  {
		victim->hit = victim->max_hit;
    	   	send_to_char("{YYour magic brings you back among the living!!{x\n\r",victim);
	   	act("{YA bright light surrounds $n and brings him back among the living!{x",victim,NULL,NULL,TO_ROOM);
	   	affect_strip(victim,gsn_death_door_protect);
	   	stop_fighting( victim, TRUE );
	   	update_pos(victim);
	   	return TRUE;
	  }
return FALSE;
}
