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
#include <time.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "fok_struct.h"

#define MOB_STAT_RESTING        2
#define MOB_STAT_WAITING_ANSWER 3
#define MOB_STAT_BEATING_SWORD  6
#define MOB_STAT_HEATING_SWORD  7
#define MOB_STAT_COOLING_SWORD  8
#define MOB_STAT_HANGING_SWORD  9

#define MOB_STAT_WAITING_SERVANT 10
#define MOB_STAT_SERVANT_LATE   11
#define MOB_STAT_MAKE_SWORD     12
#define MOB_STAT_MAKE_MACE      13
#define MOB_STAT_MAKE_DAGGER    14
#define MOB_STAT_WEAPON_READY   15

#define MOB_STAT_SERV_STOP      6
#define MOB_STAT_SERV_GO        7

/* time values are in seconds of real time */
#define OBJ_VNUM_WEAPONSMITH_SWORD1  3021
#define OBJ_VNUM_WEAPONSMITH_SWORD2  3022
#define WEAPONSMITH_NEEDS_IRON_TIME  30

/* command procedures needed */
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_open		);
DECLARE_DO_FUN(do_close		);
DECLARE_DO_FUN(do_backstab	);
DECLARE_DO_FUN(do_flee		);
DECLARE_DO_FUN(do_tell		);
DECLARE_DO_FUN(do_gd		);
DECLARE_DO_FUN(do_track		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_kill		);
DECLARE_DO_FUN(do_unlock	);
DECLARE_DO_FUN(do_lock		);
DECLARE_DO_FUN(do_drop		);
DECLARE_DO_FUN(do_get		);
DECLARE_DO_FUN(do_north		);
DECLARE_DO_FUN(do_south		);
DECLARE_DO_FUN(do_east		);
DECLARE_DO_FUN(do_west		);
DECLARE_DO_FUN(do_sacrifice	);
DECLARE_DO_FUN(do_give		);
DECLARE_DO_FUN(do_rescue	);

DECLARE_MPROG_FUN_DEATH( death_prog_servant );

/*
 * The following special functions are available for mobiles.
 */

/* : quest code added, one line, more throughout file */ 
DECLARE_SPEC_FUN(       spec_questmaster        );   /* Vassago */
/*  end */ 
DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_frost	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_adept		);
DECLARE_SPEC_FUN(	spec_cast_cleric	);
DECLARE_SPEC_FUN(	spec_cast_judge		);
DECLARE_SPEC_FUN(	spec_cast_mage		);
DECLARE_SPEC_FUN(	spec_cast_seneschal	);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_executioner	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_thief		);
DECLARE_SPEC_FUN(	spec_nasty		);
DECLARE_SPEC_FUN(	spec_troll_member	);
DECLARE_SPEC_FUN(	spec_ogre_member	);
DECLARE_SPEC_FUN(	spec_patrolman		);
DECLARE_SPEC_FUN(       spec_slayer             );
DECLARE_SPEC_FUN(       spec_cast_kingdom       );
DECLARE_SPEC_FUN(       spec_special_guard      );
DECLARE_SPEC_FUN(       spec_stalker            );
DECLARE_SPEC_FUN(       spec_keeper1            );
DECLARE_SPEC_FUN(       spec_keeper2            );
DECLARE_SPEC_FUN(       spec_keeper3            );
DECLARE_SPEC_FUN(	spec_snake_charm	);
DECLARE_SPEC_FUN(       spec_servant            );
DECLARE_SPEC_FUN(       spec_weaponsmith        );
DECLARE_SPEC_FUN(       spec_repairman		);

/* the function table */
const   struct  spec_type    spec_table[] =
{
    { "spec_questmaster",       spec_questmaster        }, /* Vassago */
    {	"spec_breath_any",		spec_breath_any		},
    {	"spec_breath_acid",		spec_breath_acid	},
    {	"spec_breath_fire",		spec_breath_fire	},
    {	"spec_breath_frost",		spec_breath_frost	},
    {	"spec_breath_gas",		spec_breath_gas		},
    {	"spec_breath_lightning",	spec_breath_lightning	},	
    {	"spec_cast_adept",		spec_cast_adept		},
    {	"spec_cast_cleric",		spec_cast_cleric	},
    {	"spec_cast_judge",		spec_cast_judge		},
    {	"spec_cast_mage",		spec_cast_mage		},
    {	"spec_cast_seneschal",		spec_cast_seneschal	},
    {	"spec_cast_undead",		spec_cast_undead	},
    {	"spec_executioner",		spec_executioner	},
    {	"spec_fido",			spec_fido		},
    {	"spec_guard",			spec_guard		},
    {	"spec_janitor",			spec_janitor		},
    {	"spec_mayor",			spec_mayor		},
    {	"spec_poison",			spec_poison		},
    {	"spec_thief",			spec_thief		},
    {	"spec_nasty",			spec_nasty		},
    {	"spec_troll_member",		spec_troll_member	},
    {	"spec_ogre_member",		spec_ogre_member	},
    {	"spec_patrolman",		spec_patrolman		},
    {	"spec_slayer",			spec_slayer		},
    {	"spec_cast_kingdom",		spec_cast_kingdom	},
    {	"spec_keeper1",			spec_keeper1		},
    {	"spec_keeper2",			spec_keeper2		},
    {	"spec_keeper3",			spec_keeper3		},
    {	"spec_stalker",			spec_stalker		},
    {	"spec_special_guard",		spec_special_guard	},
    {	"spec_snake_charm",		spec_snake_charm	},
    {   "spec_repairman",		spec_repairman		},  
    {	"spec_servant",			spec_servant		},
    {	"spec_weaponsmith",		spec_weaponsmith	},
    {	NULL,				NULL			}
};

