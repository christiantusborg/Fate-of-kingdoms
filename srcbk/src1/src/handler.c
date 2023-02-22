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
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"
#include "tables.h"



/* command procedures needed */
DECLARE_DO_FUN(do_return	);
DECLARE_DO_FUN(do_wake		);
DECLARE_DO_FUN(do_raffects      );
DECLARE_DO_FUN(do_exits		);
DECLARE_DO_FUN(do_track		);
/*
 * Local functions.
 */
void	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
void    raffect_to_char args( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch) );
void    raffect_back_char       args( ( ROOM_INDEX_DATA *room, CHAR_DATA *ch) );
bool    is_safe_rspell  args( ( int level, CHAR_DATA *victim) );
void 	getpidmud	args( (void) );
int 		args( getpid(void) );
/* friend stuff -- for NPC's mostly */
bool is_friend(CHAR_DATA *ch,CHAR_DATA *victim)
{
    if (is_same_group(ch,victim))
	return TRUE;

    
    if (!IS_NPC(ch))
	return FALSE;

    if (!IS_NPC(victim))
    {
	if (IS_SET(ch->off_flags,ASSIST_PLAYERS))
	    return TRUE;
	else if ( (IS_SET(ch->off_flags,ASSIST_CLAN))
		 && ((ch->pIndexData->vnum-100)/10)==victim->kingdom )
	    return TRUE;
	else
	    return FALSE;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
	return FALSE;

    if (IS_SET(ch->off_flags,ASSIST_ALL))
	return TRUE;

    if (ch->group && ch->group == victim->group)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_VNUM) 
    &&  ch->pIndexData == victim->pIndexData)
	return TRUE;

    if (IS_SET(ch->off_flags,ASSIST_RACE) && ch->race == victim->race)
	return TRUE;
     
    if (IS_SET(ch->off_flags,ASSIST_ALIGN)
    &&  !IS_SET(ch->act,ACT_NOALIGN) && !IS_SET(victim->act,ACT_NOALIGN)
    &&  ((IS_GOOD(ch) && IS_GOOD(victim))
    ||	 (IS_EVIL(ch) && IS_EVIL(victim))
    ||   (IS_NEUTRAL(ch) && IS_NEUTRAL(victim))))
	return TRUE;

    return FALSE;
}


void room_record(char *name,ROOM_INDEX_DATA *room,sh_int door)
{
  ROOM_HISTORY_DATA *rec;
  int i;

  for (i=0,rec = room->history;i < 5 && rec != NULL;
       i++,rec = rec->next);

  if (i < 5) {
    rec = alloc_perm(sizeof(ROOM_HISTORY_DATA)); 
    rec->next = room->history;
    if (rec->next != NULL)
      rec->next->prev = rec; 
    room->history = rec; 
    rec->name = NULL;
  }
  else { 
    rec = room->history->next->next->next->next; 
    rec->prev->next = NULL; 
    rec->next = room->history; 
    rec->next->prev = rec; 
    room->history = rec; 
  }
  rec->prev = NULL;

  if(rec->name) {
     free_string(rec->name);
  }


  rec->name = str_dup(name);
  rec->went = door;
}

/* returns number of people on an object */
int count_users(OBJ_DATA *obj)
{
    CHAR_DATA *fch;
    int count = 0;

    if (obj->in_room == NULL)
	return 0;

    for (fch = obj->in_room->people; fch != NULL; fch = fch->next_in_room)
	if (fch->on == obj)
	    count++;

    return count;
}
     
/* returns material number */
int material_lookup (const char *name)
{
    return 0;
}


int weapon_lookup (const char *name)
{
    int type;

    for (type = 0; weapon_table[type].name != NULL; type++)
    {
	if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])
	&&  !str_prefix(name,weapon_table[type].name))
	    return type;
    }
 
    return -1;
}

bool kingdom_ok(CHAR_DATA *ch, sh_int sn) {

/*
  if ((ch->kingdom != skill_table[sn].kingdom)
	&&  skill_table[sn].kingdom != KINGDOM_NONE) {
	send_to_char("You don't belong in that kingdom.\n",ch);
	if(!IS_NPC(ch)) 
	ch->pcdata->learned[sn] = 0; 
        return FALSE;
	}

    if (kingdom_table[ch->kingdom].obj_ptr->in_room->vnum !=
          kingdom_table[ch->kingdom].room_vnum) {
      send_to_char("You cannot find the Kingdom Power within you.\n\r",ch);
      return FALSE;
    }

*/

  if ( ((ch->kingdom != skill_table[sn].kingdom)
        &&  skill_table[sn].kingdom != KINGDOM_NONE) )

/*||
	((skill_table[sn].kingdom != KINGDOM_NONE &&
      kingdom_table[ch->kingdom].obj_ptr != NULL) &&
   (kingdom_table[ch->kingdom].obj_ptr->carried_by == NULL && 
   kingdom_table[ch->kingdom].obj_ptr->carried_by->in_room == NULL) &&
   (kingdom_table[ch->kingdom].obj_ptr->in_obj == NULL &&
    kingdom_table[ch->kingdom].obj_ptr->in_obj->in_room == NULL) &&
      kingdom_table[ch->kingdom].obj_ptr->in_room != NULL &&
	(kingdom_table[ch->kingdom].obj_ptr->in_room->vnum !=
          kingdom_table[ch->kingdom].room_vnum))) */{
      send_to_char("You cannot find the Kingdom Power within you.\n\r",ch);
    return FALSE;
}

  return TRUE;
}

int weapon_type (const char *name)
{
    int type;
 
    for (type = 0; weapon_table[type].name != NULL; type++)
    {
        if (LOWER(name[0]) == LOWER(weapon_table[type].name[0])
        &&  !str_prefix(name,weapon_table[type].name))
            return weapon_table[type].type;
    }
 
    return WEAPON_EXOTIC;
}

char *item_name(int item_type)
{
    int type;

    for (type = 0; item_table[type].name != NULL; type++)
	if (item_type == item_table[type].type)
	    return item_table[type].name;
    return "none";
}

char *weapon_name( int weapon_type)
{
    int type;
 
    for (type = 0; weapon_table[type].name != NULL; type++)
        if (weapon_type == weapon_table[type].type)
            return weapon_table[type].name;
    return "exotic";
}

int weapon_attack_lookup  (const char *name)
{
    int att;

    for ( att = 0; attack_table[att].name != NULL; att++)
    {
	if (LOWER(name[0]) == LOWER(attack_table[att].name[0])
	&&  (!attack_table[att].weaponallow == 0)
	&&  !str_prefix(name,attack_table[att].name))
	    return att;
    }

    return 0;
}

int attack_lookup  (const char *name)
{
    int att;

    for ( att = 0; attack_table[att].name != NULL; att++)
    {
	if (LOWER(name[0]) == LOWER(attack_table[att].name[0])
	&&  !str_prefix(name,attack_table[att].name))
	    return att;
    }

    return 0;
}

int trap_attack_lookup  (const char *name)
{
    int att;

    for ( att = 0; trap_attack_table[att].name != NULL; att++)
    {
	if (LOWER(name[0]) == LOWER(trap_attack_table[att].name[0])
	&&  !str_prefix(name,trap_attack_table[att].name))
	    return att;
    }

    return 0;
}


/* returns a flag for wiznet */
long wiznet_lookup (const char *name)
{
    int flag;

    for (flag = 0; wiznet_table[flag].name != NULL; flag++)
    {
	if (LOWER(name[0]) == LOWER(wiznet_table[flag].name[0])
	&& !str_prefix(name,wiznet_table[flag].name))
	    return flag;
    }

    return -1;
}

/* returns class number */
int class_lookup (const char *name)
{
   int class;
 
   for ( class = 0; class < MAX_CLASS; class++)
   {
        if (LOWER(name[0]) == LOWER(class_table[class].name[0])
        &&  !str_prefix( name,class_table[class].name))
            return class;
   }
 
   return -1;
}

int class_lookup_load (const char *name)
{
   int class;
 
   for ( class = 0; class < MAX_CLASS; class++)
   {
        if (LOWER(name[0]) == LOWER(class_table[class].name[0])
        &&  !str_prefix( name,class_table[class].name))
            return class;
   }
 
   return class_lookup("human");
}

/* for immunity, vulnerabiltiy, and resistant
   the 'globals' (magic and weapons) may be overriden
   three other cases -- wood, silver, and iron -- are checked in fight.c */

int check_immune(CHAR_DATA *ch, int dam_type)
{
    int immune, def;
    int bit;

    immune = -1;
    def = IS_NORMAL;

    if (dam_type == DAM_NONE)
	return immune;

    if (dam_type <= 3)
    {
	if (IS_SET(ch->imm_flags,IMM_WEAPON))
	    def = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_WEAPON))
	    def = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_WEAPON))
	    def = IS_VULNERABLE;
    }
    else /* magical attack */
    {	
	if (IS_SET(ch->imm_flags,IMM_MAGIC))
	    def = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_MAGIC))
	    def = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_MAGIC))
	    def = IS_VULNERABLE;
    }

    /* set bits to check -- VULN etc. must ALL be the same or this will fail */
    switch (dam_type)
    {
	case(DAM_BASH):		bit = IMM_BASH;		break;
	case(DAM_PIERCE):	bit = IMM_PIERCE;	break;
	case(DAM_SLASH):	bit = IMM_SLASH;	break;
	case(DAM_FIRE):		bit = IMM_FIRE;		break;
	case(DAM_COLD):		bit = IMM_COLD;		break;
	case(DAM_LIGHTNING):	bit = IMM_LIGHTNING;	break;
	case(DAM_ACID):		bit = IMM_ACID;		break;
	case(DAM_POISON):	bit = IMM_POISON;	break;
	case(DAM_NEGATIVE):	bit = IMM_NEGATIVE;	break;
	case(DAM_HOLY):		bit = IMM_HOLY;		break;
	case(DAM_ENERGY):	bit = IMM_MAGIC;	break;
	case(DAM_MENTAL):	bit = IMM_MENTAL;	break;
	case(DAM_DISEASE):	bit = IMM_DISEASE;	break;
	case(DAM_DROWNING):	bit = IMM_DROWNING;	break;
	case(DAM_LIGHT):	bit = IMM_LIGHT;	break;
	case(DAM_CHARM):	bit = IMM_CHARM;	break;
	case(DAM_SOUND):	bit = IMM_SOUND;	break;
	default:		return def;
    }

    if (IS_SET(ch->imm_flags,bit))
	immune = IS_IMMUNE;
    else if (IS_SET(ch->res_flags,bit) && immune != IS_IMMUNE)
	immune = IS_RESISTANT;
    else if (IS_SET(ch->vuln_flags,bit))
    {
	if (immune == IS_IMMUNE)
	    immune = IS_RESISTANT;
	else if (immune == IS_RESISTANT)
	    immune = IS_NORMAL;
	else
	    immune = IS_VULNERABLE;
    }

    if (immune == -1)
	return def;
    else
      	return immune;
}

/* checks the material, if TRUE, will check if material exists,
 * if FALSE, will if the material exists and if it can me removed.
 */
bool check_material( OBJ_DATA *obj, char *material )
{
    if ( strstr( obj->material, material ) != NULL )
	return TRUE;
    else
        return FALSE;

}

bool is_metal( OBJ_DATA *obj )
{

  if ( check_material(obj, "silver") ||
       check_material(obj, "gold") ||
       check_material(obj, "iron") ||
       check_material(obj, "mithril") ||
       check_material(obj, "adamantite") ||
       check_material(obj, "steel") ||
       check_material(obj, "lead") ||
       check_material(obj, "bronze") ||
       check_material(obj, "copper") ||
       check_material(obj, "brass") ||
       check_material(obj, "platinium") ||
       check_material(obj, "titanium") ||
       check_material(obj, "aliminum") )
    return TRUE;

  return FALSE;

}

/* checks mob format */
bool is_old_mob(CHAR_DATA *ch)
{
    if (ch->pIndexData == NULL)
	return FALSE;
    else if (ch->pIndexData->new_format)
	return FALSE;
    return TRUE;
}
 
