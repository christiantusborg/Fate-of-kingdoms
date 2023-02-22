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
#include <string.h>
#include <time.h>
#include "merc.h"
/*#include "music.h"*/

/* command procedures needed */
DECLARE_DO_FUN(do_save_clans	);
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_track		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_quaff		);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_reboot	);
#include <unistd.h>     /*  These two is for FORK() the save process */
#include <signal.h>     /*                                           */

/*
 * Local functions.
 */
void    quest_update    args( ( void ) ); /* Vassago - quest.c */
int	hit_gain	args( ( CHAR_DATA *ch ) );
int	mana_gain	args( ( CHAR_DATA *ch ) );
int	move_gain	args( ( CHAR_DATA *ch ) );
void	mobile_update	args( ( void ) );
void	weather_update	args( ( void ) );
void	char_update	args( ( void ) );
void	obj_update	args( ( void ) );
void	aggr_update	args( ( void ) );
void  check_copyover    args( ( void ) );
void    underwater_update    args( ( void ) );

int	potion_cure_level	args( ( OBJ_DATA *potion ) );
int	potion_arm_level	args( ( OBJ_DATA *potion ) );
bool	potion_cure_blind	args( ( OBJ_DATA *potion ) );
bool	potion_cure_poison	args( ( OBJ_DATA *potion ) );
bool	potion_cure_disease	args( ( OBJ_DATA *potion ) );
void    room_update     args( ( void ) );
void    room_affect_update      args( ( void ) );
void	auction_update	args( ( void ) );
/* used for saving */

int	save_number = 0;



/*
 * Advancement stuff.
 */
void advance_level( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    int add_hp;
    int add_mana;
    int add_move;
    int add_prac;

    if (IS_NPC(ch) )  {
       bug( "Advance_level: a mob to advance!", 0);
       return;
    }

    ch->pcdata->last_level = 
	( ch->played + (int) (current_time - ch->logon) ) / 3600;
if(ch->level <= 50) {
     if (!IS_SET(ch->comm, COMM_BLOCKTITLE)) {
    sprintf( buf, "the %s",
	title_table [ch->class] [ch->level] [ch->sex == SEX_FEMALE ? 1 : 0] );
    set_title( ch, buf );
	}
}
/*
if ((strlen(ch->description) < 250 ) && (ch->level > 8)) {
send_to_char("{RWARNING:{w Remember to have a adequate description by level 11\n",ch);
send_to_char("or else you will gain nothing. (more than 250 characters)\n",ch);
}

if ((strlen(ch->pcdata->background) < 500 ) && (ch->level > 18)) {
send_to_char("{RWARNING:{w Remember to have a adequate background by level 21\n",ch);
send_to_char("or else you will gain nothing. (more than 500 characters)\n",ch);
}

if ((strlen(ch->description) < 250 ) && (ch->level > 10)) {
send_to_char("{WYou gain nothing, since you do not have an adequate description.{x\n\r",ch);
return;
}

if ((strlen(ch->pcdata->background) < 500 ) && (ch->level > 20)) {
send_to_char("{WYou gain nothing, since you do not have an adequate background.{x\n\r",ch);
return;
}
*/
   if (((current_time - ch->time_last_level)<LAST_LEVEL_TIME) && (ch->level >= 30))
    {
     sprintf( log_buf, "%s gained a in realy short time and he is %d)",ch->name,ch->level);
     log_string( log_buf );
     wiznet(log_buf,ch,NULL,WIZ_CHEAT,0,0);
    }

   ch->time_last_level = current_time;

    add_hp	= con_app[get_curr_stat(ch,STAT_CON)].hitp + number_range(
		    class_table[ch->class].hp_min,
		    class_table[ch->class].hp_max );
    add_mana 	= ((get_curr_stat(ch,STAT_INT)*get_curr_stat(ch,STAT_INT))/25.0 + (get_curr_stat(ch,STAT_WIS)/10) + number_range(-2,5));
    


    //if (!class_table[ch->class].fMana)
    //	add_mana /= 2.67;
    add_move	= number_range( 1, (get_curr_stat(ch,STAT_CON)
				  + get_curr_stat(ch,STAT_DEX))/6 );
    add_prac	= wis_app[get_curr_stat(ch,STAT_WIS)].practice;

    add_hp = add_hp * HITS_MOD;
    //add_mana = add_mana * MANA_MOD;
    add_move = add_move * MOVE_MOD;

    add_hp	= UMAX(  3, add_hp   );
    add_mana	= UMAX(  3, add_mana );
    add_move	= UMAX(  6, add_move );

    ch->max_hit 	+= add_hp;
    ch->max_mana	+= add_mana;
    ch->max_move	+= add_move;
    ch->practice	+= add_prac;
    ch->train		+= ch->level%5==0?1:0;

    ch->pcdata->perm_hit	+= add_hp;
    ch->pcdata->perm_mana	+= add_mana;
    ch->pcdata->perm_move	+= add_move;

    sprintf( buf,
	"{WYour gain is: {G%d{W/{g%d {Whp, {M%d{W/{m%d {Wm, {Y%d{W/{Y%d {Wmv {B%d{W/{b%d {Wprac.{x\n\r",
	add_hp,		ch->max_hit,
	add_mana,	ch->max_mana,
	add_move,	ch->max_move,
	add_prac,	ch->practice
	);
    send_to_char( buf, ch );
    if (!(ch->level%10))
	send_to_char("{WYou also gain a training session.{x\n\r",ch);
    return;
}   



void gain_exp( CHAR_DATA *ch, int gain )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) || ch->level >= LEVEL_HERO )
	return;
/*
    ch->exp = UMAX( exp_per_level(ch,ch->pcdata->points), ch->exp + gain );
    while ( ch->level < LEVEL_HERO && ch->exp >= 
	exp_per_level(ch,ch->pcdata->points) * (ch->level+1) )
*/

    ch->exp = UMAX( base_exp(ch,ch->pcdata->points), ch->exp + gain );
    while ( ch->level < LEVEL_HERO &&
           exp_to_level(ch,ch->pcdata->points) <= 0)
    {
	send_to_char( "{CYou gain a new level of knowledge!!{x  ", ch );
	ch->level += 1;

        /* Level counting */
        if (ch->level > 5)
          total_levels++;

        /* Level 30 log message */
        if (ch->level == 80)
          {
            sprintf(log_buf, "%s made level 80.", ch->name);
            log_string(log_buf);
          }

        sprintf(log_buf, "%s attained level %d.",ch->name,ch->level);
        log_string(log_buf);
	sprintf(buf,"[$N has attained level %d]",ch->level);
	wiznet(buf,ch,NULL,WIZ_LEVELS,0,0);
	advance_level( ch );
	save_char_obj(ch);
    }

    return;
}



/*
 * Regeneration stuff.
 */
int hit_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain =  5 + ch->level;

	   if (IS_AFFECTED(ch,AFF_REGENERATION))
            gain *= 2;

	switch(ch->position)
	{
            default :           gain /= 2;                      break;
            case POS_SLEEPING:  gain = 3 * gain/2;              break;
            case POS_RESTING:                                   break;
            case POS_FIGHTING:  gain /= 3;                      break;
 	}

	
    }
    else
    {
	

	gain = UMAX(3,get_curr_stat(ch,STAT_CON) - 3 + ch->level/2); 
	gain += class_table[ch->class].hp_max - 10;
 	number = number_percent();
	
	if (number < get_skill(ch,gsn_fast_healing))
	{
	    gain += number * gain / 100;
	    if (ch->hit < ch->max_hit)
		check_improve(ch,gsn_fast_healing,TRUE,8);
	}

	switch ( ch->position )
	{
	    default:	   	gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:  	gain /= 2;			break;
	    case POS_FIGHTING: 	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   < 0 )
	    gain /= 4;

	if ( ch->pcdata->condition[COND_THIRST] < 0 )
	    gain /= 4;
    }

    gain = gain * ch->in_room->heal_rate / 100;
    
    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
	gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) )
	gain /=2 ;

    if (IS_AFFECTED(ch,AFF_SLOW) )
	gain *=2 ;

    if(ch->in_room == get_room_index(ROOM_VNUM_SHADOWPLANE))
{
    send_to_char( "The shadow plane drains life from you!\n\r",  ch );
	gain = -40 ;
}
    return UMIN(gain, ch->max_hit - ch->hit);
}