/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup( const char *name )
{
   int i;
 
   for ( i = 0; spec_table[i].name != NULL; i++)
   {
        if (LOWER(name[0]) == LOWER(spec_table[i].name[0])
        &&  !str_prefix( name,spec_table[i].name))
            return spec_table[i].function;
   }
 
    return 0;
}

char *spec_name( SPEC_FUN *function)
{
    int i;

    for (i = 0; spec_table[i].function != NULL; i++)
    {
	if (function == spec_table[i].function)
	    return spec_table[i].name;
    }

    return NULL;
}


bool spec_questmaster (CHAR_DATA *ch)
{
    if (ch->fighting != NULL) return spec_cast_mage( ch );
    return FALSE;
}

bool spec_troll_member( CHAR_DATA *ch)
{
    CHAR_DATA *vch, *victim = NULL;
    int count = 0;
    char *message;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL 
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
	return FALSE;

    /* find an ogre to beat up */
    for (vch = ch->in_room->people;  vch != NULL;  vch = vch->next_in_room)
    {
	if (!IS_NPC(vch) || ch == vch)
	    continue;

	if (vch->pIndexData->vnum == MOB_VNUM_PATROLMAN)
	    return FALSE;

	if (vch->pIndexData->group == GROUP_VNUM_OGRES
	&&  ch->level > vch->level - 2 && !is_safe(ch,vch))
	{
	    if (number_range(0,count) == 0)
		victim = vch;

	    count++;
	}
    }

    if (victim == NULL)
	return FALSE;

    /* say something, then raise hell */
    switch (number_range(0,6))
    {
	default:  message = NULL; 	break;
	case 0:	message = "$n yells 'I've been looking for you, punk!'";
		break;
	case 1: message = "With a scream of rage, $n attacks $N.";
		break;
	case 2: message = 
		"$n says 'What's slimy Ogre trash like you doing around here?'";
		break;
	case 3: message = "$n cracks his knuckles and says 'Do ya feel lucky?'";
		break;
	case 4: message = "$n says 'There's no cops to save you this time!'";
		break;	
	case 5: message = "$n says 'Time to join your brother, spud.'";
		break;
	case 6: message = "$n says 'Let's rock.'";
		break;
    }

    if (message != NULL)
    	act(message,ch,NULL,victim,TO_ALL);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}

bool spec_ogre_member( CHAR_DATA *ch)
{
    CHAR_DATA *vch, *victim = NULL;
    int count = 0;
    char *message;
 
    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
        return FALSE;

    /* find an troll to beat up */
    for (vch = ch->in_room->people;  vch != NULL;  vch = vch->next_in_room)
    {
        if (!IS_NPC(vch) || ch == vch)
            continue;
 
        if (vch->pIndexData->vnum == MOB_VNUM_PATROLMAN)
            return FALSE;
 
        if (vch->pIndexData->group == GROUP_VNUM_TROLLS
        &&  ch->level > vch->level - 2 && !is_safe(ch,vch))
        {
            if (number_range(0,count) == 0)
                victim = vch;
 
            count++;
        }
    }
 
    if (victim == NULL)
        return FALSE;
 
    /* say something, then raise hell */
    switch (number_range(0,6))
    {
	default: message = NULL;	break;
        case 0: message = "$n yells 'I've been looking for you, punk!'";
                break;
        case 1: message = "With a scream of rage, $n attacks $N.'";
                break;
        case 2: message =
                "$n says 'What's Troll filth like you doing around here?'";
                break;
        case 3: message = "$n cracks his knuckles and says 'Do ya feel lucky?'";
                break;
        case 4: message = "$n says 'There's no cops to save you this time!'";
                break;
        case 5: message = "$n says 'Time to join your brother, spud.'";
                break;
        case 6: message = "$n says 'Let's rock.'";
                break;
    }
 
    if (message != NULL)
    	act(message,ch,NULL,victim,TO_ALL);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}

