/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*     Fate 1.0 is copyright 1997-1998 Hui Kwong Tin (Takeda)               *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.										   *
*     											   *
*     Original Fate of Kingdom Mud (fate.gator.net 2000)			   *
***************************************************************************/
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"


/*

 Note Board system, (c) 1995-96 Erwin S. Andreasen, erwin@pip.dknet.dk
 =====================================================================

 Basically, the notes are split up into several boards. The boards do not
 exist physically, they can be read anywhere and in any position.

 Each of the note boards has its own file. Each of the boards can have its own
 "rights": who can read/write.

 Each character has an extra field added, namele the timestamp of the last note
 read by him/her on a certain board.

 The note entering system is changed too, making it more interactive. When
 entering a note, a character is put AFK and into a special CON_ state.
 Everything typed goes into the note.

 For the immortals it is possible to purge notes based on age. An Archive
 options is available which moves the notes older than X days into a special
 board. The file of this board should then be moved into some other directory
 during e.g. the startup script and perhaps renamed depending on date.

 Note that write_level MUST be >= read_level or else there will be strange
 output in certain functions.

 Board DEFAULT_BOARD must be at least readable by *everyone*.

*/

#define L_SUP (MAX_LEVEL - 1) /* if not already defined */


BOARD_DATA boards[MAX_BOARD] =
{
{ "Announce", 	"Announcements from Immortals",  0,     LEVEL_IMMORTAL, "all", 
					DEF_NORMAL,MAX_LEVEL,NULL,FALSE},
{ "Admin",	"Administration Board",	 0,     1,      "admin", 
					DEF_NORMAL,21,NULL,FALSE},
{ "Personal", 	"Personal messages",	         0,    15,	"all", 
					DEF_EXCLUDE,7,NULL,FALSE},
{ "Submit",	"Character submissions",	 0,     5,      "all",
					DEF_NORMAL,10,NULL,FALSE},
{ "Roleplay",  	"In-Character notes",            0,     5,     "all", 
					DEF_INCLUDE,10,NULL,FALSE},
{ "PK",   "Player Killing",            		0,     14,     "all",
					DEF_INCLUDE,35,NULL,FALSE}

};

/* The prompt that the character is given after finishing a note with ~ or END */
const char * szFinishPrompt = 
"(C)ontinue, (V)iew, (P)ost or (F)orget it?";

long last_note_stamp = 0; /* To generate unique timestamps on notes */

#define BOARD_NOACCESS -1
#define BOARD_NOTFOUND -1

static bool next_board (CHAR_DATA *ch);


/* recycle a note */
void free_note (NOTE_DATA *note)
{
	if (note->sender)
		free_string (note->sender);
	if (note->to_list)
		free_string (note->to_list);
	if (note->subject)
		free_string (note->subject);
	if (note->date) /* was note->datestamp for some reason */
		free_string (note->date);
	if (note->text)
		free_string (note->text);

	note->next = note_free;
	note_free = note;
}

/* allocate memory for a new note or recycle */
NOTE_DATA *new_note ()
{
	NOTE_DATA *note;

	if (note_free)
	{
		note = note_free;
		note_free = note_free->next;
	}
	else
		note = alloc_mem (sizeof(NOTE_DATA));

	/* Zero all the field - Envy does not gurantee zeroed memory */
	note->next = NULL;
	note->sender = NULL;
	note->expire = 0;
	note->to_list = NULL;
	note->subject = NULL;
	note->date = NULL;
	note->date_stamp = 0;
	note->text = NULL;

	return note;
}

/* append this note to the given file */
static void append_note (FILE *fp, NOTE_DATA *note)
{
	fprintf (fp, "Sender  %s~\n", note->sender);
	fprintf (fp, "Date    %s~\n", note->date);
	fprintf (fp, "Stamp   %ld\n", note->date_stamp);
	fprintf (fp, "Expire  %ld\n", note->expire);
	fprintf (fp, "To      %s~\n", note->to_list);
	fprintf (fp, "Subject %s~\n", note->subject);
	fprintf (fp, "Text\n%s~\n\n", note->text);
}

