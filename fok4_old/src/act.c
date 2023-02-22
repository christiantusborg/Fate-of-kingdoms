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

void  	do_act_warrior	args( ( CHAR_DATA *ch) );

const	struct	act_table_type	act_table	[] =
{
	{ "warrior",	ACT_WARRIOR,	do_act_warrior },
//	{ "cleric",	ACT_CLERIC,	do_act_cleric },
//	{ "mage",	ACT_MAGE,	do_act_mage },
//	{ "thief",	ACT_THIEF,	do_act_thief },
	{ NULL,		NULL,		NULL }
};



void violence_update_act(void)
{
  int	act = 0;
  CHAR_DATA *ch;
  CHAR_DATA *ch_next;
//bug("I win you lose",0);
 	for ( ch = char_list; ch != NULL; ch = ch_next )
 	  {
		ch_next = ch->next;
		
		for (act = 0;act_table[act].name != NULL; act++)
	  	 {
			if (IS_SET(ch->act,act_table[act].class_act))
				(*act_table[act].do_act) (ch);
		  }
	  }
} 

void	do_act_warrior(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	
	victim = ch->last_fought;
	if (ch->position == POS_STANDING)
		if (!CAN_DETECT(ch, DETECT_HIDDEN))
		{
		do_detect_hidden(ch, "");
		do_say( ch, "Hello Honey, am I on crack or what?!" );
		}
}

