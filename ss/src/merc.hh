/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   			   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   	   *
 *									   			   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   		   *
 *									   			   *
 *  Much time and thought has gone into this software and you are	         *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   		   *
 ***************************************************************************/
 
/***************************************************************************
*	ROM 2.4 is copyright 1993-1995 Russ Taylor			         *
*	ROM has been brought to you by the ROM consortium		   	   *
*	    Russ Taylor (rtaylor@pacinfo.com)				         *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			         *
*	    Brian Moore (rom@rom.efn.org)				   		   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   	   *
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

/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_DO_ACT( act_fun )		void act_fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#define DECLARE_GAME_FUN( fun )		void fun( )
#define DECLARE_MPROG_FUN_BRIBE( fun ) void fun( )
#define DECLARE_MPROG_FUN_ENTRY( fun ) void fun( )
#define DECLARE_MPROG_FUN_GREET( fun ) void fun( )
#define DECLARE_MPROG_FUN_GIVE( fun ) void fun( )
#define DECLARE_MPROG_FUN_FIGHT( fun ) void fun( )
#define DECLARE_MPROG_FUN_DEATH( fun ) bool fun( )
#define DECLARE_MPROG_FUN_AREA( fun ) void fun( )
#define DECLARE_MPROG_FUN_SPEECH( fun ) void fun( )

#define DECLARE_IPROG_FUN_WEAR(fun) void fun()
#define DECLARE_IPROG_FUN_REMOVE(fun) void fun()
#define DECLARE_IPROG_FUN_GET(fun) void fun()
#define DECLARE_IPROG_FUN_DROP(fun) void fun()
#define DECLARE_IPROG_FUN_SAC(fun) bool fun()
#define DECLARE_IPROG_FUN_ENTRY(fun) void fun()
#define DECLARE_IPROG_FUN_GIVE(fun) void fun()
#define DECLARE_IPROG_FUN_GREET(fun) void fun()
#define DECLARE_IPROG_FUN_FIGHT(fun) void fun()
#define DECLARE_IPROG_FUN_DEATH(fun) bool fun()
#define DECLARE_IPROG_FUN_SPEECH(fun) void fun()
#define DECLARE_IPROG_FUN_AREA(fun) void fun()
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_DO_ACT( act_fun )	DO_ACT    act_fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#define DECLARE_GAME_FUN( fun )		GAME_FUN  fun

#define DECLARE_MPROG_FUN_BRIBE( fun )      MPROG_FUN_BRIBE fun
#define DECLARE_MPROG_FUN_ENTRY( fun )      MPROG_FUN_ENTRY fun
#define DECLARE_MPROG_FUN_GREET( fun )      MPROG_FUN_GREET fun
#define DECLARE_MPROG_FUN_GIVE( fun )       MPROG_FUN_GIVE fun
#define DECLARE_MPROG_FUN_FIGHT( fun )      MPROG_FUN_FIGHT fun
#define DECLARE_MPROG_FUN_DEATH( fun )      MPROG_FUN_DEATH fun
#define DECLARE_MPROG_FUN_AREA( fun )       MPROG_FUN_AREA fun
#define DECLARE_MPROG_FUN_SPEECH( fun )     MPROG_FUN_SPEECH fun

#define DECLARE_IPROG_FUN_WEAR(fun) IPROG_FUN_WEAR fun
#define DECLARE_IPROG_FUN_REMOVE(fun) IPROG_FUN_REMOVE fun
#define DECLARE_IPROG_FUN_GET(fun) IPROG_FUN_GET fun
#define DECLARE_IPROG_FUN_DROP(fun) IPROG_FUN_DROP fun
#define DECLARE_IPROG_FUN_SAC(fun) IPROG_FUN_SAC fun
#define DECLARE_IPROG_FUN_ENTRY(fun) IPROG_FUN_ENTRY fun
#define DECLARE_IPROG_FUN_GIVE(fun) IPROG_FUN_GIVE fun
#define DECLARE_IPROG_FUN_GREET(fun) IPROG_FUN_GREET fun
#define DECLARE_IPROG_FUN_FIGHT(fun) IPROG_FUN_FIGHT fun
#define DECLARE_IPROG_FUN_DEATH(fun) IPROG_FUN_DEATH fun
#define DECLARE_IPROG_FUN_SPEECH(fun) IPROG_FUN_SPEECH fun
#define DECLARE_IPROG_FUN_AREA(fun) IPROG_FUN_AREA fun
#endif


/* system calls */
int unlink();
int system();



/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
/*typedef int				long; */
typedef int				bool;
#define unix
#else
typedef short   int			sh_int;
typedef unsigned char			bool;
#endif

/* ea */
#define MSL MAX_STRING_LENGTH
#define MIL MAX_INPUT_LENGTH

/*
 * Structure types.
 */
typedef struct	affect_data		AFFECT_DATA;
typedef struct	area_data		AREA_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct 	buf_type	 	BUFFER;
typedef struct	char_data		CHAR_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	exit_data		EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct help_area_data          HELP_AREA;   
typedef struct	kill_data		KILL_DATA;
typedef struct	mem_data		MEM_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct	note_data		NOTE_DATA;
typedef struct  obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct  gen_data		GEN_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	shop_data		SHOP_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;
typedef struct  iprog_data              IPROG_DATA;
typedef struct  mprog_data              MPROG_DATA;
typedef struct  room_history_data       ROOM_HISTORY_DATA;
typedef struct  disabled_data           DISABLED_DATA;
typedef struct  auction_data            AUCTION_DATA; 
typedef struct  quest_data              QUEST_DATA;
typedef struct  mprog_list              MPROG_LIST;
typedef struct  mprog_code              MPROG_CODE;
typedef struct  religion_data           RELIGION_DATA;

/*
 * Colour stuff by Lope of Loping Through The MUD
 */
#define CLEAR		"[0;37m"		/* Resets Colour	*/
#define C_RED		"[0;31m"	/* Normal Colours	*/
#define C_GREEN		"[0;32m"
#define C_YELLOW	"[0;33m"
#define C_BLUE		"[0;34m"
#define C_MAGENTA	"[0;35m"
#define C_CYAN		"[0;36m"
#define C_WHITE		"[0;37m"
#define C_D_GREY	"[1;30m"  	/* Light Colors		*/
#define C_B_RED		"[1;31m"
#define C_B_GREEN	"[1;32m"
#define C_B_YELLOW	"[1;33m"
#define C_B_BLUE	"[1;34m"
#define C_B_MAGENTA	"[1;35m"
#define C_B_CYAN	"[1;36m"
#define C_B_WHITE	"[1;37m"
#define STYLE_FLASH     "\E[5m" /* Flash */
#define C_UNDERSCORE	"\e[4m"
#define C_ITALIC	"\e[6m"
/* ANSI Colors */
#define CLR_FOR_DAWN "\033[1;34;44m"
#define CLR_FOR_MORNING "\033[1;37;44m"
#define CLR_FOR_DAY "\033[1;33;44m"
#define CLR_FOR_EVENING "\033[1;31;44m"
#define CLR_FOR_NIGHT "\033[0;30;44m"

#define CLR_BLINK "\033[5m"
#define CLR_BOLD "\033[1m"
#define CLR_NORMAL "\033[0;37;40m"

#define CLR_BLUE "\033[0;34m"


/*
 * Function types.
 */
typedef	void DO_FUN	args( ( CHAR_DATA *ch, char *argument ) );
typedef	void DO_ACT	args( ( CHAR_DATA *ch ) );
typedef bool SPEC_FUN	args( ( CHAR_DATA *ch ) );
typedef void SPELL_FUN	args( ( int sn, int level, CHAR_DATA *ch, void *vo,int target ) );
typedef void GAME_FUN   args( ( CHAR_DATA *ch, CHAR_DATA *croupier, char *argument ) );
typedef void MPROG_FUN_BRIBE args( ( CHAR_DATA *mob, CHAR_DATA *ch, int amount ) );
typedef void MPROG_FUN_ENTRY args( ( CHAR_DATA *mob ) );
typedef void MPROG_FUN_GREET args( ( CHAR_DATA *mob, CHAR_DATA *ch ) );
typedef void MPROG_FUN_GIVE args(( CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj ));
typedef void MPROG_FUN_FIGHT args( ( CHAR_DATA *mob, CHAR_DATA *victim ));
typedef bool MPROG_FUN_DEATH args( ( CHAR_DATA *mob ) );
typedef void MPROG_FUN_AREA args( ( CHAR_DATA *mob ) );
typedef void MPROG_FUN_SPEECH args((CHAR_DATA *mob, CHAR_DATA *ch, char *speech));

typedef void IPROG_FUN_WEAR args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_REMOVE args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_GET args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_DROP args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef bool IPROG_FUN_SAC args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_ENTRY args((OBJ_DATA *obj));
typedef void IPROG_FUN_GIVE args((OBJ_DATA *obj, CHAR_DATA *from, CHAR_DATA *to));
typedef void IPROG_FUN_GREET args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_FIGHT args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef bool IPROG_FUN_DEATH args((OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_SPEECH args((OBJ_DATA *obj, CHAR_DATA *ch, char *speech));
typedef  void IPROG_FUN_AREA args((OBJ_DATA *obj));

/*
 *  COMMAND extra bits..
 */
#define CMD_KEEP_HIDE		1
#define CMD_GHOST		2
#define CMD_KEEP_FADE		4
#define CMD_KEEP_DIMENSION	8
#define CMD_STONEFORM		16
#define CMD_AFTERLIFE		32

/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 5608
#define MAX_INPUT_LENGTH	  256
#define PAGELEN			   22
/* * Game parameters. * Increase the max'es if you add more of something.
* Adjust the pulse numbers to suit yourself. */
#define MAX_CHUNKS 27                    /* Used in ssm.c */

#define MPROG_BRIBE               (A)
#define MPROG_ENTRY               (B)
#define MPROG_GREET               (C)
#define MPROG_GIVE                (D)
#define MPROG_FIGHT               (E)
#define MPROG_DEATH               (F)
#define MPROG_AREA                (G)
#define MPROG_SPEECH              (H)

#define IPROG_WEAR                (A)
#define IPROG_REMOVE              (B)
#define IPROG_DROP                (C)
#define IPROG_SAC                 (D)
#define IPROG_GIVE                (E)
#define IPROG_GREET               (F)
#define IPROG_FIGHT               (G)
#define IPROG_DEATH               (H)
#define IPROG_SPEECH              (I)
#define IPROG_ENTRY               (J)
#define IPROG_GET                 (K)
#define IPROG_AREA                (L)

/*
 * The quest compleate bits
 */
#define QUEST_WEAPONSMITH	(A)
#define QUEST_GOLEM		(B)

/*
 * The kingdom allow command
 */

#define KINGDOM_C_ADD		(A)
#define KINGDOM_C_BAN		(B)
#define KINGDOM_C_WITHDRAW	(C)
#define KINGDOM_C_DEPOSIT	(D)
#define KINGDOM_C_PROMOTE	(E)
#define KINGDOM_C_DEMOTE	(F)
#define KINGDOM_C_CON		(G)
#define KINGDOM_C_DE		(H)
#define KINGDOM_C_WAR		(I)
#define KINGDOM_C_PEACE		(J)
#define KINGDOM_C_GUARD		(K)

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */
#define MAX_SOCIALS		  256
#define MAX_SKILL		  462
#define MAX_GROUP		   50
#define MAX_IN_GROUP		   50
#define MAX_ALIAS		   18

/*
 * MOBprog definitions
 */
#define TRIG_ACT       (A)
#define TRIG_BRIBE     (B)
#define TRIG_DEATH     (C)
#define TRIG_ENTRY     (D)
#define TRIG_FIGHT     (E)
#define TRIG_GIVE      (F)
#define TRIG_GREET     (G)
#define TRIG_GRALL     (H)
#define TRIG_KILL      (I)
#define TRIG_HPCNT     (J)
#define TRIG_RANDOM    (K)
#define TRIG_SPEECH    (L)
#define TRIG_EXIT      (M)
#define TRIG_EXALL     (N)
#define TRIG_DELAY     (O)
#define TRIG_SURR      (P)

struct mprog_list
{
    int                        trig_type;
    char *             trig_phrase;
    int             vnum;
    char *             code;
    MPROG_LIST *       next;
    bool               valid;
};

struct mprog_code
{
    int             vnum;
    char *             code;
    MPROG_CODE *       next;
};

/* max_class does not include a NULL like max_pc_race does */
/* This means max_class=(your number of classes) */
/* and max_pc_race=(your number of races)+1      */
#define CLASS_WAR 0
#define CLASS_MER 1
#define CLASS_PDN 2
#define CLASS_BKT 3
#define CLASS_THI 4
#define CLASS_INQ 5
#define CLASS_SPE 6
#define CLASS_CLE 7
#define CLASS_PRO 8
#define CLASS_MAG 9
#define CLASS_RAN 10
#define CLASS_ASN 11
#define CLASS_BRD 12
#define CLASS_MAL 13
#define CLASS_ALC 14
#define CLASS_DRD 15
#define CLASS_MNK 16
#define CLASS_NEC 17

#define MAX_CLASS		   18
#define MAX_PC_RACE		   20
#define MAX_KINGDOM		   8
#define MAX_LEVEL		   100
#define LEVEL_HERO		   (MAX_LEVEL - 19)
#define LEVEL_IMMORTAL		   (MAX_LEVEL - 18)

#define PULSE_PER_SECOND	    4
#define PULSE_VIOLENCE		  ( 3 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE_TINY	  ( 2 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE_SMALL	  ( 2.5 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE_MEDIUM	  ( 3 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE_LARGE	  ( 3.5 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE_HUGE	  ( 4 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE_GIANT	  ( 4.5 * PULSE_PER_SECOND)
#define PULSE_VIOLENCE_ACT	  ( 3 * PULSE_PER_SECOND)
#define PULSE_MOBILE		  ( 4 * PULSE_PER_SECOND)
#define PULSE_MUSIC		  ( 6 * PULSE_PER_SECOND)
#define PULSE_TICK		  (30 * PULSE_PER_SECOND)
#define PULSE_AREA		  (60 * PULSE_PER_SECOND)
#define FIGHT_DELAY_TIME 	   60
#define LAST_LEVEL_TIME    	   180
#define PULSE_RAFFECT		  ( 3 * PULSE_MOBILE) 
#define PULSE_AUCTION             (10 * PULSE_PER_SECOND) /* 10 seconds */
#define PULSE_UNDERWATER          (20 * PULSE_PER_SECOND)
#define PULSE_WEATHER             (150 * PULSE_PER_SECOND)

#define IMPLEMENTOR		MAX_LEVEL
#define	CREATOR			(MAX_LEVEL - 5)
#define SUPREME			(MAX_LEVEL - 10)
#define DEITY			(MAX_LEVEL - 11)
#define GOD			(MAX_LEVEL - 12)
#define IMMORTAL		(MAX_LEVEL - 13)
#define DEMI			(MAX_LEVEL - 14)
#define ANGEL			(MAX_LEVEL - 15)
#define AVATAR			(MAX_LEVEL - 16)
#define HERO			LEVEL_HERO


#define KINGDOM_NONE	0
#define KINGDOM_GONDOR		1
#define KINGDOM_ALDON		2
#define KINGDOM_MORDUKHAN	3
#define KINGDOM_DRAKKON		4
/* #define KINGDOM_HORDE		5 */
#define KINGDOM_5	5
/* #define KINGDOM_AOYURI		5 */
#define KINGDOM_ARKADIA		6
/*#define KINGDOM_TRISTAN		7*/
#define KINGDOM_XELENIA         7
#define KINGDOM_KAZMIR           8
 
#define MAX_KINGDOM_MEMBERS 40
/*
 * Kingdom structure
 */
struct kingdom_type
{
  char *short_name;
  char *long_name;
  char *name;
  int obj_vnum;
  int room_vnum;
  int gold;
  OBJ_DATA *obj_ptr;
  char *members[40];
  char *rank_titles[10];
  char *rank_titlesf[10];
  int rank_slots[10];
  int at_war_with[MAX_KINGDOM];
};

/* one disabled command */
struct disabled_data
{
       DISABLED_DATA *next; /* pointer to next node */
       struct new_cmd_type const *command; /* pointer to the command struct*/
       char *disabled_by; /* name of disabler */
       sh_int level; /* level of disabler */
};

struct color_type
{
  char *name;
  char *code;
};


#include "board.h"

/*
 * Site ban structure.
 */

#define BAN_SUFFIX		A
#define BAN_PREFIX		B
#define BAN_NEWBIES		C
#define BAN_ALL			D
#define BAN_PERMIT		E
#define BAN_PERMANENT		F

struct  religion_data
{
    int               rank;
    bool              valid;
    char              * name;
    RELIGION_DATA     * next;
};

/* tries to add a struct for the tattoo table */
struct religion_type 
{
char *leader;
char *order;
int vnum;
};

struct hometown_type {
  char *name;
  int altar[3];
  int recall[3];
  int pit[3];
  /* good, neutral, evil */
};

struct ethos_type {
  char *name;
};


struct	ban_data
{
    BAN_DATA *	next;
    bool	valid;
    sh_int	ban_flags;
    sh_int	level;
    char *	name;
};

struct buf_type
{
    BUFFER *    next;
    bool        valid;
    sh_int      state;  /* error state of the buffer */
    sh_int      size;   /* size in k */
    char *      string; /* buffer's string */
};



/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
};



/*
 * Connected state for a channel.
 */
#define CON_PLAYING			 0
#define CON_GET_NAME			 1
#define CON_GET_OLD_PASSWORD		 2
#define CON_CONFIRM_NEW_NAME		 3
#define CON_GET_NEW_PASSWORD		 4
#define CON_CONFIRM_NEW_PASSWORD	 5
#define CON_GET_NEW_RACE		 6
#define CON_GET_NEW_SEX			 7
#define CON_GET_NEW_CLASS		 8
#define CON_GET_ALIGNMENT		 9
#define CON_DEFAULT_CHOICE		10
#define CON_GEN_GROUPS			11
#define CON_PICK_WEAPON			12
#define CON_READ_IMOTD			13
#define CON_READ_MOTD			14
#define CON_BREAK_CONNECT		15
#define CON_ROLL_STATS                  16
#define CON_ACCEPT_STATS                17
#define CON_PICK_HOMETOWN               18
#define CON_GET_ETHOS                   19
#define CON_CREATE_DONE                 20
#define CON_COPYOVER_RECOVER            26
#define CON_UNIVERSAL			27
#define CON_MAINMENU			28
#define CON_NOTEWRITE			29
#define	CON_SETWIZILEVEL		30
#define	CON_IRL				31
/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    bool		valid;
    char *		host;
    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    char		inbuf		[4 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char		inlast		[MAX_INPUT_LENGTH];
    int			repeat;
    char *		outbuf;
    int			outsize;
    int			outtop;
    char *		showstr_head;
    char *		showstr_point;
    void *              pEdit;		/* OLC */
    char **             pString;	/* OLC */
    int			editor;		/* OLC */
};



/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	dex_app_type
{
    sh_int	defensive;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};



/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3
#define TO_ALL		    4



/*
 * Help table types.
 */
struct  help_data
{
    HELP_DATA * next;
    HELP_DATA * next_area;  
    sh_int      level;
//    sh_int      number;
    char *      keyword;
    char *      text;
};

struct help_area_data
{
       HELP_AREA *     next;
       HELP_DATA *     first;
       HELP_DATA *     last;
       AREA_DATA *     area;
       char *          filename;
       bool            changed;
};

/*
 * Shop types.
 */
#define MAX_TRADE	 5

struct	shop_data
{
    SHOP_DATA *	next;			/* Next shop in list		*/
    int	keeper;			/* Vnum of shop keeper mob	*/
    sh_int	buy_type [MAX_TRADE];	/* Item types shop will buy	*/
    sh_int	profit_buy;		/* Cost multiplier for buying	*/
    sh_int	profit_sell;		/* Cost multiplier for selling	*/
    sh_int	open_hour;		/* First opening hour		*/
    sh_int	close_hour;		/* First closing hour		*/
};



/*
 * Per-class stuff.
 */

#define MAX_GUILD 	6
#define STAT_STR 	0
#define STAT_INT	1
#define STAT_WIS	2
#define STAT_DEX	3
#define STAT_CON	4
#define MAX_STATS 	5

struct	class_type
{
    char *	name;			/* the full name of the class */
    char 	who_name	[4];	/* Three-letter name for 'who'	*/
    sh_int	attr_prime;		/* Prime attribute		*/
    sh_int	weapon;			/* First weapon			*/
    int		guild[MAX_GUILD];	/* Vnum of guild rooms		*/
    sh_int	skill_adept;		/* Maximum skill level		*/
    sh_int	thac0_00;		/* Thac0 for level  0		*/
    sh_int	thac0_32;		/* Thac0 for level 32		*/
    sh_int	hp_min;			/* Min hp gained on leveling	*/
    sh_int	hp_max;			/* Max hp gained on leveling	*/
    bool	fMana;			/* Class gains mana on level	*/
    char *	base_group;		/* base skills gained		*/
    char *	default_group;		/* default skills gained	*/
    int         points;                 /* Cost in exp of class         */
    sh_int      stats[MAX_STATS];       /* Stat modifiers */
    char *      align;                  /* Mandatory alignment */
};

struct top_type
{
    char *	name;
    int	        number;
};

struct item_type
{
    int		type;
    char *	name;
};

struct confuse_type
{
    char *	name;
    int		type;
    int		wher;
};
struct weapon_type
{
    char *	name;
    int	vnum;
    sh_int	type;
    sh_int	*gsn;
};

struct wiznet_type
{
    char *	name;
    char *	control;
    long 	flag;
    int		level;
};

struct attack_type
{
    char *	name;			/* name */
    char *	noun;			/* message */
    int   	damage;			/* damage class */
};

struct race_type
{
    char *	name;			/* call name of the race */
    bool	pc_race;		/* can be chosen by pcs */
    long	act;			/* act bits for the race */
    long	aff;			/* aff bits for the race */
    long	aff2;			/* aff2 bits for the race */
    long	detect;			/* detect bits for the race */
    long	off;			/* off bits for the race */
    long	imm;			/* imm bits for the race */
    long        res;			/* res bits for the race */
    long	vuln;			/* vuln bits for the race */
    long	form;			/* default form flag for the race */
    long	parts;			/* default parts for the race */
};


struct pc_race_type  /* additional data for pc races */
{
    char *	name;			/* MUST be in race_type */
    char 	who_name[10];
    int		points;			/* cost in exp of the race */
    sh_int	class_mult[MAX_CLASS];	/* exp multiplier for class, * 100 */
    char *	skills[5];		/* bonus skills for the race */
    sh_int 	stats[MAX_STATS];	/* starting stats */
    sh_int	max_stats[MAX_STATS];	/* maximum stats */
    sh_int	size;			/* aff bits for the race */
    int         hp_bonus;               /* Initial hp bonus */
    int         mana_bonus;             /* Initial mana bonus */
    int         prac_bonus;             /* Initial practice bonus */
    char *      align;                  /* Mandatory alignment */
};


struct spec_type
{
    char * 	name;			/* special function name */
    SPEC_FUN *	function;		/* the function */
};

struct prac_type
{
    char *      sh_name;
    char *      name;
    int         number;
};


/*
 * Data structure for notes.
 */
/* 5 defines removed for old note system */
struct	note_data
{
    NOTE_DATA *	next;
    bool 	valid;
    sh_int	type;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
    time_t  	date_stamp;
    time_t      expire; /* board code added */
};


/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    bool		valid;
    sh_int		where;
    sh_int		type;
    sh_int		level;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
};

