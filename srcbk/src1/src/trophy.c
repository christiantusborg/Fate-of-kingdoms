/***************************************************************************
*     Fate 4.0 is copyright 2000-2003 Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/****************************************************************************
				
****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"  
#include "recycle.h"  
#include "tables.h"  
#include "lookup.h" 
#include "arena.h"
#include "Log.h"

int trophy_add( CHAR_DATA *ch, CHAR_DATA *victim)
{
  int i;
  bool done = FALSE;
  int count;
  int vnum = -1;
  int level_range;
  
  level_range = victim->level - ch->level;
 
  if (level_range > -20)
  	vnum = victim->pIndexData->vnum;
  else
	return -1;
	// Check if mob is in the list
	for ( i = 0; i < 5; i++ )
	  {
		if (ch->pcdata->trophyvnum[i] == vnum)
		  {
		  	count = ch->pcdata->trophycount[i] += 1;
		  	done = TRUE;
		  	break;
		  }
	  }
	if (done) //if in the list out
		return count;

	//Move and make room for new add
	for ( i = 5; i > 0; i-- )
	  {
	  	ch->pcdata->trophyvnum[i]  = ch->pcdata->trophyvnum[i-1];
	  	ch->pcdata->trophycount[i] = ch->pcdata->trophycount[i-1];
	  }
	  

for ( i = 29; i > 5; i-- )
	  {
	  	ch->pcdata->trophyvnum[i]  = -1;
	  	ch->pcdata->trophycount[i] = -1;
	  }	  
	//Add new
	ch->pcdata->trophyvnum[0]  = vnum;
	ch->pcdata->trophycount[0] = 1;
return 1;
}

void do_trohpy(CHAR_DATA *ch, char *argument)
{
	
	MOB_INDEX_DATA *pMobIndex;
	int i;
	char buf[MAX_STRING_LENGTH];
	
	send_to_char("--- Mobile trohpylist ----------------------------------\n\r", ch );
	for ( i = 0; i < 5; i++ )
	  {	
	    	if ( ( pMobIndex = get_mob_index( ch->pcdata->trophyvnum[i] ) ) == NULL )
	 	  {
		     	xprintf(buf, "- None                : - %s -\n\r",trophy_count(ch->pcdata->trophycount[i]));
			send_to_char(buf,  ch );
		  }
		 else
		  {
		     	xprintf(buf, "- %-20s:  - %s -\n\r",pMobIndex->short_descr,trophy_count(ch->pcdata->trophycount[i]));
			send_to_char(buf,  ch );
	}	  }
return;
}