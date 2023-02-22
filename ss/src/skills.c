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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"

DECLARE_DO_FUN( do_say		);

void do_disarm	args ( ( CHAR_DATA *ch, char *argument ) );

void disarm	args ( ( CHAR_DATA *ch, CHAR_DATA *victim ));
/* used to converter of prac and train */
void do_gain(CHAR_DATA *ch, char *argument)
{
//    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *trainer;


    char       arg[ MAX_INPUT_LENGTH ];
    char       arg1[ MAX_INPUT_LENGTH ];  //Gain rp
    char       buf[ MAX_INPUT_LENGTH ];
    int      value =0;
    int 	valueXP= 0;
    
    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 ); //Gain rp

  /*  if (!str_prefix(arg,"rp"))
    {
        send_to_char( "Syntax: gain rp <tokens>.\n\r", ch );
        return;
    }

	value = atoi( arg );
	
  	  if (ch->rptoken < value)
  	  {
  	  	send_to_char("You can't convert more tokens then you have.\n\r",ch);
  	  	return;
  	  }


 	
 	if (value > 0)
	  {
	  	ch->rptoken -= value;
	  	valueXP = value *500;
	  	gain_exp(ch, valueXP);
	  	xprintf( buf,"You have converted %d tokens into %d xp.\n\r",value,valueXP);
	  	send_to_char(buf,ch);
	  }
	else
		send_to_char("You can't convert less than 1 token\n\r",ch);





//        send_to_char("Command Disabled.\n\r",ch);
        return;



    if (IS_NPC(ch))
        return;
*/
    /* find a trainer */
    for ( trainer = ch->in_room->people;
          trainer != NULL;
     trainer = trainer->next_in_room)

        if (IS_NPC(trainer) && IS_SET(trainer->act,ACT_TRAIN))
            break;

    if (trainer == NULL || !can_see(ch,trainer))
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

//    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
				do_say(trainer,"You may convert 10 practices into 1 train.");
        do_say(trainer,"You may revert 1 train into 10 practices.");
        do_say(trainer,"You may convert 1 rptoken into 500 experiance points..");
    		do_say(trainer,"Simply type 'gain convert' or 'gain revert' or 'gain rp (tokens)'.");
        return;
    }

    if (!str_prefix(arg,"revert"))
    {
        if (ch->train < 1)
        {
            act("$N tells you 'You are not yet ready.'",
                ch,NULL,trainer,TO_CHAR);
            return;
        }

        act("$N helps you apply your training to practice",
                ch,NULL,trainer,TO_CHAR);
        ch->practice += 10;
        ch->train -=1 ;
  return;
    }

    if (!str_prefix(arg,"convert"))
    {
        if (ch->practice < 10)
        {
            act("$N tells you 'You are not yet ready.'",
                ch,NULL,trainer,TO_CHAR);
            return;
        }

        act("$N helps you apply your practice to training",
                ch,NULL,trainer,TO_CHAR);
        ch->practice -= 10;
        ch->train +=1 ;
        return;
        }
        
        if (!str_prefix(arg,"rp"))
        {
	value = atoi( arg );
	value = atoi( arg1 ); //Gain rp
	
  	  if (ch->rptoken < value)
  	  {
  	  	send_to_char("You can't convert more tokens then you have.\n\r",ch);
  	  	return;
  	  }
if (value > 0)
	  {
	  	ch->rptoken -= value;
	  	valueXP = value *500;
	  	gain_exp(ch, valueXP);
	  	xprintf( buf,"You have converted %d tokens into %d xp.\n\r",value,valueXP);
	  	send_to_char(buf,ch);
	  }
	else
		send_to_char("You can't convert less than 1 token\n\r",ch);





//        send_to_char("Command Disabled.\n\r",ch);
        return;



    if (IS_NPC(ch))
        return;
  }

    act("$N tells you 'I do not understand...'",ch,NULL,trainer,TO_CHAR);

}


/* RT spells and skills show the players spells (or skills) */

void do_spells(CHAR_DATA *ch, char *argument)
{
 
    
    BUFFER *output; 
    char spell_list[LEVEL_HERO][MAX_STRING_LENGTH];
    char spell_columns[LEVEL_HERO];
    int sn,lev,mana;
    bool found = FALSE;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
      return;

    output = new_buf();



    
    
    /* initilize data */
    for (lev = 0; lev <= LEVEL_HERO; lev++)
    {
        spell_columns[lev] = 0;
        spell_list[lev][0] = '\0';
    }


    for (sn = 0; sn < MAX_SKILL; sn++)
    {
      if (skill_table[sn].name == NULL)
        break;


      if (skill_table[sn].skill_level[ch->class] < LEVEL_HERO &&
          skill_table[sn].spell_fun != spell_null &&
(skill_table[sn].kingdom == ch->kingdom || skill_table[sn].kingdom == KINGDOM_NONE)         )
      {
            
        found = TRUE;
        lev = skill_table[sn].skill_level[ch->class];
        if (ch->level < lev)
          xprintf(buf,"{W%-18s  {Cn/a{w      ", skill_table[sn].name);
        else
        {
          mana = mana_cost (ch,skill_table[sn].min_mana,lev);
          xprintf(buf,"{W%-18s  {C%3d mana{w	",skill_table[sn].name,mana);
        }


        if (spell_list[lev][0] == '\0')
          xprintf(spell_list[lev],"\n\r{WLevel %2d: {w%s",lev,buf);
   else /* append */
        {
          
          if ( ++spell_columns[lev] % 2 == 0)
            strcat(spell_list[lev],"\n\r          ");
         

          strcat(spell_list[lev],buf);
        }
      }
    }

    /* return results */


    if (!found)
    {
      send_to_char("You know no spells.\n\r",ch);
      return;
    }

    for (lev = 0; lev < LEVEL_HERO; lev++)
      if (spell_list[lev][0] != '\0')
      add_buf(output,spell_list[lev]);
	add_buf(output,"\n\r");
	page_to_char(buf_string(output),ch);
	free_buf(output);
//    send_to_char(spell_list[lev],ch);
//      send_to_char("\n\r",ch);
}

void do_skills(CHAR_DATA *ch, char *argument)
{
    BUFFER *output;
    char skill_list[LEVEL_HERO][MAX_STRING_LENGTH];
    char skill_columns[LEVEL_HERO];
    int sn,lev;
    bool found = FALSE;
    char buf[MAX_STRING_LENGTH];
    

	
    if (IS_NPC(ch))
      return;

//send_to_char("BUG BUG LOCACTE --- A ---\n\r",ch);	  
    output = new_buf();

 /* initilize data */
    for (lev = 0; lev <= LEVEL_HERO; lev++)
    {
        skill_columns[lev] = 0;
        skill_list[lev][0] = '\0';
    }

//	      send_to_char("BUG BUG LOCACTE --- B ---\n\r",ch);
    for (sn = 0; sn < MAX_SKILL; sn++)
    {
      if (skill_table[sn].name == NULL )
        break;

//send_to_char("BUG BUG LOCACTE --- C ---\n\r",ch);
	    
//return;
		
      if (skill_table[sn].skill_level[ch->class] <= LEVEL_HERO &&
          skill_table[sn].spell_fun == spell_null &&
(skill_table[sn].kingdom == ch->kingdom || skill_table[sn].kingdom ==
KINGDOM_NONE)
         )
      {
        found = TRUE;
        lev = skill_table[sn].skill_level[ch->class];
        if (ch->level < lev)

          xprintf(buf,"{W%-18s {Cn/a{w      ", skill_table[sn].name);
        else
          xprintf(buf,"{W%-18s {C%3d%%{w      ",skill_table[sn].name,
                                         ch->pcdata->learned[sn]);

        if (skill_list[lev][0] == '\0')
          xprintf(skill_list[lev],"\n\r{WLevel %2d: {w%s",lev,buf);
        else /* append */
        {
          if ( ++skill_columns[lev] % 2 == 0)
            strcat(skill_list[lev],"\n\r          ");
          strcat(skill_list[lev],buf);
   }
      }
    }

    /* return results */

    if (!found)
    {
      send_to_char("You know no skills.\n\r",ch);
      return;
    }
//send_to_char("BUG BUG LOCACTE --- D ---\n\r",ch);
    for (lev = 0; lev < LEVEL_HERO; lev++)
      if (skill_list[lev][0] != '\0')
	add_buf(output,skill_list[lev]);
	add_buf(output,"\n\r");

	
	page_to_char(buf_string(output),ch);
	free_buf(output);
//        send_to_char(skill_list[lev],ch);

}


int base_exp(CHAR_DATA *ch, int points)
{
  int expl;

  if (IS_NPC(ch))    return 1500;
  expl = 1500 + pc_race_table[ch->race].points +
                        class_table[ch->class].points + ch->level*100;
  
  return expl;// * pc_race_table[ch->race].class_mult[ch->class]/100);
}

int exp_to_level(CHAR_DATA *ch, int points)
{
 int base;

  base = base_exp(ch,points);
  if (ch->level == 76)
  base = base_exp(ch,points) + 5000;
  else if (ch->level == 77)
  base = base_exp(ch,points) + 12500;
  else if (ch->level == 78)
  base = base_exp(ch,points) + 20000;
  else if (ch->level == 79)
  base = base_exp(ch,points) + 35000;
  else if (ch->level == 80)
  base = base_exp(ch,points) + 50000;
  else if (ch->level == 81)
  base = base_exp(ch,points) + 60000;

  return (base - exp_this_level(ch,ch->level,points) );
}

int exp_this_level(CHAR_DATA *ch, int level, int points)
{
  int base;

  base = base_exp(ch,points);
  return (ch->exp - (ch->level * base ) );
}


int exp_per_level(CHAR_DATA *ch, int points)
{
    int expl;

    if (IS_NPC(ch))
        return 1500;
 expl = 1500 + pc_race_table[ch->race].points +
                        class_table[ch->class].points + ch->level*100;
   return expl; //* pc_race_table[ch->race].class_mult[ch->class]/100;
}

/* checks for skill improvement */
void check_improve( CHAR_DATA *ch, int sn, bool success, int multiplier )
{
    int chance;
    char buf[100];

    if (IS_NPC(ch))
        return;

    if (ch->level < skill_table[sn].skill_level[ch->class]
    ||  skill_table[sn].rating[ch->class] == 0
    ||  ch->pcdata->learned[sn] == 0
    ||  ch->pcdata->learned[sn] == 100)
        return;  /* skill is not known */
/* 
  if(!IS_SET(ch->quest, QUEST_WEAPONSMITH ) && ch->level >= 40) 
	return;
   if(!IS_SET(ch->quest, QUEST_GOLEM ) && ch->level >= 60) 
	return;
*/
    /* check to see if the character has a chance to learn */
    chance = 10 * int_app[get_curr_stat(ch,STAT_INT)].learn;
    chance /= (         multiplier
                *       ((skill_table[sn].rating[ch->class]*2))
                *       4);
    chance += ch->level;

    if (number_range(1,1000) > chance)
        return;

    /* now that the character has a CHANCE to learn, see if they really
have */

    if (success)
    {
        chance = URANGE(5,100 - ch->pcdata->learned[sn], 95);
        if (number_percent() < chance)
 {
            xprintf(buf,"You have become better at %s!",
                    skill_table[sn].name);
            act_new(buf,ch,NULL,NULL,TO_CHAR,POS_DEAD);
            ch->pcdata->learned[sn]++;
            gain_exp(ch,2 * skill_table[sn].rating[ch->class]);
        }
    }

    else
    {
        chance = URANGE(5,ch->pcdata->learned[sn]/2,30);
        if (number_percent() < chance)
        {
            xprintf(buf,
                "You learn from your mistakes, and your %s skill improves.",
                skill_table[sn].name);
            act_new(buf,ch,NULL,NULL,TO_CHAR,POS_DEAD);
            ch->pcdata->learned[sn] += number_range(1,3);
            ch->pcdata->learned[sn] = UMIN(ch->pcdata->learned[sn],100);
            gain_exp(ch,2 * skill_table[sn].rating[ch->class]);
        }
    }
}


/* use for adding all skills available for that ch  */
void group_add( CHAR_DATA *ch )
{
    int sn;

    if (IS_NPC(ch)) /* NPCs do not have skills */
        return;

     for (sn = 0;  sn < MAX_SKILL; sn++)
          if (skill_table[sn].kingdom == 0 
                && ch->pcdata->learned[sn] < 1
              && skill_table[sn].skill_level[ch->class] < LEVEL_IMMORTAL)
            ch->pcdata->learned[sn] = 1;
        return;

}


void do_slist(CHAR_DATA *ch, char *argument)
{
    char skill_list[LEVEL_HERO][MAX_STRING_LENGTH];
    char skill_columns[LEVEL_HERO];
    int sn,lev,class;
    bool found = FALSE;
    char output[4*MAX_STRING_LENGTH];
 char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
      return;

    output[0] = '\0';
    argument = one_argument(argument, arg);
    if (arg[0] == '\0')
        {
         send_to_char("syntax: slist <class name>.\n\r",ch);
         return;
        }
    class = class_lookup(arg);
    if (class == -1)
        {
         send_to_char("That is not a valid class.\n\r",ch);
     return;
        }
    /* initilize data */
    for (lev = 0; lev < LEVEL_HERO; lev++)
    {
        skill_columns[lev] = 0;
        skill_list[lev][0] = '\0';
    }

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
      if (skill_table[sn].name == NULL )
        break;


      if (skill_table[sn].skill_level[class] < LEVEL_HERO &&
          skill_table[sn].kingdom == KINGDOM_NONE )
      {
        found = TRUE;
        lev = skill_table[sn].skill_level[class];
        xprintf(buf,"%-18s          ",skill_table[sn].name);
        if (skill_list[lev][0] == '\0')
          xprintf(skill_list[lev],"\n\rLevel %2d: %s",lev,buf);
        else /* append */
        {
          if ( ++skill_columns[lev] % 2 == 0)
            strcat(skill_list[lev],"\n\r          ");
          strcat(skill_list[lev],buf);
        }
      }
    }

    /* return results */

    if (!found)
    {
      send_to_char("That class know no skills.\n\r",ch);
      return;
    }

    for (lev = 0; lev < LEVEL_HERO; lev++)
      if (skill_list[lev][0] != '\0')
        strcat(output,skill_list[lev]);
    strcat(output,"\n\r");
    page_to_char(output,ch);
}


/* returns group number */
int group_lookup (const char *name)
{
   int gr;

   for ( gr = 0; prac_table[gr].sh_name != NULL; gr++)
   {
        if (LOWER(name[0]) == LOWER(prac_table[gr].sh_name[0])
        &&  !str_prefix( name,prac_table[gr].sh_name))
            return gr;
   }

   return -1;
}


/* Zardic temp steal fix start*/
void get_objx( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    char buffer[100];

    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
	send_to_char( "You can't take that.\n\r", ch );
	return;
    }

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
	act( "$d: you can't carry that many items.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if ((!obj->in_obj || obj->in_obj->carried_by != ch)
    &&  (get_carry_weight(ch) + get_obj_weight(obj) > can_carry_w(ch)))
    {
	act( "$d: you can't carry that much weight.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if (!can_loot(ch,obj))
    {
	act("Corpse looting is not permitted.",ch,NULL,NULL,TO_CHAR );
	return;
    }

    if (obj->in_room != NULL)
    {
	for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
	    if (gch->on == obj)
	    {
		act("$N appears to be using $p.",
		    ch,obj,gch,TO_CHAR);
		return;
	    }
    }
		

    if ( container != NULL )
    {
/*        if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  get_trust(ch) < obj->level)
	{
	    send_to_char("You are not powerful enough to use it.\n\r",ch);
	    return;
        } */

    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  !CAN_WEAR(container, ITEM_TAKE)
	&&  !IS_OBJ_STAT(obj,ITEM_HAD_TIMER))
	    obj->timer = 0;	
	act( "You tries to steal $p from $P.", ch, obj, container,TO_CHAR );
/*	act( "$n gets $p from $P.", ch, obj, container, TO_ROOM ); */
	REMOVE_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	obj_from_obj( obj );
    }
    else
    {
	act( "You get $p.", ch, obj, container, TO_CHAR );
	act( "$n gets $p.", ch, obj, container, TO_ROOM );
	obj_from_room( obj );
    }

    if ( obj->item_type == ITEM_MONEY)
    {
        ch->silver += obj->value[1];
        ch->gold += obj->value[2];

        if (IS_SET(ch->act,PLR_AUTOSPLIT))
        { /* AUTOSPLIT code */
    	  members = 0;
    	  for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	  {
            if (!IS_AFFECTED(gch,AFF_CHARM) && is_same_group( gch, ch ) )
              members++;
    	  }

	  if ( members > 1 && (obj->value[0] > 1 || obj->value[1]))
	  {
	    xprintf(buffer,"%d %d",obj->value[0],obj->value[1]);
	    do_split(ch,buffer);	
	  }
        }
 
	extract_obj( obj );
    }
    else
    {
	obj_to_char( obj, ch );
    }

    return;
}
/* Zardic temp steal fix end*/