/* where definitions */
#define TO_AFFECTS	0
#define TO_OBJECT	1
#define TO_IMMUNE	2
#define TO_RESIST	3
#define TO_VULN		4
#define TO_WEAPON	5
#define TO_AFFECTS2	6
#define TO_DETECTS      7
#define TO_RACE		8
/* where definitions for room */
#define TO_ROOM_AFFECTS 0
#define TO_ROOM_CONST   1
#define TO_ROOM_FLAGS   2

/* room applies */
#define APPLY_ROOM_NONE         0
#define APPLY_ROOM_HEAL         1
#define APPLY_ROOM_MANA         2


/*
 * A kill structure (indexed by level).
 */
struct	kill_data
{
    int		number;
    int		killed;
};



/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD	   	3060
#define MOB_VNUM_VAGABOND	   	3063
#define MOB_VNUM_CAT		   	3066
#define MOB_VNUM_FIDO		   	3062

#define MOB_VNUM_VAMPIRE	    	3404

#define MOB_VNUM_SHADOW               	10
#define MOB_VNUM_SPECIAL_GUARD        	11
#define MOB_VNUM_BEAR                 	12
#define MOB_VNUM_SPIRIT               	232
#define MOB_VNUM_NIGHTWALKER          	14
#define MOB_VNUM_STALKER              	15
#define MOB_VNUM_SQUIRE               	16
#define MOB_VNUM_MIRROR_IMAGE         	17
#define MOB_VNUM_ZOMBIE                	18
#define MOB_VNUM_SHADE                	00
#define MOB_VNUM_EARTH_ELEMENTAL	19
#define MOB_VNUM_FIRE_ELEMENTAL		23
#define MOB_VNUM_WATER_ELEMENTAL	22
#define MOB_VNUM_AIR_ELEMENTAL		21
#define MOB_VNUM_ELOHIM_DOG             28
#define MOB_VNUM_WHITE_TIGER		33
#define MOB_VNUM_WOLF			34
#define MOB_VNUM_BEAR2			35
#define MOB_VNUM_LION			36
/*New kingdom stuff*/
#define MOB_VNUM_WARLOCK		24
#define MOB_VNUM_ARGUS_DRAGON		25
#define MOB_VNUM_TALASTIN_SOLDIER       26
#define MOB_VNUM_ARKADIA_TREEFOLK       27
#define OBJ_VNUM_ARGUS_ORB		246
#define OBJ_VNUM_TALASTIN_SHIELD        246
#define OBJ_VNUM_ARGUS_DAGGER		243
#define OBJ_VNUM_GONDER_RING		2404
#define OBJ_VNUM_GONDER_SWORD		2405
#define OBJ_VNUM_GONDER_ORNAMENT	2406
#define OBJ_VNUM_NARSSIUS_CLAW		25871
#define MOB_VNUM_NARSSIUS_DEMON		25871
#define OBJ_VNUM_BLOOD			6

#define MOB_VNUM_PATROLMAN	   	2106
#define GROUP_VNUM_TROLLS	   	2100
#define GROUP_VNUM_OGRES	   	2101

#define MOB_VNUM_WONDERWOLF             3831
#define MOB_VNUM_WONDERSPIDER           3830
#define MOB_VNUM_WONDERBABOON           3832
#define MOB_VNUM_WONDERCAMEL            3833
#define MOB_VNUM_LICH	     		3834
#define MOB_VNUM_VAMPIRE2     		3835

/* RT ASCII conversions -- used so we can have letters in this file */

#define A		  	1
#define B			2
#define C			4
#define D			8
#define E			16
#define F			32
#define G			64
#define H			128

#define I			256
#define J			512
#define K		        1024
#define L		 	2048
#define M			4096
#define N		 	8192
#define O			16384
#define P			32768

#define Q			65536
#define R			131072
#define S			262144
#define T			524288
#define U			1048576
#define V			2097152
#define W			4194304
#define X			8388608

#define Y			16777216
#define Z			33554432
#define aa			67108864 	/* doubled due to conflicts */
#define bb			134217728
#define cc			268435456
#define dd			536870912
#define ee			1073741824
#define ff			2147483648
//#define gg			4294967296

#define GROUP_NONE              0
#define GROUP_FIRE		1
#define GROUP_COLD		2
#define GROUP_LIGHTNING		3
#define GROUP_PRO_MAGIC		4
#define GROUP_STEALTH		5
#define GROUP_ACID		6
#define GROUP_POISON		7
#define GROUP_WEAPONMASTER      8
#define GROUP_ATTACK            9
#define GROUP_BEGUILING         10
#define GROUP_BENEDICTIONS      11
#define GROUP_CREATION          12
#define GROUP_CURATIVE          13
#define GROUP_DETECTION         14
#define GROUP_ENCHANTMENT       15
#define GROUP_ENHANCEMENT       16
#define GROUP_HEALING           17
#define GROUP_ILLUSION          18
#define GROUP_MALADICTIONS      19
#define GROUP_PROTECTIVE        20
#define GROUP_TRANSPORTATION    21
#define GROUP_NATURE            22
#define GROUP_FIGHTMASTER       23
#define GROUP_SUDDENDEATH       24
#define GROUP_KINGDOM           25
#define GROUP_DEFENSIVE         26
#define GROUP_WIZARD            27
#define GROUP_MIND              28
#define GROUP_NECRO		29

/* adds traps */
#define TRAP_DAM_SLEEP      0
#define TRAP_DAM_TELEPORT   1
#define TRAP_DAM_FIRE       2
#define TRAP_DAM_COLD       3
#define TRAP_DAM_ACID       4
#define TRAP_DAM_ENERGY     5
#define TRAP_DAM_BLUNT      6
#define TRAP_DAM_PIERCE     7
#define TRAP_DAM_SLASH      8

#define TRAP_EFF_MOVE       (A) /* trigger on movement */
#define TRAP_EFF_OBJECT     (B) /* trigger on get or put */
#define TRAP_EFF_ROOM       (C) /* affect all in room */
#define TRAP_EFF_OPEN       (K) /* trigger on open */
/* end */

/*
 * ACT bits for mobs.  *ACT*
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		(A)		/* Auto set for mobs	*/
#define ACT_SENTINEL	    	(B)		/* Stays in one room	*/
#define ACT_SCAVENGER	      	(C)		/* Picks up objects	*/
#define ACT_AGGRESSIVE		(F)    		/* Attacks PC's		*/
#define ACT_STAY_AREA		(G)		/* Won't leave area	*/
#define ACT_WIMPY		(H)
#define ACT_PET			(I)		/* Auto set for pets	*/
#define ACT_TRAIN		(J)		/* Can train PC's	*/
#define ACT_PRACTICE		(K)		/* Can practice PC's	*/
#define ACT_UNDEAD		(O)
#define ACT_CLERIC		(Q)
#define ACT_MAGE		(R)
#define ACT_THIEF		(S)
#define ACT_WARRIOR		(T)
#define ACT_NOALIGN		(U)
#define ACT_NOPURGE		(V)
#define ACT_OUTDOORS		(W)
#define ACT_INDOORS		(Y)
#define ACT_IS_HEALER		(aa)
#define ACT_MOUNTABLE		(bb)
#define ACT_UPDATE_ALWAYS	(cc)
#define ACT_IS_CHANGER		(dd)
#define ACT_NOTRACK		(ee)

/* damage classes */
#define DAM_NONE                0
#define DAM_BASH                1
#define DAM_PIERCE              2
#define DAM_SLASH               3
#define DAM_FIRE                4
#define DAM_COLD                5
#define DAM_LIGHTNING           6
#define DAM_ACID                7
#define DAM_POISON              8
#define DAM_NEGATIVE            9
#define DAM_HOLY                10
#define DAM_ENERGY              11
#define DAM_MENTAL              12
#define DAM_DISEASE             13
#define DAM_DROWNING            14
#define DAM_LIGHT		15
#define DAM_OTHER               16
#define DAM_HARM		17
#define DAM_CHARM		18
#define DAM_SOUND		19
#define DAM_THIRST		20
#define DAM_HUNGER		21
#define DAM_LIGHT_V             22
#define DAM_TRAP_ROOM           23

/* OFF bits for mobiles *OFF  */
#define OFF_AREA_ATTACK         (A)
#define OFF_BACKSTAB            (B)
#define OFF_BASH                (C)
#define OFF_BERSERK             (D)
#define OFF_DISARM              (E)
#define OFF_DODGE               (F)
#define OFF_FADE                (G)
#define OFF_FAST                (H)
#define OFF_KICK                (I)
#define OFF_KICK_DIRT           (J)
#define OFF_PARRY               (K)
#define OFF_RESCUE              (L)
#define OFF_TAIL                (M)
#define OFF_TRIP                (N)
#define OFF_CRUSH		(O)
#define ASSIST_ALL       	(P)
#define ASSIST_ALIGN	        (Q)
#define ASSIST_RACE    	     	(R)
#define ASSIST_PLAYERS      	(S)
#define ASSIST_GUARD        	(T)
#define ASSIST_VNUM		(U)
#define ASSIST_CLAN		(V)
#define OFF_XATTACK		(W)
#define OFF_XATTACK2		(X)
#define OFF_XATTACK3		(Y)
#define OFF_XATTACK4		(Z)
#define OFF_XATTACK5		(aa)
#define OFF_XATTACK6		(bb)

/* return values for check_imm */
#define IS_NORMAL		0
#define IS_IMMUNE		1
#define IS_RESISTANT		2
#define IS_VULNERABLE		3

/* IMM bits for mobs */
#define IMM_SUMMON              (A)
#define IMM_CHARM               (B)
#define IMM_MAGIC               (C)
#define IMM_WEAPON              (D)
#define IMM_BASH                (E)
#define IMM_PIERCE              (F)
#define IMM_SLASH               (G)
#define IMM_FIRE                (H)
#define IMM_COLD                (I)
#define IMM_LIGHTNING           (J)
#define IMM_ACID                (K)
#define IMM_POISON              (L)
#define IMM_NEGATIVE            (M)
#define IMM_HOLY                (N)
#define IMM_ENERGY              (O)
#define IMM_MENTAL              (P)
#define IMM_DISEASE             (Q)
#define IMM_DROWNING            (R)
#define IMM_LIGHT		(S)
#define IMM_SOUND		(T)
#define IMM_WOOD                (X)
#define IMM_SILVER              (Y)
#define IMM_IRON                (Z)

/* RES bits for mobs *RES */
#define RES_SUMMON		(A)
#define RES_CHARM		(B)
#define RES_MAGIC               (C)
#define RES_WEAPON              (D)
#define RES_BASH                (E)
#define RES_PIERCE              (F)
#define RES_SLASH               (G)
#define RES_FIRE                (H)
#define RES_COLD                (I)
#define RES_LIGHTNING           (J)
#define RES_ACID                (K)
#define RES_POISON              (L)
#define RES_NEGATIVE            (M)
#define RES_HOLY                (N)
#define RES_ENERGY              (O)
#define RES_MENTAL              (P)
#define RES_DISEASE             (Q)
#define RES_DROWNING            (R)
#define RES_LIGHT		(S)
#define RES_SOUND		(T)
#define RES_WOOD                (X)
#define RES_SILVER              (Y)
#define RES_IRON                (Z)