int mana_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = 5 + ch->level;
	switch (ch->position)
	{
	    default:		gain /= 2;		break;
	    case POS_SLEEPING:	gain = 3 * gain/2;	break;
   	    case POS_RESTING:				break;
	    case POS_FIGHTING:	gain /= 3;		break;
    	}
    }
    else
    {
	gain = (get_curr_stat(ch,STAT_WIS) 
	      + get_curr_stat(ch,STAT_INT) + ch->level) / 2;
	number = number_percent();
	if (number < get_skill(ch,gsn_meditation))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_meditation,TRUE,8);
	}
	number = number_percent();
	if (number < get_skill(ch,gsn_trance))
	{
	    gain += number * gain / 100;
	    if (ch->mana < ch->max_mana)
	        check_improve(ch,gsn_trance,TRUE,8);
	}
      if (number < get_skill(ch,gsn_transcendental_meditation))
        {
            gain += number * gain / 100;
            if (ch->mana < ch->max_mana)
                check_improve(ch,gsn_transcendental_meditation,TRUE,8);
        }  
	if (!class_table[ch->class].fMana)
	    gain /= 2;

	switch ( ch->position )
	{
	    default:		gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:	gain /= 2;			break;
	    case POS_FIGHTING:	gain /= 6;			break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   < 0 )
	    gain /= 4;

	if ( ch->pcdata->condition[COND_THIRST] < 0 )
	    gain /= 4;

    }

    gain = gain * ch->in_room->mana_rate / 100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[4] / 100;

    if ( IS_AFFECTED( ch, AFF_POISON ) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) )
        gain /= 2 ;
    if (IS_AFFECTED(ch,AFF_SLOW) )
        gain *= 2 ;

    if(ch->in_room == get_room_index(ROOM_VNUM_SHADOWPLANE))
	gain = -40 ;

    return UMIN(gain, ch->max_mana - ch->mana);
}



int move_gain( CHAR_DATA *ch )
{
    int gain;

    if (ch->in_room == NULL)
	return 0;

    if ( IS_NPC(ch) )
    {
	gain = ch->level;
    }
    else
    {
	gain = UMAX( 15, ch->level );

	switch ( ch->position )
	{
	case POS_SLEEPING: gain += get_curr_stat(ch,STAT_DEX);		break;
	case POS_RESTING:  gain += get_curr_stat(ch,STAT_DEX) / 2;	break;
	}

	if ( ch->pcdata->condition[COND_HUNGER]   < 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] < 0 )
	    gain /= 2;
    }

    gain = gain * ch->in_room->heal_rate/100;

    if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
	gain = gain * ch->on->value[3] / 100;

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE) || IS_AFFECTED(ch,AFF_SLOW))
        gain /=2 ;

    if(ch->in_room == get_room_index(ROOM_VNUM_SHADOWPLANE))
	gain = -40 ;

    return UMIN(gain, ch->max_move - ch->move);
}



void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    int condition;
    int damage_hunger;
    int fdone;
    CHAR_DATA *vch,*vch_next;

    if (IS_SET(ch->in_room->room_flags,ROOM_ARENA)) {
	ch->pcdata->condition[iCond] = 20;
	}

    if ( value == 0 || IS_NPC(ch) || ch->level >= LEVEL_IMMORTAL ||
	IS_SET(ch->comm,COMM_BUSY) || !ch->desc ||
	IS_SET(ch->in_room->room_flags,ROOM_ARENA))
	return;
    condition				= ch->pcdata->condition[iCond];

    ch->pcdata->condition[iCond]	= URANGE( -6, condition + value, 96 );

    if ( iCond == COND_FULL && (ch->pcdata->condition[COND_FULL] < 0) )
       ch->pcdata->condition[COND_FULL] = 0;

/*    if ( (iCond == COND_DRUNK) && (condition < 1) ) 
      ch->pcdata->condition[COND_DRUNK] = 0; */

    if ( ch->pcdata->condition[iCond] < 1 && ch->pcdata->condition[iCond] > -6 )
    {
	switch ( iCond )
	{
	case COND_HUNGER:
	    send_to_char( "{cYou are hungry.{x\n\r",  ch );
	    break;

	case COND_THIRST:
	    send_to_char( "{cYou are thirsty.{x\n\r", ch );
	    break;
     
	case COND_DRUNK:
	    if ( condition == 0 )
		send_to_char( "{cYou are sober.{x\n\r", ch );
	    break;

	case COND_BLOODLUST:
            if ( condition != 0 )
                send_to_char( "You are hungry for blood.\n\r", ch );
            break;
	}
    }

    if ( ch->pcdata->condition[iCond] == -6 && ch->exp >= PK_MIN)
    {
	switch ( iCond )
	{
	case COND_HUNGER:
	    send_to_char( "{CYou are starving!{x\n\r",  ch );
	    act( "{c$n is starving!{x",  ch, NULL, NULL, TO_ROOM );
	    ch->hit -= ch->max_hit * number_range(2, 4) / 100;
	    damage( ch, ch, 1, TYPE_HIT, DAM_HUNGER, FALSE );
	    if ( ch->position == POS_SLEEPING ) 
		return;       /* coz he's dead  :)   */
	    break;

	case COND_THIRST:
	    send_to_char( "{CYou are dying of thirst!{x\n\r", ch );
	    act( "{c$n is dying of thirst!{x", ch, NULL, NULL, TO_ROOM );
	    ch->hit -= ch->max_hit * number_range(2, 4) / 100;
	    damage( ch, ch, 1, TYPE_HIT, DAM_THIRST, FALSE );
	    break;
	
	   case COND_BLOODLUST:
            fdone = 0;
            send_to_char( "You are suffering from thrist of blood!\n\r",ch);
            act("$n is suffering from thirst of blood!",ch,NULL,NULL,TO_ROOM );
            if (ch->in_room && ch->in_room->people && !ch->fighting)
                {
                 if (!IS_AWAKE(ch)) do_stand(ch,"");
                 for ( vch = ch->in_room->people;
                        vch != NULL && ch->fighting == NULL; vch =vch_next)
                 {
                  vch_next = vch->next_in_room;
                  if ( ch != vch && can_see(ch,vch) &&
                        !is_safe_nomessage(ch,vch) )
                   {
                    do_yell(ch,"BLOOD! I NEED BLOOD!");
			do_murder(ch,vch->name);
                    fdone = 1;
                   }
                 }
                }
            if (fdone) break;
            damage_hunger = ch->max_hit * number_range(2, 4) / 100;
            if (!damage_hunger) damage_hunger = 1;
            damage( ch, ch, damage_hunger, TYPE_HUNGER, DAM_THIRST, TRUE);
            if ( ch->position == POS_SLEEPING )
                return;
            break;
	}
    }



    return;
}



