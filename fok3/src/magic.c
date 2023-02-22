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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"

/* command procedures needed */
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_kill		);

/*
 * Local functions.
 */
void	say_spell	args( ( CHAR_DATA *ch, int sn ) );

/* imported functions */
bool    remove_obj      args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
void 	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );



/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
    int sn;
    
    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&   !str_prefix( name, skill_table[sn].name ) )
	    return sn;
    }

    return -1;
}

int find_spell( CHAR_DATA *ch, const char *name )
{
    /* finds a spell the character can cast if possible */
    char buf[MAX_STRING_LENGTH];
    int sn, found = -1;

    if (IS_NPC(ch))
	return skill_lookup(name);

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
//        send_to_char("A",ch);
	if (skill_table[sn].name == NULL)
	    break;
//        send_to_char("B",ch);
/*        if(skill_table[sn].name == "reserved")
	    break; */
//        send_to_char("C",ch);
        xprintf(buf, "Find Spell %s,%s!\n\r",skill_table[sn].name,name);

	if (LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&  !str_prefix(name,skill_table[sn].name))
	{
	    if ( found == -1)
		found = sn;
	    if (ch->level >= skill_table[sn].skill_level[ch->class]
	    &&  ch->pcdata->learned[sn] > 0)
		    return sn;
	}
    }
    return found;
}





/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA *victim, int dam_type )
{
    int save;

    save = 50 + ( victim->level - level) * 5 - victim->saving_throw * 2;
    if (IS_AFFECTED(victim,AFF_BERSERK))
	save += victim->level/2;

    switch(check_immune(victim,dam_type))
    {
	case IS_IMMUNE:		return TRUE;
	case IS_RESISTANT:	save += 2;	break;
	case IS_VULNERABLE:	save -= 2;	break;
    }

    if (!IS_NPC(victim) && class_table[victim->class].fMana)
	save = 9 * save / 10;
    save = URANGE( 5, save, 95 );
    return number_percent( ) < save;
}

/* RT save for dispels */

bool saves_dispel( int dis_level, int spell_level, int duration)
{
    int save;
    
    if (duration == -1)
      spell_level += 5;
      /* very hard to dispel permanent effects */

    save = 35 + (spell_level - dis_level) * 5;
    save = URANGE( 5, save, 95 );
    return number_percent( ) < save;
}

/* co-routine for dispel magic and cancellation */

bool check_dispel( int dis_level, CHAR_DATA *victim, int sn)
{
    AFFECT_DATA *af;

    if (is_affected(victim, sn))
    {
	for ( af = victim->affected; af != NULL; af = af->next )
        {
            if ( af->type == sn )
            {
                if (!saves_dispel(dis_level,af->level,af->duration))
                {
                    affect_strip(victim,sn);
        	    if ( skill_table[sn].msg_off )
        	    {
            		send_to_char( skill_table[sn].msg_off, victim );
            		send_to_char( "\n\r", victim );
        	    }
		    return TRUE;
		}
		else
		    af->level--;
            }
        }
    }
    return FALSE;
}

/* for finding mana costs -- temporary version */
int mana_cost (CHAR_DATA *ch, int min_mana, int level)
{
	int mana = 1000;
	
    if (ch->level + 2 == level)
	return 1000;
    
    mana = UMAX(min_mana,(100/(2 + ch->level - level)));

    if (get_curr_stat(ch,STAT_WIS) != 15)
    	mana += (mana * wis_app[get_curr_stat(ch,STAT_WIS)].manacost) / 100;
	
	return mana;
}

/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int target;

if ( is_affected(ch, gsn_duo_dimension))
          {
		send_to_char("You can't cast spell in to a dimension your not a part of.\n\r", ch);
		return;
	  }

if (ch->class == CLASS_BRD)
{
send_to_char("{cBards would much rather sing.{x\n\r",ch);
return;
}

if (ch->class == CLASS_MER)
{
send_to_char("{cShaman would much rather Dance.{x\n\r",ch);
return;
}


if ((ch->class == CLASS_PDN) || (ch->class == CLASS_CLE))
{
send_to_char("{cYou are above casting spells, try commune.{x\n\r",ch);
return;
}

    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;

     if (is_affected(ch, gsn_garble) || is_affected(ch, gsn_deafen))
     {
        send_to_char("You can't get the right intonations.\n\r", ch);
        return;
     }
     if (!IS_NPC(ch) && is_affected(ch, gsn_charm_person))
     {
        send_to_char("You should ask your master first.\n\r", ch);
        return;
     }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

//    xprintf(buf, "%s,%s,%s,%s!\n\r",argument,arg1,arg2,target_name);
//    send_to_char(buf,ch);

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Cast which what where?\n\r", ch );
	return;
    }
/*
    if (ch->kingdom == KINGDOM_HORDE && !IS_IMMORTAL(ch)) {
      send_to_char("You are a member of Horde, not a filthy magician!\n\r",ch);
      return;
    }
*/
    //sn = find_spell( ch,arg1 );
//    xprintf(buf, "Find Spell %d,%s!\n\r",sn,arg1);
//    send_to_char(buf,ch);
//    xprintf(buf, "%d,level %d, level for class %d \n\r",ch->pcdata->learned[sn],ch->level,skill_table[sn].skill_level[ch->class]);
//    send_to_char(buf,ch); 
   
    if ( ( sn = find_spell( ch,arg1 ) ) < 0
    || ( !IS_NPC(ch) && (ch->level < skill_table[sn].skill_level[ch->class]
    ||   		 ch->pcdata->learned[sn] == 0)))
    {
	send_to_char( "You don't know any spells of that name.\n\r", ch );
	return;
    }


    if ( skill_table[sn].spell_fun == spell_null )
    {
	send_to_char("That's not a spell!\n\r",ch);
/*        send_to_char("That's not a spell!  You tried to cast a skill!\n\r",ch);
        send_to_char("For that, you are being penalized 1 percentage point.\n\r"                        ,ch);
        if (ch->pcdata->learned[sn] > 1)
          ch->pcdata->learned[sn] -= 1;
*/
        return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
	send_to_char( "You can not concentrate enough.\n\r", ch );
	return;
    }

    if (!kingdom_ok(ch,sn))
      return;

    if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
    {
        send_to_char("Your spell fizzles out and fails.\n\r",ch);
        act("$n's spell fizzles out and fails.", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
	mana = 50;
    else
	mana = mana_cost (ch,skill_table[sn].min_mana,(skill_table[sn].skill_level[ch->class]));

    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
        if ( is_affected(ch,gsn_spellbane))
          {
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//            damage_spell(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
 	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Cast the spell on whom?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) )
	{
	    if ( is_safe(ch,victim) )
	    {
		send_to_char("Something causes your spell to fail.\n\r",ch);
		return;
	    }
	    if ( is_safe(ch, victim) )
		return;
	}

	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can't do that on your own follower.\n\r",
		ch );
	    return;
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
        if (!IS_NPC(ch) && victim != ch &&
            ch->fighting != victim && victim->fighting != ch &&
            (IS_SET(victim->affected_by,AFF_CHARM) || !IS_NPC(victim)))
          {
            if (!can_see(victim, ch))
                do_yell(victim, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you sorcerous dog!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                     ch->doppel->name : ch->name);
                 do_yell(victim,buf);
              }
          }
        if ( is_affected(victim,gsn_spellbane) &&  (/*IS_NPC(victim) ||*/
            number_percent() < 2*victim->pcdata->learned[gsn_spellbane]/3) 
		&& sn != 524 && sn !=526)  /* mental knife can't be deflected */
					   /* and scourge... */
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//                damage_spell(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
              damage_spell(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              multi_hit(victim,ch,TYPE_UNDEFINED);
            }
            return;
          }
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) )
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//                damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);              
               }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "You cannot cast this spell on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;

        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }

	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the spell be cast upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name , NULL) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
	break;

    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char("Cast the spell on whom or what?\n\r",ch);
		return;
	    }

	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL, target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}

	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
	    if(is_safe_spell(ch,victim,FALSE) && victim != ch)
	    {
		send_to_char("Your spell didn't work.\n\r",ch);
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can't do that on your own follower.\n\r",
		    ch );
		return;
	    }

	    if ( is_safe(ch, victim) )
		return;

	    vo = (void *) victim;
	}
	else if ((obj = get_obj_here( ch, NULL,target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	break;

    case TAR_OBJ_CHAR_DEF:
	if (arg2[0] == '\0')
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL, target_name)) != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name, NULL)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	break;
    }

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "You don't have enough mana.\n\r", ch );
	return;
    }

    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
	say_spell( ch, sn );
    
    if (ch->race == race_lookup("elf") && (ch->in_room->sector_type == SECT_FOREST))
        WAIT_STATE( ch, skill_table[sn].beats/1.7 );
       else
        WAIT_STATE( ch, skill_table[sn].beats );
    if ( number_percent( ) > get_skill(ch,sn) - (ch->lagspell * 9))
    {
	send_to_char( "{cYou lost your concentration.{x\n\r", ch );
	check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
    }
    else
    {
	ch->mana -= mana;
/*	if (IS_NPC(ch) || class_table[ch->class].fMana)
	// class has spells  */
	    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
/*	else And i dont like it give to much troble
	    (*skill_table[sn].spell_fun) (sn, 3 * ch->level/4, ch, vo,target); */
	check_improve(ch,sn,TRUE,1);
    }


    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		if ( victim->position != POS_SLEEPING )
		multi_hit( victim, ch, TYPE_UNDEFINED );

		break;
	    }
	}
    }

    return;
}


void do_sing( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int target;

if ( is_affected(ch, gsn_duo_dimension))
          {
		send_to_char("You can't cast spell in to a dimension your not a part of.\n\r", ch);
		return;
	  }

if (ch->class != CLASS_BRD)
{
send_to_char("{cA Bard would do a much better job.{x\n\r",ch);
return;
}



    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;

     if (is_affected(ch, gsn_garble) || is_affected(ch, gsn_deafen))
     {
        send_to_char("Your singing is lousy for some reason.\n\r", ch);
        return;
     }
     if (is_affected(ch, gsn_charm_person))
     {
        send_to_char("You should ask your master first.\n\r", ch);
        return;
     }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Sing what?\n\r", ch );
	return;
    }
/* Bards can join Horde...
    if (ch->kingdom == KINGDOM_HORDE && !IS_IMMORTAL(ch)) {
      send_to_char("You are a member of Horde, not a filthy magician!\n\r",ch);
      return;
    }
*/

    if ( ( sn = find_spell( ch,arg1 ) ) < 0
    || ( !IS_NPC(ch) && (ch->level <
skill_table[sn].skill_level[ch->class]
    ||   		 ch->pcdata->learned[sn] == 0)))
    {
	send_to_char( "You do not know any songs of that name.\n\r", ch );
	return;
    }

    if ( skill_table[sn].spell_fun == spell_null )
    {
        send_to_char("That is not a song!\n\r",ch);
        if (ch->pcdata->learned[sn] > 1)
          ch->pcdata->learned[sn] -= 1;
        return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
	send_to_char( "You can not sing that like this.\n\r", ch );
	return;
    }

    if (!kingdom_ok(ch,sn))
      return;

    if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
    {
        send_to_char("Your open your mouth to sing but your voice is absorbed here.\n\r",ch);
        act("$n's voice is absorbed here.", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
	mana = 50;
    else
	mana = mana_cost (ch,skill_table[sn].min_mana,(skill_table[sn].skill_level[ch->class]));

    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_sing: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
/* spellbane dont work on songs
        if ( is_affected(ch,gsn_spellbane))
          {
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
 */
 	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Who do you wish to sing that to?\n\r", ch);
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL,  target_name ) ) == NULL )
	    {
		send_to_char( "They are not here.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) )
	{
	    if ( is_safe(ch,victim) )
	    {
		send_to_char("Something in this room keeps your from singing.\n\r",ch);
		return;
	    }
	    if ( is_safe(ch, victim) )
		return;
	}
/*
	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can not do that on your own follower.\n\r",
		ch );
	    return;
	}
*/
	vo = (void *) victim;
	target = TARGET_CHAR;
        if (!IS_NPC(ch) && victim != ch &&
            ch->fighting != victim && victim->fighting != ch &&
            (IS_SET(victim->affected_by,AFF_CHARM) || !IS_NPC(victim)))
          {
            if (!can_see(victim, ch))
                do_yell(victim, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you singing fool!",

(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                     ch->doppel->name : ch->name);
                 do_yell(victim,buf);
              }
          }
/* spellbane commented out
        if ( is_affected(victim,gsn_spellbane)
           &&  (number_percent() <
2*victim->pcdata->learned[gsn_spellbane]/3)
		 && sn != 524 && sn !=526)  
*/
		/* mental knife and scourge can't be deflected */
/*
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
                damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE,
TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
              damage(victim,ch,10 *
victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              multi_hit(victim,ch,TYPE_UNDEFINED);
                 }
            return;
          }
*/          
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL, target_name ) ) == NULL )
	    {
		send_to_char( "They are not here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
 /* spellbane commented out
        if ( is_affected(victim,gsn_spellbane) )
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the
spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the
spell!",ch,NULL,NULL,TO_ROOM);
                damage(victim,ch,10 *
victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
              damage(victim,ch,10 *
victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
                 }
            return;
          }
*/
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "This song is not meant for others.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;
/* spellbane commented out
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the
spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the
spell!",ch,NULL,NULL,TO_ROOM);
            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
*/
	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What item are you going to sing to you lunatic?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name , NULL) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
 /* spellbane commented out
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the
spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the
spell!",ch,NULL,NULL,TO_ROOM);
            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
*/
	break;

    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char("Who or what are you singing to?\n\r",ch);
		return;
	    }

	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL,target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}

	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
	    if(is_safe_spell(ch,victim,FALSE) && victim != ch)
	    {
		send_to_char("Your song seems to have had no effect.\n\r",ch);
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can not do that to your own follower.\n\r",
		    ch );
		return;
	    }

	    if ( is_safe(ch, victim) )
		return;

	    vo = (void *) victim;
	}
	else if ((obj = get_obj_here( ch, NULL, target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
	break;

    case TAR_OBJ_CHAR_DEF:
	if (arg2[0] == '\0')
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL, target_name)) != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name , NULL)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
	break;
    }

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "Your mind is drained.\n\r", ch );
	return;
    }

    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
	say_spell( ch, sn );

    if (ch->race == race_lookup("elf") && (ch->in_room->sector_type == SECT_FOREST))
        WAIT_STATE( ch, skill_table[sn].beats/1.7 );
       else
        WAIT_STATE( ch, skill_table[sn].beats );

    if ( number_percent( ) > get_skill(ch,sn) - (ch->lagspell * 9))
    {
	send_to_char( "{cYou left out a verse.{x\n\r", ch );
	check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
    }
    else
    {
	ch->mana -= mana;
	if (IS_NPC(ch) || class_table[ch->class].fMana)
	/* class has spells */
	    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
	else
	    (*skill_table[sn].spell_fun) (sn, 3 * ch->level/4, ch,
vo,target);
	check_improve(ch,sn,TRUE,1);
    }

    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target ==
TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		if ( victim->position != POS_SLEEPING )
		multi_hit( victim, ch, TYPE_UNDEFINED );

		break;
	    }
	}
    }

    return;
}

void do_dance( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int target;


if (ch->class != CLASS_MER)
{
send_to_char("{cA Shaman would do a much better job.{x\n\r",ch);
return;
}



    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;

     if (is_affected(ch, gsn_garble) || is_affected(ch, gsn_deafen))
     {
        send_to_char("Your dance is lousy for some reason.\n\r", ch);
        return;
     }
     if (is_affected(ch, gsn_charm_person))
     {
        send_to_char("You should ask your master first.\n\r", ch);
        return;
     }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Dance what?\n\r", ch );
	return;
    }