/* VULN bits for mobs */
#define VULN_SUMMON		(A)
#define VULN_CHARM		(B)
#define VULN_MAGIC              (C)
#define VULN_WEAPON             (D)
#define VULN_BASH               (E)
#define VULN_PIERCE             (F)
#define VULN_SLASH              (G)
#define VULN_FIRE               (H)
#define VULN_COLD               (I)
#define VULN_LIGHTNING          (J)
#define VULN_ACID               (K)
#define VULN_POISON             (L)
#define VULN_NEGATIVE           (M)
#define VULN_HOLY               (N)
#define VULN_ENERGY             (O)
#define VULN_MENTAL             (P)
#define VULN_DISEASE            (Q)
#define VULN_DROWNING           (R)
#define VULN_LIGHT		(S)
#define VULN_SOUND		(T)
#define VULN_WOOD               (X)
#define VULN_SILVER             (Y)
#define VULN_IRON		(Z)

/* body form */
#define FORM_EDIBLE             (A)
#define FORM_POISON             (B)
#define FORM_MAGICAL            (C)
#define FORM_INSTANT_DECAY      (D)
#define FORM_OTHER              (E)  /* defined by material bit */

/* actual form */
#define FORM_ANIMAL             (G)
#define FORM_SENTIENT           (H)
#define FORM_UNDEAD             (I)
#define FORM_CONSTRUCT          (J)
#define FORM_MIST               (K)
#define FORM_INTANGIBLE         (L)

#define FORM_BIPED              (M)
#define FORM_CENTAUR            (N)
#define FORM_INSECT             (O)
#define FORM_SPIDER             (P)
#define FORM_CRUSTACEAN         (Q)
#define FORM_WORM               (R)
#define FORM_BLOB		(S)

#define FORM_MAMMAL             (V)
#define FORM_BIRD               (W)
#define FORM_REPTILE            (X)
#define FORM_SNAKE              (Y)
#define FORM_DRAGON             (Z)
#define FORM_AMPHIBIAN          (aa)
#define FORM_FISH               (bb)
#define FORM_COLD_BLOOD		(cc)

/* body parts */
#define PART_HEAD               (A)
#define PART_ARMS               (B)
#define PART_LEGS               (C)
#define PART_HEART              (D)
#define PART_BRAINS             (E)
#define PART_GUTS               (F)
#define PART_HANDS              (G)
#define PART_FEET               (H)
#define PART_FINGERS            (I)
#define PART_EAR                (J)
#define PART_EYE		(K)
#define PART_LONG_TONGUE        (L)
#define PART_EYESTALKS          (M)
#define PART_TENTACLES          (N)
#define PART_FINS               (O)
#define PART_WINGS              (P)
#define PART_TAIL               (Q)
/* for combat */
#define PART_CLAWS              (U)
#define PART_FANGS              (V)
#define PART_HORNS              (W)
#define PART_SCALES             (X)
#define PART_TUSKS		(Y)


/*
 * Bits for 'affected_by'.  *AFF*
 * Used in #MOBILES.
 */
#define AFF_BLIND		(A)
#define AFF_INVISIBLE		(B)
#define AFF_REGENERATION        (C)
#define AFF_SCREAM              (E)
#define AFF_BLOODTHIRST 	(F)
#define AFF_STUN                (G)
#define AFF_SANCTUARY		(H)
#define AFF_FAERIE_FIRE		(I)
#define AFF_INFRARED		(J)
#define AFF_CURSE		(K)
#define AFF_CORRUPTION          (L)
#define AFF_POISON		(M)
#define AFF_PROTECT_EVIL	(N)
#define AFF_CAMOUFLAGE		(O)
#define AFF_SNEAK		(P)
#define AFF_HIDE		(Q)
#define AFF_SLEEP		(R)
#define AFF_CHARM		(S)
#define AFF_FLYING		(T)
#define AFF_PASS_DOOR		(U)
#define AFF_HASTE		(V)
#define AFF_CALM		(W)
#define AFF_PLAGUE		(X)
#define AFF_WEAKEN		(Y)
#define AFF_FAERIE_FOG		(Z)
#define AFF_BERSERK		(aa)
#define AFF_DETECT_FADE		(bb)
#define AFF_FADE	        (cc)
#define AFF_SLOW		(dd)
#define AFF_PROTECT_GOOD	(ee)



/*   Lack of bits in a long integer, i start a new long integer here.
 *   will be tested irregularly, by only their own routines.
 */

#define AFF2_HERBS		(A)
#define AFF2_WEBS		(B)
#define AFF2_HOLD		(C)
#define AFF2_INVUL		(D)
#define AFF2_PANIC		(E)
#define AFF2_TOXIC		(F)
#define AFF2_PARALYZE		(G)
#define AFF2_IRONSKIN		(H)
#define AFF2_STONESHAPE		(I)
#define AFF2_PHASING		(J)
#define AFF2_SUBMERGED		(K)
#define AFF2_CONTAGION		(L)
#define AFF2_WITHER		(M)
#define AFF2_CHARISMATIC	(N)
#define AFF2_GHOUL		(O)
#define AFF2_VAMPIRIC		(P)
#define AFF2_PROTECT_UNDEAD	(Q)
#define AFF2_BARRIER		(R)
#define AFF2_SUBMISSION		(S)
#define AFF2_ARMLOCK		(T)
/*
 * *AFF* bits for rooms
 */
#define AFF_ROOM_SHOCKING               (A)
#define AFF_ROOM_L_SHIELD               (B)
#define AFF_ROOM_THIEF_TRAP             (C)
#define AFF_ROOM_RANDOMIZER             (D)
#define AFF_ROOM_NIGHTFALL		(E)
#define AFF_ROOM_REGENERATION           (F)
#define AFF_ROOM_CURSE                  (K)
#define AFF_ROOM_POISON                 (M)
#define AFF_ROOM_SLEEP                  (R)
#define AFF_ROOM_PLAGUE                 (X)
#define AFF_ROOM_SLOW                   (dd)

/*
 * Bits for 'detection'.  *DETECT*
 * Used in chars.
 */
#define DETECT_IMP_INVIS        (A)
#define DETECT_FADE             (B)
#define DETECT_EVIL             (C)
#define DETECT_INVIS            (D)
#define DETECT_MAGIC            (E)
#define DETECT_HIDDEN           (F)
#define DETECT_GOOD             (G)
#define DETECT_SNEAK            (H)
#define ADET_FEAR               (Q)
#define ADET_FORM_TREE          (R)
#define ADET_FORM_GRASS         (S)
#define ADET_WEB                (T)
#define D_SUFFIX		(U)
#define D_PREFIX		(V)
#define D_LOCKHOST		(X)
#define DETECT_LIFE             (Y)
#define DARK_VISION             (Z)
#define DETECT_STEAL		(aa)
#define ACUTE_VISION            (ee)
#define DETECT_UNDEAD           (K)


/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2

/* AC types */
#define AC_PIERCE			0
#define AC_BASH				1
#define AC_SLASH			2
#define AC_EXOTIC			3

/* dice */
#define DICE_NUMBER			0
#define DICE_TYPE			1
#define DICE_BONUS			2

/* size */
#define SIZE_TINY			0
#define SIZE_SMALL			1
#define SIZE_MEDIUM			2
#define SIZE_LARGE			3
#define SIZE_HUGE			4
#define SIZE_GIANT			5



/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_SILVER_ONE	      1
#define OBJ_VNUM_GOLD_ONE	      2
#define OBJ_VNUM_GOLD_SOME	      3
#define OBJ_VNUM_SILVER_SOME	      4
#define OBJ_VNUM_COINS		      5

#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_GUTS		     16
#define OBJ_VNUM_BRAINS		     17

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22
#define OBJ_VNUM_DISC		     23
#define OBJ_VNUM_PORTAL		     25
#define OBJ_VNUM_BROKEN              29
#define OBJ_VNUM_SPEC_WEAPON_S       30
#define OBJ_VNUM_SPEC_WEAPON_M       32
#define OBJ_VNUM_SPEC_WEAPON_D       34
#define OBJ_VNUM_SAANGREAL          146
#define OBJ_VNUM_DIAMOND_RING       146
#define OBJ_VNUM_WEDDING_BAND       146
#define OBJ_VNUM_ROSE		   1001
//#define OBJ_VNUM_STEEL		   3830
#define OBJ_VNUM_TOTEM		   3832

#define OBJ_VNUM_PIT		   3010

#define OBJ_VNUM_SCHOOL_MACE	   3700
#define OBJ_VNUM_SCHOOL_DAGGER	   3701
#define OBJ_VNUM_SCHOOL_SWORD	   3702
#define OBJ_VNUM_SCHOOL_SPEAR	   3717
#define OBJ_VNUM_SCHOOL_STAFF	   3718
#define OBJ_VNUM_SCHOOL_AXE	   3719
#define OBJ_VNUM_SCHOOL_FLAIL	   3720
#define OBJ_VNUM_SCHOOL_WHIP	   3721
#define OBJ_VNUM_SCHOOL_POLEARM    3722
#define OBJ_VNUM_SCHOOL_BOW        3723
#define OBJ_VNUM_SCHOOL_LANCE      3724

#define OBJ_VNUM_SCHOOL_VEST	   3703
#define OBJ_VNUM_SCHOOL_SHIELD	   3704
#define OBJ_VNUM_SCHOOL_BANNER     3716
#define MAX_VNUM 32768   /* Stock max vnums, change if yours is different*/

#define OBJ_VNUM_MAP		   3162
#define OBJ_VNUM_MAP_NT		   3167
#define OBJ_VNUM_MAP_OFCOL	   3162
#define OBJ_VNUM_MAP_SM		   3164
#define OBJ_VNUM_MAP_ARKHAM	   3162
#define OBJ_VNUM_MAP_TV 	   1814

#define OBJ_VNUM_WHISTLE	   2116

#define OBJ_VNUM_EMPTY_VIAL	     42
#define OBJ_VNUM_STEAK               27
#define OBJ_VNUM_RANGER_STAFF        28

#define OBJ_VNUM_DEPUTY_BADGE        203
#define OBJ_VNUM_JUSTICE_BADGE       203
#define OBJ_VNUM_JUSTICE_SHIELD1     204
#define OBJ_VNUM_JUSTICE_SHIELD2     205
#define OBJ_VNUM_JUSTICE_SHIELD3     206
#define OBJ_VNUM_JUSTICE_SHIELD4     207

#define OBJ_VNUM_CHAOS_BLADE        253

#define OBJ_VNUM_PLATE              246
#define OBJ_VNUM_KUJI_WEAPON	    264
#define OBJ_VNUM_DRAGONSWORD        243
#define OBJ_VNUM_DRAGONMACE         245
#define OBJ_VNUM_DRAGONDAGGER       244

#define OBJ_VNUM_HORDE_PONCHO       36

#define OBJ_VNUM_HORDE_THRONE        222   /* bloodvillage.are */
#define OBJ_VNUM_ASPER_ALTAR         230   /* tower.are        */
#define OBJ_VNUM_CRESCENT_ALTAR      251
#define OBJ_VNUM_KUJIKIRI_SKULL      262   /* Sewer.are        */
#define OBJ_VNUM_GEMMA_ALTAR         241  /* knightcastle.are */
#define OBJ_VNUM_SIRIUS_STAND        200   /* justice.are      */
#define OBJ_VNUM_TRISTAN_ALTAR	     272       /* tristan.are	*/
#define OBJ_VNUM_POTION_SILVER        45
#define OBJ_VNUM_POTION_GOLDEN        46
#define OBJ_VNUM_POTION_SWIRLING      47
#define OBJ_VNUM_HAMMER			48


#define OBJ_VNUM_MARK_TIME               2800
#define OBJ_VNUM_MARK_WISDOM             2801
#define OBJ_VNUM_MARK_JUSTICE            2802
#define OBJ_VNUM_MARK_FAITH              2803
#define OBJ_VNUM_MARK_DECEPTION          2804
#define OBJ_VNUM_MARK_HATE               2805
#define OBJ_VNUM_MARK_NATURE             2806
#define OBJ_VNUM_MARK_CREATION           2807
#define OBJ_VNUM_MARK_NEUTRAILITY        2808
#define OBJ_VNUM_MARK_WAR                2809
#define OBJ_VNUM_MARK_HONOR              2810
#define OBJ_VNUM_MARK_UNDERSTANDING      2813
#define OBJ_VNUM_MARK_QUESTS             2814
#define OBJ_VNUM_MARK_LAW                2815
#define OBJ_VNUM_MARK_LEADERSHIP         2816
#define OBJ_VNUM_MARK_HOPE               2817
#define OBJ_VNUM_MARK_KNOWLEDGE		 2818
#define OBJ_VNUM_MARK_ORDER		 2819
#define OBJ_VNUM_MARK_PEACE		 2820
#define OBJ_VNUM_MARK_CHAOS		 2821
#define OBJ_VNUM_MARK_DEATH		 2822
#define OBJ_VNUM_MARK_LIFE		 2823
#define OBJ_VNUM_MARK_MERCY		 2824
#define OBJ_VNUM_MARK_MAGIC		 2825


#define OBJ_VNUM_IRON_BAR		1405
#define MAX_RELIGION 25

/* Define religion #s */
#define RELIGION_NONE           0
#define RELIGION_TAKEDA		1
#define RELIGION_BAYARD         2
#define RELIGION_EXPLORER       3
#define RELIGION_ARIANEIRA      4
#define RELIGION_SILVARA        5
#define RELIGION_THERONE        6
#define RELIGION_CEDRIC         7

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5
#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10
#define ITEM_CLOTHING		     11
#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13
#define ITEM_CONTAINER		     15
#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_INGREDIENT		     21
#define ITEM_BOAT		     22
#define ITEM_CORPSE_NPC		     23
#define ITEM_CORPSE_PC		     24
#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26
#define ITEM_PROTECT		     27
#define ITEM_MAP		     28
#define ITEM_PORTAL		     29
#define ITEM_WARP_STONE		     30
#define ITEM_ROOM_KEY		     31
#define ITEM_GEM		     32
#define ITEM_JEWELRY		     33
#define ITEM_JUKEBOX		     34
#define ITEM_TRAP		     35
#define ITEM_ARROW		     36
#define ITEM_TATTOO		     37

/*
 * Extra flags.  *EXT*
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		(A)
#define ITEM_HUM		(B)
#define ITEM_DARK		(C)
#define ITEM_LOCK		(D)
#define ITEM_EVIL		(E)
#define ITEM_INVIS		(F)
#define ITEM_MAGIC		(G)
#define ITEM_NODROP		(H)
#define ITEM_BLESS		(I)
#define ITEM_ANTI_GOOD		(J)
#define ITEM_ANTI_EVIL		(K)
#define ITEM_ANTI_NEUTRAL	(L)
#define ITEM_NOREMOVE		(M)
#define ITEM_INVENTORY		(N)
#define ITEM_NOPURGE		(O)
#define ITEM_ROT_DEATH		(P)
#define ITEM_VIS_DEATH		(Q)
#define ITEM_NOSAC		(R) 
/* not used, ITEM_NO_SAC wear flag is instead */
#define ITEM_NONMETAL		(S)
#define ITEM_NOLOCATE		(T)
#define ITEM_MELT_DROP		(U)
#define ITEM_HAD_TIMER		(V)
#define ITEM_SELL_EXTRACT	(W)
#define ITEM_BURN_PROOF		(Y)
#define ITEM_NOUNCURSE		(Z)
#define ITEM_DIMENSION		(aa)
/* adds hidden items */
#define ITEM_HIDDEN		(bb)
#define ITEM_INDESTRUCT		(cc)
#define ITEM_NODONATE		(dd)
#define ITEM_MOBGIVE		(ee)

/*
 * Wear flags.   *WEAR*
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		(A)
#define ITEM_WEAR_FINGER	(B)
#define ITEM_WEAR_NECK		(C)
#define ITEM_WEAR_BODY		(D)
#define ITEM_WEAR_HEAD		(E)
#define ITEM_WEAR_LEGS		(F)
#define ITEM_WEAR_FEET		(G)
#define ITEM_WEAR_HANDS		(H)
#define ITEM_WEAR_ARMS		(I)
#define ITEM_WEAR_SHIELD	(J)
#define ITEM_WEAR_ABOUT		(K)
#define ITEM_WEAR_WAIST		(L)
#define ITEM_WEAR_WRIST		(M)
#define ITEM_WIELD		(N)
#define ITEM_HOLD		(O)
#define ITEM_NO_SAC		(P)
#define ITEM_WEAR_FLOAT		(Q)
#define ITEM_WEAR_TATTOO        (R)
/* defines ornaments a flag */
#define ITEM_WEAR_ORNAMENT		(S)

/* weapon class */
#define WEAPON_EXOTIC		0
#define WEAPON_SWORD		1
#define WEAPON_DAGGER		2
#define WEAPON_SPEAR		3
#define WEAPON_MACE		4
#define WEAPON_AXE		5
#define WEAPON_FLAIL		6
#define WEAPON_WHIP		7
#define WEAPON_POLEARM		8
#define WEAPON_BOW		9
#define WEAPON_ARROW            10
#define WEAPON_LANCE            11

