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
#include "merc.h"
#include <stdlib.h>

DECLARE_DO_FUN( do_say );
DECLARE_DO_FUN( do_font );
DECLARE_DO_FUN( do_echo );
void do_qnew         args( ( CHAR_DATA *ch, char *argument ) );
void do_qstart       args( ( CHAR_DATA *ch, char *argument ) );
void do_qminlevel    args( ( CHAR_DATA *ch, char *argument ) );
void do_qmaxlevel    args( ( CHAR_DATA *ch, char *argument ) );
void do_qname        args( ( CHAR_DATA *ch, char *argument ) );
void do_qshort       args( ( CHAR_DATA *ch, char *argument ) );
void do_qlong        args( ( CHAR_DATA *ch, char *argument ) );
void do_qprotection  args( ( CHAR_DATA *ch, char *argument ) );
void do_qend         args( ( CHAR_DATA *ch, char *argument ) );
void do_qlist        args( ( CHAR_DATA *ch, char *argument ) );
void do_qadd         args( ( CHAR_DATA *ch, char *argument ) );
void do_qremove      args( ( CHAR_DATA *ch, char *argument ) );
void do_qapply       args( ( CHAR_DATA *ch, char *argument ) );
void do_qlistp       args( ( CHAR_DATA *ch, char *argument ) );
void do_qquit        args( ( CHAR_DATA *ch, char *argument ) );


ROOM_INDEX_DATA *find_location args( ( CHAR_DATA *ch, char * arg ) );
#define QUEST_OBJ 151
struct reward_type 
{
  char *name;
  char *keyword;
  int cost;
  bool object;
  int value;
  void *      where;
};

struct quest_desc_type {
  char *name;
  char *long_descr;
  char *short_descr;
};

/* Descriptions of quest items go here:
Format is: "keywords", "Short description", "Long description" */
const struct quest_desc_type quest_desc[] =
{
{"quest sceptre", 	"The Sceptre of Courage is lieing here, waiting to be returned to its owner.", "The Sceptre of Courage"},
{"quest crown", 	"The Crown of Wisdom is lieing here, waiting to be returned to its owner.", "The Crown of Wisdom"},
{"quest gauntlet", 	"The Gauntlets of Strength are lieing here, waiting to be returned to its owner.", "The Gauntlets of Strength"},
{NULL, NULL, NULL}
};

/* Local functions */
void generate_quest	args(( CHAR_DATA *ch, CHAR_DATA *questman ));
void quest_update	args(( void ));
bool quest_level_diff   args(( int clevel, int mlevel));
bool chance		args(( int num		));

bool chance(int num)
{
    if (number_range(1,100) <= num) return TRUE;
    else return FALSE;
}

void do_scatter(CHAR_DATA *ch, char *argument)
{ 
  CHAR_DATA *victim;
  OBJ_DATA *obj;
  long mcounter;
  int mob_vnum;
  ROOM_INDEX_DATA *room;
  MOB_INDEX_DATA *vsearch;
  char arg1[MAX_INPUT_LENGTH];
  char buf[MAX_INPUT_LENGTH];
   
  argument = one_argument( argument, arg1 );
  if (arg1[0] == '\0')
    {
  	 send_to_char("scatter what?\n", ch);
   	 return;
    }
   if  ( (obj = get_obj_carry(ch,arg1,NULL) ) == NULL )
   {
      send_to_char("nothing like that in heaven or earth\n", ch);
      return;
   }
  victim = NULL;  
  vsearch = NULL;
  mcounter = 0;
  while((vsearch == NULL)&&(mcounter<2000))
  	{	 
	   mcounter++;
	   mob_vnum = number_range(400, 32000);
	   if ( (vsearch = get_mob_index(mob_vnum) ) != NULL )
	   {
	   	   victim = get_char_world( ch, vsearch->player_name );
		   if (victim!= NULL)
		   {
		      if (!strstr(victim->pIndexData->area->builders, "Unlinked"))
		  	break;            
		   }
		   else
	   		vsearch = NULL;
	   }//if vsearch !=null
		 
	}//while
 if ((vsearch == NULL)||(victim==NULL))
 {
          send_to_char("search failed", ch);
	  return;
 }
 if ((room = get_room_index(victim->in_room->vnum)) != NULL)
 {
	xprintf(buf, "Look in the general area of %s for %s!\r\n VNum:[%d]",room->area->name, room->name,room->vnum);
	send_to_char(buf, ch);
	obj_from_char(obj);
	obj_to_room(obj, room);
 }
	return;
}//do_scatter

