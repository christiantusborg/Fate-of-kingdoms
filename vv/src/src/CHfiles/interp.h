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

/* this is a listing of all the commands and command related data */

void do_function args((CHAR_DATA *ch, DO_FUN *do_fun, char *argument));

/* for command types */ 
#define ML 	MAX_LEVEL	/* implementor */
#define L0	MAX_LEVEL - 1  	/* system */
#define L1	MAX_LEVEL - 5  	/* creator */
#define L2	MAX_LEVEL - 10	/* supreme being */
#define L3	MAX_LEVEL - 11	/* deity */
#define L4 	MAX_LEVEL - 12	/* god */
#define L5	MAX_LEVEL - 13	/* immortal */
#define L6	MAX_LEVEL - 14	/* demigod */
#define L7	MAX_LEVEL - 15	/* angel */
#define L8	MAX_LEVEL - 16	/* avatar */
#define L9	MAX_LEVEL - 17	/* avatar */
#define IM	LEVEL_IMMORTAL 	/* angel */
#define HE	LEVEL_HERO	/* hero */

#define COM_INGORE	1


/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
     sh_int             auth;    
    sh_int		log;
    sh_int              show;
    sh_int		extra;	/* keep hide... etc */
};

/* the command table itself */
extern	const	struct	cmd_type	cmd_table	[];

struct	new_cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
    sh_int		group;
    sh_int		level;
    sh_int             auth;    
    sh_int		log;
    sh_int              show;
    sh_int		extra;	/* keep hide... etc */
};


struct	cmd_group_type
{
    char * const	name;	/*Name of group*/
    sh_int		groupid; /*id of group*/
    sh_int              auth;    /*who can grant it??*/
};
/* the command table itself */
extern	const	struct	new_cmd_type	new_cmd_table	[];

DECLARE_DO_FUN( do_save_clans         );

/*
 * Command functions.
 * Defined in act_*.c (mostly).
 */