/* weapon types */
#define WEAPON_FLAMING		(A)
#define WEAPON_FROST		(B)
#define WEAPON_VAMPIRIC		(C)
#define WEAPON_SHARP		(D)
#define WEAPON_VORPAL		(E)
#define WEAPON_TWO_HANDS	(F)
#define WEAPON_SHOCKING		(G)
#define WEAPON_POISON		(H)
#define WEAPON_MANADRAIN	(I)

/* gate flags */
#define GATE_NORMAL_EXIT	(A)
#define GATE_NOCURSE		(B)
#define GATE_GOWITH		(C)
#define GATE_BUGGY		(D)
#define GATE_RANDOM		(E)

/* furniture flags */
#define STAND_AT		(A)
#define STAND_ON		(B)
#define STAND_IN		(C)
#define SIT_AT			(D)
#define SIT_ON			(E)
#define SIT_IN			(F)
#define REST_AT			(G)
#define REST_ON			(H)
#define REST_IN			(I)
#define SLEEP_AT		(J)
#define SLEEP_ON		(K)
#define SLEEP_IN		(L)
#define PUT_AT			(M)
#define PUT_ON			(N)
#define PUT_IN			(O)
#define PUT_INSIDE		(P)

/*ingredience flags */
#define ING_DRAGONBLOOD		(A)
#define ING_MANDRAKE		(B)
#define ING_IVORY		(C)
#define ING_GUANO		(D)
#define ING_LIZARDEYE		(E)
#define ING_WAX			(F)
#define ING_BONE		(G)
#define ING_SALT		(H)
#define ING_FIREFLY		(I)
#define ING_POISON_IVY		(J)
#define ING_ALOE		(K)
#define ING_ANEMONE		(L)
#define ING_BARBERRY		(M)
#define ING_CORIANDER		(N)
#define ING_DRAGONEYE		(O)
#define ING_GARLIC		(P)
#define ING_GRAPELEAF		(Q)
#define ING_LAVENDER		(R)
#define ING_HEMLOCK		(S)
#define ING_HYACINTH		(T)
#define ING_ANDROMEDA		(U)


/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STR		      1
#define APPLY_DEX		      2
#define APPLY_INT		      3
#define APPLY_WIS		      4
#define APPLY_CON		      5
#define APPLY_SEX		      6
#define APPLY_CLASS		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_AC		     17
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVES		     20
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_SPELL_AFFECT	     25

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8
#define CONT_PUT_ON		     16



/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_CHAT		   1200
#define ROOM_VNUM_TEMPLE	   3001
#define ROOM_VNUM_ALTAR		   3054
#define ROOM_VNUM_PIT		   3369
#define ROOM_VNUM_SCHOOL	   3700
#define ROOM_VNUM_BALANCE	   4500
#define ROOM_VNUM_CIRCLE	   4400
#define ROOM_VNUM_DEMISE	   4201
#define ROOM_VNUM_HONOR		   4300
#define ROOM_VNUM_SHADOWPLANE     10000
#define MIN_ARENA_ROOM		   11305
#define MAX_ARENA_ROOM		   11353
#define ARENA_WAR_ROOM		   11300
#define ROOM_VNUM_ARENA_SAFE	   11354
#define ROOM_VNUM_AFTERLIFE	   3854

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		(A)
#define ROOM_NO_MOB		(C)
#define ROOM_INDOORS		(D)

#define ROOM_PRIVATE		(J)
#define ROOM_SAFE		(K)
#define ROOM_SOLITARY		(L)
#define ROOM_PET_SHOP		(M)
#define ROOM_NO_RECALL		(N)
#define ROOM_IMP_ONLY		(O)
#define ROOM_GODS_ONLY		(P)
#define ROOM_HEROES_ONLY	(Q)
#define ROOM_NEWBIES_ONLY	(R)
#define ROOM_LAW		(S)
#define ROOM_NOWHERE		(T)
#define ROOM_BANK               (U)

#define ROOM_NO_MAGIC           (W)
#define ROOM_NOSUMMON           (X)

#define ROOM_REGISTRY           (bb)
#define ROOM_BOUNTY		(cc)
#define ROOM_ARENA		(dd)
#define ROOM_WILDERNESS		(ee)

/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      (A)
#define EX_CLOSED		      (B)
#define EX_LOCKED		      (C)
#define EX_PICKPROOF		      (F)
#define EX_NOPASS		      (G)
#define EX_EASY			      (H)
#define EX_HARD			      (I)
#define EX_INFURIATING		      (J)
#define EX_NOCLOSE		      (K)
#define EX_NOLOCK		      (L)
#define EX_HIDDEN		      (M)
#define EX_HIDDEN_NOFIND              (N)


/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      0
#define SECT_CITY		      1
#define SECT_FIELD		      2
#define SECT_FOREST		      3
#define SECT_HILLS		      4
#define SECT_MOUNTAIN		      5
#define SECT_WATER_SWIM		      6
#define SECT_WATER_NOSWIM	      7
#define SECT_UNUSED		      8
#define SECT_AIR		      9
#define SECT_DESERT		     10
#define SECT_ROAD		     11
#define SECT_ENTER		     12
#define SECT_ROCK_MOUNTAIN	     13
#define SECT_SNOW_MOUNTAIN	     14
#define SECT_SWAMP		     15
#define SECT_JUNGLE		     16
#define SECT_RUINS		     17
#define SECT_CAVERN		     18
#define SECT_MAX		     19


/*
 * Equpiment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE		     -1
#define WEAR_LIGHT		      0
#define WEAR_FLOAT		      1
#define WEAR_ORNAMENT_1		      2
#define WEAR_ORNAMENT_2		      3
#define WEAR_FINGER_L		      4
#define WEAR_FINGER_R		      5
#define WEAR_NECK_1		      6
#define WEAR_NECK_2		      7
#define WEAR_BODY		      8
#define WEAR_HEAD		      9
#define WEAR_LEGS		     10
#define WEAR_FEET		     11
#define WEAR_HANDS		     12
#define WEAR_ARMS		     13
#define WEAR_SHIELD		     14
#define WEAR_ABOUT		     15
#define WEAR_WAIST		     16
#define WEAR_WRIST_L		     17
#define WEAR_WRIST_R		     18
#define WEAR_HOLD		     19
#define WEAR_WIELD		     20
#define WEAR_SECONDARY		     21
/* defines a wear loc for tattoos */
#define WEAR_TATTOO		     22
#define WEAR_STUCK_IN                23
#define MAX_WEAR		     22



/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2
#define COND_HUNGER		      3
#define COND_BLOODLUST                4
#define COND_TOXIC	              5

/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_RESTING		      5
#define POS_SITTING		      6
#define POS_FIGHTING		      7
#define POS_STANDING		      8



/*
 * ACT bits for players.
 */
#define PLR_IS_NPC		(A)		/* Don't EVER set.	*/
#define PLR_BOUGHT_PET		(B)

/* RT auto flags */
#define PLR_AUTOASSIST		(C)
#define PLR_AUTOEXIT		(D)
#define PLR_AUTOLOOT		(E)
#define PLR_AUTOSAC             (F)
#define PLR_AUTOGOLD		(G)
#define PLR_AUTOSPLIT		(H)
/* took out plr_color define and moved it down, used to be (I) */
#define PLR_WANTED		(J)

/* RT personal flags */
#define PLR_HOLYLIGHT		(N)
#define PLR_CANLOOT		(P)
#define PLR_NOSUMMON		(Q)
#define PLR_NOFOLLOW		(R)
#define PLR_UNUSED		(S) /* used to be CANINDUCT */
#define PLR_GHOST		(T)
#define PLR_QUESTING		(V)

/* penalty flags */
#define PLR_PERMIT		(U)
#define PLR_LOG			(W)
#define PLR_DENY		(X)
#define PLR_FREEZE		(Y)
#define PLR_THIEF		(Z)
#define PLR_KILLER		(aa)
/* quest code added, next 2 lines */
#define PLR_QUESTOR     	(bb)
#define IS_QUESTOR(ch)     	(IS_SET((ch)->act, PLR_QUESTOR))
/* end */
#define PLR_COLOR		(cc)		/* for lope */
#define PLR_AUTODAMAGE		(dd)
#define PLR_ARENA		(ee)

/* RT comm flags -- may be used on both mobs and chars */
#define COMM_QUIET              (A)
#define COMM_DEAF            	(B)
#define COMM_NOWIZ              (C)
#define COMM_NOAUCTION          (D)
#define COMM_NOGOSSIP           (E)
#define COMM_NOQUESTION         (F)
#define COMM_NOMUSIC            (G)
#define COMM_NOQUOTE		(I)
#define COMM_BLOCKTITLE		(H)
#define COMM_SHOUTSOFF		(J)

/* display flags */
#define COMM_TRUE_TRUST		(K)
#define COMM_COMPACT		(L)
#define COMM_BRIEF		(M)
#define COMM_PROMPT		(N)
#define COMM_COMBINE		(O)
#define COMM_TELNET_GA		(P)
#define COMM_SHOW_AFFECTS	(Q)
#define COMM_NOKINGDOM		(R)

/* penalties */
#define COMM_NOEMOTE		(T)
#define COMM_NOSHOUT		(U)
#define COMM_NOTELL		(V)
#define COMM_NOCHANNELS		(W)
#define COMM_SNOOP_PROOF	(Y)
#define COMM_AFK		(Z)
#define COMM_OLC		(aa)
#define COMM_NONOTE		(bb)
#define COMM_NOQUEST		(cc)
#define COMM_NONEWBIE		(dd)
#define COMM_BUSY		(ee)

/* WIZnet flags */
#define WIZ_ON			(A)
#define WIZ_TICKS		(B)
#define WIZ_LOGINS		(C)
#define WIZ_SITES		(D)
#define WIZ_LINKS		(E)
#define WIZ_DEATHS		(F)
#define WIZ_RESETS		(G)
#define WIZ_MOBDEATHS		(H)
#define WIZ_FLAGS		(I)
#define WIZ_PENALTIES		(J)
#define WIZ_SACCING		(K)
#define WIZ_LEVELS		(L)
#define WIZ_SECURE		(M)
#define WIZ_SWITCHES		(N)
#define WIZ_SNOOPS		(O)
#define WIZ_RESTORE		(P)
#define WIZ_LOAD		(Q)
#define WIZ_NEWBIE		(R)
#define WIZ_PREFIX		(S)
#define WIZ_SPAM		(T)
#define WIZ_QUESTS		(U)
#define WIZ_PASSWORD		(V)
#define WIZ_DELETE		(W)
#define WIZ_CHEAT		(Y) 
#define WIZ_RP			(Z)
#define WIZ_PROG		(aa)

#define TAG_PLAYING             (A)
#define TAG_FROZEN              (B)
#define TAG_RED                 (C)
#define TAG_BLUE                (D)

#define INFO_ON                 (A)
#define INFO_LEVELS             (B)
#define INFO_CONSENT            (C)
#define INFO_ARENA		(D)
#define INFO_FTAG		(E)
#define INFO_ICOOC		(F)

/* Race Flags for Item */
/* added by Takeda 2/26/00 */
#define RACE_HUMAN		(A)
#define RACE_ELF		(B)
#define RACE_DROW		(C)
#define RACE_DWARF		(D)
#define RACE_DUERGAR		(E)
#define RACE_PIXIE		(F)
#define RACE_GARGOYLE		(G)
#define RACE_DRACONIAN		(H)
#define RACE_ARIAL		(I)
#define RACE_FELAR		(J)
#define RACE_MERMAN		(K)
#define RACE_GIANT		(L)
#define RACE_TROLL		(M)
#define RACE_OGRE		(N)
#define RACE_ORC		(O)
#define RACE_DARNOK		(P)
#define RACE_ILLITHID		(Q)

/* Class Flags for Item */
/* added by Takeda 2/26/00 */
#define CLASS_WARRIOR		(A)
#define CLASS_MERCENARY		(B)
#define CLASS_PALADIN		(C)
#define CLASS_BLACK_KNIGHT	(D)
#define CLASS_THIEF		(E)
#define CLASS_INQUISITOR	(F)
#define CLASS_SPELLFILCHER	(G)
#define CLASS_CLERIC		(H)
#define CLASS_PRODIGY		(I)
#define CLASS_MAGE		(J)
#define CLASS_RANGER		(K)
#define CLASS_ASSASSIN		(L)
#define CLASS_BARD		(M)
#define CLASS_MALIGNER		(N)
#define CLASS_ALCHEMIST		(O)
#define CLASS_DRUID		(P)
#define CLASS_MONK		(Q)
#define CLASS_NECROMANCER	(R)

struct  quest_data
{
    CHAR_DATA * quest_god;
    char      * name;
    char      * short_name;
    char      * long_name;
    bool        protection;
    bool        closed;
    sh_int      min_level;
    sh_int      max_level;
};

/* 
 * auction data 
 */

struct  auction_data
{
    OBJ_DATA  * item;   /* a pointer to the item */
    CHAR_DATA * seller; /* a pointer to the seller - which may NOT quit */
    CHAR_DATA * buyer;  /* a pointer to the buyer - which may NOT quit */
    int         bet;    /* last bet - or 0 if noone has bet anything */
    sh_int      going;  /* 1,2, sold */
    sh_int      pulse;  /* how many pulses (.25 sec) until another call-out ? */
};

struct info_type
{
    char *      name;
    long        flag;
    int         level;
};

#define  TITLE_IMM    2
#define  TITLE_PRE    1
#define  TITLE_NORMAL 0

/*auth groups*/
#define  GROUP_MORTAL 1
#define  GROUP_IMMORT 2
#define  GROUP_SPYING 3
#define  GROUP_ASS    4
#define  GROUP_HOLC   5
#define  GROUP_ADM    6
#define  GROUP_IMP    7
#define  GROUP_SADM   8
#define  GROUP_ACTION 9
#define  GROUP_WTF    10
#define  GROUP_SOLC   11
#define  GROUP_BOLC   11
#define  GROUP_ADMIN  12
#define  GROUP_TBH    13
#define  GROUP_TOLC   11