void do_bagpeek( CHAR_DATA *ch, char *argument )
{    
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    xprintf(buf,"in");
    one_argument( buf, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );


   if (    !IS_NPC( ch )
        && ch->level < skill_table[gsn_bagpeek].skill_level[ch->class] )
   {
      send_to_char( "You do not know how to do that...\n\r", ch );
      return;
   }

    
    if ( ( victim = get_char_room( ch, NULL, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;


    if ( arg3[0] == '\0' )
     {
      send_to_char( "At what will you greedy small eye look?\n\r", ch );
      return;
     } 

    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    { 
	/* 'look in' */

   if ( victim != ch
    &&   !IS_NPC(ch)
    &&   number_percent( ) < get_skill(ch,gsn_bagpeek))
    {
/*	send_to_char( "\n\r{rYou {Wpeek {rat the inventory:{x\n\r", ch ); */
	check_improve(ch,gsn_bagpeek,TRUE,3);

    

	if ( ( obj = get_obj_here( victim,NULL, arg3 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }
	
	
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    xprintf( buf, "It's %sfilled with  a %s liquid.\n\r",
		obj->value[1] <     obj->value[0] / 4
		    ? "less than half-" :
		obj->value[1] < 3 * obj->value[0] / 4
		    ? "about half-"     : "more than half-",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p holds:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
     send_to_char( "You did not get a look in the container.\n\r", ch );
     	return;
    }
         	return;
    }

void do_bagsteal( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
//    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    CHAR_DATA *victim;
//    CHAR_DATA *gch;
//    bool found;
    int percent;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    
    
    send_to_char( "Bagsteal <name> <obj> <container>\n\r", ch );
    
    if ( ( victim = get_char_room( ch, NULL,  arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
        
    if (is_safe(ch,victim))
	return;


    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
     {
      send_to_char("Don't be so greedy!\n\r",ch);
      return;
     }
      	

	if ( ( container = get_obj_here( victim, NULL,arg3 ) ) == NULL )
	{
	    act( "I see no $T here.", ch, NULL, arg3, TO_CHAR );
	    return;
	}
     switch ( container->item_type )
	{
	default:
	    send_to_char( "That's not a container.\n\r", ch );
	    return;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	    break;

	case ITEM_CORPSE_PC:
	    {

		if (!can_loot(ch,container))
		{
		    send_to_char( "You can't do that.\n\r", ch );
		    return;
		}
	    }
	}

	if ( IS_SET(container->value[1], CONT_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	    return;
	}

	    obj = get_obj_list( ch, arg2, container->contains );
	    if ( obj == NULL )
	    {
		act( "I see nothing like that in the $T.",
		    ch, NULL, arg3, TO_CHAR );
		return;
	    }


    WAIT_STATE( ch, skill_table[gsn_bagsteal].beats );

    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );
    percent += can_see( victim, ch ) ? -10 : 0;
    percent -= 5;

    if ( /* ch->level + 5 < victim->level || */
         victim->position == POS_FIGHTING
    || ( !IS_NPC(ch) && percent > ch->pcdata->learned[gsn_bagsteal] ) )
    {
	/*
	 * Failure.
	 */
	send_to_char( "Oops.\n\r", ch );
	affect_strip(ch,gsn_sneak);
	REMOVE_BIT(ch->affected_by,AFF_SNEAK);
        xprintf(buf,"%s tries to steal from %s!",ch->name,victim->name);
        wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);

	act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );
	switch(number_range(0,3))
	{
	case 0 :
	   xprintf( buf, "%s is a lousy thief!", ch->name );
	   break;
        case 1 :
	   xprintf( buf, "%s couldn't rob %s way out of a paper bag!",
		    ch->name,(ch->sex == 2) ? "her" : "his");
	   break;
	case 2 :
	    xprintf( buf,"%s tried to rob me!",ch->name );
	    break;
	case 3 :
	    xprintf(buf,"Keep your hands out of there, %s!",ch->name);
	    
	    break;
        }
        if (!IS_AWAKE(victim))
            do_wake(victim,"");
	if (IS_AWAKE(victim))
            do_yell( victim, buf );
	if ( !IS_NPC(ch) )
	{
	    if ( IS_NPC(victim) )
	    {
	        check_improve(ch,gsn_bagsteal,FALSE,2);
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	    else
	    {
 /*	     get_objx( ch, obj, container ); */
 	     wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
		if ( !IS_SET(ch->act, PLR_THIEF) )
		{
/*		    SET_BIT(ch->act, PLR_THIEF);
		    send_to_char( "*** You are now a THIEF!! ***\n\r", ch ); */
		    return;
                 }
           return;
               }
    return;         
              } 
              
    return;
}
get_objx( ch, obj, container );

xprintf(buf,"%s steals from %s [%d] %s!",ch->name,victim->name,obj->pIndexData->vnum,obj->name);
 if (!IS_NPC(victim))
   log_string( buf );
wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
save_char_obj( ch );
return; 
}

void do_jab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_jab].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }


    if (arg[0] == '\0')
    {
  	  victim = ch->fighting;
  	  if (victim == NULL)
      {
  	    send_to_char("But you aren't fighting anyone!\n\r",ch);
  	    return;
  	  }
    }
 
    else if ((victim = get_char_room( ch, NULL,  arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    

    if ( victim == ch )
    {
	   send_to_char( "How are you going to jab yourself exactly???\n\r", ch );
	   return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is your friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }


    if ( is_safe( ch, victim ) )
      return;

    if ( (( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL) || 
	      (obj->value[0]!=WEAPON_SPEAR)
	   )
    {
	  send_to_char( "You need to wield a spear in order to jab.\n\r", ch );
	  return;
    }

    WAIT_STATE( ch, skill_table[gsn_jab].beats );

    if ( number_percent( ) < get_skill(ch,gsn_jab)
    || ( get_skill(ch,gsn_jab) >= 2 && !IS_AWAKE(victim) ) )
     { 
      check_improve(ch,gsn_jab,TRUE,1);
      one_hit( ch, victim, gsn_jab ,FALSE);
     }
    else
     {
     check_improve(ch,gsn_jab,FALSE,1);
     damage( ch, victim, 0, gsn_jab,DAM_NONE,TRUE);
     }
  return;
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
  && skill_table[sn1].target == TAR_CHAR_OFFENSIVE
  && (*skill_table[sn1].spell_fun) != (*spell_null) )
        legal1 = TRUE;

  if ( sn2 != -1
  && get_skill( ch, sn2 )
  && skill_table[sn2].target == TAR_CHAR_OFFENSIVE
  && (*skill_table[sn2].spell_fun) != (*spell_null) )
        legal2 = TRUE;

  if ( !legal1 && !legal2 )
        {
        WAIT_STATE( ch, skill_table[gsn_multiburst].beats );
        send_to_char( "Your multiburst fails.\n\r", ch );
        check_improve(ch,gsn_multiburst,FALSE,0);
        return;
        }
  cost1 = skill_table[sn1].min_mana;
  cost2 = skill_table[sn2].min_mana;

  cost += cost1;
  cost += cost1 * 0.2;
  cost += cost2;
  cost += cost2 * 0.2;;

  if ( ch->mana < cost && ch->level < LEVEL_IMMORTAL )
        {
	WAIT_STATE( ch, skill_table[gsn_multiburst].beats );
        send_to_char( "You don't have enough mana to multiburst these spells.\n\r",ch);
        return;
        }

  WAIT_STATE( ch, skill_table[gsn_multiburst].beats );

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
 
void do_vild( CHAR_DATA *ch, char *argument)
{
    int chance;
    //hp_percent;
    
    if ((ch->mana < 25) || (ch->move < 10))
    {
	send_to_char("{cYou are to drained to start aiming.{x\n\r",ch);
	return;
    }

/*
    if (ch->level < skill_table[gsn_aim].skill_level[ch->class])
    {
	send_to_char("{cYou turn red in the face, but nothing happens.{x\n\r",ch);
	return;
    }
*/

    /* damage -- below 50% of hp helps, above hurts */
     chance = ch->pcdata->learned[gsn_aim] / 2;



  if (is_affected(ch,gsn_aim))
    { 
      send_to_char("You cannot plan you attacks any better.\n\r",ch);
      return;
    }

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);

	send_to_char("{cYour attacks are now planned!{x\n\r",ch);
	check_improve(ch,gsn_aim,TRUE,2);

	af.where	= TO_AFFECTS;
	af.type		= gsn_aim;
	af.level	= ch->level;
	af.duration	= 3;
	af.modifier	= number_range(ch->level/2,ch->level +1);
 	af.bitvector = 0;
	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

    }

    else
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->mana -= 25;
	ch->move -= 10;

	send_to_char("{cYou failed to plan you attacks.{x\n\r",ch);
	check_improve(ch,gsn_aim,FALSE,2);
    }
}

void do_bandage( CHAR_DATA *ch, char *argument)
{
    int heal;

    if (!kingdom_ok(ch,gsn_bandage))
      return;

    if ( get_skill(ch,gsn_bandage) == 0
        ||  IS_NPC(ch) ||
        (!IS_NPC(ch)
         && ch->level < skill_table[gsn_bandage].skill_level[ch->class]))
    {
        send_to_char("Huh?.\n\r",ch);
        return;
  }

    if (IS_AFFECTED(ch,AFF_REGENERATION))
    {
        send_to_char("You have already using your bandage.\n\r",ch);
        return;
    }


    if (number_percent() < get_skill(ch,gsn_bandage) )
    {
        AFFECT_DATA af;

        WAIT_STATE(ch, PULSE_VIOLENCE);


        send_to_char("You place your bandage to your shoulder!\n\r",ch);
   act("$n places a bandage to $s shulder.",ch,NULL,NULL,TO_ROOM);
        check_improve(ch,gsn_bandage,TRUE,2);

        heal = dice(4, 8) + ch->level / 2;
        ch->hit = UMIN( ch->hit + heal, ch->max_hit );
        update_pos( ch );
        send_to_char( "You feel better!\n\r", ch);
 
        af.where        = TO_AFFECTS;
        af.type         = gsn_bandage;
        af.level        = ch->level;
        af.duration     = ch->level / 10;
        af.modifier     = UMIN(15,ch->level/2);
        af.bitvector    = AFF_REGENERATION;
        af.location     = 0;
        affect_to_char(ch,&af);
 }

    else
    {
     WAIT_STATE(ch, PULSE_VIOLENCE);

     send_to_char("You failed to place your bandage to your shoulder.\n\r", ch);
     //check_improve(ch,gsn_bloodthirst,FALSE,2);
	check_improve(ch,gsn_bandage,FALSE,2);   // Derkec
    }
}

void do_charge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int dam, wait;

    one_argument(argument,arg);

    if (!IS_NPC(ch)
    && ch->level < skill_table[gsn_charge].skill_level[ch->class])
    {
	send_to_char("{cYou are not trained in this skill.{x\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }
    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You can't reach.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    obj = get_eq_char(ch, WEAR_WIELD);
    if (obj == NULL)
	{
	send_to_char("You cannot charge without an weapon!\n\r",ch);
	return;
	}

    if (obj->value[0] != WEAPON_POLEARM && obj->value[0] != WEAPON_LANCE)
      {
	send_to_char("You need to be wielding a polearm or lance!!\n\r",ch);
	return;
	}

    if ( ch->fighting == victim )
    {
	send_to_char( "{cYou can't charge someone you fighting face to face.{x\n\r", ch );
	return;
    }

    if (is_affected(victim, gsn_protective_shield))
     {
	act("{cYour charge seems to bounce off $N.{x", ch, NULL, victim,TO_CHAR);
	act("{c$n's charge seems to bounce off you.{x", ch, NULL, victim,
	    TO_VICT);
	act("{c$n's charge seems to bounce off $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }

    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      return;
      
      
    
    dam = 0;//GET_DAMROLL(ch);
    dam += number_range(10, ch->level * 4);
    dam += number_range (GET_DAMROLL(ch) /5, GET_DAMROLL(ch));

     

 if (MOUNTED(ch))
     dam = dam * 2;

    WAIT_STATE(ch, skill_table[gsn_charge].beats);
    if (IS_NPC(ch) || number_percent() < (get_skill(ch, gsn_charge)*0.7))
    {
	act("{c$n charges $N, and knocks them over!{x",
	    ch, NULL, victim, TO_NOTVICT);
	act("{c$n charges right into you!{x",
	    ch, NULL, victim, TO_VICT);
	act("{cYou charge right into $N, and knock $M over!{x",
	    ch, NULL, victim, TO_CHAR);
	damage( ch, victim, dam, gsn_charge, DAM_BASH, TRUE );
	check_improve(ch, gsn_charge, TRUE, 1);
	wait = 1;

 if (MOUNTED(ch))
    wait = wait * 2;
    
	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 1; break;
	case 2: wait = 2; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	victim->position = POS_RESTING;

    }
    else
    {
	act("{c$n charges $N, but runs right past!{x",
	    ch, NULL, victim, TO_NOTVICT);
	act("{c$n charges you, but runs right past!{x",
	    ch, NULL, victim, TO_VICT);
	act("{cYou try to charge $N, but run past $M!{x",
	    ch, NULL, victim, TO_CHAR);
	damage( ch, victim, 0, gsn_charge, DAM_BASH, TRUE );
	check_improve(ch, gsn_charge, FALSE, 1);
    }
    return;
}


void do_poison( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *tmp_vict;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)
    || ch->level < skill_table[gsn_poison_smoke].skill_level[ch->class])
    {
	send_to_char("{CHuh?{x\n\r",ch);
	return;
    }

    if (ch->position < skill_table[gsn_poison_smoke].minimum_position)
    {
	send_to_char("{cYou can't concentrage enough.{x\n\r",ch);
	return;
    }

    if (ch->mana < 20)
    {
	send_to_char("{cYou don't have enought mana.{x\n\r",ch);
	return;
    }

    send_to_char("{CA cloud of poison smoke fills in the room.{x\n\r",ch);
    act("{CA cloud of poison smoke fills the room.{x",ch,NULL,NULL,TO_ROOM);
    WAIT_STATE(ch, skill_table[gsn_poison_smoke].beats);
    if (number_percent() > ch->pcdata->learned[gsn_poison_smoke])
    {
	send_to_char("{cYou lost your concentration.{x\n\r",ch);
	check_improve(ch, gsn_poison_smoke, FALSE, 1);
	ch->mana -= 10;
	return;
    }
    else
    {
	for (tmp_vict = ch->in_room->people; tmp_vict != NULL;
	    tmp_vict = tmp_vict->next_in_room)

	    if (!is_safe_nomessage(ch, tmp_vict))
	    {
		if (!IS_NPC(ch) && tmp_vict != ch && ch->fighting != tmp_vict
		&& tmp_vict->fighting != ch
		&& (IS_SET(tmp_vict->affected_by, AFF_CHARM)
		|| !IS_NPC(tmp_vict)))
		{
		    if (!can_see(tmp_vict, ch))
	do_yell(tmp_vict,"Help! Someone is attacking me!");
		    else
		    {
			xprintf(buf,"Die, %s, you sorcerous dog!{x",
			(is_affected(ch, gsn_doppelganger)
			&& !IS_IMMORTAL(tmp_vict)) ? ch->doppel->name :
			ch->name);
	do_yell(tmp_vict,buf);
		    }
		}
		if (tmp_vict != ch)
		{
		    spell_poison(gsn_poison, ch->level, ch,
			tmp_vict, TARGET_CHAR);
		    multi_hit(tmp_vict, ch, TYPE_UNDEFINED);
		}
	    }
	ch->mana -= 20;
	check_improve(ch, gsn_poison_smoke, TRUE, 1);
	return;
    }
}

void do_shieldbash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    int Xchance = 0;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    OBJ_DATA *obj;

     wait = 0;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_shieldbash)) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_shieldbash].skill_level[ch->class]))
    {
	send_to_char("{cShield Bashing? What's that?{x\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }
    
    if (ch->submission > 0)
	{
	send_to_char( "{cYou can't do that in an armlock{x\n\r", ch );
	return;
    	}

    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x{x\n\r",ch);
	return;
    }
if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].skill_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= chance)
        {
        check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
         send_to_char( "{cSomeone trying to sneek up on you.{x\n\r", victim );
	 return;
        }
        
     }
        
if (!IS_NPC(victim))     
      if(number_percent() < victim->pcdata->learned[gsn_reaction]/ 1.2)
	{
	check_improve(victim,gsn_reaction,TRUE,1);
	WAIT_STATE(ch, skill_table[gsn_shieldbash].beats * 3/2);
	act("{cYou reacted and dodge $n's bash!{x",ch,NULL,victim,TO_VICT);
	act("{c$N, reacts and dodges your bash!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n reacts and dodges $N's powerful bash.{x",ch,NULL,victim,TO_NOTVICT);
	return;
	}

    if (victim->position < POS_FIGHTING)
    {
	act("{cYou'll have to let $M get back up first.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cYou try to hit your head with your shield, but fail.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( ( obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
{
	send_to_char("{cYou need a shield to bash with it!{x\n\r",ch);
	return;
 }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is your friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }
/*
    if (is_affected(victim, gsn_protective_shield))
     {
	act("{cYour shield bash seems to slide around $N.{x", ch, NULL, victim, TO_CHAR);
	act("{c$n's bash slides off your protective shield.{x", ch, NULL, victim,
	    TO_VICT);
	act("{c$n's bash seems to slide around $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }
 */
    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_shieldbash].beats * 3/2);
       return;
      }
    /* modifiers */

    if(victim->kingdom == KINGDOM_DRAKKON)
     if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_shieldbash].beats * 3/2);
       return;
      }
    /* modifiers */





    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 10;

    /* level */
    chance += (ch->level - victim->level) * 2;
    chance -= ch->lagattack * 5;
    /* now the attack */
    if (number_percent() < chance)
    {

	act("{c$n sends you sprawling with a powerful shield bash!{x",
		ch,NULL,victim,TO_VICT);
	act("{cYou slam into $N with your shield, and send $M flying!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n sends $N sprawling with a powerful bash with his shield.{x",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_shieldbash,TRUE,1);

	wait = 3;

	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

//	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats);
	victim->position = POS_RESTING;
        if (victim->lagattack < 9)
	  victim->lagattack = 9;
	if (victim->lagflee   < 5)
	 victim->lagflee   = 5;
	if (victim->lagstand   <5)
	  victim->lagstand  = 5;
	if (victim->lagspell   < 5)
	  victim->lagspell = 5;
	damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/10),gsn_shieldbash,
	    DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_shieldbash,DAM_BASH, TRUE);
	act("{cYou fall flat on your face!{x",
	    ch,NULL,victim,TO_CHAR);
	act("{c$n falls flat on $s face.{x",
	    ch,NULL,victim,TO_NOTVICT);
	act("{cYou evade $n's shield bash, causing $m to fall flat on $s face.{x",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_shieldbash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! Someone is bashing me with a shield!");
	else
	  {
	    xprintf(buf, "Help! %s is bashing me with a shield!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	do_yell(victim,buf);
	  }
      }
}

