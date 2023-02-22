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
#include "recycle.h"

DECLARE_DO_FUN(  do_look	);

extern char *target_name;

void spell_cobble( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    BUFFER *output;
    char buf[MAX_STRING_LENGTH];
    char ittype[5];
    int i, a;

    if (obj->wear_loc != -1) {
        send_to_char("The item must be carried to be made into footwear.\n\r",ch);
        return;
    }
    act("$n's hands fly with mystic guidance, cutting up $p...",ch,obj,NULL,TO_ROOM);
    act("Your hands fly with mystic guidance, cutting up $p...",ch,obj,NULL,TO_CHAR);
    if ( number_percent() <= ( 10 - ch->level + obj->level ) ) {
        act("And assemble a useless pile of rags!",ch,obj,NULL,TO_CHAR);
        act("And assemble a useless pile of rags!",ch,obj,NULL,TO_ROOM);
        extract_obj(obj);
        return;
    }
     for ( i = 0; ( (i < ( MAX_STRING_LENGTH - 1 )) && (obj->short_descr[i] != '\0') ); i++ )

    xprintf(ittype, "%c%c%c%c", obj->short_descr[i - 3],
                                obj->short_descr[i - 2],
                                obj->short_descr[i - 1],
                                obj->short_descr[i] );

/* 2 lines for testing */
/*    xprintf( buf, "Working on a %s.\n\r", ittype );
      send_to_char( buf, ch );  */

    if ( ( str_cmp(ittype, "robe" ) ) && ( str_cmp(ittype, "hide" ) ) &&
         ( str_cmp(ittype, "pelt" ) ) && ( str_cmp(ittype, "skin" ) ) &&
         ( str_cmp(ittype, "loak" ) ) && ( str_cmp(ittype, "ncho" ) ) ) {
       send_to_char("You can only make footwear from things best described as\n\rrobes, cloaks, ponchos, hides, pelts, or skins.\n\r", ch );
       return;
    }
    if ( !str_cmp(ittype, "loak" ) )
       i -= 6;
    else if ( !str_cmp(ittype, "ncho" ) )
       i -= 7;
    else
       i -= 5;
    output = new_buf();

    add_buf( output, "a pair of " );
    a = 0;
    if ( ( obj->short_descr[0] == 'a' )
      && ( obj->short_descr[1] == ' ' ) )
       a += 2;
    if ( ( obj->short_descr[0] == 'a' )
      && ( obj->short_descr[1] == 'n' )
      && ( obj->short_descr[2] == ' ' ) )
       a += 3;

    for ( ; a <= i; a++ ) {
       xprintf(buf, "%c", obj->short_descr[a] );
       add_buf( output, buf );
    }

    if ( !str_cmp(ittype, "robe" ) )
       add_buf( output, "slippers" );
    if ( !str_cmp(ittype, "loak" ) )
       add_buf( output, "slippers" );
    if ( !str_cmp(ittype, "ncho" ) )
       add_buf( output, "shoes" );
    if ( !str_cmp(ittype, "skin" ) )
       add_buf( output, "shoes" );
    if ( !str_cmp(ittype, "pelt" ) )
       add_buf( output, "boots" );
    if ( !str_cmp(ittype, "hide" ) )
       add_buf( output, "boots" );

    obj->wear_flags = ITEM_WEAR_FEET|ITEM_TAKE;
    if ( obj->item_type != ITEM_ARMOR ) {
       obj->item_type = ITEM_ARMOR;
       if ( ( !str_cmp(ittype, "robe") ) || ( !str_cmp(ittype, "loak") ) )
          a = 1;
       else if ( ( !str_cmp(ittype, "ncho") ) || ( !str_cmp(ittype,"skin") ) )
          a = 2;
       else
          a = 3;
       a += ( obj->level / 10 );
       obj->value[0] = obj->value[1] = obj->value[2] = a;
    }
    obj->value[0] = ( obj->value[0] * ch->pcdata->learned[sn] / 100 );
    obj->value[1] = ( obj->value[1] * ch->pcdata->learned[sn] / 100 );
    obj->value[2] = ( obj->value[2] * ch->pcdata->learned[sn] / 100 );

/* test line */
/*    send_to_char(str_dup( buf_string(output)), ch); */

    if ( !str_cmp(ittype, "pelt" ) )
       add_buf( output, "boots" );
    if ( !str_cmp(ittype, "hide" ) )
       add_buf( output, "boots" );

    obj->wear_flags = ITEM_WEAR_FEET|ITEM_TAKE;
    if ( obj->item_type != ITEM_ARMOR ) {
       obj->item_type = ITEM_ARMOR;
       if ( ( !str_cmp(ittype, "robe") ) || ( !str_cmp(ittype, "loak") ) )
          a = 1;
       else if ( ( !str_cmp(ittype, "ncho") ) || ( !str_cmp(ittype, "skin") ) )
          a = 2;
       else
          a = 3;
       a += ( obj->level / 10 );
       obj->value[0] = obj->value[1] = obj->value[2] = a;
    }
    obj->value[0] = ( obj->value[0] * ch->pcdata->learned[sn] / 100 );
    obj->value[1] = ( obj->value[1] * ch->pcdata->learned[sn] / 100 );
    obj->value[2] = ( obj->value[2] * ch->pcdata->learned[sn] / 100 );

/* test line */
/*    send_to_char( buf_string(output), ch); */

    obj->short_descr = str_dup( buf_string(output));

    act("And assemble $p.",ch,obj,NULL,TO_ROOM);
    act("And assemble $p.",ch,obj,NULL,TO_CHAR);
    free_buf(output);
}