/* Auth defines */
#define auth_mortal 	0
#define auth_protect	1
#define auth_reboot	2
#define auth_log	3
#define auth_set	4
#define auth_smite	5
#define auth_limited	6
#define auth_guild	7
#define auth_popularity	8
#define auth_objdump	9
#define auth_shutdown	10
#define auth_fvlist	11
#define auth_wizlock	12
#define auth_raffect	13
#define auth_raffects	14
#define auth_force	15
#define auth_punish	16
#define auth_load	17
#define auth_newlock	18
#define auth_nochannels	19
#define auth_dontuse	20
//#define auth_nonote	21
//#define auth_noshout	22
//#define auth_notell	23
//#define auth_noquit	24
//#define auth_pecho	25
//#define auth_pardon	26
#define auth_purge	27
#define auth_restore	28
#define auth_slay	29
#define auth_avator	30
#define auth_teleport	31
#define auth_transfer	32
#define auth_poofin	33
#define auth_poofout	34
//#define auth_gecho	35
//#define auth_gecho_font	36
#define auth_holylight	37
#define auth_incognito	38
#define auth_memory	39
#define auth_mwhere	40
#define auth_owhere	41
#define auth_peace	42
#define auth_echo	43
#define auth_return	44
#define auth_snoop	45
#define auth_stat	46
#define auth_string	47
#define auth_switch	48
#define auth_wizinvis	49
//#define auth_wrlist	50
#define auth_vnum	51
//#define auth_zecho	52
#define auth_clone	53
#define auth_wiznet	54
#define auth_omni	55
#define auth_admint	56
#define auth_immtitle	57
#define auth_imotd	58
#define auth_immtalk	59
#define auth_smote	60
#define auth_prefix	61
#define auth_bagsteal	62
#define auth_kingdom	63
#define auth_edit	64
#define auth_marry	65
#define auth_divorce	66
#define auth_asave	67
#define auth_alist	68
#define auth_resets	69
#define auth_goto_arena	70
#define auth_redit	71
#define auth_medit	72
#define auth_aedit	73
#define auth_oedit	74
#define auth_bagpeek	75
#define auth_hedit	76
#define auth_sedit	77
#define auth_vlist 	78
#define auth_religion_follower 	79
#define auth_smallmap	82
#define auth_wset	83
#define auth_mlimit	84
#define auth_grant	85
#define auth_sinfo	86
#define auth_disable	87
#define auth_seize	88
#define auth_copyover	89
//#define auth_desocket	90
#define auth_mlevel	91
#define auth_olevel	92
#define auth_traplist	93
#define auth_uvlist	94
#define auth_save_clans	95
#define auth_kingdom_scan	98	
#define auth_anon_shout	99
#define auth_printmap	100
#define auth_pload	101
#define auth_ofb	102
#define auth_bonus	103
//#define auth_check	104
#define auth_exlist	105
//#define auth_pkban	106
#define auth_search	107
#define auth_map	108
#define auth_kgrant	109
#define auth_job	110
#define auth_arena	111
#define auth_at		112
#define auth_cast	113
#define auth_commune	114
#define auth_sing	115
#define auth_auction	116
#define auth_buy	117
#define auth_hometown	118
#define auth_bounty	119
#define auth_channels	120
#define auth_color	121
#define auth_mode_icooc	123
#define auth_exits	124
#define auth_get	125
#define auth_crestored	126
#define auth_group	127
#define auth_hit	128
#define auth_inventory	129
#define auth_kill	130
#define auth_look	131
#define auth_msp	132
#define auth_order	133
#define auth_practice	134
#define auth_rest	135
#define auth_sit	136
#define auth_sockets	137
#define auth_stand	138
#define auth_startarena	139
#define auth_tell	140
#define auth_unlock	141
#define auth_vt		142
#define auth_wield	143
#define auth_second	144
#define auth_aim	145
#define auth_wizhelp	146
#define auth_arenaout	147
#define auth_move	148
#define auth_goto	149
#define auth_socketsq	150
#define auth_affects	152
#define auth_affections	153
#define auth_areas	154
#define auth_balance	155
#define auth_bug	156
#define auth_commands	157
#define auth_compare	158
#define auth_consider	159
#define auth_count	160
#define auth_credits	161
#define auth_deposit	162
#define auth_equipment	163
#define auth_examine	164
#define auth_help	165
#define auth_background	166
#define auth_motd	167
#define auth_read	168
#define auth_report	169
#define auth_rules	170
#define auth_score	171
#define auth_scan	172
#define auth_skills	173
#define auth_socials	174
#define auth_show	175
#define auth_spells	176
#define auth_story	177
#define auth_time	178
#define auth_typo	179
#define auth_weather	180
#define auth_who	181
#define auth_whoami	182
#define auth_whois	183
#define auth_withdraw	184
#define auth_wizlist	185
#define auth_worth	186
#define auth_slist	187
//#define auth_glist	188
#define auth_repair	189
#define auth_estimate	190
#define auth_finger	191
#define auth_tbh	192
//#define auth_mk		193
#define auth_alias	194
#define auth_mark	195
#define auth_clear	196
#define auth_autolist	197
#define auth_autoassist	198
#define auth_autoexit	199
#define auth_autogold	200
#define auth_autoloot	201
#define auth_autosac	202
#define auth_autosplit	203
#define auth_autodamage	204
#define auth_brief	205
#define auth_combine	206
#define auth_compact	207
#define auth_description	208
#define auth_delete	209
#define auth_doas	210
#define auth_nofollow	211
#define auth_noloot	212
#define auth_nosummon	213
#define auth_outfit	214
#define auth_tick	215
#define auth_password	216
#define auth_prompt	217
#define auth_scroll	218
#define auth_title	219
#define auth_pretitle	220
#define auth_unalias	221
#define auth_wimpy	222
#define auth_sitelock	223
#define auth_afk	224
#define auth_deaf	225
#define auth_emote	226
#define auth_pmote	227
#define auth_gtell	228
#define auth_board	229
#define auth_music	230
#define auth_info	231
#define auth_newbie	232
#define auth_note	233
#define auth_pose	234
#define auth_ooc	235
#define auth_gossip	236
#define auth_quest	237
#define auth_questtalk	238
#define auth_questinfo	239
#define auth_quiet	240
#define auth_reply	241
#define auth_replay	242
#define auth_say	243
#define auth_kedit	244
#define auth_shout	245
#define auth_warcry	246
#define auth_dark_vision	247
#define auth_detect_hidden	248
#define auth_bear_call	249
#define auth_yell	250
#define auth_narrate	251
#define auth_gd		252
#define auth_dance	253
#define auth_pray	254
#define auth_brandish	255
#define auth_butcher	256
#define auth_close	257
#define auth_drink	258
#define auth_drop	259
#define auth_eat	260
#define auth_envenom	261
#define auth_fill	262
#define auth_give	263
#define auth_heal	264
#define auth_hold	265
#define auth_list	266
#define auth_lock	267
#define auth_lockon	268
#define auth_locktitle	269
#define auth_lore	270
#define auth_open	271
#define auth_pick	272
#define auth_relock	273
#define auth_pour	274
#define auth_put	275
#define auth_quaff	276
#define auth_recite	277
#define auth_remove	278
#define auth_request	279
#define auth_sell	280
#define auth_take	281
#define auth_sacrifice	282
#define auth_junk	283
#define auth_value	284
#define auth_wear	285
#define auth_zap	286
#define auth_acute	287
#define auth_barkskin	288
#define auth_sharpen	289
#define auth_vanish	290
#define auth_blindness	291
#define auth_poison	292
#define auth_ambush	293
#define auth_assassinate	294
#define auth_immtitle	295
#define auth_bash	296
#define auth_shieldbash	297
#define auth_backstab	298
#define auth_berserk	299
#define auth_bloodthirst	300
#define auth_blackjack	301
#define auth_caltraps	302
#define auth_camouflage	303
#define auth_charge	304
#define auth_circle	305
#define auth_cleave	306
#define auth_dirt	307
#define auth_disarm	308
#define auth_flee	309
#define auth_guard	310
#define auth_kick	311
#define auth_multiburst	312
#define auth_murder	313
#define auth_jab	314
#define auth_nerve	315
#define auth_rescue	316
#define auth_knightsvirtue	317
#define auth_truesight	318
#define auth_spellbane	319
#define auth_strangle	320
#define auth_surrender	321
#define auth_tame	322
#define auth_throw	323
#define auth_trip	324
#define auth_mount	325
#define auth_dismount	326
#define auth_whirlwind	327
#define auth_mob	328
#define auth_endure	329
#define auth_enter	330
#define auth_follow	331
#define auth_gain	332
#define auth_go		333
#define auth_herbs	334
#define auth_hunt	335
#define auth_hide	336
#define auth_quit	337
#define auth_recall	338
#define auth_rent	339
#define auth_save	340
#define auth_sleep	341
//#define auth_slook	342
//#define auth_lookup	343
#define auth_sneak	344
#define auth_split	345
#define auth_steal	346
#define auth_train	347
#define auth_visible	348
#define auth_wake	349
#define auth_wanted	350
#define auth_where	351
#define auth_donate	352
#define auth_push	353
#define auth_shoot	353
#define auth_smithing	354
#define auth_drag	355
#define auth_camp	356
#define auth_layhands	357
#define auth_vnum_short	358
#define auth_ban	359
#define auth_bandage	360
#define auth_ftag	361
#define auth_tag	362
#define auth_red	363
#define auth_blue	364
#define auth_rename	365
#define auth_advance	366
#define auth_dump	367
#define auth_trust	368
#define auth_violate	369
#define auth_track	370
#define auth_builder	371
#define auth_cpassword	372
#define auth_balefire	373
#define auth_top	374
#define auth_deny	375
#define auth_disconnect	378
#define auth_flag	379
#define auth_freeze	380
#define auth_illegal	381
#define auth_permban	382
#define auth_scatter    383
#define auth_disengage  384
#define auth_distract   385
#define auth_crush      386
#define auth_stoneform	387
#define auth_swoop      388
#define auth_breath	389
#define auth_phase	390
//#define auth_replicate	390
#define auth_dive	391
#define auth_emerge	392
#define auth_update	393
#define auth_totem	394
#define auth_paralysis	395
#define auth_feign_death	396
#define auth_arise	397
#define auth_gouge	398
#define auth_submission	399
#define auth_mounted_fighting 400

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    SPEC_FUN *		spec_fun;
    GAME_FUN * 		game_fun;
    MPROG_DATA *        moprogs;
    int                 progtypes;
    SHOP_DATA *		pShop;
    MPROG_LIST *        mprogs;    
    AREA_DATA *		area;		/* OLC */
    int		vnum;
    int		group;
    bool		new_format;
    int		count;
    int		killed;
    char *		player_name;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    long		act;
    long		affected_by;
    long		affected_by2;
    long                detection;
    sh_int		alignment;
    sh_int		level;
    sh_int		hitroll;
    int			hit[3];
    int		mana[3];
    int		damage[3];
    int		ac[4];
    sh_int 		dam_type;
    long		off_flags;
    long		imm_flags;
    long		res_flags;
    long		vuln_flags;
    sh_int		start_pos;
    sh_int		default_pos;
    sh_int		sex;
    sh_int		race;
    long		wealth;
    long		form;
    long		parts;
    sh_int		size;
    char *		material;
    sh_int                practicer;
    long               mprog_flags;    
};

/* memory settings */
#define MEM_CUSTOMER	A
#define MEM_SELLER	B
#define MEM_HOSTILE	C
#define MEM_AFRAID	D

/* memory for mobs */
struct mem_data
{
    MEM_DATA 	*next;
    bool	valid;
    int		id;
    int 	reaction;
    time_t 	when;
};

struct mprog_data
{
  MPROG_FUN_BRIBE * bribe_prog;
  MPROG_FUN_ENTRY * entry_prog;
  MPROG_FUN_GIVE *  give_prog;
  MPROG_FUN_GREET * greet_prog;
  MPROG_FUN_FIGHT * fight_prog;
  MPROG_FUN_DEATH * death_prog;
  MPROG_FUN_AREA * area_prog;
  MPROG_FUN_SPEECH * speech_prog;

};

struct iprog_data
{
  IPROG_FUN_WEAR *wear_prog;
  IPROG_FUN_REMOVE *remove_prog;
  IPROG_FUN_GET *get_prog;
  IPROG_FUN_DROP *drop_prog;
  IPROG_FUN_SAC *sac_prog;
  IPROG_FUN_ENTRY *entry_prog;
  IPROG_FUN_GIVE *give_prog;
  IPROG_FUN_GREET *greet_prog;
  IPROG_FUN_FIGHT *fight_prog;
  IPROG_FUN_DEATH *death_prog;
  IPROG_FUN_SPEECH *speech_prog;
  IPROG_FUN_AREA *area_prog;
};


/*
 * One character (PC or NPC). *CHAR_DATA*
 */
struct	char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *		reply;
    CHAR_DATA *		last_fought;
    CHAR_DATA *		assassin;    
    time_t              last_fight_time;
    time_t              last_death_time;
    time_t		time_last_level;
    time_t              laston;
    CHAR_DATA *		pet;
    CHAR_DATA *         mprog_target;   
    CHAR_DATA *		doppel;
    CHAR_DATA *		guarding;
    CHAR_DATA *		guarded_by;
    MEM_DATA *		memory;
    SPEC_FUN *		spec_fun;
    GAME_FUN *		game_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	affected;

    OBJ_DATA *		carrying;
    OBJ_DATA *		on;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    ROOM_INDEX_DATA *	load_room;
    AREA_DATA *		zone;
    PC_DATA *		pcdata;
    GEN_DATA *		gen_data;
    bool		valid;
    char *		name;
    long		id;
    sh_int		version;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		prompt;
    char *		prefix;
    char *		immtitle;
    sh_int		group;
    sh_int		sex;
    sh_int		class;
    sh_int		race;
    sh_int		kingdom;
    sh_int		kingdom_rank;
    sh_int		hometown;
    sh_int		ethos;
    sh_int		level;
    sh_int		legendlevel;
    sh_int		trust;
    int			played;
    int			lines;  /* for the pager */
    time_t		logon;
    sh_int		timer;
    sh_int		lagstand;
    sh_int		lagflee;
    sh_int		lagspell;
    sh_int		lagattack;
    sh_int		wait;
    sh_int		daze;
    int		hit;
    int		max_hit;
    int		mana;
    int		max_mana;
    int		move;
    int		max_move;
    long		gold;
    long		silver;
    int			exp;
    long		act;
    long		comm;   /* RT added to pad the vector */
    long		wiznet; /* wiz stuff */
    long		imm_flags;
    long		res_flags;
    long		vuln_flags;
    sh_int		invis_level;
    sh_int		incog_level;
    long		affected_by;
    long		affected_by2;
    long                detection;
    sh_int		position;
    sh_int		practice;
    sh_int		train;
    sh_int		carry_weight;
    sh_int		carry_number;
    sh_int		saving_throw;
    sh_int		alignment;
    sh_int		hitroll;
    sh_int		damroll;
    sh_int		armor[4];
    sh_int		wimpy;
    /* stats */
    sh_int		perm_stat[MAX_STATS];
    sh_int		mod_stat[MAX_STATS];
    /* parts stuff */
    long		form;
    long		parts;
    sh_int		size;
    char*		material;
    /* mobile stuff */
    long		off_flags;
    sh_int		damage[3];
    sh_int		dam_type;
    sh_int		start_pos;
    sh_int		default_pos;
    sh_int             mprog_delay;        
    int			status;		/* A variable for mobprogs */
    int                 progtypes;
    bool 		extracted;
    CHAR_DATA	*	char_in_mind;  /* for mprogs to remember a hero */
    int			quest;	       /* which quests are well done */
    int                 religion;
    int                 rank_religion;
    int			endur;
    int			qflag;
    int			pking;
    int			vt;
    int			msp;
    CHAR_DATA * hunting;        /* hunt data */
    bool                riding; /* mount data, not implemented yet */
    CHAR_DATA * mount;
    CHAR_DATA *         questgiver; /* Vassago */
    sh_int              nextquest; /* Vassago */
    sh_int              countdown; /* Vassago */
    sh_int              questobj; /* Vassago */
    sh_int              questmob; /* Vassago */
    int			questpoints;
    int			quest_curr;
    int			quest_accum;
    bool		notewrite;
    long		lastpk;
    long		lastkingdom;
    char *		lastname;
    char *		sitelock;
    long                tag_flags; 
    long		info;
    long		kincom;	      
    int			pkban;
    int			tbh;
    int			charmie;
    int			rptoken;
    int			trptoken;
    int			nochanned;
    int			icmode;
    int                 anchor;
    int			crime;
    int			wantedtime;
    int			ironskincount;
};



/*
 * Data which only PC's have.
 */
struct	pc_data
{
    PC_DATA *		next;
    QUEST_DATA *        quest;
    QUEST_DATA *        quest_apply;
    BUFFER * 		buffer;
    bool		valid;
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		title;
    char *		pretitle;
    int			perm_hit;
    int			perm_mana;
    int			perm_move;
    int			countdown;
    sh_int		true_sex;
    int			last_level;
    sh_int		condition	[5];
    sh_int		learned		[MAX_SKILL];
    bool		group_known	[MAX_GROUP];
    sh_int		points;
    bool              	confirm_delete;
    char *		alias[MAX_ALIAS];
    char * 		alias_sub[MAX_ALIAS];
    char *		host;
    char *		lasthost;
    long                authorized[3];  
    long		bank_s;
    long		bank_g;
    BOARD_DATA *			board; /* The current board */
    time_t				last_note[MAX_BOARD]; /* last note for the boards */
 	NOTE_DATA *			in_progress;
     int 		security;	/* OLC */ /* Builder security */
    char *		sign;
    int			death;
    int			room_number;
    int			killedbyplayers;
    int			killedmobs;
    int 		pkills;
    int			bounty;
    int			owed;
    int			total_death;
    char *		background;
    char *		email;
    char *		real_name;
    AFFECT_DATA *	arenaaffected;
    long		arenaaffected_by;
    long		arenaaffected_by2;
    int 		arenahp;
    int			arenamp;
    int 		arenamove;
    char *		spouse;
    int			rptoken;
    sh_int		gainswap;
    long                new_authorized[20];
    sh_int        	score[32];
    };

/* Data for generating characters -- only used during generation */
struct gen_data
{
    GEN_DATA	*next;
    bool	valid;
    bool	skill_chosen[MAX_SKILL];
    bool	group_chosen[MAX_GROUP];
    int		points_chosen;
};



/*
 * Liquids.
 */
#define LIQ_WATER        0

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[5];
};



/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    bool valid;
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};


/*
 * Prototype for an object.  *OID*
 */
struct	obj_index_data
{
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    AREA_DATA *		area;		/* OLC */
    bool		new_format;
    char *		name;
    char *		short_descr;
    char *		description;
    int		vnum;
    int		reset_num;
    char *		material;
    sh_int		item_type;
    long		class_flags;
    long		race_flags;
    int			extra_flags;
    int			wear_flags;
    sh_int		level;
    sh_int 		condition;  
    sh_int 		repop; 
    sh_int		count; 
    sh_int		weight;
//    sh_int		limlevel;
    int			cost;
    int			value[5];
    int                 progtypes;
    int                 limit;
    IPROG_DATA	       *iprogs;

};



/*
 * One object.  *OD*
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    OBJ_DATA *		on;
    CHAR_DATA *		carried_by;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    bool		valid;
    bool		enchanted;
    char *	        owner;
    char *		name;
    char *		short_descr;
    char *		description;
    sh_int		item_type;
    long		class_flags;
    long		race_flags;
    int			extra_flags;
    int			wear_flags;
    sh_int		wear_loc;
    sh_int		weight;
    int			cost;
//    sh_int		limlevel;
    sh_int		level;
    sh_int 		condition;  
    char *		material;
    sh_int		timer;
    int			value	[5];
    int			progtypes;
    char 		*from;
    int			altar;
    int			pit;
    bool		extracted;
    int			lootamount;
    int			butcher;
};



/*
 * Exit data.
 */
struct	exit_data
{
    union
    {
	ROOM_INDEX_DATA *	to_room;
	int			vnum;
    } u1;
    sh_int		exit_info;
    int			key;
    char *		keyword;
    char *		description;
    EXIT_DATA *		next;		/* OLC */
    int			rs_flags;	/* OLC */
    int			orig_door;	/* OLC */
};



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    int		arg1;
    int		arg2;
    int		arg3;
    int		arg4;
};



/*
 * Area definition.
 */