/* for returning skill information */
int get_skill(CHAR_DATA *ch, int sn)
{
    int skill;

    if (sn == -1) /* shorthand for level based skills */
    {
	skill = ch->level * 5 / 2;
    }

    else if (sn < -1 || sn > MAX_SKILL)
    {
	bug("Bad sn %d in get_skill.",sn);
	skill = 0;
    }

    else if (!IS_NPC(ch))
    {
	if (ch->level < skill_table[sn].skill_level[ch->class])
	    skill = 0;
	else
	    skill = ch->pcdata->learned[sn];
    }

    else /* mobiles */
    {


        if (skill_table[sn].spell_fun != spell_null)
	    skill = 40 + 2 * ch->level;

	else if (sn == gsn_sneak || sn == gsn_hide)
	    skill = ch->level * 2 + 20;

        else if ((sn == gsn_dodge && IS_SET(ch->off_flags,OFF_DODGE))
 	||       (sn == gsn_parry && IS_SET(ch->off_flags,OFF_PARRY)))
	    skill = ch->level * 2;

 	else if (sn == gsn_shield_block)
	    skill = 10 + 2 * ch->level;

	else if (sn == gsn_dirt 
	&& (IS_SET(ch->act,ACT_WARRIOR) || IS_SET(ch->act,ACT_THIEF)))
	    skill = 10 + 4 * ch->level;

	else if (sn == gsn_second_attack 
	&& (IS_SET(ch->act,ACT_WARRIOR) || IS_SET(ch->act,ACT_THIEF)))
	    skill = 10 + 3 * ch->level;

	else if (sn == gsn_third_attack && IS_SET(ch->act,ACT_WARRIOR))
	    skill = 4 * ch->level - 40;

	else if (sn == gsn_hand_to_hand)
	    skill = 40 + 2 * ch->level;

 	else if (sn == gsn_trip && IS_SET(ch->off_flags,OFF_TRIP))
	    skill = 10 + 3 * ch->level;

 	else if (sn == gsn_bash && IS_SET(ch->off_flags,OFF_BASH))
	    skill = 10 + 3 * ch->level;

 	else if (sn == gsn_shieldbash && IS_SET(ch->off_flags,OFF_BASH))
	    skill = 10 + 3 * ch->level;

	else if (sn == gsn_disarm 
	     &&  (IS_SET(ch->off_flags,OFF_DISARM) 
	     ||   IS_SET(ch->act,ACT_WARRIOR)
	     ||	  IS_SET(ch->act,ACT_THIEF)))
	    skill = 20 + 3 * ch->level;

	else if (sn == gsn_berserk && IS_SET(ch->off_flags,OFF_BERSERK))
	    skill = 3 * ch->level;

	else if (sn == gsn_kick)
	    skill = 10 + 3 * ch->level;

	else if (sn == gsn_backstab && IS_SET(ch->act,ACT_THIEF))
	    skill = 20 + 2 * ch->level;

  	else if (sn == gsn_rescue)
	    skill = 40 + ch->level; 

	else if (sn == gsn_recall)
	    skill = 40 + ch->level;

	else if (sn == gsn_sword
	||  sn == gsn_dagger
	||  sn == gsn_spear
	||  sn == gsn_mace
	||  sn == gsn_axe
	||  sn == gsn_flail
	||  sn == gsn_whip
	||  sn == gsn_polearm
        ||  sn == gsn_lance
	||  sn == gsn_bow)
	    skill = 40 + 5 * ch->level / 2;

	else 
	   skill = 0;
    }

    if (ch->daze > 0)
    {
	if (skill_table[sn].spell_fun != spell_null)
	    skill /= 2;
	else
	    skill = 2 * skill / 3;
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	skill = 9 * skill / 10;

    return URANGE(0,skill,100);
}

/* for returning weapon information */
int get_weapon_sn(CHAR_DATA *ch)
{
    OBJ_DATA *wield;
    int sn;

    wield = get_eq_char( ch, WEAR_WIELD );
    if (wield == NULL || wield->item_type != ITEM_WEAPON)
        sn = gsn_hand_to_hand;
    else switch (wield->value[0])
    {
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
	case(WEAPON_BOW):	sn = gsn_bow;		break;
        case(WEAPON_ARROW):     sn = gsn_arrow;         break;
        case(WEAPON_LANCE):     sn = gsn_lance;         break;
   }
   return sn;
}

/* for returning weapon information */
int get_second_sn(CHAR_DATA *ch)
{
    OBJ_DATA *wield;
    int sn;

    wield = get_eq_char( ch, WEAR_SECONDARY );
    if (wield == NULL || wield->item_type != ITEM_WEAPON)
        sn = 0;
    else switch (wield->value[0])
    {
        default :               sn = 0;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
	case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
        case(WEAPON_BOW):       sn = gsn_bow;           break;
        case(WEAPON_ARROW):     sn = gsn_arrow;         break;
        case(WEAPON_LANCE):     sn = gsn_lance;         break;
   }
   return sn;
}

int get_weapon_skill(CHAR_DATA *ch, int sn)
{
     int skill;

     /* -1 is exotic */
    if (IS_NPC(ch))
    {
	if (sn == -1)
	    skill = 3 * ch->level;
	else if (sn == gsn_hand_to_hand)
	    skill = 40 + 2 * ch->level;
	else 
	    skill = 40 + 5 * ch->level / 2;
    }
    
    else
    {
	if (sn == -1)
	    skill = 3 * ch->level;
	else
	    skill = ch->pcdata->learned[sn];
    }

    return URANGE(0,skill,100);
} 


void full_strip_affect(CHAR_DATA *ch)
{
  int i;
  
  while ( ch->affected )
    affect_remove( ch, ch->affected );

   ch->affected_by = race_table[ch->race].aff;
   ch->affected_by2 = race_table[ch->race].aff2;
   ch->detection = race_table[ch->race].detect;

  for (i = 0; i < 4; i++)
    ch->armor[i]= 100;
  reset_char(ch);
}

/* used to de-screw characters */
void reset_char(CHAR_DATA *ch)
{
     int loc,mod,stat;
     OBJ_DATA *obj;
     AFFECT_DATA *af;
     int i;

     if (IS_NPC(ch))
	return;

    if (ch->pcdata->perm_hit == 0 
    ||	ch->pcdata->perm_mana == 0
    ||  ch->pcdata->perm_move == 0
    ||	ch->pcdata->last_level == 0)
    {
    /* do a FULL reset */
	for (loc = 0; loc < MAX_WEAR; loc++)
	{
	    obj = get_eq_char(ch,loc);
	    if (obj == NULL)
		continue;
	    if (!obj->enchanted)
	    for ( af = obj->pIndexData->affected; af != NULL; af = af->next )
	    {
		mod = af->modifier;
		switch(af->location)
		{
		    case APPLY_SEX:	ch->sex		-= mod;
					if (ch->sex < 0 || ch->sex >2)
					    ch->sex = IS_NPC(ch) ?
						0 :
						ch->pcdata->true_sex;
									break;
		    case APPLY_MANA:	ch->max_mana	-= mod;		break;
		    case APPLY_HIT:	ch->max_hit	-= mod;		break;
		    case APPLY_MOVE:	ch->max_move	-= mod;		break;
		}
	    }

            for ( af = obj->affected; af != NULL; af = af->next )
            {
                mod = af->modifier;
                switch(af->location)
                {
                    case APPLY_SEX:     ch->sex         -= mod;         break;
                    case APPLY_MANA:    ch->max_mana    -= mod;         break;
                    case APPLY_HIT:     ch->max_hit     -= mod;         break;
                    case APPLY_MOVE:    ch->max_move    -= mod;         break;
                }
            }
	}
	/* now reset the permanent stats */
	ch->pcdata->perm_hit 	= ch->max_hit;
	ch->pcdata->perm_mana 	= ch->max_mana;
	ch->pcdata->perm_move	= ch->max_move;
	ch->pcdata->last_level	= ch->played/3600;
	if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)
	 {
		if (ch->sex > 0 && ch->sex < 3)
	    	    ch->pcdata->true_sex	= ch->sex;
		else
		    ch->pcdata->true_sex 	= 0;
	}	    

    }

    /* now restore the character to his/her true condition */
    for (stat = 0; stat < MAX_STATS; stat++)
	ch->mod_stat[stat] = 0;

    if (ch->pcdata->true_sex < 0 || ch->pcdata->true_sex > 2)
	ch->pcdata->true_sex = 0; 
    ch->sex		= ch->pcdata->true_sex;
    ch->max_hit 	= ch->pcdata->perm_hit;
    ch->max_mana	= ch->pcdata->perm_mana;
    ch->max_move	= ch->pcdata->perm_move;
   
    for (i = 0; i < 4; i++)
    	ch->armor[i]	= 100;

    ch->hitroll		= 0;
    ch->damroll		= 0;
    ch->saving_throw	= 0;

    /* now start adding back the effects */
    for (loc = 0; loc < MAX_WEAR; loc++)
    {
        obj = get_eq_char(ch,loc);
        if (obj == NULL)
            continue;

	for (i = 0; i < 4; i++)
	    ch->armor[i] -= apply_ac( obj, loc, i );

        if (!obj->enchanted)
	for ( af = obj->pIndexData->affected; af != NULL; af = af->next )
        {
            mod = af->modifier;
            switch(af->location)
            {
		case APPLY_STR:		ch->mod_stat[STAT_STR]	+= mod;	break;
		case APPLY_DEX:		ch->mod_stat[STAT_DEX]	+= mod; break;
		case APPLY_INT:		ch->mod_stat[STAT_INT]	+= mod; break;
		case APPLY_WIS:		ch->mod_stat[STAT_WIS]	+= mod; break;
		case APPLY_CON:		ch->mod_stat[STAT_CON]	+= mod; break;

		case APPLY_SEX:		ch->sex			+= mod; break;
		case APPLY_MANA:	ch->max_mana		+= mod; break;
		case APPLY_HIT:		ch->max_hit		+= mod; break;
		case APPLY_MOVE:	ch->max_move		+= mod; break;

		case APPLY_AC:		
		    for (i = 0; i < 4; i ++)
			ch->armor[i] += mod; 
		    break;
		case APPLY_HITROLL:	ch->hitroll		+= mod; break;
		case APPLY_DAMROLL:	ch->damroll		+= mod; break;
	
		case APPLY_SAVES:		ch->saving_throw += mod; break;
		case APPLY_SAVING_ROD: 		ch->saving_throw += mod; break;
		case APPLY_SAVING_PETRI:	ch->saving_throw += mod; break;
		case APPLY_SAVING_BREATH: 	ch->saving_throw += mod; break;
		case APPLY_SAVING_SPELL:	ch->saving_throw += mod; break;
	    }
        }
 
        for ( af = obj->affected; af != NULL; af = af->next )
        {
            mod = af->modifier;
            switch(af->location)
            {
                case APPLY_STR:         ch->mod_stat[STAT_STR]  += mod; break;
                case APPLY_DEX:         ch->mod_stat[STAT_DEX]  += mod; break;
                case APPLY_INT:         ch->mod_stat[STAT_INT]  += mod; break;
                case APPLY_WIS:         ch->mod_stat[STAT_WIS]  += mod; break;
                case APPLY_CON:         ch->mod_stat[STAT_CON]  += mod; break;
 
                case APPLY_SEX:         ch->sex                 += mod; break;
                case APPLY_MANA:        ch->max_mana            += mod; break;
                case APPLY_HIT:         ch->max_hit             += mod; break;
                case APPLY_MOVE:        ch->max_move            += mod; break;
 
                case APPLY_AC:
                    for (i = 0; i < 4; i ++)
                        ch->armor[i] += mod;
                    break;
		case APPLY_HITROLL:     ch->hitroll             += mod; break;
                case APPLY_DAMROLL:     ch->damroll             += mod; break;
 
                case APPLY_SAVES:         ch->saving_throw += mod; break;
                case APPLY_SAVING_ROD:          ch->saving_throw += mod; break;
                case APPLY_SAVING_PETRI:        ch->saving_throw += mod; break;
                case APPLY_SAVING_BREATH:       ch->saving_throw += mod; break;
                case APPLY_SAVING_SPELL:        ch->saving_throw += mod; break;
            }
	}
    }
  
    /* now add back spell effects */
    for (af = ch->affected; af != NULL; af = af->next)
    {
        mod = af->modifier;
        switch(af->location)
        {
                case APPLY_STR:         ch->mod_stat[STAT_STR]  += mod; break;
                case APPLY_DEX:         ch->mod_stat[STAT_DEX]  += mod; break;
                case APPLY_INT:         ch->mod_stat[STAT_INT]  += mod; break;
                case APPLY_WIS:         ch->mod_stat[STAT_WIS]  += mod; break;
                case APPLY_CON:         ch->mod_stat[STAT_CON]  += mod; break;
 
                case APPLY_SEX:         ch->sex                 += mod; break;
                case APPLY_MANA:        ch->max_mana            += mod; break;
                case APPLY_HIT:         ch->max_hit             += mod; break;
                case APPLY_MOVE:        ch->max_move            += mod; break;
 
                case APPLY_AC:
                    for (i = 0; i < 4; i ++)
                        ch->armor[i] += mod;
                    break;
                case APPLY_HITROLL:     ch->hitroll             += mod; break;
                case APPLY_DAMROLL:     ch->damroll             += mod; break;
 
                case APPLY_SAVES:         ch->saving_throw += mod; break;
                case APPLY_SAVING_ROD:          ch->saving_throw += mod; break;
                case APPLY_SAVING_PETRI:        ch->saving_throw += mod; break;
                case APPLY_SAVING_BREATH:       ch->saving_throw += mod; break;
                case APPLY_SAVING_SPELL:        ch->saving_throw += mod; break;
        } 
    }
    /* make sure sex is RIGHT!!!! */
    if (ch->sex < 0 || ch->sex > 2)
	ch->sex = ch->pcdata->true_sex;
 
}


/*
 * Retrieve a character's trusted level for permission checking.
 */
int get_trust( CHAR_DATA *ch )
{
//    if ( ch->desc != NULL && ch->desc->original != NULL )
//	ch = ch->desc->original;

	if(ch->desc && ch->desc->original)
	ch = ch->desc->original;

    if ( ch->trust != 0 && IS_SET(ch->comm,COMM_TRUE_TRUST))
	return ch->trust;

    if ( IS_NPC(ch) && ch->level >= LEVEL_HERO )
	return LEVEL_HERO - 1;
    else
	return ch->level;
}


/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA *ch )
{
    return 17 + ( ch->played + (int) (current_time - ch->logon) ) / 72000;
}

/* command for retrieving stats */
int get_curr_stat( CHAR_DATA *ch, int stat )
{/*
    int max;
OBJ_DATA *object;
// char buf[MAX_STRING_LENGTH];
    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	max = 25;

    else
    {
*/

    int max = 30;
    int Current = 0;
    int Bonus = 0;
    int Count = 0;
    

	Current = ch->perm_stat[stat];
	Count = 1;
	max = get_max_train(ch,stat);
	Bonus = ch->mod_stat[stat];
	
	if (Bonus < 0)
		{
			Current = max + Bonus;
if (Current < 3)
	Current = 3;
			
			return Current;
		}
		
	while (Bonus > 0)
	  {
		if (Current < get_max_train(ch,stat))
		  {
			Current++;
			Bonus--	;	  
		  }
		 else
		  {
			Count++;
			Bonus -= Count;
			if (Bonus > 0)
				Current++;
		  } 
	  }

	if (Current > 30)
		Current = 30;

        
// 	max = UMIN(max,25);

if (Current < 3)
	Current = 3;

	
return Current;  
/*

      // Max stat is the max you can get. Period.
        max = get_max_train(ch,stat);
 	max = UMIN(max,25);
    }

object = get_eq_char( ch, WEAR_HANDS );
if ( (  object != NULL ) && (stat == STAT_STR) )
  if (object->pIndexData->vnum == 190)
    return URANGE(3,25,25);
 

    return URANGE(3,ch->perm_stat[stat] + ch->mod_stat[stat], max);
*/
}


/* command for returning max training score */
int get_max_train( CHAR_DATA *ch, int stat )
{
    int max;

    if (IS_NPC(ch) || ch->level > LEVEL_IMMORTAL)
	return 25;

    max = pc_race_table[ch->race].max_stats[stat];
    if (class_table[ch->class].attr_prime == stat)
	if ( ch->race == race_lookup("human"))
	max += 2;

    return UMIN(max,25);
}
   
	
/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 1000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 10000;

    return MAX_WEAR + get_curr_stat(ch,STAT_DEX) - 10 + ch->size;
}



/*
 * Retrieve a character's carry capacity.
 */
int can_carry_w( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return 10000000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 10000;

    return str_app[get_curr_stat(ch,STAT_STR)].carry * 10 + ch->level * 25;
}



/*
 * See if a string is one of the names of an object.
 */
bool is_name( char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list, *string;

    string = str;
    /* we need ALL parts of string to match part of namelist */
    for ( ; ; )  /* start parsing string */
    {
	str = one_argument(str,part);

	if (part[0] == '\0' )
	    return TRUE;

	/* check to see if this is part of namelist */
	list = namelist;
	for ( ; ; )  /* start parsing namelist */
	{
	    list = one_argument(list,name);
	    if (name[0] == '\0')  /* this name was not found */
		return FALSE;

/*.  :: enabled mortal abbreviations, 5 lines commented
  *	    if (!str_cmp(string,name))
  *		return TRUE; 
  *	    if (!str_cmp(part,name))
  *		break;
  `*/

	    if (!str_prefix(string,name))
		return TRUE; /* full pattern match */

	    if (!str_prefix(part,name))
		break;

	}
    }
}

bool is_full_name( char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list, *string;


    string = str;
    /* we need ALL parts of string to match part of namelist */
    for ( ; ; )  /* start parsing string */
    {
	str = one_argument(str,part);

	if (part[0] == '\0' )
	    return TRUE;

	/* check to see if this is part of namelist */
	list = namelist;
	for ( ; ; )  /* start parsing namelist */
	{
	    list = one_argument(list,name);
	    if (name[0] == '\0')  /* this name was not found */
		return FALSE;

  	    if (!str_cmp(string,name))
  		return TRUE; 
  	    if (!str_cmp(part,name))
  		break;
	}
    }
}

