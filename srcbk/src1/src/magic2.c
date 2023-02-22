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
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_murder	);
DECLARE_DO_FUN(do_kill		);



extern char *target_name;

void spell_portal( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone, *stone1;
    bool component,component1;
	if ((ch->pcdata->quest !=NULL))
	{
		QUEST_DATA *quest = ch->pcdata->quest;
		if (quest->protection)	
		{
			send_to_char( "You are in a quest. Portal is disabled.\n\r", ch );
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
    ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
    ||   strstr( ch->in_room->area->builders, "Unlinked" ) 
    ||   IS_SET( ch->in_room->room_flags, ROOM_ARENA )
    ||   IS_SET( victim->in_room->room_flags, ROOM_ARENA )
    ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_NONE) ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }   

if ( strstr( victim->in_room->area->builders, "Unlinked" ) )
{
     send_to_char("You can't portal to areas that aren't linked yet!\n\r",ch);
     return;
}
	component = FALSE;
	component1 = FALSE;
    stone = get_eq_char(ch,WEAR_HOLD);
    if (!IS_IMMORTAL(ch)
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
	component = TRUE;

    stone1 = get_eq_char(ch,WEAR_FLOAT);
    if (!IS_IMMORTAL(ch)
    &&  (stone1 == NULL || stone1->item_type != ITEM_WARP_STONE))
	component1 = TRUE;

    if(component == TRUE && component1 == TRUE)
    {
        send_to_char("You lack the proper component for this spell.\n\r",ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
/*     	act("It flares brightly and vanishes!",ch,stone,NULL,TO_CHAR);
     	extract_obj(stone); */
    }
    if (stone1 != NULL && stone1->item_type == ITEM_WARP_STONE) 
     	act("You draw upon the power of $p.",ch,stone1,NULL,TO_CHAR);
   
    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 2 + level / 25; 
    portal->value[3] = victim->in_room->vnum;

    obj_to_room(portal,ch->in_room);

    act("$p rises up from the ground.",ch,portal,NULL,TO_ROOM);
    act("$p rises up before you.",ch,portal,NULL,TO_CHAR);
}

void spell_nexus( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *portal, *stone, *stone1;
    ROOM_INDEX_DATA *to_room, *from_room;
    bool component,component1;
    from_room = ch->in_room;
 
        if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   (to_room = victim->in_room) == NULL
    ||   !can_see_room(ch,to_room) || !can_see_room(ch,from_room)
    ||   IS_SET(to_room->room_flags, ROOM_SAFE)
    ||	 IS_SET(from_room->room_flags,ROOM_SAFE)
    ||   IS_SET(to_room->room_flags, ROOM_PRIVATE)
    ||   strstr( ch->in_room->area->builders, "Unlinked" ) 
    ||   IS_SET(to_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(to_room->room_flags, ROOM_ARENA )
    ||   IS_SET(from_room->room_flags, ROOM_ARENA )
    ||   IS_SET(to_room->room_flags, ROOM_NOSUMMON)
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */
    ||   (IS_NPC(victim) && is_safe_nomessage(ch, victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_NONE) ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }   

if ( strstr( to_room->area->builders, "Unlinked" ) )
{
     send_to_char( "You can't use nexus to get to areas that aren't linked yet!\n\r",ch );
     return;
}
 
	component = FALSE;
	component1 = FALSE;
    stone = get_eq_char(ch,WEAR_HOLD);
    if (!IS_IMMORTAL(ch)
    &&  (stone == NULL || stone->item_type != ITEM_WARP_STONE))
	component = TRUE;

    stone1 = get_eq_char(ch,WEAR_FLOAT);
    if (!IS_IMMORTAL(ch)
    &&  (stone1 == NULL || stone1->item_type != ITEM_WARP_STONE))
	component1 = TRUE;

    if(component == TRUE && component1 == TRUE)
    {
        send_to_char("You lack the proper component for this spell.\n\r",ch);
        return;
    }

    if (stone != NULL && stone->item_type == ITEM_WARP_STONE)
    {
     	act("You draw upon the power of $p.",ch,stone,NULL,TO_CHAR);
/*     	act("It flares brightly and vanishes!",ch,stone,NULL,TO_CHAR);
     	extract_obj(stone); */
    }
    if (stone1 != NULL && stone1->item_type == ITEM_WARP_STONE) 
     	act("You draw upon the power of $p.",ch,stone1,NULL,TO_CHAR);

    /* portal one */ 
    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 1 + level / 10;
    portal->value[3] = to_room->vnum;
 
    obj_to_room(portal,from_room);
 
    act("$p rises up from the ground.",ch,portal,NULL,TO_ROOM);
    act("$p rises up before you.",ch,portal,NULL,TO_CHAR);

    /* no second portal if rooms are the same */
    if (to_room == from_room)
	return;

    /* portal two */
    portal = create_object(get_obj_index(OBJ_VNUM_PORTAL),0);
    portal->timer = 1 + level/10;
    portal->value[3] = from_room->vnum;

    obj_to_room(portal,to_room);

    if (to_room->people != NULL)
    {
	act("$p rises up from the ground.",to_room->people,portal,NULL,TO_ROOM);
	act("$p rises up from the ground.",to_room->people,portal,NULL,TO_CHAR);
    }
}


void spell_disintegrate( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  CHAR_DATA *tmp_ch;
  OBJ_DATA *obj;
  OBJ_DATA *obj_next;
  int i;

    if ( IS_SET( ch->in_room->room_flags, ROOM_ARENA ) )      /* ARENA */
	{
	send_to_char("{CSome force stops you from disintegrating here.{x\n\r",ch);
	return;
	}

    if (!IS_IMMORTAL(ch))   /* immortal can disintegrate anyone */
    {
    if (IS_NPC(victim))
    if ((ch->level*1.2) < victim->level) 
	/* can disintegrate mobs up to 20% levels above you*/
	{
	send_to_char("{cYou failed to disintegrate them.{x\n\r",ch);
	return;
	}
    if (!IS_NPC(victim))
    if (ch->level*1.15 < victim->level) 
	/* can disintegrate pcs of 15% levels above you*/
	{
	send_to_char("{cYou failed to disintegrate them.{x\n\r",ch);
	return;
	}

    }

    act_color( "{C$N's thin light ray ###DISINTEGRATES### you!{x", 
		victim, NULL, ch, TO_CHAR, POS_RESTING, 0 );
    act_color( "{C$n's thin light ray ###DISINTEGRATES### $N!{x", 
		ch, NULL, victim, TO_NOTVICT, POS_RESTING, 0);
    act_color( "{CYour thin light ray ###DISINTEGRATES### $N!{x", 
		ch, NULL, victim, TO_CHAR, POS_RESTING, 0 );
    send_to_char( "{rYou have been KILLED!{x\n\r", victim );

    act("{C$N does not exist anymore!{x\n\r", ch, NULL, victim, TO_CHAR);
    act("{C$N does not exist anymore!{x\n\r", ch, NULL, victim, TO_ROOM);

send_to_char("{CYou wake up at your hometown tremendously wounded, but alive.{x\n\r",victim);
send_to_char("{CYou lost everything, but your life. It is a miracle you did not die.{x\n\r",victim);
send_to_char("{CYour form is disturbed for a time due to the attack.{x\n\r", victim);
send_to_char("{CYou are safe from other attacks during this time.{x\n\r",victim);

    /*  disintegrate the objects... */
    victim->gold = 0;
    victim->silver = 0;

    for ( obj = victim->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
	if ( 
	   ( number_percent() > (75-(2*(victim->level - ch->level))) )
	   || (IS_IMMORTAL(ch))
	   )
	extract_obj( obj );
    }

    if ( IS_NPC(victim) )
    {
      victim->pIndexData->killed++;
      kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
      extract_char( victim, TRUE );
      return;
    }
    
    extract_char( victim, FALSE );

    while ( victim->affected )
      affect_remove( victim, victim->affected );
    victim->affected_by   = 0;
    for (i = 0; i < 4; i++)
      victim->armor[i]= 100;
    victim->position      = POS_RESTING;
    victim->hit           = /* UMAX( 1, victim->hit  ); */ 1;
    victim->mana  = /* UMAX( 1, victim->mana ); */ 1;

    /*    victim->move    =  UMAX( 1, victim->move ); */
    /* RT added to prevent infinite deaths */
    REMOVE_BIT(victim->act, PLR_KILLER);
    REMOVE_BIT(victim->act, PLR_THIEF);
    REMOVE_BIT(victim->act, PLR_WANTED);
    REMOVE_BIT(victim->act, PLR_BOUGHT_PET);

    victim->pcdata->condition[COND_THIRST] = 40;
    victim->pcdata->condition[COND_HUNGER] = 40;
    victim->pcdata->condition[COND_FULL] = 40;
    victim->pcdata->condition[COND_BLOODLUST] = 40;

    for (tmp_ch = char_list; tmp_ch != NULL; tmp_ch = tmp_ch->next)
      if (tmp_ch->last_fought == victim)
        tmp_ch->last_fought = NULL;

    return;
}


void spell_ranger_staff( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    OBJ_DATA *staff;
    AFFECT_DATA tohit;
    AFFECT_DATA todam;

    staff = create_object( get_obj_index(OBJ_VNUM_RANGER_STAFF),level);
    send_to_char("You create a ranger's staff!\n\r",ch);
    act("$n creates a ranger's staff!",ch,NULL,NULL,TO_ROOM);

    staff->value[1] = level / 5;
    staff->value[2] = (3*level/4 + 6) / ( level / 8 );

    tohit.where		   = TO_OBJECT;
    tohit.type             = -1;
    tohit.level            = ch->level; 
    tohit.duration         = -1;
    tohit.location         = APPLY_HITROLL;
    tohit.modifier         = 2 + level/5;
    tohit.bitvector        = 0;
    affect_to_obj(staff,&tohit);

    todam.where		   = TO_OBJECT;
    todam.type             = -1;
    todam.level            = ch->level; 
    todam.duration         = -1;
    todam.location         = APPLY_DAMROLL;
    todam.modifier         = 2 + level/5;
    todam.bitvector        = 0;
    affect_to_obj(staff,&todam);
    staff->timer = level;
    obj_to_char(staff,ch);
    return;
}

void spell_transform( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if (is_affected(ch,sn))
    {
      send_to_char("You already have that spell cast on you.\n\r",ch);
      return;
    }

 switch (ch->class)
{ 
case CLASS_MAG:
case CLASS_MAL:
case CLASS_PRO:
case CLASS_ALC:
case CLASS_SPE:
case CLASS_DRD:
case CLASS_CLE: 
  if(!is_affected(ch,sn))
         {
        af.level = level;
        af.where = TO_AFFECTS;
        af.type  = sn;
        af.level = level;
        af.bitvector = 0;
        af.where = TO_AFFECTS;
        af.location = APPLY_HIT;
        af.modifier = ch->max_mana*.4;
        af.duration = level/4;
        affect_to_char(ch, &af );
        }
	break;

case CLASS_BRD:
case CLASS_INQ:
  if(!is_affected(ch,sn))
         {
        af.level = level;
        af.where = TO_AFFECTS;
        af.type  = sn;
        af.level = level;
        af.bitvector = 0;
        af.where = TO_AFFECTS;
        af.location = APPLY_HIT;
        af.modifier = ch->max_mana*.3;
        af.duration = level/5;
        affect_to_char(ch, &af );
        }
	break;

case CLASS_PDN:
case CLASS_MNK:
case CLASS_BKT:
case CLASS_RAN:
case CLASS_ASN:
  if(!is_affected(ch,sn))
         {
        af.level = level;
        af.where = TO_AFFECTS;
        af.type  = sn;
        af.level = level;
        af.bitvector = 0;
        af.where = TO_AFFECTS;
        af.location = APPLY_HIT;
        af.modifier = ch->max_mana*.2;
        af.duration = level/6;
        affect_to_char(ch, &af );
        }
	break;
case CLASS_THI:
case CLASS_MER:
case CLASS_WAR:
  if(!is_affected(ch,sn))
         {
        af.level = level;
        af.where = TO_AFFECTS;
        af.type  = sn;
        af.level = level;
        af.bitvector = 0;
        af.where = TO_AFFECTS;
        af.location = APPLY_HIT;
        af.modifier = 0;
        af.duration = level/7;
        affect_to_char(ch, &af );
        }
 	break;
default:
send_to_char("{cHmm This is a but write note to admin with you class and name.{x",ch);
  if(!is_affected(ch,sn))
         {
        af.level = level;
        af.where = TO_AFFECTS;
        af.type  = sn;
        af.level = level;
        af.bitvector = 0;
        af.where = TO_AFFECTS;
        af.location = APPLY_HIT;
        af.modifier = 0;
        af.duration = level/2;
        affect_to_char(ch, &af );
        }
  send_to_char("You suddenly grow bigger!\n\r",ch);
	break;
 }
}

void spell_mana_transfer( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (victim == ch)
    {
      send_to_char("You would implode if you tried to transfer mana to yourself.\n\r",ch);
      return;
    }

  if (ch->kingdom != victim->kingdom)
    {
      send_to_char("You may only cast this spell on fellow kingdom members.\n\r",ch);
      return;
    }

  if (ch->hit < 50)
    damage_spell(ch,ch,50,sn,DAM_NONE, TRUE);
  else {
    damage_spell(ch,ch,50,sn,DAM_NONE, TRUE);
    victim->mana = UMIN(victim->max_mana, victim->mana + number_range(20,120));
  }
}
    
void spell_mental_knife( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  damage_spell(ch,victim,number_range(1,4)*level,sn,DAM_MENTAL, TRUE);

  if (!is_affected(victim,sn) && !saves_spell(level, victim, DAM_MENTAL))
    {
      af.where		    = TO_AFFECTS;
      af.type               = sn;
      af.level              = level; 
      af.duration           = level;
      af.location           = APPLY_INT;
      af.modifier           = -7;
      af.bitvector          = 0;
      affect_to_char(victim,&af);

      af.location = APPLY_WIS;
      affect_to_char(victim,&af);
      act("Your mental knife sears $N's mind!",ch,NULL,victim,TO_CHAR);
      act("$n's mental knife sears your mind!",ch,NULL,victim,TO_VICT);
      act("$n's mental knife sears $N's mind!",ch,NULL,victim,TO_NOTVICT);
    }
}

void spell_celestial_spirit( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *spirit;
  AFFECT_DATA af;
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char("{cYou lack the power to summon another spirit right now.{x\n\r", ch);
      return;
    }

  send_to_char("You attempt to summon a celestial spirit.\n\r",ch);
  act("$n attempts to summon a spirit from the celestial plane.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SPIRIT)
	{
	  send_to_char("Two spirits are more than you can control!\n\r",ch);
	  return;
	}
    }

  spirit = create_mobile( get_mob_index(MOB_VNUM_SPIRIT) );

  for (i=0;i < MAX_STATS; i++)
    {
      spirit->perm_stat[i] = ch->perm_stat[i];
    }

  spirit->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
		: URANGE(ch->pcdata->perm_hit,1*ch->pcdata->perm_hit,30000);
  spirit->hit = spirit->max_hit;
  spirit->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  spirit->mana = spirit->max_mana;
  spirit->level = ch->level;
  for (i=0; i < 3; i++)
    spirit->armor[i] = interpolate(spirit->level,100,-100);
  spirit->armor[3] = interpolate(spirit->level,100,0);
  spirit->gold = 0;
  spirit->timer = 0;
  spirit->damage[DICE_NUMBER] = number_range(level/15, level/10);   
  spirit->damage[DICE_TYPE] = number_range(level/3, level/2);
  spirit->damage[DICE_BONUS] = number_range(level/8, level/6);

  char_to_room(spirit,ch->in_room);
  send_to_char("A spirit arrives from the celestial plane.\n\r",ch);
  act("A spirit arrives from the celestial plane!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  if (number_percent() < 40)
    {
      if ( can_see( spirit, ch ) )
        do_say(spirit, "Your planar meddling is not appreciated!!");
      else
        do_say(spirit, "Who dares disturb the planar balance!!??");
      do_murder(spirit, ch->name);
    }
  else {
    SET_BIT(spirit->affected_by, AFF_CHARM);
    spirit->master = spirit->leader = ch;
  }

}

void spell_scourge( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *tmp_vict;
  CHAR_DATA *tmp_next;
  char buf[MAX_STRING_LENGTH];

  static const sh_int dam_each[] = 
    {
	  0,
	  0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,
	  0,   0,   0,   0,  25,	 30,  35,  40,  45,  50,
	 55,  60,  65,  70,  75,	 77,  79,  81,  83,  85,
	 87,  89,  91,  93,  95,	 97,  98,  99, 100, 101,
	102, 103, 104, 105, 106,	107, 108, 109, 110, 111
	};
  int dam;
  
  level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
  level	= UMAX(0, level);
  dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );

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
		    (is_affected(ch,gsn_doppelganger)&&!IS_IMMORTAL(tmp_vict))?
		     ch->doppel->name : ch->name);
		  do_yell(tmp_vict,buf);
		}
	    }
	    
	  if (!is_affected(tmp_vict,sn)) {
	    

	    if (number_percent() < 2 * level)
	      spell_poison(gsn_poison, level, ch, tmp_vict, TARGET_CHAR);

	    if (number_percent() < 2 * level)
	      spell_blindness(gsn_blindness,level,ch,tmp_vict, TARGET_CHAR);

	    if (number_percent() < 2 * level)
	      spell_weaken(gsn_weaken, level, ch, tmp_vict, TARGET_CHAR);

            if (saves_spell(level,tmp_vict, DAM_FIRE))
	      dam /= 2;
	    damage_spell( ch, tmp_vict, dam, sn, DAM_FIRE, TRUE );
	  }

	}
    }
}

