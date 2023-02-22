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

#define NOTE_DIR  				 "../note"
#define DEF_NORMAL  0 /* No forced change, but default (any string)   */
#define DEF_INCLUDE 1 /* 'names' MUST be included (only ONE name!)    */
#define DEF_EXCLUDE 2 /* 'names' must NOT be included (one name only) */
#define MAX_BOARD 	 6
#define DEFAULT_BOARD 0 /* default board is board #0 in the boards      */
                        /* It should be readable by everyone!           */
#define MAX_LINE_LENGTH 72 
        /* enforce a max length of 80 on text lines,reject longer lines */
        /* This only applies in the Body of the note */
#define MAX_NOTE_TEXT (4*MAX_STRING_LENGTH - 1000)
#define BOARD_NOTFOUND -1 /* Error code from board_lookup() and board_number */
/* Data about a board */
typedef struct board_data BOARD_DATA;

struct board_data
{
	char *short_name; /* Max 8 chars */
	char *long_name;  /* Explanatory text, should be no more than 40 ?
chars */

	int read_level; /* minimum level to see board */
	int write_level;/* minimum level to post notes */

	char *names;       /* Default recipient */
	int force_type; /* Default action (DEF_XXX) */

	int purge_days; /* Default expiration */

	/* Non-constant data */

	NOTE_DATA *note_first; /* pointer to board's first note */
	bool changed; /* currently unused */

};

/* External variables */

extern BOARD_DATA boards[MAX_BOARD]; /* Declare */


/* Prototypes */

void finish_note (BOARD_DATA * board, NOTE_DATA *note); /* attach a note
to a board */
void free_note   (NOTE_DATA *note); /* deallocate memory used by a note */
void load_boards (void); /* load all boards */
int board_lookup (const char *name); /* Find a board with that name */
bool is_note_to (CHAR_DATA *ch, NOTE_DATA *note); /* is tha note to ch? */
void personal_message (const char *sender, const char *to, const char *subject, const int expire_days, const char *text);
void make_note (const char* board_name, const char *sender, const char *to, const char *subject, const int expire_days, const char *text);
void save_notes ();

/* for nanny */
void handle_con_note_to 		(DESCRIPTOR_DATA *d, char * argument);
void handle_con_note_subject 	(DESCRIPTOR_DATA *d, char * argument);
void handle_con_note_expire 	(DESCRIPTOR_DATA *d, char * argument);
void handle_con_note_text 		(DESCRIPTOR_DATA *d, char * argument);
void handle_con_note_finish 	(DESCRIPTOR_DATA *d, char * argument);


/* Commands */

DECLARE_DO_FUN (do_note		);
DECLARE_DO_FUN (do_board	);

