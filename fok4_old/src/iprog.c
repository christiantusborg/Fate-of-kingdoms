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
#include "interp.h"

DECLARE_IPROG_FUN_WEAR( wear_prog_excalibur );
DECLARE_IPROG_FUN_REMOVE( remove_prog_excalibur );
DECLARE_IPROG_FUN_DEATH( death_prog_excalibur );
DECLARE_IPROG_FUN_SPEECH( speech_prog_excalibur );
DECLARE_IPROG_FUN_SAC( sac_prog_excalibur );

DECLARE_IPROG_FUN_SAC( sac_prog_kingdom_item );
DECLARE_IPROG_FUN_GET( get_prog_kingdom_item );
DECLARE_IPROG_FUN_GIVE( give_prog_kingdom_item );

DECLARE_IPROG_FUN_FIGHT( fight_prog_sub_weapon   );
DECLARE_IPROG_FUN_GET( get_prog_hedgehog );
DECLARE_IPROG_FUN_FIGHT( fight_prog_ruby );
DECLARE_IPROG_FUN_SPEECH( speech_prog_saangreal );

DECLARE_IPROG_FUN_FIGHT( fight_prog_chaos_blade );
DECLARE_IPROG_FUN_DEATH( death_prog_chaos_blade );

DECLARE_IPROG_FUN_FIGHT( fight_prog_mark_healing );
DECLARE_IPROG_FUN_FIGHT( fight_prog_mark_power );
DECLARE_IPROG_FUN_FIGHT( fight_prog_mark_time );
DECLARE_IPROG_FUN_FIGHT( fight_prog_mark_destruction);
DECLARE_IPROG_FUN_FIGHT( fight_prog_mark_love );
DECLARE_IPROG_FUN_FIGHT( fight_prog_mark_magic );
DECLARE_IPROG_FUN_FIGHT( fight_prog_mark_deception );

DECLARE_IPROG_FUN_FIGHT( fight_prog_golden_weapon );
DECLARE_IPROG_FUN_DEATH( death_prog_golden_weapon );

DECLARE_IPROG_FUN_GET( get_prog_heart );

DECLARE_IPROG_FUN_WEAR( wear_prog_bracer );
DECLARE_IPROG_FUN_REMOVE( remove_prog_bracer );

DECLARE_IPROG_FUN_WEAR( wear_prog_Vaezeron );
DECLARE_IPROG_FUN_REMOVE( remove_prog_Vaezeron );

DECLARE_IPROG_FUN_WEAR(wear_prog_ranger_staff);
DECLARE_IPROG_FUN_FIGHT( fight_prog_ranger_staff );
DECLARE_IPROG_FUN_DEATH( death_prog_ranger_staff );

DECLARE_IPROG_FUN_WEAR(wear_prog_coconut);
DECLARE_IPROG_FUN_ENTRY(entry_prog_coconut);
DECLARE_IPROG_FUN_GREET(greet_prog_coconut);
DECLARE_IPROG_FUN_GET(get_prog_coconut);
DECLARE_IPROG_FUN_REMOVE(remove_prog_coconut);

DECLARE_IPROG_FUN_FIGHT(fight_prog_firegauntlets);
DECLARE_IPROG_FUN_WEAR(wear_prog_firegauntlets);
DECLARE_IPROG_FUN_REMOVE(remove_prog_firegauntlets);

DECLARE_IPROG_FUN_FIGHT(fight_prog_vorbalblade);
DECLARE_IPROG_FUN_GET(get_prog_spec_weapon);
DECLARE_IPROG_FUN_DROP(drop_prog_dice);
DECLARE_IPROG_FUN_AREA(area_prog_potato);
DECLARE_IPROG_FUN_FIGHT(fight_prog_shockwave);
DECLARE_IPROG_FUN_FIGHT(fight_prog_snake );
DECLARE_IPROG_FUN_GREET( greet_prog_dexbridge );
DECLARE_IPROG_FUN_FIGHT( fight_prog_dragonsbane );
DECLARE_IPROG_FUN_SPEECH( speech_prog_ringgondor );
DECLARE_IPROG_FUN_SPEECH( speech_prog_dog );

DECLARE_IPROG_FUN_WEAR(wear_prog_Dermostest);
DECLARE_IPROG_FUN_REMOVE( remove_prog_Dermostest );
DECLARE_IPROG_FUN_SPEECH( speech_prog_Dermostest );
DECLARE_IPROG_FUN_FIGHT( fight_prog_Dermostest   );

DECLARE_IPROG_FUN_SAC( sac_prog_vebrek );
DECLARE_IPROG_FUN_FIGHT( fight_prog_vebrek   );
DECLARE_IPROG_FUN_SPEECH( speech_prog_vebrek );

DECLARE_IPROG_FUN_DROP(drop_prog_Wonderegg);

//DECLARE_IPROG_FUN_WEAR( wear_prog_Coldresist );


void    raw_kill        args( ( CHAR_DATA *ch,CHAR_DATA *victim ) );
void    raw_kill_org    args( ( CHAR_DATA *victim, int part ) );


