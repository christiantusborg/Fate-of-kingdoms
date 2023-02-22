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
                             Damage
****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "fight.h"
#include "tables.h"

bool damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show )
{
 if (damage_hit( ch, victim, dam, dt, dam_type,show ))
	return TRUE; 

return FALSE;
}


bool damage_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show ) {

    char log_buf[MAX_STRING_LENGTH];
    OBJ_DATA *object;
    OBJ_DATA *wield;
    bool immune = FALSE;
    int dam2 ;
    bool VictimWanted;
    int object_wear = -1;
    int body_part;
    VictimWanted=FALSE;

	if (ch->msp == 1)
		send_to_char("!!MUSIC(music/fokfight1.mid T=fight)",ch);

	if ( victim->position == POS_DEAD )
		return FALSE;

	if ( victim != ch )
	  {

		if ( is_safe( ch, victim ) )
			return FALSE;
		check_killer( ch, victim );

		if ( victim->position > POS_STUNNED )
		  {
			if ( victim->fighting == NULL )
			  {
				set_fighting( victim, ch );
				if ( IS_NPC( victim ) && HAS_TRIGGER( victim, TRIG_KILL ) )
					mp_percent_trigger( victim, ch, NULL, NULL, TRIG_KILL);
			  }
			if (victim->lagstand < 1)
				victim->position = POS_FIGHTING;
		  }

		if ( victim->position > POS_STUNNED )
		  {
			if ( ch->fighting == NULL )
				set_fighting( ch, victim );


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



		if ( victim->master == ch )
			stop_follower( victim );
	  }

	if(IS_NPC(ch) && ch->master != NULL && ch->in_room != ch->master->in_room )
		do_fleealways(ch,"");

    
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
	if (check_sanctuary(victim,dt))
		dam /= 2;



	dam = affect_dwarf_take_damage_drunk(victim,dam);
	dam = affect_damage_stoneshape(victim,dam);
  
	if ( IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
		dam -= dam / 4;

	if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )
		dam -= dam / 4;

	if ( is_affected(victim,gsn_resistance) )
		dam -= dam /3;

	if (is_affected(victim, gsn_protection_heat_cold) 
	 && ((dam_type == DAM_FIRE) 
	    || (dam_type == DAM_COLD)))
		dam -= dam / 4;
       
	if ( IS_AFFECTED2(victim, AFF2_PROTECT_UNDEAD) && IS_SET(ch->form,FORM_UNDEAD))
		dam /= 3;



	if ( is_affected(ch,gsn_spell_force_nature) )
		if  (dt != gsn_backstab 
		 && dt != gsn_cleave
		 && dt != gsn_ambush 
		 && dt != gsn_dual_backstab 
		 && dt != gsn_circle
		 && dt != gsn_assassinate)      
			dam += dam *.4 ;


	
	dam = affect_immune_damage(victim,dam_type,dam);
	dam = affect_bone_armor_damage(victim,dam);

 	if ( dt != TYPE_HUNGER &&  dt >= TYPE_HIT && ch != victim )
          {
		check_weapon_destroy( ch, victim , FALSE);
		
		if ( (dam2 = critical_strike(ch,victim,dam)) != 0 )
		dam = dam2;

	  }

/*  adds codes for mobs to use magical items */
          if ( IS_NPC( ch )
              && number_percent() < UMIN( 25, UMAX( 10, ch->level ) )
              && !IS_NPC( victim ) )
              use_magical_item( ch );


    



        
  if ( dt != TYPE_HUNGER &&  dt >= TYPE_HIT && ch != victim && dam > 0)   
     if (ch->race == race_lookup("duergar") && (number_percent() <= 5 ))
    {
    if ((weather_info.sunlight == SUN_SET) || (weather_info.sunlight == SUN_DARK)
    || (!IS_OUTSIDE(ch)))
    {
    act("{c$n bypass $N's defences and strikes a critical spot.{x",ch,NULL,victim,TO_NOTVICT);
    act("{%{rOUCH!!{x{c you feel dissy as a vital part is struck{x",victim,NULL,NULL,TO_CHAR);
    act("{%{CCRITICAL!!{x{c you penetrate your opponents defences and strike a vital part{x",ch,NULL,NULL,TO_CHAR);
    dam = dam * 5;
    }
    }
    if (!IS_NPC(ch))
    dam = dam  / 3;
    else
    dam = dam / 1.8;

	if (check_cheat_damage(ch,dam))
		return FALSE;

    
    if (number_range(1,100) < 1)
    {
    	
    
	 switch (body_part)  // type of weapon

	 {
		case 1: 
			object_wear = number_range(1,6);
			break;
		case 2:
			object_wear = number_range(7,9);
			break;
		case 3:
			object_wear = number_range(10,12);
			break;
		case 4:
			object_wear = number_range(13,14);
			break;
		default: break;    
          }

	if (object_wear > 0)
	 {
    	  object = get_eq_char (victim, wear_loc_flags[object_wear].bit);
    	  wield = get_eq_char( ch, WEAR_WIELD );
    	  damage_to_obj(ch,wield,object,dam);
    	}
   }
   

    if ( show ) 
      dam_message( ch, victim, dam, dt, immune );

    

    if (dam == 0)
	return FALSE;
	
    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */

	if (IS_AFFECTED2(ch, AFF2_VAMPIRIC) && (get_eq_char( ch, WEAR_WIELD ) == NULL))
	  {
		ch->hit = ch->hit + dam/3;
		
		if (ch->master != NULL)
			ch->master->hit = ch->master->hit + dam/6;
	  }
  

       
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
   

   if (IS_AFFECTED2(ch, AFF2_GHOUL) && (get_eq_char( ch, WEAR_WIELD ) == NULL) 
   && (dam_type == DAM_BASH))
    {
     if ( !saves_spell( ch->level-5, victim, DAM_NEGATIVE ) && !is_affected(victim, gsn_paralysis) )
    {
    AFFECT_DATA af;
    
    af.type      = gsn_paralysis;
    af.level     = ch->level;
    af.duration  = 0;
    af.where     = TO_AFFECTS2;
    af.bitvector = AFF2_PARALYZE;    
    affect_to_char( victim, &af );
    af.where     = TO_AFFECTS;
    af.bitvector = AFF_STUN;
    affect_to_char( victim, &af );
    act("{CYour strike causes $N to be paralysed{x",ch,NULL,victim,TO_CHAR); 
    act("{CYou are paralysed by $N's touch.{x",victim,NULL,ch,TO_CHAR);
    act("{C$N paralyses $n with his touch.{x",victim,NULL,ch,TO_NOTVICT);  	
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
        
    if (is_affected(victim, gsn_cloak_fear))
     if ( !saves_spell( victim->level-5, ch, DAM_NEGATIVE ))
     {
      send_to_char( "An eerie feeling grows in your as you strike.\n\r",ch);
      act("$n's eyes grow wide with terror.",ch,NULL,NULL,TO_ROOM);
      interpret(ch,"flee");
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
    	if (is_affected(victim, gsn_death_door_protect))
    	{
    	   victim->hit = victim->max_hit;
    	   send_to_char("{YYour magic bring you back among the living!!{x\n\r",victim);
	   act("{YA bright light surrounds $n and brings him back among the living!{x",victim,NULL,NULL,TO_ROOM);
	   affect_strip(victim,gsn_death_door_protect);
	   stop_fighting( victim, TRUE );
	   update_pos(victim);
	}
	else
	{   
	    send_to_char("{rYou have been KILLED!!{x\n\r",victim);
	    act("{r$n is DEAD!{x",victim,NULL,NULL,TO_ROOM);
	    if (ch->race == race_lookup("orc") && (ch != victim))
  		{
    		send_to_char("{cYou feel revived by the blood of your victim.{x\n\r",ch);
    		act("$n seems revived as he drinks the blood of his fallen enemy.",ch,NULL,victim,TO_NOTVICT);
    		ch->hit = UMIN( ch->hit + victim->level * 8, ch->max_hit );
 		}
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

}
ch->crime = 0;
if ( !IS_NPC(victim) )
{
if ( ch!=victim )
 {

         //ch->crime += 5;

	if (IS_SET(victim->act, PLR_WANTED))
	  {
		ch->crime = 0;
		VictimWanted=TRUE;
	  }
     
	send_to_char("{CThe spirit of another enemy haunts you, waiting for your death.{x\n\r",ch);
	if (ch->race == race_lookup("ogre"))
   	  {
		send_to_char("{cYou feel strengthened by the blood of your foe.{x\n\r",ch);
	    	send_to_char("{cYou attach another bone to your belt.{x\n\r",ch);	   
	    	act("$n Attach one of $N's bones to his belt.",ch,NULL,victim,TO_NOTVICT);
	  }
     /* see if the victim needs some haunting */
     
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




raw_kill(ch,victim);
}
        /* RT new auto commands */

/*	if (!IS_NPC(ch)
	&&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
	&&  corpse->item_type == ITEM_CORPSE_NPC &&
	can_see_obj(ch,corpse))
	{
	    corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 

	    if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		 corpse && corpse->contains) // exists and not empty */
/*		do_get(ch,"all corpse");

	   if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                    corpse && corpse->contains  && // exists and not empty */
 /*                   !IS_SET(ch->act,PLR_AUTOLOOT))  {
		do_get(ch,"gold corpse");
		do_get(ch,"silver corpse");
                }
            
	    if ( IS_SET(ch->act, PLR_AUTOSAC) )
	     {
       	      if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
		return TRUE;  // leave if corpse has treasure */
/*	      else
		do_sacrifice(ch,"corpse");
	     }
	}*/



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
