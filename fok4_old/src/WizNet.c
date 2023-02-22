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
				Wiznet
****************************************************************************/
#include <sys/types.h> 
#include <sys/time.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <time.h>
#include "merc.h"  
#include "interp.h"

/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           "{C[{BWIZNET{C]{x ",	WIZ_ON,         IM },
   {    "prefix",	"{C[{GGAME{C]{x ",	WIZ_PREFIX,	IM },
   {    "ticks",        "{C[{RSYSTEM{C]{x ",	WIZ_TICKS,      L2 },
   {    "logins",       "{C[{RLOGIN{C]{x ",	WIZ_LOGINS,     IM },
   {    "sites",        "{C[{RLOGIN{C]{x ",	WIZ_SITES,      L4 },
   {    "links",        "{C[{RSYSTEM{C]{x ",	WIZ_LINKS,      L7 },
   {	"newbies",	"{C[{RLOGIN{C]{x ",	WIZ_NEWBIE,	IM },
   {	"spam",		"{C[{RSYSTEM{C]{x ",	WIZ_SPAM,	L5 },
   {    "deaths",       "{C[{GGAME{C]{x ",	WIZ_DEATHS,     L2 },
   {    "resets",       "{C[{RSYSTEM{C]{x ",	WIZ_RESETS,     L4 },
   {    "mobdeaths",    "{C[{GGAME{C]{x ",	WIZ_MOBDEATHS,  L4 },
   {    "flags",	"{C[{RSYSTEM{C]{x ",	WIZ_FLAGS,	L5 },
   {	"penalties",	"{C[{GSYSTEM{C]{x ",	WIZ_PENALTIES,	L5 },
   {	"saccing",	"{C[{GGAME{C]{x ",	WIZ_SACCING,	L5 },
   {	"levels",	"{C[{GGAME{C]{x ",	WIZ_LEVELS,	IM }, 
   {	"load",		"{C[{GSYSTEM{C]{x ",	WIZ_LOAD,	L2 },
   {	"restore",	"{C[{GSYSTEM{C]{x ",	WIZ_RESTORE,	L2 },
   {	"snoops",	"{C[{GSYSTEM{C]{x ",	WIZ_SNOOPS,	L2 },
   {	"switches",	"{C[{GSYSTEM{C]{x ",	WIZ_SWITCHES,	L2 },
   {	"secure",	"{C[{GSYSTEM{C]{x ",	WIZ_SECURE,	L1 },
   {	"quests",	"{C[{GSYSTEM{C]{x ",	WIZ_QUESTS,	IM },
   {    "Password",     "{C[{RLOGIN{C]{x ",	WIZ_PASSWORD,   L5 },
   {    "delete",       "{C[{GSYSTEM{C]{x ",	WIZ_DELETE,   L5 },
   {    "cheat",       "{C[{GSYSTEM{C]{x ",	WIZ_CHEAT,   L5 },
   {    "rp",          "{C[{GGAMEE{C]{x ",	WIZ_RP,   L5 },   
   {    "prog",        "{C[{RSYSTEM{C]{x ",	WIZ_PROG,   L5 },      
   
   {	NULL,		"", 0,		0  }
};