void do_blindness( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *tmp_vict;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)
    || ch->level < skill_table[gsn_blindness_dust].skill_level[ch->class])
    {
	send_to_char("{CHuh?{x\n\r",ch);
	return;
    }

    if (ch->position < skill_table[gsn_blindness_dust].minimum_position)
    {
	send_to_char("{cYou can't concentrage enough.{x\n\r",ch);
	return;
    }

    if (ch->mana < 20)
    {
	send_to_char("{cYou don't have enought mana.{x\n\r",ch);
	return;
    }

    send_to_char("{CA cloud of dust fills in the room.{x\n\r",ch);
    act("{CA cloud of dust fills the room.{x",ch,NULL,NULL,TO_ROOM);
    WAIT_STATE(ch, skill_table[gsn_blindness_dust].beats);
    if (number_percent() > ch->pcdata->learned[gsn_blindness_dust])
    {
	send_to_char("{cYou lost your concentration.{x\n\r",ch);
	check_improve(ch, gsn_blindness_dust, FALSE, 1);
	ch->mana -= 10;
	return;
    }
    else
    {
	for (tmp_vict = ch->in_room->people; tmp_vict != NULL;
	    tmp_vict = tmp_vict->next_in_room)

	    if (!is_safe_nomessage(ch, tmp_vict))
	    {
		if (!IS_NPC(ch) && tmp_vict != ch && ch->fighting != tmp_vict
		&& tmp_vict->fighting != ch
		&& (IS_SET(tmp_vict->affected_by, AFF_CHARM)
		|| !IS_NPC(tmp_vict)))
		{
		    if (!can_see(tmp_vict, ch))
	do_yell(tmp_vict,"Help! Someone is attacking me!");
		    else
		    {
			xprintf(buf,"Die, %s, you sorcerous dog!{x",
			(is_affected(ch, gsn_doppelganger)
			&& !IS_IMMORTAL(tmp_vict)) ? ch->doppel->name :
			ch->name);
	do_yell(tmp_vict,buf);
		    }
		}
		if (tmp_vict != ch)
		{
		    spell_blindness(gsn_blindness, ch->level, ch,
			tmp_vict, TARGET_CHAR);
		    multi_hit(tmp_vict, ch, TYPE_UNDEFINED);
		}
	    }
	ch->mana -= 20;
	check_improve(ch, gsn_blindness_dust, TRUE, 1);
	return;
    }
}

void do_barkskin(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;

    if ( is_affected( ch, gsn_barkskin ) )
    {
	  send_to_char("{cYour skin is already covered in bark.{x\n\r",ch);
	return;
    }

 if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_barkskin].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (number_percent() > ch->pcdata->learned[gsn_barkskin])
    {
      send_to_char("{cYou try to cover in bark, but fail.{x\n\r", ch);
      act("{c$n takes some leaves from the floor and puts them on his head.{x", ch, NULL, NULL, TO_ROOM);
      return;
    }

  ch->mana -= 40;


    af.where	 = TO_AFFECTS;
    af.type      = gsn_barkskin;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.location  = APPLY_AC;
    af.modifier  = -(ch->level * 1.5);
    af.bitvector = 0;
    affect_to_char( ch, &af );
    act( "{c$n's skin becomes covered in bark.{x", ch, NULL, NULL, TO_ROOM );
    send_to_char( "{cYour skin becomes covered in bark.{x\n\r", ch );
    return;
}

void do_phase( CHAR_DATA *ch, char *argument )
{
     AFFECT_DATA af;
  
   
  
    if (ch->race != race_lookup("darnok"))
   	 {
	    send_to_char("{cYou don't know how to phase.{x\n\r",ch);
	    return;
	 }
	 
        
    if (is_affected(ch,gsn_phase))
    {
    	send_to_char("You can't assimilate enough energy to phase just yet.{x\n\r",ch);
	return;
    }
    
      send_to_char("{cYou start switching between the prime and the etherplane.{x\n\r", ch);
      act("{c$n becomes somewhat blurry and partially translucent.{x", ch, NULL, NULL, TO_ROOM);
  
       	af.where     = TO_AFFECTS2;
    	af.type      = gsn_phase;
    	af.level     = ch->level;
    	af.duration  = 10;
    	af.location  = APPLY_NONE;
    	af.modifier  = 0;
    	af.bitvector = AFF2_PHASING;
    	affect_to_char( ch, &af );
   
	af.where     = TO_AFFECTS;
    	af.type      = gsn_phase;
    	af.level     = ch->level;
    	af.duration  = 40;
    	af.location  = APPLY_NONE;
    	af.modifier  = 0;
    	af.bitvector = 0;
    	affect_to_char( ch, &af );
    
    WAIT_STATE(ch, skill_table[gsn_phase].beats);
  
    return;
}
void do_warcry(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA af;

  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_warcry].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (is_affected(ch,gsn_bless) || is_affected(ch, gsn_warcry))
    {
      send_to_char("{cYou are blessed though.{x\n\r",ch);
      return;
    }

  if (ch->mana < 30)
    {
      send_to_char("{cYou can not concentrate enough right now.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_warcry].beats );

  if (number_percent() > ch->pcdata->learned[gsn_warcry])
    {
      send_to_char("{cYou grunt softly.{x\n\r", ch);
      act("{c$n makes some soft grunting noises.{x", ch, NULL, NULL, TO_ROOM);
      return;
    }

  ch->mana -= 30;

  af.where	= TO_AFFECTS;
  af.type      = gsn_warcry;
  af.level	 = ch->level;
  af.duration  = 6+ch->level;
  af.location  = APPLY_HITROLL;
  af.modifier  = ch->level / 8;
  af.bitvector = 0;
  affect_to_char( ch, &af );

  af.location  = APPLY_SAVING_SPELL;
  af.modifier  = 0 - ch->level / 8;
  affect_to_char( ch, &af );
  send_to_char( "{cYou feel righteous as you yell out your warcry.{x\n\r", ch );
  return;
}


void do_resistance(CHAR_DATA *ch, char *argument)
{
  if (!kingdom_ok(ch,gsn_resistance))
    return;
  if (ch->level < skill_table[gsn_resistance].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }
  if (is_affected(ch,gsn_resistance))
    {
      send_to_char("{cYou are as resistant as you will get.{x\n\r",ch);
      return;
    }
/*    
    if ( IS_AFFECTED(ch, AFF_SANCTUARY) )
     {
      send_to_char("You are already in sanctuary or resistance you can't
be both.\n\r",ch);
      return;
     }    
*/
  if (ch->mana < 50)
    {
      send_to_char("{cYou cannot muster up the energy.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_resistance].beats );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_resistance])
    {
      AFFECT_DATA af;

      af.where	= TO_AFFECTS;
      af.type 	= gsn_resistance;
      af.level 	= ch->level;
      af.duration = ch->level / 6;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = 0;

      affect_to_char(ch,&af);
      ch->mana -= 50;

      act("{cYou feel tough!{x",ch,NULL,NULL,TO_CHAR);
      act("{c$n looks tougher.{x",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_resistance,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

     send_to_char("{cYou flex your muscles, but you don't feel tougher.{x\n\r",ch);
      act("{c$n flexes $s muscles, trying to look tough.{x",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_resistance,FALSE,1);
    }

}

void do_stronghold(CHAR_DATA *ch, char *argument)
{
 if (!kingdom_ok(ch, gsn_stronghold))
    return;

  if (IS_NPC(ch) || ch->level < skill_table[gsn_stronghold].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (ch->move < 50)
    {
	send_to_char("You feel exhausted to go back to the stronghold.\n\r",ch);
      return;
    }
}
void do_trophy(CHAR_DATA *ch, char *argument)
{
  int trophy_vnum;
  OBJ_DATA *trophy;
  AFFECT_DATA af;
  OBJ_DATA *part;
  char arg[MAX_INPUT_LENGTH];
  int level;
  one_argument( argument, arg );

  if (!kingdom_ok(ch, gsn_trophy))
    return;

  if (IS_NPC(ch) || ch->level < skill_table[gsn_trophy].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (is_affected(ch, gsn_trophy))
    {
      send_to_char("{cBut you've already got one trophy!{x\n\r", ch);
      return;
    }

  if (ch->mana < 30)
    {
      send_to_char("{cYou feel too weak to concentrate on a trophy.{x\n\r", ch);
      return;
    }

  if ( arg[0] == '\0' )
    {
      send_to_char( "{cMake a trophy of what?{x\n\r", ch );
      return;
    }

  if ( ( part = get_obj_carry( ch, arg,NULL ) ) == NULL )
    {
      send_to_char( "{cYou do not have that body part.{x\n\r", ch );
      return;
    }

    if ( number_percent( ) < (get_skill( ch, gsn_trophy )/3)*2 )
    {
      send_to_char("{cYou failed and destroyed it.{x\n\r", ch);
      extract_obj(part);
      return;
    }

  WAIT_STATE(ch, skill_table[gsn_trophy].beats);

  if (part->pIndexData->vnum == OBJ_VNUM_SLICED_ARM)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_SLICED_LEG)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_SEVERED_HEAD)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_TORN_HEART)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_GUTS)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_BRAINS)
    {
      send_to_char("{cWhy don't you just eat those instead?{x\n\r", ch);
      return;
    }
  else
    {
      send_to_char("{cYou can't make a trophy out of that!{x\n\r", ch);
      return;
    }

  if (part->from[0] == '\0')
    {
      send_to_char("{cInvalid body part.{x\n\r", ch);
      return;
    }

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_trophy])
    {
      af.where  = TO_AFFECTS;
      af.type	= gsn_trophy;
      af.level	= ch->level;
      af.duration	= ch->level;
      af.modifier	= 0;
      af.bitvector 	= 0;

      af.location	= 0;
      affect_to_char(ch,&af);

      if ( trophy_vnum != 0 )
	{
	  char buf[MAX_STRING_LENGTH];
	  level = UMIN(part->level + 5, 60);

	  trophy = create_object( get_obj_index( trophy_vnum ), level );
	  trophy->timer = ch->level * 2;

	  xprintf( buf, trophy->short_descr, part->from );
	  free_string( trophy->short_descr );
	  trophy->short_descr = str_dup( buf );

	  xprintf( buf, trophy->description, part->from );
	  free_string( trophy->description );
	  trophy->description = str_dup( buf );
	  trophy->cost  = 0;
	  ch->mana -= 30;
	  af.where	= TO_OBJECT;
	  af.type 	= -1;
	  af.level	= level;
	  af.duration	= -1;
	  af.location	= APPLY_DAMROLL;
	  af.modifier  = ch->level / 5;
	  af.bitvector	= 0;
	  affect_to_obj( trophy, &af );
	  af.location	= APPLY_HITROLL;
	  af.modifier  = ch->level / 5;
	  af.bitvector	= 0;
	  affect_to_obj( trophy, &af );
/*	  af.location	= APPLY_HIT;
	  affect_to_obj( trophy, &af );*/
	  af.location	= APPLY_INT;
	  af.modifier	= level>20?-2:-1;
	  affect_to_obj( trophy, &af );
	  af.location	= APPLY_STR;
	  af.modifier	= level>20?2:1;
	  affect_to_obj( trophy, &af );

/*	  affect_to_obj( trophy, &af ); */
	  trophy->value[0] = ch->level;
	  trophy->value[1] = ch->level;
	  trophy->value[2] = ch->level;
	  trophy->value[3] = ch->level;


	  obj_to_char(trophy, ch);
          check_improve(ch, gsn_trophy, TRUE, 1);

	  act("{cYou make a poncho from $p!{x",ch,part,NULL,TO_CHAR);
	  act("{c$n makes a poncho from $p!{x",ch,part,NULL,TO_ROOM);

	  extract_obj(part);
	  return;
	}
    }
    else
    {
      send_to_char("{cYou destroyed it.{x\n\r", ch);
      extract_obj(part);
      ch->mana -= 15;
      check_improve(ch, gsn_trophy, FALSE, 1);
    }
}