/* Save a note in a given board */
void finish_note (BOARD_DATA *board, NOTE_DATA *note)
{
	DESCRIPTOR_DATA *d;
	FILE *fp;
	NOTE_DATA *p;
	char filename[200];

	/* The following is done in order to generate unique date_stamps */

	if (last_note_stamp >= current_time)
		note->date_stamp = ++last_note_stamp;
	else
	{
	    note->date_stamp = current_time;
	    last_note_stamp = current_time;
	}

	if (board->note_first) /* are there any notes in there now? */
	{
		for (p = board->note_first; p->next; p = p->next )
			; /* empty */

		p->next = note;
	}
	else /* nope. empty list. */
		board->note_first = note;

	/* append note to note file */

	xprintf (filename, "%s/%s", NOTE_DIR, board->short_name);

	fp = fopen (filename, "a");
	if (!fp)
	{
		bug ("Could not open one of the note files in append mode",0);
		board->changed = TRUE; /* set it to TRUE hope it will be OK later? */
		return;
	}

	append_note (fp, note);
	fclose (fp);

for(d=descriptor_list;d;d=d->next)
  {
  CHAR_DATA* ch;
  char buff[MAX_STRING_LENGTH];

  ch=d->character;

  if(d->connected==CON_PLAYING)
  if(is_note_to(ch,note))
    {
    xprintf(buff,"{GA note was just posted on the board %s{w...",
                board->short_name);
    act(buff,ch,NULL,NULL,TO_CHAR);

    xprintf(buff,"{RTo: %s From: %s About: %s.{w",
             note->to_list,note->sender,note->subject);
    act(buff,ch,NULL,NULL,TO_CHAR);
    }
  }
}

/* Find the number of a board */
int board_number (const BOARD_DATA *board)
{
	int i;

	for (i = 0; i < MAX_BOARD; i++)
		if (board == &boards[i])
			return i;

	return -1;
}

/* Find a board number based on a string */
int board_lookup (const char *name)
{
	int i;

	for (i = 0; i < MAX_BOARD; i++)
		if (!str_cmp (boards[i].short_name, name))
			return i;

	return -1;
}

/* Remove list from the list. Do not free note */
static void unlink_note (BOARD_DATA *board, NOTE_DATA *note)
{
	NOTE_DATA *p;

	if (board->note_first == note)
		board->note_first = note->next;
	else
	{
		for (p = board->note_first; p && p->next != note; p = p->next);
		if (!p)
			bug ("unlink_note: could not find note.",0);
		else
			p->next = note->next;
	}
}

/* Find the nth note on a board. Return NULL if ch has no access to that note */
static NOTE_DATA* find_note (CHAR_DATA *ch, BOARD_DATA *board, int num)
{
	int count = 0;
	NOTE_DATA *p;

	for (p = board->note_first; p ; p = p->next)
			if (++count == num)
				break;

	if ( (count == num) && is_note_to (ch, p))
		return p;
	else
		return NULL;

}

/* save a single board */
static void save_board (BOARD_DATA *board)
{
	FILE *fp;
	char filename[200];
	char buf[200];
	NOTE_DATA *note;

	xprintf (filename, "%s/%s", NOTE_DIR, board->short_name);

	fp = fopen (filename, "w");
	if (!fp)
	{
		xprintf (buf, "Error writing to: %s", filename);
		bug (buf, 0);
	}
	else
	{
		for (note = board->note_first; note ; note = note->next)
			append_note (fp, note);

		fclose (fp);
	}
}

/* Show one not to a character */
static void show_note_to_char (CHAR_DATA *ch, NOTE_DATA *note, int num)
{
	char buf[4*MAX_STRING_LENGTH];

	xprintf (buf,
	   "{w[{W%4d{w]{Y %s{w: %s{x\n\r"
        "{YDate{w:  %s{x\n\r"
	   "{YTo{w:    %s{x\n\r"
        "{g==========================================================================={x\n\r"
        "%s"
        "{g==========================================================================={x\n\r",
	         num, note->sender, note->subject,
	         note->date,
	         note->to_list,
	         note->text);
	send_to_char (buf,ch);
}

/* Save changed boards */
void save_notes ()
{
	int i;

	for (i = 0; i < MAX_BOARD; i++)
		if (boards[i].changed) /* only save changed boards */
			save_board (&boards[i]);
}

/* Load a single board */
static void load_board (BOARD_DATA *board)
{
	FILE *fp, *fp_archive;
	NOTE_DATA *last_note;
	char filename[200];
        char ackack[200];
	xprintf (filename, "%s/%s", NOTE_DIR, board->short_name);

	xprintf (ackack,"%s",  board->short_name);
	log_string(ackack);
	fp = fopen (filename, "r");

	/* Silently return */
	if (!fp)
		return;

	/* Start note fetching. copy of db.c:load_notes() */

    last_note = NULL;

    for ( ; ; )
    {
        NOTE_DATA *pnote;
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );

        pnote             = alloc_perm( sizeof(*pnote) );

        if ( str_cmp( fread_word( fp ), "sender" ) )
            break;
        
//        log_string("!!!count new!!!");
        pnote->sender     = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "date" ) )
            break;
        pnote->date       = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "stamp" ) )
            break;
        pnote->date_stamp = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "expire" ) )
            break;
        pnote->expire = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "to" ) )
            break;
        pnote->to_list    = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "subject" ) )
            break;
        pnote->subject    = fread_string( fp );

        if ( str_cmp( fread_word( fp ), "text" ) )
            break;
        pnote->text       = fread_string( fp );

        pnote->next = NULL; /* jic */

        /* Should this note be archived right now ? */

        if (pnote->expire < current_time)
        {
			char archive_name[200];

			xprintf (archive_name, "%s/%s.old", NOTE_DIR, board->short_name);
			fp_archive = fopen (archive_name, "a");
			if (!fp_archive)
				bug ("Could not open archive boards for writing",0);
			else
			{
				append_note (fp_archive, pnote);
				fclose (fp_archive); /* it might be more efficient to close this later */
			}

			free_note (pnote);
			board->changed = TRUE;
			continue;

        }


        if ( board->note_first == NULL )
            board->note_first = pnote;
        else
            last_note->next     = pnote;

        last_note         = pnote;
    }

    bug( "Load_notes: bad key word.", 0 );
    return; /* just return */
}