void spell_doppelganger( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( (ch == victim) || 
      (is_affected(ch, sn) && (ch->doppel == victim)) )
    {
      act("You already look like $M.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_NPC(victim))
    {
     act("$N is too different from yourself to mimic.",ch,NULL,victim,TO_CHAR);
     return;
   }

  if (IS_IMMORTAL(victim))
    {
      send_to_char("Yeah, sure. And I'm the Pope.\n\r",ch);
      return;
    }

  act("You change form to look like $N.",ch,NULL,victim,TO_CHAR);
  act("$n changes form to look like YOU!",ch,NULL,victim,TO_VICT);
  act("$n changes form to look like $N!",ch,NULL,victim,TO_NOTVICT);

  af.where 		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = (2 * level)/3;
  af.location           = APPLY_NONE;
  af.modifier           = 0;
  af.bitvector          = 0;

  affect_to_char(ch,&af);  
  ch->doppel = victim;

}
  
void spell_manacles( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (!IS_SET(victim->act, PLR_WANTED))
    {
      act("But $N is not wanted.",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (!is_affected(victim, sn) && !saves_spell(level, victim, DAM_CHARM))
    {
      AFFECT_DATA af;

      af.where		    = TO_AFFECTS;
      af.type               = sn;
      af.level              = level; 
      af.duration           = 5 + level/5;
      af.bitvector          = 0;

      af.modifier           = 0 - (get_curr_stat(victim, STAT_DEX)-4);
      af.location           = APPLY_DEX;
      affect_to_char(victim, &af);

      af.modifier           = -5;
      af.location           = APPLY_HITROLL;
      affect_to_char(victim, &af);

	
      af.modifier           = -10;
      af.location           = APPLY_DAMROLL;
      affect_to_char(victim, &af);

      spell_charm_person(gsn_charm_person, level, ch, vo, TARGET_CHAR);
    }
}

void spell_shield_justice( int sn, int level, CHAR_DATA *ch, void *vo,int target) {
  int shield_vnum;
  OBJ_DATA *shield;
  
  if (level >= 41)
    shield_vnum = OBJ_VNUM_JUSTICE_SHIELD4;
  else if (level >= 31)
    shield_vnum = OBJ_VNUM_JUSTICE_SHIELD3;
  else if (level >= 21)
    shield_vnum = OBJ_VNUM_JUSTICE_SHIELD2;
  else shield_vnum = OBJ_VNUM_JUSTICE_SHIELD1;

  shield = create_object( get_obj_index(shield_vnum), level );
  shield->timer = level;
  shield->cost  = 0;
  obj_to_char(shield, ch);
  
  act("You create $p!",ch,shield,NULL,TO_CHAR);
  act("$n creates $p!",ch,shield,NULL,TO_ROOM);
}
  
void spell_guard_call( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *guard;
  CHAR_DATA *guard2;
  char buf[] = "Guards! Guards!";
  AFFECT_DATA af;
  int i;

  if (is_affected(ch, sn))
    {
      send_to_char("The Senate guards are not about to heed your call.\n\r", ch);
      return;
    }

  do_yell(ch, buf);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch)
	{
	  do_say(gch, "What? I'm not good enough?");
	  return;
	}
    }

  guard = create_mobile( get_mob_index(MOB_VNUM_SPECIAL_GUARD) );

  for (i=0;i < MAX_STATS; i++)
    guard->perm_stat[i] = ch->perm_stat[i];

  guard->max_hit = ch->max_hit*2;
  guard->hit = guard->max_hit*2;
  guard->max_mana = ch->max_mana*2;
  guard->mana = guard->max_mana*2;
  guard->alignment = ch->alignment;
  guard->level = ch->level;
  for (i=0; i < 3; i++)
    guard->armor[i] = interpolate(guard->level,100,-200);
  guard->armor[3] = interpolate(guard->level,100,-100);
  guard->sex = ch->sex;
  guard->gold = 0;
  guard->timer = 0;

  guard2 = create_mobile(guard->pIndexData);
  clone_mobile(guard,guard2);
  
  SET_BIT(guard->affected_by, AFF_CHARM);
  SET_BIT(guard2->affected_by, AFF_CHARM);
  guard->master = guard2->master = ch;
  guard->leader = guard2->leader = ch;

  char_to_room(guard,ch->in_room);
  char_to_room(guard2,ch->in_room);
  send_to_char("Two Senate guards come to your rescue!\n\r",ch);
  act("Two Senate guards come to $n's rescue!",ch,NULL,NULL,TO_ROOM);
  
  
  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 30;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
	

}

void spell_nightwalker( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *walker;
  AFFECT_DATA af;
  char buf[100];
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char("You feel too weak to summon a Nightwalker now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a Nightwalker.\n\r",ch);
  act("$n attempts to summon a Nightwalker.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_NIGHTWALKER)
	{
	  send_to_char("Two Nightwalkers are more than you can control!\n\r",ch);
	  return;
	}
    }

  walker = create_mobile( get_mob_index(MOB_VNUM_NIGHTWALKER) );

  for (i=0;i < MAX_STATS; i++)
    {
      walker->perm_stat[i] = ch->perm_stat[i];
    }

  walker->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : URANGE(ch->pcdata->perm_hit,1*ch->pcdata->perm_hit,30000);
  walker->hit = walker->max_hit;
  walker->max_mana = ch->max_mana;
  walker->mana = walker->max_mana;
  walker->level = ch->level;
  for (i=0; i < 3; i++)
    walker->armor[i] = interpolate(walker->level,100,-100);
  walker->armor[3] = interpolate(walker->level,100,0);
  walker->gold = 0;
  walker->timer = 0;
  walker->damage[DICE_NUMBER] = number_range(level/15, level/10);   
  walker->damage[DICE_TYPE]   = number_range(level/3, level/2);
  walker->damage[DICE_BONUS]  = 0;
   
  char_to_room(walker,ch->in_room);
  send_to_char("A Nightwalker rises from the shadows!\n\r",ch);
  act("A Nightwalker rises from the shadows!",ch,NULL,NULL,TO_ROOM);
  xprintf(buf, "A Nightwalker kneels before you.");
  send_to_char( buf, ch );
  xprintf(buf, "A Nightwalker kneels before %s!", ch->name );
  act( buf, ch, NULL, NULL, TO_ROOM );

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(walker->affected_by, AFF_CHARM);
  walker->master = walker->leader = ch;

}
  
void spell_eyes( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *ori_room;

    if (( (victim = get_char_world(ch, target_name)) == NULL) || (!can_see(ch,victim)))
      {
	send_to_char("Your spy network reveals no such player.\n\r",ch);
	return;
      }

    if ( (victim->level > ch->level + 7) && saves_spell(level, victim, DAM_NONE) )
      {
	send_to_char("Your spy network cannot find that player.\n\r",ch);
	return;
      }

   if (ch->kingdom_rank < 4)
     {
      send_to_char( "You rank do not get this spell.\n\r", ch);
      return;
     }        
  
    if (ch==victim)
      do_look( ch, "auto" );
    else {
      ori_room = ch->in_room;
      char_from_room( ch );
      char_to_room( ch, victim->in_room );
      do_look( ch, "auto" );
      char_from_room( ch );
      char_to_room( ch, ori_room );
    }
}

void spell_shadow_cloak( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected by a shadow cloak.\n\r",ch);
	else
	  act("$N is already protected by a shadow cloak.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where		= TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 24;
    af.modifier  = -level;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel the shadows protect you.\n\r", victim );
    if ( ch != victim )
	act("A cloak of shadows protect $N.",ch,NULL,victim,TO_CHAR);
    return;
}
  
void spell_nightfall( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af,af2;

    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("This room has already been affected by nightfall.\n\r",ch);
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
    af.bitvector = AFF_ROOM_NIGHTFALL;
    affect_to_room( ch->in_room, &af );

    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.level    = level;
    af2.duration  = ch->level / 10;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af2 );
    send_to_char( "The room starts to dim from view.\n\r", ch);
    act("The room starts to dim from view by $n's magic.",ch,NULL,NULL,TO_ROOM);
    return;

}
	      
void spell_mirror( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
  int mirrors,new_mirrors;
  CHAR_DATA *gch;
  CHAR_DATA *tmp_vict;
  char long_buf[MAX_STRING_LENGTH];
  char short_buf[20];
  int order;

  if (IS_NPC(victim)) {
    send_to_char("Only players can be mirrored.\n\r",ch);
    return;
  }
/*
  for (mirrors = 0, gch = char_list; gch != NULL; gch = gch->next)
    if (IS_NPC(gch) && is_affected(gch,gsn_mirror)
	&& is_affected(gch,gsn_doppelganger) && gch->doppel == victim)
      mirrors++;*/

 /* Engur's code*/
  for (mirrors = 0, gch = char_list; gch != NULL;gch = gch->next) {
    if (gch == NULL) break;
    if (IS_NPC(gch) && is_affected(gch,gsn_mirror) 
	&& !str_cmp(gch->name,victim->name))
	mirrors++;
   }

if (mirrors >= level/5) {
  if (ch==victim) 
    send_to_char("You cannot be further mirrored.\n\r",ch);
  else
    act("$N cannot be further mirrored.",ch,NULL,victim,TO_CHAR);
  return;
}

  af.where     = TO_AFFECTS;
  af.level     = level;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;

  /* Find the permanent description of the mirror for after the mirrored
     person quits */
  for (tmp_vict = victim; is_affected(tmp_vict,gsn_doppelganger);
       tmp_vict = tmp_vict->doppel);

  xprintf(long_buf, "%s%s%s is here.\n\r",tmp_vict->pcdata->pretitle,
	tmp_vict->name,
	tmp_vict->pcdata->title);
  xprintf(short_buf, tmp_vict->name);

  order = number_range(0,level/5 - mirrors); /* randomly place the real one */

  for (new_mirrors=0; mirrors + new_mirrors < level/5;new_mirrors++) {
    gch = create_mobile( get_mob_index(MOB_VNUM_MIRROR_IMAGE) );

    free_string(gch->name);
    free_string(gch->short_descr);
    free_string(gch->long_descr);
    free_string(gch->description);
    gch->exp  = tmp_vict->exp;
    gch->practice = tmp_vict->level;	
    gch->name = str_dup(tmp_vict->name);
    gch->short_descr = str_dup(short_buf);
    gch->long_descr = str_dup(long_buf);
    
gch->description = (tmp_vict->description == NULL)? 
                       NULL : str_dup(tmp_vict->description);
    gch->sex = tmp_vict->sex;
    
    af.type = gsn_doppelganger;
    af.duration = level;
    affect_to_char(gch,&af);
    af.type = gsn_mirror;
    af.duration = -1;
    affect_to_char(gch,&af);

    gch->max_hit = gch->hit = 1;
    gch->level = 1;
    gch->doppel = victim;
    gch->master = victim;
    gch->size = ch->size;
    char_to_room(gch,victim->in_room);

    if (new_mirrors == order) {
      char_from_room(victim);
      char_to_room(victim,gch->in_room);
    }
      

    if (ch==victim) {
      send_to_char("A mirror image of yourself appears beside you!\n\r",ch);
      act("A mirror image of $n appears beside $M!",ch,NULL,victim,TO_ROOM);
    }
    else {
      act("A mirror of $N appears beside $M!",ch,NULL,victim,TO_CHAR);
      act("A mirror of $N appears beside $M!",ch,NULL,victim,TO_NOTVICT);
      send_to_char("A mirror image of yourself appears beside you!\n\r",
		   victim);
    }
    
  }
}    
 
void spell_garble( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim) {
    send_to_char("Garble whose speech?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn)) {
    act("$N's speech is already garbled.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
      send_to_char("You cannot garble that person.\n\r",ch);
      return;
  } 

  if (saves_spell(level,victim, DAM_NONE))
    return;

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 10;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  affect_to_char(victim,&af);

  act("You have garbled $N's speech!",ch,NULL,victim,TO_CHAR);
  send_to_char("You feel your tongue contort.\n\r",victim);
}

void spell_confuse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (is_affected(victim,gsn_confuse))
  {
    act("$N is already thoroughly confused.",ch,NULL,victim,TO_CHAR);
    return;
  }
  
  if (saves_spell(level,victim, DAM_MENTAL))
  {
    send_to_char("You failed.\n\r", ch);
    return;
  }


  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 12;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  affect_to_char(victim,&af);
  act("$n looks bewildered.",victim,NULL,NULL,TO_ROOM);
  send_to_char("You have lost track of your plans.\n\r", victim);
}

/* Spells by Galadriel */

void spell_terangreal( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
  
    if (IS_NPC(victim))
	return;

    af.where		= TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 10;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You are overcome by a sudden surge of fatigue.\n\r",
		     victim );
	act( "$n falls into a deep sleep.", victim, NULL, NULL, TO_ROOM );
	victim->position = POS_SLEEPING;
    }

    return;
}
    