/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    EXIT_DATA *pexit;
    int door;
    OBJ_DATA *obj;


    /* Examine all mobs. */
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

  if ( IS_AFFECTED(ch, AFF_REGENERATION ) && ch->in_room != NULL )
        {
         ch->hit = UMIN( ch->hit + ch->level / 10, ch->max_hit );
         send_to_char("",ch);
        }

        if ( IS_AFFECTED(ch, AFF_CORRUPTION ) && ch->in_room != NULL )
        {
         ch->hit = UMAX( ch->hit - ch->level / 3, 3 );
         if (ch->position < POS_RESTING)
         damage(ch,ch,1,gsn_corruption,DAM_NONE,TRUE);
         send_to_char("",ch);
        }

	if ( !IS_NPC(ch) || ch->in_room == NULL || 
	(IS_AFFECTED(ch,AFF_CHARM) && ch->pIndexData->vnum!=11))
	    continue;

	if (ch->in_room->area->empty && !IS_SET(ch->act,ACT_UPDATE_ALWAYS))
	    continue;

	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	}

	if (ch->pIndexData->pShop != NULL) /* give him some gold */
	    if ((ch->gold * 100 + ch->silver) < ch->pIndexData->wealth)
	    {
		ch->gold += ch->pIndexData->wealth * number_range(1,20)/5000000;
		ch->silver += ch->pIndexData->wealth * number_range(1,20)/50000;
	    }
       /*
        * Check triggers only if mobile still in default position
        */
       if ( ch->position == ch->pIndexData->default_pos )
       {
           /* Delay */
           if ( HAS_TRIGGER( ch, TRIG_DELAY)
           &&   ch->mprog_delay > 0 )
           {
               if ( --ch->mprog_delay <= 0 )
               {
                   mp_percent_trigger( ch, NULL, NULL, NULL, TRIG_DELAY);
                   continue;
               }
           }
           if ( HAS_TRIGGER( ch, TRIG_RANDOM) )
           {
               if( mp_percent_trigger( ch, NULL, NULL, NULL, TRIG_RANDOM) )
               continue;
           }
       }

	/*
	 *  Potion useing and stuff for intelligent mobs
	 */

	if ( ch->position == POS_STANDING ||
	     ch->position == POS_RESTING ||
	     ch->position == POS_FIGHTING )
	{

	   if ( get_curr_stat(ch, STAT_INT ) > 15 &&
		(ch->hit<ch->max_hit*0.9 || 
		 IS_AFFECTED(ch,AFF_BLIND) ||
		 IS_AFFECTED(ch,AFF_POISON) ||
		 IS_AFFECTED(ch,AFF_PLAGUE) || ch->fighting!=NULL) )  
	   {
	      for(obj=ch->carrying;obj!=NULL;obj=obj->next_content)
		 if ( obj->item_type == ITEM_POTION )
		 {
		   if ( ch->hit < ch->max_hit*0.9 )  /* hp curies */
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
		 }  
	   }
	}


	/* That's all for sleeping / busy monster, and empty zones */
	if ( ch->position != POS_STANDING )
	    continue;

        if ( IS_SET(ch->progtypes,MPROG_AREA) &&
            (ch->in_room->area->nplayer > 0))
          (ch->pIndexData->moprogs->area_prog) (ch);


        if (ch->position < POS_STANDING)
          continue;

	/* Scavenge */
	if ( IS_SET(ch->act, ACT_SCAVENGER)
	&&   ch->in_room->contents != NULL
	&&   number_bits( 6 ) == 0 )
	{
	    OBJ_DATA *obj;
	    OBJ_DATA *obj_best;
	    int max;

	    max         = 1;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	    {
		if ( CAN_WEAR(obj, ITEM_TAKE) && can_loot(ch, obj)
		     && obj->cost > max  && obj->cost > 0)
		{
		    obj_best    = obj;
		    max         = obj->cost;
		}
	    }

	    if ( obj_best )
	    {
		obj_from_room( obj_best );
		obj_to_char( obj_best, ch );
		act( "$n gets $p.", ch, obj_best, NULL, TO_ROOM );
	    }
	}

	/* Wander */
	if ( !IS_SET(ch->act, ACT_SENTINEL) 
	&& number_bits(3) == 0
	&& ( door = number_bits( 5 ) ) <= 5
	&& ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&&   !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB)
	&& ( !IS_SET(ch->act, ACT_STAY_AREA)
	||   pexit->u1.to_room->area == ch->in_room->area ) 
	&& ( !IS_SET(ch->act, ACT_OUTDOORS)
	||   !IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS)) 
	&& ( !IS_SET(ch->act, ACT_INDOORS)
	||   IS_SET(pexit->u1.to_room->room_flags,ROOM_INDOORS)))
	{
	    move_char( ch, door, FALSE );
	}
    }

    return;
}

int potion_cure_level( OBJ_DATA *potion )
{
int cl;
int i;
  cl = 0;
  for (i=1;i<5;i++)
  {
    if ( skill_lookup("cure critical") == potion->value[i] )
      cl += 3;
    if ( skill_lookup("cure light") == potion->value[i])
      cl += 1;
    if ( skill_lookup("cure serious") == potion->value[i] )
      cl += 2;
    if ( skill_lookup("heal") == potion->value[i])
      cl += 4;
  }
  return(cl);
}
int potion_arm_level( OBJ_DATA *potion )
{
int al;
int i;
  al = 0;
  for (i=1;i<5;i++)
  {
    if ( skill_lookup("armor") == potion->value[i] )
      al += 1;
    if ( skill_lookup("shield") == potion->value[i])
      al += 1;
    if ( skill_lookup("stone skin") == potion->value[i] )
      al += 2;
    if ( skill_lookup("sanctuary") == potion->value[i])
      al += 4;
    if ( skill_lookup("protection") == potion->value[i])
      al += 3;
  }
  return(al);
}

bool potion_cure_blind( OBJ_DATA *potion )
{
int i;
  for (i=0;i<5;i++)
  {
    if (skill_lookup("cure blindness") == potion->value[i])
      return(TRUE);
  }
  return(FALSE);
}
bool potion_cure_poison( OBJ_DATA *potion )
{
int i;
  for (i=0;i<5;i++)
  {
    if (skill_lookup("cure poison") == potion->value[i] )
      return(TRUE);
  }
  return(FALSE);
}
bool potion_cure_disease( OBJ_DATA *potion )
{
int i;
  for (i=0;i<5;i++)
  {
    if (skill_lookup("cure disease") == potion->value[i] )
      return(TRUE);
  }
  return(FALSE);
}