struct	area_data
{
    AREA_DATA *		next;
    HELP_AREA *                helps;      
    char *		file_name;
    char *		name;
    char *		credits;
    sh_int		age;
    sh_int		nplayer;
    sh_int		low_range;
    sh_int		high_range;
    int 		min_vnum;
    int		max_vnum;
    bool		empty;
    unsigned long	count;
/* adds 4 for olc */
     char *		builders;	/* OLC */ /* Listing of */
     int		vnum;		/* OLC */ /* Area vnum  */
     int		area_flags;	/* OLC */
     int		security;	/* OLC */ /* Value 1-9  */
     int		worth;
     char *		owner;
};

/* People who have been here last */
struct room_history_data
{
  char *name;
  sh_int went; /* door number */
  ROOM_HISTORY_DATA *next;
  ROOM_HISTORY_DATA *prev;
};

/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *   aff_next;
    ROOM_INDEX_DATA *	next;
    CHAR_DATA *		people;
    OBJ_DATA *		contents;
    EXTRA_DESCR_DATA *	extra_descr;
    AREA_DATA *		area;
    EXIT_DATA *		exit	[6];
    EXIT_DATA * 	old_exit[6];
    RESET_DATA *	reset_first;	/* OLC */
    RESET_DATA *	reset_last;	/* OLC */
    char *		name;
    char *		description;
    char *		owner;
    int		vnum;
    int			room_flags;
    sh_int		light;
    sh_int		sector_type;
    sh_int		heal_rate;
    sh_int 		mana_rate;
    ROOM_HISTORY_DATA * history;
    AFFECT_DATA *       affected;
    long                affected_by;
};



#define		FULL_WIN	1
#define		FULL_LOSS	2
#define		FULL_KILL	3
#define		FULL_LEVEL	4
#define		RACE_WIN	5
#define		RACE_LOSS	6
#define		RACE_KILL	7
#define		RACE_LEVEL	8
#define		CLASS_WIN	9
#define		CLASS_LOSS	10
#define		CLASS_KILL	11
#define		CLASS_LEVEL	12
#define		KINGDOM_WIN	13
#define		KINGDOM_LOSS	14
#define		KINGDOM_KILL	15
#define		KINGDOM_LEVEL	16
#define		DUEL_WIN	17
#define		DUEL_LOSS	18
#define		DUEL_KILL	19
#define		DUEL_LEVEL	20
#define		TEAM_WIN	21
#define		TEAM_LOSS	22
#define		TEAM_KILL	23
#define		TEAM_LEVEL	24
#define		ALIG_WIN	25
#define		ALIG_LOSS	26
#define		ALIG_KILL	27
#define		ALIG_LEVEL	28
#define		MOB_DEATH	29
#define		KILLED_MOB	30
#define		PLAYER_DEATH	31
#define		PLAYER_KILL	32
#define		MAX_SCORE	32
/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000
#define TYPE_HUNGER                  999


/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4
#define TAR_OBJ_CHAR_DEF	    5
#define TAR_OBJ_CHAR_OFF	    6

#define TARGET_CHAR		    0
#define TARGET_OBJ		    1
#define TARGET_ROOM		    2
#define TARGET_NONE		    3



/*
 * Skills include spells as a particular case.
 */
/* 
   added constant to all vars except
   skill level array and spell_fun and the pgsn thing
   That goes for whole STRUCT definition
	took CONST from the name field because line 188
	of magic.c was whining *boggle*
*/
struct	skill_type
{
    char *	name;			/* Name of skill */
    sh_int	skill_level[MAX_CLASS];	/* Level needed by class*/
    const sh_int	rating[MAX_CLASS];	/* How hard it is to learn*/
    SPELL_FUN *	spell_fun;	/* Spell pointer (for spells)*/
    const sh_int	target;		/* Legal targets	*/
    const sh_int	minimum_position;/* Position for caster / user*/
    sh_int *	pgsn;		/* Pointer to associated gsn*/
    const sh_int	slot;		/* Slot for #OBJECT loading*/
    const sh_int	min_mana;	/* Minimum mana used	*/
    const sh_int	beats;		/* Waiting time after use*/
    const char *	noun_damage;	/* Damage message	*/
    const char *	msg_off;	/* Wear off message	*/
    const char *	msg_obj;	/* Wear off message for obects*/
    const sh_int 	kingdom;	/* Kingdom spells 	*/
    const sh_int        group;          /* skill group for practicing */
};


struct auth_type
{
    char *      name;
    char *      level;
    char *      commands[25]; /* 25 is an arbitrary number */
};


/*
 * These are skill_lookup return values for common skills and spells.
 */
extern	sh_int	gsn_dual_wield;
extern	sh_int	gsn_whirlwind;
extern  sh_int  gsn_scream;
extern  sh_int  gsn_critical;
extern	sh_int	gsn_heighten;
extern	sh_int	gsn_domination;

extern	sh_int	gsn_backstab;
extern	sh_int	gsn_dodge;
extern  sh_int  gsn_envenom;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_relock;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern	sh_int	gsn_bagpeek;
extern	sh_int	gsn_bagsteal;

extern	sh_int	gsn_disarm;
extern	sh_int	gsn_enhanced_damage;
extern	sh_int	gsn_brutal_damage;
extern	sh_int	gsn_kick;
extern	sh_int	gsn_feign_death;
extern	sh_int	gsn_crush;
extern	sh_int	gsn_swoop;
extern	sh_int	gsn_parry;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_second_attack;
extern	sh_int	gsn_third_attack;
extern  sh_int  gsn_extra_attack;
extern  sh_int  gsn_blindness_dust;
extern  sh_int  gsn_poison_smoke;
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_blade_barrier;
extern	sh_int	gsn_control_undead;
extern	sh_int	gsn_death_door;
extern	sh_int	gsn_death_door_protect;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_remove_curse;
extern	sh_int	gsn_invis;
//extern	sh_int	gsn_improved_invis;
extern	sh_int	gsn_phasing;
extern	sh_int	gsn_shadow_door;
extern	sh_int	gsn_hold_person;
extern	sh_int	gsn_paralysis;
extern	sh_int	gsn_mass_invis;
extern  sh_int  gsn_plague;
extern  sh_int  gsn_contagious;
extern  sh_int  gsn_contagion;
extern  sh_int  gsn_contagion_s;
extern  sh_int  gsn_contagion_w;
extern  sh_int  gsn_contagion_d;
extern  sh_int  gsn_contagion_c;
extern  sh_int  gsn_contagion_i;
extern  sh_int  gsn_wither;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;
extern  sh_int  gsn_fly;
extern  sh_int  gsn_strengthen_ether;
extern  sh_int  gsn_death_protection;
extern  sh_int  gsn_sanctuary;
extern sh_int gsn_forestry;
extern sh_int gsn_mountaineer;
extern sh_int gsn_reaction;
/* new gsns */
extern sh_int  gsn_axe;
extern sh_int  gsn_dagger;
extern sh_int  gsn_flail;
extern sh_int  gsn_mace;
extern sh_int  gsn_polearm;
extern sh_int  gsn_shield_block;
extern sh_int  gsn_spear;
extern sh_int  gsn_sword;
extern sh_int  gsn_bow;
extern sh_int  gsn_arrow;
extern sh_int  gsn_lance;
extern sh_int  gsn_whip;
extern sh_int  gsn_swim;
extern sh_int  gsn_bash;
extern sh_int  gsn_shieldbash;
extern sh_int  gsn_berserk;
extern sh_int  gsn_dirt;
extern sh_int  gsn_hand_to_hand;
extern sh_int  gsn_trip;
extern sh_int  gsn_submission;
extern sh_int  gsn_mounted_fighting;
extern sh_int  gsn_corruption;
extern sh_int  gsn_regeneration;
extern sh_int  gsn_regenerate;
extern sh_int  gsn_insanity;
extern sh_int  gsn_fast_healing;
extern sh_int  gsn_haggle;
extern sh_int  gsn_undead_mastery;
extern sh_int  gsn_lore;
extern sh_int  gsn_meditation;
extern sh_int  gsn_multiburst;
extern sh_int  gsn_dive;

extern sh_int  gsn_scrolls;
extern sh_int  gsn_staves;
extern sh_int  gsn_wands;
extern sh_int  gsn_recall;
extern sh_int  gsn_detect_hidden;
extern sh_int  gsn_acute_vision;

extern sh_int  gsn_fourth_attack;
extern sh_int  gsn_fifth_attack;
extern sh_int  gsn_sixth_attack;
extern sh_int  gsn_dual_backstab;
extern sh_int  gsn_double_whirlwind;
extern sh_int  gsn_cleave;
extern sh_int  gsn_counter;
extern sh_int  gsn_ambush;
extern sh_int  gsn_camouflage;
extern sh_int  gsn_circle;
extern sh_int  gsn_nerve;
extern sh_int  gsn_endure;
extern sh_int  gsn_quiet_movement;
extern sh_int  gsn_herbs;
extern sh_int  gsn_tame;
extern sh_int  gsn_butcher;
extern sh_int  gsn_assassinate;
extern sh_int  gsn_wanted;
extern sh_int  gsn_caltraps;
extern sh_int  gsn_throw;
extern sh_int  gsn_strangle;
extern sh_int  gsn_blackjack;
extern sh_int  gsn_gouge;
extern sh_int  gsn_bloodthirst;
extern sh_int  gsn_spellbane;
extern sh_int  gsn_resistance;
extern sh_int  gsn_deathblow;
extern sh_int  gsn_doppelganger;
extern sh_int  gsn_fade;
extern sh_int  gsn_garble;
extern sh_int  gsn_confuse;
extern sh_int  gsn_mirror;
extern sh_int  gsn_track;
extern sh_int  gsn_riding;
extern sh_int  gsn_bandage;
extern sh_int  gsn_layhands;
extern sh_int  gsn_camp;
extern sh_int  gsn_smithing;
extern sh_int  gsn_hunt;
extern sh_int  gsn_world_find;
extern sh_int  gsn_vanish;
extern sh_int  gsn_barkskin;
extern sh_int  gsn_chaos_blade;
extern sh_int  gsn_terangreal;
extern sh_int  gsn_dispel_evil;
extern sh_int  gsn_dispel_good;
extern sh_int  gsn_wrath;
extern sh_int  gsn_stalker;
extern sh_int  gsn_tesseract;
extern sh_int  gsn_randomizer;
extern sh_int  gsn_trophy;
extern sh_int  gsn_stronghold;
extern sh_int  gsn_truesight;
extern sh_int  gsn_brew;
extern sh_int  gsn_flamestrike;
extern sh_int  gsn_shadowlife;
extern sh_int  gsn_shade;
extern sh_int  gsn_justice_badge;
extern sh_int  gsn_remove_badge;
extern sh_int  gsn_dragon_strength;
extern sh_int  gsn_dragon_breath;
extern sh_int  gsn_warcry;
extern sh_int  gsn_stoneform;
extern sh_int  gsn_entangle;
extern sh_int  gsn_dragonsword;
extern sh_int  gsn_title;
extern sh_int  gsn_guard;
extern sh_int  gsn_guard_call;
extern sh_int  gsn_love_potion;
extern sh_int  gsn_deafen;
extern sh_int  gsn_protective_shield;
extern sh_int  gsn_protection_heat_cold;
extern sh_int  gsn_trance;
extern sh_int  gsn_celestial_spirit;
extern sh_int  gsn_nightwalker;
extern sh_int  gsn_bear_call;
extern sh_int  gsn_squire;
extern sh_int  gsn_lightning_bolt;
extern sh_int  gsn_disperse;
extern sh_int  gsn_bless;
extern sh_int  gsn_dimension;
extern sh_int  gsn_animate_dead;
extern sh_int  gsn_duo_dimension;
extern sh_int  gsn_weaken;
extern sh_int  gsn_feeblemind;
extern sh_int  gsn_haste;
extern sh_int  gsn_cure_critical;
extern sh_int  gsn_cure_poison;
extern sh_int  gsn_cure_disease;
extern sh_int  gsn_cure_blindness;
extern sh_int  gsn_cure_serious;
extern sh_int  gsn_magic_missile;
extern sh_int  gsn_faerie_fire;
extern sh_int  gsn_shield;
extern sh_int  gsn_demonfire;
extern sh_int  gsn_chill_touch;
extern sh_int  gsn_cure_light;
extern sh_int  gsn_settraps;
extern sh_int  gsn_black_death;
extern sh_int  gsn_deadly_venom;
extern sh_int  gsn_lethargic_mist;
extern sh_int  gsn_slow;
extern sh_int  gsn_mysterious_dream;
extern sh_int  gsn_cursed_lands;

extern sh_int  gsn_web;
extern sh_int  gsn_esp;
extern sh_int  gsn_invul;
extern sh_int  gsn_ironskin;
extern sh_int  gsn_enhanced_armor;
extern sh_int  gsn_burning_hands;
extern sh_int  gsn_acid_blast;
extern sh_int  gsn_chill_shield;
extern sh_int  gsn_fire_shield;
extern sh_int  gsn_cloak_fear;
extern sh_int  gsn_vampiric_aura;
extern sh_int  gsn_ghoul_touch;
extern sh_int  gsn_bone_armor;
extern sh_int  gsn_malison;
extern sh_int  gsn_wail_banshee;
extern sh_int  gsn_vampiric_touch;
extern sh_int  gsn_ray_of_truth;
extern sh_int  gsn_spell_craft;
extern sh_int  gsn_transform;
extern sh_int  gsn_dark_vision;
extern sh_int  gsn_sharpen;
extern sh_int  gsn_charge;
extern sh_int  gsn_breath;
extern sh_int  gsn_phase;
extern sh_int  gsn_totem;
extern sh_int  gsn_panic;
extern sh_int  gsn_aim;
/*New kingdom spells/skills*/
extern sh_int  gsn_soul_coil;
extern sh_int  gsn_death_coil;
extern sh_int  gsn_aura_ignorance;
extern sh_int  gsn_transcendental_meditation;
extern sh_int  gsn_lightning_reflexes;
extern sh_int  gsn_fade;
extern sh_int  gsn_ironwill;
extern sh_int  gsn_triggered_ironwill;
extern sh_int  gsn_aura_divine_deflection;
extern sh_int  gsn_divine_deflection;
extern sh_int  gsn_circle_protectio;
extern sh_int  gsn_gondor_ornament;
extern sh_int  gsn_gondor_sword;
extern sh_int  gsn_gondor_ring;
extern sh_int  gsn_argus_dagger;
extern sh_int  gsn_argus_orb;
extern sh_int  gsn_argus_dragon;
extern sh_int  gsn_wicthelf;
extern sh_int  gsn_form_wraith;
extern sh_int  gsn_paranoia;
extern sh_int  gsn_spell_nassius_demon;
extern sh_int  gsn_spell_narssius_claw;
extern sh_int  gsn_blade_dance; 
extern sh_int  gsn_spell_talastin_shield;
extern sh_int  gsn_spell_talastin_soldier;
extern sh_int  gsn_spell_talastin_mindblock;
extern sh_int  gsn_spell_illusion_fields;
extern sh_int  gsn_spell_dragon_speed;
extern sh_int  gsn_spell_dragon_wrath;
extern sh_int  gsn_spell_dragon_lure;
extern sh_int  gsn_6th_Sense;
extern sh_int  gsn_spell_call_treefolk;
extern sh_int  gsn_spell_force_nature;
extern sh_int  gsn_battle_prowess;
extern sh_int  gsn_spell_heal_anywhere;
extern sh_int  gsn_jab;
extern sh_int  gsn_7_attack ;
extern sh_int  gsn_8_attack ;
extern sh_int  gsn_9_attack ;
extern sh_int  gsn_toxic;
 /*
 * Utility macros.
 */
#define IS_VALID(data)		((data) != NULL && (data)->valid)
#define VALIDATE(data)		((data)->valid = TRUE)
#define INVALIDATE(data)	((data)->valid = FALSE)
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define ENTRE(min,num,max)     ( ((min) < (num)) && ((num) < (max)) )
#define CHECK_POS(a, b, c)     {			\
                                       (a) = (b);	\
                                       if ( (a) < 0 )	\
                                       bug( "CHECK_POS : " c " == %d < 0", a );	\
                               }			
#define IS_WATER( var )         (((var)->sector_type == SECT_WATER_SWIM) || ((var)->sector_type == SECT_WATER_NOSWIM))
#define PERCENT(cur, max)	(max<=0?0:((cur)*100)/(max))
#define IS_NULLSTR(str)                ((str) == NULL || (str)[0] == '\0')

#define IS_AUTHORIZED(ch, cmd) \
       (IS_SET((ch)->pcdata->authorized[(cmd)/32], (long)1 << ((cmd)%32)))
#define AUTHORIZE(ch, cmd) \
       (SET_BIT((ch)->pcdata->authorized[(cmd)/32], (long)1 << ((cmd)%32)))
#define REVOKE(ch, cmd) \
       (REMOVE_BIT((ch)->pcdata->authorized[(cmd)/32], (long)1 <<((cmd)%32)))

#define NEW_IS_AUTHORIZED(ch, cmd) \
       (IS_SET((ch)->pcdata->new_authorized[(cmd)/32], (long)1 << ((cmd)%32)))
#define NEW_AUTHORIZE(ch, cmd) \
       (SET_BIT((ch)->pcdata->new_authorized[(cmd)/32], (long)1 << ((cmd)%32)))
#define NEW_REVOKE(ch, cmd) \
       (REMOVE_BIT((ch)->pcdata->new_authorized[(cmd)/32], (long)1 <<((cmd)%32)))       

/*
 * Character macros.
 */