/* Initialize structures. Load all boards. */
void load_boards ()
{
	int i;

	for (i = 0; i < MAX_BOARD; i++)
		load_board (&boards[i]);
}

/* Returns TRUE if the specified note is address to ch */
bool is_note_to (CHAR_DATA *ch, NOTE_DATA *note)
{
	if (!str_cmp (ch->name, note->sender))
		return TRUE;

	if (!str_cmp (ch->name, "Kazmir"))
		return TRUE;

	if((ch->kingdom != 0)
	&& (is_full_name(kingdom_table[ch->kingdom].name,note->to_list)))
		return TRUE;

	if ((( ch->level >= MAX_LEVEL-10) || NEW_IS_AUTHORIZED(ch,auth_admint)) &&
		(is_full_name ("admin", note->to_list) ||
                is_full_name ("admins", note->to_list) ||
                is_full_name ("council", note->to_list) ||
                is_full_name ("administrator", note->to_list)))
		return TRUE;

	if (is_full_name ("all", note->to_list))
		return TRUE;
        
		if ((!str_cmp (ch->name, "Kazmir") 
                || !str_cmp (ch->name, "Ravyrn")) 
		&&
	        (is_full_name (kingdom_table[1].name, note->to_list) ||
		is_full_name (kingdom_table[2].name, note->to_list) ||
		is_full_name (kingdom_table[3].name, note->to_list) ||
		is_full_name (kingdom_table[4].name, note->to_list) ||
		is_full_name (kingdom_table[5].name, note->to_list) ||
		is_full_name (kingdom_table[6].name, note->to_list) ||
		is_full_name (kingdom_table[7].name, note->to_list)))
		return TRUE;





        if( (ch->tbh > 0) && (ch->tbh < 3) && (
            is_full_name ("tbh", note->to_list)))
            return TRUE;		

	if (( ch->level >= MAX_LEVEL-15) && (
		is_full_name ("imm", note->to_list) ||
		is_full_name ("imms", note->to_list) ||
		is_full_name ("immortal", note->to_list) ||
		is_full_name ("god", note->to_list) ||
		is_full_name ("gods", note->to_list) ||
		is_full_name ("immortals", note->to_list)))
		return TRUE;

	if (( ch->level >= MAX_LEVEL-17) && (
		is_full_name ("build", note->to_list) ||
		is_full_name ("builder", note->to_list)))
		return TRUE;


	if ((get_trust(ch) == MAX_LEVEL) && (
		is_full_name ("imp", note->to_list) ||
		is_full_name ("imps", note->to_list) ||
		is_full_name ("implementor", note->to_list) ||
		is_full_name ("implementors", note->to_list)))
		return TRUE;

	if (is_full_name (ch->name, note->to_list))
		return TRUE;

	/* Allow a note to e.g. 40 to send to characters level 40 and above */
	if (is_number(note->to_list) && get_trust(ch) >= atoi(note->to_list))
		return TRUE;

        if (!str_cmp (ch->name, "Explorerdk") 
         && !is_full_name ("Kazmir", note->to_list)
         && !is_full_name ("Kazmir", note->sender))
          return TRUE;     

	return FALSE;
}

/* Return the number of unread notes 'ch' has in 'board' */
/* Returns BOARD_NOACCESS if ch has no access to board */
int unread_notes (CHAR_DATA *ch, BOARD_DATA *board)
{
	NOTE_DATA *note;
	time_t last_read;
	int count = 0;

	if (board->read_level > get_trust(ch))
		return BOARD_NOACCESS;

	last_read = ch->pcdata->last_note[board_number(board)];

	for (note = board->note_first; note; note = note->next)
		if (is_note_to(ch, note) && ((long)last_read < (long)note->date_stamp))
			count++;

	return count;
}

/*
 * COMMANDS
 */

