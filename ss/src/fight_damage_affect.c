/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     	 								   *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		   *
***************************************************************************/

/***************************************************************************
                         Damage/Fight Affect
****************************************************************************/
//int affect_damage_stoneshape(CHAR_DATA *victim, int dam)
//int affect_bone_armor_damage(CHAR_DATA *victim,int dam)
//int affect_immune_damage(CHAR_DATA *victim,int dam_type,int dam)
//int affect_size_damage(CHAR_DATA *ch,int dam)
//int affect_armor_damage(CHAR_DATA *victim,int dam_type,int dam)
//int affect_dwarf_take_damage_drunk(CHAR_DATA *victim, int dam)
//int get_affect_enhanced_damage(CHAR_DATA *ch,int dam)
//int get_affect_brutal_damage(CHAR_DATA *ch,int dam)
//int get_dwarf_drunk_damage_affect(CHAR_DATA *ch,int dam)
//int get_sun_affect_ac(CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac)
//get_object_to_damage(int part)
//int get_drunk_affect_ac(CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac)
//void affect_poison_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
//void affect_vampiric_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
//void affect_flaming_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
//void affect_frost_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
//void affect_shocking_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
//void affect_manadrain_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)

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
#include "tables.h"

int affect_damage_stoneshape(CHAR_DATA *victim, int dam)
{
	if ( IS_AFFECTED2(victim, AFF2_STONESHAPE))
	  {
		if (number_percent() < 98 )
		  {
			act( "{cThe attacks deflects on the tough surface!{x",victim, NULL, NULL, TO_ROOM );
			send_to_char("{cYou feel an itch as you are struck!{x\n\r",victim);
			dam /= 50;
		  }
		else
		  {
			act( "{c$n's stoneshell cracks as it is struck.{x",victim, NULL, NULL, TO_ROOM );
			send_to_char("Your form cracks as it's struck, you're able to move again.\n\r", victim);
			dam /= 30;
			affect_strip(victim,gsn_stoneform);
		  }
	  }
return dam;
}
int affect_bone_armor_damage(CHAR_DATA *victim,int dam)
{
	if ( is_affected(victim,gsn_bone_armor))
	  {
		if (victim->ironskincount > 1)
		  {
			act( "{c$n's magical aura weakens as it is struck.{x",victim, NULL, NULL, TO_ROOM );
			send_to_char("A layer of the bones crumble as they're struck\n\r",victim);
			victim->ironskincount--;
			dam /= 10;
		  }
		else
		  {
			act( "{c$n's bone armor crumbles and fades.{x",victim, NULL, NULL, TO_ROOM );
			send_to_char("The last layer of the armor crumbles and fades\n\r",victim);	
			affect_strip(victim,gsn_bone_armor);
			victim->ironskincount = 0;
			dam /= 5;
		  }		
	  }	
return dam;
}

int affect_immune_damage(CHAR_DATA *victim,int dam_type,int dam)
{
	switch(check_immune(victim,dam_type))
	  {
		case(IS_IMMUNE):
			dam /= 10;
		 break;
		case(IS_RESISTANT):
			dam -= dam/3;
		 break;
		case(IS_VULNERABLE):
			dam += dam/2;
		 break;
	  }	
return dam;
}

int affect_size_damage(CHAR_DATA *ch,int dam)
{
    if (ch->size == SIZE_TINY)
    dam /= 1.5;
    else if (ch->size == SIZE_SMALL)
    dam /= 1.2;
    else if (ch->size == SIZE_LARGE)
    dam *= 1.2;
    else if (ch->size == SIZE_HUGE)
    dam *= 1.4;
    else if (ch->size == SIZE_GIANT)
    dam *= 1.6;

return dam;
}