void iprog_set(OBJ_INDEX_DATA *objindex,const char *progtype, const char *name)
{

   if (!str_cmp(progtype, "wear_prog"))
     {
 /*
  *    if (!str_cmp(name, "your_wear_function_here"))
  *          objindex->mprogs->wear_prog = your_wear_function_here;
  *    else if (!str_cmp(name, "next_wear_function")) ...
  */

 /* insert your functions here */
       if (!str_cmp(name, "wear_prog_excalibur"))
	 objindex->iprogs->wear_prog = wear_prog_excalibur;
       else if (!str_cmp(name, "wear_prog_bracer"))
	 objindex->iprogs->wear_prog = wear_prog_bracer;
       else if (!str_cmp(name, "wear_prog_Vaezeron"))
	 objindex->iprogs->wear_prog = wear_prog_Vaezeron;
       else if (!str_cmp(name, "wear_prog_coconut"))
	 objindex->iprogs->wear_prog = wear_prog_coconut;
       else if (!str_cmp(name, "wear_prog_firegauntlets"))
	 objindex->iprogs->wear_prog = wear_prog_firegauntlets;
       else if (!str_cmp(name, "wear_prog_ranger_staff"))
	 objindex->iprogs->wear_prog = wear_prog_ranger_staff;
       else if (!str_cmp(name, "wear_prog_Dermostest"))
	 objindex->iprogs->wear_prog = wear_prog_Dermostest;
//     else if (!str_cmp(name, "wear_prog_Coldresist"))
//	 objindex->iprogs->wear_prog = wear_prog_Coldresist;
       else 
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }

       SET_BIT(objindex->progtypes, IPROG_WEAR);
       return;
     }

   if (!str_cmp(progtype, "remove_prog"))
     {
       if (!str_cmp(name,"remove_prog_excalibur"))
	 objindex->iprogs->remove_prog = remove_prog_excalibur;
       else if (!str_cmp(name, "remove_prog_bracer"))
	 objindex->iprogs->remove_prog = remove_prog_bracer;
       else if (!str_cmp(name, "remove_prog_Vaezeron"))
	 objindex->iprogs->remove_prog = remove_prog_Vaezeron;	 
       else if (!str_cmp(name, "remove_prog_coconut"))
	 objindex->iprogs->remove_prog = remove_prog_coconut;
       else if (!str_cmp(name, "remove_prog_firegauntlets"))
	 objindex->iprogs->remove_prog = remove_prog_firegauntlets;
       else if (!str_cmp(name, "remove_prog_Dermostest"))
	 objindex->iprogs->remove_prog = remove_prog_Dermostest;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_REMOVE);
       return;
     }
   if (!str_cmp(progtype, "get_prog"))
     {
       if (!str_cmp(name,"get_prog_kingdom_item"))
	 objindex->iprogs->get_prog = get_prog_kingdom_item;
       else if (!str_cmp(name, "get_prog_hedgehog"))
	 objindex->iprogs->get_prog = get_prog_hedgehog;
       else if (!str_cmp(name, "get_prog_heart"))
	 objindex->iprogs->get_prog = get_prog_heart;
       else if (!str_cmp(name, "get_prog_coconut"))
	 objindex->iprogs->get_prog = get_prog_coconut;
       else if (!str_cmp(name, "get_prog_spec_weapon"))
	 objindex->iprogs->get_prog = get_prog_spec_weapon;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_GET);
       return;
     }
   if (!str_cmp(progtype, "drop_prog"))
     {
       if (!str_cmp(name, "drop_prog_dice"))
	 objindex->iprogs->drop_prog = drop_prog_dice;
       else if (!str_cmp(name, "drop_prog_Wonderegg"))
	 objindex->iprogs->drop_prog = drop_prog_Wonderegg;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_DROP);
       return;
     }

   /* Sacrificing: returning TRUE prevents destruction, but the player still
    * gets gold. So the player's gold should be reduced.
    */
   if (!str_cmp(progtype, "sac_prog")) 
     {
       if (!str_cmp(name, "sac_prog_excalibur"))
	 objindex->iprogs->sac_prog = sac_prog_excalibur;
       else if (!str_cmp(name, "sac_prog_kingdom_item"))
	 objindex->iprogs->sac_prog = sac_prog_kingdom_item;
	else if (!str_cmp(name, "sac_prog_vebrek"))
	 objindex->iprogs->sac_prog = sac_prog_vebrek;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_SAC);
       return;
     }
   if (!str_cmp(progtype, "entry_prog"))
     {
       if (!str_cmp(name, "entry_prog_coconut"))
         objindex->iprogs->entry_prog = entry_prog_coconut;
       else
         {
           bug("Load_improgs: 'I': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, IPROG_ENTRY);
       return;
     }
   if (!str_cmp(progtype, "give_prog"))
     {
       if (!str_cmp(name, "give_prog_kingdom_item"))
         objindex->iprogs->give_prog = give_prog_kingdom_item;
       else
         {
           bug("Load_improgs: 'I': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, IPROG_GIVE);
       return;
     }
   if (!str_cmp(progtype, "greet_prog"))
     {
       if (!str_cmp(name, "greet_prog_coconut"))
         objindex->iprogs->greet_prog = greet_prog_coconut;
       else if (!str_cmp(name, "greet_prog_dexbridge"))
         objindex->iprogs->greet_prog = greet_prog_dexbridge;
       else
         {
           bug("Load_improgs: 'I': Function not found for vnum %d",
               objindex->vnum);
           exit(1);
         }
       SET_BIT(objindex->progtypes, IPROG_GREET);
       return;
     }
   if (!str_cmp(progtype, "fight_prog"))
     {
       if (!str_cmp(name, "fight_prog_ranger_staff"))
	 objindex->iprogs->fight_prog = fight_prog_ranger_staff;       
       else if (!str_cmp(name, "fight_prog_sub_weapon"))
	 objindex->iprogs->fight_prog = fight_prog_sub_weapon;
       else if (!str_cmp(name, "fight_prog_ruby"))
	 objindex->iprogs->fight_prog = fight_prog_ruby;
       else if (!str_cmp(name, "fight_prog_chaos_blade"))
	 objindex->iprogs->fight_prog = fight_prog_chaos_blade;
       else if (!str_cmp(name, "fight_prog_mark_healing"))
	 objindex->iprogs->fight_prog = fight_prog_mark_healing;
       else if (!str_cmp(name, "fight_prog_mark_power"))
	 objindex->iprogs->fight_prog = fight_prog_mark_power;
       else if (!str_cmp(name, "fight_prog_mark_destruction"))
	 objindex->iprogs->fight_prog = fight_prog_mark_destruction;
       else if (!str_cmp(name, "fight_prog_mark_time"))
	 objindex->iprogs->fight_prog = fight_prog_mark_time;
       else if (!str_cmp(name, "fight_prog_mark_love"))
         objindex->iprogs->fight_prog = fight_prog_mark_love;
       else if (!str_cmp(name, "fight_prog_mark_magic"))
	 objindex->iprogs->fight_prog = fight_prog_mark_magic;
       else if (!str_cmp(name, "fight_prog_mark_deception"))
	 objindex->iprogs->fight_prog = fight_prog_mark_deception;
       else if (!str_cmp(name, "fight_prog_golden_weapon"))
	 objindex->iprogs->fight_prog = fight_prog_golden_weapon;
       else if (!str_cmp(name, "fight_prog_snake"))
	 objindex->iprogs->fight_prog = fight_prog_snake;
       else if (!str_cmp(name, "fight_prog_shockwave"))
	 objindex->iprogs->fight_prog = fight_prog_shockwave;
       else if (!str_cmp(name, "fight_prog_firegauntlets"))
	 objindex->iprogs->fight_prog = fight_prog_firegauntlets;
       else if (!str_cmp(name, "fight_prog_vorbalblade"))
	 objindex->iprogs->fight_prog = fight_prog_vorbalblade;
       else if (!str_cmp(name, "fight_prog_dragonsbane"))
	 objindex->iprogs->fight_prog = fight_prog_dragonsbane;
       else if (!str_cmp(name, "fight_prog_Dermostest"))
	 objindex->iprogs->fight_prog = fight_prog_Dermostest;
       else if (!str_cmp(name, "fight_prog_vebrek"))
	 objindex->iprogs->fight_prog = fight_prog_vebrek;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_FIGHT);
       return;
     }
   if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
     {
       if (!str_cmp(name, "death_prog_excalibur"))
	 objindex->iprogs->death_prog = death_prog_excalibur;
       else if (!str_cmp(name, "death_prog_ranger_staff"))
	 objindex->iprogs->death_prog = death_prog_ranger_staff;
       else if (!str_cmp(name, "death_prog_chaos_blade"))
	 objindex->iprogs->death_prog = death_prog_chaos_blade;
       else if (!str_cmp(name, "death_prog_golden_weapon"))
	 objindex->iprogs->death_prog = death_prog_golden_weapon;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_DEATH);
       return;
     }
   if (!str_cmp(progtype, "speech_prog"))
     {
       if (!str_cmp(name, "speech_prog_excalibur"))
	 objindex->iprogs->speech_prog = speech_prog_excalibur;
       else if(!str_cmp(name, "speech_prog_saangreal"))
	 objindex->iprogs->speech_prog = speech_prog_saangreal;
       else if(!str_cmp(name, "speech_prog_ringgondor"))
	 objindex->iprogs->speech_prog = speech_prog_ringgondor;	 
       else if(!str_cmp(name, "speech_prog_dog"))
	 objindex->iprogs->speech_prog = speech_prog_dog;	 	 
       else if(!str_cmp(name, "speech_prog_Dermostest"))
	 objindex->iprogs->speech_prog = speech_prog_Dermostest;
       else if(!str_cmp(name, "speech_prog_vebrek"))
	 objindex->iprogs->speech_prog = speech_prog_vebrek;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_SPEECH);
       return;
     }

   if (!str_cmp(progtype, "area_prog"))
     {
       if (!str_cmp(name, "area_prog_potato"))
	 objindex->iprogs->area_prog = area_prog_potato;
       else
	 {
	   bug("Load_improgs: 'I': Function not found for vnum %d",
	       objindex->vnum);
	   exit(1);
	 }
       SET_BIT(objindex->progtypes, IPROG_AREA);
       return;
     }


   bug( "Load_improgs: 'I': invalid program type for vnum %d",objindex->vnum);
   exit(1);
 }


/* ITEM PROGRAM FUNCTIONS */