/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int diff;

    buf[0] = '\0';
    buf2[0] = '\0';

    switch ( ++time_info.hour )
    {
    case  5:
	weather_info.sunlight = SUN_LIGHT;
	strcat( buf, "{BThe {Yday {Bhas begun.{x\n\r" );
	break;

    case  6:
	weather_info.sunlight = SUN_RISE;
	strcat( buf, "{BThe {Ysun {Brises in the east.{x\n\r" );
	break;

    case 19:
	weather_info.sunlight = SUN_SET;
	strcat( buf, "{DThe {Ysun{D slowly disappears in the west.{x\n\r" );
	break;

    case 20:
	weather_info.sunlight = SUN_DARK;
	strcat( buf, "{DThe night has begun.{x\n\r" );
	break;

    case 24:
	time_info.hour = 0;
	time_info.day++;
	break;
    }

    if ( time_info.day   >= 35 )
    {
	time_info.day = 0;
	time_info.month++;
    }

    if ( time_info.month >= 17 )
    {
	time_info.month = 0;
	time_info.year++;
    }

    /*
     * Weather change.
     */
    if ( time_info.month >= 9 && time_info.month <= 16 )
	diff = weather_info.mmhg >  985 ? -2 : 2;
    else
	diff = weather_info.mmhg > 1015 ? -2 : 2;

    weather_info.change   += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
    weather_info.change    = UMAX(weather_info.change, -12);
    weather_info.change    = UMIN(weather_info.change,  12);

    weather_info.mmhg += weather_info.change;
    weather_info.mmhg  = UMAX(weather_info.mmhg,  960);
    weather_info.mmhg  = UMIN(weather_info.mmhg, 1040);

    switch ( weather_info.sky )
    {
    default: 
	bug( "Weather_update: bad sky %d.", weather_info.sky );
	weather_info.sky = SKY_CLOUDLESS;
	break;

    case SKY_CLOUDLESS: 
	if ( weather_info.mmhg <  990
	|| ( weather_info.mmhg < 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "{BThe sky is getting {Dcloudy.{x\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_CLOUDY:
	if ( weather_info.mmhg <  970
	|| ( weather_info.mmhg <  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "{BIt starts to {brain.{x\n\r" );
	    weather_info.sky = SKY_RAINING;
	}

	if ( weather_info.mmhg > 1030 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "{BThe clouds disappear.{x\n\r" );
	    weather_info.sky = SKY_CLOUDLESS;
	}
	break;

    case SKY_RAINING:
	if ( weather_info.mmhg <  970 && number_bits( 2 ) == 0 )
	{
	    strcat( buf, "{dL{Dightning {d{%flashes{X {D in the sky.{x\n\r" );
	    strcat( buf2,"!!SOUND(sounds/fokthunder.wav)\n\r" );
	    weather_info.sky = SKY_LIGHTNING;
	}

	if ( weather_info.mmhg > 1030
	|| ( weather_info.mmhg > 1010 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "{BThe rain stopped.{x\n\r" );
	    weather_info.sky = SKY_CLOUDY;
	}
	break;

    case SKY_LIGHTNING:
	if ( weather_info.mmhg > 1010
	|| ( weather_info.mmhg >  990 && number_bits( 2 ) == 0 ) )
	{
	    strcat( buf, "{BThe lightning has stopped.{x\n\r" );
	    weather_info.sky = SKY_RAINING;
	    break;
	}
	break;
    }

    if ( buf[0] != '\0' )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   IS_OUTSIDE(d->character)
	    &&   IS_AWAKE(d->character) ) {
		send_to_char( buf, d->character );
		 if(buf2[0] != '\0' ) {
			if(d->character->msp == 1) 
				send_to_char(buf2, d->character );
				} /* if dconnect */
			} /* buf2 */
	}
    }

    return;
}



/*
 * Update all chars, including mobs.
*/
void char_update( void )
{   
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;
/*    char buf2[MAX_STRING_LENGTH]; ::used in shit i commented out */
/*    char      *buff; */

    static time_t last_save_time = -1;

    ch_quit = NULL; 

    /* adds autosaving of clans*/
    if (save_number==20) {
	ch=char_list; /* just initalize, not needed, bad coding, change later */
        do_save_clans(ch,"");
        }

    /* update save counter */
    save_number++;

    if (save_number > 29)
	save_number = 0;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	ch_next = ch->next;

        /* Remove caltraps effect after fighting is over */
        if (is_affected(ch,gsn_caltraps) && !ch->fighting)
          affect_strip(ch,gsn_caltraps);

        /* Reset sneak and hide */
        if ( !(ch->fighting) && !IS_AFFECTED(ch,AFF_SNEAK) &&
            (race_table[ch->race].aff & AFF_SNEAK))
          send_to_char("{gYou begin to sneak again.{x\n\r",ch);

        if ( !(ch->fighting) && !IS_AFFECTED(ch,AFF_HIDE) &&
            (race_table[ch->race].aff & AFF_HIDE))
          send_to_char("{gYou step back into the shadows.{x\n\r",ch);

        SET_BIT(ch->affected_by, race_table[ch->race].aff );

        if ( ch->timer > 20 && !IS_NPC(ch) )
            ch_quit = ch;

        /* Mad mobs track twice per tick */
        if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CALM)
            && ch->fighting == NULL
            && IS_AWAKE(ch) && ch->last_fought != NULL
            && !IS_SET(ch->act,ACT_NOTRACK)
            && ch->in_room != ch->last_fought->in_room 
	    && ch->level > 11
            && !is_affected(ch,skill_lookup("awe"))) {
          do_track(ch,ch->last_fought->name);
          if ((ch->in_room != ch->last_fought->in_room && ch->status!=2)
	     && ch->level > 11
	     && !is_affected(ch,skill_lookup("awe")))
            do_track(ch,ch->last_fought->name);
        }

	if ( ch->position >= POS_STUNNED )
	{
            /* check to see if we need to go home */
            if (IS_NPC(ch) && ch->zone != NULL && ch->zone != ch->in_room->area
            && ch->desc == NULL &&  ch->fighting == NULL && ch->progtypes==0
	    && !IS_AFFECTED(ch,AFF_CHARM) && number_percent() < 1)
            {
            	act("{c$n wanders off.{x",ch,NULL,NULL,TO_ROOM);
            	extract_char(ch,TRUE);
            	continue;
            }

        if (ch->icmode > 1) 
         ch->icmode--; //TA nochan2

        if (ch->icmode == 1)
 		{
 		 send_to_char("You drop out of ic mode.\n\r",ch);
 		 if (IS_SET(ch->info,INFO_ICOOC))	
 		  REMOVE_BIT(ch->info,INFO_ICOOC);;
 		 ch->icmode =0;
 		}


        if (ch->nochanned > 1) 
         ch->nochanned--; //TA nochan2
   	 
        if (ch->nochanned == 1)
 		{
 		 send_to_char("Channels restored",ch);
 		 if (IS_SET(ch->comm, COMM_NOCHANNELS))	
 		  REMOVE_BIT(ch->comm, COMM_NOCHANNELS);
 		 ch->nochanned =0;
 		}
 

	    if ( ch->hit  < ch->max_hit )
		ch->hit  += hit_gain(ch);
	    else
		ch->hit = ch->max_hit;

	    if ( ch->mana < ch->max_mana )
		ch->mana += mana_gain(ch);
	    else
		ch->mana = ch->max_mana;

	    if ( ch->move < ch->max_move )
		ch->move += move_gain(ch);
	    else
		ch->move = ch->max_move;
	}

	if ( ch->position == POS_STUNNED )
	    update_pos( ch );

         if ( !IS_NPC(ch) && ch->pking > 0 )
         {
            ch->pking--;
         }

	if ( !IS_NPC(ch) && ch->level < LEVEL_IMMORTAL )
	{
	    OBJ_DATA *obj;

	    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
	    &&   obj->item_type == ITEM_LIGHT
	    &&   obj->value[2] > 0 )
	    {
		if ( --obj->value[2] == 0 && ch->in_room != NULL )
		{
		    --ch->in_room->light;
		    act( "{c$p goes out.{x", ch, obj, NULL, TO_ROOM );
		    act( "{c$p flickers and goes out.{x", ch, obj, NULL,TO_CHAR );
		    extract_obj( obj );
		}
	 	else if ( obj->value[2] <= 5 && ch->in_room != NULL) {
		    act("{c$p flickers.{x",ch,obj,NULL,TO_CHAR);
		/*    act( "***** BUG TEST *******",ch,obj,NULL,TO_CHAR); */
/*           sprintf(buf2, "*** BUG*** %s - %s.", ch->name,obj->name); */
/*           log_string(buf2); */
/*  removes the above just because it is...DUMB! */
               }
	}

	    if (IS_IMMORTAL(ch))
		ch->timer = 0;

	    if ( ++ch->timer >= 12 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "{W$n disappears into the void.{x",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "{WYou disappear into the void.{x\n\r",ch );
		    if (ch->level > 1  )
		        save_char_obj( ch );
		 if ( !IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
		    if ( ch->level < 10 )  {
  		      char_from_room( ch );
		      char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		    }
		}
	    }
 if ( !IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) ) {
	    gain_condition( ch, COND_DRUNK,  -1 );
	    gain_condition( ch, COND_FULL, ch->size > SIZE_MEDIUM ? -4 : -2 );
	    if ( ch->in_room->sector_type == SECT_DESERT )
	      gain_condition( ch, COND_THIRST, -3 );
	    else
	      gain_condition( ch, COND_THIRST, -1 );
	    gain_condition( ch, COND_HUNGER, ch->size > SIZE_MEDIUM ? -2 : -1);
	}
}
	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
	    {
		paf->duration--;
		if (number_range(0,4) == 0 && paf->level > 0)
		  paf->level--;  /* spell strength fades with time */
            }
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			send_to_char( "{g", ch );
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "{x\n\r", ch );
		    }
		}
	  
		affect_remove( ch, paf );
	    }
	}

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */

        if (is_affected(ch, gsn_plague) && ch != NULL)
        {
            AFFECT_DATA *af, plague;
            CHAR_DATA *vch;
            int dam;

	    if (ch->in_room == NULL)
		continue;
	            
	    act("{g$n writhes in agony as plague sores erupt from $s skin.{x",
		ch,NULL,NULL,TO_ROOM);
	    send_to_char("{gYou writhe in agony from the plague.{x\n\r",ch);
            for ( af = ch->affected; af != NULL; af = af->next )
            {
            	if (af->type == gsn_plague)
                    break;
            }
        
            if (af == NULL)
            {
            	REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
		continue;
            }
        
            if (af->level == 1)
		continue;
        
	    plague.where		= TO_AFFECTS;
            plague.type 		= gsn_plague;
            plague.level 		= af->level - 1; 
            plague.duration 	= number_range(1,2 * plague.level);
            plague.location		= APPLY_STR;
            plague.modifier 	= -5;
            plague.bitvector 	= AFF_PLAGUE;
        
            for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(plague.level + 2,vch,DAM_DISEASE) 
		&&  !IS_IMMORTAL(vch)
		&&  !IS_SET(vch->imm_flags,IMM_DISEASE)
            	&&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(2) == 0)
            	{
            	    send_to_char("{gYou feel hot and feverish.{x\n\r",vch);
            	    act("{g$n shivers and looks very ill.{x",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&plague);
            	}
            }

	    dam = UMIN(ch->level,af->level/5+1);
	    ch->mana -= dam;
	    ch->move -= dam;
	    damage( ch, ch, dam, gsn_plague,DAM_DISEASE,FALSE);
            if (number_range(1, 100) < 70 )
  	      damage( ch, ch, UMAX(ch->max_hit/20, 50), gsn_plague,DAM_DISEASE,TRUE);
        }
	else if ( IS_AFFECTED(ch, AFF_POISON) && ch != NULL
	     &&   !IS_AFFECTED(ch,AFF_SLOW))

	{
	    AFFECT_DATA *poison;

	    poison = affect_find(ch->affected,gsn_poison);

	    if (poison != NULL)
	    {
	        act( "{g$n shivers and suffers.{x", ch, NULL, NULL,TO_ROOM);
	        send_to_char( "{gYou shiver and suffer.{x\n\r", ch );
	        damage(ch,ch,poison->level/10 + 1,gsn_poison,
		    DAM_POISON,TRUE);
	    }
	}

	else if ( ch->position == POS_INCAP && number_range(0,1) == 0)
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
	else if ( ch->position == POS_MORTAL )
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE,FALSE);
	}
    }

    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */

    if (last_save_time == -1 || current_time - last_save_time > 300) {
      last_save_time = current_time;
      for (ch = char_list; ch != NULL; ch = ch_next)
          {
            ch_next = ch->next;
	    if ( !IS_NPC(ch) )
              save_char_obj(ch);
            if ( ch == ch_quit || ch->timer > 20 )
              do_quit(ch, "");
          }
    }
    return;
}