void do_truesight(CHAR_DATA *ch, char *argument)
{
  if (!kingdom_ok(ch,gsn_truesight))
    return;

  if (IS_NPC(ch) ||
        ch->level < skill_table[gsn_truesight].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (is_affected(ch,gsn_truesight))
    {
      send_to_char("{cYour eyes are as sharp as they will get.{x\n\r",ch);
      return;
    }

  if (ch->mana < 50)
    {
      send_to_char("{cYou cannot seem to focus enough.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_truesight].beats );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_truesight])
    {
      AFFECT_DATA af;

      af.where  = TO_DETECTS;
      af.type 	= gsn_truesight;
      af.level 	= ch->level;
      af.duration = ch->level;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = DETECT_HIDDEN;
      affect_to_char(ch, &af);

      af.bitvector = DETECT_INVIS;
      affect_to_char(ch, &af);

      af.bitvector = DETECT_MAGIC;
      affect_to_char(ch,&af);

      ch->mana -= 50;

      act("{cYou look around sharply!{x",ch,NULL,NULL,TO_CHAR);
      act("{c$n looks more enlightened.{x",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_truesight,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

     send_to_char("{cYou look about sharply, but you don't see anything new.{x\n\r"
		  ,ch);
      act("{c$n looks around sharply but doesn't seem enlightened.{x",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_truesight,FALSE,1);
    }

}

void do_vanish(CHAR_DATA *ch, char *argument)
{
  ROOM_INDEX_DATA *pRoomIndex;

  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_vanish].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (ch->mana < 1)
    {
      send_to_char("{cYou lack the mental power for now.{x\n\r",ch);
      return;
    }

  if (is_affected(ch, gsn_charm_person))
    {
      send_to_char("{cYour master wouldn't like that.{x\n\r",ch);
      return;
    }
  if (number_percent() > ch->pcdata->learned[gsn_vanish])
    {
      ch->mana -= 1;
      send_to_char("{cYou throw down a small globe, which produces some smoke.{x\n\r", ch);
      act("{c$n throws a small globe, resulting only some smoke.{x", ch, NULL, NULL, TO_ROOM);
      return;
    }
  if ( ch->in_room == NULL
      ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
    {
      send_to_char( "{cYou failed.{x\n\r", ch );
      return;
    }

  for ( ; ; )
    {
      pRoomIndex = get_room_index( number_range( 0, 65535 ) );
      if ( pRoomIndex != NULL )
	if ( can_see_room(ch,pRoomIndex) && !room_is_private(pRoomIndex)
	    && ch->in_room->area == pRoomIndex->area)

	  break;
    }
  
 ch->mana -= 1; 
  act( "{c$n throws down a small globe.{x", ch, NULL, NULL, TO_ROOM );
  send_to_char("{cYou throw down a small globe, and vanish in a puff of smoke.{x\n\r",ch);

  if (!IS_NPC(ch) && ch->fighting != NULL && number_bits(1) == 1) {
    send_to_char("{cYou failed.{x\n\r",ch);
    return;
  }

  act( "{c$n is gone!{x",ch,NULL,NULL,TO_ROOM);

  char_from_room( ch );
  char_to_room( ch, pRoomIndex );
  act( "{c$n appears from nowhere.{x", ch, NULL, NULL, TO_ROOM );
	do_look(ch,"auto");
  stop_fighting(ch,TRUE);
  WAIT_STATE( ch, skill_table[gsn_vanish].beats );
  return;
}

void do_distract( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;
    
   if (ch->race != race_lookup("felar"))
    {
    	send_to_char("You don't know how to distract people.\n\r", ch);
        return;
    }
    
   if (MOUNTED(ch))
    {
        send_to_char("You can't hide while mounted.\n\r", ch);
        return;
    }

    if (RIDDEN(ch))
    {
        send_to_char("You can't hide while being ridden.\n\r", ch);
        return;
    }

    
    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE )  )  {
      send_to_char( "You can not hide while glowing.\n\r", ch );
      return;
    }
    
    if ( ch->position != POS_FIGHTING)
    {
      send_to_char( "You need someone to distract.\n\r", ch );
      return;
    }
       
      for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( (gch->fighting == ch) && ( CAN_DETECT(gch, DETECT_HIDDEN) ) &&
	(can_see(gch, ch))) {
	send_to_char( "That guy's definitely to clever to be distracted.\n\r", ch );
        return;
       }
    }
    
    act( "{c$n throws down a small globe.{x", ch, NULL, NULL, TO_ROOM );
    act( "{c$n is gone!{x", ch, NULL, NULL, TO_ROOM );
    SET_BIT(ch->affected_by, AFF_HIDE);  
    send_to_char("{cYou throw down a small globe, and step into the shadows.{x\n\r",ch);
    stop_fighting( ch, TRUE );
    WAIT_STATE(ch, PULSE_VIOLENCE);
 
    return;
}

void do_feign_death( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;
    int chance;
    OBJ_DATA *corpse;
    char buf[MAX_STRING_LENGTH];
    
   if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_feign_death].skill_level[ch->class] )
    {
	send_to_char(
	    "{cHuh?!{x\n\r", ch );
	return;
    }


    if ( ch->position != POS_FIGHTING )
    {
	send_to_char( "{cYou are not fighting.{x\n\r", ch );
	return;
    }

   if (ch->hit > ch->max_hit/8) 
   {
	send_to_char( "{cYou're way to healthy to make that believable{x\n\r", ch );
	return;
    }
   if (MOUNTED(ch))
    {
        send_to_char("You can't hide while mounted.\n\r", ch);
        return;
    }

    if (RIDDEN(ch))
    {
        send_to_char("You can't hide while being ridden.\n\r", ch);
        return;
    }

    
    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE )  )
    {
      send_to_char( "Faerie fire would reveal your illusion.\n\r", ch );
      return;
    }
    
     WAIT_STATE( ch, skill_table[gsn_feign_death].beats );
     send_to_char("{cYou use you magic to falsify you death.{x\n\r",ch);
     for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( (gch->fighting == ch))
	{
	chance = ch->pcdata->learned[gsn_feign_death];
    	chance -= (((gch->level+20)-ch->level)*3);
    	if (number_percent( ) < chance)
    	{
    	SET_BIT(ch->affected_by,AFF_FADE);
    	check_improve(ch,gsn_feign_death,TRUE,1);
    	corpse = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC),0);
    	corpse->timer   = number_range( 200, 300 );
    	xprintf( buf, corpse->short_descr, ch->name );
    	free_string( corpse->short_descr );
    	corpse->short_descr = str_dup( buf );
    	xprintf( buf, corpse->description, ch->name );
    	free_string( corpse->description );
    	corpse->description = str_dup( buf );
    	obj_to_room(corpse,ch->in_room);
    	act( "{r$n is DEAD!.{x", ch, NULL, NULL, TO_ROOM );
    	send_to_char( "{CThe spirit of another enemy haunts you, waiting for your death.\n\r{x", gch);
       	act( "{cYou hear $n's death cry.{x", ch, NULL, NULL, TO_ROOM );
       	stop_fighting( ch, TRUE );
       	ch->position = POS_SLEEPING;
	}
	else
	{
	act( "{c$n attempts to fake his own death.{x", ch, NULL, NULL, TO_ROOM );
	check_improve(ch,gsn_feign_death,FALSE,1);
        stop_fighting( ch, TRUE );
        ch->position = POS_SLEEPING;
	}
      }
   } 
    return;
}

void do_paralysis( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA af;
    int chance;
    
    one_argument(argument,arg);
    
    if (ch->race != race_lookup("illithid"))
    {
    	send_to_char("You don't wield such mental powers.\n\r", ch);
        return;
    }
    
    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }
    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You wouldn't want to alter your own mind.{x\n\r",ch);
	return;
    }
    chance = 20;
    chance += (get_curr_stat(ch,STAT_INT) * 5) - (get_curr_stat(victim,STAT_INT) * 5);
    chance += (ch->level * 2) - (victim->level * 2);
    
        
 
    if ( is_affected( victim, gsn_paralysis ) )
    {
        act("$N is already paralysed.",ch,NULL,victim,TO_CHAR);
        return;
    }
    
    if (is_affected(ch, gsn_paralysis))
    {
      send_to_char("{cYou need time to recharge your psychic powers.{x\n\r", ch);
      return;
    }
    WAIT_STATE(ch, PULSE_VIOLENCE*1.5);
    
    if (number_percent( ) > chance)
    {
    act( "$n looks puzzled for a second", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You resist an attempt to enter your mind.\n\r", victim );
    return;
    }
    
    af.type      = gsn_paralysis;
    af.level     = ch->level;
    af.duration  = 0;
    af.where     = TO_AFFECTS;
    af.bitvector = AFF_STUN;
    affect_to_char( victim, &af );
    
    af.type      = gsn_paralysis;
    af.level     = ch->level;
    af.duration  = 0;
    af.where     = TO_AFFECTS2;
    af.bitvector = AFF2_PARALYZE;    
    affect_to_char( victim, &af );
    
    
      af.type	= gsn_paralysis;
      af.level	= ch->level;
      af.duration	= 20;
      af.modifier	= 0;
      af.bitvector 	= 0;
      af.location	= 0;
      affect_to_char(ch,&af);    
    
    act( "$n has been immobilized.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You have been paralyzed.\n\r", victim );
    return;
}

void do_stoneform( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    
   if (ch->race != race_lookup("gargoyle"))
    {
    	send_to_char("You can't mend into stone.\n\r", ch);
        return;
    }
    
    if (is_affected(ch,gsn_stoneform))
    {
      send_to_char("{cYou transform back into your normal form.{x\n\r",ch);
      act( "{cThe statue slowly tranforms into $n.{x", ch, NULL, NULL, TO_ROOM );
      WAIT_STATE(ch, PULSE_VIOLENCE * 1.5);
      affect_strip(ch,gsn_stoneform);
      return;
    }
    
    if ((ch->pking > 5) && !IS_IMMORTAL(ch)) 
	{
	send_to_char("You are way to pumped up to conduct the transformation!!\n\r",ch);
	return;
	}

/*
    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
      {
        send_to_char("{CYou are way to pumped up to conduct the transformation!!{x\n\r",ch);
        return;
      }
 */     
   if (MOUNTED(ch))
    {
        send_to_char("Transforming to stone on a mount would cause you to lose its balance.\n\r", ch);
        return;
    }

    if (RIDDEN(ch))
    {
        send_to_char("You can't do that while being ridden.\n\r", ch);
        return;
    }

    
    
    act( "{c$n's skin slowly strengthens and recieves a more rocklike nature.{x", ch, NULL, NULL, TO_ROOM );
    send_to_char("{cSlowly your skin turns as hard as rock.{x\n\r",ch);
    af.where	 = TO_AFFECTS2;
    af.type      = gsn_stoneform;
    af.level	 = ch->level;
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF2_STONESHAPE;
    affect_to_char( ch, &af );
    WAIT_STATE(ch, PULSE_VIOLENCE * 2);
 
    return;
}

void do_swoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int dam, chance;
   
    one_argument(argument,arg);

    if (ch->race != race_lookup("arial"))
   	 {
	    send_to_char("{cYou wouldn't know how.{x\n\r",ch);
	    return;
	 }
	 
    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }
    else if ((victim = get_char_room( ch, NULL,arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You can't attack yourself.{x\n\r",ch);
	return;
    }
    
            	
    if (is_safe(ch,victim))
	return;

    
    if ( ch->fighting == victim )
    {
	send_to_char( "{cYou're to busy to swoop anyone.{x\n\r", ch );
	return;
    }
    
    if (IS_AFFECTED(victim,AFF_FLYING))
    {
    	send_to_char( "{cYou cannot swoopattack someone who is flying.{x\n\r", ch );
    	return;
    }

    chance = 75;
    chance += (get_curr_stat(ch,STAT_DEX) * 3) - (get_curr_stat(victim,STAT_DEX) * 3);
    chance += (ch->level * 2) - (victim->level * 2);
    if (IS_AFFECTED(victim,AFF_HASTE) && !(IS_AFFECTED(ch,AFF_HASTE))){
    chance -= 45;
    }
    
    if (IS_AFFECTED(ch,AFF_HASTE) && !(IS_AFFECTED(victim,AFF_HASTE))){
    chance += 65;
    }
    if (number_percent( ) > chance){
    	act( "$n avoids $N's swooping attack.", victim, NULL, ch, TO_NOTVICT );
    	act( "$n swoops down towards you, but you manage to step aside before you were hit", ch, NULL, victim, TO_VICT );
    	send_to_char( "You steer directly towards your target but miscalculate his position.\n\r", ch );
    	switch(number_range(1,5)) {
     case 1:
       	 af.where     = TO_AFFECTS;
    	 af.type      = gsn_sleep;
       	 af.level     = ch->level;
    	 af.duration  = 2;
    	 af.location  = APPLY_NONE;
    	 af.modifier  = 0;
    	 af.bitvector = AFF_SLEEP;
    	 affect_to_char( ch, &af );
         ch->position = POS_SLEEPING;
         send_to_char( "You fail to slow down in time and crashes to the ground.\n\r", ch );
         break;
     case 2:
     send_to_char( "You abort your failed swoop and head back to the sky.\n\r", ch );
     act( "$n aborts his dive and turns back to the sky.", ch, NULL, victim, TO_NOTVICT );
     act( "$n aborts his dive and turns back to the sky before you manage to attack.", ch, NULL, victim, TO_VICT );
     break;
     case 3:
     send_to_char( "You abort your failed swoop and head back to the sky.\n\r", ch );
     act( "$n aborts his dive and turns back to the sky.", ch, NULL, victim, TO_NOTVICT );
     act( "$n aborts his dive and turns back to the sky before you manage to attack.", ch, NULL, victim, TO_VICT );
     break;
     case 4:
     send_to_char( "You abort your failed swoop and head back to the sky.\n\r", ch );
     act( "$n aborts his dive and turns back to the sky.", ch, NULL, victim, TO_NOTVICT );
     act( "$n aborts his dive and turns back to the sky before you manage to attack.", ch, NULL, victim, TO_VICT );
     break;
     case 5:
     send_to_char( "You abort your failed swoop and head back to the sky.\n\r", ch );
     act( "$n aborts his dive and turns back to the sky.", ch, NULL, victim, TO_NOTVICT );
     act( "$n aborts his dive and turns back to the sky before you manage to attack.", ch, NULL, victim, TO_VICT );
     break;
       }
     return;
    }
    
    dam = 30;//GET_DAMROLL(ch);
    dam += number_range(ch->level * 5, ch->level * 15);
    dam += number_range (GET_DAMROLL(ch) /5, GET_DAMROLL(ch));
     
     send_to_char( "Your swooping assault strikes your target. \n\r", ch );
     act( "$n hits $N in a swooping attack.", ch, NULL, victim, TO_NOTVICT );
     act( "$n surprises you with a swooping attack from the air.", ch, NULL, victim, TO_VICT );
    
    if ( number_percent() < 25 )
       {
       	af.where     = TO_AFFECTS;
    	af.type      = gsn_blindness;
    	af.level     = ch->level;
    	af.duration  = ch->level / 2;
    	af.location  = APPLY_NONE;
    	af.modifier  = 0;
    	af.bitvector = AFF_BLIND;
    	affect_to_char( victim, &af );
        dam = dam * 2;
        send_to_char( "Your beak pierces out your victims eye and causes severe damage.\n\r", ch );
        act( "$n's beak penetrates $N's eyelids and blinds him.", ch, NULL, victim, TO_NOTVICT );
        act( "OUCH! You wince in pain as you are blindfolded by $n's beak.", ch, NULL, victim, TO_VICT );
       }
   
    
    
    damage( ch, victim, dam, gsn_swoop, DAM_PIERCE, TRUE );
    if (!IS_NPC(victim)){
   	 send_to_char( "You disengage and return to the sky.\n\r", ch );
   	 act( "$n disengages and returns to the sky", ch, NULL, NULL, TO_NOTVICT );
      	 stop_fighting( ch, TRUE );
        }
    WAIT_STATE(ch, skill_table[gsn_swoop].beats);
  
    return;
}

void do_totem (CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA af;
  OBJ_DATA *totem;

  if (ch->race != race_lookup("ogre"))
   	 {
	    send_to_char("{cYou wouldn't know how.{x\n\r",ch);
	    return;
	 }
	 
  if (is_affected(ch,gsn_totem))
    {
    	send_to_char("You can't built another totem, yet.{x\n\r",ch);
	return;
    }
	 
  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, gsn_totem ))
    {
     send_to_char("A totem has already been planted in this room.\n\r",ch);
     return;
  }
  

    af.where     = TO_ROOM_AFFECTS;
    af.type      = gsn_totem;
    af.level     = ch->level;
    af.duration  = 2 + ch->level / 5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_REGENERATION;
    affect_to_room( ch->in_room, &af );
    
     af.where     = TO_AFFECTS;
     af.type      = gsn_totem;
     af.level     = ch->level;
     af.duration  = 15;
     af.location  = APPLY_NONE;
     af.modifier  = 0;
     af.bitvector = 0;
     affect_to_char( ch, &af );
     
    totem = create_object(get_obj_index(OBJ_VNUM_TOTEM),0);
    totem->timer = 2 + ch->level / 5; 
    obj_to_room(totem,ch->in_room);
    send_to_char("You raise a tall terrifying totempole \n\r",ch);
    act("The room gets an uncomfortable aura as $n raises a unpleasant totem.\n\r",ch,NULL,NULL,TO_ROOM);

}

