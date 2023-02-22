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
#include "tables.h"
#include "arena.h"
#include "Log.h"

DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN(	do_exits	);

/* command procedures needed */
void do_quit_count      args( ( CHAR_DATA *ch, char *argument ) );
void do_quit_org	args( (CHAR_DATA *ch, char *argument, bool Count, bool blnWiznet) );
char	*makedrunk	args( (char *string, CHAR_DATA *ch) );
/*
 * Drunk struct
 */
struct struckdrunk
{
        int     min_drunk_level;
        int     number_of_rep;
        char    *replacement[11];
};


char * makedrunk (char *string, CHAR_DATA * ch)
{
/* This structure defines all changes for a character */
  struct struckdrunk drunk[] =
  {
    {3, 10,
     {"a", "a", "a", "A", "aa", "ah", "Ah", "ao", "aw", "oa", "ahhhh"}},
    {8, 5,
     {"b", "b", "b", "B", "B", "vb"}},
    {3, 5,
     {"c", "c", "C", "cj", "sj", "zj"}},
    {5, 2,
     {"d", "d", "D"}},
    {3, 3,
     {"e", "e", "eh", "E"}},
    {4, 5,
     {"f", "f", "ff", "fff", "fFf", "F"}},
    {8, 2,
     {"g", "g", "G"}},
    {9, 6,
     {"h", "h", "hh", "hhh", "Hhh", "HhH", "H"}},
    {7, 6,
     {"i", "i", "Iii", "ii", "iI", "Ii", "I"}},
    {9, 5,
     {"j", "j", "jj", "Jj", "jJ", "J"}},
    {7, 2,
     {"k", "k", "K"}},
    {3, 2,
     {"l", "l", "L"}},
    {5, 8,
     {"m", "m", "mm", "mmm", "mmmm", "mmmmm", "MmM", "mM", "M"}},
    {6, 6,
     {"n", "n", "nn", "Nn", "nnn", "nNn", "N"}},
    {3, 6,
     {"o", "o", "ooo", "ao", "aOoo", "Ooo", "ooOo"}},
    {3, 2,
     {"p", "p", "P"}},
    {5, 5,
     {"q", "q", "Q", "ku", "ququ", "kukeleku"}},
    {4, 2,
     {"r", "r", "R"}},
    {2, 5,
     {"s", "ss", "zzZzssZ", "ZSssS", "sSzzsss", "sSss"}},
    {5, 2,
     {"t", "t", "T"}},
    {3, 6,
     {"u", "u", "uh", "Uh", "Uhuhhuh", "uhU", "uhhu"}},
    {4, 2,
     {"v", "v", "V"}},
    {4, 2,
     {"w", "w", "W"}},
    {5, 6,
     {"x", "x", "X", "ks", "iks", "kz", "xz"}},
    {3, 2,
     {"y", "y", "Y"}},
    {2, 9,
     {"z", "z", "ZzzZz", "Zzz", "Zsszzsz", "szz", "sZZz", "ZSz", "zZ", "Z"}}  };

  char buf[1024];
  char temp;
  int pos = 0;
  int drunklevel;
  int randomnum;

  /* Check how drunk a person is... */
  if (IS_NPC(ch))
        drunklevel = 0;
  else
        drunklevel = ch->pcdata->condition[COND_DRUNK];

  if (drunklevel > 0)
    {
      do
        {
          if ( pos >= MAX_STRING_LENGTH ) break;  
          temp = toupper (*string);
          if ((temp >= 'A') && (temp <= 'Z'))
            {
              if (drunklevel > drunk[temp - 'A'].min_drunk_level)
                {
                  randomnum = number_range (0, drunk[temp - 'A'].number_of_rep);
                  strcpy (&buf[pos], drunk[temp - 'A'].replacement[randomnum]);
                  pos += strlen (drunk[temp - 'A'].replacement[randomnum]);
                }
              else
                buf[pos++] = *string;
            }
          else
            {
              if ((temp >= '0') && (temp <= '9'))
                {
                  temp = '0' + number_range (0, 9);
                  buf[pos++] = temp;
                }
              else
                buf[pos++] = *string;
            }
        }
      while (*string++);
      buf[pos] = '\0';          /* Mark end of the string... */
      xcatf(string, buf);
      return(string);
    }
  return (string);
}

char *make_talk_hasted(char *argument)
{
char nbuf[MAX_STRING_LENGTH];
int opos,npos;

   for ( opos=0,npos=0 ; argument[opos]!='\0' ; opos++,npos++ ) 
   {
	if (!isspace(argument[opos]))
	    nbuf[npos]=argument[opos];
        else npos--;
   }
   nbuf[npos]='\0';

return(strdup(nbuf));
}


void do_delet( CHAR_DATA *ch, char *argument)
{
    send_to_char("{cYou must type the full command to delete yourself{x.\n\r",ch);
}

void do_delete( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];
    char pfile[MAX_STRING_LENGTH];
// FILE *fp;
// char *res;

   if (IS_NPC(ch))
	return;

   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
	    send_to_char("{cDelete status removed{x.\n\r",ch);
	    ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
    	    xprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
/*  makes it so ch's above lev 10 dont get to re use names */
/*
if (ch->level > 10)
{  
       fclose( fpReserve );
       if ( (fp = fopen( BADNAME_FILE , "a" ) ) != NULL )
       {
	 while ( !feof (fp) )
         {
                   res = fread_string( fp );
                   if (!str_cmp( res, ch->name ) )
                   break;

                   if ( res == NULL )
                   {
                   fprintf(fp,"%s\n",ch->name);
                   fpReserve = fopen( NULL_FILE, "r" );
                   fclose( fp );
                   break;
                   }
       }
}
       else
         bug( "Nanny: BADNAMES.TXT not found!", 0 );
       fpReserve = fopen( NULL_FILE, "w" );
       fclose( fp );
}
*/
/*  end */
	    if(ch->kingdom > 0) {
	    subtract_member(ch->name,ch->kingdom); }
	    wiznet("[$N was deleted]",ch,NULL,WIZ_DELETE,0,0);
	    do_quit_count(ch,"");
     
           if (ch->level > 10)
            {
             xprintf(pfile,"/home/fate/.ftpbak/ss/Backup/delete/%s.bak",capitalize( ch->name ) );
             rename(strsave,pfile);
            }
           else
	    unlink(strsave);
	    return;
 	}
    }

    if (argument[0] != '\0')
    {
	send_to_char("{cJust type delete. No argument{x.\n\r",ch);
	return;
    }

    send_to_char("{cType delete again to confirm this command{x.\n\r",ch);
    send_to_char("{cWARNING: this command is irreversible{x.\n\r",ch);
    send_to_char("{cTyping delete with an argument will undo delete status{x.\n\r",
	ch);
    ch->pcdata->confirm_delete = TRUE;
    wiznet("[$N types 'delete']",ch,NULL,WIZ_DELETE,0,0);
}