/* Bards can join Horde...
    if (ch->kingdom == KINGDOM_HORDE && !IS_IMMORTAL(ch)) {
      send_to_char("You are a member of Horde, not a filthy magician!\n\r",ch);
      return;
    }
*/

    if ( ( sn = find_spell( ch,arg1 ) ) < 0
    || ( !IS_NPC(ch) && (ch->level <
skill_table[sn].skill_level[ch->class]
    ||   		 ch->pcdata->learned[sn] == 0)))
    {
	send_to_char( "You do not know any Dances of that name.\n\r", ch );
	return;
    }

    if ( skill_table[sn].spell_fun == spell_null )
    {
        send_to_char("That is not a Dance!\n\r",ch);
        if (ch->pcdata->learned[sn] > 1)
          ch->pcdata->learned[sn] -= 1;
        return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
	send_to_char( "You can not dance that like this.\n\r", ch );
	return;
    }

    if (!kingdom_ok(ch,sn))
      return;

    if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
    {
        send_to_char("Your open your mouth to sing but your voice is absorbed here.\n\r",ch);
        act("$n's voice is absorbed here.", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
	mana = 50;
    else
	mana = mana_cost (ch,skill_table[sn].min_mana,(skill_table[sn].skill_level[ch->class]));
    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_sing: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
/* spellbane dont work on songs
        if ( is_affected(ch,gsn_spellbane))
          {
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
 */
 	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Who do you wish to dance that to?\n\r", ch);
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL,  target_name ) ) == NULL )
	    {
		send_to_char( "They are not here.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) )
	{
	    if ( is_safe(ch,victim) )
	    {
		send_to_char("Something in this room keeps your from danceing.\n\r",ch);
		return;
	    }
	    if ( is_safe(ch, victim) )
		return;
	}
/*
	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can not do that on your own follower.\n\r",
		ch );
	    return;
	}
*/
	vo = (void *) victim;
	target = TARGET_CHAR;
        if (!IS_NPC(ch) && victim != ch &&
            ch->fighting != victim && victim->fighting != ch &&
            (IS_SET(victim->affected_by,AFF_CHARM) || !IS_NPC(victim)))
          {
            if (!can_see(victim, ch))
                do_yell(victim, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you danceing fool!",

(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                     ch->doppel->name : ch->name);
                 do_yell(victim,buf);
              }
          }
/* spellbane commented out
        if ( is_affected(victim,gsn_spellbane)
           &&  (number_percent() <
2*victim->pcdata->learned[gsn_spellbane]/3)
		 && sn != 524 && sn !=526)  
*/
		/* mental knife and scourge can't be deflected */
/*
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
                damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE,
TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
              damage(victim,ch,10 *
victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              multi_hit(victim,ch,TYPE_UNDEFINED);
                 }
            return;
          }
*/          
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL, target_name ) ) == NULL )
	    {
		send_to_char( "They are not here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
 /* spellbane commented out
        if ( is_affected(victim,gsn_spellbane) )
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the
spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the
spell!",ch,NULL,NULL,TO_ROOM);
                damage(victim,ch,10 *
victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
              damage(victim,ch,10 *
victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
                 }
            return;
          }
*/
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "This dance is not meant for others.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;
/* spellbane commented out
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the
spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the
spell!",ch,NULL,NULL,TO_ROOM);
            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
*/
	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What item are you going to dance to you lunatic?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name , NULL) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
 /* spellbane commented out
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the
spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the
spell!",ch,NULL,NULL,TO_ROOM);
            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
*/
	break;

    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char("Who or what are you dance to?\n\r",ch);
		return;
	    }

	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL, target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}

	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
	    if(is_safe_spell(ch,victim,FALSE) && victim != ch)
	    {
		send_to_char("Your danc seems to have had no effect.\n\r",ch);
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can not do that to your own follower.\n\r",
		    ch );
		return;
	    }

	    if ( is_safe(ch, victim) )
		return;

	    vo = (void *) victim;
	}
	else if ((obj = get_obj_here( ch, NULL, target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
	break;

    case TAR_OBJ_CHAR_DEF:
	if (arg2[0] == '\0')
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL, target_name)) != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name , NULL)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
	break;
    }

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "Your mind is drained.\n\r", ch );
	return;
    }

    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
	say_spell( ch, sn );

    if (ch->race == race_lookup("elf") && (ch->in_room->sector_type == SECT_FOREST))
        WAIT_STATE( ch, skill_table[sn].beats/1.7 );
       else
        WAIT_STATE( ch, skill_table[sn].beats );

    if ( number_percent( ) > get_skill(ch,sn) - (ch->lagspell * 9))
    {
	send_to_char( "{cYou left out a verse.{x\n\r", ch );
	check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
    }
    else
    {
	ch->mana -= mana;
	if (IS_NPC(ch) || class_table[ch->class].fMana)
	/* class has spells */
	    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
	else
	    (*skill_table[sn].spell_fun) (sn, 3 * ch->level, ch,
vo,target);
	check_improve(ch,sn,TRUE,1);
    }

    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target ==
TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		if ( victim->position != POS_SLEEPING )
		multi_hit( victim, ch, TYPE_UNDEFINED );

		break;
	    }
	}
    }

    return;
}


void do_commune( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    int target;

if ( is_affected(ch, gsn_duo_dimension))
          {
		send_to_char("You can't cast spell in to a dimension your not a part of.\n\r", ch);
		return;
	  }

if (!((ch->class == CLASS_CLE) || (ch->class == CLASS_PDN)) )
{
send_to_char("{cYou are not devout enough to commune.{x\n\r",ch);
return;
}

    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;

 /* garble dont work on commune
     if (is_affected(ch, gsn_garble) || is_affected(ch, gsn_deafen))
     {
        send_to_char("You can not get the right intonations.\n\r", ch);
        return;
     }
  */
     if (is_affected(ch, gsn_charm_person))
     {
        send_to_char("You should ask your master first.\n\r", ch);
        return;
     }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Commune for what?\n\r", ch );
	return;
    }
/*
    if (ch->kingdom == KINGDOM_HORDE && !IS_IMMORTAL(ch)) {
      send_to_char("As a member of Horde, you serve the Gods, not commune with them!\n\r",ch);
      return;
    }
*/
    if ( ( sn = find_spell( ch,arg1 ) ) < 0
    || ( !IS_NPC(ch) && (ch->level < skill_table[sn].skill_level[ch->class]
    ||   		 ch->pcdata->learned[sn] == 0)))
    {
	send_to_char( "You have not earned that privilege.\n\r", ch );
	return;
    }

    if ( skill_table[sn].spell_fun == spell_null )
    {
        send_to_char("That is not something to commune for!\n\r",ch);
        send_to_char("You feel further from knowledge with that dumb mistake.\n\r",ch);
        if (ch->pcdata->learned[sn] > 1)
          ch->pcdata->learned[sn] -= 1;
        return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
	send_to_char( "You can not concentrate enough.\n\r", ch );
	return;
    }

    if (!kingdom_ok(ch,sn))
      return;

    if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
    {
        send_to_char("Your communion powers fizzle out and fail.\n\r",ch);
        act("$n's communion powers fizzle out and fail.", ch, NULL, NULL,
TO_ROOM);
        return;
    }

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
	mana = 50;
    else
	mana = mana_cost (ch,skill_table[sn].min_mana,(skill_table[sn].skill_level[ch->class]));

    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_commune: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
        if ( is_affected(ch,gsn_spellbane))
          {
            act("Your spellbane protects you!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane protects $m!",ch,NULL,NULL,TO_ROOM);
//            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
 	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Who is the target?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL, target_name ) ) == NULL )
	    {
		send_to_char( "They are not here.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) )
	{
	    if ( is_safe(ch,victim) )
	    {
		send_to_char("This room is protected with a truly divine aura.\n\r",ch);
		return;
	    }
	    if ( is_safe(ch, victim) )
		return;
	}
/*
	if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can't do that on your own follower.\n\r",
		ch );
	    return;
	}
*/
	vo = (void *) victim;
	target = TARGET_CHAR;
        if (!IS_NPC(ch) && victim != ch &&
            ch->fighting != victim && victim->fighting != ch &&
            (IS_SET(victim->affected_by,AFF_CHARM) || !IS_NPC(victim)))
          {
            if (!can_see(victim, ch))
                do_yell(victim, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you heathen!",

(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                     ch->doppel->name : ch->name);
                 do_yell(victim,buf);
              }
          }
        if ( is_affected(victim,gsn_spellbane) &&  (/*IS_NPC(victim) ||*/
            number_percent() < 2*victim->pcdata->learned[gsn_spellbane]/3)
		&& sn != 524 && sn !=526)  /* mental knife can't be
deflected */
					   /* and scourge... */
          {
            if (ch==victim)
              {
                act("Your spellbane protects you!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane protects your from t!",ch,NULL,NULL,TO_ROOM);
//                damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE,TRUE);
              }
            else {
              act("$N deflects your divine power!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's divine power!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's divine power!",ch,NULL,victim,TO_NOTVICT);
              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              multi_hit(victim,ch,TYPE_UNDEFINED);
            }
            return;
          }
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, NULL,target_name ) ) == NULL )
	    {
		send_to_char( "They are not here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) )
          {
            if (ch==victim)
              {
                act("Your spellbane protects you!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane protects you!",ch,NULL,NULL,TO_ROOM);
//                damage(victim,ch,10 *victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);             
                  }
            else {
              act("$N deflects your divine power!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's divine power!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's divine power!",ch,NULL,victim,TO_NOTVICT);
  //            damage(victim,ch,10 *victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "You are the only person able to benefit from that.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;

        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane protects you!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane protects $m!",ch,NULL,NULL,TO_ROOM);
//            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }

	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "On what item?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name , NULL) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane protects you!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane protects $m!",ch,NULL,NULL,TO_ROOM);
//            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }
	break;

    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char("On whom or what?\n\r",ch);
		return;
	    }

	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL,target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}

	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
	    if(is_safe_spell(ch,victim,FALSE) && victim != ch)
	    {
		send_to_char("Your communion was a failure.\n\r",ch);
		return;
	    }

	    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	    {
		send_to_char( "You can not do that on your own follower.\n\r",
		    ch );
		return;
	    }

	    if ( is_safe(ch, victim) )
		return;

	    vo = (void *) victim;
	}
	else if ((obj = get_obj_here( ch, NULL, target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
	break;

    case TAR_OBJ_CHAR_DEF:
	if (arg2[0] == '\0')
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room( ch, NULL, target_name)) != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name , NULL)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You do not see that here.\n\r",ch);
	    return;
	}
	break;
    }

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "You do not have enough mana.\n\r", ch );
	return;
    }

    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
	say_spell( ch, sn );

    if (ch->race == race_lookup("elf") && (ch->in_room->sector_type == SECT_FOREST))
        WAIT_STATE( ch, skill_table[sn].beats/1.7 );
       else
        WAIT_STATE( ch, skill_table[sn].beats );
//   if ( str_cmp( skill_table[sn].name, "paralysis" ) ){
    if ( number_percent( ) > get_skill(ch,sn) - (ch->lagspell * 9))
    {
 	send_to_char( "{cYou lost your concentration.{x\n\r", ch );
	check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
//    }
}
    else
    {
 	ch->mana -= mana;
	if (IS_NPC(ch) || class_table[ch->class].fMana)
	/* class has spells */
	    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
	else
	    (*skill_table[sn].spell_fun) (sn, 3 * ch->level/4, ch,
vo,target);
	check_improve(ch,sn,TRUE,1);
    }

    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target ==
TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		if ( victim->position != POS_SLEEPING )
		multi_hit( victim, ch, TYPE_UNDEFINED );

		break;
	    }
	}
    }

    return;
}


/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
    void *vo;
    int target = TARGET_NONE;
char buf[MAX_STRING_LENGTH];

    if ( sn <= 0 )
	return;

    if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
    {
	bug( "Obj_cast_spell: bad sn %d.", sn );
	return;
    }

/*
	if (obj != NULL)
    if ((skill_table[sn].objectDissallow[0] == 0) && (obj->item_type == ITEM_POTION))
    {
		
		xprintf(buf,"The Spell '%s' is no longer allowed in potions\n\r",skill_table[sn].name);
		send_to_char( buf,ch );
		send_to_char( "Report where you found the potion to the administrators\n\r",ch );
		send_to_char( "Code changed by Kazmir 20/11/2004\n\r",ch );
	return;
    }
*/
/*
	if (obj != NULL)
    if ((skill_table[sn].objectDissallow[1] == 0) && (obj->item_type == ITEM_SCROLL))
    {
		
		xprintf(buf,"The Spell '%s' is no longer allowed in scrolls\n\r",skill_table[sn].name);
		send_to_char( buf,ch );
		send_to_char( "Report where you found the potion to the administrators\n\r",ch );
		send_to_char( "Code changed by Kazmir 4/12/2004\n\r",ch );
	return;
    }
    
 	*/
 	/*
 	if (obj != NULL)
    if ((skill_table[sn].objectDissallow[2] == 0) && (obj->item_type == ITEM_PILL))
    {
		
		xprintf(buf,"The Spell '%s' is no longer allowed in pills\n\r",skill_table[sn].name);
		send_to_char( buf,ch );
		send_to_char( "Report where you found the potion to the administrators\n\r",ch );
		send_to_char( "Code changed by Kazmir 4/12/2004\n\r",ch );
	return;
    }

*/


    switch ( skill_table[sn].target )
    {
    default:
	bug( "Obj_cast_spell: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	vo = NULL;
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( victim == NULL )
	    victim = ch->fighting;
	if ( victim == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	if (is_safe(ch,victim) && ch != victim)
	{
	    send_to_char("Something isn't right...\n\r",ch);
	    return;
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) && (/*IS_NPC(victim) ||*/
                number_percent() < 2*ch->pcdata->learned[gsn_spellbane]/3) )
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//                damage(victim,ch,10 * level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
//              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }

	break;

    case TAR_CHAR_DEFENSIVE:
    case TAR_CHAR_SELF:
	if ( victim == NULL )
	    victim = ch;
	vo = (void *) victim;
	target = TARGET_CHAR;
        if ( is_affected(victim,gsn_spellbane) )
          {
            if (ch==victim)
              {
                act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
                act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//                damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
              }
            else {
              act("$N deflects your spell!",ch,NULL,victim,TO_CHAR);
              act("You deflect $n's spell!",ch,NULL,victim,TO_VICT);
              act("$N deflects $n's spell!",ch,NULL,victim,TO_NOTVICT);
//              damage(victim,ch,10 * victim->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            }
            return;
          }
        break;

    case TAR_OBJ_INV:
	if ( obj == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	vo = (void *) obj;
	target = TARGET_OBJ;
        if ( is_affected(ch,gsn_spellbane) )
          {
            act("Your spellbane deflects the spell!",ch,NULL,NULL,TO_CHAR);
            act("$n's spellbane deflects the spell!",ch,NULL,NULL,TO_ROOM);
//            damage(ch,ch,10 * ch->level,gsn_spellbane,DAM_NEGATIVE, TRUE);
            return;
          }

	break;

    case TAR_OBJ_CHAR_OFF:
	if ( victim == NULL && obj == NULL)
	 {
	    if (ch->fighting != NULL) {
		victim = ch->fighting; }
	    else
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }
         }
	    if (victim != NULL)
	    {
		if (is_safe_spell(ch,victim,FALSE) && ch != victim)
		{
		    send_to_char("Somehting isn't right...\n\r",ch);
		    return;
		}

		vo = (void *) victim;
		target = TARGET_CHAR;
	    }
	    else
	    {
		vo = (void *) obj;
		target = TARGET_OBJ;
	    }
	break;


    case TAR_OBJ_CHAR_DEF:
	if (victim == NULL && obj == NULL)
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if (victim != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}

	break;
    }

    target_name = "";
    (*skill_table[sn].spell_fun) ( sn, level, ch, vo,target);



    if ( (skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );

		break;
	    }
	}
    }

    return;
}



/*
 * Spell functions.
 */
void spell_acid_blast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 12 );
    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}