/* The main quest function */
void do_job(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *questman;
    OBJ_DATA *obj=NULL, *obj_next;
    OBJ_INDEX_DATA *questinfoobj;
    MOB_INDEX_DATA *questinfo;
    char buf [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    int i;

/* Add your rewards here.  Works as follows:
"Obj name shown when quest list is typed", "keywords for buying",
"Amount of quest points",  Does it load an object?,  IF it loads an
object, then the vnum, otherwise a value to set in the next thing,  This
is last field is a part of the char_data to be modified */ 

const struct reward_type reward_table[]= 
{
/*
  { "A Flask of Endless Water","flask water",   500, TRUE,  46,    0},
  { "Jug O' Moonshine",        "jug moonshine", 300, TRUE,  47,    0},
  { "Potion of Extra Healing", "extra healing", 300, TRUE,  4639,  0},
  { "Potion of Sanctuary",     "sanctuary",     150, TRUE,  3081,  0},
*/
  { "Boots of Speed",  	       "Boots Speed",    5500, TRUE, 145,0},
  { "Egg of Wonders",	      "Egg Wonders",     300, TRUE, 3831, 0},
  { "Armor of the Warrior",   "armor warrior",  4500, TRUE, 130, 0},
  { "A Small Large bag",      "small large bag",1500, TRUE, 102, 0},
  { "Armguards Of the gods",  "armguards gods", 1700, TRUE, 124,0},
  { "Amulet of Patience",     "amulet patience",650, TRUE, 135, 0},
  { "Collar of Demon",	      "collar demon",   1500, TRUE, 121, 0},
  { "Bracer of Magic",	      "bracer magic",   750, TRUE, 136, 0},
  { "Crown of Thorns",	      "crown thorn",	900, TRUE, 137, 0},
  { "Ring of Light",	      "ring light",    500, TRUE, 139, 0},
  { "10,000 Gold Pieces",     "10000 gold",	750, FALSE, 10000,&ch->gold},
  { "4 Practices",    	       "practices", 	500, FALSE, 4, &ch->practice},
 /* { "Mark of your God",		"mark",	1000,TRUE,religion_table[ch->religion].vnum,0},*/
  { NULL, NULL, 0, FALSE, 0, 0  } /* Never remove this!!! */
};


    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);

    if(IS_NPC(ch))
	{ send_to_char("NPC's can't quest.\n\r",ch); return; }

    if (arg1[0] == '\0')
    {
   send_to_char("JOB commands: Info, Time, Request, Complete, Quit, List, and Buy.\n\r",ch);
        send_to_char("For more information, type 'Help Quest'.\n\r",ch);
	return;
    }

    if (!strcmp(arg1, "info"))
    {
	if (IS_SET(ch->act,PLR_QUESTOR))
	{
	    if (ch->questmob == -1 && 
		ch->questgiver->short_descr != NULL) {
	        xprintf(buf,"Your job is ALMOST complete!\n\rGet back to %s before your time runs out!\n\r",ch->questgiver->short_descr);
		send_to_char(buf, ch);
	    }
	    else if (ch->questobj > 0)
	    {
                questinfoobj = get_obj_index(ch->questobj);
		if (questinfoobj != NULL) {
		    xprintf(buf,"You are on a job to recover the fabled %s!\n\r",questinfoobj->name);		
		    send_to_char(buf, ch);
		}
		else send_to_char("You aren't currently on a job.\n\r",ch);
		return;
	    }
	    else if (ch->questmob > 0)
	    {
                questinfo = get_mob_index(ch->questmob);
		if (questinfo != NULL) {
		    xprintf(buf,"You are on a job to slay the dreaded %s!\n\r",questinfo->short_descr);
		    send_to_char(buf, ch);
		}
		else send_to_char("You have no pending job right now.\n\r",ch);
		return;
	    }
	}
	else
	    send_to_char("You have no pending job right now.\n\r",ch);
	return;
    }
    else if (!strcmp(arg1, "time"))
    {
	if (!IS_SET(ch->act,PLR_QUESTOR))
	{
	    send_to_char("You have no pending job right now.\n\r",ch);
	    if (ch->nextquest > 1) {
	        xprintf(buf,"There are %d minutes remaining until you can go on another job.\n\r",ch->nextquest);
		send_to_char(buf, ch);
	    }
	    else if (ch->nextquest == 1) {
		xprintf(buf, "There is less than a minute remaining until you can go on another job.\n\r");
		send_to_char(buf, ch);
	    }
	}
        else if (ch->countdown > 0)
        {
	    xprintf(buf, "Time left for current job: %d\n\r",ch->countdown);
	    
	    send_to_char(buf, ch);
	}
	return;
    }

    for ( questman = ch->in_room->people; questman != NULL; questman = questman->next_in_room )
    {
	if (!IS_NPC(questman)) continue;
        if (questman->spec_fun == spec_lookup( "spec_questmaster" )) break;
    }

    if (questman == NULL || questman->spec_fun != spec_lookup("spec_questmaster" ))
    {
        send_to_char("No one here has a job for you.\n\r",ch);
        return;
    }

    if ( questman->fighting != NULL)
    {
	send_to_char("Wait until the fighting stops.\n\r",ch);
        return;
    }

    ch->questgiver = questman;

    if (!strcmp(arg1, "list"))
    {
        act("$n bargains with $N.", ch, NULL, questman, TO_ROOM); 
	act ("You ask $N what $E has for you",ch, NULL, questman, TO_CHAR);
	send_to_char("Current Items available for Purchase:\n\r", ch);
	if(reward_table[0].name == NULL)
	  send_to_char("  Nothing.\n\r",ch);
	else {
	send_to_char("  [{WCost{w]     [{BName{w]\n\r",ch);
	for(i=0;reward_table[i].name != NULL;i++) {
	      xprintf(buf,"   {W%-4d{w       {b%s{w\n\r"
			,reward_table[i].cost,reward_table[i].name);
	      send_to_char(buf, ch);
	}
	     }
	send_to_char("\n\rTo buy an item, type 'job buy <item>'.\n\r",ch);
	return;
    }

    else if (!strcmp(arg1, "buy"))
    {
	bool found=FALSE;
	if (arg2[0] == '\0')
	{
	    send_to_char("To buy an item, type 'job buy <item>'.\n\r",ch);
	    return;
	}
	/* Use keywords rather than the name listed in qwest list */
	/* Do this to avoid problems with something like 'qwest buy the'*/
	/* And people getting things they don't want... */
      for(i=0;reward_table[i].name != NULL;i++)
	if (is_name(arg2, reward_table[i].keyword))
	{ found = TRUE;
	    if (ch->quest_curr >= reward_table[i].cost)
	    {
		ch->quest_curr -= reward_table[i].cost;
		if(reward_table[i].object) {
		   if(is_name(arg2,"mark") &&
			(ch->religion < 1) && (ch->religion < MAX_RELIGION)) {
			send_to_char("Sorry you can't buy that you don't follow any religion!!\n\r",ch);
			return; }
	            obj = create_object(get_obj_index(reward_table[i].value),ch->level); }
		else
		   {
		   xprintf(buf,"In exchange for %d glory, %s gives you %s.\n\r",
			reward_table[i].cost, questman->short_descr, reward_table[i].name );
		   send_to_char(buf, ch);
		   *(int *)reward_table[i].where += reward_table[i].value;
		   }
		break;
	    }
	    else
	    {
		xprintf(buf, "Sorry, %s, but you don't have enough glory for that.",ch->name);
		do_say(questman,buf);
		return;
	    }
	}
	if(!found)
	{
	    xprintf(buf, "I don't have that item, %s.",ch->name);
	    do_say(questman, buf);
	}
	if (obj != NULL)
	{
	    xprintf(buf,"In exchange for %d glory, %s gives you %s.\n\r",
		    reward_table[i].cost, questman->short_descr, obj->short_descr);
	    send_to_char(buf, ch);
	    obj_to_char(obj, ch);
	    if ( obj->wear_loc == WEAR_TATTOO )
	     equip_char(ch, obj, WEAR_TATTOO);
	    
	}
	return;
    }
    else if (!strcmp(arg1, "request"))
    {
        act("$n asks $N for a quest.", ch, NULL, questman, TO_ROOM); 
	act("You ask $N for a quest.",ch, NULL, questman, TO_CHAR);
	if (IS_SET(ch->act,PLR_QUESTOR))
	{
	    xprintf(buf, "But you're already on a job!");
	    do_say(questman, buf);
	    return;
	}
	if (ch->nextquest > 0)
	{
	    xprintf(buf, "There is nothing you can do for me right now, %s",ch->name);
	    do_say(questman, buf);
	    xprintf(buf, "Come back later.");
	    do_say(questman, buf);
	    return;
	}

	xprintf(buf, "Thank you %s, you are truly wise in politics.",ch->name);
	if (!IS_SET(ch->act,PLR_QUESTOR))
	do_say(questman, buf);
        ch->questmob = 0;
	ch->questobj = 0;
	generate_quest(ch, questman);

        if (ch->questmob > 0 || ch->questobj > 0)
	{
            ch->countdown = number_range(10,30);
	    SET_BIT(ch->act,PLR_QUESTOR);
	    xprintf(buf, "You have %d minutes to complete this job.",ch->countdown);
	    do_say(questman, buf);
	    xprintf(buf, "May the gods go with you!");
	    do_say(questman, buf);
	}
	return;
    }

    else if (!strcmp(arg1, "complete"))
    {
        act("$n informs $N $e has completed $s job.", ch, NULL, questman, TO_ROOM); 
	act("You inform $N you have completed $s job.",ch, NULL, questman, TO_CHAR);
	if (ch->questgiver != questman)
	{
	    xprintf(buf, "I never sent you on a job! Perhaps you're thinking of someone else.");
	    do_say(questman,buf);
	    return;
	}

	if (IS_SET(ch->act,PLR_QUESTOR))
	{
	bool obj_found = FALSE;
	    if (ch->questobj > 0 && ch->countdown > 0)
	    {
    		for (obj = ch->carrying; obj != NULL; obj= obj_next)
    		{
        	    obj_next = obj->next_content;
        
		    if (obj != NULL && obj->pIndexData->vnum == ch->questobj)
		    {
			obj_found = TRUE;
            	        break;
		    }
        	}
	    }

     if ((ch->questmob == -1 || (ch->questobj && obj_found)) 
	&& ch->countdown > 0)
	{
	int reward, pointreward;
	int level;
		level = ch->level;
		reward = number_range(1*level,3*level);
                pointreward = number_range(1,level);

		xprintf(buf, "Congratulations on completing your job!");
		do_say(questman,buf);
		xprintf(buf,"As a reward, I am giving you %d glory points, and %d gold.",pointreward,reward);
		do_say(questman,buf);

	        REMOVE_BIT(ch->act,PLR_QUESTOR);
	        ch->questgiver = NULL;
	        ch->countdown = 0;
	        ch->questmob = 0;
		ch->questobj = 0;
	        ch->nextquest = 10;
		ch->gold += reward;
		ch->quest_curr += pointreward;
		ch->quest_accum += pointreward;
		if(obj_found) extract_obj(obj);
	 return;
	}
     else if((ch->questmob > 0 || ch->questobj > 0)
          && ch->countdown > 0)
	    {
		xprintf(buf, "You haven't completed the job yet, but there is still time!");
		do_say(questman, buf);
		return;
	    }
     }
	if (ch->nextquest > 0)
	{
	    xprintf(buf,"But you didn't complete your job in time!");
	}
	else xprintf(buf, "You have to request a job first, %s.",ch->name);
	do_say(questman, buf);
	return;
    }

    else if (!strcmp(arg1, "quit"))
    {
        act("$n informs $N $e wishes to quit $s job.", ch, NULL,questman, TO_ROOM); 
	act ("You inform $N you wish to quit $s job.",ch, NULL, questman, TO_CHAR);
	if (ch->questgiver != questman)
	{
	    xprintf(buf, "I never sent you on a job! Perhaps you're thinking of someone else.");
	    do_say(questman,buf);
	    return;
	}

	if (IS_SET(ch->act,PLR_QUESTOR))
	{
            REMOVE_BIT(ch->act,PLR_QUESTOR);
            ch->questgiver = NULL;
            ch->countdown = 0;
            ch->questmob = 0;
	    ch->questobj = 0;
            ch->nextquest = 30;

	    xprintf(buf, "Your job is over, but for your cowardly behavior, you may not quest again for 30 minutes.");
	    do_say(questman,buf);
		    return;
        }
	else
	{
	send_to_char("You aren't on a job!",ch);
	return;
	} 
    }


    send_to_char("JOB commands: Info, Time, Request, Complete, Quit, List, and Buy.\n\r",ch);
    send_to_char("For more information, type 'Help job'.\n\r",ch);
    return;
}