/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    /* lists all channels and their status */
    send_to_char("{C Channel      Status{x\n\r",ch);
    send_to_char("{C---------------------{x\n\r",ch);

    send_to_char("{cAuction        ",ch);
    if (!IS_SET(ch->comm,COMM_NOAUCTION))
      send_to_char("{cON{x\n\r",ch);
    else
      send_to_char("{cOFF{x\n\r",ch);
    send_to_char("{cGOSSIP	  ",ch);
    if (!IS_SET(ch->comm,COMM_NOGOSSIP))
	send_to_char("{cON{x\n\r",ch);
    else
	send_to_char("{cOFF{x\n\r",ch);

    send_to_char("{cOOC           ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUESTION))
      send_to_char("{cON{x\n\r",ch);
    else
      send_to_char("{cOFF{x\n\r",ch);

    send_to_char("{cQUEST           ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUEST))
      send_to_char("{cON{x\n\r",ch);
    else
      send_to_char("{cOFF{x\n\r",ch);

    send_to_char("{cMUSIC           ",ch);
    if (!IS_SET(ch->comm,COMM_NOMUSIC))
      send_to_char("{cON{x\n\r",ch);
    else
      send_to_char("{cOFF{x\n\r",ch);

    if((ch->level <= 10) || IS_IMMORTAL(ch)) {
    send_to_char("{cNEWBIE           ",ch);
    if (!IS_SET(ch->comm,COMM_NONEWBIE))
      send_to_char("{cON{x\n\r",ch);
    else
      send_to_char("{cOFF{x\n\r",ch);
	}

    if (IS_IMMORTAL(ch))
    {
      send_to_char("{cGod channel    ",ch);
      if(!IS_SET(ch->comm,COMM_NOWIZ))
        send_to_char("{cON{x\n\r",ch);
      else
        send_to_char("{cOFF{x\n\r",ch);
    }

    send_to_char("{cMessengerShouts",ch);
    if (!IS_SET(ch->comm,COMM_SHOUTSOFF))
      send_to_char("{cON{x\n\r",ch);
    else
      send_to_char("{cOFF{x\n\r",ch);

    send_to_char("{cTells          ",ch);

    if (!IS_SET(ch->comm,COMM_DEAF))
	send_to_char("{cON{x\n\r",ch);
    else
	send_to_char("{cOFF{x\n\r",ch);

/*
     send_to_char("arena            ",ch);

    if (!IS_SET(ch->comm,COMM_NOARENA))
      send_to_char("ON\n\r", ch);
    else
      send_to_char("OFF\n\r", ch);
*/
    send_to_char("{cQuiet mode     ",ch);
    if (IS_SET(ch->comm,COMM_QUIET))
      send_to_char("{cON{x\n\r",ch);
    else
      send_to_char("{cOFF{x\n\r",ch);
    send_to_char("Mud Sound Protocol	",ch);
    if (ch->msp == 1)
	send_to_char("{cON{x\n\r",ch);
     else
	send_to_char("{cOFF{x\n\r",ch);
     send_to_char("Virtual Terminal	",ch);
	if (ch->vt == 1)
	send_to_char("{cON{x\n\r",ch);
     else
	send_to_char("{cOFF{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_AFK))
	send_to_char("{YYou are AFK.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_SNOOP_PROOF))
	send_to_char("{YYou are immune to snooping.{x\n\r",ch);

    if (ch->lines != PAGELEN)
    {
	if (ch->lines)
	{
	    xprintf(buf,"{cYou display {Y%d{c lines of scroll.{x\n\r",ch->lines+2);
	    send_to_char(buf,ch);
 	}
	else
	    send_to_char("{cScroll buffering is off.{x\n\r",ch);
    }


    if (IS_SET(ch->comm,COMM_NOSHOUT))
      send_to_char("{YYou cannot shout.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOTELL))
      send_to_char("{YYou cannot use tell.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
     send_to_char("{YYou cannot use channels.{x\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOEMOTE))
      send_to_char("{YYou cannot show emotions.{x\n\r",ch);

}

void garble(char *garbled,char *speech)
{
  int i;

  for (i = 0; speech[i] != (char) NULL; i++) {
    if (speech[i] >= 'a' && speech[i] <= 'z')
      garbled[i] = 'a' + number_range(0,25);
    else if (speech[i] >= 'A' && speech[i] <= 'Z')
      garbled[i] = 'A' + number_range(0,25);
    else garbled[i] = speech[i];
  }
  garbled[i] = '\0';
}


/* RT deaf blocks out all shouts */

void do_deaf( CHAR_DATA *ch, char *argument)
{

   if (IS_SET(ch->comm,COMM_DEAF))
   {
     send_to_char("{cYou can hear again.{x\n\r",ch);
     REMOVE_BIT(ch->comm,COMM_DEAF);
   }
   else
   {
     send_to_char("{cFrom now on, you will ignore what people tell you.{x\n\r",ch);
     SET_BIT(ch->comm,COMM_DEAF);
   }
}

/* RT quiet blocks out all communication */

void do_quiet ( CHAR_DATA *ch, char * argument)
{
    if (IS_SET(ch->comm,COMM_QUIET))
    {
      send_to_char("{cQuiet mode removed.{x\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_QUIET);
    }
   else
   {
     send_to_char("{cFrom now on, you will only listen to others in the same room.{x\n\r",ch);
     SET_BIT(ch->comm,COMM_QUIET);
   }
}

void do_afk ( CHAR_DATA *ch, char * argument)
{
    if (IS_NPC(ch)) {
	send_to_char("Mob can't go afk.\n",ch);
	return;
	}

   if ((ch->pking > 0) && !IS_IMMORTAL(ch))
        {
        send_to_char("You are having way too much fun right now!!\n\r",ch);
        return;
        }

    if (IS_SET(ch->comm,COMM_AFK))
    {
      send_to_char("{cYou are no longer afk. Type 'replay' to see if you missed anything.{x\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_AFK);
    }
   else
   {

    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME)
      {
        send_to_char("{CYour adrenalin is gushing! You can't go afk yet.{x!\n\r",ch);
        return;
      }      

     send_to_char("{cYou quietly go afk.{x\n\r",ch);
     SET_BIT(ch->comm,COMM_AFK);
   }
}

void do_busy ( CHAR_DATA *ch, char * argument)
{
    if (IS_SET(ch->comm,COMM_BUSY))
    {
      send_to_char("{cYou are no longer so busy. Type 'replay' to see if you missed anything.{x\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BUSY);
    }
   else
   {
     send_to_char("{cYou quietly go into busy mode and start writing a note.{x\n\r",ch);
     SET_BIT(ch->comm,COMM_BUSY);
   }
}


void do_replay (CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
	send_to_char("{cIf you only had a brain..{x.\n\r",ch);
	return;
    }

    page_to_char(buf_string(ch->pcdata->buffer),ch);
    clear_buf(ch->pcdata->buffer);
}



void do_say( CHAR_DATA *ch,char *argument )
{
do_say_org(ch,argument,TRUE);
}
void do_say_org( CHAR_DATA *ch,char *argument ,bool ooc)
{

  char buf2[MAX_INPUT_LENGTH];
  CHAR_DATA *room_char;
  OBJ_DATA *char_obj;
  OBJ_DATA *obj, *obj_next;
  CHAR_DATA *vch;
  char buf[MAX_STRING_LENGTH];
  
  char log_buf[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA *location;
  
    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }
    if ( ch->in_room == NULL )  {
      send_to_char( "But, you are not in a room!\n\r", ch );
      return;
    }

    smash_percent(argument);

	if ( !IS_SET( ch->quest, QUEST_GOLEM ) && ch->level >= 55) 
	{
	
	xprintf(buf, "%s",argument);
		if (!str_cmp (buf, "sa'angreal") && ch->in_room->vnum == 6413 || ch->in_room->vnum == 6412)
		 {
		    
		    location = get_room_index (6440);
		     char_from_room( ch );
			 char_to_room( ch, location );

		 }
		
	}
	 if (!IS_NPC(ch))
	
		if (IS_AFFECTED(ch,AFF_HASTE) &&  ch->pcdata->condition[COND_DRUNK] <= 0)
      			argument=make_talk_hasted(argument);

	if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
		argument = makedrunk(argument,ch);


    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

if (!IS_NPC(ch))
 {
  if (ooc)
  {
   for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
     if (!is_affected(vch, gsn_deafen))
     act( "{g$n says '{G$t{g'{x{W[{GOOC{W]{X " ,ch,buf, vch,TO_VICT);
    }

   if (!is_affected(ch, gsn_deafen))
     act( "{gYou say '{G$T{g'{x{W[{GOOC{W]{X",ch, NULL,buf, TO_CHAR);

  }
 else
  {


   Log_system (ch,buf,LOG_ROLEPLAY_SAY,0,0);
   for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
     if (!is_affected(vch, gsn_deafen))
     act( "{g$n says '{G$t{g'{x" ,ch,buf, vch,TO_VICT);
    }

   if (!is_affected(ch, gsn_deafen))
     act( "{gYou say '{G$T{g'{x",ch, NULL,buf, TO_CHAR);
  
   xprintf(log_buf, "[%s] [%d] Roleplaying by %s: %s",(char *) ctime( &current_time ),ch->in_room->vnum,ch->name,buf);
   wiznet(log_buf,ch,NULL,WIZ_RP,0,0);
   xprintf(log_buf, "[%s] [%d](ICSAY): %s",ch->name,ch->in_room->vnum,buf);
   log_string(log_buf);
  }
}
else
 {
   for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
     if (!is_affected(vch, gsn_deafen))
     act( "{g$n says '{G$t{g'{x" ,ch,buf, vch,TO_VICT);
    }

   if (!is_affected(ch, gsn_deafen))
     act( "{gYou say '{G$T{g'{x",ch, NULL,buf, TO_CHAR);
 }
/*
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (!is_affected(vch, gsn_deafen))
          act( "{g$n says '{G$t{g'{x" ,ch,xtype, vch,TO_VICT);
    }

   if (!is_affected(ch, gsn_deafen))
     act( "{gYou say '{G$T{g'{x",ch, NULL,xtype, TO_CHAR);
*/
    if ( !IS_NPC(ch) )
    {
       CHAR_DATA *mob, *mob_next;
       for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
       {
           mob_next = mob->next_in_room;
           if ( IS_NPC(mob) && HAS_TRIGGER_MOB( mob, TRIG_SPEECH ) 
           &&   mob->position == mob->pIndexData->default_pos )
             //bug("Check",0);
             {
              p_act_trigger( argument, mob, NULL, NULL, ch, NULL, NULL, TRIG_SPEECH );
							return;
						}
	   

	    for ( obj = mob->carrying; obj; obj = obj_next )
	    {
				obj_next = obj->next_content;
				if ( HAS_TRIGGER_OBJ( obj, TRIG_SPEECH ) )
		    	p_act_trigger( argument, NULL, obj, NULL, ch, NULL, NULL, TRIG_SPEECH );
	    }
       }  
     }
   

  for (room_char = ch->in_room->people; room_char != NULL;
         room_char = room_char->next_in_room)
    {
      if (IS_SET(room_char->progtypes,MPROG_SPEECH) && room_char != ch) 
      {
        xprintf(buf2,"MPROG_SPEECH: Sayer: %s, Mob: %s",PERS(ch,ch),
                PERS(room_char,room_char));
/*        log_string(buf2);  */
        (room_char->pIndexData->moprogs->speech_prog) (room_char,ch,buf);
      }
    }

    for (char_obj = ch->carrying; char_obj != NULL;
       char_obj = char_obj->next_content)
    {
      if (IS_SET(char_obj->progtypes,IPROG_SPEECH))
        (char_obj->pIndexData->iprogs->speech_prog) (char_obj,ch,buf);
    }

    for (char_obj = ch->in_room->contents; char_obj != NULL;
       char_obj = char_obj->next_content)
    {
      if (IS_SET(char_obj->progtypes,IPROG_SPEECH))
        (char_obj->pIndexData->iprogs->speech_prog) (char_obj,ch,buf);
    }
    
	for ( obj = ch->in_room->contents; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( HAS_TRIGGER_OBJ( obj, TRIG_SPEECH ) )
		p_act_trigger( argument, NULL, obj, NULL, ch, NULL, NULL, TRIG_SPEECH );
	}
	
	if ( HAS_TRIGGER_ROOM( ch->in_room, TRIG_SPEECH ) )
	    p_act_trigger( argument, NULL, NULL, ch->in_room, ch, NULL, NULL, TRIG_SPEECH );
    
    return;
}

void do_new_say( CHAR_DATA *ch, char *argument )
{
do_new_say_org( ch, argument,TRUE);
}

void do_new_say_ooc( CHAR_DATA *ch, char *argument )
{
do_new_say_org( ch, argument,FALSE);
}

void do_new_say_org( CHAR_DATA *ch, char *argument,bool ooc)
{
  char *buf3;
  char command[MAX_INPUT_LENGTH];        
  char xtype[MAX_INPUT_LENGTH];        
  CHAR_DATA *vch;
  char buf[MAX_STRING_LENGTH];
  char newbuf[MAX_STRING_LENGTH];
  char log_buf[MAX_STRING_LENGTH];
  char *verb;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }
    if ( ch->in_room == NULL )  {
      send_to_char( "But, you are not in a room!\n\r", ch );
      return;
    }

/* Stupid ppl have problem saying stuff :P */
/*
if(get_curr_stat(ch,STAT_INT) <= 13) {

argument = string_replace( argument, "I","ME" );
argument = string_replace( argument, "i","me" );
argument = string_replace( argument, "my","me" );
argument = string_replace( argument, "My","Me" );
}
*/
if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
 if (IS_AFFECTED(ch,AFF_HASTE) && ch->pcdata->condition[COND_DRUNK] <=0)
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

verb="say";

   switch ( argument[strlen(argument)-1] )
   {
       default: verb = verb;     break;
      case '!': verb = "exclaim"; break;
      case '.': verb = "state"; break;
      case '?': verb = "ask";     break;
   }

   switch ( argument[strlen(argument)-2] )
   {
       default: verb = verb;     break;
      case '!': verb = "shriek"; break;
      case '.': verb = "mutter"; break;
      case '?': verb = "wonder";break;
   }

   switch ( argument[strlen(argument)-3] )
   {
       default: verb = verb;     break;
      case '!': verb = "nearly choke"; break;
      case '.': verb = "mutter"; break;
      case '?': verb = "nearly choke";break;
   }

if (strstr((argument), ":)" )) 	verb = "smile";
else if (strstr((argument), ":>" )) 	verb = "grin";
else if (strstr((argument), ":p" )) 	verb = "tease";
else if (strstr((argument), ":0" )) 	verb = "gawk";
else if (strstr((argument), "(:" )) 	verb = "smile";
else if (strstr((argument), "<:" )) 	verb = "grin";
else if (strstr((argument), ":P" )) 	verb = "tease";
else if (strstr((argument), "0:" )) 	verb = "gawk";
else if (strstr((argument), "):" )) 	verb = "frown";
else if (strstr((argument), ":(" )) 	verb = "frown";
else if (strstr((argument), ";)" )) 	verb = "wink";

else if (strstr((argument), "damn")) verb = "swear";
else if (strstr((argument), "Damn" )) 	verb = "swear";
else if (strstr((argument), "DAMN" )) 	verb = "swear";
else if (strstr((argument), "shit" )) 	verb = "swear";
else if (strstr((argument), "Shit" )) 	verb = "swear";
else if (strstr((argument), "SHIT" )) 	verb = "swear";
else if (strstr((argument), "fuck" )) 	verb = "swear";
else if (strstr((argument), "Fuck" )) 	verb = "swear";
else if (strstr((argument), "FUCK" )) 	verb = "swear";
else if (strstr((argument), "dick" )) 	verb = "swear";
else if (strstr((argument), "Dick" )) 	verb = "swear";
else if (strstr((argument), "DICK" )) 	verb = "swear";
else if (strstr((argument), "any spares" )) verb = "grovel";
else if (strstr((argument), "sorry" )) verb = "apologize";
else if (strstr((argument), "Sorry" )) verb = "apologize";
else if (strstr((argument), "hello" )) verb = "greet";
/*
else if (strstr((argument), "hi" )) verb = "greet";
*/
else if (strstr((argument), "shut up")) verb = "scowl";
else if (strstr((argument), "yes" )) verb = "agree";
else if (strstr((argument), "konnichiwa")) verb = "greet";
else if (strstr((argument), "gomennasai")) verb = "apologize";
else if (strstr((argument), "sayonara")) verb = "wave";
else if (strstr((argument), "ohayoo"))	verb = "greet";
else if (strstr((argument), "bye"))	verb = "wave";
else if (strstr((argument), "please"))  verb = "beg";
else if (strstr((argument), "waaah"))   verb = "cry";

///******
buf3 = str_dup(argument);
buf3 = one_argument( buf3, command );        


if (verb=="say")
{

do_say_org(ch,argument,ooc);
}
else
{
if (!IS_NPC(ch))
 {
  if (ooc)
  {
   xprintf(xtype, "{W[{GOOC{W]{X");
  }
 else
  {
   xprintf(xtype, "{X");
   xprintf(log_buf, "[%s] [%d] Roleplaying by %s: %s",(char *) ctime( &current_time ),ch->in_room->vnum,ch->name,buf);
   Log_system (ch,log_buf,LOG_ROLEPLAY_SAY,0,0);
   wiznet(log_buf,ch,NULL,WIZ_RP,0,0);
  }
 }
else
 {
 xprintf(xtype, " ");
 
 } 

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (!is_affected(vch, gsn_deafen)) {
          xprintf(newbuf, "{g$n %ss, '{G$t{g'%s{x", verb,xtype);
          act( newbuf, ch, buf, vch, TO_VICT);
	}
    }
   if (!is_affected(ch, gsn_deafen)) {
     xprintf(newbuf, "{gYou %s, '{G$T{g'{x%s", verb,xtype);
     act( newbuf, ch, NULL, buf, TO_CHAR);
	}
}

    return;
}

/*  :: new messenger shout */
void do_shout( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
    {
    send_to_char("{cMessengers do not work for the likes of you.{x\n\r",ch);
    return;
    }

    if ( (ch->in_room->sector_type != SECT_CITY)
	|| (IS_SET(ch->in_room->room_flags, ROOM_WILDERNESS)) )
    {
    send_to_char("{cYou can not call for messengers here.{x\n\r",ch);
    return;
    }

    if (strlen(argument) > 70) {
    send_to_char("{cThese cheap messengers could not possibly remember that much.{x\n\r",ch);
    argument[68]='.';
    argument[69]='.';
    argument[70]='.';
    argument[71]='\0';
    strcpy(argument,argument);
    }

    if (argument[0] == '\0' && !IS_NPC(ch) )
    {
      	if (IS_SET(ch->comm,COMM_SHOUTSOFF))
      	{
            send_to_char("{cYou can hear the shouts of messengers again.{x\n\r",ch);
            REMOVE_BIT(ch->comm,COMM_SHOUTSOFF);
      	}
      	else
      	{
            send_to_char("{cYou will no longer hear the shouts of messengers.{x\n\r",ch);
            SET_BIT(ch->comm,COMM_SHOUTSOFF);
      	}
      	return;
    }

    if ( IS_SET(ch->comm, COMM_NOSHOUT) && !IS_NPC( ch ) )
    {
        send_to_char( "Nobody wants to be a messenger for you{x.\n\r", ch );
        return;
    }

    if ( IS_SET(ch->comm, COMM_DEAF))
    {
        send_to_char( "You are deaf, you would not be able to understand your messengers{x.\n\r",ch);
        return;
    }

    if (ch->gold < 10 )
    {
        send_to_char( "You do not have enough gold to hire messengers{x.\n\r",ch);
        return;
    }

    if (ch->level < 25 )
    {
        send_to_char("{cMessengers only work for distinguished adventurers{x.\n\rBeing those of or greater than the 25th rank{x.\n\r",ch);
        return;
    }


    REMOVE_BIT(ch->comm,COMM_SHOUTSOFF);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

send_to_char("{gYou call throughout the city, hiring messengers.{x\n\r",ch);
WAIT_STATE(ch,16);
send_to_char("{gNearly a dozen messengers arrive heeding your call.{x\n\r",ch);
WAIT_STATE(ch,10);
send_to_char("{gYou give them each a gold piece and they scatter...{x\n\r",ch);
ch->gold -= 10;

    if (!is_affected(ch, gsn_deafen))
            act(
"{BAll across the land, messengers are yelling your message:{x\n\r     {B'{Y$T{B'{x",
               ch, NULL, buf, TO_CHAR);

    WAIT_STATE( ch, 24 );
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected == CON_PLAYING &&
	     d->character != ch &&
	     d->character->in_room != NULL &&
	     d->character->in_room->area == ch->in_room->area &&
/*  hopefully makes shouts go to road but not wBilderness */
	( (ch->in_room->sector_type == SECT_ROAD)
	|| (!IS_SET(ch->in_room->room_flags, ROOM_WILDERNESS)) ) &&
	     !IS_SET(victim->comm, COMM_SHOUTSOFF) &&
	     !IS_SET(victim->comm, COMM_QUIET) &&
             !IS_SET(d->character->comm, COMM_DEAF) &&
             !is_affected(d->character, gsn_deafen))
	 {

if (d->character->position == POS_SLEEPING)
      {
act(
      "{BEven in your sleep, you hear a messenger yell $n's message:{x\n\r     {B'{Y$t{B'{x",
                      ch,buf,d->character,TO_VICT);      }
else
      {             act(
      "{BA messenger, passing through the area, yells $n's message:{x\n\r     {B'{Y$t{B'{x",
                      ch,buf,d->character,TO_VICT);
      }
    }
  }    return;
}