void spell_firewind( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 12 );
    if ( saves_spell( level, victim, DAM_FIRE ) )
        dam /= 2;
    act("$n dissappears in a gust of burning winds.",victim,NULL,NULL,TO_ROOM);
    damage_spell( ch, victim, dam, sn,DAM_FIRE,TRUE );
    return;
}
/*
void spell_multi_missile(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    int dam;
    int num_missile;

    // first strike
    num_missile = ((level/7));

    dam = dice(level/2,3);
    if (saves_spell(level,victim,DAM_ENERGY))
 	dam /= 3;

    switch(number_range(1,5)) {
    case 1:
    act("A {Mpink{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Mpink{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Mpink{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_ENERGY,TRUE); 
	break;
    case 2:
    act("A {Rred{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Rred{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Rred{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_FIRE,TRUE); 
    break;
    case 3: 
    act("A {Cblue{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Cblue{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Cblue{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_COLD,TRUE); 
	break;
    case 4:
    act("A {Wwhite{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Wwhite{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Wwhite{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_HOLY,TRUE); 
	break;
    case 5: 
    act("A {Dblack{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Dblack{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Dblack{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_NEGATIVE,TRUE); 
	break;
    default: 
    act("A {Mpink{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Mpink{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Mpink{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_ENERGY,TRUE); 
	break;
	}

    last_vict = victim;
    num_missile -= 1;   // decrement number of missiles

    // new targets 
    while (num_missile > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict && num_missile > 0)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
	    dam = dice(level/2,3);

	    if (saves_spell(level,tmp_vict,DAM_ENERGY))
		dam /= 3;
    switch(number_range(1,5)) {
    case 1:
    act("A {Mpink{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Mpink{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Mpink{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_ENERGY,TRUE); 
	break;
    case 2:
    act("A {Rred{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Rred{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Rred{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_FIRE,TRUE); 
    break;
    case 3: 
    act("A {Cblue{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Cblue{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Cblue{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_COLD,TRUE); 
	break;
    case 4:
    act("A {Wwhite{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Wwhite{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Wwhite{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_HOLY,TRUE); 
	break;
    case 5: 
    act("A {Dblack{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Dblack{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Dblack{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_NEGATIVE,TRUE); 
	break;
    default: 
    act("A {Mpink{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Mpink{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,victim,TO_CHAR);
    act("A {Mpink{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,victim,TO_VICT);
    damage(ch,victim,dam,sn,DAM_ENERGY,TRUE); 
	break;
	}
	    num_missile -= 1;  // decrement number of missiles 
	  }
	}   // end target searching loop 

	if (!found) // no target found, hit the caster 
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) // no double hits 
	  {
	    return;
	  }

	  last_vict = ch;
	  num_missile -= 1;  // decrement damage
	  if (ch == NULL)
	    return;
	}
    // now go back and find more targets
    }
}
*/
void spell_multi_missile(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict= (CHAR_DATA *) vo;
    bool found;
    int dam;
    int num_missile;
    int chance,save;
    /* first strike */
    num_missile = ((level/6));

    //dam = dice(level/2,4);
/*
    if (saves_spell(level,victim,DAM_ENERGY))
 	dam /= 3;
*/   
   found = FALSE;
   while (num_missile > 0)
    {
     
     chance = number_range(1,100);
     dam = dice(level/2,4);
     save   = number_range(1,2);  
    
//    if (saves_spell(level,victim,DAM_ENERGY)&& save == 1)
// 	dam /= 2;
//    else if (saves_spell(level,victim,DAM_ENERGY) && save == 2)
//       dam /= 2;
    
    
     if ( chance <= 80)
      {
       tmp_vict = victim;
       found = TRUE;
      }

     else if ( chance <=95 )
      { 
       found = FALSE;
      }

     else if ( chance <=100 )
      { 
       tmp_vict = ch;
       found = TRUE;
      }
       
   if (found == TRUE)
   {
    switch(number_range(1,5)) {
    case 1:
    act("A {Mpink{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,tmp_vict,TO_NOTVICT);
    act("A {Mpink{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,tmp_vict,TO_CHAR);
    act("A {Mpink{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,tmp_vict,TO_VICT);
    damage_spell(ch,tmp_vict,dam,sn,DAM_ENERGY,TRUE); 
	break;
    case 2:
    act("A {Rred{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,tmp_vict,TO_NOTVICT);
    act("A {Rred{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,tmp_vict,TO_CHAR);
    act("A {Rred{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,tmp_vict,TO_VICT);
    damage_spell(ch,tmp_vict,dam,sn,DAM_FIRE,TRUE); 
    break;
    case 3: 
    act("A {Cblue{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,tmp_vict,TO_NOTVICT);
    act("A {Cblue{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,tmp_vict,TO_CHAR);
    act("A {Cblue{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,tmp_vict,TO_VICT);
    damage_spell(ch,tmp_vict,dam,sn,DAM_COLD,TRUE); 
	break;
    case 4:
    act("A {Wwhite{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,tmp_vict,TO_NOTVICT);
    act("A {Wwhite{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,tmp_vict,TO_CHAR);
    act("A {Wwhite{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,tmp_vict,TO_VICT);
    damage_spell(ch,tmp_vict,dam,sn,DAM_HOLY,TRUE); 
	break;
    case 5: 
    act("A {Dblack{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,tmp_vict,TO_NOTVICT);
    act("A {Dblack{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,tmp_vict,TO_CHAR);
    act("A {Dblack{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,tmp_vict,TO_VICT);
    damage_spell(ch,tmp_vict,dam,sn,DAM_NEGATIVE,TRUE); 
	break;
    default: 
    act("A {Mpink{w missile erupts from $n's hand and hits $N in the chest.",
        ch,NULL,tmp_vict,TO_NOTVICT);
    act("A {Mpink{w missile flies from your hand and hits $N in the chest.",
	ch,NULL,tmp_vict,TO_CHAR);
    act("A {Mpink{w missile flies from $n's hand and hits you in the chest!",
	ch,NULL,tmp_vict,TO_VICT);
    damage_spell(ch,tmp_vict,dam,sn,DAM_ENERGY,TRUE); 
	break;
	}

  }
  if ( found == FALSE )
   {
    switch(number_range(1,5)) {
    case 1:
    act("A {Mpink{w missile erupts from $n's hand misses $N.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Mpink{w missile flies from your hand misses $N",
	ch,NULL,victim,TO_CHAR);
    act("A {Mpink{w missile flies from $n's hand misses $N!",
	ch,NULL,victim,TO_VICT);
    //damage(ch,victim,dam,sn,DAM_ENERGY,TRUE); 
	break;
    case 2:
    act("A {Rred{w missile erupts from $n's hand misses $N.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Rred{w missile flies from your hand misses $N.",
	ch,NULL,victim,TO_CHAR);
    act("A {Rred{w missile flies from $n's hand misses $N!",
	ch,NULL,victim,TO_VICT);
    //damage(ch,victim,dam,sn,DAM_FIRE,TRUE); 
    break;
    case 3: 
    act("A {Cblue{w missile erupts from $n's hand misses $N.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Cblue{w missile flies from your hand misses $N.",
	ch,NULL,victim,TO_CHAR);
    act("A {Cblue{w missile flies from $n's hand misses $N!",
	ch,NULL,victim,TO_VICT);
    //damage(ch,victim,dam,sn,DAM_COLD,TRUE); 
	break;
    case 4:
    act("A {Wwhite{w missile erupts from $n's hand misses $N.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Wwhite{w missile flies from your hand misses $N.",
	ch,NULL,victim,TO_CHAR);
    act("A {Wwhite{w missile flies from $n's hand misses $N!",
	ch,NULL,victim,TO_VICT);
    //damage(ch,victim,dam,sn,DAM_HOLY,TRUE); 
	break;
    case 5: 
    act("A {Wwhite{w missile erupts from $n's hand misses $N.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Wwhite{w missile flies from your hand misses $N.",
	ch,NULL,victim,TO_CHAR);
    act("A {Wwhite{w missile flies from $n's hand misses $N!",
	ch,NULL,victim,TO_VICT);
    //damage(ch,victim,dam,sn,DAM_NEGATIVE,TRUE); 
	break;
    default: 
    act("A {Wwhite{w missile erupts from $n's hand misses $N.",
        ch,NULL,victim,TO_NOTVICT);
    act("A {Wwhite{w missile flies from your hand misses $N.",
	ch,NULL,victim,TO_CHAR);
    act("A {Wwhite{w missile flies from $n's hand misses $N!",
	ch,NULL,victim,TO_VICT);
    //damage(ch,victim,dam,sn,DAM_ENERGY,TRUE); 
	break;
   }	
}
num_missile -= 1;   /* decrement number of missiles */
 }
return;
}

void spell_ice_ray( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dam;

    dam = dice( level, 12 );

    act("A blue ray from $n's hand strikes $N.",ch,NULL,victim,TO_NOTVICT);
    act("A blue ray shoots from your finger and strikes $N.",ch,NULL,victim,TO_CHAR);
    act("$n points at you sending a chilling ray into your chest.",ch,NULL,victim,TO_VICT);

    if ( !saves_spell( level, victim,DAM_COLD ) )
    {
	act("$n turns blue and shivers.",victim,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 12;
	af.location  = APPLY_STR;
	af.modifier  = -2;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_spell( ch, victim, dam, sn,DAM_COLD ,TRUE);
    return;
}

void spell_ice_storm(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    AFFECT_DATA af;
    int dam;
    int num_blast;

    /* first strike */
    num_blast = level/13;
    act("{CA blue ray from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{CA blue ray shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{C$n points at you sending a chilling ray into your chest.{w",ch,NULL,victim,TO_VICT);

    dam = dice(level/2,10);
	    if (!saves_spell(level,victim,DAM_COLD))
    {
	act("$n turns blue and shivers.",victim,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_spell(ch,victim,dam,sn,DAM_COLD,TRUE);
    last_vict = victim;
    num_blast -= 1;   /* decrement number of blasts */

    /* new targets */
    while (num_blast > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict && num_blast > 0)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
    act("{CA blue ray from $n's hand strikes $N.{w",ch,NULL,tmp_vict,TO_NOTVICT);
    act("{CA blue ray shoots from your finger and strikes $N.{w"
        ,ch,NULL,tmp_vict,TO_CHAR);
    act("{C$n points at you sending a chilling ray into your chest.{w"
        ,ch,NULL,tmp_vict,TO_VICT);

	    dam = dice(level/2,10);
	    if (!saves_spell(level,tmp_vict,DAM_COLD))
    {
	act("$n turns blue and shivers.",tmp_vict,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( tmp_vict, &af );
    }
    else
    {
	dam /= 2;
    }

	    damage_spell(ch,tmp_vict,dam,sn,DAM_COLD,TRUE);
	    num_blast -= 1;  /* decrement number of blasts */
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    return;
	  }

	  last_vict = ch;
	  num_blast -= 1;  /* decrement number of blasts */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}

void spell_fire_storm(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    AFFECT_DATA af;
    int dam;
    int num_blast;

    /* first strike */
    num_blast = level/13;
    act("{RA red ray from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{RA red ray shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{R$n points at you sending a fiery ray into your chest.{w",ch,NULL,victim,TO_VICT);

    dam = dice(level/2,10);
	    if (!saves_spell(level,victim,DAM_COLD))
    {
	act("$n starts to burn up.",victim,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_DEX;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_spell(ch,victim,dam,sn,DAM_FIRE,TRUE);
    last_vict = victim;
    num_blast -= 1;   /* decrement number of blasts */

    /* new targets */
    while (num_blast > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict && num_blast > 0)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
    act("{RA red ray from $n's hand strikes $N.{w",ch,NULL,tmp_vict,TO_NOTVICT);
    act("{RA red ray shoots from your finger and strikes $N.{w"
        ,ch,NULL,tmp_vict,TO_CHAR);
    act("{R$n points at you sending a fiery ray into your chest.{w"
        ,ch,NULL,tmp_vict,TO_VICT);

	    dam = dice(level/2,10);
	    if (!saves_spell(level,tmp_vict,DAM_FIRE))
    {
	act("$n starts to burn up.",tmp_vict,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_DEX;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( tmp_vict, &af );
    }
    else
    {
	dam /= 2;
    }

	    damage_spell(ch,tmp_vict,dam,sn,DAM_FIRE,TRUE);
	    num_blast -= 1;  /* decrement number of blasts */
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    return;
	  }

	  last_vict = ch;
	  num_blast -= 1;  /* decrement number of blasts */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}

void spell_lightning_storm(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    AFFECT_DATA af;
    int dam;
    int num_blast;

    /* first strike */
    num_blast = level/13;
    act("{WA white ray from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{WA white ray shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{W$n points at you sending a shocking ray into your chest.{w",ch,NULL,victim,TO_VICT);

    dam = dice(level/2,10);
	    if (!saves_spell(level,victim,DAM_COLD))
    {
	act("$n start to limp.",victim,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_CON;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_spell(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);
    last_vict = victim;
    num_blast -= 1;   /* decrement number of blasts */

    /* new targets */
    while (num_blast > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict && num_blast > 0)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
    act("{WA white ray from $n's hand strikes $N.{w",ch,NULL,tmp_vict,TO_NOTVICT);
    act("{WA white ray shoots from your finger and strikes $N.{w"
        ,ch,NULL,tmp_vict,TO_CHAR);
    act("{W$n points at you sending a shocking ray into your chest.{w"
        ,ch,NULL,tmp_vict,TO_VICT);

	    dam = dice(level/2,10);
	    if (!saves_spell(level,tmp_vict,DAM_LIGHTNING))
    {
	act("$n starts to limp.",tmp_vict,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_CON;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( tmp_vict, &af );
    }
    else
    {
	dam /= 2;
    }

	    damage_spell(ch,tmp_vict,dam,sn,DAM_LIGHTNING,TRUE);
	    num_blast -= 1;  /* decrement number of blasts */
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    return;
	  }

	  last_vict = ch;
	  num_blast -= 1;  /* decrement number of blasts */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}

void spell_meteor_swarm(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    AFFECT_DATA af;
    int dam;
    int num_blast;

    /* first strike */
    num_blast = level/10;
    act("{yA meteor from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{yA meteor shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{y$n points at you sending a fiery meteor into your chest.{w",ch,NULL,victim,TO_VICT);

    dam = dice(level/2,10);
	    if (!saves_spell(level,victim,DAM_FIRE))
    {
	act("$n get blown back by the force.",victim,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_DEX;
	af.modifier  = -1;
	af.bitvector = 0;
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_spell(ch,victim,dam,sn,DAM_FIRE,TRUE);
    last_vict = victim;
    num_blast -= 1;   /* decrement number of blasts */

    /* new targets */
    while (num_blast > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict && num_blast > 0)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
    act("{yA meteor from $n's hand strikes $N.{w",ch,NULL,tmp_vict,TO_NOTVICT);
    act("{yA meteor shoots from your finger and strikes $N.{w"
        ,ch,NULL,tmp_vict,TO_CHAR);
    act("{y$n points at you sending a fiery meteor into your chest.{w"
        ,ch,NULL,tmp_vict,TO_VICT);

	    dam = dice(level/2,10);
	    if (!saves_spell(level,tmp_vict,DAM_FIRE))
    {
	act("$n gets blown back by the force.",tmp_vict,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_DEX;
	af.modifier  = -1;
	af.bitvector = 0;
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( tmp_vict, &af );
    }
    else
    {
	dam /= 2;
    }

	    damage_spell(ch,tmp_vict,dam,sn,DAM_FIRE,TRUE);
	    num_blast -= 1;  /* decrement number of blasts */
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    return;
	  }

	  last_vict = ch;
	  num_blast -= 1;  /* decrement number of blasts */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}

void spell_divine_word(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    AFFECT_DATA af;
    int dam;
    int num_blast;

    /* first strike */
    num_blast = level/10;

    act("{WA holy ray from $n's hand strikes $N.{w",ch,NULL,victim,TO_NOTVICT);
    act("{WA holy ray shoots from your finger and strikes $N.{w",ch,NULL,victim,TO_CHAR);
    act("{W$n points at you sending a holy ray into your chest.{w",ch,NULL,victim,TO_VICT);

    dam = dice(level/2,10);
	    if (!saves_spell(level,victim,DAM_HOLY))
    {
	act("$n starts tumble.",victim,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_HITROLL;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_spell(ch,victim,dam,sn,DAM_HOLY,TRUE);
    last_vict = victim;
    num_blast -= 1;   /* decrement number of blasts */

    /* new targets */
    while (num_blast > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict && num_blast > 0)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
    act("{WA holy ray from $n's hand strikes $N.{w",ch,NULL,tmp_vict,TO_NOTVICT);
    act("{WA holy ray shoots from your finger and strikes $N.{w"
        ,ch,NULL,tmp_vict,TO_CHAR);
    act("{W$n points at you sending a holy ray into your chest.{w"
        ,ch,NULL,tmp_vict,TO_VICT);

	    dam = dice(level/2,10);
	    if (!saves_spell(level,tmp_vict,DAM_HOLY))
    {
	act("$n starts to tumble.",tmp_vict,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 2;
	af.location  = APPLY_HITROLL;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( tmp_vict, &af );
    }
    else
    {
	dam /= 2;
    }

	    damage_spell(ch,tmp_vict,dam,sn,DAM_HOLY,TRUE);
	    num_blast -= 1;  /* decrement number of blasts */
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    return;
	  }

	  last_vict = ch;
	  num_blast -= 1;  /* decrement number of blasts */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}

/* Vision spell similiar to 'at person look' */

void spell_vision( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *original;

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_HERO)  /* NOT trust */
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_MENTAL))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_ENERGY ) ) )
    {
        send_to_char( "There is a disturbance in the ether.\n\r", ch );
        return;
    }


    act("$n falls into a trance.",ch,NULL,NULL,TO_ROOM);
    send_to_char("Through a cloudy fog, you have a vision.\n\r",ch);
    original = ch->in_room;
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    send_to_char("You feel a shiver down your spine.\n\r",victim);
	do_look(ch,"auto");
    char_from_room(ch);
    char_to_room(ch, original);
}
void spell_restoration( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->hit = UMIN( victim->hit + 275, victim->max_hit );
    update_pos( victim );
    act("$n glows with a blinding light.",ch,NULL,NULL,TO_ROOM);
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "You surge with healing power.\n\r", ch );
    return;
}
void spell_destroy_cursed( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj;
    OBJ_DATA  *obj_next;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int        yesno  = 0;

    for ( obj = victim->carrying; obj; obj = obj_next )
    {
        obj_next = obj->next_content;

	if ( IS_SET( obj->extra_flags, ITEM_NODROP )
	    && obj->wear_loc == WEAR_NONE )
	{
    act( "You convulse as you toss $p to the ground, destroying it.",
		victim, obj, NULL, TO_CHAR );
    act( "$n convulses as $e tosses $p to the ground, destroying it.",
		victim, obj, NULL, TO_ROOM );
	    extract_obj( obj );
	    yesno = 1;
	}
    }

    if ( ch != victim && yesno )
        send_to_char( "Ok.\n\r", ch );
    return;
}
/* Remove Alignment by Thelonius for EnvyMud */
void spell_remove_alignment( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( !IS_SET( obj->extra_flags, ITEM_EVIL		)
    &&   !IS_SET( obj->extra_flags, ITEM_ANTI_GOOD	)
    &&   !IS_SET( obj->extra_flags, ITEM_ANTI_EVIL	)
    &&   !IS_SET( obj->extra_flags, ITEM_ANTI_NEUTRAL	) )
    {
	send_to_char( "Nothing happens.\n\r", ch );
	return;
    }

    if ( number_percent( ) < ch->pcdata->learned[sn] *
	 ( 33 + ( 33 * ( ch->level - obj->level ) / 20 ) ) / 100 )
    {
	REMOVE_BIT( obj->extra_flags, ITEM_EVIL		);
	REMOVE_BIT( obj->extra_flags, ITEM_ANTI_GOOD	);
	REMOVE_BIT( obj->extra_flags, ITEM_ANTI_EVIL	);
	REMOVE_BIT( obj->extra_flags, ITEM_ANTI_NEUTRAL	);
	act( "$p hums briefly, then lies quiet.", ch, obj, NULL, TO_CHAR );
	act( "$p hums briefly, then lies quiet.", ch, obj, NULL, TO_ROOM );
	return;
    }

    SET_BIT( obj->extra_flags, ITEM_NODROP );
    obj->wear_flags = 1;			/* Useless   */
    obj->cost       = 0;			/* Worthless */
    act( "$p blazes brightly, then turns grey.", ch, obj, NULL, TO_CHAR );
    act( "$p blazes brightly, then turns grey.", ch, obj, NULL, TO_ROOM );

    return;

}
void spell_adrenaline_control ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
        return;

    af.type	 = sn;
    af.duration	 = level - 5;
    af.location	 = APPLY_DEX;
    af.modifier	 = 2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location	= APPLY_CON;
    affect_to_char( victim, &af );

    send_to_char( "You have given yourself an adrenaline rush!\n\r", ch );
    act( "$n has given $mself an adrenaline rush!", ch, NULL, NULL,
	TO_ROOM );

    return;
}



void spell_agitation ( int sn, int level, CHAR_DATA *ch, void *vo )
{
                 CHAR_DATA *victim       = (CHAR_DATA *) vo;
    static const int        dam_each [ ] =
    {
	0,
	 0,  0,  0,  0,  0,      12, 15, 18, 21, 24,
	24, 24, 25, 25, 26,      26, 26, 27, 27, 27,
	28, 28, 28, 29, 29,      29, 30, 30, 30, 31,
	31, 31, 32, 32, 32,      33, 33, 33, 34, 34,
	34, 35, 35, 35, 36,      36, 36, 37, 37, 37
    };
		 int        dam;

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam	     = number_range( dam_each[level] / 2, dam_each[level] * 2 );

    if ( saves_spell( level, victim,DAM_ENERGY ) )
      dam /= 2;

    damage_spell( ch, victim, dam, sn, WEAR_NONE ,TRUE);

    return;
}


void spell_awe ( int sn, int level, CHAR_DATA *ch, void *vo )
  {
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim->fighting == ch && !saves_spell( level, victim,DAM_ENERGY ) )
    {
    	victim->last_fought = NULL;
	stop_fighting ( victim, TRUE);
	victim->last_fought = NULL;
	act( "$N is in AWE of you!", ch, NULL, victim, TO_CHAR    );
	act( "You are in AWE of $n!",ch, NULL, victim, TO_VICT    );
	act( "$N is in AWE of $n!",  ch, NULL, victim, TO_NOTVICT );

        if(!is_affected(victim,sn))
        {
         af.type	= sn;
      	 af.level	= victim->level;
      	 af.duration	= ch->level/2;
      	 af.modifier	= 0;
      	 af.bitvector   = 0;
      	 af.location	= 0;
      	 affect_to_char(victim,&af);
        }
    }
    return;
}



void spell_ballistic_attack ( int sn, int level, CHAR_DATA *ch, void *vo )
  {
                 CHAR_DATA *victim       = (CHAR_DATA *) vo;
    static const int        dam_each [ ] =
    {
	 0,
	 3,  4,  4,  5,  6,       6,  6,  7,  7,  7,
	 7,  7,  8,  8,  8,       9,  9,  9, 10, 10,
	10, 11, 11, 11, 12,      12, 12, 13, 13, 13,
	14, 14, 14, 15, 15,      15, 16, 16, 16, 17,
	17, 17, 18, 18, 18,      19, 19, 19, 20, 20
    };
		 int        dam;

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam	     = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_ENERGY ) )
      dam /= 2;

    act( "You chuckle as a stone strikes $N.", ch, NULL, victim,
	TO_CHAR );

    damage_spell( ch, victim, dam, sn, WEAR_NONE,TRUE );

    return;
}


void spell_cell_adjustment ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( is_affected( victim, gsn_poison ) )
    {
	affect_strip( victim, gsn_poison );
	send_to_char( "A warm feeling runs through your body.\n\r", victim );
	act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
    }
    if ( is_affected( victim, gsn_curse  ) )
    {
	affect_strip( victim, gsn_curse  );
	send_to_char( "You feel better.\n\r", victim );
    }
    if ( is_affected( victim, gsn_plague ) )	/* Plague addition by Maniac */
    {
	affect_strip( victim, gsn_plague );
	send_to_char( "Your plague sores seem to disappear.\n\r", victim );
    }
    send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_combat_mind ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if ( victim == ch )
	  send_to_char( "You already understand battle tactics.\n\r",
		       victim );
	else
	  act( "$N already understands battle tactics.",
	      ch, NULL, victim, TO_CHAR );
	return;
    }

    af.type	 = sn;
    af.duration	 = level + 3;
    af.location	 = APPLY_HITROLL;
    af.modifier	 = level / 6;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location	 = APPLY_AC;
    af.modifier	 = - level/2 - 5;
    affect_to_char( victim, &af );

    if ( victim != ch )
        send_to_char( "OK.\n\r", ch );
    send_to_char( "You gain a keen understanding of battle tactics.\n\r",
		 victim );
    return;
}



void spell_complete_healing ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    victim->hit = victim->max_hit;
    update_pos( victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    send_to_char( "Ahhhhhh...You are completely healed!\n\r", victim );
    return;
}



void spell_control_flames ( int sn, int level, CHAR_DATA *ch, void *vo )
{
                 CHAR_DATA *victim       = (CHAR_DATA *) vo;
    static const int        dam_each [ ] =
    {
	 0,
	 0,  0,  0,  0,  0,       0,  0, 16, 20, 24,
	28, 32, 35, 38, 40,      42, 44, 45, 45, 45,
	46, 46, 46, 47, 47,      47, 48, 48, 48, 49,
	49, 49, 50, 50, 50,      51, 51, 51, 52, 52,
	52, 53, 53, 53, 54,      54, 54, 55, 55, 55
    };
		 int        dam;

    if ( !get_eq_char( ch, WEAR_LIGHT ) )
    {
	send_to_char( "You must be carrying a light source.\n\r", ch );
	return;
    }

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam	     = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_ENERGY ) )
        dam /= 2;

    damage_spell( ch, victim, dam, sn, WEAR_NONE,TRUE );

    return;
}


void spell_death_field ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int        dam;
    int        hpch;

    if ( !IS_EVIL( ch ) )
    {
	send_to_char( "You are not evil enough to do that!\n\r", ch);
	return;
    }





    send_to_char( "A black haze emanates from you!\n\r", ch );
    act ( "A black haze emanates from $n!", ch, NULL, ch, TO_ROOM );

    for ( vch = ch->in_room->people; vch; vch = vch_next )
    {
	vch_next = vch->next_in_room;
	
	if (is_safe_spell(ch,vch,TRUE))
	    continue;
	    
	if (vch != ch)
	{
	    hpch = URANGE( 10, ch->hit, 999 );
	    if ( !saves_spell( level, vch,DAM_ENERGY )
		&& (   level >= vch->level + 5
		    && level >= 1 ) )
            {
		vch->hit = vch->hit/2;
		update_pos( vch );
		send_to_char( "The haze envelops you!\n\r", vch );
		act( "The haze envelops $N!", ch, NULL, vch, TO_ROOM );
		multi_hit(vch, ch, TYPE_UNDEFINED);
            }
	    else
	    {
	        dam = number_range( hpch / 16 + 1, hpch / 8 );
		dam *=3.5;
		damage_spell( ch, vch, dam, sn, WEAR_NONE,TRUE );
	    }
	 }
    }
    return;
}



void spell_detonate ( int sn, int level, CHAR_DATA *ch, void *vo )
{
                 CHAR_DATA *victim       = (CHAR_DATA *) vo;
    static const int        dam_each [ ] =
    {
	  0,
	  0,   0,   0,   0,   0,        0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,        0,   0,   0,   0,  75,
	 80,  85,  90,  95, 100,      102, 104, 106, 108, 110,
	112, 114, 116, 118, 120,      122, 124, 126, 128, 130,
	132, 134, 136, 138, 140,      142, 144, 146, 148, 150
    };
		 int        dam;

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam	     = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_OTHER ) )
        dam /= 2;
    damage_spell( ch, victim, dam, sn, WEAR_NONE ,TRUE);
    return;
}


void spell_displacement ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
	return;

    af.type	 = sn;
    af.duration	 = level - 4;
    af.location	 = APPLY_AC;
    af.modifier	 = 4 - level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "Your form shimmers, and you appear displaced.\n\r",
		 victim );
    act( "$N shimmers and appears in a different location.",
	ch, NULL, victim, TO_NOTVICT );
    return;
}



void spell_domination ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
	send_to_char( "Dominate yourself?  You're weird.\n\r", ch );
	return;
    }

    if (   IS_AFFECTED( victim, AFF_CHARM )
	|| IS_AFFECTED( ch,     AFF_CHARM )
	|| level < victim->level
	|| saves_spell( level, victim,DAM_OTHER ) )
        return;

    if ( victim->master )
        stop_follower( victim );
    add_follower( victim, ch );

    af.type	 = sn;
    af.duration	 = number_fuzzy( level / 4 );
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( victim, &af );

    act( "Your will dominates $N!", ch, NULL, victim, TO_CHAR );
    act( "Your will is dominated by $n!", ch, NULL, victim, TO_VICT );
    return;
}



void spell_ectoplasmic_form ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED( victim, AFF_PASS_DOOR ) )
        return;


    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );


/*
    af.type	 = sn;
    af.duration	 = number_fuzzy( level / 4 );
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );
*/
    send_to_char( "You turn translucent.\n\r", victim );
    act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM );
    return;

}



void spell_ego_whip ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim,DAM_OTHER ) )
        return;

    af.type	 = sn;
    af.duration	 = level;
    af.location	 = APPLY_HITROLL;
    af.modifier	 = -7;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location	 = APPLY_SAVING_SPELL;
    af.modifier	 = 2;
    affect_to_char( victim, &af );

    af.location	 = APPLY_AC;
    af.modifier	 = level;
    affect_to_char( victim, &af );

    act( "You ridicule $N about $S childhood.", ch, NULL, victim, TO_CHAR    );
    send_to_char( "Your ego takes a beating.\n\r", victim );
    act( "$N's ego is crushed by $n!",          ch, NULL, victim, TO_NOTVICT );

    return;
}



