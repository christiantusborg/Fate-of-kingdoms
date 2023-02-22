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
				Creation
****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>   /* for va_list at act_color() from CF */
#include <unistd.h> /* OLC -- for close read write etc */  

#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "fok_struct.h"

/* forked save */
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
/*
 * Global variables.
 */
//const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
//const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
void do_clear_login(DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{

 send_to_char("\033[0;0H\033[2J", ch );
char buf[MAX_INPUT_LENGTH];
write_to_buffer(d,"\n\r           ,###%%%%%%^^^^^""'`.`    ,  (@)--------------------------------(@)",0);
write_to_buffer(d,"\n\r       #####%%%%%%%%%^^^""""'``          |*|      Fate of Kingdoms IV       |*|",0);
write_to_buffer(d,"\n\r   `#######%%%%%%%%^^'""'' `    ``     (@)--------------------------------(@)",0);
write_to_buffer(d,"\n\r    ( ,- `{?,-' `. ,/%%%%            |*|       Character creation       |*|",0);
write_to_buffer(d,"\n\r     '   .'#    /%%%%%  ...          |*|                                |*|",0);
write_to_buffer(d,"\n\r      `-'| (  / %%% .,''             |*| By Christian Tusborg           |*|",0);
write_to_buffer(d,"\n\r        ,'  `.-----------------------(@)--------------------------------(@)",0);
write_to_buffer(d,"\n\r       /     )- \\#### #### ##### ####### %%%% %%%%% %%%%% `..		",0);
write_to_buffer(d,"\n\r      ( , ^\\ \\   #### #|FATE OF KINGDOMS IV|# %%%% %%%%%%%%`""		",0);
write_to_buffer(d,"\n\r       I   /\\ \\    %### ####### ##### %%%%%%  %%%%% %%""'''		",0);
write_to_buffer(d,"\n\r       |  <  \\.\\       ""'##### ##%%%% %%""''				",0);
write_to_buffer(d,"\n\r      /'    )    \\                    					",0);
write_to_buffer(d,"\n\r    /     ,/'\\   ``,	(@)--------------------------------(@)		",0);
write_to_buffer(d,"\n\r   (   ,/`--,___,\\`+`.  |*|             Selecct:           |*|  	",0);
write_to_buffer(d,"\n\r    `v__---------\\ \\ `  (@)--------------------------------(@)		",0);
sprintf( buf, "\n\r              `\\ `\\`V	|*|              %-15s   |*|		",argument);
write_to_buffer(d,buf,0);
write_to_buffer(d,"\n\r                `\\`\\	(@)--------------------------------(@)		",0);
write_to_buffer(d,"\n\r                  `\\`\\. 							",0);
write_to_buffer(d,"\n\r                    `\\s\\`							",0);
write_to_buffer(d,"\n\r                       `V~							",0);
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


void roll_stat( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
char buf[MAX_INPUT_LENGTH];
int i;
	for (i=0; i < MAX_STATS;i++)
		ch->perm_stat[i] = number_range(1,30);
	
write_to_buffer(d,"\n\r",0);
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
return;
}

char *get_stat_alias( CHAR_DATA *ch, int which )
{
char *stat;
int istat;

    if ( which == STAT_STR)  {
      istat=ch->perm_stat[STAT_STR];
      if      ( istat >  29 ) stat = "Godly  ";
      else if ( istat >= 28 ) stat = "Supreme ";
      else if ( istat >= 26 ) stat = "Titanic   ";
      else if ( istat >= 24 ) stat = "Mighty   ";
      else if ( istat >= 22 ) stat = "Powerful   ";
      else if ( istat >= 19 ) stat = "Forceful    ";
      else if ( istat >= 16 ) stat = "Muscular    ";
      else if ( istat >= 13 ) stat = "Average    ";
      else if ( istat >= 10 ) stat = "Poor    ";
      else if ( istat >= 7 ) stat = "Wimpy    ";
      else if ( istat >= 4 ) stat = "Weakling    ";
      else                    stat = "Puny ";
      return(stat);
    }

    if ( which == STAT_WIS)  {
      istat=ch->perm_stat[STAT_WIS];
      if      ( istat >  29 ) stat = "Mystical  ";
      else if ( istat >= 28 ) stat = "Inconceivable ";
      else if ( istat >= 26 ) stat = "Transcendent   ";
      else if ( istat >= 24 ) stat = "Enlightened   ";
      else if ( istat >= 22 ) stat = "Wise   ";
      else if ( istat >= 19 ) stat = "Profound    ";
      else if ( istat >= 16 ) stat = "Insightful    ";
      else if ( istat >= 13 ) stat = "Average    ";
      else if ( istat >= 10 ) stat = "Forgetful    ";
      else if ( istat >= 7 ) stat = "Naive    ";
      else if ( istat >= 4 ) stat = "Moronic    ";
      else                    stat = "Huh? ";
      return(stat);
    }

    if ( which == STAT_CON)  {
      istat=ch->perm_stat[STAT_CON];
      if      ( istat >  29 ) stat = "Ideal  ";
      else if ( istat >= 28 ) stat = "Flawless ";
      else if ( istat >= 26 ) stat = "Splendid   ";
      else if ( istat >= 24 ) stat = "Vigorous   ";
      else if ( istat >= 22 ) stat = "Tough   ";
      else if ( istat >= 19 ) stat = "Fit    ";
      else if ( istat >= 16 ) stat = "Sound    ";
      else if ( istat >= 13 ) stat = "Average    ";
      else if ( istat >= 10 ) stat = "Vulnerable    ";
      else if ( istat >= 7 ) stat = "Fragile    ";
      else if ( istat >= 4 ) stat = "Shatterable    ";
      else                    stat = "Deteriorating ";
      return(stat);
    }

    if ( which == STAT_INT)  {
      istat=ch->perm_stat[STAT_INT];
      if      ( istat >  29 ) stat = "Absolute  ";
      else if ( istat >= 28 ) stat = "Ingenious ";
      else if ( istat >= 26 ) stat = "Lucent   ";
      else if ( istat >= 24 ) stat = "Brilliant   ";
      else if ( istat >= 22 ) stat = "Exceptional  ";
      else if ( istat >= 19 ) stat = "Gifted    ";
      else if ( istat >= 16 ) stat = "Perceptive    ";
      else if ( istat >= 13 ) stat = "Average    ";
      else if ( istat >= 10 ) stat = "Thick    ";
      else if ( istat >= 7 ) stat = "Foolish    ";
      else if ( istat >= 4 ) stat = "Imbecilic    ";
      else                    stat = "Gu-gu-gu ";
      return(stat);
    }

    if ( which == STAT_DEX)  {
      istat=ch->perm_stat[STAT_DEX];
      if      ( istat >  29 ) stat = "Unlimited  ";
      else if ( istat >= 28 ) stat = "Sublime ";
      else if ( istat >= 26 ) stat = "Extraordinary   ";
      else if ( istat >= 24 ) stat = "Proficient   ";
      else if ( istat >= 22 ) stat = "Deft  ";
      else if ( istat >= 19 ) stat = "Nimble    ";
      else if ( istat >= 16 ) stat = "Handy    ";
      else if ( istat >= 13 ) stat = "Average    ";
      else if ( istat >= 10 ) stat = "Unskilled    ";
      else if ( istat >= 7 ) stat = "Fumbling    ";
      else if ( istat >= 4 ) stat = "Inept    ";
      else                    stat = "Trollish ";
      return(stat);
    }

    if ( which == STAT_AGI)  {
      istat=ch->perm_stat[STAT_AGI];
      if      ( istat >  29 ) stat = "Unearthly  ";
      else if ( istat >= 28 ) stat = "Sovereign ";
      else if ( istat >= 26 ) stat = "Blazing   ";
      else if ( istat >= 24 ) stat = "Swift   ";
      else if ( istat >= 22 ) stat = "Quick  ";
      else if ( istat >= 19 ) stat = "Diligent    ";
      else if ( istat >= 16 ) stat = "Energetic    ";
      else if ( istat >= 13 ) stat = "Average    ";
      else if ( istat >= 10 ) stat = "Graceless    ";
      else if ( istat >= 7 ) stat = "Awkward    ";
      else if ( istat >= 4 ) stat = "Stumbling    ";
      else                    stat = "Elephantine ";
      return(stat);
    }

    if ( which == STAT_CHA)  {
      istat=ch->perm_stat[STAT_CHA];
      if      ( istat >  29 ) stat = "Angelic  ";
      else if ( istat >= 28 ) stat = "Exalted ";
      else if ( istat >= 26 ) stat = "Irresistible   ";
      else if ( istat >= 24 ) stat = "Majestic   ";
      else if ( istat >= 22 ) stat = "Captivating  ";
      else if ( istat >= 19 ) stat = "Charming    ";
      else if ( istat >= 16 ) stat = "Pleasant    ";
      else if ( istat >= 13 ) stat = "Average    ";
      else if ( istat >= 10 ) stat = "Unnotable    ";
      else if ( istat >= 7 ) stat = "Unpleasant    ";
      else if ( istat >= 4 ) stat = "Foul    ";
      else                    stat = "Repulsives ";
      return(stat);
    }

   bug( "Stat_alias: Bad stat number.", 0 );
   return(NULL);

} 

void class_list_select(CHAR_DATA *ch)
{

char buf[MAX_INPUT_LENGTH];
int  class_no,col;
    col = 0;
    send_to_char("\n\r{xSubclasses will become available later. Read Help subclass.\n\rYou can select:{x\n\r\n\r",ch);          
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
    return;
}

void race_list(CHAR_DATA *ch)
{

char buf[MAX_INPUT_LENGTH];
int  race_no,col;

    col = 0;
    send_to_char("\n\rThe following races are available:\n\r", ch);
    for (race_no = 1; race_no < MAX_PC_RACE-1; race_no++)
     {
       sprintf( buf, "%-15s", pc_race_table[race_no].name );
       send_to_char( buf,ch );
       if ( ++col % 4 == 0 )
	send_to_char( "\n\r", ch );     	
     }
    return;
}

void Creation_name(DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];	
	switch ( *argument )
	  {
		//Creation a new player
		case 'y': case 'Y':
			//sprintf( buf, "New character.\n\rGive me a password for %s: %s",ch->name, (char *) echo_off_str );
			sprintf( buf, "New character.\n\rGive me a password for %s:",ch->name);
			write_to_buffer( d, buf, 0 );
			d->connected = CON_GET_NEW_PASSWORD;
		break;

		//Oops did type wrong name
		case 'n': case 'N':
			write_to_buffer( d, "Ok, what IS it, then? ", 0 );
			free_char( d->character );
			d->character = NULL;
			d->connected = CON_GET_NAME;
		break;

		//Did't Yes or NO
		default:
			write_to_buffer( d, "Please type Yes or No? ", 0 );
		break;
	}
}

void Creation_password( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
  char *pwdnew;
  char *p;
	//New line
	write_to_buffer( d, "\n\r", 2 );

	//Is the password long enuff?
	if ( strlen(argument) < 5 )
	  {
		write_to_buffer( d,"Password must be at least five characters long.\n\rPassword: ",0 );
		return;
	  }

	//Crypting new password
	pwdnew = crypt( argument, ch->name );
	
	//Check if password ok
	for ( p = pwdnew; *p != '\0'; p++ )
	  {
		if ( *p == '~' )
		  {
			write_to_buffer( d,"New password not acceptable, try again.\n\rPassword: ",0 );
			return;
		  }
	  }

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );
	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
}

void Creation_confirm( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
	write_to_buffer( d, "\n\r", 2 );

	//Check if password is the same
	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	  {
		write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",0 );
		d->connected = CON_GET_NEW_PASSWORD;
		return;
	  }
	
	write_to_buffer(d,"\n\r",0);
	//write_to_buffer( d, (char *) echo_on_str, 0 );

	do_clear_login(d,ch,"Race");
	
	//Write race list
	race_list(ch);
	write_to_buffer(d,"\n\r",0);
	write_to_buffer(d,"What are you? ",0);
	d->connected = CON_GET_NEW_RACE;
}

void Creation_race( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  int race;
	one_argument(argument,arg);

	if (!str_cmp(arg,"help"))
	  {
		argument = one_argument(argument,arg);
	    
		if (argument[0] == '\0')
		  {
			do_clear_login(d,ch,"Race");
			race_list(ch);
		  }
		else
		  {
			do_clear_login(d,ch,"Race");
			do_help(ch,argument);
		  }
           
		return;
	  }

	race = race_lookup(argument);

	if (race == 0 || !race_table[race].pc_race)
	  {
		do_clear_login(d,ch,"Race");
		race_list(ch);
		write_to_buffer(d,"\n\rThat was not a valid race.\n\r",0);
		write_to_buffer(d,"\n\r",0);
            	write_to_buffer(d,"What is your race? (help for more information) ",0);
		return;
	  }

        ch->race = race;

	ch->affected_by = ch->affected_by|race_table[race].aff;
	ch->affected_by2= ch->affected_by2|race_table[race].aff2;
	ch->detection   = ch->detection|race_table[race].detect;
	ch->imm_flags	= ch->imm_flags|race_table[race].imm;
	ch->res_flags	= ch->res_flags|race_table[race].res;
	ch->vuln_flags	= ch->vuln_flags|race_table[race].vuln;
	ch->form	= race_table[race].form;
	ch->parts	= race_table[race].parts;

	ch->size = pc_race_table[race].size;

        write_to_buffer( d, "What is your sex (M/F)? ", 0 );
        d->connected = CON_GET_NEW_SEX;
}

void Creation_sex( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  int iClass;
  
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;
			    ch->pcdata->true_sex = SEX_MALE;
			    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;
			    ch->pcdata->true_sex = SEX_FEMALE;
			    break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat is your sex? ", 0 );
	    return;
	}

	do_clear_login(d,ch,"Class");
	class_list_select(ch);
       strcpy( buf, "\n\rSelect a class:\n\r" );
       write_to_buffer( d, buf, 0 );

        d->connected = CON_GET_NEW_CLASS;
}

void Creation_class( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
  int iClass,i;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  
	iClass = class_lookup(argument);
	argument = one_argument(argument,arg);

	if (!str_cmp(arg,"help"))
	  {
	    if (argument[0] == '\0') {
		do_clear_login(d,ch,"Class");
		class_list_select(ch);
	    }
	    else {
		do_clear_login(d,ch,"Class");
		do_help(ch,argument);
	    }
            write_to_buffer(d,"HELP CLASSES to list, or HELP <class> for specific information.\n\rWhat is your class? ",0);
	    return;
	  }

	if ( iClass == -1 )
	{
	    write_to_buffer( d,"\n\rThat's not a class.\n\rWhat is your class (help for more information)? ", 0 );
	    return;
	}

	if (pc_race_table[ch->race].class_mult[iClass] == -1)
	  {
	    do_clear_login(d,ch,"Class");
	    class_list_select(ch);
	    write_to_buffer( d,"\n\rThat class is not available for your race.\n\rChoose again: ",0);
	    return;
	  }
        ch->class = iClass;

	do_clear_login(d,ch,"Stats");
	roll_stat(d,ch,argument );
	d->connected = CON_ACCEPT_STATS;
}

void Creation_stats( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
  int i;
  char buf[MAX_STRING_LENGTH];
  
	switch( argument[0] )
	  {
	  case 'H': case 'h': case '?':
	    do_help(ch,"stats");
	    break;
	  case 'y': case 'Y':
	    for (i=0; i < MAX_STATS;i++)
	      ch->mod_stat[i] = 0;
	    write_to_buffer( d, "\n\r", 2 );

	    write_to_buffer( d, "You may be good, neutral, or evil.\n\r",0);
	    write_to_buffer( d, "Which alignment (G/N/E)? ",0);
	    d->connected = CON_GET_ALIGNMENT;
	    break;

	  case 'n': case 'N':
		do_clear_login(d,ch,"Stats");
		roll_stat(d,ch,argument );
	    	d->connected = CON_ACCEPT_STATS;
	    break;

	  default:
	    write_to_buffer(d,"Please answer (Y/N)? ",0);
	    break;
	  }
}

void Creation_alignment( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
	switch( argument[0])
	  {
	  case 'g' : case 'G' : ch->alignment = 750;
	    if (ch->race == race_lookup("ogre") || ch->race == race_lookup("giant")
	    || ch->race == race_lookup("darnok") || ch->race == race_lookup("draconian")
	    || ch->race == race_lookup("drow") || ch->race == race_lookup("duergar")
	    || ch->race == race_lookup("illithid") || ch->race == race_lookup("orc")
	    || ch->race == race_lookup("troll") || ch->race == race_lookup("felar")
	    || ch->race == race_lookup("gargoyle") || ch->class == class_lookup("assassin") || ch->class == class_lookup("bard")
	    || ch->class == class_lookup("black-knight") || ch->class == class_lookup("maligner")
	    || ch->class == class_lookup("druid") || ch->class == class_lookup("necromancer"))
	      {
		write_to_buffer(d,
"Your previous selections prevent you from being good.\n\rWhich alignment? ",0);
		d->connected = CON_GET_ALIGNMENT;
		break;
	      }
	    do_clear(ch,"");
	    do_help(ch, "hometown");
	    write_to_buffer(d, "\n\r", 0);
	    write_to_buffer(d,
			    "Which city do you want as your hometown?\n\r",0);
	    write_to_buffer(d,"Sanaria, New Thalos,Gondor city, or Ofcol [S/N/G/O]? ",0);
	    d->connected = CON_PICK_HOMETOWN;
	    break;

	  case 'n' : case 'N' : ch->alignment = 0;
	    if (ch->race == race_lookup("arial") || ch->race == race_lookup("merma")
	    || ch->race == race_lookup("troll") || ch->race == race_lookup("draconian")
	    || ch->race == race_lookup("drow") || ch->race == race_lookup("duergar")
	    || ch->race == race_lookup("illithid") || ch->race == race_lookup("orc")
	    || (ch->class == class_lookup("monk") && ch->race != race_lookup("felar")) || ch->class == class_lookup("assassin")
	    || ch->class == class_lookup("paladin") || ch->class == class_lookup("ranger")
	    || ch->class == class_lookup("black-knight") || ch->class == class_lookup("necromancer")
	    || ch->class == class_lookup("maligner"))
	     {
		write_to_buffer(d,
"Your previous selections prevent you from being neutral.\n\rWhich alignment? ",0);
		d->connected = CON_GET_ALIGNMENT;
		break;
	      }
	    do_clear(ch,"");
	    do_help(ch, "hometown");
	    write_to_buffer(d, "\n\r", 0);
	    write_to_buffer(d,
			    "Which city do you want as your hometown?\n\r",0);
	    write_to_buffer(d,"Sanaria, New Thalos,Gondor city, or Ofcol [S/N/G/O]? ",0);
	    d->connected = CON_PICK_HOMETOWN;
	    break;
	  case 'e' : case 'E' :
	    if (ch->race == race_lookup("arial") || ch->race == race_lookup("merma")
	    || ch->race == race_lookup("pixie") || ch->race == race_lookup("dwarf")
	    || ch->race == race_lookup("elf") || ch->race == race_lookup("gargoyle")
	    || ch->race == race_lookup("giant") || ch->class == class_lookup("inquisitor")
	    || ch->class == class_lookup("monk") || ch->class == class_lookup("bard")
	    || ch->class == class_lookup("paladin") || ch->class == class_lookup("ranger")
	    || ch->class == class_lookup("druid"))
	      {
		write_to_buffer(d,
"Your previous selections prevent you from being evil.\n\rWhich alignment?? ",0);
		d->connected = CON_GET_ALIGNMENT;
		break;
	      }
	    else
	      {
		ch->alignment = -750;
    	        do_clear(ch,"");
		do_help(ch, "hometown");
		write_to_buffer(d, "\n\r", 0);
		write_to_buffer(d,
			"Which city do you want as your hometown?\n\r",0);
		write_to_buffer(d,"Sanaria, New Thalos,Gondor city, or Ofcol [S/N/G/O]? ",
				0);
		d->connected = CON_PICK_HOMETOWN;
		break;
	      }
	  default:
	    write_to_buffer(d,"That's not a valid alignment.\n\r",0);
	    write_to_buffer(d,"Which alignment (G/N/E)? ",0);
	    break;
	  }
d->connected = CON_CREATE_DONE;
}

void Creation_hometown(DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
	switch(argument[0]) {
	case 'H' : case 'h' : case '?' : do_help(ch, "hometown"); break;
	case 'S' : case 's' : ch->hometown = 0;
	  if ( ch->class != 3 ) {
	    write_to_buffer(d, "\n\rWhich ethos do you want to follow, Lawful, Neutral or Chaotic? (or type help) ", 0);
	    d->connected = CON_GET_ETHOS;
	  } else {
	    ch->ethos = 1;
	    write_to_buffer(d,"You have a lawful ethos.\n\r",0);
	    write_to_buffer(d,"\n\rPress return to continue:\n\r",0);
	    d->connected = CON_CREATE_DONE;
	    break;
	  }
	  break;
	
	case 'N' : case
	'n' : ch->hometown = 1;
	  if ( ch->class != 3 ) {
	    write_to_buffer(d, "\n\rWhich ethos do you want to follow, Lawful, Neutral or Chaotic? (or type help) ", 0);
	    d->connected = CON_GET_ETHOS;
	  } else {
	    ch->ethos = 1;
	    write_to_buffer(d,"You have a lawful ethos.\n\r",0);
	    write_to_buffer(d,"\n\rPress return to continue:\n\r",0);
	    d->connected = CON_CREATE_DONE;
	  }
	  break;
	case 'R' : case 'r' :
	write_to_buffer(d,"This hometown is being revamped! ",0);
	return;

	  if (!IS_GOOD(ch)) {
	    write_to_buffer(d,
	       "\n\rThat is not a valid hometown for your alignment.\n\r",0);
	    write_to_buffer(d, "Which hometown do you want? ", 0);
	    return;
	  }
	  ch->hometown = 2;
	  if ( ch->class != 3 ) {
	    write_to_buffer(d, "\n\rWhich ethos do you want to follow, Lawful, Neutral or Chaotic? (or type help) ", 0);
	    d->connected = CON_GET_ETHOS;
	  } else {
	    ch->ethos = 1;
	    write_to_buffer(d,"You have a lawful ethos.\n\r",0);
	    write_to_buffer(d,"\n\rPress return to continue:\n\r",0);
	    d->connected = CON_CREATE_DONE;
	  }
	  break;
	  
	case 'O' : case 'o' :
	  if (!IS_NEUTRAL(ch)) {
	    write_to_buffer(d,
	       "\n\rThat is not a valid hometown for your alignment.\n\r",0);
	    write_to_buffer(d, "Which hometown do you want? ", 0);
	    return;
	  }
	  ch->hometown = 3;
 	  write_to_buffer(d, "\n\rWhich ethos do you want to follow, Lawful, Neutral or Chaotic? (or type help) ", 0);
	  d->connected = CON_GET_ETHOS;
	  break;
	case 'A' : case 'a' :

	write_to_buffer(d,"This hometown is being revamped! ",0);
	return;

	  if (!IS_EVIL(ch)) {
	    write_to_buffer(d,
	       "\n\rThat is not a valid hometown for your alignment.\n\r",0);
	    write_to_buffer(d, "Which hometown do you want? ", 0);
	    return;
	  }
	  ch->hometown = 4;
	  write_to_buffer(d, "\n\rWhich ethos do you want to follow, Lawful, Neutral or Chaotic? (or type help) ", 0);
	  d->connected = CON_GET_ETHOS;
	  break;
	case 'G' : case 'g' :
	  if (IS_EVIL(ch))
	    {
	    write_to_buffer(d,"\n\rThat is not a valid hometown for your alignment.\n\r",0);
	    write_to_buffer(d, "Which hometown do you want? ", 0);
	    return;
	   }
	  ch->hometown = 5;
	  if ( ch->class != 3 ) {
	    write_to_buffer(d, "\n\rWhich ethos do you want to follow, Lawful, Neutral or Chaotic? (or type help) ", 0);
	    d->connected = CON_GET_ETHOS;
	  } else {
	    ch->ethos = 1;
	    write_to_buffer(d,"You have a lawful ethos.\n\r",0);
	    write_to_buffer(d,"\n\rPress return to continue:\n\r",0);
	    d->connected = CON_CREATE_DONE;
	  }
	  break;	  
	default:
	  write_to_buffer(d, "\n\rThat is not a valid hometown.\n\r", 0);
	  write_to_buffer(d, "Which hometown do you want? ", 0);
	  return;
	}
}

void Creation_ethos( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
	switch(argument[0]) {
	case 'H': case 'h': case '?': do_help(ch, "alignment"); return; break;
	case 'L': case 'l': ch->ethos = 1; break;
	case 'N': case 'n': ch->ethos = 2; break;
	case 'C': case 'c': ch->ethos = 3; break;
	default:
	  write_to_buffer(d, "\n\rThat is not a valid ethos.\n\r", 0);
	  write_to_buffer(d, "What ethos do you want, (L/N/C)? ", 0);
	  return;
	}

	/* continue to CON_CREATE_DONE */
}

void deity_list(CHAR_DATA *ch)
{
/*
char buf[MAX_INPUT_LENGTH];
int  deity,col;

    col = 0;
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
     }*/
    return;
}