int affect_armor_damage(CHAR_DATA *victim,int dam_type,int dam,int body_part)
{
     switch(dam_type)
    {
    case(DAM_PIERCE):	dam -= (dam *(-GET_AC(victim,AC_PIERCE))/1700);	break;
    case(DAM_BASH):	dam -= (dam *(-GET_AC(victim,AC_BASH))/1700);	break;
    case(DAM_SLASH):	dam -= (dam * (-GET_AC(victim,AC_SLASH))/1700);	break;
    default:	dam -= (dam *(-GET_AC(victim,AC_EXOTIC))/1700);	break;
    }
return dam;
}

int affect_dwarf_take_damage_drunk(CHAR_DATA *victim, int dam)
{

	if (victim->race == race_lookup("dwarf"))
	  {
		if ( !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] > 10 )
			dam *= 0.95;      		
		if ( !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] > 30 )
			dam *= 0.9;
		if ( !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] > 50 )
			dam *= 0.9;
		if ( !IS_NPC(victim) && victim->pcdata->condition[COND_DRUNK] > 70 )
			dam *= 0.9;
    	  }
return dam;
}

int get_affect_enhanced_damage(CHAR_DATA *ch,int dam)
{
	int diceroll;
	
	if ( get_skill(ch,gsn_enhanced_damage) > 0 )
	  {
		diceroll = number_percent();
		if (diceroll <= get_skill(ch,gsn_enhanced_damage))
		  {
			check_improve(ch,gsn_enhanced_damage,TRUE,6);
			dam += dam * diceroll/100;
		  }
	  }
return dam;
}    
      	     
int get_affect_brutal_damage(CHAR_DATA *ch,int dam)
{      	     
	int diceroll;
	
	if ( get_skill(ch,gsn_brutal_damage) > 0 )
	  {
		diceroll = number_percent();
		if (diceroll <= get_skill(ch,gsn_brutal_damage))
		  {
			check_improve(ch,gsn_brutal_damage,TRUE,6);
			dam += dam * diceroll/200;
		  }
	  }
return dam;
}


int get_dwarf_drunk_damage_affect(CHAR_DATA *ch,int dam)
{
	if (ch->race == race_lookup("dwarf"))
	  {
		if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 )
			dam += number_range(1,ch->level/2);      		
		if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 30 )
			dam += number_range(1,ch->level/2);
		if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 50 )
			dam += number_range(1,ch->level/2);
		if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 70 )
			dam += number_range(1,ch->level/2);    	
	  }
return dam;
}

int get_drow_damage_night(CHAR_DATA *ch,int dam)
{      	     
	int diceroll;
	if (ch->race == race_lookup("drow")) 
	  {
		if ((weather_info.sunlight == SUN_SET) ||
		 (weather_info.sunlight == SUN_DARK) ||
		 (!IS_OUTSIDE(ch)))	
	  {
			diceroll = number_percent();
			if (diceroll <= 75)
		  	{
				dam += dam * 1.5;
		  }
		  }
	else
	  {
			diceroll = number_percent();
			if (diceroll <= 75)
		  	{
				dam *= 0.8;
		   }		
	  }
	  }
return dam;
}

int get_sun_affect_ac(CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac)
{
	if (victim->race == race_lookup("drow")) 
	  {
		if ((weather_info.sunlight == SUN_SET) ||
		 (weather_info.sunlight == SUN_DARK) ||
		 (!IS_OUTSIDE(victim)))
		  {
			victim_ac -= victim->level/5;
		  }
		else
		  {
			victim_ac += ch->level/15;
		  }
	  }
    
	if (ch->race == race_lookup("drow")) 
	  {
		if ((weather_info.sunlight == SUN_SET) ||
		(weather_info.sunlight == SUN_DARK) ||
		(!IS_OUTSIDE(victim)))
		  {
			victim_ac += ch->level/6;
		  }
		else
		  {
			victim_ac -= victim->level/10;
		  }
	 }
return victim_ac;
}

int get_drunk_affect_ac(CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac)
{
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 )
		victim_ac -= victim->level/12;     		
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 30 )
		victim_ac -= victim->level/15;
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 50 )
		victim_ac -= victim->level/20;
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 70 )
		victim_ac -= victim->level/25;