void spell_energy_containment ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
        return;

    af.type	 = sn;
    af.duration	 = level / 2 + 7;
    af.modifier	 = -level / 5;
    af.location  = APPLY_SAVING_SPELL;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You can now absorb some forms of energy.\n\r", ch );
    return;
}



void spell_enhanced_strength ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
        return;

    af.type	 = sn;
    af.duration	 = level;
    af.location	 = APPLY_STR;
    af.modifier	 = 1 + ( level >= 15 ) + ( level >= 25 );
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You are HUGE!\n\r", victim );
    return;
}



void spell_flesh_armor ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
        if (victim == ch)
          send_to_char("Your skin is already as hard as a steel.\n\r",ch);
        else
          act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.type	 = sn;
    af.duration	 = level;
    af.location	 = APPLY_AC;
    af.modifier	 = -40;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "Your flesh turns to steel.\n\r", victim );
    act( "$N's flesh turns to steel.", ch, NULL, victim, TO_NOTVICT);
    return;
}




void spell_inflict_pain ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage_spell( ch, (CHAR_DATA *) vo, dice( 2, 10 ) + level / 2, sn,WEAR_NONE,TRUE );
    return;
}



void spell_intellect_fortress ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *gch;
    AFFECT_DATA af;

    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( !is_same_group( gch, ch ) || is_affected( gch, sn ) )
	    continue;

	send_to_char( "A virtual fortress forms around you.\n\r", gch );
	act( "A virtual fortress forms around $N.", gch, NULL, gch, TO_ROOM );

	af.type	     = sn;
	af.duration  = 24;
	af.location  = APPLY_AC;
	af.modifier  = -40;
	af.bitvector = 0;
	affect_to_char( gch, &af );
    }
    return;
}



void spell_lend_health ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int        hpch;

    if ( ch == victim )
    {
	send_to_char( "Lend health to yourself?  What a weirdo.\n\r", ch );
	return;
    }
    hpch = UMIN( 50, victim->max_hit - victim->hit );
    if ( hpch == 0 )
    {
	act( "Nice thought, but $N doesn't need healing.", ch, NULL,
	    victim, TO_CHAR );
	return;
    }
    if ( ch->hit-hpch < 50 )
    {
	send_to_char( "You aren't healthy enough yourself!\n\r", ch );
	return;
    }
    victim->hit += hpch;
    ch->hit     -= hpch;
    update_pos( victim );
    update_pos( ch );

    act( "You lend some of your health to $N.", ch, NULL, victim, TO_CHAR );
    act( "$n lends you some of $s health.",     ch, NULL, victim, TO_VICT );

    return;
}



void spell_levitation ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED( victim, AFF_FLYING ) )
        return;

    af.type	 = sn;
    af.duration	 = level + 3;
    af.location	 = APPLY_NONE;
    af.modifier	 = 0;
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );

    send_to_char( "Your feet rise off the ground.\n\r", victim );
    act( "$n's feet rise off the ground.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_mental_barrier ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
        return;

    af.type	 = sn;
    af.duration	 = 24;
    af.location	 = APPLY_AC;
    af.modifier	 = -20;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You erect a mental barrier around yourself.\n\r",
		 victim );
    return;
}



void spell_mind_thrust ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage_spell( ch, (CHAR_DATA *) vo, dice( 1, 10 ) + level / 2, sn, WEAR_NONE,TRUE);
    return;
}



void spell_project_force ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage_spell( ch, (CHAR_DATA *) vo, dice( 4, 6 ) + level, sn, WEAR_NONE,TRUE);
    return;
}



void spell_psionic_blast ( int sn, int level, CHAR_DATA *ch, void *vo )
{
                 CHAR_DATA *victim       = (CHAR_DATA *) vo;
    static const int        dam_each [ ] =
    {
	  0,
	  0,   0,   0,   0,   0,        0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,        0,  45,  50,  55,  60,
	 64,  68,  72,  76,  80,       82,  84,  86,  88,  90,
	 92,  94,  96,  98, 100,      102, 104, 106, 108, 100,
	112, 114, 116, 118, 120,      122, 124, 126, 128, 130
    };
		 int        dam;

    level    = UMIN( level, sizeof( dam_each ) / sizeof( dam_each[0] ) - 1 );
    level    = UMAX( 0, level );
    dam	     = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_OTHER ) )
        dam /= 2;

    damage_spell( ch, victim, dam, sn, WEAR_NONE,TRUE );

    return;
}



void spell_psychic_crush ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage_spell( ch, (CHAR_DATA *) vo, dice( 3, 5 ) + level, sn, WEAR_NONE,TRUE );
    return;
}



void spell_psychic_drain ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim,DAM_OTHER ) )
        return;

    af.type	 = sn;
    af.duration	 = level / 2;
    af.location	 = APPLY_STR;
    af.modifier	 = -1 - ( level >= 10 ) - ( level >= 20 ) - ( level >= 30 );
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You feel drained.\n\r", victim );
    act( "$n appears drained of strength.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_psychic_healing ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice( 3, 6 ) + 2 * level / 3 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );

    send_to_char( "You feel better!\n\r", victim );
    return;
}



void spell_share_strength ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
	send_to_char( "You can't share strength with yourself.\n\r", ch );
	return;
    }
    if ( is_affected( victim, sn ) )
    {
	act( "$N already shares someone's strength.", ch, NULL, victim,
	    TO_CHAR );
	return;
    }
    if ( get_curr_stat(ch,STAT_STR) <= 5 )
    {
	send_to_char( "You are too weak to share your strength.\n\r", ch );
	return;
    }

    af.type	 = sn;
    af.duration	 = level;
    af.location	 = APPLY_STR;
    af.modifier	 =  1 + ( level >= 20 ) + ( level >= 30 );
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.modifier	 = -1 - ( level >= 20 ) - ( level >= 30 );
    affect_to_char( ch,     &af );

    act( "You share your strength with $N.", ch, NULL, victim, TO_CHAR );
    act( "$n shares $s strength with you.",  ch, NULL, victim, TO_VICT );
    return;
}



void spell_thought_shield ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
        return;

    af.type	 = sn;
    af.duration	 = level;
    af.location	 = APPLY_AC;
    af.modifier	 = -20;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You have created a shield around yourself.\n\r", ch );
    return;
}

void spell_ultrablast ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int        dam,extra;
    int        hpch;

       vch_next = vch->next_in_room;
   extra = 0;
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
	 extra++;
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
	    hpch = UMAX( 10, ch->hit );
	    ch->mana -= 5 + (extra * 3);
	    dam  = 300 + number_range( hpch / 4+1, hpch / 2 );
	    dam = dam * (1 + (extra / 10));
	    if ( saves_spell( level, vch,DAM_OTHER ) )
	        dam /= 2;
	    damage_spell( ch, vch, dam, sn, WEAR_NONE,TRUE );
	if  ( (ch->mana < 5) || (extra > 20))
	 break;
	}
    return;
}

void spell_animate_dead_2( int sn, int level, CHAR_DATA *ch, void *vo)
/* Animate Dead by Garion */
{
    CHAR_DATA   *zombie;
    OBJ_DATA    *obj;
    CHAR_DATA *gch;
     int numcharmie,extramana,chance, boost;
 AFFECT_DATA af;

    obj = get_obj_here( ch, NULL, "corpse");

    if ( obj == NULL )
    {
        send_to_char("There is no corpse here.\n\r", ch);
        return;
    }

     if ( !IS_NPC(ch) &&   ch->level < skill_table[gsn_undead_mastery].skill_level[ch->class] )
     boost = 0; 
     else
     boost = (ch->pcdata->learned[gsn_undead_mastery]);
     	 
chance = get_skill(ch,sn);
chance += ch->level;
chance -= obj->level;

if (number_percent( ) > chance)
 {
  extract_obj( obj );
  send_to_char("The corpse was too power full to animate for you.\n\r", ch);
  check_improve(ch,gsn_undead_mastery,FALSE,1);
  return;
 } 

    

  if (is_affected(ch, sn))
    {
      send_to_char("You cannot create zombies right now.\n\r", ch);
      return;
    }

    ch->charmie = 0;
    extramana = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && IS_AFFECTED(gch,AFF_CHARM))
	  {
	    ch->charmie++;
	    extramana += 10;
	  }  
    }


    if((ch->class == CLASS_MAL) || (ch->class == CLASS_NEC)) 
	numcharmie = ch->level/10;
     else
        numcharmie = ch->level/15;

    if(numcharmie < 0)
	numcharmie = 1;

    if(ch->charmie >= numcharmie)
	return;

    ch->mana -= extramana;


    if ( obj->item_type != ITEM_CORPSE_NPC )
    {
        send_to_char("You cannot animate that.\n\r", ch);
        return;
    }
    else 
    {
   
        OBJ_DATA *equip, *equip_next;
	check_improve(ch,gsn_undead_mastery,TRUE,1);
        zombie = create_mobile( get_mob_index( MOB_VNUM_ZOMBIE ) );
        //SET_BIT( zombie->act, ACT_PET );
        SET_BIT( zombie->affected_by, AFF_CHARM );
        char_to_room( zombie, ch->in_room );
     if (obj->level < 10)
        zombie->level = ( 10);
     else if (obj->level < 15)
        zombie->level = ( 12);
     else 
      zombie->level = (obj->level - 5 + (boost/20));

     zombie->max_hit = ( zombie->level *60 * (1 + (float) boost/100) );
     zombie->hit = zombie->max_hit;
     zombie->armor[AC_BASH] = -50 * (1 + (float) boost/200);
     zombie->armor[AC_SLASH] = -10 * (1 + (float) boost/200);
     zombie->armor[AC_PIERCE] = -100 * (1 + (float) boost/200);
     zombie->armor[AC_EXOTIC] = -25 * (1 + (float) boost/200);
     zombie->hitroll       = level/2 * (1 + (float) boost/200);
     zombie->damroll       = level/2 * (1 + (float) boost/200);
     zombie->damage[DICE_NUMBER] = level/20 * (1 + (float) boost/100);
     zombie->damage[DICE_TYPE] = level/20 * (1 + (float) boost/100);

        for ( equip = obj->contains; equip != NULL; equip = equip_next )
        {
            equip_next = equip->next_content;
            obj_from_obj( equip );
            obj_to_char( equip, zombie );
        }

        extract_obj( obj );
        if ( ch->alignment >= -1000 )
        {
            ch->alignment = UMAX( -1000, ch->alignment - 200 );
        }

        send_to_char("You call upon dark powers to animate the corpse.\n\r",
                ch);
        act("$n calls down evil spirits to inhabit the corpse.", ch, NULL,
                NULL, TO_ROOM);
        add_follower( zombie, ch );
        zombie->leader = ch;
        interpret( zombie, "wear all" );
  af.where              = TO_AFFECTS;
  af.type               = sn;
  af.level              = level;
  af.duration           = 5;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

        return;
    }
}

void spell_rock_flesh( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA  *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
        if (victim == ch)
          send_to_char("Your skin is already as hard as a rock.\n\r",ch);
        else
          act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
        return;
    }

	af.type      = sn;
	af.duration  = level;
	af.location  = APPLY_AC;
	af.modifier  = -50;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	send_to_char( "Your flesh turns to rock.\n\r", victim );
	act( "$n's flesh turns to rock.", victim, NULL, NULL, TO_ROOM );
	return;
}

void spell_summon_dead( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *zombie;
        AFFECT_DATA af;
        int boost;
        
  if (is_affected(ch,sn))
    {
      send_to_char("{cYou lack the power to summon another zombie right now.{x\n\r",ch);
      return;
    }
    
     if ( !IS_NPC(ch) &&   ch->level < skill_table[gsn_undead_mastery].skill_level[ch->class] )
     boost = 0; 
     else
     boost = (ch->pcdata->learned[gsn_undead_mastery]);
    check_improve(ch,gsn_undead_mastery,TRUE,1);

	zombie = create_mobile( get_mob_index( MOB_VNUM_ZOMBIE ) );
//	SET_BIT( zombie->act, ACT_PET);			/* set some     */
	SET_BIT( zombie->affected_by, AFF_CHARM );	/* stuff	*/
	char_to_room (zombie, ch->in_room);		/* get it here	*/
	zombie->level = number_range( 10, 20 )+ ((float) boost*ch->level/100);		/* setup level	*/
	zombie->max_hit = (ch->max_hit*2 * (1 + (float)boost/100));	/* give it hp	*/
	zombie->hit = zombie->max_hit;		/* give it full health	*/
	zombie->armor[AC_BASH] = -100 * (1 + (float)boost/100);
	zombie->armor[AC_SLASH] = -20 * (1 + (float)boost/100);
	zombie->armor[AC_PIERCE] = -200 * (1 + (float)boost/100);
	zombie->armor[AC_EXOTIC] = -50 * (1 + (float)boost/100);
	zombie->hitroll       = level/2 * (1 + (float)boost/100);
	zombie->damroll       = level/2 * (1 + (float)boost/100);
	zombie->damage[DICE_NUMBER] = level/20 * (1 + (float)boost/100);
        zombie->damage[DICE_TYPE] = level/15 * (1 + (float)boost/100);

	send_to_char("You call upon the dark powers and summon a dead warrior.\n\r", ch);
	act( "$n calls upon dark powers to bring back a dead warrior.", ch, NULL, NULL, TO_ROOM );
	add_follower( zombie, ch);
	zombie->leader = ch;
  af.where              = TO_AFFECTS;
  af.type               = sn;
  af.level              = level;
  af.duration           = 40;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

	return;
}


/* By Wynn */
void spell_animal_essence( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA 	*victim = (CHAR_DATA *) vo;
	AFFECT_DATA	af;

	if ( !IS_AFFECTED( victim, AFF_FLYING ) )
		{
		af.where		 = TO_AFFECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = AFF_FLYING;
		affect_to_char( victim, &af );
		send_to_char( "Your feet rise of the ground.\n\r", victim );
		act( "$n's feet rise off the ground.", ch, NULL, victim, TO_NOTVICT );
		}

	if ( !CAN_DETECT( victim, DETECT_HIDDEN ) )
		{
		af.where 	 = TO_DETECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = DETECT_HIDDEN;
		affect_to_char( victim, &af );
		send_to_char( "Your awarness improves.\n\r", victim );
		}

	if ( !CAN_DETECT( victim, DETECT_INVIS ) )
		{
		af.where		 = TO_DETECTS;
		af.type		 = sn;
		af.level		 = level;
		af.duration  = level/2;
		af.location  = APPLY_NONE;
		af.modifier  = 0;
		af.bitvector = DETECT_INVIS;
		affect_to_char( victim, &af );
		send_to_char( "Your eyes tingle with detect invisibility.\n\r", victim );
		}
	return ;
}


/*Airius WWW*/
void spell_drain_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

