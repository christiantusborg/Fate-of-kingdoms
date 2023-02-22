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

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"


 void spell_torturer( int sn, int level, CHAR_DATA *ch, void *vo )
 {
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int tdice,dam;
	int hit,move,times = 0,dtimes = 0;
	AFFECT_DATA af;
	AFFECT_DATA *paf;
 		
/* [TAKEN OUT BY MAKUS]
	//Only evil characters can use this spell.
	if(!IS_EVIL(ch))
	 {
		send_to_char("You are not quite wicked enough to do that.\n\r",ch);
		return ;
	 }		
[TAKEN OUT BY MAKUS] */ 
	//Protection evil, block torturer.
	if ( is_affected( victim, skill_lookup("protection evil") ) )
	 {
		send_to_char( "Your unaffected by torturer.\n\r", victim );
		act("$n is unaffected by torturer.",victim,NULL,NULL,TO_ROOM);
		return;
	 }

	//Torturer will hard any with less then 30% of max.
	hit = victim->max_hit * 0.30;
	if (victim->hit < hit)
	 {
		send_to_char("Your torturer is killing him, better stop now.\n\r.",ch);
		return;
	 }

	//Check if victim is affected, how many times it casted on him.
	if ( is_affected( victim, sn ) )
		for ( paf = victim->affected; paf != NULL; paf = paf->next )
			if ( paf->type == sn )
				times = paf->modifier;
 		
	//Modify how time is casted to make sure there is a min and max.
	if (times > 20)
		dtimes = 10;
	else
		dtimes = times /2;			
     
	if (dtimes < 1)
		dtimes = 1;
 
	if (dtimes > 10)
		dtimes = 10;
     	    	
	tdice = 4 * dtimes;

	//Damage math.
	dam = dice( level, tdice );
     
	//Victime move is drained
	victim->move -= victim->max_move * 0.10;
     
	//There a save throw vs this spell and can half damage.
	if ( saves_spell( level, victim, DAM_NEGATIVE ) )
		dam /= 2;
 
	//If it casted more then 7 times on victim it send victim in resting
	if (times > 7)
		victim->position = POS_RESTING;
 
	//Add to the affect list
	af.where              = TO_AFFECTS;
	af.type               = sn;
	af.level              = level;
	af.duration           = 1;
	af.bitvector          = 0;
	af.modifier           = 1;
	af.location           = APPLY_NONE;
	affect_join( victim, &af );
 
	//Ch is starting to go mad and starts to grin when he cast spell.
	if (times > 7)
		interpret(ch,"grin");
     
	//Damage is send vs victim.
	damage_spell( ch, victim, dam, sn,DAM_NEGATIVE,TRUE );
 
	//Chance Ch going insanin,
	if ( (times > 10) &&  (number_percent( ) < 25) )
		if (!IS_AFFECTED(ch,AFF_BLOODTHIRST ))
		 {
			af.where     = TO_AFFECTS;
			af.type      = sn;
			af.level     = level;
			af.duration  = level / 10;
			af.location  = 0;
			af.modifier  = 0;
			af.bitvector = AFF_BLOODTHIRST;
			affect_to_char( ch, &af );
			send_to_char( "You are aggressive as a battlerager.\n\r", ch );
			act("$n looks with red eyes.",ch,NULL,NULL,TO_ROOM);
		 }				
 			
	//Chance victim going insanin,
	if ( (times > 10) &&  (number_percent( ) < times*2 ))
		if (!IS_AFFECTED(victim,AFF_BLOODTHIRST ))
		 {
			af.where     = TO_AFFECTS;
			af.type      = sn;
			af.level     = level;
			af.duration  = level / 10;
			af.location  = 0;
			af.modifier  = 0;
			af.bitvector = AFF_BLOODTHIRST;
			affect_to_char( victim, &af );
			send_to_char( "You are aggressive as a battlerager.\n\r", victim );
			act("$n looks with red eyes.",victim,NULL,NULL,TO_ROOM);
		 }		

	return;
}

