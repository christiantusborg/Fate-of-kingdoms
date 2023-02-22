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
				Arena
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

const	struct	arena_kind_type	arena_kind_table[] =
{
	{"full",   "Full Circle Arena",      ARENA_FULL,	100,100,25},
	{"kingdom","Battle of the Kingdoms", ARENA_KINGDOM,	100,100,25},
	{"race",   "Battle of the Races",    ARENA_RACE,	100,100,25},
	{"class",  "Battle of the Classes",  ARENA_CLASS,	100,100,25},
	{"align",  "Battle of Good and Evil",ARENA_ALIGNMENT,   100,100,25},
	{"team",   "Team Battle",            ARENA_TEAM,	100,100,25},
	{"dual",   "One vs. One Dual",       ARENA_DUEL,	100,100,25},
	{ "",	   "",	                        0,                0,0, 0}
};

const	struct	arena_zone_type	arena_zone_table[] =
{
	{"full",   "Full Circle Arena",11305,11353},
	{"bloodbath","Total bloodbath",11377,11377},
	{"town","Town of Kalrukuz",11355,11375},

	{ "",	   "",	                  0,  0}
};

void arena_main(CHAR_DATA *ch, char *argument)
{
  char cmd[MAX_INPUT_LENGTH];
  
  
	if (argument[0] == '\0')
	  {
		send_to_char("Syntax:\n\r",ch);
		send_to_char("  arena credit\n\r",ch);
		send_to_char("  arena start <arena type> <arena zone>\n\r",ch);
		send_to_char("  arena start <arena type> <arena zone> <low level> <high level>\n\r",ch);
		send_to_char("  arena join\n\r",ch);
		send_to_char("  arena who\n\r",ch);
		send_to_char("  arena out\n\r",ch);
		if (IS_IMMORTAL(ch))
		send_to_char("  arena reset <ticks>\n\r",ch);
		send_to_char("Arena type: full, kingdom, race, class, team \n\r",ch);
		send_to_char("Arena zone: full, bloodbath, town\n\r",ch);
		return;
	  }
	argument = one_argument(argument, cmd);
	if ( !str_cmp( cmd, "start" ))
		arena_begin(ch,argument);
	else if  ( !str_cmp( cmd, "out" ))
		do_arena_out(ch,"");
	else if  ( !str_cmp( cmd, "who" ))
		arena_who(ch,"");
	else if  ( !str_cmp( cmd, "join" ))
		arena_join(ch,"");
	else if  ( !str_cmp( cmd, "credit" ))
		arena_credit(ch,"");
	else if ( !str_cmp( cmd, "reset" ) && IS_IMMORTAL(ch)) 
		arena_init(ch,argument);
}

void arena_credit(CHAR_DATA *ch, char *argument)
{
	send_to_char("-----------------------------------------------------\n\r", ch);
	send_to_char(" - Original code By Christian Tusborg (Kazmir)      -\n\r", ch);
	send_to_char(" - Created for Fate of Kingdom 4.0                  -\n\r", ch);
	send_to_char(" - Bug reporting fok-bug@tusborg.dk                 -\n\r", ch);
	send_to_char(" - Text By Jens Roland (Scarpia)                    -\n\r", ch);
	send_to_char("-----------------------------------------------------\n\r", ch);
  return;
}