bool spec_patrolman(CHAR_DATA *ch)
{
    CHAR_DATA *vch,*victim = NULL;
    OBJ_DATA *obj;
    char *message;
    int count = 0;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
        return FALSE;

    /* look for a fight in the room */
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch == ch)
	    continue;

	if (vch->fighting != NULL)  /* break it up! */
	{
	    if (number_range(0,count) == 0)
	        victim = (vch->level > vch->fighting->level) 
		    ? vch : vch->fighting;
	    count++;
	}
    }

    if (victim == NULL || (IS_NPC(victim) && victim->spec_fun == ch->spec_fun))
	return FALSE;

    if (((obj = get_eq_char(ch,WEAR_NECK_1)) != NULL 
    &&   obj->pIndexData->vnum == OBJ_VNUM_WHISTLE)
    ||  ((obj = get_eq_char(ch,WEAR_NECK_2)) != NULL
    &&   obj->pIndexData->vnum == OBJ_VNUM_WHISTLE))
    {
	act("You blow down hard on $p.",ch,obj,NULL,TO_CHAR);
	act("$n blows on $p, ***WHEEEEEEEEEEEET***",ch,obj,NULL,TO_ROOM);

    	for ( vch = char_list; vch != NULL; vch = vch->next )
    	{
            if ( vch->in_room == NULL )
            	continue;

            if (vch->in_room != ch->in_room 
	    &&  vch->in_room->area == ch->in_room->area)
            	send_to_char( "You hear a shrill whistling sound.\n\r", vch );
    	}
    }

    switch (number_range(0,6))
    {
	default:	message = NULL;		break;
	case 0:	message = "$n yells 'All roit! All roit! break it up!'";
		break;
	case 1: message = 
		"$n says 'Society's to blame, but what's a bloke to do?'";
		break;
	case 2: message = 
		"$n mumbles 'bloody kids will be the death of us all.'";
		break;
	case 3: message = "$n shouts 'Stop that! Stop that!' and attacks.";
		break;
	case 4: message = "$n pulls out his billy and goes to work.";
		break;
	case 5: message = 
		"$n sighs in resignation and proceeds to break up the fight.";
		break;
	case 6: message = "$n says 'Settle down, you hooligans!'";
		break;
    }

    if (message != NULL)
	act(message,ch,NULL,NULL,TO_ALL);

    multi_hit(ch,victim,TYPE_UNDEFINED);

    return TRUE;
}

/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, char *spell_name )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 3 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( spell_name ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TARGET_CHAR);
    return TRUE;
}



/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch )
{
    if ( ch->position != POS_FIGHTING )
	return FALSE;

    switch ( number_bits( 3 ) )
    {
    case 0: return spec_breath_fire		( ch );
    case 1:
    case 2: return spec_breath_lightning	( ch );
    case 3: return spec_breath_gas		( ch );
    case 4: return spec_breath_acid		( ch );
    case 5:
    case 6:
    case 7: return spec_breath_frost		( ch );
    }

    return FALSE;
}



bool spec_breath_acid( CHAR_DATA *ch )
{
    return dragon( ch, "acid breath" );
}



bool spec_breath_fire( CHAR_DATA *ch )
{
    return dragon( ch, "fire breath" );
}



bool spec_breath_frost( CHAR_DATA *ch )
{
    return dragon( ch, "frost breath" );
}



bool spec_breath_gas( CHAR_DATA *ch )
{
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL,TARGET_CHAR);
    return TRUE;
}



bool spec_breath_lightning( CHAR_DATA *ch )
{
    return dragon( ch, "lightning breath" );
}