void spell_agony( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;

/* [TAKEN OUT BY MAKUS]
	//Only evil characters can use this spell.
	if(!IS_EVIL(ch))
	 {
		send_to_char("You are not quite wicked enough to do that.\n\r",ch);
		return ;
	 }		
[TAKEN OUT BY MAKUS] */ 
	//Protection evil, block torturer.
	if ( is_affected( victim, skill_lookup("protection evil") ) )
	 {
		send_to_char( "Your unaffected by torturer.\n\r", victim );
		act("$n is unaffected by torturer.",victim,NULL,NULL,TO_ROOM);
		return;
	 }
	
	//Random damage.
	if ( number_percent( ) < 10)
		level = level * number_range(1,3);
	else if ( number_percent( ) < 20)
		level = level * number_range(1,4);
	else if ( number_percent( ) < 30)
		level = level * number_range(1,5);
	else if ( number_percent( ) < 40)
		level = level * number_range(1,7);
	else if ( number_percent( ) < 50)
		level = level * number_range(1,8);
	else if ( number_percent( ) < 60)
		level = level * number_range(1,9);
	else if ( number_percent( ) < 70)
		level = level * number_range(1,10);
	else if ( number_percent( ) < 90)
		level = level * number_range(1,11);
	else if ( number_percent( ) < 95)
		level = level * number_range(1,12);
	else if ( number_percent( ) < 98)
		level = level * number_range(1,13);
	else
		level = level * number_range(1,14);
		
	//Damage dice
	dam = dice( 2,level );
	
	//Damage is send vs victim.
	damage_spell( ch, victim, dam, sn,DAM_NEGATIVE,TRUE );
	
	return;	

}

void spell_exorcism( int sn, int level, CHAR_DATA *ch, void *vo )
{

return;
}

void spell_storm_vengeance( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
		
		dam = ((ch->level * 80)/10) + dice(15,10);
		damage_spell( ch, victim, dam, sn, DAM_FIRE ,TRUE);
    
    act("{WA white ray from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{WA white ray shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{W$n points at you sending a shocking ray into your chest.{w",ch,NULL,victim,TO_VICT);    
    
    return;
}