void do_breath( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch, *vch_next;
    AFFECT_DATA af;
    int dam;
    bool found = FALSE;
   
    one_argument(argument,arg);

    if (ch->race != race_lookup("draconian"))
   	 {
	    send_to_char("{cYou wouldn't know how.{x\n\r",ch);
	    return;
	 }
	 
    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }
    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You can't attack yourself.{x\n\r",ch);
	return;
    }
    
    if (is_affected(ch,gsn_dragon_breath))
    {
    	send_to_char("Your throat is still sore.{x\n\r",ch);
	return;
    }
            	
    if (is_safe(ch,victim))
	return;

    
    dam = 10;//GET_DAMROLL(ch);
    dam += number_range(ch->level * 5, ch->level * 15);
        
     act("$n breathes forth a cone of fire.",ch,NULL,victim,TO_NOTVICT);
     act("$n breathes a cone of hot fire over you!",ch,NULL,victim,TO_VICT);
     act("You breath forth a cone of fire.",ch,NULL,NULL,TO_CHAR);
     
     for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch)
	&&   (ch->fighting != vch || vch->fighting != ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;
     }  
      
  
        for ( vch = ch->in_room->people; vch; vch = vch_next )
   {
      vch_next = vch->next_in_room;
         found = TRUE;
       if ( vch != ch && !is_same_group(ch, vch) )  
      {
         act( "{C$n turns towards YOU!{x", ch, NULL, vch, TO_VICT    );
        if (!saves_spell(ch->level,victim,DAM_FIRE))      
       {
       	send_to_char( "Your eyes tear up from smoke...you can't see a thing!\n\r", vch);
       	af.where     = TO_AFFECTS;
    	af.type      = gsn_breath;
    	af.level     = ch->level;
    	af.duration  = 1.5;
    	af.location  = APPLY_NONE;
    	af.modifier  = 0;
    	af.bitvector = AFF_BLIND;
    	affect_to_char( vch, &af );
        damage( ch, vch, dam * 2, gsn_breath, DAM_FIRE, TRUE );
        act( "$n is blinded by smoke.", vch, NULL, NULL, TO_ROOM );
       }
   else
      {
      damage( ch, vch, dam, gsn_breath, DAM_FIRE, TRUE );
      }
    }
 }    
	af.where     = TO_AFFECTS;
    	af.type      = gsn_dragon_breath;
    	af.level     = ch->level;
    	af.duration  = 2;
    	af.location  = APPLY_NONE;
    	af.modifier  = 0;
    	af.bitvector = 0;
    	affect_to_char( ch, &af );
    
    WAIT_STATE(ch, skill_table[gsn_breath].beats);
  
    return;
}

void do_dive ( CHAR_DATA *ch, char * argument)
{
    AFFECT_DATA af;
    	
    if (ch->race != race_lookup("merma"))
    {
	send_to_char("{cYou wouldn't know how.{x\n\r",ch);
	return;
    }
	
    if ((ch->in_room->sector_type != SECT_WATER_SWIM)
     && (ch->in_room->sector_type != SECT_WATER_NOSWIM ))
    {
     	send_to_char("You need to be in water to dive.\n\r", ch);
        return;
    }     	
    
    if (MOUNTED(ch))
    {
        send_to_char("You should dismount before attempting to dive.\n\r", ch);
        return;
    }

    if (RIDDEN(ch))
    {
        send_to_char("You wouldn't wanna take your poor rider down with you.\n\r", ch);
        return;
    }

    
    
    act( "{c$n dives down to the bottom{x", ch, NULL, NULL, TO_ROOM );
    send_to_char("{cYou dive to the bottom.{x\n\r",ch);
    af.where	 = TO_AFFECTS2;
    af.type      = gsn_dive;
    af.level	 = ch->level;
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF2_SUBMERGED;
    affect_to_char( ch, &af );
     
    return;
}

void do_emerge ( CHAR_DATA *ch, char * argument)
{
	if ( IS_AFFECTED2(ch, AFF2_SUBMERGED)) {
	affect_strip(ch,gsn_dive);
	send_to_char("{cYou reemerge at the surface.{x\n\r",ch);
	}
	else
	send_to_char("{cYou are not below water.{x\n\r",ch);
	
	return;	
}


void do_nerve(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  one_argument(argument,arg);
  if (ch->level < skill_table[gsn_nerve].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }
  if (ch->fighting == NULL)
    {
      send_to_char("{cYou aren't fighting anyone.{x\n\r",ch);
      return;
    }

  victim = ch->fighting;

  if (is_safe(ch,victim)) return;

  if (is_affected(ch,gsn_nerve))
    {
      send_to_char("{cYou cannot weaken that character any more.{x\n\r",ch);
      return;
    }
  check_killer(ch,victim);
  WAIT_STATE( ch, skill_table[gsn_nerve].beats );

  if (IS_NPC(ch) ||
      number_percent() < (ch->pcdata->learned[gsn_nerve] + ch->level
                         + get_curr_stat(ch,STAT_DEX))/2)
    {
      AFFECT_DATA af,af2,tohit,todam;
      af.where  = TO_AFFECTS;
      af.type 	= gsn_nerve;
      af.level 	= ch->level;
      af.duration = PULSE_VIOLENCE * 3;
      af.location = APPLY_STR;
      af.modifier = -5;
      af.bitvector = 0;
      affect_to_char(victim,&af);
      
      af2.where  = TO_AFFECTS;
      af2.type 	= gsn_nerve;
      af2.level 	= ch->level;
//      af2.duration = ch->level * PULSE_VIOLENCE/PULSE_TICK;
			      af2.duration = PULSE_VIOLENCE * 3;
      af2.location = APPLY_DEX;
      af2.modifier = -5;
      af2.bitvector = 0;
      affect_to_char(victim,&af2);

      tohit.where     = TO_AFFECTS;
      tohit.type      = gsn_nerve;
      tohit.level     = ch->level;
      tohit.duration  = PULSE_VIOLENCE * 3;
      tohit.location  = APPLY_HITROLL;
      tohit.modifier  = -20;
      tohit.bitvector = 0;
      affect_to_char( victim, &tohit );

      todam.where = TO_AFFECTS;
      todam.type = gsn_nerve;
      todam.level = ch->level;
      todam.duration = PULSE_VIOLENCE * 3;
      todam.location = APPLY_DAMROLL;
      todam.modifier = -20;
      todam.bitvector = 0;
      affect_to_char( victim, &todam);


      act("{cYou weaken $N with your nerve pressure.{x",ch,NULL,victim,TO_CHAR);
      act("{c$n weakens you with $s nerve pressure.{x",ch,NULL,victim,TO_VICT);
      act("{c$n weakens $N with $s nerve pressure.{x",ch,NULL,victim,TO_NOTVICT);
      check_improve(ch,gsn_nerve,TRUE,1);
    }
  else
    {
      send_to_char("{cYou press the wrong points and fail.{x\n\r",ch);
      act("{c$n tries to weaken you with nerve pressure, but fails.{x",
	  ch,NULL,victim,TO_VICT);
      act("{c$n tries to weaken $N with nerve pressure, but fails.{x",
	  ch,NULL,victim,TO_NOTVICT);
      check_improve(ch,gsn_nerve,FALSE,1);
    }

    multi_hit(victim,ch,TYPE_UNDEFINED);

    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position != POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	do_yell(victim,"Help! I'm being attacked by someone!");
	else
	{
	  xprintf( buf, "Help! I'm being attacked by %s!{x",
		  (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		  ch->doppel->name : ch->name );
	do_yell(victim,buf);
	}
      }
  return;
}

void do_endure(CHAR_DATA *ch, char *arg)
{
  AFFECT_DATA af;

  if (IS_NPC(ch))
    {
      send_to_char("{cYou have no endurance whatsoever.{x\n\r",ch);
      return;
    }

  if ( ch->level < skill_table[gsn_endure].skill_level[ch->class] ||
       ch->pcdata->learned[gsn_endure] <= 1 )
    {
      send_to_char("{cYou lack the concentration.{x\n\r",ch);
      return;
    }

  if (is_affected(ch,gsn_endure))
    {
      send_to_char("{cYou cannot endure more concentration.{x\n\r",ch);
      return;
    }


  WAIT_STATE( ch, skill_table[gsn_endure].beats );

  af.where 	= TO_AFFECTS;
  af.type 	= gsn_endure;
  af.level 	= ch->level;
  af.duration = ch->level / 4;
  af.location = APPLY_SAVING_SPELL;
  af.modifier = -1 * (ch->pcdata->learned[gsn_endure] / 10);
  af.bitvector = 0;

  affect_to_char(ch,&af);

  send_to_char("{cYou prepare yourself for magical encounters.{x\n\r",ch);
  act("{c$n concentrates for a moment, then resumes $s position.{x",
      ch,NULL,NULL,TO_ROOM);
  check_improve(ch,gsn_endure,TRUE,1);
}

void do_tame(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument,arg);

  if (arg[0] == '\0')
    {
      send_to_char("{cYou are beyond taming.{x\n\r",ch);
      act("{c$n tries to tame $mself but fails miserably.{x",ch,NULL,NULL,TO_ROOM);
      return;
    }

  if ( (victim = get_char_room( ch, NULL, arg)) == NULL)
    {
      send_to_char("{cThey're not here.{x\n\r",ch);
      return;
    }

  if (IS_NPC(ch))
    {
      send_to_char("{cWhy don't you tame yourself first?{x",ch);
      return;
    }

  if (!IS_NPC(victim))
    {
      act("{c$N is beyond taming.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (!IS_SET(victim->act,ACT_AGGRESSIVE))
    {
      act("{c$N is not usually aggressive.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_tame].beats );

  if (number_percent() < ch->pcdata->learned[gsn_tame] + 15
                         + 4*(ch->level - victim->level))
    {
      REMOVE_BIT(victim->act,ACT_AGGRESSIVE);
      SET_BIT(victim->affected_by,AFF_CALM);
      send_to_char("{cYou calm down.{x\n\r",victim);
      act("{cYou calm $N down.{x",ch,NULL,victim,TO_CHAR);
      act("{c$n calms $N down.{x",ch,NULL,victim,TO_NOTVICT);
      stop_fighting(victim,TRUE);
      check_improve(ch,gsn_tame,TRUE,1);
    }
  else
    {
      send_to_char("{cYou failed.{x\n\r",ch);
      act("{c$n tries to calm down $N but fails.{x",ch,NULL,victim,TO_NOTVICT);
      act("{c$n tries to calm you down but fails.{x",ch,NULL,victim,TO_VICT);
      check_improve(ch,gsn_tame,FALSE,1);
    }
}

void do_assassinate( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int chance;

    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_assassinate].skill_level[ch->class] )
      {
	send_to_char("{cYou don't know how to assassinate.{x\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "{cYou don't want to kill your beloved master.{x\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cAssassinate whom?{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Suicide is against your way.{x\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( IS_IMMORTAL( victim ) && !IS_NPC(victim) )
    {
	send_to_char( "{cYour hands pass through.{x\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't assassinate a fighting person.{x\n\r", ch );
	return;
    }

    if ( (get_eq_char( ch, WEAR_WIELD ) != NULL) ||
	 (get_eq_char( ch, WEAR_HOLD  ) != NULL) )  {
	send_to_char(
	"{cYou need both hands free to assassinate somebody.{x\n\r", ch );
	return;
    }

    if ( (victim->hit < victim->max_hit) &&
	 (can_see(victim, ch)) )

    {
	act( "{c$N is hurt and suspicious ... you can't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
    chance = ch->pcdata->learned[gsn_assassinate]/20;
    if (!IS_AWAKE(victim))
    chance += 10;
    chance += (ch->level - victim->level);
   
    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_assassinate].beats );
	if (victim->level <= ch->level && number_percent( ) < chance) 
	  {
		act("{rYou {R+{D+{R+{rAS{RSA{DSSI{RNA{rTE{R+{D+{R+{r $N!{x",ch,NULL,victim,TO_CHAR);
		act("{r$n {R+{D+{R+{rAS{RSA{DSSI{RNA{rTE{R+{D+{R+{r $N!{x",ch,NULL,victim,TO_NOTVICT);
		act("{r$n {R+{D+{R+{rAS{RSA{DSSI{RNA{rTE{R+{D+{R+{r you!{x",ch,NULL,victim,TO_VICT);
		raw_kill(ch,victim);
	  }
	else
	  {
		check_improve(ch,gsn_assassinate,FALSE,1);
		damage( ch, victim, 0, gsn_assassinate,DAM_NONE, TRUE );
      }

    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING)
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! Someone tried to assassinate me!");
	else
	  {
	    xprintf( buf, "Help! %s tried to assassinate me!{x",
		    (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		    ch->doppel->name : ch->name );
	do_yell(victim,buf);
	  }
      }
    return;
  }


void do_caltraps(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim = ch->fighting;

  if (IS_NPC(ch) || ch->pcdata->learned[gsn_caltraps] < 1)
    {
      send_to_char("{cCaltraps? Is that a dance step?{x\n\r",ch);
      return;
    }

  if (victim == NULL)
    {
      send_to_char("{cYou must be in combat.{x\n\r",ch);
      return;
    }

  if (is_safe(ch,victim))
    return;

  act("{cYou throw a handful of sharp spikes at the feet of $N.{x",
      ch,NULL,victim,TO_CHAR);
  act("{c$n throws a handful of sharp spikes at your feet!{x",
      ch,NULL,victim,TO_VICT);

  WAIT_STATE(ch,skill_table[gsn_caltraps].beats);

  if (!IS_NPC(ch) && number_percent() >= ch->pcdata->learned[gsn_caltraps])
    {
      damage(ch,victim,0,gsn_caltraps,DAM_PIERCE, TRUE);
      check_improve(ch,gsn_caltraps,FALSE,1);
      return;
    }

  damage(ch,victim,ch->level,gsn_caltraps,DAM_PIERCE, TRUE);

  if (!is_affected(victim,gsn_caltraps))
    {
      AFFECT_DATA tohit,todam,todex;

      tohit.where     = TO_AFFECTS;
      tohit.type      = gsn_caltraps;
      tohit.level     = ch->level;
      tohit.duration  = -1; /* update removes it after combat */
      tohit.location  = APPLY_HITROLL;
      tohit.modifier  = -5;
      tohit.bitvector = 0;
      affect_to_char( victim, &tohit );

      todam.where = TO_AFFECTS;
      todam.type = gsn_caltraps;
      todam.level = ch->level;
      todam.duration = -1;
      todam.location = APPLY_DAMROLL;
      todam.modifier = -5;
      todam.bitvector = 0;
      affect_to_char( victim, &todam);

      todex.type = gsn_caltraps;
      todex.level = ch->level;
      todex.duration = -1;
      todex.location = APPLY_DEX;
      todex.modifier = -5;
      todex.bitvector = 0;
      affect_to_char( victim, &todex);

      act("{c$N starts limping.{x",ch,NULL,victim,TO_CHAR);
      act("{cYou start to limp.{x",ch,NULL,victim,TO_VICT);
      check_improve(ch,gsn_caltraps,TRUE,1);
    }
}


void do_throw( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_throw].skill_level[ch->class] )
    {
	send_to_char(
	    "{cA clutz like you couldn't throw down a worm.{x\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }
    
    if (ch->submission > 0)
	{
	send_to_char( "{cYou can't do that in an armlock{x\n\r", ch );
	return;
    	}
    	
    /*if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
    {
	send_to_char( "{cLets see, two weapons and an enemy.. you have 3 arms?{x\n\r", ch );
	return;
    }*/

    if (is_safe(ch,victim))
      return;


    WAIT_STATE( ch, skill_table[gsn_throw].beats );

    if ((is_affected(victim, gsn_protective_shield))&&(number_percent() >=20) )	//mike added a 20% chance of protective shield fail
     {
	act("{cYou fail to reach $s arm.{x", ch, NULL, victim, TO_CHAR);
	act("{c$n fails to throw you.{x", ch, NULL, victim, TO_VICT);
	act("{c$n fails to throw $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }

    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE( ch, skill_table[gsn_throw].beats );
       return;
      }
      
    if(victim->kingdom == KINGDOM_DRAKKON)
     if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE( ch, skill_table[gsn_throw].beats );
       return;
      }
      


    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_throw] - ch->lagattack * 6)
    {
      act("{cYou throw $N to the ground with stunning force.{x",
	  ch,NULL,victim,TO_CHAR);
      act("{c$n throws you to the ground with stunning force.{x",
	  ch,NULL,victim,TO_VICT);
      act("{c$n throws $N to the ground with stunning force.{x",
	  ch,NULL,victim,TO_NOTVICT);
//      WAIT_STATE(victim,2 * PULSE_VIOLENCE);
	victim->position = POS_RESTING;
        if (victim->lagattack < 4)
	  victim->lagattack = 4;
	if (victim->lagflee   < 4)
	 victim->lagflee   = 4;
	if (victim->lagstand   < 4)
	  victim->lagstand  = 4;
	if (victim->lagspell   < 10)
	  victim->lagspell = 10;

      damage( ch, victim,ch->level + get_curr_stat(ch,STAT_STR),
	     gsn_throw,DAM_BASH, TRUE );
      check_improve(ch,gsn_throw,TRUE,1);
    }
    else
    {
	act( "{cYou fail to grab your opponent.{x", ch, NULL, NULL, TO_CHAR);
	act( "{c$N tries to throw you, but fails.{x", victim, NULL, ch, TO_CHAR);
	act( "{c$n tries to grab $N's arm.{x", ch, NULL, victim, TO_NOTVICT);
	check_improve(ch,gsn_throw,FALSE,1);
    }

    return;
}

void do_strangle(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];
    int bonus = 0;
    int temp = 0;
    int temp1 = 0;
    int Xchance = 0;
    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_strangle].skill_level[ch->class] )
    {
	send_to_char("{cYou lack the skill to strangle.{x\n\r",ch);
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )  {
	send_to_char( "{cYou don't want to grab your beloved masters' neck.{x\n\r",ch);
	return;
    }

    if ( (victim = get_char_room( ch, NULL, argument)) == NULL)
      {
	send_to_char("{cYou do not see that person here.{x\n\r",ch);
	return;
      }

    if (ch==victim)
      {
	send_to_char("{cEven you are not that stupid.{x\n\r",ch);
	return;
      }

    /*if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't strangle a fighting person.{x\n\r", ch );
	return;
    }*/