bool spec_cast_adept( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 
	     && !IS_NPC(victim) && victim->level < 11)
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    switch ( number_bits( 4 ) )
    {
    case 0:
	act( "$n utters the word 'abrazak'.", ch, NULL, NULL, TO_ROOM );
	spell_armor( skill_lookup( "armor" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 1:
	act( "$n utters the word 'fido'.", ch, NULL, NULL, TO_ROOM );
	spell_bless( skill_lookup( "bless" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 2:
	act("$n utters the words 'judicandus noselacri'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_blindness( skill_lookup( "cure blindness" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 3:
	act("$n utters the words 'judicandus dies'.", ch,NULL, NULL, TO_ROOM );
	spell_cure_light( skill_lookup( "cure light" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 4:
	act( "$n utters the words 'judicandus sausabru'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_poison( skill_lookup( "cure poison" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 5:
	act("$n utters the word 'candusima'.", ch, NULL, NULL, TO_ROOM );
	spell_refresh( skill_lookup("refresh"),ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 6:
	act("$n utters the words 'judicandus eugzagz'.",ch,NULL,NULL,TO_ROOM);
	spell_cure_disease(skill_lookup("cure disease"),
	    ch->level,ch,victim,TARGET_CHAR);
    }

    return FALSE;
}


bool spec_cast_cleric( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "cause serious";  break;
//      case  2: min_level =  7; spell = "earthquake";     break;       
	case  2: 
	case  3: min_level =  9; spell = "cause critical"; break;
	case  4: min_level = 10; spell = "dispel evil";    break;
	case  5: min_level = 12; spell = "curse";          break;
	case  6: min_level = 12; spell = "change sex";     break;
	case  7: min_level = 13; spell = "flamestrike";    break;
	case  8:
	case  9:
	case 10: min_level = 15; spell = "harm";           break;
	case 11: min_level = 15; spell = "plague";	   break;
	default: min_level = 16; spell = "dispel magic";   break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}

bool spec_cast_judge( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
 
    if ( ch->position != POS_FIGHTING )
        return FALSE;
 
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
        v_next = victim->next_in_room;
        if ( victim->fighting == ch && number_bits( 2 ) == 0 )
            break;
    }
 
    if ( victim == NULL )
        return FALSE;
 
    spell = "high explosive";
    if ( ( sn = skill_lookup( spell ) ) < 0 )
        return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_cast_mage( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "chill touch";    break;
	case  2: min_level =  7; spell = "weaken";         break;
	case  3: min_level =  8; spell = "teleport";       break;
	case  4: min_level = 11; spell = "colour spray";   break;
	case  5: min_level = 12; spell = "change sex";     break;
	case  6: min_level = 13; spell = "energy drain";   break;
	case  7:
	case  8:
	case  9: min_level = 15; spell = "fireball";       break;
	case 10: min_level = 20; spell = "plague";	   break;
	default: min_level = 20; spell = "acid blast";     break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}

bool spec_cast_seneschal( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
/*    char buf[160];*/

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 1 ) == 0 )
	    break;
    }


    if ( victim == NULL )
	return FALSE;

        switch ( dice(1,16) )
	{
	case  0: spell = "blindness";      break;
	case  1: spell = "chill touch";    break;
	case  2: spell = "weaken";         break;
	case  3: spell = "blindness";      break;
	case  4: spell = "colour spray";   break;
	case  5: spell = "acid blast";     break;
	case  6: spell = "energy drain";   break;
	case  7:
	case  8:
	case  9: spell = "fireball";       break;
	case 10: spell = "plague";	   break;
	case 11: spell = "poison";         break;
	case 12: spell = "burning hands";  break;
	default: spell = "";     break;
	}

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_cast_undead( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";          break;
	case  1: min_level =  3; spell = "weaken";         break;
	case  2: min_level =  6; spell = "chill touch";    break;
	case  3: min_level =  9; spell = "blindness";      break;
	case  4: min_level = 12; spell = "poison";         break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";           break;
	case  7: min_level = 21; spell = "teleport";       break;
	case  8: min_level = 20; spell = "plague";	   break;
	default: min_level = 18; spell = "harm";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_executioner( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    crime = "";
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER) 
	&&   can_see(ch,victim))
	    { crime = "KILLER"; break; }

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF) 
	&&   can_see(ch,victim))
	    { crime = "THIEF"; break; }

/*
	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED))
	 { 
	  if((victim->kingdom > 0) && victim->kingdom != ch->kingdom) 
	   {
	    crime = "CRIMINAL"; break; 
	   }
	    else if(victim->kingdom == 0) 
	     {
	       crime = "CRIMINAL"; break; 
	     }
	} 
*/
    }

    if ( victim == NULL )
	return FALSE;

    sprintf( buf, "%s the %s is over here.",
	victim->name, crime );
    REMOVE_BIT(ch->comm,COMM_NOSHOUT);
    do_yell( ch, buf );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}

			

bool spec_fido( CHAR_DATA *ch )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE_NPC )
	    continue;

   if(corpse->item_type == ITEM_CORPSE_NPC) {
	act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM );
	for ( obj = corpse->contains; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}
	extract_obj( corpse );
	return TRUE;
    }
	}
    return FALSE;
}


bool spec_janitor( CHAR_DATA *ch )
{
    OBJ_DATA *trash;
    OBJ_DATA *trash_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( trash = ch->in_room->contents; trash != NULL; trash = trash_next )
    {
	trash_next = trash->next_content;
	if ( !IS_SET( trash->wear_flags, ITEM_TAKE ) || !can_loot(ch,trash))
	    continue;
	if ( trash->item_type == ITEM_DRINK_CON
	||   trash->item_type == ITEM_TRASH
	||   trash->cost < 10 )
	{
   if(trash->item_type != ITEM_CORPSE_NPC) {
	    act( "$n picks up some trash.", ch, NULL, NULL, TO_ROOM );
	    obj_from_room( trash );
	    obj_to_char( trash, ch );
	    return TRUE;
	}
    }
 }
    return FALSE;
}