void do_wiznet( CHAR_DATA *ch, char *argument )
  {
	int flag;
	char buf[MAX_STRING_LENGTH];
	char flagz[MAX_STRING_LENGTH];
	int flagw;
                
	if ( argument[0] == '\0' )
	// Show wiznet options - just like channel command
   	  {
		if (!IS_SET(ch->wiznet,WIZ_ON))
       		   {
			send_to_char("Wiznet is currently off.\n\r",ch);
			send_to_char("Type wiznet 'on' to turn wiznet on.\n\r",ch);
			return;
		  }
	        else
		  {
			send_to_char("{C--------------------------------------------------------------------{x\n\r",ch);
			send_to_char("{C                Wiznet is currently on.{x\n\r", ch);
			send_to_char("{C--------------------------------------------------------------------{x\n\r",ch);

		        // List of all wiznet options */
		        // Reset Buf
		        buf[0] = '\0';

		        flagw = 0;        
		        for (flag = 0; wiznet_table[flag].name != NULL; flag++)
			 {
				if (wiznet_table[flag].level <= get_trust(ch))
				  {
					if (flagw == 6)
					  {
	                			send_to_char("\n\r",ch);
						flagw = 0;
					  } 
					flagw += 1;

					if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
						sprintf(flagz, "{C%-10s{x ",wiznet_table[flag].name);
					else
						sprintf(flagz, "{c%-10s{x ",wiznet_table[flag].name);
			
					send_to_char(flagz, ch);   
				  }
			 }
			send_to_char("{C--------------------------------------------------------------------{x\n\r",ch);
		  }
     		return;
	  }    
 
	if (!str_prefix(argument,"on"))
	  {     
		send_to_char("{cWelcome to Wiznet!{x\n\r",ch);
		SET_BIT(ch->wiznet,WIZ_ON);
		return;
	  }
                
	if (!str_prefix(argument,"off"))
	  {
		send_to_char("{cSigning off of Wiznet.{x\n\r",ch);
		REMOVE_BIT(ch->wiznet,WIZ_ON);
		return;
	  }
   
	if (!str_prefix(argument,"all"))
	  {
		if (!IS_SET(ch->wiznet,WIZ_ON))
			SET_BIT(ch->wiznet,WIZ_ON);

		buf[0] = '\0';
		for (flag = 0; wiznet_table[flag].name != NULL; flag++)
		  {
			if (wiznet_table[flag].level <= get_trust(ch))
			  {
				sprintf(buf,"{cYou will now see {C%s{c on wiznet.{x\n\r",
				wiznet_table[flag].name);
				send_to_char(buf,ch);
				SET_BIT(ch->wiznet,wiznet_table[flag].flag);
			  }
		  }
		strcat(buf,"\n\r");
		send_to_char("{cThe following options are now enabled{x\n\r", ch);
		return;
	  }
        
	flag = wiznet_lookup(argument);
        
	if (flag == -1 || get_trust(ch) < wiznet_table[flag].level) 
	  {
		send_to_char("{cNo such WizNet option.{x\n\r",ch);
		return;
	  }

	if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
	  {
		sprintf(buf,"{cYou will no longer see {C%s{c on wiznet.{x\n\r",wiznet_table[flag].name);
		send_to_char(buf,ch);
		REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
		return;
	  }
	else  
	  {
		sprintf(buf,"{cYou will now see {c%s{C on wiznet.{x\n\r",wiznet_table[flag].name);
		send_to_char(buf,ch);
		SET_BIT(ch->wiznet,wiznet_table[flag].flag);
		return;
	  }
  }


void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj,
	    long flag, long flag_skip, int min_level) 
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int cflag;
    
/*    cflag = flag;*/

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->connected == CON_PLAYING
	&&  IS_IMMORTAL(d->character) 
	&&  IS_SET(d->character->wiznet,WIZ_ON) 
	&&  (!flag || IS_SET(d->character->wiznet,flag))
	&&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
	&&  get_trust(d->character) >= min_level
	&&  d->character != ch)
        {
           /* if (IS_SET(d->character->wiznet,WIZ_PREFIX))
                send_to_char("{B[WIZNET]{x{R ",d->character);
            else*/
/*                send_to_char( "{Z", d->character );*/
          
          for (cflag = 0; wiznet_table[cflag].name != NULL; cflag++)  
	   {
	    if (wiznet_table[cflag].flag == flag)
	    {
            sprintf(buf,"%s{x",wiznet_table[cflag].control);
            send_to_char(buf, d->character );
            send_to_char( "{C", d->character );
            act_new(string,d->character,obj,ch,TO_CHAR,POS_DEAD);
	    send_to_char( "{x", d->character );
	    }
	   }
        }
    }
 
    return;
}
