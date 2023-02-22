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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* unlink() */ 
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "fok_struct.h"

extern const   struct auth_type auth_table [];
bool	check_social	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );
//bool  check_disabled (const struct cmd_type *command);
bool  check_disabled (int cmd);
bool can_be_ordered     args( (CHAR_DATA *ch, char *argument ) );


DISABLED_DATA *disabled_first;  
#define END_MARKER    "END" /* for load_disabled() and save_disabled() */

/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2



/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;

/*
 * immortals log file
 */
FILE                            *imm_log;

/*NEW NEW NEW NEW NEW NEW*/

const	struct	new_cmd_type	new_cmd_table	[] =
{

    /*
     * Common movement commands.
     */
{ "north",	do_north,	POS_STANDING,GROUP_MORTAL,0,auth_move,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "east",	do_east,	POS_STANDING,GROUP_MORTAL,0,auth_move,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION  },
{ "south",	do_south,	POS_STANDING,GROUP_MORTAL,0,auth_move,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "west",	do_west,	POS_STANDING,GROUP_MORTAL,0,auth_move,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "up",		do_up,		POS_STANDING,GROUP_MORTAL,0,auth_move,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "down",	do_down,	POS_STANDING,GROUP_MORTAL,0,auth_move,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
    /* 
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
{ "job",	do_job,      	POS_RESTING,	GROUP_MORTAL,0, auth_job,LOG_NORMAL,1,CMD_STONEFORM|CMD_AFTERLIFE },
//{ "arena",	do_war,		POS_STANDING,	GROUP_MORTAL,0, auth_arena,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE  },
{ "at",		do_at,          POS_DEAD,	GROUP_IMMORT,L5,auth_at,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_DIMENSION|CMD_STONEFORM  },
{ "cast",	do_cast,	POS_FIGHTING,	GROUP_MORTAL,0,	auth_cast,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "commune",	do_commune,	POS_FIGHTING,	GROUP_MORTAL,0,	auth_commune,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "sing",	do_sing,	POS_FIGHTING,	GROUP_MORTAL,0,	auth_sing,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "auction",    do_auction,     POS_SLEEPING,	GROUP_MORTAL,0, auth_auction,LOG_NORMAL, 1, CMD_GHOST|CMD_STONEFORM|CMD_AFTERLIFE },
{ "buy",	do_buy,		POS_RESTING,	GROUP_MORTAL,0,	auth_buy,LOG_NORMAL, 1, CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "hometown",	do_hometown,	POS_RESTING,	GROUP_MORTAL,0,	auth_hometown,LOG_NORMAL,1, CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "bounty",	do_bounty,	POS_RESTING,	GROUP_MORTAL,0,	auth_bounty,LOG_ALWAYS,1, CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "channels",   do_channels,    POS_DEAD,	GROUP_MORTAL,0, auth_channels,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE},
{ "colour",	do_colour,	POS_DEAD,	GROUP_MORTAL,0, auth_color,LOG_NORMAL, 1},
{ "color",	do_colour,	POS_DEAD,	GROUP_MORTAL,0, auth_color,LOG_NORMAL, 1},
{ "exits",	do_exits,	POS_RESTING,	GROUP_MORTAL,0,	auth_exits,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "get",	do_get,		POS_RESTING,	GROUP_MORTAL,0,	auth_get,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },
{ "goto",       do_goto,        POS_DEAD,	GROUP_IMMORT,L8,auth_goto,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_DIMENSION },
{ "group",      do_group,       POS_SLEEPING,	GROUP_MORTAL,0, auth_group,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "hit",	do_kill,	POS_FIGHTING,	GROUP_MORTAL,0,	auth_hit,LOG_NORMAL, 1,0},
{ "inventory",	do_inventory,	POS_DEAD,	GROUP_MORTAL,0,	auth_inventory,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "kill",	do_kill,	POS_FIGHTING,	GROUP_MORTAL,0,	auth_kill,LOG_NORMAL, 1,CMD_STONEFORM|CMD_AFTERLIFE},
{ "look",	do_look,	POS_RESTING,	GROUP_MORTAL,0,	auth_look,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "msp",	do_msp,		POS_DEAD,	GROUP_MORTAL,0,	auth_msp,LOG_NORMAL, 1 },
{ "order",	do_order,	POS_RESTING,	GROUP_MORTAL,0,	auth_order,LOG_ALWAYS,1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "practice",   do_practice,	POS_SLEEPING,	GROUP_MORTAL,0, auth_practice,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "rest",	do_rest,	POS_SLEEPING,	GROUP_MORTAL,0,	auth_rest,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "sit",	do_sit,		POS_SLEEPING,	GROUP_MORTAL,0, auth_sit,LOG_NORMAL, 1, CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "sockets",    do_sockets,	POS_DEAD,	GROUP_SPYING,L5,auth_sockets,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_DIMENSION},
{ "stand",	do_stand,	POS_SLEEPING,	GROUP_MORTAL,0,	auth_stand,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
//{ "startarena",	do_startwar,	POS_STANDING,	GROUP_MORTAL,0, auth_startarena,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },  
{ "tell",	do_tell,	POS_RESTING,	GROUP_MORTAL,0,	auth_tell,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "unlock",     do_unlock,      POS_RESTING,	GROUP_MORTAL,0, auth_unlock,LOG_NORMAL, 1,CMD_STONEFORM},
{ "vt",		do_vt,		POS_DEAD,	GROUP_MORTAL,0,	auth_vt,LOG_NORMAL, 1 }, 
{ "wield",	do_wear,	POS_RESTING,	GROUP_MORTAL,0,	auth_wield,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM},
{ "dual",	do_second,	POS_RESTING,	GROUP_MORTAL,0,	auth_second,LOG_NORMAL,1,CMD_KEEP_DIMENSION|CMD_STONEFORM},
{ "second",	do_second,	POS_RESTING,	GROUP_MORTAL,0,	auth_second,LOG_NORMAL,1,CMD_KEEP_DIMENSION},
{ "wizhelp",	do_wizhelp,	POS_DEAD,	GROUP_IMMORT,IM,auth_wizhelp,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
//{ "arenaout",	do_arenaout,	POS_DEAD,	GROUP_MORTAL,0,	auth_arenaout,LOG_NORMAL,1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },
{ "arise",	do_arise,	POS_DEAD,	GROUP_MORTAL,0,	auth_arise,LOG_NORMAL,1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "dive",	do_dive,	POS_RESTING, 	GROUP_MORTAL,0, auth_dive,LOG_NORMAL, 1},
{ "emerge",	do_emerge,	POS_RESTING, 	GROUP_MORTAL,0, auth_emerge,LOG_NORMAL, 1},

    /*
     * Informational commands.
     */
{ "affects",	do_affects,	POS_DEAD,	GROUP_MORTAL,0,	 auth_affects,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "affections", do_affections,  POS_DEAD,    	GROUP_MORTAL,0,auth_affections,LOG_ALWAYS, 1},
{ "areas",	do_areas,	POS_DEAD,	GROUP_MORTAL,0, auth_areas,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "balance",	do_balance,	POS_STANDING,	GROUP_MORTAL,0, auth_balance,LOG_NORMAL, 1,CMD_KEEP_DIMENSION },
{ "bug",	do_bug,		POS_DEAD,	GROUP_MORTAL,0, auth_bug,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "commands",	do_commands,	POS_DEAD,	GROUP_MORTAL,0, auth_commands,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "compare",	do_compare,	POS_RESTING,	GROUP_MORTAL,0, auth_compare,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "consider",	do_consider,	POS_RESTING,	GROUP_MORTAL,0, auth_consider,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "count",	do_count,	POS_SLEEPING,	GROUP_MORTAL,0, auth_count,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION }, 
{ "credits",	do_credits,	POS_DEAD,	GROUP_MORTAL,0, auth_credits,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "deposit",	do_deposit,	POS_STANDING,	GROUP_MORTAL,0, auth_deposit,LOG_NORMAL, 1,CMD_KEEP_DIMENSION},
{ "equipment",	do_equipment,	POS_DEAD,	GROUP_MORTAL,0, auth_equipment,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "examine",	do_examine,	POS_RESTING,	GROUP_MORTAL,0, auth_examine,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "help",	do_help,	POS_DEAD,	GROUP_MORTAL,0, auth_help,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "background",	do_background,	POS_DEAD,	GROUP_MORTAL,0, auth_background,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "motd",	do_motd,	POS_DEAD,       GROUP_MORTAL,0, auth_motd,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "update",	do_update,	POS_DEAD,       GROUP_MORTAL,0, auth_update,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "read",	do_read,	POS_RESTING,	GROUP_MORTAL,0, auth_read,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "report",	do_report,	POS_RESTING,	GROUP_MORTAL,0, auth_report,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "rules",	do_rules,	POS_DEAD,	GROUP_MORTAL,0, auth_rules,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "score",	do_score,	POS_DEAD,	GROUP_MORTAL,0, auth_score,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "scan",       do_scan,        POS_RESTING,    GROUP_MORTAL,0, auth_scan,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "skills",	do_skills,	POS_DEAD,	GROUP_MORTAL,0, auth_skills,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "socials",	do_socials,	POS_DEAD,	GROUP_MORTAL,0,auth_socials,LOG_NORMAL,1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "show",	do_show,	POS_DEAD,	GROUP_MORTAL,0, auth_show,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "spells",	do_spells,	POS_DEAD,	GROUP_MORTAL,0, auth_spells,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "story",	do_story,	POS_DEAD,	GROUP_MORTAL,0, auth_story,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "time",	do_time,	POS_DEAD,	GROUP_MORTAL,0, auth_time,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "typo",	do_typo,	POS_DEAD,	GROUP_MORTAL,0, auth_typo,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "weather",	do_weather,	POS_RESTING,	GROUP_MORTAL,0, auth_weather,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "who",	do_who,		POS_DEAD,	GROUP_MORTAL,0, auth_who,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "whoami",	do_whoami,	POS_DEAD,	GROUP_MORTAL,0, auth_whoami,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "whois",	do_whois,	POS_DEAD,	GROUP_MORTAL,0, auth_whois,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "withdraw",	do_withdraw,	POS_STANDING, 	GROUP_MORTAL,0, auth_withdraw,LOG_NORMAL, 1,0},
{ "wizlist",	do_wizlist,	POS_DEAD,       GROUP_MORTAL,0, auth_wizlist,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "worth",	do_worth,	POS_SLEEPING,	GROUP_MORTAL,0, auth_worth,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION }, 
{ "slist",	do_slist,	POS_DEAD,	GROUP_ADM,L3, auth_slist, LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
//{ "glist",	do_glist,	POS_DEAD,	GROUP_WTF,L0, auth_glist,LOG_NORMAL,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "repair",	do_repair,	POS_RESTING,	GROUP_MORTAL,0, auth_repair,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "estimate",	do_estimate,	POS_RESTING,	GROUP_MORTAL,0, auth_estimate,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "finger",	do_finger,	POS_DEAD,	GROUP_MORTAL,0, auth_finger,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "tbh",	do_tbh,		POS_DEAD,	GROUP_TBH,0, auth_tbh,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },     
    /*
     * Configuration commands.
     */
{ "alia",	do_alia,	POS_DEAD,	 GROUP_MORTAL,0,auth_alias,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "alias",	do_alias,	POS_DEAD,	 GROUP_MORTAL,0,auth_alias,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "clear",	do_clear,	POS_DEAD,	 GROUP_MORTAL,0,auth_clear,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "cls",	do_clear,	POS_DEAD,	 GROUP_MORTAL,0,auth_clear,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autolist",	do_autolist,	POS_DEAD,	 GROUP_MORTAL,0,auth_autolist,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autoassist",	do_autoassist,	POS_DEAD,        GROUP_MORTAL,0,auth_autoassist,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autoexit",	do_autoexit,	POS_DEAD,        GROUP_MORTAL,0,auth_autoexit,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autogold",	do_autogold,	POS_DEAD,        GROUP_MORTAL,0,auth_autogold,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autoloot",	do_autoloot,	POS_DEAD,        GROUP_MORTAL,0,auth_autoloot,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autosac",	do_autosac,	POS_DEAD,        GROUP_MORTAL,0,auth_autosac,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autosplit",	do_autosplit,	POS_DEAD,        GROUP_MORTAL,0,auth_autosplit,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "autodamage",	do_autodamage,	POS_DEAD,	 GROUP_MORTAL,0,auth_autodamage,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },    
{ "brief",	do_brief,	POS_DEAD,        GROUP_MORTAL,0,auth_brief,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "combine",	do_combine,	POS_DEAD,        GROUP_MORTAL,0,auth_combine,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "compact",	do_compact,	POS_DEAD,        GROUP_MORTAL,0,auth_compact,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "description",do_description,	POS_DEAD,	 GROUP_MORTAL,0,auth_description,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "delet",	do_delet,	POS_DEAD,	 GROUP_MORTAL,0,auth_delete,LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "delete",	do_delete,	POS_STANDING,	 GROUP_MORTAL,0,auth_delete,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "nofollow",	do_nofollow,	POS_DEAD,        GROUP_MORTAL,0,auth_nofollow,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "noloot",	do_noloot,	POS_DEAD,        GROUP_MORTAL,0,auth_noloot,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "nosummon",	do_nosummon,	POS_DEAD,        GROUP_MORTAL,0,auth_nosummon,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "outfit",	do_outfit,	POS_RESTING,	 GROUP_MORTAL,0,auth_outfit,LOG_NORMAL, 1,0},
{ "tick",	do_tick,	POS_DEAD, 	 GROUP_ADMIN,L0,auth_tick,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "password",	do_password,	POS_DEAD,	 GROUP_MORTAL,0,auth_password,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "prompt",	do_prompt,	POS_DEAD,        GROUP_MORTAL,0,auth_prompt,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "scroll",	do_scroll,	POS_DEAD,	 GROUP_MORTAL,0,auth_scroll,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "title",	do_title,	POS_DEAD,	 GROUP_ASS,L4,auth_title,LOG_ALWAYS,1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
//{ "pretitle",	do_pretitle,	POS_DEAD,	 GROUP_ASS,L4,auth_pretitle,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "unalias",	do_unalias,	POS_DEAD,	 GROUP_MORTAL,0,auth_unalias,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "wimpy",	do_wimpy,	POS_DEAD,	 GROUP_MORTAL,0,auth_wimpy,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "sitelock",	do_sitelock,	POS_DEAD,	 GROUP_MORTAL,0,auth_sitelock,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
//{ "mkpretitle",	do_mk,		POS_DEAD,	 GROUP_ASS,L4,auth_mk,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },    
    /*
     * Communication commands.
     buraya kadar ok...|CMD_KEEP_FADE|CMD_KEEP_DIMENSION olayi
     */
{ "afk",	do_afk,		POS_SLEEPING,	 GROUP_MORTAL,0,auth_afk,LOG_NORMAL, 1,CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "deaf",	do_deaf,	POS_DEAD,	 GROUP_MORTAL,0,auth_deaf,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "emote",	do_emote,	POS_RESTING,	 GROUP_MORTAL,0,auth_emote,LOG_ALWAYS, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "pmote",	do_pmote,	POS_RESTING,	 GROUP_MORTAL,0,auth_pmote,LOG_ALWAYS, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ ",",		do_emote,	POS_RESTING,	 GROUP_MORTAL,0,auth_emote,LOG_ALWAYS, 0, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "gtell",	do_gtell,	POS_DEAD,	 GROUP_MORTAL,0,auth_gtell,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ ";",		do_gtell,	POS_DEAD,	 GROUP_MORTAL,0,auth_gtell,LOG_NORMAL, 0, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "board",	do_board,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_board,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "music",	do_music,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_music,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "info",	do_info,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_info,LOG_NORMAL,1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "newbie",	do_newbie,	POS_DEAD,	 GROUP_MORTAL,0,auth_newbie,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "note",	do_note,	POS_DEAD,	 GROUP_MORTAL,0,auth_note,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "pose",	do_pose,	POS_RESTING,	 GROUP_MORTAL,0,auth_pose,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "ooc",	do_ooc,		POS_DEAD,	 GROUP_MORTAL,0,auth_ooc,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "gossip",	do_gossip,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_gossip,LOG_ALWAYS, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },
{ "quest",	do_quest,	POS_DEAD,   	 GROUP_MORTAL,0,auth_quest,LOG_NORMAL,1,CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "questch",	do_questch,	POS_SLEEPING,    GROUP_ACTION,0,auth_questtalk,LOG_NORMAL,1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "questinfo",	do_questinfo,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_questinfo,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "quiet",	do_quiet,	POS_SLEEPING, 	 GROUP_MORTAL,0,auth_quiet,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "reply",	do_reply,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_reply,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_DIMENSION },
{ "replay",	do_replay,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_replay,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "say",	do_new_say,	POS_RESTING,	 GROUP_MORTAL,0,auth_say,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_DIMENSION|CMD_KEEP_FADE},
{ "'",		do_new_say,	POS_RESTING, 	 GROUP_MORTAL,0,auth_say,LOG_NORMAL, 0, CMD_GHOST|CMD_KEEP_DIMENSION|CMD_KEEP_FADE },
{ "isay",	do_new_say_ooc,	POS_RESTING, 	 GROUP_MORTAL,0,auth_say,LOG_NORMAL, 0, CMD_GHOST|CMD_KEEP_DIMENSION|CMD_KEEP_FADE },
{ "talk",	do_say,		POS_RESTING,	 GROUP_MORTAL,0,auth_say,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_DIMENSION|CMD_KEEP_FADE },
{ "shout",	do_shout,	POS_RESTING,	 GROUP_MORTAL,0,auth_shout,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },
{ "message",	do_shout,	POS_RESTING,	 GROUP_MORTAL,0,auth_shout,LOG_NORMAL, 1, CMD_GHOST|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },
{ "warcry",     do_warcry,      POS_FIGHTING,    GROUP_MORTAL,0,auth_warcry,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM},
{ "dark",	do_dark_vision, POS_STANDING,	 GROUP_MORTAL,0,auth_dark_vision,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "detect",     do_detect_hidden,POS_RESTING,    GROUP_MORTAL,0,auth_detect_hidden,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "bearcall",   do_bear_call,   POS_FIGHTING,    GROUP_MORTAL,0,auth_bear_call,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE},
{ "yell",	do_yell,	POS_RESTING,	 GROUP_MORTAL,0,auth_yell,LOG_ALWAYS, 1, CMD_GHOST|CMD_KEEP_DIMENSION },
{ "narrate",	do_narrate,	POS_RESTING, 	 GROUP_MORTAL,0,auth_narrate,LOG_NORMAL, 1,CMD_GHOST },
//{ "gt",         do_gd,          POS_SLEEPING,	 GROUP_MORTAL,0,auth_gd,LOG_ALWAYS, 1,CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
//{ "gd",         do_gd,          POS_SLEEPING,	 GROUP_MORTAL,0,auth_gd,LOG_ALWAYS, 1,CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "gd",         do_kingdomchannel,          POS_SLEEPING,	 GROUP_MORTAL,0,auth_gd,LOG_ALWAYS, 1,CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "pray",       do_pray,        POS_DEAD,        GROUP_MORTAL,0,auth_pray,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },

    /*
     * Object manipulation commands.
     */
{ "brandish",	do_brandish,	POS_RESTING,	 GROUP_MORTAL,0,auth_brandish,LOG_NORMAL, 1,CMD_KEEP_DIMENSION|CMD_STONEFORM },
{ "butcher",    do_butcher,     POS_STANDING,    GROUP_MORTAL,0,auth_butcher,LOG_NORMAL, 1,CMD_STONEFORM },
{ "close",	do_close,	POS_RESTING,	 GROUP_MORTAL,0,auth_close,LOG_NORMAL, 1,CMD_KEEP_DIMENSION },
{ "drink",	do_drink,	POS_RESTING,	 GROUP_MORTAL,0,auth_drink,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "drop",	do_drop,	POS_RESTING,	 GROUP_MORTAL,0,auth_drop,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "eat",	do_eat,		POS_RESTING,	 GROUP_MORTAL,0,auth_eat,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "envenom",	do_envenom,	POS_RESTING,	 GROUP_MORTAL,0,auth_envenom,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "fill",	do_fill,	POS_RESTING,	 GROUP_MORTAL,0,auth_fill,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "give",	do_give,	POS_RESTING,	 GROUP_MORTAL,0,auth_give,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "heal",	do_heal,	POS_RESTING,	 GROUP_MORTAL,0,auth_heal,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "hold",	do_wear,	POS_RESTING,	 GROUP_MORTAL,0,auth_hold,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "list",	do_list,	POS_RESTING,	 GROUP_MORTAL,0,auth_list,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_KEEP_FADE },
{ "lock",	do_lock,	POS_RESTING,	 GROUP_MORTAL,0,auth_lock,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "lockon",	do_lockon,	POS_DEAD,	 GROUP_MORTAL,0,auth_lockon,LOG_NORMAL,1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "locktitle",  do_locktitle,   POS_DEAD,	 GROUP_MORTAL,0,auth_locktitle,LOG_NORMAL, 1,CMD_KEEP_DIMENSION},
{ "lore",       do_lore,        POS_RESTING,     GROUP_MORTAL,0,auth_lore,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },
{ "open",	do_open,	POS_RESTING,	 GROUP_MORTAL,0,auth_open,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "pick",	do_pick,	POS_RESTING,	 GROUP_MORTAL,0,auth_pick,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM },
{ "relock",	do_relock,	POS_RESTING,	 GROUP_MORTAL,0,auth_relock,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM },
{ "pour",	do_pour,	POS_RESTING,	 GROUP_MORTAL,0,auth_pour,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_KEEP_DIMENSION },
{ "put",	do_put,		POS_RESTING,	 GROUP_MORTAL,0,auth_put,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "quaff",	do_quaff,	POS_RESTING,	 GROUP_MORTAL,0,auth_quaff,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE },
{ "recite",	do_recite,	POS_RESTING,	 GROUP_MORTAL,0,auth_recite,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE },
{ "remove",	do_remove,	POS_RESTING,	 GROUP_MORTAL,0,auth_remove,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM },
{ "request",    do_request,     POS_STANDING,    GROUP_MORTAL,0,auth_request,LOG_NORMAL, 1,0 },
{ "sell",	do_sell,	POS_RESTING,	 GROUP_MORTAL,0,auth_sell,LOG_NORMAL, 1,0 },
{ "take",	do_get,		POS_RESTING,	 GROUP_MORTAL,0,auth_take,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "sacrifice",	do_sacrifice,	POS_RESTING,	 GROUP_MORTAL,0,auth_sacrifice,LOG_NORMAL, 1,0 },
{ "junk",       do_sacrifice,   POS_RESTING,     GROUP_MORTAL,0,auth_junk,LOG_NORMAL, 1,0 },
{ "value",	do_value,	POS_RESTING,	 GROUP_MORTAL,0,auth_value,LOG_NORMAL, 1,0 },
{ "wear",	do_wear,	POS_RESTING,	 GROUP_MORTAL,0,auth_wear,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM },
{ "zap",	do_zap,		POS_RESTING,	 GROUP_MORTAL,0,auth_zap,LOG_NORMAL, 1,CMD_STONEFORM },

    /*
      Old spells which are now skills.
     */
{ "acute",	do_acute_vision,POS_RESTING,     GROUP_MORTAL,0,auth_acute,LOG_NORMAL, 1,CMD_STONEFORM|CMD_AFTERLIFE },
{ "barkskin",	do_barkskin,	POS_RESTING,     GROUP_MORTAL,0,auth_barkskin,LOG_NORMAL, 1,CMD_STONEFORM|CMD_AFTERLIFE },
{ "sharpen",	do_sharpen,	POS_RESTING,	 GROUP_MORTAL,0,auth_sharpen,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_STONEFORM },
{ "vanish",	do_vanish,	POS_RESTING,     GROUP_MORTAL,0,auth_vanish,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_STONEFORM|CMD_AFTERLIFE },
{ "blindness",	do_blindness,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_blindness,LOG_NORMAL, 1,CMD_STONEFORM|CMD_AFTERLIFE },
{ "poison",	do_poison,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_poison,LOG_NORMAL, 1,CMD_STONEFORM },
    /*
     * Combat commands.
     */
{ "ambush",     do_ambush,      POS_STANDING,    GROUP_MORTAL,0,auth_ambush,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_STONEFORM },
{ "assassinate",do_assassinate, POS_STANDING,    GROUP_MORTAL,0,auth_assassinate,LOG_NORMAL, 1,CMD_STONEFORM },
{ "backstab",	do_backstab,	POS_STANDING,	 GROUP_MORTAL,0,auth_backstab,LOG_NORMAL, 1,CMD_STONEFORM },
{ "bash",	do_bash,	POS_FIGHTING,    GROUP_MORTAL,0,auth_bash,LOG_NORMAL, 1,CMD_STONEFORM },
{ "shieldbash",	do_shieldbash,	POS_FIGHTING,    GROUP_MORTAL,0,auth_shieldbash,LOG_NORMAL, 1,CMD_STONEFORM },
{ "bs",		do_backstab,	POS_STANDING,	 GROUP_MORTAL,0,auth_backstab,LOG_NORMAL, 0,CMD_STONEFORM },
{ "berserk",	do_berserk,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_berserk,LOG_NORMAL, 1,CMD_STONEFORM|CMD_AFTERLIFE },
{ "bloodthirst",do_bloodthirst,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_bloodthirst,LOG_NORMAL, 1,CMD_STONEFORM },
{ "blackjack",	do_blackjack,	POS_STANDING,	 GROUP_MORTAL,0,auth_blackjack,LOG_NORMAL, 1,CMD_STONEFORM },
{ "gouge",	do_gouge,	POS_STANDING,	 GROUP_MORTAL,0,auth_gouge,LOG_NORMAL, 1,CMD_STONEFORM },
{ "caltraps",   do_caltraps,    POS_FIGHTING,    GROUP_MORTAL,0,auth_caltraps,LOG_NORMAL, 1,CMD_STONEFORM },
{ "camouflage", do_camouflage,  POS_STANDING,    GROUP_MORTAL,0,auth_camouflage,LOG_NORMAL, 1,CMD_STONEFORM|CMD_AFTERLIFE },
{ "distract",   do_distract,    POS_FIGHTING,    GROUP_MORTAL,0,auth_distract,LOG_NORMAL, 1,0|CMD_AFTERLIFE },
{ "charge",	do_charge,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_charge,LOG_NORMAL, 1,CMD_STONEFORM },
{ "circle",     do_circle,      POS_FIGHTING,    GROUP_MORTAL,0,auth_circle,LOG_NORMAL, 1,CMD_STONEFORM },
{ "cleave",     do_cleave,      POS_STANDING,    GROUP_MORTAL,0,auth_cleave,LOG_NORMAL, 1,CMD_STONEFORM },
{ "dirt",	do_dirt,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_dirt,LOG_NORMAL, 1,CMD_STONEFORM },
{ "disarm",	do_disarm,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_disarm,LOG_NORMAL, 1,CMD_STONEFORM },
{ "flee",	do_flee,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_flee,LOG_NORMAL, 1,CMD_STONEFORM },
{ "guard",      do_guard,       POS_STANDING,    GROUP_MORTAL,0,auth_guard,LOG_NORMAL, 1,CMD_STONEFORM },
{ "kick",	do_kick,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_kick,LOG_NORMAL, 1,CMD_STONEFORM },
{ "multiburst",	do_multiburst,	POS_STANDING,	 GROUP_MORTAL,0,auth_multiburst,LOG_NORMAL, 0,CMD_STONEFORM },
{ "murde",	do_murde,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_murder,LOG_NORMAL, 0,CMD_STONEFORM },
{ "murder",	do_murder,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_murder,LOG_ALWAYS, 1,CMD_STONEFORM },
{ "nerve",      do_nerve,       POS_FIGHTING,    GROUP_MORTAL,0,auth_nerve,LOG_NORMAL, 1,CMD_STONEFORM },
{ "rescue",	do_rescue,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_rescue,LOG_NORMAL, 0,CMD_STONEFORM },
{ "resistance", do_resistance,  POS_FIGHTING,    GROUP_MORTAL,0,auth_knightsvirtue,LOG_NORMAL, 0,CMD_STONEFORM },
{ "truesight",  do_truesight,   POS_FIGHTING,    GROUP_MORTAL,0,auth_truesight,LOG_NORMAL, 0,0 },
{ "spellbane",  do_spellbane,   POS_FIGHTING,    GROUP_MORTAL,0,auth_spellbane,LOG_NORMAL, 0,CMD_STONEFORM },
{ "strangle",   do_strangle,    POS_STANDING,    GROUP_MORTAL,0,auth_strangle,LOG_NORMAL, 1,CMD_KEEP_FADE|CMD_STONEFORM },
//{ "surrender",	do_surrender,	POS_FIGHTING,    GROUP_MORTAL,0,auth_surrender,LOG_NORMAL, 0,0 },
{ "tame",       do_tame,        POS_FIGHTING,    GROUP_MORTAL,0,auth_tame,LOG_NORMAL, 1,CMD_STONEFORM },
{ "throw",      do_throw,       POS_FIGHTING,    GROUP_MORTAL,0,auth_throw,LOG_NORMAL, 1,CMD_STONEFORM },
{ "trip",	do_trip,	POS_FIGHTING,    GROUP_MORTAL,0,auth_trip,LOG_NORMAL, 1,CMD_STONEFORM },
{ "submission",	do_submission,	POS_FIGHTING,    GROUP_MORTAL,0,auth_submission,LOG_NORMAL, 1,CMD_STONEFORM },
{ "mount",	do_mount,	POS_STANDING,    GROUP_MORTAL,0,auth_mount,LOG_NORMAL, 1,CMD_STONEFORM},
{ "dismount",	do_dismount,	POS_STANDING,    GROUP_MORTAL,0,auth_dismount,LOG_NORMAL, 1,CMD_STONEFORM},
{ "disengage",	do_disengage,	POS_FIGHTING,    GROUP_MORTAL,0,auth_disengage,LOG_NORMAL, 1,CMD_STONEFORM },
{ "whirlwind",	do_whirlwind,   POS_FIGHTING,    GROUP_MORTAL,0,auth_whirlwind,LOG_NORMAL, 1,CMD_STONEFORM },
{ "crush",	do_crush,       POS_STANDING,    GROUP_MORTAL,0,auth_crush,LOG_NORMAL, 1,CMD_STONEFORM },
{ "stoneform",	do_stoneform,   POS_FIGHTING,    GROUP_MORTAL,0,auth_stoneform,LOG_NORMAL, 1,0 },
{ "swoop",	do_swoop,       POS_STANDING,    GROUP_MORTAL,0,auth_swoop,LOG_NORMAL, 1,0 },
{ "breath",	do_breath,      POS_FIGHTING,    GROUP_MORTAL,0,auth_breath,LOG_NORMAL, 1,0 },
{ "phase",	do_phase,       POS_FIGHTING,    GROUP_MORTAL,0,auth_phase,LOG_NORMAL, 1,0 },
{ "totem",	do_totem,       POS_STANDING,    GROUP_MORTAL,0,auth_totem,LOG_NORMAL, 1,0 },
{ "paralysis",	do_paralysis,   POS_FIGHTING,    GROUP_MORTAL,0,auth_paralysis,LOG_NORMAL, 1,0 },
{ "feign death",do_feign_death, POS_FIGHTING,    GROUP_MORTAL,0,auth_feign_death,LOG_NORMAL, 1,CMD_STONEFORM },

     /*
     * Mob command interpreter (placed here for faster scan...)
     */
{ "mob",           do_mob,         POS_DEAD,     GROUP_MORTAL,0,auth_mob,LOG_NEVER, 0,0 },

    /*
     * Miscellaneous commands.
     */
{ "endure",	do_endure,      POS_STANDING,    GROUP_MORTAL,0,auth_endure,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_STONEFORM|CMD_AFTERLIFE },
{ "enter", 	do_enter, 	POS_STANDING,	 GROUP_MORTAL,0,auth_enter,LOG_NORMAL, 1, CMD_GHOST },
{ "follow",	do_follow,	POS_RESTING,	 GROUP_MORTAL,0,auth_follow,LOG_NORMAL, 1, CMD_KEEP_HIDE },
{ "gain",	do_gain,	POS_STANDING,	 GROUP_MORTAL,0,auth_gain,LOG_NORMAL, 1,0 },
{ "go",		do_enter,	POS_STANDING,	 GROUP_MORTAL,0,auth_go,LOG_NORMAL, 0, CMD_GHOST },
{ "herbs",      do_herbs,       POS_STANDING,    GROUP_MORTAL,0,auth_herbs,LOG_NORMAL, 1,CMD_STONEFORM|CMD_AFTERLIFE },
{ "hunt",	do_hunt,	POS_STANDING,	 GROUP_MORTAL,0,auth_hunt,LOG_NORMAL, 1,CMD_STONEFORM },
{ "hide",	do_hide,	POS_RESTING,	 GROUP_MORTAL,0,auth_hide,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE },
{ "qui",	do_qui,		POS_DEAD,	 GROUP_MORTAL,0,auth_quit,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "quit",	do_quit,	POS_DEAD,	 GROUP_MORTAL,0,auth_quit,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE },
{ "recall",	do_recall,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_recall,LOG_NORMAL, 1,0|CMD_AFTERLIFE },
{ "/",		do_recall,	POS_FIGHTING,	 GROUP_MORTAL,0,auth_recall,LOG_NORMAL, 0,0|CMD_AFTERLIFE },
{ "rent",	do_rent,	POS_DEAD,	 GROUP_MORTAL,0,auth_rent,LOG_NORMAL, 0,0 },
{ "save",	do_save,	POS_DEAD,	 GROUP_MORTAL,0,auth_save,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE },
{ "sleep",	do_sleep,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_sleep,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
//{ "slook",	do_slook,	POS_DEAD,	 GROUP_ADM,L0,auth_slook,LOG_NORMAL,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
//{ "lookup",	do_slookup,	POS_DEAD,	 GROUP_MORTAL,L0,auth_lookup,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "sneak",	do_sneak,	POS_STANDING,	 GROUP_MORTAL,0,auth_sneak,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM|CMD_AFTERLIFE },
{ "split",	do_split,	POS_RESTING,	 GROUP_MORTAL,0,auth_split,LOG_NORMAL, 1,0 },
{ "steal",	do_steal,	POS_STANDING,	 GROUP_MORTAL,0,auth_steal,LOG_NORMAL, 1,CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_STONEFORM },
{ "train",	do_train,	POS_RESTING,	 GROUP_MORTAL,0,auth_train,LOG_NORMAL, 1,0 },
{ "visible",	do_visible,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_visible,LOG_NORMAL, 1,0 },
{ "wake",	do_wake,	POS_SLEEPING,	 GROUP_MORTAL,0,auth_wake,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "wanted",     do_wanted,      POS_STANDING,    GROUP_SADM,L3,auth_wanted,LOG_ALWAYS, 1,0 },
{ "where",	do_where,	POS_RESTING,	 GROUP_MORTAL,0,auth_where,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE },
{ "donate",	do_donate,	POS_RESTING,	 GROUP_MORTAL,0,auth_donate,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION|CMD_AFTERLIFE },
{ "push",	do_push,	POS_STANDING,	 GROUP_MORTAL,0,auth_push,LOG_NORMAL, 1, 0},
{ "shoot",	do_shoot,	POS_STANDING,	 GROUP_MORTAL,0,auth_shoot,LOG_NORMAL,1,CMD_STONEFORM},
{ "smithing",	do_smithing,	POS_STANDING,	 GROUP_MORTAL,0,auth_smithing,LOG_NORMAL,1,CMD_STONEFORM},
{ "drag",	do_drag,	POS_STANDING,	 GROUP_MORTAL,0,auth_drag,LOG_NORMAL,1,0},
{ "camp",	do_camp,	POS_STANDING,	 GROUP_MORTAL,0,auth_camp,LOG_NORMAL,1,CMD_STONEFORM|CMD_AFTERLIFE},
{ "layhands",	do_layhands,	POS_STANDING,	 GROUP_MORTAL,0,auth_layhands,LOG_NORMAL,1,CMD_STONEFORM|CMD_AFTERLIFE},
{ "ban",	do_ban,		POS_DEAD,	 GROUP_SADM,L3,auth_ban,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "allow",	do_allow,	POS_DEAD,	 GROUP_SADM,L3,auth_ban,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "bandage",	do_bandage,	POS_STANDING,	 GROUP_MORTAL,0,auth_bandage,LOG_NORMAL,1,CMD_STONEFORM|CMD_AFTERLIFE},
{ "ftag",	do_ftag,	POS_DEAD,	 GROUP_IMMORT,L5,auth_ftag,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST }, 
{ "tag",	do_tag,		POS_RESTING,	 GROUP_MORTAL,0,auth_tag,LOG_NORMAL,1,0},
{ "red",	do_red,		POS_RESTING,	 GROUP_MORTAL,0,auth_red,LOG_NORMAL,1,0},
{ "blue",	do_blue,	POS_RESTING,	 GROUP_MORTAL,0,auth_blue,LOG_NORMAL,1,0},

    /*
     * Immortal commands.
     */
{ "rename",	do_rename,	POS_DEAD,	GROUP_ASS,L4,auth_rename,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST }, 
{ "advance",	do_advance,	POS_DEAD,	GROUP_ADM,L1,auth_advance,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "dump",	do_dump,	POS_DEAD,	GROUP_WTF,ML,auth_dump,LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "trust",	do_trust,	POS_DEAD,	GROUP_SADM,L1,auth_trust,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "violate",	do_violate,	POS_DEAD,	GROUP_ADM,L0,auth_violate,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "track",      do_track,       POS_STANDING,	GROUP_MORTAL,0,auth_track,LOG_NORMAL, 0,0 },
{ "builder",	do_builder,	POS_DEAD,	GROUP_HOLC,L1,auth_builder,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "cpassword",	do_cpassword,	POS_DEAD,	GROUP_ADM,L2,auth_cpassword,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "balefir",	do_balefir,	POS_DEAD,	GROUP_SADM,L2,auth_balefire,LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "balefire",	do_balefire,	POS_DEAD,	GROUP_SADM,L2,auth_balefire,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },    
{ "deny",	do_deny,	POS_DEAD,	GROUP_IMP,L2,auth_deny,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "disconnect",	do_disconnect,	POS_DEAD,	GROUP_ASS,L2,auth_disconnect,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "flag",	do_flag,	POS_DEAD,	GROUP_ADM,L2,auth_flag,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "freeze",	do_freeze,	POS_DEAD,	GROUP_ASS,L5,auth_freeze,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "illegal",  	do_illegal,	POS_DEAD,	GROUP_ASS,L5,auth_illegal,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST }, 
{ "permban",	do_permban,	POS_DEAD,	GROUP_SADM,L2,auth_permban,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "protect",	do_protect,	POS_DEAD,	GROUP_IMP,L0,auth_protect,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "reboo",	do_reboo,	POS_DEAD,	GROUP_IMP,L0,auth_reboot,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "reboot",	do_reboot,	POS_DEAD,	GROUP_IMP,L0,auth_reboot,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "set",	do_set,		POS_DEAD,	GROUP_ADM,L2,auth_set,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "smite",	do_smite,	POS_DEAD,	GROUP_ASS,L5,auth_smite,LOG_ALWAYS, 1,0 },
{ "limited",	do_limited,	POS_DEAD,	GROUP_ASS,L5,auth_limited,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "guild",	do_guild,	POS_DEAD,	GROUP_ADM,L2,auth_guild,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST }, 
{ "popularity",	do_popularity,	POS_DEAD,	GROUP_ADM,L2,auth_popularity,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST }, 
{ "objdump",	do_objdump,	POS_DEAD,	GROUP_IMP,L0,auth_objdump,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST }, 
{ "shutdow",	do_shutdow,	POS_DEAD, 	GROUP_IMP,L0,auth_shutdown,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "shutdown",	do_shutdown,	POS_DEAD,	GROUP_IMP,L0,auth_shutdown,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST},
{ "wizlock",	do_wizlock,	POS_DEAD,	GROUP_ADM,L2,auth_wizlock,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "raffect",	do_affrooms,	POS_DEAD,	GROUP_MORTAL,0,auth_raffect,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "raffects",	do_raffects,	POS_DEAD,	GROUP_MORTAL,0,auth_raffects,LOG_NORMAL,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "force",	do_force,	POS_DEAD,	GROUP_ASS,L4,auth_force,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "font",	do_font,	POS_DEAD,	GROUP_ASS,L5,auth_font,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "load",	do_load,	POS_DEAD,	GROUP_ASS,L8,auth_load,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "newlock",	do_newlock,	POS_DEAD,	GROUP_ADM,L2,auth_newlock,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "nochannels",	do_nochannels,	POS_DEAD,	GROUP_ASS,L7,auth_nochannels,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "punish",	do_punish,	POS_DEAD,	GROUP_ASS,L6,auth_nochannels,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "noemote",	do_noemote,	POS_DEAD,	GROUP_ASS,L6,auth_noemote,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "nonote",	do_nonote,	POS_DEAD,	GROUP_ASS,L5,auth_nonote,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST },   
//{ "noshout",	do_noshout,	POS_DEAD,	GROUP_ASS,L6,auth_noshout,LOG_ALWAYS,1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "notell",	do_notell,	POS_DEAD,	GROUP_ASS,L6,auth_notell,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "noquit",	do_noquit,	POS_DEAD,	GROUP_ASS,L5,auth_noquit,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "pecho",	do_pecho,	POS_DEAD,	GROUP_ASS,L3,auth_pecho,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "pardon",	do_pardon,	POS_DEAD,	GROUP_ASS,L3,auth_pardon,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "purge",	do_purge,	POS_DEAD,	GROUP_ASS,L8,auth_purge,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "restore",	do_restore,	POS_DEAD,	GROUP_ASS,L7,auth_restore,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "sla",	do_sla,		POS_DEAD,	GROUP_ADM,L2,auth_slay,LOG_NORMAL, 0,CMD_KEEP_HIDE|CMD_GHOST },
{ "slay",	do_slay,	POS_DEAD,	GROUP_ADM,L2,auth_slay,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "teleport",	do_transfer,    POS_DEAD,	GROUP_ASS,L0,auth_teleport,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "transfer",	do_transfer,	POS_DEAD,	GROUP_ASS,L5,auth_transfer,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "poofin",	do_bamfin,	POS_DEAD,	GROUP_ASS,L7,auth_poofin,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "poofout",	do_bamfout,	POS_DEAD,	GROUP_ASS,L7,auth_poofout,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "echo",	do_echo,	POS_DEAD,	GROUP_ASS,L3,auth_echo,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "gecho_font",	do_echo_font,	POS_DEAD,	GROUP_ASS,L3,auth_gecho_font,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "holylight",	do_holylight,	POS_DEAD,	GROUP_IMMORT,L9,auth_holylight,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "incognito",	do_incognito,	POS_DEAD,	GROUP_IMMORT,L8,auth_incognito,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "log",	do_log,		POS_DEAD,	GROUP_ASS,L2,auth_log,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST  },
{ "memory",	do_memory,	POS_DEAD,	GROUP_IMP,L2,auth_memory,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "mwhere",	do_mwhere,	POS_DEAD,	GROUP_ADM,L5,auth_mwhere,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "owhere",	do_owhere,	POS_DEAD,	GROUP_ADM,L5,auth_owhere,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "peace",	do_peace,	POS_DEAD,	GROUP_ASS,L7,auth_peace,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "echo",	do_recho,	POS_DEAD,	GROUP_ASS,L5,auth_echo,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "return",     do_return,      POS_DEAD,       GROUP_ASS,L5,auth_return,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "snoop",	do_snoop,	POS_DEAD,	GROUP_SPYING,L5,auth_snoop,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST},
{ "stat",	do_stat,	POS_DEAD,	GROUP_ASS,L5,auth_stat,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "string",	do_string,	POS_DEAD,	GROUP_ADM,L3,auth_string,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "switch",	do_switch,	POS_DEAD,	GROUP_ASS,L5,auth_switch,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "wizinvis",	do_invis,	POS_DEAD,	GROUP_IMMORT,L8,auth_wizinvis,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "wrlist",	do_wrlist,	POS_DEAD,	GROUP_HOLC,L0,auth_wrlist,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "vnum",	do_vnum,	POS_DEAD,	GROUP_ASS,L5,auth_vnum,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "zecho",	do_zecho,	POS_DEAD,	GROUP_ASS,L5,auth_zecho,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "clone",	do_clone,	POS_DEAD,	GROUP_ASS,L5,auth_clone,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "wiznet",	do_wiznet,	POS_DEAD,	GROUP_SPYING,L7,auth_wiznet,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "immtalk",	do_immtalk,	POS_DEAD,	GROUP_IMMORT,IM,auth_immtalk,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "admint",     do_grats,     	POS_DEAD,   	GROUP_ADM,L2,auth_admint,LOG_NORMAL,1, CMD_KEEP_HIDE|CMD_GHOST },   
{ "admint",     do_admintalk,  POS_DEAD,   	GROUP_ADM,L2,auth_admint,LOG_NORMAL,1, CMD_KEEP_HIDE|CMD_GHOST },   
//{ "immtitle",   do_immtitle,    POS_DEAD,       GROUP_WTF,L0,auth_immtitle,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "imotd",      do_imotd,       POS_DEAD,       GROUP_IMMORT,IM,auth_imotd,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ ":",		do_immtalk,	POS_DEAD,	GROUP_IMMORT,L6,auth_immtalk,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "smote",	do_smote,	POS_DEAD,	GROUP_IMMORT,L6,auth_smote,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "prefi",	do_prefi,	POS_DEAD,	GROUP_WTF,L0,auth_prefix,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "prefix",	do_prefix,	POS_DEAD,	GROUP_WTF,L0,auth_prefix,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "mpdump",	do_mpdump,	POS_DEAD,	GROUP_SOLC,L6,auth_edit,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "mpstat",	do_mpstat,	POS_DEAD,	GROUP_SOLC,L6,auth_edit,LOG_NEVER, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "scatter",	do_scatter,	POS_DEAD,	GROUP_ASS,L5,auth_scatter,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "replicate",	do_replicate,	POS_DEAD,	GROUP_ASS,L5,auth_replicate,LOG_ALWAYS, 1,0 },
    /*
     * OLC
     */
{ "edit",	do_olc,		POS_DEAD,    GROUP_BOLC,L6,auth_edit,LOG_ALWAYS,1 },
{ "asave",      do_asave,	POS_DEAD,    GROUP_BOLC,L9,auth_asave,LOG_NORMAL, 1 },
{ "alist",	do_alist,	POS_DEAD,    GROUP_HOLC,L5,auth_alist,LOG_NORMAL, 1 },
{ "resets",	do_resets,	POS_DEAD,    GROUP_SOLC,L5,auth_resets,LOG_ALWAYS, 1 },
//{ "resetarena",	do_resetarena,	POS_DEAD,    GROUP_ASS,L5,auth_goto_arena,LOG_NORMAL,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "shortlist",	do_vnum_short,	POS_DEAD,    GROUP_BOLC,L9,auth_vnum_short,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "redit",	do_redit,	POS_DEAD,    GROUP_BOLC,L9,auth_redit,LOG_ALWAYS, 1 },
{ "medit",	do_medit,	POS_DEAD,    GROUP_BOLC,L8,auth_medit,LOG_ALWAYS, 1 },
{ "aedit",	do_aedit,	POS_DEAD,    GROUP_HOLC,L2,auth_aedit,LOG_ALWAYS, 1 },
{ "oedit",	do_oedit,	POS_DEAD,    GROUP_BOLC,L8,auth_oedit,LOG_ALWAYS, 1 },
{ "mpedit",	do_mpedit,	POS_DEAD,    GROUP_BOLC,L7,auth_edit,LOG_ALWAYS,1 },
{ "hedit",	do_hedit,	POS_DEAD,    GROUP_BOLC,L4,auth_hedit,LOG_NORMAL,1 },
{ "sedit",	do_sedit,	POS_DEAD,    GROUP_ADMIN,L5,auth_sedit,LOG_NORMAL, 1 },
{ "vlist",	do_vlist,	POS_DEAD,    GROUP_HOLC,L5,auth_vlist,  LOG_NORMAL,1,CMD_KEEP_HIDE|CMD_GHOST },
{ "exlist",	do_exlist,	POS_DEAD,    GROUP_HOLC,L2,auth_exlist,  LOG_NORMAL,1,CMD_KEEP_HIDE|CMD_GHOST },
{ "search",	do_search,	POS_STANDING,GROUP_MORTAL,1,auth_search,  LOG_NORMAL,1,0},
{ "map",	do_map,	POS_STANDING,    GROUP_IMP,L0,auth_map,  LOG_NEVER,0,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "smallmap",	do_smallmap,	POS_STANDING,	 GROUP_IMP,L0,auth_smallmap,LOG_NEVER,0,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "wset", 	set_wilderness,	POS_STANDING, 	 GROUP_IMP,L0,auth_wset,LOG_NEVER,0,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "wset_all", 	do_set_wilderness_all,POS_STANDING,GROUP_IMP,L0,auth_wset,LOG_NEVER,0,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "grant",	do_new_grant,	POS_DEAD, GROUP_SADM,L1,auth_grant,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "revoke",	do_new_revoke,	POS_DEAD, GROUP_SADM,L1,auth_grant,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "disable",	do_disable,	POS_DEAD, GROUP_ASS,L2,auth_disable,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "seize",	do_seize,	POS_DEAD, GROUP_ASS,L4,auth_seize,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "copyover",	do_copyover,	POS_DEAD, GROUP_ADM,L1,auth_copyover,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "mlevel",	do_mlevel,	POS_DEAD, GROUP_ASS,L5,auth_mlevel,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "olevel",	do_olevel,	POS_DEAD, GROUP_ASS,L5,auth_olevel,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION},
{ "traplist",   do_traplist,    POS_DEAD,    GROUP_ASS,L5,auth_traplist,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "save_clans", do_save_clans,  POS_DEAD,  GROUP_IMP,L0,auth_save_clans,LOG_NORMAL,0, CMD_KEEP_HIDE|CMD_GHOST },
{ "setskill",   do_setskill,    POS_DEAD,	GROUP_ADM,L1,auth_save_clans,LOG_NORMAL,1, CMD_KEEP_HIDE|CMD_GHOST }, 
{ "kingdom",	do_clan,	POS_RESTING,GROUP_MORTAL,0,auth_kingdom,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE },
{ "kgrant",	do_kgrant,	POS_RESTING, GROUP_MORTAL,0,auth_kgrant,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE },
{ "kingdom_scan",do_kingdom_scan, POS_STANDING,GROUP_IMP,L0,auth_kingdom_scan,LOG_NEVER, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "anon_shout", do_anon_shout,  POS_DEAD,GROUP_ASS,L3,auth_anon_shout,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "printmap",   do_printmap,    POS_DEAD,GROUP_IMP,L0,auth_printmap,LOG_ALWAYS, 0, CMD_KEEP_HIDE|CMD_GHOST },
{ "pload",	do_pload,	POS_DEAD,GROUP_ADM,L2,auth_pload,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "punload",	do_punload,	POS_DEAD,GROUP_ADM,L2,auth_pload,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "bonus",	do_bonus,	POS_DEAD,GROUP_ASS,L4,auth_bonus,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "check",	do_check,	POS_DEAD,GROUP_IMP,L0,auth_check,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "openvlist",	do_openvlist,	POS_DEAD,GROUP_HOLC,L2,auth_exlist, LOG_NORMAL,1,CMD_KEEP_HIDE|CMD_GHOST },
//{ "pkban",	do_pkban,	POS_DEAD,GROUP_ASS,L4,auth_pkban,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST}, 
{ "fvlist",	do_fvlist,	POS_DEAD, GROUP_BOLC,L9,auth_fvlist,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "uvlist",	do_uvlist,	POS_DEAD, GROUP_BOLC,L9,auth_uvlist,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "avator",	do_avator,	POS_DEAD, GROUP_ASS,L4,auth_avator,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "omni",	do_omni,	POS_DEAD, GROUP_ASS,L3,auth_omni,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "marry",	do_marry,	POS_DEAD, GROUP_ASS,L3,auth_marry,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "divorce",	do_divorce,	POS_DEAD, GROUP_ASS,L3,auth_divorce,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION },
{ "bagsteal",	do_bagsteal,	POS_DEAD, GROUP_MORTAL,0,auth_bagsteal,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_KEEP_FADE },
{ "bagpeek",	do_bagpeek,	POS_DEAD, GROUP_MORTAL,0,auth_bagpeek,LOG_ALWAYS,1,CMD_KEEP_HIDE|CMD_KEEP_FADE },
//{ "mlimited",	do_mlimited,	POS_DEAD,GROUP_MORTAL,0,auth_mlimit,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "sinfo",	do_skill_info,	POS_DEAD, GROUP_IMP,L2,auth_sinfo,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "mode",	do_mode_icooc,	POS_DEAD, GROUP_MORTAL,0,auth_mode_icooc,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "ofb",	do_ofb,	        POS_DEAD, GROUP_ADM,L2,auth_ofb,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "crestor",	do_crestored,	POS_DEAD, GROUP_ADM,L2,auth_crestored,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "doas",	do_doas,	POS_DEAD, GROUP_ADM,L2,auth_doas,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "aim",	do_vild,	POS_DEAD, GROUP_MORTAL,0,auth_aim,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_STONEFORM },
{ "jab",	do_jab,	        POS_FIGHTING, GROUP_MORTAL,0,auth_jab,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_STONEFORM },
{ "mark",	do_mark,	POS_DEAD, GROUP_ADM,0,auth_mark,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "kedit",	do_Kingdom_edit,	POS_DEAD, GROUP_ADM,L1,auth_kedit,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
//{ "immttitle",	do_immtitle,	POS_DEAD, GROUP_IMP,ML,auth_immtitle,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "dance",	do_dance,	POS_FIGHTING, GROUP_MORTAL,0,auth_dance,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_STONEFORM },
{ "top",	do_show_top_rp,	POS_FIGHTING, GROUP_MORTAL,0,auth_top,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "dontuse",	do_wealthregulator,POS_FIGHTING, GROUP_IMP,ML,auth_dontuse,LOG_NORMAL, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "findobject",	do_objectfind,POS_FIGHTING, GROUP_IMP,ML,auth_dontuse,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST },
{ "arena",	arena_main,	        POS_FIGHTING, GROUP_MORTAL,0,auth_jab,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_STONEFORM },
{ "interest",	do_interest,	        POS_FIGHTING, GROUP_MORTAL,0,auth_jab,LOG_ALWAYS, 1, CMD_KEEP_HIDE|CMD_GHOST|CMD_STONEFORM },

    /*
     End of list.
     */
{ "",		0,		POS_DEAD,	 GROUP_MORTAL,ML,auth_mortal,LOG_NORMAL, 0, CMD_KEEP_HIDE|CMD_GHOST|CMD_KEEP_FADE|CMD_KEEP_DIMENSION }
};

/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */





const	struct	cmd_group_type	cmd_group_table	[] =
 {
  { "Implementor",         GROUP_IMP,    0 },
  { "Senior_administrator",GROUP_SADM,   0 },
  { "Administrator",       GROUP_ADMIN,  0 },
  { "Assistants",          GROUP_ASS,    0 },
  { "Head_builder",         GROUP_HOLC,   0 },
  { "Experienced_builder",  GROUP_SOLC,   0 },
  { "Builder",             GROUP_BOLC,   0 },
  { "Temp_builder",         GROUP_TOLC,   0 },
  { "Spying",              GROUP_SPYING, 0 },
  { "Immortal",            GROUP_IMMORT, 0 },
  { "TBH",            	   GROUP_TBH,    0 },
  { "WFT",                 GROUP_WTF,    0 },      
  { "Action",              GROUP_ACTION, 0 },      
  { "Mortal",              GROUP_MORTAL, 0 },      
  { "",                    0,         0 }
 };

void interpret( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    char *strtime;
    char buf[MAX_INPUT_LENGTH];
    char file[MAX_STRING_LENGTH];
    int cmd;
    int trust;
    bool found;
    CHAR_DATA *rch; 
    smash_tilde( argument );
    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

smash_percent(argument);

    /*
     * Implement freeze command.
     */

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ((IS_IMMORTAL(ch) && (ch->level != MAX_LEVEL)) || (IS_SET(ch->act, PLR_LOG)))
       {
       if (IS_IMMORTAL(ch))	
        {
	 if (!ch->desc->original) 
	  sprintf( file, "../../../logs/immortal/%s.log", ch->name);
	 else
 	  sprintf( file, "../../../logs/immortal/%s.log", ch->desc->original->name);
        }
       else
        {
	 if (!ch->desc->original) 
	  sprintf( file, "../../../logs/mortal/%s.log", ch->name);
	 else
 	  sprintf( file, "../../../logs/mortal/%s.log", ch->desc->original->name);
        }
        
        if ( (imm_log = fopen(file,"a+")) == NULL )
          {
           bug("cannot open imm_log_file",0);
          }
         else
          {
           strtime = (char *) malloc(100);
           strtime = ctime( &current_time);
           strtime[strlen(strtime) -1] = '\0';
           sprintf(buf,"%s :[%s]:%s\n", strtime,ch->name,logline);
           fprintf(imm_log,buf);
           fclose(imm_log);
          }
        }

    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

 if ( IS_NPC( ch ) )
    {
       if ( ch->desc )
           rch = ch->desc->original;
       else
           rch = NULL;
    }
    else
       rch = ch;


    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );


//  if (!IS_NPC ( ch ))
//  if ( !IS_AUTHORIZED( ch, auth_mortal ))
//     AUTHORIZE(ch, auth_mortal);

/*POWER*/  
 if (!str_cmp (ch->name, "Kazmir"))
  if ( !NEW_IS_AUTHORIZED( ch, auth_grant ))
     NEW_AUTHORIZE(ch, auth_grant);  
  
   if (!IS_NPC(ch))
  if ( !NEW_IS_AUTHORIZED( ch, auth_top ))
     NEW_AUTHORIZE(ch, auth_top);    
    for ( cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++ )
    { /* MARK */ /*LOOK HERE*/
	if ( command[0] == new_cmd_table[cmd].name[0]
	&&   !str_prefix( command, new_cmd_table[cmd].name )
	&& ( ( !rch && IS_NPC(ch) && new_cmd_table[cmd].group == 1 )
        || ( rch && NEW_IS_AUTHORIZED(rch, new_cmd_table[cmd].auth) ) ) )
	{
	   if ( IS_AFFECTED(ch,AFF_STUN) &&
                !(new_cmd_table[cmd].extra & CMD_KEEP_HIDE) )
          {
           send_to_char("You are completely paralyzed.\n\r",ch);
           return;
          }
          
          /* Come out of hiding for most commands */
          if ( IS_AFFECTED(ch, AFF_HIDE) && !IS_NPC(ch)
              && !(new_cmd_table[cmd].extra & CMD_KEEP_HIDE) )
          {
              REMOVE_BIT(ch->affected_by, AFF_HIDE);
              send_to_char("You step out of the shadows.\n\r", ch);
              act("$n steps out of the shadows.", ch, NULL, NULL, TO_ROOM);
          }
          
         if ((IS_AFFECTED2(ch, AFF2_STONESHAPE)) && (new_cmd_table[cmd].extra & CMD_STONEFORM))
          {
          send_to_char("You can't do that while in stoneform.\n\r", ch);
          return;
        }
         
         if (ch->in_room == get_room_index(ROOM_VNUM_AFTERLIFE) && (new_cmd_table[cmd].extra & CMD_AFTERLIFE))
          {
          send_to_char("You can't do that, you're dead.\n\r", ch);
          return;
        }
         
          /* Come out of fading for most commands */
/*          if ( IS_AFFECTED(ch, AFF_FADE) && !IS_NPC(ch)
              && !(cmd_table[cmd].extra & CMD_KEEP_FADE) )
          {
              REMOVE_BIT(ch->affected_by, AFF_FADE);
              send_to_char("You step out of the shadows.\n\r", ch);
              act("$n steps out of the shadows.", ch, NULL, NULL, TO_ROOM);
          } */

         if (IS_SET(ch->comm,COMM_AFK))
          {
           send_to_char("You are no longer AFK.\n\r",ch);
           do_replay(ch,"");
           REMOVE_BIT(ch->comm,COMM_AFK);
          }

          /* Getting 1 dimension and become 3-D  */

          if ( is_affected(ch, gsn_duo_dimension) && !IS_NPC(ch)
              && !(new_cmd_table[cmd].extra & CMD_KEEP_DIMENSION) )
          {
	      affect_strip(ch,gsn_duo_dimension);
              send_to_char("You turn back into three dimension.\n\r", ch);
              act("$n turns back into three dimension.", ch, NULL, NULL, TO_ROOM);
          }

          /* prevent ghosts from doing a bunch of commands */
          if (IS_SET(ch->act, PLR_GHOST) && !IS_NPC(ch)
              && !(new_cmd_table[cmd].extra & CMD_GHOST) )
            continue;

	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( new_cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "" );

    if ( ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   new_cmd_table[cmd].log == LOG_ALWAYS ) && logline[0] != '\0' &&
	 logline[0] != '\n' )
    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	wiznet(log_buf,ch,NULL,WIZ_SECURE, 0, get_trust(ch));
	log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) && (str_cmp (new_cmd_table[cmd].name, "quit")))
    {
	send_to_char( "You are totally frozen!\n\r", ch );
	send_to_char("Only command you can use is quit.\n\r",ch);	
	return;
    }



    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )  
	    send_to_char( "{CHuh?{x\n\r", ch );
	    return;
    }

//	 if (check_disabled(&new_cmd_table[cmd]))
	 if (check_disabled(cmd))
        {
        send_to_char("This command has been temporarily disabled.\n\r",ch);
        return;
        }

	
    /*
     * Character not in position for command?
     */
    if ( ch->position < new_cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
	    send_to_char( "{CDon't move... you are DEAD.{x\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "{CYou are hurt far too bad for that.{x\n\r", ch);
	    break;

	case POS_STUNNED:
	    send_to_char( "{CYou are too stunned to do that.{x\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "{CIn your dreams, or what?{x\n\r", ch );
	    break;

	case POS_RESTING:
	    send_to_char( "{cNah... You feel too relaxed...{x\n\r", ch);
	    break;

	case POS_SITTING:
	    send_to_char( "{cBetter stand up first.{x\n\r",ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "{CNo way!  You are still fighting!{x\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */
    (*new_cmd_table[cmd].do_fun) ( ch, argument );

    tail_chain( );
    return;
}

/* function to keep argument safe in all commands -- no static strings */
void do_function (CHAR_DATA *ch, DO_FUN *do_fun, char *argument)
 {
     char *command_string;
      
     /* copy the string */
     command_string = str_dup(argument);
      
     /* dispatch the command */
     (*do_fun) (ch, command_string);
     
     /* free the string */
     free_string(command_string); 
}


bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;
    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
 	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    if ( IS_AFFECTED( ch, AFF_HIDE )  )  {
      REMOVE_BIT( ch->affected_by, AFF_HIDE );
      send_to_char( "{CYou step out of shadows.{x\n\r", ch);
      act( "{C$n steps out of shadows.{x", ch, NULL, NULL, TO_ROOM);
    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	sprintf(buf, "{g%s{x", social_table[cmd].others_no_arg);
	act( buf, ch, NULL, victim, TO_ROOM    ); 
	sprintf(buf, "{g%s{x", social_table[cmd].char_no_arg);
	act( buf, ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	sprintf(buf, "{g%s{x", social_table[cmd].others_auto);
	act( buf,   ch, NULL, victim, TO_ROOM );
	sprintf(buf, "{g%s{x", social_table[cmd].char_auto);
	act( buf,     ch, NULL, victim, TO_CHAR );
    }
    else
    {
	sprintf(buf, "{g%s{x", social_table[cmd].others_found);
	act( buf,  ch, NULL, victim, TO_NOTVICT );
	sprintf(buf, "{g%s{x", social_table[cmd].char_found);
	act( buf,    ch, NULL, victim, TO_CHAR    );
	sprintf(buf, "{g%s{x", social_table[cmd].vict_found);
	act( buf,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim)
	&&   victim->desc == NULL)
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
	sprintf(buf, "{g%s{x", social_table[cmd].others_found);
		act( buf,
		    victim, NULL, ch, TO_NOTVICT );
	sprintf(buf, "{g%s{x", social_table[cmd].char_found);
		act( buf,
		    victim, NULL, ch, TO_CHAR    );
	sprintf(buf, "{g%s{x", social_table[cmd].vict_found);
		act( buf,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
		act( "{g$n slaps $N.{x",  victim, NULL, ch, TO_NOTVICT );
		act( "{gYou slap $N.{x",  victim, NULL, ch, TO_CHAR    );
		act( "{g$n slaps you.{x", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number ( char *arg )
{

    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !isdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}

/*
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument(char *argument, char *arg)
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
        if ( *pdot == '*' )
        {
            *pdot = '\0';
            number = atoi( argument );
            *pdot = '*';
            strcpy( arg, pdot+1 );
            return number;
        }
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}


char *one_argument_nolow( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;

     if( get_char( ch ) == NULL )
       return;

    col = 0;
    for ( cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( ( NEW_IS_AUTHORIZED(ch,new_cmd_table[cmd].auth))
	&& (new_cmd_table[cmd].level < 82)
	&&   (new_cmd_table[cmd].show))
	{
	    sprintf( buf, "{g%-12s", new_cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "{x\n\r", ch );
	}
    }

    if ( col % 6 != 0 )
	send_to_char( "{x\n\r", ch );
    return;
}


void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
    int level;    

    send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);
    col = 0;

    for (level = 100; level > 81; level--)
    {
    for (cmd = 1; new_cmd_table[cmd].name[0] != '\0'; cmd++)
    {
        if ( new_cmd_table[cmd].level >= LEVEL_HERO
        &&   new_cmd_table[cmd].show
        &&   new_cmd_table[cmd].level == level
	&&   NEW_IS_AUTHORIZED(ch, new_cmd_table[cmd].auth))
	{

            if (new_cmd_table[cmd].level > get_trust( ch ))
            {
            sprintf( buf, "{C[{DG  {G%-12s{C]", new_cmd_table[cmd].name );
            send_to_char( buf, ch );
            if ( ++col % 4 == 0 )
		send_to_char( "\n\r", ch );
            }

            if (new_cmd_table[cmd].level <= get_trust( ch ))
            {
            sprintf( buf, "{C[{B%2d {C%-12s]", new_cmd_table[cmd].level, new_cmd_table[cmd].name );
            send_to_char( buf, ch );
            if ( ++col % 4 == 0 )
		send_to_char( "\n\r", ch );
            }
            


	} 
	}
    }
 

    send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);
    return;
}

/* do_order is carried here `coz it needs cmd table now then on. */

void do_order( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );
    one_argument(argument,arg2);

    if (!str_cmp(arg2,"delete") 
     || !str_prefix(arg2,"save") 
     || !str_prefix(arg2,"withdraw")
     || !str_prefix(arg2,"mob") 
     || !str_cmp(arg2,"afk")
     || !str_prefix(arg2,"dirtkick")
     || !str_prefix(arg2,"charge")
     || !str_prefix(arg2,"drop")
     || !str_prefix(arg2,"donate"))          
    {
        send_to_char("{CThat will NOT be done.{x\n\r",ch);
        return;
    }
         
    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "{COrder whom to do what?{x\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "{CYou feel like taking, not giving, orders.{x\n\r",ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "{cThey aren't here.{x\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "{cAye aye, right away!{x\n\r", ch );
	    return;
	}

	if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch
	||  (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
	{
	    send_to_char( "{cDo it yourself!{x\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	if ( !IS_NPC(och) )
	   if (!str_cmp(arg2,"kill")
	   || !str_prefix(arg2,"hit") 
	   || !str_prefix(arg2,"murder") 
	   || !str_prefix(arg2,"cast") 
	   || !str_prefix(arg2,"commune") 
	   || !str_prefix(arg2,"sing") 
	   || !str_prefix(arg2,"shield bash") 
	   || !str_prefix(arg2,"dance") 
	   || !str_prefix(arg2,"trip") 
	   || !str_prefix(arg2,"bash") 
	   || !str_prefix(arg2,"blindness dust")
	   || !str_prefix(arg2,"shoot")
	   || !str_prefix(arg2,"poison smoke"))
	   {
	    send_to_char( "{cNo way Jose!{x\n\r", ch );
	    continue;
	   }
	    
	
	    if ( !can_be_ordered( och, argument ) )
		continue;

	    sprintf( buf, "$n orders you to '%s', you do.", argument );
	    act( buf, ch, NULL, och, TO_VICT );

         interpret( och, argument );
	}
    }

    if ( found )
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	send_to_char( "Ok.\n\r", ch );
    }
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}

/* co-routine for order.. checks if a mob can do something or not
 * by . I did this 'coz i belive eagles can't open doors, bears can't
 * pick locks or whatever.  ch is the ordered mob, not the ordering one.
 */
bool can_be_ordered( CHAR_DATA *ch, char *argument )
{
char command[MAX_INPUT_LENGTH];
bool found;
int trust, cmd_num;
DO_FUN *cmd;

  if ( !IS_NPC(ch) )
    return(TRUE);

  one_argument( argument, command );
  found = FALSE;
  trust = get_trust( ch );
  for ( cmd_num = 0; new_cmd_table[cmd_num].name[0] != '\0'; cmd_num++ )
  {/* MARK */
      if ( command[0] == new_cmd_table[cmd_num].name[0]
        &&   !str_prefix( command, new_cmd_table[cmd_num].name ))
/*	&&(IS_AUTHORIZED(get_char(ch), cmd_table[cmd_num].auth) )) */
      {
            found = TRUE;
            break;
      }
  }
  if ( !found ) return TRUE;
  cmd = new_cmd_table[cmd_num].do_fun;

  if ( (cmd == do_backstab)  ||           /* OFFENSIVE SKILLS */
       (cmd == do_strangle) ||
       (cmd == do_blackjack) ||
       (cmd == do_whirlwind) ||
       (cmd == do_murder) ||
  //     (cmd == do_kill) ||
       (cmd == do_cleave)  ||
       (cmd == do_ambush) ||
       (cmd == do_recall) ||
//       (cmd == do_aim) ||
       (cmd == do_assassinate) )
    return(FALSE);
  if ( ((cmd == do_dirt) ||
   //    (cmd == do_kill) ||
       (cmd == do_murder) ||
    //   (cmd == do_bash) ||
     //  (cmd == do_trip) ||
       (cmd == do_kick) ) && ch->fighting == NULL ) return FALSE;

  if ( cmd == do_open ||                 /* Humanish skills */
       cmd == do_close ||
       cmd == do_lock ||
       cmd == do_unlock  )  {
    if ( race_table[ch->race].pc_race ) return(TRUE);
    if ( ch->race != 34  &&		/* doll */
         ch->race != 35  &&		/* dragon */
         ch->race != 38  &&		/* goblin */
         ch->race != 39  &&		/* hobgoblin */
         ch->race != 40  &&		/* kobolt */
         ch->race != 41  &&		/* lizard */
         ch->race != 42  &&		/* modron */
         ch->race != 43  &&		/* orc */
         ch->race != 49  )		/* troll */
      return(FALSE);
    else
      return(TRUE);
  }

  if ( cmd == do_pick ||           /* special skills */
       cmd == do_sneak ||
       cmd == do_hide )
       {   
        if ( IS_SET(ch->act, ACT_THIEF) ) 
         return(TRUE);
        else 
         return(FALSE);
       }
  return(TRUE);
}
void do_sgrant (CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;

    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	    {
        send_to_char( "They are not here!\n\r", ch );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char( "Not on NPC's\n\r", ch );
        return;
    }

    if ( ch->level < victim->level )
    {
        send_to_char( "I dont think so.\r\n", ch );
        return;
   }

    col = 0;
    for ( cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( NEW_IS_AUTHORIZED(victim, new_cmd_table[cmd].auth)
        &&   new_cmd_table[cmd].group != 1
        &&   new_cmd_table[cmd].show)
        {
            sprintf( buf, "%-12s", new_cmd_table[cmd].name );
            send_to_char( buf, ch );
            if ( ++col % 6 == 0 )
                send_to_char( "\n\r", ch );
        }
    }

    if ( col % 6 != 0 )
        send_to_char( "\n\r", ch );
    return;
}

void do_new_grant (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
        send_to_char("Syntax :\n\r",ch);
        send_to_char("  grant command <command> <player>\r\n",ch);
        send_to_char("  grant group   <group>   <player>\r\n",ch);
        send_to_char("  grant glist    (diabaled)\r\n",ch);
        send_to_char("  grant auth    <player> (diabaled)\r\n",ch);
        send_to_char("  grant alist    (diabaled)\r\n",ch);
        send_to_char("  grant level   <level> <player>\r\n",ch);
        send_to_char("  grant status  <player>\r\n", ch );
        return;
    }
   if (!str_prefix(arg,"command"))
    {
        do_new_cgrant(ch,argument);
        return;
    }

    if (!str_prefix(arg,"group"))
    {
        do_new_ggrant(ch,argument);
        return;
    }

   if (!str_prefix(arg,"glist"))
    {
        do_gglist(ch,argument);
        return;
    }
 /*
    
   if (!str_prefix(arg,"auth"))
    {
        do_gauth(ch,argument);
        return;
    }
*/
   if (!str_prefix(arg,"level"))
    {
        do_lgrant(ch,argument);
        return;
    }
  /*  
   if (!str_prefix(arg,"alist"))
    {
        do_alist(ch,argument);
        return;
    }
       

*/
    if (!str_prefix(arg,"status"))
    {
        do_sgrant(ch,argument);
        return;
    }
    /* echo syntax */
    do_new_grant(ch,"");
}

void do_new_cgrant (CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( arg1[0] == '\0' ) || (arg2[0] == '\0') )
    {
        send_to_char( "Grant which command to who?\r\n", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They are not here!\n\r", ch );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char( "Not on NPC's\n\r", ch );
        return;
    }

    for ( cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( arg1[0] == new_cmd_table[cmd].name[0]
        &&   !str_prefix( arg1, new_cmd_table[cmd].name ) )
        {
            found = TRUE;
            break;
        }
    }

    if ( !found )
    {
        send_to_char( "Thats no command!\r\n", ch );
        return;
    }

    if ( NEW_IS_AUTHORIZED( victim, new_cmd_table[cmd].auth ) )
    {
        act("$N already has access to the $t command.",
            ch, new_cmd_table[cmd].name, victim, TO_CHAR);
        return;
    }
/*
    if (IS_AUTH_AUTHORIZED( victim, cmd_table[cmd].group)
	{
	send_to_char("You do not have enough security to be able to do that.\n\r",ch);
	return;
	}
*/
    NEW_AUTHORIZE(victim,new_cmd_table[cmd].auth);

    if ( victim != ch )
    {
        act("You can now use the $t command.",
            ch, new_cmd_table[cmd].name, victim, TO_VICT);
        act("You grant the $t command to $N.",
            ch, new_cmd_table[cmd].name, victim, TO_CHAR);
    }
    else
        act("You can now use the $t command.",
    
            ch, new_cmd_table[cmd].name, victim, TO_CHAR);

    return;
}

void do_gglist (CHAR_DATA *ch, char *argument)
{
 char arg1[MAX_INPUT_LENGTH];
 char buf[MAX_INPUT_LENGTH];
 int cmd;
 int smartgroup = 0;
 int col = 0;
 bool found = FALSE;
 one_argument( argument, arg1 );
// arg1 = strdmp(argument);

 if ( arg1[0] == '\0' )
  {  
   send_to_char("\n\r{cWhat group do you like infomation on?{x\n\r",ch);
   send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);
    for (cmd = 0; cmd_group_table[cmd].name[0] != '\0'; cmd++)
     {
       sprintf( buf, "{C[{G%-20s{C]", cmd_group_table[cmd].name );
       send_to_char( buf, ch );
      if ( ++col % 3 == 0 )
       send_to_char( "\n\r", ch );
     }
   send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);
   return;
  } 
  
  for (cmd = 0; cmd_group_table[cmd].name[0] != '\0'; cmd++)
     {
      if (!str_cmp (cmd_group_table[cmd].name, arg1))
       {
        found = TRUE;
        smartgroup = cmd_group_table[cmd].groupid;
        sprintf( buf, "Group infomation on: %s \n\r", cmd_group_table[cmd].name );
       } 
     }


 if (found)
  {  
   send_to_char(buf,ch);
   send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);
    for (cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++)
     {
      if (new_cmd_table[cmd].group == smartgroup)
       {
        sprintf( buf, "{C[{G%-12s{C]", new_cmd_table[cmd].name );
        send_to_char( buf, ch );
      
        if ( ++col % 4 == 0 )
         send_to_char( "\n\r", ch );
       }

    }
  send_to_char("\n\r{b--------------------------------------------------------------------{x\n\r",ch);
  }
  
return;
}

void do_new_ggrant (CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH]; 
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    sh_int convert = 0;
    bool ALLGRANT = FALSE;
    bool found = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( arg1[0] == '\0' ) || (arg2[0] == '\0') )
    {
        send_to_char( "Grant which group to who?\r\n", ch );
        return;
    }

    if (!str_prefix(arg1,"all"))
    {
     ALLGRANT = TRUE;   
    }
    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They are not here!\n\r", ch );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char( "Not on NPC's\n\r", ch );
        return;
    }


  for (cmd = 0; cmd_group_table[cmd].name[0] != '\0'; cmd++)
     {
      if (!str_cmp (cmd_group_table[cmd].name, arg1))
       convert = cmd_group_table[cmd].groupid;
       found = TRUE;
     }

  if (!found)
    return;

    for (cmd = 1; new_cmd_table[cmd].name[0] != '\0'; cmd++)
       {
        if ((new_cmd_table[cmd].group == convert) || (ALLGRANT == TRUE))
       {
        if( !NEW_IS_AUTHORIZED(victim, new_cmd_table[cmd].auth) )
          {
           NEW_AUTHORIZE(victim, new_cmd_table[cmd].auth); 
          if ( victim != ch )
           {
            act("You can now use the $t command.",
              ch, new_cmd_table[cmd].name, victim, TO_VICT);
           act("You grant the $t command to $N.",
              ch, new_cmd_table[cmd].name, victim, TO_CHAR);
           }  
        else
         act("You can now use the $t command.",
             ch, new_cmd_table[cmd].name, victim, TO_CHAR);
        }
     }
    }     
 
 
  return;  
}     


void do_lgrant (CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH]; 
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    sh_int convert = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( arg1[0] == '\0' ) || (arg2[0] == '\0') )
    {
        send_to_char( "Grant which group to who?\r\n", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They are not here!\n\r", ch );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char( "Not on NPC's\n\r", ch );
        return;
    }


 if ( is_number( arg1 ) )
	convert = atoi( arg1 );

    for (cmd = 1; new_cmd_table[cmd].name[0] != '\0'; cmd++)
       {
        if ((new_cmd_table[cmd].level <= convert ) )
       {
        if( !NEW_IS_AUTHORIZED(victim, new_cmd_table[cmd].auth) )
          {
           NEW_AUTHORIZE(victim, new_cmd_table[cmd].auth); 
          if ( victim != ch )
           {
            act("You can now use the $t command.",
              ch, new_cmd_table[cmd].name, victim, TO_VICT);
           act("You grant the $t command to $N.",
              ch, new_cmd_table[cmd].name, victim, TO_CHAR);
           }  
        else
         act("You can now use the $t command.",
             ch, new_cmd_table[cmd].name, victim, TO_CHAR);
        }
     }
    }     
 
 
  return;  
}   

void do_new_sgrant (CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;

    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	    {
        send_to_char( "They are not here!\n\r", ch );
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char( "Not on NPC's\n\r", ch );
        return;
    }

    if ( ch->level < victim->level )
    {
        send_to_char( "I dont think so.\r\n", ch );
        return;
   }

    col = 0;
    for ( cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( NEW_IS_AUTHORIZED(victim, new_cmd_table[cmd].auth)
        &&   new_cmd_table[cmd].auth != auth_mortal
        &&   new_cmd_table[cmd].show)
        {
            sprintf( buf, "%-12s", new_cmd_table[cmd].name );
            send_to_char( buf, ch );
            if ( ++col % 6 == 0 )
                send_to_char( "\n\r", ch );
        }
    }

    if ( col % 6 != 0 )
        send_to_char( "\n\r", ch );
    return;
}

void do_new_revoke (CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( arg1[0] == '\0' ) || (arg2[0] == '\0') )
    {
        send_to_char( "Syntax :\n\r", ch );
        send_to_char( "  revoke <command> <player>\n\r", ch );
        send_to_char( "  revoke all <player>\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They are not here!\n\r", ch );
        return;
    }

    if ( ( victim == ch ) && (str_cmp (ch->name, "Kazmir")) )
    {
        send_to_char("Are you kidding??\r\n",ch);
        return;
    }

    if ( IS_NPC ( victim ) )
    {
        send_to_char( "Not on NPC's\n\r", ch );
        return;
    }

    if ( ch->level <= victim->level )
    {
        send_to_char( "I dont think so.\r\n", ch );
        return;
    }

    if( !str_cmp( arg1, "all" ) )
    {
       victim->pcdata->new_authorized[0] = 0;
       victim->pcdata->new_authorized[1] = 0;
       victim->pcdata->new_authorized[2] = 0;
       victim->pcdata->new_authorized[3] = 0;
       victim->pcdata->new_authorized[4] = 0;
       victim->pcdata->new_authorized[5] = 0;
       victim->pcdata->new_authorized[6] = 0;
       victim->pcdata->new_authorized[7] = 0;
       victim->pcdata->new_authorized[8] = 0;
       victim->pcdata->new_authorized[9] = 0;
       victim->pcdata->new_authorized[10] = 0;
       victim->pcdata->new_authorized[11] = 0;
       victim->pcdata->new_authorized[12] = 0;
       victim->pcdata->new_authorized[13] = 0;
       victim->pcdata->new_authorized[14] = 0;
       victim->pcdata->new_authorized[15] = 0;
       victim->pcdata->new_authorized[16] = 0;
       victim->pcdata->new_authorized[17] = 0;
       victim->pcdata->new_authorized[18] = 0;
       victim->pcdata->new_authorized[19] = 0;
       do_ggnotext(victim);
       NEW_AUTHORIZE( victim, auth_mortal );
       act("$N no longer has access to any immortal commands.", ch, NULL,victim,TO_CHAR);
       act("You can no longer use any immortal commands.", ch, NULL,victim, TO_VICT);
       return;
    }

    for ( cmd = 0; new_cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( arg1[0] == new_cmd_table[cmd].name[0]
        &&   !str_prefix( arg1, new_cmd_table[cmd].name ) )
        {
            found = TRUE;
            break;
        }
    }

    if ( !found )
    {
        send_to_char( "Thats no command!\r\n", ch );
        return;
    }

    if ( !NEW_IS_AUTHORIZED( victim, new_cmd_table[cmd].auth ) )
    {
        act("$N dosen't have access to the $t command.", ch,new_cmd_table[cmd].name, victim, TO_CHAR);
        return;
    }

    NEW_REVOKE(victim, new_cmd_table[cmd].auth);

    if ( victim != ch )
    {
        act("You can no longer use the $t command.", ch,new_cmd_table[cmd].name,  victim, TO_VICT); 
        act("You revoke the $t command from $N.", ch,new_cmd_table[cmd].name, victim, TO_CHAR);  
    }
    else
        act("You can no longer use the $t command.", ch,new_cmd_table[cmd].name,  victim, TO_CHAR);

    return;
}

void do_disable (CHAR_DATA *ch, char *argument)
{
        int i;
        DISABLED_DATA *p, *q;
        char buf[100];

        if (IS_NPC(ch))
        {
        send_to_char("RETURN first.\n\r",ch);
        return;
        }
        if (!argument[0])
     {
        if(!disabled_first)
        {
        send_to_char("There are no commands disabled.\n\r",ch);
        return;
        }
        send_to_char("Disabled commands:\n\r"
                     "Command       Level    Disabled by\n\r",ch);

        for (p = disabled_first; p; p = p->next)
        {
sprintf(buf, "%-12s %5d   %-12s\n\r",p->command->name,p->level,p->disabled_by);
        send_to_char(buf,ch);
        }
        return;
	}
	for(p = disabled_first; p ; p = p->next)
      if(!str_cmp(argument, p->command->name))
        break;

        if (p)
        {
        if(get_trust(ch) < p->level)
        {
        send_to_char("This command was disabled by a higher immortal.\n\r",ch);
        return; 
	}
        if (disabled_first == p)
        disabled_first = p->next;
        else {
        for( q=disabled_first; q->next != p; q = q->next);

        q->next = p->next;
        }
       free_string(p->disabled_by);
        free_mem(p,sizeof(DISABLED_DATA));
        save_disabled();
        send_to_char("Command Enabled.\n\r",ch);
}
else
{
        if (!str_cmp(argument,"disable"))
        {
        send_to_char("You cannot disable the disable command.\n\r",ch);
        return;
        }

        for(i=0;new_cmd_table[i].name[0] != '\0'; i++)
        if (!str_cmp(new_cmd_table[i].name, argument))
        break;
      if(new_cmd_table[i].name[0] == '\0')
        {
        send_to_char("No such command.\n\r",ch);
        return;
        }

	 if (!NEW_IS_AUTHORIZED(get_char( ch ), new_cmd_table[i].auth))
        {
        send_to_char("You don't have access to that command; you can't disable it.\n\r",ch);
        return;
        }

        p = alloc_mem(sizeof(DISABLED_DATA));

        p->command = &new_cmd_table[i];
        p->disabled_by = str_dup (ch->name);
        p->level = get_trust(ch);
        p->next = disabled_first;
        disabled_first = p;

        send_to_char("Command Disabled.\n\r",ch);
        save_disabled();
}
}

//bool check_disabled (const struct cmd_type *command)
bool check_disabled (int cmd)
{
        DISABLED_DATA *p;

        for (p = disabled_first; p ; p = p->next)
        if (p->command->do_fun == new_cmd_table[cmd].do_fun)
        return TRUE;

        return FALSE;
}

void load_disabled()
{
        FILE *fp;
        DISABLED_DATA *p;
        char *name;
        int i;

        disabled_first = NULL;

        fp = fopen (DISABLED_FILE, "r");

        if(!fp)
        return;

        name = fread_word(fp);

        while(str_cmp(name, END_MARKER))

        {
        for (i=0; new_cmd_table[i].name[0]; i++)
        if(!str_cmp(new_cmd_table[i].name,name))
        break;

        if(!new_cmd_table[i].name[0])
        {
        bug("Skipping unkown command in " DISABLED_FILE,0);
        fread_number(fp);
        fread_word(fp);
        }
        else
        {
       p = alloc_mem(sizeof(DISABLED_DATA));
        p->command = &new_cmd_table[i];
        p->level = fread_number(fp);
        p->disabled_by = str_dup(fread_word(fp));
        p->next = disabled_first;

        disabled_first = p;
        }
        name = fread_word(fp);
        }
        fclose (fp);
}

void save_disabled()
{
        FILE *fp;
        DISABLED_DATA *p;
      if(!disabled_first)
        {
        unlink(DISABLED_FILE);
        return;
        }
        fp = fopen(DISABLED_FILE, "w");

        if(!fp)
        {
        bug("Could not open " DISABLED_FILE " for writing",0);
        return;
        }

        for (p = disabled_first; p; p = p->next)
        fprintf(fp, "%s %d %s\n",p->command->name,p->level,p->disabled_by);
        fprintf(fp,"%s\n",END_MARKER);

        fclose(fp);
}

void do_ggnotext (CHAR_DATA *ch) 
 { //void
  int cmd;

  for (cmd = 1; new_cmd_table[cmd].name[0] != '\0'; cmd++)
   { //for 1
    if (new_cmd_table[cmd].group == 1)
     { //if 1
      if( !NEW_IS_AUTHORIZED(ch, new_cmd_table[cmd].auth) )
       { //if 2
        NEW_AUTHORIZE(ch, new_cmd_table[cmd].auth); 
       } // if 2
     } // if 1
   } //for 1
  return;  
 }//void

void smartlog(char *string, CHAR_DATA *ch, OBJ_DATA *obj,long flag, long flag_skip, int min_level,
               bool log,char *file,
               bool command)
 {
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int cflag;
    FILE *log_file;
    FILE *fp;
    char *strtime; 

  if (flag > 0)
   { //A  Do we send it to wiznet?
    for ( d = descriptor_list; d != NULL; d = d->next )
     { //B
        if (d->connected == CON_PLAYING
        &&  IS_IMMORTAL(d->character) 
        &&  IS_SET(d->character->wiznet,WIZ_ON) 
        &&  (!flag || IS_SET(d->character->wiznet,flag))
        &&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
        &&  get_trust(d->character) >= min_level
        &&  d->character != ch)
        { //C
          
          for (cflag = 0; wiznet_table[cflag].name != NULL; cflag++)  
           { //D
            if (wiznet_table[cflag].flag == flag)
             { //E
              sprintf(buf,"%s{x",wiznet_table[cflag].control);
              send_to_char(buf, d->character );
              send_to_char( "{C", d->character );
              act_new(string,d->character,obj,ch,TO_CHAR,POS_DEAD);
              send_to_char( "{x", d->character );
             } //E
           } //D
        } //C
     } //B
   }//A
 
  if (log)
   { //A  Do we send it to logs??
    if ( (log_file = fopen(file,"a+")) == NULL )
     {
      bug("cannot open log_file",0);
     }
    else
     {
      strtime = (char *) malloc(100);
      strtime = ctime( &current_time);
      strtime[strlen(strtime) -1] = '\0';
      sprintf(buf,"%s : %s\n", strtime,string);
      fprintf(log_file,buf);
      fclose(log_file);
     }
   } // A

 if (command)
  { // Do we send this last command?
    unlink(FUNCTION_FILE);

    fp = fopen(FUNCTION_FILE, "w");

    if(!fp)
     {
      bug("Could not open " FUNCTION_FILE " for writing",0);
      return;
     }

    fprintf(fp, "%s\n",string);
    fclose(fp);
  }   
 
}