bool spec_mayor( CHAR_DATA *ch )
{
    static const char open_path[] =
	"W3a3003b000c000d111Oe333333Oe22c222112212111a1S.";

    static const char close_path[] =
	"W3a3003b000c000d111CE333333CE22c222112212111a1S.";

    static const char *path;
    static int pos;
    static bool move;
    OBJ_DATA *key;

    if ( !move )
    {
	if ( time_info.hour ==  6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0', FALSE );
	break;

    case 'W':
	ch->position = POS_STANDING;
	act( "$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
	act( "$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'a':
	do_say( ch, "Hello Honey!" );
	break;

    case 'b':
	do_say(ch, "What a view!  I must do something about that dump!");
	break;

    case 'c':
	do_say(ch,"Vandals, Youngsters have no respect for anything!");
	break;

    case 'd':
	do_say(ch,"Good day, citizens!");
	break;

    case 'e':
	do_say(ch,"A new day dawns for Sanaria");
	break;

    case 'E':
	do_say(ch,"Lord Judis maintains justice.. another day can end");
	break;

    case 'O':
	do_unlock( ch, "gate" );
	do_open( ch, "gate" );
	interpret( ch, "emote unlocks the gate key from the gate.");
	for( key=ch->in_room->contents; key!=NULL; key=key->next_content )
	  if ( key->pIndexData->vnum == 3379 )
	    break;
	if ( key != NULL )
	  SET_BIT( key->wear_flags, ITEM_TAKE );
	do_get( ch, "gatekey" );
	break;

    case 'C':
	do_close( ch, "gate" );
	do_lock( ch, "gate" );
	do_drop( ch, "key" );
	interpret( ch, "emote locks the gate key to the gate, with chain.");
	for( key=ch->in_room->contents; key!=NULL; key=key->next_content )
	  if ( key->pIndexData->vnum == 3379 )
	    break;
	if ( key != NULL )
	  REMOVE_BIT( key->wear_flags, ITEM_TAKE );
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
    || ( victim = ch->fighting ) == NULL
    ||   number_percent( ) > 2 * ch->level )
	return FALSE;

    act( "You bite $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n bites $N!",  ch, NULL, victim, TO_NOTVICT );
    act( "$n bites you!", ch, NULL, victim, TO_VICT    );
    spell_poison( gsn_poison, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}

bool spec_arena( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
    || ( victim = ch->fighting ) == NULL
    ||   number_percent( ) > 2 * ch->level )
	return FALSE;

    act( "You bite $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n bites $N!",  ch, NULL, victim, TO_NOTVICT );
    act( "$n bites you!", ch, NULL, victim, TO_VICT    );
    spell_poison( gsn_poison, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    long gold,silver;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( IS_NPC(victim)
	||   victim->level >= LEVEL_IMMORTAL
	||   number_bits( 5 ) != 0 
	||   !can_see(ch,victim))
	    continue;

	if ( IS_AWAKE(victim) && number_range( 0, ch->level ) == 0 )
	{
	    act( "You discover $n's hands in your wallet!",
		ch, NULL, victim, TO_VICT );
	    act( "$N discovers $n's hands in $S wallet!",
		ch, NULL, victim, TO_NOTVICT );
	    return TRUE;
	}
	else
	{
	    gold = victim->gold * UMIN(number_range(1,20),ch->level / 2) / 100;
	    gold = UMIN(gold, ch->level * ch->level * 10 );
	    ch->gold     += gold;
	    victim->gold -= gold;
	    silver = victim->silver * UMIN(number_range(1,20),ch->level/2)/100;
	    silver = UMIN(silver,ch->level*ch->level * 25);
	    ch->silver	+= silver;
	    victim->silver -= silver;
	    return TRUE;
	}
    }

    return FALSE;
}

bool spec_keeper_shadow( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;


    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	do_say( ch, "Greetings, dark one." );
	do_tell( ch, "ggolem Someone entered the shirine GGolem.");
	return TRUE;
    }

    return FALSE;

}



bool spec_cast_kingdom( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0)
	  break;
    }

    if ( victim == NULL )
	return FALSE;

	
    switch ( number_bits( 4 ) )
    {
    case 0:
	act( "$n utters the word 'abracal'.", ch, NULL, NULL, TO_ROOM );
	spell_armor( skill_lookup( "armor" ), ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 1:
	act( "$n utters the word 'balc'.", ch, NULL, NULL, TO_ROOM );
	spell_bless( skill_lookup( "bless" ), ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 2:
	act( "$n utters the word 'judicandus noselacba'.", ch, NULL, NULL, TO_ROOM );
	spell_cure_blindness( skill_lookup( "cure blindness" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 3:
	act( "$n utters the word 'judicandus bacla'.", ch, NULL, NULL,TO_ROOM);
	spell_cure_light( skill_lookup( "cure light" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 4:
	act( "$n utters the words 'judicandus sausabcla'.", 
	    ch, NULL, NULL, TO_ROOM );
	spell_cure_poison( skill_lookup( "cure poison" ),
	    ch->level, ch, victim, TARGET_CHAR );
	return TRUE;

    case 5:
	act( "$n utters the words 'candabala'.", ch, NULL, NULL, TO_ROOM );
	spell_refresh( skill_lookup( "refresh" ), ch->level, ch, victim, 
TARGET_CHAR );
	return TRUE;

    }

    return FALSE;
}


bool spec_slayer( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
      {
	v_next = victim->next_in_room;

	if (IS_NPC(victim) && victim->pIndexData->vnum == 3011 && 
	    can_see(ch,victim)) {
	  do_yell(ch, "I have come to reclaim my empire!");
	  do_kill(ch, "enforcer");
	  return TRUE;
	}
      }
    return FALSE;
  }


bool spec_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
    char *crime;
    int max_evil; 
 
    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
      return FALSE;
    
    max_evil = 300; 
    ech      = NULL;
    crime    = "";
    
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
      {
	v_next = victim->next_in_room;
	
	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER) )
	  { crime = "KILLER"; break; }
	
	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF) )
	  { crime = "THIEF"; break; }
/*	
	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED))
	 { 
	if((victim->kingdom > 0) && victim->kingdom != ch->kingdom) {
	crime = "CRIMINAL"; break; }
	else if(victim->kingdom == 0) {
	crime = "CRIMINAL"; break; }
	} 
*/	
	if ( victim->fighting != NULL
	    &&   victim->fighting != ch
	    &&   victim->alignment < max_evil )
	  {
	    if (IS_EVIL(victim))
	      {
		max_evil = -350; 
		ech      = victim;
	      }
	    else
	      ech = victim;
	  }
      }
    
    if ( victim != NULL )
      {
	sprintf( buf, "%s is a %s!  PROTECT THE INNOCENT!!  BANZAI!!",
		victim->name, crime );
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
      }
    
    if ( ech != NULL )
      {
	act( "$n screams 'PROTECT THE INNOCENT!!  BANZAI!!",
	    ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
      }
    
    return FALSE;
  }




bool spec_special_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    crime    = "";

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER) )
	    { crime = "KILLER"; break; }

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF) )
	    { crime = "THIEF"; break; }