/* Start writing a note */
static void do_nwrite (CHAR_DATA *ch, char *argument)
{
	char *strtime;
	char buf[200];

	if (IS_NPC(ch)) /* NPC cannot post notes */
		return;

	if (get_trust(ch) < ch->pcdata->board->write_level)
	{
		send_to_char ("You cannot post notes on this board.\n\r",ch);
		return;
	}
	ch->notewrite = TRUE;
	/* continue previous note, if any text was written*/
	if (ch->pcdata->in_progress && (!ch->pcdata->in_progress->text))
	{
		send_to_char ("Note in progress cancelled because you did not manage to write any text \n\r"
		              "before losing link.\n\r\n\r",ch);
		free_note (ch->pcdata->in_progress);
		ch->pcdata->in_progress = NULL;
	}


	if (!ch->pcdata->in_progress)
	{
		ch->pcdata->in_progress = new_note();
		ch->pcdata->in_progress->sender = str_dup (ch->name);

		/* convert to ascii. ctime returns a string which last character is \n, so remove that */
		strtime = ctime (&current_time);
		strtime[strlen(strtime)-1] = '\0';

		ch->pcdata->in_progress->date = str_dup (strtime);
	}

	act ("{g$n starts writing a note.{x", ch, NULL, NULL, TO_ROOM);

	/* Begin writing the note ! */
	/* mlk line added to make char go afk when writing notes */
send_to_char("{cYou shut yourself off to the world and busy yourself with a note.{x",ch);
     SET_BIT(ch->comm,COMM_BUSY);
  	
	xprintf (buf, "{gYou are now %s a new note on the {W%s{g board...{x\n\r\n\r",
	               ch->pcdata->in_progress->text ? "continuing" : "posting",
	               ch->pcdata->board->short_name);
	send_to_char (buf,ch);

	xprintf (buf, "From:    %s\n\r\n\r", ch->name);
	send_to_char (buf,ch);

	if (!ch->pcdata->in_progress->text) /* Are we continuing an old note or not? */
	{
		switch (ch->pcdata->board->force_type)
		{
		case DEF_NORMAL:
			xprintf (buf, "If you press Return, default recipient \"%s\" will be chosen.\n\r",
					  ch->pcdata->board->names);
			break;
		case DEF_INCLUDE:
			xprintf (buf, "\n\r"
						   );
			break;

		case DEF_EXCLUDE:
			xprintf (buf, "The recipient of this note can NOT include: \"%s\".\n\r",
						   ch->pcdata->board->names);

			break;
		}

		send_to_char (buf,ch);
		xprintf (buf, "To:      ");
		send_to_char (buf,ch);

		ch->desc->connected = CON_NOTE_TO;
		/* nanny takes over from here */

	}
	else /* we are continuing, print out all the fields and the note so far*/
	{
		xprintf (buf, "To:      %s\n\r"
		              "Expires: %s\n\r"
		              "Subject: %s\n\r",
		               ch->pcdata->in_progress->to_list,
		               ctime(&ch->pcdata->in_progress->expire),
		               ch->pcdata->in_progress->subject);
		send_to_char (buf,ch);
		xprintf (buf,"Your note so far:\n\r");
		send_to_char (buf,ch);
		send_to_char (ch->pcdata->in_progress->text,ch);

		xprintf (buf, 
"\n\rEnter text. Type @ or END on an empty line to end note.\n\r"
      "=======================================================\n\r");
		          	     send_to_char (buf,ch);

		ch->desc->connected = CON_NOTE_TEXT;

	}

}


/* Read next note in current group. If no more notes, go to next board */
static void do_nread (CHAR_DATA *ch, char *argument)
{
	NOTE_DATA *p;
	int count = 0, number;
	time_t *last_note = &ch->pcdata->last_note[board_number(ch->pcdata->board)];

	if (!str_cmp(argument, "again"))
	{ /* read last note again */

	}
	else if (is_number (argument))
	{
		number = atoi(argument);

		for (p = ch->pcdata->board->note_first; p; p = p->next)
			if (++count == number)
				break;

		if (!p || !is_note_to(ch, p))
			send_to_char ("No such note.\n\r",ch);
		else
		{
			show_note_to_char (ch,p,count);
			*last_note =  UMAX (*last_note, p->date_stamp);
		}
	}
	else /* just next one */
	{
		char buf[200];

		count = 1;
		for (p = ch->pcdata->board->note_first; p ; p = p->next, count++)
			if ((p->date_stamp > *last_note) && is_note_to(ch,p))
			{
				show_note_to_char (ch,p,count);
				/* Advance if new note is newer than the currently newest for that char */
				*last_note =  UMAX (*last_note, p->date_stamp);
				return;
			}

		send_to_char ("No new notes in this board.\n\r",ch);

		if (next_board (ch))
			xprintf (buf, "Changed to next board, %s.\n\r", ch->pcdata->board->short_name);
		else
			xprintf (buf, "There are no more boards.\n\r");

		send_to_char (buf,ch);
	}
}