bool is_name_imm( char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list, *string;


    string = str;
    /* we need ALL parts of string to match part of namelist */
    for ( ; ; )  /* start parsing string */
    {
	str = one_argument(str,part);

	if (part[0] == '\0' )
	    return TRUE;

	/* check to see if this is part of namelist */
	list = namelist;
	for ( ; ; )  /* start parsing namelist */
	{
	    list = one_argument(list,name);
	    if (name[0] == '\0')  /* this name was not found */
		return FALSE;

	    if (!str_prefix(string,name))
		return TRUE; /* full pattern match */

	    if (!str_prefix(part,name))
		break;
	}
    }
}


/* enchanted stuff for eq */
void affect_enchant(OBJ_DATA *obj)
{
    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
        AFFECT_DATA *paf, *af_new;
        obj->enchanted = TRUE;

        for (paf = obj->pIndexData->affected;
             paf != NULL; paf = paf->next)
        {
	    af_new = new_affect();

            af_new->next = obj->affected;
            obj->affected = af_new;
 
	    af_new->where	= paf->where;
            af_new->type        = UMAX(0,paf->type);
            af_new->level       = paf->level;
            af_new->duration    = paf->duration;
            af_new->location    = paf->location;
            af_new->modifier    = paf->modifier;
            af_new->bitvector   = paf->bitvector;
        }
    }
}
           

/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )
{
    OBJ_DATA *wield;
    int mod,i;

    mod = paf->modifier;

    if ( fAdd )
    {
	switch (paf->where)
	{
	case TO_AFFECTS:
	      SET_BIT(ch->affected_by, paf->bitvector);
	    break;
	 case TO_AFFECTS2:
	 SET_BIT(ch->affected_by2, paf->bitvector);
    	 break;
	case TO_DETECTS:
	SET_BIT(ch->detection, paf->bitvector);
	break;
	case TO_IMMUNE:
	    SET_BIT(ch->imm_flags,paf->bitvector);
	    break;
	case TO_RESIST:
	    SET_BIT(ch->res_flags,paf->bitvector);
	    break;
	case TO_VULN:
	    SET_BIT(ch->vuln_flags,paf->bitvector);
	    break;
	}
    }
    else
    {
        switch (paf->where)
        {
        case TO_AFFECTS:
              REMOVE_BIT(ch->affected_by, paf->bitvector);
            break;
	case TO_AFFECTS2:
	      REMOVE_BIT(ch->affected_by2, paf->bitvector);
    	break;
	case TO_DETECTS:
		REMOVE_BIT(ch->detection, paf->bitvector);
	break;
        case TO_IMMUNE:
            REMOVE_BIT(ch->imm_flags,paf->bitvector);
            break;
        case TO_RESIST:
            REMOVE_BIT(ch->res_flags,paf->bitvector);
            break;
        case TO_VULN:
            REMOVE_BIT(ch->vuln_flags,paf->bitvector);
            break;
        }
	mod = 0 - mod;
    }

    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STR:           ch->mod_stat[STAT_STR]	+= mod;	break;
    case APPLY_DEX:           ch->mod_stat[STAT_DEX]	+= mod;	break;
    case APPLY_INT:           ch->mod_stat[STAT_INT]	+= mod;	break;
    case APPLY_WIS:           ch->mod_stat[STAT_WIS]	+= mod;	break;
    case APPLY_CON:           ch->mod_stat[STAT_CON]	+= mod;	break;
    case APPLY_SEX:           ch->sex			+= mod;	break;
    case APPLY_CLASS:						break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:						break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:          ch->max_mana		+= mod;	break;
    case APPLY_HIT:           ch->max_hit		+= mod;	break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_AC:
        for (i = 0; i < 4; i ++)
            ch->armor[i] += mod;
        break;
    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    case APPLY_SAVES:   ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_ROD:    ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_PETRI:  ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_BREATH: ch->saving_throw		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_SPELL_AFFECT:  					break;
    }

    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     */
    if ( !IS_NPC(ch) && ( wield = get_eq_char( ch, WEAR_WIELD ) ) != NULL
    &&   get_obj_weight(wield) > (str_app[get_curr_stat(ch,STAT_STR)].wield*10))
    {
	static int depth;

	if ( depth == 0 )
	{
	    depth++;
	    act( "You drop $p.", ch, wield, NULL, TO_CHAR );
	    act( "$n drops $p.", ch, wield, NULL, TO_ROOM );
	    obj_from_char( wield );
	    obj_to_room( wield, ch->in_room );
	    depth--;
	}
    }

    return;
}


/* find an effect in an affect list */
AFFECT_DATA  *affect_find(AFFECT_DATA *paf, int sn)
{
    AFFECT_DATA *paf_find;
    
    for ( paf_find = paf; paf_find != NULL; paf_find = paf_find->next )
    {
        if ( paf_find->type == sn )
	return paf_find;
    }

    return NULL;
}

/* fix object affects when removing one */
void affect_check(CHAR_DATA *ch,int where,int vector)
{
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    if (where == TO_OBJECT || where == TO_WEAPON || vector == 0)
	return;

    for (paf = ch->affected; paf != NULL; paf = paf->next)
	if (paf->where == where && paf->bitvector == vector)
	{
	    switch (where)
	    {
	        case TO_AFFECTS:
		      SET_BIT(ch->affected_by,vector);
		    break;
		case TO_AFFECTS2:
			SET_BIT(ch->affected_by2, vector);
    		break;
		case TO_DETECTS:
			SET_BIT(ch->detection, vector);
		break;
	        case TO_IMMUNE:
		    SET_BIT(ch->imm_flags,vector);   
		    break;
	        case TO_RESIST:
		    SET_BIT(ch->res_flags,vector);
		    break;
	        case TO_VULN:
		    SET_BIT(ch->vuln_flags,vector);
		    break;
	    }
	    return;
	}

    for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
	if (obj->wear_loc == -1 || obj->wear_loc == WEAR_STUCK_IN)
	    continue;

            for (paf = obj->affected; paf != NULL; paf = paf->next)
            if (paf->where == where && paf->bitvector == vector)
            {
                switch (where)
                {
                    case TO_AFFECTS:
                      SET_BIT(ch->affected_by,vector);
                        break;
		   case TO_AFFECTS2:
		 	SET_BIT(ch->affected_by2,vector);
			break;
		 case TO_DETECTS:
			SET_BIT(ch->detection,vector);
			break;
                    case TO_IMMUNE:
                        SET_BIT(ch->imm_flags,vector);
                        break;
                    case TO_RESIST:
                        SET_BIT(ch->res_flags,vector);
                        break;
                    case TO_VULN:
                        SET_BIT(ch->vuln_flags,vector);
                  
                }
                return;
            }

        if (obj->enchanted)
	    continue;

        for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
            if (paf->where == where && paf->bitvector == vector)
            {
                switch (where)
                {
                    case TO_AFFECTS:
                      SET_BIT(ch->affected_by,vector);
                    break;
		     case TO_AFFECTS2:
			SET_BIT(ch->affected_by2,vector);
		    break;
			case TO_DETECTS:
			SET_BIT(ch->detection,vector);
		    break;
                    case TO_IMMUNE:
                        SET_BIT(ch->imm_flags,vector);
                        break;
                    case TO_RESIST:
                        SET_BIT(ch->res_flags,vector);
                        break;
                    case TO_VULN:
                        SET_BIT(ch->vuln_flags,vector);
                        break;
                }
                return;
            }
    }
}

/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    paf_new = new_affect();

    *paf_new		= *paf;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

    affect_modify( ch, paf_new, TRUE );
    return;
}

/* give an affect to an object */
void affect_to_obj(OBJ_DATA *obj, AFFECT_DATA *paf)
{
    AFFECT_DATA *paf_new;

    paf_new = new_affect();

    *paf_new		= *paf;
    paf_new->next	= obj->affected;
    obj->affected	= paf_new;

    /* apply any affect vectors to the object's extra_flags */
    if (paf->bitvector)
        switch (paf->where)
        {
        case TO_OBJECT:
    	    SET_BIT(obj->extra_flags,paf->bitvector);
	    break;
        case TO_WEAPON:
	    if (obj->item_type == ITEM_WEAPON)
	        SET_BIT(obj->value[4],paf->bitvector);
	    break;
        }
    

    return;
}



/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    int where;
    int vector;

    if ( ch->affected == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }

    affect_modify( ch, paf, FALSE );
    where = paf->where;
    vector = paf->bitvector;

    if ( paf == ch->affected )
    {
	ch->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = ch->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Affect_remove: cannot find paf.", 0 );
	    return;
	}
    }

    free_affect(paf);

    affect_check(ch,where,vector);
    return;
}

void affect_remove_obj( OBJ_DATA *obj, AFFECT_DATA *paf)
{
    int where, vector;
    if ( obj->affected == NULL )
    {
        bug( "Affect_remove_object: no affect.", 0 );
        return;
    }

    if (obj->carried_by != NULL && obj->wear_loc != -1)
	affect_modify( obj->carried_by, paf, FALSE );

    where = paf->where;
    vector = paf->bitvector;

    /* remove flags from the object if needed */
    if (paf->bitvector)
	switch( paf->where)
        {
        case TO_OBJECT:
            REMOVE_BIT(obj->extra_flags,paf->bitvector);
            break;
        case TO_WEAPON:
            if (obj->item_type == ITEM_WEAPON)
                REMOVE_BIT(obj->value[4],paf->bitvector);
            break;
        }

    if ( paf == obj->affected )
    {
        obj->affected    = paf->next;
    }
    else
    {
        AFFECT_DATA *prev;

        for ( prev = obj->affected; prev != NULL; prev = prev->next )
        {
            if ( prev->next == paf )
            {
                prev->next = paf->next;
                break;
            }
        }

        if ( prev == NULL )
        {
            bug( "Affect_remove_object: cannot find paf.", 0 );
            return;
        }
    }

    free_affect(paf);

    if (obj->carried_by != NULL && obj->wear_loc != -1)
	affect_check(obj->carried_by,where,vector);
    return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf );
    }

    return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;
    bool found;

    found = FALSE;
    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->level = (paf->level += paf_old->level) / 2;
	//    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old );
	    break;
	}
    }

    affect_to_char( ch, paf );
    return;
}



/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *prev_room = ch->in_room;

    if ( ch->in_room == NULL )
    {
	bug( "Char_from_room: NULL.", 0 );
	return;
    }

    if ( !IS_NPC(ch) )
	--ch->in_room->area->nplayer;

    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 );
    }

    ch->in_room      = NULL;
    ch->next_in_room = NULL;
    ch->on 	     = NULL;  /* sanity check! */

   if (MOUNTED(ch))
    {
     ch->mount->riding  = FALSE;
     ch->riding         = FALSE;
    }

    if ( prev_room && prev_room->affected_by )
          raffect_back_char( prev_room, ch);
    return;
}



/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    OBJ_DATA *obj;
    
    if ( pRoomIndex == NULL )
    {
	ROOM_INDEX_DATA *room;

	if ((room = get_room_index(ROOM_VNUM_TEMPLE)) != NULL)
	    char_to_room(ch,room);

	bug( "Char_to_room: NULL.", 0 );
		
	return;
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;

    if ( !IS_NPC(ch) )
    {
	if (ch->in_room->area->empty)
	{
	    ch->in_room->area->empty = FALSE;
	    ch->in_room->area->age = 0;
	}
	++ch->in_room->area->nplayer;
    }

    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
    &&   obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0 )
	++ch->in_room->light;
	
//    if (IS_AFFECTED(ch,AFF_PLAGUE))
	  while (IS_AFFECTED(ch,AFF_PLAGUE))
    {
        AFFECT_DATA *af, plague;
        CHAR_DATA *vch;
        
        for ( af = ch->affected; af != NULL; af = af->next )
        {
            if (af->type == gsn_plague)
                break;
        }
        
        if (af == NULL)
        {
            REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
            return;
        }
        
        if (af->level == 1)
            return;
        
	plague.where		= TO_AFFECTS;
        plague.type 		= gsn_plague;
        plague.level 		= af->level - 1; 
        plague.duration 	= number_range(1,2 * plague.level);
        plague.location		= APPLY_STR;
        plague.modifier 	= -5;
        plague.bitvector 	= AFF_PLAGUE;
        
        for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            if (!saves_spell(plague.level - 2,vch,DAM_DISEASE) 
	    &&  !IS_IMMORTAL(vch) &&
            	!IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(6) == 0)
            {
            	send_to_char("You feel hot and feverish.\n\r",vch);
            	act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
            	affect_join(vch,&plague);
            }
        }
	break;
    }

     if ( ch->in_room->affected_by )
        {
         if (IS_IMMORTAL(ch))
                do_raffects(ch,"");
         else raffect_to_char( ch->in_room, ch);
        }

    do_exits( ch, "vt" );
    return;
}



/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
    obj->next_content	 = ch->carrying;
    ch->carrying	 = obj;
    obj->carried_by	 = ch;
    obj->in_room	 = NULL;
    obj->in_obj		 = NULL;
    ch->carry_number	+= get_obj_number( obj );
    ch->carry_weight	+= get_obj_weight( obj );
}



/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj )
{
    CHAR_DATA *ch;

    if ( ( ch = obj->carried_by ) == NULL )
    {
	bug( "Obj_from_char: null ch.", 0 );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
	unequip_char( ch, obj );

    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Obj_from_char: obj not in list.", 0 );
    }

    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number	-= get_obj_number( obj );
    ch->carry_weight	-= get_obj_weight( obj );
    return;
}



/*
 * Find the ac value of an obj, including position effect.
 */
int apply_ac( OBJ_DATA *obj, int iWear, int type )
{
    if (obj == NULL)
     return 0;
     
    if ( obj->item_type != ITEM_ARMOR )
	return 0;

    switch ( iWear )
    {
    case WEAR_BODY:	return 3 * obj->value[type];
    case WEAR_HEAD:	return 2 * obj->value[type];
    case WEAR_LEGS:	return 2 * obj->value[type];
    case WEAR_FEET:	return     obj->value[type];
    case WEAR_HANDS:	return     obj->value[type];
    case WEAR_ARMS:	return     obj->value[type];
    case WEAR_SHIELD:	return     obj->value[type];
    case WEAR_FINGER_L: return .7* obj->value[type];
    case WEAR_FINGER_R: return .7* obj->value[type];
    case WEAR_NECK_1:	return .7* obj->value[type];
    case WEAR_NECK_2:	return .7* obj->value[type];
    case WEAR_ABOUT:	return 2 * obj->value[type];
    case WEAR_WAIST:	return     obj->value[type];
    case WEAR_WRIST_L:	return     obj->value[type];
    case WEAR_WRIST_R:	return     obj->value[type];
    case WEAR_HOLD:	return     obj->value[type];
    case WEAR_TATTOO:	return     obj->value[type];
    case WEAR_ORNAMENT_1: return .5* obj->value[type];
    case WEAR_ORNAMENT_2: return .5* obj->value[type];
    }

    return 0;
}



/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == iWear )
	    return obj;
    }

    return NULL;
}