/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
/*  adds one so the fumble code works */
    CHAR_DATA *ch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
/*    char buf[MAX_STRING_LENGTH]; */
    OBJ_DATA *t_obj, *pit, *next_obj;
    AFFECT_DATA *paf, *paf_next;
    static int pit_count = 1;


    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;

	/* go through affects and decrement */
        for ( paf = obj->affected; paf != NULL; paf = paf_next )
        {
            paf_next    = paf->next;
            if ( paf->duration > 0 )
            {
                paf->duration--;
                if (number_range(0,4) == 0 && paf->level > 0)
                  paf->level--;  /* spell strength fades with time */
            }
            else if ( paf->duration < 0 )
                ;
            else
            {
                if ( paf_next == NULL
                ||   paf_next->type != paf->type
                ||   paf_next->duration > 0 )
                {
                    if ( paf->type > 0 && skill_table[paf->type].msg_obj )
                    {
			if (obj->carried_by != NULL)
			{
			    rch = obj->carried_by;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_CHAR);
			}
			if (obj->in_room != NULL 
			&& obj->in_room->people != NULL)
			{
			    rch = obj->in_room->people;
			    act(skill_table[paf->type].msg_obj,
				rch,obj,NULL,TO_ALL);
			}
                    }
                }

                affect_remove_obj( obj, paf );
            }
        }


/*  Item area progs... These are intelligent items, updates are the triggers
    and they can behave     GG()  (based on Carrion Fields)
*/
        for(t_obj = obj; t_obj->in_obj; t_obj = t_obj->in_obj);

        if (IS_SET(obj->progtypes,IPROG_AREA))
           if ( ( t_obj->in_room != NULL &&
                (t_obj->in_room->area->nplayer > 0))
               ||
                (t_obj->carried_by &&
                 t_obj->carried_by->in_room &&
                 t_obj->carried_by->in_room->area->nplayer > 0) )
              (obj->pIndexData->iprogs->area_prog) (obj);
/*  End of ITEMPROG */
/*
    if (strstr(obj->pIndexData->area->builders, "Unlinked")
        && (!IS_IMMORTAL(obj->carried_by) && !IS_NPC(obj->carried_by))) {
        sprintf(buf,"%s is not approved yet.\n\r",obj->short_descr);
	act(buf, obj->carried_by, obj, NULL, TO_CHAR );
        sprintf(buf,"%s is crumbles into dust.\n\r",obj->short_descr);
	act(buf, obj->carried_by, obj, NULL, TO_CHAR );
	extract_obj(obj);
        return;
        }
*/
        if ( check_material( obj, "ice" ) )  {
	  if ( obj->carried_by != NULL )  {
	    if ( obj->carried_by->in_room->sector_type == SECT_DESERT )
	      if ( number_percent() < 40 )  {
		act( "The extreme heat melts $p.", obj->carried_by, obj, NULL, TO_CHAR );
		extract_obj( obj );
		continue;
	      }
	  }
	  else if ( obj->in_room != NULL )
	    if ( obj->in_room->sector_type == SECT_DESERT )
	      if ( number_percent() < 50 )  {
		if ( obj->in_room->people != NULL )  {
		  act( "The extreme heat melts $p.", obj->in_room->people, obj, NULL, TO_ROOM );
		  act( "The extreme heat melts $p.", obj->in_room->people, obj, NULL, TO_CHAR );
	        }
		extract_obj( obj );
		continue;
	      }
	}

        if ( !check_material( obj, "glass" ) && obj->item_type==ITEM_POTION)  
	{
/*	  if ( obj->carried_by != NULL )  
	  {
	    if ( (obj->carried_by->in_room->sector_type == SECT_DESERT) &&
		 (!IS_NPC(obj->carried_by)) )
	      if ( number_percent() < 20 )  
	      {
//		act( "$p evaporates.", obj->carried_by, obj, NULL, TO_CHAR );
		extract_obj( obj );
		continue;
	      }
	  }
*/
	  if ( obj->in_room != NULL )
	    if ( obj->in_room->sector_type == SECT_DESERT )
	      if ( number_percent() < 30 )  {
		if ( obj->in_room->people != NULL )  {
		  act( "$p evaporates from the extreme heat.",obj->in_room->people, obj, NULL, TO_ROOM );
		  act( "$p evaporates from the extreme heat.",obj->in_room->people, obj, NULL, TO_CHAR );
	        }
		extract_obj( obj );
		continue;
	      }
	}