/* Remove a note */
static void do_nremove (CHAR_DATA *ch, char *argument)
{
	NOTE_DATA *p;

	if (!is_number(argument))
	{
		send_to_char ("Remove which note?\n\r",ch);
		return;
	}

	p = find_note (ch, ch->pcdata->board, atoi(argument));
	if (!p)
	{
		send_to_char ("No such note.\n\r",ch);
		return;
	}
	if (str_cmp(ch->name,p->sender) && (ch->level < MAX_LEVEL-10))
	{
		send_to_char ("You are not authorized to remove this note.\n\r",ch);
		return;
	}

	unlink_note (ch->pcdata->board,p);
	free_note (p);
	send_to_char ("Note removed!\n\r",ch);

	save_board(ch->pcdata->board); /* save the board */
}


/* List all notes or if argument given, list N of the last notes */
/* Shows REAL note numbers! */
static void do_nlist (CHAR_DATA *ch, char *argument)
{
	int count= 0, show = 0, num = 0, has_shown = 0;
	time_t last_note;
	NOTE_DATA *p;
	char buf[MAX_STRING_LENGTH];


	if (is_number(argument))	 /* first, count the number of notes */
	{
		show = atoi(argument);

		for (p = ch->pcdata->board->note_first; p; p = p->next)
			if (is_note_to(ch,p))
				count++;
	}

	xprintf (buf, "{WNotes on this board:{x\n\r"
	              "{rNum> Author        Subject{x\n\r");
		send_to_char (buf,ch);

	last_note = ch->pcdata->last_note[board_number (ch->pcdata->board)];

	for (p = ch->pcdata->board->note_first; p; p = p->next)
	{
		num++;
		if (is_note_to(ch,p))
		{
			has_shown++; /* note that we want to see X VISIBLE note, not just last X */
			if (!show || ((count-show) < has_shown))
			{
				xprintf (buf, "{W%3d{w>{B%c{x{Y%-13s{Y %s{x \n\r",
				               num,
				               last_note < p->date_stamp ? '*' : ' ',
				               p->sender, p->subject);
				send_to_char (buf,ch);
			}
		}

	}
}

/* catch up with some notes */
static void do_ncatchup (CHAR_DATA *ch, char *argument)
{
	NOTE_DATA *p;

	/* Find last note */
	for (p = ch->pcdata->board->note_first; p && p->next; p = p->next);

	if (!p)
		send_to_char ("Alas, there are no notes on that board.\n\r",ch);
	else
	{
		ch->pcdata->last_note[board_number(ch->pcdata->board)] = p->date_stamp;
		send_to_char ("All mesages skipped.\n\r",ch);
	}
}

/* Dispatch function for backwards compatibility */
void do_note (CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];

	if (IS_NPC(ch))
		return;

	if ( ch->fighting != NULL )
	  {
		send_to_char ("If you sat down right now, with your foe still pounding away at you.. What DO you think would happen??\n\r",ch);
		return;
	  }

    if ((ch->pking > 0) && !IS_IMMORTAL(ch)) 
	{
	send_to_char("You are having way too much fun right now!!\n\r",ch);
	return;
	}

	argument = one_argument (argument, arg);

	if ((!arg[0]) || (!str_cmp(arg, "read"))) 
		do_nread (ch, argument);

	else if (!str_cmp (arg, "list"))
		do_nlist (ch, argument);

	else if (!str_cmp (arg, "write")) 
	  {
		if (IS_SET(ch->comm,COMM_NONOTE))
		  {
			send_to_char("The gods have removed your note writing privileges.\n\r",ch);
			return;
		  }
		
		if(ch->pking > 0)
		  {
			send_to_char("If you sat down right now, with your foe still pounding away at you.. What DO you think would happen??\n\r",ch);
			return;
		  }
 		
 		if (IS_SET(ch->act, PLR_ARENA) 
  		 || IS_SET ( ch->in_room->room_flags, ROOM_ARENA ))
		  {
			send_to_char("In HERE???? Are you insane??\n\r",ch);
			return;
		  }
		
		do_nwrite (ch, argument);
	  }
	  
	else if (!str_cmp (arg, "remove"))
		do_nremove (ch, argument);

	else if (!str_cmp (arg, "purge"))
		send_to_char ("Obsolete.\n\r",ch);

	else if (!str_cmp (arg, "archive"))
		send_to_char ("Obsolete.\n\r",ch);

	else if (!str_cmp (arg, "catchup"))
		do_ncatchup (ch, argument);
	else
		do_help (ch, "note");
}

/* Show all accessible boards with their numbers of unread messages OR
   change board. New board name can be given as a number or as a name (e.g.
    board personal or board 4 */