void fight_prog_dragonsbane(OBJ_DATA *obj, CHAR_DATA *ch)
{
  CHAR_DATA *victim;
  
  if ( ch == NULL || ch->fighting == NULL )
    return;
  
  victim = ch->fighting;
  if ( victim->race != 18 )
    return;
  if ( get_eq_char(ch, WEAR_WIELD ) != obj )
    return;

  switch(dice(1,17))
  {
	case 1:
	case 2:
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  break;
	case 3:
	case 4:
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  break;
	case 5:
	case 6:
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  break;
	case 7:
	case 8:
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  one_hit( ch, victim, TYPE_UNDEFINED,FALSE );
	  break;
	case 9:
	case 10:
	  act( "Your $p emits a green aura and acid sprays from it's blade!", ch, obj, NULL, TO_CHAR);
	  act( "$n's $p emits a green aura and acid sprays from it's blade!", ch, obj, NULL, TO_ROOM);
	  obj_cast_spell(gsn_acid_blast,ch->level, ch, victim, obj);
	  break;
	case 11:
	  break;
	case 12:
	case 13:
	  act( "$p glows slightly.", ch, obj, NULL, TO_CHAR );
	  obj_cast_spell(gsn_cure_serious,ch->level, ch, ch, obj );
	  obj_cast_spell(gsn_cure_serious,ch->level, ch, ch, obj );
	  break;
	case 14:
	  act( "$p glows brightly.", ch, obj, NULL, TO_CHAR );
	  obj_cast_spell(gsn_cure_critical,ch->level, ch, ch, obj );
	  obj_cast_spell(gsn_cure_critical,ch->level, ch, ch, obj );
	  break;
	case 15:
	case 16:
	  act( "$p makes a deep cut in $N's skin!",ch,obj,victim,TO_CHAR);
	  act( "$p makes a deep cut in $N's skin!",ch,obj,victim,TO_ROOM);
	  damage( ch, victim, 500, 3, DAM_MENTAL, FALSE);
	  break;
  }
  if ( victim == NULL )
  {
	obj_from_char( obj );
	obj_to_room( obj, get_room_index(1) );
	dump_to_scr( "Victim died of dragon's bane\n\r" );
	return;
  }
  return;
}

void greet_prog_dexbridge(OBJ_DATA *obj, CHAR_DATA *ch)
{
int chance;

  chance = 0;
  switch( get_curr_stat( ch, STAT_DEX ) )
  {
    case 15:  chance =  5; break;
    case 16:  chance = 10; break;
    case 17:  chance = 15; break;
    case 18:  chance = 20; break;
    case 19:  chance = 25; break;
    case 20:  chance = 30; break;
    case 21:  chance = 50; break;
    case 22:  chance = 70; break;
    case 23:  chance = 80; break;
    case 24:  chance = 100; break;
    case 25:  chance = 100; break;
  }

  if ( number_percent() > chance )   /* Fallen down */
  {
     send_to_char( "\n\rYour feet slips of the of the bridge and you fall down!\n\r\n",ch);
     act( "$n looses $s balance and falls down!", ch, NULL, NULL, TO_ROOM );
     char_from_room( ch );
     char_to_room( ch, get_room_index(obj->value[0]) );
     damage( ch, ch, 50, gsn_cleave, DAM_BASH, FALSE );
     do_look( ch, "" );
     act( "$n has arrived, falling from the bridge above.", ch, NULL, NULL, TO_ROOM );
  }

  return;
}

void wear_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("$p begins to shine a bright white.",ch,obj,NULL,TO_CHAR);
  act("$p begins to shine a bright white.",ch,obj,NULL,TO_ROOM);
}

void wear_prog_bracer(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_haste))
    {
      send_to_char("As you slide your arms into these bracers, they mold to your skin.\n\r", ch);
      send_to_char("Your hands and arms feel incredibly light.\n\r", ch);

      af.where = TO_AFFECTS;
      af.type = gsn_haste;
      af.duration = -1;
      af.level = ch->level;
      af.bitvector = AFF_HASTE;
      af.location = APPLY_DEX;
      af.modifier = 1 + (ch->level >= 18) + (ch->level >= 30) + (ch->level >= 45);
      affect_to_char(ch, &af);
    }
}

/***************/
void wear_prog_Vaezeron(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_dark_vision))
   {
    af.where     = TO_AFFECTS;
    af.type      = gsn_dark_vision;
    af.level     = ch->level;
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char( "Your eyes glows red.\n\r", ch );
    return;                 
   }
}   

void remove_prog_Vaezeron(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_dark_vision))
    {
      affect_strip(ch, gsn_dark_vision);
      send_to_char("Your eyes no longer glows red.\n\r", ch);
    }
}

/***************/



void remove_prog_bracer(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_haste))
    {
      affect_strip(ch, gsn_haste);
      send_to_char("Your hands and arms feel heavy again.\n\r", ch);
    }
}


void remove_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("$p stops glowing.",ch,obj,NULL,TO_CHAR);
  act("$p stops glowing.",ch,obj,NULL,TO_ROOM);
}

bool death_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act_new("$p starts to glow with a blue aura.",ch,obj,NULL,TO_CHAR,POS_DEAD);
  act("$p starts to glow with a blue aura,",ch,obj,NULL,TO_ROOM);
  ch->hit = ch->max_hit;
  send_to_char("You feel much better.",ch);
  act("$n looks much better.",ch,NULL,NULL,TO_ROOM);
  return TRUE; /* prevents death */
}

void speech_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{

  if (!str_cmp(speech, "sword of acid")      /* acid blast, wielder's level */
      && (ch->fighting) && (get_eq_char(ch,WEAR_WIELD) == obj))
    {
      send_to_char("Acid sprays from the blade of Excalibur.\n\r",ch);
      act("Acid sprays from the blade of Excalibur.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_acid_blast,ch->level,ch,ch->fighting,NULL);
      WAIT_STATE(ch, 3 * PULSE_VIOLENCE);
    }
}
 
void speech_prog_ringgondor(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{

    CHAR_DATA *victim = (CHAR_DATA *) ch;
    ROOM_INDEX_DATA *location;

  if (!str_cmp(speech, "recall")      /* if useing ring */
      &&( (get_eq_char(ch,WEAR_FINGER_L) == obj) || (get_eq_char(ch,WEAR_FINGER_R) ) ) )
    {

     if (IS_NPC(victim))
      return;


    if ((location = get_room_index( 2401 )) == NULL)
    {
	send_to_char("You are completely lost.\n\r",victim);
	return;
    }

    if (ch->desc != NULL && current_time - ch->last_fight_time
	< FIGHT_DELAY_TIME)
      {
	send_to_char("You are too pumped to pray now.\n\r",ch);
	return;
      }

    if (IS_SET(victim->in_room->room_flags,ROOM_NO_RECALL) ||
	IS_AFFECTED(victim,AFF_CURSE) ||
	( strstr( victim->in_room->area->builders, "Unlinked" ) ))
    {
	send_to_char("Spell failed.\n\r",victim);
	return;
    }

    if (victim->fighting != NULL)
	stop_fighting(victim,TRUE);

    ch->move /= 2;
    act("$n disappears.",victim,NULL,NULL,TO_ROOM);
    char_from_room(victim);
    char_to_room(victim,location);
    act("$n appears in the room.",victim,NULL,NULL,TO_ROOM);
    do_look(victim,"auto");
 }
}
 
  
bool sac_prog_excalibur(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("The gods are infuriated!",ch,NULL,NULL,TO_CHAR);
  act("The gods are infuriated!",ch,NULL,NULL,TO_ROOM);
  damage(ch,ch,
	 (ch->hit - 1) > 1000? 1000 : (ch->hit - 1),
	 TYPE_HIT,DAM_HOLY, TRUE);
  ch->gold = 0;
/*  ch->siver = 0; */
  return TRUE; /* Not destroyed */
}

void fight_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj && number_percent() < 10)
    {
      send_to_char("Your ranger's staff glows blue!\n\r",ch);
      act("$n's ranger's staff glows blue!",ch,NULL,NULL,TO_ROOM);
      
      obj_cast_spell(gsn_cure_critical,ch->level,ch,ch,obj);
    }
}