/*New DoT spell (Exaethian)*/
void spell_misery( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED(victim,AFF_MISERY))
        {
         act("$N is already in misery.\n\r",ch,NULL,victim,TO_CHAR);
         return;
        }

    if (saves_spell(level,victim,DAM_NEGATIVE) ||
        (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
        if (ch == victim)
          send_to_char("You feel a small stab of pain, but recover quickly.\n\r",ch);
   else
          act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level * 3/4;
    af.duration  = (10 + level / 5);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_MISERY;
    affect_join(victim,&af);

    send_to_char
      ("Your body begins to ache and shiver.\n\r",victim);
    act("$n wails in misery.",
      victim,NULL,NULL,TO_ROOM);
}

void spell_life_leech( int sn, int level, CHAR_DATA *ch, void *vo, int target, int leech )
{
	  CHAR_DATA *victim = (CHAR_DATA *) vo;
	  AFFECT_DATA af;
	  
	  if (IS_AFFECTED2(victim,AFF2_LIFE_LEECH))
	  	  {
	  	  	act("You are already leeching their life!\n\r",ch,NULL,victim,TO_CHAR);
	  	  	return;
	  	  }
	  
	  if (saves_spell(level,victim,DAM_NEGATIVE) ||
	  	   (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
	  	{
	  		if (ch == victim)
	  			send_to_char("You feel your life begin to drain from you, but it fades quickly.\n\r",ch);
	  		else
	  			act("$N resists your leeching.",ch,NULL,victim,TO_CHAR);
	  		return;
      }
  
  af.where			= TO_AFFECTS2;
  af.type				= sn;
  af.level			= level*3/4;
  af.duration		=	5;
  af.location		=	APPLY_NONE;
  af.modifier		=	0;
  af.bitvector	= AFF2_LIFE_LEECH;
  affect_join(victim,&af);
  
  send_to_char
      ("Your life begins to drain from you!\n\r",victim);
    act("The life begins to drain from $n!",
      victim,NULL,NULL,TO_ROOM);
  
  ch->hit += victim->hit/50;
}

/*void spell_shield_of_vengeance( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
		CHAR_DATA *victim = (CHAR_DATA *) vo;
		OBJ_DATA *obj;
		if ( ( obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
			{
				send_to_char("{cYou need to wear a shield to throw it!{x\n\r",ch);
				return;
 			}
 			
		int dam;
		dam = ((ch->level * 80)/15) + dice(15,10);
		damage_spell( ch, victim, dam, sn, DAM_FIRE ,TRUE);
    
    act("{WA white ray from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{WA white ray shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{W$n points at you sending a shocking ray into your chest.{w",ch,NULL,victim,TO_VICT);    
    
		CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;

    if (saves_spell(level, victim,DAM_OTHER))
    {
				send_to_char( "You feel a brief tingling sensation.\n\r",victim);
				send_to_char( "You failed.\n\r", ch);
				return;
    }

    /* begin running through the spells */

    /*if (check_dispel(level,victim,skill_lookup("armor")))
				found = TRUE;

    if (check_dispel(level,victim,skill_lookup("bless")))
				found = TRUE;

    if (check_dispel(level,victim,skill_lookup("calm")))
    {
				found = TRUE;
				act("$n no longer looks so peaceful...",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("change sex")))
    {
				found = TRUE;
				act("$n looks more like $mself again.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("charm person")))
    {
        found = TRUE;
        act("$n regains $s free will.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("chill touch")))
    {
        found = TRUE;
        act("$n looks warmer.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("curse")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("dark vision")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect good")))
				found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect invis")))
        found = TRUE;
 
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect magic")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("faerie fire")))
    {
        act("$n's pink outline fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("faerie fog")))
    {
        act("$n's purple outline fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("fly")))
    {
        act("$n falls to the ground!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("frenzy")))
    {
        act("$n no longer looks so wild.",victim,NULL,NULL,TO_ROOM);;
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("giant strength")))
    {
        act("$n no longer looks so mighty.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("haste")))
    {
        act("$n is no longer moving so quickly.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("infravision")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("invis")))
    {
        act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("mass invis")))
    {
        act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("pass door")))
        found = TRUE;
 

    if (check_dispel(level,victim,skill_lookup("protection evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection good")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("sanctuary")))
    {
        act("The white aura around $n's body vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (IS_AFFECTED(victim,AFF_SANCTUARY) 
	&& !saves_dispel(level, victim->level,-1)
	&& !is_affected(victim,skill_lookup("sanctuary")))
    {
	REMOVE_BIT(victim->affected_by,AFF_SANCTUARY);
        act("The white aura around $n's body vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("shield")))
    {
        act("The shield protecting $n vanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("sleep")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("slow")))
    {
        act("$n is no longer moving so slowly.",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("spellbane")))
				found = TRUE;

    if (check_dispel(level,victim,skill_lookup("stone skin")))
    {
        act("$n's skin regains its normal texture.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("ice storm")))
    {
				act("$n feels warmer.",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("fire storm")))
    {
				act("$n feels cooler.",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("lightning storm")))
    {
				act("$n's muscle starts responding.",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("meteor swarm")))
    {
				act("$n recover from the blast.",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("divine word")))
    {
				act("$n feels a lot better.",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("mental knife")))
    {
				act("$n feels smarter",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("weaken")))
    {
        act("$n looks stronger.",victim,NULL,NULL,TO_ROOM);
				found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("wither")))
    {
				act("$n looks revitalized.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("detect undead")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("vampiric aura")))
    {
				act("$n seems less charismatic.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("cloak of fear")))
    {
				act("$n suddenly seems less terrifying.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("cloak of fear")))
    {
				act("$n suddenly seems less terrifying.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("decay")))
    {
				act("$n's armor returns to its original state.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("protection undead")))
        found = TRUE;

     if (check_dispel(level,victim,skill_lookup("bone armor")))
    {
				act("$n's bone armor crumbles and fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("malison")))
        found = TRUE;

       
    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
	return;
}
		
/**/