void do_anon_shout( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char arg1[MAX_INPUT_LENGTH];
    char everything[MAX_STRING_LENGTH];

xcatf(arg1,"an important");

if (argument[0]=='\0')
   send_to_char("Syntax:  anon_shout <message>\n\r",ch);

    if (!is_affected(ch, gsn_deafen)) {
	xprintf(everything,"{BAll across the land, messengers are yelling %s message:{x\n\r     {B'{Y%s{B'{x",arg1,argument);
        act(everything, ch, NULL, NULL, TO_CHAR);
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected == CON_PLAYING &&
	     d->character != ch &&
/*	     d->character->in_room != NULL &&
	     d->character->in_room->area == ch->in_room->area &&
	( (ch->in_room->sector_type == SECT_ROAD)
	|| (!IS_SET(ch->in_room->room_flags, ROOM_WILDERNESS)) ) &&
*/	     !IS_SET(victim->comm, COMM_SHOUTSOFF) &&
	     !IS_SET(victim->comm, COMM_QUIET) &&
             !IS_SET(d->character->comm, COMM_DEAF) &&
             !is_affected(d->character, gsn_deafen))
	 {

if (d->character->position == POS_SLEEPING)
	xprintf(everything,
"{BEven in your sleep, you hear a messenger yell %s's message:{x\n\r     {B'{Y%s{B'{x",arg1,argument);

else
	xprintf(everything,
"{BA messenger, passing through the area, yells %s's message:{x\n\r     {B'{Y%s{B'{x",arg1,argument);

act(everything,ch,NULL,d->character,TO_VICT);      
    }
  }    return;
}


