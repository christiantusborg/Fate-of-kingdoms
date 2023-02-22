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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "merc.h"

DECLARE_DO_FUN (do_help);

const char wizutil_id [] = "$Id: wizutil.c,v 1.6 1996/01/04 21:30:45 root Exp root $";

/* To have VLIST show more than vnum 0 - 9900, change the number below: */ 

#define MAX_SHOW_VNUM 193

#define NUL '\0'


extern ROOM_INDEX_DATA *       room_index_hash         [MAX_KEY_HASH]; /* db.c */

/* opposite directions */
const sh_int opposite_dir [6] = { DIR_SOUTH, DIR_WEST, DIR_NORTH, 
DIR_EAST, DIR_DOWN, DIR_UP };


/* get the 'short' name of an area (e.g. MIDGAARD, MIRROR etc. */
/* assumes that the file_name saved in the AREA_DATA struct is something like midgaard.are */
char * area_name (AREA_DATA *pArea)
{
	static char buffer[64]; /* short file_name */
	char  *period;

	assert (pArea != NULL);
	
	strncpy (buffer, pArea->file_name, 64); /* copy the file_name */	
	period = strchr (buffer, '.'); /* find the period (midgaard.are) */
	if (period) /* if there was one */
		*period = '\0'; /* terminate the string there (midgaard) */
		
	return buffer;	
}

typedef enum {exit_from, exit_to, exit_both} exit_status;

/* depending on status print > or < or <> between the 2 rooms */
void room_pair (ROOM_INDEX_DATA* left, ROOM_INDEX_DATA* right, exit_status ex, char *buffer)
{
	char *sExit;
	
	switch (ex)
	{
		default:
			sExit = "??"; break; /* invalid usage */
		case exit_from:
			sExit = "< "; break;
		case exit_to:
			sExit = " >"; break;
		case exit_both:
			sExit = "<>"; break;
	}
	
sprintf (buffer, "%5d %-26.26s %s%5d %-26.26s(%-8.8s)\n\r",
			  left->vnum, left->name, 
			  sExit,
			  right->vnum, right->name,
			  area_name(right->area)
	    );
}

/* for every exit in 'room' which leads to or from pArea but NOT both, print it */
void checkexits (ROOM_INDEX_DATA *room, AREA_DATA *pArea, char* buffer)
{
	char buf[MAX_STRING_LENGTH];
	int i;
	EXIT_DATA *exit;
	ROOM_INDEX_DATA *to_room;
	
	strcpy (buffer, "");
	for (i = 0; i < 6; i++)
	{
		exit = room->exit[i];
		if (!exit)
			continue;
		else
			to_room = exit->u1.to_room;
		
		if (to_room)  /* there is something on the other side */
                  {
			if ( (room->area == pArea) && (to_room->area !=pArea) )
			{ /* an exit from our area to another area */
			  /* check first if it is a two-way exit */
			
				if ( to_room->exit[opposite_dir[i]] &&
			to_room->exit[opposite_dir[i]]->u1.to_room ==room)
					room_pair (room,to_room,exit_both,buf); /* <> */
				else
					room_pair (room,to_room,exit_to,buf); /* > */
				
				strcat (buffer, buf);				
			}			
			else			
			if ( (room->area != pArea) && (exit->u1.to_room->area == pArea) )
			{ /* an exit from another area to our area */

				if  (!
			    	 (to_room->exit[opposite_dir[i]] &&
			 to_room->exit[opposite_dir[i]]->u1.to_room ==room)
					)
				/* two-way exits are handled in the other if */
				{						
					room_pair (to_room,room,exit_from,buf);
					strcat (buffer, buf);
				}
				
			} /* if room->area */
		}	
	} /* for */
	
}

/* for now, no arguments, just list the current area */
void do_exlist (CHAR_DATA *ch, char * argument)
{
	AREA_DATA* pArea;
	ROOM_INDEX_DATA* room;
	int i;
	char buffer[MAX_STRING_LENGTH];
	
	pArea = ch->in_room->area; /* this is the area we want info on */
	for (i = 0; i < MAX_KEY_HASH; i++) /* room index hash table */
	for (room = room_index_hash[i]; room != NULL; room = room->next)
	/* run through all the rooms on the MUD */
	
	{
		checkexits (room, pArea, buffer);
		send_to_char (buffer, ch);
	}
} 

/* show a list of all used VNUMS */

#define COLUMNS 		5   /* number of columns */
#define MAX_ROW 		((MAX_SHOW_VNUM / COLUMNS)+1) /* rows */

void do_vlist (CHAR_DATA *ch, char *argument)
{
	int i,j,vnum;
	ROOM_INDEX_DATA *room;
	char buffer[MAX_ROW*100]; /* should be plenty */
	char buf2 [100];
	
	for (i = 0; i < MAX_ROW; i++)
	{
		strcpy (buffer, ""); /* clear the buffer for this row */
		
		for (j = 0; j < COLUMNS; j++) /* for each column */
		{
			vnum = ((j*MAX_ROW) + i); /* find a vnum whih should be there */
			if (vnum < MAX_SHOW_VNUM)
			{
				room = get_room_index (vnum * 100 + 1); /* each zone has to have a XXX01 room */
				sprintf (buf2, "%3d %-8.8s  ", vnum, 
						 room ? area_name(room->area) : "-" ); 
						 /* something there or unused ? */
				strcat (buffer,buf2);				
			} 
		} /* for columns */
		
		send_to_char (buffer,ch);
		send_to_char ("\n\r",ch);
	} /* for rows */
}