/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
    AFFECT_DATA *paf;
    int i;

   if (iWear == WEAR_STUCK_IN)
    {
        obj->wear_loc = iWear;
        return;
    }

    if ( get_eq_char( ch, iWear ) != NULL )
    {
	bug( "Equip_char: already equipped (%d).", iWear );
	return;
    }

    if ( (( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) ))
    && (!IS_NPC(ch)) )//mod by mike to prevent mobs loosing thier items
    {
	/*
	 * Thanks to Morgenes for the bug fix here!
	 */
	act( "{gYou are zapped by $p and drop it.{x", ch, obj, NULL, TO_CHAR);
	act( "{g$n is zapped by $p and drops it.{x",  ch, obj, NULL,TO_ROOM );
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	return;
    }

    for (i = 0; i < 4; i++)
    	ch->armor[i]      	-= apply_ac( obj, iWear,i );
    obj->wear_loc	 = iWear;

    if (!obj->enchanted)
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	    if ( paf->location != APPLY_SPELL_AFFECT )
	        affect_modify( ch, paf, TRUE );
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	if ( paf->location == APPLY_SPELL_AFFECT )
    	    affect_to_char ( ch, paf );
	else
	    affect_modify( ch, paf, TRUE );

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL )
	++ch->in_room->light;

    if (IS_SET(obj->progtypes,IPROG_WEAR))
       (obj->pIndexData->iprogs->wear_prog) (obj,ch);

    return;
}



/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
    AFFECT_DATA *paf = NULL;
    AFFECT_DATA *lpaf = NULL;
    AFFECT_DATA *lpaf_next = NULL;
    int i;

    if ( obj->wear_loc == WEAR_NONE )
    {
	bug( "Unequip_char: already unequipped.", 0 );
	return;
    }

  if ( obj->wear_loc == WEAR_STUCK_IN )
    {
        obj->wear_loc = WEAR_NONE;
        return;
    }

    for (i = 0; i < 4; i++)
    	ch->armor[i]	+= apply_ac( obj, obj->wear_loc,i );
    obj->wear_loc	 = -1;

    if (!obj->enchanted)
     {
	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
	    if ( paf->location == APPLY_SPELL_AFFECT )
	    {
	        for ( lpaf = ch->affected; lpaf != NULL; lpaf = lpaf_next )
	        {
		    lpaf_next = lpaf->next;
		    if ((lpaf->type == paf->type) &&
		        (lpaf->level == paf->level) &&
		        (lpaf->location == APPLY_SPELL_AFFECT))
		    {
		        affect_remove( ch, lpaf );
			lpaf_next = NULL;
		    }
	        }
	    }
	    else
	    {
	        affect_modify( ch, paf, FALSE );
		affect_check(ch,paf->where,paf->bitvector);
	    }
       }
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	if ( paf->location == APPLY_SPELL_AFFECT )
	{
	    bug ( "Norm-Apply: %d", 0 );
	    for ( lpaf = ch->affected; lpaf != NULL; lpaf = lpaf_next )
	    {
		lpaf_next = lpaf->next;
		if ((lpaf->type == paf->type) &&
		    (lpaf->level == paf->level) &&
		    (lpaf->location == APPLY_SPELL_AFFECT))
		{
		    bug ( "location = %d", lpaf->location );
		    bug ( "type = %d", lpaf->type );
		    affect_remove( ch, lpaf );
		    lpaf_next = NULL;
		}
	    }
	}
	else
	{
	    affect_modify( ch, paf, FALSE );
	    affect_check(ch,paf->where,paf->bitvector);	
	}

    if ( obj->item_type == ITEM_LIGHT
    &&   obj->value[2] != 0
    &&   ch->in_room != NULL
    &&   ch->in_room->light > 0 )
	--ch->in_room->light;

    if (IS_SET(obj->progtypes,IPROG_REMOVE))
      (obj->pIndexData->iprogs->remove_prog) (obj,ch);

    return;
}



/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
    ROOM_INDEX_DATA *in_room;
    CHAR_DATA *ch;

    if ( ( in_room = obj->in_room ) == NULL )
    {
	bug( "obj_from_room: NULL.", 0 );
	return;
    }

    for (ch = in_room->people; ch != NULL; ch = ch->next_in_room)
	if (ch->on == obj)
	    ch->on = NULL;

    if ( obj == in_room->contents )
    {
	in_room->contents = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = in_room->contents; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_room: obj not found.", 0 );
	    return;
	}
    }

    obj->in_room      = NULL;
    obj->next_content = NULL;
    return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
{
    obj->next_content		= pRoomIndex->contents;
    pRoomIndex->contents	= obj;
    obj->in_room		= pRoomIndex;
    obj->carried_by		= NULL;
    obj->in_obj			= NULL;
    return;
}



/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
    obj->next_content		= obj_to->contains;
    obj_to->contains		= obj;
    obj->in_obj			= obj_to;
    obj->in_room		= NULL;
    obj->carried_by		= NULL;
    if (obj_to->pIndexData->vnum == OBJ_VNUM_PIT)
        obj->cost = 0; 

    for ( ; obj_to != NULL; obj_to = obj_to->in_obj )
    {
	if ( obj_to->carried_by != NULL )
	{
/*	    obj_to->carried_by->carry_number += get_obj_number( obj ); */
	    obj_to->carried_by->carry_weight += get_obj_weight( obj )
		* WEIGHT_MULT(obj_to) / 100;
	}
    }

    return;
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_from;

    if ( ( obj_from = obj->in_obj ) == NULL )
    {
	bug( "Obj_from_obj: null obj_from.", 0 );
	return;
    }

    if ( obj == obj_from->contains )
    {
	obj_from->contains = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = obj_from->contains; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_obj: obj not found.", 0 );
	    return;
	}
    }

    obj->next_content = NULL;
    obj->in_obj       = NULL;

    for ( ; obj_from != NULL; obj_from = obj_from->in_obj )
    {
	if ( obj_from->carried_by != NULL )
	{
/*	    obj_from->carried_by->carry_number -= get_obj_number( obj ); */
	    obj_from->carried_by->carry_weight -= get_obj_weight( obj ) 
		* WEIGHT_MULT(obj_from) / 100;
	}
    }
if (obj_from != NULL)
	    if (obj_from->item_type == ITEM_CORPSE_PC)
	    save_copyover_obj();

    return;
}

/* Extract an object and modify the count */
void extract_obj( OBJ_DATA *obj )
{
  extract_obj_1(obj,TRUE);
}

/* Extract an object and do not modify the count */
void extract_obj_nocount( OBJ_DATA *obj )
{
  extract_obj_1(obj,FALSE);
}

/*
 * Extract an obj from the world.
 */
void extract_obj_1( OBJ_DATA *obj, bool count )
{
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    int i;
    char buf[MAX_STRING_LENGTH];

    if (obj->extracted)  /* if the object has already been extracted once */
      {
        xprintf(buf, "Warning! Extraction of %s, vnum %d.", obj->name,
                obj->pIndexData->vnum);
        bug(buf, 0);
        return; /* if it's already been extracted, something bad is going on */
      }
    else
      obj->extracted = TRUE;  /* if it hasn't been extracted yet, now
                               * it's being extracted. */
 
    if ( obj->in_room != NULL )
	obj_from_room( obj );
    else if ( obj->carried_by != NULL )
	obj_from_char( obj );
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );

    for (i=1;i < MAX_KINGDOM;i++)
      if (obj->pIndexData->vnum == kingdom_table[i].obj_vnum) {
        obj->pIndexData->count--;
        kingdom_table[i].obj_ptr = NULL;
      }

    for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
    {
	obj_next = obj_content->next_content;
	extract_obj_1( obj_content, count );
    }

    if ( object_list == obj )
    {
	object_list = obj->next;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = object_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == obj )
	    {
		prev->next = obj->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );
	    return;
	}
    }
    if (count)
      --obj->pIndexData->count;
    free_obj(obj);
    return;
}

void extract_char( CHAR_DATA *ch, bool fPull )
{
  extract_char_org( ch, fPull, TRUE );
  return;
}

void extract_char_nocount( CHAR_DATA *ch, bool fPull )
{
  extract_char_org( ch, fPull, FALSE );
  return;
}


/*
 * Extract a char from the world.
 */
void extract_char_org( CHAR_DATA *ch, bool fPull, bool Count )
{
    CHAR_DATA *wch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *wield;
    int i;    
    char buf[MAX_STRING_LENGTH];

    if (fPull) /* only for total extractions should it check */
    {
    if (ch->extracted)  /* if the char has already been extracted once */
      {
        xprintf(buf, "Warning! Extraction of %s.", ch->name);
        bug(buf, 0);
        return; /* if it's already been extracted, something bad is going on */
      }
    else
      ch->extracted = TRUE;  /* if it hasn't been extracted yet, now
                               * it's being extracted. */
    }

    if ( ch->in_room == NULL )
    {
	bug( "Extract_char: NULL.", 0 );
	return;
    }
    
    nuke_pets(ch);
    ch->pet = NULL; /* just in case */

    if ( fPull )

	die_follower( ch );
    
    stop_fighting( ch, TRUE );

    if ((wield = get_eq_char(ch, WEAR_WIELD)) != NULL)
      unequip_char(ch, wield); /* prevent weapon recursion code from
                                * kicking in and forcing the player
                                * to drop their weapon while quitting. */
    
if ( !IS_SET ( ch->in_room->room_flags, ROOM_ARENA ) )
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( Count )
  	  extract_obj( obj );
	else
	  extract_obj_nocount( obj );
    }
    
    char_from_room( ch );

    if ( !fPull )
    {
        if (IS_GOOD(ch))
          i = 0;
        else if (IS_EVIL(ch))
          i = 2;
        else
          i = 1;
        if (ch->level > 50)
        char_to_room(ch, get_room_index(ROOM_VNUM_AFTERLIFE));
        else
        char_to_room(ch, get_room_index(hometown_table[ch->hometown].altar[i]));
	return;
    }

    if ( !IS_NPC( ch ) && fPull )
    {
        ch->pcdata->quest = NULL;
        REVOKE(ch, auth_questtalk);
    }


    if ( IS_NPC(ch) )
	--ch->pIndexData->count;

 if ( ch->mount && ch->mount->mount == ch )
        {
            ch->mount->mount = NULL;

            if (ch->mount->riding)
            {
                act("You fall off of $N.", ch->mount, NULL, ch, TO_CHAR);
                act("$n falls off of $N.", ch->mount, NULL, ch, TO_ROOM);
                ch->mount->riding = FALSE;
                if (!IS_IMMORTAL(ch->mount))
             ch->mount->position = POS_SITTING;
            }
        }

    if ( ch->desc != NULL && ch->desc->original != NULL )
    {
	do_return( ch, "" );
	ch->desc = NULL;
    }

    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch->reply == ch )
	    wch->reply = NULL;
//        if ( ch->mprog_target == ch )
//           wch->mprog_target = NULL;
    }

    if ( ch == char_list )
    {
       char_list = ch->next;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = char_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == ch )
	    {
		prev->next = ch->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_char: char not found.", 0 );
	    return;
	}
    }

    if ( ch->desc != NULL )
	ch->desc->character = NULL;
    free_char( ch );
    return;
}

/*
 * Find a random char in the room.
 *
 */
CHAR_DATA *get_char_random_room( CHAR_DATA *ch ,int type)
{
  CHAR_DATA *rch,*vch;
  int summ=0;
  int z;
  for (rch=ch->in_room->people;rch!=NULL;rch=rch->next_in_room)
  {
       if (!can_see(ch,rch))
	  continue;
       if (type == 0 && !IS_NPC(rch)) continue;
       if (type == 1 &&  IS_NPC(rch)) continue;
  summ++;   
  }
  vch=NULL;
  z=dice(1,summ);
  summ=0;
  for (rch=ch->in_room->people;rch!=NULL;rch=rch->next_in_room)
  {
       if (!can_see(ch,rch))
	  continue;
       if (type == 0 && !IS_NPC(rch)) continue;
       if (type == 1 &&  IS_NPC(rch)) continue;
       summ++;
       if (z==summ)  {
		vch=rch;
		break;
	}
    }
    return vch;
}
/*
 * Find a random char in the world.
 *  0=npc 1=pc 2=npc&pc
 */
CHAR_DATA *get_char_random_world( CHAR_DATA *ch ,int type)
{
  CHAR_DATA *rch,*vch;
  int summ=0;
  int z;
  if ( ( vch = get_char_random_room( ch,type) ) != NULL )
	return vch;
  
  for (rch=char_list;rch!=NULL;rch=rch->next)
  {
       if (!can_see(ch,rch))
	  continue;
       if (type == 0 && !IS_NPC(rch)) continue;
       if (type == 1 &&  IS_NPC(rch)) continue;
  summ++;   
  }
  vch=NULL;
  z=dice(1,summ);
  summ=0;
  for (rch=ch->in_room->people;rch!=NULL;rch=rch->next_in_room)
  {
       if (!can_see(ch,rch))
	  continue;
       if (type == 0 && !IS_NPC(rch)) continue;
       if (type == 1 &&  IS_NPC(rch)) continue;
	summ++;
       if (z==summ)  {
		vch=rch;
		break;
	}
    }
    return vch;
}
/*
 * Find a char in the room.

 */


CHAR_DATA *get_char_room( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;


/* confuse */
     if (is_affected(ch,gsn_confuse)) {
	  rch = get_char_random_room(ch,2);
	  return rch;
	 } 

    if ( !str_cmp( arg, "self" ) )
	return ch;

    if ( ch && room )
    {
	bug( "get_char_room received multiple types (ch/room)", 0 );
	return NULL;
    }

    if ( ch )
	rch = ch->in_room->people;
    else
	rch = room->people;


    for ( ; rch != NULL; rch = rch->next_in_room )
    {
	if ( (ch && !can_see( ch, rch )) || !is_name( arg, rch->name ) )
	    continue;
	if ( ++count == number )
	    return rch;
    }

    return NULL;
}

CHAR_DATA *get_char_room2( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument, int *number )
{
    CHAR_DATA *rch;
    int count;
    int ugly;

    if (room == NULL ) return NULL;
    count  = 0;
    ugly   = 0;

    if ( !str_cmp( argument, "ugly" ) )
        ugly = 1;

    for ( rch = room->people; rch != NULL; rch = rch->next_in_room )
    {
        if ( !can_see( ch, rch ) )
         continue;
/*
        if ( ugly && (count + 1) == *number && IS_VAMPIRE(rch) )
           return rch;
*/
        if ( (is_affected(rch,gsn_doppelganger)
              && !IS_SET(ch->act,PLR_HOLYLIGHT))?
        !is_name(argument,rch->doppel->name):!is_name(argument,rch->name))
          continue;

        if ( ++count == *number )
            return rch;
    }

    *number -= count;
    return NULL;
}



/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ch && ( wch = get_char_room( ch, NULL, argument ) ) != NULL )
	return wch;

/* confuse*/
    if (is_affected(ch,gsn_confuse)) 
    {
	  wch = get_char_random_world(ch,2);
          return wch;
   }
    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( wch->in_room == NULL || ( ch && !can_see( ch, wch ) ) 
	||   !is_name( arg, wch->name ) )
	    continue;
	if ( ++count == number )
	    return wch;
    }


    return NULL;
}


