/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/****************************************************************************
				Log
****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "Log.h"

FILE                            *MainLog;

void Inet(CHAR_DATA *ch, char *LogLine,int Kingdom,int Inet)
{
	DESCRIPTOR_DATA *d;
	char buffer[MAX_STRING_LENGTH];
	if (Inet & INET_ARENA)
	  {
		sprintf(buffer,"{R[{YARENA{R]: %s",LogLine);
		for ( d = descriptor_list; d != NULL; d = d->next )
		  {
			CHAR_DATA *victim;
			victim = d->original ? d->original : d->character;
			if ( d->connected == CON_PLAYING)
				send_to_char(buffer,victim);			

		 }
	  }
               
}

void Log_system (CHAR_DATA *ch, char *LogLine,int Log,int Kingdom,int InetFlag)
{
	char LogNameAndTime[MAX_STRING_LENGTH]; 
	char buffer[MAX_STRING_LENGTH];
	char LogFile[MAX_STRING_LENGTH];
  	time_t curtime;
  	struct tm *loctime;
	char *strtime; 

	strtime = (char *) malloc(100);
	strtime = ctime( &current_time);
	strtime[strlen(strtime) -1] = '\0';

	if (Log & LOG_INET)
		Inet(ch,LogLine,Kingdom,InetFlag);

	if (ch != NULL)
		sprintf(LogNameAndTime,"%s :[%s]: %s\n", strtime,ch->name,LogLine);
	else
		sprintf(LogNameAndTime,"%s :[System]: %s\n",strtime,LogLine);


	curtime = time (NULL);
	loctime = localtime (&curtime);
  	strftime (buffer, MAX_STRING_LENGTH, "%F", loctime);



		
	if (Log & LOG_ROLEPLAY_SAY)
	  {
		sprintf( LogFile, "../../../logs/say/%s.log",buffer);		
	  }
	if (Log & LOG_ROLEPLAY_GOSSIP)
	  {
		sprintf( LogFile, "../../../logs/gossip/%s.log",buffer);
	  }

	if (Log & LOG_BUG)
	  {
	    sprintf( LogFile, "../../../logs/bug/%s.log",buffer);
	  }
	  
       if ( ( MainLog = fopen(LogFile,"a+")) == NULL )
          {
            bug("*BUG* File: Log.c Function: Log Error: Cannot open log file",0);
            return;
          }



	fprintf(MainLog,LogNameAndTime);
	fclose(MainLog);


/*	  
	if (Log & LOG_KINGDOM)
	if (Log & LOG_SYSTEM) 
	if (Log & LOG_LOGIN)
	if (Log & LOG_BOOT)
	if (Log & LOG_PUNISH)
	if (Log & LOG_DEATH)
	if (Log & LOG_GAIN)
	if (Log & LOG_LEVEL)
	if (Log & LOG_MOBIL_PROGRAM)
	if (Log & LOG_OLC)
	if (Log & LOG_PLAYERKILLING)
	if (Log & LOG_HIGHRISK)
	if (Log & LOG_SYSTEM)

	if (Log & LOG_TRACE)
*/

}

const   struct info_type info_table[] =
  {

/*      NAME                    FLAG              LEVEL */

   {    "on",           INFO_ON,        0  },
/*   {    "levels",       INFO_LEVELS,    84  },
   {    "consent",      INFO_CONSENT,   84 }, */
   {	"arena",	INFO_ARENA,	0  },
   {	"ftag",		INFO_FTAG,	0  },
   {    NULL,           0,              0  }
   };

void do_info( CHAR_DATA *ch, char *argument )
{
    int flag;
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
        if (IS_SET(ch->info,INFO_ON))
        {
            send_to_char("You will no longer see info messages.\n\r",ch);
            REMOVE_BIT(ch->info,INFO_ON);
        }
        else
        {
            send_to_char("You will now see info messages.\n\r",ch);
            SET_BIT(ch->info,INFO_ON);
        }
        return;
    }

    if (!str_prefix(argument,"on"))
    {
        send_to_char("You will now see info messages.\n\r",ch);
      SET_BIT(ch->info,INFO_ON);
        return;
    }

    if (!str_prefix(argument,"off"))
    {
        send_to_char("You will no longer see info messages.\n\r",ch);
        REMOVE_BIT(ch->info,INFO_ON);
        return;
    }

        /* Right, thats the basic on off switch done, now for the options!*/

    /* show info status */
    if (!str_prefix(argument,"status"))
    {
        buf[0] = '\0';

        if (!IS_SET(ch->info,INFO_ON))
            strcat(buf,"off ");

        for (flag = 0; info_table[flag].name != NULL; flag++) 
            if (IS_SET(ch->info,info_table[flag].flag))
            {
                strcat(buf,info_table[flag].name);              
                strcat(buf," ");
            }

        strcat(buf,"\n\r");

        send_to_char("You receive the following items of information:\n\r",ch);
        send_to_char(buf,ch);
        return;
    }

/* Now to see what options are available to a player... */

    if (!str_prefix(argument,"show"))
    /* list of all info options */
    {
        buf[0] = '\0';

        for (flag = 0; info_table[flag].name != NULL; flag++) 
        {
            if (info_table[flag].level <= get_trust(ch)) 
            {
                strcat(buf,info_table[flag].name);
                strcat(buf," ");
            }
        }

    strcat(buf,"\n\r");

        send_to_char("The following info options are available to you:\n\r",ch);
        send_to_char(buf,ch);
        return;
    }

/* Toggle an option! */

    flag = info_lookup(argument);

    if (flag == -1 || get_trust(ch) < info_table[flag].level)
    {
        send_to_char("No such option.\n\r",ch);
        return;
    }

    if (IS_SET(ch->info,info_table[flag].flag))
    {
        sprintf(buf,"You will no longer see %s on the info channel.\n\r",
                info_table[flag].name);
        send_to_char(buf,ch);
        REMOVE_BIT(ch->info,info_table[flag].flag);
        return;
    }
    else
    {
        sprintf(buf,"You will now see %s on the info channel.\n\r",
                        info_table[flag].name);
        send_to_char(buf,ch);
    SET_BIT(ch->info,info_table[flag].flag);
        return;
    }

}

/* Voltecs Info Channel */
void info(char *string, CHAR_DATA *ch, OBJ_DATA *obj,
                     long flag, long flag_skip, int min_level)
{
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->connected == CON_PLAYING
        &&  IS_SET(d->character->info, INFO_ON)
        &&  (!flag || IS_SET(d->character->info,flag))
        &&  (!flag_skip || !IS_SET(d->character->info,flag_skip))
        &&  get_trust(d->character) >= min_level
        &&  d->character != ch)
        {
//                send_to_char("{mINFO:{x ",d->character);
            act_new(string,d->character,obj,ch,TO_CHAR,POS_DEAD);
        }
    }
    return;
}

/* returns a flag for Voltecs info channel */
long info_lookup (const char *name)
{
    int flag;

    for (flag = 0; info_table[flag].name != NULL; flag++)
    {
        if (LOWER(name[0]) == LOWER(info_table[flag].name[0])
        && !str_prefix(name,info_table[flag].name))
            return flag;
    }

    return -1;
}