/*
	if(IS_GOOD(ch))
	{
		send_to_char("You are far too nice to use such majiks.\n\r",ch);
		return ;
	}

	if(!IS_EVIL(ch))
	{
		send_to_char("You are not quite wicked enough to do that.\n\r",ch);
		return ;
	}*/
	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
/*		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{*/


		  if(IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC))
		  {
			send_to_char("That weapon is already quite evil.\n\r", ch);
			return ;
		  }
		  if(IS_OBJ_STAT(obj,ITEM_BLESS))
		  {
			send_to_char("That weapon is too holy to be touched by your majiks.\n\r", ch);
			return ;
		  }
/*		  if(!IS_OBJ_STAT(obj,ITEM_EVIL))
			SET_BIT(obj->extra_flags, ITEM_EVIL);
		  if(!IS_OBJ_STAT(obj,ITEM_ANTI_GOOD))
			SET_BIT(obj->extra_flags, ITEM_ANTI_GOOD);
		  if(!IS_OBJ_STAT(obj,ITEM_ANTI_NEUTRAL))
			SET_BIT(obj->extra_flags, ITEM_ANTI_NEUTRAL);
*/
		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level/2;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_VAMPIRIC;
		  affect_to_obj(obj, &af);

		  act("$p becomes dark and evil.",ch,obj,NULL,TO_ALL);
		  return ;
		}
//	}
	return ;
}


/*Airius WWW*/
void spell_empower_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
/*		if(obj->value[0]!=WEAPON_SWORD 
                 && obj->value[0]!=WEAPON_DAGGER 
                 && obj->value[0]!=WEAPON_SPEAR 
                 && obj->value[0]!=WEAPON_AXE 
                 && obj->value[0]!=WEAPON_POLEARM 
                 && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else 
		{*/
		  if(IS_WEAPON_STAT(obj,WEAPON_SHOCKING))
		  {
			send_to_char("That weapon is already imbued with power.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_SHOCKING;
		  affect_to_obj(obj, &af);

		  act("$p sparks with electricity.",ch,obj,NULL,TO_ALL);
		  return ;
		}
//	}
	return ;
}


/*Airius WWW*/
void spell_flame_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
/*		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && 
obj->value[0]!=WEAPON_EXOTIC && obj->value[0]!=WEAPON_MACE && obj->value[0]!=WEAPON_LANCE && 
obj->value[0]!=WEAPON_WHIP)
		 return ;
		else 
		{*/
		  if(IS_WEAPON_STAT(obj,WEAPON_FLAMING))
		  {
			send_to_char("That weapon is already flaming.\n\r", ch);
			return ;
		  }

		  if(IS_WEAPON_STAT(obj,WEAPON_FROST))
		  {
			send_to_char("That weapon is too cold to accept the magic.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_FLAMING;
		  affect_to_obj(obj, &af);

		  act("$p gets a fiery aura.",ch,obj,NULL,TO_ALL);
		  return ;
		}
//	}
	return ;
}

/*Airius WWW*/
void spell_frost_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
/*		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && 
obj->value[0]!=WEAPON_EXOTIC && obj->value[0]!=WEAPON_WHIP && 
obj->value[0]!=WEAPON_MACE && obj->value[0]!=WEAPON_LANCE)
		 return ;
		else
		{*/
		  if(IS_WEAPON_STAT(obj,WEAPON_FROST))
		  {
			send_to_char("That weapon is already wickedly cold.\n\r", ch);
			return ;
		  }
		  if(IS_WEAPON_STAT(obj,WEAPON_FLAMING))
		  {
			send_to_char("That weapon is too warm to accept the magic.\n\r", ch);
			return ;
		  }

		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_FROST;
		  affect_to_obj(obj, &af);

		  act("$p grows wickedly cold.",ch,obj,NULL,TO_ALL);
		  return ;
		}
//	}
	return ;
}


/* by Airius WWW */
void spell_holy_aura( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!IS_GOOD(victim))
	{
		if(ch == victim)
			send_to_char("You are not holy enough to cast this spell.\n\r", ch);
		else
			send_to_char("They are too unrighteous!\n\r", ch);
		return ;
	}
	if(!is_affected(victim,sn))
	{
		af.where      = TO_AFFECTS;
		af.type		  = sn;
		af.level		  = level;
		af.duration	  = level/3;
		af.location	  = APPLY_AC;
		af.bitvector  = 0;
		af.modifier	  = -level;
		affect_to_char( victim, &af);
		af.where		  = TO_IMMUNE;
		af.modifier	  = 0;
		af.location	  = APPLY_NONE;
		af.bitvector	  = IMM_NEGATIVE;
		affect_to_char( victim, &af);
		send_to_char("You are surrounded by a gold aura.\n\r",victim);
		act("$n is surrounded with a gold aura.", ch, NULL, victim, TO_NOTVICT);
	}
	else
	{
		if(ch == victim)
			send_to_char("You are already protected by divine magic.\n\r", ch);
		else
			send_to_char("They are already protected.\n\r", ch);
		return ;
	}

	return ;
}


/* by Airius  WWW  */
void spell_illusions_granduer( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;
	 AFFECT_DATA af;

	 if(!is_affected(victim,sn))
	 {
	 af.where       = TO_AFFECTS;
	 af.type	= sn;
	 af.level	= level;
	 af.duration	= level*5;
	 af.location	= APPLY_HIT;
	 af.modifier    = level;
	 af.bitvector	= 0;
	 affect_to_char( victim, &af );
         af.modifier    = level*5;
	 af.location    = APPLY_MANA;
	 affect_to_char( victim, &af );
	 send_to_char( "You appear mightier then you are!\n\r", victim );
	 act( "$n grows in stature and appears quite mighty now!\n\r", victim, NULL, NULL, TO_ROOM );
	 }
	 else
		send_to_char("You are having delusions of granduer now.\n\r", victim );
	 return ;
}

/* by Airius */
void spell_improved_phantasm( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;

	dam = dice(level, 9);
	if(saves_spell( level, victim, DAM_ENERGY))
		dam = dam/3;
    damage_spell( ch, victim, dam, sn,DAM_ENERGY,TRUE );

}


/* by Airius */
void spell_magic_resistance( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/10+1;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_MAGIC;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from magic.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from magic.\n\r", victim);

	return ;
}


/* by Airius */
void spell_phantasmal_force( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;

	dam = dice(level, 4);
	if(saves_spell( level, victim, DAM_ENERGY))
		dam = 0;
    damage_spell( ch, victim, dam, sn,DAM_ENERGY,TRUE );

}


/* by Airius */
void spell_protection_fire( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/2;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_FIRE;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from flames.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from fire.\n\r", victim);

	return ;
}

/* by Airius */
void spell_protection_cold( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/2;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_COLD;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from cold.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from cold.\n\r", victim);

	return ;
}

/* by Airius */
void spell_protection_lightning( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/2;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_LIGHTNING;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from lightning.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from lightning.\n\r", victim);

	return ;
}

void spell_poison_immunity( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_IMMUNE;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/4+10;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = IMM_POISON;
	affect_to_char(victim, &af);
	send_to_char( "You are immune to all forms of poison.\n\r", victim);
	}
	else
		send_to_char( "You are already immune to poison.\n\r", victim);

	return ;
}

void spell_plague_immunity( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_IMMUNE;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/4+10;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = IMM_DISEASE;
	affect_to_char(victim, &af);
	send_to_char( "You are immune to all forms of disease.\n\r", victim);
	}
	else
		send_to_char( "You are already immune to disease.\n\r", victim);

	return ;
}

void spell_rage ( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA	*victim = (CHAR_DATA *)	vo;
	AFFECT_DATA	af;

	if ( is_affected( victim, sn ) )
	return ;
	af.where		= TO_AFFECTS;
	af.type		= sn;
	af.level	 	= level;
	af.duration = level / 4;
	af.location = APPLY_DAMROLL;
	af.modifier = level / 5;
	af.bitvector = 0;
	affect_to_char( victim, &af );

	af.location = APPLY_AC;
	af.modifier = level*3;
	affect_to_char( victim, &af );

	af.location = APPLY_HIT;
	af.modifier = level/2;
	affect_to_char( victim, &af );
	victim->hit = UMIN( victim->hit + level*2, victim->max_hit );
	update_pos( victim );

	if (ch != victim )
	send_to_char( "Ok.\n\r", ch );
	send_to_char( "You feel enraged!\n\r", victim );
	return ;
}

void spell_rejuvinate( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA 	*victim = (CHAR_DATA *) vo;

	victim->hit = victim->max_hit;
	update_pos( victim );
	send_to_char( "You feel rejuvinated...\n\r", victim );
	if ( ch != victim )
		send_to_char( "Ok.\n\r", ch );
	if ( is_affected( victim, gsn_curse ) )
		{
		affect_strip( victim, gsn_curse );
		send_to_char( "A warm feeling fills your body.\n\r", victim);
		act( "$N looks better.", ch, NULL, victim, TO_NOTVICT );
		}

	if ( is_affected( victim, gsn_poison ) )
		{
		affect_strip( victim, gsn_poison );
		send_to_char( "You feel better.\n\r", victim);
		}
	return ;
}

void spell_resist_poison( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(!is_affected(victim,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/4+10;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_POISON;
	affect_to_char(victim, &af);
	send_to_char( "You are protected from all forms of poison.\n\r", victim);
	}
	else
		send_to_char( "You are already protected from poison.\n\r", victim);

	return ;
}



void spell_rune_edge( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{
	/*	
if(IS_WEAPON_STAT(obj,WEAPON_SHARP)||IS_WEAPON_STAT(obj,WEAPON_VORPAL))
		  {
			send_to_char("That weapon is already quite sharp.\n\r", ch);
			return ;
		  }
*/
		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_SHARP;
		  affect_to_obj(obj, &af);

		  act("$p looks newly honed.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}


void spell_runic_blade( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	obj=(OBJ_DATA *) vo;

	if(obj->item_type != ITEM_WEAPON)
	{
		send_to_char("You can only target sharp weapons.\n\r",ch);
		return ;
	}
	else
	{
		if(obj->value[0]!=WEAPON_SWORD && obj->value[0]!=WEAPON_DAGGER &&
			obj->value[0]!=WEAPON_SPEAR && obj->value[0]!=WEAPON_AXE &&
			obj->value[0]!=WEAPON_POLEARM && obj->value[0]!=WEAPON_EXOTIC)
		 return ;
		else
		{
/*		  if(IS_WEAPON_STAT(obj,WEAPON_VORPAL) ||
IS_WEAPON_STAT(obj,WEAPON_SHARP))
		  {
			send_to_char("That weapon is already magically sharp.\n\r", ch);
			return ;
		  }
*/
		  af.where    = TO_WEAPON;
		  af.type     = sn;
		  af.level    = level/2;
		  af.duration = level;
		  af.location = 0;
		  af.modifier = 0;
		  af.bitvector= WEAPON_VORPAL;
		  affect_to_obj(obj, &af);

		  act("$p gleams with magical strength.",ch,obj,NULL,TO_ALL);
		  return ;
		}
	}
	return ;
}

void spell_fire_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
         CHAR_DATA *victim = (CHAR_DATA *) vo;
         AFFECT_DATA af;

         if ( is_affected( victim, sn ))
         {
        if (victim == ch)
          send_to_char("You already have a fire shield.\n\r",ch);
        else
          act("$N already has a shield of fire.",
                        ch,NULL,victim,TO_CHAR);
                  return;
         }

         if (is_affected(victim,skill_lookup("chill shield")))
         {    if (!check_dispel(level,victim,skill_lookup("chill shield")))
        {
                 if (victim != ch)
                          send_to_char("Spell failed.\n\r",ch);
                 send_to_char("Your chill shield is destroyed.\n\r",victim);
                 return;
        }
                  send_to_char("You failed!", ch);
                  return;
         }

         af.where     = TO_AFFECTS;
         af.type      =  gsn_fire_shield;
         af.level     = level;
         if (victim == ch)
         af.duration  = level/5;
         else
         af.duration  = level/10;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = 0;
         affect_to_char( victim, &af );
         af.where         = TO_RESIST;
         af.bitvector = RES_COLD;
         affect_to_char( victim, &af );
         af.where         = TO_VULN;
         af.bitvector = VULN_FIRE;
         send_to_char( "You are surrounded by a fire shield.\n\r", victim);
         act("$n is surrounded by a fire shield.",victim,NULL,NULL,TO_ROOM);
         if ( ch != victim )
                  send_to_char( "Ok.\n\r", ch );
         return;
}

void spell_chill_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
         CHAR_DATA *victim = (CHAR_DATA *) vo;
         AFFECT_DATA af;

         if ( is_affected( victim, sn ))
         {
        if (victim == ch)
          send_to_char("You already have a chill shield.\n\r",ch);
        else
   act("$N already has a shield of icy magic.",
                        ch,NULL,victim,TO_CHAR);
                  return;
         }

         if (is_affected(victim,skill_lookup("fire shield")))
         {
        if (!check_dispel(level,victim,skill_lookup("fire shield")))
        {
                 if (victim != ch)
                          send_to_char("Spell failed.\n\r",ch);
                 send_to_char("Your fire shield is destroyed.\n\r",victim);
                 return;
        }
                  act("$n's fire shield is snuffed out.",victim,NULL,NULL,TO_ROOM);
                  return;
         }

         af.where     = TO_AFFECTS;
         af.type      = gsn_chill_shield;
         af.level     = level;
         if (victim == ch)
                af.duration  = level/5;
         else
                af.duration  = level/10;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = 0;
         affect_to_char( victim, &af );
         af.where         = TO_RESIST;
         af.bitvector = RES_FIRE;
         affect_to_char( victim, &af );
         af.where         = TO_VULN;
         af.bitvector = VULN_COLD;
  send_to_char( "You are surrounded by an icy aura.\n\r", victim );
         act("$n is surrounded by an icy aura.",victim,NULL,NULL,TO_ROOM);
         if ( ch != victim )
                  send_to_char( "Ok.\n\r", ch );
         return;
}
void spell_extension( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf, *afpaf;
    char buf[MAX_STRING_LENGTH];

    if (obj->wear_loc != -1) {
        send_to_char("The item must be carried to be extended.\n\r",ch);
        return;
    }
    for ( afpaf = paf = obj->affected; paf != NULL; paf = paf->next ) {
        if ( paf->duration < 0 )
                continue;
        if ( afpaf->duration < 0 ) {
                afpaf = paf;
                continue;
        }
    if ( afpaf->duration > paf->duration )
                afpaf = paf;
    }
    if ( ( afpaf != NULL ) && ( afpaf->duration > 0 ) ) {
        afpaf->duration += dice(((ch->pcdata->learned[sn] * level ) / 100), 10);
        if ( afpaf->duration > 9999 )
            afpaf->duration = 9999;
        xprintf( buf, "The %s affect seems steadier.\n\r",skill_table[afpaf->type].name );
        send_to_char( buf, ch );
    }
    else
        act("You couldn't find any effect on $p to extend.",ch,obj,NULL,TO_CHAR);
    return;

}


void spell_soul_coil( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
 /*Aldon Kingdom spell Fate of Kingdoms*/
 CHAR_DATA *victim = (CHAR_DATA *) vo;

  if (victim == ch)
    {
      send_to_char("You would implode if you tried to soul coil yourself.\n\r",ch);
      return;
    }

  if (victim->mana < 150)
   {
    ch->mana += victim->mana;
    victim->mana = 0;
   }
  else
   {
    ch->mana += 150;
    victim->mana -= 150;
   } 

   act("$n's soul coil strikes $N in the chest.",ch,NULL,victim,TO_NOTVICT);
   act("You strike $N in the chest with your soul coil.",ch,NULL,victim,TO_CHAR);
   act("$n's soul coil strikes you in the chest!",ch,NULL,victim,TO_VICT);
   return;
}

void spell_death_coil( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
 CHAR_DATA *victim = (CHAR_DATA *) vo;
 int dam;

  if (victim == ch)
    {
      send_to_char("You would implode if you tried to soul coil yourself.\n\r",ch);
      return;
    }

    
   dam = victim->hit / 100 * 10;
   
   if (dam > 250)
     dam = 250;
    
    damage_spell( ch, victim, dam, sn,DAM_NEGATIVE,TRUE );   
 if (!IS_SET(victim->imm_flags,IMM_NEGATIVE))
   ch->mana   += dam;
   
   if(victim != ch)
    {
     act("$n's death coil strikes $N in the chest.",ch,NULL,victim,TO_NOTVICT);
     act("You strike $N in the chest with your death coil.", ch,NULL,victim,TO_CHAR);
     act("$n's death coil strikes you in the chest!",ch,NULL,victim,TO_VICT);
    }
   else
    {
     send_to_char("You damage your body to fuel your magic.\n\r",ch);
     act("$N's death coil swirls around $S body.",NULL,NULL,victim,TO_NOTVICT);
    }
 return;
}


void spell_aura_ignorance( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

       if(!is_affected(victim,sn))
        {
         af.where                        = TO_VULN;
         af.type                 = sn;
         af.level                        = level;
         af.duration             = level/10+1;
         af.location     = APPLY_NONE;
         af.modifier     = 0;
         af.bitvector   = VULN_MAGIC;
         affect_to_char(victim, &af);
         send_to_char( "You are surrounded by an aura of ignorance.\n\r",victim);
         act("$n is surrounded by an aura of ignorance.",victim,NULL,NULL,TO_ROOM);
        }
       else
         send_to_char("They already have an aura of ignorance.\n\r",ch);
 return ;
}   

void spell_fade( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
//    OBJ_DATA *obj;
    AFFECT_DATA af;


    /* character invisibility */
    victim = (CHAR_DATA *) vo;

    if ( IS_AFFECTED(victim, AFF_FADE) )
	return;

    if(is_affected(victim,skill_lookup("form wraith")))
     return;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FOG) )
     {
      send_to_char( "You can not fade while glowing.\n\r", ch );
      return;
     }
    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = level / 3.5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_FADE;
    affect_to_char( victim, &af );
    af.where     = TO_AFFECTS;
    af.type      = gsn_form_wraith;
    af.level     = ch->level;
    af.duration  = ch->level /3;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    return;
}