/*
	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED))
	 { 
	if((victim->kingdom > 0) && victim->kingdom != ch->kingdom) {
	crime = "CRIMINAL"; break; }
	else if(victim->kingdom == 0) {
	crime = "CRIMINAL"; break; }
	}  */
    }

    if ( victim != NULL )
    {
	sprintf( buf, "%s is a %s!  PROTECT THE INNOCENT!!  BANZAI!!",
	    victim->name, crime );
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}

bool spec_stalker(CHAR_DATA *ch)
{
  char buf[100];
  CHAR_DATA *victim;
  CHAR_DATA *wch;
  CHAR_DATA *wch_next;
  int i;

  victim = ch->last_fought;

  if (ch->fighting != NULL)
    return FALSE;

  if (victim == NULL)
    {
      ch->kingdom = KINGDOM_XELENIA;
      do_gd(ch, "To their shame, my cowardly victim has left the land. I must leave also.");
      extract_char(ch, TRUE);
      return TRUE;
    }

  if (IS_GOOD(victim))
    i = 0;
  if (IS_EVIL(victim))
    i = 2;
  else
    i = 1;
  
  for (wch = ch->in_room->people; wch != NULL; wch = wch_next)
    wch_next = wch->next_in_room;
  
  if (victim == wch) {
    sprintf(buf,"%s, you criminal! Now you die!", victim->name);
    do_yell(ch,buf);
    multi_hit(ch,wch,TYPE_UNDEFINED);
    return TRUE;
  }
  do_track(ch, victim->name);

  if (ch->status == 5)
    {
      if (ch->in_room != 
get_room_index(hometown_table[victim->hometown].recall[1]))
	{
      	  char_from_room(ch);
      	  char_to_room(ch,
		   get_room_index(hometown_table[victim->hometown].recall[i]));
      	  do_track(ch, victim->name);
      	  return TRUE;
    	}
       else 
    	{
      	  ch->kingdom = KINGDOM_XELENIA;
      	  sprintf(buf, "To my shame I have lost track of %s.  I must leave.",
	      victim->name);
      	  do_gd(ch, buf);
      	  extract_char(ch, TRUE);
      	  return TRUE;
    	}
    }
  return FALSE;
}

bool spec_keeper1(CHAR_DATA *ch)
{
  if (ch->fighting != NULL)
    return FALSE;
  if (ch->position < POS_SLEEPING)
    return FALSE;

  if (time_info.hour == 18 && ch->status != 1)
    {
      act("$n closes up the shop for the night.", ch, NULL, NULL, TO_ROOM);
      move_char(ch, 4, FALSE);
      act("$n yawns and settles down for a few hours of sleep.", ch, NULL,
	NULL, TO_ROOM);
      ch->position = POS_SLEEPING;
      ch->status = 1;
      return TRUE;
    }
  else if (time_info.hour == 0 && ch->status != 0)
    {
      ch->position = POS_STANDING;
      act("$n wakes up and heads downstairs for work.",ch,NULL,NULL,TO_ROOM);
      move_char(ch, 5, FALSE);
      act("$n is open for business.", ch, NULL, NULL, TO_ROOM);
      ch->status = 0;
      return TRUE;
    }
  return FALSE;
}


bool spec_keeper2(CHAR_DATA *ch)
{
  if (ch->fighting != NULL)
    return FALSE;
  if (ch->position < POS_SLEEPING)
    return FALSE;

  if (time_info.hour == 23 && ch->status != 1)
    {
      act("$n closes up the shop for the night.", ch, NULL, NULL, TO_ROOM);
      move_char(ch, 4, FALSE);
      act("$n yawns and settles down for a few hours of sleep.", ch, NULL,
	NULL, TO_ROOM);
      ch->position = POS_SLEEPING;
      ch->status = 1;
      return TRUE;
    }
  else if (time_info.hour == 5 && ch->status != 0)
    {
      ch->position = POS_STANDING;
      act("$n wakes up and heads downstairs for work.",ch,NULL,NULL,TO_ROOM);
      move_char(ch, 5, FALSE);
      act("$n is open for business.", ch, NULL, NULL, TO_ROOM);
      ch->status = 0;
      return TRUE;
    }
  return FALSE;
}

bool spec_keeper3(CHAR_DATA *ch)
{
  if (ch->fighting != NULL)
    return FALSE;
  if (ch->position < POS_SLEEPING)
    return FALSE;

  if (time_info.hour == 19 && ch->status != 1)
    {
      act("$n closes up the shop for the night.", ch, NULL, NULL, TO_ROOM);
      move_char(ch, 4, FALSE);
      act("$n yawns and settles down for a few hours of sleep.", ch, NULL,
	NULL, TO_ROOM);
      ch->position = POS_SLEEPING;
      ch->status = 1;
      return TRUE;
    }
  else if (time_info.hour == 4 && ch->status != 0)
    {
      ch->position = POS_STANDING;
      act("$n wakes up and heads downstairs for work.",ch,NULL,NULL,TO_ROOM);
      move_char(ch, 5, FALSE);
      act("$n is open for business.", ch, NULL, NULL, TO_ROOM);
      ch->status = 0;
      return TRUE;
    }
  return FALSE;
}