void spell_sedai_heal( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{

    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
      {
	send_to_char
	  ("The sa'angreal has been used for this purpose too recently.\n\r",
	   ch);
	return;
      }
    af.where		= TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 50;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    ch->hit = UMIN( ch->hit + 150, ch->max_hit );
    update_pos( ch );
    send_to_char( "A warm feeling fills your body.\n\r", ch );
    act("$n looks better.", ch, NULL, NULL, TO_ROOM);
    return;
}  
  
  
void spell_sedai_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char("The sa'angreal has been used for that too recently.\n\r"
		   ,ch); 
      return;
    }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = level;
  af.location  = APPLY_AC;
  af.modifier  = -30;
  af.bitvector = 0;
  affect_to_char( ch, &af );
  act( "$n is surrounded by a mystical shield.",ch, NULL,NULL,TO_ROOM );
  send_to_char( "You are surrounded by a mystical shield.\n\r", ch );
  return;
}
    

void spell_saidar( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
  AFFECT_DATA af;
  
  if ( is_affected( ch, sn ) )
    {
      send_to_char
	("The sa'angreal has been used for this purpose too recently.\n\r",
	 ch);
      return;
    }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 70;
  af.location  = 0;
  af.modifier  = 0;
  af.bitvector = 0;
  affect_to_char( ch, &af );
  dam = dice(level, 7);
  
  damage_spell(ch,victim,dam,sn,DAM_HOLY, TRUE);
  
}  
  
void spell_amnesia( int sn, int level, CHAR_DATA *ch, void *vo, int target )  
{
  int i;
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (IS_NPC(victim))
    return;
  
  if ( IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
  {
   send_to_char( "Something blocks the effect of the spell!\n\r", ch );
        return;
  }
  
    i = number_range(0,MAX_SKILL);
    if(victim->pcdata->learned[i] == 0)
    return;

    if(victim->pcdata->learned[i] >= 1) 
    victim->pcdata->learned[i] -= 10;
    else
    victim->pcdata->learned[i] = 1;

  act("You feel your memories slip away.",victim,NULL,NULL,TO_CHAR);
  act("$n gets a blank look on $s face.",victim,NULL,NULL,TO_ROOM);
}
    

void spell_chaos_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *blade;
  
  blade = create_object( get_obj_index(OBJ_VNUM_CHAOS_BLADE),level);
  send_to_char("You create a blade of chaos!\n\r",ch);
  act("$n creates a blade of chaos!",ch,NULL,NULL,TO_ROOM);

  blade->timer = level / 2;
  
  obj_to_char(blade,ch);
}    

void spell_brand( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *tattoo;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int i;

  if (IS_NPC(victim))
    {
      act("$N is too dumb to worship you!", ch, NULL, victim, TO_CHAR);
      return;
    }

  for (i = 0; i < MAX_RELIGION; i++)
    {
      if (!str_cmp(ch->name, religion_table[i].leader))
	{
  	  tattoo = get_eq_char(victim, WEAR_TATTOO);
   	  if (tattoo != NULL)
    	    {
     	      act("$N is already branded!",
		ch, NULL, victim, TO_CHAR);
      	      act("$n tried to give you another brand but failed.",
		ch, NULL, victim, TO_VICT);
              act("$n tried to give $N another brand but failed.",
		ch, NULL, victim, TO_NOTVICT);
      	      return;
    	    }
  	  else
    	    { 
      	      tattoo = create_object(get_obj_index(religion_table[i].vnum),60);
       	      act("You brand $N with $p!",ch, tattoo, victim, TO_CHAR);
      	      act("$n brands $N with $p!",ch,tattoo,victim,TO_NOTVICT);
      	      act("$n brands you painfully with $p!",ch,tattoo,victim,TO_VICT);

      	      obj_to_char(tattoo,victim);
      	      equip_char(victim, tattoo, WEAR_TATTOO);
	      return;
	    }
	}    
    }
  send_to_char("You do not have a brand.\n\r", ch);
  return;    
}

void spell_remove_brand( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *tattoo;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
 
  tattoo = get_eq_char(victim, WEAR_TATTOO);
  if (tattoo != NULL)
    {
      extract_obj(tattoo);
      act("Through a painful process, your brand has been destroyed by $n.",
	ch, NULL, victim, TO_VICT);
      act("You remove the brand from $N.", ch, NULL, victim, TO_CHAR);
      act("$N's brand is destroyed by $n.", ch, NULL, victim, TO_NOTVICT);
    }
  else
    act("$N does not bear a brand.", ch, NULL, victim, TO_CHAR);
}


void spell_wrath( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
  AFFECT_DATA af;
  
  if ( !IS_NPC(ch) && IS_EVIL(ch) )
    victim = ch;
  
  if ( IS_GOOD(victim) )
    {
      act( "The gods protect $N.", ch, NULL, victim, TO_ROOM );
      return;
    }
  
  if ( IS_NEUTRAL(victim) )
    {
      act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
      return;
    }
  if (victim->hit > (ch->level * 8))
    dam = dice( 2*level, 4 );
  else
    dam = UMIN(victim->hit, dice(2*level,4));
  if ( saves_spell( level, victim, DAM_HOLY ) )
    dam /= 2;
  damage_spell( ch, victim, dam, sn, DAM_HOLY, TRUE );
  
  if ( IS_AFFECTED(victim, AFF_CURSE) || saves_spell( level, victim, DAM_HOLY ) )
    return;
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 2*level;
  af.location  = APPLY_HITROLL;
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


void spell_randomizer(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  ROOM_INDEX_DATA *pRoomIndex;
  EXIT_DATA *pexit;
  int d0;
  int d1;
  AFFECT_DATA af;
  char log_buf[MAX_INPUT_LENGTH];

   send_to_char("This spell have been removed!\n\r",ch);
   return;

  if ( is_affected( ch, sn ) )
    {
      send_to_char
	("Your power of randomness has been exhausted for now.\n\r",
	 ch);
      return;
    }
  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(
	    "This room is far too orderly for your powers to work on it.\n\r",
		   ch);
      return;
    }

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = UMIN(level + 15, 60);
  af.location  = 0;
  af.modifier  = 0;
  af.bitvector = 0;

  pRoomIndex = get_room_index(ch->in_room->vnum);

  if (number_bits(1) == 0)
    {
      send_to_char("Despite your efforts, the universe resisted chaos.\n\r",
		   ch);
      if (ch->trust >= MAX_LEVEL-12)
        af.duration  = 1;
      else
	af.duration = 2*level;
      affect_to_char(ch, &af);
      return;
    }
  for (d0 = 0; d0 < 5; d0++)
    {
      d1 = number_range(d0, 5);
      pexit = pRoomIndex->exit[d0];
      pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
      pRoomIndex->exit[d1] = pexit;

    }
  if (ch->trust >= MAX_LEVEL-12)
    af.duration = 1;
  else
    af.duration = 4*level;
  affect_to_char(ch, &af);
  send_to_char("The room was successfully randomized!\n\r", ch);
  send_to_char("You feel very drained from the effort.\n\r", ch);
  ch->hit -= UMIN(200, ch->hit/2);

  xprintf(log_buf, "%s used randomizer in room %d", ch->name, ch->in_room->vnum);
  log_string(log_buf);

}

void spell_stalker( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim;
  CHAR_DATA *stalker;
  AFFECT_DATA af;
  int i;
  char log_buf[MAX_INPUT_LENGTH];

  if (is_affected(ch, sn))
    {
      send_to_char("You have just sent a stalker.\n\r", ch);
      return;
    }

  if ( ( victim = get_char_world( ch, target_name ) ) == NULL
      ||   victim == ch || victim->in_room == NULL
      || IS_NPC(victim) || !IS_SET(victim->act, PLR_WANTED))
    {
      send_to_char("You failed.\n\r", ch);
      return;
    }

  send_to_char("You attempt to summon a stalker.\n\r",ch);
  act("$n attempts to summon a stalker.",ch,NULL,NULL,TO_ROOM);

  stalker = create_mobile( get_mob_index(MOB_VNUM_STALKER) );
  
  for (i=0;i < MAX_STATS; i++)
    {
      stalker->perm_stat[i] = victim->perm_stat[i];
    }

  stalker->max_hit = UMIN(30000,2 * victim->max_hit);
  stalker->hit = stalker->max_hit;
  stalker->max_mana = victim->max_mana;
  stalker->mana = stalker->max_mana;
  stalker->level = victim->level;
  for (i=0; i < 3; i++)
    stalker->armor[i] = interpolate(stalker->level,100,-100);
  stalker->armor[3] = interpolate(stalker->level,100,0);
  stalker->gold = 0;
  stalker->invis_level = 51;
  /*code by rhax & engur */
  char_to_room(stalker,ch->in_room);
  stalker->last_fought = victim;
  act("An invisible stalker arrives to stalk $n!",victim,NULL,NULL,TO_ROOM);
  send_to_char("An invisible stalker has been summoned.\n\r", ch);

  xprintf(log_buf, "%s used stalker on %s", ch->name, victim->name);
  log_string(log_buf);
  
  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 2;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}

/*  
void spell_tesseract( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim;
  CHAR_DATA *wch;
  CHAR_DATA *wch_next;
  bool gate_pet;

  if( (victim = get_char_world(ch,target_name))==NULL
	|| victim == ch
	|| victim->in_room == NULL
	|| ch->in_room == NULL) 
	 {
	  send_to_char("You failed.\n\r",ch);
	  return;
	 }

  
  if ( !can_see_room(ch,victim->in_room) 
      ||   (is_safe_nomessage(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON))
      ||   room_is_private(victim->in_room)
      ||   IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
      ||   IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
      ||   (!IS_NPC(victim) && victim->level > LEVEL_HERO)
      ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
      ||   (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOSUMMON) && is_safe_nomessage(ch,victim))
      ||   (saves_spell( level, victim, DAM_NONE ) ) )
    {
      send_to_char( "You failed.\n\r", ch );
      return;
    }

if ( strstr( victim->in_room->area->builders, "Unlinked" ) )
{
     send_to_char( "You can't tesseract to areas that aren't linked yet!\n\r",ch );
     return;
}
  
  if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
    gate_pet = TRUE;
  else
    gate_pet = FALSE;

  for (wch = ch->in_room->people; wch != NULL; wch = wch_next)
    {
      wch_next = wch->next_in_room;
      if (is_same_group(wch, ch) && wch != ch)
	{
	  act("$n utters some strange words and, with a sickening lurch, you feel time",ch, NULL, wch, TO_VICT);
	  act("and space shift around you.", ch, NULL, wch, TO_VICT);
          if(victim->in_room == NULL) {
		bug("Tesseract: victim room has become NULL!!!",0);
                return;
          }
	  char_from_room(wch);
	  char_to_room(wch,victim->in_room);
	  act("$n arrives suddenly.",wch,NULL,NULL,TO_ROOM);
	  if (wch->in_room == NULL)
	     bug("Tesseract: other char sent to NULL room",0); 
	  else
	     do_look(wch,"auto");
	} 
    }
 
  act("With a sudden flash of light, $n and $s friends disappear!",ch,NULL,NULL,TO_ROOM);
  send_to_char("As you utter the words, time and space seem to blur.  You feel as though\n\rspace and time are shifting all around you while you remain motionless.\n\r",ch);
  char_from_room(ch);
  char_to_room(ch,victim->in_room);

  act("$n arrives suddenly.",ch,NULL,NULL,TO_ROOM);
  if (ch->in_room == NULL)
     bug("Tesseract: char sent to NULL room",0); 
  else
     do_look(ch,"auto");

  if (gate_pet)
    {
      send_to_char("You feel time and space shift around you.\n\r",ch->pet);
      char_from_room(ch->pet);
      char_to_room(ch->pet,victim->in_room);
      act("$n arrives suddenly.",ch->pet,NULL,NULL,TO_ROOM);
      if (ch->pet->in_room == NULL)
         bug("Tesseract: pet sent to NULL room",0); 
      else
         do_look(ch->pet,"auto");
    }
}
*/

