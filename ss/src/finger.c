/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"

void finger_show	args ((CHAR_DATA *ch, CHAR_DATA *victim,bool isnoline));

void do_finger(CHAR_DATA *ch, char *argument)
{
DESCRIPTOR_DATA d;
CHAR_DATA *victim;
bool isChar = FALSE;
char arg1[MAX_STRING_LENGTH];
argument = one_argument(argument, arg1);

    if ( ( victim = get_char_world( ch, arg1 ) ) != NULL )
    {
    finger_show(ch,victim,TRUE);
    return;
    }
    

  isChar = load_char_obj(&d, arg1); /* char pfile exists? */

  if (!isChar)
  {
    send_to_char("Load Who? Are you sure? I cant seem to find them.\n\r",ch);
    return;
  }
  reset_char(d.character);    

  victim = d.character;
  finger_show(ch,victim,FALSE);
 return;
}


void finger_show(CHAR_DATA *ch, CHAR_DATA *victim,bool isnoline)
{

char buf[MAX_STRING_LENGTH];
bool laston_found = TRUE;
int d, h, m, s = 0;


    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

xprintf(buf, "                             %16s\n\r", victim->name);
send_to_char(buf, ch);
send_to_char("---------------------------------------------------------------\n\r", ch);
xprintf(buf,"Real Name: %s{x\n\r", victim->pcdata->real_name !=NULL ? victim->pcdata->real_name:"Not specified.");
send_to_char(buf, ch);
xprintf(buf,"Email:     %s{x\n\r", victim->pcdata->email != NULL ? victim->pcdata->email:"Not specified.");
send_to_char(buf, ch);
//xprintf(buf,"Icq:       %16s{x MSN: %16s{x\n\r", victim->pcdata->icq != NULL ? victim->pcdata->icq:"Not specified.",
//	victim->pcdata->msn != NULL ? victim->pcdata->msn:"Not specified.");
//send_to_char(buf, ch);
send_to_char("---------------------------------------------------------------\n\r", ch);
xprintf(buf,"Status:%-16s{x  Level: %d\n\r", victim->immtitle != NULL ? victim->immtitle:"Mortal",victim->level);
send_to_char(buf, ch);
xprintf(buf,"Kingdom:   %-16s{x  Rank:  %s{x\n\r", kingdom_table[victim->kingdom].short_name,kingdom_table[victim->kingdom].rank_titles[victim->kingdom_rank]);
send_to_char(buf, ch);
xprintf(buf,"Class:     %-16s{x  Race:  %s{x\n\r",class_table[victim->class].name,race_table[victim->race].name);
send_to_char(buf, ch);
send_to_char("---------------------------------------------------------------\n\r", ch);
xprintf(buf,"Glory Points:    %4d  Roleplay Tokens:    %4d\n\r",victim->quest_accum,victim->rptoken);
send_to_char(buf, ch);
xprintf(buf,"Death by mobils: %4d  Death by players:  %4d\n\r",ch->pcdata->score[MOB_DEATH],ch->pcdata->score[PLAYER_DEATH]);
send_to_char(buf, ch);
xprintf(buf,"Mobils killed:   %4d  Players killed:     %4d\n\r",ch->pcdata->score[KILLED_MOB],ch->pcdata->score[PLAYER_KILL]);
send_to_char(buf, ch);
xprintf(buf,"Total Time online: %d\n\r",(victim->played + (int) (current_time - victim->logon) ) / 3600);
send_to_char(buf, ch);
send_to_char("---------------------------------------------------------------\n\r", ch);
send_to_char("                 *  Won * Lost * Killed * Total Levels\n\r", ch);
xprintf(buf,"Full Circle:     *%4d  *%4d  * %4d   *     %4d *\n\r",victim->pcdata->score[FULL_WIN],victim->pcdata->score[FULL_LOSS],victim->pcdata->score[FULL_KILL],victim->pcdata->score[FULL_LEVEL]);
send_to_char(buf, ch);                                                                         
xprintf(buf,"Kingdoms:        *%4d  *%4d  * %4d   *     %4d *\n\r",victim->pcdata->score[KINGDOM_WIN],victim->pcdata->score[KINGDOM_LOSS],victim->pcdata->score[KINGDOM_KILL],victim->pcdata->score[KINGDOM_LEVEL]);
send_to_char(buf, ch);                                                                         
xprintf(buf,"Races:           *%4d  *%4d  * %4d   *     %4d *\n\r",victim->pcdata->score[RACE_WIN],victim->pcdata->score[RACE_LOSS],victim->pcdata->score[RACE_KILL],victim->pcdata->score[RACE_LEVEL]);
send_to_char(buf, ch);                                                                         
xprintf(buf,"classes:         *%4d  *%4d  * %4d   *     %4d *\n\r",victim->pcdata->score[CLASS_WIN],victim->pcdata->score[CLASS_LOSS],victim->pcdata->score[CLASS_KILL],victim->pcdata->score[CLASS_LEVEL]);
send_to_char(buf, ch);                                                                         
xprintf(buf,"Good/Evil:       *%4d  *%4d  * %4d   *     %4d *\n\r",victim->pcdata->score[ALIG_WIN],victim->pcdata->score[ALIG_LOSS],victim->pcdata->score[ALIG_KILL],victim->pcdata->score[ALIG_LEVEL]);
send_to_char(buf, ch);                                                                         
xprintf(buf,"Team Battle:     *%4d  *%4d  * %4d   *     %4d *\n\r",victim->pcdata->score[TEAM_WIN],victim->pcdata->score[TEAM_LOSS],victim->pcdata->score[TEAM_KILL],victim->pcdata->score[TEAM_LEVEL]);
send_to_char(buf, ch);                                                                         
xprintf(buf,"Dual:            *%4d  *%4d  * %4d   *     %4d *\n\r",victim->pcdata->score[DUEL_WIN],victim->pcdata->score[DUEL_LOSS],victim->pcdata->score[DUEL_KILL],victim->pcdata->score[DUEL_LEVEL]);
send_to_char(buf, ch);
send_to_char("---------------------------------------------------------------\n\r", ch);

if (ch->level >= 87)
	{
    		s = current_time - victim->laston;
    		d = s/86400;
    		s-=d*86400;
    		h = s/3600;
    		s -= h*3600;
    		m = s/60;
    		s -= m*60;
		xprintf(buf,"%s{x last logged on at %s{x\n\r",victim->name,laston_found ? ctime(&victim->laston) : "Not found.");
		send_to_char(buf, ch);
		xprintf(buf,"That was %d days, %d hours, %d minutes and %d seconds ago.\n\r",d, h, m, s);
		send_to_char(buf, ch);
		if (isnoline)
		  {
			xprintf(buf,"%s is currently playing.\n\r",victim->name);
			send_to_char(buf, ch);
		  }

		xprintf(buf, "%s{x last logged on from %s{x\n\r",victim->name,victim->pcdata->host);
		send_to_char(buf, ch);
	}
send_to_char("---------------------------------------------------------------\n\r", ch);
}