void fight_prog_sub_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj && number_percent() < 30)
    {
      if ( ((float) ch->hit)/((float) ch->max_hit) > 0.9)
	send_to_char("Your weapon whispers, 'You're doing great!'\n\r",ch);
      else if ( ((float) ch->hit)/((float) ch->max_hit) > 0.6)
	send_to_char("Your weapon whispers, 'Keep up the good work!'\n\r",ch);
      else if ( ((float) ch->hit)/((float) ch->max_hit) > 0.4)
	  send_to_char("Your weapon whispers, 'You can do it!'\n\r",ch);
      else send_to_char("Your weapon whispers, 'Run away! Run away!'\n\r",ch);
    }
}

bool death_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("Your ranger's staff disappears.\n\r",ch);
  act("$n's ranger's staff disappears.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  return FALSE;
}


void get_prog_spec_weapon(OBJ_DATA *obj, CHAR_DATA *ch) 
{
/*  :: comments out because it was buggy */
/* prolly cuz i was loading eq onto kroudar and givin to mortals */

/*
  if ( strstr( obj->extra_descr->description, ch->name ) != NULL )  
  {
    if ( IS_AFFECTED( ch, AFF_POISON ) && (dice(1,5)==1) )  {
      send_to_char( "Your weapon glows blue.", ch );
      act( "$n's weapon glows blue.", ch, NULL, NULL, TO_ROOM );
      spell_cure_poison( 0, 30, ch, ch, TARGET_CHAR );
      return;
    }
    if ( IS_AFFECTED( ch, AFF_CURSE ) && (dice(1,5)==1) )  {
      send_to_char( "Your weapon glows blue.", ch );
      act( "$n's weapon glows blue.", ch, NULL, NULL, TO_ROOM );
      spell_remove_curse( gsn_remove_curse, 30, ch, ch, TARGET_CHAR );
      return;
    }
    send_to_char( "Your weapon's humming gets louder.\n\r", ch );
    return;
  }
*/

  act( "You are zapped by $p and drop it.", ch, obj, NULL, TO_CHAR );

  obj_from_char( obj );
  obj_to_room( obj, ch->in_room );

  switch( dice(1, 10) )  {
  case 1:
    spell_curse( gsn_curse, ch->level < 10? 1 : ch->level-9, ch, ch, TARGET_CHAR );
    break;
  case 2:
    spell_poison( gsn_poison, ch->level < 10? 1 : ch->level-9, ch, ch, TARGET_CHAR );
    break;
  }
  return;
    
}

void get_prog_kingdom_item(OBJ_DATA *obj, CHAR_DATA *ch) 
{
  if (IS_NPC(ch))
    {
      act("You are not worthy to have $p and drop it.",
	 ch, obj, NULL, TO_CHAR);
      act("$n is not worthy to have $p and drops it.",
	 ch, obj, NULL, TO_ROOM);
      obj_from_char(obj);
      obj_to_room(obj, ch->in_room);
      return;
    }
  else
    {

      DESCRIPTOR_DATA *d;

      for ( d = descriptor_list; d; d = d->next )
        {
          if ( d->connected == CON_PLAYING && 
	     kingdom_table[d->character->kingdom].obj_ptr == obj)
     	  act_color("{CYou feel a shudder in your Kingdom Power!{w",
		   d->character,NULL,NULL,TO_CHAR,POS_DEAD,0);
    	}
    }
}

bool sac_prog_kingdom_item(OBJ_DATA *obj, CHAR_DATA *ch)
{
  OBJ_DATA *container;
  OBJ_DATA *item;
  char buf[160];
  int i;

  act("The gods are infuriated!",ch,NULL,NULL,TO_CHAR);
  act("The gods are infuriated!",ch,NULL,NULL,TO_ROOM);
/*  if (kingdom_table[ch->kingdom].obj_ptr == obj) 
    damage(ch,ch,ch->hit + 10,TYPE_HIT,DAM_HOLY);
  else */  /* temp change */ 
    damage(ch,ch,325,TYPE_HIT,DAM_HOLY, TRUE); /* 325: ANNIHILATE */
  ch->gold = 0;

  for(i=0;i<MAX_KINGDOM;i++)
    if (kingdom_table[i].obj_ptr == obj) break;
  if ( i < MAX_KINGDOM )  {
    if ( obj->pIndexData->vnum == kingdom_table[KINGDOM_GONDOR].obj_vnum )
      container = create_object( get_obj_index( OBJ_VNUM_SIRIUS_STAND ),60 );
    else if ( obj->pIndexData->vnum == kingdom_table[KINGDOM_ALDON].obj_vnum)
      container = create_object( get_obj_index( OBJ_VNUM_KUJIKIRI_SKULL ), 60);
    /*else if ( obj->pIndexData->vnum == kingdom_table[KINGDOM_HORDE].obj_vnum)
      container = create_object( get_obj_index( OBJ_VNUM_HORDE_THRONE ),60 );*/
    else if ( obj->pIndexData->vnum ==
kingdom_table[KINGDOM_MORDUKHAN].obj_vnum)
      container = create_object( get_obj_index( OBJ_VNUM_CRESCENT_ALTAR ),60 );
    else if ( obj->pIndexData->vnum == kingdom_table[KINGDOM_DRAKKON].obj_vnum)
      container = create_object( get_obj_index( OBJ_VNUM_GEMMA_ALTAR ), 60);
    else
      container = create_object( get_obj_index( OBJ_VNUM_ASPER_ALTAR),60);

    item = create_object( get_obj_index( kingdom_table[i].obj_vnum), 60 );
    obj_to_obj( item, container );
    obj_to_room( container, get_room_index(kingdom_table[i].room_vnum) );
    sprintf( buf, "You see %s forming again slowly.\n\r", 
	container->short_descr );
    if ( get_room_index(kingdom_table[i].room_vnum)->people != NULL )  {
	act( buf, get_room_index(kingdom_table[i].room_vnum)->people,NULL,NULL, TO_CHAR);
	act( buf, get_room_index(kingdom_table[i].room_vnum)->people,NULL,NULL, TO_ROOM);
    }
  }
  else
    bug( "iprog: Sac_kingdom_item: Was not the kingdom's item.", 0);

  return FALSE; /* destroyed */
} 

void give_prog_kingdom_item(OBJ_DATA *obj, CHAR_DATA *ch, CHAR_DATA *mob)
{
  if (IS_NPC(mob))

    {
      act("You are not worthy to have $p and drop it.",
	 mob, obj, NULL, TO_CHAR);
      act("$n is not worthy to have $p and drops it.",
	 mob, obj, NULL, TO_ROOM);
      obj_from_char(obj);
      obj_to_room(obj, mob->in_room);
      return;
    }
}

void get_prog_hedgehog(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (IS_NPC(ch) || IS_IMMORTAL(ch))
    return;
  
  else
    {
      obj_cast_spell(gsn_terangreal,ch->level,ch,ch,NULL);
      act("The little hedgehog slips from your hand as you fall asleep.",
	  ch, NULL, NULL, TO_CHAR);
      act("The little hedgehog slips from $n's hand as $e falls asleep.",
	  ch, NULL, NULL, TO_ROOM);
      obj_from_char(obj);
      obj_to_room(obj, ch->in_room);
      return;
    }
}