void spell_tesseract( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim;
   CHAR_DATA *wch;
   CHAR_DATA *wch_next;
	if ((ch->pcdata->quest !=NULL))
	{
		QUEST_DATA *quest = ch->pcdata->quest;
		if (quest->protection)	
		{
			send_to_char( "You are in a quest. Tesseract  is disabled.\n\r", ch );
			return;			
		}
	}
   if( ( victim = get_char_world(ch,target_name))==NULL
      || victim == ch
      || victim->in_room == NULL
      || ch->in_room == NULL 
      || !can_see_room(ch,victim->in_room) 
      || room_is_private(victim->in_room)
      || IS_SET(victim->in_room->room_flags, ROOM_NOSUMMON)
      || IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
      || (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */ 
      || (IS_NPC(victim) && is_safe_nomessage(ch, victim)
      && IS_SET(victim->imm_flags,IMM_SUMMON))
      || (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOSUMMON)
      && is_safe_nomessage(ch,victim))
      || (saves_spell( level, victim, DAM_OTHER ) ) ) 
   {
      send_to_char( "You failed.\n\r", ch );
      return;
   }

   if ( strstr( victim->in_room->area->builders, "Unlinked" ) )
   {
     send_to_char( "You can't tesseract to areas that aren't linked yet!\n\r",ch );
     return;
   }
  
   if ( strstr( victim->in_room->area->builders, "nogate" ) )
   {
     send_to_char( "You can't tesseract to areas Quest blocked for it!\n\r",ch );
     return;
   }  
   act("A rift in space-time opens before you.",victim,NULL,NULL,TO_ROOM);
   for (wch = ch->in_room->people; wch != NULL; wch = wch_next)
   {
      wch_next = wch->next_in_room;
      if (is_same_group(wch, ch) && wch != ch)
      {
         send_to_char("A rift in space-time opens before you and you feel yourself propelled into it.\n\r",wch);
         if(victim->in_room == NULL) {
            bug("Tesseract: victim room has become NULL!!!",0);
            return;
         }
         char_from_room(wch);
         char_to_room(wch,victim->in_room);
         act("$n steps out of the rift.",wch,NULL,NULL,TO_ROOM);
         if (wch->in_room == NULL)
        bug("Tesseract: other char sent to NULL room",0); 
     else
        do_look(wch,"auto");
      } 
   }
 
   act("$n waves $s hand, and $s group disappears into a rift in space-time.",ch,NULL,NULL,TO_ROOM);
   send_to_char("You open a rift in space-time and propel your group through it.\n\r",ch);
   char_from_room(ch);
   char_to_room(ch,victim->in_room);

   act("$n steps out of the rift and waves $s hand, the rift closes.",ch,NULL,NULL,TO_ROOM);
   if (ch->in_room == NULL)
      bug("Tesseract: char sent to NULL room",0); 
   else
      do_look(ch,"auto");
  
}


void spell_brew( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{   
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    OBJ_DATA *potion;
    OBJ_DATA *vial;
    int spell;

    if (obj->item_type != ITEM_TRASH && obj->item_type != ITEM_TREASURE
	&& obj->item_type != ITEM_KEY)
      {
	send_to_char("That can't be transformed into a potion.\n\r",ch);
	return;
      }
    
    if (obj->wear_loc != -1)
      {
	send_to_char("The item must be carried to be brewed.\n\r",ch);
	return;
      }

    for( vial=ch->carrying; vial != NULL; vial=vial->next_content )
      if ( vial->pIndexData->vnum == OBJ_VNUM_EMPTY_VIAL )
        break;
    if (  vial == NULL )  {
	send_to_char( "You don't have any vials to brew the potion into.\n\r"
			, ch );
	return;
    }
 

    if (number_percent() < 50)
      { 
	send_to_char("You failed and destroyed it.\n\r", ch);
	extract_obj(obj);
	return;
      }
	
    if (obj->item_type == ITEM_TRASH)
      potion = create_object( get_obj_index(OBJ_VNUM_POTION_SILVER), level);
    else if (obj->item_type == ITEM_TREASURE)
      potion = create_object( get_obj_index(OBJ_VNUM_POTION_GOLDEN), level);
    else
      potion = create_object( get_obj_index(OBJ_VNUM_POTION_SWIRLING), level);

    /* now set the potion's values */

    spell = 0;

    potion->value[0] = level;

    if (obj->item_type == ITEM_TRASH)
      {
	if (number_percent() < 20)
	  spell = skill_lookup("fireball");
	else if (number_percent() < 40)
	  spell = skill_lookup("cure poison");
	else if (number_percent() < 60)
	  spell = skill_lookup("cure blind");
	else if (number_percent() < 80)
	  spell = skill_lookup("cure disease");
	else
	  spell = skill_lookup("word of recall");
      }
    else if (obj->item_type == ITEM_TREASURE)
      {
	switch(number_bits(3)) {
	case 0:
	  spell = skill_lookup("cure critical");
	  break;
	case 1:
	  spell = skill_lookup("haste");
	  break;
	case 2:
	  spell = skill_lookup("frenzy");
	  break;
	case 3:
	  spell = skill_lookup("create spring");
	  break;
	case 4:
	  spell = skill_lookup("holy word");
	  break;
	case 5:
	  spell = skill_lookup("invis");
	  break;
	case 6:
	  spell = skill_lookup("heal");
	  break;
	case 7:
	  spell = skill_lookup("heal");
	  break;
	  
	}
      }
    else
      {
	if (number_percent() < 20)
	  spell = skill_lookup("detect magic");
	else if (number_percent() < 40)
	  spell = skill_lookup("detect invis");
	else if (number_percent() < 65)
	  spell = skill_lookup("pass door");
	else
	  spell = skill_lookup("acute vision");
      }
    
    potion->value[1] = spell;
    extract_obj(obj);
    xprintf(buf1, "%s labeled '%s'", potion->short_descr, skill_table[potion->value[1]].name);
    free_string(potion->short_descr);
    potion->short_descr = str_dup(buf1);
    xprintf(buf2, "%s %s", potion->name, skill_table[potion->value[1]].name);
    free_string(potion->name);
    potion->name = str_dup(buf2);
    act("You brew $p from your resources!", ch, potion, NULL, TO_CHAR);
    act("$n brews $p from $s resources!", ch, potion, NULL, TO_ROOM);

    obj_to_char(potion, ch);
    extract_obj( vial );

}
void spell_shadowlife( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int weapon_vnum;
  OBJ_DATA *weapon;
  AFFECT_DATA *pAf;    

  weapon_vnum = OBJ_VNUM_KUJI_WEAPON;
  weapon = create_object( get_obj_index(weapon_vnum), level + 5);
  weapon->timer = 2 * level;
  weapon->cost  = 0;
  weapon->weight = level/5;
  weapon->value[0] = WEAPON_EXOTIC;
  weapon->value[1] = level/8;
  weapon->value[2] = level/8;

    pAf             =   new_affect();
    pAf->location   =   APPLY_DAMROLL;
    pAf->modifier   =   level/8;
    pAf->where      =   TO_OBJECT;
    pAf->type       =   -1;
    pAf->duration   =   -1;
    pAf->bitvector  =   0;
    pAf->level      =  weapon->level;
    pAf->next       =  weapon->affected;
    weapon->affected  =   pAf;

    pAf             =   new_affect();
    pAf->location   =   APPLY_HITROLL;
    pAf->modifier   =   level/8;
    pAf->where      =   TO_OBJECT;
    pAf->type       =   -1;
    pAf->duration   =   -1;
    pAf->bitvector  =   0;
    pAf->level      =   weapon->level;
    pAf->next       =   weapon->affected;
    weapon->affected  =   pAf;
  obj_to_char(weapon, ch);

send_to_char("You summon the shadows.\n\r",ch);
act("The shadows suddenly transforms into $p!",ch,weapon,NULL,TO_CHAR);
act("$n is endowed with $p!",ch,weapon,NULL,TO_ROOM);
act("$n is endowed with $p!",ch,weapon,NULL,TO_ROOM);
}  


#if 0
void spell_oldshadowlife( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  CHAR_DATA *shadow;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];
  char *name;

  if (IS_NPC(victim))
    {
      send_to_char("Now, why would you want to do that?!?\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char("You don't have the strength to raise a Shadow now.\n\r",
		   ch);
      return;
    }

  act("You give life to $N's shadow!",ch, NULL, victim, TO_CHAR);
  act("$n gives life to $N's shadow!",ch,NULL,victim,TO_NOTVICT);
  act("$n gives life to your shadow!", ch, NULL, victim, TO_VICT);
  
  shadow = create_mobile( get_mob_index(MOB_VNUM_SHADOW) );
  
  for (i=0;i < MAX_STATS; i++)
    {
      shadow->perm_stat[i] = ch->perm_stat[i];
    }
  
  shadow->max_hit = (3 * ch->max_hit) / 4;
  shadow->hit = shadow->max_hit;
  shadow->max_mana = (3 * ch->max_mana) / 4;
  shadow->mana = shadow->max_mana;
  shadow->alignment = ch->alignment;
  shadow->level = ch->level;
  for (i=0; i < 3; i++)
  shadow->armor[i] = interpolate(shadow->level,100,-100);
  shadow->armor[3] = interpolate(shadow->level,100,0);
  shadow->sex = victim->sex;
  shadow->gold = 0;

  name		= IS_NPC(victim) ? victim->short_descr : victim->name;
  xprintf( buf, shadow->short_descr, name );
  free_string( shadow->short_descr );
  shadow->short_descr = str_dup( buf );

  xprintf( buf, shadow->long_descr, name );
  free_string( shadow->long_descr );
  shadow->long_descr = str_dup( buf );
  
  xprintf( buf, shadow->description, name );
  free_string( shadow->description );
  shadow->description = str_dup( buf );
  
  char_to_room(shadow,ch->in_room);

  do_murder(shadow, victim->name);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}  

void spell_shade( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  OBJ_DATA *obj = (OBJ_DATA *) vo;
  CHAR_DATA *gch;
  CHAR_DATA *shade;
  AFFECT_DATA af;
  char buff[100];
  int i;
  char buf[MAX_STRING_LENGTH];
  char *name;
  int count;
  OBJ_DATA *tmp_obj;
  OBJ_DATA *tmp_next;

  if (obj->pIndexData->vnum != OBJ_VNUM_CORPSE_PC)
    {
      send_to_char("You can't raise a shade from that!\n\r", ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char("You feel too weak to summon a Shade now.\n\r",
		   ch);
      return;
    }

  act("You attempt to raise a shade from $p.",ch, obj, NULL, TO_CHAR);
  act("$n attempts to raise a shade from $p.",ch,obj,NULL,TO_ROOM);

  count = 0;
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch)
	{
	  count++;
	  if (count >= ch->level/15 && ch->class != class_lookup("mage"))
	    {
	      send_to_char(
       	   "You are already controlling as many charmed mobs as you can!\n\r"
			   ,ch);
	      return;
	    }
	  else if (count >= ch->level/10)
	    {
	      send_to_char(
	   "You are already controlling as many charmed mobs as you can!\n\r"
			,ch);
	      return;
	    }
	}
    }
  shade = create_mobile( get_mob_index(MOB_VNUM_SHADE) );

  for (i=0;i < MAX_STATS; i++)
    {
      shade->perm_stat[i] = ch->perm_stat[i];
    }

  shade->max_hit = (3 * ch->max_hit) / 4;
  shade->hit = shade->max_hit;
  shade->max_mana = (3 * ch->max_mana) / 4;
  shade->mana = shade->max_mana;
  shade->level = (3 * ch->level) / 4;
  for (i=0; i < 3; i++)
    shade->armor[i] = interpolate(shade->level,100,-100);
  shade->armor[3] = interpolate(shade->level,100,0);
  shade->gold = 0;

  name		= obj->from;

  xprintf( buf, shade->short_descr, name );
  free_string( shade->short_descr );
  shade->short_descr = str_dup( buf );

  xprintf( buf, shade->long_descr, name );
  free_string( shade->long_descr );
  shade->long_descr = str_dup( buf );
  
  xprintf( buf, shade->description, name );
  free_string( shade->description );
  shade->description = str_dup( buf );
    
  char_to_room(shade,ch->in_room);
  act("A Shade rises up from $p!",ch, obj, NULL, TO_CHAR);
  act("A Shade rises up from $p!",ch,obj,NULL,TO_ROOM);

  obj_from_room(obj);
  
  /* empty the corpse into the room */
  for (tmp_obj = obj->contains;tmp_obj != NULL;
       tmp_obj = tmp_next)
    {
      tmp_next = tmp_obj->next_content;
      obj_from_obj(tmp_obj);
      obj_to_room(tmp_obj,ch->in_room);
    }
  xprintf(buff, "kneel %s", ch->name);
  interpret(shade, buff);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(shade->affected_by, AFF_CHARM);
  shade->master = shade->leader = ch;
  extract_obj(obj);
}  
#endif

void spell_justice_badge( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *badge;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  OBJ_DATA *obj_next;

  if ( (get_eq_char(ch, WEAR_ORNAMENT_1)  != NULL ) &&
	(get_eq_char(ch, WEAR_ORNAMENT_1)  != NULL ) )
  {
    send_to_char( "But you are wearing something else.\n\r", ch );
    return;
  }

  for ( badge = ch->carrying; badge != NULL;
       badge = obj_next )
    {
      obj_next = badge->next_content;
      if (badge->pIndexData->vnum == OBJ_VNUM_DEPUTY_BADGE 
	  || badge->pIndexData->vnum == OBJ_VNUM_JUSTICE_BADGE)
	{
	  act("Your $p vanishes.",ch, badge, NULL, TO_CHAR);
	  obj_from_char(badge);
	  extract_obj(badge);
	  continue;
	}
    }


  badge = create_object( get_obj_index(OBJ_VNUM_JUSTICE_BADGE),level);
  badge->timer = 200;
  act("You wear the justice badge!",ch, NULL, NULL, TO_CHAR);
/*  act("$n gives you a deputy badge!",ch,NULL,victim,TO_VICT);  */
  act("$n wears the $s justice badge!", ch, NULL, NULL, TO_ROOM);

  obj_to_char(badge,victim);
  if ( get_eq_char(ch, WEAR_ORNAMENT_1)  == NULL )
    equip_char( ch, badge, WEAR_ORNAMENT_1 );
  else if ( get_eq_char(ch, WEAR_ORNAMENT_2)  == NULL )
    equip_char( ch, badge, WEAR_ORNAMENT_2 ); 
  else
  {
    send_to_char( "But you are wearing something else.\n\r", ch );
    return;
  }

}    