void arena_begin(CHAR_DATA *ch, char *argument)
{
	char strArenaType[MAX_INPUT_LENGTH];
	char strArenaLowLevel[MAX_INPUT_LENGTH];
	char strArenaHighLevel[MAX_INPUT_LENGTH];
	char strArenaStartText[MAX_INPUT_LENGTH];
	char strArenaZone[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	int  intArenaType = -1;
	int  ArenaStartCost = 100;
        int  intArenaTypeCount = 0;
	//There is running an arena
	if (ArenaStarted == TRUE)
	  {
		send_to_char("There is already combat going on in the arena!\n\r", ch);
		return;
	  }


	//Can we start it now or do we have to wait for some time befor we start new one
	if (ArenaNextStart > 0)
	  {
		sprintf(buf,"{WArena janitor tells you 'Give me %d minutes to clean up.'{x\n\r",ArenaNextStart);
    		send_to_char(buf,ch);
    		return;
	  }
	
	//How much do it cost to start the arena
        argument = one_argument(argument,strArenaType);
        
	intArenaType = -1;
	ArenaZone    = -1;
	for ( intArenaTypeCount = 0; arena_kind_table[intArenaTypeCount].bit != 0; intArenaTypeCount++ )
	 {

		if ( !str_cmp(arena_kind_table[intArenaTypeCount].shortname,strArenaType))
		  {
		    intArenaType    = arena_kind_table[intArenaTypeCount].bit;
		    ArenaStartCost  = arena_kind_table[intArenaTypeCount].coststart;
		    ArenaJoinCost  = arena_kind_table[intArenaTypeCount].costjoin;
		  }
	 }

	if (ArenaStartCost > (ch->gold * 100 + ch->silver))
	  {
	  	send_to_char("You do not have enough gold..\n\r", ch);
		return;
	  }

	deduct_cost(ch,ArenaStartCost);
	ArenaPool += ArenaStartCost;
	ArenaHowManyIn = 0;
	
	//Spilt argument
	argument = one_argument(argument,strArenaZone);

	for ( intArenaTypeCount = 0; arena_zone_table[intArenaTypeCount].low_vnum != 0; intArenaTypeCount++ )
	 {
		if ( !str_cmp(arena_zone_table[intArenaTypeCount].shortname,strArenaZone))
		  {

		    ArenaLowVnum  = arena_zone_table[intArenaTypeCount].low_vnum;
		    ArenaMaxVnum  = arena_zone_table[intArenaTypeCount].max_vnum;
		    ArenaZone     = intArenaTypeCount;

		  }
	 }

        if ( (ArenaZone == -1) || (intArenaType == -1) )
	  {
	  	send_to_char("Arena Zone or Type is wrong\n\r", ch);
		return;
	  }
        
	argument = one_argument(argument,strArenaLowLevel);
	argument = one_argument(argument,strArenaHighLevel);
	if ( strArenaHighLevel[0] != '\0' ) 
	  {
			//Right level argument

			if (atoi(strArenaLowLevel) <= 1 && atoi(strArenaHighLevel) >= 81)
			  {
      				send_to_char("The level has to be between 1 and 81.\n\r", ch);
    				return;
  			  }
  			//Set arena levels
  			ArenaLowLevel	= atoi(strArenaLowLevel);
  			ArenaHighLevel  = atoi(strArenaHighLevel);
	  }
	else
	 {
  			ArenaLowLevel	= 1;
  			ArenaHighLevel  = 81;
	 }

	//Select arena type
	switch ( intArenaType  )
	  {
		case ARENA_FULL:
			//Set Arena tpye and arena send text
			ArenaType = ARENA_FULL;
			sprintf(strArenaStartText,"{RFull Circle Arena Started by {c%s at %s. (%d to %d){x\n\r",ch->name,arena_zone_table[ArenaZone].longname,ArenaLowLevel,ArenaHighLevel);
		    break;
		case ARENA_KINGDOM:
			//Set Arena tpye and arena send text
			ArenaType = ARENA_KINGDOM;
			sprintf(strArenaStartText,"{RBattle of Kingdoms Started by {c%s at %s. (%d to %d){x\n\r",ch->name,arena_zone_table[ArenaZone].longname,ArenaLowLevel,ArenaHighLevel);
		    break;
		case ARENA_RACE:
			//Set Arena tpye and arena send text
			ArenaType = ARENA_RACE;
			sprintf(strArenaStartText,"{RBattle of the Races Arena Started by {c%s at %s. (%d to %d){x\n\r",ch->name,arena_zone_table[ArenaZone].longname,ArenaLowLevel,ArenaHighLevel);
		    break;
		case ARENA_CLASS:
			//Set Arena tpye and arena send text
			ArenaType = ARENA_CLASS;
			sprintf(strArenaStartText,"{RBattle of the Classes Arena Started by {c%s at %s. (%d to %d){x\n\r",ch->name,arena_zone_table[ArenaZone].longname,ArenaLowLevel,ArenaHighLevel);
		    break;
		case ARENA_ALIGNMENT:
			//Set Arena tpye and arena send text
			ArenaType = ARENA_ALIGNMENT;
			sprintf(strArenaStartText,"{RBattle of Good and Evil Arena Started by {c%s at %s. (%d to %d){x\n\r",ch->name,arena_zone_table[ArenaZone].longname,ArenaLowLevel,ArenaHighLevel);
		    break;
		case ARENA_TEAM:
			//Set Arena tpye and arena send text
			ArenaType = ARENA_TEAM;
			sprintf(strArenaStartText,"{RFull Team Arena Started by {c%s at %s. (%d to %d){x\n\r",ch->name,arena_zone_table[ArenaZone].longname,ArenaLowLevel,ArenaHighLevel);
		    break;		    		    		    
	        default:
			send_to_char("Selected arena is not a valid arena type\n\r",ch);
			return;
		    break;
	  }
	//Init global arena stuff
	Log_system (ch,strArenaStartText,LOG_INET,KNET_NULL,INET_ARENA);
//	ArenaZone	= 1;	
	ArenaTimeToRun	= 3;
	ArenaHowManyIn	= 0;
	ArenaStarted	= TRUE;

}

void arena_update(void)
{
	char strArenaSendText[MAX_INPUT_LENGTH];
	
	
	if (ArenaNextStart > 0)
	  ArenaNextStart--;
	//if we did get here and there is no arena going on

	if (!ArenaStarted)
		return;
		
	if (ArenaRunning)
		return;


	//Time to wait befor run
	if (ArenaTimeToRun > 0)
	  {
    		sprintf(strArenaSendText, "%d tick%s left to join the war.\n\r", ArenaTimeToRun,ArenaTimeToRun == 1 ? "" : "s");
      			Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
    		sprintf(strArenaSendText, "%d %s %s fighting in the war, so far.\n\r", ArenaHowManyIn, ArenaHowManyIn ==1 ? "person" : "people", ArenaHowManyIn == 1 ? "is" : "are");
      			Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
    		sprintf(strArenaSendText, "Type of war: %s\n\r",arena_kind_table[ArenaType -1].longname);
      			Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
    		sprintf(strArenaSendText, "The war is at: %s\n\r",arena_zone_table[ArenaZone].longname);
      			Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
    		sprintf(strArenaSendText, "And it for this levels: %d to %d\n\r",ArenaLowLevel,ArenaHighLevel);
      			Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
		ArenaTimeToRun--;
		return;
	  }

	//Run the arena or not?
	if (!arenacanrun())
	  {
		sprintf(strArenaSendText,"{RThe arena announcer has cancelled today's fight due to lack of participants.{x\n\r");
		Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
		arena_reset();
	  }
//	else
//		arena_start();

  return;
}

bool arenacanrun(void)
{
  DESCRIPTOR_DATA *d;
  bool blnKingdom = FALSE; 
  bool blnClass	  = FALSE; 
  bool blnRace	  = FALSE; 
  bool blnAlign	  = FALSE; 
  int LastKingdom = -1;
  int LastClass	  = -1;
  int LastRace	  = -1;	 
  int LastAlign	  = -1;
	
	//To few to do any thing
	if (ArenaHowManyIn < 2)
	  {
	   arena_reset();
	   return FALSE;
	  }

	//Looping to see who is in how of diffrent types
	for(d = descriptor_list; d != NULL; d = d->next)
	  {
		CHAR_DATA *vch;
		vch = d->original ? d->original : d->character;
		if ( d->connected == CON_PLAYING 
		 && !IS_NPC(vch)
		 && IS_SET(vch->act, PLR_ARENA))
		  {
			//can we run kingdom?
			if ( (LastKingdom != vch->kingdom) && (LastKingdom != -1) )
				blnKingdom = TRUE;

			//can we run class
			if ( (LastClass != vch->class) && (LastClass != -1) )
				blnClass = TRUE;
			LastClass = vch->class;
			//can we run race	
			if ( (LastRace != vch->race) && (LastRace != -1) )
				blnRace = TRUE;	
			LastRace = vch->race;
			//can we run  align
			if ( (LastAlign != vch->kingdom) && (LastAlign != -1) )
				blnAlign = TRUE;
			LastAlign = vch->kingdom;
		  }       
	  }               
	                  
	//do we reset arena or not?	
	switch ( ArenaType  )
	 {
		case ARENA_KINGDOM:
			if (!blnKingdom)
			  {
				arena_reset();
				return FALSE;
			  }
			arena_start();
		    break;
		case ARENA_RACE:
			if (!blnRace)
			  {
				arena_reset();
				return FALSE;
			  }
		      arena_start();
		    break;
		case ARENA_CLASS:
			if (!blnClass)
			  {
				arena_reset();
				return FALSE;
			  }
			arena_start();
		    break;
		case ARENA_ALIGNMENT:
			if (!blnAlign)
			  {
				arena_reset();
				return FALSE;
			  }
			arena_start();
		    break;
		case ARENA_TEAM:
			if (ArenaHowManyIn < 4)
	  		  {
	   			arena_reset();
	   			return FALSE;
	  		  }    		   
	  	        arena_start();
    		   break;
		case ARENA_FULL:
		
			arena_start();
			return TRUE;
		    break;
	        default:

		    break;
      }
return TRUE;
}




void arena_reset(void)
{
	DESCRIPTOR_DATA *d;

	ArenaZone	= -1;	
	ArenaTimeToRun	= -1;
	ArenaHowManyIn	= -1;
  	ArenaLowLevel	= -1;
  	ArenaHighLevel  = -1;
  	ArenaType	= -1;
	ArenaNextStart  = 1;
	ArenaStarted	= FALSE;
	ArenaRunning    = FALSE;


	//kick all out of arena!!!
	for(d = descriptor_list; d != NULL; d = d->next)
	  {
		CHAR_DATA *vch;
		vch = d->original ? d->original : d->character;
		if ( d->connected == CON_PLAYING 
		 && !IS_NPC(vch)
		 && IS_SET(vch->act, PLR_ARENA) )
		   {
		  	arena_out(vch,"");
		   }
	  }
}

void arena_win(void)
{
   DESCRIPTOR_DATA *d;
   int winnings = 0;

   //winnings = ArenaPool / ArenaHowManyIn;
   
	for(d = descriptor_list; d != NULL; d = d->next)
	  {
		CHAR_DATA *vch;
		vch = d->original ? d->original : d->character;
		if ( d->connected == CON_PLAYING 
		 && !IS_NPC(vch)
		 && IS_SET(vch->act, PLR_ARENA) )
		  {
			vch->gold += winnings;
	switch ( ArenaType  )
	  {
		case ARENA_KINGDOM:
			vch->pcdata->score[KINGDOM_WIN]++;
		    break;
		case ARENA_RACE:
			vch->pcdata->score[RACE_WIN]++;
		    break;
		case ARENA_CLASS:
			vch->pcdata->score[CLASS_WIN]++;
		    break;
		case ARENA_ALIGNMENT:
			vch->pcdata->score[ALIG_WIN]++;
		    break;
		case ARENA_TEAM:
			vch->pcdata->score[TEAM_WIN]++;
			break;		  
		case ARENA_FULL:
			vch->pcdata->score[FULL_WIN]++;
			break;		  
		case ARENA_DUEL:
			vch->pcdata->score[DUEL_WIN]++;
		    break;
	        default:

		    break;
		}	

			arena_out(vch,"");
		  }       
	  }
arena_init(NULL,"10");
}

void arena_start(void)
{
  char strArenaSendText[MAX_INPUT_LENGTH];	
  DESCRIPTOR_DATA *d;
  ROOM_INDEX_DATA *random;
  bool fRed = FALSE;
	//looping to find and do stuff to ppl who joined arena
	ArenaRunning	= TRUE;


	
	for(d = descriptor_list; d != NULL; d = d->next)
	  {
		CHAR_DATA *vch;
		vch = d->original ? d->original : d->character;
		if ( d->connected == CON_PLAYING && !IS_NPC(vch))
		if (IS_SET(vch->act, PLR_ARENA))
		  {

			if (ArenaType == ARENA_TEAM)
 			  {
          			if ((fRed = !fRed))
          			 {
          			  SET_BIT(vch->tag_flags,TAG_RED);
          			  send_to_char("You on {RRed{X team",vch);
          			 }
          			else
          			 {
          			  SET_BIT(vch->tag_flags,TAG_BLUE);
          			  send_to_char("You on {bBlue{x team",vch);
          			 }
			  }
			//get random arena room
			random = get_room_index(number_range(ArenaLowVnum,ArenaMaxVnum));
			//remove player from ready room
			char_from_room(vch);
			//set player in random room in arena
			char_to_room(vch, random);
			//make playrer look
			do_look(vch,"auto");
		  }
	  }
  sprintf(strArenaSendText,"{ROn the signal of the arena announcer, the mayhem commences.{x\n\r");
  Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
return;
}

bool arena_checkwin(void)
{
  DESCRIPTOR_DATA *d;
  bool blnKingdom = FALSE; 
  bool blnClass	  = FALSE; 
  bool blnRace	  = FALSE; 
  bool blnAlign	  = FALSE; 
  bool blnTeam	  = FALSE;  
  int LastKingdom = -1;
  int LastClass	  = -1;
  int LastRace	  = -1;	 
  int LastTeam	  = -1;
  int countin     =  0;
  
  char strArenaSendText[MAX_INPUT_LENGTH];	
  CHAR_DATA *vch = NULL;
  CHAR_DATA *victim = NULL;	
	//Yes if there is ONLY 1 playere in area he did win no matter type of arena

	//looping to see if we have a winner any way
	for(d = descriptor_list; d != NULL; d = d->next)
	  {
		vch = d->original ? d->original : d->character;
		if ( d->connected == CON_PLAYING 
		 && !IS_NPC(vch)
		 && IS_SET(vch->act, PLR_ARENA) )
		  {
		victim = vch;
		countin++;
	switch ( ArenaType  )
	  {
		case ARENA_KINGDOM:
			if ( (LastKingdom != vch->kingdom) && (LastKingdom != -1))
				blnKingdom = TRUE;
                        LastKingdom = vch->kingdom;
		    break;
		case ARENA_RACE:
			if ( (LastRace != vch->race) && (LastRace != -1))
				blnRace = TRUE;				
                        LastRace = vch->race;
		    break;
		case ARENA_CLASS:
			if ( (LastClass != vch->class) && (LastClass != -1) )
				blnClass = TRUE;
			LastClass = vch->class;
		    break;
		case ARENA_ALIGNMENT:
/*			if ( (LastAlign != vch->kingdom) && (LastAlign != -1))
				blnAlign = TRUE;
			LastAlign = vch->kingdom; */
		    break;
		case ARENA_TEAM:
          		if (IS_SET(vch->tag_flags,TAG_RED) )
          		 {
          			if ((LastTeam != 1) && (LastTeam != -1))
			           blnTeam = TRUE;
				LastTeam = 1;
			 }          		

          		if (IS_SET(vch->tag_flags,TAG_BLUE) )
          		 {
          			if ((LastTeam != 2) && (LastTeam != -1))
			           blnTeam = TRUE;
				LastTeam = 2;
			 }          		
			break;		  
		case ARENA_FULL:
		case ARENA_DUEL:
		    break;
	        default:

		    break;
		}    
		  }       
	  }               
	                  
	//Result if we have a winner
	switch ( ArenaType  )
	  {
		case ARENA_KINGDOM:
			if (!blnKingdom)
			  {
		  	  	sprintf(strArenaSendText,"{RThe arena announcer calls the victory of %s in the battle of Kingdoms! {x\n\r",kingdom_table[LastKingdom].name);
Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
			  	arena_win();
				return TRUE;
			  }
		    break;
		case ARENA_RACE:
			if (!blnRace)
			  {
		  	  	sprintf(strArenaSendText,"{RThe triumph of the %s is official in today's historical onslaught.{x\n\r",pc_race_table[LastRace].name);
Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
				arena_win();
				return TRUE;
			  }
		    break;
		case ARENA_CLASS:
			if (!blnClass)
			  {
		  	  	sprintf(strArenaSendText,"{RCarnage is brought to an end as the %ss stand proudly to see the sad demise of their foes. {x\n\r",class_table[LastClass].name);
Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
				arena_win();
				return TRUE;
			  }
		    break;
		case ARENA_ALIGNMENT:
			if (!blnAlign)
			  {
		  	  	sprintf(strArenaSendText,"{RFrom the bloodshed among the heroes of good and evil, %s emerges victoriously!{x\n\r",kingdom_table[LastKingdom].name);
Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
				arena_win();
				return TRUE;
			  }
		    break;

		case ARENA_TEAM:
			if (countin < 2)
			  {
		  	  	sprintf(strArenaSendText,"{RThe %s team wins the arena campaign.{x\n\r",(IS_SET(vch->tag_flags,TAG_RED) ? "Red" : "Blue"));
				Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
			  	arena_win();
				return TRUE;
			  }
			  
			break;		  
		case ARENA_FULL:
		case ARENA_DUEL:
			if (countin < 2)
			  {

		  	  	sprintf(strArenaSendText,"{RThe arena announcer salutes %s, the champion of the battlefield.{x\n\r",victim->name);
	//	  	  	sprintf(strArenaSendText,"{RJust minutes ago, %t perished humiliatingly to a fierce lethal strike delivered by %s.{x\n\r");
				Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
			  	arena_win();
				return TRUE;
			  }
		    break;
	        default:

		    break;
		}    
//No winner		    
return FALSE;
}


void arena_out(CHAR_DATA *ch, char *argument)
{
    //Mobile do get kicked out of arena they just die
    if (!IS_NPC(ch))
     {
	if (!IS_SET(ch->act, PLR_ARENA))
	  {
		send_to_char("Hello?! Look around - Guess what you not in arena!!.\n\r", ch);
		return;
	  }


        if (IS_SET(ch->tag_flags,TAG_BLUE))
       		REMOVE_BIT(ch->tag_flags,TAG_BLUE);
        if (IS_SET(ch->tag_flags,TAG_RED))
       		REMOVE_BIT(ch->tag_flags,TAG_RED);
    	//can we set player back to normal?
    	if (ch->pcdata->room_number != 0)
    	  {
    		//set playrer back to as befor
    		ch->hit		= ch->pcdata->arenahp;
    		ch->mana	= ch->pcdata->arenamp;
    		ch->move 	= ch->pcdata->arenamove;

		full_strip_affect(ch);		

    		ch->last_fight_time = -1;
    		ch->pking = 0;   
    	 	stop_fighting( ch, TRUE );
    	 	char_from_room(ch);
    	 	ch->hit = ch->max_hit;
    	 	update_pos( ch );
    		char_to_room(ch,get_room_index(ch->pcdata->room_number));
		REMOVE_BIT(ch->act, PLR_ARENA);
    	  }
    	else
    	  {
    	 	//Well we can realy do any here it kind a bug it they get in with out join
    	 	//But it can easy happen by immortal.
    		ch->last_fight_time = -1;
    		ch->pking = 0;   
    	 	stop_fighting( ch, TRUE );
    	 	full_strip_affect(ch);
    	 	send_to_char("*BUG* How did you get into arena????.\n\r",ch);	
    	 	char_from_room(ch);
    	 	char_to_room(ch, (get_room_index(ROOM_VNUM_ARENA_SAFE)));
    	 	ch->hit = ch->max_hit;
    	 	update_pos( ch );
    	 	REMOVE_BIT(ch->act, PLR_ARENA);

    	  }
     }	
   else
     {
     	//Die mobile!!!
     	raw_kill(NULL,ch);
     }
  return;
}

void arena_init(CHAR_DATA *ch, char *argument)
{
char cmd[MAX_INPUT_LENGTH];

if (ch != NULL)
{
	char strArenaSendText[MAX_INPUT_LENGTH];	
	sprintf(strArenaSendText,"{RThe arena janitor was subjected to divine intervention by %s.{x\n\r",ch->name);
	Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
	sprintf(strArenaSendText,"{RSand, dirt and rocks whirl around uncontrollably, eradicating all{x\n\r");
	Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
	//We simply reset arena
	arena_reset();
	//uhm this useless but we have to make something here
	
  	argument = one_argument(argument, cmd);
	if (atoi(cmd))
	 ArenaNextStart = atoi(cmd);
	else 
	 ArenaNextStart = 0;
	sprintf(strArenaSendText,"{Rtraces of battle. The battlefield will be sparkling in %d ticks.{x\n\r",ArenaNextStart);
	Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);

}
else
{
 arena_reset();
        argument = one_argument(argument, cmd);
        if (atoi(cmd))
         ArenaNextStart = atoi(cmd);
        else
         ArenaNextStart = 0;
}
  return;	
}