void fight_prog_ruby(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj)
    switch(number_bits(3)) {
    case 0:
    case 1:
    
      act("Your dagger makes a black, poisonous scratch on $N's arm!",
	  ch, NULL, ch->fighting, TO_CHAR);
      act("$n's dagger makes a black, poisonous scratch on your arm!",
	  ch, NULL, ch->fighting, TO_VICT);
      act("$n's dagger makes a black, poisonous scratch on $N's arm!"
	  ,ch, NULL, ch->fighting, TO_NOTVICT);
      
      ch->fighting->hit = 1;
      break;
      
    case 2:
      send_to_char
	("Your dagger turns on you and scratches your arm with poison!\n\r",
	 ch);
      act("$n's dagger scratches $m with poison!", ch,NULL,NULL,TO_ROOM);
      
      ch->hit = 1;
      break;
    }
}

void speech_prog_saangreal(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
  if (!str_cmp(speech, "sa'angreal") && (get_eq_char(ch, WEAR_HOLD) == obj)
           && !IS_NPC(ch))
    obj_cast_spell(86,ch->level,ch,ch,NULL);

  else if (!str_cmp(speech, "valere") && (get_eq_char(ch,WEAR_HOLD) == obj)
           && !IS_NPC(ch))
    obj_cast_spell(87,ch->level,ch,ch,NULL);

  else if (!str_cmp(speech, "saidar") && (get_eq_char(ch,WEAR_HOLD) == obj)
	   && (ch->fighting) && !IS_NPC(ch))
    {
      act("A blast of energy bursts from your hand toward $N!",
	  ch,NULL,ch->fighting,TO_CHAR);
      act("A blast of energy bursts from $n's hand toward you!",
	  ch,NULL,ch->fighting,TO_VICT);
      act("A blast of energy bursts from $n's hand toward $N!",
	  ch,NULL,ch->fighting,TO_NOTVICT);
      obj_cast_spell(88,ch->level,ch,ch->fighting,NULL);
    }
}
      
void fight_prog_chaos_blade(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj)
    switch(number_bits(6)) {
    case 0:
      
      act("The chaotic blade trembles violently!", ch, NULL, NULL, TO_ROOM);
      send_to_char("Your chaotic blade trembles violently!\n\r", ch);
      obj_cast_spell(gsn_mirror,ch->level,ch,ch,obj); 
      break;

    case 1:

      act("The chaotic blade shakes a bit.", ch, NULL, NULL, TO_ROOM);
      send_to_char("Your chaotic blade shakes a bit.\n\r", ch);
      obj_cast_spell(gsn_garble,ch->level,ch,ch->fighting,obj);
      break;

    case 2:

      act("The chaotic blade shivers uncontrollably!",ch,NULL,NULL,TO_ROOM);
      send_to_char("Your chaotic blade shivers uncontrollably!\n\r",ch);
      obj_cast_spell(gsn_confuse, ch->level,ch,ch->fighting,obj);
      break;

    }
}

bool death_prog_chaos_blade(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("Your chaotic blade disappears.\n\r",ch);
  act("$n's chaotic blade disappears.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  return FALSE;
}

void fight_prog_mark_healing(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(7)) {
    case 0:
    case 1:
      obj_cast_spell(gsn_cure_light, ch->level, ch, ch, obj);
      break;
    case 2:
      obj_cast_spell(gsn_cure_serious, ch->level, ch, ch, obj);
      break;
    }
}


void fight_prog_mark_deception(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(7)) {
    case 0:
    case 1:
    case 2:
      obj_cast_spell(gsn_blindness, ch->level, ch,ch->fighting, obj);
      break;
    case 3:
      obj_cast_spell(gsn_shield, ch->level,ch,ch, obj);
      break;
    case 4:
      obj_cast_spell(gsn_blindness_dust, ch->level,ch,ch->fighting, obj);
      break;
    }
}

void fight_prog_mark_love(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(6)) {
    case 0:
      obj_cast_spell(gsn_bless, ch->level/2, ch, ch,obj);
      break;
    case 1:
      obj_cast_spell(gsn_cure_serious, ch->level/2, ch, ch, obj);
      break;
    }
}

void fight_prog_mark_time(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(7)) {
    case 0:
      obj_cast_spell(gsn_cure_light, ch->level, ch, ch, obj); 
      break;
    case 1:
      obj_cast_spell(gsn_demonfire, ch->level/2.5, ch, ch->fighting, obj);
      break;
    }
}

void fight_prog_mark_magic(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(7)) {
    case 0:
    case 1:
      obj_cast_spell(gsn_ray_of_truth, ch->level, ch, ch->fighting, obj);
      break;
    case 2:
      obj_cast_spell(gsn_shield, ch->level, ch, ch->fighting, obj);
      break;
    case 3:
	if(ch->max_mana > ch->mana) {
	send_to_char("You feel as Therone recharges your mind.\n\r",ch); 
		ch->mana += 10; }
      break;
    }
}

void fight_prog_mark_destruction(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_TATTOO) == obj)
    switch(number_bits(7)) {
    case 0:
      obj_cast_spell(gsn_curse, ch->level, ch, ch->fighting, obj);
      break;
    case 1:
      obj_cast_spell(gsn_weaken, ch->level, ch, ch->fighting, obj);
      break;
    case 2:
      obj_cast_spell(gsn_plague, ch->level, ch, ch->fighting, obj);
      break;
    }
}

void fight_prog_mark_power(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if(get_eq_char(ch, WEAR_TATTOO) == obj)
    {
    if (number_percent() < 50)
      {
	switch(number_bits(6)) {
	case 0:
	  if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
	      ||  is_affected(ch,skill_lookup("frenzy")))
	    {
	      send_to_char("You get a little madder.\n\r",ch);
	      return;
	    }

	  af.where = TO_AFFECTS;
	  af.type = gsn_berserk;
	  af.level = ch->level;
	  af.duration = ch->level / 3;
	  af.modifier = ch->level / 6;
	  af.bitvector = AFF_BERSERK;

	  af.location = APPLY_HITROLL;
	  affect_to_char(ch, &af);

	  af.location = APPLY_DAMROLL;
	  affect_to_char(ch, &af);

	  af.modifier = 10 * (ch->level / 6);
	  af.location = APPLY_AC;
	  affect_to_char(ch, &af);
	  
	  ch->hit += ch->level * 2;
	  ch->hit = UMIN(ch->hit,ch->max_hit);
	  
	  send_to_char("Your pulse races as you are consumned by rage!\n\r",
		       ch);
	  act("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM);

	  break;
	}
      }
    else
      {
	switch(number_bits(6)) {
	case 0:
	  ch->hit += 15;
          ch->hit = UMIN(ch->hit,ch->max_hit);
	  do_yell(ch, "No Mercy!");
	  break;
	case 1:
	  ch->hit += 7;
          ch->hit = UMIN(ch->hit,ch->max_hit);
	  do_yell(ch, "Death to your opponent!");
	  break;
	case 2:
	  ch->hit += 14;
          ch->hit = UMIN(ch->hit,ch->max_hit);
	  do_yell(ch, "Los Valdar Cuebiyari!");
	  break;
	case 3:
	  ch->hit += 3;
          ch->hit = UMIN(ch->hit,ch->max_hit);
	  do_yell(ch, "Carai an Caldazar! Carai an Ellisande! Al Ellisande!");
	  break;
	case 4:
	  do_yell(ch, "Siempre Vive el Riesgo!");
	  break;
	}
      }
    }
}


bool death_prog_golden_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("Your golden weapon disappears.\n\r",ch);
  act("$n's golden weapon disappears.",ch,NULL,NULL,TO_ROOM);
  extract_obj(obj);
  return FALSE;
}

void fight_prog_golden_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj)
    {
      if (number_percent() < 4)
	{
	  act("Your $p glows bright blue!\n\r",ch, obj, NULL, TO_CHAR);
	  act("$n's $p glows bright blue!",ch,obj,NULL,TO_ROOM);
	  
	  obj_cast_spell(gsn_cure_critical,ch->level,ch,ch,obj);
	  return;
	}
      else if (number_percent() > 92)
	{
	  act("Your $p glows bright blue!\n\r",ch, obj, NULL, TO_CHAR);
	  act("$n's $p glows bright blue!",ch,obj,NULL,TO_ROOM);
	  
	  obj_cast_spell(gsn_cure_serious,ch->level,ch,ch,obj);
	  return;
	}
    }
}
void get_prog_heart(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (obj->timer == 0)
    obj->timer = 24;
}