void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int ass;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already armored.\n\r",ch);
	else
	  act("$N is already armored.",ch,NULL,victim,TO_CHAR);
	return;
    }

    ass = (ch->class == class_lookup("assassin")) ? level/2 : level;
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = ass;
    af.duration  = 24;
    af.modifier  = -20;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel someone protecting you.\n\r", victim );
    if ( ch != victim )
	act("$N is protected by your magic.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_bless( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;

    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;
	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	{
	    act("$p is already blessed.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (IS_OBJ_STAT(obj,ITEM_EVIL))
	{
	    AFFECT_DATA *paf;

	    paf = affect_find(obj->affected,gsn_curse);
	    if (!saves_dispel(level,paf != NULL ? paf->level : obj->level,0))
	    {
		if (paf != NULL)
		    affect_remove_obj(obj,paf);
		act("$p glows a pale blue.",ch,obj,NULL,TO_ALL);
		REMOVE_BIT(obj->extra_flags,ITEM_EVIL);
		return;
	    }
	    else
	    {
		act("The evil of $p is too powerful for you to overcome.",
		    ch,obj,NULL,TO_CHAR);
		return;
	    }
	}

	af.where	= TO_OBJECT;
	af.type		= sn;
	af.level	= level;
	af.duration	= 6 + level;
	af.location	= APPLY_SAVES;
	af.modifier	= -1;
	af.bitvector	= ITEM_BLESS;
	affect_to_obj(obj,&af);

	act("$p glows with a holy aura.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character target */
    victim = (CHAR_DATA *) vo;


    if ( victim->position == POS_FIGHTING || is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already blessed.\n\r",ch);
	else
	  act("$N already has divine favor.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 6+level;
    af.location  = APPLY_HITROLL;
    af.modifier  = level / 8;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - level / 8;
    affect_to_char( victim, &af );
    send_to_char( "You feel righteous.\n\r", victim );
    if ( ch != victim )
	act("You grant $N the favor of your god.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int ass;
if (ch->class == 13) {
    if ( IS_AFFECTED(victim, AFF_BLIND) || 
        saves_spell(level+5,victim,DAM_OTHER)) {  
      send_to_char( "You failed.\n\r", ch );
	return;
    }}
 else
     {if ( IS_AFFECTED(victim, AFF_BLIND) ||
        saves_spell(level,victim,DAM_OTHER))  {
      send_to_char( "You failed.\n\r", ch );
        return;
    } }    

    ass = (ch->class == class_lookup("assassin")) ? level/2 : level;
if (!IS_AFFECTED(victim, AFF_BLIND))
 {
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = ass;
    af.location  = APPLY_HITROLL;
 if (ch->class == 13)   
  af.modifier  = -10;
 else
  af.modifier  = -5;
    af.duration  = 1+level;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
    send_to_char( "You are blinded!\n\r", victim );
    act("$n appears to be blinded.",victim,NULL,NULL,TO_ROOM);
    return;
  }  
}



void spell_burning_hands(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  0,  0,	14,	17, 20, 23, 26, 29,
	29, 29, 30, 30,	31,	31, 32, 32, 33, 33,
	34, 34, 35, 35,	36,	36, 37, 37, 38, 38,
	39, 39, 40, 40,	41,	41, 42, 42, 43, 43,
	44, 44, 45, 45,	46,	46, 47, 47, 48, 48
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_FIRE) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_FIRE,TRUE);
    return;
}



void spell_call_lightning( int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You must be out of doors.\n\r", ch );
	return;
    }

    if ( weather_info.sky < SKY_RAINING )
    {
	send_to_char( "You need bad weather.\n\r", ch );
	return;
    }

    dam = dice(level/2, 8);

    send_to_char( "Gods' lightning strikes your foes!\n\r", ch );
    act( "$n calls lightning to strike $s foes!",
	ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if (vch == ch)
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && !is_same_group(ch, vch) )
	      if (is_safe(ch, vch))
		continue;
		damage_spell( ch, vch, saves_spell( level,vch,DAM_LIGHTNING)
		? dam / 2 : dam, sn,DAM_LIGHTNING,TRUE);
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area
	&&   IS_OUTSIDE(vch)
	&&   IS_AWAKE(vch) )
	    send_to_char( "Lightning flashes in the sky.\n\r", vch );
    }

    return;
}

/* RT calm spell stops all fighting in the room */

void spell_calm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *vch;
    int mlevel = 0;
    int count = 0;
    int high_level = 0;
    int chance;
    AFFECT_DATA af;

    /* get sum of all mobile levels in the room */
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->position == POS_FIGHTING)
	{
	    count++;
	    if (IS_NPC(vch))
	      mlevel += vch->level;
	    else
	      mlevel += vch->level/2;
	    high_level = UMAX(high_level,vch->level);
	}
    }

    /* compute chance of stopping combat */
    chance = 4 * level - high_level + 2 * count;

    if (IS_IMMORTAL(ch)) /* always works */
      mlevel = 0;

    if (number_range(0, chance) >= mlevel)  /* hard to stop large fights */
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
   	{
	    if (IS_NPC(vch) && (IS_SET(vch->imm_flags,IMM_MAGIC) ||
				IS_SET(vch->act,ACT_UNDEAD)))
	      return;

	    if (IS_AFFECTED(vch,AFF_CALM) || IS_AFFECTED(vch,AFF_BERSERK)
	    ||  is_affected(vch,skill_lookup("frenzy")))
	      return;

	    send_to_char("A wave of calm passes over you.\n\r",vch);

	    if (vch->fighting || vch->position == POS_FIGHTING)
	      stop_fighting(vch,FALSE);


	    af.where = TO_AFFECTS;
	    af.type = sn;
	    af.level = level;
	    af.duration = level/4;
	    af.location = APPLY_HITROLL;
	    if (!IS_NPC(vch))
	      af.modifier = -5;
	    else
	      af.modifier = -2;
	    af.bitvector = AFF_CALM;
	    affect_to_char(vch,&af);

	    af.location = APPLY_DAMROLL;
	    affect_to_char(vch,&af);
	}
    }
}