DECLARE_DO_FUN( do_ambush       );
DECLARE_DO_FUN(	do_advance	);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_affections   );
DECLARE_DO_FUN( do_afk		);
DECLARE_DO_FUN( do_alia		);
DECLARE_DO_FUN( do_alias	);
DECLARE_DO_FUN(	do_allow	);
DECLARE_DO_FUN( do_gossip	);
DECLARE_DO_FUN(	do_areas	);
DECLARE_DO_FUN( do_assassinate  );
DECLARE_DO_FUN(	do_at		);
DECLARE_DO_FUN(	do_auction	);
DECLARE_DO_FUN( do_clan		);
DECLARE_DO_FUN( do_clear	);
DECLARE_DO_FUN( do_cls		);
DECLARE_DO_FUN( do_autoassist	);
DECLARE_DO_FUN( do_autoexit	);
DECLARE_DO_FUN( do_autogold	);
DECLARE_DO_FUN( do_autolist	);
DECLARE_DO_FUN( do_autoloot	);
DECLARE_DO_FUN( do_autosac	);
DECLARE_DO_FUN( do_autosplit	);
DECLARE_DO_FUN( do_autodamage	);
DECLARE_DO_FUN(	do_backstab	);
DECLARE_DO_FUN(	do_balance	);
DECLARE_DO_FUN(	do_bamfin	);
DECLARE_DO_FUN(	do_bamfout	);
DECLARE_DO_FUN(	do_ban		);
DECLARE_DO_FUN( do_bash		);
DECLARE_DO_FUN( do_shieldbash	);
DECLARE_DO_FUN( do_berserk	);
DECLARE_DO_FUN( do_bloodthirst  );
DECLARE_DO_FUN( do_blackjack    );
DECLARE_DO_FUN(	do_brandish	);
DECLARE_DO_FUN( do_brief	);
DECLARE_DO_FUN(	do_bug		);
DECLARE_DO_FUN( do_butcher      );
DECLARE_DO_FUN(	do_buy		);
DECLARE_DO_FUN( do_caltraps     );
DECLARE_DO_FUN( do_circle       );
DECLARE_DO_FUN( do_camouflage   );
DECLARE_DO_FUN(	do_cast		);
DECLARE_DO_FUN( do_changes	);
DECLARE_DO_FUN( do_channels	);
DECLARE_DO_FUN( do_cleave       );
DECLARE_DO_FUN( do_clone	);
DECLARE_DO_FUN(	do_close	);
DECLARE_DO_FUN( do_colour       );
DECLARE_DO_FUN(	do_commands	);
DECLARE_DO_FUN( do_combine	);
DECLARE_DO_FUN( do_commune	);
DECLARE_DO_FUN( do_compact	);
DECLARE_DO_FUN(	do_compare	);
DECLARE_DO_FUN(	do_consider	);
DECLARE_DO_FUN( do_count	);
DECLARE_DO_FUN(	do_credits	);
DECLARE_DO_FUN( do_deaf		);
DECLARE_DO_FUN( do_delet	);
DECLARE_DO_FUN( do_delete	);
DECLARE_DO_FUN(	do_deny		);
DECLARE_DO_FUN(	do_deposit	);
DECLARE_DO_FUN(	do_description	);
DECLARE_DO_FUN( do_dirt		);
DECLARE_DO_FUN(	do_disarm	);
DECLARE_DO_FUN(	do_disconnect	);
DECLARE_DO_FUN(	do_down		);
DECLARE_DO_FUN( do_dismount      );
DECLARE_DO_FUN(	do_drink	);
DECLARE_DO_FUN(	do_drop		);
DECLARE_DO_FUN( do_dump		);
DECLARE_DO_FUN(	do_east		);
DECLARE_DO_FUN(	do_eat		);
DECLARE_DO_FUN(	do_echo	);
DECLARE_DO_FUN(	do_echo_font    );
DECLARE_DO_FUN(	do_emote	);
DECLARE_DO_FUN( do_endure       );
DECLARE_DO_FUN( do_enter	);
DECLARE_DO_FUN( do_envenom	);
DECLARE_DO_FUN(	do_equipment	);
DECLARE_DO_FUN(	do_examine	);
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN(	do_exlist	);
DECLARE_DO_FUN( do_fade         );
DECLARE_DO_FUN(	do_fill		);
DECLARE_DO_FUN( do_flag		);
DECLARE_DO_FUN(	do_flee		);
DECLARE_DO_FUN(	do_follow	);
DECLARE_DO_FUN(	do_font  	);
DECLARE_DO_FUN(	do_force	);
DECLARE_DO_FUN(	do_freeze	);
DECLARE_DO_FUN( do_gain		);
DECLARE_DO_FUN(	do_get		);
DECLARE_DO_FUN(	do_give		);
DECLARE_DO_FUN( do_gossip	);
DECLARE_DO_FUN(	do_goto		);
DECLARE_DO_FUN( do_grats	);
DECLARE_DO_FUN(	do_group	);
DECLARE_DO_FUN(	do_gtell	);
DECLARE_DO_FUN( do_guard        );
DECLARE_DO_FUN( do_heal		);
DECLARE_DO_FUN(	do_help		);
DECLARE_DO_FUN(	do_background	);
DECLARE_DO_FUN( do_herbs        );
DECLARE_DO_FUN(	do_hide		);
DECLARE_DO_FUN(	do_hide_obj	);
DECLARE_DO_FUN(	do_holylight	);
DECLARE_DO_FUN(	do_idea		);
DECLARE_DO_FUN(	do_immtalk	);
DECLARE_DO_FUN( do_incognito	);
DECLARE_DO_FUN( do_imotd	);
DECLARE_DO_FUN(	do_inventory	);
DECLARE_DO_FUN(	do_invis	);
DECLARE_DO_FUN(	do_kick		);
DECLARE_DO_FUN(	do_kill		);
DECLARE_DO_FUN(	do_newkill      );
DECLARE_DO_FUN(	do_list		);
DECLARE_DO_FUN( do_load		);
DECLARE_DO_FUN(	do_lock		);
DECLARE_DO_FUN(	do_log		);
DECLARE_DO_FUN(	do_look		);
DECLARE_DO_FUN(	do_lore		);
DECLARE_DO_FUN( do_map		);
DECLARE_DO_FUN( do_smallmap		);
DECLARE_DO_FUN( set_wilderness	);
DECLARE_DO_FUN( do_set_wilderness_all	);
DECLARE_DO_FUN(	do_memory	);
DECLARE_DO_FUN(	do_makelist	);
DECLARE_DO_FUN(	do_mfind	);
DECLARE_DO_FUN(	do_mload	);
DECLARE_DO_FUN( do_mob         );               
DECLARE_DO_FUN( do_mpstat      );
DECLARE_DO_FUN( do_mpdump      );
DECLARE_DO_FUN(	do_mset		);
DECLARE_DO_FUN( do_msp		);
DECLARE_DO_FUN(	do_mstat	);
DECLARE_DO_FUN(	do_mwhere	); 
DECLARE_DO_FUN( do_motd		);
DECLARE_DO_FUN( do_mount        );
DECLARE_DO_FUN(	do_murde	);
DECLARE_DO_FUN(	do_murder	);
DECLARE_DO_FUN( do_nerve        );
DECLARE_DO_FUN( do_music	);
DECLARE_DO_FUN( do_newlock	);
DECLARE_DO_FUN( do_news		);
DECLARE_DO_FUN( do_nochannels	);
DECLARE_DO_FUN( do_notick	);
DECLARE_DO_FUN(	do_noemote	);
DECLARE_DO_FUN( do_nofollow	);
DECLARE_DO_FUN( do_noloot	);
DECLARE_DO_FUN(	do_north	);
DECLARE_DO_FUN(	do_noshout	);
DECLARE_DO_FUN( do_nosummon	);
DECLARE_DO_FUN(	do_note		);
DECLARE_DO_FUN(	do_notell	);
DECLARE_DO_FUN(	do_objdump	);
DECLARE_DO_FUN(	do_ofind	);
DECLARE_DO_FUN(	do_oload	);
DECLARE_DO_FUN(	do_open		);
DECLARE_DO_FUN(	do_order	);
DECLARE_DO_FUN(	do_oset		);
DECLARE_DO_FUN(	do_ostat	);
DECLARE_DO_FUN( do_outfit	);
DECLARE_DO_FUN( do_owhere	);
DECLARE_DO_FUN(	do_pardon	);
DECLARE_DO_FUN(	do_password	);
DECLARE_DO_FUN(	do_peace	);
DECLARE_DO_FUN( do_pecho	);
DECLARE_DO_FUN( do_penalty	);
DECLARE_DO_FUN( do_permban	);
DECLARE_DO_FUN(	do_pick		);
DECLARE_DO_FUN( do_pretitle	);
DECLARE_DO_FUN(	do_printmap	);
DECLARE_DO_FUN( do_rename	);
DECLARE_DO_FUN(	do_relock	);
/* DECLARE_DO_FUN( do_play		);  */
DECLARE_DO_FUN( do_popularity	);
DECLARE_DO_FUN( do_pmote	);
DECLARE_DO_FUN(	do_pose		);
DECLARE_DO_FUN( do_pour		);
DECLARE_DO_FUN(	do_practice	);
DECLARE_DO_FUN( do_prefi	);
DECLARE_DO_FUN( do_prefix	);
DECLARE_DO_FUN( do_prompt	);
DECLARE_DO_FUN( do_protect	);
DECLARE_DO_FUN( do_guild	);
DECLARE_DO_FUN(	do_purge	);
DECLARE_DO_FUN(	do_put		);
DECLARE_DO_FUN(	do_quaff	);
DECLARE_DO_FUN(	do_ooc	);
DECLARE_DO_FUN(	do_qui		);
DECLARE_DO_FUN( do_quiet	);
DECLARE_DO_FUN(	do_quit		);
DECLARE_DO_FUN( do_quote	);
DECLARE_DO_FUN( do_read		);
DECLARE_DO_FUN(	do_reboo	);
DECLARE_DO_FUN(	do_reboot	);
DECLARE_DO_FUN(	do_recall	);
DECLARE_DO_FUN(	do_recho	);
DECLARE_DO_FUN(	do_recite	);
DECLARE_DO_FUN(	do_remove	);
DECLARE_DO_FUN(	do_rent		);
DECLARE_DO_FUN( do_replay	);
DECLARE_DO_FUN(	do_reply	);
DECLARE_DO_FUN(	do_report	);
DECLARE_DO_FUN(	do_request	);
DECLARE_DO_FUN(	do_rescue	);
DECLARE_DO_FUN( do_resistance   );
DECLARE_DO_FUN(	do_rest		);
DECLARE_DO_FUN(	do_restore	);
DECLARE_DO_FUN(	do_return	);
DECLARE_DO_FUN(	do_rset		);
DECLARE_DO_FUN(	do_rstat	);
DECLARE_DO_FUN( do_rules	);
DECLARE_DO_FUN(	do_sacrifice	);
DECLARE_DO_FUN(	do_save		);
DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN(	do_new_say	);
DECLARE_DO_FUN(	do_scan		);
DECLARE_DO_FUN(	do_score	);
DECLARE_DO_FUN( do_scroll	);
DECLARE_DO_FUN(	do_search	);
DECLARE_DO_FUN(	do_sell		);
DECLARE_DO_FUN( do_set		);
DECLARE_DO_FUN(	do_shout	);
DECLARE_DO_FUN( do_show		);
DECLARE_DO_FUN(	do_shutdow	);
DECLARE_DO_FUN(	do_shutdown	);
DECLARE_DO_FUN( do_sing		);
DECLARE_DO_FUN( do_sit		);
DECLARE_DO_FUN( do_skills	);
DECLARE_DO_FUN(	do_sla		);
DECLARE_DO_FUN(	do_slay		);
DECLARE_DO_FUN(	do_sleep	);
DECLARE_DO_FUN(	do_slookup	);
DECLARE_DO_FUN( do_smote	);
DECLARE_DO_FUN(	do_sneak	);
DECLARE_DO_FUN(	do_snoop	);
DECLARE_DO_FUN( do_socials	);
DECLARE_DO_FUN(	do_south	);
DECLARE_DO_FUN( do_sockets	);
DECLARE_DO_FUN( do_spellbane    );
DECLARE_DO_FUN( do_spells	);
DECLARE_DO_FUN(	do_split	);
DECLARE_DO_FUN(	do_sset		);
DECLARE_DO_FUN(	do_stand	);
DECLARE_DO_FUN( do_stat		);
DECLARE_DO_FUN( do_limited	);
DECLARE_DO_FUN(	do_steal	);
DECLARE_DO_FUN( do_story	);
DECLARE_DO_FUN( do_strangle     );
DECLARE_DO_FUN( do_string	);
DECLARE_DO_FUN(        do_surrender    );     
DECLARE_DO_FUN(	do_switch	);
DECLARE_DO_FUN( do_tame         );
DECLARE_DO_FUN(	do_tell		);
DECLARE_DO_FUN( do_throw        );
DECLARE_DO_FUN( do_tick         );
DECLARE_DO_FUN(	do_time		);
DECLARE_DO_FUN(	do_title	);
DECLARE_DO_FUN(	do_track	);
DECLARE_DO_FUN(	do_train	);
DECLARE_DO_FUN(	do_transfer	);
DECLARE_DO_FUN( do_trip		);
DECLARE_DO_FUN( do_trophy       );
DECLARE_DO_FUN(	do_trust	);
DECLARE_DO_FUN( do_truesight    );
DECLARE_DO_FUN(	do_typo		);
DECLARE_DO_FUN( do_unalias	);
DECLARE_DO_FUN(	do_unlock	);
DECLARE_DO_FUN(	do_up		);
DECLARE_DO_FUN(	do_value	);
DECLARE_DO_FUN(	do_visible	);
DECLARE_DO_FUN( do_violate	);
DECLARE_DO_FUN( do_vlist	);
DECLARE_DO_FUN( do_vnum		);
DECLARE_DO_FUN(	do_wake		);
DECLARE_DO_FUN( do_wanted       );
DECLARE_DO_FUN( do_detect_hidden);
DECLARE_DO_FUN( do_bear_call    );
DECLARE_DO_FUN(	do_wear		);
DECLARE_DO_FUN(	do_second	); 
DECLARE_DO_FUN(	do_weather	);
DECLARE_DO_FUN(	do_west		);
DECLARE_DO_FUN(	do_where	);
DECLARE_DO_FUN(	do_who		);
DECLARE_DO_FUN(	do_whoami	);
DECLARE_DO_FUN( do_whois	);
DECLARE_DO_FUN(	do_wimpy	);
DECLARE_DO_FUN(	do_withdraw	);
DECLARE_DO_FUN(	do_wizhelp	);
DECLARE_DO_FUN(	do_wizlock	);
DECLARE_DO_FUN( do_wizlist	);
DECLARE_DO_FUN( do_wiznet	);
DECLARE_DO_FUN( do_worth	);
DECLARE_DO_FUN(	do_yell		);
DECLARE_DO_FUN(	do_narrate	);
DECLARE_DO_FUN(	do_pray		);
DECLARE_DO_FUN(	do_gd		);
DECLARE_DO_FUN(	do_zap		);
DECLARE_DO_FUN(	do_zecho	);
DECLARE_DO_FUN( do_olc		);
DECLARE_DO_FUN( do_asave	);
DECLARE_DO_FUN( do_alist	);
DECLARE_DO_FUN( do_resets	);
DECLARE_DO_FUN( do_redit	);
DECLARE_DO_FUN( do_aedit	);
DECLARE_DO_FUN( do_medit	);
DECLARE_DO_FUN( do_oedit	);
DECLARE_DO_FUN( do_kingdom_scan	);
DECLARE_DO_FUN( do_smite	);
DECLARE_DO_FUN( do_acute_vision	);
DECLARE_DO_FUN( do_vanish	);
DECLARE_DO_FUN( do_barkskin	);
DECLARE_DO_FUN(	do_dark_vision	);
DECLARE_DO_FUN(	do_sharpen	);
DECLARE_DO_FUN(	do_blindness	);
DECLARE_DO_FUN(	do_poison	);
DECLARE_DO_FUN(	do_charge	);
DECLARE_DO_FUN(	do_bounty	);
DECLARE_DO_FUN(	do_hometown	);
DECLARE_DO_FUN(	do_badname	);
DECLARE_DO_FUN(	do_whirlwind	);
DECLARE_DO_FUN( do_traplist     );
DECLARE_DO_FUN( do_anon_shout   );
DECLARE_DO_FUN( do_grant        );
DECLARE_DO_FUN( do_cgrant       );
DECLARE_DO_FUN( do_ggrant       );
DECLARE_DO_FUN( do_sgrant       );
DECLARE_DO_FUN( do_revoke       );
DECLARE_DO_FUN( do_disable      );
DECLARE_DO_FUN( do_seize	);
DECLARE_DO_FUN( do_copyover	);
DECLARE_DO_FUN( do_mlevel	);
DECLARE_DO_FUN( do_olevel	);
DECLARE_DO_FUN( do_desocket	);
DECLARE_DO_FUN( do_immtitle	);
DECLARE_DO_FUN( do_repair	);
DECLARE_DO_FUN( do_estimate	);
DECLARE_DO_FUN( do_affrooms	);
DECLARE_DO_FUN( do_noquit	);
DECLARE_DO_FUN( do_slist	);
DECLARE_DO_FUN( do_glist	);
DECLARE_DO_FUN( do_slook	);
DECLARE_DO_FUN( do_finger	);
DECLARE_DO_FUN( do_donate	);
DECLARE_DO_FUN( do_push		);
DECLARE_DO_FUN( do_pload	);
DECLARE_DO_FUN( do_punload	);
DECLARE_DO_FUN( do_setskill	);
DECLARE_DO_FUN( do_hunt		);
DECLARE_DO_FUN( do_shoot	);
DECLARE_DO_FUN( do_smithing	);
DECLARE_DO_FUN( do_wrlist	);
DECLARE_DO_FUN( do_drag		);
DECLARE_DO_FUN( do_vt		);
DECLARE_DO_FUN( do_camp		);
DECLARE_DO_FUN( do_layhands	);
DECLARE_DO_FUN( do_bandage	);
DECLARE_DO_FUN( do_raffects	);
DECLARE_DO_FUN( do_startwar	);
DECLARE_DO_FUN( do_check	);
DECLARE_DO_FUN( do_bonus	);
DECLARE_DO_FUN( do_nonote	);
DECLARE_DO_FUN( do_builder	);
DECLARE_DO_FUN( do_cpassword	);
DECLARE_DO_FUN( do_allow 	);
DECLARE_DO_FUN( do_job		);
DECLARE_DO_FUN( do_quest	);
DECLARE_DO_FUN( do_questch	);
DECLARE_DO_FUN( do_newbie	);
DECLARE_DO_FUN( do_resetarena   );
DECLARE_DO_FUN( do_mpedit      );
DECLARE_DO_FUN( do_hedit       );
DECLARE_DO_FUN( do_balefir	);
DECLARE_DO_FUN( do_balefire	);
DECLARE_DO_FUN( do_afk		);
DECLARE_DO_FUN( do_openvlist       );
DECLARE_DO_FUN( do_war		);
DECLARE_DO_FUN( do_arenaout	);
DECLARE_DO_FUN( do_warcry       );
DECLARE_DO_FUN( do_sedit	);
DECLARE_DO_FUN( do_multiburst	);
DECLARE_DO_FUN( do_questinfo	);
DECLARE_DO_FUN( do_locktitle	);
DECLARE_DO_FUN( do_music	);
DECLARE_DO_FUN( do_lockon	);
DECLARE_DO_FUN( do_sitelock	);
DECLARE_DO_FUN( do_illegal	);
DECLARE_DO_FUN( do_ftag 	);
DECLARE_DO_FUN( do_tag 		);
DECLARE_DO_FUN( do_red 		);
DECLARE_DO_FUN( do_blue		);
DECLARE_DO_FUN( do_info		);
DECLARE_DO_FUN( do_kgrant	);
DECLARE_DO_FUN( do_pkban	); /*Kazmir Add 14/03/00*/
DECLARE_DO_FUN( do_tbh   );  
DECLARE_DO_FUN( do_new_grant	); 
DECLARE_DO_FUN( do_new_revoke   );
DECLARE_DO_FUN( do_fvlist       );
DECLARE_DO_FUN( do_avator       );
DECLARE_DO_FUN( do_omni		);
DECLARE_DO_FUN( do_marry	);
DECLARE_DO_FUN( do_divorce	);
DECLARE_DO_FUN( do_bagsteal	);
DECLARE_DO_FUN( do_bagpeek	);
DECLARE_DO_FUN( do_mlimited	);
DECLARE_DO_FUN( do_skill_info	);
DECLARE_DO_FUN( do_uvlist	);
DECLARE_DO_FUN( do_mode_icooc	);
DECLARE_DO_FUN( do_ofb	        );
DECLARE_DO_FUN( do_crestored    );
DECLARE_DO_FUN( do_doas         );
DECLARE_DO_FUN( do_vild         );
DECLARE_DO_FUN( do_mk         );