/* add code to make people fumble potions */
/*  adds another for fumble code */
ch = obj->carried_by;

   if ( obj->item_type==ITEM_POTION 
	&& obj->carried_by !=NULL
        && !IS_NPC(obj->carried_by) )
   {
      if ( IS_NOT_RESTING (ch)
           && number_percent() < ((25-get_curr_stat(ch,STAT_DEX))* 0.04))
      {
	act( "{CYou clumsily fumble with $p and damage it, it is useless now.{x", ch,obj,NULL,TO_CHAR );
	act( "{C$n clumsily fumbles with $p and ruins it.{x",ch,obj,NULL,TO_ROOM );
	extract_obj( obj );	continue;
      }
   }
/*  end */
/*  adds code to RARELY drop an object out of pure clumsiness */
if ( obj->carried_by !=NULL && !IS_NPC(obj->carried_by) )
{
if ( IS_NOT_RESTING (ch)
     && number_percent() < ((25-get_curr_stat(ch,STAT_DEX))* 0.001) )
     {
act( "{COops! you just dropped $p by accident.{x", 
						ch,obj,NULL,TO_CHAR );
act( "{COops, $n just accidently dropped $p.{x",
						ch,obj,NULL,TO_ROOM );
     continue;
     }
}
/*  end */

/*  :: replaced by falling code
 	if ( obj->timer <= 0 || --obj->timer > 0 )
 	continue;    
 */
 	if ( obj->timer <= 0 || --obj->timer > 0 )
 	{
               if (obj->in_room &&
                obj->in_room->sector_type == SECT_AIR &&
                (obj->wear_flags & ITEM_TAKE) &&
                obj->in_room->exit[5] &&
                obj->in_room->exit[5]->u1.to_room)
               {
               ROOM_INDEX_DATA *new_room = obj->in_room->exit[5]->u1.to_room;
 
               if (( rch = obj->in_room->people ) != NULL )
                  {
                  act( "$p falls away.", rch, obj, NULL, TO_ROOM );
                  act( "$p falls away.", rch, obj, NULL, TO_CHAR );
                  }
 
               obj_from_room(obj);
               obj_to_room(obj, new_room); 
 
               if (( rch = obj->in_room->people ) != NULL )
                  {
                  act( "$p floats by.", rch, obj, NULL, TO_ROOM );
                  act( "$p floats by.", rch, obj, NULL, TO_CHAR );
                  }
               }
 	continue;
 	}
 /*  end :: falling code commented out because of errors */

	switch ( obj->item_type )
	{
	default:              message = "{c$p crumbles into dust.{x"; break;
	case ITEM_FOUNTAIN:   message = "{c$p dries up.{x";         break;
	case ITEM_CORPSE_NPC: message = "{c$p decays into dust.{x"; break;
	case ITEM_CORPSE_PC:  message = "{c$p decays into dust.{x"; break;
	case ITEM_FOOD:       message = "{c$p decomposes.{x";	break;
	case ITEM_POTION:     message = "{c$p has evaporated from disuse.{x";break;
	case ITEM_PORTAL:     message = "{c$p fades out of existence.{x"; break;
	case ITEM_CONTAINER: 
	    if (CAN_WEAR(obj,ITEM_WEAR_FLOAT))
		if (obj->contains)
		    message = 
		"{c$p flickers and vanishes, spilling its contents on the floor.{x";
		else
		    message = "{c$p flickers and vanishes.{x";
	    else
		message = "{c$p crumbles into dust.{x";
	    break;
	}

	if ( obj->carried_by != NULL )
	{
	    if (IS_NPC(obj->carried_by) 
	    &&  obj->carried_by->pIndexData->pShop != NULL)
		obj->carried_by->silver += obj->cost/5;
	    else
	    {
	    	act( message, obj->carried_by, obj, NULL, TO_CHAR );
		if ( obj->wear_loc == WEAR_FLOAT)
		    act(message,obj->carried_by,obj,NULL,TO_ROOM);
	    }
	}
	else if ( obj->in_room != NULL
	&&      ( rch = obj->in_room->people ) != NULL )
	{
	    if (! (obj->in_obj && obj->in_obj->pIndexData->vnum == OBJ_VNUM_PIT
	           && !CAN_WEAR(obj->in_obj,ITEM_TAKE)))
	    {
	    	act( message, rch, obj, NULL, TO_ROOM );
	    	act( message, rch, obj, NULL, TO_CHAR );
	    }
	}

       /* Pit sweeper. In an hour or two, cleans the pit. change 
          pit_count check to 0 to activate. We may need this later, 
          if we observe too much obj's in game.
        */
        pit_count = ++pit_count % 120; /* more or less an hour */
        if (obj->pIndexData->vnum == OBJ_VNUM_PIT &&
            pit_count == 121) {
          for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj) {
            next_obj = t_obj->next_content;
            obj_from_obj(t_obj);
            extract_obj(t_obj);
          }
        }

        if ((obj->item_type == ITEM_CORPSE_PC || obj->wear_loc == WEAR_FLOAT)
	&&  obj->contains)
	{   /* save the contents */
     	    OBJ_DATA *t_obj, *next_obj;

	    for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
	    {
		next_obj = t_obj->next_content;
		obj_from_obj(t_obj);

		if (obj->in_obj) /* in another object */
		    obj_to_obj(t_obj,obj->in_obj);

		else if (obj->carried_by)  /* carried */
		    if (obj->wear_loc == WEAR_FLOAT)
			if (obj->carried_by->in_room == NULL)
			    extract_obj(t_obj);
			else
			    obj_to_room(t_obj,obj->carried_by->in_room);
		    else
		    	obj_to_char(t_obj,obj->carried_by);

		else if (obj->in_room == NULL)  /* destroy it */
		    extract_obj(t_obj);

                else { /* to the pit */
                  for (pit = get_room_index(obj->altar)->contents;
                       pit != NULL && pit->pIndexData->vnum != obj->pit;
                       pit = pit->next);

                  if (pit == NULL)
                    obj_to_room(t_obj,obj->in_room);
                  else obj_to_obj(t_obj,pit);
                }
	    }
	}

	extract_obj( obj );
    }

    return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	wch_next = wch->next;

      if (IS_AWAKE(wch) &&
  IS_AFFECTED(wch,AFF_BLOODTHIRST) && wch->fighting == NULL)    
        {
          for ( vch = wch->in_room->people;
               vch != NULL && wch->fighting == NULL; vch = vch_next)
            {
              vch_next = vch->next_in_room;
              if ( wch != vch && can_see(wch,vch) &&
                  !is_safe_nomessage(wch,vch) )
                {
                  act_color("{rMORE BLOOD! MORE BLOOD! MORE BLOOD!!!{x",
                            wch,NULL,NULL,TO_CHAR,POS_RESTING,0);
                  do_murder(wch,vch->name);
                }
            }
        }


	if ( IS_NPC(wch)
	||   wch->level >= LEVEL_IMMORTAL
	||   wch->in_room == NULL 
	||   wch->in_room->area->empty)
	    continue;

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if   ((!IS_NPC(ch))
	    ||	 (!IS_SET(ch->act, ACT_AGGRESSIVE) && (ch->last_fought == NULL))
	    ||   IS_SET(ch->in_room->room_flags,ROOM_SAFE)
	    ||   IS_AFFECTED(ch,AFF_CALM)
	    ||   ch->fighting != NULL
	    ||   IS_AFFECTED(ch, AFF_CHARM)
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch) )
	    ||   !can_see( ch, wch ) 
	    ||   number_bits(1) == 0
            ||   is_safe_nomessage(ch,wch))

		continue;

          /* Mad mob attacks! */
          if (ch->last_fought == wch
               && !is_affected(ch,skill_lookup("awe"))) {

            sprintf(buf,"%s! Now you die!",
                    (is_affected(wch,gsn_doppelganger) &&
                     !IS_SET(ch->act,PLR_HOLYLIGHT))?
                    PERS(wch->doppel,ch) : PERS(wch,ch));
            do_yell(ch,buf);
            wch = check_guard(wch, ch); /* see if wch is being guarded */

            multi_hit(ch,wch,TYPE_UNDEFINED);
            continue;
          }

          if (ch->last_fought != NULL)
            continue;

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;

		if ( !IS_NPC(vch)
		&&   vch->level < LEVEL_IMMORTAL
		&&   ch->level >= vch->level - 5 
		&&   ( !IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch) )
		&&   can_see( ch, vch ) )
		{
		    if ( number_range( 0, count ) == 0 )
			victim = vch;
		    count++;
		}
	    }

	    if ( victim == NULL )
		continue;

	    if ( !is_safe_nomessage( ch, victim ) ) {
               victim = check_guard(victim, ch); /* see if victim is guarded */
	       multi_hit( ch, victim, TYPE_UNDEFINED );
	    }
	}
    }

    return;
}



