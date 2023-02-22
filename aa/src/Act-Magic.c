/****************************************************************************
*     Fate of Kingdom 4.0 is copyright 2000-???? Christian Tusborg (Kazmir) *
*     By using this code, you have agreed to follow the terms of the        *
*     Fate license.  The use of this code without the permission of the     *
*     author is prohibited.  You will be punished to the fullest extend of  *
*     the law.								    *
*     									    *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		    *
****************************************************************************/

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

/****************************************************************************
                              Act Magic
*****************************************************************************/

// Lookup a skill by slot number. Used for object loading.
int slot_lookup( int slot )
/*****************************************************************************
Called By: File: Act-Magic.c | Function: send_spell(CHAR_DATA *ch)
Pre:  slot > 0 | ON PRE ERROR: return and report bug | if fBootDb boot abort()
Post: return sn;
*****************************************************************************/
{
	extern bool fBootDb;
	int sn;

	if ( slot <= 0 )
	  {
		bug( "*BUG* File: Act-Magic.c | Function:  slot_lookup | Error: slot <= 1",0 );
		return -1;
	  }

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	  {
		if ( slot == skill_table[sn].slot )
			return sn;
	  }

	if ( fBootDb )
	  {
		bug( "*BUG* File: Act-Magic.c | Function:  slot_lookup | Error: slot <= 1 (%d)",slot );
		abort( );
	  }
	return -1; 
}