void spell_remove_badge( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  OBJ_DATA *badge;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  OBJ_DATA *obj_next;

  badge = 0;

  for ( badge = victim->carrying; badge != NULL;
       badge = obj_next )
    {
      obj_next = badge->next_content;
      if (badge->pIndexData->vnum == OBJ_VNUM_DEPUTY_BADGE 
	  || badge->pIndexData->vnum == OBJ_VNUM_JUSTICE_BADGE)
	{
	  act("Your $p vanishes.",ch, badge, NULL, TO_CHAR);
/*	  act("$n takes $p from you!",ch,badge,victim,TO_VICT); */
	  act("$n's $p vanishes.", ch, badge, NULL, TO_ROOM);
	  
	  obj_from_char(badge);
	  extract_obj(badge);
	  continue;
	}
    }
  return;
}

void spell_dragon_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  AFFECT_DATA af;

  if (is_affected(ch, sn))
    {
      send_to_char("You are already full of the strength of the dragon.\n\r",
		ch);
      return;
    }

  af.where		= TO_AFFECTS;
  af.type = sn;
  af.level = level;
  af.duration = level / 3;
  af.bitvector = 0;

  af.modifier = 2;
  af.location = APPLY_HITROLL;
  affect_to_char(ch, &af);

  af.modifier = 2;
  af.location = APPLY_DAMROLL;
  affect_to_char(ch, &af);

  af.modifier = 10;
  af.location = APPLY_AC;
  affect_to_char(ch, &af);

  af.modifier = 2;
  af.location = APPLY_STR;
  affect_to_char(ch, &af);

  af.modifier = -2;
  af.location = APPLY_DEX;
  affect_to_char(ch, &af);

  send_to_char("The strength of the dragon enters you.\n\r", ch);
  act("$n looks a bit meaner now.", ch, NULL, NULL, TO_ROOM);
}

void spell_dragon_breath(int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;

  static const sh_int dam_each[] =
    {
      0,
      2,  5,  9,  11, 14,  17, 20, 23, 26, 29,
      29, 30, 30, 31, 32,  32, 33, 34, 34, 35,
      35, 36, 37, 38, 39,  40, 40, 41, 41, 42,
      43, 43, 44, 45, 45,  46, 47, 48, 48, 49,
      50, 51, 52, 53, 54,  55, 56, 57, 58, 60

    };
  int dam;

  level = UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
  level = UMAX(0, level);
  dam = number_range(dam_each[level] / 3, dam_each[level] * 1 );

  if (!is_safe_spell(ch, victim, TRUE))
    {
      if (saves_spell(level, victim, DAM_FIRE))
	dam /= 2;
      damage_spell(ch, victim, dam, sn, DAM_FIRE, TRUE);
    }
}

void spell_golden_aura( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;
  CHAR_DATA *vch = vo;

  for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
      if (!is_same_group(vch, ch)) 
	continue;

      if ( is_affected( vch, sn ))

	/*|| is_affected(vch, gsn_bless) || 
	  IS_AFFECTED(vch, AFF_PROTECT_EVIL))*/
	{
	  if (vch == ch)
	    send_to_char("You are already protected by a golden aura.\n\r",ch);
	  else
	    act("$N is already protected by a golden aura.",ch,NULL,vch,TO_CHAR);
	  continue;
	}
	
      af.where		= TO_AFFECTS;
      af.type      = sn;
      af.level	 = level;
      af.duration  = 6 + level;
      af.modifier  = 0;
      af.location  = APPLY_NONE;
      af.bitvector = AFF_PROTECT_EVIL;
      affect_to_char( vch, &af );
      
      af.modifier = level/8;
      af.location = APPLY_HITROLL;
      af.bitvector = 0;
      affect_to_char(vch, &af);
      
      af.modifier = 0 - level/8;
      af.location = APPLY_SAVING_SPELL;
      affect_to_char(vch, &af);

  send_to_char( "The essence of the ashen dragon bathes you in a golden aura.\n\r", vch );
      if ( ch != vch )
 	act("A golden aura surrounds $N.",ch,NULL,vch,TO_CHAR);

    }
}

void spell_dragonplate( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int plate_vnum;
  OBJ_DATA *plate;
  
if (ch->in_room->sector_type == SECT_CITY)
	{
	send_to_char("The Ashen Dragon would never come to the city.\n\r",ch);
	return;
	}

if (ch->in_room->sector_type == SECT_INSIDE)
	{
	send_to_char("The Ashen Dragon would never come indoors.\n\r",ch);
	return;
	}

  plate_vnum = OBJ_VNUM_PLATE;
  plate = create_object( get_obj_index(plate_vnum), level + 5);
  plate->timer = 2 * level;
  plate->cost  = 0;
  obj_to_char(plate, ch);
 

send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
act("You are endowed by an ashen dragon with $p!",ch,plate,NULL,TO_CHAR);
act("$n kneels in servitude as an ashen dragon appears in the sky.",ch,plate,NULL,TO_ROOM);
act("$n is endowed with $p by an ashen dragon!",ch,plate,NULL,TO_ROOM);
}

void spell_squire( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *squire;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another squire right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a squire.\n\r",ch);
  act("$n attempts to summon a squire.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SQUIRE)
	{
	  send_to_char("Two squires are more than you need!\n\r",ch);
	  return;
	}
    }

  squire = create_mobile( get_mob_index(MOB_VNUM_SQUIRE) );

  for (i=0;i < MAX_STATS; i++)
    {
      squire->perm_stat[i] = ch->perm_stat[i];
    }

  squire->max_hit = ch->max_hit;
  squire->hit = squire->max_hit;
  squire->max_mana = ch->max_mana;
  squire->mana = squire->max_mana;
  squire->level = ch->level;
  squire->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
    squire->armor[i] = interpolate(squire->level,100,-100);
  squire->armor[3] = interpolate(squire->level,100,0);
  squire->gold = 0;

  xprintf( buf, squire->short_descr, ch->name );
  free_string( squire->short_descr );
  squire->short_descr = str_dup( buf );

  xprintf( buf, squire->long_descr, ch->name );
  free_string( squire->long_descr );
  squire->long_descr = str_dup( buf );
  
  xprintf( buf, squire->description, ch->name );
  free_string( squire->description );
  squire->description = str_dup( buf ); 
  
  char_to_room(squire,ch->in_room);
  send_to_char("A squire arrives from nowhere!\n\r",ch);
  act("A squire arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(squire->affected_by, AFF_CHARM);
  squire->master = squire->leader = ch;
  
}
void spell_earth_elemental( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *eelement;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another earth elemental right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a earth elemental.\n\r",ch);
  act("$n attempts to summon a earth elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_EARTH_ELEMENTAL)
	{
	  send_to_char("Two earth elemental are more than you need!\n\r",ch);
	  return;
	}
    }
  eelement = create_mobile( get_mob_index(MOB_VNUM_EARTH_ELEMENTAL) );

  for (i=0;i < MAX_STATS; i++)
    {
      eelement->perm_stat[i] = ch->perm_stat[i];
    }

  eelement->max_hit = ch->max_hit*3;
  eelement->hit = eelement->max_hit;
  eelement->max_mana = ch->max_mana*3;
  eelement->mana = eelement->max_mana;
  eelement->hitroll                = level*3.5;
  eelement->damroll                = level*3.5;
  eelement->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  eelement->level = ch->level;
  for (i=0; i < 3; i++)
    eelement->armor[i] = interpolate(eelement->level*2,100,-100);
  eelement->armor[3] = interpolate(eelement->level*2,100,0);
  eelement->gold = 0;

  xprintf( buf, eelement->short_descr, ch->name );
  free_string( eelement->short_descr );
  eelement->short_descr = str_dup( buf );

  xprintf( buf, eelement->long_descr, ch->name );
  free_string( eelement->long_descr );
  eelement->long_descr = str_dup( buf );
  
  xprintf( buf, eelement->description, ch->name );
  free_string( eelement->description );
  eelement->description = str_dup( buf ); 
  
  char_to_room(eelement,ch->in_room);
  send_to_char("An earth elemental arrives from nowhere!\n\r",ch);
  act("An earth elemental arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(eelement->affected_by, AFF_CHARM);
  eelement->master = eelement->leader = ch;
}
void spell_fire_elemental( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *eelement;
  AFFECT_DATA af;
  AFFECT_DATA maf;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another fire elemental right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a fire elemental.\n\r",ch);
  act("$n attempts to summon a fire elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL)
	{
	  send_to_char("Two fire elemental are more than you need!\n\r",ch);
	  return;
	}
    }
  eelement = create_mobile( get_mob_index(MOB_VNUM_FIRE_ELEMENTAL) );

  for (i=0;i < MAX_STATS; i++)
    {
      eelement->perm_stat[i] = ch->perm_stat[i];
    }

  eelement->max_hit = ch->max_hit*2.5;
  eelement->hit = eelement->max_hit;
  eelement->max_mana = ch->max_mana*2.5;
  eelement->mana = eelement->max_mana;
  eelement->hitroll                = level*3.2;
  eelement->damroll                = level*3.2;
  eelement->level = ch->level;
  eelement->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
    eelement->armor[i] = interpolate(eelement->level*1.1,100,-100);
  eelement->armor[3] = interpolate(eelement->level*1.1,100,0);
  eelement->gold = 0;

  xprintf( buf, eelement->short_descr, ch->name );
  free_string( eelement->short_descr );
  eelement->short_descr = str_dup( buf );

  xprintf( buf, eelement->long_descr, ch->name );
  free_string( eelement->long_descr );
  eelement->long_descr = str_dup( buf );
  
  xprintf( buf, eelement->description, ch->name );
  free_string( eelement->description );
  eelement->description = str_dup( buf ); 
  
  char_to_room(eelement,ch->in_room);
  send_to_char("An fire elemental arrives from nowhere!\n\r",ch);
  act("An fire elemental arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  maf.where     = TO_AFFECTS;
  maf.type      = gsn_fire_shield;
  maf.level     = level;
  maf.duration  = -1;
  maf.location  = APPLY_NONE;
  maf.modifier  = 0;
  maf.bitvector = 0;
    affect_to_char( eelement, &maf );

  SET_BIT(eelement->affected_by, AFF_CHARM);
  eelement->master = eelement->leader = ch;
}
void spell_air_elemental( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *eelement;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another air elemental right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a air elemental.\n\r",ch);
  act("$n attempts to summon a air elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_AIR_ELEMENTAL)
	{
	  send_to_char("Two air elemental are more than you need!\n\r",ch);
	  return;
	}
    }
  eelement = create_mobile( get_mob_index(MOB_VNUM_AIR_ELEMENTAL) );

  for (i=0;i < MAX_STATS; i++)
    {
      eelement->perm_stat[i] = ch->perm_stat[i];
    }

  eelement->max_hit = ch->max_hit*2;
  eelement->hit = eelement->max_hit*2;
  eelement->max_mana = ch->max_mana*2;
  eelement->mana = eelement->max_mana*2;
  eelement->hitroll                = 30+level/4;
  eelement->damroll                = 30+level/4;
  eelement->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  eelement->level = ch->level;
  for (i=0; i < 3; i++)
    eelement->armor[i] = interpolate(eelement->level+15,100,-100);
  eelement->armor[3] = interpolate(eelement->level+15,100,0);
  eelement->gold = 0;

  xprintf( buf, eelement->short_descr, ch->name );
  free_string( eelement->short_descr );
  eelement->short_descr = str_dup( buf );

  xprintf( buf, eelement->long_descr, ch->name );
  free_string( eelement->long_descr );
  eelement->long_descr = str_dup( buf );
  
  xprintf( buf, eelement->description, ch->name );
  free_string( eelement->description );
  eelement->description = str_dup( buf ); 
  
  char_to_room(eelement,ch->in_room);
  send_to_char("An air elemental arrives from nowhere!\n\r",ch);
  act("An air elemental arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(eelement->affected_by, AFF_CHARM);
  eelement->master = eelement->leader = ch;
}
void spell_water_elemental( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *eelement;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another water elemental right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a water elemental.\n\r",ch);
  act("$n attempts to summon a water elemental.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_WATER_ELEMENTAL)
	{
	  send_to_char("Two water elemental are more than you need!\n\r",ch);
	  return;
	}
    }
  eelement = create_mobile( get_mob_index(MOB_VNUM_WATER_ELEMENTAL) );

  for (i=0;i < MAX_STATS; i++)
    {
      eelement->perm_stat[i] = ch->perm_stat[i];
    }

  eelement->max_hit = ch->max_hit;
  eelement->hit = eelement->max_hit;
  eelement->max_mana = ch->max_mana;
  eelement->mana = eelement->max_mana;
  eelement->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  eelement->hitroll                = level;
  eelement->damroll                = level;
  eelement->level = ch->level;
  for (i=0; i < 3; i++)
    eelement->armor[i] = interpolate(eelement->level/2,100,-100);
  eelement->armor[3] = interpolate(eelement->level/2,100,0);
  eelement->gold = 0;

  xprintf( buf, eelement->short_descr, ch->name );
  free_string( eelement->short_descr );
  eelement->short_descr = str_dup( buf );

  xprintf( buf, eelement->long_descr, ch->name );
  free_string( eelement->long_descr );
  eelement->long_descr = str_dup( buf );
  
  xprintf( buf, eelement->description, ch->name );
  free_string( eelement->description );
  eelement->description = str_dup( buf ); 
  
  char_to_room(eelement,ch->in_room);
  send_to_char("An water elemental arrives from nowhere!\n\r",ch);
  act("An water elemental arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(eelement->affected_by, AFF_CHARM);
  eelement->master = eelement->leader = ch;
}

void spell_dragonsword( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int sword_vnum;
  OBJ_DATA *sword;
  char arg[MAX_INPUT_LENGTH];

if (ch->in_room->sector_type == SECT_CITY)
	{
	send_to_char("The Ashen Dragon would never come so close to the city.\n\r",ch);
	return;
	}

if (ch->in_room->sector_type == SECT_INSIDE)
	{
	send_to_char("The Ashen Dragon would never come here.\n\r",ch);
	return;
	}
  
  target_name = one_argument(target_name, arg);
  sword_vnum = 0;

  if (!str_cmp(arg, "sword"))
    sword_vnum = OBJ_VNUM_DRAGONSWORD;
  else if (!str_cmp(arg, "mace"))
    sword_vnum = OBJ_VNUM_DRAGONMACE;
  else if (!str_cmp(arg, "dagger"))
    sword_vnum = OBJ_VNUM_DRAGONDAGGER;
  else
    {
      send_to_char("You must specify a type, sword, mace or dagger.\n\r",
ch);
      return;
    }

  sword = create_object( get_obj_index(sword_vnum), level);
  sword->timer = level;
  sword->cost  = 0;
  obj_to_char(sword, ch);

send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
act("You are endowed by an ashen dragon with $p!",ch,sword,NULL,TO_CHAR);
act("$n kneels in servitude as an ashen dragon appears in the sky.",ch,sword,NULL,TO_ROOM);
act("$n is endowed with $p by an ashen dragon!",ch,sword,NULL,TO_ROOM);
}

void spell_entangle( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;
  
  if (ch->in_room->sector_type == SECT_INSIDE || 
      ch->in_room->sector_type == SECT_CITY || 
      ch->in_room->sector_type == SECT_DESERT || 
      ch->in_room->sector_type == SECT_AIR)
    {
      send_to_char("No plants can grow here.\n\r", ch);
      return;
    }
      
  dam = dice(level, 5);
  
//  dam = number_range(level, 4 * level);
  if ( saves_spell( level+5, victim, DAM_PIERCE ) )
    dam /= 2;
  
  damage_spell(ch,victim,ch->level,gsn_entangle,DAM_PIERCE, TRUE);
  
  act("The thorny plants spring up around $n, entangling $s legs!", victim, 
      NULL, NULL, TO_ROOM);
  act("The thorny plants spring up around you, entangling your legs!", victim,
      NULL, NULL, TO_CHAR);
  victim->move -= dam *1.5;

  if (!is_affected(victim,gsn_entangle))
    {
      AFFECT_DATA todex;
      
      todex.type = gsn_entangle;
      todex.level = level;
      todex.duration = level / 10;
      todex.location = APPLY_DEX;
      todex.modifier = -1;
      todex.bitvector = 0;
      affect_to_char( victim, &todex);
      
    }
  return;
}

void spell_love_potion( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  AFFECT_DATA af;

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 50;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  send_to_char("You feel like looking at people.\n\r", ch);
}

void spell_protective_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target ) {
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
   
  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
       	send_to_char("You are already surrounded by a protective shield.\n\r",
		     ch); 
      else
       	act("$N is already surrounded by a protective shield.",ch,NULL,
	    victim,TO_CHAR);
      return;
    }
  af.where	= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = number_fuzzy( level / 10 );
  af.location  = APPLY_AC;
  af.modifier  = -20;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  act( "$n is surrounded by a protective shield.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "You are surrounded by a protective shield.\n\r", victim );
  return;
}
	
void spell_deafen( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (ch == victim) {
    send_to_char("Deafen who?\n\r",ch);
    return;
  }

  if (is_affected(victim,sn)) {
    act("$N is already deaf.",ch,NULL,victim,TO_CHAR);
    return;
  }

  if (is_safe_nomessage(ch,victim)) {
      send_to_char("You cannot deafen that person.\n\r",ch);
      return;
  }

 if (ch->class == 13)
  {
   if (saves_spell(level+5,victim, DAM_NONE))
     return;
   }
  else   
  {
   if (saves_spell(level,victim, DAM_NONE))
     return;
   }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 10;
  af.modifier  = 0;
  af.location  = 0;
  af.bitvector = 0;
  affect_to_char(victim,&af);

  act("You have deafened $N!",ch,NULL,victim,TO_CHAR);
  send_to_char("A loud ringing fills your ears...you can't hear anything!\n\r",
	       victim);
}
/*
void spell_protection_heat_cold( int sn, int level, CHAR_DATA *ch, void *vo, int target ) {
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
   
  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
       	send_to_char("You are already protected from the elements.\n\r",
		     ch); 
      else
       	act("$N is already protected from the elements.",ch,NULL,
	    victim,TO_CHAR);
      return;
    }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 24;
  af.location  = APPLY_NONE;
  af.modifier  = 0;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  act( "$n is protected from the elements.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "You are protected from the elements.\n\r", victim );
  return;
}
*/	
void spell_disperse( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
  ROOM_INDEX_DATA *pRoomIndex;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
    {
      send_to_char("You aren't up to dispersing this crowd.\n\r",ch);
      return;
    }

  for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
      vch_next = vch->next_in_room;


      if ( (vch->in_room != NULL)
      || !can_see_room(ch,vch->in_room) 
      || room_is_private(vch->in_room)
      || IS_SET(vch->in_room->room_flags, ROOM_NOSUMMON)
      || IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON)
      || (!IS_NPC(vch) && vch->level >= LEVEL_HERO)  /* NOT trust */ 
      || (IS_NPC(vch) && is_safe_nomessage(ch, vch)
      && IS_SET(vch->imm_flags,IMM_SUMMON))
      || (!IS_NPC(vch) && IS_SET(vch->act,PLR_NOSUMMON)
      && is_safe_nomessage(ch,vch))
      || (saves_spell( level, vch, DAM_OTHER ) ) ) 

/*
      && ( !IS_SET(vch->in_room->room_flags, ROOM_NO_RECALL)
      && (IS_NPC(vch) && !IS_SET(vch->act, ACT_AGGRESSIVE)) 
      || ((!IS_NPC(vch) && (vch->exp > PK_MIN)) && (vch->level < level))  
      || (((((!is_safe_nomessage(ch, vch)) && (vch != ch))) 
      && !IS_SET(vch->imm_flags, IMM_SUMMON)))
      || (IS_NPC(vch) && !IS_SET(vch->imm_flags, IMM_SUMMON)) 
      || (!IS_NPC(vch) && (level < vch->level+5) &&(vch != ch))
      || !strstr( ch->in_room->area->builders, "Unlinked" ) 
      || ( !is_safe_nomessage(ch,vch) && vch != ch))) */
	{
      if(IS_NPC(vch) )
	  if((vch->pIndexData->vnum == 110) ||
	    (vch->pIndexData->vnum == 120) ||
	    (vch->pIndexData->vnum == 130) ||
	    (vch->pIndexData->vnum == 140) ||
	    (vch->pIndexData->vnum == 150) ||
	    (vch->pIndexData->vnum == 160)) 
		return;
	  for ( ; ; )
  	    {
	      pRoomIndex = get_room_index( number_range( 0, 65535 ) );
	      if ( pRoomIndex != NULL )
	        if ( can_see_room(ch,pRoomIndex)
	    	&&   !room_is_private(pRoomIndex) && 
		  !IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL))
	      break;
	    }

	  send_to_char("The world spins around you!\n\r",vch);
	  act( "$n vanishes!", vch, NULL, NULL, TO_ROOM );
	  char_from_room( vch );
	  char_to_room( vch, pRoomIndex );
	  act( "$n slowly fades into existence.", vch, NULL, NULL, TO_ROOM );
	  do_look( vch, "auto" );
	}
    }
  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level	 = level;
  af.duration  = 15;
  af.modifier  = 0;
  af.location  = APPLY_NONE;
  af.bitvector = 0;
  affect_to_char( ch, &af );

}


void spell_honor_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if (is_affected(victim, sn))
    {
      if (victim == ch)
        send_to_char("But you're already protected by your honor.\n\r", ch);
      else
	send_to_char("They're already protected by their honor.\n\r", ch);
      return;
    }

  af.where		= TO_AFFECTS;
  af.type      = sn;
  af.level	 = level;
  af.duration  = 24;
  af.modifier  = -30;
  af.location  = APPLY_AC;
  af.bitvector = 0;
  affect_to_char( victim, &af );

  spell_remove_curse(skill_lookup("remove curse"), level, ch, victim, TARGET_CHAR);
  spell_bless(skill_lookup("bless"), level, ch, victim, TARGET_CHAR);

  send_to_char("Your honor protects you.\n\r", victim);
  act("$n's Honor protects $m.", victim, NULL, NULL, TO_ROOM);
  return;
}