if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].skill_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= Xchance)
        {
        check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_strangle].beats * 3/2);
         send_to_char( "{cSomeone is trying to sneak up on you.{x\n\r", victim );
	 return;
        }
        
     }

    if (is_affected(ch,gsn_strangle))
      return;

    if (is_safe(ch,victim))
      return;

	ch->pking = 5;
	victim->pking = 5;

    victim->last_fight_time = current_time;
    ch->last_fight_time = current_time;

    WAIT_STATE(ch,skill_table[gsn_strangle].beats);

if (IS_AFFECTED(victim,AFF_FLYING) && !(IS_AFFECTED(ch,AFF_FLYING)))
	{
	send_to_char("Strangle someone who is flying... try jumping higher.\n\r",ch);
	return;
	}

/*
    if(ch->kingdom != KINGDOM_KUJIKIRI)
	bonus = -20;
	else
	bonus = 10;
*/


/* Kazmir adds 30/11/99 */	
     bonus += URANGE( 0, (get_curr_stat(ch,STAT_DEX)-20)*2, 10);	
     bonus -= URANGE( 0, (get_curr_stat(victim,STAT_CON)-20)*2, 10);	
/* Kazmir adds 30/11/99 */


    if ( IS_NPC(victim) )
	if ( victim->pIndexData->pShop != NULL )
	   bonus -= 40;
	   
//	   	   bonus += 100;
/* Kazmir out
    if (IS_NPC(ch) ||
	number_percent() < 0.4 * (ch->pcdata->learned[gsn_strangle]+bonus))
*/	
    temp += bonus;
//    xprintf(buf, "A %d\n\r",temp );
//    send_to_char(buf,ch);
    temp += ch->pcdata->learned[gsn_strangle];
//    xprintf(buf, "B %d\n\r",temp );
//    send_to_char(buf,ch);
    temp = temp * 0.5;
//    xprintf(buf, "C %d\n\r",temp );
//    send_to_char(buf,ch);
    temp += 2 * (ch->level - victim->level);
    temp1 = number_range(1,100);
//    xprintf(buf, "D %d\n\r",temp1 );
//    send_to_char(buf,ch);
    
//    if (IS_NPC(ch) || (number_range(1,100) < (0.5 * (ch->pcdata->learned[gsn_strangle]+bonus) ) ) )	
    if (IS_NPC(ch) || (temp1 < temp ))

      {
	act("{cYou grab hold of $N's neck and put $M to sleep.{x",
	    ch,NULL,victim,TO_CHAR);
	act("{c$n grabs hold of your neck and puts you to sleep.{x",
	    ch,NULL,victim,TO_VICT);
	act("{c$n grabs hold of $N's neck and puts $M to sleep.{x",
	    ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_strangle,TRUE,1);

	af.type = gsn_strangle;
        af.where = TO_AFFECTS;
	af.level = ch->level;
/*	af.duration = 1; Kazmir out*/
        af.duration = 2;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_SLEEP;
	affect_join ( victim,&af );

	if (IS_AWAKE(victim))
	  victim->position = POS_SLEEPING;
      }
    else
      {
//	char buf[MAX_STRING_LENGTH];

	damage(ch,victim,0,gsn_strangle,DAM_NONE, TRUE);
	check_improve(ch,gsn_strangle,FALSE,1);
	if (!can_see(victim, ch))
	do_yell(victim, "Help! I'm being strangled by someone!");
	else
	  {
	    xprintf(buf, "Help! I'm being strangled by %s!{x",
		    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim))?
		    ch->doppel->name : ch->name );
	    if (!IS_NPC(victim)) 
	do_yell(victim,buf);
	  }
      }
}

void do_blackjack(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int chance  = 0;
    int Xchance = 0;
    int durachance; /* Kazmir in*/

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_blackjack].skill_level[ch->class] )

    {
	send_to_char("{CHuh?{x\n\r",ch);
	return;
    }

    if ( (victim = get_char_room( ch, NULL, argument)) == NULL)
      {
	send_to_char("{cYou do not see that person here.{x\n\r",ch);
	return;
      }

    if (ch==victim)
      {
	send_to_char("{cYou idiot?! Blackjack your self?!{x\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )  {
	send_to_char( "{cYou don't want to hit your beloved masters' head with a full filled jack.{x\n\r",ch);
	return;
    }
    

    if (IS_AFFECTED(victim,AFF_SLEEP))
    {
      if (!IS_AWAKE(victim))
      {
      act("{c$E is already asleep.{x",ch,NULL,victim,TO_CHAR);
      return;
      }
      else
      {
      act("{c$E is to alert to be blackjacked.{x",ch,NULL,victim,TO_CHAR);
      return;
      }
    }  

	ch->pking = 5;
	victim->pking = 5;

    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't blackjack a fighting person.{x\n\r", ch );
	return;
    }



if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].skill_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= Xchance)
        {
         check_improve(victim,gsn_blackjack,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_blackjack].beats * 3/2);
         send_to_char( "{cSomeone trying to sneak up on you.{x\n\r", victim );
	 return;
        }
        
     }

    if (is_safe(ch,victim))
      return;
    victim->last_fight_time = current_time;
    ch->last_fight_time = current_time;

    WAIT_STATE(ch,skill_table[gsn_blackjack].beats);

/*    chance = 0.4 * ch->pcdata->learned[gsn_blackjack]; */
      chance = 0.7 * ch->pcdata->learned[gsn_blackjack];
    
/*    chance += URANGE( 0, (get_curr_stat(ch,STAT_DEX)-20)*2, 10); Kazmir  out*/
/* Kazmir add 30/11/99*/
 chance += URANGE( 0, (get_curr_stat(ch,STAT_DEX)-20)*2, 10);
 chance -= URANGE( 0, (get_curr_stat(victim,STAT_CON)-20)*2, 10);
/* Kazmir add 30/11/99*/     

    chance += can_see(victim, ch) ? 0 : 5;
/*
  if(ch->kingdom != KINGDOM_KUJIKIRI)
        chance -= 20;
        else
        chance += 10;
*/
if (IS_AFFECTED(victim,AFF_FLYING) && !(IS_AFFECTED(ch,AFF_FLYING)))
	{
	send_to_char("Blackjack someone who is flying... try jump higher.\n\r",ch);
	return;
	}

    if ( IS_NPC(victim) )
	if ( victim->pIndexData->pShop != NULL )
	   chance -= 40;

/* Kazmir add 29/11/99*/
  if (get_curr_stat(ch,STAT_STR) < 24) 
     durachance = 3;
     
  if (get_curr_stat(ch,STAT_STR) < 20)
     durachance = 2;
     
  if (get_curr_stat(ch,STAT_STR) < 15)
     durachance = 1;          
/* Kazmir add 29/11/99*/     



    if (IS_NPC(ch) ||
	number_percent() < chance)
      {
	act("{cYou hit $N's head with a lead filled sack.{x",
	    ch,NULL,victim,TO_CHAR);
	act("{cYou feel a sudden pain erupt through your skull!{x",
	    ch,NULL,victim,TO_VICT);
	act("{c$n whacks $N at the back of $S head with a heavy looking sack! Ouch!",
	    ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_blackjack,TRUE,1);

	af.type = gsn_blackjack;
        af.where = TO_AFFECTS;
	af.level = ch->level;
	af.duration = 2; /* Kazmir out */
/*	af.duration = durachance;*/ /* Kazmir in*/
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_SLEEP;
	affect_join ( victim,&af );

	if (IS_AWAKE(victim))
	  victim->position = POS_SLEEPING;
      }
    else
      {
	char buf[MAX_STRING_LENGTH];

	damage(ch,victim,20,gsn_blackjack,DAM_NONE, TRUE);
	check_improve(ch,gsn_blackjack,FALSE,1);
	if (!IS_NPC(victim))
	 {
	  if (!can_see(victim, ch))
	   do_yell(victim,"Help! I'm being blackjacked by someone!");
	  else
	   {
	    xprintf(buf, "Help! I'm being blackjacked by %s!{x",
	    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim))?
	    ch->doppel->name : ch->name );
	    if (!IS_NPC(victim)) 
	    do_yell(victim,buf);
	   }
	 }  
  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    xprintf(buf, " the Nerveless Nuisance");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

      }
}