void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    if ( IS_SET(ch->comm, COMM_NOTELL) || IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char( "Your message didn't get through{x.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char( "{cPerhaps you should listen first.{x\n\r", ch);
	return;
    }

    if (IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char("{cYou must turn off deaf mode first{x.\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
    if ( ( victim = get_char_world( ch, arg ) ) == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here{x.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act("$N seems to have misplaced $S link...try again later.",
	    ch,NULL,victim,TO_CHAR);
        xprintf(buf,"{W%s tells you '%s'{x\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
	return;
    }

    if ( !(IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    && !IS_IMMORTAL(ch))
    {
	act( "$E is not receiving tells.", ch, 0, victim, TO_CHAR );
  	return;
    }

    if (IS_SET(victim->comm,COMM_AFK) || IS_SET(victim->comm,COMM_BUSY))
    {
	if (IS_NPC(victim))
	{
	    act("$E is BUSY, and not receiving tells.",ch,NULL,victim,TO_CHAR);
	    return;
	}

	act("$E is BUSY, but your tell will go through when $E finishes.",
	    ch,NULL,victim,TO_CHAR);
	xprintf(buf,"{W%s tells you '%s'{x\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
	add_buf(victim->pcdata->buffer,buf);
	return;
    }


if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
  if (IS_AFFECTED(ch,AFF_HASTE) && ch->pcdata->condition[COND_DRUNK] <= 0)
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

   if (!is_affected(ch, gsn_deafen))
     act("{WYou tell $N '$t'{x",
               ch,buf,victim,TO_CHAR);
   act("{W$n tells you '$t'{x",
             ch,buf,victim,TO_VICT);

    victim->reply	= ch;
  if ( !IS_NPC(ch) && IS_NPC(victim) && HAS_TRIGGER_MOB(victim,TRIG_SPEECH) )
      p_act_trigger( buf, victim, NULL, NULL, ch, NULL, NULL, TRIG_ACT );


    return;
}


void do_reply( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->comm, COMM_NOTELL) )
    {
	send_to_char( "Your message didn't get through{x.\n\r", ch );
	return;
    }

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here{x.\n\r", ch );
	return;
    }

    strcpy(buf,argument);
    if ( victim->desc == NULL && !IS_NPC(victim))
    {
if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
  if (is_affected(ch,gsn_haste) && ch->pcdata->condition[COND_DRUNK] <= 0)
           argument=make_talk_hasted(argument);

        if (is_affected(ch,gsn_garble))
          garble(buf,argument);
        else
          strcpy(buf,argument);
        act("$N seems to have misplaced $S link...try again later.",
            ch,NULL,victim,TO_CHAR);
        xprintf(buf,"{W%s replies '%s'{x\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
        return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    &&  !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
    {
        act_new( "$E is not receiving tells.", ch, 0, victim, TO_CHAR,POS_DEAD);
        return;
    }

    if (!IS_IMMORTAL(victim) && !IS_AWAKE(ch))
    {
	send_to_char( "In your dreams, or what?\n\r", ch );
	return;
    }

    if (IS_SET(victim->comm,COMM_AFK) || IS_SET(victim->comm,COMM_BUSY))
    {
        if (IS_NPC(victim))
        {
            act_new("$E is BUSY, and not receiving tells.",
		ch,NULL,victim,TO_CHAR,POS_DEAD);
            return;
        }

        act_new("$E is BUSY, but your tell will go through when $E finishes.",
            ch,NULL,victim,TO_CHAR,POS_DEAD);
        xprintf(buf,"{W%s replies '%s'{x\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
        return;
    }

   if (!is_affected(ch, gsn_deafen))
     act("{WYou reply to $N '$t'{x",
               ch,buf,victim,TO_CHAR);
   act("{W$n replies '$t'{x",
              ch,buf,victim,TO_VICT);

    victim->reply	= ch;

    return;
}



void do_yell( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    CHAR_DATA *victim;
    AFFECT_DATA af;


    if ( IS_SET(ch->comm, COMM_NOSHOUT) && !IS_NPC(ch) )
    {
        send_to_char( "You can't yell{x.\n\r", ch );
        return;
    }

    if (ch->move < ch->max_move * .05)
    {
    send_to_char("{cYou must catch your breath some first{x.\n\r",ch);
    return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Yell what?\n\r", ch );
	return;
    }

//   if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
//	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
  if (is_affected(ch,gsn_haste) && ch->pcdata->condition[COND_DRUNK] <= 0)
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

ch->move -= ch->max_move * .05;
   if (!is_affected(ch, gsn_deafen))
     act("{YYou yell '$t'{x",
               ch,buf,NULL,TO_CHAR);

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !IS_SET(d->character->comm,COMM_QUIET)
        &&   !IS_SET(d->character->comm, COMM_DEAF)
        &&   !is_affected(d->character, gsn_deafen))
	{
            act("{Y$n yells '$t'{x",
                      ch,buf,d->character,TO_VICT);
	}
    }
	
 if (!str_cmp (ch->name, "Tryna"))
  {
   if ( ( victim = get_char_world( ch, "Tyche" ) ) != NULL)
    {
      /*Make Tyche fly to Tryna*/
      if (!str_cmp (buf, "Here Tyche"))
       {
        location = ch->in_room;
        if ( victim->fighting == NULL )
         {
          act( "{GTyche spreads her {cm{Cagnificent {yy{Yellow {Gwings and glides away.{x", victim, NULL, NULL, TO_ROOM);
          char_from_room( victim );
          char_to_room( victim, location );
          act( "{GTyche soars down and lands next to Tryna gracefully.{x", victim, NULL, NULL, TO_ROOM);
          send_to_char( "{GTyche soars down and lands next to you gracefully.{x\n\r", ch );
         }
       }  
       /*END Make Tyche fly to Tryna*/ 

      /*Make Tyche follow Tryna*/
       if (!str_cmp (buf, "Follow Tyche"))
        {
         if ( ( victim = get_char_room( ch,NULL ,"Tyche" ) ) == NULL )
          {
	   send_to_char( "Tyche aren't here{x.\n\r", ch );
	   return;
          }
 
         if ( victim->master !=NULL )
	  stop_follower( victim );
         
         add_follower( victim, ch );
         victim->leader = ch;
         af.where     = TO_AFFECTS;
         af.type      = skill_lookup("charm person");
         af.level     = ch->level;
         af.duration  = -1;
         af.location  = 0;
         af.modifier  = 0;
         af.bitvector = AFF_CHARM;
         affect_to_char( victim, &af );
        }
       /*END Make Tyche follow Tryna*/ 
       /*Make Tyche stop follow Tryna*/ 
       if (!str_cmp (buf, "Nofol Tyche"))
        {
         if ( victim->master !=NULL )
	  stop_follower( victim );        
        }
       /*END Make Tyche stop follow Tryna*/ 
  
       if (!str_cmp (buf, "Home Tyche"))
        {
/* 
	 if ( ( location = find_location( ch, 29901 ) ) == NULL )
	 {
	  send_to_char( "BUG: Report -> Home of Tyche is delete.\n\r", ch );
	  return;
	 }        
*/
        if ( victim->master !=NULL )
	 stop_follower( victim );
	  
        act( "{GTyche spreads her {cm{Cagnificent {yy{Yellow {Gwings and glides away.{x", victim, NULL, NULL, TO_ROOM); 
        if (get_room_index( 29901 ) != NULL)
         {
          char_from_room( victim );
          char_to_room( victim, get_room_index( 29901 ) );
         } 
         
        act( "{GTyche soars down and lands here.{x", victim, NULL, NULL, TO_ROOM);
        }
  
 }
 }
    return;
}

/*  adds narrate */
void do_narrate( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_INPUT_LENGTH];


    if ( IS_SET(ch->comm, COMM_NOSHOUT) && !IS_NPC(ch) )
    {
        send_to_char( "You can't narrate{x.\n\r", ch );
        return;
    }

    if (ch->move < ch->max_move * .05)
    {
    send_to_char("{cYou are too tired to do whatever you narrate was going to be{x.\n\r",ch);
    return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Narrate what?\n\r", ch );
	return;
    }

if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
  if (is_affected(ch,gsn_haste) && ch->pcdata->condition[COND_DRUNK] <= 0)
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

	ch->move -= ch->max_move * .05;
   if (!is_affected(ch, gsn_deafen))
     act("{YYou narrate: {g$t{x",
               ch,buf,NULL,TO_CHAR);

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !IS_SET(d->character->comm,COMM_QUIET)
        &&   !IS_SET(d->character->comm, COMM_DEAF)
        &&   !is_affected(d->character, gsn_deafen))
	{
            act("{Y$n narrates: {g$t{x",
                      ch,buf,d->character,TO_VICT);
	}
    }

    return;
}


void do_emote( CHAR_DATA *ch, char *argument )
{
char buf[MAX_INPUT_LENGTH];

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "You can't show your emotions{x.\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char( "Emote what?\n\r", ch );
        return;
    }

if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
  if (is_affected(ch,gsn_haste) && ch->pcdata->condition[COND_DRUNK] <= 0)
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

    MOBtrigger = FALSE;
    act( "{g$n $T{x", ch, NULL, buf, TO_ROOM );
    act( "{g$n $T{x", ch, NULL, buf, TO_CHAR );
    MOBtrigger = TRUE;
    return;
}

void do_oemote( CHAR_DATA *ch, char *argument )
{
char buf[MAX_INPUT_LENGTH];

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "You can't show your emotions{x.\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char( "Emote what?\n\r", ch );
        return;
    }

if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
  if (is_affected(ch,gsn_haste) && ch->pcdata->condition[COND_DRUNK] <= 0)
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

    MOBtrigger = FALSE;
    act( "{g$n $T{x (OOC)", ch, NULL, buf, TO_ROOM );
    act( "{g$n $T{x (OOC)", ch, NULL, buf, TO_CHAR );
    MOBtrigger = TRUE;
    return;
}

void do_pmote( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char *letter,*name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    int matches = 0;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "You can't show your emotions{x.\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        send_to_char( "Emote what?\n\r", ch );
        return;
    }

    act( "{g$n $t{x", ch, argument, NULL, TO_CHAR );

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->desc == NULL || vch == ch)
	    continue;

	if ((letter = strstr(argument,vch->name)) == NULL)
	{
            MOBtrigger = FALSE;
	    act("$N $t",vch,argument,ch,TO_CHAR);
	    continue;
	    MOBtrigger = TRUE;
	}

	strcpy(temp,argument);
	temp[strlen(argument) - strlen(letter)] = '\0';
   	last[0] = '\0';
 	name = vch->name;

	for (; *letter != '\0'; letter++)
	{
	    if (*letter == '\'' && matches == strlen(vch->name))
	    {
		strcat(temp,"r");
		continue;
	    }

	    if (*letter == 's' && matches == strlen(vch->name))
	    {
		matches = 0;
		continue;
	    }

 	    if (matches == strlen(vch->name))
	    {
		matches = 0;
	    }

	    if (*letter == *name)
	    {
		matches++;
		name++;
		if (matches == strlen(vch->name))
		{
		    strcat(temp,"{gyou");
		    last[0] = '\0';
		    name = vch->name;
		    continue;
		}
		strncat(last,letter,1);
		continue;
	    }

	    matches = 0;
	    strcat(temp,last);
	    strncat(temp,letter,1);
	    last[0] = '\0';
	    name = vch->name;
	}
	MOBtrigger = FALSE;
	act("{g$N $t{x",vch,temp,ch,TO_CHAR);
        MOBtrigger = TRUE;
    }

    return;
}