CHAR_DATA *get_char_zone( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( ch && ( wch = get_char_room( ch, NULL, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( wch->in_room == NULL || ( ch && !can_see( ch, wch ) ) 
	||   !is_name( arg, wch->name ) )
	    continue;

	if (ch->pIndexData->area != wch->pIndexData->area)
	 	continue;
	 	
	if ( ++count == number )
	    return wch;
    }


    return NULL;
}

CHAR_DATA *get_char( CHAR_DATA *ch )
{
    if ( ch->pcdata )
      return ch;
    else if ( ch->desc )
      return ch->desc->original;
    else
      return NULL;
}


/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    return obj;
    }

    return NULL;
}


/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
      if ( !IS_IMMORTAL(ch) )  {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
      }
      else  {
	if ( can_see_obj( ch, obj ) && is_name_imm( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
      }
    }

    return NULL;
}



/*
 * Find an obj in player's inventory.
 */


OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument, CHAR_DATA *viewer )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
        if ( obj->wear_loc == WEAR_NONE
        &&   ( viewer ? can_see_obj( viewer, obj ) : TRUE )
        &&   is_name( arg, obj->name ) )
        {
            if ( ++count == number )
                            return obj;
        }
    }

    return NULL;
}

/*
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument CHAR_DATA *viewer)
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
      if (!IS_IMMORTAL( ch ) )  {
	if ( obj->wear_loc == WEAR_NONE
	&&   (can_see_obj( ch, obj ) ) 
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
      }
      else  {
	if ( obj->wear_loc == WEAR_NONE
	&&   (can_see_obj( ch, obj ) ) 
	&&   is_name_imm( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
      }
    }

    return NULL;
}
*/
OBJ_DATA *get_obj_carry_see_anyway( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
      if (!IS_IMMORTAL( ch ) )  {
	if ( obj->wear_loc == WEAR_NONE
	&&   is_name( arg, obj->name ) 
	&& !IS_SET( obj->extra_flags, ITEM_INVENTORY))
	{
	    if ( ++count == number )
		return obj;
	}
      }
      else  {
	if ( obj->wear_loc == WEAR_NONE
	&&   is_name_imm( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
      }
    }

    return NULL;
}


/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument, bool character )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
        if ( obj->wear_loc != WEAR_NONE
                &&  ( character ? can_see_obj( ch, obj ) : TRUE)
        &&   is_name( arg, obj->name ) )
        {
            if ( ++count == number )
                return obj;
        }
    }

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, ROOM_INDEX_DATA *room, char *argument )
{
    OBJ_DATA *obj;
    int number, count;
    char arg[MAX_INPUT_LENGTH];

    if ( ch && room )
    {
	bug( "get_obj_here received a ch and a room",0);
	return NULL;
    }

    number = number_argument( argument, arg );
    count = 0;

    if ( ch )
    {
	obj = get_obj_list( ch, argument, ch->in_room->contents );
	if ( obj != NULL )
	    return obj;

	if ( ( obj = get_obj_carry( ch, argument, ch ) ) != NULL )
	    return obj;

	if ( ( obj = get_obj_wear( ch, argument, TRUE ) ) != NULL )
	    return obj;
    }
    else
    {
	for ( obj = room->contents; obj; obj = obj->next_content )
	{
	    if ( !is_name( arg, obj->name ) )
		continue;
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    if ( ch && ( obj = get_obj_here( ch, NULL, argument ) ) != NULL )
	return obj;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( ( ch && !can_see_obj( ch, obj ) )
	|| !is_name( arg, obj->name ) )
	    continue;
	if ( ++count == number )
	    return obj;
    }

    return NULL;
}


/* deduct cost from a character */

void deduct_cost(CHAR_DATA *ch, int cost)
{
    int silver = 0, gold = 0;

    silver = UMIN(ch->silver,cost); 

    if (silver < cost)
    {
	gold = ((cost - silver + 99) / 100);
	silver = cost - 100 * gold;
    }

    ch->gold -= gold;
    ch->silver -= silver;

    if (ch->gold < 0)
    {
	bug("deduct costs: gold %d < 0",ch->gold);
	ch->gold = 0;
    }
    if (ch->silver < 0)
    {
	bug("deduct costs: silver %d < 0",ch->silver);
	ch->silver = 0;
    }
}   
/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int gold, int silver )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    if ( gold < 0 || silver < 0 || (gold == 0 && silver == 0) )
    {
	bug( "Create_money: zero or negative money.",UMIN(gold,silver));
	gold = UMAX(1,gold);
	silver = UMAX(1,silver);
    }

    if (gold == 0 && silver == 1)
    {
	obj = create_object( get_obj_index( OBJ_VNUM_SILVER_ONE ), 0 );
    }
    else if (gold == 1 && silver == 0)
    {
	obj = create_object( get_obj_index( OBJ_VNUM_GOLD_ONE), 0 );
    }
    else if (silver == 0)
    {
        obj = create_object( get_obj_index( OBJ_VNUM_GOLD_SOME ), 0 );
        xprintf( buf, obj->short_descr, gold );
        free_string( obj->short_descr );
        obj->short_descr        = str_dup( buf );
        obj->value[1]           = gold;
        obj->cost               = gold;
	obj->weight		= gold/5;
    }
    else if (gold == 0)
    {
        obj = create_object( get_obj_index( OBJ_VNUM_SILVER_SOME ), 0 );
        xprintf( buf, obj->short_descr, silver );
        free_string( obj->short_descr );
        obj->short_descr        = str_dup( buf );
        obj->value[0]           = silver;
        obj->cost               = silver;
	obj->weight		= silver/20;
    }
 
    else
    {
	obj = create_object( get_obj_index( OBJ_VNUM_COINS ), 0 );
	xprintf( buf, obj->short_descr, silver, gold );
	free_string( obj->short_descr );
	obj->short_descr	= str_dup( buf );
	obj->value[0]		= silver;
	obj->value[1]		= gold;
	obj->cost		= 100 * gold + silver;
	obj->weight		= gold / 5 + silver / 20;
    }

    return obj;
}


/*
 * Return # of objects which an object counts as.
 * Thanks to Tony Chamberlain for the correct recursive code here.
 */
int get_obj_number( OBJ_DATA *obj )
{
    int number;
/* 
    if (obj->item_type == ITEM_CONTAINER || obj->item_type == ITEM_MONEY
    ||  obj->item_type == ITEM_GEM || obj->item_type == ITEM_JEWELRY)
        number = 0;
*/
    if ( obj->item_type == ITEM_MONEY )
	number = 0;
    else
        number = 1;

/* 
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
        number += get_obj_number( obj );
*/ 
    return number;
}

int get_obj_realnumber( OBJ_DATA *obj )
{
    int number = 1;

    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
        number += get_obj_number( obj );

    return number;
}

/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
    int weight;
    OBJ_DATA *tobj;

    weight = obj->weight;
    for ( tobj = obj->contains; tobj != NULL; tobj = tobj->next_content )
	weight += get_obj_weight( tobj ) * WEIGHT_MULT(obj) / 100;

    return weight;
}

int get_true_weight(OBJ_DATA *obj)
{
    int weight;
 
    weight = obj->weight;
    for ( obj = obj->contains; obj != NULL; obj = obj->next_content )
        weight += get_obj_weight( obj );
 
    return weight;
}

/*
 * True if room is dark.
 */
bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex ,CHAR_DATA *ch)
{
/*    CHAR_DATA *cha;
    OBJ_DATA *obj; */

    if(IS_RAFFECTED(pRoomIndex,AFF_ROOM_NIGHTFALL))
	return TRUE;

/*    if (ch != NULL && IS_AFFECTED(ch,AFF_INFRARED))
	   return FALSE; */
	   
    if (ch != NULL && ch->race == race_lookup("drow"))
        return FALSE;
        
    if (ch != NULL && is_affected(ch,gsn_dark_vision))
	    return FALSE;

    if ( pRoomIndex->light > 0 )
	return FALSE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_DARK) )
	return TRUE;

    if ( pRoomIndex->sector_type == SECT_INSIDE
    ||   pRoomIndex->sector_type == SECT_CITY )
	return FALSE;

    if ( weather_info.sunlight == SUN_SET
    ||   weather_info.sunlight == SUN_DARK )
	return TRUE;

    return FALSE;
}


bool is_room_owner(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
{
    if (room->owner == NULL || room->owner[0] == '\0')
	return FALSE;

    return is_full_name(ch->name,room->owner);
}

/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA *rch;
    int count;


    if (pRoomIndex->owner != NULL && pRoomIndex->owner[0] != '\0')
	return TRUE;

    count = 0;
    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
	count++;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
	return TRUE;
    
    if ( IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) )
	return TRUE;

    return FALSE;
}

/* visibility on a room -- for entering and exits */
bool can_see_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) 
    &&  get_trust(ch) < MAX_LEVEL)
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_GODS_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_HEROES_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags,ROOM_NEWBIES_ONLY)
    &&  ch->level > 5 && !IS_IMMORTAL(ch))
	return FALSE;

/* hard code for Hall of Instruction */
    if (pRoomIndex->vnum == 3800
    &&  ch->level > 10 && !IS_IMMORTAL(ch))
	return FALSE;

    return TRUE;
}



/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{

int stat_victim = 0;
int stat_ch     = 0;

/* RT changed so that WIZ_INVIS has levels */
    if ( ch == victim )
	return TRUE;

    if ( ch == NULL || victim == NULL )
     {
	dump_to_scr( ">>>>>>>> CAN_ SEE ERROR <<<<<<<<<<<\n\r" );
	return FALSE;
     }
         
    if ( get_trust(ch) < victim->invis_level)
	return FALSE;

    if (get_trust(ch) < victim->incog_level && ch->in_room != victim->in_room)
	return FALSE;

/*
    if (((ch->level == 84) && victim->level < 83) && !IS_NPC(victim))
	return FALSE;
*/


    if ( (!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT)) 
    ||   (IS_NPC(ch) && IS_IMMORTAL(ch)))
//     if ( (ch->level >= (MAX_LEVEL - 14)) || !IS_NPC(victim))
//      {
 	return TRUE;
 //     }
  //   else
   //   {
    //   return FALSE;
    //  }
      
    if ( IS_AFFECTED(ch, AFF_BLIND) )
	return FALSE;

    if ( ch->in_room == NULL )
        return FALSE;

    if ( room_is_dark( ch->in_room ,ch) && !IS_AFFECTED(ch, AFF_INFRARED) )
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE)
    &&   !CAN_DETECT(ch, DETECT_INVIS) )
	return FALSE;
	
  /*  if ( IS_AFFECTED(victim, AFF_IMPROVED_INVISIBLE)
    &&   !CAN_DETECT(ch, DETECT_INVIS) )
	return FALSE;*/

    if (( is_affected(victim,gsn_duo_dimension)) 
        && (!is_affected(ch,gsn_duo_dimension))) 
	return FALSE;


    if ( is_affected(victim,gsn_spell_illusion_fields)) 
     {
      stat_ch     = ch->perm_stat[STAT_INT] + ch->perm_stat[STAT_WIS];
      stat_victim = victim->perm_stat[STAT_INT] + victim->perm_stat[STAT_WIS];

     if (stat_ch <= stat_victim)
      	return FALSE;
     }
        


    /* sneaking 
    if ( IS_AFFECTED(victim, AFF_SNEAK)
    &&   !CAN_DETECT(ch,DETECT_HIDDEN)
    &&   victim->fighting == NULL)
    {
	int chance;
	chance = get_skill(victim,gsn_sneak);
	chance += get_curr_stat(ch,STAT_DEX) * 3/2;
 	chance -= get_curr_stat(ch,STAT_INT) * 2;
	chance += ch->level - victim->level * 3/2;

	if (number_percent() < chance)
	    return FALSE;
    }
*/

    if (IS_AFFECTED(victim,AFF_CAMOUFLAGE) &&
        !CAN_DETECT(ch,ACUTE_VISION))
      return FALSE;

    if ( IS_AFFECTED(victim, AFF_HIDE)
    &&   !CAN_DETECT(ch, DETECT_HIDDEN)
    &&   victim->fighting == NULL)
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_FADE) && !IS_AFFECTED(ch, AFF_FADE)
    &&   !CAN_DETECT(ch, DETECT_FADE)
    &&   victim->fighting == NULL)
	return FALSE;

    return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    /*Immortals see ALL objects when use Holylight*/
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) )
	return TRUE;

    /*Don't Know the point of this look into it when you get time*/
    if ( IS_SET(obj->extra_flags,ITEM_VIS_DEATH))
	return FALSE;

    /*You get humming object if you blind and not deaf*/
    if ( IS_AFFECTED( ch, AFF_BLIND ) && (!IS_SET(obj->extra_flags, ITEM_HUM) || is_affected(ch, gsn_deafen)))
       return FALSE;
        
    /*You can see light*/
    if ( obj->item_type == ITEM_LIGHT && obj->value[2] != 0 )
	return TRUE;

    /*You can only see invis objcet if you affected by detect invis*/
    if ( IS_SET(obj->extra_flags, ITEM_INVIS)
    &&   !CAN_DETECT(ch, DETECT_INVIS) )
        return FALSE;


    /*You can only see hidden objcet if you affected by Hidden invis*/
    if ( IS_SET(obj->extra_flags, ITEM_HIDDEN)
    &&   !CAN_DETECT(ch, DETECT_HIDDEN) )
        return FALSE;

    /*You can see glowing if dark*/
    if ( IS_OBJ_STAT(obj,ITEM_GLOW))
	return TRUE;

    if ( room_is_dark( ch->in_room ,ch) && !IS_AFFECTED(ch, AFF_INFRARED) )
	return FALSE;

    return TRUE;
}



/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    if ( !IS_NPC(ch) && ch->level >= LEVEL_IMMORTAL )
	return TRUE;

    return FALSE;
}

bool can_gate( CHAR_DATA *ch, CHAR_DATA *victim )
{
	
if ( ( victim == NULL)
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
    ||   victim->level >= ch->level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)
    ||   (!IS_NPC(victim) && ch->in_room->area != victim->in_room->area ))
    return FALSE;
else
    return TRUE;
}



/*
 * Return ascii name of an item type.
 */
char *item_type_name( OBJ_DATA *obj )
{
    switch ( obj->item_type )
    {
    case ITEM_LIGHT:		return "light";
    case ITEM_SCROLL:		return "scroll";
    case ITEM_WAND:		return "wand";
    case ITEM_STAFF:		return "staff";
    case ITEM_WEAPON:		return "weapon";
    case ITEM_TREASURE:		return "treasure";
    case ITEM_ARMOR:		return "armor";
    case ITEM_CLOTHING:		return "clothing";
    case ITEM_POTION:		return "potion";
    case ITEM_FURNITURE:	return "furniture";
    case ITEM_TRASH:		return "trash";
    case ITEM_CONTAINER:	return "container";
    case ITEM_DRINK_CON:	return "drink container";
    case ITEM_KEY:		return "key";
    case ITEM_FOOD:		return "food";
    case ITEM_INGREDIENT:	return "ingredient";
    case ITEM_MONEY:		return "money";
    case ITEM_BOAT:		return "boat";
    case ITEM_CORPSE_NPC:	return "npc corpse";
    case ITEM_CORPSE_PC:	return "pc corpse";
    case ITEM_FOUNTAIN:		return "fountain";
    case ITEM_PILL:		return "pill";
    case ITEM_MAP:		return "map";
    case ITEM_PORTAL:		return "portal";
    case ITEM_WARP_STONE:	return "warp stone";
    case ITEM_GEM:		return "gem";
    case ITEM_JEWELRY:		return "jewelry";
    case ITEM_TATTOO:		return "tattoo";
    case ITEM_ARROW:		return "arrow";
    case ITEM_JUKEBOX:		return "juke box";
    case ITEM_TRAP:		return "trap";
    }

    bug( "Item_type_name: unknown type %d.", obj->item_type );
    return "(unknown)";
}