void arena_join(CHAR_DATA *ch, char *argument)
{
   
  ROOM_INDEX_DATA *location;
  char strArenaSendText[MAX_INPUT_LENGTH];	
	
	//Mobiles DO NOT ENTER. Its bad enough that players create them inside arena!!
	if(IS_NPC(ch))
	  {
		send_to_char("Sorry - You must be a REAL living person, in front of a computer (drinking Coke by the gallons), to play. NPC's Not Allowed!\n\r",ch);
		return;
	  }


	//Make sure we don't have players useing this to escaping pk
	if (ch->pking > 0)
	  {
		send_to_char("No way!! Why settle for fake combat when you've got the real deal right here?\n\r",ch);
		return;
	  }


	//Ups Some area file did go bad or arena not ccompleted
	if ((location = get_room_index(ARENA_WAR_ROOM)) == NULL)
	  {
		send_to_char("Arena is not yet completed, sorry.\n\r", ch);
		return;
	  }

	//You can ONLY join arena once
	if (IS_SET(ch->act, PLR_ARENA))
	  {
		send_to_char("Hello?! Look around - where do you think you ARE??.\n\r", ch);
		return;
	  }

	//We don't have an arena running
	if (ArenaStarted != TRUE) 
	  {
		send_to_char("There is currently no battle scheduled in the arena.\n\r", ch);
		return;
	  }


	//You can't join a running arena after we did start!
	if (ArenaRunning)
	  {
		send_to_char("You're too late, and the gates are closed. The fun has just begun without you.\n\r",ch);
		return;
	  }


	//Make sure you don't have players join if they are charmed
	if ( IS_AFFECTED(ch, AFF_CHARM)
	 &&   ch->master != NULL
	 &&   ch->in_room == ch->master->in_room )
	  {
		send_to_char( "{CWhat?  And leave your beloved master?{x\n\r",ch);
		return;
	  }

  	if ( (ch->level < ArenaLowLevel) || (ch->level > ArenaHighLevel))
	  {
		send_to_char( "{CNaa this arena war is not for your level!!!{x\n\r",ch);
		return;
	  }	
	
	if ( (ArenaType == ARENA_KINGDOM) && (ch->kingdom == 0))
	  {
		send_to_char( "{CWake up, this for kingdoms and you not in one!!!{x\n\r",ch);
		return;
	  }
	act("{R$n goes to the battlefield of war!.", ch, NULL, NULL,TO_ROOM);

	//Send them to ready room
	ch->pcdata->room_number =(ch->in_room == NULL ? 3001 : ch->in_room->vnum);
	char_from_room(ch);
	char_to_room(ch, location);
	SET_BIT(ch->act, PLR_ARENA);
	sprintf(strArenaSendText,"%s joins the blood bath!.{x\n\r",ch->name);
        Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
	act("$n arrives at the battlefield!", ch, NULL, NULL,TO_ROOM);
	//Add more players to arena
	ArenaHowManyIn++;
	//Set player arena data to make sure we don't abuse it to cure stuff
	ch->pcdata->arenahp = ch->hit;
	ch->pcdata->arenamp = ch->mana;
	ch->pcdata->arenamove = ch->move;
   	ch->hit		= ch->max_hit;
    	ch->mana	= ch->max_mana;
    	ch->move 	= ch->max_move;
	ch->pcdata->arenaaffected = ch->affected;
	ch->pcdata->arenaaffected_by = ch->affected_by;
	ch->pcdata->arenaaffected_by2 = ch->affected_by2;

	//Make the p look
	do_look(ch,"auto");
      return;


}