/*
 * All the posing stuff.
 */
struct	pose_table_type
{
    char *	message[2*MAX_CLASS];
};

const	struct	pose_table_type	pose_table	[]	=
{
    {
	{
	    "You sizzle with energy.",
	    "$n sizzles with energy.",
	    "You feel very holy.",
	    "$n looks very holy.",
	    "You perform a small card trick.",
	    "$n performs a small card trick.",
	    "You show your bulging muscles.",
	    "$n shows $s bulging muscles."
	}
    },

    {
	{
	    "You turn into a butterfly, then return to your normal shape.",
	    "$n turns into a butterfly, then returns to $s normal shape.",
	    "You nonchalantly turn wine into water.",
	    "$n nonchalantly turns wine into water.",
	    "You wiggle your ears alternately.",
	    "$n wiggles $s ears alternately.",
	    "You crack nuts between your fingers.",
	    "$n cracks nuts between $s fingers."
	}
    },

    {
	{
	    "Blue sparks fly from your fingers.",
	    "Blue sparks fly from $n's fingers.",
	    "A halo appears over your head.",
	    "A halo appears over $n's head.",
	    "You nimbly tie yourself into a knot.",
	    "$n nimbly ties $mself into a knot.",
	    "You grizzle your teeth and look mean.",
	    "$n grizzles $s teeth and looks mean."
	}
    },

    {
	{
	    "Little red lights dance in your eyes.",
	    "Little red lights dance in $n's eyes.",
	    "You recite words of wisdom.",
	    "$n recites words of wisdom.",
	    "You juggle with daggers, apples, and eyeballs.",
	    "$n juggles with daggers, apples, and eyeballs.",
	    "You hit your head, and your eyes roll.",
	    "$n hits $s head, and $s eyes roll."
	}
    },

    {
	{
	    "A slimy green monster appears before you and bows.",
	    "A slimy green monster appears before $n and bows.",
	    "Deep in prayer, you levitate.",
	    "Deep in prayer, $n levitates.",
	    "You steal the underwear off every person in the room.",
	    "Your underwear is gone!  $n stole it!",
	    "Crunch, crunch -- you munch a bottle.",
	    "Crunch, crunch -- $n munches a bottle."
	}
    },

    {
	{
	    "You turn everybody into a little pink elephant.",
	    "You are turned into a little pink elephant by $n.",
	    "An angel consults you.",
	    "An angel consults $n.",
	    "The dice roll ... and you win again.",
	    "The dice roll ... and $n wins again.",
	    "... 98, 99, 100 ... you do pushups.",
	    "... 98, 99, 100 ... $n does pushups."
	}
    },

    {
	{
	    "A small ball of light dances on your fingertips.",
	    "A small ball of light dances on $n's fingertips.",
	    "Your body glows with an unearthly light.",
	    "$n's body glows with an unearthly light.",
	    "You count the money in everyone's pockets.",
	    "Check your money, $n is counting it.",
	    "Arnold Schwarzenegger admires your physique.",
	    "Arnold Schwarzenegger admires $n's physique."
	}
    },

    {
	{
	    "Smoke and fumes leak from your nostrils.",
	    "Smoke and fumes leak from $n's nostrils.",
	    "A spot light hits you.",
	    "A spot light hits $n.",
	    "You balance a pocket knife on your tongue.",
	    "$n balances a pocket knife on your tongue.",
	    "Watch your feet, you are juggling granite boulders.",
	    "Watch your feet, $n is juggling granite boulders."
	}
    },

    {
	{
	    "The light flickers as you rap in magical languages.",
	    "The light flickers as $n raps in magical languages.",
	    "Everyone levitates as you pray.",
	    "You levitate as $n prays.",
	    "You produce a coin from everyone's ear.",
	    "$n produces a coin from your ear.",
	    "Oomph!  You squeeze water out of a granite boulder.",
	    "Oomph!  $n squeezes water out of a granite boulder."
	}
    },

    {
	{
	    "Your head disappears.",
	    "$n's head disappears.",
	    "A cool breeze refreshes you.",
	    "A cool breeze refreshes $n.",
	    "You step behind your shadow.",
	    "$n steps behind $s shadow.",
	    "You pick your teeth with a spear.",
	    "$n picks $s teeth with a spear."
	}
    },

    {
	{
	    "A fire elemental singes your hair.",
	    "A fire elemental singes $n's hair.",
	    "The sun pierces through the clouds to illuminate you.",
	    "The sun pierces through the clouds to illuminate $n.",
	    "Your eyes dance with greed.",
	    "$n's eyes dance with greed.",
	    "Everyone is swept off their foot by your hug.",
	    "You are swept off your feet by $n's hug."
	}
    },

    {
	{
	    "The sky changes color to match your eyes.",
	    "The sky changes color to match $n's eyes.",
	    "The ocean parts before you.",
	    "The ocean parts before $n.",
	    "You deftly steal everyone's weapon.",
	    "$n deftly steals your weapon.",
	    "Your karate chop splits a tree.",
	    "$n's karate chop splits a tree."
	}
    },

    {
	{
	    "The stones dance to your command.",
	    "The stones dance to $n's command.",
	    "A thunder cloud kneels to you.",
	    "A thunder cloud kneels to $n.",
	    "The Grey Mouser buys you a beer.",
	    "The Grey Mouser buys $n a beer.",
	    "A strap of your armor breaks over your mighty thews.",
	    "A strap of $n's armor breaks over $s mighty thews."
	}
    },

    {
	{
	    "The heavens and grass change colour as you smile.",
	    "The heavens and grass change colour as $n smiles.",
	    "The Burning Man speaks to you.",
	    "The Burning Man speaks to $n.",
	    "Everyone's pocket explodes with your fireworks.",
	    "Your pocket explodes with $n's fireworks.",
	    "A boulder cracks at your frown.",
	    "A boulder cracks at $n's frown."
	}
    },

    {
	{
	    "Everyone's clothes are transparent, and you are laughing.",
	    "Your clothes are transparent, and $n is laughing.",
	    "An eye in a pyramid winks at you.",
	    "An eye in a pyramid winks at $n.",
	    "Everyone discovers your dagger a centimeter from their eye.",
	    "You discover $n's dagger a centimeter from your eye.",
	    "Mercenaries arrive to do your bidding.",
	    "Mercenaries arrive to do $n's bidding."
	}
    },

    {
	{
	    "A black hole swallows you.",
	    "A black hole swallows $n.",
	    "Valentine Michael Smith offers you a glass of water.",
	    "Valentine Michael Smith offers $n a glass of water.",
	    "Where did you go?",
	    "Where did $n go?",
	    "Four matched Percherons bring in your chariot.",
	    "Four matched Percherons bring in $n's chariot."
	}
    },

    {
	{
	    "The world shimmers in time with your whistling.",
	    "The world shimmers in time with $n's whistling.",
	    "Gods give you a staff.",
	    "Gods give $n a staff.",
	    "Click.",
	    "Click.",
	    "Atlas asks you to relieve him.",
	    "Atlas asks $n to relieve him."
	}
    }
};