void spell_cancellation( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;

    level += 2;

    if ((!IS_NPC(ch) && IS_NPC(victim) &&
	 !(IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim) ) ||
	(IS_NPC(ch) && !IS_NPC(victim)) )
    {
	send_to_char("You failed, try dispel magic.\n\r",ch);
	return;
    }

    /* unlike dispel magic, the victim gets NO save */

    /* begin running through the spells */

    if (check_dispel(level,victim,skill_lookup("armor")))
	found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("bless")))
        found = TRUE;
 /*
    if (check_dispel(level,victim,skill_lookup("blindness")))
    {
        found = TRUE;
        act("$n is no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
*/
    if (check_dispel(level,victim,skill_lookup("calm")))
    {
	found = TRUE;
	act("$n no longer looks so peaceful...",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("change sex")))
    {
        found = TRUE;
	act("$n looks more like $mself again.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("charm person")))
    {
	found = TRUE;
	act("$n regains $s free will.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("chill touch")))
    {
	found = TRUE;
	act("$n looks warmer.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("curse")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("dark vision")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect evil")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect good")))
	found = TRUE;
/*
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
	found = TRUE;
*/
    if (check_dispel(level,victim,skill_lookup("detect invis")))
	found = TRUE;
/*
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
	found = TRUE;
*/
    if (check_dispel(level,victim,skill_lookup("detect magic")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("faerie fire")))
    {
	act("$n's pink outline fades.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("ice storm")))
    {
	act("$n feels warmer.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("fire storm")))
    {
	act("$n feels cooler.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("lightning storm")))
    {
	act("$n's muscle starts responding.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("meteor swarm")))
    {
	act("$n recover from the blast.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("divine word")))
    {
	act("$n feels a lot better.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("mental knife")))
    {
	act("$n feels smarter",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("faerie fog")))
    {
	act("$n's purple outline fades.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("fly")))
    {
	act("$n falls to the ground!",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("frenzy")))
    {
	act("$n no longer looks so wild.",victim,NULL,NULL,TO_ROOM);;
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("giant strength")))
    {
	act("$n no longer looks so mighty.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("haste")))
    {
	act("$n is no longer moving so quickly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("infravision")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("invis")))
    {
	act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    
/*    if (check_dispel(level,victim,skill_lookup("improved invis")))
    {
	act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }*/

    if (check_dispel(level,victim,skill_lookup("mass invis")))
    {
	act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("pass door")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection evil")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection good")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("sanctuary")))
    {
	act("The white aura around $n's body vanishes.",
	    victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("shield")))
    {
	act("The shield protecting $n vanishes.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("sleep")))
	found = TRUE;
    if (check_dispel(level,victim,skill_lookup("ice ray")))
	found = TRUE;
    if (check_dispel(level,victim,skill_lookup("slow")))
    {
	act("$n is no longer moving so slowly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("spellbane")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("stone skin")))
    {
	act("$n's skin regains its normal texture.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("weaken")))
    {
	act("$n looks stronger.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("wither")))
    {
	act("$n looks revitalized.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("detect undead")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("vampiric aura")))
    {
	act("$n seems less charismatic.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("cloak of fear")))
    {
	act("$n suddenly seems less terrifying.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
      
    if (check_dispel(level,victim,skill_lookup("decay")))
    {
	act("$n's armor returns to its original state.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("protection undead")))
        found = TRUE;

     if (check_dispel(level,victim,skill_lookup("bone armor")))
    {
	act("$n's bone armor crumbles and fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("malison")))
        found = TRUE;
 
    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
}

void spell_cause_light( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    int dam;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    
    dam = dice( level, 3 );
    damage_spell( ch, victim, dam, sn,DAM_HARM,TRUE);
    return;
}



void spell_cause_critical(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    int dam;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    
    dam = dice( level, 10 );
    damage_spell( ch, victim, dam, sn,DAM_HARM,TRUE);
    return;
}



void spell_cause_serious(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    int dam;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    
    dam = dice( level, 6 );
    damage_spell( ch, victim, dam, sn,DAM_HARM,TRUE);
    return;
}

void spell_chain_lightning(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    char buf[MAX_STRING_LENGTH];
    bool found;
    int dam;

    /* first strike */

    act("A lightning bolt leaps from $n's hand and arcs to $N.",
	ch,NULL,victim,TO_ROOM);
    act("A lightning bolt leaps from your hand and arcs to $N.",
	ch,NULL,victim,TO_CHAR);
    act("A lightning bolt leaps from $n's hand and hits you!",
	ch,NULL,victim,TO_VICT);

    dam = dice(level,6);
    if (saves_spell(level,victim,DAM_LIGHTNING))
	dam /= 3;
    damage_spell(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);

    if (!IS_NPC(ch) && victim != ch &&
        ch->fighting != victim && victim->fighting != ch &&
        (IS_SET(victim->affected_by,AFF_CHARM) || !IS_NPC(victim)))
      {
        if (!can_see(victim, ch))
          do_yell(victim, "Help someone is attacking me!");
        else
          {
            xprintf(buf,"Die, %s, you sorcerous dog!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?
                    ch->doppel->name : ch->name);
            do_yell(victim,buf);
          }
      }


    last_vict = victim;
    level -= 4;   /* decrement damage */

    /* new targets */
    while (level > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
	    if (is_safe(ch, tmp_vict) )  {
	      act( "The bolt passes around $n's body.", ch, NULL, NULL, TO_ROOM );
	      act( "The bolt passes around your body.", ch, NULL, NULL, TO_CHAR );
	    }
	    else  {
	      act("The bolt arcs to $n!",tmp_vict,NULL,NULL,TO_ROOM);
	      act("The bolt hits you!",tmp_vict,NULL,NULL,TO_CHAR);
	      dam = dice(level,6);

            if (!IS_NPC(ch) && tmp_vict != ch &&
                ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
                (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
              {
            if (!can_see(tmp_vict, ch))
                do_yell(tmp_vict, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you sorcerous dog!",                    
		(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
                     ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
              }
          }

	      if (saves_spell(level,tmp_vict,DAM_LIGHTNING))
		dam /= 3;
	      damage_spell(ch,tmp_vict,dam,sn,DAM_LIGHTNING,TRUE);
	      level -= 4;  /* decrement damage */
	    }
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    act("The bolt seems to have fizzled out.",ch,NULL,NULL,TO_ROOM);
	    act("The bolt grounds out through your body.",
		ch,NULL,NULL,TO_CHAR);
	    return;
	  }

	  last_vict = ch;
	  act("The bolt arcs to $n...whoops!",ch,NULL,NULL,TO_ROOM);
	  send_to_char("You are struck by your own lightning!\n\r",ch);
	  dam = dice(level,6);
	  if (saves_spell(level,ch,DAM_LIGHTNING))
	    dam /= 3;
	  damage_spell(ch,ch,dam,sn,DAM_LIGHTNING,TRUE);
	  level -= 4;  /* decrement damage */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}


void spell_change_sex( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ))
    {
	if (victim == ch)
	  send_to_char("You've already been changed.\n\r",ch);
	else
	  act("$N has already had $s(?) sex changed.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (saves_spell(level , victim,DAM_OTHER))
	return;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 2 * level;
    af.location  = APPLY_SEX;
    do
    {
	af.modifier  = number_range( 0, 2 ) - victim->sex;
    }
    while ( af.modifier == 0 );
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel different.\n\r", victim );
    act("$n doesn't look like $mself anymore...",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    CHAR_DATA *gch;
    int numcharmie;
//    char buf[MAX_STRING_LENGTH];

    if (is_safe(ch,victim)) return;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
	return;
    }
    if ( victim->fighting != NULL )
    {
	send_to_char( "You should wait your victim's fight over.\n\r", ch );
	return;
    }


    if ( IS_AFFECTED(victim, AFF_CHARM)
    ||   IS_AFFECTED(ch, AFF_CHARM)
    ||   level <= victim->level
    ||   IS_SET(victim->imm_flags,IMM_CHARM)
    ||   saves_spell( level, victim,DAM_CHARM) )
	return;
/* mages should be the most powerful charmers */

/*
    if((ch->class != CLASS_MAG) && (ch->class !=CLASS_BRD) && ch->class != CLASS_PRO) 
	if((ch->level-5) < victim->level)
		return;
    numcharmie = level/15 + 1;
*/
/*
    if(numcharmie == 0)
	numcharmie = 1;
*/

    ch->charmie = 0;

    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) && IS_AFFECTED(gch,AFF_CHARM))
	    ch->charmie++;
    }


    if(ch->class == CLASS_MAG)
//    || (ch->class == CLASS_BRD) || ch->class == CLASS_PRO)
	numcharmie = ch->level/10;
     else
        numcharmie = ch->level/15;

    if(numcharmie < 0)
	numcharmie = 1;

    if(ch->charmie >= numcharmie)
	return;


    if ( victim->master !=NULL )
	stop_follower( victim );
    add_follower( victim, ch );
    victim->leader = ch;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    ch->charmie++;
    affect_to_char( victim, &af );
    act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
    if ( ch != victim )
	act("$N looks at you with adoring eyes.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_chill_touch( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  6,  7,  8,	 9, 12, 13, 13, 13,
	14, 14, 14, 15, 15,	15, 16, 16, 16, 17,
	17, 17, 18, 18, 18,	19, 19, 19, 20, 20,
	20, 21, 21, 21, 22,	22, 22, 23, 23, 23,
	24, 24, 24, 25, 25,	25, 26, 26, 26, 27
    };
    AFFECT_DATA af;
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( !saves_spell( level, victim,DAM_COLD ) )
    {
	act("$n turns blue and shivers.",victim,NULL,NULL,TO_ROOM);
	af.where     = TO_AFFECTS;
	af.type      = sn;
	af.level     = level;
	af.duration  = 6;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_spell( ch, victim, dam, sn, DAM_COLD,TRUE );
    return;
}



void spell_colour_spray( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  0,  0,  0,	 0,  0,  0,  0,  0,
	30, 35, 40, 45, 50,	55, 55, 55, 56, 57,
	58, 58, 59, 60, 61,	61, 62, 63, 64, 64,
	65, 66, 67, 67, 68,	69, 70, 70, 71, 72,
	73, 73, 74, 75, 76,	76, 77, 78, 79, 79
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2,  dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_LIGHT) )
	dam /= 2;
    else
	spell_blindness(skill_lookup("blindness"),
	    level/2,ch,(void *) victim,TARGET_CHAR);

    damage_spell( ch, victim, dam, sn, DAM_LIGHT,TRUE );
    return;
}



void spell_continual_light(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *light;

    if (target_name[0] != '\0')  /* do a glow on some object */
    {
	light = get_obj_carry(ch,target_name , NULL);

	if (light == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}

	if (IS_OBJ_STAT(light,ITEM_GLOW))
	{
	    act("$p is already glowing.",ch,light,NULL,TO_CHAR);
	    return;
	}

	SET_BIT(light->extra_flags,ITEM_GLOW);
	act("$p glows with a white light.",ch,light,NULL,TO_ALL);
	return;
    }

    light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
    obj_to_room( light, ch->in_room );
    act( "$n twiddles $s thumbs and $p appears.",   ch, light, NULL, TO_ROOM );
    act( "You twiddle your thumbs and $p appears.", ch, light, NULL, TO_CHAR );
    return;
}



void spell_control_weather(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    if ( !str_cmp( target_name, "better" ) )
	weather_info.change += dice( level / 3, 4 );
    else if ( !str_cmp( target_name, "worse" ) )
	weather_info.change -= dice( level / 3, 4 );
    else  {
	send_to_char ("Do you want it to get better or worse?\n\r", ch );
	return;
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
    mushroom->value[0] = level / 2;
    mushroom->value[1] = level;
    obj_to_room( mushroom, ch->in_room );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR );
    return;
}
/*
void spell_create_rose( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *rose;
    rose = create_object(get_obj_index(OBJ_VNUM_ROSE), 0);
    act("$n has created a beautiful red rose.",ch,rose,NULL,TO_ROOM);
    send_to_char("You create a beautiful red rose.\n\r",ch);
    obj_to_char(rose,ch);
    return;
}
*/


void spell_create_spring(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *spring;

    spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
    spring->timer = level;
    obj_to_room( spring, ch->in_room );
    act( "$p flows from the ground.", ch, spring, NULL, TO_ROOM );
    act( "$p flows from the ground.", ch, spring, NULL, TO_CHAR );
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "It is unable to hold water.\n\r", ch );
	return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
	send_to_char( "It contains some other liquid.\n\r", ch );
	return;
    }

    water = UMIN(
		level * (weather_info.sky >= SKY_RAINING ? 4 : 2),
		obj->value[0] - obj->value[1]
		);

    if ( water > 0 )
    {
	obj->value[2] = LIQ_WATER;
	obj->value[1] += water;
	if ( !is_full_name( "water", obj->name ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    xprintf( buf, "%s water", obj->name );
	    free_string( obj->name );
	    obj->name = str_dup( buf );
	}
	act( "$p is filled.", ch, obj, NULL, TO_CHAR );
	check_look( obj );
    }

    return;
}



void spell_cure_blindness(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_blindness ) && !is_affected( victim, gsn_gouge ))
    {
	if (victim == ch)
	  send_to_char("You aren't blind.\n\r",ch);
	else
	  act("$N doesn't appear to be blinded.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_blindness))
    {
	send_to_char( "Your vision returns!\n\r", victim );
	act("$n is no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
    else if (check_dispel(level,victim,gsn_gouge))
    {
	send_to_char( "Your eyes are healed!\n\r", victim );
	act("$n's wounded eyes are magically cured.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}

void spell_mend(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_arrow ) )
    {
	if (victim == ch)
	  send_to_char("You weren't pierced by an arrow.\n\r",ch);
	else
	  act("$N doesn't appear pierced by an arrow.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_arrow))
    {
	send_to_char( "Your arrow has been pulled out!\n\r", victim );
	act("$n's arrow has been pulled out.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}

void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(3, 8) + level - 6;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

/* RT added to cure plague */
void spell_cure_disease( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;

    if ( !is_affected( victim, gsn_plague ) && !is_affected( victim, gsn_contagion)  )
    {
	if (victim == ch)
	  send_to_char("You aren't ill.\n\r",ch);
	else
	  act("$N doesn't appear to be diseased.",ch,NULL,victim,TO_CHAR);
	return;
    }
     if (check_dispel(level,victim,skill_lookup("contagion")))
    {
    	found = TRUE;
	act("$n looks relieved as $s sores vanish.",victim,NULL,NULL,TO_ROOM);
    }
  
    if (check_dispel(level,victim,skill_lookup("plague")))
    {
    	found = TRUE;
	act("$n looks relieved as $s sores vanish.",victim,NULL,NULL,TO_ROOM);
    }
    
    if (!found)
        send_to_char("spell failed.\n\r",ch);
}



void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(1, 8) + level / 3;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_poison ) )
    {
	if (victim == ch)
	  send_to_char("You aren't poisoned.\n\r",ch);
	else
	  act("$N doesn't appear to be poisoned.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_poison))
    {
	send_to_char("A warm feeling runs through your body.\n\r",victim);
	act("$n looks much better.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}

void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(2, 8) + level /2 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;

    /* deal with the object case first */
if (ch->class == 13)
 level += 7;

    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;
	if (IS_OBJ_STAT(obj,ITEM_EVIL))
	{
	    act("$p is already filled with evil.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (IS_OBJ_STAT(obj,ITEM_BLESS))
	{
	    AFFECT_DATA *paf;

	    paf = affect_find(obj->affected,skill_lookup("bless"));
	    if (!saves_dispel(level,paf != NULL ? paf->level : obj->level,0))
	    {
		if (paf != NULL)
		    affect_remove_obj(obj,paf);
		act("$p glows with a red aura.",ch,obj,NULL,TO_ALL);
		REMOVE_BIT(obj->extra_flags,ITEM_BLESS);
		return;
	    }
	    else
	    {
		act("The holy aura of $p is too powerful for you to overcome.",
		    ch,obj,NULL,TO_CHAR);
		return;
	    }
	}

	af.where        = TO_OBJECT;
	af.type         = sn;
	af.level        = level;
	af.duration     = 2 * level;
	af.location     = APPLY_SAVES;
	af.modifier     = +1;
	af.bitvector    = ITEM_EVIL;
	affect_to_obj(obj,&af);

	act("$p glows with a malevolent aura.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character curses */
    victim = (CHAR_DATA *) vo;
 if (ch->class == 13)
   {
    if (IS_AFFECTED(victim,AFF_CURSE)||saves_spell(level+5,victim,DAM_NEGATIVE))
	return;
  }
 else {
     if (IS_AFFECTED(victim,AFF_CURSE) ||saves_spell(level,victim,DAM_NEGATIVE))
        return;
     } 
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 2*level;
    af.location  = APPLY_HITROLL;
 if (ch->class == 13)  
  af.modifier  = -1 * (level / 7);
 else
   af.modifier  = -1 * (level / 8); 
   af.bitvector = AFF_CURSE;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = level / 8;
    affect_to_char( victim, &af );

    send_to_char( "You feel unclean.\n\r", victim );
    if ( ch != victim )
	act("$N looks very uncomfortable.",ch,NULL,victim,TO_CHAR);
    return;
}


/* RT replacement demonfire spell */

void spell_demonfire(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && !IS_EVIL(ch) )
    {
	victim = ch;
	send_to_char("The demons turn upon you!\n\r",ch);
    }

    ch->alignment = UMAX(-1000, ch->alignment - 50);

    if (victim != ch)
    {
	act("$n calls forth the demons of Hell upon $N!",
	    ch,NULL,victim,TO_ROOM);
	act("$n has assailed you with the demons of Hell!",
	    ch,NULL,victim,TO_VICT);
	send_to_char("You conjure forth the demons of hell!\n\r",ch);
    }
    dam = dice( level, 10 );
    if ( saves_spell( level, victim,DAM_NEGATIVE) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);
    spell_curse(gsn_curse, 3 * level / 4, ch, (void *) victim,TARGET_CHAR);
}

void spell_detect_evil( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_EVIL) )
    {
	if (victim == ch)
	  send_to_char("You can already sense evil.\n\r",ch);
	else
	  act("$N can already detect evil.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_EVIL;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_detect_good( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_GOOD) )
    {
	if (victim == ch)
	  send_to_char("You can already sense good.\n\r",ch);
	else
	  act("$N can already detect good.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_GOOD;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

/*
void spell_acute_vision( int sn, int level, CHAR_DATA *ch, void *vo, int target )
 {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, ACUTE_VISION) )
    {
        if (victim == ch)
          send_to_char("Your vision is already acute. \n\r",ch);
        else
          act("$N already sees acutely.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.where		= TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ACUTE_VISION;
    affect_to_char( victim, &af );
    send_to_char( "Your vision sharpens.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}
*/

void spell_detect_hidden(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_HIDDEN) )
    {
	if (victim == ch)
	  send_to_char("You are already as alert as you can be. \n\r",ch);
	else
	  act("$N can already sense hidden lifeforms.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = DETECT_HIDDEN;
    affect_to_char( victim, &af );
    send_to_char( "Your awareness improves.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int ass;

    if ( CAN_DETECT(victim, DETECT_INVIS) )
    {
	if (victim == ch)
	  send_to_char("You can already see invisible.\n\r",ch);
	else
	  act("$N can already see invisible things.",ch,NULL,victim,TO_CHAR);
	return;
    }

    ass = (ch->class == class_lookup("assassin")) ? level/2 : level;
    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level     = ass;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_INVIS;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_MAGIC) )
    {
	if (victim == ch)
	  send_to_char("You can already sense magical auras.\n\r",ch);
	else
	  act("$N can already detect magic.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_MAGIC;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
    {
	if ( obj->value[3] != 0 )
	    send_to_char( "You smell poisonous fumes.\n\r", ch );
	else
	    send_to_char( "It looks delicious.\n\r", ch );
    }
    else
    {
	send_to_char( "It doesn't look poisoned.\n\r", ch );
    }

    return;
}



void spell_dispel_evil( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && IS_EVIL(ch) )
	victim = ch;

    if ( IS_GOOD(victim) )
    {
	act( "Gods protects $N.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (victim->hit > (ch->level * 4))
      dam = dice( level, 4 );
    else
      dam = UMAX(victim->hit, dice(level,4));
    if ( saves_spell( level, victim,DAM_HOLY) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_HOLY ,TRUE);
    return;
}


void spell_dispel_good( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && IS_GOOD(ch) )
	victim = ch;

    if ( IS_EVIL(victim) )
    {
	act( "$N is protected by $S evil.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (victim->hit > (ch->level * 4))
      dam = dice( level, 4 );
    else
      dam = UMAX(victim->hit, dice(level,4));
    if ( saves_spell( level, victim,DAM_NEGATIVE) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);
    return;
}


/* modified for enhanced use */

void spell_dispel_magic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;

    if (saves_spell(level, victim,DAM_OTHER))
    {
	send_to_char( "You feel a brief tingling sensation.\n\r",victim);
	send_to_char( "You failed.\n\r", ch);
	return;
    }

    /* begin running through the spells */

    if (check_dispel(level,victim,skill_lookup("armor")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("bless")))
	found = TRUE;

 /*  if (check_dispel(level,victim,skill_lookup("blindness")))
    {
	found = TRUE;
	act("$n is no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }*/

    if (check_dispel(level,victim,skill_lookup("calm")))
    {
	found = TRUE;
	act("$n no longer looks so peaceful...",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("change sex")))
    {
	found = TRUE;
	act("$n looks more like $mself again.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("charm person")))
    {
        found = TRUE;
        act("$n regains $s free will.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("chill touch")))
    {
        found = TRUE;
        act("$n looks warmer.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("curse")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("dark vision")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("detect good")))
	found = TRUE;
/*
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
        found = TRUE;
*/ 
    if (check_dispel(level,victim,skill_lookup("detect invis")))
        found = TRUE;
 
        found = TRUE;
/* 
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
        found = TRUE;
*/
    if (check_dispel(level,victim,skill_lookup("detect magic")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("faerie fire")))
    {
        act("$n's pink outline fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("faerie fog")))
    {
        act("$n's purple outline fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("fly")))
    {
        act("$n falls to the ground!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("frenzy")))
    {
        act("$n no longer looks so wild.",victim,NULL,NULL,TO_ROOM);;
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("giant strength")))
    {
        act("$n no longer looks so mighty.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("haste")))
    {
        act("$n is no longer moving so quickly.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("infravision")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("invis")))
    {
        act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    
/*    if (check_dispel(level,victim,skill_lookup("improved invis")))
    {
        act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }*/
 
    if (check_dispel(level,victim,skill_lookup("mass invis")))
    {
        act("$n fades into existance.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("pass door")))
        found = TRUE;
 

    if (check_dispel(level,victim,skill_lookup("protection evil")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("protection good")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("sanctuary")))
    {
        act("The white aura around $n's body vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (IS_AFFECTED(victim,AFF_SANCTUARY) 
	&& !saves_dispel(level, victim->level,-1)
	&& !is_affected(victim,skill_lookup("sanctuary")))
    {
	REMOVE_BIT(victim->affected_by,AFF_SANCTUARY);
        act("The white aura around $n's body vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("shield")))
    {
        act("The shield protecting $n vanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("sleep")))
        found = TRUE;

    if (check_dispel(level,victim,skill_lookup("slow")))
    {
        act("$n is no longer moving so slowly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("spellbane")))
	found = TRUE;

    if (check_dispel(level,victim,skill_lookup("stone skin")))
    {
        act("$n's skin regains its normal texture.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("ice storm")))
    {
	act("$n feels warmer.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("fire storm")))
    {
	act("$n feels cooler.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("lightning storm")))
    {
	act("$n's muscle starts responding.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("meteor swarm")))
    {
	act("$n recover from the blast.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("divine word")))
    {
	act("$n feels a lot better.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("mental knife")))
    {
	act("$n feels smarter",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("weaken")))
    {
        act("$n looks stronger.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("wither")))
    {
	act("$n looks revitalized.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("detect undead")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("vampiric aura")))
    {
	act("$n seems less charismatic.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("cloak of fear")))
    {
	act("$n suddenly seems less terrifying.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("cloak of fear")))
    {
	act("$n suddenly seems less terrifying.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("decay")))
    {
	act("$n's armor returns to its original state.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("protection undead")))
        found = TRUE;

     if (check_dispel(level,victim,skill_lookup("bone armor")))
    {
	act("$n's bone armor crumbles and fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    
    if (check_dispel(level,victim,skill_lookup("malison")))
        found = TRUE;

       
    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
	return;
}

void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    send_to_char( "The earth trembles beneath your feet!\n\r", ch );
    act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && !is_safe_spell(ch,vch,TRUE) && !is_same_group(ch, vch))
	      if ( is_safe(ch, vch) )
		continue;
	    if ( ch == vch )
		continue;
		if (IS_AFFECTED(vch,AFF_FLYING))
		    damage_spell(ch,vch,0,sn,DAM_BASH,TRUE);
		else
		    damage_spell( ch,vch,level + dice(2, 8), sn, DAM_BASH,TRUE);
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "The earth trembles and shivers.\n\r", vch );
    }

    return;
}

void spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo,int 
target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
//    AFFECT_DATA *paf;
    int result, fail;
    int ac_bonus, added;
    bool ac_found = FALSE;
    AFFECT_DATA *paf, *paf_next;

    if (obj->item_type != ITEM_ARMOR)
    {
	send_to_char("That isn't an armor.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
	return;
    }

    /* this means they have no bonus */
    ac_bonus = 0;
    fail = 15;	/* base 25% chance of failure */

    /* find the bonuses */

    if (!obj->enchanted)
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	{
	    if ( paf->location == APPLY_AC )
	    {
	    	ac_bonus = paf->modifier;
		ac_found = TRUE;
	    	fail += (ac_bonus * ac_bonus/2);
 	    }

	    else  /* things get a little harder */
	    	fail += 20;
    	}
 
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_AC )
  	{
	    ac_bonus = paf->modifier;
	    ac_found = TRUE;
	    fail += 3 * (ac_bonus * ac_bonus);
	}

	else /* things get a little harder */
	    fail += 20;
    }

    /* apply other modifiers */
    fail -= level;

    if (IS_OBJ_STAT(obj,ITEM_BLESS))
	fail -= 15;
    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,85);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 10))  /* item destroyed */
    {
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_CHAR);
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 7)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows brightly, then fades, and is badly damaged.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

	/* clear all flags */
	obj->extra_flags = 0;
	return;
    }

   if (result < (fail / 3)) /* item disenchanted */
    {
     for ( paf = obj->affected; paf != NULL; paf = paf_next )
       {
          paf_next    = paf->next;
           
           if (paf->location == APPLY_AC)
            {
             affect_remove_obj( obj, paf );
            }
       }
     	act("$p glows brightly, then fades, and is damaged.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;
	return;
    }





    if ( result <= fail )  /* failed, no bad result */
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
	AFFECT_DATA *af_new;
	obj->enchanted = TRUE;

	for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next) 
	{
	    af_new = new_affect();
	
	    af_new->next = obj->affected;
	    obj->affected = af_new;

	    af_new->where	= paf->where;
	    af_new->type 	= UMAX(0,paf->type);
	    af_new->level	= paf->level;
	    af_new->duration	= paf->duration;
	    af_new->location	= paf->location;
	    af_new->modifier	= paf->modifier;
	    af_new->bitvector	= paf->bitvector;
	}
    }

    if (result <= (90 - level/5))  /* success! */
    {
	act("$p shimmers with a gold aura.",ch,obj,NULL,TO_CHAR);
	act("$p shimmers with a gold aura.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = -5;
    }
    
    else  /* exceptional enchant */
    {
	act("$p glows a brillant gold!",ch,obj,NULL,TO_CHAR);
	act("$p glows a brillant gold!",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags,ITEM_MAGIC);
	SET_BIT(obj->extra_flags,ITEM_GLOW);
	added = -7;
    }
		
    /* now add the enchantments */

    if (obj->level < LEVEL_HERO)
	obj->level = UMIN(LEVEL_HERO - 1,obj->level + 1);

    if (ac_found)
    {
	for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_AC)
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
	    }
	}
    }
    else /* add a new affect */
    {
 	paf = new_affect();

	paf->where	= TO_OBJECT;
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_AC;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->affected;
    	obj->affected	= paf;
    }

}




void spell_enchant_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
//    AFFECT_DATA *paf;
    int result, fail;
    int hit_bonus, dam_bonus, added, dam;
    bool hit_found = FALSE, dam_found = FALSE;
    AFFECT_DATA *paf, *paf_next;

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That isn't a weapon.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
	return;
    }

    /* this means they have no bonus */
    hit_bonus = 0;
    dam_bonus = 0;
    fail = 15;	/* base 25% chance of failure */

    /* find the bonuses */

    if (!obj->enchanted)
    	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    	{
            if ( paf->location == APPLY_HITROLL )
            {
	    	hit_bonus = paf->modifier;
		hit_found = TRUE;
	    	fail += 2 * (hit_bonus * hit_bonus);
 	    }

	    else if (paf->location == APPLY_DAMROLL )
	    {
	    	dam_bonus = paf->modifier;
		dam_found = TRUE;
	    	fail += 2 * (dam_bonus * dam_bonus);
	    }

	    else  /* things get a little harder */
	    	fail += 25;
    	}
 
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_HITROLL )
  	{
	    hit_bonus = paf->modifier;
	    hit_found = TRUE;
	    fail += 2 * (hit_bonus * hit_bonus);
	}

	else if (paf->location == APPLY_DAMROLL )
  	{
	    dam_bonus = paf->modifier;
	    dam_found = TRUE;
	    fail += 2 * (dam_bonus * dam_bonus);
	}

	else /* things get a little harder */
	    fail += 25;
    }

    /* apply other modifiers */
    fail -= 3 * level/2;

    if (IS_OBJ_STAT(obj,ITEM_BLESS))
	fail -= 15;
    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,95);

    result = number_percent();

    dam = number_range(1,obj->level);
    /* the moment of truth */
    if (result < (fail / 6))  /* item destroyed */
    {
	act("$p shivers violently and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p shivers violently and explodeds!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	damage_spell( ch, ch, dam, TYPE_HIT, DAM_FIRE, FALSE);
	return;
    }


    if (result < (fail / 5)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows {Cbrightly, then fades, and is badly damaged.",ch,obj,NULL,TO_CHAR);
	act("$p glows {Cbrightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next; 
	    free_affect(paf);
	}
	obj->affected = NULL;

	/* clear all flags */
	obj->extra_flags = 0;
	return;
    }

   if (result < (fail / 3)) /* item disenchanted */
    {
     for ( paf = obj->affected; paf != NULL; paf = paf_next )
       {
          paf_next    = paf->next;
           
           if ( ( paf->location == APPLY_DAMROLL ) || (paf->location == APPLY_HITROLL) )
            {
             affect_remove_obj( obj, paf );
            }
       }
     	act("$p glows brightly, then fades, and is damaged.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;
	return;
    }


    if ( result <= fail )  /* failed, no bad result */
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
	AFFECT_DATA *af_new;
	obj->enchanted = TRUE;

	for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next) 
	{
	    af_new = new_affect();
	
	    af_new->next = obj->affected;
	    obj->affected = af_new;

	    af_new->where	= paf->where;
	    af_new->type 	= UMAX(0,paf->type);
	    af_new->level	= paf->level;
	    af_new->duration	= paf->duration;
	    af_new->location	= paf->location;
	    af_new->modifier	= paf->modifier;
	    af_new->bitvector	= paf->bitvector;
	}
    }

    if (result <= (100 - level/5))  /* success! */
    {
	act("$p glows blue.",ch,obj,NULL,TO_CHAR);
	act("$p glows blue.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = 2;
    }
    
    else  /* exceptional enchant */
    {
	act("$p glows a brillant blue!",ch,obj,NULL,TO_CHAR);
	act("$p glows a brillant blue!",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags,ITEM_MAGIC);
	SET_BIT(obj->extra_flags,ITEM_GLOW);
	added = 3;
    }
		
    /* now add the enchantments */ 

    if (obj->level < LEVEL_HERO - 1)
	obj->level = UMIN(LEVEL_HERO - 1,obj->level + 1);

    if (dam_found)
    {
	for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_DAMROLL)
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
		if (paf->modifier > 4)
		    SET_BIT(obj->extra_flags,ITEM_HUM);
	    }
	}
    }
    else /* add a new affect */
    {
	paf = new_affect();

	paf->where	= TO_OBJECT;
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_DAMROLL;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->affected;
    	obj->affected	= paf;
    }

    if (hit_found)
    {
        for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
            if ( paf->location == APPLY_HITROLL)
            {
		paf->type = sn;
                paf->modifier += added;
                paf->level = UMAX(paf->level,level);
                if (paf->modifier > 4)
                    SET_BIT(obj->extra_flags,ITEM_HUM);
            }
	}
    }
    else /* add a new affect */
    {
        paf = new_affect();
 
        paf->type       = sn;
        paf->level      = level;
        paf->duration   = -1;
        paf->location   = APPLY_HITROLL;
        paf->modifier   =  added;
        paf->bitvector  = 0;
        paf->next       = obj->affected;
        obj->affected   = paf;
    }

}



/*
 * Drain XP, MANA, HP.
 * Caster gains HP.
 */
void spell_energy_drain( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

/*
    if (victim != ch)
        ch->alignment = UMAX(-1000, ch->alignment - 50);
*/
 if (ch->class == 13)
  level += 7;
  
    if ( saves_spell( level, victim,DAM_NEGATIVE) )
    {
	send_to_char("You feel a momentary chill.\n\r",victim);
	return;
    }


    if ( victim->level <= 2 )
    {
	dam		 = ch->hit + 1;
    }
    else
    {
	gain_exp( victim, 0 - number_range( level/2, 3 * level / 2 ) );
     if (ch->class == 13)
      {
	victim->mana	/= 2.5;
	victim->move	/= 2.5;
	dam		 = dice(2, level);
     }
   else {
            victim->mana    /= 2;
        victim->move    /= 2;
        dam              = dice(1, level);
     }
	ch->hit		+= dam;
    }

    send_to_char("You feel your life slipping away!\n\r",victim);
    send_to_char("Wow....what a rush!\n\r",ch);
    damage_spell( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);

    return;
}

void spell_hellfire( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

  dam = dice(level, 7);

  damage_spell(ch,victim,dam,sn,DAM_FIRE, TRUE);

}

void spell_iceball( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *tmp_vict;
    CHAR_DATA *tmp_next;
    char buf[MAX_STRING_LENGTH];

    static const sh_int dam_each[] =
    {
          0,
          0,   0,   0,   0,   0,          0,   0,   0,   0,   0,
          0,   0,   0,   0,  45,         50,  55,  60,  65,  70,
         75,  80,  85,  90,  95,         97,  99, 101, 101, 103,
        105, 107, 109, 111, 113,        115, 117, 119, 121, 123,
        125, 127, 129, 131, 133,        135, 137, 139, 141, 143
    };
    int dam;
    int movedam;

    level       = UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level       = UMAX(0, level);
    dam         = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    movedam     = number_range( ch->level, 2 * ch->level );

  for (tmp_vict = ch->in_room->people;tmp_vict != NULL;
       tmp_vict = tmp_next)
    {
      tmp_next = tmp_vict->next_in_room;

      if ( !is_safe_spell(ch,tmp_vict,TRUE))
        {
          if (!IS_NPC(ch) && tmp_vict != ch &&
              ch->fighting != tmp_vict && tmp_vict->fighting != ch &&
              (IS_SET(tmp_vict->affected_by,AFF_CHARM) || !IS_NPC(tmp_vict)))
            {
            if (!can_see(tmp_vict, ch))
                do_yell(tmp_vict, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you sorcerous dog!",
	(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))? ch->doppel->name : ch->name);
                 do_yell(tmp_vict,buf);
              }
          }

          if (saves_spell(level,tmp_vict, DAM_COLD))
            dam /= 2;
          damage_spell( ch, tmp_vict, dam, sn, DAM_COLD, TRUE );
          tmp_vict->move -= UMIN(tmp_vict->move,movedam);

        }
    }
}

void spell_fireball( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	  1,
	  1,   1,   1,   1,   1,	 20, 20,  25,  25,  25,
	 25,  25,  25,  25,  30,	 35,  40,  45,  50,  55,
	 60,  65,  70,  75,  80,	 82,  84,  86,  88,  90,
	 92,  94,  96,  98, 100,	102, 104, 106, 108, 110,
	112, 114, 116, 118, 120,	122, 124, 126, 128, 130
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim, DAM_FIRE) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_FIRE ,TRUE);
    return;
}


void spell_fireproof(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA af;

    if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
    {
	act("$p is already protected from burning.",ch,obj,NULL,TO_CHAR);
	return;
    }

    af.where     = TO_OBJECT;
    af.type      = sn;
    af.level     = level;
    af.duration  = number_fuzzy(level / 4);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ITEM_BURN_PROOF;

    affect_to_obj(obj,&af);

    act("You protect $p from fire.",ch,obj,NULL,TO_CHAR);
    act("$p is surrounded by a protective aura.",ch,obj,NULL,TO_ROOM);
}



void spell_flamestrike( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice(6 + level / 2, 8);
    if ( saves_spell( level, victim,DAM_FIRE) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_FIRE ,TRUE);
    return;
}



void spell_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int ass;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) )
	return;

    ass = (ch->class == class_lookup("assassin")) ? level/2 : level;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = ass;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = 2 * level;
    af.bitvector = AFF_FAERIE_FIRE;
    affect_to_char( victim, &af );
    send_to_char( "You are surrounded by a pink outline.\n\r", victim );
    act( "$n is surrounded by a pink outline.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_faerie_fog( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *ich;
    AFFECT_DATA af;

    act( "$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You conjure a cloud of purple smoke.\n\r", ch );

    for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
    {
	if (ich->invis_level > 0)
	    continue;

	if ( ich == ch || saves_spell( level+5, ich,DAM_OTHER) )
	    continue;

	affect_strip ( ich, gsn_invis			);
	affect_strip ( ich, gsn_spell_illusion_fields	);
	affect_strip ( ich, gsn_mass_invis		);
	REMOVE_BIT   ( ich->affected_by, AFF_HIDE	);
	REMOVE_BIT   ( ich->affected_by, AFF_FADE	);
	REMOVE_BIT   ( ich->affected_by, AFF_INVISIBLE	);

        /* An elf sneaks eternally */
        if ( IS_NPC(ich) || !IS_SET(race_table[ich->race].aff,AFF_SNEAK) )
          {
            affect_strip ( ich, gsn_sneak                       );
            REMOVE_BIT   ( ich->affected_by, AFF_SNEAK  );
          }

    if ( !(IS_AFFECTED(ich, AFF_FAERIE_FOG)) )

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_FAERIE_FOG;
    affect_to_char( ich, &af );

	act( "$n is revealed!", ich, NULL, NULL, TO_ROOM );
	send_to_char( "You are revealed!\n\r", ich );
    }


    return;
}

void spell_floating_disc( int sn, int level,CHAR_DATA *ch,void *vo,int target )
{
    OBJ_DATA *disc, *floating;

    floating = get_eq_char(ch,WEAR_FLOAT);
    if (floating != NULL && IS_OBJ_STAT(floating,ITEM_NOREMOVE))
    {
    	if(floating->pIndexData->vnum == OBJ_VNUM_DISC)
    	 {
    	  floating->timer = 1;
    	  act("$n's floating disc wavers.",ch,NULL,NULL,TO_ROOM);
    	  send_to_char("Your floating disc is drained of energy.\n\r",ch);
    	  return;
    	 }
	act("You can't remove $p.",ch,floating,NULL,TO_CHAR);
	return;
    }

    disc = create_object(get_obj_index(OBJ_VNUM_DISC), 0);
    disc->value[0]	= ch->level * 10; /* 10 pounds per level capacity */
    disc->value[3]	= ch->level * 5; /* 5 pounds per level max per item */
    disc->timer		= ch->level * 2 - number_range(0,level / 2); 

    act("$n has created a floating black disc.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You create a floating disc.\n\r",ch);
    obj_to_char(disc,ch);
    wear_obj(ch,disc,TRUE);
    return;
}


void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FLYING) )
    {
	if (victim == ch)
	  send_to_char("You are already airborne.\n\r",ch);
	else
	  act("$N doesn't need your help to fly.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level + 3;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );
    send_to_char( "Your feet rise off the ground.\n\r", victim );
    act( "$n's feet rise off the ground.", victim, NULL, NULL, TO_ROOM );
    return;
}

/* RT clerical berserking spell */

void spell_frenzy(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
	if (victim == ch)
	  send_to_char("You are already in a frenzy.\n\r",ch);
	else
	  act("$N is already in a frenzy.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim,skill_lookup("calm")))
    {
	if (victim == ch)
	  send_to_char("Why don't you just relax for a while?\n\r",ch);
	else
	  act("$N doesn't look like $e wants to fight anymore.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if ((IS_GOOD(ch) && !IS_GOOD(victim)) ||
	(IS_NEUTRAL(ch) && !IS_NEUTRAL(victim)) ||
	(IS_EVIL(ch) && !IS_EVIL(victim))
       )
    {
	act("Your god doesn't seem to like $N",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type 	 = sn;
    af.level	 = level;
    af.duration	 = level / 3;
    af.modifier  = level / 6;
    af.bitvector = 0;

    af.location  = APPLY_HITROLL;
    affect_to_char(victim,&af);

    af.location  = APPLY_DAMROLL;
    affect_to_char(victim,&af);

    af.modifier  = 10 * (level / 12);
    af.location  = APPLY_AC;
    affect_to_char(victim,&af);

    send_to_char("You are filled with holy wrath!\n\r",victim);
    act("$n gets a wild look in $s eyes!",victim,NULL,NULL,TO_ROOM);
}

/* RT ROM-style gate */
    
void spell_gate( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    bool gate_pet;
/* Disabled by andeus
    if (current_time - ch->last_fight_time < FIGHT_DELAY_TIME)
      {
	send_to_char( "You can't concentrate enough.\n\r", ch );
	return;
      }
*/ 
	if ((ch->pcdata->quest !=NULL))
	{
		QUEST_DATA *quest = ch->pcdata->quest;
		if (quest->protection)	
		{
			send_to_char( "You are in a quest. and gate is disabled.\n\r", ch );
			return;			
		}
	}

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   strstr( ch->in_room->area->builders, "Unlinked" ) 
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(ch->in_room->room_flags, ROOM_ARENA )
    ||   IS_SET(victim->in_room->room_flags, ROOM_ARENA )
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area )
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

if ( strstr( victim->in_room->area->builders, "Unlinked" ) )
{
     send_to_char( "You can't gate to areas that aren't linked yet!\n\r",ch );
     return;
}

    if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
	gate_pet = TRUE;
    else
	gate_pet = FALSE;

    act("$n steps through a gate and vanishes.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You step through a gate and vanish.\n\r",ch);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("$n has arrived through a gate.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

    if (gate_pet)
    {
	act("$n steps through a gate and vanishes.",ch->pet,NULL,NULL,TO_ROOM);
	send_to_char("You step through a gate and vanish.\n\r",ch->pet);
	char_from_room(ch->pet);
	char_to_room(ch->pet,victim->in_room);
	act("$n has arrived through a gate.",ch->pet,NULL,NULL,TO_ROOM);
	do_look(ch->pet,"auto");
    }
}



void spell_giant_strength(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already as strong as you can get!\n\r",ch);
	else
	  act("$N can't get any stronger.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "Your muscles surge with heightened power!\n\r", victim );
    act("$n's muscles surge with heightened power.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_harm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 14 );
    if ( saves_spell( level, victim, DAM_FIRE ) )
        dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_HARM ,TRUE);
    return;
}

/* RT haste spell */

void spell_haste( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
    if ( is_affected( victim, sn ) || IS_AFFECTED(victim,AFF_HASTE)
    ||   IS_SET(victim->off_flags,OFF_FAST))
    {
	if (victim == ch)
	  send_to_char("You can't move any faster!\n\r",ch);
	else
	  act("$N is already moving as fast as $E can.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    if (IS_AFFECTED(victim,AFF_SLOW))
    {
	if (!check_dispel(level,victim,skill_lookup("slow")))
	{
	    if (victim != ch)
		send_to_char("Spell failed.\n\r",ch);
	    send_to_char("You feel momentarily faster.\n\r",victim);
	    return;
	}
	act("$n is moving less slowly.",victim,NULL,NULL,TO_ROOM);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    if (victim == ch)
      af.duration  = level/2;
    else
      af.duration  = level/4;
    af.location  = APPLY_DEX;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    af.bitvector = AFF_HASTE;
    affect_to_char( victim, &af );
    send_to_char( "You feel yourself moving more quickly.\n\r", victim );
    act("$n is moving more quickly.",victim,NULL,NULL,TO_ROOM);
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->hit = UMIN( victim->hit + 175, victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_heat_metal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose, *obj_next;
    int dam = 0;
    bool fail = TRUE;

   if (!saves_spell(level + 2,victim,DAM_FIRE)
   &&  !IS_SET(victim->imm_flags,IMM_FIRE))
   {
	for ( obj_lose = victim->carrying;
	      obj_lose != NULL;
	      obj_lose = obj_next)
	{
	    obj_next = obj_lose->next_content;
	    if ( number_range(1,2 * level) > obj_lose->level
	    &&   !saves_spell(level,victim,DAM_FIRE)
	    &&   is_metal(obj_lose)
	    &&   (obj_lose->pIndexData->limit <= 0)
	    &&   !IS_OBJ_STAT(obj_lose,ITEM_BURN_PROOF))
	    {
		switch ( obj_lose->item_type )
		{
		case ITEM_ARMOR:
		if (obj_lose->wear_loc != -1) /* remove the item */
		{
		    if (can_drop_obj(victim,obj_lose)
		    &&  (obj_lose->weight / 10) <
			number_range(1,2 * get_curr_stat(victim,STAT_DEX))
		    &&  remove_obj( victim, obj_lose->wear_loc, TRUE ))
		    {
			act("$n yelps and throws $p to the ground!",
			    victim,obj_lose,NULL,TO_ROOM);
			act("You remove and drop $p before it burns you.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 3);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* stuck on the body! ouch! */
		    {
			act("Your skin is seared by $p!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level));
			fail = FALSE;
		    }

		}
		else /* drop it if we can */
		{
		    if (can_drop_obj(victim,obj_lose))
		    {
			act("$n yelps and throws $p to the ground!",
			    victim,obj_lose,NULL,TO_ROOM);
			act("You and drop $p before it burns you.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 6);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* cannot drop */
		    {
			act("Your skin is seared by $p!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 2);
			fail = FALSE;
		    }
		}
		break;
		case ITEM_WEAPON:
		if (obj_lose->wear_loc != -1) /* try to drop it */
		{
		    if (IS_WEAPON_STAT(obj_lose,WEAPON_FLAMING))
			continue;

		    if (can_drop_obj(victim,obj_lose)
		    &&  remove_obj(victim,obj_lose->wear_loc,TRUE))
		    {
			act("$n is burned by $p, and throws it to the ground.",
			    victim,obj_lose,NULL,TO_ROOM);
			send_to_char(
			    "You throw your red-hot weapon to the ground!\n\r",
			    victim);
			dam += 1;
			obj_from_char(obj_lose);
			obj_to_room(obj_lose,victim->in_room);
			fail = FALSE;
		    }
		    else /* YOWCH! */
		    {
			send_to_char("Your weapon sears your flesh!\n\r",
			    victim);
			dam += number_range(1,obj_lose->level);
			fail = FALSE;
		    }
		}
		else /* drop it if we can */
		{
		    if (can_drop_obj(victim,obj_lose))
		    {
			act("$n throws a burning hot $p to the ground!",
			    victim,obj_lose,NULL,TO_ROOM);
			act("You and drop $p before it burns you.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 6);
			obj_from_char(obj_lose);
			obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
		    }
		    else /* cannot drop */
		    {
			act("Your skin is seared by $p!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 2);
			fail = FALSE;
		    }
		}
		break;
		}
	    }
	}
    }
    if (fail)
    {
	send_to_char("Your spell had no effect.\n\r", ch);
	send_to_char("You feel momentarily warmer.\n\r",victim);
    }
    else /* damage! */
    {
	if (saves_spell(level,victim,DAM_FIRE))
	    dam = 2 * dam / 3;
	damage_spell(ch,victim,dam,sn,DAM_FIRE,TRUE);
    }
}

/* RT really nasty high-level attack spell */
void spell_holy_word(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int dam;
    int bless_num, curse_num, frenzy_num;

    bless_num = skill_lookup("bless");
    curse_num = skill_lookup("curse");
    frenzy_num = skill_lookup("frenzy");

    act("$n utters a word of divine power!",ch,NULL,NULL,TO_ROOM);
    send_to_char("You utter a word of divine power.\n\r",ch);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;

	if ((IS_GOOD(ch) && IS_GOOD(vch)) ||
	    (IS_EVIL(ch) && IS_EVIL(vch)) ||
	    (IS_NEUTRAL(ch) && IS_NEUTRAL(vch)) )
	{
	  send_to_char("You feel full more powerful.\n\r",vch);
	  spell_frenzy(frenzy_num,level,ch,(void *) vch,TARGET_CHAR);
	  spell_bless(bless_num,level,ch,(void *) vch,TARGET_CHAR);
	}

	else if ((IS_GOOD(ch) && IS_EVIL(vch)) ||
		 (IS_EVIL(ch) && IS_GOOD(vch)) )
	{
          if (!is_safe_spell(ch,vch,TRUE))
          {
            if (!IS_NPC(ch) && vch != ch &&
                ch->fighting != vch && vch->fighting != ch &&
                (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
              {
            if (!can_see(vch, ch))
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you sorcerous dog!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

            spell_curse(curse_num,level,ch,(void *) vch, TARGET_CHAR);
            send_to_char("You are struck down!\n\r",vch);
            dam = dice(level,6);
            damage_spell(ch,vch,dam,sn,DAM_ENERGY, TRUE);
          }
        }

	else if (IS_NEUTRAL(ch))
	{
          if (!is_safe_spell(ch,vch,TRUE))
          {
            if (!IS_NPC(ch) && vch != ch &&
                ch->fighting != vch && vch->fighting != ch &&
                (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
              {
            if (!can_see(vch, ch))
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you sorcerous dog!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

            spell_curse(curse_num,level/2,ch,(void *) vch, TARGET_CHAR);
            send_to_char("You are struck down!\n\r",vch);
            dam = dice(level,4);
            damage_spell(ch,vch,dam,sn,DAM_ENERGY, TRUE);
	  }
	}
    }

    send_to_char("You feel drained.\n\r",ch);
    gain_exp( ch, -1 * number_range(1,10) * 5);
    ch->move /= (4/3);
    ch->hit /= (4/3);
}

void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;


    if (ch->level+15 < obj->level)
      {
    send_to_char("Your not strong enuff to identify is object.",ch);
    return;
    }
    xprintf( buf,
	"Object '%s' is type %s, extra flags %s.\n\rrace flags %s,\n\rclass flags %s,\n\rWeight is %d, value is %d, level is %d\n\rMaterial is %s.\n",
	obj->name,
	item_type_name( obj ),
	extra_bit_name( obj->extra_flags ),
	race_bit_name(obj->race_flags),
	class_bit_name(obj->class_flags),
	obj->weight,
	obj->cost,
	obj->level,
	obj->material
	);
    send_to_char( buf, ch );

    switch ( obj->item_type )
    {
    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_PILL:
	xprintf( buf, "Level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	{
	    send_to_char(" '",ch);
	    send_to_char(skill_table[obj->value[4]].name,ch);
	    send_to_char("'",ch);
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	xprintf( buf, "Has %d/%d charges of level %d",
	    obj->value[2], obj->value[1],obj->value[0] );
	send_to_char( buf, ch );
      
	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_DRINK_CON:
        xprintf(buf,"It holds %s-colored %s.\n\r",
	    liq_table[obj->value[2]].liq_color,
            liq_table[obj->value[2]].liq_name);
        send_to_char(buf,ch);
        break;

    case ITEM_CONTAINER:
	xprintf(buf,"Capacity: %d#  Maximum weight: %d#  flags: %s\n\r",
	    obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
	send_to_char(buf,ch);
	if (obj->value[4] != 100)
	{
	    xprintf(buf,"Weight multiplier: %d%%\n\r",
		obj->value[4]);
	    send_to_char(buf,ch);
	}
	break;
		
    case ITEM_WEAPON:
 	send_to_char("Weapon type is ",ch);
	switch (obj->value[0])
	{
	    case(WEAPON_EXOTIC) : send_to_char("exotic.\n\r",ch);	break;
	    case(WEAPON_SWORD)  : send_to_char("sword.\n\r",ch);	break;	
	    case(WEAPON_DAGGER) : send_to_char("dagger.\n\r",ch);	break;
	    case(WEAPON_SPEAR)	: send_to_char("spear/staff.\n\r",ch);	break;
	    case(WEAPON_MACE) 	: send_to_char("mace/club.\n\r",ch);	break;
	    case(WEAPON_AXE)	: send_to_char("axe.\n\r",ch);		break;
	    case(WEAPON_FLAIL)	: send_to_char("flail.\n\r",ch);	break;
	    case(WEAPON_WHIP)	: send_to_char("whip.\n\r",ch);		break;
	    case(WEAPON_POLEARM): send_to_char("polearm.\n\r",ch);	break;
	    case(WEAPON_BOW)	: send_to_char("bow.\n\r",ch);		break;
 	    case(WEAPON_ARROW)  : send_to_char("arrow.\n\r",ch); break;
            case(WEAPON_LANCE)  : send_to_char("lance.\n\r",ch); break;
	    default		: send_to_char("unknown.\n\r",ch);	break;
 	}
	if (obj->pIndexData->new_format)
	    xprintf(buf,"Damage is %dd%d (average %d).\n\r",
		obj->value[1],obj->value[2],
		(1 + obj->value[2]) * obj->value[1] / 2);
	else
	    xprintf( buf, "Damage is %d to %d (average %d).\n\r",
	    	obj->value[1], obj->value[2],
	    	( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );
        if (obj->value[4])  /* weapon flags */
        {
            xprintf(buf,"Weapons flags: %s\n\r",weapon_bit_name(obj->value[4]));
            send_to_char(buf,ch);
	}
	break;

    case ITEM_ARMOR:
	xprintf( buf, 
	"Armor class is %d pierce, %d bash, %d slash, and %d vs. magic.\n\r", 
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	xprintf( buf, "Affects %s by %d, level %d",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char(buf,ch);
	
	if ( paf->duration > -1)
	    xprintf(buf,", %d hours.\n\r",paf->duration);
	else
	    xprintf(buf,".\n\r");
	send_to_char( buf, ch );
	if (paf->bitvector)
	{
	    switch(paf->where)
	    {
		case TO_AFFECTS:
		    xprintf(buf,"Adds %s affect.\n",
			affect_bit_name(paf->bitvector));
		    break;

		case TO_AFFECTS2:
    		xprintf(buf, "Also adds %s affect.\n",
        	affect2_bit_name(paf->bitvector));
    		break;

		case TO_DETECTS:
		xprintf(buf, "Adds %s effect.\n",
		detect_bit_name(paf->bitvector));
		break;

                case TO_WEAPON:
                    xprintf(buf,"Adds %s weapon flags.\n",
                        weapon_bit_name(paf->bitvector));
		    break;
		case TO_OBJECT:
		    xprintf(buf,"Adds %s object flag.\n",
			extra_bit_name(paf->bitvector));
		    break;
		case TO_IMMUNE:
		    xprintf(buf,"Adds immunity to %s.\n",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_RESIST:
		    xprintf(buf,"Adds resistance to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_VULN:
		    xprintf(buf,"Adds vulnerability to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		default:
		    xprintf(buf,"Unknown bit %d: %d\n\r",
			paf->where,paf->bitvector);
		    break;
	    }
	    send_to_char(buf,ch);
	}
    }

    if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	xprintf( buf, "Affects %s by %d, level %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char( buf, ch );
        if (paf->bitvector)
        {
            switch(paf->where)
            {
                case TO_AFFECTS:
                    xprintf(buf,"Adds %s affect.\n",
                        affect_bit_name(paf->bitvector));
                    break;
		case TO_AFFECTS2:
                xprintf(buf, "Also adds %s affect.\n",
                affect2_bit_name(paf->bitvector));
                break;

                case TO_DETECTS:
                xprintf(buf, "Adds %s effect.\n",
                detect_bit_name(paf->bitvector));
                break;

                case TO_OBJECT:
                    xprintf(buf,"Adds %s object flag.\n",
                        extra_bit_name(paf->bitvector));
                    break;
                case TO_IMMUNE:
                    xprintf(buf,"Adds immunity to %s.\n",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_RESIST:
                    xprintf(buf,"Adds resistance to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_VULN:
                    xprintf(buf,"Adds vulnerability to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                default:
                    xprintf(buf,"Unknown bit %d: %d\n\r",
                        paf->where,paf->bitvector);
                    break;
            }
            send_to_char(buf,ch);
        }
    }

    return;
}



void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFRARED) )
    {
	if (victim == ch)
	  send_to_char("You can already see in the dark.\n\r",ch);
	else
	  act("$N already has infravision.\n\r",ch,NULL,victim,TO_CHAR);
	return;
    }
    act( "$n's eyes glow red.\n\r", ch, NULL, NULL, TO_ROOM );

    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 2 * level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes glow red.\n\r", victim );
    return;
}



void spell_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;
    int ass;

    ass = (ch->class == class_lookup("assassin")) ? level/2 : level;

    /* object invisibility */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (IS_OBJ_STAT(obj,ITEM_INVIS))
	{
	    act("$p is already invisible.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	af.where	= TO_OBJECT;
	af.type		= sn;
	af.level	= ass;
	af.duration	= level + 12;
	af.location	= APPLY_NONE;
	af.modifier	= 0;
	af.bitvector	= ITEM_INVIS;
	affect_to_obj(obj,&af);

	act("$p fades out of sight.",ch,obj,NULL,TO_ALL);
	return;
    }

    /* character invisibility */
    victim = (CHAR_DATA *) vo;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;
	
/*  if ( IS_AFFECTED(ch, AFF_IMPROVED_INVISIBLE) )
 {
	send_to_char( "You are already invisible.\n\r", ch );
	return;
 }*/

    if ( IS_AFFECTED(victim, AFF_FAERIE_FOG) )
 {
    send_to_char( "You can't be invisible while you are glowing.\n\r", victim );
    return;
 }
    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = ass;
    af.duration  = level + 12;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    return;
}



void spell_know_alignment(int sn,int level,CHAR_DATA *ch,void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char *msg;
    int ap;

    ap = victim->alignment;

	 if ( ap >  700 ) msg = "$N has a pure and good aura.";
    else if ( ap >  350 ) msg = "$N is of excellent moral character.";
    else if ( ap >  100 ) msg = "$N is often kind and thoughtful.";
    else if ( ap > -100 ) msg = "$N doesn't have a firm moral commitment.";
    else if ( ap > -350 ) msg = "$N lies to $S friends.";
    else if ( ap > -700 ) msg = "$N is a black-hearted murderer.";
    else msg = "$N is the embodiment of pure evil!.";

    act( msg, ch, NULL, victim, TO_CHAR );
    return;
}



void spell_lightning_bolt(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  0,  0,  0,	 0,  0,  0, 25, 28,
	31, 34, 37, 40, 40,	41, 42, 42, 43, 44,
	44, 45, 46, 46, 47,	48, 48, 49, 50, 50,
	51, 52, 52, 53, 54,	54, 55, 56, 56, 57,
	58, 58, 59, 60, 60,	61, 62, 62, 63, 64
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_LIGHTNING) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_LIGHTNING ,TRUE);
    return;
}



void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    char buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = IS_IMMORTAL(ch) ? 200 : 2 * level;

    buffer = new_buf();

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_full_name( target_name, obj->name )
	||   IS_OBJ_STAT(obj,ITEM_NOLOCATE) || number_percent() > 2 * level
	||   ch->level < obj->level)
	    continue;

	found = TRUE;
	number++;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by))
	{
	    xprintf( buf, "one is carried by %s\n\r",
		PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    if (IS_IMMORTAL(ch) && in_obj->in_room != NULL)
		xprintf( buf, "one is in %s [Room %d]\n\r",
		    in_obj->in_room->name, in_obj->in_room->vnum);
	    else
		xprintf( buf, "one is in %s\n\r",
		    in_obj->in_room == NULL
			? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	add_buf(buffer,buf);

	if (number >= max_found)
	    break;
    }

    if ( !found )
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
    else
	page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

    return;
}



void spell_magic_missile( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	11, 11, 11, 11, 11,	12, 12, 12, 12, 12,
	13, 13, 13, 13, 13,	14, 14, 14, 14, 14
    };
    int dam;

    if ( is_affected( ch, 67 ) )  {
	if ( ch->level > 4 )  {
	  send_to_char( "Your magic missiles fizzle out near your victim.\n\r", ch );
          act( "Your shield blocks $N's magic missiles.", victim, NULL, ch, TO_CHAR);
        }
	else  {
	  send_to_char( "Your magic missile fizzle out near your victim.\n\r", ch );
          act( "Your shield blocks $N's magic missile.", victim, NULL, ch, TO_CHAR);
	}
	return;
    }

    
    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);

    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_ENERGY) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    if ( ch->level > 4 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage_spell( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }
    if ( ch->level > 8 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage_spell( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }
    if ( ch->level > 12 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage_spell( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }
    if ( ch->level > 16 )  {
      dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
      if ( saves_spell( level, victim,DAM_ENERGY) )
  	  dam /= 2;
      damage_spell( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    }

    return;
}

void spell_mass_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;

    heal_num = skill_lookup("heal");
    refresh_num = skill_lookup("refresh");

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ((IS_NPC(ch) && IS_NPC(gch)) ||
	    (!IS_NPC(ch) && !IS_NPC(gch)))
	{
	    spell_heal(heal_num,level,ch,(void *) gch,TARGET_CHAR);
	    spell_refresh(refresh_num,level,ch,(void *) gch,TARGET_CHAR);
	}
    }
}


void spell_mass_invis( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;
    CHAR_DATA *gch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( !is_same_group( gch, ch ) || IS_AFFECTED(gch, AFF_INVISIBLE) )
	    continue;
	act( "$n slowly fades out of existence.", gch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade out of existence.\n\r", gch );

	af.where     = TO_AFFECTS;
	af.type      = sn;
	af.level     = level/2;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( gch, &af );
    }
    send_to_char( "Ok.\n\r", ch );

    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    send_to_char( "That's not a spell!\n\r", ch );
    return;
}



void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
    {
	if (victim == ch)
	  send_to_char("You are already out of phase.\n\r",ch);
	else
	  act("$N is already shifted out of phase.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );
    act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You turn translucent.\n\r", victim );
    return;
}

/* RT plague spell, very nasty */

void spell_plague( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

if (IS_SET(victim->imm_flags,IMM_DISEASE))
    {
	if (ch == victim)
	  send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
	else
	  act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
     return;
    }
    
 if(ch->class == 13)
  {
    if (saves_spell(level+5,victim,DAM_DISEASE) ||
	(IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
	if (ch == victim)
	  send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
	else
	  act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
	return;
    }
  else
   {
     if (saves_spell(level,victim,DAM_DISEASE) ||
        (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
        if (ch == victim)
          send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
        else
          act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
        return;
    }       
   } 
   } 
    af.where     = TO_AFFECTS;
    af.type 	  = sn;
    af.level	  = level * 3/4;
    af.duration  = 5;
    af.location  = APPLY_STR;
  if (ch->class == 13) 
    af.modifier  = -10;
  else
    af.modifier  = -5;
    af.bitvector = AFF_PLAGUE;
    affect_join(victim,&af);

    send_to_char
      ("You scream in agony as plague sores erupt from your skin.\n\r",victim);
    act("$n screams in agony as plague sores erupt from $s skin.",
	victim,NULL,NULL,TO_ROOM);
}

void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;


    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
	{
	    if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	    {
		act("Your spell fails to corrupt $p.",ch,obj,NULL,TO_CHAR);
		return;
	    }
	    obj->value[3] = 1;
	    act("$p is infused with poisonous vapors.",ch,obj,NULL,TO_ALL);
	    return;
	}

	if (obj->item_type == ITEM_WEAPON)
	{
/*	    if (IS_WEAPON_STAT(obj,WEAPON_FLAMING)
	    ||  IS_WEAPON_STAT(obj,WEAPON_FROST)
	    ||  IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC)
	    ||  IS_WEAPON_STAT(obj,WEAPON_SHARP)
	    ||  IS_WEAPON_STAT(obj,WEAPON_VORPAL)
	    ||  IS_WEAPON_STAT(obj,WEAPON_SHOCKING)
	    ||  IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	    {
		act("You can't seem to envenom $p.",ch,obj,NULL,TO_CHAR);
		return;
	    }
*/

	    if (IS_WEAPON_STAT(obj,WEAPON_POISON))
	    {
		act("$p is already envenomed.",ch,obj,NULL,TO_CHAR);
		return;
	    }

	    af.where	 = TO_WEAPON;
	    af.type	 = sn;
	    af.level	 = level / 2;
	    af.duration	 = level/8;
	    af.location	 = 0;
	    af.modifier	 = 0;
	    af.bitvector = WEAPON_POISON;
	    affect_to_obj(obj,&af);

	    act("$p is coated with deadly venom.",ch,obj,NULL,TO_ALL);
	    return;
	}

	act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
	return;
    }

    victim = (CHAR_DATA *) vo;

    if ( saves_spell( level, victim,DAM_POISON) )
    {
	act("$n turns slightly green, but it passes.",victim,NULL,NULL,TO_ROOM);
	send_to_char("You feel momentarily ill, but it passes.\n\r",victim);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = -2;
    af.bitvector = AFF_POISON;
    affect_join( victim, &af );
    send_to_char( "You feel very sick.\n\r", victim );
    act("$n looks very ill.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_protection_evil(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT_EVIL) 
    ||   IS_AFFECTED(victim, AFF_PROTECT_GOOD))
    {
	if (victim == ch)
	  send_to_char("You are already protected.\n\r",ch);
	else
	  act("$N is already protected.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 24;
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = AFF_PROTECT_EVIL;
    affect_to_char( victim, &af );
    send_to_char( "You feel holy and pure.\n\r", victim );
    if ( ch != victim )
	act("$N is protected from evil.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_protection_good(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD)
    ||   IS_AFFECTED(victim, AFF_PROTECT_EVIL))
    {
	if (victim == ch)
	  send_to_char("You are already protected.\n\r",ch);
	else
	  act("$N is already protected.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 24;
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = AFF_PROTECT_GOOD;
    affect_to_char( victim, &af );
    send_to_char( "You feel aligned with darkness.\n\r", victim );
    if ( ch != victim )
	act("$N is protected from good.",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_ray_of_truth (int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, align;

    if (IS_EVIL(ch) )
    {
	victim = ch;
	send_to_char("The energy explodes inside you!\n\r",ch);
    }

    if (victim != ch)
    {
	act("$n raises $s hand, and a blinding ray of light shoots forth!",
	    ch,NULL,NULL,TO_ROOM);
	send_to_char(
	   "You raise your hand and a blinding ray of light shoots forth!\n\r",
	   ch);
    }

    if (IS_GOOD(victim))
    {
	act("$n seems unharmed by the light.",victim,NULL,victim,TO_ROOM);
	send_to_char("The light seems powerless to affect you.\n\r",victim);
	return;
    }

    dam = dice( level, 10 );
    if ( saves_spell( level, victim,DAM_HOLY) )
				dam /= 2;

/*
    align = victim->alignment;
    align -= 350;

    if (align < -1000)
	align = -1000 + (align + 1000) / 3;

    dam = (dam * align * align) / 1000000;
*/
    damage_spell( ch, victim, dam, sn, DAM_HOLY ,TRUE);
    spell_blindness(gsn_blindness,
	3 * level / 4, ch, (void *) victim,TARGET_CHAR);
}


void spell_recharge( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int chance, percent;

    if (obj->item_type != ITEM_WAND && obj->item_type != ITEM_STAFF)
    {
	send_to_char("That item does not carry charges.\n\r",ch);
	return;
    }

    if (obj->value[0] >= 3 * level / 2)
    {
	send_to_char("Your skills are not great enough for that.\n\r",ch);
	return;
    }

    if (obj->value[1] == 0)
    {
	send_to_char("That item has already been recharged once.\n\r",ch);
	return;
    }

    chance = 40 + 2 * level;

    chance -= obj->value[0]; /* harder to do high-level spells */
/*    chance -= (obj->value[1] - obj->value[2]) *
	      (obj->value[1] - obj->value[2]);
*/
    chance -= UMAX(0,obj->value[1]-obj->value[2]);
    chance = UMAX(level/2,chance);

    percent = number_percent();

	if (obj->value[1] > 1)
	obj->value[1]--;
	
    if (percent < chance / 2)
    {
	act("$p glows brightly.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly.",ch,obj,NULL,TO_ROOM);
	obj->value[2] = UMAX(obj->value[1],obj->value[2]);
/*	if (obj->value[1] > 1)
	obj->value[1]--;*/
	return;
    }

    else if (percent <= chance)
    {
	int chargeback,chargemax;

	act("$p glows softly.",ch,obj,NULL,TO_CHAR);
	act("$p glows softly.",ch,obj,NULL,TO_ROOM);

	chargemax = obj->value[1] - obj->value[2];

	if (chargemax > 0)
	    chargeback = UMAX(1,chargemax * percent / 100);
	else
	    chargeback = 0;

	obj->value[2] += chargeback;
/*    if (obj->value[1] > 1)
	obj->value[1]--;*/
	return;
    }

    else if (percent <= UMIN(95, 3 * chance / 2))
    {
	send_to_char("Nothing seems to happen.\n\r",ch);
/*	if (obj->value[1] > 1)
	    obj->value[1]--;*/
	return;
    }

    else /* whoops! */
    {
	act("$p glows brightly and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly and explodes!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
    }
}

void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->move = UMIN( victim->move + level, victim->max_move );
    if (victim->max_move == victim->move)
	send_to_char("You feel fully refreshed!\n\r",victim);
    else
	send_to_char( "You feel less tired.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    bool found = FALSE;

    /* do object cases first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE))
	{
	    if (!IS_OBJ_STAT(obj,ITEM_NOUNCURSE)
	    &&  !saves_dispel(level + 2,obj->level,0))
	    {
		REMOVE_BIT(obj->extra_flags,ITEM_NODROP);
		REMOVE_BIT(obj->extra_flags,ITEM_NOREMOVE);
		act("$p glows blue.",ch,obj,NULL,TO_ALL);
		return;
	    }

	    act("The curse on $p is beyond your power.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	else  {
	  send_to_char( "Nothing happens...\n\r", ch );
	  return;
	}
    }

    /* characters */
    victim = (CHAR_DATA *) vo;

    if (check_dispel(level,victim,gsn_curse))
    {
	send_to_char("You feel better.\n\r",victim);
	act("$n looks more relaxed.",victim,NULL,NULL,TO_ROOM);
    }

   for (obj = victim->carrying; (obj != NULL && !found); obj = obj->next_content)
   {
	if ((IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE))
	&&  !IS_OBJ_STAT(obj,ITEM_NOUNCURSE))
	{   /* attempt to remove curse */
	    if (!saves_dispel(level,obj->level,0))
	    {
		found = TRUE;
		REMOVE_BIT(obj->extra_flags,ITEM_NODROP);
		REMOVE_BIT(obj->extra_flags,ITEM_NOREMOVE);
		act("Your $p glows blue.",victim,obj,NULL,TO_CHAR);
		act("$n's $p glows blue.",victim,obj,NULL,TO_ROOM);
	    }
	 }
    }
}

void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
/*
    if ( is_affected(victim,gsn_resistance) )
     {
      send_to_char("You are already in sanctuary and can't be resistance same time.\n\r",ch);
      return;
     }
     
*/
    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
    {
	if (victim == ch)
	  send_to_char("You are already in sanctuary.\n\r",ch);
	else
	  act("$N is already in sanctuary.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 6;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a white aura.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a white aura.\n\r", victim );
    return;
}



void spell_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int ran;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded from harm.\n\r",ch);
	else
	  act("$N is already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }

    ran = (ch->class == class_lookup("ranger")) ? level/2 : level;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = ran;
    af.duration  = 8 + level;
    af.location  = APPLY_AC;
    af.modifier  = -20;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a force shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a force shield.\n\r", victim );
    return;
}



void spell_shocking_grasp(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const int dam_each[] =
    {
	 0,
	 0,  0,  0,  0,  0,	 0, 20, 25, 29, 33,
	36, 39, 39, 39, 40,	40, 41, 41, 42, 42,
	43, 43, 44, 44, 45,	45, 46, 46, 47, 47,
	48, 48, 49, 49, 50,	50, 51, 51, 52, 52,
	53, 53, 54, 54, 55,	55, 56, 56, 57, 57
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_LIGHTNING) )
	dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_LIGHTNING ,TRUE);
    return;
}



void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

   if (ch->class == 13)
    level += 7;
    
    if ( IS_AFFECTED(victim, AFF_SLEEP)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD))
    ||   (level + 5) < victim->level
    ||   saves_spell( level-4, victim,DAM_OTHER) )
	return;

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 1 + level/10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You feel very sleepy ..... zzzzzz.\n\r", victim );
	act( "$n goes to sleep.", victim, NULL, NULL, TO_ROOM );
	victim->position = POS_SLEEPING;
    }
    return;
}

void spell_slow( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || IS_AFFECTED(victim,AFF_SLOW))
    {
	if (victim == ch)
	  send_to_char("You can't move any slower!\n\r",ch);
	else
	  act("$N can't get any slower than that.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }
  if (ch->class == 13)
   {
    if (saves_spell(level+7,victim,DAM_OTHER) ||  IS_SET(victim->imm_flags,IMM_MAGIC))
     {
      if (victim != ch)
        send_to_char("Nothing seemed to happen.\n\r",ch);
        send_to_char("You feel momentarily lethargic.\n\r",victim);
        return;
     }
   } 
  else 
   {
    if (saves_spell(level+2,victim,DAM_OTHER)
      ||  IS_SET(victim->imm_flags,IMM_MAGIC))
     {
      if (victim != ch)
       send_to_char("Nothing seemed to happen.\n\r",ch);
       send_to_char("You feel momentarily lethargic.\n\r",victim);
       return;
     }
   }

if (IS_AFFECTED(victim,AFF_HASTE))
    {
	if (!check_dispel(level,victim,skill_lookup("haste")))
	{
	    if (victim != ch)
		send_to_char("Spell failed.\n\r",ch);
	    send_to_char("You feel momentarily slower.\n\r",victim);
	    return;
	}

	act("$n is moving less quickly.",victim,NULL,NULL,TO_ROOM);
	return;
    }


    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_DEX;
    af.modifier  = -1 - (level >= 18) - (level >= 25) - (level >= 32);
    af.bitvector = AFF_SLOW;
    affect_to_char( victim, &af );
    send_to_char( "You feel yourself slowing d o w n...\n\r", victim );
    act("$n starts to move in slow motion.",victim,NULL,NULL,TO_ROOM);
    return;
}




void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int ass;

    if ( is_affected( ch, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already as hard as a rock.\n\r",ch);
	else
	  act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
	return;
    }

    ass = (ch->class == class_lookup("assassin")) ? level/2 : level;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = ass;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -40;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's skin turns to stone.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin turns to stone.\n\r", victim );
    return;
}



void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(ch->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET( ch->in_room->room_flags, ROOM_ARENA )
    ||   IS_SET( victim->in_room->room_flags, ROOM_ARENA )
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_AGGRESSIVE))
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL)
    ||   victim->fighting != NULL
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||	 (IS_NPC(victim) && victim->pIndexData->pShop != NULL)
    ||   (!IS_NPC(victim) && is_safe_nomessage(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER))
    ||   (ch->in_room->area != victim->in_room->area && !IS_NPC(victim))
    ||   (victim->in_room->exit[0] == NULL &&
          victim->in_room->exit[1] == NULL &&
          victim->in_room->exit[2] == NULL &&
          victim->in_room->exit[3] == NULL &&
          victim->in_room->exit[4] == NULL && victim->in_room->exit[5] == NULL) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

if ( strstr( victim->in_room->area->builders, "Unlinked" ) )
{
    send_to_char("You can't summon from areas that aren't linked yet!\n\r",ch);
    return;
}

    act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, ch->in_room );
    act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
    act( "$n has summoned you!", ch, NULL, victim,   TO_VICT );
    do_look( victim, "auto" );
    return;
}



void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *pRoomIndex;

    if ( victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   strstr( ch->in_room->area->builders, "Unlinked" ) 
    || ( victim != ch && IS_SET(victim->imm_flags,IMM_SUMMON))
    || ( !IS_NPC(ch) && victim->fighting != NULL )
    || ( victim != ch
    && ( saves_spell( level - 5, victim,DAM_OTHER))))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    pRoomIndex = get_random_room(victim);

    if (victim != ch)
	send_to_char("You have been teleported!\n\r",victim);

    act( "$n vanishes!", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, pRoomIndex );
    act( "$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM );
    do_look( victim, "auto" );
    return;
}



void spell_ventriloquate( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char speaker[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;

    target_name = one_argument( target_name, speaker );

    xprintf( buf1, "{g%s says '{G%s{g'.{x\n\r",              speaker, target_name );
    xprintf( buf2, "{gSomeone makes %s say '{G%s{g'.{x\n\r", speaker, target_name );
    buf1[0] = UPPER(buf1[0]);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if ( !is_name( speaker, vch->name ) )
	    send_to_char( saves_spell(level,vch,DAM_OTHER) ? buf2 : buf1, vch );
    }

    return;
}



void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
  if(ch->class == 13)
   {
  if ( is_affected( victim, sn ) || saves_spell(level+5,victim,DAM_OTHER))
        return;    
   }
 else
  {  
   if ( is_affected( victim, sn ) || saves_spell( level,victim,DAM_OTHER))
	return;
  }
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 2;
    af.location  = APPLY_STR;
 if (ch->class == 13) 
   af.modifier  = -1 * (level / 5);
  else
    af.modifier  = -1 * (level / 4); 
   af.bitvector = AFF_WEAKEN;
    affect_to_char( victim, &af );
    send_to_char( "You feel your strength slip away.\n\r", victim );
    act("$n looks tired and weak.",victim,NULL,NULL,TO_ROOM);
    return;
}



/* RT recall spell is back */

void spell_word_of_recall( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
//ok  this spell did totaly suck so i changed it

    CHAR_DATA *victim = ch;
    ROOM_INDEX_DATA *location;
    int to_room_vnum;

    if (IS_NPC(victim))
      return;

  if (victim->fighting != NULL)
   {
    send_to_char("You can not concentrate enough.\n\r",ch);
    return;
   }


  if (!str_cmp (target_name, "anchor"))
   {
    victim->anchor = victim->in_room->vnum;
    send_to_char("You are now anchored to this room.\n\r",ch);
    return;
   } 

  if (!str_cmp (target_name, "cancel"))
  {
   victim->anchor = 0;
   send_to_char("You have revoked your anchor.\n\r",ch);  
   return;
  } 

  if (victim->anchor > 0 )
   to_room_vnum = victim->anchor;
  else 
   to_room_vnum = hometown_table[ch->hometown].recall[IS_GOOD(ch)?0:IS_NEUTRAL(ch)?1:IS_EVIL(ch)?2:1];

    if ((location = get_room_index( to_room_vnum )) == NULL)
    {
	send_to_char("You are completely lost.\n\r",victim);
	return;
    }
/*
    if (ch->desc != NULL && current_time - ch->last_fight_time
	< FIGHT_DELAY_TIME)
      {
	send_to_char("You are too pumped to pray now.\n\r",ch);
	return;
      }*/
/*
    if ((!IS_NPC(vo)) && (is_safe_nomessage(ch,vo) && (IS_SET(vo->act,PLR_NOSUMMON)))
	{
	send_to_char("Your victim does not seem to willing recall.\n\r,ch);
	return;
	}
*/
    if (IS_SET(victim->in_room->room_flags,ROOM_NO_RECALL) ||
	IS_AFFECTED(victim,AFF_CURSE)
	||IS_SET(ch->in_room->room_flags, ROOM_ARENA )
        || ( strstr( victim->in_room->area->builders, "Unlinked" ) ))
    {
	send_to_char("Spell failed.\n\r",victim);
	return;
    }

    if (victim->fighting != NULL)
	stop_fighting(victim,TRUE);

//    ch->move /= 2; 
    act("$n disappears.",victim,NULL,NULL,TO_ROOM);
    char_from_room(victim);
    char_to_room(victim,location);
    act("$n appears in the room.",victim,NULL,NULL,TO_ROOM);
    do_look(victim,"auto");
}

/*
 * NPC spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,hp_dam,dice_dam,hpch;

    act("$n spits acid at $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n spits a stream of corrosive acid at you.",ch,NULL,victim,TO_VICT);
    act("You spit acid at $N.",ch,NULL,victim,TO_CHAR);

    hpch = UMAX(12,ch->hit);
    hp_dam = number_range(hpch/11 + 1, hpch/6);
    dice_dam = dice(level,16);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
    dam = UMAX(dam,300);    
    if (saves_spell(level,victim,DAM_ACID))
    {
	acid_effect(victim,level/2,dam/4,TARGET_CHAR);
	damage_spell(ch,victim,dam/2,sn,DAM_ACID,TRUE);
    }
    else
    {
	acid_effect(victim,level,dam,TARGET_CHAR);
	damage_spell(ch,victim,dam,sn,DAM_ACID,TRUE);
    }
}



void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;

    act("$n breathes forth a cone of fire.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a cone of hot fire over you!",ch,NULL,victim,TO_VICT);
    act("You breath forth a cone of fire.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);
      dam = number_range(dam/3,level*2.5);
    fire_effect(victim->in_room,level,dam/2,TARGET_ROOM);

    for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch)
	&&   (ch->fighting != vch || vch->fighting != ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;

	if (vch == victim) /* full damage */
	{
	    if (saves_spell(level,vch,DAM_FIRE))
	    {
		fire_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_spell(ch,vch,dam/2,sn,DAM_FIRE,TRUE);
	    }
	    else
	    {
		fire_effect(vch,level,dam,TARGET_CHAR);
		damage_spell(ch,vch,dam,sn,DAM_FIRE,TRUE);
	    }
	}
	else /* partial damage */
	{
	    if (saves_spell(level - 2,vch,DAM_FIRE))
	    {
		fire_effect(vch,level/4,dam/8,TARGET_CHAR);
		damage_spell(ch,vch,dam/4,sn,DAM_FIRE,TRUE);
	    }
	    else
	    {
		fire_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_spell(ch,vch,dam/2,sn,DAM_FIRE,TRUE);
	    }
	}
    }
}

void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam, hpch;

    act("$n breathes out a freezing cone of frost!",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a freezing cone of frost over you!",
	ch,NULL,victim,TO_VICT);
    act("You breath out a cone of frost.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX(12,ch->hit);
    hp_dam = number_range(hpch/11 + 1, hpch/6);
    dice_dam = dice(level,16);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
    cold_effect(victim->in_room,level,dam/2,TARGET_ROOM); 

    for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch) 
	&&   (ch->fighting != vch || vch->fighting != ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;


	if (vch == victim) /* full damage */
	{
	    if (saves_spell(level,vch,DAM_COLD))
	    {
		cold_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_spell(ch,vch,dam/2,sn,DAM_COLD,TRUE);
	    }
	    else
	    {
		cold_effect(vch,level,dam,TARGET_CHAR);
		damage_spell(ch,vch,dam,sn,DAM_COLD,TRUE);
	    }
	}
	else
	{
	    if (saves_spell(level - 2,vch,DAM_COLD))
	    {
		cold_effect(vch,level/4,dam/8,TARGET_CHAR);
		damage_spell(ch,vch,dam/4,sn,DAM_COLD,TRUE);
	    }
	    else
	    {
		cold_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_spell(ch,vch,dam/2,sn,DAM_COLD,TRUE);
	    }
	}
    }
}

    
void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act("$n breathes out a cloud of poisonous gas!",ch,NULL,NULL,TO_ROOM);
    act("You breath out a cloud of poisonous gas.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX(16,ch->hit);
    hp_dam = number_range(hpch/15+1,8);
    dice_dam = dice(level,12);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
    poison_effect(ch->in_room,level,dam,TARGET_ROOM);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;

	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(ch) && IS_NPC(vch) 
	&&   (ch->fighting == vch || vch->fighting == ch)))
	    continue;
	if ( is_safe(ch, vch) )
          continue;
          if (!IS_NPC(ch) && vch != ch &&
              ch->fighting != vch && vch->fighting != ch &&
              (IS_SET(vch->affected_by,AFF_CHARM) || !IS_NPC(vch)))
            {
            if (!can_see(vch, ch))
                do_yell(vch, "Help someone is attacking me!");
            else
              {
                 xprintf(buf,"Die, %s, you sorcerous dog!",
                    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(vch))?
                     ch->doppel->name : ch->name);
                 do_yell(vch,buf);
              }
          }

	if (saves_spell(level,vch,DAM_POISON))
	{
	    poison_effect(vch,level/2,dam/4,TARGET_CHAR);
	    damage_spell(ch,vch,dam/2,sn,DAM_POISON,TRUE);
	}
	else
	{
	    poison_effect(vch,level,dam,TARGET_CHAR);
	    damage_spell(ch,vch,dam,sn,DAM_POISON,TRUE);
	}
    }
}

void spell_lightning_breath(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,hp_dam,dice_dam,hpch;

    act("$n breathes a bolt of lightning at $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a bolt of lightning at you!",ch,NULL,victim,TO_VICT);
    act("You breathe a bolt of lightning at $N.",ch,NULL,victim,TO_CHAR);

    hpch = UMAX(10,ch->hit);
    hp_dam = number_range(hpch/9+1,hpch/5);
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);

    if (saves_spell(level,victim,DAM_LIGHTNING))
    {
	shock_effect(victim,level/2,dam/4,TARGET_CHAR);
	damage_spell(ch,victim,dam/2,sn,DAM_LIGHTNING,TRUE);
    }
    else
    {
	shock_effect(victim,level,dam,TARGET_CHAR);
	damage_spell(ch,victim,dam,sn,DAM_LIGHTNING,TRUE); 
    }
}

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
void spell_general_purpose(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
 
    dam = number_range( 25, 100 );
    if ( saves_spell( level, victim, DAM_PIERCE) )
        dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_PIERCE ,TRUE);
    return;
}

void spell_high_explosive(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
 
    dam = number_range( 30, 120 );
    if ( saves_spell( level, victim, DAM_PIERCE) )
        dam /= 2;
    damage_spell( ch, victim, dam, sn, DAM_PIERCE ,TRUE);
    return;
}

void spell_lightning_shield(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("This room has been shielded already.\n\r",ch);
        return;
    }

    if ( is_affected(ch,sn))
    {
        send_to_char("This spell is used too recently.\n\r",ch);
        return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 40;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_L_SHIELD;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level    = ch->level;
    af2.duration  = level / 10;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );

    ch->in_room->owner = str_dup( ch->name );
    send_to_char( "The room starts to be filled with lightnings.\n\r", ch);
    act("The room starts to be filled with $n's lightnings.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_shocking_trap(int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("This room has already trapped with shocks waves.\n\r",ch);
        return;
    }

    if ( is_affected(ch,sn))
 {
        send_to_char("This spell is used too recently.\n\r",ch);
        return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 40;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SHOCKING;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level    = level;
    af2.duration  = ch->level / 10;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );
    send_to_char( "The room starts to be filled with shock waves.\n\r", ch);
    act("The room starts to be filled with $n's shock waves.",ch,NULL,NULL,TO_ROOM);
    return;
}

void spell_lifesong(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num;

    heal_num = skill_lookup("cure serious");

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ((IS_NPC(ch) && IS_NPC(gch)) ||
	    (!IS_NPC(ch) && !IS_NPC(gch)))
	{
	    spell_cure_serious(heal_num,level,ch,(void *) gch,TARGET_CHAR);
	}
    }
}

void spell_endursong(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num;

    heal_num = skill_lookup("refresh");

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ((IS_NPC(ch) && IS_NPC(gch)) ||
	    (!IS_NPC(ch) && !IS_NPC(gch)))
	{
	    spell_refresh(heal_num,level,ch,(void *) gch,TARGET_CHAR);
	}
    }
}

void spell_invul(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
    if ( IS_AFFECTED2(victim, AFF2_INVUL) )
    {
	if (victim == ch)
	  send_to_char("You are already have aura of invulnerability.\n\r",ch);
	else
	  act("$N has a aura of invulnerability already.",ch,NULL,victim,TO_CHAR);
	return;
    }

 if (is_affected(ch, gsn_invul))
    {
      send_to_char("{cThe gods deny your request!{x\n\r", ch);
      return;
    }

    af.where     = TO_AFFECTS2;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = 1;
    af.modifier	= 0;
    af.location	= 0;
    af.bitvector = AFF2_INVUL;
    affect_to_char( victim, &af );

      af.type	= gsn_invul;
      af.level	= ch->level;
      af.duration	= 15;
      af.modifier	= 0;
      af.bitvector = 0;
      af.location	= 0;
      affect_to_char(ch,&af);

    
    send_to_char( "The gods grants you an aura of invulnerability!\n\r", victim );
    if ( ch != victim )
	act("$N is granted an aura of invulnerability!",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_master_sight( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( !CAN_DETECT( ch, DETECT_HIDDEN ) )
                {
                af.where         = TO_DETECTS;
                af.type          = sn;
                af.level                 = level;
                af.duration  = level;
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = DETECT_HIDDEN;
                affect_to_char( ch, &af );
                send_to_char( "Your awarness improves.\n\r", victim );
                }

  if ( !CAN_DETECT( ch, DETECT_INVIS ) )
                {
                af.where                 = TO_DETECTS;
                af.type          = sn;
                af.level                 = level;
                af.duration  = level;
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = DETECT_INVIS;
                affect_to_char( ch, &af ); 
                send_to_char( "Your eyes tingle with detect invisibility.\n\r",ch);
                }

  if ( !CAN_DETECT( ch, ACUTE_VISION ) )
                {
                af.where                 = TO_DETECTS;
                af.type          = sn;
                af.level                 = level;
                af.duration  = level;
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = ACUTE_VISION;
                affect_to_char( ch, &af );
                send_to_char( "Your eyes tingle with acute vision.\n\r",ch);
                }
return;
}

void spell_scan_person(int sn,int level,CHAR_DATA *ch,void *vo,int target
)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char buf[MAX_STRING_LENGTH]; 
    if (victim->imm_flags)
    {
        xprintf(buf, "They immmune to: %s\n\r",imm_bit_name(victim->imm_flags));
        send_to_char(buf,ch);
    }
	else
	send_to_char("They are not immune to anything.\n\r",ch);

    if (victim->res_flags)
    {
        xprintf(buf, "They can Resist: %s\n\r",imm_bit_name(victim->res_flags));
        send_to_char(buf,ch);
    }
	else
	send_to_char("They are not resistant to anything.\n\r",ch);


    if (victim->vuln_flags)
    {
        xprintf(buf, "They are Vulnerable to: %s\n\r",imm_bit_name(victim->vuln_flags));
        send_to_char(buf,ch);
    }
	else
	send_to_char("They are not vulnerable to anything.\n\r",ch);

    return;
}

void spell_transfer_object(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
   char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    target_name = one_argument(target_name,arg1);
    one_argument(target_name,arg2);

	if (arg1[0] == '\0'
	|| arg2[0] == '\0')
	{
	send_to_char("Send what to whom?\n\r",ch);
	return;
 }

	if ( ( obj = get_obj_carry( ch, arg1 , NULL) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}


    victim = get_char_world(ch,arg2);
	if (victim == NULL)
	{
send_to_char("They aren't here.\n\r",ch);
	return;
	}

    if (IS_NPC(victim))
    {
    send_to_char("They wouldn't want that sent to them.\n\r",ch);
    return;
    }

    if (victim == ch)
	{
send_to_char("Why not just hold it in your other hand?\n\r",ch);
	return;
	}

    if (IS_IMMORTAL(victim) && !IS_IMMORTAL(ch))
    {
    send_to_char("You can't send to an immortal.\n\r",ch);
    return;
    }
    if (IS_SET(obj->extra_flags,ITEM_NODROP))
    {
    send_to_char("You can't seem to remove it from yourself to prepare for transfer.\n\r",ch);
    return;
    }
    if (victim->carry_number >= can_carry_n(victim)
    || victim->carry_weight >= can_carry_w(victim))
    {
    act("$p glows softly but nothing happens.",ch,obj,0,TO_CHAR);
    act("$p glows softly but nothing happens.",ch,obj,0,TO_ROOM);
    return;
    }

    act("$p glows softly and vanishes!",ch,obj,0,TO_CHAR);
    act("$p glows softly and vanishes!",ch,obj,0,TO_ROOM);
    obj_from_char(obj);
    obj_to_char(obj,victim);
act("You suddenly feel heavier as $p pops into your inventory!",victim,obj,0,TO_CHAR);
act("$p suddenly appears from nowhere and pops into $n's possession!",victim,obj,0,TO_ROOM);
    return;
}

void spell_create_rose( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *rose;
    char color[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    
    char *tag;
//    EXTRA_DESCR_DATA *ed_next;
    tag = one_argument(target_name, color);
    rose = create_object(get_obj_index(OBJ_VNUM_ROSE), 0);
    
    if(!strcmp(color,"mauve")||!strcmp(color,"crimson")||!strcmp(color,"blood-red")
       ||!strcmp(color,"red")||!strcmp(color,"burgundy")||!strcmp(color,"white")
       ||!strcmp(color,"whithered white")||!strcmp(color,"pink")||!strcmp(color,"light pink")
       ||!strcmp(color,"dark pink")||!strcmp(color,"yellow")||!strcmp(color,"orange")
       ||!strcmp(color,"coral")||!strcmp(color,"lavender")||!strcmp(color,"peach")
       ||!strcmp(color,"black")||!strcmp(color,"midnight black")||!strcmp(color,"jet black")
       ||!strcmp(color,"acid"))
    {
        
        if(!strcmp(color,"acid"))
        {
            strcpy(buf1, "{wa {Bp{bs{Cy{cc{Gh{ge{Md{me{Rl{ri{Yc{w rose");
            strcpy(buf2, "A freaky rose is tripping out here.");
            strcpy(buf3, "Your mind warps, you reel, you question your existence, you see God.\n");
        } 
        else
        {
            if(!strcmp(color,"orange"))
            {
                xprintf(buf1, "an %s rose", color);
            } 
            else xprintf(buf1, "a %s rose", color);
            xprintf(buf2, "A %s rose is lying on the ground.", color);
            xprintf(buf3, "It's the perfect example of %s.\n",buf1);
        }
        free_extra_descr(rose->extra_descr);
        rose->extra_descr=new_extra_descr();
        rose->extra_descr->keyword=str_dup("rose");
        rose->extra_descr->description=str_dup(buf3);
        free_string(rose->description);
        rose->description=str_dup(buf2);
        if(tag[0]!='\0')
            xprintf(buf1, "%s with a tag that reads: '%s'",buf1,tag);
        free_string(rose->short_descr);
        rose->short_descr=str_dup(buf1);
    }
    else if(color[0]!='\0')
     {
     extract_obj(rose);
     act("Roses don't come in that color.",ch,rose,NULL,TO_CHAR);
     return;
     }
    act("$n has created $p.",ch,rose,NULL,TO_ROOM);
    act("You have created $p.",ch,rose,NULL,TO_CHAR);
    obj_to_char(rose,ch);
    return;
}