#define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_MIRROR(ch)		((ch)->pIndexData->vnum == MOB_VNUM_MIRROR_IMAGE)
#define IS_IMMORTAL(ch)		(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)		(get_trust(ch) >= LEVEL_HERO)
#define IS_TRUSTED(ch,level)	(get_trust((ch)) >= (level))
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, 	(sn))) 
#define IS_AFFECTED2(ch,sn)	(IS_SET((ch)->affected_by2, 	(sn)))
#define CAN_DETECT(ch, sn)      (IS_SET((ch)->detection, 	(sn)))
#define	IS_PK(ch, vt)		(!IS_NPC((ch)) & !IS_NPC((vt)))

#define GET_AGE(ch)		((int) (17 + ((ch)->played \
				    + current_time - (ch)->logon )/72000))

#define IS_GOOD(ch)		(ch->alignment >= 350)
#define IS_EVIL(ch)		(ch->alignment <= -350)
#define IS_NEUTRAL(ch)		(!IS_GOOD(ch) && !IS_EVIL(ch))
#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define IS_NOT_RESTING(ch)		(ch->position > POS_RESTING)
#define GET_AC(ch,type)		((ch)->armor[type]			    \
		        + ( IS_AWAKE(ch)			    \
			? dex_app[get_curr_stat(ch,STAT_DEX)].defensive : 0 ))
#define GET_HITROLL(ch)	\
		((ch)->hitroll+str_app[get_curr_stat(ch,STAT_STR)].tohit)
#define GET_DAMROLL(ch) \
		((ch)->damroll+str_app[get_curr_stat(ch,STAT_STR)].todam)

#define IS_OUTSIDE(ch)		(!IS_SET(				    \
				    (ch)->in_room->room_flags,		    \
				    ROOM_INDOORS))
/* WAIT_STATE *2 is because in mow code player ticks are double of rom. */
/* #define WAIT_STATE(ch, npulse)	((ch)->wait = IS_IMMORTAL((ch))?1:UMAX((ch)->wait, (npulse)*2)) */
#define WAIT_STATE(ch, npulse)	((ch)->wait = UMAX((ch)->wait, (npulse)))
#define DAZE_STATE(ch, npulse)  ((ch)->daze = UMAX((ch)->daze, (npulse)))
#define get_carry_weight(ch)	((ch)->carry_weight + (ch)->silver/10 +  \
					(ch)->gold * 2 / 5)
#define HAS_TRIGGER(ch,trig)	(IS_SET((ch)->pIndexData->mprog_flags,(trig))) 
#define IS_SWITCHED( ch )       ( ch->desc && ch->desc->original )
#define IS_BUILDER(ch, Area)   ( !IS_NPC(ch) && !IS_SWITCHED( ch ) && 	\
                               ( ch->pcdata->security >= Area->security \
				|| strstr( Area->builders, ch->name ) 	\
                               || strstr( Area->builders, "All" ) ) )

#define CH(descriptor)		((descriptor)->original ? (descriptor)->original : (descriptor)->character)

#define PERS(ch, looker)        ( can_see( looker, (ch) ) ?         \
                                ( IS_NPC(ch) ? (ch)->short_descr :(ch)->name ) \
                                : ( (ch)->level > 85 ? "A divine being" :"someone") )

/*
 * Object macros.
 */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))
#define IS_WEAPON_STAT(obj,stat)(IS_SET((obj)->value[4],(stat)))
#define WEIGHT_MULT(obj)	((obj)->item_type == ITEM_CONTAINER ? \
	(obj)->value[4] : 100)

/* skill defines */
#define CLEVEL_OK(ch,skill)     (ch->level > skill_table[skill].skill_level[ch->class] )
#define KINGDOM_OK(ch,skill)      (skill_table[skill].kingdom == KINGDOM_NONE || skill_table[skill].kingdom == ch->kingdom )

/*
 * room macros
 */

#define IS_ROOM_AFFECTED(room, sn) (IS_SET((room)->affected_by,(sn)))
#define IS_RAFFECTED(room, sn)  (IS_SET((room)->affected_by, (sn)))
#define MOUNTED(ch)     ((!IS_NPC(ch) && ch->mount && ch->riding) ? \
                                ch->mount : NULL)
#define RIDDEN(ch)      ((IS_NPC(ch) && ch->mount && ch->riding) ? \
                                ch->mount : NULL)
#define IS_DRUNK(ch)    (IS_NPC(ch)  ? \
                              FALSE : ch->pcdata->condition[COND_DRUNK] > 10)

/*
 * Structure for a social in the socials table.
 */
struct	social_type
{
    char *    name;
    char *    char_no_arg;
    char *    others_no_arg;
    char *    char_found;
    char *    others_found;
    char *    vict_found;
    char *    char_not_found;
    char *    char_auto;
    char *    others_auto;
};



/*
 * Global constants.
 */
extern	const	struct	str_app_type	str_app		[31];
extern	const	struct	int_app_type	int_app		[31];
extern	const	struct	wis_app_type	wis_app		[31];
extern	const	struct	dex_app_type	dex_app		[31];
extern	const	struct	con_app_type	con_app		[31];

extern	const	struct	class_type	class_table	[MAX_CLASS];
extern	const	struct	weapon_type	weapon_table	[];
extern	const	struct	confuse_type	confuse_table	[];
extern  const   struct  item_type	item_table	[];
extern	const	struct	wiznet_type	wiznet_table	[];
extern	const	struct	attack_type	attack_table	[];
extern	const	struct	attack_type	trap_attack_table[];
extern  const	struct  race_type	race_table	[];
extern	const	struct	pc_race_type	pc_race_table	[MAX_PC_RACE];
extern  const	struct	spec_type	spec_table	[];
extern	const	struct	liq_type	liq_table	[];
extern	struct	skill_type	skill_table	[MAX_SKILL];
extern  const   struct  hometown_type   hometown_table  [];
extern  const   struct  religion_type   religion_table  [];
extern		AUCTION_DATA	  *	auction;
extern  char *  const  month_name  [];
extern  const   struct  ethos_type      ethos_table     [];
extern struct social_type *social_table;
extern  const   struct  info_type       info_table      []; 
extern	char *	const			title_table	[MAX_CLASS]
							[MAX_LEVEL+1]
							[2];
extern struct  kingdom_type      kingdom_table     [];
extern  const   struct  prac_type       prac_table      [];

/*
 * Global variables.
 */
/* board code added, one line */
extern		NOTE_DATA	  *	note_free;
extern		HELP_DATA	  *	help_first;
extern		SHOP_DATA	  *	shop_first;

extern		CHAR_DATA	  *	char_list;
extern		DESCRIPTOR_DATA   *	descriptor_list;
extern		OBJ_DATA	  *	object_list;

extern         MPROG_CODE        *     mprog_list; 

extern		char			bug_buf		[];
extern		time_t			current_time;
extern		bool			fLogAll;
extern		FILE *			fpReserve;
extern		KILL_DATA		kill_table	[];
extern		char			log_buf		[];
extern		TIME_INFO_DATA		time_info;
extern		WEATHER_DATA		weather_info;
extern         bool                    MOBtrigger;   
extern          long                    total_levels; /* for Obj limits */
extern          DISABLED_DATA     *     disabled_first;
extern          int                     copyover_counter;
extern          int                     loadhelp;
extern		int			arena_counter;
extern          ROOM_INDEX_DATA   *     top_affected_room;
char *	crypt		args( ( const char *key, const char *salt ) );

void    wipe_pfiles     args( ( void ) );
void    remove_inactive args( ( char*, int ) ); 
/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif

/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
/*Public Report logs*/
#define TYPO_FILE       "../html/TYPOS.TXT" /* For 'typo'*/
#define BUG_FILE        "../html/BUGS.TXT" /* For 'bug' and bug()*/
#define WEBWHO_FILE     "../html/webwho.html" /* For 'bug' and bug()*/
#define TOP_FILE        "./top.dat" /* for top data*/
#define TOP_HTML        "../html/top.html" /* top list html*/
#define HELPS_FILE	"fate.are"
/*Load*/
#define CLASS_DIR       "../class/"

/* Player files */
#define PLAYER_DIR      "../player/" 
#define ARCHIVE_DIR     "../player/archive/"             /* Archive for pwiped pfiles */
#define PFILE_EXPIRE 60				  /* Number of days before archiving inactive pfiles*/
#define ARCHIVE_EXPIRE 60			  /* Number of days before removing archived pfiles*/
        	
/*Backup*/
#define PLAYERBAK_DIR   "../playerbak/"
#define DEATH_DIR       "../deathbak/"

#define GOD_DIR         "../gods/"  		/* list of gods */
#define TEMP_FILE	"../player/romtmp"
#define NULL_FILE	"/dev/null"		/* To reserve one stream */
#define AREA_LIST       "./AREA.LST"  /* List of areas*/
#define ILLEGAL_FILE    "./ILLEGAL.TXT"  /* List of areas*/
#define REGISTER_FILE   "./REGISTER.TXT"  /* List of areas*/
#define SHUTDOWN_FILE   "./SHUTDOWN.TXT"/* For 'shutdown'*/
#define BAN_FILE	"./BAN.TXT"
#define AREASTAT_FILE	"./AREA_STAT.TXT"
#define DISABLED_FILE "disabled.txt"  /* disabled commands */ 
#define HELP_FILE	"fate.are"
#define IMM_LOG_FILE    "../logs/immortals.log"
#define GOSSIP_LOG      "../logs/gossip.log"
#define BOOT_LOG        "../logs/boot.succes"
#define FUNCTION_FILE   "../logs/lastfunction.log"

/* determines object repops with this formula...
	REPOP_CONST+(pObjIndex->repop*(100-REPOP_CONST/100)) */
#define REPOP_CONST 50

/* mobs are given new ac values based on this, make it higher for better ac */
#define AC_CONST 6.3

/* these are here to change what people gain on levelling */
/* make sure to rm update.o when you change these    */
#define HITS_MOD 1.05
#define MANA_MOD 1.00
#define MOVE_MOD 0.90

#define PK_RANGE 0.15    /* 30% range, 15% + or - your own total exp */
#define PK_MIN 41000		/* 41000 */

#define MAX_NEWBIES 999 /* above this # of players newbies not allowed */
#define MAX_OLDIES  999 /* above this # only imms allowed (53+) */

/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define GF	GAME_FUN
#define AD	AFFECT_DATA
#define MPC    MPROG_CODE  

/* act_comm.c */
void  	check_sex	args( ( CHAR_DATA *ch) );
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	stop_follower	args( ( CHAR_DATA *ch ) );
void 	nuke_pets	args( ( CHAR_DATA *ch ) );
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void    do_info         args( (CHAR_DATA *ch, char *argument   ) );
void    info            args( (char *string, CHAR_DATA *ch, OBJ_DATA *obj,
                        long flag, long flag_skip, int min_level) );
long    info_lookup     args( (const char *name) );
void do_yell            args( ( CHAR_DATA *ch, char *argument ) );
void do_split           args( ( CHAR_DATA *ch, char *argument ) );

/* act_enter.c */
RID  *get_random_room   args ( (CHAR_DATA *ch) );

/* act_info.c */
void	set_title	  args( ( CHAR_DATA *ch, char *title ) );
void    show_list_to_char args( ( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing ) );

/* act_move.c */
void	move_char	args( ( CHAR_DATA *ch, int door, bool follow ) );
void hunt_victim	args( (CHAR_DATA *ch) );
int find_path		args( ( int in_room_vnum, int out_room_vnum,
				CHAR_DATA *ch, int depth, int in_zone ) );
void war_update		args( ( void ) );
void do_wake            args( ( CHAR_DATA *ch, char *argument ) );

/* act_obj.c */
bool can_loot		args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void   wear_obj        args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace) );  
void    get_obj         args( ( CHAR_DATA *ch, OBJ_DATA *obj,
                            OBJ_DATA *container ) );
bool    check_dispel    args(( int dis_level, CHAR_DATA *victim, int sn));
void 	check_look	args( (OBJ_DATA *obj) );

/* act_wiz.c */
void wiznet		args( (char *string, CHAR_DATA *ch, OBJ_DATA *obj,
			       long flag, long flag_skip, int min_level ) );
void copyover_fate      args( ( void ) );
void copyover_recover   args( ( void ) );
void     add_illegal      args( (CHAR_DATA *ch, char *name) );
void     remove_illegal   args( (CHAR_DATA *ch, char *name) );
bool     check_illegal    args( (char *name) );
void     list_illegal     args( ( CHAR_DATA *ch ) );
void     do_transfer	args( ( CHAR_DATA *ch, char *argument ) );


/* alias.c */
void 	substitute_alias args( (DESCRIPTOR_DATA *d, char *input) );

/* ban.c */
bool	check_ban	args( ( char *site, int type) );
bool check_notallow args( (char *site,char *bansite) );
bool	check_site	args( (CHAR_DATA *ch, char *site) );

/* clan.c */
void load_clans args( (void) );
void add_member args( (char *name, int kingdom) );
void subtract_member args( (char *name, int kingdom) );
void remove_guardians args ( (CHAR_DATA *ch) );
void    send_to_quest   args( ( char *argument, CHAR_DATA *ch ) );

/* comm.c */
void	show_string	args( ( struct descriptor_data *d, char *input) );
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt,
			    int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void	center_to_char	args( ( char *argument, CHAR_DATA *ch, int columns ) );
void	page_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	act_new		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type,
			    int min_pos) );
void    act_color       args( ( const char *format, CHAR_DATA *ch,
                            const void *arg1, const void *arg2, int type,
                            int min_pos, ...) );
void    dump_to_scr	args( ( char *text ) );
void   printf_to_char  args( ( CHAR_DATA *, char *, ... ) );
void   bugf            args( ( char *, ... ) );

 /*
  * Colour stuff by Lope of Loping Through The MUD
  */
  
//log.c  
void Log_system 	args( (CHAR_DATA *ch, char *LogLine,int Log,int Kingdom,int InetFlag) );
/* 4 line added for lope */
int	colour		args( ( char type, CHAR_DATA *ch, char *string ) );
void	colourconv	args( ( char *buffer, const char *txt, CHAR_DATA *ch ) );
void	send_to_char_bw	args( ( const char *txt, CHAR_DATA *ch ) );
void	page_to_char_bw	args( ( const char *txt, CHAR_DATA *ch ) );
int     strlencolor     args( ( char *argument 		       ) );
bool    warring         args( (int kingdom, int enemy          ) );
/* db.c */
void 	smash_percent	args( ( char *str ) );
void 	restor_percent	args( ( char *str ) );
void   reset_area      args( ( AREA_DATA * pArea ) );          /* OLC */
void   reset_room      args( ( ROOM_INDEX_DATA *pRoom ) );     /* OLC */
char *  print_flags     args( ( int flag ));
void    boot_db         args( ( void ) );
void    load_classes            args( ( void ) );
void    save_classes            args( ( void ) );
void    area_update     args( ( void ) );
CD *    create_mobile   args( ( MOB_INDEX_DATA *pMobIndex ) );
void    clone_mobile    args( ( CHAR_DATA *parent, CHAR_DATA *clone) );
OD *    create_object   args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
OD *    create_object_nocount args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
OD *    create_object_org args((OBJ_INDEX_DATA *pObjIndex,int level,bool Count));
void    clone_object     args( ( OBJ_DATA *parent, OBJ_DATA *clone ) );
void    clear_char      args( ( CHAR_DATA *ch ) );
char *  get_extra_descr args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
MID *   get_mob_index   args( ( int vnum ) );
OID *   get_obj_index   args( ( int vnum ) );
RID *   get_room_index  args( ( int vnum ) );
MPC *  get_mprog_index args( ( int vnum ) );    
char    fread_letter    args( ( FILE *fp ) );
int     fread_number    args( ( FILE *fp ) );
long    fread_flag      args( ( FILE *fp ) );
char *  fread_string    args( ( FILE *fp ) );
char *  fread_string_eol args(( FILE *fp ) );
void    fread_to_eol    args( ( FILE *fp ) );
char *  fread_word      args( ( FILE *fp ) );
long    flag_convert    args( ( char letter) );
void *  alloc_mem       args( ( int sMem ) );
void *  alloc_perm      args( ( int sMem ) );
void    free_mem        args( ( void *pMem, int sMem ) );
char *  str_dup         args( ( const char *str ) );
void    free_string     args( ( char *pstr ) );
int     number_fuzzy    args( ( int number ) );
int     number_range    args( ( int from, int to ) );
int     number_percent  args( ( void ) );
int     number_door     args( ( void ) );
int     number_bits     args( ( int width ) );
long     number_mm       args( ( void ) );
int     dice            args( ( int number, int size ) );
int     interpolate     args( ( int level, int value_00, int value_32 ) );
void    smash_tilde     args( ( char *str ) );
bool    str_cmp         args( ( const char *astr, const char *bstr ) );
bool    str_prefix      args( ( const char *astr, const char *bstr ) );
bool    str_infix       args( ( const char *astr, const char *bstr ) );
bool    str_suffix      args( ( const char *astr, const char *bstr ) );
char *  capitalize      args( ( const char *str ) );
void    append_file     args( ( CHAR_DATA *ch, char *file, char *str ) );
void    bug             args( ( const char *str, int param ) );
void    log_string      args( ( const char *str ) );
void    tail_chain      args( ( void ) );
int     prac_lookup     args( ( const char *name ) );
char 	*prac_name	args( ( int number ) );

