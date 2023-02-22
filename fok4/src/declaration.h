/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/***************************************************************************
                       Declaration of functions
****************************************************************************/

// ac.c 
char * makedrunk 		args ( ( char *string, CHAR_DATA * ch) );
char *make_talk_hasted		args ( ( char *argument) );
void do_delet			args ( ( CHAR_DATA *ch, char *argument) );
void do_delete			args ( ( CHAR_DATA *ch, char *argument) );
void do_channels		args ( ( CHAR_DATA *ch, char *argument) );
void garble			args ( ( char *garbled,char *speech) );
void do_deaf			args ( ( CHAR_DATA *ch, char *argument) );
void do_quiet 			args ( ( CHAR_DATA *ch, char * argument) );
void do_afk 			args ( ( CHAR_DATA *ch, char * argument) );
void do_busy 			args ( ( CHAR_DATA *ch, char * argument) );
void do_replay 			args ( ( CHAR_DATA *ch, char *argument) );
void do_grats			args ( ( CHAR_DATA *ch, char *argument ) );
void do_say			args ( ( CHAR_DATA *ch,char *argument) );
void do_say_org			args ( ( CHAR_DATA *ch,char *argument,bool ooc ) );
void do_new_say			args ( ( CHAR_DATA *ch, char *argument ) );
void do_new_say_org		args ( ( CHAR_DATA *ch, char *argument,bool ooc ) );
void do_shout			args ( ( CHAR_DATA *ch, char *argument ) );
void do_anon_shout		args ( ( CHAR_DATA *ch, char *argument ) );
void do_tell			args ( ( CHAR_DATA *ch, char *argument ) );
void do_reply			args ( ( CHAR_DATA *ch, char *argument ) );
void do_yell			args ( ( CHAR_DATA *ch, char *argument ) );
void do_narrate			args ( ( CHAR_DATA *ch, char *argument ) );
void do_emote			args ( ( CHAR_DATA *ch, char *argument ) );
void do_pmote			args ( ( CHAR_DATA *ch, char *argument ) );
void do_bug			args ( ( CHAR_DATA *ch, char *argument ) );
void do_typo			args ( ( CHAR_DATA *ch, char *argument ) );
void do_rent			args ( ( CHAR_DATA *ch, char *argument ) );
void do_qui			args ( ( CHAR_DATA *ch, char *argument ) );
void do_quit			args ( ( CHAR_DATA *ch, char *argument ) );
void do_quit_count		args ( ( CHAR_DATA *ch, char *argument ) );
void do_quit_org		args ( ( CHAR_DATA *ch, char *argument, bool Count, bool blnWiznet ) );
void do_save			args ( ( CHAR_DATA *ch, char *argument ) );
void do_follow			args ( ( CHAR_DATA *ch, char *argument ) );
void add_follower		args ( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void stop_follower		args ( ( CHAR_DATA *ch ) );
void nuke_pets			args ( ( CHAR_DATA *ch ) );
void die_follower		args ( ( CHAR_DATA *ch ) );
void do_group			args ( ( CHAR_DATA *ch, char *argument ) );
void do_split			args ( ( CHAR_DATA *ch, char *argument ) );
void do_gtell			args ( ( CHAR_DATA *ch, char *argument ) );
bool is_same_group		args ( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void do_colour			args ( ( CHAR_DATA *ch, char *argument ) );
void do_gd			args ( ( CHAR_DATA *ch, char *argument ) );
void do_pray			args ( ( CHAR_DATA *ch, char *argument) );
void do_msp			args ( ( CHAR_DATA *ch, char *argument ) );
void do_vt			args ( ( CHAR_DATA *ch, char *argument ) );
void do_locktitle		args ( ( CHAR_DATA *ch, char *argument) );
void do_lockon			args ( ( CHAR_DATA *ch, char *argument) );
bool check_site			args ( ( CHAR_DATA *ch,char *site) );
void do_sitelock		args ( ( CHAR_DATA *ch, char *argument) );