void do_pose( CHAR_DATA *ch, char *argument )
{
    int level;
    int pose;

    if ( IS_NPC(ch) )
	return;

    level = UMIN( ch->level, sizeof(pose_table) / sizeof(pose_table[0]) - 1 );
    pose  = number_range(0, level);

    act( pose_table[pose].message[2*ch->class+0], ch, NULL, NULL, TO_CHAR );
    act( pose_table[pose].message[2*ch->class+1], ch, NULL, NULL, TO_ROOM );

    return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
    append_file( ch, BUG_FILE, argument );
    send_to_char( "Bug logged{x.\n\r", ch );
    return;
}

void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "Typo logged{x.\n\r", ch );
    return;
}

void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "There is no rent here. Just save and quit{x.\n\r", ch );
    return;
}


void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out{x.\n\r", ch );
    return;
}


void do_quit( CHAR_DATA *ch, char *argument )
{
   do_quit_org(ch, argument, FALSE, TRUE );
   return;
}

void do_quit_count( CHAR_DATA *ch, char *argument )
{
   do_quit_org(ch, argument, TRUE,TRUE );
   return;
}

void do_quit_org( CHAR_DATA *ch, char *argument, bool Count, bool blnWiznet )
{
    DESCRIPTOR_DATA *d, *d_next;
    CHAR_DATA *vch;
    int id;
    if ( IS_NPC(ch) )
	return;

    if (ch->level == 1)
    send_to_char("{CCharacter not saved, Goodbye.\n\r{x",ch);

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "{CNo way! You are fighting.{x\n\r", ch );
	return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
	send_to_char( "{CYou're not DEAD yet.{x\n\r", ch );
	return;
    }

    if ( ch->qflag == 1 ) 
    {
	send_to_char("You can't quit.\n\r",ch);
	return;
    }

    if ((ch->pking > 0) && !IS_IMMORTAL(ch)) 
	{
	send_to_char("You are having way too much fun right now!!\n\r",ch);
	return;
	}

    if ( ch->last_fight_time != -1 && !IS_IMMORTAL(ch) &&
        (current_time - ch->last_fight_time)<FIGHT_DELAY_TIME) 
      {
        send_to_char("{CYour adrenalin is gushing! You can't quit yet.{x\n\r",ch);
        return;
      }

   if ( auction->item != NULL && ((ch == auction->buyer) || (ch == auction->seller)) )
    {
  send_to_char ("Wait till you have sold/bought the item on auction.\n\r",ch);
        return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "{CYou don't want to leave your dear master.{x\n\r",ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_SLEEP ) )
    {
	send_to_char( "{CWhy don't you finish your fight?{x\n\r", ch );
	return;
    }

    /* Don't allow to quit if there is an areana going on */
    if (IS_SET(ch->act, PLR_ARENA))
    {
	send_to_char("You cannot quit while you are in the arena.\n\r",ch);
	return;
	}