void spell_ironwill( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (victim != ch)
         send_to_char("Spell failed.\n\r",ch);

        if(!is_affected(victim,sn))
        {
         af.type	= gsn_ironwill;
      	 af.level	= victim->level;
      	 af.duration	= 50;
      	 af.modifier	= 0;
      	 af.bitvector = 0;
      	 af.location	= 0;
      	 affect_to_char(victim,&af);
         send_to_char( "You are determination.\n\r",victim);
         act("A look of grave determination enters $n's eyes.",victim,NULL,NULL,TO_ROOM);
        }
        else
          send_to_char("You already have a ironwill.\n\r",ch);
        return ;
}   

void spell_wicthelf( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  CHAR_DATA *gch;
  CHAR_DATA *witchelf;
  CHAR_DATA *warlock;
  AFFECT_DATA af;
  char buf[MAX_STRING_LENGTH];
  int i;

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another witchelf right now.\n\r",ch);
      return;
    }

 switch(number_range(1,2))
 {
  case 1:
  send_to_char("You attempt to summon a witchelf.\n\r",ch);
  act("$n attempts to summon a witchelf.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_SQUIRE)
	{
	  send_to_char("Two witchelfs are more than you need!\n\r",ch);
	  return;
	}
    }

  witchelf = create_mobile( get_mob_index(MOB_VNUM_SQUIRE) );

  for (i=0;i < MAX_STATS; i++)
    {
      witchelf->perm_stat[i] = ch->perm_stat[i];
    }

  witchelf->max_hit = ch->max_hit;
  witchelf->hit = witchelf->max_hit;
  witchelf->max_mana = ch->max_mana;
  witchelf->mana = witchelf->max_mana;
  witchelf->level = ch->level;
  witchelf->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
    witchelf->armor[i] = interpolate(witchelf->level,100,-100);
  witchelf->armor[3] = interpolate(witchelf->level,100,0);
  witchelf->gold = 0;

  xprintf( buf, witchelf->short_descr, ch->name );
  free_string( witchelf->short_descr );
  witchelf->short_descr = str_dup( buf );

  xprintf( buf, witchelf->long_descr, ch->name );
  free_string( witchelf->long_descr );
  witchelf->long_descr = str_dup( buf );
  
  xprintf( buf, witchelf->description, ch->name );
  free_string( witchelf->description );
  witchelf->description = str_dup( buf ); 
  
  char_to_room(witchelf,ch->in_room);
  send_to_char("A witchelf arrives from nowhere!\n\r",ch);
  act("A witchelf arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(witchelf->affected_by, AFF_CHARM);
  witchelf->master = witchelf->leader = ch;
 break;
 case 2:
    send_to_char("You attempt to summon a witchelf.\n\r",ch);
  act("$n attempts to summon a witchelf.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_WARLOCK)
	{
	  send_to_char("Two witchelf are more than you need!\n\r",ch);
	  return;
	}
    }

  warlock = create_mobile( get_mob_index(MOB_VNUM_WARLOCK) );

  for (i=0;i < MAX_STATS; i++)
    {
      warlock->perm_stat[i] = ch->perm_stat[i];
    }

  warlock->max_hit = ch->max_hit/3;
  warlock->hit = warlock->max_hit/3;
  warlock->max_mana = ch->max_mana/3;
  warlock->mana = warlock->max_mana/3;
  warlock->level = ch->level;
  warlock->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
    warlock->armor[i] = interpolate(warlock->level,100,-300);
  warlock->armor[3] = interpolate(warlock->level,100,-200);
  warlock->gold = 0;

  xprintf( buf, warlock->short_descr, ch->name );
  free_string( warlock->short_descr );
  warlock->short_descr = str_dup( buf );

  xprintf( buf, warlock->long_descr, ch->name );
  free_string( warlock->long_descr );
  warlock->long_descr = str_dup( buf );
  
  xprintf( buf, warlock->description, ch->name );
  free_string( warlock->description );
  warlock->description = str_dup( buf ); 
  
  char_to_room(warlock,ch->in_room);
  send_to_char("A warlock arrives from nowhere!\n\r",ch);
  act("A warlock arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(warlock->affected_by, AFF_CHARM);
  warlock->master = warlock->leader = ch;
 break;
    default: 
      send_to_char("Report this to Admins!\n\r",ch);
    break;
 }

}
/*Argus pet*/

void spell_argus_dragon( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *dragon;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another dragon right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a dragon.\n\r",ch);
  act("$n attempts to summon a dragon.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_ARGUS_DRAGON)
	{
	  send_to_char("Two dragons are more than you need!\n\r",ch);
	  return;
	}
    }

  dragon = create_mobile( get_mob_index(MOB_VNUM_ARGUS_DRAGON) );

  for (i=0;i < MAX_STATS; i++)
    {
      dragon->perm_stat[i] = ch->perm_stat[i];
    }

  dragon->max_hit = ch->max_hit;
  dragon->hit = dragon->max_hit;
  dragon->max_mana = ch->max_mana;
  dragon->mana = dragon->max_mana;
  dragon->level = ch->level;
  dragon->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
    dragon->armor[i] = interpolate(dragon->level,100,-100);
  dragon->armor[3] = interpolate(dragon->level,100,0);
  dragon->gold = 0;

  xprintf( buf, dragon->short_descr, ch->name );
  free_string( dragon->short_descr );
  dragon->short_descr = str_dup( buf );

  xprintf( buf, dragon->long_descr, ch->name );
  free_string( dragon->long_descr );
  dragon->long_descr = str_dup( buf );
  
  xprintf( buf, dragon->description, ch->name );
  free_string( dragon->description );
  dragon->description = str_dup( buf ); 
  
  char_to_room(dragon,ch->in_room);
  send_to_char("A dragon arrives from nowhere!\n\r",ch);
  act("A dragon arrives from nowhere!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(dragon->affected_by, AFF_CHARM);
  dragon->master = dragon->leader = ch;
  
}

/*Argus orb*/
void spell_argus_orb( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int orb_vnum;
  OBJ_DATA *orb;
  AFFECT_DATA af;

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

 if(is_affected(ch,skill_lookup("Orb of Honor")))
    {
     send_to_char( "You can't cast this spell now.\n\r", ch);   
     return;
     }

  orb_vnum = OBJ_VNUM_ARGUS_ORB;
  orb = create_object( get_obj_index(orb_vnum), level + 5);
  orb->timer = 2 * level;
  orb->cost  = 0;
  obj_to_char(orb, ch);
 
  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
  
 send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
 act("You are endowed by an ashen dragon with $p!",ch,orb,NULL,TO_CHAR);
 act("$n kneels in servitude as an ashen dragon appears in the sky.",ch,orb,NULL,TO_ROOM);
 act("$n is endowed with $p by an ashen dragon!",ch,orb,NULL,TO_ROOM); 
}

/*Argus Dagger*/
void spell_argus_dagger( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int dagger_vnum;
  OBJ_DATA *dagger;
  AFFECT_DATA af;

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

 if(is_affected(ch,skill_lookup("Dagger of the Guardian")))
    {
     send_to_char( "You can't cast this spell now.\n\r", ch);   
     return;
     }

  dagger_vnum = OBJ_VNUM_ARGUS_DAGGER;
  dagger = create_object( get_obj_index(dagger_vnum), level + 5);
  dagger->timer = 2 * level;
  dagger->cost  = 0;
  obj_to_char(dagger, ch);
 
  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
  
  send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
  act("You are endowed by an ashen dragon with $p!",ch,dagger,NULL,TO_CHAR);
  act("$n kneels in servitude as an ashen dragon appears in the sky.",ch,dagger,NULL,TO_ROOM);
  act("$n is endowed with $p by an ashen dragon!",ch,dagger,NULL,TO_ROOM);
}

/*Gondor ring*/

void spell_gondor_ring( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int ring_vnum;
  OBJ_DATA *ring;
  AFFECT_DATA af;

 if(is_affected(ch,skill_lookup("signet ring")))
    {
     send_to_char( "You can't cast this spell now.\n\r", ch);   
     return;
     }

  ring_vnum = OBJ_VNUM_GONDER_RING;
  ring = create_object( get_obj_index(ring_vnum), level + 5);
  ring->timer = 3 * level;
  ring->cost  = 0;
  obj_to_char(ring, ch);
 
  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 3 * level - 2;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
  
//send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
act("You are endowed by the gods with $p!",ch,ring,NULL,TO_CHAR);
act("$n kneels in servitude as a gods angel appears in the sky.",ch,ring,NULL,TO_ROOM);
act("$n is endowed with $p by an gods!",ch,ring,NULL,TO_ROOM);
}

void spell_gondor_sword( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int sword_vnum;
  OBJ_DATA *sword;
  AFFECT_DATA af;

 if(is_affected(ch,skill_lookup("sword of flame")))
    {
     send_to_char( "You can't cast this spell now.\n\r", ch);   
     return;
     }


    if (( ch->kingdom_rank != 6) && ( ch->kingdom_rank != 8) && ( ch->kingdom_rank != 9) )
     {
      send_to_char( "You rank do not get this spell.\n\r", ch);   
      return;
     }

   if(is_affected(ch,skill_lookup("justice brooch")) && (ch->kingdom_rank != 9) )
    {
     send_to_char( "You can only have justice brooch or  sword of flame.\n\r", ch);   
     return;
     }

  sword_vnum = OBJ_VNUM_GONDER_SWORD;
  sword = create_object( get_obj_index(sword_vnum), level + 5);
  sword->timer = 50;
  sword->cost  = 0;
  obj_to_char(sword, ch);
 
    af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 50 - 2;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
  
//send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
act("You are endowed by the gods with $p!",ch,sword,NULL,TO_CHAR);
act("$n kneels in servitude as a gods angel appears in the sky.",ch,sword,NULL,TO_ROOM);
act("$n is endowed with $p by an gods!",ch,sword,NULL,TO_ROOM);
}

void spell_gondor_ornament( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int ornament_vnum;
  OBJ_DATA *ornament;
  AFFECT_DATA af;  

 if(is_affected(ch,skill_lookup("justice brooch")))
    {
     send_to_char( "You can't cast this spell now.\n\r", ch);   
     return;
     }

     if (( ch->kingdom_rank != 7) && ( ch->kingdom_rank != 8) && ( ch->kingdom_rank != 9))
     {
      send_to_char( "You rank do not get this spell.\n\r", ch);   
      return;
     }
 
    if(is_affected(ch,skill_lookup("sword of flame")) && (ch->kingdom_rank != 9) )
    {
     send_to_char( "You can only have justice brooch or  sword of flame.\n\r", ch);   
     return;
     }
   
  ornament_vnum = OBJ_VNUM_GONDER_ORNAMENT;
  ornament = create_object( get_obj_index(ornament_vnum), level + 5);
  ornament->timer = 50;
  ornament->cost  = 0;
  obj_to_char(ornament, ch);
 
  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 50 - 2;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  


//send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
act("You are endowed by the gods with $p!",ch,ornament,NULL,TO_CHAR);
act("$n kneels in servitude as a gods angel appears in the sky.",ch,ornament,NULL,TO_ROOM);
act("$n is endowed with $p by an gods!",ch,ornament,NULL,TO_ROOM);
}

/*Argus */
void spell_circle_protection(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    AFFECT_DATA af;
            
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
        if ((IS_NPC(ch) && IS_NPC(gch)) ||
            (!IS_NPC(ch) && !IS_NPC(gch)))
        {

	if(!is_affected(gch,sn))
	{
	af.where			= TO_RESIST;
	af.type			= sn;
	af.level			= level;
	af.duration		= level/10+1;
	af.location 	= APPLY_NONE;
	af.modifier 	= 0;
	af.bitvector   = RES_MAGIC;
	affect_to_char(gch, &af);
	send_to_char( "You are protected from magic.\n\r", gch);
	}
        }
    }
}

void spell_divine_deflection(int sn, int level, CHAR_DATA *ch, void *vo)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;



        if (victim != ch)
         send_to_char("Spell failed.\n\r",ch);

       if ( victim->position != POS_FIGHTING)
	{
	send_to_char( "The Immmortal can't deflect blows you not fighting anyone.\n\r",victim);
	return;
	}

        if(!is_affected(victim,sn))
        {
         /*When can you cast it and when not?*/
         af.type	= gsn_divine_deflection;
      	 af.level	= victim->level;
      	 af.duration	= 20;
      	 af.modifier	= 0;
      	 af.bitvector = 0;
      	 af.location	= 0;
      	 affect_to_char(victim,&af);
         /*when you will get harmed or not*/
         af.type	= gsn_aura_divine_deflection;
      	 af.level	= victim->level;
      	 af.duration	= 2;
      	 af.modifier	= 0;
      	 af.bitvector = 0;
      	 af.location	= 0;
      	 affect_to_char(victim,&af);
         send_to_char( "The Immmortal will now deflect your attacks.\n\r",victim);
         act("$n is now .",victim,NULL,NULL,TO_ROOM);
        }
        else
          send_to_char("The can't pray for it now.\n\r",ch);
        return ;
}
/*Narssius spells*/
void spell_nassius_demon( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *demon;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another demon right now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a demon.\n\r",ch);
  act("$n attempts to summon a demon.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_NARSSIUS_DEMON)
	{
	  send_to_char("Two demons are more than you need!\n\r",ch);
	  return;
	}
    }

  demon = create_mobile( get_mob_index(MOB_VNUM_NARSSIUS_DEMON) );

  for (i=0;i < MAX_STATS; i++)
    {
      demon->perm_stat[i] = ch->perm_stat[i];
    }

  demon->max_hit = ch->max_hit;
  demon->hit = demon->max_hit;
  demon->max_mana = ch->max_mana;
  demon->mana = demon->max_mana;
  demon->level = ch->level;
  demon->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
    demon->armor[i] = interpolate(demon->level,100,-100);
  demon->armor[3] = interpolate(demon->level,100,0);
  demon->gold = 0;

  xprintf( buf, demon->short_descr, ch->name );
  free_string( demon->short_descr );
  demon->short_descr = str_dup( buf );

  xprintf( buf, demon->long_descr, ch->name );
  free_string( demon->long_descr );
  demon->long_descr = str_dup( buf );
  
  xprintf( buf, demon->description, ch->name );
  free_string( demon->description );
  demon->description = str_dup( buf ); 
  
  char_to_room(demon,ch->in_room);
  send_to_char("A demon arrives from hell!\n\r",ch);
  act("A demon arrives from hell!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(demon->affected_by, AFF_CHARM);
  demon->master = demon->leader = ch;
  
}

void spell_paranoia( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = dice( level, 17 );
    if ( saves_spell( level, victim, DAM_MENTAL ) )
      {
        dam /= 2;
//        if ( saves_spell( level, victim, DAM_MENTAL ) )
//         do_flee(victim,"");
      }  
    act("$n dissappears in a gust of burning winds.",victim,NULL,NULL,TO_ROOM);
    damage_spell( ch, victim, dam, sn,DAM_MENTAL,TRUE );
    return;
}

void spell_narssius_claw( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int sword_vnum;
  OBJ_DATA *sword;
  AFFECT_DATA af;

 if(is_affected(ch,skill_lookup("claw of the demon")))
    {
     send_to_char( "You can't cast this spell now.\n\r", ch);   
     return;
     }


  sword_vnum = OBJ_VNUM_NARSSIUS_CLAW;
  sword = create_object( get_obj_index(sword_vnum), level + 5);
  sword->timer = 50;
  sword->cost  = 0;
  obj_to_char(sword, ch);
 
    af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 50 - 2;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
  
//send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
act("You are endowed by the gods with $p!",ch,sword,NULL,TO_CHAR);
act("$n kneels in servitude as a gods demon appears in the sky.",ch,sword,NULL,TO_ROOM);
act("$n is endowed with $p by an gods!",ch,sword,NULL,TO_ROOM);
}

//Drakkon spell
void spell_dragon_wrath( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

    if ( victim!=ch)
    {
         send_to_char("How are you planning to impose the wrath of the dragon into someone else?\n\r",ch);
         return;
    }

   if ( is_affected( victim, sn ) )
   {
    send_to_char("You already have the wrath of the dragon in you.\n\r",ch);
    return;
   }
   
   af.where      = TO_AFFECTS;
   af.type      = sn;
   af.level      = level;
   af.duration  = 24;
   af.modifier  = level*-2;
   af.location  = APPLY_AC;
   af.bitvector = 0;
   affect_to_char( victim, &af );

   af.where      = TO_AFFECTS;
   af.type      = sn;
   af.level      = level;
   af.duration  = 24;
   af.bitvector = 0;
   af.modifier  = ch->level/3;
   af.location  = APPLY_HITROLL;
   affect_to_char( victim, &af );

   af.where      = TO_AFFECTS;
   af.type      = sn;
   af.duration  = 24;
   af.bitvector = 0;
   af.modifier  = ch->level/3;
   af.location  = APPLY_DAMROLL;
   affect_to_char( victim, &af );
   
   send_to_char( "You feel someone protecting you.\n\r", victim );
   
 return;
}

void spell_illusion_fields( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FOG) )
 {
    send_to_char( "You can't be invisible while you are glowing.\n\r", victim );
    return;
 }


   if ( is_affected( victim, sn ) )
   {
    send_to_char("You already have the illusion of the dragon in you.\n\r",ch);
    return;
   }
   
    af.where      = TO_AFFECTS;
    af.type      = sn;
    af.duration  = 10;
    af.modifier  = 0;
    af.bitvector = 0;
    af.location  = 0;
    send_to_char( "You feel you have illusion the of the dragon.\n\r", victim );
    affect_to_char( victim, &af );