/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler( void )
{
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int	    pulse_music;
    static  int	    pulse_raffect;
    static  int	    pulse_underwater;

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= PULSE_AREA;
	/* number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 ); */
        quest_update     ( );
	area_update	( );
	room_update	( );
    }

    if ( --pulse_music	  <= 0 )
    {
	pulse_music	= PULSE_MUSIC;
/*	song_update(); */
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }
    if ( --pulse_raffect <= 0 )
    {
	pulse_raffect = PULSE_RAFFECT;
	room_affect_update( );
    }

    if ( --pulse_underwater    <= 0 )
    {
        pulse_underwater     = PULSE_UNDERWATER;
        underwater_update    ( );
    }

    if ( --pulse_point    <= 0 )
    {
	wiznet("[ -- TICK -- ]",NULL,NULL,WIZ_TICKS,0,0);
	pulse_point     = PULSE_TICK;
/* number_range( PULSE_TICK / 2, 3 * PULSE_TICK / 2 ); */
	weather_update	( );
	char_update	( );
	war_update( );
	obj_update	( );
	check_copyover  ( );
	update_arena();
/*	who_html_update ( ); */
/* next block is for area popularity */
{
CHAR_DATA *ch;

        for (ch = char_list; ch != NULL; ch = ch->next)
            if (!IS_NPC(ch) && ch->in_room != NULL)
                    ch->in_room->area->count =
                      UMIN(ch->in_room->area->count+1,5000000);
}

    }

    aggr_update( );
    auction_update( );
    tail_chain( );

    return;
}

void update_arena( void )
{
	if(arena_counter > 0)
	arena_counter--;
	else
	arena_counter = 0;
}

void check_copyover( void )
{
 char buf[MAX_STRING_LENGTH];
 DESCRIPTOR_DATA *d;

 switch(copyover_counter)
 {
  case -1:
     break;
  case 0:
    sprintf(buf,"***** COPYOVER by System - please remain seated!\n\r");
    for (d = descriptor_list; d != NULL; d = d->next)
          write_to_buffer(d,buf,0);
     copyover_fate();
     return;
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 10:
  case 15:
  case 30:
  case 60:
    sprintf(buf,"***** COPYOVER IN %i TICKS *****\n\r",copyover_counter);
  for (d = descriptor_list; d != NULL; d = d->next)
          write_to_buffer(d,buf,0);
  default:
    copyover_counter--; 
    break;
 }
}