void generate_quest(CHAR_DATA *ch, CHAR_DATA *questman)
{
    CHAR_DATA *victim;
    MOB_INDEX_DATA *vsearch;
    ROOM_INDEX_DATA *room;
    OBJ_DATA *questitem;
    char buf [MAX_STRING_LENGTH];
    long mcounter;
    int mob_vnum;

    for (mcounter = 0; mcounter < 99999; mcounter ++)
    {
	mob_vnum = number_range(200, 32600);

	if ( (vsearch = get_mob_index(mob_vnum) ) != NULL )
	{
            if (quest_level_diff(ch->level, vsearch->level) == TRUE
                && vsearch->pShop == NULL
    		&& !IS_SET(vsearch->act, ACT_TRAIN)
    		&& !IS_SET(vsearch->act, ACT_PRACTICE)
    		&& !IS_SET(vsearch->act, ACT_IS_HEALER)
		&& !IS_SET(vsearch->act, ACT_PET)
		&& !IS_SET(vsearch->affected_by, AFF_CHARM)
		&& !IS_SET(vsearch->affected_by, AFF_INVISIBLE)
		&& number_percent() < 40) break;
		else vsearch = NULL;
	}
    }

    if (( vsearch == NULL || ( victim = get_char_world( ch, vsearch->player_name ) ) == NULL )
	 || strstr(victim->pIndexData->area->builders, "Unlinked") 
         || strstr(victim->pIndexData->area->builders, "NoQuest") 
         || strstr(victim->pIndexData->area->builders, "Noquest"))

    {
	xprintf(buf, "I'm sorry, but I don't have any job for you at this time.");
	do_say(questman, buf);
	xprintf(buf, "Try again later.");
	do_say(questman, buf);
	ch->nextquest = 2;
        return;
    }

    if( ( ( room = find_location( ch, victim->name ) ) == NULL ) ||
	strstr(room->area->builders, "Unlinked" )    )
    {
	xprintf(buf, "I'm sorry, but I don't have any job for you at this time.");
	do_say(questman, buf);
	xprintf(buf, "Try again later.");
	do_say(questman, buf);
	ch->nextquest = 2;
        return;
    }

    /*  40% chance it will send the player on a 'recover item' quest. */

    if (number_percent() < 40) 
    {
	int numobjs=0;
	int descnum = 0;

	for(numobjs=0;quest_desc[numobjs].name != NULL;numobjs++)
	    ;
	numobjs--;
	descnum = number_range(0,numobjs);
        questitem = create_object( get_obj_index(QUEST_OBJ), ch->level ); 

	if(descnum > -1)
	{
	if(questitem->short_descr)
	   free_string(questitem->short_descr);
	if(questitem->description)
	   free_string(questitem->description);
	if(questitem->name)
	   free_string(questitem->name);

	questitem->name        = str_dup(quest_desc[descnum].name);
	questitem->description = str_dup(quest_desc[descnum].long_descr);
	questitem->short_descr = str_dup(quest_desc[descnum].short_descr);
}
	obj_to_room(questitem, room);
	ch->questobj = questitem->pIndexData->vnum;

	xprintf(buf, "Vile pilferers have stolen %s from the royal treasury!",questitem->short_descr);
	do_say(questman, buf);
	do_say(questman, "My court wizardess, with her magic mirror, has pinpointed its location.");

	xprintf(buf, "Look in the general area of %s for %s!",room->area->name, room->name);
	do_say(questman, buf);

	return;
    }

    /* Quest to kill a mob */

    else 
    {
    switch(number_range(0,1))
    {
	case 0:
        xprintf(buf, "An enemy of mine, %s, is making vile threats against the crown.",victim->short_descr);
        do_say(questman, buf);
        xprintf(buf, "This threat must be eliminated!");
        do_say(questman, buf);
	break;

	case 1:
	xprintf(buf, "Rune's most heinous criminal, %s, has escaped from the dungeon!",victim->short_descr);
	do_say(questman, buf);
	xprintf(buf, "Since the escape, %s has murdered %d civillians!",victim->short_descr, number_range(2,20));
	do_say(questman, buf);
	do_say(questman,"The penalty for this crime is death, and you are to deliver the sentence!");

	break;
    }

    if (room->name != NULL)
    {
        xprintf(buf, "Seek %s out somewhere in the vicinity of %s!",victim->short_descr,room->name);
        do_say(questman, buf);

	xprintf(buf, "That location is in the general area of %s.",room->area->name);
	do_say(questman, buf);
    }
    ch->questmob = victim->pIndexData->vnum;
    }

    return;
}