return;
}   

void spell_dragon_speed( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

   if ( is_affected( victim, sn ) )
   {
    send_to_char("You already have the speed of the dragon in you.\n\r",ch);
    return;
   }
   
    af.where      = TO_AFFECTS;
    af.type      = sn;
    af.duration  = 10;
    af.modifier  = 0;
    af.bitvector = 0;
    af.location  = 0;
    send_to_char( "You feel you have the speed of the dragon.\n\r", victim );
    affect_to_char( victim, &af );
return;
}   

void spell_talastin_mindblock( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;
   
    af.where      = TO_AFFECTS;
    af.type      = sn;
    af.duration  = 10;
    af.modifier  = 0;
    af.bitvector = 0;
    af.location  = 0;
    send_to_char( "You feel you mind is blocked.\n\r", victim );
    affect_to_char( victim, &af );
return;
}   


void spell_dragon_lure( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
//    CHAR_DATA *gch;
//    int numcharmie;
//    char buf[MAX_STRING_LENGTH];

    if (is_safe(ch,victim)) 
     return;

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
    ||   saves_spell( level+5, victim,DAM_CHARM) )
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

//Talastin
void spell_talastin_soldier( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *demon;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    {
      send_to_char("You cannot command another Talastin Soldier right now.\n\r",ch);
      return;
    }

  send_to_char("You attempt to summon a Talastin Soldier.\n\r",ch);
  act("$n attempts to summon a Talastin Soldier.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_TALASTIN_SOLDIER)
	{
	  send_to_char("Two Talastin Soldier are more than you need!\n\r",ch);
	  return;
	}
    }

  demon = create_mobile( get_mob_index(MOB_VNUM_TALASTIN_SOLDIER) );

  for (i=0;i < MAX_STATS; i++)
    {
      demon->perm_stat[i] = ch->perm_stat[i];
    }

  demon->max_hit = ch->max_hit/2;
  demon->hit = demon->max_hit;
  demon->max_mana = ch->max_mana/2;
  demon->mana = demon->max_mana;
  demon->level = ch->level;
  demon->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
  demon->armor[i] = interpolate(demon->level,100,-100);
  demon->armor[3] = interpolate(demon->level,100,0);
  demon->gold = 0;

  xprintf( buf, demon->short_descr, ch->name );
  free_string( demon->short_descr );
  demon->short_descr = str_dup( buf );

  xprintf( buf, demon->long_descr, ch->name );
  free_string( demon->long_descr );
  demon->long_descr = str_dup( buf );
  
  xprintf( buf, demon->description, ch->name );
  free_string( demon->description );
  demon->description = str_dup( buf ); 
  
  char_to_room(demon,ch->in_room);
  send_to_char("A Talastin Soldier arrives from no where!\n\r",ch);
  act("A Talastin Soldier arrives from no where!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(demon->affected_by, AFF_CHARM);
  demon->master = demon->leader = ch;
return;  
}

void spell_talastin_shield( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  int sword_vnum;
  OBJ_DATA *sword;
  AFFECT_DATA af;

 if(is_affected(ch,skill_lookup("Imperial Shield")))
    {
     send_to_char( "You can't cast this spell now.\n\r", ch);   
     return;
     }


  sword_vnum = OBJ_VNUM_TALASTIN_SHIELD;
  sword = create_object( get_obj_index(sword_vnum), level + 5);
  sword->timer = 50;
  sword->cost  = 0;
  obj_to_char(sword, ch);
 
    af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 50 - 2;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  
  
//send_to_char("You call forth the aid of a golden-clawed ashen dragon.\n\r",ch);
act("You are endowed by the Talastin Soldier with $p!",ch,sword,NULL,TO_CHAR);
act("$n kneels in servitude as a gods Talastin Soldier appears.",ch,sword,NULL,TO_ROOM);
act("$n is endowed with $p by an Talastin Soldier!",ch,sword,NULL,TO_ROOM);
return;
}
//
void spell_call_treefolk( int sn, int level, CHAR_DATA *ch, void *vo, int target )	
{
  CHAR_DATA *gch;
  CHAR_DATA *demon;
  AFFECT_DATA af;
  int i;
  char buf[MAX_STRING_LENGTH];

  if (is_affected(ch,sn))
    { 
      send_to_char("You cannot command another Call Treefolk right now.\n\r",ch);
      return;
    }

  send_to_char("You attempt to summon Treefolk .\n\r",ch);
  act("$n attempts to summon Treefolk .",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_ARKADIA_TREEFOLK)
	{
	  send_to_char("Two Treefolk are more than you need!\n\r",ch);
	  return;
	}
    }

  demon = create_mobile( get_mob_index(MOB_VNUM_ARKADIA_TREEFOLK) );

  for (i=0;i < MAX_STATS; i++)
    {
      demon->perm_stat[i] = ch->perm_stat[i];
    }

  demon->max_hit = ch->max_hit;
  demon->hit = demon->max_hit;
  demon->max_mana = ch->max_mana;
  demon->mana = demon->max_mana;
  demon->level = ch->level;
  demon->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;
  for (i=0; i < 3; i++)
  demon->armor[i] = interpolate(demon->level,100,-100);
  demon->armor[3] = interpolate(demon->level,100,0);
  demon->gold = 0;

  xprintf( buf, demon->short_descr, ch->name );
  free_string( demon->short_descr );
  demon->short_descr = str_dup( buf );

  xprintf( buf, demon->long_descr, ch->name );
  free_string( demon->long_descr );
  demon->long_descr = str_dup( buf );
  
  xprintf( buf, demon->description, ch->name );
  free_string( demon->description );
  demon->description = str_dup( buf ); 
  
  char_to_room(demon,ch->in_room);
  send_to_char("A Treefolk arrives from no where!\n\r",ch);
  act("A Treefolk arrives from no where!",ch,NULL,NULL,TO_ROOM);

  af.where		= TO_AFFECTS;
  af.type               = sn;
  af.level              = level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

  SET_BIT(demon->affected_by, AFF_CHARM);
  demon->master = demon->leader = ch;
return;  
}

void spell_force_nature( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   AFFECT_DATA af;

   if ( is_affected( victim, sn ) )
   {
    send_to_char("You already have the force of nature.\n\r",ch);
    return;
   }
   
    af.where      = TO_AFFECTS;
    af.type      = sn;
    af.duration  = 2;
    af.modifier  = 0;
    af.bitvector = 0;
    af.location  = 0;
    affect_to_char( victim, &af );
    send_to_char("You are filled with the Wrath of Nature!\n\r",ch);
    return;
}   

void spell_heal_anywhere( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
   CHAR_DATA *victim;
   AFFECT_DATA af;
   int heal;

   if ( ( victim = get_char_world( ch, target_name ) ) == NULL || victim->in_room == NULL)
   {
	   send_to_char( "You failed.\n\r", ch );
	   return;
   }

   if ( is_affected( ch, sn ))
   {
      send_to_char("You have to wait before casting this again.\n\r", ch);
	   return;
   }

   if(victim == ch)
   {
      send_to_char( "Only others may benefit from this spell.\n\r.", ch);
      return;
   }

   if(IS_NPC(victim))
   {
      send_to_char( "This spell can only be targeted at players.\n\r", ch);
      return;
   }


   heal = dice(4, 6) + level;
   victim->hit = UMIN( victim->hit + heal, victim->max_hit );

   af.where     = TO_AFFECTS;
   af.type      = sn;
   af.level     = level;

   if(ch->pcdata->learned[sn] < 75)
      af.duration  = 3;
   else if (ch->pcdata->learned[sn] < 99)
      af.duration  = 2;
   else
      af.duration  = 1;

   af.location  = APPLY_NONE;
   af.modifier  = 0;
   af.bitvector = 0;
   affect_to_char( ch, &af );

   send_to_char("You feel better!\n\r", victim);
   act( "$n appears less wounded.\n\r", victim, NULL, NULL, TO_ROOM );
   act( "$n's eyes unfocus for a moment.\n\r", ch, NULL, NULL,   TO_ROOM );
   send_to_char("Ok.\n\r", ch);

   return;
}