void do_gouge(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int chance  = 10;
    int dam;

    if (ch->level < skill_table[gsn_gouge].skill_level[ch->class]
       || IS_NPC( ch ) )
    {
	send_to_char("{CHuh?{x\n\r",ch);
	return;
    }

    if ( (victim = get_char_room( ch, NULL, argument)) == NULL)
      {
	send_to_char("{cYou do not see that person here.{x\n\r",ch);
	return;
      }

    if (ch==victim)
      {
	send_to_char("{cgouging yourself seems a bit to painful?!{x\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )  {
	send_to_char( "{cYou don't want to go against your masters orders.{x\n\r",ch);
	return;
    }
   

    if (is_safe(ch,victim))
      return;
 
    WAIT_STATE(ch,skill_table[gsn_gouge].beats);
    
    chance += ((ch->level - victim->level)*2);
    chance += (get_curr_stat(ch,STAT_DEX) * 3) - (get_curr_stat(victim,STAT_DEX) * 2);
    if ( (get_eq_char( ch, WEAR_WIELD ) != NULL) ||
	 (get_eq_char( ch, WEAR_HOLD  ) != NULL) )
	 chance += 20;
    if ( IS_AFFECTED( ch, AFF_HASTE ))
    	chance += 10;
    
    dam = (get_curr_stat(ch,STAT_STR) * 3);
    dam += number_range(1, ch->level);
    if (number_percent( ) < chance)
    {    
    damage( ch, victim, dam*3, gsn_gouge, DAM_PIERCE, TRUE );    
  
	af.type = gsn_gouge;
        af.where = TO_AFFECTS;
	af.level = ch->level;
	af.duration = -1; 
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_BLIND;
	affect_join ( victim,&af );
	act( "{YYou claw $N's eyes out.{x", ch, NULL, victim, TO_CHAR );
	act( "{Y$N's eyes are clawed out by $n.{x", ch, NULL, victim, TO_NOTVICT );
	act( "{RAARRGGGHH!!, {Y$n ripped your eyes out!!.{x", ch, NULL, victim, TO_VICT );
     }
     else if (number_percent( ) < chance * 2)
     {
     damage( ch, victim, dam, gsn_gouge, DAM_PIERCE, TRUE );
     check_improve(ch,gsn_gouge,TRUE,1);
     }
     else
     {
     	act( "{cYou reach out for $N's eyes, but your hands are easily avoided.{x", ch, NULL, victim, TO_CHAR );
     	check_improve(ch,gsn_gouge,FALSE,1);
     }
     
}

/*  adds */
void do_whirlwind( CHAR_DATA *ch, char *argument )
{ 
   CHAR_DATA *pChar; 
   CHAR_DATA *pChar_next;
   OBJ_DATA *wield;
   bool found = FALSE;

   if (    !IS_NPC( ch )
        && ch->level < skill_table[gsn_whirlwind].skill_level[ch->class] )
   {
      send_to_char( "You do not know how to do that...\n\r", ch );
      return;
   }

   if ( ( wield = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
   {
      send_to_char( "You need to wield a weapon first...\n\r", ch );
      return;
   }

   act( "{g$n holds $p firmly, and swings in a huge arc...{x", ch,wield,NULL, TO_ROOM );
   act( "{gYou hold $p firmly, and swings in a huge arc...{x",  ch,wield,NULL, TO_CHAR );

   pChar_next = NULL;
   for ( pChar = ch->in_room->people; pChar; pChar = pChar_next )
   {
      pChar_next = pChar->next_in_room;
         found = TRUE;
       if (!is_same_group( pChar, ch ))
      {
         act( "{C$n turns towards YOU!{x", ch, NULL, pChar, TO_VICT    );
         one_hit( ch, pChar, gsn_whirlwind, FALSE );          
         check_improve(ch,gsn_whirlwind,TRUE,1);
          if (number_percent( ) < (ch->pcdata->learned[gsn_double_whirlwind]-15))
          {
          one_hit( ch, pChar, gsn_double_whirlwind, FALSE );
          check_improve(ch,gsn_double_whirlwind,TRUE,1);
          }
          else
          check_improve(ch,gsn_double_whirlwind,FALSE,1);
       }
   }

   if ( !found )
   {
      act( "{g$n looks dizzy, and a tiny bit embarassed.{x", ch, NULL,NULL,TO_ROOM );
      act( "{gYou feel dizzy, and a tiny bit embarassed.{x", ch, NULL,NULL,TO_CHAR );
      check_improve(ch,gsn_whirlwind,FALSE,1);
   }

   WAIT_STATE( ch, skill_table[gsn_whirlwind].beats );

   if ( !found && number_percent() < 25 )
   {
      act( "$n loses $s balance and falls into a heap.",  ch, NULL, NULL,TO_ROOM );
      act( "You lose your balance and fall into a heap.", ch, NULL, NULL,TO_CHAR );
      ch->position = POS_STUNNED;
   }

   return;
}


void do_bloodthirst( CHAR_DATA *ch, char *argument)
{
    int chance, hp_percent;

    if (!kingdom_ok(ch,gsn_bloodthirst))
      return;

    if ((chance = get_skill(ch,gsn_bloodthirst)) == 0
	||  IS_NPC(ch) ||
	(!IS_NPC(ch)
	 && ch->level < skill_table[gsn_bloodthirst].skill_level[ch->class]))
    {
	send_to_char("{cYou're not that thirsty.{x\n\r",ch);
	return;
    }

  if (IS_AFFECTED(ch,AFF_BLOODTHIRST))
    {
        send_to_char("Your thirst for blood continues.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
        send_to_char("You're feeling to mellow to be bloodthirsty.\n\r",ch);
        return;
    }

    if (ch->fighting == NULL)
      {
	send_to_char("{cYou need to be fighting.{x\n\r",ch);
	return;
      }

    /* modifiers */

    /* damage -- below 50% of hp helps, above hurts */
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch, PULSE_VIOLENCE);


	send_to_char("{cYou hunger for blood!{x\n\r",ch);
	act("{c$n gets a bloodthirsty look in $s eyes.{x",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_bloodthirst,TRUE,2);

	af.where        = TO_AFFECTS;
        af.type         = gsn_bloodthirst;
        af.level        = ch->level;
        af.duration     = ch->level / 5;
        af.modifier     = UMIN(15,ch->level);
        af.bitvector    = AFF_BLOODTHIRST;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= - UMIN(ch->level - 5,35);
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,3 * PULSE_VIOLENCE);

	send_to_char("{cYou feel bloodthirsty for a moment, but it passes.{x\n\r",
		     ch);
	check_improve(ch,gsn_bloodthirst,FALSE,2);
    }
}


void do_spellbane(CHAR_DATA *ch, char *argument)
{
  if (!kingdom_ok(ch,gsn_spellbane))
    return;
  if (ch->level < skill_table[gsn_spellbane].skill_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }
  if (is_affected(ch,gsn_spellbane))
    {
      send_to_char("{cYou are already deflecting spells.{x\n\r",ch);
      return;
    }

  if (ch->mana < 50)
    {
      send_to_char("{cYou cannot muster up the energy.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_spellbane].beats );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_spellbane])
    {
      AFFECT_DATA af;

      af.where = TO_AFFECTS;
      af.type 	= gsn_spellbane;
      af.level 	= ch->level;
      af.duration = ch->level+5;
      af.location = APPLY_SAVING_SPELL;
      af.modifier = -ch->level/4;
      af.bitvector = 0;

      affect_to_char(ch,&af);
      ch->mana -= 0;

      act("{cYour hatred of magic surrounds you.{x",ch,NULL,NULL,TO_CHAR);
      act("{c$n fills the air with $s hatred of magic.{x",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_spellbane,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

      send_to_char("{cYou get red in the face, but nothing happens.{x\n\r",ch);
      act("{c$n makes some wild and angry gestures, but nothing happens.{x",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_spellbane,FALSE,1);
    }

}


void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "{cRescue whom?{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL,  arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cWhat about fleeing instead?{x\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
	send_to_char( "{cDoesn't need your help!{x\n\r", ch );
	return;
    }

    if ( ch->fighting == victim )
    {
	send_to_char( "{cToo late.{x\n\r", ch );
	return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
	send_to_char( "{cThat person is not fighting right now.{x\n\r", ch );
	return;
    }
    if (IS_NPC(ch) && ch->master != NULL && IS_NPC(victim))
      return;

    if (is_safe(ch, fch))
	return;

    if (ch->master != NULL)
      {
	if (is_safe(ch->master, fch)) return;
      }

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_rescue] )
    {
	send_to_char( "{cYou fail the rescue.{x\n\r", ch );
	check_improve(ch,gsn_rescue,FALSE,1);
	return;
    }

    act( "{cYou rescue $N!{x",  ch, NULL, victim, TO_CHAR    );
    act( "{c$n rescues you!{x", ch, NULL, victim, TO_VICT    );
    act( "{c$n rescues $N!{x",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    check_killer( ch, fch );
    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}



void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_kick].skill_level[ch->class] )
    {
	send_to_char(
	    "{cYou better leave the martial arts to fighters.{x\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_kick] )
    {
	damage( ch,victim,number_range(1,1.5 * ch->level),gsn_kick,DAM_BASH, TRUE
);
	check_improve(ch,gsn_kick,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_kick,DAM_BASH, TRUE );
	check_improve(ch,gsn_kick,FALSE,1);
    }

    return;
}

void do_crush( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    EXIT_DATA *pexit;
    ROOM_INDEX_DATA *to_room;
    AFFECT_DATA af;
    int dam, chance, door;
    
     char * const rev_name [] =
    {
        "the south", "the west", "the north", "the east", "below", "above"
    };
   
    one_argument(argument,arg);
    
    

    if (ch->race != race_lookup("giant"))
   	 {
	    send_to_char("{cYou wouldn't know how.{x\n\r",ch);
	    return;
	 }
	 
    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }
    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You can't attack yourself.{x\n\r",ch);
	return;
    }
    
    if (victim->size == SIZE_HUGE)
    {
    	send_to_char("You can't crush someone as big as yourself.{x\n\r",ch);
	return;
    }
            	
    if (is_safe(ch,victim))
	return;

    
    if ( ch->fighting == victim )
    {
	send_to_char( "{cYou're to busy to crush anyone.{x\n\r", ch );
	return;
    }
    
    if ( victim->hit < (0.5 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "{c$N is hurt and suspicious and not interested in letting you close enough.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
    
    chance = 50;
    chance += (get_curr_stat(ch,STAT_DEX) * 3) - (get_curr_stat(victim,STAT_DEX) * 3);
    chance += (ch->level * 2) - (victim->level * 2);
    if (IS_AFFECTED(victim,AFF_FLYING)) {
    chance -= 20;
    }
    
    if (IS_AFFECTED(victim,AFF_HASTE) && !(IS_AFFECTED(ch,AFF_HASTE))){
    chance -= 30;
    }
    
    if (IS_AFFECTED(ch,AFF_HASTE) && !(IS_AFFECTED(victim,AFF_HASTE))){
    chance += 30;
    }
    if (number_percent( ) > chance){
    	act( "$n avoids $N's crushing fist.", victim, NULL, ch, TO_NOTVICT );
    	act( "$n huge fist flies through the air, but you dodge it just in time", ch, NULL, victim, TO_VICT );
    	send_to_char( "Your aim is slightly off and you miss your target.\n\r", ch );
        return;
    }
    
    dam = 50;//GET_DAMROLL(ch);
    dam += number_range(ch->level * 1, ch->level * 10);
    dam += number_range (GET_DAMROLL(ch) /5, GET_DAMROLL(ch));
    if (victim->size == SIZE_TINY)  dam *= 4;
        else if (victim->size == SIZE_SMALL)  dam *= 3;
        else if (victim->size == SIZE_MEDIUM)  dam *= 2;
      
     act("$n's huge fist collides with $N's chest.",ch,NULL,victim,TO_NOTVICT);  
     send_to_char( "Your fist collides with your victims chest.\n\r", ch );
     if ( number_percent() < 90 )
       	 {
    	  WAIT_STATE(victim, PULSE_VIOLENCE * 2);
          victim->position = POS_RESTING; 
         if (victim->lagattack < 8)
	  victim->lagattack = 8;
	 if (victim->lagflee   < 9)
	  victim->lagflee   = 9;
	 if (victim->lagstand   <9)
	  victim->lagstand  = 9;
	 if (victim->lagspell   < 6)
	  victim->lagspell = 6;   
         send_to_char("A huge fist knocks you to the ground.\n\r",victim);
         damage( ch, victim, dam, gsn_crush, DAM_BASH, TRUE );
         }
         else
         {
         stop_fighting( victim, TRUE );
         door = number_range(0,3);
         stop_fighting( victim, TRUE );
         af.where     = TO_AFFECTS;
         af.type      = gsn_sleep;
         af.level     = ch->level;
         af.duration  = 2;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = AFF_SLEEP;
         affect_join( victim, &af );
         victim->position = POS_SLEEPING;
      act("$n flies through the air as the fist strikes him with tremendous force",victim,NULL,NULL,TO_NOTVICT);
      if  ( ( ( pexit   = ch->in_room->exit[door] ) == NULL)
       ||   ( (to_room = pexit->u1.to_room   ) == NULL ) )
         {
          send_to_char( "You're send reeling through the air as a huge fist hits you.{x\n\r", victim );
          send_to_char( "You lose consciousness as you hit the ground.{x\n\r", victim );
	  act("Slams into the wall and falls to the ground unconscious.",victim,NULL,NULL,TO_NOTVICT);
	  damage( ch, victim, dam*2, gsn_crush, DAM_BASH, FALSE );
	 }
	else
	{
         act( "$n {Wflies{x $T!", victim, NULL, dir_name[door], TO_ROOM );
         act( "You {Wfly{x $T!\n\r", victim, NULL, dir_name[door], TO_CHAR);
         char_from_room( victim );
         char_to_room( victim, pexit->u1.to_room );
         send_to_char( "You're send reeling through the air as a huge fist hits you.{x\n\r", victim );
         send_to_char( "You lose consciousness as you hit the ground.{x\n\r", victim );
         act( "You notice movement $T.",victim, NULL, rev_name[door], TO_ROOM );
         act( "$n {Wflies{x into the room!", victim, NULL, NULL, TO_ROOM );
         damage( ch, victim, dam*2, gsn_crush, DAM_BASH, FALSE );
        }
            stop_fighting( victim, TRUE );
            victim->position = POS_SLEEPING;
           }
    
    WAIT_STATE(ch, skill_table[gsn_crush].beats);
  
    return;
}

void do_circle( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *person;


    if ( IS_NPC(ch)
    ||   ch->level < skill_table[gsn_circle].skill_level[ch->class] )
    {
	send_to_char("{cYou don't know how to circle.{x\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    if ((get_eq_char(ch,WEAR_WIELD) == NULL) ||
	attack_table[get_eq_char(ch,WEAR_WIELD)->value[3]].damage
	   != DAM_PIERCE)
      {
       send_to_char("{cYou must wield a piercing weapon to circle stab.{x\n\r",ch);
       return;
     }
    
    // Removed for safety. DERKEC 6/25/00
    //if (is_safe(ch,victim)) return;

    WAIT_STATE( ch, skill_table[gsn_circle].beats );

    for (person = ch->in_room->people;person != NULL;
	 person = person->next_in_room)
      {
	if (person->fighting == ch)
	  {
	    send_to_char("{cYou can't circle while defending yourself.{x\n\r",ch);
	    return;
	  }
      }

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_circle] )
    {
	one_hit(ch, victim, gsn_circle,FALSE);
	check_improve(ch,gsn_circle,TRUE,3);
    }
    else
    {
	damage( ch, victim, 0, gsn_circle,TYPE_UNDEFINED, TRUE);
	check_improve(ch,gsn_circle,FALSE,3);
    }

    return;
}




void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon;

    hth = 0;

	if (ch->master != NULL && IS_NPC(ch))
	return;


      

    if ((chance = get_skill(ch,gsn_disarm)) == 0)
    {
	send_to_char( "{cYou don't know how to disarm opponents.{x\n\r", ch );
	return;
    }

    if ( get_eq_char( ch, WEAR_WIELD ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM))))
    {
	send_to_char( "{cYou must wield a weapon to disarm.{x\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    {
	send_to_char( "{cYour opponent is not wielding a weapon.{x\n\r", ch );
	return;
    }
  /*if ((obj->pIndexData->limit >= 0) && (IS_NPC (victim)))
    {
	send_to_char("You can't disarm them\n\r",ch);
        return;
    }*/


    // find weapon skills 
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));


    if ( get_eq_char(ch,WEAR_WIELD) == NULL)
	chance = chance * hth/150;
    else
	chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;


    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_STR);


    chance += (ch->level - victim->level) * 2;


    if (number_percent() < chance)
    {
    	WAIT_STATE( ch, skill_table[gsn_disarm].beats );
	disarm( ch, victim );
	check_improve(ch,gsn_disarm,TRUE,1);
    }
    else
    {
	WAIT_STATE(ch,skill_table[gsn_disarm].beats);
	act("{cYou fail to disarm $N.{x",ch,NULL,victim,TO_CHAR);
	act("{c$n tries to disarm you, but fails.{x",ch,NULL,victim,TO_VICT);
	act("{c$n tries to disarm $N, but fails.{x",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_disarm,FALSE,1);
    }
    return;
}

void do_ambush( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_ambush].skill_level[ch->class] )
      {
	send_to_char("{cYou don't know how to ambush.{x\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cAmbush whom?{x\n\r", ch );
	return;
    }


    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cHow can you ambush yourself?{x\n\r", ch );
	return;
    }

	 if ( victim->hit < (0.75 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "{c$N is hurt and suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }


    if (can_see(victim,ch) && IS_AWAKE(victim))
      {
	send_to_char("{cBut they can see you.{x\n\r",ch);
	return;
      }

    if ( victim->hit < (0.9 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "{c$N is hurt and suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{cKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_ambush].beats );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < ch->pcdata->learned[gsn_ambush] )
      {
	check_improve(ch,gsn_ambush,TRUE,1);
	multi_hit(ch,victim,gsn_ambush);
      }
    else
      {
	check_improve(ch,gsn_ambush,FALSE,1);
	damage( ch, victim, 0, gsn_ambush,DAM_NONE, TRUE );
      }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! I've been ambushed by someone!");
	else
	  {
	    xprintf( buf, "Help! I've been ambushed by %s!{x",
		    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim)) ?
		    ch->doppel->name : ch->name );
	do_yell(victim,buf);
  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    xprintf(buf, " the Woodland Assailant");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

	  }
      }
    return;
}