return victim_ac;
}

void affect_poison_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
		if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_POISON))
		  {
			int level;
			AFFECT_DATA *poison, af;

			if ((poison = affect_find(wield->affected,gsn_poison)) == NULL)
				level = wield->level;
			else
				level = poison->level;
			
			if (!saves_spell(level,victim,DAM_POISON))
			  {
				send_to_char("{gYou feel poison coursing through your veins.{x",victim);
				act("{g$n is poisoned by the venom on $p.{x",victim,wield,NULL,TO_ROOM);

				af.where     = TO_AFFECTS;
				af.type      = gsn_poison;
		                af.level     = level;
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
}

void affect_vampiric_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_VAMPIRIC))
        {
            dam = number_range(1, wield->level / 5 + 1);
            act("{C$p draws life from $n.{x",victim,wield,NULL,TO_ROOM);
            act("{CYou feel $p drawing your life away.{x",victim,wield,NULL,TO_CHAR);
            damage(ch,victim,dam,0,DAM_NEGATIVE,FALSE);
            ch->hit += dam/2;
        }
}

void affect_flaming_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FLAMING))
	  {
		dam = number_range(1,wield->level / 4 + 1);
		act("{C$n is burned by $p.{x",victim,wield,NULL,TO_ROOM);
		act("{C$p sears your flesh.{x",victim,wield,NULL,TO_CHAR);
		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
			fire_effect( (void *) victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_FIRE,FALSE);
        }
}

void affect_frost_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FROST))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p freezes $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe cold touch of $p surrounds you with ice.{x",victim,wield,NULL,TO_CHAR);

		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_COLD,FALSE);
	  }
}

void affect_shocking_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;

	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SHOCKING))
	  {
		dam = number_range(1,wield->level/5 + 2);
		act("{C$n is struck by lightning from $p.{x",victim,wield,NULL,TO_ROOM);
		act("{CYou are shocked by $p.{x",victim,wield,NULL,TO_CHAR);
		
		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
			shock_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_LIGHTNING,FALSE);
	  }
}

void affect_manadrain_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
	
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

void affect_acid_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_ACID))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p acidic bite $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe acidic bite of $p surrounds you with acid.{x",victim,wield,NULL,TO_CHAR);

//		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
//			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_ACID,FALSE);
	  }
}

void affect_lightning_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_LIGHTNING))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p lightning stike $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe lightning stike of $p surrounds you with lightning.{x",victim,wield,NULL,TO_CHAR);

//		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
//			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_LIGHTNING,FALSE);
	  }
}

void affect_negative_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_NEGATIVE))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p negative charge $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe negative charge of $p surrounds you with energy.{x",victim,wield,NULL,TO_CHAR);

//		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
//			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_NEGATIVE,FALSE);
	  }
}

void affect_harm_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_HARM))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p harm $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe harm of $p hurt you.{x",victim,wield,NULL,TO_CHAR);

//		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
//			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_HARM,FALSE);
	  }
}

void affect_disease_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_DISEASE))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p disease $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe disease of $p hurt you.{x",victim,wield,NULL,TO_CHAR);

//		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
//			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_DISEASE,FALSE);
	  }
}

void affect_energy_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_ENERGY))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p energy charge $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe energy charge of $p surrounds you with energy.{x",victim,wield,NULL,TO_CHAR);

//		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
//			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_ENERGY,FALSE);
	  }
}

void affect_sound_weapon(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield)
{
int dam;
        
	if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SOUND))
	  {
		dam = number_range(1,wield->level / 6 + 2);
		act("{C$p energy charge $n.{x",victim,wield,NULL,TO_ROOM);
		act("{CThe energy charge of $p surrounds you with energy.{x",victim,wield,NULL,TO_CHAR);

//		if (!IS_SET(victim->in_room->room_flags, ROOM_ARENA)) 
//			cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,0,DAM_SOUND,FALSE);
	  }
}