void room_update( void )
{
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *room_next;

    for ( room = top_affected_room; room ; room = room_next )
    {
        AFFECT_DATA *paf;
        AFFECT_DATA *paf_next;

        room_next = room->aff_next;

        for ( paf = room->affected; paf != NULL; paf = paf_next )
        {
            paf_next    = paf->next;
       if ( paf->duration > 0 )
            {
                paf->duration--;
/*
                if (number_range(0,4) == 0 && paf->level > 0)
                  paf->level--;
         spell strength shouldn't fade with time
         because checks safe_rpsell with af->level */
            }
            else if ( paf->duration < 0 )
                ;
            else
            {
                if ( paf_next == NULL
                ||   paf_next->type != paf->type
                ||   paf_next->duration > 0 )
                {
/*
                    if ( paf->type > 0 && skill_table[paf->type].msg_off )
                    {
                        act( skill_table[paf->type].msg_off, ch );
                        send_to_char( "\n\r", ch );
                    }
*/
                }

                affect_remove_room( room, paf );
            }
        }

    }
    return;
}
void room_affect_update( void )
{
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *room_next;

    for ( room = top_affected_room; room ; room = room_next )
    {
        room_next = room->aff_next;

        while (IS_ROOM_AFFECTED(room, AFF_ROOM_PLAGUE) && room->people !=NULL)
        {
            AFFECT_DATA *af, plague;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
                if (af->type == gsn_black_death)
               break;
            }

            if (af == NULL)
            {
                REMOVE_BIT(room->affected_by,AFF_ROOM_PLAGUE);
                break;
            }

            if (af->level == 1)
                af->level = 2;

            plague.where                = TO_AFFECTS;
            plague.type                 = gsn_plague;
            plague.level                = af->level - 1;
            plague.duration             = number_range(1,2 * plague.level);
            plague.location             = APPLY_NONE;
       plague.modifier             = -5;
            plague.bitvector            = AFF_PLAGUE;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(plague.level ,vch,DAM_DISEASE)
                &&  !IS_IMMORTAL(vch)
                &&  !IS_SET(vch->imm_flags,IMM_DISEASE)
                &&  !is_safe_rspell(af->level,vch)
                &&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(2) == 0)
                {
                    send_to_char("You feel hot and feverish.\n\r",vch);
                    act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
                    affect_join(vch,&plague);
                }
            }
         break;
        }
       while (IS_ROOM_AFFECTED(room, AFF_ROOM_POISON) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
                if (af->type == gsn_deadly_venom)
                    break;
            }

            if (af == NULL)
            {
                REMOVE_BIT(room->affected_by,AFF_ROOM_POISON);
                break;
            }

            if (af->level == 1)
                af->level = 2;

            paf.where           = TO_AFFECTS;
            paf.type            = gsn_poison;
            paf.level           = af->level - 1;
            paf.duration        = number_range(1,2 * paf.level);
            paf.location        = APPLY_NONE;
            paf.modifier        = -5;
            paf.bitvector       = AFF_POISON;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level ,vch,DAM_POISON)
                &&  !IS_IMMORTAL(vch)
                &&  !IS_SET(vch->imm_flags,IMM_POISON)
                &&  !is_safe_rspell(af->level,vch)

         &&  !IS_AFFECTED(vch,AFF_POISON) && number_bits(2) == 0)
                {
                    send_to_char("You feel very sick.\n\r",vch);
                    act("$n looks very ill.",vch,NULL,NULL,TO_ROOM);
                    affect_join(vch,&paf);
                }
            }
         break;
        }

        while (IS_ROOM_AFFECTED(room, AFF_ROOM_SLOW) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
             if (af->type == gsn_lethargic_mist)
                    break;
            }

            if (af == NULL)
            {
                REMOVE_BIT(room->affected_by,AFF_ROOM_SLOW);
                break;
            }

            if (af->level == 1)
                af->level = 2;

            paf.where           = TO_AFFECTS;
            paf.type            = gsn_slow;
            paf.level           = af->level - 1;
            paf.duration        = number_range(1,2 * paf.level);
      paf.location        = APPLY_NONE;
            paf.modifier        = -5;
            paf.bitvector       = AFF_SLOW;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level ,vch,DAM_OTHER)
                &&  !IS_IMMORTAL(vch)
                &&  !is_safe_rspell(af->level,vch)
                &&  !IS_AFFECTED(vch,AFF_SLOW) && number_bits(2) == 0)
                {
                    send_to_char("You start to move less quickly.\n\r",vch);
                    act("$n is moving less quickly.",vch,NULL,NULL,TO_ROOM);
                    affect_join(vch,&paf);
                }
            }
         break;
     }


        while (IS_ROOM_AFFECTED(room, AFF_ROOM_CURSE) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
             if (af->type == gsn_cursed_lands)
                    break;
            }

            if (af == NULL)
            {
                REMOVE_BIT(room->affected_by,AFF_ROOM_CURSE);
                break;
            }

            if (af->level == 1)
                af->level = 2;

            paf.where           = TO_AFFECTS;
            paf.type            = gsn_curse;
            paf.level           = af->level - 1;
            paf.duration        = number_range(1,2 * paf.level);
            paf.location        = APPLY_NONE;
            paf.modifier        = -5;
            paf.bitvector       = AFF_ROOM_CURSE;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level ,vch,DAM_OTHER)
                &&  !IS_IMMORTAL(vch)
                &&  (vch->class != 13)
                &&  !is_safe_rspell(af->level,vch)
                &&  !IS_AFFECTED(vch,AFF_ROOM_CURSE) && number_bits(2) == 0)
                {
                    send_to_char("You start to move less quickly.\n\r",vch);
                    act("$n is moving less quickly.",vch,NULL,NULL,TO_ROOM);
                    affect_join(vch,&paf);
                }
            }
         break;
     }


        while (IS_ROOM_AFFECTED(room, AFF_ROOM_SLEEP) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
                if (af->type == gsn_mysterious_dream)
                    break;
            }

            if (af == NULL)
            {
                REMOVE_BIT(room->affected_by,AFF_ROOM_SLEEP);
                break;
        }

            if (af->level == 1)
                af->level = 2;

            paf.where           = TO_AFFECTS;
            paf.type            = gsn_sleep;
            paf.level           = af->level - 1;
            paf.duration        = number_range(1,2 * paf.level);
            paf.location        = APPLY_NONE;
            paf.modifier        = -5;
            paf.bitvector       = AFF_SLEEP;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level - 4,vch,DAM_CHARM)
                &&  !IS_IMMORTAL(vch)
       &&  !is_safe_rspell(af->level,vch)
                &&  !(IS_NPC(vch) && IS_SET(vch->act,ACT_UNDEAD) )
                &&  !IS_AFFECTED(vch,AFF_SLEEP) && number_bits(2) == 0)
                {
                  if (IS_AWAKE(vch))
                   {
                    send_to_char("You feel very sleepy.......zzzzzz.\n\r",vch);
                    act("$n goes to sleep.",vch,NULL,NULL,TO_ROOM);
                    vch->position = POS_SLEEPING;
                   }
                  affect_join(vch,&paf);
                }
            }
         break;
        }
/* new ones here
        while (IS_ROOM_AFFECTED(room, AFF_ROOM_) && room->people != NULL)
        {
            AFFECT_DATA *af, paf;
            CHAR_DATA *vch;

            for ( af = room->affected; af != NULL; af = af->next )
            {
                if (af->type == gsn_)
                    break;
            }

            if (af == NULL)
            {
                REMOVE_BIT(room->affected_by,AFF_ROOM_);
                break;
            }

            if (af->level == 1)
                af->level = 2;

            paf.where           = TO_AFFECTS;
            paf.type            = gsn_;
            paf.level           = af->level - 1;
            paf.duration        = number_range(1,2 * paf.level);
            paf.location        = APPLY_NONE;
            paf.modifier        = -5;
            paf.bitvector       = AFF_;

            for ( vch = room->people; vch != NULL; vch = vch->next_in_room)
            {
                if (!saves_spell(paf.level + 2,vch,DAM_)
                &&  !IS_IMMORTAL(vch)
                &&  !is_safe_rspell(af->level,vch)
           &&  !IS_AFFECTED(vch,AFF_) && number_bits(2) == 0)
                {
                    send_to_char("You feel hot and feverish.\n\r",vch);
                    act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
                    affect_join(vch,&paf);
                }
            }
         break;
        }
*/
    }
    return;
}

void underwater_update( void )
{
   CHAR_DATA *ch;
   CHAR_DATA *ch_next;

return;

   for ( ch = char_list; ch != NULL; ch = ch_next)
   {
    ch_next = ch->next;
    
    if (ch == NULL)
     continue;
     
    if (IS_NPC(ch))
     continue;
  
    if ((ch->in_room->sector_type != SECT_WATER_NOSWIM) && (ch->in_room->sector_type != SECT_WATER_SWIM))
     continue;

    if (IS_IMMORTAL(ch))
     continue;

    if (IS_AFFECTED(ch,AFF_FLYING))
     continue;

/*
    if ((!IS_NPC(ch)
    && !IS_IMMORTAL(ch)
    && ch->in_room->sector_type == SECT_WATER_NOSWIM
    && !IS_AFFECTED(ch,AFF_FLYING)) ||
    (!IS_NPC(ch)
    && !IS_IMMORTAL(ch)
    && ch->in_room->sector_type == SECT_WATER_SWIM
     && !IS_AFFECTED(ch,AFF_FLYING)))*/
  if (0 == 0)
    {
     
        OBJ_DATA *obj;
            bool found;

            /*
             * Look for a boat.
             */
            found = FALSE;
	if(get_skill(ch, gsn_swim) > 1) 
	 check_improve(ch, gsn_swim, TRUE, 1);

	if(number_percent() < get_skill(ch, gsn_swim))
		found = TRUE;

            if (IS_IMMORTAL(ch))
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
     if ( ch->hit > 20)
     {
      ch->position = POS_RESTING;
       ch->hit /= 2;
      send_to_char("You're drowning!!!\n\r", ch);
     }
     else
     {
      ch->hit = 1;
      raw_kill(ch);
      send_to_char("You are DEAD!!\n\r", ch );
     }
     }
    }
   }
}