bool quest_level_diff(int clevel, int mlevel)
{
    if (clevel < 9 && mlevel < clevel + 3) return TRUE;
    else if (clevel <= 9 && mlevel < clevel + 5 
	  && mlevel > clevel - 5) return TRUE;
    else if (clevel <= 14 && mlevel < clevel + 7 
	  && mlevel > clevel - 5) return TRUE;
    else if (clevel <= 21 && mlevel < clevel + 9 
	  && mlevel > clevel - 4) return TRUE;
    else if (clevel <= 29 && mlevel < clevel + 11 
	  && mlevel > clevel - 3) return TRUE;
    else if (clevel <= 37 && mlevel < clevel + 13
	  && mlevel > clevel - 2) return TRUE;
    else if (clevel <= 55 && mlevel < clevel + 15
	  && mlevel > clevel - 1) return TRUE;
    else if (clevel <= 70 && mlevel < clevel + 17
          && mlevel > clevel - 1) return TRUE;
    else if(clevel > 70) return TRUE; 
    else return FALSE;
}
/*
bool quest_level_diff(int clevel, int mlevel)
{
     if
        (
        (mlevel > (clevel * 1.25 + 3))
        &&
        (mlevel < (clevel * 1.35 + 3))
        )
     return TRUE;
     else return FALSE;
}
*/
/* Called from update_handler() by pulse_area */