if (blnWiznet) {
    send_to_char(
	"{CAlas, all good things must come to an end.{x\n\r",ch);
    if (ch->pcdata->confirm_delete)
       act( "{G$n has left the land, never to return.{x", ch, NULL,NULL,TO_ROOM );
    else act( "{G$n has left the land.{x", ch, NULL, NULL,TO_ROOM );
    sprintf( log_buf, "%s has quit.", ch->name );
    log_string( log_buf );
     wiznet("[$N has left Fate]",ch,NULL,WIZ_LOGINS,0,get_trust(ch));
}
    for (vch=char_list;vch != NULL;vch = vch->next) {
      if (is_affected(vch,gsn_doppelganger) && vch->doppel == ch) {
        send_to_char("{CYou shift to your true form as your victim leaves.{x\n\r",
                     vch);
        affect_strip(vch,gsn_doppelganger);
      }

      if (vch->guarding == ch)
        {
          act("{cYou stops guarding $N.{x", vch, NULL, ch, TO_CHAR);
          act("{c$n stops guarding you.{x", vch, NULL, ch, TO_VICT);
          act("{c$n stops guarding $N.{x", vch, NULL, ch, TO_NOTVICT);
          vch->guarding  = NULL;
          ch->guarded_by = NULL;
        }
      if (vch->last_fought == ch)
        vch->last_fought = NULL;

    }

    if (ch->guarded_by != NULL)
      {
        ch->guarded_by->guarding = NULL;
        ch->guarded_by = NULL;
      }
  if (ch->assassin != NULL)
   {
    ch->assassin->assassin = NULL;
    ch->assassin = NULL;
   } 

    /*
     * After extract_char the ch is no longer valid!
     */
    affect_strip(ch,skill_lookup( "aid" ));
    affect_strip(ch,skill_lookup( "illusions grandeur" ));

    
    if (ch->level > 1)
    save_char_obj( ch );
    id = ch->id;
     /*  board code added */
     /* Free note that might be there somehow */
     if (ch->pcdata->in_progress)
     	free_note (ch->pcdata->in_progress);
     /*  end */
    d = ch->desc;
    if ( Count )
      extract_char( ch, TRUE );
    else
      extract_char_nocount( ch, TRUE );
    if ( d != NULL )
	close_socket( d );


    /* toast evil cheating bastards    */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
	CHAR_DATA *tch;

	d_next = d->next;
	tch = d->original ? d->original : d->character;
	if (tch && tch->id == id)
	{
	    extract_char_nocount(tch,TRUE);
	    close_socket(d);
	}
    }


    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{

    if ( IS_NPC(ch) )
	return;

    if (ch->level<2)	
    {
	send_to_char("{CYou have not been here long enough to save. Gain a level first.{x\n\r", ch);
	return;
    }

    save_char_obj( ch );
    send_to_char("{cSaving. Remember that Fate auto saves. You do not need to save{x.\n\r", ch);
    WAIT_STATE(ch, 10); 
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here{x.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself{x.\n\r", ch );
	    return;
	}
	stop_follower(ch);
	return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
    {
	act("$N doesn't seem to want any followers{x.\n\r",
             ch,NULL,victim, TO_CHAR);
        return;
    }

    REMOVE_BIT(ch->act,PLR_NOFOLLOW);

    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}


void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    if ( can_see( master, ch ) )
	act( "$n now follows you.", ch, NULL, master, TO_VICT );

    act( "You now follow $N.",  ch, NULL, master, TO_CHAR );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
	if (IS_NPC(ch))
	  {
		
		stop_fighting( ch, TRUE );
		ch->last_fought = NULL;
	 }
    }

    if ( can_see( ch->master, ch ) && ch->in_room != NULL)
    {
	act( "$n stops following you.",     ch, NULL, ch->master, TO_VICT    );
    	act( "You stop following $N.",      ch, NULL, ch->master, TO_CHAR    );
    }
    if (ch->master->pet == ch)
	ch->master->pet = NULL;

    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{
    CHAR_DATA *pet;

    if ((pet = ch->pet) != NULL)
    {
    	stop_follower(pet);
    	if (pet->in_room != NULL)
    	    act("$N slowly fades away.",ch,NULL,pet,TO_NOTVICT);
    	extract_char_nocount(pet,TRUE);
    }
    ch->pet = NULL;

    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;

    if ( ch->master != NULL )
    {
    	if (ch->master->pet == ch)
    	    ch->master->pet = NULL;
	stop_follower( ch );
    }

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
    }

    return;
}




void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	xprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		  xprintf( buf,
		  "[%s] %-16s %4d%% hp %4d%% mana %4d%% mv %5d xp\n\r",
		    IS_NPC(gch) ? "Mob" : class_table[gch->class].who_name,
		    capitalize( PERS(gch, ch) ),
		    PERCENT(gch->hit,   gch->max_hit),
		    PERCENT(gch->mana,  gch->max_mana),
		    PERCENT(gch->move,  gch->max_move),
		    gch->exp );
		send_to_char( buf, ch );
	    }
	}
	return;
    }

    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "They aren't here{x.\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (IS_AFFECTED(victim,AFF_CHARM))
    {
        send_to_char("{cYou can't remove charmed mobs from your group{x.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
    {
    	act("You like your master too much to leave $m!",ch,NULL,victim,TO_VICT);
    	return;
    }


  if ( is_same_group( victim, ch ) && ch != victim )
    {
      if (ch->guarding == victim || victim->guarded_by == ch)
        {
          act("You stop guarding $N.", ch, NULL, victim, TO_CHAR);
          act("$n stops guarding you.", ch, NULL, victim, TO_VICT);
          act("$n stops guarding $N.", ch, NULL, victim, TO_NOTVICT);
          victim->guarded_by = NULL;
          ch->guarding       = NULL;
        }

      victim->leader = NULL;
      act( "$n removes $N from $s group.",   ch, NULL, victim, TO_NOTVICT );
      act( "$n removes you from $s group.",  ch, NULL, victim, TO_VICT    );
      act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR    );

      if (victim->guarded_by != NULL &&
          !is_same_group(victim,victim->guarded_by))
        {
          act("You stop guarding $N.",victim->guarded_by,NULL,victim,TO_CHAR);
          act("$n stops guarding you.",victim->guarded_by,NULL,victim,TO_VICT);
          act("$n stops guarding $N.",victim->guarded_by,NULL,victim,TO_NOTVICT);
          victim->guarded_by->guarding = NULL;
          victim->guarded_by           = NULL;
        }
      return;
    }

/*
  if ( ch->level - victim->level < -8
      ||   ch->level - victim->level > 8 )
    {
      act( "$N cannot join $n's group.",     ch, NULL, victim, TO_NOTVICT );
      act( "You cannot join $n's group.",    ch, NULL, victim, TO_VICT    );
      act( "$N cannot join your group.",     ch, NULL, victim, TO_CHAR    );
      return;
    }


  if (IS_GOOD(ch) && IS_EVIL(victim))
    {
      act( "You are too evil for $n's group.", ch, NULL, victim, TO_VICT);
      act( "$N is too evil for your group!", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (IS_GOOD(victim) && IS_EVIL(ch))
    {
      act("You are too pure to join $n's group!", ch, NULL, victim, TO_VICT);
      act("$N is too pure for your group!", ch, NULL, victim, TO_CHAR);
      return;
    }

*/
  victim->leader = ch;
  act( "$N joins $n's group.", ch, NULL, victim, TO_NOTVICT );
  act( "You join $n's group.", ch, NULL, victim, TO_VICT    );
  act( "$N joins your group.", ch, NULL, victim, TO_CHAR    );
  return;

}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount_gold = 0, amount_silver = 0;
    int share_gold, share_silver;
    int extra_gold, extra_silver;

    argument = one_argument( argument, arg1 );
	       one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }

    amount_silver = atoi( arg1 );

    if (arg2[0] != '\0')
	amount_gold = atoi(arg2);

    if ( amount_gold < 0 || amount_silver < 0)
    {
	send_to_char( "Your group wouldn't like that{x.\n\r", ch );
	return;
    }

    if ( amount_gold == 0 && amount_silver == 0 )
    {
	send_to_char( "You hand out zero coins, but no one notices{x.\n\r", ch );
	return;
    }

    if ( ch->gold <  amount_gold || ch->silver < amount_silver)
    {
	send_to_char( "You don't have that much to split{x.\n\r", ch );
	return;
    }

    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CHARM))
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all{x.\n\r", ch );
	return;
    }

    share_silver = amount_silver / members;
    extra_silver = amount_silver % members;

    share_gold   = amount_gold / members;
    extra_gold   = amount_gold % members;

    if ( share_gold == 0 && share_silver == 0 )
    {
	send_to_char( "Don't even bother, cheapskate{x.\n\r", ch );
	return;
    }

    ch->silver	-= amount_silver;
    ch->silver	+= share_silver + extra_silver;
    ch->gold 	-= amount_gold;
    ch->gold 	+= share_gold + extra_gold;

    if (share_silver > 0)
    {
	xprintf(buf,
	    "You split %d silver coins. Your share is %d silver{x.\n\r",
 	    amount_silver,share_silver + extra_silver);
	send_to_char(buf,ch);
    }

    if (share_gold > 0)
    {
	xprintf(buf,
	    "You split %d gold coins. Your share is %d gold{x.\n\r",
	     amount_gold,share_gold + extra_gold);
	send_to_char(buf,ch);
    }

    if (share_gold == 0)
    {
	xprintf(buf,"$n splits %d silver coins. Your share is %d silver.",
		amount_silver,share_silver);
    }
    else if (share_silver == 0)
    {
	xprintf(buf,"$n splits %d gold coins. Your share is %d gold.",
		amount_gold,share_gold);
    }
    else
    {
	xprintf(buf,
"$n splits %d silver and %d gold coins, giving you %d silver and %d gold{x.\n\r",
	 amount_silver,amount_gold,share_silver,share_gold);
    }

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group(gch,ch) && !IS_AFFECTED(gch,AFF_CHARM))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share_gold;
	    gch->silver += share_silver;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    int i;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
    if (is_affected(ch,gsn_haste))
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf,argument);
    else
      strcpy(buf,argument);

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */

    for ( i = 0, gch = char_list; gch != NULL; gch = gch->next )
    {
      if ( is_same_group( gch, ch ) && !is_affected(gch, gsn_deafen))
        {
          act_new("{m$n tells the group '{M$t{m'{x",
                  ch,buf,gch,TO_VICT,POS_DEAD);
          i++;
        }
    }

    if (i > 1 && !is_affected(ch, gsn_deafen))
      act_new("{mYou tell your group '{M$t{m'{x",
                ch,buf,NULL,TO_CHAR,POS_DEAD);
    else send_to_char( "Quit talking to yourself. You are all alone{x.\n\r",ch);

    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == NULL || bch == NULL)
	return FALSE;

    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