/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_NONE:		return "none";
    case APPLY_STR:		return "strength";
    case APPLY_DEX:		return "dexterity";
    case APPLY_INT:		return "intelligence";
    case APPLY_WIS:		return "wisdom";
    case APPLY_CON:		return "constitution";
    case APPLY_SEX:		return "sex";
    case APPLY_CLASS:		return "class";
    case APPLY_LEVEL:		return "level";
    case APPLY_AGE:		return "age";
    case APPLY_MANA:		return "mana";
    case APPLY_HIT:		return "hp";
    case APPLY_MOVE:		return "moves";
    case APPLY_GOLD:		return "gold";
    case APPLY_EXP:		return "experience";
    case APPLY_AC:		return "armor class";
    case APPLY_HITROLL:		return "hit roll";
    case APPLY_DAMROLL:		return "damage roll";
    case APPLY_SAVES:		return "saves";
    case APPLY_SAVING_ROD:	return "save vs rod";
    case APPLY_SAVING_PETRI:	return "save vs petrification";
    case APPLY_SAVING_BREATH:	return "save vs breath";
    case APPLY_SAVING_SPELL:	return "save vs spell";
    case APPLY_SPELL_AFFECT:	return "none";
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return "(unknown)";
}



/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) xprintf( buf, " blind"         );
    if ( vector & AFF_INVISIBLE     ) xprintf( buf, " invisible"     );
 //   if ( vector & AFF_IMPROVED_INVISIBLE     ) xprintf( buf, "improved_invisible"     );
    if ( vector & AFF_SCREAM        ) xprintf( buf, " scream"        );
    if ( vector & AFF_STUN          ) xprintf( buf, " stun"   );
    if ( vector & AFF_DETECT_FADE   ) xprintf( buf, " detect_fade"   );
    if ( vector & AFF_SANCTUARY     ) xprintf( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) xprintf( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) xprintf( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) xprintf( buf, " curse"         );
    if ( vector & AFF_POISON        ) xprintf( buf, " poison"        );
    if ( vector & AFF_PROTECT_EVIL  ) xprintf( buf, " prot_evil"     );
    if ( vector & AFF_SLEEP         ) xprintf( buf, " sleep"         );
    if ( vector & AFF_SNEAK         ) xprintf( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) xprintf( buf, " hide"          );
    if ( vector & AFF_FADE          ) xprintf( buf, " fade"          );
    if ( vector & AFF_CHARM         ) xprintf( buf, " charm"         );
    if ( vector & AFF_FLYING        ) xprintf( buf, " flying"        );
    if ( vector & AFF_PASS_DOOR     ) xprintf( buf, " pass_door"     );
    if ( vector & AFF_BERSERK	    ) xprintf( buf, " berserk"	    );
    if ( vector & AFF_CALM	    ) xprintf( buf, " calm"	    );
    if ( vector & AFF_HASTE	    ) xprintf( buf, " haste"	    );
    if ( vector & AFF_SLOW          ) xprintf( buf, " slow"          );
    if ( vector & AFF_PLAGUE	    ) xprintf( buf, " plague" 	    );
    if ( vector & AFF_FAERIE_FOG    ) xprintf( buf, " faerie_fog"    );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *detect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & DETECT_IMP_INVIS   ) xprintf( buf, " detect_imp_inv"   );
    if ( vector & DETECT_EVIL   ) xprintf( buf, " detect_evil"   );
    if ( vector & DETECT_GOOD   ) xprintf( buf, " detect_good"   );
    if ( vector & DETECT_INVIS  ) xprintf( buf, " detect_invis"  );
    if ( vector & DETECT_MAGIC  ) xprintf( buf, " detect_magic"  );
    if ( vector & DETECT_UNDEAD ) xprintf( buf, " detect_undead"  );
    if ( vector & DETECT_HIDDEN ) xprintf( buf, " detect_hidden" );
    if ( vector & DARK_VISION   ) xprintf( buf, " dark_vision"   );
    if ( vector & ACUTE_VISION  ) xprintf( buf, " acute_vision"   );
    if ( vector & ADET_FEAR     ) xprintf( buf, " fear"   );
    if ( vector & ADET_FORM_TREE  ) xprintf( buf, " form_tree"   );
    if ( vector & ADET_FORM_GRASS ) xprintf( buf, " form_grass"   );
    if ( vector & ADET_WEB      ) xprintf( buf, " web"   );
    if ( vector & DETECT_LIFE   ) xprintf( buf, " life"   );
    if ( vector & DETECT_SNEAK  ) xprintf( buf, " detect_sneak"   );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *affect2_bit_name( int vector )
{
    static char buf[512];
    buf[0] = '\0';
    /* Make this look just like the affect_bit_name section */
  if ( vector & AFF2_HERBS   ) xprintf( buf, "herbs"   );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( int extra_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) xprintf( buf, " glow"         );
    if ( extra_flags & ITEM_HUM          ) xprintf( buf, " hum"          );
    if ( extra_flags & ITEM_DARK         ) xprintf( buf, " dark"         );
    if ( extra_flags & ITEM_LOCK         ) xprintf( buf, " lock"         );
    if ( extra_flags & ITEM_EVIL         ) xprintf( buf, " evil"         );
/*  add hidden objs */
    if ( extra_flags & ITEM_HIDDEN       ) xprintf( buf, " hidden" );
    if ( extra_flags & ITEM_INVIS        ) xprintf( buf, " invis"        );
    if ( extra_flags & ITEM_MAGIC        ) xprintf( buf, " magic"        );
    if ( extra_flags & ITEM_NODROP       ) xprintf( buf, " nodrop"       );
    if ( extra_flags & ITEM_BLESS        ) xprintf( buf, " bless"        );
    if ( extra_flags & ITEM_DIMENSION    ) xprintf( buf, " dimension"    );
/*  adds traps */
    if ( extra_flags & ITEM_TRAP         ) xprintf( buf, " trap"         );
    if ( extra_flags & ITEM_ANTI_GOOD    ) xprintf( buf, " anti-good"    );
    if ( extra_flags & ITEM_ANTI_EVIL    ) xprintf( buf, " anti-evil"    );
    if ( extra_flags & ITEM_ANTI_NEUTRAL ) xprintf( buf, " anti-neutral" );
    if ( extra_flags & ITEM_NOREMOVE     ) xprintf( buf, " noremove"     );
    if ( extra_flags & ITEM_INVENTORY    ) xprintf( buf, " inventory"    );
    if ( extra_flags & ITEM_NOPURGE	 ) xprintf( buf, " nopurge"	);
    if ( extra_flags & ITEM_VIS_DEATH	 ) xprintf( buf, " vis_death"	);
    if ( extra_flags & ITEM_ROT_DEATH	 ) xprintf( buf, " rot_death"	);
    if ( extra_flags & ITEM_NOLOCATE	 ) xprintf( buf, " no_locate"	);
    if ( extra_flags & ITEM_SELL_EXTRACT ) xprintf( buf, " sell_extract" );
    if ( extra_flags & ITEM_BURN_PROOF	 ) xprintf( buf, " burn_proof"	);
    if ( extra_flags & ITEM_MOBGIVE	 ) xprintf( buf, " mob_give"	);
    if ( extra_flags & ITEM_NOUNCURSE	 ) xprintf( buf, " no_uncurse"	);
    if ( extra_flags & ITEM_NODONATE	 ) xprintf( buf, " no_donate"	);
    if ( extra_flags & ITEM_INDESTRUCT	 ) xprintf( buf, " indestruct"	);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

/*
 * Return ascii name of class flags vector.
 * will make it easy for class prevention
 * coded by Takeda 2/26/00
 */
char *class_bit_name( int class_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( class_flags & CLASS_WARRIOR         ) xprintf( buf, "%s warrior",buf);
    if ( class_flags & CLASS_MERCENARY         ) xprintf( buf, "%s mercenary",buf);
    if ( class_flags & CLASS_PALADIN         ) xprintf( buf, "%s paladin",buf);
    if ( class_flags & CLASS_BLACK_KNIGHT   ) xprintf( buf, "%s black_knight",buf);
    if ( class_flags & CLASS_THIEF         ) xprintf( buf, "%s thief",buf);
    if ( class_flags & CLASS_INQUISITOR         ) xprintf( buf, "%s inquisitor",buf);
    if ( class_flags & CLASS_SPELLFILCHER   ) xprintf( buf, "%s spellfilcher",buf);
    if ( class_flags & CLASS_CLERIC         ) xprintf( buf, "%s cleric",buf);
    if ( class_flags & CLASS_PRODIGY         ) xprintf( buf, "%s prodigy",buf);
    if ( class_flags & CLASS_MAGE         ) xprintf( buf, "%s mage",buf);
    if ( class_flags & CLASS_RANGER         ) xprintf( buf, "%s ranger",buf);
    if ( class_flags & CLASS_ASSASSIN         ) xprintf( buf, "%s assassin",buf);
    if ( class_flags & CLASS_BARD         ) xprintf( buf, "%s bard",buf);
    if ( class_flags & CLASS_MALIGNER         ) xprintf( buf, "%s maligner",buf);
    if ( class_flags & CLASS_ALCHEMIST         ) xprintf( buf, "%s alchemist",buf);
    if ( class_flags & CLASS_DRUID         ) xprintf( buf, "%s druid",buf);
    if ( class_flags & CLASS_MONK         ) xprintf( buf, "%s monk",buf);
    if ( class_flags & CLASS_NECROMANCER       ) xprintf( buf, "%s necromancer",buf);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

/*
 * Return ascii name of race flags vector.
 * will make it easy for race prevention
 * coded by Takeda 2/26/00
 */
char *race_bit_name( int race_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( race_flags & RACE_HUMAN         ) xprintf( buf, "%s human",buf);
    if ( race_flags & RACE_ELF         ) xprintf( buf, "%s elf",buf);
    if ( race_flags & RACE_DROW         ) xprintf( buf, "%s drow",buf);
    if ( race_flags & RACE_DWARF         ) xprintf( buf, "%s dwarf",buf);
    if ( race_flags & RACE_DUERGAR         ) xprintf( buf, "%s duergar",buf);
    if ( race_flags & RACE_PIXIE       ) xprintf( buf, "%s pixie",buf);
    if ( race_flags & RACE_GARGOYLE         ) xprintf( buf, "%s gargoyle",buf);
    if ( race_flags & RACE_DRACONIAN         ) xprintf( buf, "%s draconian",buf);
    if ( race_flags & RACE_ARIAL         ) xprintf( buf, "%s arial",buf);
    if ( race_flags & RACE_FELAR         ) xprintf( buf, "%s felar",buf);
    if ( race_flags & RACE_MERMAN         ) xprintf( buf, "%s merman",buf);
    if ( race_flags & RACE_GIANT         ) xprintf( buf, "%s giant",buf);
    if ( race_flags & RACE_TROLL         ) xprintf( buf, "%s troll",buf);
    if ( race_flags & RACE_OGRE         ) xprintf( buf, "%s ogre",buf);
    if ( race_flags & RACE_ORC         ) xprintf( buf, "%s orc",buf);
    if ( race_flags & RACE_DARNOK         ) xprintf( buf, "%s darnok",buf);
    if ( race_flags & RACE_ILLITHID         ) xprintf( buf, "%s illithid",buf);
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}
  

/* return ascii name of an act vector */
char *act_bit_name( int act_flags )
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET(act_flags,ACT_IS_NPC))
    { 
 	xprintf(buf," npc");
    	if (act_flags & ACT_SENTINEL 	) xprintf(buf, " sentinel");
    	if (act_flags & ACT_SCAVENGER	) xprintf(buf, " scavenger");
	if (act_flags & ACT_AGGRESSIVE	) xprintf(buf, " aggressive");
	if (act_flags & ACT_STAY_AREA	) xprintf(buf, " stay_area");
	if (act_flags & ACT_WIMPY	) xprintf(buf, " wimpy");
	if (act_flags & ACT_PET		) xprintf(buf, " pet");
	if (act_flags & ACT_TRAIN	) xprintf(buf, " train");
	if (act_flags & ACT_PRACTICE	) xprintf(buf, " practice");
	if (act_flags & ACT_UNDEAD	) xprintf(buf, " undead");
	if (act_flags & ACT_CLERIC	) xprintf(buf, " cleric");
	if (act_flags & ACT_MAGE	) xprintf(buf, " mage");
	if (act_flags & ACT_THIEF	) xprintf(buf, " thief");
	if (act_flags & ACT_WARRIOR	) xprintf(buf, " warrior");
	if (act_flags & ACT_NOALIGN	) xprintf(buf, " no_align");
	if (act_flags & ACT_NOPURGE	) xprintf(buf, " no_purge");
	if (act_flags & ACT_IS_HEALER	) xprintf(buf, " healer");
	if (act_flags & ACT_IS_CHANGER  ) xprintf(buf, " changer");
	if (act_flags & ACT_MOUNTABLE	) xprintf(buf, " can_mount");
	if (act_flags & ACT_NOTRACK     ) xprintf(buf," notrack");
	if (act_flags & ACT_OUTDOORS) xprintf(buf," outdoors");
	if (act_flags & ACT_INDOORS) xprintf(buf," indoors");
    }
    else
    {
	xprintf(buf," player");
/*  line add for lope */
 	if (act_flags & PLR_COLOR       ) xprintf(buf, " color");
	if (act_flags & PLR_AUTOASSIST	) xprintf(buf, " autoassist");
	if (act_flags & PLR_AUTOEXIT	) xprintf(buf, " autoexit");
	if (act_flags & PLR_AUTOLOOT	) xprintf(buf, " autoloot");
	if (act_flags & PLR_AUTOSAC	) xprintf(buf, " autosac");
	if (act_flags & PLR_AUTOGOLD	) xprintf(buf, " autogold");
	if (act_flags & PLR_AUTOSPLIT	) xprintf(buf, " autosplit");
	if (act_flags & PLR_HOLYLIGHT	) xprintf(buf, " holy_light");
	if (act_flags & PLR_CANLOOT	) xprintf(buf, " loot_corpse");
	if (act_flags & PLR_NOSUMMON	) xprintf(buf, " no_summon");
	if (act_flags & PLR_NOFOLLOW	) xprintf(buf, " no_follow");
	if (act_flags & PLR_FREEZE	) xprintf(buf, " frozen");
	if (act_flags & PLR_THIEF	) xprintf(buf, " THIEF");
	if (act_flags & PLR_KILLER	) xprintf(buf, " KILLER");
	if (act_flags & PLR_WANTED	) xprintf(buf, " WANTED");
	if (act_flags & PLR_GHOST	) xprintf(buf, " GHOST");
	if (act_flags & PLR_ARENA	) xprintf(buf, " arena");
	if (act_flags & PLR_AUTODAMAGE	) xprintf(buf, " autodamage");
    }
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *comm_bit_name(int comm_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (comm_flags & COMM_QUIET		) xprintf(buf, " quiet");
    if (comm_flags & COMM_DEAF		) xprintf(buf, " deaf");
    if (comm_flags & COMM_NOWIZ		) xprintf(buf, " no_wiz");
    if (comm_flags & COMM_NOAUCTION	) xprintf(buf, " no_auction");
    if (comm_flags & COMM_NOGOSSIP	) xprintf(buf, " no_gossip");
    if (comm_flags & COMM_NOQUESTION	) xprintf(buf, " no_ooc");
    if (comm_flags & COMM_NOMUSIC	) xprintf(buf, " no_music");
    if (comm_flags & COMM_NOQUOTE	) xprintf(buf, " no_quote");
    if (comm_flags & COMM_COMPACT	) xprintf(buf, " compact");
    if (comm_flags & COMM_BRIEF		) xprintf(buf, " brief");
    if (comm_flags & COMM_PROMPT	) xprintf(buf, " prompt");
    if (comm_flags & COMM_COMBINE	) xprintf(buf, " combine");
    if (comm_flags & COMM_NOEMOTE	) xprintf(buf, " no_emote");
    if (comm_flags & COMM_NOSHOUT	) xprintf(buf, " no_shout");
    if (comm_flags & COMM_NOTELL	) xprintf(buf, " no_tell");
    if (comm_flags & COMM_NOCHANNELS	) xprintf(buf, " no_channels");
    if (comm_flags & COMM_NONOTE	) xprintf(buf, " no_note");

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *imm_bit_name(int imm_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (imm_flags & IMM_SUMMON		) xprintf(buf, " summon");
    if (imm_flags & IMM_CHARM		) xprintf(buf, " charm");
    if (imm_flags & IMM_MAGIC		) xprintf(buf, " magic");
    if (imm_flags & IMM_WEAPON		) xprintf(buf, " weapon");
    if (imm_flags & IMM_BASH		) xprintf(buf, " blunt");
    if (imm_flags & IMM_PIERCE		) xprintf(buf, " piercing");
    if (imm_flags & IMM_SLASH		) xprintf(buf, " slashing");
    if (imm_flags & IMM_FIRE		) xprintf(buf, " fire");
    if (imm_flags & IMM_COLD		) xprintf(buf, " cold");
    if (imm_flags & IMM_LIGHTNING	) xprintf(buf, " lightning");
    if (imm_flags & IMM_ACID		) xprintf(buf, " acid");
    if (imm_flags & IMM_POISON		) xprintf(buf, " poison");
    if (imm_flags & IMM_NEGATIVE	) xprintf(buf, " negative");
    if (imm_flags & IMM_HOLY		) xprintf(buf, " holy");
    if (imm_flags & IMM_ENERGY		) xprintf(buf, " energy");
    if (imm_flags & IMM_MENTAL		) xprintf(buf, " mental");
    if (imm_flags & IMM_DISEASE	) xprintf(buf, " disease");
    if (imm_flags & IMM_DROWNING	) xprintf(buf, " drowning");
    if (imm_flags & IMM_LIGHT		) xprintf(buf, " light");
    if (imm_flags & VULN_IRON		) xprintf(buf, " iron");
    if (imm_flags & VULN_WOOD		) xprintf(buf, " wood");
    if (imm_flags & VULN_SILVER	) xprintf(buf, " silver");

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *wear_bit_name(int wear_flags)
{
    static char buf[512];

    buf [0] = '\0';
    if (wear_flags & ITEM_TAKE		) xprintf(buf, " take");
    if (wear_flags & ITEM_WEAR_FINGER	) xprintf(buf, " finger");
    if (wear_flags & ITEM_WEAR_NECK	) xprintf(buf, " neck");
    if (wear_flags & ITEM_WEAR_BODY	) xprintf(buf, " torso");
    if (wear_flags & ITEM_WEAR_HEAD	) xprintf(buf, " head");
    if (wear_flags & ITEM_WEAR_LEGS	) xprintf(buf, " legs");
    if (wear_flags & ITEM_WEAR_FEET	) xprintf(buf, " feet");
    if (wear_flags & ITEM_WEAR_HANDS	) xprintf(buf, " hands");
    if (wear_flags & ITEM_WEAR_ARMS	) xprintf(buf, " arms");
    if (wear_flags & ITEM_WEAR_SHIELD	) xprintf(buf, " shield");
    if (wear_flags & ITEM_WEAR_ABOUT	) xprintf(buf, " body");
    if (wear_flags & ITEM_WEAR_WAIST	) xprintf(buf, " waist");
    if (wear_flags & ITEM_WEAR_WRIST	) xprintf(buf, " wrist");
    if (wear_flags & ITEM_WIELD		) xprintf(buf, " wield");
    if (wear_flags & ITEM_HOLD		) xprintf(buf, " hold");
    if (wear_flags & ITEM_WEAR_FLOAT	) xprintf(buf, " float");
    if (wear_flags & ITEM_WEAR_ORNAMENT	) xprintf(buf, " ornament");

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *form_bit_name(int form_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (form_flags & FORM_POISON	) xprintf(buf, " poison");
    else if (form_flags & FORM_EDIBLE	) xprintf(buf, " edible");
    if (form_flags & FORM_MAGICAL	) xprintf(buf, " magical");
    if (form_flags & FORM_INSTANT_DECAY	) xprintf(buf, " instant_rot");
    if (form_flags & FORM_OTHER		) xprintf(buf, " other");
    if (form_flags & FORM_ANIMAL	) xprintf(buf, " animal");
    if (form_flags & FORM_SENTIENT	) xprintf(buf, " sentient");
    if (form_flags & FORM_UNDEAD	) xprintf(buf, " undead");
    if (form_flags & FORM_CONSTRUCT	) xprintf(buf, " construct");
    if (form_flags & FORM_MIST		) xprintf(buf, " mist");
    if (form_flags & FORM_INTANGIBLE	) xprintf(buf, " intangible");
    if (form_flags & FORM_BIPED		) xprintf(buf, " biped");
    if (form_flags & FORM_CENTAUR	) xprintf(buf, " centaur");
    if (form_flags & FORM_INSECT	) xprintf(buf, " insect");
    if (form_flags & FORM_SPIDER	) xprintf(buf, " spider");
    if (form_flags & FORM_CRUSTACEAN	) xprintf(buf, " crustacean");
    if (form_flags & FORM_WORM		) xprintf(buf, " worm");
    if (form_flags & FORM_BLOB		) xprintf(buf, " blob");
    if (form_flags & FORM_MAMMAL	) xprintf(buf, " mammal");
    if (form_flags & FORM_BIRD		) xprintf(buf, " bird");
    if (form_flags & FORM_REPTILE	) xprintf(buf, " reptile");
    if (form_flags & FORM_SNAKE		) xprintf(buf, " snake");
    if (form_flags & FORM_DRAGON	) xprintf(buf, " dragon");
    if (form_flags & FORM_AMPHIBIAN	) xprintf(buf, " amphibian");
    if (form_flags & FORM_FISH		) xprintf(buf, " fish");
    if (form_flags & FORM_COLD_BLOOD 	) xprintf(buf, " cold_blooded");

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *part_bit_name(int part_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (part_flags & PART_HEAD		) xprintf(buf, " head");
    if (part_flags & PART_ARMS		) xprintf(buf, " arms");
    if (part_flags & PART_LEGS		) xprintf(buf, " legs");
    if (part_flags & PART_HEART		) xprintf(buf, " heart");
    if (part_flags & PART_BRAINS	) xprintf(buf, " brains");
    if (part_flags & PART_GUTS		) xprintf(buf, " guts");
    if (part_flags & PART_HANDS		) xprintf(buf, " hands");
    if (part_flags & PART_FEET		) xprintf(buf, " feet");
    if (part_flags & PART_FINGERS	) xprintf(buf, " fingers");
    if (part_flags & PART_EAR		) xprintf(buf, " ears");
    if (part_flags & PART_EYE		) xprintf(buf, " eyes");
    if (part_flags & PART_LONG_TONGUE	) xprintf(buf, " long_tongue");
    if (part_flags & PART_EYESTALKS	) xprintf(buf, " eyestalks");
    if (part_flags & PART_TENTACLES	) xprintf(buf, " tentacles");
    if (part_flags & PART_FINS		) xprintf(buf, " fins");
    if (part_flags & PART_WINGS		) xprintf(buf, " wings");
    if (part_flags & PART_TAIL		) xprintf(buf, " tail");
    if (part_flags & PART_CLAWS		) xprintf(buf, " claws");
    if (part_flags & PART_FANGS		) xprintf(buf, " fangs");
    if (part_flags & PART_HORNS		) xprintf(buf, " horns");
    if (part_flags & PART_SCALES	) xprintf(buf, " scales");

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *weapon_bit_name(int weapon_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (weapon_flags & WEAPON_FLAMING	) xprintf(buf, " flaming");
    if (weapon_flags & WEAPON_FROST	) xprintf(buf, " frost");
    if (weapon_flags & WEAPON_VAMPIRIC	) xprintf(buf, " vampiric");
    if (weapon_flags & WEAPON_SHARP	) xprintf(buf, " sharp");
    if (weapon_flags & WEAPON_VORPAL	) xprintf(buf, " vorpal");
    if (weapon_flags & WEAPON_TWO_HANDS ) xprintf(buf, " two-handed");
    if (weapon_flags & WEAPON_SHOCKING 	) xprintf(buf, " shocking");
    if (weapon_flags & WEAPON_POISON	) xprintf(buf, " poison");
    if (weapon_flags & WEAPON_MANADRAIN	) xprintf(buf, " manadrain");

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

char *cont_bit_name( int cont_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (cont_flags & CONT_CLOSEABLE	) xprintf(buf, " closable");
    if (cont_flags & CONT_PICKPROOF	) xprintf(buf, " pickproof");
    if (cont_flags & CONT_CLOSED	) xprintf(buf, " closed");
    if (cont_flags & CONT_LOCKED	) xprintf(buf, " locked");

    return (buf[0] != '\0' ) ? buf+1 : "none";
}


char *off_bit_name(int off_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (off_flags & OFF_AREA_ATTACK	) xprintf(buf, " area attack");
    if (off_flags & OFF_BACKSTAB	) xprintf(buf, " backstab");
    if (off_flags & OFF_BASH		) xprintf(buf, " bash");
    if (off_flags & OFF_BERSERK		) xprintf(buf, " berserk");
    if (off_flags & OFF_DISARM		) xprintf(buf, " disarm");
    if (off_flags & OFF_DODGE		) xprintf(buf, " dodge");
    if (off_flags & OFF_FADE		) xprintf(buf, " fade");
    if (off_flags & OFF_FAST		) xprintf(buf, " fast");
    if (off_flags & OFF_KICK		) xprintf(buf, " kick");
    if (off_flags & OFF_KICK_DIRT	) xprintf(buf, " kick_dirt");
    if (off_flags & OFF_PARRY		) xprintf(buf, " parry");
    if (off_flags & OFF_RESCUE		) xprintf(buf, " rescue");
    if (off_flags & OFF_TAIL		) xprintf(buf, " tail");
    if (off_flags & OFF_TRIP		) xprintf(buf, " trip");
    if (off_flags & OFF_CRUSH		) xprintf(buf, " crush");
    if (off_flags & ASSIST_ALL		) xprintf(buf, " assist_all");
    if (off_flags & ASSIST_ALIGN	) xprintf(buf, " assist_align");
    if (off_flags & ASSIST_RACE		) xprintf(buf, " assist_race");
    if (off_flags & ASSIST_PLAYERS	) xprintf(buf, " assist_players");
    if (off_flags & ASSIST_GUARD	) xprintf(buf, " assist_guard");
    if (off_flags & ASSIST_VNUM		) xprintf(buf, " assist_vnum");
    if (off_flags & ASSIST_CLAN		) xprintf(buf, " assist_clan");
    if (off_flags & OFF_XATTACK		) xprintf(buf, " xattack");
    if (off_flags & OFF_XATTACK2	) xprintf(buf, " xattack2");
    if (off_flags & OFF_XATTACK3	) xprintf(buf, " xattack3");
    if (off_flags & OFF_XATTACK4	) xprintf(buf, " xattack4");
    if (off_flags & OFF_XATTACK5	) xprintf(buf, " xattack5");
    if (off_flags & OFF_XATTACK6	) xprintf(buf, " xattack6");

    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

void    affect_modify_room      args( ( ROOM_INDEX_DATA *room, AFFECT_DATA
*paf, bool fAdd ) );

/*
 * Apply or remove an affect to a room.
 */
void affect_modify_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf, bool fAdd )
{
    int mod;

    mod = paf->modifier;

    if ( fAdd )
    {

     switch (paf->where)
        {
        case TO_ROOM_AFFECTS:
              SET_BIT(room->affected_by, paf->bitvector);
            break;
        case TO_ROOM_FLAGS:
              SET_BIT(room->room_flags, paf->bitvector);
            break;
        case TO_ROOM_CONST:
            break;
        }
    }
    else
    {
        switch (paf->where)
        {
        case TO_ROOM_AFFECTS:
           REMOVE_BIT(room->affected_by, paf->bitvector);
            break;
        case TO_ROOM_FLAGS:
              REMOVE_BIT(room->room_flags, paf->bitvector);
            break;
        case TO_ROOM_CONST:
            break;
        }
        mod = 0 - mod;
    }

    switch ( paf->location )
    {
    default:
        bug( "Affect_modify_room: unknown location %d.", paf->location );
        return;

    case APPLY_ROOM_NONE:                                       break;
    case APPLY_ROOM_HEAL:       room->heal_rate += mod;         break;
    case APPLY_ROOM_MANA:       room->mana_rate += mod;         break;
    }

    return;
}

/*
 * Give an affect to a room.
 */
void affect_to_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;
    ROOM_INDEX_DATA *pRoomIndex;

    if ( ! room->affected )
  {
     if ( top_affected_room )
     {
      for ( pRoomIndex  = top_affected_room;
          pRoomIndex->aff_next != NULL;
          pRoomIndex  = pRoomIndex->aff_next )
                        continue;
      pRoomIndex->aff_next = room;
     }
     else top_affected_room = room;
     room->aff_next = NULL;
    }

    paf_new = new_affect();

    *paf_new            = *paf;
    paf_new->next       = room->affected;
  room->affected      = paf_new;

    affect_modify_room( room , paf_new, TRUE );
    return;
}

void affect_check_room(ROOM_INDEX_DATA *room,int where,int vector)
{
    AFFECT_DATA *paf;

    if (vector == 0)
        return;

    for (paf = room->affected; paf != NULL; paf = paf->next)
        if (paf->where == where && paf->bitvector == vector)
        {
            switch (where)
        {
                case TO_ROOM_AFFECTS:
                      SET_BIT(room->affected_by,vector);
                    break;
                case TO_ROOM_FLAGS:
                      SET_BIT(room->room_flags, vector);
                    break;
                case TO_ROOM_CONST:
                    break;
            }
            return;
        }
}

/*
 * Remove an affect from a room.
 */
void affect_remove_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    int where;
    int vector;


    if ( room->affected == NULL )
    {
        bug( "Affect_remove_room: no affect.", 0 );
        return;
    }

    affect_modify_room( room, paf, FALSE );
    where = paf->where;
    vector = paf->bitvector;

    if ( paf == room->affected )
  {
        room->affected  = paf->next;
    }
    else
    {
        AFFECT_DATA *prev;

        for ( prev = room->affected; prev != NULL; prev = prev->next )
        {
            if ( prev->next == paf )
            {
                prev->next = paf->next;
                break;
            }
        }

        if ( prev == NULL )
      {
            bug( "Affect_remove_room: cannot find paf.", 0 );
            return;
        }
    }

    if ( !room->affected )
    {
     ROOM_INDEX_DATA *prev;

     if (top_affected_room  == room)
        {
         top_affected_room = room->aff_next;
        }
     else
        {
         for(prev = top_affected_room; prev->aff_next; prev = prev->aff_next )
          {
            if ( prev->aff_next == room )
            {
                prev->aff_next = room->aff_next;
                break;
            }
          }
         if ( prev == NULL )
          {
            bug( "Affect_remove_room: cannot find room.", 0 );
            return;
          }
        }
      room->aff_next = NULL;

     }

  free_affect(paf);

    affect_check_room(room,where,vector);
    return;
}

/*
 * Strip all affects of a given sn.
 */
void affect_strip_room( ROOM_INDEX_DATA *room, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = room->affected; paf != NULL; paf = paf_next )
    {
      paf_next = paf->next;
        if ( paf->type == sn )
            affect_remove_room( room, paf );
    }

    return;
}



/*
 * Return true if a room is affected by a spell.
 */
bool is_affected_room( ROOM_INDEX_DATA *room, int sn )
{
    AFFECT_DATA *paf;
   for ( paf = room->affected; paf != NULL; paf = paf->next )
    {
        if ( paf->type == sn )
            return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;
  bool found;

    found = FALSE;
    for ( paf_old = room->affected; paf_old != NULL; paf_old = paf_old->next )
    {
        if ( paf_old->type == paf->type )
        {
            paf->level = (paf->level += paf_old->level) / 2;
            paf->duration += paf_old->duration;
            paf->modifier += paf_old->modifier;
            affect_remove_room( room, paf_old );
            break;
        }
    }

    affect_to_room( room, paf );
    return;
}

/*
 * Return ascii name of an raffect location.
 */
char *raffect_loc_name( int location )
{
    switch ( location )
    {
    case APPLY_ROOM_NONE:       return "none";
    case APPLY_ROOM_HEAL:       return "heal rate";
    case APPLY_ROOM_MANA:       return "mana rate";
    }

    bug( "Affect_location_name: unknown location %d.", location );
    return "(unknown)";
}


/*
 * Return ascii name of an affect bit vector.
 */
char *raffect_bit_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_ROOM_SHOCKING  ) xprintf( buf, " shocking"        );
    if ( vector & AFF_ROOM_L_SHIELD  ) xprintf( buf, " lightning_shield");
    if ( vector & AFF_ROOM_THIEF_TRAP) xprintf( buf, " thief_trap"      );
    if ( vector & AFF_ROOM_CURSE     ) xprintf( buf, " curse"           );
    if ( vector & AFF_ROOM_POISON    ) xprintf( buf, " poison"          );
    if ( vector & AFF_ROOM_PLAGUE    ) xprintf( buf, " plague"          );
    if ( vector & AFF_ROOM_SLEEP     ) xprintf( buf, " sleep"           );
    if ( vector & AFF_ROOM_SLOW      ) xprintf( buf, " slow"            );
//    if ( vector & AFF_ROOM_FEAR      ) xprintf( buf, " fear"            );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

bool is_safe_rspell_nom(int level, CHAR_DATA *victim )
{
  if (IS_SET(victim->in_room->room_flags,ROOM_ARENA))
	return FALSE;

  if  ( victim->level < 7  && !IS_NPC(victim) )
    return TRUE;

        if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
        return TRUE;

  if ( !IS_NPC(victim) &&
      ((level >= victim->level + 7) || (victim->level >= level + 7)))
    return TRUE;

  return FALSE;
}


bool is_safe_rspell(int level, CHAR_DATA *victim)
{
  if (is_safe_rspell_nom(level,victim))
    {
      act("The gods protect $n.",victim,NULL,NULL,TO_CHAR);
      act("The gods protect $n from the spell of room.",victim,NULL,NULL,TO_ROOM);
      return TRUE;
    }
  else return FALSE;
}

void raffect_to_char( ROOM_INDEX_DATA *room, CHAR_DATA *ch)
 {
  AFFECT_DATA *paf;

  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_L_SHIELD) )
  {
     int sn;
     CHAR_DATA *vch;

     if ( (sn = skill_lookup("lightning shield")) == -1 )
        { bug("Bad sn for lightning shield",0); return; }

     for (vch=room->people;vch;vch=vch->next_in_room)
        {
         if (is_room_owner(vch,room)) break;
        }

    if ( !vch )
        {
         bug("Owner of lightning shield left the room.",0);
         free_string(room->owner);
         room->owner = str_dup("");
         affect_strip_room( room,sn);
        }
     else
     {
      send_to_char("The protective shield of room blocks you.\n\r",ch);
      act("$N has entered the room.",vch,NULL,ch,TO_CHAR);
      do_wake(vch,"");

      if ( (paf = affect_find(room->affected,sn)) == NULL )
         { bug("Bad paf for lightning shield",0); return; }

      if (!is_safe_rspell(paf->level,ch))
        {
         damage( vch,ch,dice(paf->level,4)+12,sn,DAM_LIGHTNING, TRUE);
         free_string(room->owner);
         room->owner = str_dup("");
         affect_remove_room( room , paf);
        }
     }
   }

  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_SHOCKING) )
  {
     int sn;

     if ( (sn = skill_lookup("shocking trap")) == -1 )
        { bug("Bad sn for shocking shield",0); return; }

     send_to_char("The shocking waves of room shocks you.\n\r",ch);

     if ( (paf = affect_find(room->affected,sn)) == NULL )
         { bug("Bad paf for shocking shield",0); return; }
     if (!is_safe_rspell(paf->level,ch))
        {
         if ( check_immune(ch, DAM_LIGHTNING) != IS_IMMUNE )
         damage(ch,ch,dice(paf->level,4)+12,TYPE_HUNGER,DAM_TRAP_ROOM,TRUE);
         affect_remove_room( room , paf);
        }
   }

  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_THIEF_TRAP) )
  {
     send_to_char("The trap ,set by someone, blocks you.\n\r",ch);

     if ( (paf = affect_find(room->affected,gsn_settraps)) == NULL )
       { bug("Bad paf for settraps",0); return; }

     if (!is_safe_rspell(paf->level,ch))
        {
         if ( check_immune(ch, DAM_PIERCE) != IS_IMMUNE )
         damage(ch,ch,dice(paf->level,5)+12,TYPE_HUNGER,DAM_TRAP_ROOM,TRUE);
         affect_remove_room( room , paf);
        }
   }
  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_SLOW)
        || IS_ROOM_AFFECTED( room, AFF_ROOM_SLEEP) )
     send_to_char("There is some mist flowing in the air.\n\r",ch);

  return;
 }