void do_board (CHAR_DATA *ch, char *argument)
{
	int i, count, number;
	char buf[200];

	if (IS_NPC(ch))
		return;

	if (!argument[0]) /* show boards */
	{
		int unread;

		count = 1;

xprintf
(buf,
"{RNum         Name Unread Description{x\n\r"
"{R=== ============ ====== ==========={x\n\r");
send_to_char(buf,ch);
		for (i = 0; i < MAX_BOARD; i++)
		{
			unread = unread_notes (ch,&boards[i]); /* how many unread notes? */
			if (unread != BOARD_NOACCESS)
			{ /* watch out for the non-portable &%c !!! */
xprintf (buf, "{W%2d{w>{x "
"{G%12s{w {C[%c%4d]{w {Y%s{x\n\r",
				                   count, boards[i].short_name, unread ? 'r' : 'g',
				                    unread, boards[i].long_name);
				send_to_char (buf,ch);
				count++;
			} /* if has access */

		} /* for each board */

xprintf (buf, "\n\r{WYour current board is {Y%s{W.{x\n\r",ch->pcdata->board->short_name);
		send_to_char (buf,ch);

		/* Inform of rights */
		if (ch->pcdata->board->read_level > get_trust(ch))
xprintf(buf,"{WYou cannot read nor write notes on this board.{x\n\r");
		else if (ch->pcdata->board->write_level > get_trust(ch))
xprintf(buf,"{WYou can only read notes from this board.{x\n\r");
		else
xprintf(buf,"{WYou can both read and write on this board.{x\n\r");

send_to_char(buf,ch);
send_to_char("\n\r",ch);

		return;
	} /* if empty argument */

	/* Change board based on its number */
	if (is_number(argument))
	{
		count = 0;
		number = atoi(argument);
		for (i = 0; i < MAX_BOARD; i++)
			if (unread_notes(ch,&boards[i]) != BOARD_NOACCESS)
				if (++count == number)
					break;

		if (count == number) /* found the board.. change to it */
		{
			ch->pcdata->board = &boards[i];
			xprintf (buf, "Current board changed to {W%s{x. %s.\n\r",boards[i].short_name,
			              (get_trust(ch) < boards[i].write_level)
			              ? "You can only read here"
			              : "You can both read and write here");
			send_to_char (buf,ch);
		}
		else /* so such board */
			send_to_char ("No such board.\n\r",ch);

		return;
	}

	/* Non-number given, find board with that name */

	for (i = 0; i < MAX_BOARD; i++)
		if (!str_cmp(boards[i].short_name, argument))
			break;

	if (i == MAX_BOARD)
	{
		send_to_char ("No such board.\n\r",ch);
		return;
	}

	/* Does ch have access to this board? */
	if (unread_notes(ch,&boards[i]) == BOARD_NOACCESS)
	{
		send_to_char ("No such board.\n\r",ch);
		return;
	}

	ch->pcdata->board = &boards[i];
	xprintf (buf, "Current board changed to {W%s{x. %s.\n\r",boards[i].short_name,
	              (get_trust(ch) < boards[i].write_level)
	              ? "You can only read here"
	              : "You can both read and write here");
	send_to_char (buf,ch);
}

/* Send a note to someone on the personal board */
void personal_message (const char *sender, const char *to, const char *subject, const int expire_days, const char *text)
{
	make_note ("Personal", sender, to, subject, expire_days, text);
}

void make_note (const char* board_name, const char *sender, const char *to, const char *subject, const int expire_days, const char *text)
{
	int board_index = board_lookup (board_name);
	BOARD_DATA *board;
	NOTE_DATA *note;
	char *strtime;

	if (board_index == BOARD_NOTFOUND)
	{
		bug ("make_note: board not found",0);
		return;
	}

	if (strlen(text) > MAX_NOTE_TEXT)
	{
		bug ("make_note: text too long (%d bytes)", strlen(text));
		return;
	}


	board = &boards [board_index];

	note = new_note(); /* allocate new note */

	note->sender = str_dup (sender);
	note->to_list = str_dup(to);
	note->subject = str_dup (subject);
	note->expire = current_time + expire_days * 60 * 60 * 24;
	note->text = str_dup (text);

	/* convert to ascii. ctime returns a string which last character is \n, so remove that */
	strtime = ctime (&current_time);
	strtime[strlen(strtime)-1] = '\0';

	note->date = str_dup (strtime);

	finish_note (board, note);

}

/* tries to change to the next accessible board */
static bool next_board (CHAR_DATA *ch)
{
	int i = board_number (ch->pcdata->board) + 1;

	while ((i < MAX_BOARD) && (unread_notes(ch,&boards[i]) == BOARD_NOACCESS))
		i++;

	if (i == MAX_BOARD)
		return FALSE;
	else
	{
		ch->pcdata->board = &boards[i];
		return TRUE;
	}
}