/* effect.c */
void    acid_effect     args( (void *vo, int level, int dam, int target) );
void    cold_effect     args( (void *vo, int level, int dam, int target) );
void    fire_effect     args( (void *vo, int level, int dam, int target) );
void    poison_effect   args( (void *vo, int level, int dam, int target) );
void    shock_effect    args( (void *vo, int level, int dam, int target) );
void    sand_effect     args( (void *vo, int level, int dam, int target) );
void    scream_effect   args( (void *vo, int level, int dam, int target) );

//ao.c
void do_get		args( ( CHAR_DATA *ch, char *argument ) );
void do_sacrifice	args( ( CHAR_DATA *ch, char *argument ) );
void damage_to_obj	args( (CHAR_DATA *ch,OBJ_DATA *wield, OBJ_DATA *worn, int damage) );
//death.c
void death_cry_org	args ( ( CHAR_DATA *ch, int part ) );
void raw_kill_org	args ( ( CHAR_DATA *victim, int part ) );
//fight_damage_check.c
bool check_death_door	args ( (CHAR_DATA *victim) );

//arena.c
bool arena_kill		args ( (CHAR_DATA *ch,CHAR_DATA *victim) );

//fight_damage.c

bool damage		args (( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show ) );
bool damage_hit		args ( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show ) );
void dam_message	args ( ( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune ) );
//fight_damage_affect.c
int affect_damage_stoneshape	args ( (CHAR_DATA *victim, int dam) );
int affect_bone_armor_damage	args ( (CHAR_DATA *victim,int dam) );
int affect_immune_damage	args ( (CHAR_DATA *victim,int dam_type,int dam) );
int affect_size_damage		args ( (CHAR_DATA *ch,int dam) );
int affect_dwarf_take_damage_drunk	args ( (CHAR_DATA *victim, int dam) );
int get_affect_enhanced_damage	args ( (CHAR_DATA *ch,int dam) );
int get_affect_brutal_damage	args ( (CHAR_DATA *ch,int dam) );
int get_dwarf_drunk_damage_affect	args ( (CHAR_DATA *ch,int dam) );
int get_sun_affect_ac	args ( (CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac) );
int get_drunk_affect_ac	args ( (CHAR_DATA *ch,CHAR_DATA *victim,int victim_ac) );
void affect_poison_weapon	args ( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_vampiric_weapon	args ( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_flaming_weapon	args ( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_frost_weapon	args ( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_shocking_weapon	args ( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );
void affect_manadrain_weapon	args ( (CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *wield) );

//fight_damage_check.c
bool check_mirror		args ( (CHAR_DATA *ch, CHAR_DATA *victim) );
bool check_phases		args ( (CHAR_DATA *victim) );
bool check_pixie_quickly	args ( (CHAR_DATA *victim) );
bool check_aura_of_invulnerability	args ( (CHAR_DATA *ch, CHAR_DATA *victim) );
bool check_sanctuary		args ( (CHAR_DATA *victim,int dt) );
bool check_cheat_damage		args ( (CHAR_DATA *ch,int dam) );
bool check_parry		args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_block		args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_dodge		args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_lightning_reflexes	args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_draggon_speed	args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_battle_prowess	args ( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
CHAR_DATA *check_guard		args ( (CHAR_DATA *ch, CHAR_DATA *mob) );
void check_assist		args ( (CHAR_DATA *ch,CHAR_DATA *victim) );
bool check_counter		args ( (CHAR_DATA *ch,CHAR_DATA *victim) );

/* fight.c */
void	one_hit		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool secondary ) );
bool 	is_safe		args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	is_safe_nomessage args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	is_safe_spell	args( (CHAR_DATA *ch, CHAR_DATA *victim, bool area ) );
void	violence_update	args( ( int size ) );
void	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
			        int dt, int class, bool show ) );
bool    damage_old      args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
                                int dt, int class, bool show ) );
void	update_pos	args( ( CHAR_DATA *victim ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void	check_killer	args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
bool    can_kill 	args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
void   gods_protect_msg args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
CHAR_DATA *  check_guard     args((CHAR_DATA *ch, CHAR_DATA *mob));
void    raw_kill        args( (CHAR_DATA *ch, CHAR_DATA *victim ) );   
bool check_battle_prowess args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void do_fleealways 		args( ( CHAR_DATA *ch, char *argument ) );
void    check_weapon_destroy    args( (CHAR_DATA *ch, CHAR_DATA *victim,bool second) );
int critical_strike		args( ( CHAR_DATA *ch, CHAR_DATA *victim,   int dam ) );
void    use_magical_item     args( ( CHAR_DATA *ch ) );
void    damage_to_object        args( (CHAR_DATA *ch, OBJ_DATA *wield,OBJ_DATA *worn, int damage) );
void    group_gain      args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    damage_to_object        args( (CHAR_DATA *ch, OBJ_DATA *wield,OBJ_DATA *worn, int damage) );
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

/* handler.c */
void 	getpidmud		args( () );
void 	full_strip_affect	args( (CHAR_DATA *ch) );
AD  	*affect_find args( (AFFECT_DATA *paf, int sn));
void	affect_check	args( (CHAR_DATA *ch, int where, int vector) );
int	count_users	args( (OBJ_DATA *obj) );
void 	deduct_cost	args( (CHAR_DATA *ch, int cost) );
void	affect_enchant	args( (OBJ_DATA *obj) );
int 	check_immune	args( (CHAR_DATA *ch, int dam_type) );
bool 	check_material	args( (OBJ_DATA *obj, char *material ) );
bool 	is_metal	args( (OBJ_DATA *obj ) );
bool	kingdom_ok	args( ( CHAR_DATA *ch, sh_int sn ) );
int 	material_lookup args( ( const char *name) );
int	weapon_lookup	args( ( const char *name) );
int	weapon_type	args( ( const char *name) );
char 	*weapon_name	args( ( int weapon_Type) );
char	*item_name	args( ( int item_type) );
int	attack_lookup	args( ( const char *name) );
int	trap_attack_lookup	args( ( const char *name) );
long	wiznet_lookup	args( ( const char *name) );
int	class_lookup	args( ( const char *name) );
bool	is_old_mob	args ( (CHAR_DATA *ch) );
int	get_skill	args( ( CHAR_DATA *ch, int sn ) );
int	get_weapon_sn	args( ( CHAR_DATA *ch ) );
int     get_second_sn    args( (CHAR_DATA *ch) );
int	get_weapon_skill args(( CHAR_DATA *ch, int sn ) );
int     get_age         args( ( CHAR_DATA *ch ) );
void	reset_char	args( ( CHAR_DATA *ch )  );
int	get_trust	args( ( CHAR_DATA *ch ) );
int	get_curr_stat	args( ( CHAR_DATA *ch, int stat ) );
int 	get_max_train	args( ( CHAR_DATA *ch, int stat ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
bool	is_name		args( ( char *str, char *namelist ) );
bool	is_name_imm	args( ( char *str, char *namelist ) );
bool	is_full_name	args( ( char *str, char *namelist ) );
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_to_obj	args( ( OBJ_DATA *obj, AFFECT_DATA *paf ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_remove_obj args( (OBJ_DATA *obj, AFFECT_DATA *paf ) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void    affect_to_room  args( ( ROOM_INDEX_DATA *room, AFFECT_DATA *paf ) );
void    affect_remove_room      args( ( ROOM_INDEX_DATA *room, AFFECT_DATA *paf ) );
void    affect_strip_room       args( ( ROOM_INDEX_DATA *ch, int sn ) );
bool    is_affected_room        args( ( ROOM_INDEX_DATA *ch, int sn ) );
void    affect_join_room        args( ( ROOM_INDEX_DATA *ch, AFFECT_DATA *paf ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear, int type ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj_nocount	args( ( OBJ_DATA *obj ) );
void	extract_obj_1	args( ( OBJ_DATA *obj, bool count ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
void	extract_char_nocount	args( ( CHAR_DATA *ch, bool fPull ) );
void	extract_char_org	args( ( CHAR_DATA *ch, bool fPull, bool Count ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_room2  args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument, int *number ) );
CD *	get_char_random_room	args( ( CHAR_DATA *ch, int type ) );
CD *	get_char_random_world	args( ( CHAR_DATA *ch, int type ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_carry_see_anyway args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	create_money	args( ( int gold, int silver ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_realnumber	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
int	get_true_weight	args( ( OBJ_DATA *obj ) );
bool	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex, CHAR_DATA *ch ) );
bool	is_room_owner	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_see_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *	item_type_name	args( ( OBJ_DATA *obj ) );
char *	affect_loc_name	args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
char *  affect2_bit_name args( ( int vector ) );
char *  detect_bit_name args( ( int vector ) );
char *	extra_bit_name	args( ( int extra_flags ) );
char *	race_bit_name	args( ( int race_flags ) );
char *	class_bit_name	args( ( int class_flags ) );
char * raffect_bit_name	args( ( int vector ) );
char * 	wear_bit_name	args( ( int wear_flags ) );
char *	act_bit_name	args( ( int act_flags ) );
char *	off_bit_name	args( ( int off_flags ) );
char *  imm_bit_name	args( ( int imm_flags ) );
char * 	form_bit_name	args( ( int form_flags ) );
char *	part_bit_name	args( ( int part_flags ) );
char *	weapon_bit_name	args( ( int weapon_flags ) );
char *  comm_bit_name	args( ( int comm_flags ) );
char *	cont_bit_name	args( ( int cont_flags) );
char *  flag_room_name	args( ( int vector) );
void    room_record    args( (char *name, ROOM_INDEX_DATA *room,sh_int door) );
bool    is_safe_rspell  args( ( int level, CHAR_DATA *victim) );
CD *    get_char        args( ( CHAR_DATA *ch ) );
char *raffect_loc_name  args( ( int location ) );
void	path_to_track	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int door) );
int opposite_door	args( (int door) );

/* interp.c */
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
bool	is_number	args( ( char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
int	mult_argument	args( ( char *argument, char *arg) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
char *	one_argument_nolow	args( ( char *argument, char *arg_first ) );
void  load_disabled   args( ( void ) );
void  save_disabled   args( ( void ) );
void do_ggnotext      args( (CHAR_DATA *ch) );
void do_lgrant 	      args( (CHAR_DATA *ch, char *argument) );
void do_new_ggrant    args( (CHAR_DATA *ch, char *argument) );
void do_new_cgrant    args( (CHAR_DATA *ch, char *argument) );
void do_gglist        args( (CHAR_DATA *ch, char *argument) );
/* lookup.c */
int    race_lookup     args( ( const char *name) );
int    item_lookup     args( ( const char *name) );
int    liq_lookup      args( ( const char *name) );

/* magic.c */
int	find_spell	args( ( CHAR_DATA *ch, const char *name) );
int 	mana_cost 	(CHAR_DATA *ch, int min_mana, int level);
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim, int dam_type ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );

/* mob_prog.c */
void   program_flow    args( ( int vnum, char *source, CHAR_DATA *mob,CHAR_DATA *ch,
                               const void *arg1, const void *arg2 ) );
void   mp_act_trigger  args( ( char *argument, CHAR_DATA *mob, CHAR_DATA *ch,
                               const void *arg1, const void *arg2, int type ) );
bool   mp_percent_trigger args( ( CHAR_DATA *mob, CHAR_DATA *ch,
                               const void *arg1, const void *arg2, int type ) );
void   mp_bribe_trigger  args( ( CHAR_DATA *mob, CHAR_DATA *ch, int amount) );
bool   mp_exit_trigger   args( ( CHAR_DATA *ch, int dir ) );
void   mp_give_trigger   args( ( CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj ) );
void   mp_greet_trigger  args( ( CHAR_DATA *ch ) );
void   mp_hprct_trigger  args( ( CHAR_DATA *mob, CHAR_DATA *ch ) );
int    clan_lookup       args(  (const char *name) );
/* mob_cmds.c */
void   mob_interpret   args( ( CHAR_DATA *ch, char *argument ) );

/* olc.c */
bool   run_olc_editor  args( ( DESCRIPTOR_DATA *d ) );
char   *olc_ed_name    args( ( CHAR_DATA *ch ) );
char   *olc_ed_vnum    args( ( CHAR_DATA *ch ) );

/* save.c */
void	save_char_obj	args( ( CHAR_DATA *ch ) );
bool	load_char_obj	args( ( DESCRIPTOR_DATA *d, char *name ) );
bool  	is_allowed	args( ( char *name) );

/* skills.c */
int     exp_to_level    args( ( CHAR_DATA *ch, int points ) );
int     base_exp        args( ( CHAR_DATA *ch, int points ) );
int     exp_this_level  args( ( CHAR_DATA *ch, int level, int points ) );

int 	exp_per_level	args( ( CHAR_DATA *ch, int points ) );
void    check_improve   args( ( CHAR_DATA *ch, int sn, bool success,
                                    int multiplier ) );
int     group_lookup    args( (const char *name) );
void    group_add       args( ( CHAR_DATA *ch ) );

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );
char *	spec_name	args( ( SPEC_FUN *function ) );

/* string.c */
void   string_edit     args( ( CHAR_DATA *ch, char **pString ) );
void    string_append   args( ( CHAR_DATA *ch, char **pString ) );
char * string_replace  args( ( char * orig, char * old, char * new ) );
void    string_add      args( ( CHAR_DATA *ch, char *argument ) );
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );
char *  first_arg       args( ( char *argument, char *arg_first, bool Case ) );
char * string_unpad    args( ( char * argument ) );
char * string_proper   args( ( char * argument ) );

/* teleport.c */
RID *	room_by_name	args( ( char *target, int level, bool error) );

/* trap.c */
bool  checkmovetrap  args( ( CHAR_DATA *ch ) );
bool  checkgetput    args( ( CHAR_DATA *ch, OBJ_DATA *obj) );
bool  checkopen      args( ( CHAR_DATA *ch, OBJ_DATA *obj) );

/* update.c */
void	advance_level	args( ( CHAR_DATA *ch ) );
void	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );
void    char_update	args( ( void ) );
void    area_update	args( ( void ) );
void    room_update     args( ( void ) );
void    update_arena	args( ( void ) );
/*  iprog.c */
void iprog_set(OBJ_INDEX_DATA *, const char *, const char *);

/* mprog.c */
void mprog_set(MOB_INDEX_DATA *, const char *, const char *);

/* random.c */
void wield_random_magic(CHAR_DATA *mob);
void wield_random_armor(CHAR_DATA *mob);


/*aw.c*/

void set_titles		args ( ( CHAR_DATA *ch, char *title , int type) );
void copyover_crash 	args ( ( void ) );
/*top.c*/
void update_top_rp	args ( ( CHAR_DATA *ch) );
void load_top		args ( (void) );

/*ac.c*/
void do_quit_org	args( (CHAR_DATA *ch, char *argument, bool Count, bool blnWiznet) );
void do_say		args( (CHAR_DATA *ch,char *argument ) );
void do_say_org		args(  ( CHAR_DATA *ch,char *argument ,bool ooc) );
void do_new_say_org	args( ( CHAR_DATA *ch, char *argument,bool ooc) );
/*ae.c*/
void do_hunt_mob	args( ( CHAR_DATA *ch, char *argument ) );
void do_enter		args( ( CHAR_DATA *ch, char *argument) );
/*am.c*/
void do_stand		args ( ( CHAR_DATA *ch, char *argument ) );
void do_visible		args ( ( CHAR_DATA *ch, char *argument ) );
void do_recall		args ( ( CHAR_DATA *ch, char *argument ) );
void do_flee		args ( ( CHAR_DATA *ch, char *argument ) );
/*ai.c*/
void  do_look		args( ( CHAR_DATA *ch, char *argument ) );

/*top.c*/
void save_top		args( (void) );
void save_top_html	args( (void) );
 
void get_who_data 	args( (char buf[MAX_INPUT_LENGTH], char arg[MAX_INPUT_LENGTH], int len) );
void web		args( ( void ) );

#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
#undef	GF
#undef AD

/*****************************************************************************
 *                                    OLC                                    *
 *****************************************************************************/

/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
#define OBJ_VNUM_DUMMY	69

/*
 * Area flags.
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */

#define MAX_DIR	6
#define NO_FLAG -99	/* Must not be used in flags or stats. */

/*
 * Global Constants
 */
extern	char *	const	dir_name        [];
extern	const	sh_int	rev_dir         [];          /* sh_int - ROM OLC */
extern	const	struct	spec_type	spec_table	[];

/*
 * Global variables
 */
extern         AREA_DATA *             area_first;
extern         AREA_DATA *             area_last;
extern         SHOP_DATA *             shop_last;

extern         int                     top_affect;
extern         int                     top_area;
extern         int                     top_ed;
extern         int                     top_exit;
extern         int                     top_help;
extern         int                     top_mob_index;
extern         int                     top_obj_index;
extern         int                     top_reset;
extern         int                     top_room;
extern         int                     top_shop;

extern         int                     top_vnum_mob;
extern         int                     top_vnum_obj;
extern         int                     top_vnum_room;

extern         char                    str_empty       [1];

extern         MOB_INDEX_DATA *        mob_index_hash  [MAX_KEY_HASH];
extern         OBJ_INDEX_DATA *        obj_index_hash  [MAX_KEY_HASH];
extern         ROOM_INDEX_DATA *       room_index_hash [MAX_KEY_HASH];

/* html */
void who_html_update args( ( void ) );

/* sedit */
void load_social_table args( () );
void save_social_table args( () );