/*  new color command */
/* still dont work */
void do_colour( CHAR_DATA *ch, char *argument )
{

        if (IS_NPC(ch))
	{

        send_to_char_bw("A shame you are colorblind, is it not?\n\r",ch);
                return;
	}
        if (IS_SET(ch->act,PLR_COLOR))
        {

                REMOVE_BIT(ch->act,PLR_COLOR);
                send_to_char("{cThe world no longer seems so colorful{x.\n\r",ch);
        }
	else
        {

            SET_BIT(ch->act,PLR_COLOR);
	    send_to_char( "You open your eyes to the world's {bC{ro{yl{co{gr{x!\n\r", ch );
        }

}

void do_gd( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_INPUT_LENGTH];

    if ( !(ch->kingdom) )
      {
	send_to_char("{cYou are not in a Kingdom{x.\n\r",ch);
	return;
      }


     if(ch->sex == SEX_FEMALE)
    xprintf(buf, "{R%s $n:{W $t{w",
	kingdom_table[ch->kingdom].rank_titlesf[ch->kingdom_rank]);
	else
    xprintf(buf, "{R%s $n:{W $t{w",
	kingdom_table[ch->kingdom].rank_titles[ch->kingdom_rank]);

     if (IS_IMMORTAL(ch) && (!str_cmp (ch->name, "Zalev") ) )
        xprintf(buf, "{RGod $n:{W $t{w");
	


if (!IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10)
	argument = makedrunk(argument,ch);

	 if (!IS_NPC(ch))
    if (is_affected(ch,gsn_haste))
      argument=make_talk_hasted(argument);

    if (is_affected(ch,gsn_garble))
      garble(buf2,argument);
    else
      strcpy(buf2,argument);

   if (!is_affected(ch, gsn_deafen)) 
       act_new(buf,ch,buf2,NULL,TO_CHAR,POS_DEAD);

  for ( d = descriptor_list; d != NULL; d = d->next )
      {
            CHAR_DATA *vch;
            vch = d->original ? d->original : d->character;
            if (d->connected == CON_PLAYING &&
		(vch->kingdom == ch->kingdom) &&
		 !IS_SET(vch->comm,COMM_QUIET))
	            { 
                act_new(buf,ch,buf2,vch,TO_VICT,POS_DEAD);
            }
        }
    return;
}

void do_pray(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("{cThe gods refuse to listen to you right now.",ch);
	  return;
	}

    send_to_char("{cYou pray to the gods..!\n\r",ch);
    send_to_char("{cThis is not an emote, Do NOT abuse this{x.\n\r",ch);
    send_to_char("{cThis is a channel to call the immortals.{x\n\r",
		ch);

    for (d = descriptor_list; d != NULL; d = d->next)
      {
	if (d->connected == CON_PLAYING && IS_IMMORTAL(d->character) &&
	    !IS_SET(d->character->comm,COMM_NOWIZ))
	  {
	    if (argument[0] == '\0')
	      {
		act("$n is PRAYING for: any god",
			ch,argument,d->character,TO_VICT);
	      }
	    else
	      {
		act("$n is PRAYING for: $t",
			ch,argument,d->character,TO_VICT);
	      }
	  }
      }
   return;
}
void do_msp( CHAR_DATA *ch, char *argument )
{
   if(ch->msp == 1 )
{
	ch->msp = 0;
	send_to_char("Mud Sound Protocol Deactivated.\n\r",ch);
 }
else
 {
	ch->msp = 1;
   send_to_char("Mud Sound Protocol Activated.\n\r",ch);
   }
}

void do_vt( CHAR_DATA *ch, char *argument )
{
  if( ch->vt == 1 )
  {
	ch->vt = 0;
     send_to_char( "\033[1;24r\033[24;1HVirtual terminal is OFF.\n\r", ch);
  }
  else
  {
	ch->vt = 1;
	do_exits(ch,"vt");
     send_to_char( "Virtual terminal is ON.\n\r", ch );
  }
}

void do_locktitle(CHAR_DATA *ch, char *argument)
{
  if (IS_SET(ch->comm, COMM_BLOCKTITLE))
  {
    REMOVE_BIT(ch->comm, COMM_BLOCKTITLE);
    send_to_char("Your title will not be lock when you level.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->comm, COMM_BLOCKTITLE);
    send_to_char("Your title will be lock when you level.\n\r", ch);
  }
}

void do_lockon(CHAR_DATA *ch, char *argument)
{
  if (IS_SET(ch->detection, D_LOCKHOST))
  {
    REMOVE_BIT(ch->detection, D_LOCKHOST);
    send_to_char("Your host will not be locked.\n\r", ch);
  }
  else
  {
    SET_BIT(ch->detection, D_LOCKHOST);
    send_to_char("Your host will be locked.\n\r", ch);
    send_to_char("Remember to use the sitelock to modified your site.\n\r", ch);
  }
}

bool check_site(CHAR_DATA *ch,char *site)
{
    char host[MAX_STRING_LENGTH];

    strcpy(host,capitalize(site));
    host[0] = LOWER(host[0]);

	if (!ch->sitelock)
		return FALSE;

        if (IS_SET(ch->detection,D_PREFIX)
        &&  IS_SET(ch->detection,D_SUFFIX)
        &&  strstr(ch->sitelock,host) != NULL)
            return TRUE;

        if (IS_SET(ch->detection,D_PREFIX)
        &&  !str_suffix(ch->sitelock,host))
            return TRUE;

        if (IS_SET(ch->detection,D_SUFFIX)
        &&  !str_prefix(ch->sitelock,host))
            return TRUE;

    return FALSE;
}

void do_sitelock(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char *name;
    bool prefix = FALSE,suffix = FALSE;

    argument = one_argument(argument,arg);  

    name = arg;

    if (name[0] == '*')
    {
        prefix = TRUE;
        name++;
    }

    if (name[strlen(name) - 1] == '*')
    {
        suffix = TRUE;
        name[strlen(name) - 1] = '\0';
   }

    if (strlen(name) == 0)
    {
        send_to_char("You have to sitelock SOMETHING.\n\r",ch);
        return;
    }

  if (prefix)
        SET_BIT(ch->detection,D_PREFIX);
	else if(IS_SET(ch->detection,D_PREFIX))
        REMOVE_BIT(ch->detection,D_PREFIX);
    if (suffix)
        SET_BIT(ch->detection,D_SUFFIX);
	else if(IS_SET(ch->detection,D_SUFFIX))
        REMOVE_BIT(ch->detection,D_SUFFIX);

    free_string(ch->sitelock);
    ch->sitelock = str_dup(name);
    xprintf(buf,"'%s' will be locked as your login site\n\r.",
		ch->sitelock);
    send_to_char( buf, ch );
    return;
}