bool arena_kill(CHAR_DATA *ch,CHAR_DATA *victim)
{
char strArenaSendText[MAX_INPUT_LENGTH];	

  if (IS_NPC(victim))
	return FALSE;
 
 if (!IS_SET(victim->act, PLR_ARENA))
   return FALSE;
 
 
 
	sprintf(strArenaSendText,"{R%s gets to play dead at the arena, when a devastating thrust by %s forces him/her to the ground.{x\n\r",victim->name,ch->name);
        Log_system (NULL,strArenaSendText,LOG_INET,KNET_NULL,INET_ARENA);
	
	arena_out(victim,"");
	
	switch ( ArenaType  )
	  {
		case ARENA_KINGDOM:
			ch->pcdata->score[KINGDOM_KILL]++;
			ch->pcdata->score[KINGDOM_LEVEL] += victim->level;
			victim->pcdata->score[KINGDOM_LOSS]++;
		    break;
		case ARENA_RACE:
			ch->pcdata->score[RACE_KILL]++;
			ch->pcdata->score[RACE_LEVEL] += victim->level;
			victim->pcdata->score[RACE_LOSS]++;
		    break;
		case ARENA_CLASS:
			ch->pcdata->score[CLASS_KILL]++;
			ch->pcdata->score[CLASS_LEVEL] += victim->level;
			victim->pcdata->score[CLASS_LOSS]++;
		    break;
		case ARENA_ALIGNMENT:
			ch->pcdata->score[ALIG_KILL]++;
			ch->pcdata->score[ALIG_LEVEL] += victim->level;
			victim->pcdata->score[ALIG_LOSS]++;
		    break;
		case ARENA_TEAM:
			ch->pcdata->score[TEAM_KILL]++;
			ch->pcdata->score[TEAM_LEVEL] += victim->level;
			victim->pcdata->score[TEAM_LOSS]++;
			break;		  
		case ARENA_FULL:
			ch->pcdata->score[FULL_KILL]++;
			ch->pcdata->score[FULL_LEVEL] += victim->level;
			victim->pcdata->score[FULL_LOSS]++;
			break;		  
		case ARENA_DUEL:
			ch->pcdata->score[DUEL_KILL]++;
			ch->pcdata->score[DUEL_LEVEL] += victim->level;
			victim->pcdata->score[DUEL_LOSS]++;
		    break;
	        default:

		    break;
		}	

	ArenaHowManyIn--;
	arena_checkwin();    	 	

 	
return TRUE;
}
void arena_who(CHAR_DATA *ch, char *argument)
{
 DESCRIPTOR_DATA *d;
 BUFFER *output;
 char Text[MAX_STRING_LENGTH];
 char Fighing[MAX_STRING_LENGTH];
 CHAR_DATA *victim;
 int hptemp;
 int matemp;
 int mvtemp;
 	output = new_buf();
 	sprintf( Text,"{c-----------------------------------------------------\n\r");
 	add_buf(output,Text);
 	sprintf( Text, "'{c-     Name       - Kills -    Race/Class -  Hp  -  Ma  -  Mv  - Position - Fighting -\n\r");
 	add_buf(output,Text);
	
	for(d = descriptor_list; d != NULL; d = d->next)
	  {
		CHAR_DATA *vch;
		vch = d->original ? d->original : d->character;
		if ( d->connected == CON_PLAYING 
		 && !IS_NPC(vch)
		 && IS_SET(vch->act, PLR_ARENA) )
		  {
 		  hptemp = vch->hit  * 100 /vch->max_hit;
 		  matemp = vch->mana * 100 /vch->max_mana;
 		  mvtemp = vch->move * 100 /vch->max_move;
 		  
 		  if (vch->fighting != NULL)
 		  	{
			 victim = vch->fighting;
			 sprintf( Fighing," %s",victim->name);
			}
 		  else
			sprintf( Fighing," %s","None");
 		  sprintf( Text, "{c%-14s %-5d %5s{x/%3s  %-5d  %-5d %-5d %-15s %-15s\n\r",
 		  vch->name,
 		  0,
 		  pc_race_table[vch->race].who_name,
 		  class_table[vch->class].who_name,
 		  hptemp,
 		  matemp,
 		  mvtemp,
 		  capitalize( position_table[vch->position].name),
		  Fighing);
 		  add_buf(output,Text);
 		 }
	  }
        page_to_char( buf_string(output), ch );
        free_buf(output);
        return;
}

void do_arena_out(CHAR_DATA *ch, char *argument)
{
	if(ch->in_room->vnum == 11300)
	 return;
	arena_out(ch, argument);
	ArenaHowManyIn--;
	arena_checkwin();
}