void fight_prog_snake(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_WIELD) == obj)
    {
      switch(number_bits(7)) {
      case 0:
  	act("One of the snake heads on your whip bites $N!", ch, NULL,
		ch->fighting, TO_CHAR);
	act("A snake from $n's whip strikes out and bites you!", ch, NULL,
		ch->fighting, TO_VICT);
	act("One of the snakes from $n's whip strikes at $N!", ch, NULL, 
		ch->fighting, TO_NOTVICT);
	obj_cast_spell(gsn_poison, ch->level, ch, ch->fighting, obj);
	break;
      case 1:
	act("One of the snake heads on your whip bites $N!", ch, NULL,
		ch->fighting, TO_CHAR);
	act("A snake from $n's whip strikes out and bites you!", ch, NULL,
		ch->fighting, TO_VICT);
	act("One of the snakes from $n's whip strikes at $N!", ch, NULL,
		ch->fighting, TO_NOTVICT);
	obj_cast_spell(gsn_weaken, ch->level, ch, ch->fighting, obj);
	break;
      }
    }
}

void area_prog_potato(OBJ_DATA *obj) {
  /* Give the potato to the first person in the room */
  ROOM_INDEX_DATA *room;
  CHAR_DATA *victim;
  OBJ_DATA *pObj;

  for (pObj = obj; pObj->in_obj; pObj = pObj->in_obj); 

  if (pObj->in_room)
      room = pObj->in_room;
  else if
     (pObj->carried_by && pObj->carried_by->in_room)
      room = pObj->carried_by->in_room;
  else 
     return;

  if (!room->people)
     return;

  if (pObj->in_room) { /* not carried, hop into his sack! */
     act("$p hops into your hands!",room->people, obj, NULL, TO_CHAR);
     act("$p hops into $n's hands!",room->people, obj, NULL, TO_ROOM);
     do_say(room->people, "Ow!! Hot! Hot!");
     damage(room->people, room->people, 5, gsn_burning_hands,DAM_FIRE, TRUE);
     if (obj->in_obj)
	obj_from_obj(obj);
     else
	obj_from_room(obj);
     obj_to_char(obj, room->people);
  }

  else {
     if (pObj->carried_by == room->people)
	victim = room->people->next_in_room?
			room->people->next_in_room : pObj->carried_by;
     else
	victim = room->people;
     
     if (pObj->carried_by == victim) {
	do_say(victim, "Ow!! Hot! Hot!");
        damage(victim,victim, 5, gsn_burning_hands,DAM_FIRE, TRUE);
        return;
     }
		
     act("$p hops out of $n's hands and into $N's!",
		pObj->carried_by, obj, victim,TO_NOTVICT);
     act("$p hops out of your hands and into $N's!",
		pObj->carried_by, obj, victim,TO_CHAR);
     act("$p hops out of $n's hands and into yours!",
		pObj->carried_by, obj, victim,TO_VICT);
     do_say(victim, "Ow!! Hot! Hot!");
     damage(victim,victim, 5, gsn_burning_hands,DAM_FIRE, TRUE);

     if (obj->in_obj)
	obj_from_obj(obj);
     else
	obj_from_char(obj);
     obj_to_char(obj,victim);
  }    
}
     

void fight_prog_shockwave(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch, WEAR_WIELD) == obj)
    switch(number_bits(6)) {
    case 0:
      act("A bolt of lightning arcs out from your axe, hitting $N!", ch, 
	NULL, ch->fighting, TO_CHAR);
      act("A bolt of lightning crackles along $n's axe and arcs towards you!",
	ch, NULL, ch->fighting, TO_VICT);
      act("A bolt of lightning shoots out from $n's axe, arcing towards $N!",
	ch, NULL, ch->fighting, TO_NOTVICT);
      obj_cast_spell(gsn_lightning_bolt, ch->level, ch, ch->fighting, obj);
      break;
    }
}

void wear_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{

  if ( ch->class != 10 )          /* ranger */
  {
     send_to_char( "You don't know to use this thing.\n\r", ch );
     unequip_char( ch, obj );
     send_to_char( "Ranger staff slides off from your hand.\n\r", ch );
     obj_from_char( obj );
     obj_to_room( obj, ch->in_room );
     return;
  }

}

void wear_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("You start to bang the coconut shells together.",ch,NULL,NULL,TO_CHAR);
  act("You hear a sound like horses galloping and you mount your steed.", 
	ch, NULL, NULL, TO_CHAR);
  act("$n pretends to mount an invisible horse.",
	ch,NULL,NULL,TO_ROOM); 
}

void entry_prog_coconut(OBJ_DATA *obj)
{
  if (obj->carried_by != NULL)
    if (get_eq_char(obj->carried_by, WEAR_HOLD) == obj)
  act("$n gallops in on his invisible steed, banging two coconuts together.",
	obj->carried_by, NULL, NULL, TO_ROOM);
}  

void greet_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (obj->carried_by != NULL)
    {
      if (get_eq_char(obj->carried_by, WEAR_HOLD) == obj && 
		obj->carried_by != ch)
	act("You hear the sound of galloping horses.", ch, NULL, NULL, TO_CHAR);
    }
  else
    send_to_char("$p beckons with the faint sound of galloping horses.\n\r",
	ch);
}

void get_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("You hold the coconut up to your ear and suddenly you hear 
the faint\n\rroar of galloping horses.\n\r", ch);
  act("$n holds a coconut up to $s ear.", ch, NULL, NULL, TO_ROOM);
}

void remove_prog_coconut(OBJ_DATA *obj, CHAR_DATA *ch)
{
  send_to_char("The sounds of horses fade away.\n\r", ch);
  act("$n pretends to dismount a horse.", ch, NULL, NULL, TO_ROOM);
}

void fight_prog_firegauntlets( OBJ_DATA *obj, CHAR_DATA *ch )
{
int dam;

  if ( get_eq_char( ch, WEAR_WIELD ) != NULL )
	return;
  if ( get_eq_char( ch, WEAR_HANDS ) != obj )
	return;
  if ( IS_NPC(ch ) )
	return;

  if ( number_percent() < 35 )  {
	dam = number_percent()/2 + 30 + ch->level;
	act( "Your gauntlets burns $N's face!", ch, NULL, ch->fighting, TO_CHAR);
	act( "$n's gauntlets burns $N's face!", ch, NULL, ch->fighting, TO_NOTVICT);
	act( "$N's gauntlets burns your face!", ch->fighting, NULL, ch, TO_CHAR);
	damage( ch, ch->fighting, dam/2, gsn_burning_hands, DAM_FIRE, TRUE);
	if ( ch == NULL || ch->fighting == NULL )
	  return;
	fire_effect( ch->fighting, obj->level/2, dam/2, TARGET_CHAR );
  }
  return;
}

void wear_prog_firegauntlets(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your hands warm up by the gauntlets.\n\r", ch );
	return;
}

void remove_prog_firegauntlets(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char( "Your hands cool down.\n\r", ch );
	return;
}