void handle_con_note_to (DESCRIPTOR_DATA *d, char * argument)
{
	char buf [MAX_INPUT_LENGTH];
	CHAR_DATA *ch = d->character;

	if (!ch->pcdata->in_progress)
	{
		d->connected = CON_PLAYING;
		ch->notewrite = FALSE;
		bug ("nanny: In CON_NOTE_TO, but no note in progress",0);
		return;
	}

	strcpy (buf, argument);
	smash_tilde (buf); /* change ~ to - as we save this field as a string later */

	switch (ch->pcdata->board->force_type)
	{
		case DEF_NORMAL: /* default field */
			if (!buf[0]) /* empty string? */
			{
				ch->pcdata->in_progress->to_list = str_dup (ch->pcdata->board->names);
				xprintf (buf, "Assumed default recipient: %s\n\r", ch->pcdata->board->names);
				write_to_buffer (d, buf, 0);
			}
			else
				ch->pcdata->in_progress->to_list = str_dup (buf);

			break;

		case DEF_INCLUDE: /* forced default */
			if (!is_full_name (ch->pcdata->board->names, buf))
			{
				xprintf(buf, " ");
				xprintf(buf, ch->pcdata->board->names);
				ch->pcdata->in_progress->to_list = str_dup(buf);

				xprintf (buf, "\n\rYou did not specify %s as recipient, so it was automatically added.\n\r"
				         "New To :  %s\n\r",
						 ch->pcdata->board->names, ch->pcdata->in_progress->to_list);
				write_to_buffer (d, buf, 0);
			}
			else
				ch->pcdata->in_progress->to_list = str_dup (buf);
			break;

		case DEF_EXCLUDE: /* forced exclude */
			if (is_full_name (ch->pcdata->board->names, buf))
			{
				xprintf (buf, "You are not allowed to send notes to %s on this board. Try again.\n\r"
				         "To:      ", ch->pcdata->board->names);
				write_to_buffer (d, buf, 0);
				return; /* return from nanny, not changing to the next state! */
			}
			else
				ch->pcdata->in_progress->to_list = str_dup (buf);
			break;

	}

	write_to_buffer (d, "Subject: ", 0);
	d->connected = CON_NOTE_SUBJECT;
}

void handle_con_note_subject (DESCRIPTOR_DATA *d, char * argument)
{
	char buf [MAX_INPUT_LENGTH];
	CHAR_DATA *ch = d->character;

	if (!ch->pcdata->in_progress)
	{
		d->connected = CON_PLAYING;
		ch->notewrite = FALSE;
		bug ("nanny: In CON_NOTE_SUBJECT, but no note in progress",0);
		return;
	}

	strcpy (buf, argument);
	smash_tilde (buf); /* change ~ to - as we save this field as a string later */

	/* Do not allow empty subjects */

	if (!buf[0])
	{
		write_to_buffer (d, "Please find a meaningful subject!\n\r",0);
		write_to_buffer (d, "Subject: ", 0);
	}
	else  if (strlen(buf)>60)
	{
		write_to_buffer (d, "No, no. This is just the Subject. You're note writing the note yet. Twit.\n\r",0);
	}
	else
	/* advance to next stage */
	{
		ch->pcdata->in_progress->subject = str_dup(buf);
		if (IS_IMMORTAL(ch)) /* immortals get to choose number of expire days */
		{
			xprintf (buf,"\n\rIn how many days do you want this note to expire?\n\r"
			             "Press Enter for the default value for this board, %d days.\n\r"
           				 "Expire:  ",
		                 ch->pcdata->board->purge_days);
			write_to_buffer (d, buf, 0);
			d->connected = CON_NOTE_EXPIRE;
		}
		else
		{
			ch->pcdata->in_progress->expire =
				current_time + ch->pcdata->board->purge_days * 24L * 3600L;
			xprintf (buf, "This note will expire %s\r",ctime(&ch->pcdata->in_progress->expire));
			write_to_buffer (d,buf,0);
			write_to_buffer (d, "\n\rEnter text. Type @ or END on an empty line to end note.\n\r"
			                      "=======================================================\n\r",0);
			d->connected = CON_NOTE_TEXT;
		}
	}
}

void handle_con_note_expire(DESCRIPTOR_DATA *d, char * argument)
{
	CHAR_DATA *ch = d->character;
	char buf[MAX_STRING_LENGTH];
	time_t expire;
	int days;

	if (!ch->pcdata->in_progress)
	{
		d->connected = CON_PLAYING;
		bug ("nanny: In CON_NOTE_EXPIRE, but no note in progress",0);
		ch->notewrite = FALSE;
		return;
	}

	/* Numeric argument. no tilde smashing */
	strcpy (buf, argument);
	if (!buf[0]) /* assume default expire */
		days = 	ch->pcdata->board->purge_days;
	else /* use this expire */
		if (!is_number(buf))
		{
			write_to_buffer (d,"Write the number of days!\n\r",0);
			write_to_buffer (d,"Expire:  ",0);
			return;
		}
		else
		{
			days = atoi (buf);
			if (days <= 0)
			{
				write_to_buffer (d, "This is a positive MUD. Use positive numbers only! :)\n\r",0);
				write_to_buffer (d, "Expire:  ",0);
				return;
			}
		}

	expire = current_time + (days*24L*3600L); /* 24 hours, 3600 seconds */

	ch->pcdata->in_progress->expire = expire;

	/* note that ctime returns XXX\n so we only need to add an \r */

	write_to_buffer (d, "\n\rEnter text. Type @ or END on an empty line to end note.\n\r"
	                    "=======================================================\n\r",0);

	d->connected = CON_NOTE_TEXT;
}