void quest_update(void)
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *ch;
    char buf[MAX_STRING_LENGTH]; 

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->character != NULL && d->connected == CON_PLAYING)
        {
        ch = d->character;
	if(IS_NPC(ch))
	continue; 
        if (ch->nextquest > 0)
        {
            ch->nextquest--;
            if (ch->nextquest == 0)
            {
                send_to_char("You feel up to another job.\n\r",ch);
                return;
            }
        }
        else if (IS_SET(ch->act,PLR_QUESTOR))
        {
            if (--ch->countdown <= 0)
            {
                ch->nextquest = 10;
                xprintf(buf,"You have run out of time for your job!\n\rYou may try again in %d minutes.\n\r",ch->nextquest);
		send_to_char(buf, ch);
                REMOVE_BIT(ch->act,PLR_QUESTOR);
                ch->questgiver = NULL;
                ch->countdown = 0;
                ch->questmob = 0;
                ch->questobj = 0;
            }
            if (ch->countdown > 0 && ch->countdown < 6)
            {
                send_to_char("Better hurry, you're almost out of time for your job!\n\r",ch);
                return;
            }
        }
        }
    }
    return;
}

void do_qecho( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];

  if( ch->pcdata->quest == NULL )
  {
    send_to_char("You aren't running a quest.\r\n", ch);
    return;
  }

  for ( d = descriptor_list; d != NULL; d = d->next )
 {
    victim = d->original ? d->original : d->character;

    if ( d->connected == CON_PLAYING &&
         ch != victim &&
         ( ( !ch->pcdata->quest->closed &&
             victim->level >= ch->pcdata->quest->min_level &&
             victim->level <= ch->pcdata->quest->max_level ) ||
           IS_IMMORTAL(victim) ) )
      {
        if(!IS_IMMORTAL(victim))
          act_new( argument, ch, NULL, d->character, TO_VICT,POS_SLEEPING);
        else
        {
          xprintf(buf, "quest> %s\r\n", argument );
          send_to_char(buf,ch);
        }
      }
  }

  return;
}

