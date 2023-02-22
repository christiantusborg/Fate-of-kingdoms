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

/*
int vampire_check_generation()
void vampire_feed()
int vampire_check_blood()
*/
/*
void give_prog_marcus(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	if(IS_IMMORTAL(ch))
		{
			do_say(mob,"You are an immortal, go back to your own business.");
			do_say(mob,"Your attempt has been logged.");
      xprintf(log_buf,"%s has tried to solve the Vampire quest.",ch->name);
			wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
      return;
		}
		
		
	char log_buf[MAX_STRING_LENGTH];
	OBJ_DATA *load_letter;
  
    if ( obj->pIndexData->vnum != OBJ_VAMPIRE_MARCUS )
    	{
				do_say(mob, "What is this? I don't need this.");
				obj_from_char( obj );
				obj_to_char( obj, ch );
				act("$N gives you $p back.", ch, obj, mob, TO_CHAR);
				act("$N gives $p back to $n.", ch, obj, mob, TO_ROOM);
				act("You give $p back to $N", mob, obj, ch, TO_CHAR);
				return;
    }
    if ( obj->pIndexData->vnum == OBJ_VAMPIRE_VICTOR_TO_MARCUS )
    	{
				do_say(mob, "What is this? I don't need this.");
				obj_from_char( obj );
				obj_to_char( obj, ch );
				act("$N gives you $p back.", ch, obj, mob, TO_CHAR);
				act("$N gives $p back to $n.", ch, obj, mob, TO_ROOM);
				act("You give $p back to $N", mob, obj, ch, TO_CHAR);
    		do_say( mob, ".");
    		quest_vampire = 2;
				return;
    }


    do_say( mob, ".");
    obj_to_char(create_object( get_obj_index(OBJ_VAMPIRE_VICTOR), -1 ), mob);
    do_say(mob,  "Now, back to work.");
	  act("$N gives you $p.", ch, obj, mob, TO_CHAR);
		act("$N gives $p to $n.", ch, obj, mob, TO_ROOM);
		act("You give $p to $N", mob, obj, ch, TO_CHAR);    
    extract_obj( obj );
    quest_vampire = 1;
  	return;
}


void give_prog_victor(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	if(IS_IMMORTAL(ch))
		{
      xprintf(log_buf,"%s has tried to solve the Vampire quest.",ch->name);
			wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
      return;
		}
		
		
	char log_buf[MAX_STRING_LENGTH];
	OBJ_DATA *load_letter;
  
    if ( obj->pIndexData->vnum != OBJ_VAMPIRE_VICTOR )
    	{
				do_say(mob, "What is this? I don't need this.");
				obj_from_char( obj );
				obj_to_char( obj, ch );
				act("$N gives you $p back.", ch, obj, mob, TO_CHAR);
				act("$N gives $p back to $n.", ch, obj, mob, TO_ROOM);
				act("You give $p back to $N", mob, obj, ch, TO_CHAR);
    		do_say( mob, ".");
    		quest_vampire = 2;
				return;
    }

    if ( obj->pIndexData->vnum != OBJ_VAMPIRE_WAREWOLF )
    	{
				do_say(mob, "What is this? I don't need this.");
				obj_from_char( obj );
				obj_to_char( obj, ch );
				act("$N gives you $p back.", ch, obj, mob, TO_CHAR);
				act("$N gives $p back to $n.", ch, obj, mob, TO_ROOM);
				act("You give $p back to $N", mob, obj, ch, TO_CHAR);
    		do_say( mob, ".");
    		quest_vampire = 2;
				return;
    }

    if ( obj->pIndexData->vnum != OBJ_VAMPIRE_HUNTER )
    	{
				do_say(mob, "What is this? I don't need this.");
				obj_from_char( obj );
				obj_to_char( obj, ch );
				act("$N gives you $p back.", ch, obj, mob, TO_CHAR);
				act("$N gives $p back to $n.", ch, obj, mob, TO_ROOM);
				act("You give $p back to $N", mob, obj, ch, TO_CHAR);
    		do_say( mob, ".");
    		quest_vampire = 2;
				return;
    }

	return;
}
 // *** WAREWOLF ***

void speech_prog_warewolf(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{

  return;

}

void give_prog_warewolf(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	return;
}

bool death_prog_warewolf(CHAR_DATA *mob)
{
 OBJ_DATA *obj;
 obj = create_object(get_obj_index(OBJ_VAMPIRE_VICTOR), 0);
 
}
 // *** HUNTER ***
void speech_prog_hunter(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{

  return;

}

void give_prog_hunter(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	return;
}

bool death_prog_hunter(CHAR_DATA *mob)
{
 OBJ_DATA *obj;
 obj = create_object(get_obj_index(OBJ_VAMPIRE_VICTOR), 0);
 
}


// *** Vampire Guest ***


void vampire_quest_begin()

void vampire_quest_end()


3 =1 ()
4 =1
5 =2  ()
6 =2
7 =3
8 =3

*/

