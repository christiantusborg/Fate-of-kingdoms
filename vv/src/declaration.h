/***************************************************************************
 *  Fate of Kingdom IV                                                     *
 *   By Chrisitan Tusborg (Kazmir).                                        *
 *									   *
 * The use of this code without the permission of Christian Tusborg        *
 * author is prohibited.  You will be punished to the fullest extend       *
 * of the law.                                                             *
 *                                                                         *
 *     Original Fate of Kingdom Mud (Mud.mysteria.dk:4000)		   *
 ***************************************************************************/
/***************************************************************************
 * File: declaration.h, declaration of functions to global use.             *
 ***************************************************************************/

int     lookup_stat       args( (char *argument,int stattype) );

//function list fok_db.c
void    save_raceses       args( ( void ) );
void    save_race          args( (int num) );
void    load_raceses       args( ( void ) );
void    load_race          args( ( int num ) );

// function list fok_olc.c
void setup_race            args( (CHAR_DATA *ch ,char * argument) );

// function list ae.c
 CHAR_DATA *get_char_area  args( ( CHAR_DATA *ch, char *argument ) );
 
 // function list am.c
 int find_door             args( ( CHAR_DATA *ch, char *arg ) );
 int find_exit             args( ( CHAR_DATA *ch, char *arg ) );
 CHAR_DATA * find_char     args( ( CHAR_DATA *ch, char *argument,int door, int range ) );
 int send_arrow            args( ( CHAR_DATA *ch, CHAR_DATA *victim,OBJ_DATA *arrow , int door, int chance ,int bonus) );

// function list fight.c 
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) ); 
void    disarm          args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

// function list magic.c
void spell_blindness   args( ( int sn, int level, CHAR_DATA *ch, void *vo, int target) );
void spell_poison      args( ( int sn, int level, CHAR_DATA *ch, void *vo, int target ) );

//function list skill.c
void get_objx          args( ( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container ) );

// function list lookup.c
//int flag_lookup        args( ( const char *name, const struct flag_type *flag_table) );

// function list random.c
void wield_random_magic args( ( CHAR_DATA *mob ) );

// function list abilities_functions.c
void do_berserk         args( ( CHAR_DATA *ch, char *argument) );

// function handel.c
int lookup_deity        args( (const char *name) );
int lookup_Bloodline    args( (const char *name) );