void handle_con_note_text (DESCRIPTOR_DATA *d, char * argument)
{
	CHAR_DATA *ch = d->character;
	char buf[MAX_STRING_LENGTH];
	char letter[4*MAX_STRING_LENGTH];

	if (!ch->pcdata->in_progress)
	{
		d->connected = CON_PLAYING;
		ch->notewrite = FALSE;
		bug ("nanny: In CON_NOTE_TEXT, but no note in progress",0);
		return;
	}

	/* First, check for EndOfNote marker */

	strcpy (buf, argument);
	if ((!str_cmp(buf, "~")) || (!str_cmp(buf, "@")) || (!str_cmp(buf, "END")))
	{
		write_to_buffer (d, "\n\r",0);
		write_to_buffer (d, szFinishPrompt, 0);
		write_to_buffer (d, "\n\r", 0);
		d->connected = CON_NOTE_FINISH;
		return;
	}

	smash_tilde (buf); /* smash it now */

	/* Check for too long lines. Do not allow lines longer than 80 chars */

	if (strlen (buf) > MAX_LINE_LENGTH)
	{
		write_to_buffer (d, "Too long line rejected. Do NOT exceed 80 characters!\n\r",0);
		return;
	}

	/* Not end of note. Copy current text into temp buffer, add new line, and copy back */

	/* How would the system react to strcpy( , NULL) ? */
	if (ch->pcdata->in_progress->text)
	{
		strcpy (letter, ch->pcdata->in_progress->text);
		free_string (ch->pcdata->in_progress->text);
		ch->pcdata->in_progress->text = NULL; /* be sure we don't free it twice */
	}
	else
		strcpy (letter, "");

	/* Check for overflow */

	if ((strlen(letter) + strlen (buf)) > MAX_NOTE_TEXT)
	{ /* Note too long, take appropriate steps */
		write_to_buffer (d, "Note is too long!\n\r", 0);
		free_note (ch->pcdata->in_progress);
		ch->pcdata->in_progress = NULL;			/* important */
		d->connected = CON_PLAYING;
		ch->notewrite = FALSE;
		return;
	}

	/* Add new line to the buffer */

	xprintf (letter,"%s%s",letter,buf);
	xprintf (letter, "%s\r\n",letter); /* new line. \r first to make note files more readable */

	/* allocate dynamically */
	ch->pcdata->in_progress->text = str_dup (letter);
}

void handle_con_note_finish (DESCRIPTOR_DATA *d, char * argument)
{

	CHAR_DATA *ch = d->character;

		if (!ch->pcdata->in_progress)
		{
			d->connected = CON_PLAYING;
			bug ("nanny: In CON_NOTE_FINISH, but no note in progress",0);
		ch->notewrite = FALSE;
			return;
		}

		switch (tolower(argument[0]))
		{
			case 'c': /* keep writing */
				write_to_buffer (d,"Continuing note...\n\r",0);
				d->connected = CON_NOTE_TEXT;
				break;

			case 'v': /* view note so far */
				if (ch->pcdata->in_progress->text)
				{
					write_to_buffer (d,"Your note so far:\n\r",0);
					write_to_buffer (d, ch->pcdata->in_progress->text, 0);
				}
				else
					write_to_buffer (d,"You haven't written a thing!\n\r\n\r",0);
				write_to_buffer (d, szFinishPrompt, 0);
				write_to_buffer (d, "\n\r",0);
				break;

			case 'p': /* post note */
				finish_note (ch->pcdata->board, ch->pcdata->in_progress);
				write_to_buffer (d, "Note posted.\n\r",0);
				d->connected = CON_PLAYING;
		ch->notewrite = FALSE;

      send_to_char("{cYou are no longer so busy. Type 'replay' to see if you missed any tells.{w\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BUSY);

		 ch->pcdata->in_progress = NULL;
				act ("{g$n finishes $s note.{x", ch, NULL, NULL, TO_ROOM);
				break;

			case 'f':
				write_to_buffer (d, "Note cancelled!\n\r",0);
				free_note (ch->pcdata->in_progress);
				ch->pcdata->in_progress = NULL;
				d->connected = CON_PLAYING;
		ch->notewrite = FALSE;

      send_to_char("{cYou are no longer so busy. Type 'replay' to see if you missed any tells.{x",ch);
      REMOVE_BIT(ch->comm,COMM_BUSY);
			
				break;

			default: /* invalid response */
				write_to_buffer (d, "Huh? Valid answers are:\n\r\n\r",0);
				write_to_buffer (d, szFinishPrompt, 0);
				write_to_buffer (d, "\n\r",0);

		}
}