void do_cleave( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_cleave].skill_level[ch->class] )
      {
	send_to_char("{cYou don't know how to cleave.{x\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cCleave whom?{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cHow can you sneak up on yourself?{x\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
    {
	act("{cBut $N is such a good friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
	send_to_char( "{cYou need to wield a weapon to cleave.{x\n\r", ch );
	return;
    }


    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't cleave a fighting person.{x\n\r", ch );
	return;
    }

    if ( (victim->hit < (2 * victim->max_hit)) &&
	 (IS_AWAKE(victim)) )
    {
	act( "{c$N is hurt and suspicious ... you can't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_cleave].beats );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < ch->pcdata->learned[gsn_cleave] )
      {
	check_improve(ch,gsn_cleave,TRUE,1);
	multi_hit(ch,victim,gsn_cleave);
      }
    else
      {
	check_improve(ch,gsn_cleave,FALSE,1);
	damage( ch, victim, 0, gsn_cleave,DAM_NONE, TRUE );
      }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position == POS_FIGHTING)
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! Someone is attacking me!");
	else
	  {
	    xprintf( buf, "Die, %s, you butchering fool!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name );
	do_yell(victim,buf);

  if ((!IS_NPC(ch)) && (number_percent() < 10)){
    char buf[MAX_STRING_LENGTH];
    xprintf(buf, " the Butcher");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

	  }
      }
    return;
}

void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int Xchance = 0;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_backstab].skill_level[ch->class] )
      {
	send_to_char("{cYou don't know how to backstab.{x\n\r",ch);
	return;
      }


    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{c$N is your beloved master.{x",ch,NULL,victim,TO_CHAR);
	return;
    }
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "{cBackstab whom?{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL,  arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }

if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].skill_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= Xchance)
        {
        check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_backstab].beats * 3/2);
         send_to_char( "{cSomeone trying to sneek up on you.{x\n\r", victim );
	 return;
        }
        
     }


    if ( victim == ch )
    {
	send_to_char( "{cHow can you sneak up on yourself?{x\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
	send_to_char( "{cYou need to wield a weapon to backstab.{x\n\r", ch );
	return;
    }

    if (obj->value[0] != WEAPON_DAGGER)
      {
	send_to_char("{cYou need to wield a dagger to backstab.{x\n\r",
		     ch);
	return;
      }

   if (IS_AFFECTED(victim,AFF_FLYING) && !(IS_AFFECTED(ch,AFF_FLYING)) && (victim->position != POS_SLEEPING))    // Modified by Derkec 11/21/99
	{
	send_to_char("Backstabbing someone who is flying... try throwing the dagger.\n\r",ch);
	return;
	}

    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't backstab a fighting person.{x\n\r", ch );
	return;
    }


    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_backstab].beats );

    if ( victim->hit < (0.75 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "{c$N is hurt and suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
/*
    if ( current_time-victim->last_fight_time<300 && (IS_AWAKE(victim) &&
	ch->kingdom != KINGDOM_CRESCENT))
    {
	act( "{c$N is suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
*/
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < ch->pcdata->learned[gsn_backstab] )
    {
	check_improve(ch,gsn_backstab,TRUE,1);
	if (!IS_NPC(ch) &&
	    number_percent( ) < (ch->pcdata->learned[gsn_dual_backstab]))
	  {
	    check_improve(ch,gsn_dual_backstab,TRUE,1);
	    multi_hit(ch,victim,gsn_backstab);
	    one_hit(ch,victim,gsn_dual_backstab,FALSE);
	  }
	else
	  {
	    check_improve(ch,gsn_dual_backstab,FALSE,1);
	    multi_hit( ch, victim, gsn_backstab);
	  }
    }
    else
    {
	check_improve(ch,gsn_backstab,FALSE,1);
	damage( ch, victim, 0, gsn_backstab,DAM_NONE, TRUE );
    }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! I've been backstabbed!");
	else
	  {
  	    xprintf( buf, "Die, %s, you backstabbing scum!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name );
	do_yell(victim,buf);

  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    xprintf(buf, " the Backstabbing Scum");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

          }
      }
    return;
}


void do_trip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_trip)) == 0
    ||   (!IS_NPC(ch)
	  && ch->level < skill_table[gsn_trip].skill_level[ch->class]))
    {
	send_to_char("{cTripping?  What's that?{x\n\r",ch);
	return;
    }


    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{c{cBut you aren't fighting anyone!{x{x\n\r",ch);
	    return;
 	}
    }

    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;
#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
	send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
	return;
    }
#endif
    if (IS_AFFECTED(victim,AFF_FLYING))
    {
	act("{c$S feet aren't on the ground.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
	act("{c$N is already down.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cYou fall flat on your face!{x\n\r",ch);
	WAIT_STATE(ch,2 * skill_table[gsn_trip].beats);
	act("{c$n trips over $s own feet!{x",ch,NULL,NULL,TO_ROOM);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{c$N is your beloved master.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_trip].beats);
       return;
      }

    if(victim->kingdom == KINGDOM_DRAKKON)
     if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_trip].beats);
       return;
      }



    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;
    chance -= ch->lagattack * 6;

    /* now the attack */
    if (number_percent() < chance)
    {
	act("{c$n trips you and you go down!{x",ch,NULL,victim,TO_VICT);
	act("{cYou trip $N and $N goes down!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n trips $N, sending $M to the ground.{x",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_trip,TRUE,1);

//	WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        WAIT_STATE(ch,skill_table[gsn_trip].beats);
	victim->position = POS_RESTING;
        if (victim->lagattack < 2)
	  victim->lagattack = 2;
	if (victim->lagflee   < 9)
	 victim->lagflee   = 9;
	if (victim->lagstand   <3)
	  victim->lagstand  = 3;
	if (victim->lagspell   < 2)
	  victim->lagspell = 2;
	damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip,
	    DAM_BASH, TRUE);
    }
    else
    {
	damage(ch,victim,0,gsn_trip,DAM_BASH, TRUE);
	WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
	check_improve(ch,gsn_trip,FALSE,1);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	do_yell(victim," Help! Someone just tripped me!");
	else
	  {
	    xprintf(buf, "Help! %s just tripped me!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	do_yell(victim,buf);
	  }
      }
}

void do_dirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt)) == 0
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_dirt].skill_level[ch->class]))
    {
	send_to_char("{cYou get your feet dirty.{x\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't in combat!{x\n\r",ch);
	    return;
	}
    }

    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
	act("{c$e's already been blinded.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cVery funny.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;
#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is such a good friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */

    /* dexterity */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_DEX);

    /* speed  */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,OFF_FAST))
	chance -= 25;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* sloppy hack to prevent false zeroes */
    if (chance % 5 == 0)
	chance += 1;

    /* terrain */

    switch(ch->in_room->sector_type)
    {
	case(SECT_INSIDE):		chance -= 20;	break;
	case(SECT_CITY):		chance -= 10;	break;
	case(SECT_FIELD):		chance +=  5;	break;
	case(SECT_FOREST):				break;
	case(SECT_HILLS):				break;
	case(SECT_JUNGLE):				break;
	case(SECT_SWAMP):				break;
	case(SECT_ROCK_MOUNTAIN):			break;
	case(SECT_SNOW_MOUNTAIN):			break;
	case(SECT_MOUNTAIN):		chance -= 10;	break;
	case(SECT_DESERT):		chance += 10;   break;
	case(SECT_CAVERN):		chance += 2;	break;
	default:			chance  =  0;   break;
    }

    if (chance == 0)
    {
	send_to_char("{cThere isn't any dirt to kick.{x\n\r",ch);
	return;
    }

    /* now the attack */
    if (number_percent() < chance)
    {
	AFFECT_DATA af;
	act("{c$n is blinded by the dirt in $s eyes!{x",victim,NULL,NULL,TO_ROOM);
        damage(ch,victim,number_range(2,5),gsn_dirt,DAM_NONE, TRUE);
	send_to_char("{cYou can't see a thing!{x\n\r",victim);
	check_improve(ch,gsn_dirt,TRUE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].beats);

	af.where	= TO_AFFECTS;
	af.type 	= gsn_dirt;
	af.level 	= ch->level;
	af.duration	= 0;
	af.location	= APPLY_HITROLL;
	af.modifier	= -4;
	af.bitvector 	= AFF_BLIND;

	affect_to_char(victim,&af);
    }
    else
    {
	damage(ch,victim,0,gsn_dirt,DAM_NONE, TRUE);
	check_improve(ch,gsn_dirt,FALSE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].beats);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
    	if (!can_see(victim,ch))
	do_yell(victim,"Someone just kicked dirt in my eyes!");
	else
	  {
	    xprintf(buf, "Die, %s!  You dirty fool!{x", (is_affected(ch,
		gsn_doppelganger) && !IS_IMMORTAL(victim)) ? ch->doppel->name
		: ch->name);
	do_yell(victim,buf);
	  }
      }

}

void do_bash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    int Xchance = 0;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( ((chance = get_skill(ch,gsn_bash)) == 0 && (!IS_NPC(ch)))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_bash].skill_level[ch->class]))
    {
	send_to_char("{cBashing? What's that?\n\r{x",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }

    else if ((victim = get_char_room( ch, NULL, arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].skill_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= chance)
        {
         check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
         send_to_char( "{cSomeone trying to sneak up on you.{x\n\r", victim );
	 return;
        }
        
     }
 

    if (victim->position < POS_FIGHTING)
    {
	act("{cYou'll have to let $M get back up first.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cYou try to bash your brains out, but fail.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;
#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif
    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is your friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }
    
    
    if (is_affected(victim, gsn_protective_shield))
     {
	act("{cYour bash seems to slide around $N.{x", ch, NULL, victim, TO_CHAR);
	act("{c$n's bash slides off your protective shield.{x", ch, NULL, victim,
	    TO_VICT);
	act("{c$n's bash seems to slide around $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }
    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
       return;
      }

    if(victim->kingdom == KINGDOM_DRAKKON)
    if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
       return;
      }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
//    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;
    chance -= get_curr_stat(victim,STAT_DEX);
    
    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
//    chance += (ch->level - victim->level) * 2;
    chance += (ch->level - victim->level);
    /* now the attack */

if (!IS_NPC(victim))     
      if(number_percent() < victim->pcdata->learned[gsn_reaction] / 1.2)
	{
	check_improve(victim,gsn_reaction,TRUE,1);
	WAIT_STATE(ch, skill_table[gsn_bash].beats * 3/2);
	act("{cYou reacted and dodge $n's bash!{x",ch,NULL,victim,TO_VICT);
	act("{c$N, reacts and dodges your bash!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n reacts and dodges $N's powerful bash.{x",ch,NULL,victim,TO_NOTVICT);
	return;
	}
	WAIT_STATE(ch,skill_table[gsn_bash].beats);



    if (number_percent() < chance)
    {

	act("{c$n sends you sprawling with a powerful bash!{x",
		ch,NULL,victim,TO_VICT);
	act("{cYou slam into $N, and send $M flying!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n sends $N sprawling with a powerful bash.{x",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_bash,TRUE,1);

	wait = 3;

	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

        WAIT_STATE(victim,skill_table[gsn_bash].beats/2);
	victim->position = POS_RESTING;
	/*setting lags*/
        if (victim->lagattack < 5)
	  victim->lagattack = 5;
	if (victim->lagflee   < 5)
	 victim->lagflee   = 5;
	if (victim->lagstand   <9)
	  victim->lagstand  = 9;
	if (victim->lagspell   < 5)
	  victim->lagspell = 5;

	damage(ch,victim,number_range(10,10 + 2 * ch->size + chance/5),gsn_bash,
	    DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_bash,DAM_BASH, TRUE);
	act("{cYou fall flat on your face!{x",
	    ch,NULL,victim,TO_CHAR);
	act("{c$n falls flat on $s face.{x",
	    ch,NULL,victim,TO_NOTVICT);
	act("{cYou evade $n's bash, causing $m to fall flat on $s face.{x",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_bash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	do_yell(victim,"Help! Someone is bashing me!");
	else
	  {
	    xprintf(buf, "Help! %s is bashing me!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	do_yell(victim,buf);
	  }
      }
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

void do_berserk( CHAR_DATA *ch, char *argument)
{
    int chance, hp_percent;

    if ((chance = get_skill(ch,gsn_berserk)) == 0
    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    ||  (!IS_NPC(ch)
    &&   ch->level < skill_table[gsn_berserk].skill_level[ch->class]))
    {
	send_to_char("{cYou turn red in the face, but nothing happens.{x\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
    ||  is_affected(ch,skill_lookup("frenzy")))
    {
	send_to_char("{cYou get a little madder.{x\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
	send_to_char("{cYou're feeling too mellow to berserk.{x\n\r",ch);
	return;
    }

    if (ch->mana < 50)
    {
	send_to_char("{cYou can't get up enough energy.{x\n\r",ch);
	return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
	chance += 10;

    /* damage -- below 50% of hp helps, above hurts */
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->mana -= 50;
	ch->move /= 2;

	/* heal a little damage */
	ch->hit += ch->level * 2;
	ch->hit = UMIN(ch->hit,ch->max_hit);

	send_to_char("{cYour pulse races as you are consumned by rage!{x\n\r",ch);
	act("{c$n gets a wild look in $s eyes.{x",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_berserk,TRUE,2);

	af.where	= TO_AFFECTS;
	af.type		= gsn_berserk;
	af.level	= ch->level;
	af.duration	= number_fuzzy(ch->level / 8);
	af.modifier	= UMAX(1,ch->level/5);
	af.bitvector 	= AFF_BERSERK;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= UMAX(10,10 * (ch->level/5));
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,3 * PULSE_VIOLENCE);
	ch->mana -= 25;
	ch->move /= 2;

	send_to_char("{cYour pulse speeds up, but nothing happens.{x\n\r",ch);
	check_improve(ch,gsn_berserk,FALSE,2);
    }
}

void do_submission( CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    int chance = 10;
    int timer = 5;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];


    if ( (chance = get_skill(ch,gsn_submission)) == 0
    ||   (IS_NPC(ch))    ||   (!IS_NPC(ch)
	  && ch->level < skill_table[gsn_submission].skill_level[ch->class]))
    {
	send_to_char("{cHuh?{x\n\r",ch);
	return;
    }
    
    if ( ch->submission > 0)
       {
    	send_to_char("{cYour opponent has a firm grip in you.{x\n\r",ch);
    	return;
       }


	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{c{cBut you aren't fighting anyone!{x{x\n\r",ch);
	    return;
 	}

    if (is_safe(ch,victim))
	return;

    
      /* size */
    if (ch->size < victim->size-2)
    {
	act("{cYou are to small to put $N in an efficient armlock.{x",ch,NULL,victim,TO_CHAR);
	return;
    }
    
    if (ch->size > victim->size+2)
    {
	act("{cYou can't seem to get a firm grip around his arm.{x",ch,NULL,victim,TO_CHAR);
	return;
    }
    
    if ( victim->submission > 0)
       {
    	send_to_char("{cYou already have a firm grip around his arm{x\n\r",ch);
    	return;
       }
    

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance -= 20;
    if (chance < 5)
        chance = 5;

     if (IS_NPC(victim) && victim->level > 75)
        chance -= (victim->level - ch->level);

    /* now the attack */
    WAIT_STATE(ch,skill_table[gsn_submission].beats);
    if (number_percent() < chance)
    {
	act("{c$n grabs your arm and attempts to break it!{x",ch,NULL,victim,TO_VICT);
	act("{cYou grab $N's arm in a lock{x",ch,NULL,victim,TO_CHAR);
	act("{c$n grabs $N's arm, forcing him to his knees.{x",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_submission,TRUE,1);	

        if (victim->lagattack < 2)
	  victim->lagattack = 2;
	if (victim->lagstand  < 9)
	  victim->lagstand  = 9;
	if (victim->lagspell   < 9)
	  victim->lagspell = 9;
	  

	timer += (get_curr_stat(ch,STAT_DEX) - get_curr_stat(victim,STAT_DEX));
	timer += ((get_curr_stat(ch,STAT_STR) - get_curr_stat(victim,STAT_STR))*1.5);
	
	if (timer < 1)
	timer = 1;
	  victim->submission = timer;
	  
	af.where	 = TO_AFFECTS2;
    	af.type      = gsn_submission;
    	af.level	 = ch->level;
    	af.duration  = -1;
    	af.location  = APPLY_NONE;
    	af.modifier  = 0;
    	af.bitvector = AFF2_SUBMISSION;
    	affect_to_char( ch, &af );
    }
    else
    {
	check_improve(ch,gsn_submission,FALSE,1);
	act("{c$n attempts to grab your arm, but you move it in time!{x",ch,NULL,victim,TO_VICT);
	act("{cYou reach for $N's but he moves it to fast.{x",ch,NULL,victim,TO_CHAR);	
    }

    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED)
      {
	if (!can_see(victim, ch))
	do_yell(victim," Help! This motherfucker is trying to break my arm!");
	else
	  {
	    xprintf(buf, "Help! is %s attempting to break my arm!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	do_yell(victim,buf);
	  }
      }
}