/* Expand the name of a character into a string that identifies THAT
   character within a room. E.g. the second 'guard' -> 2. guard
*/   
const char * name_expand (CHAR_DATA *ch)
{
	int count = 1;
	CHAR_DATA *rch;
	char name[MAX_INPUT_LENGTH]; /*  HOPEFULLY no mob has a name longer than THAT */

	static char outbuf[MAX_INPUT_LENGTH];	
	
	if (!IS_NPC(ch))
		return ch->name;
		
	one_argument (ch->name, name); /* copy the first word into name */
	
	if (!name[0]) /* weird mob .. no keywords */
	{
		strcpy (outbuf, ""); /* Do not return NULL, just an empty buffer */
		return outbuf;
	}
		
	for (rch = ch->in_room->people; rch && (rch != ch);rch = rch->next_in_room)
		if (is_name (name, rch->name))
			count++;
			

	sprintf (outbuf, "%d.%s", count, name);
	return outbuf;
}


void do_for (CHAR_DATA *ch, char *argument)
{
	char range[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	bool fGods = FALSE, fMortals = FALSE, fMobs = FALSE, fEverywhere = FALSE, found;
	ROOM_INDEX_DATA *room, *old_room;
	CHAR_DATA *p, *p_next;
	int i;
	
	argument = one_argument (argument, range);
	
	if (!range[0] || !argument[0]) /* invalid usage? */
	{
		do_help (ch, "for");
		return;
	}
	
	if (!str_prefix("quit", argument))
	{
		send_to_char ("Are you trying to crash the MUD or something?\n\r",ch);
		return;
	}
	
	
	if (!str_cmp (range, "all"))
	{
		fMortals = TRUE;
		fGods = TRUE;
	}
	else if (!str_cmp (range, "gods"))
		fGods = TRUE;
	else if (!str_cmp (range, "mortals"))
		fMortals = TRUE;
	else if (!str_cmp (range, "mobs"))
		fMobs = TRUE;
	else if (!str_cmp (range, "everywhere"))
		fEverywhere = TRUE;
	else
		do_help (ch, "for"); /* show syntax */

	/* do not allow # to make it easier */		
	if (fEverywhere && strchr (argument, '#'))
	{
		send_to_char ("Cannot use FOR EVERYWHERE with the # thingie.\n\r",ch);
		return;
	}
		
	if (strchr (argument, '#')) /* replace # ? */
	{ 
		for (p = char_list; p ; p = p_next)
		{
			p_next = p->next; /* In case someone DOES try to AT MOBS SLAY # */
			found = FALSE;
			
			if (!(p->in_room) || room_is_private(p->in_room) || (p == ch))
				continue;
			
			if (IS_NPC(p) && fMobs)
				found = TRUE;
			else if (!IS_NPC(p) && p->level >= LEVEL_IMMORTAL && fGods)
				found = TRUE;
			else if (!IS_NPC(p) && p->level < LEVEL_IMMORTAL && fMortals)
				found = TRUE;

			/* It looks ugly to me.. but it works :) */				
			if (found) /* p is 'appropriate' */
			{
				char *pSource = argument; /* head of buffer to be parsed */
				char *pDest = buf; /* parse into this */
				
				while (*pSource)
				{
					if (*pSource == '#') /* Replace # with name of target */
					{
						const char *namebuf = name_expand (p);
						
						if (namebuf) /* in case there is no mob name ?? */
							while (*namebuf) /* copy name over */
								*(pDest++) = *(namebuf++);

						pSource++;
					}
					else
						*(pDest++) = *(pSource++);
				} /* while */
				*pDest = '\0'; /* Terminate */
				
				/* Execute */
				old_room = ch->in_room;
				char_from_room (ch);
				char_to_room (ch,p->in_room);
				interpret (ch, buf);
				char_from_room (ch);
				char_to_room (ch,old_room);
				
			} /* if found */
		} /* for every char */
	}
	else /* just for every room with the appropriate people in it */
	{
		for (i = 0; i < MAX_KEY_HASH; i++) /* run through all the buckets */
			for (room = room_index_hash[i] ; room ; room = room->next)
			{
				found = FALSE;
				
				/* Anyone in here at all? */
				if (fEverywhere) /* Everywhere executes always */
					found = TRUE;
				else if (!room->people) /* Skip it if room is empty */
					continue;
					
					
				/* Check if there is anyone here of the requried type */
				/* Stop as soon as a match is found or there are no more ppl in room */
				for (p = room->people; p && !found; p = p->next_in_room)
				{

					if (p == ch) /* do not execute on oneself */
						continue;
						
					if (IS_NPC(p) && fMobs)
						found = TRUE;
					else if (!IS_NPC(p) && (p->level >= LEVEL_IMMORTAL) && fGods)
						found = TRUE;
					else if (!IS_NPC(p) && (p->level <= LEVEL_IMMORTAL) && fMortals)
						found = TRUE;
				} /* for everyone inside the room */
						
				if (found && !room_is_private(room)) /* Any of the required type here AND room not private? */
				{
					/* This may be ineffective. Consider moving character out of old_room
					   once at beginning of command then moving back at the end.
					   This however, is more safe?
					*/
				
					old_room = ch->in_room;
					char_from_room (ch);
					char_to_room (ch, room);
					interpret (ch, argument);
					char_from_room (ch);
					char_to_room (ch, old_room);
				} /* if found */
			} /* for every room in a bucket */
	} /* if strchr */
} /* do_for */