void spell_kazmir_asshole(int sn, int level, CHAR_DATA *ch, void *vo)
{

 CHAR_DATA *victim = (CHAR_DATA *) vo;
 ROOM_INDEX_DATA *pRoomIndex;
    
 	  if ((IS_NPC(victim)) && (victim != ch))
	    {
/*
              if ( saves_spell( level - 5, victim,DAM_OTHER))
               {
	        send_to_char( "You failed.\n\r", ch );
	        return;
               }
*/
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
        
return;	
}

void spell_cure_corruption(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_corruption ) )
    {
	if (victim == ch)
	  send_to_char("You aren't corrupted.\n\r",ch);
	else
	  act("$N doesn't appear to be corrupted.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (check_dispel(level,victim,gsn_corruption))
    {
	send_to_char( "You feel yourself healthy again.!\n\r", victim );
	act("$n is no longer corrupted.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}

void spell_spectral_fist( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dam;
    EXIT_DATA *pexit;
    ROOM_INDEX_DATA *to_room;
    int door;
    
     char * const rev_name [] =
    {
        "the south", "the west", "the north", "the east", "below", "above"
    };
  
 
    if ( saves_spell( level, victim, DAM_BASH ) )
       {      
        dam = 0;
        send_to_char("You manage to avoid a huge fist aimed at you.\n\r",victim);
        act("A huge fist forms out of thin air and flies past $n.",victim,NULL,NULL,TO_ROOM);
       }
    else
       {
       	if ( number_percent() < 90 )
       	 {
    	  dam = dice( level, 18 );
       	  damage_spell( ch, victim, dam, sn,DAM_BASH,TRUE );
          WAIT_STATE(victim, PULSE_VIOLENCE / 2);
          victim->position = POS_RESTING; 
         if (victim->lagattack < 8)
	  victim->lagattack = 8;
	 if (victim->lagflee   < 9)
	  victim->lagflee   = 9;
	 if (victim->lagstand   <9)
	  victim->lagstand  = 9;
	 if (victim->lagspell   < 6)
	  victim->lagspell = 6;   
         send_to_char("A huge magical fist knocks you to the ground.\n\r",victim);
         act("A huge fist forms out of thin air and strikes $n.",victim,NULL,NULL,TO_ROOM);
         }
        else
         {
         door = number_range(0,3);
	 dam = dice( level, 25 );
	 damage_spell( ch, victim, dam, sn,DAM_BASH,TRUE );
         stop_fighting( victim, TRUE );
         af.where     = TO_AFFECTS;
         af.type      = gsn_sleep;
         af.level     = level;
         af.duration  = 2;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = AFF_SLEEP;
         affect_join( victim, &af );
    		
      act("$n flies through the air as the fist strikes him with tremendous force",victim,NULL,NULL,TO_NOTVICT);
      if  ( ( ( pexit   = ch->in_room->exit[door] ) == NULL)
       ||   ( (to_room = pexit->u1.to_room   ) == NULL ) )
         {
          send_to_char("A huge magical fist knocks you to the ground.\n\r",victim);
          act("A huge fist forms out of thin air and strikes $n.",victim,NULL,NULL,TO_ROOM);
	  act("Slams into the wall and falls to the ground unconscious.",victim,NULL,NULL,TO_NOTVICT);
    	 }
	else
	{
         act( "$n {Wflies{x $T!", victim, NULL, dir_name[door], TO_ROOM );
         act( "You {Wfly{x $T!\n\r", victim, NULL, dir_name[door], TO_CHAR);
         char_from_room( victim );
         char_to_room( victim, pexit->u1.to_room );
         send_to_char( "You're send reeling through the air as the fist hits you.{x\n\r", victim );
         send_to_char( "You lose consciousness as you hit the ground.{x\n\r", victim );
         act( "You notice movement $T.",victim, NULL, rev_name[door], TO_ROOM );
         act( "$n {Wflies{x into the room!", victim, NULL, NULL, TO_ROOM );
        }
            victim->position = POS_SLEEPING;
           }
        }
    return;
}

void spell_power_word_kill( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  
    if ((ch->hit * 0.8 > victim->hit) && (number_range(1,100) > 50))
      {
     send_to_char( "You focus on your target as you utter the incantation.{x\n\r", ch );
     act("A flashing green light surrounds $n",victim,NULL,NULL,TO_NOTVICT);
     act( "A flashing green light blinds you as $n utters an incantation.{x",ch,NULL,victim,TO_VICT);
     send_to_char( "You collapse, struggling for every breath.{x\n\r", victim );
     send_to_char( "Slowly you slip into a dark unfriendly place{x\n\r", victim );
     act("$n's utters a devastating incantation while staring at $N",ch,NULL,victim,TO_NOTVICT);
     act("$n collapses on the ground, DEAD!",victim,NULL,NULL,TO_NOTVICT);
     send_to_char( "{rYou are DEAD!{x\n\r", victim );
     raw_kill( ch,victim );
     }
    else
    {
    send_to_char( "You feel an intense but brief pain erupt from your head.{x\n\r", victim );
    send_to_char( "You focus on your target as you utter the incantation.{x\n\r", ch );
    act("$n staggers for a moment.",victim,NULL,NULL,TO_NOTVICT);
    return;
    }
return;
 }

void spell_feeblemind( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 

   if ( is_affected( victim, sn ) || saves_spell( level,victim,DAM_OTHER))
	return;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = 15;
    af.location  = APPLY_INT;
    af.modifier  = -30;
    af.bitvector = AFF_WEAKEN;
    affect_to_char( victim, &af );
    send_to_char( "Your head feels heavy.\n\r", victim );
    send_to_char( "Your surroundings seem more complex.\n\r", victim );
    act("$n looks puzzled.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_shadow_door( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
   CHAR_DATA *gch;
   AFFECT_DATA af;
   
    if ( IS_AFFECTED(ch, AFF_INVISIBLE) )
 {
	send_to_char( "You are already invisible.\n\r", ch );
	return;
 }
  
 /* if ( IS_AFFECTED(ch, AFF_IMPROVED_INVISIBLE) )
 {
	send_to_char( "You are already invisible.\n\r", ch );
	return;
 }*/
 
    if ( IS_AFFECTED(ch, AFF_FAERIE_FOG) )
 {
    send_to_char( "You can't be invisible while you are glowing.\n\r", ch );
    return;
 }
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( (gch->fighting == ch) && ( CAN_DETECT(gch, DETECT_INVIS) ) &&
	( !IS_AFFECTED(ch, AFF_BLIND))) {
        return;
       }
    }
    
    act( "$n steps through a gate and vanishes.", ch, NULL, NULL, TO_ROOM );
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = 12;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( ch, &af );
    send_to_char( "You fade out of existence.\n\r", ch );
    stop_fighting( ch, TRUE );
return;
}

void spell_hold_person( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int chance;
    
    chance = (get_curr_stat(ch,STAT_INT) * 8) - (get_curr_stat(victim,STAT_INT) * 8);
    chance += (ch->level * 2) - (victim->level * 2);
    
    if (number_percent( ) > chance){
    act( "$n looks puzzled for a second", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You resist an attempt to enter your mind.\n\r", victim );
    return;
    }
    
 
    if ( is_affected( victim, sn ) )
    {
        act("$N is already paralysed.",ch,NULL,victim,TO_CHAR);
        return;
    }
    
    if (is_affected(ch, sn))
    {
      send_to_char("{cYou need time to recharge your psychic powers.{x\n\r", ch);
      return;
    }
    
    af.type      = sn;
    af.level     = level;
    af.duration  = 2;
    af.where     = TO_AFFECTS;
    af.bitvector = AFF_STUN;
    affect_to_char( victim, &af );
    
    af.type      = sn;
    af.level     = level;
    af.duration  = 2;
    af.where     = TO_AFFECTS2;
    af.bitvector = AFF2_PARALYZE;    
    affect_to_char( victim, &af );
    
    
      af.type	= sn;
      af.level	= level;
      af.duration	= 20;
      af.modifier	= 0;
      af.bitvector 	= 0;
      af.location	= 0;
      affect_to_char(ch,&af);
     

    
    act( "$n has been immobilized.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You have been paralyzed.\n\r", victim );
    return;
}

void spell_regeneration( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED(victim,AFF_REGENERATION))
        {
         act("$N is already regenerating.\n\r",ch,NULL,victim,TO_CHAR);
         return;
        }


    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = (10);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_REGENERATION;
    affect_join(victim,&af);

    send_to_char
      ("Your wounds start closing as your body regenerates.\n\r",victim);
    act("$n skin suddenly seems alive.",
      victim,NULL,NULL,TO_ROOM);
}

void spell_ironskin(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
    if ( IS_AFFECTED2(victim, AFF2_IRONSKIN) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already as iron.\n\r",ch);
	  return;
    }

 if (is_affected(ch, gsn_ironskin))
    {
      send_to_char("{cYour skin is already as iron.{x\n\r", ch);
      return;
    }

    af.where     = TO_AFFECTS2;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.modifier	= 0;
    af.location	= 0;
    af.bitvector = AFF2_IRONSKIN;
    affect_to_char( victim, &af );

      af.type	= gsn_ironskin;
      af.level	= ch->level;
      af.duration	= ch->level;
      af.modifier	= 0;
      af.bitvector = 0;
      af.location	= 0;
      affect_to_char(ch,&af);

    
    send_to_char( "A magical aura surrounds you, less than an inch from your body\n\r", victim );
    if ( ch != victim )
	act("$N is surrounded by a magical aura!",ch,NULL,victim,TO_CHAR);
    return;
}

/*void spell_improved_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    victim = (CHAR_DATA *) vo;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;
    
    if ( IS_AFFECTED(victim, AFF_IMPROVED_INVISIBLE) )
	return;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FOG) )
 {
    send_to_char( "You can't be invisible while you are glowing.\n\r", victim );
    return;
 }
 
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) )
 {
    send_to_char( "You can't be invisible while you are glowing.\n\r", victim );
    return;
 }
 
    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );

    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level     = level;
    af.duration  = level + 12;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_IMPROVED_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    return;
}*/

void spell_detect_undead( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( CAN_DETECT(victim, DETECT_UNDEAD) )
    {
	if (victim == ch)
	  send_to_char("You can already sense the presence of the dead.\n\r",ch);
	else
	  act("$N can already detect undead.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_DETECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level/2;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = DETECT_UNDEAD;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_contagion( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED(victim,AFF2_CONTAGION))
        {
         act("$N is already contageous.\n\r",ch,NULL,victim,TO_CHAR);
         return;
        }

    if (saves_spell(level,victim,DAM_DISEASE) || (IS_SET(victim->form,FORM_UNDEAD))
    || (IS_SET(victim->form,FORM_CONSTRUCT)))
    {
        if (ch == victim)
          send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
   else
          act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
        return;
    }

    af.where     = TO_AFFECTS2;
    af.type      = sn;
    af.level     = level;
    af.duration  = (level / 4);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF2_CONTAGION;
    affect_join(victim,&af);

    send_to_char
      ("You feel yourself slowly weaken.\n\r",victim);
    act("$n looks very tired as red spots appears on him.",
      victim,NULL,NULL,TO_ROOM);
}

void spell_wither( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	 CHAR_DATA *victim = (CHAR_DATA *) vo;
	 AFFECT_DATA af;
         
         if (IS_AFFECTED2(victim, AFF2_WITHER))
         {
         send_to_char( "Your target is already withering.\n\r", ch );
         return;
         }

         if (saves_spell(level+5,victim,DAM_OTHER)) {  
         send_to_char( "You failed.\n\r", ch );
	 return;
	 }
	 
	 af.where       = TO_AFFECTS2;
	 af.type	= sn;
	 af.level	= level;
	 af.duration	= level*3;
	 af.location	= APPLY_HIT;
	 af.modifier    = level*(-2);
	 af.bitvector	= AFF2_WITHER;
	 affect_to_char( victim, &af );
         af.location    = APPLY_MANA;
         af.modifier    = level*(-2);	 
	 affect_to_char( victim, &af );
	 send_to_char( "You wither and age!\n\r", victim );
	 act( "$n seems significant weaker.\n\r", victim, NULL, NULL, TO_ROOM );
	 return ;
}

void spell_raise_shadelich( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA   *lich;
    OBJ_DATA    *obj;
    CHAR_DATA *gch;
     int chance, boost;

    obj = get_obj_here( ch, NULL,  "corpse");

    if ( obj == NULL )
    {
        send_to_char("There is no corpse here.\n\r", ch);
        return;
    }
    
    if (obj->level < ch->level/2)
    {
  	extract_obj( obj );
  	send_to_char("A lich is to powerful to be raised out of such an unworthy host.\n\r", ch);
  	return;
     }
     
     if ( !IS_NPC(ch) &&   ch->level < skill_table[gsn_undead_mastery].skill_level[ch->class] )
     boost = 0; 
     else
     boost = (ch->pcdata->learned[gsn_undead_mastery]);
    
    
chance = get_skill(ch,sn);
chance += ch->level*4;
chance -= obj->level*4 + 20;

if (number_percent( ) > chance)
 {
  extract_obj( obj );
  send_to_char("The corpse was too powerful to raised by you.\n\r", ch);
  check_improve(ch,gsn_undead_mastery,FALSE,1);
  return;
 } 

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_LICH)
	{
	  send_to_char("You can only control 1 lich at a time.\n\r",ch);
	  return;
	}
    }

    if ( obj->item_type != ITEM_CORPSE_NPC )
    {
        send_to_char("You cannot animate that.\n\r", ch);
        return;
    }
    else 
    {   
    	check_improve(ch,gsn_undead_mastery,TRUE,1);
        lich = create_mobile( get_mob_index( MOB_VNUM_LICH ) );
        //SET_BIT( lich->act, ACT_PET );
        SET_BIT( lich->affected_by, AFF_CHARM );
        char_to_room( lich, ch->in_room );
     if (obj->level < ch->level)
        lich->level = ch->level-10 + boost/20;
     else 
      lich->level = (obj->level - 5 + boost/20 );

     lich->max_hit = ( lich->level *30 * (1 + (float) boost/100));
     lich->hit = lich->max_hit;

        extract_obj( obj );

        send_to_char("You awaken a lich from the remains of the corpse.\n\r",
                ch);
        act("$n awakens a lich from the remains of the corpse.", ch, NULL,
                NULL, TO_ROOM);
        add_follower( lich, ch );
        lich->leader = ch;


        return;
    }
}

void spell_vampiric_aura( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
         CHAR_DATA *victim = (CHAR_DATA *) vo;
         AFFECT_DATA af;

         if ( is_affected( victim, sn ))
         {
		send_to_char("You already have an unnatural charismatic aura.\n\r",ch);
                  return;
         }


         af.where     = TO_AFFECTS2;
         af.type      = sn;
         af.level     = level;
         af.duration  = level/5;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = AFF2_CHARISMATIC;
         affect_to_char( victim, &af );

         send_to_char( "You seem a lot more appealing.\n\r", victim);
         act("$n suddenly seems almost unnaturally appealing. ",victim,NULL,NULL,TO_ROOM);
         return;
}

void spell_ghoul_touch( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
         CHAR_DATA *victim = (CHAR_DATA *) vo;
         AFFECT_DATA af;

         if ( is_affected( victim, sn ))
         {
		send_to_char("You already have a paralyzing touch.\n\r",ch);
                  return;
         }
         if ( IS_AFFECTED2( victim, AFF2_VAMPIRIC ))
         {
		send_to_char("Your hands are already transformed.\n\r",ch);
                  return;
         }


         af.where     = TO_AFFECTS2;
         af.type      = sn;
         af.level     = level;
         af.duration  = level/5;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = AFF2_GHOUL;
         affect_to_char( victim, &af );

         send_to_char( "Your hands is covered with decaying flesh\n\r", victim);
         act("$n's hands is covered with decaying flesh. ",victim,NULL,NULL,TO_ROOM);
         return;
}

void spell_cloak_of_fear( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
         CHAR_DATA *victim = (CHAR_DATA *) vo;
         AFFECT_DATA af;

         if ( is_affected( victim, sn ))
         {
        if (victim == ch)
          send_to_char("You are already engulfed in an aura of fear.\n\r",ch);
        else
          act("$N already is affected by an aura of fear.",
                        ch,NULL,victim,TO_CHAR);
                  return;
         }
         
         af.where     = TO_AFFECTS;
         af.type      =  gsn_cloak_fear;
         af.level     = level;
         if (victim == ch)
         af.duration  = level/5;
         else
         af.duration  = level/10;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = 0;
         affect_to_char( victim, &af );
         send_to_char( "You are surrounded by terrifying aura.\n\r", victim);
         act("$n is surrounded by a terrifying aura.",victim,NULL,NULL,TO_ROOM);
         if ( ch != victim )
                  send_to_char( "Ok.\n\r", ch );
         return;
}

void spell_raise_vampire( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA   *vampire;
    OBJ_DATA    *obj;
    CHAR_DATA *gch;
     int chance;
     int boost;

    obj = get_obj_here( ch, NULL,  "corpse");

    if ( obj == NULL )
    {
        send_to_char("There is no corpse here.\n\r", ch);
        return;
    }
    
    if (obj->level < ch->level/1.5)
    {
  	extract_obj( obj );
  	send_to_char("The corpse is not suitable to raise a vampire.\n\r", ch);
  	return;
     }
     if ( !IS_NPC(ch) &&   ch->level < skill_table[gsn_undead_mastery].skill_level[ch->class] )
     boost = 0; 
     else
     boost = (ch->pcdata->learned[gsn_undead_mastery]);

    
chance = get_skill(ch,sn);
chance += ch->level*4;
chance -= obj->level*4 + 20;

if (number_percent( ) > chance)
 {
  extract_obj( obj );
  send_to_char("The corpse was too powerful to raised by you.\n\r", ch);
  check_improve(ch,gsn_undead_mastery,FALSE,1);
  return;
 } 

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_VAMPIRE2)
	{
	  send_to_char("You can only control 1 vampire at a time.\n\r",ch);
	  return;
	}
    }

    if ( obj->item_type != ITEM_CORPSE_NPC )
    {
        send_to_char("You cannot animate that.\n\r", ch);
        return;
    }
    else 
    { 
    	check_improve(ch,gsn_undead_mastery,TRUE,1);  
        vampire = create_mobile( get_mob_index( MOB_VNUM_VAMPIRE2 ) );
        //SET_BIT( vampire->act, ACT_PET );
        SET_BIT( vampire->affected_by, AFF_CHARM );
        char_to_room( vampire, ch->in_room );
     if (obj->level < ch->level)
        vampire->level = ch->level;
     else 
      vampire->level = (obj->level );

     vampire->max_hit = ( vampire->level *80 * (1 + (float) boost/100));
     vampire->hit = vampire->max_hit;
     vampire->hitroll                = level*1.5 * (1 + (float) boost/100);
     vampire->damroll                = level*1.5 * (1 + (float)boost/100);
     vampire->armor[AC_BASH] = -350 * (1 + (float) boost/50);
     vampire->armor[AC_SLASH] = -300 * (1 + (float) boost/50);
     vampire->armor[AC_PIERCE] = 500;
     vampire->armor[AC_EXOTIC] = -250 * (1 + (float) boost/50);
     vampire->damage[DICE_NUMBER] = level/10 * (1 + (float) boost/100);
     vampire->damage[DICE_TYPE] = level/6 * (1 + (float) boost/100);
     if (boost > 80)
     SET_BIT( vampire->off_flags, OFF_XATTACK );
     if (boost == 100)
     SET_BIT( vampire->off_flags, OFF_XATTACK2 );

        extract_obj( obj );

        send_to_char("A vampire is brought to life from the corpse.\n\r",
                ch);
        act("$n revives an old corpse as a vampire.", ch, NULL,
                NULL, TO_ROOM);
        add_follower( vampire, ch );
        vampire->leader = ch;


        return;
    }
}

void spell_decay( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    
    af.where	 = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.modifier  = level/10;
    af.duration  = level/5;
    af.location  = APPLY_AC;
    af.bitvector = 0;    
    affect_join( victim, &af );
    send_to_char( "Your armor becomes corrosive and decays.\n\r", victim );
    act("$n's armor turns corrosive and decays",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_command( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *argument;
    CHAR_DATA *victim;

    argument = one_argument( target_name, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Command whom to do what?\n\r", ch );
	return;
    }

    one_argument(argument,arg2);
  
    if (!str_cmp(arg2,"delete") || !str_prefix(arg2,"mob"))
    {
	send_to_char("That will NOT be done.\n\r",ch);
	return;
    }

    xprintf( buf, "$n commands you to '%s'.", argument );

    
	
	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

    	
	
	act( buf, ch, NULL, victim, TO_VICT );
	interpret( victim, arg2 );
    
    send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_vampiric_touch( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
         CHAR_DATA *victim = (CHAR_DATA *) vo;
         AFFECT_DATA af;

         if ( is_affected( victim, sn ))
         {
		send_to_char("You already have a paralyzing touch.\n\r",ch);
                  return;
         }

	 if ( IS_AFFECTED2( victim, AFF2_GHOUL ))
         {
		send_to_char("Your hands are already transformed.\n\r",ch);
                  return;
         }

         af.where     = TO_AFFECTS2;
         af.type      = sn;
         af.level     = level;
         af.duration  = level/5;
         af.location  = APPLY_NONE;
         af.modifier  = 0;
         af.bitvector = AFF2_VAMPIRIC;
         affect_to_char( victim, &af );

         send_to_char( "Small thorns grow out of your hands\n\r", victim);
         act("$n's hands is covered with small thorns. ",victim,NULL,NULL,TO_ROOM);
         return;
}

void spell_protection_undead(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED2(victim, AFF2_PROTECT_UNDEAD)) 
    {
	if (victim == ch)
	  send_to_char("You are already protected.\n\r",ch);
	else
	  act("$N is already protected.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.where     = TO_AFFECTS2;
    af.type      = sn;
    af.level     = level;
    af.duration  = level/2;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF2_PROTECT_UNDEAD;
    affect_to_char( victim, &af );
    send_to_char( "You feel safe against the dead.\n\r", victim );
    if ( ch != victim )
	act("$N is protected from undead.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_control_undead( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    CHAR_DATA *gch;
    int numcharmie;

    if (is_safe(ch,victim)) return;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
	return;
    }
    
    if (!IS_SET(victim->form,FORM_UNDEAD))
    {
	send_to_char( "You can only control undeads!\n\r", ch );
	return;
    }
    
    if (IS_AFFECTED(ch, AFF_CHARM)
    ||   level <= victim->level+10
    ||   IS_SET(victim->imm_flags,IMM_CHARM)
    ||   saves_spell( level+15, victim,DAM_CHARM) )
	return;


    ch->charmie = 0;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && IS_AFFECTED(gch,AFF_CHARM))
	    ch->charmie++;
    }


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
    af.duration  = level/3;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    ch->charmie++;
    affect_to_char( victim, &af );
    act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
    if ( ch != victim )
	act("$N cold eyes glare at you as it turns to follow.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_death_door(int sn, int level, CHAR_DATA *ch, void *vo)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;


        if(is_affected(victim,gsn_death_door))
        {
        send_to_char( "You cannot boost your defences any further.\n\r",victim);
        return;
	}
        	
        af.where     = TO_AFFECTS;
    	af.type      = gsn_death_door;
    	af.level     = level;
    	af.duration  = 100;
    	af.location  = 0;
    	af.modifier  = 0;
    	af.bitvector = 0;
      	affect_to_char(victim,&af);
      	
      	af.where     = TO_AFFECTS;
    	af.type      = gsn_death_door_protect;
    	af.level     = level;
    	af.duration  = 100;
    	af.location  = 0;
    	af.modifier  = 0;
    	af.bitvector = 0;
      	affect_to_char(victim,&af);

         send_to_char( "You're now able to cheat death.\n\r",victim);
         act("$n is preparing to cheat death. .",victim,NULL,NULL,TO_ROOM);

        return ;
}

void spell_bone_armor(int sn, int level, CHAR_DATA *ch, void *vo)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;


        if(is_affected(victim,gsn_bone_armor))
        {
        send_to_char( "You are already surrounded by an armor of bones.\n\r",victim);
        return;
	}
        	
        af.where     = TO_AFFECTS2;
    	af.type      = gsn_bone_armor;
    	af.level     = level;
    	af.duration  = 5 + level/10;
    	af.location  = 0;
    	af.modifier  = 0;
    	af.bitvector = 0;
      	affect_to_char(victim,&af); 
      	victim->ironskincount = level/4;     	

         send_to_char( "Bones appear out of thin air, and covers all your vital parts.\n\r",victim);
         act("$n is covered in an armor of bones.",victim,NULL,NULL,TO_ROOM);

        return ;
}

void spell_malison(int sn, int level, CHAR_DATA *ch, void *vo)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        	
        af.where     = TO_AFFECTS;
    	af.type      = gsn_malison;
    	af.level     = level;
    	af.duration  = 5 + level/10;
    	af.location  = APPLY_SAVES;
    	af.modifier  = level/15;
    	af.bitvector = 0;
      	affect_join(victim,&af); 

         send_to_char( "Your magical defences are lowered.\n\r",victim);
         act("$n magical defence is weakened.",victim,NULL,NULL,TO_ROOM);
        return ;
}

void spell_wail_of_banshee( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    AFFECT_DATA af;
    
    if (is_affected(ch,gsn_wail_banshee))
        {
        send_to_char( "You are to drained to let out another wail.\n\r",ch);
        return;
	}
   	af.where     = TO_AFFECTS;
    	af.type      = gsn_wail_banshee;
    	af.level     = level;
    	af.duration  = 8;
    	af.location  = APPLY_HITROLL;
    	af.modifier  = -15;
    	af.bitvector = 0;
      	affect_to_char(ch,&af); 

    send_to_char( "You let out a deadly wail!\n\r", ch );
    act( "$n's lungs release a terrifying and deadly wail!", ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
    	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( is_safe(ch, vch) )
		continue;
	    if ( ch == vch )
		continue;
 	
	 if (!saves_spell(level-10,vch,DAM_NEGATIVE) && (ch->level +5 > vch->level))
	 {
	 send_to_char( "The scream crumbles your bones!\n\r", vch );
	 act( "$n collapse under the effect of the wail.!", vch, NULL, NULL, TO_ROOM );
	 raw_kill(ch,vch);	
	 continue;	
	 }
	 multi_hit(vch, ch, TYPE_UNDEFINED);
	}
	if ( vch->in_room->area == ch->in_room->area)
	    send_to_char( "A terrifying scream chills your bones.\n\r", vch );
    }
    
 

    return;
}
void spell_etherbind( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    
    if (IS_SET(victim->form,FORM_UNDEAD))
    {
    	victim->hit = UMIN( victim->hit + level*3, victim->max_hit );
    	update_pos( victim );
    	send_to_char( "A warm feeling fills your body.\n\r", victim );
	send_to_char( "ok\n\r", ch );
    }
    else
   { 
   	dam = dice(level, 7);
    	damage( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);
   }   
    return;
}
void spell_strengthen_ether( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,gsn_strengthen_ether))
     {
    	send_to_char( "Your targets link to the etherplane is as strong as it can get.\n\r", ch );
    	return;	
    }
    
    if (!IS_SET(victim->form,FORM_UNDEAD))
    {
    	send_to_char( "Your target gains no strength from the etherplane.\n\r", ch );
    	return;	
    }
    
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 6+level;
    af.location  = APPLY_HITROLL;
    af.modifier  = level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - level / 2;
    affect_to_char( victim, &af );
    
    af.location  = APPLY_DAMROLL;
    af.modifier  = level;
    affect_to_char( victim, &af );
    
    af.location  = APPLY_AC;
    af.modifier  = 0 - level*4;
    affect_to_char( victim, &af );
    
    af.location  = APPLY_HIT;
    af.modifier  = level*10;
    affect_to_char( victim, &af );
    send_to_char( "Your strengthen the bond between your target and the etherplane.\n\r", ch );
    act("$n strengthen the link between $N and the etherplane",ch,NULL,victim,TO_NOTVICT);
   
    return;
}

