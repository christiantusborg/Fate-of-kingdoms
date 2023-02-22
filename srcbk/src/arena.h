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

void arena_credit	args ( (CHAR_DATA *ch, char *argument) );
void arena_begin	args ( (CHAR_DATA *ch, char *argument) );
void arena_update	args ( (void) );
bool arenacanrun	args ( (void) );
void arena_reset	args ( (void) );
void arena_win		args ( (void) );
void arena_start	args ( (void) );
bool arena_checkwin	args ( (void) );
void do_arena_out	args ( (CHAR_DATA *ch, char *argument) );
void arena_out		args ( (CHAR_DATA *ch, char *argument) );
void arena_init		args ( (CHAR_DATA *ch, char *argument) );
void arena_join		args ( (CHAR_DATA *ch, char *argument) );
void arena_who		args ( (CHAR_DATA *ch, char *argument) );
void do_arena_clean	args ( (void) );

int min_level;
int max_level;
int reallywar;
int inwar;
int wartype;
int wartimeleft;
int wartimer;
bool inmiddle;
bool iswar;

#define ARENA_OUTERRORROOM 2904
#define ARENA_FULL 	1
#define ARENA_KINGDOM	2
#define ARENA_RACE	3
#define ARENA_CLASS	4
#define ARENA_ALIGNMENT	5
#define ARENA_TEAM	6
#define ARENA_DUEL	7
      
bool	ArenaStarted;
bool	ArenaRunning;
int	ArenaNextStart;
int	ArenaLowLevel;
int	ArenaHighLevel;
int	ArenaType;
int	ArenaTimeToRun;
int	ArenaHowManyIn;
int	ArenaZone;
int	ArenaPool;
int	ArenaJoinCost;
int	ArenaLowVnum;
int	ArenaMaxVnum;

struct	arena_kind_type
{
	char * const	shortname;
	char * const	longname;    
	sh_int		bit;
	sh_int		coststart;
	sh_int		costjoin;	
	sh_int		clean;
};

struct	arena_zone_type
{
	char * const	shortname;
	char * const	longname;    
//	sh_int		number;
	sh_int		low_vnum;
	sh_int		max_vnum;

};
