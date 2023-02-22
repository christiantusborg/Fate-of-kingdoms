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
/* used to converter of prac and train */
void do_gain(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *trainer;

    if (IS_NPC(ch))
        return;

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

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	do_say(trainer,"You may convert 10 practices into 1 train.");
        do_say(trainer,"You may revert 1 train into 10 practices.");
    do_say(trainer,"Simply type 'gain convert' or 'gain revert'.");
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
    for (lev = 0; lev < LEVEL_HERO; lev++)
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
(skill_table[sn].kingdom == ch->kingdom || skill_table[sn].kingdom ==
KINGDOM_NONE)
        )
      {
        found = TRUE;
        lev = skill_table[sn].skill_level[ch->class];
        if (ch->level < lev)
          sprintf(buf,"{W%-18s  {Cn/a{w      ", skill_table[sn].name);
        else
        {
          mana = UMAX(skill_table[sn].min_mana,
                      100/(2 + ch->level - lev));
          sprintf(buf,"{W%-18s  {C%3d mana{w	",skill_table[sn].name,mana);
        }

        if (spell_list[lev][0] == '\0')
          sprintf(spell_list[lev],"\n\r{WLevel %2d: {w%s",lev,buf);
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

    output = new_buf();

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


      if (skill_table[sn].skill_level[ch->class] <= LEVEL_HERO &&
          skill_table[sn].spell_fun == spell_null &&
(skill_table[sn].kingdom == ch->kingdom || skill_table[sn].kingdom ==
KINGDOM_NONE)
         )
      {
        found = TRUE;
        lev = skill_table[sn].skill_level[ch->class];
        if (ch->level < lev)

          sprintf(buf,"{W%-18s {Cn/a{w      ", skill_table[sn].name);
        else
          sprintf(buf,"{W%-18s {C%3d%%{w      ",skill_table[sn].name,
                                         ch->pcdata->learned[sn]);

        if (skill_list[lev][0] == '\0')
          sprintf(skill_list[lev],"\n\r{WLevel %2d: {w%s",lev,buf);
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

  return (expl * pc_race_table[ch->race].class_mult[ch->class]/100);
}

int exp_to_level(CHAR_DATA *ch, int points)
{
 int base;

  base = base_exp(ch,points);
  return ( base - exp_this_level(ch,ch->level,points) );
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
   return expl * pc_race_table[ch->race].class_mult[ch->class]/100;
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
            sprintf(buf,"You have become better at %s!",
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
            sprintf(buf,
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
        sprintf(buf,"%-18s          ",skill_table[sn].name);
        if (skill_list[lev][0] == '\0')
          sprintf(skill_list[lev],"\n\rLevel %2d: %s",lev,buf);
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

void do_glist( CHAR_DATA *ch , char *argument)
{
 char arg[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];
 int group,count;

 one_argument(argument,arg);

 if (arg[0] == '\0')
  {
   send_to_char("Syntax : glist <group>\n\r",ch);
   return;
  }

 if ((group = group_lookup(arg) ) == -1)
  {
   send_to_char("That is not a valid group.\n\r",ch);
   return;
  }

 sprintf(buf,"Now listing group %s :\n\r",prac_table[group].sh_name);
 send_to_char(buf,ch);
 buf[0] = '\0';
 for(count = 0 ; count < MAX_SKILL; count++)
  {
   if ( (group == GROUP_NONE && !CLEVEL_OK(ch,count) &&
        skill_table[count].group == GROUP_NONE ) ||
        (group != skill_table[count].group) || !KINGDOM_OK(ch,count) )
     continue;
   if ( buf[0] != '\0')
    {
     sprintf(buf , "%-18s%-18s\n\r", buf,skill_table[count].name);
     send_to_char(buf,ch);
     buf[0] = '\0';
    }
   else sprintf(buf, "%-18s",skill_table[count].name);
  }
}

void do_slook( CHAR_DATA *ch, char *argument)
{
     int sn;
     char arg[MAX_INPUT_LENGTH];
     char buf[MAX_STRING_LENGTH];

     one_argument(argument,arg);
     if (arg[0] == '\0')
        {
         send_to_char("Syntax : slook <skill or spell name>.\n\r",ch);
         return;
        }

     if ( (sn = skill_lookup(arg)) == -1 )
     {
         send_to_char("That is not a spell or skill.\n\r",ch);
         return;
        }

     sprintf(buf,"Skill :%s in group %s.\n\r",
        skill_table[sn].name,prac_table[skill_table[sn].group].sh_name);
     send_to_char(buf,ch);

     return;
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
	    sprintf(buffer,"%d %d",obj->value[0],obj->value[1]);
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

    sprintf(buf,"in");
    one_argument( buf, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );


   if (    !IS_NPC( ch )
        && ch->level < skill_table[gsn_bagpeek].skill_level[ch->class] )
   {
      send_to_char( "You do not know how to do that...\n\r", ch );
      return;
   }

    
    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
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

    

	if ( ( obj = get_obj_here( victim, arg3 ) ) == NULL )
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

	    sprintf( buf, "It's %sfilled with  a %s liquid.\n\r",
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
    
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
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
      	

	if ( ( container = get_obj_here( victim, arg3 ) ) == NULL )
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
        sprintf(buf,"%s tries to steal from %s!",ch->name,victim->name);
        wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);

	act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );
	switch(number_range(0,3))
	{
	case 0 :
	   sprintf( buf, "%s is a lousy thief!", ch->name );
	   break;
        case 1 :
	   sprintf( buf, "%s couldn't rob %s way out of a paper bag!",
		    ch->name,(ch->sex == 2) ? "her" : "his");
	   break;
	case 2 :
	    sprintf( buf,"%s tried to rob me!",ch->name );
	    break;
	case 3 :
	    sprintf(buf,"Keep your hands out of there, %s!",ch->name);
	    
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

sprintf(buf,"%s steals from %s [%d] %s!",ch->name,victim->name,obj->pIndexData->vnum,obj->name);
 if (!IS_NPC(victim))
   log_string( buf );
wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
save_char_obj( ch );
return; 
}