void spell_raise_dead( int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA   *creature;
    OBJ_DATA    *obj;
    CHAR_DATA *gch;
    int chance, numcharmie;


    obj = get_obj_here( ch, NULL, "corpse");

    if ( obj == NULL )
    {
        send_to_char("There is no corpse here.\n\r", ch);
        return;
    }    
     
  if (get_mob_index(  obj->value[0]) == NULL)
  {
  	extract_obj( obj );
  	send_to_char("The corpse is to old to be brought back.\n\r", ch);
  	return;
  }
  
    
    
chance = get_skill(ch,sn);
chance += ch->level*2;
chance -= (obj->level*2 + 20);

if (number_percent( ) > chance)
 {
  extract_obj( obj );
  send_to_char("The corpse crumble as you attempt to raise it.\n\r", ch);
  return;
 } 

   ch->charmie = 0;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && IS_AFFECTED(gch,AFF_CHARM))
	    ch->charmie++;
    }

	numcharmie = ch->level/10;


    if(numcharmie < 0)
	numcharmie = 1;

    if(ch->charmie >= numcharmie)
	{
        send_to_char("You are unable to control more followers.\n\r", ch);
        return;
    	}

    if ( obj->item_type != ITEM_CORPSE_NPC )
    {
        send_to_char("You cannot raise that.\n\r", ch);
        return;
    }
    else 
    {  
    	extract_obj( obj );
        creature = create_mobile( get_mob_index(  obj->value[0]) );
        SET_BIT( creature->form, FORM_UNDEAD );
        send_to_char("You establish a link between the corpse and the etherplane.\n\r", ch);
        act("$n grants life to the corpse.", ch, NULL, NULL, TO_ROOM);
        chance = get_skill(ch,sn);
	chance += ch->level*4;
	chance -= (creature->level*4 + 20);
	if (number_percent( ) < chance && !IS_SET(creature->imm_flags,IMM_CHARM)
	&& !IS_SET(creature->imm_flags,IMM_MAGIC))
 	{
 	//SET_BIT( creature->act, ACT_PET );
        SET_BIT( creature->affected_by, AFF_CHARM );
        char_to_room( creature, ch->in_room );
        act("$N is submissive to your will", ch, NULL, creature, TO_CHAR);
        add_follower( creature, ch );
        creature->leader = ch;
	}
	else
	{
	char_to_room( creature, ch->in_room );
		if (can_see(creature, ch))
		{
		act("$N is angered and attacks you.", ch, NULL, creature, TO_CHAR);
		act("$N is angered and attacks $n.", ch, NULL, creature, TO_NOTVICT);
		multi_hit(creature, ch, TYPE_UNDEFINED);
		}
		else
		act("$n's cold eyes prowl the surroundings for whoever disturbed its rest.", creature, NULL, NULL, TO_ROOM);
	}

    }
 return;
}

void spell_shield_of_the_dead( int sn, int level, CHAR_DATA *ch, void *vo)
{
    OBJ_DATA    *obj;
    int chance;
    AFFECT_DATA af;

    obj = get_obj_here( ch, NULL,  "corpse");

    if ( obj == NULL )
    {
        send_to_char("There is no corpse here.\n\r", ch);
        return;
    }    
     
    if (is_affected(ch,sn))
     {
    	send_to_char( "You can't draw more strength from the dead.\n\r", ch );
    	return;	
     }
    
    
chance = get_skill(ch,sn);
chance += ch->level*3;
chance -= obj->level*3;

if (number_percent( ) > chance)
 {
  extract_obj( obj );
  send_to_char("The corpse crumble as you attempt to raise it.\n\r", ch);
  return;
 } 

 
    if ( obj->item_type != ITEM_CORPSE_NPC )
    {
        send_to_char("You cannot raise that.\n\r", ch);
        return;
    }
    else
    {
    	af.where     	= TO_AFFECTS;
    	af.type      	= sn;
    	af.level 	= obj->level;
    	af.duration  	= obj->level/2;
    	af.location  	= APPLY_AC;
    	af.modifier  	= 0-obj->level;
    	af.bitvector 	= 0;
    	affect_to_char( ch, &af );
    	
    	af.location  	= APPLY_HITROLL;
    	af.modifier  	= obj->level/5;
    	affect_to_char( ch, &af );
    	
    	af.location  	= APPLY_DAMROLL;
    	af.modifier  	= obj->level/5;
    	affect_to_char( ch, &af );
    	
    	af.location  	= APPLY_HIT;
    	af.modifier  	= obj->level;
    	affect_to_char( ch, &af );
    	
    	af.location  	= APPLY_SAVING_SPELL;
    	af.modifier  	= 0 - obj->level/4;
    	affect_to_char( ch, &af );
    	
    	if (obj->level > 100)
    	{
    	af.where     	= TO_AFFECTS2;
    	af.duration  	= obj->level/10;
    	af.bitvector 	= AFF2_INVUL;
    	affect_to_char( ch, &af );
	}
     }
     
	extract_obj( obj );
	send_to_char("You draw strength from the corpse.\n\r", ch);
	act("$n draws energy from the corpse.", ch, NULL, NULL, TO_ROOM);
  
 return;
}

void spell_blade_barrier( int sn, int level, CHAR_DATA *ch, void *vo)
{
    AFFECT_DATA af;
     
    if (IS_AFFECTED2(ch,AFF2_BARRIER))
     {
    	send_to_char( "No room for more hammers.\n\r", ch );
    	return;	
     }






    	af.where     	= TO_AFFECTS2;
    	af.type      	= sn;
    	af.level 	= level;
    	af.duration  	= level/8;
    	af.location  	= 0;
    	af.modifier  	= 0;
    	af.bitvector 	= AFF2_BARRIER;
    	affect_to_char( ch, &af );
     
	send_to_char("Several hammers start swirling around you at high velocity.\n\r", ch);
	act("$n is surrounded by numerous swirling hammers.", ch, NULL, NULL, TO_ROOM);
  
 return;
}

void spell_imbue(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *obj;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;

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
     if ( !str_cmp( arg2, "sanc" ) || !str_cmp( arg2, "sanctuary" ) 
      || !str_cmp( arg2, "fly" ) || !str_cmp( arg2, "flying" )
      || !str_cmp( arg2, "invis" ) || !str_cmp( arg2, "invisible" )
      || !str_cmp( arg2, "passdoor" ) || !str_cmp( arg2, "protectionevil" )
      || !str_cmp( arg2, "protevil" ) || !str_cmp( arg2, "protectevil" )
      || !str_cmp( arg2, "haste" ))
      {
	paf = new_affect();

	paf->where	= TO_AFFECTS;
	paf->level	= level;
	paf->duration	= 2;
	
	if ( !str_cmp( arg2, "sanc" ) || !str_cmp( arg2, "sanctuary" ) )
	{
	paf->type	= gsn_sanctuary;
        paf->bitvector  = AFF_SANCTUARY;
        paf->next       = obj->affected;
    	obj->affected   = paf;
    	send_to_char("sanc\n\r",ch);
	return;
	}
	if ( !str_cmp( arg2, "fly" ) || !str_cmp( arg2, "flying" ) )
	{
	paf->type	= gsn_fly;
        paf->bitvector  = AFF_FLYING;
        paf->next       = obj->affected;
    	obj->affected   = paf;
    	send_to_char("fly\n\r",ch);
	return;
	}
	if ( !str_cmp( arg2, "passdoor" ))
	{
	paf->type	= sn;
        paf->bitvector  = AFF_PASS_DOOR;
        paf->next       = obj->affected;
    	obj->affected   = paf;
    	send_to_char("passdoor\n\r",ch);
	return;
	}
	if ( !str_cmp( arg2, "invis" ) || !str_cmp( arg2, "invisible" ) )
	{
	paf->type	= gsn_invis;
        paf->bitvector  = AFF_INVISIBLE;
        paf->next       = obj->affected;
    	obj->affected   = paf;
    	send_to_char("invis\n\r",ch);
	return;
	}
	if ( !str_cmp( arg2, "haste" ))
	{
	paf->type	= gsn_haste;
        paf->bitvector  = AFF_HASTE;
        paf->next       = obj->affected;
    	obj->affected   = paf;
    	send_to_char("haste\n\r",ch);
	return;
	}
	if ( !str_cmp( arg2, "protevil" ) || !str_cmp( arg2, "protectionevil" )
	|| !str_cmp( arg2, "protectevil" ) )
	{
	paf->type	= sn;
        paf->bitvector  = AFF_PROTECT_EVIL;
        paf->next       = obj->affected;
    	obj->affected   = paf;
    	send_to_char("protevil\n\r",ch);
	return;
	}
     }
     else
     {
	send_to_char("That is not an acceptable affect\n\r",ch);
	return;
     }
	
    return;
}

void spell_resurrection( int sn, int level, CHAR_DATA *ch, void *vo)
{
    OBJ_DATA    *obj;
    OBJ_DATA *equip, *equip_next;
    CHAR_DATA *victim;


    obj = get_obj_here( ch, NULL,  "corpse");

    if ( obj == NULL )
    {
        send_to_char("There is no corpse here.\n\r", ch);
        return;
    }
    
         
    if ( obj->item_type != ITEM_CORPSE_PC )
    {
        send_to_char("You cannot raise that.\n\r", ch);
        return;
    }
    else 
    {  
        if ( ( (victim = get_char_world( ch, obj->owner ) ) == NULL)
        || (victim->in_room != get_room_index(ROOM_VNUM_AFTERLIFE)))
        {
        	send_to_char("The corpse is unsuitable at the moment.\n\r",ch);
        	return;
        }
    	for ( equip = obj->contains; equip != NULL; equip = equip_next )
       {
            equip_next = equip->next_content;
            obj_from_obj( equip );
            obj_to_char( equip, victim );
        }
        send_to_char("You grant life to the corpse.\n\r", ch);
        act("$n brings $N back to life.", ch, NULL, victim, TO_ROOM);
        send_to_char("You have been granted life.\n\r", victim);
        char_from_room( victim );
    	char_to_room( victim, ch->in_room );
    	extract_obj( obj );
    	victim->hit = victim->max_hit;
    	victim->mana = victim->max_mana;
    	victim->move = victim->max_move;
    	do_look( victim, "auto" );

    }
 return;
}

void spell_revive( int sn, int level, CHAR_DATA *ch, void *vo)
{
    OBJ_DATA    *obj;
    OBJ_DATA *equip, *equip_next;
    CHAR_DATA *victim;
    AFFECT_DATA af;


    obj = get_obj_here( ch, NULL,  "corpse");

    if ( obj == NULL )
    {
        send_to_char("There is no corpse here.\n\r", ch);
        return;
    }
    
         
    if ( obj->item_type != ITEM_CORPSE_PC )
    {
        send_to_char("You cannot raise that.\n\r", ch);
        return;
    }
    else 
    {  
        if ( ( (victim = get_char_world( ch, obj->owner ) ) == NULL)
        || (victim->in_room != get_room_index(ROOM_VNUM_AFTERLIFE)))
        {
        	send_to_char("The corpse is unsuitable at the moment.\n\r",ch);
        	return;
        }
    	for ( equip = obj->contains; equip != NULL; equip = equip_next )
       {
            equip_next = equip->next_content;
            obj_from_obj( equip );
            obj_to_char( equip, victim );
        }
        send_to_char("You grant life to the corpse.\n\r", ch);
        act("$n brings $N back to life.", ch, NULL, victim, TO_ROOM);
        send_to_char("You have been granted life.\n\r", victim);
        char_from_room( victim );
    	char_to_room( victim, ch->in_room );
    	extract_obj( obj );
    	victim->hit = 1;
    	victim->mana = 1;
    	victim->move = victim->max_move;
    	af.where     	= TO_AFFECTS;
    	af.type      	= sn;
    	af.level 	= level;
    	af.duration  	= 100;
    	af.location  	= APPLY_HIT;
    	af.modifier  	= 0 - victim->max_hit/2;
    	af.bitvector 	= 0;
    	affect_to_char( victim, &af );
    	af.location  	= APPLY_MANA;
    	af.modifier  	= 0 - victim->max_mana/2;
    	affect_to_char( victim, &af );
    	af.location  	= APPLY_SAVING_SPELL;
    	af.modifier  	= victim->level/2;
    	affect_to_char( victim, &af );
    	af.location  	= APPLY_HITROLL;
    	af.modifier  	= 0 - victim->level/2;
    	affect_to_char( victim, &af );
    	af.location  	= APPLY_DAMROLL;
    	af.modifier  	= 0 - victim->level/2;
    	affect_to_char( victim, &af );
    	af.where     	= TO_AFFECTS2;
    	af.type      	= sn;
    	af.level 	= level;
    	af.duration  	= 10;
    	af.bitvector 	= AFF2_WITHER;
    	affect_to_char( victim, &af );
    	do_look( victim, "auto" );

    }
 return;
}