void fight_prog_vorbalblade( OBJ_DATA *obj, CHAR_DATA *ch )
{
CHAR_DATA *victim;

  if ( IS_NPC(ch) ) 
	return;

  if ( get_eq_char( ch, WEAR_WIELD ) != obj )
    return;

  victim = ch->fighting;

  if ( number_percent() < 5 )  {
    send_to_char( 
	"Your weapon swings at your victim's neck without your control!\n\r", ch);
    if ( number_percent() < 40 )  {

if ( IS_SET( ch->in_room->room_flags, ROOM_ARENA ) )      /* ARENA */
{
send_to_char("A shame you can chop off heads in the arena.\n\r",ch);
return;
}
	act( "It makes an huge arc in the air, chopping $N's head OFF!",
	     ch, NULL, victim, TO_CHAR);
	act( "$N's weapon whistles in the air, chopping your head OFF!",
	     ch, NULL, victim, TO_NOTVICT);
	act( "$n's weapon whistles in the air, chopping $N's head OFF!",
	     ch, NULL, victim, TO_ROOM);
	act( "$n is DEAD!!", victim, NULL, NULL, TO_ROOM );
	act( "$n is DEAD!!", victim, NULL, NULL, TO_CHAR );
	raw_kill_org( victim, 3 );
	send_to_char( "You have been KILLED!!\n\r", victim );
	return;
    }
  }
  return;
}

void drop_prog_dice( OBJ_DATA *obj, CHAR_DATA *ch)
{
int result1, result2;
char buf[160];

  if ( obj->value[0] < 4 ) 
    result1 = dice(1, 6);
  else 
    result1 = dice( 1, obj->value[0] );
  if ( obj->value[0] < 4 ) 
    result2 = dice(1, 6);
  else 
    result2 = dice( 1, obj->value[0] );

  if ( obj->value[1] == 0 )  {
    sprintf( buf, "Dice rolls to %d.", result1 );
    act( buf, ch, NULL, NULL, TO_CHAR );
    act( buf, ch, NULL, NULL, TO_ROOM );
    return;
  }
  sprintf( buf, "Two dice roll to %d and %d.", result1, result2 );
  act( buf, ch, NULL, NULL, TO_CHAR );
  act( buf, ch, NULL, NULL, TO_ROOM );
  return;
}

void speech_prog_dog(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{

    CHAR_DATA *dog;
    CHAR_DATA *gch;
    int i;
    char buf[MAX_STRING_LENGTH];
   
   if ( ch->position == POS_FIGHTING)
    {
     send_to_char("You can't call it while you fighting!\n\r",ch);
     return;
    } 

  if (!str_cmp(speech, "yaja")      /* if useing ring */
      &&( (get_eq_char(ch,WEAR_FINGER_L) == obj) || (get_eq_char(ch,WEAR_FINGER_R) ) ) )
    {

     if (IS_NPC(ch))
      return;

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_ELOHIM_DOG)
	{
	  send_to_char("Boy you only have one dog!\n\r",ch);
	  return;
	}
    }

   dog = create_mobile( get_mob_index(MOB_VNUM_ELOHIM_DOG) );

   for (i=0;i < MAX_STATS; i++)
     {
      dog->perm_stat[i] = ch->perm_stat[i];
     }

   dog->max_hit = ch->max_hit;
   dog->hit = dog->max_hit;
   dog->max_mana = ch->max_mana;
   dog->mana = dog->max_mana;
   dog->level = 1;
   dog->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
   for (i=0; i < 3; i++)
   dog->armor[i] = interpolate(ch->level,100,-100);
   dog->armor[3] = interpolate(ch->level,100,0);
   dog->gold = 0;

   sprintf( buf, dog->short_descr, ch->name );
   free_string( dog->short_descr );
   dog->short_descr = str_dup( buf );

   sprintf( buf, dog->long_descr, ch->name );
   free_string( dog->long_descr );
   dog->long_descr = str_dup( buf );
  
   sprintf( buf, dog->description, ch->name );
   free_string( dog->description );
   dog->description = str_dup( buf ); 
  
   char_to_room(dog,ch->in_room);
   send_to_char("Trofast comes jumping out from behind you!\n\r",ch);
   act("Trofast comes jumping out from behind $n",ch,NULL,NULL,TO_ROOM);

   SET_BIT(dog->affected_by, AFF_CHARM);
   dog->master = dog->leader = ch;
   return;  
 }
}
/***************/
void wear_prog_Dermostest(OBJ_DATA *obj, CHAR_DATA *ch)
{
  AFFECT_DATA af;

  if (!is_affected(ch, gsn_sanctuary))
   {
    af.where     = TO_AFFECTS;
    af.type      = gsn_sanctuary;
    af.level     = ch->level;
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char( "You are surrounded by a white aura.\n\r", ch );
    return;                 
   }
}   

void remove_prog_Dermostest(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (is_affected(ch, gsn_sanctuary))
    {
      affect_strip(ch, gsn_sanctuary);
      send_to_char("The white aura around your body fades.\n\r", ch);
    }
}

void fight_prog_Dermostest(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj)
    {
      if (number_percent() < 15)
	{
	send_to_char("A blast of acid is launched from your weapon\n\r",ch);
        act("A blast of acid is send forth from $n's weapon",ch,NULL,NULL,TO_ROOM);
        obj_cast_spell(gsn_acid_blast, ch->level, ch, ch->fighting, obj);
        return;
	}
      else if (number_percent() > 85)
	{ 
      send_to_char("Your weapon glows bright blue\n\r",ch);
      act("$n's weapon start shining in a bright blue aura!",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_cure_critical,ch->level,ch,ch,obj);
      return;
	}
    }
}