void send_to_quest( char *argument, CHAR_DATA *ch )
{
  DESCRIPTOR_DATA *d;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH];

  if( ch->pcdata->quest == NULL )
  {
    bug("NULL Quest data.",0);
    return;
  }

  for ( d = descriptor_list; d != NULL; d = d->next )
  {
    victim = d->original ? d->original : d->character;

    if ( d->connected == CON_PLAYING &&
         ch != victim &&
         victim->pcdata->quest &&
         victim->pcdata->quest->quest_god == ch )
    {
      act_new( argument, ch, NULL, d->character, TO_VICT,POS_SLEEPING );
    }
  }

  xprintf(buf, "quest> %s\r\n", argument );
  send_to_char(buf,ch);

  return;
}

void do_quest( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  argument = one_argument( argument, arg );

  if ( ch->desc->original )
  {
    send_to_char("You may not use quest while switched.\n\r", ch );
    return;
  }

  if ( IS_IMMORTAL(ch) )
  {
    if ( arg[0] == '\0' )
    {
      send_to_char("Syntax :                 \r\n", ch);
      if( ch->pcdata->quest == NULL )
        send_to_char("         quest new\r\n", ch);
      else
      {
        if( !ch->pcdata->quest->closed )
        {
          send_to_char("         quest start\r\n",ch);
          send_to_char("         quest minlevel <level>\r\n",ch);
          send_to_char("         quest maxlevel <level>\r\n",ch);
          send_to_char("         quest name     <name>\r\n",ch);
          send_to_char("         quest short    <short desc>\r\n",ch);
          send_to_char("         quest long     <text>\r\n",ch);
          send_to_char("         quest protection\r\n\r\n",ch);
          send_to_char("         quest list\r\n\r\n",ch);
          send_to_char("         quest add <player>\r\n",ch);
        }
        else
        {
          send_to_char("         quest end\r\n\r\n",ch);
        }
        send_to_char("         quest remove <player>\r\n",ch);
      }
      return;
    }

    if( ch->pcdata->quest == NULL )
    {
      if( !str_cmp(arg, "new") )
      {
        do_qnew( ch, argument );
        return;
      }
    }
    else
    {
      if( !ch->pcdata->quest->closed )
      {
        if( !str_cmp(arg, "start") )
        {
          do_qstart( ch, argument );
          return;
        }

        if( !str_cmp(arg, "minlevel") )
        {
          do_qminlevel( ch, argument );
          return;
        }

        if( !str_cmp(arg, "maxlevel") )
        {
          do_qmaxlevel( ch, argument );
          return;
        }

        if( !str_cmp(arg, "name") )
        {
          do_qname( ch, argument );
          return;
        }

        if( !str_cmp(arg, "short") )
        {
          do_qshort( ch, argument );
          return;
        }

        if( !str_cmp(arg, "long") )
        {
          do_qlong( ch, argument );
          return;
        }

        if( !str_cmp(arg, "protection") )
        {
          do_qprotection( ch, argument );
          return;
        }
      }
      else
      {
        if( !str_cmp(arg, "end") )
        {
         do_qend( ch, argument );
          return;
        }
      }

      if( !str_cmp(arg, "add") )
      {
        do_qadd( ch, argument );
        return;
      }

      if( !str_cmp(arg, "list") )
      {
        do_qlist( ch, argument );
        return;
      }

      if( !str_cmp(arg, "remove") )
      {
        do_qremove( ch, argument );
        return;
      }
    }

    do_quest( ch, "" );
  }
  else
  {
    if ( arg[0] == '\0' )
    {
      send_to_char("Syntax :                 \r\n", ch);
      send_to_char("         quest apply <quest>\r\n",ch);
      send_to_char("         quest list\r\n",ch);
      send_to_char("         quest quit\r\n",ch);

      return;
    }

    if( !str_cmp(arg, "apply") )
    {
      do_qapply( ch, argument );
      return;
    }

    if( !str_cmp(arg, "list") )
    {
      do_qlistp( ch, argument );
      return;
    }

    if( !str_cmp(arg, "quit") )
    {
      do_qquit( ch, argument );
      return;
    }

    do_quest( ch, "" );
  }

  return;
}

void do_qnew( CHAR_DATA *ch, char *argument )
{
  QUEST_DATA *quest;

  quest = ch->pcdata->quest = alloc_mem( sizeof( QUEST_DATA ) );
  quest->quest_god  = ch;
  quest->name       = NULL;
  quest->short_name = NULL;
  quest->long_name  = NULL;
  quest->protection = FALSE;
  quest->closed     = FALSE;
  quest->min_level  = 0;
  quest->max_level  = 0;

  AUTHORIZE( ch, auth_questtalk );
  send_to_char("Ok.\r\n",ch);
  do_echo(ch,"{YQUEST!!! {W-------> type 'quest' for more information!.");
  return;
}

void do_qstart( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;

    if ( quest->name == NULL       ||
         quest->short_name == NULL || 
         quest->long_name == NULL  ||
         quest->min_level == 0     ||
         quest->max_level == 0 )
    {
        send_to_char("You havent finished setting up the quest parameters.\r\n",ch);
        return;
    }

    send_to_quest("The Quest has started.",ch);
    send_to_char("Ok.\r\n",ch);
    ch->pcdata->quest->closed = TRUE;

    return;
}