void raffect_back_char( ROOM_INDEX_DATA *room, CHAR_DATA *ch)
 {
  if ( IS_ROOM_AFFECTED( room, AFF_ROOM_L_SHIELD) )
  {
   int sn;

    if ( (sn = skill_lookup("lightning shield")) == -1 )
        { bug("Bad sn for lightning shield",0); return; }
    if (is_room_owner(ch,room))
        {
         free_string(room->owner);
         room->owner = str_dup("");
         affect_strip_room( room,sn);
        }
   }
  return;
 }

/*
 * Return ascii name of an affect bit vector.
 */
char *flag_room_name( int vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & ROOM_DARK         ) xprintf( buf, " dark"         );
    if ( vector & ROOM_NO_MOB        ) xprintf( buf, " nomob"        );
    if ( vector & ROOM_INDOORS      ) xprintf( buf, " indoors"      );
    if ( vector & ROOM_PRIVATE      ) xprintf( buf, " private"      );
    if ( vector & ROOM_SAFE         ) xprintf( buf, " safe"         );
    if ( vector & ROOM_SOLITARY     ) xprintf( buf, " solitary"     );
    if ( vector & ROOM_PET_SHOP     ) xprintf( buf, " petshop"      );
    if ( vector & ROOM_NO_RECALL    ) xprintf( buf, " norecall"     );
    if ( vector & ROOM_IMP_ONLY     ) xprintf( buf, " imp_only"     );
    if ( vector & ROOM_GODS_ONLY    ) xprintf( buf, " god_only"     );
    if ( vector & ROOM_HEROES_ONLY  ) xprintf( buf, " heroes"       );
    if ( vector & ROOM_NEWBIES_ONLY ) xprintf( buf, " newbies"      );
    if ( vector & ROOM_LAW          ) xprintf( buf, " law"          );
    if ( vector & ROOM_NOWHERE      ) xprintf( buf, " nowhere"      );
    if ( vector & ROOM_BANK         ) xprintf( buf, " bank"         );
    if ( vector & ROOM_NO_MAGIC     ) xprintf( buf, " nomagic"      );
    if ( vector & ROOM_NOSUMMON     ) xprintf( buf, " nosummon"     );
    if ( vector & ROOM_REGISTRY     ) xprintf( buf, " registry"     );
    return ( buf[0] != '\0' ) ? buf+1 : "none";
}