void spell_dimension( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
            
   if ( is_affected(victim, gsn_duo_dimension))
    {
      affect_strip(victim,gsn_duo_dimension);
      send_to_char("You turn back into three dimension.\n\r", victim);
//      act("$n turns back into three dimension.", ch, NULL, NULL, TO_ROOM);
    }    
    
    return;
}


void spell_duo_dimension( int sn, int level, CHAR_DATA *ch, void *vo, int target ) {
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;
   
  if ( is_affected( victim, sn ) )
    {
      if (victim == ch)
       	send_to_char("You already have two dimensions.\n\r",ch); 
      else
       	act("$N is already in another dimension.",ch,NULL,victim,TO_CHAR);
      return;
    }

  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = 3 + level/35;
  af.location  = APPLY_NONE;
  af.modifier  = 0;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  act( "$n has lost one dimension, and gone out of sight.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "You loose one dimension and go out of sight.\n\r", victim );
  return;
}

void spell_animate_dead( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj;
    CHAR_DATA *gch;
    CHAR_DATA *mob;
    AFFECT_DATA af;
    int i;

    obj = get_obj_here( ch, NULL,target_name );
    
    if (ch->alignment == 750)
	{
        send_to_char( "That evil act can darken your pure soul.\n\r", ch );
        return;
	}

    if (ch->alignment == 0)
	{
        send_to_char( "You are not evil enough to cast that spell.\n\r", ch );
        return;
	}


  if (is_affected(ch, sn))
    {
      send_to_char("You cannot create zombies right now.\n\r", ch);
      return;
    }

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_ZOMBIE)
	{
	  send_to_char("What's wrong with the zombie you've got?",ch);
	  return;
	}
    }


    if ( obj == NULL )
    {
        send_to_char( "Animate what?\n\r", ch );
        return;
    }

    /* Nothing but NPC corpses. */

    if( obj->item_type != ITEM_CORPSE_NPC)
    {
            send_to_char( "It would serve no purpose...\n\r", ch );
        return;
    }

    if( obj->level > (ch->level + 5) )
    {
        send_to_char( "You couldn't call forth such a great spirit.\n\r", ch );
        return;
    }

    if( ch->pet != NULL )
    {
        send_to_char( "You already have a pet.\n\r", ch );
        return;
    }

    /* Chew on the zombie a little bit, recalculate level-dependant stats */

    mob = create_mobile( get_mob_index( MOB_VNUM_ZOMBIE ) );
    mob->level                  = obj->level;
    mob->max_hit                = mob->level * 8 + number_range(
                                        mob->level * mob->level/4,
                                        mob->level * mob->level);

    mob->max_hit *= .9;
    mob->hitroll		= (level * 2 )/5;
    mob->damroll		= level/3;
    mob->sex			= 0;
    mob->hit                    = mob->max_hit;
    mob->max_mana               = 100 + dice(mob->level,10);
    mob->mana                   = mob->max_mana;
    for (i = 0; i < 3; i++)
        mob->armor[i]           = interpolate(mob->level,100,-100);
    mob->armor[3]               = interpolate(mob->level,100,0);

    for (i = 0; i < MAX_STATS; i++)
        mob->perm_stat[i] = 11 + mob->level/4;

    /* You rang? */
    char_to_room( mob, ch->in_room );
    act( "$p springs to life as a hideous zombie!", ch, obj, NULL, TO_ROOM );
    act( "$p springs to life as a hideous zombie!", ch, obj, NULL, TO_CHAR );

    extract_obj(obj);

    /* Yessssss, massssssster... */
    SET_BIT(mob->affected_by, AFF_CHARM);
    SET_BIT(mob->act, ACT_PET);
    mob->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
    add_follower( mob, ch );
    mob->leader = ch;
    ch->pet = mob;
    /* For a little flavor... */
    do_say( mob, "How may I serve you, master?" );

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

    return;

}
void spell_shadow_walk( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    ROOM_INDEX_DATA *location;

    act("$n breaks the ties to the material plane and step into the shadow world",ch,NULL,NULL,TO_ROOM);
    send_to_char("You overcome your ties to the material plane and step into the shadow world.\n\r",ch);


    char_from_room(ch);
    location = get_room_index(ROOM_VNUM_SHADOWPLANE);
    if (ch->in_room == location) /* Baxter */
	return;
    char_from_room(ch);
    char_to_room(ch, location);
    act("$n has came to the shadow world.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");
    return;	
}

/* Room Spells by Takeda NOT ADDED YET */
void spell_sanctify_lands(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  if (number_bits(1) == 0)
    {
      send_to_char("Despite your efforts, the universe chose the chaos.\n\r",ch);
      return;
    }

  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_CURSE))
        {
         affect_strip_room(ch->in_room,gsn_cursed_lands);
         send_to_char("The curse of the land wears off.\n\r",ch);
         act("The curse of the land wears off.\n\r",ch,NULL,NULL,TO_ROOM);
        }
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_POISON))
        {
         affect_strip_room(ch->in_room,gsn_deadly_venom);
         send_to_char("The land seems more healthy.\n\r",ch);

       act("The land seems more healthy.\n\r",ch,NULL,NULL,TO_ROOM);
        }
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLEEP))
        {
         send_to_char("The land wake up from mysterious dream.\n\r",ch);
         act("The land wake up from mysterious dream.\n\r",ch,NULL,NULL,TO_ROOM);
         affect_strip_room(ch->in_room,gsn_mysterious_dream);
        }
//  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_FEAR))
//        {
//         send_to_char("The totem is purified and look less scary\n\r",ch);
//         act("The totem has been purified and seems less scary\n\r",ch,NULL,NULL,TO_ROOM);
 //        affect_strip_room(ch->in_room,gsn_totem);
 //       }
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_PLAGUE))
        {
         send_to_char("The disease of the land has been treated.\n\r",ch);
         act("The disease of the land has been treated.\n\r",ch,NULL,NULL,TO_ROOM);
         affect_strip_room(ch->in_room,gsn_black_death);
        }
  if (IS_RAFFECTED(ch->in_room,AFF_ROOM_SLOW))
        {
         send_to_char("The lethargic mist dissolves.\n\r",ch);
    act("The lethargic mist dissolves.\n\r",ch,NULL,NULL,TO_ROOM);
         affect_strip_room(ch->in_room,gsn_lethargic_mist);
        }
    return;
}


