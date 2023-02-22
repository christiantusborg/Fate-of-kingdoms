/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		   *
***************************************************************************/

#include <sys/types.h> 
#include <sys/time.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include "merc.h"  
#include "recycle.h" 
#include "tables.h"  
/*
const	struct	kingdom_cmd_type	kingdom_cmd_table	[] =
{
	{ "peace","kpeace",auth_kpeace,KINGDOM_LOG},
	{ "war","kwar",auth_kpeace,KINGDOM_LOG},
	{ "surrender","ksurrender",auth_kpeace,KINGDOM_LOG},
	{ "ceasefire","kceasefire",auth_kpeace,KINGDOM_LOG},
	{ "ally","kally",auth_kpeace,KINGDOM_LOG},
	pact
	{ "banish","kbanish",auth_kpeace,KINGDOM_LOG},
	{ "induct","kinduct",auth_kpeace,KINGDOM_LOG},
	{ "resign","kresign",auth_kpeace,KINGDOM_LOG},
	{ "promote","kpromote",auth_kpeace,KINGDOM_LOG},
	{ "demote","kdemote",auth_kpeace,KINGDOM_LOG},
	{ "members","kmembers",auth_kpeace,KINGDOM_LOG},
	{ "prospects","kprospects",auth_kpeace,KINGDOM_LOG},
	{ "areas","kareas",auth_kpeace,KINGDOM_LOG},
	{ "balance","kbalance",auth_kpeace,KINGDOM_LOG},
	{ "withdraw","kwithdraw",auth_kpeace,KINGDOM_LOG},
	{ "message","kmessage",auth_kpeace,KINGDOM_LOG},
	{ "deposit","kdeposit",auth_kpeace,KINGDOM_LOG},
	{ "law","klaw",auth_kpeace,KINGDOM_LOG},
	{ "guardian","kguardian",auth_kpeace,KINGDOM_LOG},
	{ "grant","kgrant",auth_kpeace,KINGDOM_LOG},
	{ "revoke","krevoke",auth_kpeace,KINGDOM_LOG},
	{ "apply","kapply",auth_kpeace,KINGDOM_LOG},
	{ "info","kinfo",auth_kpeace,KINGDOM_LOG},
	{ "status","kstatus",auth_kpeace,KINGDOM_LOG},
	{ "casualties","kcasualties",auth_kpeace,KINGDOM_LOG}
}

void kingdom_command(CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];
  int cmd;
  int col;
  int level;    

	send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);
	col = 0;
    
	for (cmd = 1; kingdom_cmd_table[cmd].name[0] != '\0'; cmd++)
	  {
		if ( kingdom_cmd_table[cmd].show
		 && kingdom_cmd_table[cmd].group == CMD_KINGDOM
		 &&   NEW_IS_AUTHORIZED(ch, kingdom_cmd_table[cmd].auth))
		  {
			xprintf( buf, "[%-12]", kingdom_cmd_table[cmd].name );
			send_to_char( buf, ch );
			
			if ( ++col % 4 == 0 )
				send_to_char( "\n\r", ch );
		  } 
	  }
	send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);

  return;
}



void do_kingdom(CHAR_DATA *ch, char *argument)
{

}
*/