void do_qminlevel( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;
    char buf[MAX_STRING_LENGTH];

    quest->min_level = atoi( argument );
    xprintf(buf, "Minimum level for quest is now %d\r\n",quest->min_level);
    send_to_char(buf,ch);

    return;
}

void do_qmaxlevel( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;
    char buf[MAX_STRING_LENGTH];

    quest->max_level = atoi( argument );
    xprintf(buf, "Maximum level for quest is now %d\r\n",quest->max_level);
    send_to_char(buf,ch);

    return;
}

void do_qname( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;
    char buf[MAX_STRING_LENGTH];

    if( quest->name != NULL )
       free_string( quest->name );

    quest->name = str_dup( argument );
    xprintf(buf, "Quest name set to: %s\r\n", quest->name);
    send_to_char(buf,ch);

    return;
}

void do_qshort( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;

if(IS_NPC(ch))
return;

string_append(ch, &quest->short_name);
return;
}

void do_qlong( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;

if(IS_NPC(ch))
return;

string_append(ch, &quest->long_name);
return;
}

void do_qprotection( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;

    if( quest->protection )
    {
        send_to_char( "Quest Protection is now OFF\r\n", ch );
        quest->protection = FALSE;
        return;
    }
    else
    {
        send_to_char( "Quest Protection is now ON\r\n", ch );
        quest->protection = TRUE;
        return;
    }
}

void do_qend( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    if( quest == NULL )
    {
       bug("Null quest pointer on exit!",0);
       return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        victim = d->original ? d->original : d->character;

        if ( d->connected == CON_PLAYING &&
             ch != victim &&
             victim->pcdata->quest &&
             victim->pcdata->quest->quest_god == ch )
        {
          victim->pcdata->quest = NULL;
          REVOKE( victim, auth_questtalk );
          act_new( "The Quest has ended. Thank you for participating.\r\n", ch, NULL, d->character, TO_VICT,POS_SLEEPING );
        }
    }

    free_mem( ch->pcdata->quest, sizeof( QUEST_DATA ) );
    ch->pcdata->quest = NULL;
    REVOKE( ch, auth_questtalk );
    send_to_char("Ok.\r\n",ch);

    return;
}

void do_qlist( CHAR_DATA *ch, char *argument )
{
    QUEST_DATA *quest = ch->pcdata->quest;
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    send_to_char("{BQ{buest {BP{barameters{W:{w\r\n",ch);

    xprintf(buf, "{BN{bame {W: %s\r\n", quest->name ? quest->name : "{W({wnone{W){n" );
    send_to_char(buf, ch);
    xprintf(buf, "{BS{bhort {BD{bescription {W:{n %s\r\n", quest->short_name ? quest->short_name : "{W({wnone{W){n" );
    send_to_char(buf, ch);
    xprintf(buf, "{BL{bong {BD{bescription {W:{w %s\r\n", quest->long_name ? quest->long_name : "{W({wnone{W){w" );
  send_to_char(buf, ch);
    xprintf(buf, "{BP{blayer {BP{brotection {W: {w%s      {BR{bunning {W:{w %s\r\n",
                 quest->protection ? "YES" : "NO",
                 quest->closed ? "YES" : "NO" );
    send_to_char(buf, ch);
    xprintf(buf, "{BM{binimum {BL{bevel {W: {w%d      {BM{baximum {BL{bevel {W: {w %d\r\n",
                 quest->min_level, quest->max_level );
    send_to_char(buf, ch);

    send_to_char("\r\nQuesting Players{W:{w\r\n",ch);
   for ( d = descriptor_list; d != NULL; d = d->next )
    {
        victim = d->original ? d->original : d->character;

        if ( d->connected == CON_PLAYING &&
             ch != victim &&
             victim->pcdata->quest &&
             victim->pcdata->quest->quest_god == ch )
        {
             xprintf(buf, "             %s%s\r\n", victim->name,
             IS_IMMORTAL(victim) ? " {W({wimm{W){w" : "" );
             send_to_char(buf,ch);
        }
    }

    if( !quest->closed )
    {
      send_to_char("\r\nApplied Players:\r\n",ch);
       for ( d = descriptor_list; d != NULL; d = d->next )
       {
           victim = d->original ? d->original : d->character;

           if ( d->connected == CON_PLAYING &&
                ch != victim &&
                victim->pcdata->quest_apply == quest )
           {
                xprintf(buf,"             %s%s\r\n", victim->name,
                IS_IMMORTAL(victim) ? " {W({wimm{W){n" : "" );
                send_to_char(buf,ch);
           }
       }
    }
}