void speech_prog_Dermostest(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
  if (!str_cmp(speech, "Eat cheese"))
    {
      send_to_char("A small voice whispers in your ear 'oh no, not the cheese'\n\r",ch);
      act("It seems as if $n is listening to someone.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_haste,ch->level,ch,ch,obj);
      WAIT_STATE(ch, 3 * PULSE_VIOLENCE);
    }
}
/***************/

void drop_prog_Wonderegg(OBJ_DATA *obj, CHAR_DATA *ch)
{

    CHAR_DATA *wolf;
    CHAR_DATA *spider;
    CHAR_DATA *baboon;
    CHAR_DATA *camel;
//    CHAR_DATA *gch;
//    OBJ_DATA *obj;
    int i;
    char buf[MAX_STRING_LENGTH];

      send_to_char("The egg cracks as it hits the ground.\n\r",ch);
      act("The egg cracks as it hits the ground..",ch,NULL,NULL,TO_ROOM);   
    if ( number_percent() < 25 )  
  {
     if (IS_NPC(ch))
      return;
 
   wolf = create_mobile( get_mob_index(MOB_VNUM_WONDERWOLF) );

   for (i=0;i < MAX_STATS; i++)
     {
      wolf->perm_stat[i] = ch->perm_stat[i];
     }

   wolf->max_hit = ch->max_hit*2;
   wolf->hit = wolf->max_hit;
   wolf->max_mana = ch->max_mana;
   wolf->mana = wolf->max_mana;
   wolf->level = ch->level*3/4;
   wolf->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
   for (i=0; i < 3; i++)
   wolf->armor[i] = interpolate(ch->level,100,-100);
   wolf->armor[3] = interpolate(ch->level,100,0);
   wolf->gold = 0;

   sprintf( buf, wolf->short_descr, ch->name );
   free_string( wolf->short_descr );
   wolf->short_descr = str_dup( buf );

   sprintf( buf, wolf->long_descr, ch->name );
   free_string( wolf->long_descr );
   wolf->long_descr = str_dup( buf );
  
   sprintf( buf, wolf->description, ch->name );
   free_string( wolf->description );
   wolf->description = str_dup( buf ); 
  
   char_to_room(wolf,ch->in_room);
   send_to_char("The shell cracks open and a fully grown wolf comes out.\n\r",ch);
   act("A fully grown wolf jumps out from the cracked egg and moves closer to $n.",ch,NULL,NULL,TO_ROOM);

   SET_BIT(wolf->affected_by, AFF_CHARM);
   wolf->master = wolf->leader = ch;
   return;  
   }
  else if ( number_percent() < 50 )  
   {
     if (IS_NPC(ch))
      return;
 
   spider = create_mobile( get_mob_index(MOB_VNUM_WONDERSPIDER) );

   for (i=0;i < MAX_STATS; i++)
     {
      spider->perm_stat[i] = ch->perm_stat[i];
     }

   spider->max_hit = ch->max_hit*2;
   spider->hit = spider->max_hit;
   spider->max_mana = ch->max_mana;
   spider->mana = spider->max_mana;
   spider->level = ch->level*3/4;
   spider->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
   for (i=0; i < 3; i++)
   spider->armor[i] = interpolate(ch->level,100,-100);
   spider->armor[3] = interpolate(ch->level,100,0);
   spider->gold = 0;

   sprintf( buf, spider->short_descr, ch->name );
   free_string( spider->short_descr );
   spider->short_descr = str_dup( buf );

   sprintf( buf, spider->long_descr, ch->name );
   free_string( spider->long_descr );
   spider->long_descr = str_dup( buf );
  
   sprintf( buf, spider->description, ch->name );
   free_string( spider->description );
   spider->description = str_dup( buf ); 
  
   char_to_room(spider,ch->in_room);
   send_to_char("The shell cracks open and a fully grown spider comes out.\n\r",ch);
   act("A fully grown spider climbs out from the cracked egg and moves closer to $n.",ch,NULL,NULL,TO_ROOM);

   SET_BIT(spider->affected_by, AFF_CHARM);
   spider->master = spider->leader = ch;
   return;  
   }
  else if ( number_percent() < 75 )  
   {
     if (IS_NPC(ch))
      return;
 
   baboon = create_mobile( get_mob_index(MOB_VNUM_WONDERBABOON) );

   for (i=0;i < MAX_STATS; i++)
     {
      baboon->perm_stat[i] = ch->perm_stat[i];
     }

   baboon->max_hit = ch->max_hit*3;
   baboon->hit = baboon->max_hit;
   baboon->max_mana = ch->max_mana;
   baboon->mana = baboon->max_mana;
   baboon->level = ch->level*3/4;
   baboon->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
   for (i=0; i < 3; i++)
   baboon->armor[i] = interpolate(ch->level,100,-100);
   baboon->armor[3] = interpolate(ch->level,100,0);
   baboon->gold = 0;

   sprintf( buf, baboon->short_descr, ch->name );
   free_string( baboon->short_descr );
   baboon->short_descr = str_dup( buf );

   sprintf( buf,baboon->long_descr, ch->name );
   free_string( baboon->long_descr );
   baboon->long_descr = str_dup( buf );
  
   sprintf( buf, baboon->description, ch->name );
   free_string( baboon->description );
   baboon->description = str_dup( buf ); 
  
   char_to_room(baboon,ch->in_room);
   send_to_char("The shell cracks open and a fully grown baboon comes out.\n\r",ch);
   act("A fully grown baboon bounces out of the cracked egg and moves closer to $n.",ch,NULL,NULL,TO_ROOM);

   SET_BIT(baboon->affected_by, AFF_CHARM);
   baboon->master = baboon->leader = ch;
   return;  
   }   
  else
   {
     if (IS_NPC(ch))
      return;
 
   camel = create_mobile( get_mob_index(MOB_VNUM_WONDERCAMEL) );

   for (i=0;i < MAX_STATS; i++)
     {
      camel->perm_stat[i] = ch->perm_stat[i];
     }

   camel->max_hit = ch->max_hit;
   camel->hit = camel->max_hit;
   camel->max_mana = ch->max_mana;
   camel->mana = camel->max_mana;
   camel->level = ch->level/2;
   camel->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
   for (i=0; i < 3; i++)
   camel->armor[i] = interpolate(ch->level,100,-100);
   camel->armor[3] = interpolate(ch->level,100,0);
   camel->gold = 0;

   sprintf( buf, camel->short_descr, ch->name );
   free_string( camel->short_descr );
   camel->short_descr = str_dup( buf );

   sprintf( buf,camel->long_descr, ch->name );
   free_string( camel->long_descr );
   camel->long_descr = str_dup( buf );
  
   sprintf( buf, camel->description, ch->name );
   free_string( camel->description );
   camel->description = str_dup( buf ); 
  
   char_to_room(camel,ch->in_room);
   send_to_char("A camelhead pops out of the cracked egg.\n\r",ch);
   send_to_char("A fully grown camel slowly gets on its feet.\n\r",ch);   
   act("A camelhead looks out of the cracked head and stares right at $n.",ch,NULL,NULL,TO_ROOM);
   act("A fully grown camel slowly gets on its feet.",ch,NULL,NULL,TO_ROOM);

   SET_BIT(camel->affected_by, AFF_CHARM);
   camel->master = camel->leader = ch;
   return;  
   } 
 }

bool sac_prog_vebrek(OBJ_DATA *obj, CHAR_DATA *ch)
{
  act("The gods are infuriated!",ch,NULL,NULL,TO_CHAR);
  act("The gods are infuriated!",ch,NULL,NULL,TO_ROOM);
  damage( ch, ch, 200, TYPE_HIT, DAM_HOLY, TRUE);
  act("The gods revoke your fortune!",ch,NULL,NULL,TO_CHAR);
  ch->gold = 0;
  return TRUE; /* Not destroyed */
}

void fight_prog_vebrek(OBJ_DATA *obj, CHAR_DATA *ch)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj)
    {
      switch(number_range(1,3)) {
	case 1:
	send_to_char("{y'{YVebrek{y' the holy avenger{x glows brightly {Rred{x.\n\r",ch);
        act("{y'{YVebrek{y' the holy avenger{x glows brightly {Rred{x.",ch,NULL,NULL,TO_ROOM);
        obj_cast_spell(gsn_ray_of_truth, obj->level, ch, ch->fighting, obj);
        break;
        case 2:
	send_to_char("{y'{YVebrek{y' the holy avenger{x glows brightly {Rred{x.\n\r",ch);
        act("{y'{YVebrek{y' the holy avenger{x glows brightly {Rred{x.",ch,NULL,NULL,TO_ROOM);
        obj_cast_spell(gsn_wrath, obj->level, ch, ch->fighting, obj);
        break;
        case 3:
	send_to_char("{yVebrek{x glows brightly {Bblue{x.\n\r",ch);
        act("{y'{YVebrek{y' the holy avenger{x glows brightly {Bblue{x.",ch,NULL,NULL,TO_ROOM);
        obj_cast_spell(gsn_cure_critical, ch->level, ch, ch, obj);
        break;
       }
}
        return;
}

void speech_prog_vebrek(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
  if (get_eq_char(ch,WEAR_WIELD) == obj)
  {
  if (!str_cmp(speech, "Cure disease"))
    {
      send_to_char("{y'{YVebrek{y' the holy avenger{Y shines{x brightly.\n\r",ch);
      act("{y'{YVebrek{y' the holy avenger {Yshines{x brightly.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_cure_disease, obj->level, ch, ch->fighting, obj);
      WAIT_STATE(ch, PULSE_VIOLENCE*2);
    }
  if (!str_cmp(speech, "Cure blindness"))
    {
      send_to_char("{y'{YVebrek{y' the holy avenger{Y shines{x brightly.\n\r",ch);
      act("{y'{YVebrek{y' the holy avenger {Yshines{x brightly.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_cure_blindness, obj->level, ch, ch->fighting, obj);
      WAIT_STATE(ch, PULSE_VIOLENCE*2);
    }
    if (!str_cmp(speech, "Cure poison"))
    {
      send_to_char("{y'{YVebrek{y' the holy avenger{Y shines{x brightly.\n\r",ch);
      act("{y'{YVebrek{y' the holy avenger {Yshines{x brightly.",ch,NULL,NULL,TO_ROOM);
      obj_cast_spell(gsn_cure_poison, obj->level, ch, ch->fighting, obj);
      WAIT_STATE(ch, PULSE_VIOLENCE*2);
    }
    
    }
  return;
}