void path_to_track( CHAR_DATA *ch, CHAR_DATA *victim, int door)
{
  ROOM_INDEX_DATA *temp;
  EXIT_DATA *pExit;
  int opdoor;
  int range = 0;

  ch->last_fight_time = current_time;
  if (!IS_NPC(victim)) victim->last_fight_time = current_time;

  if (IS_NPC(victim) && victim->position != POS_DEAD)
   {
    victim->last_fought = ch;

    if ( (opdoor = opposite_door( door )) == -1)
	{
	 bug("In path_to_track wrong door: %d",door);
	 return;
	}
    temp = ch->in_room;
    while (1)
     {
      range++;
      if ( victim->in_room == temp ) break;
      if ((pExit = temp->exit[ door ]) == NULL
	  || (temp = pExit->u1.to_room) == NULL)
       {
	bug("In path_to_track: couldn't calculate range %d",range);
	return;
       }
      if ( range > 100)
       {
	bug("In path_to_track: range exceeded 100",0);
	return;
       }
     }

    temp = victim->in_room;
    while (--range > 0)
       {
        room_record(ch->name,temp, opdoor);
        if ((pExit = temp->exit[opdoor]) == NULL
	    || (temp = pExit->u1.to_room) == NULL )
	{
	 xprintf_2(log_buf,"Path to track: Range: %d Room: %d opdoor:%d",
		range,temp->vnum,opdoor); 
	 bug(log_buf,0);
	 return;
	}
       }
    do_track(victim,"");
  }
 return;
}

int opposite_door(int door)
{
  int opdoor;

  switch ( door )
   {
    case 0: opdoor=2;   break;
    case 1: opdoor=3;   break;
    case 2: opdoor=0;   break;
    case 3: opdoor=1;   break;
    case 4: opdoor=5;   break;
    case 5: opdoor=4;   break;
    default: opdoor=-1; break;
  }
  return opdoor;
}

void getpidmud(void)
{

//	pid_t	pid = -1;
//	FILE	*fp = NULL;
//	char buf[MAX_STRING_LENGTH];

//	if ( (fp = fopen("mud.pid", "w")) == NULL)
//	{
//		printf("Unable to open output file.\n");
		return;
//	}
	
//	pid = getpid();
//	xprintf(buf, "kill -9 %d", pid);
//	fwrite(buf, sizeof(char), strlen(buf), fp);
	
//	fclose(fp);
//	return;

}


bool *is_group_mobile( CHAR_DATA *ch, CHAR_DATA *friend )
{

	if (friend == NULL)
		return FALSE;
		
	if (ch == NULL)
		return FALSE;


	if (ch->pIndexData->group == friend->pIndexData->group)
		return TRUE;
	
return FALSE;
}

CHAR_DATA *find_group_tank( CHAR_DATA *ch, int grouptype )
{
 CHAR_DATA *victim = ch->fighting;
 CHAR_DATA *friend = NULL;

	if (victim == NULL)
		return NULL;
	
	friend = ch->fighting;
	if (friend == NULL && (ch->pIndexData->group == friend->pIndexData->group))
		return friend;
}

bool fight_all_room(CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
CHAR_DATA *pChar;
CHAR_DATA *pChar_next;
ROOM_INDEX_DATA *room;


		room = get_room_index(ch->in_room);
   
		if (room != NULL) 
		  {
			for ( pChar = room->people; pChar; pChar = pChar_next )
			  {
      				pChar_next = pChar->next_in_room;
      				
    				
      				if (pChar == ch)
      					continue;

      				if (pChar->in_room != ch->in_room)
      					continue;
      					
				if (pChar->fighting != ch)
    				 return TRUE;
    			  }
		}
return FALSE;
}

int find_gate_mobile_in_zone(CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
char bug_buf[MAX_STRING_LENGTH];
int room_vnum;
CHAR_DATA *pChar;
CHAR_DATA *pChar_next;
AREA_DATA *pArea=ch->in_room->area;
ROOM_INDEX_DATA *room;

	for ( room_vnum = pArea->min_vnum; room_vnum <= pArea->max_vnum; room_vnum++ )
	  {
		room = get_room_index(room_vnum);
   
		if (room != NULL) 
		  {
			for ( pChar = room->people; pChar; pChar = pChar_next )
			  {
      				pChar_next = pChar->next_in_room;
      				
      				
      				xprintf( bug_buf, "Pchar: %d ch: %d", pChar->level, ch->level );
      				bug( bug_buf, 0 );
      				
      				if (pChar == ch)
      					continue;
      				if (pChar->level >= ch->level + 3)
      					continue;
      				
      				if (pChar->in_room == ch->in_room)
      					continue;
      					
				if ( (pChar->in_room == NULL)
   				  &&   (IS_SET(pChar->in_room->room_flags, ROOM_SAFE))
			  	  &&   (IS_SET(pChar->in_room->room_flags, ROOM_PRIVATE))
	    		  	  &&   (IS_SET(pChar->in_room->room_flags, ROOM_SOLITARY))
	    		  	  &&   (IS_SET(ch->in_room->room_flags, ROOM_NOSUMMON))
    			  	  &&   (strstr(ch->in_room->area->builders, "Unlinked" ))
    			  	  &&   (IS_SET(pChar->in_room->room_flags, ROOM_NOSUMMON))
	    		  	  &&   (IS_SET(ch->in_room->room_flags, ROOM_ARENA ))
	    			  &&   (IS_SET(pChar->in_room->room_flags, ROOM_ARENA) )
	    			  &&   (!can_see(ch, pChar)) )
    				  	continue;
    				 
    				return room_vnum;
    			  }
    		 }
    	  }
return -1;
}