/*  FROM NEW ROM    by: GG()  */

bool spec_nasty( CHAR_DATA *ch )
{
    CHAR_DATA *victim, *v_next;
    long gold;
 
    if (!IS_AWAKE(ch)) {
       return FALSE;
    }
 
    if (ch->position != POS_FIGHTING) {
       for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
       {
          v_next = victim->next_in_room;
          if (!IS_NPC(victim)
             && (victim->level > ch->level)
             && (victim->level < ch->level + 10))
          {
	     do_backstab(ch,victim->name);
             if (ch->position != POS_FIGHTING)
                 do_murder(ch,victim->name);
             /* should steal some coins right away? :) */
             return TRUE;
          }
       }
       return FALSE;    /*  No one to attack */
    }
 
    /* okay, we must be fighting.... steal some coins and flee */
    if ( (victim = ch->fighting) == NULL)
        return FALSE;   /* let's be paranoid.... */
 
    switch ( number_bits(2) )
    {
        case 0:  act( "$n rips apart your coin purse, spilling your gold!",
                     ch, NULL, victim, TO_VICT);
                 act( "You slash apart $N's coin purse and gather his gold.",
                     ch, NULL, victim, TO_CHAR);
                 act( "$N's coin purse is ripped apart!",
                     ch, NULL, victim, TO_NOTVICT);
                 gold = victim->gold / 10;  /* steal 10% of his gold */
                 victim->gold -= gold;
                 ch->gold     += gold;
                 return TRUE;
 
        case 1:  do_flee( ch, "");
                 return TRUE;
 
        default: return FALSE;
    }
}

/*     
 *        ** For quests **
 */

time_t last_time_weaponsmith; /* global for weaponsmith quest */


bool spec_servant(CHAR_DATA *mob)
{
/* news are directions
   G greet
   P pay
   T thank
   S pick up+smile
   W wave
   H greet weaponsmith
   D submit iron  or  tell about problem
   C check for dwarf
   A check for weaponsmith
*/

char action[] =  " nwwwwwwwnnnnwwsCGPTSWneesssseeeeeeesAHD	";
               /*01234567890123456789012345678901234567890123456        55*/
CHAR_DATA *ch;
static int point;
static int last_stat;
static bool asked;
static bool FAULT_D, FAULT_W;
char buf[160];
CHAR_DATA *ws;

  if ( mob->status == 0 )  {
     mob->status = MOB_STAT_SERV_STOP;
     last_stat = mob->status;
     point = 1;
     FAULT_D = FALSE;
     FAULT_W = FALSE;
     return FALSE;
  }
  if ( mob->fighting != NULL && mob->fighting->fighting == mob)  
  {
    for ( ws = mob->in_room->people; ws != NULL; ws = ws->next_in_room )
    {
       if ( IS_NPC(ws) && ws->pIndexData->vnum == 3003 )
           break;
    }
    if ( ws == NULL ) return(FALSE);
    do_say(mob, "Help me master!");
    do_rescue( ws, "novice" );
    do_rescue( ws, "novice" );
    do_rescue( ws, "novice" );
    return(FALSE);
  }

  switch(mob->status)  {
  case MOB_STAT_SERV_STOP:
    if ( (time_info.hour == 10) && !asked )  {
      do_say( mob, "Do you need anything, master?");
      asked = TRUE;
    }
    else if (time_info.hour != 10)
       asked = FALSE;
    last_stat = mob->status;
    break;
  case MOB_STAT_SERV_GO:
    if (last_stat != mob->status)  {
      point = 1;
      FAULT_W = FALSE;
      FAULT_D = FALSE;
    }
    sprintf(buf, "my command: %c", action[point] );
/*    do_say( mob, buf );      */
    switch( action[point] )  {
    case 'n':
      do_north( mob, "");
      break;
    case 's':
      do_south( mob, "");
      break;
    case 'e':
      do_east( mob, "");
      break;
    case 'w':
      do_west( mob, "");
      break;
    case 'G':
      do_say( mob, "Hi there!  I've came to buy the iron." );
      break;
    case 'C':
      FAULT_D = TRUE;
      for(ch=mob->in_room->people; ch!=NULL; ch=ch->next_in_room)  {
        if (!IS_NPC(ch) ) continue;
        if (ch->pIndexData->vnum == 3306) FAULT_D = FALSE;
      }
      if (FAULT_D)  {
        point += 5;  /* skip shopping */
        interpret( mob, "emote looks around, searching for someone.");
        do_say( mob, "Damn!  Where is he?");
      }
      break;
    case 'P':
      do_say( mob, "Here you are." );
      do_give( mob, "1000 silver seller" );
      break;
    case 'T':
      do_say( mob, "Thank you." );
      break;
    case 'S':
      interpret(mob, "smile");
      do_get( mob, "iron" );
      break;
    case 'W':
      interpret(mob, "wave");
      break;
    case 'H':
      if ( !FAULT_W )
	do_say(mob, "Hi master. I got the iron for you.");
      else
        do_say(mob, "Hi master. I rushed to the village, but he was not there.");
      break;
    case 'D':
      if (!FAULT_W)  {
        do_give(mob, "iron weaponsmith");
	mob->status = MOB_STAT_SERV_STOP;
        point = 0;
      }
      break;
    case 'A':
      FAULT_W = TRUE;
      if ( mob->in_room->vnum != 3011 )  {
        act("$n wanders on home.", mob, NULL, NULL, TO_ROOM);
/*        death_prog_servant( mob );
        extract_char( mob, TRUE ); */
	char_from_room( mob );
	act("$n leaves.", mob, NULL, NULL, TO_ROOM );
	char_to_room( mob, get_room_index( 3011 ) );
	act("$n has arrived.", mob, NULL, NULL, TO_ROOM );
        return TRUE;
      }
      for(ch=mob->in_room->people; ch!=NULL; ch=ch->next_in_room)  {
        if (!IS_NPC(ch) ) continue;
        if (ch->pIndexData->vnum == 3003) FAULT_W = FALSE;
      }
      if (FAULT_W)  {
        point = 0;  /* to begining */
        interpret( mob, "emote looks around, searching for someone.");
        do_say( mob, "Master? Where are you?");
        do_drop(mob, "iron" );
	do_sacrifice(mob, "iron");
        last_time_weaponsmith = current_time;
        mob->status = MOB_STAT_SERV_STOP;
      }        
      break;
    }
    point++;
    if ( point > 55 ) point = 1;
    break;
  }
  return TRUE;  
}

