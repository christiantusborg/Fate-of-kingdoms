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
 		
	//Only evil characters can use this spell.
	if(!IS_EVIL(ch))
	 {
		send_to_char("You are not quite wicked enough to do that.\n\r",ch);
		return ;
	 }		
 
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

	//Check if victim is affcted, how many times it casted on him.
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

	//Only evil characters can use this spell.
	if(!IS_EVIL(ch))
	 {
		send_to_char("You are not quite wicked enough to do that.\n\r",ch);
		return ;
	 }		
 
	//Protection evil, block torturer.
	if ( is_affected( victim, skill_lookup("protection evil") ) )
	 {
		send_to_char( "Your unaffected by torturer.\n\r", victim );
		act("$n is unaffected by torturer.",victim,NULL,NULL,TO_ROOM);
		return;
	 }
	
	//Random damage.
	if ( number_percent( ) < 25)
		level = level * number_range(1,3);
	else if ( number_percent( ) < 50)
		level = level * number_range(1,4);
	else if ( number_percent( ) < 75)
		level = level * number_range(1,5);
	else if ( number_percent( ) < 80)
		level = level * number_range(1,7);
	else if ( number_percent( ) < 85)
		level = level * number_range(1,8);
	else if ( number_percent( ) < 90)
		level = level * number_range(1,9);
	else if ( number_percent( ) < 95)
		level = level * number_range(1,10);
	else if ( number_percent( ) < 97)
		level = level * number_range(1,11);
	else if ( number_percent( ) < 98)
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
		
		dam = ch->level * 5;
		dam += 50 * number_range(1,5);
		damage_spell( ch, victim, dam, sn, DAM_FIRE ,TRUE);
    
    act("{WA white ray from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{WA white ray shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{W$n points at you sending a shocking ray into your chest.{w",ch,NULL,victim,TO_VICT);    
    
    return;
}