void say_spell( CHAR_DATA *ch, int sn )
/*****************************************************************************
Called By: File: Act-Magic.c | Function: send_spell(CHAR_DATA *ch)
Pre:  sn > 0 | ch != NULL | ON PRE ERROR: return and report bug
Post: ch == ch | sn == sn
Act: Utter mystical words for an sn (Send Text to players and room)
*****************************************************************************/
{
	char buf  [MAX_STRING_LENGTH];
	char buf2 [MAX_STRING_LENGTH];
	CHAR_DATA *rch;
	char *pName;
	int iSyl;
	int length;

	struct syl_type
	  {
		char *	old;
		char *	new;
	  };

	static const struct syl_type syl_table[] =
	  {
		{ " ",		" "		},
		{ "ar",		"avbra"		},
		{ "au",		"kaxda"		},
		{ "bless",	"fiedo"		},
		{ "blind",	"nosge"		},
		{ "bur",	"mosa"		},
		{ "cu",		"judi"		},
		{ "de",		"oculfo"	},
		{ "en",		"uniso"		},
		{ "light",	"dyes"		},
		{ "lo",		"hyi"		},
		{ "mor",	"zagk"		},
		{ "move",	"sido"		},
		{ "ness",	"lacri"		},
		{ "ning",	"illa"		},
		{ "per",	"duhda"		},
		{ "ra",		"gru"		},
		{ "fresh",	"ima"		},
		{ "re",		"candus"	},
		{ "son",	"sawbru"	},
		{ "tect",	"inre"		},
		{ "tri",	"cela"		},
		{ "ven",	"norfo"		},
		{ "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
		{ "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
		{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
		{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
		{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
		{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
		{ "y", "l" }, { "z", "k" },
		{ "", "" }
	  };

	//Just to make sure there is a ch
	if (ch == NULL)
	 {
		bug( "*BUG* File: Act-Magic.c | Function: say_spell | Error: ch == NULL",0 );
		return;
	 }

	//Just to make sure there is a sn

	if (sn < 1)
	 {
		bug( "*BUG* File: Act-Magic.c | Function: say_spell | Error: sn < 1",0);
		return;
	 }


	buf[0]	= '\0';
	for ( pName = skill_table[sn].name; *pName != '\0'; pName += length )
	  {
		for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
		  {
			if ( !str_prefix( syl_table[iSyl].old, pName ) )
			  {
				strcat( buf, syl_table[iSyl].new );
				break;
			  }
		  }

		if ( length == 0 )
			length = 1;
	  }

	//switch statement for classes
	switch (ch->class)
	    { 
		case CLASS_MAG:
			xprintf( buf2, "{g$n waves $s hands fancifully and utters, '%s'.{x", buf);
			xprintf( buf,  "{g$n waves $s hands fancifully and utters, '%s'.{x",skill_table[sn].name);
			send_to_char("{cWith a fanciful wave of hands your magic is cast.{x",ch);
		  break;
		
		case CLASS_MAL:
			xprintf( buf2, "{g$n's eyes flash red as $e utters, '%s'.{x", buf);
			xprintf( buf,  "{g$n's eyes flash red as $e utters, '%s'.{x",skill_table[sn].name);
			send_to_char("{cWith a flash of red in your eyes, your magic is cast.{x",ch);
		  break;

		case CLASS_CLE: 
			xprintf( buf2, "{g$n breathes deeply as $e closes $s eyes in communion.{x"); 
			xprintf( buf,  "{g$n breathes deeply as $e closes $s eyes in communion.{x"); 
			send_to_char("{cYou commune with the divine for your powers...{x",ch);
		  break;

		case CLASS_PDN:
			xprintf( buf2, "{g$n closes $s eyes for a moment and nods solemnly.{x");
			xprintf( buf,  "{g$n closes $s eyes for a moment and nods solemnly.{x");
			send_to_char("{cYou commune with the divine for your powers...{x",ch);
		  break;

		case CLASS_BKT:
			xprintf( buf2, "{g$n utters fiercely, '%s'.{x", buf);
			xprintf( buf,  "{g$n utters fiercely, '%s'.{x", skill_table[sn].name);
			send_to_char("{cYou fiercely utter some arcane words.{x" ,ch);
		  break;

		case CLASS_PRO:
			xprintf( buf2, "{g$n fixes $s eyes in absolute concentration for a split second.{x" );
			xprintf( buf,  "{g$n fixes $s eyes in absolute concentration for a split second.{x" );
			send_to_char("{cYou bring your mind to absolute focus.{x",ch);
		  break;

		case CLASS_BRD:
			xprintf( buf2, "{g$n clears $s throat.{x" );
			xprintf( buf,  "{g$n clears $s throat.{x" );
			send_to_char("{cYou clear your throat in preparation...{x" ,ch);
		  break;

		case CLASS_ALC:
			xprintf( buf2, "{g$n traces symbols in the air with $s fingers.{x");
			xprintf( buf,  "{g$n traces symbols in the air with $s fingers.{x");
			send_to_char("{cYou trace symbols of the elements in the air with your finger.{x" ,ch);
		  break;

		case CLASS_SPE:
			xprintf( buf2, "{g$n crudely utters the words, '%s'.{x", buf);
			xprintf( buf,  "{g$n crudely utters the words, '%s'.{x", skill_table[sn].name);
			send_to_char("{cYou try your best to cast the magics{x",ch);
		  break;

		case CLASS_INQ:
			xprintf( buf2, "{g$n utters with an eerie fervor, '%s'.{x", buf);
			xprintf( buf,  "{g$n utters with an eerie fervor, '%s'.{x", skill_table[sn].name);
			send_to_char("{cYou commune with the fire burning within.{x",ch);
		  break;

		case CLASS_ASN:
			xprintf( buf2, "{g$n waves $s hand deftly as $e utters, '%s'.{x", buf);
			xprintf( buf,  "{g$n waves $s hand deftly as $e utters, '%s'.{x", skill_table[sn].name);
			send_to_char("{cYou wave your hand deftly as you bring forth dark magics.{x",ch);
		  break;

		case CLASS_RAN:
			xprintf( buf2, "{g$n carefully utters the words, '%s'.{x", buf );
			xprintf( buf,  "{g$n carefully utters the words, '%s'.{x", skill_table[sn].name );
			send_to_char("{cWith thoughts of nature, you cast your magic.{x" ,ch);
		  break;
	
		case CLASS_THI:
		case CLASS_MER:
		case CLASS_WAR:
			xprintf( buf2, "{g$n barely pronounces the words, '%s'.{x", buf );
			xprintf( buf,  "{g$n barely pronounces the words, '%s'.{x", skill_table[sn].name );
			send_to_char("{cYou barely manage to pronounce the words.{x" ,ch);
		  break;
		
		default:
			xprintf( buf2, "{g$n utters the words, '%s'.{x", buf );
			xprintf( buf,  "{g$n utters the words, '%s'.{x", skill_table[sn].name );
			send_to_char("{cYou utter the arcane words.{x",ch);
			bug( "*BUG* File: Act-Magic.c | Function: say_spell | Error: Class not switch statement for classes",0 );
		  break;
	  }

	//New line
	send_to_char("\n\r",ch);

	//send it both ch and room
	//spell craft allow to see real spell name
	for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
	  {
		if ( rch != ch )
		  {
			act((get_skill(rch,gsn_spell_craft)*0.9)/(class_table[rch->class].fMana?1:2)<number_percent() ? buf2 : buf, ch, NULL, rch, TO_VICT );
	  		check_improve( rch, gsn_spell_craft, TRUE, 5 );
		  }
	  }

	return;
}

/*

void spell_send( CHAR_DATA *ch, target)
{

	switch (skill_table[sn].target )
 	  {

		default:
			bug( "*BUG* File: Act-Magic.c | Function: send_spell | Error: sn < 1",0);
		return;

		case TAR_IGNORE:
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
				if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
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
				send_to_char( "You can't do that on your own follower.\n\r",ch );
				return;
			  }

			vo = (void *) victim;
			target = TARGET_CHAR;

			if (!IS_NPC(ch) 
			 && victim != ch 
			 && ch->fighting != victim 
			 && victim->fighting != ch 
			 && (IS_SET(victim->affected_by,AFF_CHARM) 
			 || !IS_NPC(victim)))
			  {
				if (!can_see(victim, ch))
					do_yell(victim, "Help someone is attacking me!");
				else
				  {
					xprintf(buf,"Die, %s, you sorcerous dog!",(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?ch->doppel->name : ch->name);
					do_yell(victim,buf);
				  }
			 }
		break; //TAR_CHAR_OFFENSIVE

		case TAR_CHAR_DEFENSIVE:
			if ( arg2[0] == '\0' )
				victim = ch;
			else
			  {
				if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
				  {
					send_to_char( "They aren't here.\n\r", ch );
					return;
				  }
			  }

			vo = (void *) victim;
			target = TARGET_CHAR;
		break; //TAR_CHAR_DEFENSIVE

		case TAR_CHAR_SELF:

			if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
			  {
				send_to_char( "You cannot cast this spell on another.\n\r", ch );
				return;
			  }

			vo = (void *) ch;
			target = TARGET_CHAR;
		break; //TAR_CHAR_SELF

		case TAR_OBJ_INV:
			if ( arg2[0] == '\0' )
			  {
				send_to_char( "What should the spell be cast upon?\n\r", ch );
				return;
			  }

			if ( ( obj = get_obj_carry( ch, target_name ) ) == NULL )
			  {
				send_to_char( "You are not carrying that.\n\r", ch );
				return;
			  }

			vo = (void *) obj;
			target = TARGET_OBJ;
		break; //TAR_OBJ_INV

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
			else if ((victim = get_char_room(ch,target_name)) != NULL)
			  {
				target = TARGET_CHAR;
			  }

			if (target == TARGET_CHAR)
			  {
				if(is_safe_spell(ch,victim,FALSE) && victim != ch)
				  {
					send_to_char("Your spell didn't work.\n\r",ch);
					return;
				  }

				if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
				  {
					send_to_char( "You can't do that on your own follower.\n\r",ch );
					return;
				  }

				if ( is_safe(ch, victim) )
					return;

				vo = (void *) victim;
			  }
			else if ((obj = get_obj_here(ch,target_name)) != NULL)
			  {
				vo = (void *) obj;
				target = TARGET_OBJ;
			  }
			else
			  {
				send_to_char("You don't see that here.\n\r",ch);
				return;
			  }
		break; //TAR_OBJ_CHAR_OFF

		case TAR_OBJ_CHAR_DEF:
			if (arg2[0] == '\0')
			  {
				vo = (void *) ch;
				target = TARGET_CHAR;
			  }
			else if ((victim = get_char_room(ch,target_name)) != NULL)
			  {
				vo = (void *) victim;
				target = TARGET_CHAR;
			  }
			else if ((obj = get_obj_carry(ch,target_name)) != NULL)
			  {
				vo = (void *) obj;
				target = TARGET_OBJ;
			  }
			else
			  {
				send_to_char("You don't see that here.\n\r",ch);
				return;
			  }
		break; //TAR_OBJ_CHAR_DEF
	  } //switch (target )

//	spell_cost(ch,sn)

	//Execute the spell code...
	(*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
	
	//check if caster get's better a the spell
	check_improve(ch,sn,TRUE,1);

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
} // void send_spell( CHAR_DATA *ch, target)

void spell_drop(CHAR_DATA *ch)
  {
	//ch->pcdate->spell_sn			= -1;
	//ch->pcdate->spell_vo			= NULL;
	//ch->pcdate->spell_target		= -1;
	//ch->pcdate->spell_target_name		= NULL;
	//ch->pcdate->spell_level		= -1;
	//ch->pcdate->spell_time 		= -1;
  	return;	
  }

void spell_bash(CHAR_DATA *ch)
  {
	//ch->pcdate->spell_sn			= -1;
	//ch->pcdate->spell_vo			= NULL;
	//ch->pcdate->spell_target		= -1;
	//ch->pcdate->spell_target_name		= NULL;
	//ch->pcdate->spell_level		= -1;
	//ch->pcdate->spell_time 		= -1;
  	return;	
  }

void spell_update(CHAR_DATA *ch)
  {
	//ch->pcdate->spell_sn			= -1;
	//ch->pcdate->spell_vo			= NULL;
	//ch->pcdate->spell_target		= -1;
	//ch->pcdate->spell_target_name		= NULL;
	//ch->pcdate->spell_level		= -1;
	//ch->pcdate->spell_time--;
	

//	if (target == TARGET_OBJ)
//		if ( ( obj = get_obj_carry( ch, target_name ) ) == NULL )
//			blnTrue = TRUE;
//	else
//		if ((victim = get_char_room(ch,target_name)) != NULL)
//			blnTrue = TRUE;		
	  	
//	if (ch->pcdate->spell_time < 1 && ch->pcdate->spell_sn >= 1 && blnTrue)
//	  {
//			(*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
//			ch->pcdata->inmemory[ch->pcdate->spell_sn]--;
			//ch->pcdate->spell_sn			= -1;
			//ch->pcdate->spell_vo			= NULL;
			//ch->pcdate->spell_target		= -1;
			//ch->pcdate->spell_target_name		= NULL;
			//ch->pcdate->spell_level		= -1;
			//ch->pcdate->spell_time		= -1;	  

//	  }
//	else
//	  {
			//ch->pcdate->spell_sn			= -1;
			//ch->pcdate->spell_vo			= NULL;
			//ch->pcdate->spell_target		= -1;
			//ch->pcdate->spell_target_name		= NULL;
			//ch->pcdate->spell_level		= -1;
			//ch->pcdate->spell_time		= -1;	  

//	  }
  	return;	
  }


void spell_set( int sn, CHAR_DATA *ch, void *vo,int target, char *target_name )
{
	//ch->pcdate->spell_sn			= sn;
	//ch->pcdate->spell_vo			= vo;
	//ch->pcdate->spell_target		= target;
	//ch->pcdate->spell_target_name		= target_name;
	//ch->pcdate->spell_level		= ch->level;
	//ch->pcdate->spell_time 		= skill_table[sn].beats[3];
  return;	
}

void do_cast_smart( CHAR_DATA *ch, char *argument )
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

	// Switched NPC's can cast spells, but others can't.

	if ( IS_NPC(ch) && ch->desc == NULL)
		return;

	if (!IS_NPC(ch) && is_affected(ch, gsn_charm_person))
	  {
		send_to_char("You should ask your master first.\n\r", ch);
		return;
	  }

	target_name = one_argument( argument, arg1 );
	one_argument( target_name, arg2 );


	if (ch->pcdata->inmemory[sn] < 1)
	  {
		send_to_char("You do not have any of those spells in your emmory.\n\r", ch);
		return;		
	  }
	 
	 
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
	        return;
	  }

	if ( ch->position < skill_table[sn].minimum_position )
	  {
		send_to_char( "You can not concentrate enough.\n\r", ch );
		return;
	  }

	if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
	  {
		send_to_char("Your spell fizzles out and fails.\n\r",ch);
		act("$n's spell fizzles out and fails.", ch, NULL, NULL, TO_ROOM);
		return;
	  }

    // Locate targets.
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;

	switch (skill_table[sn].target )
 	  {

		default:
			bug( "*BUG* File: Act-Magic.c | Function: send_spell | Error: sn < 1",0);
		return;

		case TAR_IGNORE:
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
				if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
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
				send_to_char( "You can't do that on your own follower.\n\r",ch );
				return;
			  }

			vo = (void *) victim;
			target = TARGET_CHAR;

			if (!IS_NPC(ch) 
			 && victim != ch 
			 && ch->fighting != victim 
			 && victim->fighting != ch 
			 && (IS_SET(victim->affected_by,AFF_CHARM) 
			 || !IS_NPC(victim)))
			  {
				if (!can_see(victim, ch))
					do_yell(victim, "Help someone is attacking me!");
				else
				  {
					xprintf(buf,"Die, %s, you sorcerous dog!",(is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(victim))?ch->doppel->name : ch->name);
					do_yell(victim,buf);
				  }
			 }
		break; //TAR_CHAR_OFFENSIVE

		case TAR_CHAR_DEFENSIVE:
			if ( arg2[0] == '\0' )
				victim = ch;
			else
			  {
				if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
				  {
					send_to_char( "They aren't here.\n\r", ch );
					return;
				  }
			  }

			vo = (void *) victim;
			target = TARGET_CHAR;
		break; //TAR_CHAR_DEFENSIVE

		case TAR_CHAR_SELF:

			if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
			  {
				send_to_char( "You cannot cast this spell on another.\n\r", ch );
				return;
			  }

			vo = (void *) ch;
			target = TARGET_CHAR;
		break; //TAR_CHAR_SELF

		case TAR_OBJ_INV:
			if ( arg2[0] == '\0' )
			  {
				send_to_char( "What should the spell be cast upon?\n\r", ch );
				return;
			  }

			if ( ( obj = get_obj_carry( ch, target_name ) ) == NULL )
			  {
				send_to_char( "You are not carrying that.\n\r", ch );
				return;
			  }

			vo = (void *) obj;
			target = TARGET_OBJ;
		break; //TAR_OBJ_INV

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
			else if ((victim = get_char_room(ch,target_name)) != NULL)
			  {
				target = TARGET_CHAR;
			  }

			if (target == TARGET_CHAR)
			  {
				if(is_safe_spell(ch,victim,FALSE) && victim != ch)
				  {
					send_to_char("Your spell didn't work.\n\r",ch);
					return;
				  }

				if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
				  {
					send_to_char( "You can't do that on your own follower.\n\r",ch );
					return;
				  }

				if ( is_safe(ch, victim) )
					return;

				vo = (void *) victim;
			  }
			else if ((obj = get_obj_here(ch,target_name)) != NULL)
			  {
				vo = (void *) obj;
				target = TARGET_OBJ;
			  }
			else
			  {
				send_to_char("You don't see that here.\n\r",ch);
				return;
			  }
		break; //TAR_OBJ_CHAR_OFF

		case TAR_OBJ_CHAR_DEF:
			if (arg2[0] == '\0')
			  {
				vo = (void *) ch;
				target = TARGET_CHAR;
			  }
			else if ((victim = get_char_room(ch,target_name)) != NULL)
			  {
				vo = (void *) victim;
				target = TARGET_CHAR;
			  }
			else if ((obj = get_obj_carry(ch,target_name)) != NULL)
			  {
				vo = (void *) obj;
				target = TARGET_OBJ;
			  }
			else
			  {
				send_to_char("You don't see that here.\n\r",ch);
				return;
			  }
		break; //TAR_OBJ_CHAR_DEF
	  } //switch (target )

	spell_set(sn,ch,vo,target,target_name);
return;
}

void do_spell_memorize( CHAR_DATA *ch, char *argument )
{
  char arg_spell[MAX_INPUT_LENGTH];
	
	one_argument( argument, arg_spell );
	sn = find_spell( ch,arg_spell )
	
	if ((sn < 1) 
	 && spell_know(ch,sn))
	  {
		send_to_char( "You don't know any spells of that name.\n\r", ch );
		return;	  	
	  }
	if (ch->pcdata->inmemory[sn] < ch->pcdata->maxmemory[sn])
		ch->pcdata->inmemory[sn]++
	  
}

void do_spell_forget( CHAR_DATA *ch, char *argument )
{
  char arg_spell[MAX_INPUT_LENGTH];
	
	one_argument( argument, arg_spell );
	sn = find_spell( ch,arg_spell )
	
	if ((sn < 1) 
	 && spell_know(ch,sn))
	  {
		send_to_char( "You don't know any spells of that name.\n\r", ch );
		return;	  	
	  }
	if (ch->pcdata->inmemory[sn] > 0)
		ch->pcdata->inmemory[sn]--
	  
}

*/