void spell_deadly_venom(int sn, int level, CHAR_DATA *ch, void *vo, int target) 
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
  {
     send_to_char("This room has already been effected by deadly venom.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_POISON;
    affect_to_room( ch->in_room, &af );

    send_to_char("The room starts to be filled by poison.\n\r",ch);
    act("The room starts to be filled by poison.\n\r",ch,NULL,NULL,TO_ROOM);


}

void spell_cursed_lands(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been cursed.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_CURSE;
    affect_to_room( ch->in_room, &af );

    send_to_char("The gods has forsaken the room.\n\r",ch);
    act("The gods has forsaken the room.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_lethargic_mist(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
   AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been full of lethargic mist.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SLOW;
    affect_to_room( ch->in_room, &af );

    send_to_char("The air in the room makes you slowing down.\n\r",ch);
    act("The air in the room makes you slowing down.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_black_death(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",ch);
      return;
  }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been diseased.\n\r",ch);
     return;
    }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_PLAGUE;
    affect_to_room( ch->in_room, &af );

    send_to_char("The room starts to be filled by disease.\n\r",ch);
  act("The room starts to be filled by disease.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_mysterious_dream(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  AFFECT_DATA af;

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("This room has already been affected by sleep gas.\n\r",ch);
     return;
  }

    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SLEEP;
    affect_to_room( ch->in_room, &af );

    send_to_char("The room starts to be seen good place to sleep.\n\r",ch);
    act("The room starts to be seen as good place to you.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_insanity ( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_NPC(ch))
        {
         send_to_char("This spell can cast on PC's only.\n\r",ch);
         return;
        }

    if (IS_AFFECTED(victim,AFF_BLOODTHIRST ) || saves_spell( level,victim,DAM_OTHER) )
        return;

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 10;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_BLOODTHIRST;
    affect_to_char( victim, &af );
    send_to_char( "You are aggressive as a battlerager.\n\r", victim );
    act("$n looks with red eyes.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_acid_arrow( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 10 );
    if ( saves_spell( level, victim, DAM_ACID ) )
        dam /= 2;
    damage_spell( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}


/* energy spells */
void spell_etheral_fist( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 10 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
        dam /= 2;
    act("A fist of black, otherworldly ether rams into $N, leaving $M looking stunned!"
                ,ch,NULL,victim,TO_NOTVICT);
  damage_spell( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}

void spell_spectral_furor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
        dam /= 2;
    act("The fabric of the cosmos strains in fury about $N!",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}

void spell_disruption( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
        dam /= 2;
    act("A weird energy encompasses $N, causing you to question $S continued existence.",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    return;
}

void spell_sonic_resonance( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    if ( saves_spell( level, victim, DAM_ENERGY ) )
        dam /= 2;
    act("A cylinder of kinetic energy enshrouds $N causing $S to resonate.",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_ENERGY,TRUE);
    WAIT_STATE( victim, skill_table[sn].beats );
    return;
}
/* mental */
void spell_mind_wrack( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
  int dam;

    dam = dice( level, 14 );
    if ( saves_spell( level, victim, DAM_MENTAL ) )
        dam /= 2;
    act("$n stares intently at $N, causing $N to seem very lethargic.",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_MENTAL,TRUE);
    return;
}

/* acid */
void spell_sulfurus_spray( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    if ( saves_spell( level, victim, DAM_ACID ) )
        dam /= 2;
    act("A stinking spray of sulfurous liquid rains down on $N." ,
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

void spell_caustic_font( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    if ( saves_spell( level, victim, DAM_ACID ) )
        dam /= 2;
    act("A fountain of caustic liquid forms below $N.  The smell of $S degenerating tissues is revolting! ",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

void spell_acetum_primus( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    if ( saves_spell( level, victim, DAM_ACID ) )
        dam /= 2;
    act("A cloak of primal acid enshrouds $N, sparks form as it consumes all it touches. ",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

/*  Electrical  */

void spell_galvanic_whip( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 7 );
    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
        dam /= 2;
    act("$n conjures a whip of ionized particles, which lashes ferociously at $N.",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}

void spell_magnetic_trust( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 8 );
    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
        dam /= 2;
    act("An unseen energy moves nearby, causing your hair to stand on end!",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}

void spell_quantum_spike( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 9 );
    if ( saves_spell( level, victim, DAM_LIGHTNING ) )
        dam /= 2;
    act("$N seems to dissolve into tiny unconnected particles, then is painfully reassembled.",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_LIGHTNING,TRUE);
    return;
}

/* negative */
void spell_hand_of_undead( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;


    if ( saves_spell( level+4, victim,DAM_NEGATIVE) )
    {
        send_to_char("You feel a momentary chill.\n\r",victim);
        return;
    }

    if ( (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
/*        || IS_VAMPIRE(victim) ) */
        {
         send_to_char("Your victim is unaffected by hand of undead.\n\r",ch);
         return;
        }
    if ( victim->level <= 2 )
    {
        dam              = ch->hit + 1;
    }
    else
   {
     dam = dice( level, 10 );
     victim->mana       /= 2;
     victim->move       /= 2;
     ch->hit            += dam / 2;
    }

    send_to_char("You feel your life slipping away!\n\r",victim);
    act("$N is grasped by an incomprehensible hand of undead!",
                ch,NULL,victim,TO_NOTVICT);
    damage_spell( ch, victim, dam, sn,DAM_NEGATIVE,TRUE);
    return;
}

void spell_corruption( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED(victim,AFF_CORRUPTION))
        {
         act("$N is already corrupting.\n\r",ch,NULL,victim,TO_CHAR);
         return;
        }

    if (saves_spell(level,victim,DAM_NEGATIVE) ||
        (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
    {
        if (ch == victim)
          send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
   else
          act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level * 3/4;
    af.duration  = (10 + level / 5);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CORRUPTION;
    affect_join(victim,&af);

    send_to_char
      ("You scream in agony as you start to decay into dust.\n\r",victim);
    act("$n screams in agony as $n start to decay into dust.",
      victim,NULL,NULL,TO_ROOM);
}

void spell_hurricane(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int dam,hp_dam,dice_dam,hpch;

    act("$n prays the gods of the storm for help.",ch,NULL,NULL,TO_NOTVICT);
    act("You pray the gods of the storm to help you.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX(16,ch->hit);
    hp_dam = number_range(hpch/15+1,8);
    dice_dam = dice(level,12);
    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
    dam = dam * 1.5;
    
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


        if (!IS_AFFECTED(vch,AFF_FLYING)) dam /= 3;


       if (vch->size == SIZE_TINY)  dam *= 1.5;
        else if (vch->size == SIZE_SMALL)  dam *= 1.3;
        else if (vch->size == SIZE_MEDIUM)  dam *= 1;
        else if (vch->size == SIZE_LARGE)  dam *= 0.9;
      else if (vch->size == SIZE_HUGE)  dam *= 0.7;
        else dam *= 0.5; 

        if (saves_spell(level,vch,DAM_OTHER))
            damage_spell(ch,vch,dam/2,sn,DAM_OTHER,TRUE);
        else
            damage_spell(ch,vch,dam,sn,DAM_OTHER,TRUE);
    }
}

void spell_web( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

     if (saves_spell (level+6, victim,DAM_OTHER) ) return;

    if ( is_affected( victim, sn ) )
    {
        if (victim == ch)
          send_to_char("You are already webbed.\n\r",ch);
        else
          act("$N is already webbed.",ch,NULL,victim,TO_CHAR);
        return;
    }

 if (is_affected(ch, gsn_web))
    {
      send_to_char("{cYou cannot bring enough concentration.{x\n\r", ch);
      return;
    }

    af.type      = sn;
    af.level     = level;
    af.duration  = level / 40;
    af.location  = APPLY_HITROLL;
    af.modifier  = -2 * ( level / 6);
    af.where     = TO_AFFECTS2;
    af.bitvector = AFF2_WEBS;
    affect_to_char( victim, &af );

    af.location  = APPLY_DEX;
    af.modifier  = -3;
    affect_to_char( victim, &af );

    af.location  = APPLY_DAMROLL;
    af.modifier  = -1 * ( level / 6);
    affect_to_char( victim, &af );
  
      af.type	= gsn_web;
      af.level	= ch->level;
      af.duration	= 4;
      af.modifier	= 0;
      af.bitvector = 0;
      af.location	= 0;
      affect_to_char(ch,&af);

    send_to_char( "You are emeshed in thick webs!\n\r", victim );
    if ( ch != victim )
     act("You emesh $N in a bundle of webs!",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_esp( int sn, int level, CHAR_DATA *ch, void *vo , int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

     if (saves_spell (level, victim,DAM_OTHER) ) return;

    if ( is_affected( victim, sn ) )
    {
        if (victim == ch)
          send_to_char("You have already esp yourself.\n\r",ch);
        else
          act("$N has already been esped.",ch,NULL,victim,TO_CHAR);
        return;
    }

 if (is_affected(ch, gsn_esp))
    {
      send_to_char("{cYou need time to recharge your esp powers.{x\n\r", ch);
      return;
    }


    af.type      = sn;
    af.level     = level;
    af.duration  = level / 40;
    af.where     = TO_AFFECTS2;
    af.bitvector = AFF2_HOLD;
    affect_to_char( victim, &af );

      af.type	= gsn_esp;
      af.level	= ch->level;
      af.duration	= 4;
      af.modifier	= 0;
      af.bitvector = 0;
      af.location	= 0;
      affect_to_char(ch,&af);

    send_to_char( "You are wrapped in a field of esp rendering you immobile!\n\r", victim );
    if ( ch != victim )
     act("You wrapped $N in a field of ESP!",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_group_defense( int sn, int level, CHAR_DATA *ch, void *vo ,int target )
{
        CHAR_DATA *gch;
        AFFECT_DATA af;
        int shield_sn, armor_sn;

        shield_sn = skill_lookup("shield");
        armor_sn = skill_lookup("armor");

        for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room)
        {
                if( !is_same_group( gch, ch))
                        continue;
                if( is_affected( gch, armor_sn ) )
                {
                    if( gch == ch)
                        send_to_char("You are already armored.\n\r",ch);
                    else
                        act( "$N is already armored.", ch, NULL, gch,TO_CHAR);
             continue;
                }

                af.type      = armor_sn;
                af.level     = level;
                af.duration  = level;
                af.location  = APPLY_AC;
                af.modifier  = -20;
                af.bitvector = 0;
                affect_to_char( gch, &af );

                send_to_char( "You feel someone protecting you.\n\r",gch);
                if( ch != gch )
                        act("$N is protected by your magic.",
                                ch, NULL, gch, TO_CHAR );

                if( !is_same_group( gch, ch ) )
              continue;
                if( is_affected( gch, shield_sn ) )
                {
                  if( gch == ch)
                        send_to_char("You are already shielded.\n\r",ch);
                  else
                        act( "$N is already shielded.", ch, NULL, gch, TO_CHAR);
                  continue;
                }

                af.type      = shield_sn;
                af.level     = level;
                af.duration  = level;
                af.location  = APPLY_AC;
                af.modifier   = -20;
                af.bitvector = 0;
                affect_to_char( gch, &af );
            send_to_char( "You are surrounded by a force shield.\n\r",gch);
                if( ch != gch )
                        act("$N is surrounded by a force shield.",
                                ch, NULL, gch, TO_CHAR );

        }
        return;
}

void spell_inspire( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
        CHAR_DATA *gch;
        AFFECT_DATA af;
        int bless_sn;
       bless_sn = skill_lookup("bless");

        for( gch=ch->in_room->people; gch != NULL; gch=gch->next_in_room )
        {
                if( !is_same_group( gch, ch) )
                        continue;
                if ( is_affected(gch, bless_sn ) )
                {
                  if(gch == ch)
                     send_to_char("You are already inspired.\n\r",ch);
                  else
                     act("$N is already inspired.",
                        ch, NULL, gch, TO_CHAR);
                  continue;
                }
                af.type      = bless_sn;
                af.level     = level;
             af.duration  = 6 + level;
                af.location  = APPLY_HITROLL;
                af.modifier  = level/12;
                af.bitvector = 0;
                affect_to_char( gch, &af );

                af.location  = APPLY_SAVING_SPELL;
                af.modifier  = 0 - level/12;
                affect_to_char( gch, &af );

                send_to_char( "You feel inspired!\n\r", gch );
                if( ch != gch )
                        act( "You inspire $N with the Creator's power!",
                                ch, NULL, gch, TO_CHAR );

        }
        return;
}

void spell_group_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;

    heal_num = skill_lookup("master healing");
    refresh_num = skill_lookup("refresh");

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room
)
    {
        if ((IS_NPC(ch) && IS_NPC(gch)) ||
            (!IS_NPC(ch) && !IS_NPC(gch)))
        {
            spell_heal(heal_num,level,ch,(void *) gch,TARGET_CHAR);
            spell_refresh(refresh_num,level,ch,(void *) gch,TARGET_CHAR);
        }
    }
}

void spell_restoring_light( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int nsn, mana_add;

    if (IS_AFFECTED(victim,AFF_BLIND))
        {
         nsn = skill_lookup("cure blindness");
         spell_cure_blindness(nsn,level,ch,(void *)victim,TARGET_CHAR);
        }
    if (IS_AFFECTED(victim,AFF_CURSE))
        {
         nsn = skill_lookup("remove curse");
         spell_remove_curse(nsn,level,ch,(void *)victim,TARGET_CHAR);
       }
    if (IS_AFFECTED(victim,AFF_POISON))
        {
	nsn = skill_lookup("cure poison");
         spell_cure_poison(nsn,level,ch,(void*)victim,TARGET_CHAR);
        }
    if (IS_AFFECTED(victim,AFF_PLAGUE))
        {
         nsn = skill_lookup("cure disease");
         spell_cure_disease(nsn,level,ch,(void *)victim,TARGET_CHAR);
        }

    if (victim->hit != victim->max_hit)
        {
         mana_add = UMIN( (victim->max_hit - victim->hit), ch->mana );
         victim->hit = UMIN( victim->hit + mana_add, victim->max_hit );
         ch->mana -= mana_add;
        }
   update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}
void spell_polymorph( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;
    int race;

    if ( is_affected( ch, sn ) )
    {
     send_to_char("You are already polymorphed.\n\r",ch);
     return;
    }

    if (target_name == NULL)
    {
     send_to_char("Usage: cast 'polymorph' <pcracename>.\n\r",ch);
    return;
    }

    race = race_lookup( target_name );

    if (race == 0 || !race_table[race].pc_race)
    {
     send_to_char("That is not a valid race to polymorph.\n\r",ch);
     return;
    }

    af.where     = TO_RACE;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/10;
    af.location  = APPLY_NONE;
    af.modifier  = race;
   af.bitvector = 0;
    affect_to_char( ch, &af );

    act("$n polymorphes $mself to $t.",
                ch, race_table[race].name, NULL,TO_ROOM );
    act( "You polymorph yourself to $t.\n\r",
                ch, race_table[race].name, NULL,TO_CHAR );
    return;
}

void spell_dragons_breath(int sn,int level,CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;
    char buf[MAX_STRING_LENGTH];

    act("You call the dragon lord to help you.",ch,NULL,NULL,TO_CHAR);
    act("$n start to breath like a dragon.",ch,NULL,victim,TO_NOTVICT);
    act("$n breath disturbs you!",ch,NULL,victim,TO_VICT);
 act("You breath the breath of lord of Dragons.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level/2,20);

    dam = UMAX(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);

dam *= 0.5;
switch( dice(1,5) )
 {
         case 1:
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
 break;
    case 2:
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
    break;
    case 3:
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
    break;
    case 4:
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
    break;
    case 5:
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
    break;
  }
}

void spell_sand_storm( int sn, int level, CHAR_DATA *ch, void *vo,int target )
 {
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;

    if ( (ch->in_room->sector_type == SECT_AIR)
        || (ch->in_room->sector_type == SECT_WATER_SWIM)
     || (ch->in_room->sector_type == SECT_WATER_NOSWIM) )
        {
         send_to_char("You don't find any sand here to make storm.\n\r",ch);
         ch->wait = 0;
         return;
        }

    act("$n creates a storm with sands on the floor.",ch,NULL,NULL,TO_ROOM);
    act("You create the ..sand.. storm.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);
    sand_effect(ch->in_room,level,dam/2,TARGET_ROOM);
  for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;

        if (is_safe_spell(ch,vch,TRUE)
        ||  (IS_NPC(vch) && IS_NPC(ch)
        &&   (ch->fighting != vch || vch->fighting != ch)))
            continue;
        if ( is_safe(ch, vch) )
          continue;

            if (saves_spell(level,vch,DAM_COLD))
            {
                sand_effect(vch,level/2,dam/4,TARGET_CHAR);
                damage_spell(ch,vch,dam/2,sn,DAM_COLD,TRUE);
            }
            else
        {
                sand_effect(vch,level,dam,TARGET_CHAR);
                damage_spell(ch,vch,dam,sn,DAM_COLD,TRUE);
            }
    }
}
void spell_enhanced_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
        if (victim == ch)
          send_to_char("You are already enhancedly armored.\n\r",ch);
        else
          act("$N is already enhancedly armored.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 24;
    af.modifier  = -60;
   af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel protected for all attacks.\n\r", victim );
    if ( ch != victim )
        act("$N is protected by your magic.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_meld_into_stone( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( is_affected( victim, sn ) )
 {
      if (victim == ch)
        send_to_char("Your skin is already covered with stone.\n\r",
                     ch);
      else
        act("$N's skin is already covered with stone.",ch,NULL,
            victim,TO_CHAR);
      return;
    }
  af.where      = TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = level / 10;
  af.location  = APPLY_AC;
  af.modifier  = -100;
  af.bitvector = 0;
  affect_to_char( victim, &af );
  act( "$n's skin melds into stone.",victim,NULL,NULL,TO_ROOM );
  send_to_char( "Your skin melds into stone.\n\r", victim );
  return;
}

void spell_scream( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch, *vch_next;
    int dam=0,hp_dam,dice_dam;
    int hpch;

    act("$n screames with a disturbing NOISE!.",ch,NULL,NULL,TO_ROOM);
    act("You scream with a powerful sound.",ch,NULL,NULL,TO_CHAR);

    hpch = UMAX( 10, ch->hit );
    hp_dam  = number_range( hpch/9+1, hpch/5 );
    dice_dam = dice(level,20);
    dam = UMAX(hp_dam + dice_dam /10 , dice_dam + hp_dam /10);

    scream_effect(ch->in_room,level,dam/2,TARGET_ROOM);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;

        if (is_safe_spell(ch,vch,TRUE)
        ||  (IS_NPC(vch) && IS_NPC(ch)
        &&   (ch->fighting != vch || vch->fighting != ch)))
            continue;
        if ( is_safe(ch, vch) )
          continue;
           WAIT_STATE(vch, ( skill_table[sn].beats + (2 *PULSE_VIOLENCE)));
            if (saves_spell(level,vch,DAM_ENERGY))
            {
                scream_effect(vch,level/2,dam/4,TARGET_CHAR);
             if (vch->fighting)  stop_fighting( vch , TRUE );
            }
            else
            {
                scream_effect(vch,level,dam,TARGET_CHAR);
             if (vch->fighting)  stop_fighting( vch , TRUE );
            }
    }
}
void spell_aid( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

  if ( is_affected(victim, sn) )
        {
        if (victim == ch )
        send_to_char("You have been already aided.\n\r",ch);
        else
        act("$N has already been aided.",ch,NULL,victim,TO_CHAR);
        return;
        }
        af.where = TO_AFFECTS;
        af.type  = sn;
        af.level = level;
 af.where = TO_AFFECTS;
        af.type  = sn;
        af.level = level;
        af.location = APPLY_HITROLL;
        af.duration = level/2;
        af.modifier = 1 + (level >=30) + (level >=60) + (level>=90);
        af.bitvector = 0;
        affect_to_char(victim, &af );
        af.where = TO_AFFECTS;
        af.location = APPLY_HIT;
   af.modifier = level*1.5;
        affect_to_char(victim, &af );
        send_to_char("You are being aided!\n\r",victim);
        if (ch != victim )
        act("$N looks aided.",ch,NULL,victim,TO_CHAR);
        return;
}


void spell_shell( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  AFFECT_DATA af;

  if ( is_affected( ch, sn ) )
  {
    if (victim == ch)
      send_to_char("One shell is enough for anybody...\n\r",ch);
    else
      act("$N is already protected by a shell...",ch,NULL,victim,TO_CHAR);
    return;
  }

  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.level     = level;
  af.duration  = level;
  af.location  = APPLY_AC;
  af.modifier  = - (level*1.5);
  af.bitvector = 0;
  affect_to_char( victim, &af );

  af.location  = APPLY_HITROLL;
  af.modifier  = level/10;
  affect_to_char( victim, &af );

  af.location  = APPLY_DAMROLL;
  af.modifier  = level/10;
  affect_to_char( victim, &af );

  af.location  = APPLY_SAVING_SPELL;
  af.modifier  = 0 - level / 8;
  affect_to_char( victim, &af );

  act( "A dense shell forms about $n's body...", victim, NULL, NULL, TO_ROOM );
  send_to_char( "You are protected by a shell surrounding your body...\n\r",
                victim );
  return;
}

void spell_fear( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;

    victim = (CHAR_DATA *) vo;

    if (saves_spell(level,victim,DAM_NEGATIVE))
        return;

    send_to_char( "You are filled with fear of your opponent.\n\r",victim);
    act("$N's eyes grow wide with terror.",ch,NULL,victim,TO_CHAR);

    interpret(victim,"flee");

    return;
}

void spell_white_tiger( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *wtiger;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another tiger right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a white tiger.\n\r",ch);
  act("$n attempts to summon a white tiger.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_WHITE_TIGER)
	{
	  send_to_char("Two white tigers are more than you need!\n\r",ch);
	  return;
	}
    }
  wtiger = create_mobile( get_mob_index(MOB_VNUM_WHITE_TIGER) );

  for (i=0;i < MAX_STATS; i++)
    {
      wtiger->perm_stat[i] = ch->perm_stat[i];
    }

  wtiger->max_hit = ch->max_hit*3;
  wtiger->hit = wtiger->max_hit*3;
  wtiger->max_mana = ch->max_mana*3;
  wtiger->mana = wtiger->max_mana;
  wtiger->hitroll                = level*3.5;
  wtiger->damroll                = level*3.5;
  wtiger->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  wtiger->level = ch->level;
  for (i=0; i < 3; i++)
    wtiger->armor[i] = interpolate(wtiger->level*2,100,-100);
  wtiger->armor[3] = interpolate(wtiger->level*2,100,0);
  wtiger->gold = 0;

  xprintf( buf, wtiger->short_descr, ch->name );
  free_string( wtiger->short_descr );
  wtiger->short_descr = str_dup( buf );

  xprintf( buf, wtiger->long_descr, ch->name );
  free_string( wtiger->long_descr );
  wtiger->long_descr = str_dup( buf );
  
  xprintf( buf, wtiger->description, ch->name );
  free_string( wtiger->description );
  wtiger->description = str_dup( buf ); 
  
  char_to_room(wtiger,ch->in_room);
  send_to_char("A White tiger arrives from nowhere!\n\r",ch);
  act("An White tiger arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(wtiger->affected_by, AFF_CHARM);
  wtiger->master = wtiger->leader = ch;
}
void spell_wolf( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *eelement;
  AFFECT_DATA af;
//  AFFECT_DATA maf;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another wolf right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a wolf.\n\r",ch);
  act("$n attempts to summon a wolf.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_WOLF)
	{
	  send_to_char("Two wolves are more than you need!\n\r",ch);
	  return;
	}
    }
  eelement = create_mobile( get_mob_index(MOB_VNUM_WOLF) );

  for (i=0;i < MAX_STATS; i++)
    {
      eelement->perm_stat[i] = ch->perm_stat[i];
    }

  eelement->max_hit = ch->max_hit;
  eelement->hit = eelement->max_hit;
  eelement->max_mana = ch->max_mana;
  eelement->mana = eelement->max_mana;
  eelement->hitroll                = level;
  eelement->damroll                = level;
  eelement->level = ch->level;
  eelement->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
  eelement->armor[i] = interpolate(eelement->level*1.1,100,-100);
  eelement->armor[3] = interpolate(eelement->level*1.1,100,0);
  eelement->gold = 0;

  xprintf( buf, eelement->short_descr, ch->name );
  free_string( eelement->short_descr );
  eelement->short_descr = str_dup( buf );

  xprintf( buf, eelement->long_descr, ch->name );
  free_string( eelement->long_descr );
  eelement->long_descr = str_dup( buf );
  
  xprintf( buf, eelement->description, ch->name );
  free_string( eelement->description );
  eelement->description = str_dup( buf ); 
  
  char_to_room(eelement,ch->in_room);
  send_to_char("A wolf arrives from nowhere!\n\r",ch);
  act("A wolf arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  


  SET_BIT(eelement->affected_by, AFF_CHARM);
  eelement->master = eelement->leader = ch;
}

void spell_forest_bear( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *eelement;
  AFFECT_DATA af;
//  AFFECT_DATA maf;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another bear right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a bear.\n\r",ch);
  act("$n attempts to summon a bear.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_BEAR2)
	{
	  send_to_char("Two bears are more than you need!\n\r",ch);
	  return;
	}
    }
  eelement = create_mobile( get_mob_index(MOB_VNUM_BEAR2) );

  for (i=0;i < MAX_STATS; i++)
    {
      eelement->perm_stat[i] = ch->perm_stat[i];
    }

 eelement->max_hit = ch->max_hit;
  eelement->hit = eelement->max_hit;
  eelement->max_mana = ch->max_mana;
  eelement->mana = eelement->max_mana;
  eelement->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  eelement->hitroll                = level;
  eelement->damroll                = level;
  eelement->level = ch->level;
  for (i=0; i < 3; i++)
    eelement->armor[i] = interpolate(eelement->level/2,100,-100);
  eelement->armor[3] = interpolate(eelement->level/2,100,0);
  eelement->gold = 0;

  xprintf( buf, eelement->short_descr, ch->name );
  free_string( eelement->short_descr );
  eelement->short_descr = str_dup( buf );

  xprintf( buf, eelement->long_descr, ch->name );
  free_string( eelement->long_descr );
  eelement->long_descr = str_dup( buf );
  
  xprintf( buf, eelement->description, ch->name );
  free_string( eelement->description );
  eelement->description = str_dup( buf ); 
  
  char_to_room(eelement,ch->in_room);
  send_to_char("An bear arrives from nowhere!\n\r",ch);
  act("An bear arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(eelement->affected_by, AFF_CHARM);
  eelement->master = eelement->leader = ch;
}

void spell_lion( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *eelement;
  AFFECT_DATA af;
 // AFFECT_DATA maf;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another lion right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a lion.\n\r",ch);
  act("$n attempts to summon a lion.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_LION)
	{
	  send_to_char("Two lion's are more than you need!\n\r",ch);
	  return;
	}
    }
  eelement = create_mobile( get_mob_index(MOB_VNUM_LION) );

  for (i=0;i < MAX_STATS; i++)
    {
      eelement->perm_stat[i] = ch->perm_stat[i];
    }

  eelement->max_hit = ch->max_hit*2.5;
  eelement->hit = eelement->max_hit*2.5;
  eelement->max_mana = ch->max_mana*2.5;
  eelement->mana = eelement->max_mana*2.5;
  eelement->hitroll                = 30+level/4;
  eelement->damroll                = 30+level/4;
  eelement->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  eelement->level = ch->level;
  for (i=0; i < 3; i++)
    eelement->armor[i] = interpolate(eelement->level+15,100,-100);
  eelement->armor[3] = interpolate(eelement->level+15,100,0);
  eelement->gold = 0;

  xprintf( buf, eelement->short_descr, ch->name );
  free_string( eelement->short_descr );
  eelement->short_descr = str_dup( buf );

  xprintf( buf, eelement->long_descr, ch->name );
  free_string( eelement->long_descr );
  eelement->long_descr = str_dup( buf );
  
  xprintf( buf, eelement->description, ch->name );
  free_string( eelement->description );
  eelement->description = str_dup( buf ); 
  
  char_to_room(eelement,ch->in_room);
  send_to_char("A lion arrives from nowhere!\n\r",ch);
  act("A lion arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 20;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(eelement->affected_by, AFF_CHARM);
  eelement->master = eelement->leader = ch;
}