bool spec_weaponsmith(CHAR_DATA *mob)
{
static int last_action, delay;
OBJ_DATA *sword;
 
  if ( last_time_weaponsmith == -1 ) last_time_weaponsmith = current_time;
  if ( mob->status == 0 ) mob->status = MOB_STAT_RESTING;
  if (delay > 2 || delay < 0) delay = 2;
  delay--;
  if ( !delay ) delay = 2;

  switch( mob->status )  {
  case MOB_STAT_RESTING:
    if (last_action != MOB_STAT_RESTING)  {
      interpret(mob, "emote sits down to his chair, taking a deep breath.");
      last_action = MOB_STAT_RESTING;
    }
    if ( (count_obj_list( get_obj_index(OBJ_VNUM_WEAPONSMITH_SWORD1), 
         mob->carrying) +
         count_obj_list( get_obj_index(OBJ_VNUM_WEAPONSMITH_SWORD2), 
	 mob->carrying) ) < 5 )
      mob->status = MOB_STAT_HEATING_SWORD;
    break;
  case MOB_STAT_BEATING_SWORD:
    if (last_action != MOB_STAT_BEATING_SWORD)  {
      interpret(mob, "emote puts the sword on the anvil, and starts beating.");
      last_action = MOB_STAT_BEATING_SWORD;
    }
    if ( dice(1, 13) > 2 ) mob->status = MOB_STAT_BEATING_SWORD;
    else if ( dice(1, 13) > 2) mob->status = MOB_STAT_HEATING_SWORD;
    else mob->status = MOB_STAT_COOLING_SWORD;
    break;
  case MOB_STAT_HEATING_SWORD:
    if (last_action != MOB_STAT_HEATING_SWORD)  {
      if ( last_action == MOB_STAT_RESTING )
	interpret(mob, "emote stops resting, and picks a half-made sword from the counter.\n\r");
      else
	interpret(mob, "emote goes beside the fire, and starts heating the sword again.");
      last_action = MOB_STAT_HEATING_SWORD;
    }
    if ( dice(1, 10) == 1 ) mob->status = MOB_STAT_BEATING_SWORD;
    else mob->status = MOB_STAT_HEATING_SWORD;
    break;
  case MOB_STAT_COOLING_SWORD:
    if (last_action != MOB_STAT_COOLING_SWORD)  {
      interpret(mob, "emote dips the hot sword into the cooling tank. TCSHHHH...");
      last_action = MOB_STAT_COOLING_SWORD;
    }
    if (dice(1,3) == 1) mob->status = MOB_STAT_HANGING_SWORD;
    break;
  case MOB_STAT_HANGING_SWORD:
    if (last_action != MOB_STAT_HANGING_SWORD)  {
      interpret(mob, "emote carefully places the new sword on to the sword stand.");
      last_action = MOB_STAT_HANGING_SWORD;
      mob->status = MOB_STAT_RESTING;
      if (dice(1,2) == 1)
        sword = create_object(get_obj_index(OBJ_VNUM_WEAPONSMITH_SWORD1),-1 );
      else
        sword = create_object(get_obj_index(OBJ_VNUM_WEAPONSMITH_SWORD2),-1 );
      obj_to_char( sword, mob );
    }
    break;

  case MOB_STAT_WAITING_ANSWER:
    break;
  case MOB_STAT_WAITING_SERVANT:
    break;
  case MOB_STAT_SERVANT_LATE:
    break;
  case MOB_STAT_MAKE_SWORD:
    break;
  case MOB_STAT_MAKE_DAGGER:
    break;
  case MOB_STAT_MAKE_MACE:
    break;
  case MOB_STAT_WEAPON_READY:
    break;
  }
 return FALSE;

}

bool spec_repairman( CHAR_DATA *ch )
{
    if ( !IS_AWAKE(ch) )
        return FALSE;
    if (number_range(0,100) == 0) 
    {
        do_say(ch, "Now it is time to repair the other equipments.");
        return TRUE;
    }
    return FALSE;
}