void do_qadd( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA  *victim;
    char buf[MAX_STRING_LENGTH];

    if( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
        send_to_char("Player not found.\r\n",ch);
        return;
    }

    if( IS_NPC(victim) )
    {
        send_to_char("NPC's cannot quest.\r\n",ch);
       return;
    }

    if( ch->pcdata->quest == NULL )
    {
        send_to_char("You arn't running a quest!\r\n",ch);
        return;
    }

    if( victim->pcdata->quest != NULL )
    {
        send_to_char("That player is already questing.\r\n", ch);
        return;
    }

    if( victim->pcdata->quest_apply != ch->pcdata->quest )
    {
       send_to_char("That player hasn't applied to be in your Quest.\r\n",ch);
        return;
    }

    xprintf(buf, "The Questmaster welcomes %s to the Quest.",victim->name);
    send_to_quest(buf,ch);

    victim->pcdata->quest = ch->pcdata->quest;
    victim->pcdata->quest_apply = NULL;
    AUTHORIZE( victim, auth_questtalk );

    send_to_char("The Questmaster has accepted your application.\r\n",victim);
    return;
}

void do_qremove( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA  *victim;
    char buf[MAX_STRING_LENGTH];

    if( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
        send_to_char("Player not found.\r\n",ch);
        return;
    }

    if( IS_NPC(victim) )
    {
        send_to_char("NPC's cannot quest.\r\n",ch);
        return;
    }

    if( ch->pcdata->quest == NULL )
    {
       send_to_char("You arn't running a quest!\r\n",ch);
        return;
    }

    if( victim->pcdata->quest != ch->pcdata->quest )
    {
        send_to_char("That player isn't questing, or is on another quest.\r\n",ch);
        return;
    }

    REVOKE( victim, auth_questtalk );
    victim->pcdata->quest = NULL;
    xprintf(buf, "The Questmaster has removed {B%s{n from the Quest.",victim->name);
    send_to_quest(buf,ch);
    send_to_char("You have been removed from the quest.\r\n",ch);

    return;
}

void do_qapply( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    bool found = FALSE;

    if( ch->pcdata->quest != NULL )
    {
        send_to_char("You are already on the quest.\r\n", ch);
        return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        victim = d->original ? d->original : d->character;

        if ( d->connected == CON_PLAYING &&
             !IS_NPC( victim ) &&
             IS_IMMORTAL(victim) &&
             victim->pcdata->quest &&
             !str_cmp(victim->pcdata->quest->name, argument) )
        {
          if( ch->level > victim->pcdata->quest->max_level ||
              ch->level < victim->pcdata->quest->min_level )
          {
             send_to_char("You are out of level range.\r\n",ch);
             return;
          }
          if( victim->pcdata->quest->closed == TRUE )
          {
             send_to_char("That quest have already begun.\r\n",ch);
             return;
          }
          ch->pcdata->quest_apply = victim->pcdata->quest;
          found = TRUE;
          break;
        }
    }

    if( !found )
    {
       xprintf(buf, "Nobody is running a quest called %s.\r\n", argument);
       send_to_char(buf,ch);
    }
    else
    {
       xprintf(buf, "You have applied to be in the {g%s{w quest.\r\n",ch->pcdata->quest_apply->name);
       send_to_char(buf,ch);
       xprintf(buf, "%s has applied for your quest.\r\n", ch->name);
       send_to_char(buf, ch->pcdata->quest_apply->quest_god);
    }

    return;
}

void do_qlistp( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    QUEST_DATA *quest = NULL;


    if( ch->pcdata->quest == NULL )
    {
       send_to_char("Available Quests\r\n\r\n",ch);
       for ( d = descriptor_list; d != NULL; d = d->next )
       {
           victim = d->original ? d->original : d->character;

           if ( d->connected == CON_PLAYING &&
                !IS_NPC( victim ) &&
               IS_IMMORTAL(victim) &&
               victim->pcdata->quest )
           {
               quest = victim->pcdata->quest;
               xprintf(buf,"{BN{bame{W:{G %-20s{BQ{buest{BM{baster{W:{G %-12s {BL{bevels{W:{G %d-%d%s%s{w\r\n",
                  quest->name, quest->quest_god->name,
                  quest->min_level, quest->max_level,
                  quest->protection ? " {W({wprotection{W){w" : "",
                  quest->closed ? " {W({wclosed{W){w" : "" );
               send_to_char(buf,ch);
           }
       }
    }
    else
    {
        do_font(ch,ch->pcdata->quest->name);
        send_to_char("{B",ch);
        send_to_char(ch->pcdata->quest->long_name,ch);
        send_to_char("{x\n",ch);

       xprintf(buf,"Other players in quest %s are{W:{w\r\n",ch->pcdata->quest->name);
       send_to_char(buf,ch);

       for ( d = descriptor_list; d != NULL; d = d->next )
       {
           victim = d->original ? d->original : d->character;


           if ( d->connected == CON_PLAYING &&
               victim->pcdata->quest == ch->pcdata->quest)
           {
               xprintf(buf,"   %s\r\n",victim->name);
               send_to_char(buf,ch);
           }
       }
    }

    return;
}

void do_qquit( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if( ch->pcdata->quest == NULL )
    {
        bug("Null Quest pointer",0);
        return;
    }

    xprintf(buf, "%s has quit from the Quest.",ch->name);
    send_to_quest(buf,ch->pcdata->quest->quest_god);
    send_to_char(buf,ch->pcdata->quest->quest_god);
    ch->pcdata->quest = NULL;
    send_to_char("Ok.\r\n",ch);
    REVOKE( ch, auth_questtalk );

    return;
}
