/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
/* #include "magic.h" */
#include "recycle.h"
#include "tables.h"
#include "lookup.h"

DECLARE_DO_FUN( do_exits	);
DECLARE_DO_FUN( do_map		);
DECLARE_DO_FUN( do_murder	);
DECLARE_DO_FUN( do_say		);
DECLARE_DO_FUN( do_help		);
DECLARE_DO_FUN( do_font		);

char *get_stat_alias( CHAR_DATA *ch, int which );

char *	const	where_name	[] =
{
    "{Y<used as a light>{x   ",
    "{Y<floating nearby>{x   ",
    "{Y<ornamentation>{x     ",
    "{Y<ornamentation>{x     ",
    "{Y<worn on finger>{x    ",
    "{Y<worn on finger>{x    ",
    "{Y<worn around neck>{x  ",
    "{Y<worn around neck>{x  ",
    "{Y<worn on torso>{x     ",
    "{Y<worn on head>{x      ",
    "{Y<worn on legs>{x      ",
    "{Y<worn on feet>{x      ",
    "{Y<worn on hands>{x     ",
    "{Y<worn on arms>{x      ",
    "{Y<worn as shield>{x    ",
    "{Y<worn about body>{x   ",
    "{Y<worn about waist>{x  ",
    "{Y<worn on wrist>{x     ",
    "{Y<worn on wrist>{x     ",
    "{Y<held>{x              ",
    "{Y<primary weapon>{x    ",
    "{Y<secondary weapon>{x  ",
    /*"{Y(branded){x           ",*/
};


/* for do_count */
int max_on = 0;



/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );
bool	is_mirror		args( ( CHAR_DATA *ch ) );
bool    where_safe		args( ( CHAR_DATA *ch, CHAR_DATA *victim) );

bool where_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
  int chexp,vchexp;
  /* Code by Engur */
  if (IS_NPC(victim) && IS_MIRROR(victim) && !IS_NPC(ch)) {
	chexp = ch->exp;
	vchexp = victim->exp;
  if ( IS_SET(victim->in_room->room_flags,ROOM_SAFE) && !IS_IMMORTAL(ch)) 
   return TRUE;
  if ( ch != victim && !IS_IMMORTAL(ch) &&  
      ( (chexp < PK_MIN) || (vchexp < PK_MIN) ||
        ( ((float) (chexp - vchexp)) / (float) chexp > 0.3) ||
	( ((float) (vchexp - chexp)) / (float) vchexp > 0.3) ) )
    return TRUE;
  if ( ch != victim && !IS_IMMORTAL(ch) &&
      ((ch->level >= victim->practice + 10) || (victim->practice >= ch->level + 10)))
    return TRUE;
  return FALSE;
  }
 return FALSE;
} 

 

bool is_mirror(CHAR_DATA *ch) 
{
   if (!IS_NPC(ch)) 
	return FALSE;
   if (IS_MIRROR(ch))
	return TRUE;
   return FALSE;
}


char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];
    static char buf_con[100];
    buf[0] = '\0';
    buf_con[0] = '\0';

    if (obj->pIndexData->vnum > 5)      /* money, gold, etc */
        xprintf(buf_con," {W[{Y%d%%{W]{w",obj->condition);
    else buf_con[0] = '\0';

    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))
    ||  (obj->description == NULL || obj->description[0] == '\0'))
	return buf;
/*  adds hidden items */
    if ( IS_OBJ_STAT(obj, ITEM_HIDDEN)     )   strcat( buf, "{D(Hidden){g ");
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "{m(Invis){g ");
    if ( CAN_DETECT(ch, DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_EVIL)   )   strcat( buf, "{R(Red Aura){g ");
    if ( CAN_DETECT(ch, DETECT_GOOD)
    &&  IS_OBJ_STAT(obj,ITEM_BLESS))	      strcat(buf,"{B(Blue Aura){g " );
    if ( CAN_DETECT(ch, DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "{M(Magical){g " );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "{Y(Glowing){g " );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "{G(Humming){g " );
   if ( IS_OBJ_STAT(obj, ITEM_INDESTRUCT ))   strcat(buf,"{W({MINDEST{W){t ");

        if( IS_WEAPON_STAT(obj, WEAPON_FLAMING) && CAN_WEAR(obj,ITEM_WIELD))
         strcat(buf,"{R(Fl{rami{Rng){w ");
        if( IS_WEAPON_STAT(obj, WEAPON_SHOCKING) && CAN_WEAR(obj,ITEM_WIELD))
        strcat(buf,"{W(Sho{wck{Wing){w ");
        if( IS_WEAPON_STAT(obj, WEAPON_FROST) && CAN_WEAR(obj,ITEM_WIELD))
        strcat(buf,"{W(F{Bros{Wt) {w");
        if( IS_WEAPON_STAT(obj, WEAPON_MANADRAIN) && CAN_WEAR(obj,ITEM_WIELD))
        strcat(buf,"{M({mAbsorbing{M) {w");
         if (  obj->pIndexData->limit != -1)
         strcat(buf,"{R(Limited){g");

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
	     strcat( buf, buf_con );
    }
    else
    {
	if ( obj->description != NULL)
	    strcat( buf, obj->description );
    }

    return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    output = new_buf();

    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj )) 
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );

	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if (prgpstrShow[iShow][0] == '\0')
	{
	    free_string(prgpstrShow[iShow]);
	    continue;
	}

	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		xprintf( buf, "(%2d) ", prgnShow[iShow] );
		add_buf(output,buf);
	    }
	    else
	    {
		add_buf(output,"     ");
	    }
	}
	add_buf(output,prgpstrShow[iShow]);
	add_buf(output,"\n\r");
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }
    page_to_char(buf_string(output),ch);

    /*
     * Clean up.
     */
    free_buf(output);
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );

    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH],message[MAX_STRING_LENGTH];

    buf[0] = '\0';

	if( !IS_NPC(ch))
    	if (IS_NPC(victim) &&ch->questmob > 0 
	&& victim->pIndexData->vnum == ch->questmob)
        strcat( buf, "{R[{rTARGET{R]{B ");

    if ( RIDDEN(victim) )
     {
      if ( ch != RIDDEN(victim) )
	{
	 strcat( buf, "(Mounted) " );
	}   
       else
	{
	 strcat( buf, "(Your mount) " );
	} 
     }
    if ( IS_SET(victim->comm,COMM_BUSY	  )   ) strcat( buf, "{r[BUSY WRITING]{B " );
    if ( IS_SET(victim->comm,COMM_AFK	  )   ) strcat( buf, "{R[AFK]{B " );
    if ( !IS_NPC( victim ) && IS_SET( victim->act, PLR_QUESTING  ) )
    strcat( buf, "{W[{rQ{RU{YE{RS{rT{w] "      );
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "{m(Invis){B " );
 //   if ( IS_AFFECTED(victim, AFF_IMPROVED_INVISIBLE)   ) strcat( buf, "{m(Invis){B " );
    if ( victim->invis_level >= LEVEL_HERO    ) strcat( buf, "{W(Wizi){B " );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "{g(Hide){B " );
    if ( IS_AFFECTED(victim, AFF_FADE)        ) strcat( buf, "{g(Wraith){B");
    if ( IS_AFFECTED(victim, AFF_CAMOUFLAGE)  ) strcat( buf, "{g(Camouflaged){B" );
    if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "{m(Charmed){B " );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "{C(Translucent){B ");
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "{M(Pink Aura){B "  );
    if ( IS_AFFECTED(victim, AFF_FAERIE_FOG) ) strcat( buf, "{m(Purple Aura){B " );
    if ( !IS_NPC(victim) && (!victim->desc )) strcat( buf, "{C(Linkless){B ");
    if(is_affected(victim,skill_lookup("fire shield")))
			strcat(buf, "{R(Fiery Aura){B ");
    if(is_affected(victim,skill_lookup("chill shield")))
			strcat(buf, "{C(Blue Aura){B ");
    if ( IS_EVIL(victim)
    &&   CAN_DETECT(ch, DETECT_EVIL)     ) strcat( buf, "{r(Red Aura){B ");
    if ( IS_GOOD(victim)
    &&   CAN_DETECT(ch, DETECT_GOOD)     ) strcat( buf, "{Y(Golden Aura){B ");
    if (IS_NPC(victim) && (IS_SET(victim->form,FORM_UNDEAD))
    &&   CAN_DETECT(ch, DETECT_UNDEAD)     ) strcat( buf, "{D(Undead){B ");
    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "{W(White Aura){B " );
    if ( IS_AFFECTED2(victim, AFF2_INVUL)   ) strcat( buf, "{W(Invul){B ");
    if ( CAN_DETECT(ch, DETECT_MAGIC) /*Made vamp aura req det magic to see- Zalev 1/5/08*/
    &&  IS_AFFECTED2(victim, AFF2_CHARISMATIC)   ) strcat( buf, "{M(Charismatic){B ");
    if ( IS_AFFECTED2(victim, AFF2_STONESHAPE)   ) strcat( buf, "{D(Statue){B ");
    if ( IS_AFFECTED2(victim, AFF2_SUBMERGED)   ) strcat( buf, "{m(Submerged){B ");
    if ( IS_AFFECTED2(victim, AFF2_PHASING)   ) strcat( buf, "{c(Etheral){B ");
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER ) )
						strcat( buf, "{r(KILLER){B " );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF  ) )
						strcat( buf, "{r(THIEF){B " );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_WANTED ) )
      strcat( buf, "{r(WANTED){B ");
    if ( IS_SET(victim->tag_flags,TAG_RED) ) strcat( buf,"{R[RED] {B"  );
    if ( IS_SET(victim->tag_flags,TAG_BLUE)) strcat( buf,"{B[BLUE] {B" );
    if (  (victim->size == SIZE_TINY)) strcat( buf,"{y(Tiny) {B" );
    if (  (victim->size == SIZE_SMALL)) strcat( buf,"{y(Small) {B" );
    if (  (victim->size == SIZE_MEDIUM)) strcat( buf,"{y(Medium) {B" );
    if (  (victim->size == SIZE_LARGE)) strcat( buf,"{y(Large) {B" );
    if (  (victim->size == SIZE_HUGE)) strcat( buf,"{y(Huge) {B" );
    if (  (victim->size == SIZE_GIANT)) strcat( buf,"{y(Giant) {B" );

    if ( victim->position == victim->start_pos && victim->long_descr[0] != '\0' )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	return;
    }

    if ( IS_SET(ch->act,PLR_HOLYLIGHT) && is_affected(victim,gsn_doppelganger))
      {
        strcat( buf, "{{"); strcat(buf, PERS(victim,ch)); strcat(buf, "} ");
      }

    if (is_affected(victim,gsn_doppelganger)  &&
        victim->doppel->long_descr[0] != '\0') {
      strcat( buf, victim->doppel->long_descr);
      send_to_char(buf, ch);
      return;
      }

/*    if (victim->long_descr[0] != '\0' &&
      !is_affected(victim,gsn_doppelganger)) {
      strcat( buf, victim->long_descr );
      send_to_char(buf, ch);
      return;
    }                                           */

    if (is_affected(victim, gsn_doppelganger))
      {
        if (!IS_NPC(victim->doppel) && !IS_SET(ch->comm, COMM_BRIEF))
          strcat(buf, victim->doppel->pcdata->pretitle);
        strcat(buf, PERS(victim->doppel, ch ));
        if (!IS_NPC(victim->doppel) && !IS_SET(ch->comm, COMM_BRIEF))
          strcat(buf, victim->doppel->pcdata->title);
      }
   else  
   {
     if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF) 
     &&   victim->position == POS_STANDING && ch->on == NULL )
	strcat( buf, victim->pcdata->pretitle );
     strcat( buf, PERS( victim, ch ) );
     if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF) 
     &&   victim->position == POS_STANDING && ch->on == NULL )
 	 strcat( buf, victim->pcdata->title );
    }

strcat(buf, "{B");

    switch ( victim->position )
    {
    case POS_DEAD:     strcat( buf, " is {rDEAD!!{x" );
break;
    case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;
    case POS_INCAP:    strcat( buf, " is incapacitated." );      break;
    case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
    case POS_SLEEPING: 
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],SLEEP_AT))
  	    {
		xprintf(message," is sleeping at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],SLEEP_ON))
	    {
		xprintf(message," is sleeping on %s.",
		    victim->on->short_descr); 
		strcat(buf,message);
	    }
	    else
	    {
		xprintf(message, " is sleeping in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else 
	    strcat(buf," is sleeping here.");
	break;
    case POS_RESTING:  
        if (victim->on != NULL)
	{
            if (IS_SET(victim->on->value[2],REST_AT))
            {
                xprintf(message," is resting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],REST_ON))
            {
                xprintf(message," is resting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else 
            {
                xprintf(message, " is resting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
	}
        else
	    strcat( buf, " is resting here." );       
	break;
    case POS_SITTING:  
        if (victim->on != NULL)
        {
            if (IS_SET(victim->on->value[2],SIT_AT))
            {
                xprintf(message," is sitting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],SIT_ON))
            {
                xprintf(message," is sitting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
                xprintf(message, " is sitting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
        }
        else
	    strcat(buf, " is sitting here.");
	break;
    case POS_STANDING: 
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],STAND_AT))
	    {
		xprintf(message," is standing at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],STAND_ON))
	    {
		xprintf(message," is standing on %s.",
		   victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
		xprintf(message," is standing in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
        else if (MOUNTED(victim))
        {
          xprintf(message," is here, riding %s.",PERS(MOUNTED(victim),ch));
          strcat(buf, message);
        }
	else
	    strcat( buf, " is here." );               
	break;
    case POS_FIGHTING:
	strcat( buf, " is here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "thin air??" );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "somone who left??" );
	break;
    }

    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}



void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;
    CHAR_DATA *tmp_vict;

    tmp_vict = is_affected(victim,gsn_doppelganger) ? victim->doppel : victim;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "{C$n looks $mself over.{x",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "{C$n looks you over.{x", ch, NULL, victim, TO_VICT    );
	    act( "{C$n looks $N over.{x",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    if ( tmp_vict->description[0] != '\0' )
    {
	send_to_char( "{B", ch );
	send_to_char( tmp_vict->description, ch );
	send_to_char( "{x", ch );
    }
    else
    {
	act( "{cYou see nothing special about $M.{x", ch, NULL, victim,TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    //strcpy( buf, PERS(tmp_vict, ch) );
	xprintf(buf,"%s",PERS(tmp_vict, ch));
    if (percent >= 100) 
	xprintf(buf,"%s is in perfect health.{x\n\r",buf);
//	strcat( buf, " is in perfect health.{x\n\r");
    else if (percent >= 90) 
	xprintf(buf,"%s has a few scratches.{x\n\r",buf);
//	strcat( buf, " has a few scratches.{x\n\r");
    else if (percent >= 75) 
	xprintf(buf,"%s has some small but disgusting cuts.{x\n\r",buf);
//	strcat( buf," has some small but disgusting cuts.{x\n\r");
    else if (percent >=  50) 
	xprintf(buf,"%s  is covered with bleeding wounds.{x\n\r",buf);
//	strcat( buf, " is covered with bleeding wounds.{x\n\r");
    else if (percent >= 30)
	xprintf(buf,"%s is gushing blood.{x\n\r",buf);
//	strcat( buf, " is gushing blood.{x\n\r");
    else if (percent >= 15)
	xprintf(buf,"%s is writhing in agony.{x\n\r",buf);
//	xcatf( buf, " is writhing in agony.{x\n\r");
    else if (percent >= 0 )
	xprintf(buf,"%s is convulsing on the ground.{x\n\r",buf);
//	xcatf(buf, " is convulsing on the ground.{x\n\r");
    else
	xprintf(buf,"%s is nearly dead.{x\n\r",buf);
//	strcat(buf, " is nearly dead.{x\n\r");

    buf[0] = UPPER(buf[0]);
    send_to_char( "{B", ch );
    send_to_char( buf , ch );
    send_to_char( "{x", ch );

    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( 
		      is_affected(victim,gsn_mirror) ? tmp_vict : victim, 
				             iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "{x\n\r", ch );
		act( "{r$N is using:{x", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "{x\n\r", ch );
	}
    }
    
    /*[TAKEN OUT BY MAKUS]
    if (victim->race == race_lookup("ogre") && !IS_NPC(victim))
    if (victim->pcdata->pkills > 1)
     {
  	 xprintf(buf,"{W(*-*){C%s has %d old bones attached to your belt.{W(*-*){x\n\r",victim->sex == 0 ? "It" : victim->sex == 1 ? "He" : "She",
  	 victim->pcdata->pkills);
   	 send_to_char(buf,ch);
     }
[TAKEN OUT BY MAKUS]*/
    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   number_percent( ) < get_skill(ch,gsn_peek))
    {
	send_to_char( "\n\r{GYou peek at the inventory:{x\n\r", ch );
	check_improve(ch,gsn_peek,TRUE,4);
	    send_to_char( "{g",ch);
	show_list_to_char( is_affected(victim,gsn_mirror) ?
                           tmp_vict->carrying : victim->carrying,
                           ch, TRUE, TRUE );
	    send_to_char( "{x",ch);

    }

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( get_trust(ch) < rch->invis_level)
	    continue;

	if ( can_see( ch, rch ) )
	{
	    show_char_to_char_0( rch, ch );
	}
	else if ( room_is_dark( ch->in_room ,ch)
	&&        IS_AFFECTED(rch, AFF_INFRARED ) )
	{
	    send_to_char( "You see glowing {rred{x eyes watching YOU!\n\r", ch );
	}
    }

    return;
} 



bool check_blind( CHAR_DATA *ch )
{

    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
    { 
	send_to_char( "{CYou can't see a thing!{x\n\r", ch ); 
	return FALSE; 
    }

    return TRUE;
}

void do_clear( CHAR_DATA *ch, char *argument )
{
  if (!IS_NPC(ch))
    send_to_char("\033[0;0H\033[2J", ch );
  return;
}

void do_whoami( CHAR_DATA *ch, char *argument )
{
	char buf[MAX_STRING_LENGTH];	

   if (IS_NPC(ch))
        return;  

    xprintf
    ( buf,
    "{m%s %s of %d years, born under the sign of %s{x\n\r",
	capitalize(race_table[ch->race].name),
	ch->sex == 0 ? "neuter" : ch->sex == 1 ? "male" : "female",
     get_age(ch), IS_NPC(ch) ? "the beast" : ch->pcdata->sign);
    send_to_char(buf,ch);

/*  adds clan info shower */
 if (!IS_NPC(ch))   /* pc's only */
  {
   if (ch->kingdom > 0) /* only if in kingdom */
    {
     if (ch->kingdom_rank==9)
      {
       xprintf(buf,"{m%s, %s, is yours to lead, how fortunate.{x\n\r",
        capitalize(kingdom_table[ch->kingdom].short_name),kingdom_table[ch->kingdom].long_name);
       send_to_char(buf,ch);
      }
     else
      {
       xprintf(buf,"{m%s has promoted you to rank %d, %s.{x\n\r",
	capitalize(kingdom_table[ch->kingdom].short_name),
	ch->kingdom_rank+1,
        kingdom_table[ch->kingdom].rank_titles[ch->kingdom_rank]);
       send_to_char(buf,ch);
      }
     }
    }
/*  ends clan info */

	xprintf
     ( buf,
     "{mYou are a level %d %s trying to survive in %s.{x\n\r",
	ch->level,
 	IS_NPC(ch) ? "mobile" : capitalize(class_table[ch->class].name),
	IS_NPC(ch) ? "Sanaria" : hometown_table[ch->hometown].name
     );
    	send_to_char( buf, ch );

     if(ch->legendlevel > 0)
     xprintf(buf,"{mYou are legendlevel %d.{x\n\r",
		ch->legendlevel);

/* add pkill shower */
if (!IS_NPC(ch))
{
   if (ch->pcdata->pkills == 0)
   {
   xprintf(buf,"{mYou have managed to stay clear of Fate's murderous customs.{x\n\r");
   send_to_char(buf,ch);
   }

   if (ch->pcdata->pkills > 1)
   {
   xprintf(buf,"{mIn your travels, you have managed to slay %d of your enemies.{x\n\r",ch->pcdata->pkills);
   send_to_char(buf,ch);
   }

   if (ch->pcdata->pkills == 1)
   {
   xprintf(buf,"{mYou have managed to kill a single enemy in your travels.{x\n\r");
   send_to_char(buf,ch);
   }
}
/*  end */

/* add total_death shower */
if (!IS_NPC(ch))
{
   if 
   ((ch->pcdata->total_death > 1) && (ch->pcdata->total_death < 10))
   {
   xprintf(buf,"{mYou bear the scars of %d deaths, a somewhat trifling matter.{x\n\r",ch->pcdata->total_death);
   send_to_char(buf,ch);
   }

   if (ch->pcdata->total_death > 9)
   {
   xprintf(buf,"{mYou bear the scars of %d deaths, you might be better off dead.{x\n\r",ch->pcdata->total_death);
   send_to_char(buf,ch);
   }

   if (ch->pcdata->total_death == 1)
   {
   xprintf(buf,"{mYou have suffered but a single death, consider yourself lucky.{x\n\r");
   send_to_char(buf,ch);
   }

   if (ch->pcdata->total_death == 0)
   {
   xprintf(buf,"{mYou have never been killed, and should be proud of yourself.{x\n\r");
   send_to_char(buf,ch);
   }
}

if (ch->religion > 0) {
xprintf(buf,"You believes the religion of %s.\n\r",
        IS_NPC(ch) ? "Takeda" : religion_table[ch->religion].leader);
    send_to_char(buf,ch);
} else send_to_char("{mYou believe in no religion, except yourself.\n\r",ch);

if( (ch->pcdata->spouse != NULL) && (strlen(ch->pcdata->spouse) > 1))  {
	xprintf(buf, "{mYou are married to %s.{w\n\r",ch->pcdata->spouse);
	send_to_char(buf,ch);
        }
else send_to_char("{mYou are unmarried, lucky you.{w\n\r",ch);

/*  end */
  return;
}

/* changes your scroll */
void do_scroll(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int lines;

    one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {
	if (ch->lines == 0)
	    send_to_char("{gYou do not page long messages.{x\n\r",ch);
	else
	{
	    xprintf(buf,"{gYou currently display %d lines per page.{x\n\r",
		    ch->lines + 2);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!is_number(arg))
    {
	send_to_char("{gYou must provide a number.{x\n\r",ch);
	return;
    }

    lines = atoi(arg);

    if (lines == 0)
    {
        send_to_char("{gPaging disabled.{x\n\r",ch);
        ch->lines = 0;
        return;
    }

    if (lines < 10 || lines > 100)
    {
	send_to_char("{gYou must provide a reasonable number.{x\n\r",ch);
	return;
    }

    xprintf(buf,"{gScroll set to %d lines.{x\n\r",lines);
    send_to_char(buf,ch);
    ch->lines = lines - 2;
}

/* RT does socials */
void do_socials(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;
     
    col = 0;
   
    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
    {
	xprintf(buf,"{g%-12s{x",social_table[iSocial].name);
	send_to_char(buf,ch);
	if (++col % 6 == 0)
	    send_to_char("\n\r",ch);
    }

    if ( col % 6 != 0)
	send_to_char("\n\r",ch);
    return;
}


 
/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd(CHAR_DATA *ch, char *argument)
{
	do_help(ch,"motd");
}

void do_update(CHAR_DATA *ch, char *argument)
{
	do_help(ch,"update");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{  
	do_help(ch,"imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
	do_help(ch,"rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
	do_help(ch,"story");
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
	do_help(ch,"wizlist");
}

/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    send_to_char("{g Action       Status\n\r",ch);
    send_to_char("---------------------\n\r",ch);
 
    send_to_char("Autoassist     ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch); 

    send_to_char("Autoexit       ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("Autogold       ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("Autoloot       ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("Autosac        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("Autosplit      ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("Autodamage     ",ch);
     if (IS_SET(ch->act,PLR_AUTODAMAGE))
        send_to_char("ON\n\r",ch);
        else
        send_to_char("OFF\n\r",ch);

    send_to_char("Color          ",ch);
    if (IS_SET(ch->act,PLR_COLOR))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("Compact mode   ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("Prompt         ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("Combine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    if (IS_SET(ch->act,PLR_NOSUMMON))
	send_to_char("You can only be summoned players within your PK range.\n\r",ch);
    else
	send_to_char("You can be summoned by anyone.\n\r",ch);
   
    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("You do not welcome followers.{x\n\r",ch);
    else
	send_to_char("You accept followers.{x\n\r",ch);
    do_scroll(ch,"");
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
    
    if (IS_SET(ch->act,PLR_AUTOASSIST))
    {
      send_to_char("Autoassist removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOASSIST);
    }
    else
    {
      send_to_char("You will now assist when needed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOASSIST);
    }
}

void do_autoexit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOEXIT))
    {
      send_to_char("Exits will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOEXIT);
    }
    else
    {
      send_to_char("Exits will now be displayed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOEXIT);
    }
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOGOLD))
    {
      send_to_char("Autogold removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOGOLD);
    }
    else
    {
      send_to_char("Automatic gold looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOGOLD);
    }
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOLOOT))
    {
      send_to_char("Autolooting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOLOOT);
    }
    else
    {
      send_to_char("Automatic corpse looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOLOOT);
    }
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOSAC))
    {
      send_to_char("Autosacrificing removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSAC);
    }
    else
    {
      send_to_char("Automatic corpse sacrificing set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSAC);
    }
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    {
      send_to_char("Autosplitting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSPLIT);
    }
    else
    {
      send_to_char("Automatic gold splitting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSPLIT);
    }
}

void do_autodamage(CHAR_DATA *ch, char *argument)
{
   if (IS_NPC(ch))
    return;

    if (IS_SET(ch->act,PLR_AUTODAMAGE))
    {
     send_to_char("Damage will not be shown.\n\r",ch);
     REMOVE_BIT(ch->act,PLR_AUTODAMAGE);
        }
     else
        {
        send_to_char("Damage will now be shown.\n\r",ch);
        SET_BIT(ch->act,PLR_AUTODAMAGE);
        }
}

void do_brief(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_BRIEF))
    {
      send_to_char("Full descriptions activated.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BRIEF);
    }
    else
    {
      send_to_char("Short descriptions activated.\n\r",ch);
      SET_BIT(ch->comm,COMM_BRIEF);
    }
}

void do_compact(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMPACT))
    {
      send_to_char("Compact mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMPACT);
    }
    else
    {
      send_to_char("Compact mode set.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMPACT);
    }
}

void do_show(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
    {
      send_to_char("Affects will no longer be shown in score.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
    else
    {
      send_to_char("Affects will now be shown in score.\n\r",ch);
      SET_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
}

void do_prompt(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
 
 
   
   restor_percent(argument);
   
   
   if ( argument[0] == '\0' )
   {
	if (IS_SET(ch->comm,COMM_PROMPT))
   	{
      	    send_to_char("You will no longer see prompts.\n\r",ch);
      	    REMOVE_BIT(ch->comm,COMM_PROMPT);
    	}
    	else
    	{
      	    send_to_char("You will now see prompts.\n\r",ch);
      	    SET_BIT(ch->comm,COMM_PROMPT);
    	}
       return;
   }
 
   if( !strcmp( argument, "all" ) )
      strcpy( buf, "%1 {Rhp %2 {Bm %3 {Gmv {Y[{C%X{Y]{C tnl{w ");
   else
   if( !strcmp( argument, "zalev" ) )
      strcpy( buf, "{C%R {B%z{x ");
   else
   if( !strcmp( argument, "one" ) )
      strcpy( buf, "{M<{R%h/%H{c-{B%m/%M{c-{G%v/%V{c-{Y%g{c-{C%Xtnl{M>{x ");
   else
   {
      if ( strlen(argument) > 50 )
         argument[50] = '\0';
      strcpy( buf, argument );
      smash_tilde( buf );
      if (str_suffix("%c",buf))
	strcat(buf," ");
	
   }
 
   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   xprintf(buf,"Prompt set to %s\n\r",ch->prompt );
   send_to_char(buf,ch);
   return;
}

void do_combine(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMBINE))
    {
      send_to_char("Long inventory selected.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMBINE);
    }
    else
    {
      send_to_char("Combined inventory selected.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMBINE);
    }
}

void do_noloot(CHAR_DATA *ch, char *argument)
{
    send_to_char("Anyone may loot your corpse.\n\r",ch);
    return;
}

void do_nofollow(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You wouldn't want to leave your dear master.\n\r",ch);
	return;
    } 

    if (IS_SET(ch->act,PLR_NOFOLLOW))
    {
      send_to_char("You now accept followers.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_NOFOLLOW);
    }
    else
    {
      send_to_char("You no longer accept followers.\n\r",ch);
      SET_BIT(ch->act,PLR_NOFOLLOW);
      die_follower( ch );
    }
}

void do_nosummon(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
      if (IS_SET(ch->imm_flags,IMM_SUMMON))
      {
	send_to_char("You are no longer immune to summoning.\n\r",ch);
	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);
      }
      else
      {
	send_to_char("You are now immune to summoning.\n\r",ch);
	SET_BIT(ch->imm_flags,IMM_SUMMON);
      }
    }
    else
    {
      if (IS_SET(ch->act,PLR_NOSUMMON))
      {
        send_to_char("You may now be summoned by anyone.\n\r",ch);
        REMOVE_BIT(ch->act,PLR_NOSUMMON);
      }
      else
      {
        send_to_char("You may only be summoned by players within your PK range.\n\r",ch);
        SET_BIT(ch->act,PLR_NOSUMMON);
      }
    }
}

void  do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *pdesc;
    int door;
    int number,count;

    if ( ch->desc == NULL )
	return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "{CYou see stars!{x\n\r", ch );
	return;
    }
    
    if ( IS_AFFECTED2(ch, AFF2_SUBMERGED))
     if ((ch->in_room->sector_type != SECT_WATER_SWIM)
     && (ch->in_room->sector_type != SECT_WATER_NOSWIM ))
    {
     	send_to_char("You are pulled out of the water.\n\r", ch);
     	affect_strip(ch,gsn_dive);
    }
       	
    if ( IS_SET(ch->affected_by, AFF_FADE))
    REMOVE_BIT(ch->affected_by, AFF_FADE);
    
    if ( ch->position == POS_SLEEPING )
    {
send_to_char( "{CYou look around, things sure are pleasant, perhaps you should wake up from your fantasy.{x\n\r", ch);
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   room_is_dark( ch->in_room ,ch) )
    {
        send_to_char( "{bIt is pitch fucking black... {x\n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    number = number_argument(arg1,arg3);
    count = 0;

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */

if (  !IS_SET(ch->in_room->room_flags, ROOM_WILDERNESS)   ) 
{ /* for NON wilderness */
	    send_to_char( "{y\n\r",ch);
	send_to_char( ch->in_room->name, ch );
	    send_to_char( "{w",ch);

	if ( (IS_IMMORTAL(ch) && (IS_NPC(ch) || IS_SET(ch->act,PLR_HOLYLIGHT)))
       ||   IS_BUILDER(ch, ch->in_room->area) )
	{
	    xprintf(buf," {M[Room %d]{w",ch->in_room->vnum);
	    send_to_char(buf,ch);
	}
	    send_to_char( "\n\r",ch);

	if ( arg1[0] == '\0'
	|| ( !IS_NPC(ch) && !IS_SET(ch->comm, COMM_BRIEF) ) )
	{
	    send_to_char( "  ",ch);
	    send_to_char( ch->in_room->description, ch );
	    send_to_char( "",ch);

	}

        if (  !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT)       )
	{
	    send_to_char("\n\r",ch);
	    send_to_char( "",ch);
	    do_exits(ch,"auto");
	    send_to_char( "",ch);
	}

} /* end if NON wilderness */
else if (  IS_SET(ch->in_room->room_flags, ROOM_WILDERNESS)   ) 
{ /* if IS wilderness */
	do_map(ch,"");
}/* end if IS wilderness */


	    send_to_char( "{g",ch);
	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	    send_to_char( "{B",ch);
	show_char_to_char( ch->in_room->people,   ch );
	    send_to_char( "{x",ch);
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "{CLook in what?{x\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, NULL, arg2 ) ) == NULL )
	{
	    send_to_char( "{CYou do not see that here.{x\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "{CThat is not a container, what were you thinking?{x\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "{CGuess what, it's empty.{x\n\r", ch );
		break;
	    }

	    xprintf( buf, "{cIt's %sfilled with  a %s liquid.{x\n\r",
		obj->value[1] <     obj->value[0] / 4
		    ? "less than half-" :
		obj->value[1] < 3 * obj->value[0] / 4
		    ? "about half-"     : "more than half-",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "{cIt is closed.{x\n\r", ch );
		break;
	    }

	    act( "{c$p holds:{x", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
       	    send_to_char( "{x", ch );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );

        /* Love potion, makes you charmed by the next person you look at */

        if (is_affected(ch, gsn_love_potion) && (victim != ch) &&
!IS_IMMORTAL(victim))
          {
            AFFECT_DATA af;

            affect_strip(ch, gsn_love_potion);

            if (ch->master)
              stop_follower(ch);
            add_follower(ch, victim);
            ch->leader = victim;

	    af.where = TO_AFFECTS;
            af.type = gsn_charm_person;
            af.level = ch->level;
            af.duration =  number_fuzzy(victim->level / 4);
            af.bitvector = AFF_CHARM;
            af.modifier = 0;
            af.location = 0;
            affect_to_char(ch, &af);

            act("Isn't $n just so nice?", victim, NULL, ch, TO_VICT);
            act("$N looks at you with adoring eyes.",victim,NULL,ch,TO_CHAR);
            act("$N looks at $n with adoring eyes.",victim,NULL,ch,TO_NOTVICT);
          }
         if ( IS_AFFECTED2(victim, AFF2_CHARISMATIC))
         {
         AFFECT_DATA af;
	    if ( IS_IMMORTAL(ch) )
	    return;
            act("$n is a true leader, hopefully you're a worthy follower", victim, NULL, ch, TO_VICT);
            act("$N is fooled by your magic and is now a loyal follower.",victim,NULL,ch,TO_CHAR);
            act("$N looks at $n with newfound admiration.",victim,NULL,ch,TO_NOTVICT);
            
            if (ch->master)
              stop_follower(ch);
            add_follower(ch, victim);
            ch->leader = victim;

	    af.where = TO_AFFECTS;
            af.type = gsn_charm_person;
            af.level = ch->level;
            af.duration = victim->level / 4;
            af.bitvector = AFF_CHARM;
            af.modifier = 0;
            af.location = 0;
            affect_to_char(ch, &af);
            
        }
	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{  /* player can see object */
	   pdesc = get_extra_descr( arg3, obj->extra_descr );
	   if ( pdesc != NULL )
	    {
	     if (++count == number)
	      {
	       send_to_char( "{c",ch);
	       send_to_char( pdesc, ch );
	       send_to_char( "{x",ch);
	       return;
	      }
	     else
	      {
	       continue;
	      } 
            }
 	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
 	    if ( pdesc != NULL )
 	     {
 	      if (++count == number)
 	       {	
	        send_to_char( "{c",ch);
		send_to_char( pdesc, ch );
	        send_to_char( "{x",ch);
		return;
	       }
	      else
	       {
	       	continue;
               }
             }
           if ( !IS_IMMORTAL(ch) )  {
	    if ( is_name( arg3, obj->name ) )
	    	if (++count == number)
	    	{
	    	    send_to_char( obj->description, ch );  
 	    	    send_to_char( "\n\r",ch);

//    send_to_char( "{CYou see nothing special about it.{x\n\r", ch);
		    return;
		  }
           }
           else  {
	    if ( is_name_imm( arg3, obj->name ) )
	    	if (++count == number)
	    	{
	    	    send_to_char( obj->description, ch );  
       	             send_to_char( "{x\n\r",ch);

//	    send_to_char("{CYou see nothing special about it.{x\n\r",ch);
		    return;
		  }
           }
	  }
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
	    send_to_char( "{c",ch);
		    send_to_char( pdesc, ch );
	    send_to_char( "{x",ch);
		    return;
	    	}

	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
	    send_to_char( "{c",ch);
		    send_to_char( pdesc, ch );
	    send_to_char( "{x",ch);
		    return;
	    	}
	}
	if ( !IS_IMMORTAL(ch) )  {
	if ( is_name( arg3, obj->name ) )
	    if (++count == number)
	    {
	    send_to_char( "{c",ch);
	    	send_to_char( obj->description, ch );
	    send_to_char( "{x",ch);
	    	send_to_char("\n\r",ch);
	    	return;
	    }
        }
        else  {
	if ( is_name_imm( arg3, obj->name ) )
	    if (++count == number)
	    {
	    send_to_char( "{c",ch);
	    	send_to_char( obj->description, ch );
	    	send_to_char("\n\r",ch);
	    send_to_char( "{x",ch);
	    	return;
	    }
        }
    }

    pdesc = get_extra_descr(arg3,ch->in_room->extra_descr);
    if (pdesc != NULL)
    {
	if (++count == number)
	{
	    send_to_char( "{c",ch);
	    send_to_char(pdesc,ch);
	    send_to_char( "{x",ch);
	    return;
	}
    }
    
    if (count > 0 && count != number)
    {
    	if (count == 1)
    	    xprintf(buf,"You only see one %s here.\n\r",arg3);
    	else
    	    xprintf(buf,"You only see %d of those here.\n\r",count);
    send_to_char( "{c",ch);
    	send_to_char(buf,ch);
    send_to_char( "{x",ch);
    	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "{cYou do not see that here.{x\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "{cNothing special there.{x\n\r", ch );
	return;
    }

    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	{
	    send_to_char( "{c",ch);
	send_to_char( pexit->description, ch );
	    send_to_char( "{x",ch);
	}
    else
	send_to_char( "{cNothing special there.{x\n\r", ch );

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "{cThe $d is closed.{x", ch, NULL, pexit->keyword,TO_CHAR);
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "{cThe $d is open.{x",   ch, NULL, pexit->keyword,TO_CHAR );
	}
    }

    return;
}

/* RT added back for the hell of it */
void do_read (CHAR_DATA *ch, char *argument )
{
	do_look(ch,argument);
}

void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	    send_to_char( "{c",ch);
	send_to_char( "Examine what?\n\r", ch );
	    send_to_char( "{x",ch);
	return;
    }

	do_look(ch,arg);

    if ( ( obj = get_obj_here( ch, NULL,  arg ) ) != NULL )
    {
	switch ( obj->item_type )
	{
	default:
	    break;
/*	
	case ITEM_JUKEBOX:
		do_play(ch,"list");
	    break;
*/
	case ITEM_MONEY:
	    if (obj->value[0] == 0)
	    {
	        if (obj->value[1] == 0)
		    xprintf(buf,"Odd...there's no coins in the pile.\n\r");
		else if (obj->value[1] == 1)
		    xprintf(buf,"Wow. One fucking gold coin.\n\r");
		else
		    xprintf(buf,"There are %d gold coins in the pile.\n\r",
			obj->value[1]);
	    }
	    else if (obj->value[1] == 0)
	    {
		if (obj->value[0] == 1)
		    xprintf(buf,"Wow. One fucking silver coin.\n\r");
		else
		    xprintf(buf,"There are %d silver coins in the pile.\n\r",
			obj->value[0]);
	    }
	    else
		xprintf(buf,
		    "There are %d gold and %d silver coins in the pile.\n\r",
		    obj->value[1],obj->value[0]);
	    send_to_char( "{c",ch);
    		send_to_char(buf,ch);
	    send_to_char( "{x",ch);
	    break;

	case ITEM_DRINK_CON:
	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    xprintf(buf,"in %s",argument);
		do_look(ch,buf);
	}
    }

    return;
}


/*
 * Thanks to Zrin for auto-exit part.
 */
/*
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    char doormark[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto = !strcmp(argument, "auto");
    int door;

    	if ( !check_blind( ch ) )
		return;

	send_to_char( "{W", ch );

  	strcpy( buf, fAuto ? "{W[{YExits{W:{x" : "{CObvious exits:{w\n\r" );

	found = FALSE;
    
	for ( door = 0; door <= 5; door++ )
	  {
		if ( ( pexit = ch->in_room->exit[door] ) != NULL
		 &&   pexit->u1.to_room != NULL
		 &&   can_see_room(ch,pexit->u1.to_room) 
		 &&   !IS_SET(pexit->exit_info, EX_HIDDEN) )
		  {
	    
			found = TRUE;
		
				if (!IS_SET(pexit->exit_info, EX_CLOSED))
					if (!IS_SET(pexit->exit_info, EX_HIDDEN))
						strcat( doormark, "{R#" );
					else
						strcat( doormark, "{w#" );
				else
					strcat( doormark, "" );
			
			if ( fAuto )
			  {
				
				if (!IS_SET(pexit->exit_info, EX_HIDDEN) && !IS_IMMORTAL(ch))
					if (!IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->exit_info, EX_HIDDEN) )
						xprintf(buf," %s%s",doormark,dir_name[door]);
					else
						strcat( buf, dir_name[door] );
			  }
			else
			  {
	
				if (!IS_SET(pexit->exit_info, EX_HIDDEN) && !IS_IMMORTAL(ch))
					if (!IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->exit_info, EX_HIDDEN) )
						xprintf( buf + strlen(buf), "%-5s - %s",capitalize( dir_name[door] ),room_is_dark( pexit->u1.to_room ,NULL) ?  "Too dark to tell" : pexit->u1.to_room->name);
					else
						xprintf( buf + strlen(buf), "%s%-5s - %s",doormark,capitalize( dir_name[door] ),room_is_dark( pexit->u1.to_room ,NULL) ?  "Too dark to tell" : pexit->u1.to_room->name);
			  }
		


		  }
	  }

	if (IS_IMMORTAL(ch))
		xprintf(buf + strlen(buf)," (room %d)\n\r",pexit->u1.to_room->vnum);
	else
		xprintf(buf + strlen(buf), "\n\r");

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "{W]{x\n\r" );

    send_to_char( buf, ch );
    return;
}
*/
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto = !strcmp(argument, "auto");
    int door;

    if ( !check_blind( ch ) )
	return;

   if( (!str_cmp(argument, "vt")) && (ch->vt == 1) )
    {
send_to_char( "\033[H\033[K\n\033[K\n\033[K", ch );
        if( IS_SET(ch->act, PLR_COLOR) )
        {
            send_to_char( "{B\033[1;75HN  U\033[2;74HW+E |", ch );
            send_to_char( "\033[3;75HS  D{W", ch );
        }
        for ( door = 0; door <= 5; door++ )
        {
            if ( ( pexit = ch->in_room->exit[door] ) != NULL
            &&   pexit->u1.to_room != NULL
            &&   can_see_room(ch,pexit->u1.to_room)
            &&   !IS_SET(pexit->exit_info, EX_CLOSED) )
            {
                switch( door )
                {
                    case 0:
                        send_to_char( "\033[1;75HN", ch );
                        break;
                    case 1:
                        send_to_char( "\033[2;76HE", ch );
                        break;
                    case 2:
                        send_to_char( "\033[3;75HS", ch );
                        break;
                    case 3:
                        send_to_char( "\033[2;74HW", ch );
                        break;
                    case 4:
                        send_to_char( "\033[1;78HU", ch );
                        break;
                    case 5:
                        send_to_char( "\033[3;78HD", ch );
                        break;
                    default:
                        break;
                }
            }
        }
        send_to_char( "\033[2;5H{R", ch );
        if( !IS_NPC(ch)
        &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
        &&   room_is_dark( ch->in_room,ch ) )
            send_to_char( "The room is pitch black... ", ch );
        else
            send_to_char( ch->in_room->name, ch );

        if( IS_IMMORTAL(ch) )
        {
            send_to_char( "\033[2;45H", ch );
            xprintf( buf, "{w[{c%d{w]", ch->in_room->vnum );
            send_to_char( buf, ch );
        }
        send_to_char( "\033[23;1H{w", ch );
    }

    if(!str_cmp(argument, "vt"))
        return;

    send_to_char( "{W", ch );

  strcpy( buf, fAuto ? "{W[{YExits{W:{x" : "{CObvious exits:{w\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room) 
	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
	    }
	    else
	    {
		xprintf( buf , "%s %-5s - %s",buf,
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room ,NULL)
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    xprintf(buf,	"%s (room %d)\n\r",buf,pexit->u1.to_room->vnum);
		else
		    xprintf(buf + strlen(buf), "\n\r");
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "{W]{x\n\r" );

    send_to_char( buf, ch );
    return;
}

void do_worth( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
    {
	xprintf(buf,"{cYou have %ld gold and %ld silver.{x\n\r",
	    ch->gold,ch->silver);
	send_to_char(buf,ch);
	return;
    }

    xprintf(buf, 
    "{cYou have %ld gold, %ld silver, and %d experience (%d exp to level).{x\n\r",
	ch->gold, ch->silver,ch->exp,
/*	(ch->level + 1) * exp_per_level(ch,ch->pcdata->points) - ch->exp); */
        exp_to_level( ch, ch->pcdata->points ) );
 
    send_to_char(buf,ch);

    return;
}



char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

/*  adds bounty code */
void do_bounty( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
char log_buf[MAX_STRING_LENGTH];     
 int amount;
    DESCRIPTOR_DATA *d;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    amount = atoi(arg2);

if (IS_NPC(ch)) return;

    if ( arg1[0] == '\0' )
    {
	do_help(ch,"bounty");
    return;
    }
else if (!IS_SET(ch->in_room->room_flags, ROOM_BOUNTY))
    {
send_to_char("You have to be in a bounty office to post and claim bounties.\n\r",
      ch);
      return;
    }
else if ( !str_cmp (arg1,"list") )
{ 
xprintf(buf,"NULL");
send_to_char("{g\n\rA dimunitive voice rattles off the bounties.\n\r{x",ch);
send_to_char("\n\r{W[BOUNTY LIST]     (bounty)(collect){x\n\r",ch); 

for ( d = descriptor_list; d != NULL; d = d->next )
{

  if (d->connected == CON_PLAYING && can_see(ch, d->character) )
  if (d->character->pcdata->bounty > 1000 || d->character->pcdata->owed > 0 )
	{
xprintf(buf,"{r%-15s{R%10d{B%10d{x\n\r",d->character->name,d->character->pcdata->bounty,d->character->pcdata->owed);
	send_to_char(buf,ch);
	} /* if2 */
} /* for */	    

   if ( !str_cmp (buf,"NULL") )
   send_to_char("{DNone.{x\n\r\n\r",ch);
   else
   send_to_char("\n\r",ch);

return;	
} /* list */

else if ( !str_cmp (arg1,"collect") )
{
	if (ch->pcdata->owed > 0)
	{
	ch->gold += ch->pcdata->owed;
	ch->pcdata->owed = 0;
send_to_char("{gYou whisper into the small hole.{x\n\r",ch);
send_to_char("{gGold falls from the hole and into your lap.{x\n\r{x",ch);
send_to_char("{gYou collect your hard earned gold bounty.{x\n\r",ch);

  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    xprintf(buf, " the Mercurial %s", capitalize(class_table[ch->class].name ));
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

	return;
	}
	else
	{
	
	send_to_char("{gYou hear some movement behind the wall and then the angered voice...{x\n\r",ch);	
	send_to_char("{GYou are but an thin-blooded daint, you are not eligible to collect.{x\n\r",ch);	
	send_to_char("{GHave a look at the bounty list if you would like to be.{x\n\r",ch);	
	return;
	}
	
return;	
}
else /* for placing bounties */
{
    if (amount < 500 )
    {
    send_to_char("{gA voice whispers '{GYou have to place at least 500 gold{g'.{x\n\r",ch);
    return;
     }

    if ( arg2[0] == '\0' || amount == 0)
    {
    send_to_char("{gA voice wispers '{GHow much?{g'{x\n\r",ch);
    return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
    send_to_char( "{gA voice whispers '{GTheir whereabouts are not known{g'.{x\n\r", ch );
    return;
    }

    if ( !IS_IMMORTAL(ch) )
    if ( victim == ch)
    {
    send_to_char("{cYou can not place a bounty on yourself, no matter how proud you are.{x\n\r",ch);
    return;
    }

    if ( IS_NPC(victim) || IS_IMMORTAL(victim))
    {
    send_to_char( "{cThat would be a waste of gold.{x\n\r", ch );
        return;
    }

    if (ch->gold < amount )
    {
              send_to_char( "{gA voice whispers '{GYou do not have that much!{g'.{x\n\r", ch );
              return;                                          
    }     

    send_to_char( "{gYou push some gold through the slit.{x\n\r", ch);
    send_to_char( "{gYou feel much better after placing the bounty.{x\n\r", ch);
    ch->gold -= amount;
    send_to_char("{cYou feel more popular in some strange way.{x\n\r",victim);
    victim->pcdata->bounty += amount;
    send_to_char("Remember to write a note on board 3 to admin,why you did add this bounty ",ch);
xprintf(log_buf, "%s Added a bounty %d gold and %s bounty is now %d.",
    ch->name,amount,victim->name,victim->pcdata->bounty);            
 log_string(log_buf);
  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    xprintf(buf, " the Contractor %s", capitalize(class_table[ch->class].name ));
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

  if ((!IS_NPC(victim)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    xprintf(buf, " the Unusually Popular %s", capitalize(class_table[victim->class].name ));
    free_string( victim->pcdata->title );victim->pcdata->title = str_dup( buf );}

    return;

} /* last else */

} /* end function */
/*  end bounty code */

void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int percent;
    const char *graph;
    char hpbar[MAX_STRING_LENGTH];
    char manabar[MAX_STRING_LENGTH];
    char movebar[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;

/* Do hp bar */
  if (ch->max_hit > 0)
          percent = ch->hit * 100 / ch->max_hit;
      else
          percent = -1;

      if (percent >= 100)
            graph = "{R##########{x";
      else if (percent >= 90)
            graph = "{R#########{r-{x";
      else if (percent >= 80)
            graph = "{R########{r--{x";
      else if (percent >= 70)
            graph = "{R#######{r---{x";
    else if (percent >= 58)
            graph = "{R######{r----{x";
      else if (percent >= 45)
            graph = "{R#####{r-----{x";
      else if (percent >= 30)
            graph = "{R####{r------{x";
      else if (percent >= 28)
            graph = "{R###{r-------{x";
      else if (percent >= 15)
            graph = "{R##{r--------{x";
        else if (percent >= 8)
       graph = "{R#{r---------{x";
       else
          graph = "{r----------{x";
        xprintf(hpbar,"%s",graph);

/* Do mana bar */
  if (ch->max_mana > 0)
          percent = ch->mana * 100 / ch->max_mana;
      else
          percent = -1;

      if (percent >= 100)
            graph = "{B##########{x";
      else if (percent >= 90)
            graph = "{B#########{b-{x";
      else if (percent >= 80)
            graph = "{B########{b--{x";
      else if (percent >= 70)
            graph = "{B#######{b---{x";
    else if (percent >= 58)
            graph = "{B######{b----{x";
      else if (percent >= 45)
            graph = "{B#####{b-----{x";
      else if (percent >= 30)
            graph = "{B####{b------{x";
      else if (percent >= 28)
            graph = "{B###{b-------{x";
      else if (percent >= 15)
            graph = "{B##{b--------{x";
        else if (percent >= 8)
       graph = "{B#{b---------{x";
       else
          graph = "{b----------{x";
        xprintf(manabar,"%s",graph);
/* Do move bar */
  if (ch->max_mana > 0)
          percent = ch->move * 100 / ch->max_move;
      else
          percent = -1;

      if (percent >= 100)
            graph = "{G##########{x";
      else if (percent >= 90)
            graph = "{G#########{g-{x";
      else if (percent >= 80)
            graph = "{G########{g--{x";
      else if (percent >= 70)
            graph = "{G#######{g---{x";
  else if (percent >= 58)
            graph = "{G######{g----{x";
      else if (percent >= 45)
            graph = "{G#####{g-----{x";
      else if (percent >= 30)
            graph = "{G####{g------{x";
      else if (percent >= 28)
            graph = "{G###{g-------{x";
      else if (percent >= 15)
            graph = "{G##{g--------{x";
        else if (percent >= 8)
           graph = "{G#{g---------{x";
       else
          graph = "{g----------{x";
        xprintf(movebar,"%s",graph);

/*
    xprintf
    ( buf,
	"{b-={B* {M%s%s%s{x {B*{b=-{x\n\r",
     IS_NPC(ch) ? "" : ch->pcdata->pretitle,
	ch->name,
     IS_NPC(ch) ? "" : ch->pcdata->title
     );

    send_to_char( buf, ch );
*/
     if ( get_trust( ch ) != ch->level )
    {
	xprintf( buf, "{mYou are trusted at level %d.{x\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }
xprintf(buf,
"{YLV. %3d {RExp TNL %10d {y+---------------------------------------+{x\n\r"
"                           {y| %10s%-27s |{x\n\r"
"HP %s %5d/%5d  {y| {WClass: %-30s {y|{x\n\r"
"MP %s %5d/%5d  {y| Kingdom: %-28s |{x\n\r"
"MV %s %5d/%5d  {y| Rank: %-31s |{x\n\r"
"{y+--------------------------{y+=======================================+{x\n\r",
ch->level,
IS_NPC(ch) ? 0 :exp_to_level(ch,ch->pcdata->points), 
ch->name,
IS_NPC(ch) ? "" : ch->pcdata->title,
hpbar,
ch->hit,
ch->max_hit,
class_table[ch->class].name,
manabar,
ch->mana,
ch->max_mana,
ch->kingdom == 0 ? "None" : kingdom_table[ch->kingdom].long_name,
movebar,
ch->move,
ch->max_move,
ch->kingdom == 0 ? "None" : kingdom_table[ch->kingdom].rank_titles[ch->kingdom_rank]);
send_to_char(buf,ch);

if (ch->level > 10)
    xprintf
    ( buf,
"{y| {mStr:{b%6d {M({B%2d{M){x     {mTrain:    {b%3d   Wimpy: {b%5d{x                 {y|{w\n\r"
"{y| {mInt:{b%6d {M({B%2d{M){x     {mPracs:    {b%3d   {mGlory Points: {B%5d{b/{B%8d{x {y|{w\n\r"
"{y| {mWis:{b%6d {M({B%2d{M){x     {mDamroll:  {b%3d   {cLegend Level: %3d            {y|{w\n\r"
"{y| {mDex:{b%6d {M({B%2d{M){x     {mHitroll:  {b%3d   {mRP tokens:    %3d            {y|{w\n\r"
"{y| {mCon:{b%6d {M({B%2d{M){x     {mSaves:    {b%3d",
ch->perm_stat[STAT_STR],get_curr_stat(ch,STAT_STR),ch->train,ch->wimpy,
ch->perm_stat[STAT_INT],get_curr_stat(ch,STAT_INT),ch->practice,ch->quest_curr,ch->quest_accum,
ch->perm_stat[STAT_WIS],get_curr_stat(ch,STAT_WIS),GET_DAMROLL(ch),ch->legendlevel,
ch->perm_stat[STAT_DEX],get_curr_stat(ch,STAT_DEX),GET_HITROLL(ch), ch->rptoken,
ch->perm_stat[STAT_CON],get_curr_stat(ch,STAT_CON),ch->saving_throw
     );
else /* for people below lev 10 */
    xprintf
    ( buf,
"{y| {mStr:{B%s {x         {mTrain: {b%3d{x          {mWimpy: {b%5d{x       {y|\n\r"
"{y| {mInt:{B%s {x         {mPracs: {b%3d{x                             {y|\n\r"
"{y| {mWis:{B%s {x         {mDamroll: {D??{x                            {y|\n\r"
"{y| {mDex:{B%s {x         {mHitroll: {D??{x                            {y|\n\r"
"{y| {mCon:{B%s {x         {mSaves:   {D??{x",
get_stat_alias(ch,STAT_STR),ch->train,ch->wimpy,
get_stat_alias(ch,STAT_INT),ch->practice,
get_stat_alias(ch,STAT_WIS),
get_stat_alias(ch,STAT_DEX),
get_stat_alias(ch,STAT_CON)
     );
    send_to_char(buf,ch);

/*    if (!IS_NPC(ch) && ch->level < LEVEL_HERO)
    {
      xprintf (buf,
	"      {mExp Tnl: {b%-5d{x",
        exp_to_level(ch,ch->pcdata->points));
      send_to_char( buf, ch );
     }
*/
    
   if (ch->ethos == 1) send_to_char("     {m-={M( {CLawful  ", ch);
    else if (ch->ethos == 2) send_to_char("     {m-={M( {CNeutral ", ch);
    else if (ch->ethos == 3) send_to_char("     {m-={M( {CChaotic ", ch);
    else
    send_to_char("     {m-={W( {CNONE!   ", ch);

    if (IS_GOOD(ch)) send_to_char("Good {M){m=-{x", ch);
    else if (IS_EVIL(ch)) send_to_char ("Evil {M){m=-{x", ch);
    else send_to_char ("Neut {M){m=-{x", ch);

    if (ch->level >= 84)
    send_to_char("       {y|\n\r",ch);
    else
    if ((ch->level <= 83) && (ch->level > 10))
    send_to_char("       {y|\n\r",ch);
    else
    send_to_char("   {y|\n\r",ch);


send_to_char("{b+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+{x\n\r",ch);

send_to_char("{mStatus: ",ch);
    switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( " {rDEAD{m,",		ch );
	break;
    case POS_MORTAL:
	send_to_char( " {RMortally Wounded{m,",	ch );
	break;
    case POS_INCAP:
	send_to_char( " {YIncapacitated{m,",	ch );
	break;
    case POS_STUNNED:
	send_to_char( " {YStunned{m,",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( " {BSleeping{m,",		ch );
	break;
    case POS_RESTING:
	send_to_char( " {bResting{m,",		ch );
	break;
    case POS_STANDING:
	send_to_char( " {bStanding{m,",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( " {gFighting{m,",		ch );
	break;
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( " {gDrunk{m,",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
	send_to_char( " {BThirsty{m,", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   <=  0 )
	send_to_char( " {yHungry{m,",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_BLOODLUST]   <=  0 )
        send_to_char( "You are hungry for blood.\n\r",  ch );
        
    if ( IS_IMMORTAL(ch))
    {
      send_to_char("     {WHoly Light: ",ch);
      if (IS_SET(ch->act,PLR_HOLYLIGHT))
        send_to_char("{Bon{x  ",ch);
      else
        send_to_char("{Roff{x ",ch);
    }

    if (ch->guarding != NULL)
      {
	xprintf(buf1, " {cGuarding %s{m,", ch->guarding->name);
	send_to_char( buf1, ch);
      }
    if (ch->guarded_by != NULL)
      {
	xprintf(buf2, " {cGuarded by %s{m,", ch->guarded_by->name);
	send_to_char(buf2, ch);
      }
    send_to_char("{x\n\r", ch);

    xprintf( buf,
	"{mCarrying:   {b%d{M/{B%d {mitems weighing {b%ld{M/{B%d {mstones.{x\n\r",
	ch->carry_number, can_carry_n(ch),
	get_carry_weight(ch), can_carry_w(ch) );
    send_to_char( buf, ch );

/*  begins replacement */
xprintf( buf, "            {mYou also carry {b%ld{m gold \
and {b%ld{m silver.{x",
       ch->gold,
       ch->silver);
       send_to_char( buf, ch );
/*  end */

send_to_char("\n\r{B+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+{x\n\r",ch);

    /* print AC values */
if (ch->level > 10)
{
xprintf( buf,
"{m[ {BArmor: {mpierce: {b%-4d  {mbash: {b%-4d  {mslash: {b%-4d  {mmagic: {b%-4d {m]{x\n\r",
		 GET_AC(ch,AC_PIERCE),
		 GET_AC(ch,AC_BASH),
		 GET_AC(ch,AC_SLASH),
		 GET_AC(ch,AC_EXOTIC));
    	send_to_char(buf,ch);
}
else
{ /* level 10 people see this */
int i;

    for (i = 0; i < 4; i++)
    {
	char * temp;

	switch(i)
	{
	    case(AC_PIERCE):	temp = "{bpiercing";	break;
	    case(AC_BASH):	temp = "{bbashing";	break;
	    case(AC_SLASH):	temp = "{bslashing";	break;
	    case(AC_EXOTIC):	temp = "{bmagic";		break;
	    default:		temp = "error";		break;
	}
	
	send_to_char("      {MYou are ", ch);

	if      (GET_AC(ch,i) >=  101 ) 
	    xprintf(buf,"hopelessly vulnerable to %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= 80) 
	    xprintf(buf,"defenseless against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= 60)
	    xprintf(buf,"barely protected from %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= 40)
	    xprintf(buf,"slightly armored against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= 20)
	    xprintf(buf,"somewhat armored against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= 0)
	    xprintf(buf,"armored against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= -20)
	    xprintf(buf,"well-armored against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= -40)
	    xprintf(buf,"very well-armored against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= -60)
	    xprintf(buf,"heavily armored against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= -80)
	    xprintf(buf,"superbly armored against %s{M.{x\n\r",temp);
	else if (GET_AC(ch,i) >= -100)
	    xprintf(buf,"almost invulnerable to %s{M.{x\n\r",temp);
	else
	    xprintf(buf,"divinely armored against %s{M.{x\n\r",temp);

	send_to_char(buf,ch);
    }
}

	if ((ch->invis_level) && !IS_NPC(ch))
      {
        xprintf( buf, "      {MInvisible: {mlevel {b%d{x\n\r",ch->invis_level);
        send_to_char(buf,ch);
      }

	if ((ch->incog_level)  && !IS_NPC(ch))
	{
	  xprintf(buf,"      {MIncognito: {mlevel {b%d{x\n\r", ch->incog_level);
	  send_to_char(buf,ch);
	}
send_to_char("{B+-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+{x\n\r",ch);
    if ((ch->nochanned > 0) && !IS_NPC(ch))
     {
      xprintf(buf,"The gods have revoked your channel priviliges, for %d tick.\n\r",ch->nochanned);
      send_to_char( buf, ch );
     }
    if ((ch->wantedtime > 0) && !IS_NPC(ch) && IS_SET(ch->act,PLR_WANTED))
     {
      xprintf(buf,"You will be considered a criminal for another %d ticks.\n\r",ch->wantedtime);
      send_to_char( buf, ch );
     }
    if ( ch->affected != NULL && IS_SET(ch->comm, COMM_SHOW_AFFECTS) )
    {
	send_to_char( "{mYou are affected by:{x\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    xprintf( buf, "{mSpell: '{Y%s{m'", skill_table[paf->type].name);
	    send_to_char( buf, ch );

	    if ( ch->level >= 20 && IS_IMMORTAL(ch) )

	    {
		if ( paf->duration != -1 )
		  xprintf( buf,
		      " modifies {b%s{m by {b%d{m for {B%d{m hours",
		      affect_loc_name( paf->location ),
		      paf->modifier,
		      paf->duration );
		else 
		  xprintf( buf,
		      " modifies {b%s{m by {b%d{m {Wpermenantly",
		      affect_loc_name( paf->location ),
		      paf->modifier );
		send_to_char( buf, ch );
	    }

	    send_to_char( "{m.{x\n\r", ch );
	}
    }

    return;
}


void do_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];
    
    if ( ch->affected != NULL )
    {
	send_to_char( "{MYou are affected by the following spells:{x\n\r",ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
/*		if (ch->level >= 20 && 
	( ch->class==3 || ch->class==13 || ch->class==6 || ch->class==7 ||
ch->class==8 || ch->class==9 || ch->class==14 ||
		ch->race == 6 || ch->race==7 || ch->class == CLASS_DRD */
                 if ( CAN_DETECT(ch, DETECT_MAGIC)  || (IS_IMMORTAL(ch)) )			 
		    xprintf( buf, "                      ");
		else
		    continue;
	    else
	    	xprintf( buf, "{mSpell: %-15s",skill_table[paf->type].name );

	    send_to_char( buf, ch );
/*
	    if ( ch->level >= 20 && 
	( ch->class==3 || ch->class==13 || ch->class==6 || ch->class==7 ||
ch->class==8 || ch->class==9 || ch->class==14 ||
		ch->race == 6 || ch->race==7 || ch->class==CLASS_DRD ||
			 IS_IMMORTAL(ch)) )*/
        if ( CAN_DETECT(ch, DETECT_MAGIC)  || (IS_IMMORTAL(ch)) )			 
	    {
		xprintf( buf,
		    "{m: modifies {Y%s{m by {Y%d{m ",
		    affect_loc_name( paf->location ),
		    paf->modifier);
		send_to_char( buf, ch );
		if ( paf->duration == -1 )
		    xprintf( buf, "permanently" );
		else
		    xprintf( buf, "for {b%d{m hours", paf->duration );
		send_to_char( buf, ch );
	    }
	    send_to_char( "{x\n\r", ch );
	    paf_last = paf;
	}
    }
    else 
	send_to_char("You are not affected by any spells.\n\r",ch);

    return;
}

/* room affects */
void do_raffects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];

    if (ch->in_room->affected != NULL )
    {
        send_to_char( "The room is affected by the following spells:\n\r",ch );
        for ( paf = ch->in_room->affected; paf != NULL; paf = paf->next )
        {
            if (paf_last != NULL && paf->type == paf_last->type)
                if (ch->level >= 20 )
                    xprintf( buf, "                      ");
                else
                    continue;
            else
                xprintf( buf, "Spell: %-15s", skill_table[paf->type].name);
    send_to_char( buf, ch );

            if ( ch->level >= 20 )
            {
                xprintf( buf,
                    ": modifies %s by %d ",
                    raffect_loc_name( paf->location ),
                    paf->modifier);
                send_to_char( buf, ch );
                if ( paf->duration == -1 || paf->duration == -2 )
                    xprintf( buf, "permanently" );
                else
                    xprintf( buf, "for %d hours", paf->duration );
                send_to_char( buf, ch );
            }
            send_to_char( "\n\r", ch );
   paf_last = paf;
        }
    }
    else
        send_to_char("The room is not affected by any spells.\n\r",ch);

    return;
}


void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    xprintf( buf,
	"{mIt is {b%d{m o'clock {b%s{m, Day of {b%s{m, {b%d%s{m the Month of {b%s{m.{x\n\r",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour %12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month] );
    send_to_char( buf, ch );

    xprintf( buf, "{mFate started up at %sThe system time is %s{x",
	str_boot_time,
	(char *) ctime( &current_time ) );
    send_to_char( buf, ch );
  xprintf(buf, "{GFate will copyover in %d minutes.{w\n",
        copyover_counter);
        send_to_char(buf,ch);
    return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    static char * const sky_look[4] =
    {
	"cloudless",
	"cloudy",
	"rainy",
	"lit by flashes of lightning"
    };

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the weather indoors.\n\r", ch );
	return;
    }

    xprintf( buf, "{cThe sky is {C%s{c and {C%s{c.{x\n\r",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "{ca warm southerly breeze blows{x"
	: "{ca cold northern gust blows{x"
	);
    send_to_char( buf, ch );
    return;
}


/*
void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];

    if ( argument[0] == '\0' )
	argument = "summary";

    // this parts handles help a b so that it returns help 'a b' 
    argall[0] = '\0';
    while (argument[0] != '\0' )
    {
	argument = one_argument(argument,argone);
	if (argall[0] != '\0')
	    strcat(argall," ");
	strcat(argall,argone);
    }


    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	if ( pHelp->level > get_trust( ch ) )
	    continue;

	if ( is_full_name( argall, pHelp->keyword ) )
	{
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {

		do_font(ch,argall);
	    }


	    if ( pHelp->text[0] == '.' )
	    {
		page_to_char( pHelp->text+1, ch );
	    }	 
	    else // skips keywords automatically 
	    {
		page_to_char( pHelp->text, ch );
	    }
	if(ch->msp == 1)
	send_to_char("!!MUSIC(music/fokhelp.mid)",ch);
	    return;
	}
    }

    send_to_char( "No help on that word.\n\r", ch );
    return;
}
*/
/*  adds background code */

void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
//    char arg[MAX_INPUT_LENGTH];
//    char argnumber[MAX_INPUT_LENGTH];
    BUFFER *Helpoutput;
    int number;
    int count;

    Helpoutput = new_buf();
    if ( argument[0] == '\0' )
        argument = "summary";

    
    /* this parts handles help a b so that it returns help 'a b' */
     argall[0] = '\0';
if (!is_number( argument ))
 {
     while (argument[0] != '\0' )
      {
       argument = one_argument(argument,argone);
       if (argall[0] != '\0')
        strcat(argall," ");
       strcat(argall,argone);
      }


    xprintf( buf, " Help file not found\n\r"
                  "{C-------------------------------------------------------{X\n\r");
    add_buf(Helpoutput,buf);
    count = 0;
    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
     {
      if ( pHelp->level > get_trust( ch ) )
       continue;

      count++;
      if ( is_full_name( argall, pHelp->keyword ))
       {
        if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
         {
          do_font(ch,argall);
         }
      
        if ( pHelp->text[0] == '.' )
         {
          page_to_char( pHelp->text+1, ch );
         }    
          else 
         {
          page_to_char( pHelp->text, ch );
         }
        free_buf(Helpoutput);      
        return;
       }
      else if ( !str_prefix( argall, pHelp->keyword ) )
       {
        xprintf( buf, "{W%6d {C*{X %s\n\r", count, pHelp->keyword);
        add_buf(Helpoutput,buf);
       }
    }

    xprintf( buf,"{C-------------------------------------------------------{X\n\r");
   add_buf(Helpoutput,buf);
   page_to_char( buf_string(Helpoutput), ch );
   free_buf(Helpoutput);    
   return;
 }
else
 {
    number = atoi(argument);
    count = 0;
    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
     {
      if ( pHelp->level > get_trust( ch ) )
       continue;

      count++;
      if ( number == count )
       {
        if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
         {
          do_font(ch,pHelp->keyword);
         }
      
        if ( pHelp->text[0] == '.' )
         {
          page_to_char( pHelp->text+1, ch );
         }    
          else 
         {
          page_to_char( pHelp->text, ch );
         }
       } 
     }  
}
free_buf(Helpoutput);    
return;    
}


void do_background( CHAR_DATA *ch, char *argument )
{

CHAR_DATA *victim;

    if ( argument[0] == '\0' ) { /* edit yours */

	if(IS_NPC(ch))
	return;

	string_append(ch, &ch->pcdata->background);
	return;
    }	
    
    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

	if(IS_AFFECTED(victim, AFF_FADE)) {
	   send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if (IS_NPC(victim) || IS_NPC(ch)) {
    send_to_char("{cYou can not do that.{x",ch);
    return;
    }  

    if (victim->pcdata->background[0]=='\0') {
	send_to_char( "{CNo one seems to know anything about that person.{x\n\r",ch );
	return;
    }
    else {
	do_font(ch,victim->name);
	send_to_char("{B",ch);
	send_to_char(victim->pcdata->background,ch);
	send_to_char("{x",ch);
	return;
    }
}

/*  end */

/* whois command */
void do_whois (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    BUFFER *output;
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool found = FALSE;
    char buf2[MAX_STRING_LENGTH];

    one_argument(argument,arg);
  
    if (arg[0] == '\0')
    {
	send_to_char("You must provide a name.\n\r",ch);
	return;
    }

    output = new_buf();
    for (d = descriptor_list; d != NULL; d = d->next)
    {
	CHAR_DATA *wch;
	char const *class;

 	if (d->connected != CON_PLAYING || !can_see(ch,d->character))
	    continue;
	
	wch = ( d->original != NULL ) ? d->original : d->character;

 	if (!can_see(ch,wch))
	    continue;

	if (!str_prefix(arg,wch->name))
	{
	    found = TRUE;
	    
	    class = class_table[wch->class].who_name;
	    switch(wch->level)
	    {
		case MAX_LEVEL - 0 : class = "IMP"; 	break;
		case MAX_LEVEL - 1 : class = "CRE";	break;
		case MAX_LEVEL - 2 : class = "SUP";	break;
		case MAX_LEVEL - 3 : class = "DEI";	break;
		case MAX_LEVEL - 4 : class = "GOD";	break;
		case MAX_LEVEL - 5 : class = "IMM";	break;
		case MAX_LEVEL - 6 : class = "DEM";	break;
		case MAX_LEVEL - 7 : class = "ANG";	break;
		case MAX_LEVEL - 8 : class = "AVA";	break;
	    }

	    if ((wch->kingdom && (ch->kingdom == wch->kingdom || 
			       IS_TRUSTED(ch,LEVEL_IMMORTAL))) ||
                               wch->level >= LEVEL_HERO)
 // ||
	//		       wch->kingdom == KINGDOM_SIRIRIUS)
	      xprintf(buf2, "{W[%s]{x ",kingdom_table[wch->kingdom].short_name);
	    else buf2[0] = '\0';

            xprintf(buf, "{G%s is a %s%s from %s{w\n\r",
		wch->name,
                wch->race < MAX_PC_RACE ?
         pc_race_table[wch->race].name  : "     ",
                IS_IMMORTAL(wch) ? " immortal" : "",
	hometown_table[wch->hometown].name);

        add_buf(output,buf);

	if((wch->kingdom > 0) && wch->kingdom_rank >= 6) 
	xprintf(buf, "  {G%s is %s of %s.{w\n\r",
		wch->sex == 0 ? "It" : wch->sex == 1 ? "He" : "She",
	kingdom_table[wch->kingdom].rank_titles[wch->kingdom_rank],
		kingdom_table[wch->kingdom].long_name	);
	else
	if((wch->kingdom > 0)  && wch->kingdom_rank < 6)
	xprintf(buf, "  {G%s is a member of %s.{w\n\r",
		wch->sex == 0 ? "It" : wch->sex == 1 ? "He" : "She",
		kingdom_table[wch->kingdom].long_name	);
	else
	xprintf(buf, "  {G%s is not affliated in any kingdoms.{w\n\r",
		wch->sex == 0 ? "It" : wch->sex == 1 ? "He" : "She");
	    add_buf(output,buf);

        if(wch->pcdata->spouse != NULL)
	xprintf(buf, "  {G%s is married to %s.{w\n\r",wch->sex == 0 ? "It" : wch->sex == 1 ? "He" : "She",wch->pcdata->spouse);
	else
        xprintf(buf, "  {G%s is unmarried.{w\n\r",wch->sex == 0 ? "It" : wch->sex == 1 ? "He" : "She");
        add_buf(output,buf);
/*
	    if (IS_TRUSTED(ch,LEVEL_IMMORTAL) || ch==wch || 
                      wch->level >= LEVEL_HERO) {
        xprintf(buf, "   {G%s has attained %d.{w\n\r",
	wch->sex == 0 ? "It" : wch->sex == 1 ? "He" : "She",
                  wch->level);
	            add_buf(output,buf);
		}
*/
/*
	      xprintf(buf, "{m[{B%2d %s %s{m]{x %s%s%s%s%s{m%s%s{x\n\r",
		      wch->level,
		      wch->race < MAX_PC_RACE ? 
		        pc_race_table[wch->race].who_name: "     ",
		      class,
		      ( (ch==wch && ch->exp < PK_MIN) || 
		        is_safe_nomessage(ch,wch) ) ? 
		       "" : "{r(PK){x ",
		      buf2,    
		      IS_SET(wch->act,PLR_WANTED) ? "{r(WANTED){x " : "",
		      IS_SET(wch->act,PLR_KILLER) ? "{r(KILLER){x " : "",
		      IS_SET(wch->act,PLR_THIEF) ? "{r(THIEF){x " : "",
		      wch->name, IS_NPC(wch) ? "" : wch->pcdata->title);
	    else
	      xprintf(buf, "{m[   {B%s{m    ] %s%s%s%s%s{m%s%s{x\n\r",
		      wch->race < MAX_PC_RACE ? 
		        pc_race_table[wch->race].who_name: "     ",
		      ( (ch==wch && ch->exp < PK_MIN) || 
		        is_safe_nomessage(ch,wch) ) ? 
		       "" : "{r(PK){x ",
		      buf2,    
		      IS_SET(wch->act,PLR_WANTED) ? "{r(WANTED){x " : "",
		      IS_SET(wch->act,PLR_KILLER) ? "{r(KILLER){x " : "",
		      IS_SET(wch->act,PLR_THIEF) ? "{r(THIEF){x " : "",
		      wch->name, IS_NPC(wch) ? "" : wch->pcdata->title);

	    strcat(output,buf);
*/
	}

    }

    if (!found)
    {
	send_to_char("No one of that name is playing.\n\r",ch);
	return;
    }

    page_to_char(buf_string(output),ch);
    free_buf(output);
}


/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who_immortal( CHAR_DATA *ch, char *argument )
{

    char buf[MAX_STRING_LENGTH];
//    char buf1[MAX_STRING_LENGTH];
 //   char arg[MAX_STRING_LENGTH];
    BUFFER *output;
    DESCRIPTOR_DATA *d;
      
    buf[0] = '\0';
    output = new_buf();
   
   for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
//	char const *Txtclass;

        /*Who can you see Begin*/
	if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	    continue;
        	
	wch   = ( d->original != NULL ) ? d->original : d->character;
	if (!can_see(ch, wch)) /* can't see switched wizi imms */
	  continue;
        /*Who can you see End*/
   
       if ( wch->level < LEVEL_IMMORTAL )
         continue;
         

       
	/*
	 * Format it up.
	 */
       
      xprintf( buf, "{c[%-22s{c]{m %s%s%s%s%s%s%s%s%s%s{x\n\r",
        wch->immtitle,
	IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {m" : (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {m" : ""),        
	IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){m " : "", 
	IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){m " : "",
	IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){m "  : "",      	
        wch->incog_level >= LEVEL_HERO ? "{M(Incog){m " : "",
        wch->invis_level >= LEVEL_HERO ? "{B(Wizi){m " : "",
	IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{m " : "",
	IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	wch->name,
	wch->pcdata->title);
      add_buf(output,buf);
    }

    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
  
}
 
/* Who list changed by Zalev to show all levels/classes/races on wholist 12-19-07*/
void do_who_mortal( CHAR_DATA *ch, char *argument )
{

    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char kingdombuf[MAX_STRING_LENGTH];
    BUFFER *output;
 //   BUFFER *immput;
    DESCRIPTOR_DATA *d;
    int iClass;
    int iRace;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int immMatch;
    int nMatch;
    int vnum;
    int count;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool fClassRestrict;
    bool fRaceRestrict;
    bool fImmortalOnly;
    bool fPKRestrict;
    bool fSiriusRestrict;
    bool fCrescentRestrict;
    bool fAsperRestrict;
    bool fKujikiriRestrict;
    bool fHordeRestrict;
    bool fGemmaRestrict;
    bool fTristanRestrict;
    bool fTattoo;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fClassRestrict = FALSE;
    fRaceRestrict = FALSE;
    fPKRestrict = FALSE;
    fImmortalOnly  = FALSE;
    fHordeRestrict = FALSE;
    fCrescentRestrict = FALSE;
    fSiriusRestrict = FALSE;
    fKujikiriRestrict = FALSE;
    fAsperRestrict = FALSE;
    fGemmaRestrict = FALSE;
    fTristanRestrict = FALSE;
    vnum = 0;
    fTattoo = FALSE;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
	rgfRace[iRace] = FALSE;


    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if (!str_cmp(arg,"pk"))
	  {
	    fPKRestrict = TRUE;
	    break;
	  }

	if (!str_cmp(arg,"sirius"))
	  {
/*
	    if (ch->kingdom != KINGDOM_SIRIUS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fSiriusRestrict = TRUE;
		break;
	    /* } */
	  }
	if (!str_cmp(arg,"argus"))
	  {
/*
	    if (ch->kingdom != KINGDOM_ARGUS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fAsperRestrict = TRUE;
		break;
//	      }
	  }
	if (!str_cmp(arg,"horde"))
	  {
/*
	    if (ch->kingdom != KINGDOM_AOYURI && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fHordeRestrict = TRUE;
		break;
//	      }
	  }
	if (!str_cmp(arg,"kujikiri"))
	  {
/*
	    if (ch->kingdom != KINGDOM_KUJIKIRI && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fKujikiriRestrict = TRUE;
		break;
	      }
//	  }
	if (!str_cmp(arg,"mordukhan"))
	  {
/*
	    if (ch->kingdom != KINGDOM_MORDUHAN &&
!IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fCrescentRestrict = TRUE;
		break;
//	      }
	  }
	if (!str_cmp(arg,"gemma"))
	  {
/*
	    if (ch->kingdom != KINGDOM_GEMMA && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fGemmaRestrict = TRUE;
		break;
	      }
//	  }
	if (!str_cmp(arg,"tristan"))
	  {
/*
	    if (ch->kingdom != KINGDOM_TRISTAN && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fTristanRestrict = TRUE;
		break;
	      }
//	  }
/* Baxter
	if (!str_cmp(arg, "tattoo"))
	  {
	    send_to_char("This function of who is disabled for now.\n\r", ch);
	    return; 
	  }
 */

	if ( is_number( arg ) && IS_IMMORTAL(ch))
	{
	    switch ( ++nNumber )
	    {
	    case 1: iLevelLower = atoi( arg ); break;
	    case 2: iLevelUpper = atoi( arg ); break; 
	    default:
		send_to_char( "This function of who is for immortals.\n\r",ch);
		return;
	    }
	}
	else
	{

	    /*
	     * Look for classes to turn on.
	     */
	    if ( arg[0] == 'i' )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
	      iClass = class_lookup(arg);
	      if (iClass == -1 || !IS_IMMORTAL(ch))
	      	{
	          iRace = race_lookup(arg);

	          if (iRace == 0 || iRace >= MAX_PC_RACE)
		    {
		      send_to_char("That's not a valid race or kingdom.\n\r",ch);
		      return;
		    }
	          else
		    {
			fRaceRestrict = TRUE;
			rgfRace[iRace] = TRUE;
		    }
		}
	      else
		{
		  fClassRestrict = TRUE;
		  rgfClass[iClass] = TRUE;
		}
	    }
	  }
      }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    immMatch = 0;
    count = 0;
    buf[0] = '\0';
    output = new_buf();
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
	char const *class;

	/*
	 * Check for match against restrictions. 
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;
	if (!can_see(ch, wch)) /* can't see switched wizi imms */
	  continue;

	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level < LEVEL_HERO )
	|| ( fClassRestrict && !rgfClass[wch->class])
	|| ( fRaceRestrict && !rgfRace[wch->race])
        || ( fPKRestrict && is_safe_nomessage(ch,wch) && !IS_IMMORTAL(wch))
/*	|| ( fTattoo &&(vnum == get_eq_char(wch,WEAR_TATTOO)->pIndexData->vnum))*/
	    || (fSiriusRestrict && wch->kingdom != KINGDOM_GONDOR)
	    || (fCrescentRestrict && wch->kingdom != KINGDOM_MORDUKHAN)
	    || (fHordeRestrict && wch->kingdom != KINGDOM_5)
	    || (fKujikiriRestrict && wch->kingdom != KINGDOM_ALDON)
	    || (fAsperRestrict && wch->kingdom != KINGDOM_ARKADIA)
	    || (fGemmaRestrict && wch->kingdom != KINGDOM_DRAKKON) 
	    || (fTristanRestrict && wch->kingdom != KINGDOM_XELENIA)
	)
	    continue;

if ( wch->level >= LEVEL_IMMORTAL )
      immMatch++;
      else
      nMatch++;
 
if ( wch->level >= LEVEL_IMMORTAL )
continue;
	/*
	 * Figure out what to print for class.
	 */
	class = class_table[wch->class].who_name;
	switch ( wch->level )
	{
	default: break;
            {
                case MAX_LEVEL - 0 : class = "IMP";     break;
                case MAX_LEVEL - 1 : class = "SYS";     break;
                case MAX_LEVEL - 2 : class = "SUP";     break;
                case MAX_LEVEL - 3 : class = "DEI";     break;
                case MAX_LEVEL - 4 : class = "GOD";     break;
                case MAX_LEVEL - 5 : class = "OSR";     break;
                case MAX_LEVEL - 8 : class = "SAD";     break;
                case MAX_LEVEL - 9 : class = "DSN";     break;
                case MAX_LEVEL - 10 : class = "ADM";     break;
                case MAX_LEVEL - 11 : class = "HAS";     break;
                case MAX_LEVEL - 12 : class = "AST";     break;
                case MAX_LEVEL - 13 : class = "LAS";     break;
                case MAX_LEVEL - 14 : class = "RET";     break;
                case MAX_LEVEL - 15 : class = "EDR";     break;
		case MAX_LEVEL - 16 : class = "NBD";	break;
		case MAX_LEVEL - 17 : class = "TBD";	break;
            }
	}

	/* Kingdom formatting */
	if (((wch->kingdom && (wch->kingdom == ch->kingdom || 
			   IS_TRUSTED(ch,LEVEL_IMMORTAL))) ||
                           wch->level <= LEVEL_HERO ||
			   wch->kingdom == KINGDOM_GONDOR) &&
			   wch->kingdom != KINGDOM_NONE)
  xprintf(kingdombuf,"{W[%s]{x ",kingdom_table[wch->kingdom].short_name);
	else kingdombuf[0] = '\0';

	/*
	 * Format it up.
	 */
if ( wch->level >= LEVEL_IMMORTAL )
     {
if (IS_TRUSTED(ch,(MAX_LEVEL - 14 )) || ch==wch ||
                   wch->level >= LEVEL_HERO)
	  xprintf( buf, "{W[{W%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
	     kingdombuf,
	    IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "", 
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            wch->incog_level >= LEVEL_HERO ? "{M(Incog){x " : "",
            wch->invis_level >= LEVEL_HERO ? "{B(Wizi){x " : "",
	    IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title );
	else
	  xprintf( buf, "{W[{W%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
	     kingdombuf,
	    	    IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
            IS_SET(wch->act, PLR_LOG)  ? "{r(LOG){x "  : "",
	    wch->pkban == 1  ?  "{R({BPKBAN{R){x":"",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name, 
	    IS_NPC(wch) ? "" : wch->pcdata->title );
//	    add_buf(immput,buf);
      }
if ( wch->level < LEVEL_IMMORTAL )
     {
if (IS_TRUSTED(ch,(MAX_LEVEL - 14 )) || ch==wch || wch->level >= LEVEL_HERO)
	  xprintf( buf, "{W[%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
	    	  ( (ch==wch && ch->exp < PK_MIN) || 
	   is_safe_nomessage(ch,wch) || IS_IMMORTAL(ch) )? 
		  "" : "{r(PK){x ",
	    kingdombuf,
	    IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
            wch->pkban == 1  ?  "{R({BPKBAN{R) {X":"",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title );
	else
	  xprintf( buf, "{W[%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
          
	  ( (ch==wch && ch->exp < PK_MIN) || 
	   is_safe_nomessage(ch,wch) || IS_IMMORTAL(ch) )? 
		  "" : "{r(PK){x ",
	    kingdombuf,
 	     IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
            wch->pkban == 1  ?  "{R({BPKBAN{R) {X":"",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title );
	    add_buf(output,buf);
      }
}
//    send_to_char( "{mImmortals Online{x\n\r", ch );
//    send_to_char( "{m---------------------{x\n\r", ch );
//    page_to_char( buf_string(immput), ch );
//    free_buf(immput);
//    send_to_char( "{cPlayers Online{x\n\r", ch );
//    send_to_char( "{c--------------{x\n\r", ch );
    page_to_char( buf_string(output), ch );
    free_buf(output);
    xprintf( buf2, "{mImmortals Found: %d{x\n\r{cPlayers found: %d{x\n\r\n\r", immMatch, nMatch );
    send_to_char(buf2,ch);
    return;
} 
void do_who( CHAR_DATA *ch, char *argument )
{
  send_to_char( "{mImmortals Online{x\n\r", ch );
  send_to_char( "{m---------------------{x\n\r", ch );
  do_who_immortal(ch,argument);
  send_to_char( "{mMortals Online{x\n\r", ch );
  send_to_char( "{m---------------------{x\n\r", ch );
  do_who_mortal(ch,argument);

return;  
}
void do_xwho( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char kingdombuf[MAX_STRING_LENGTH];
    BUFFER *output;
    BUFFER *immput;
    DESCRIPTOR_DATA *d;
    int iClass;
    int iRace;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int immMatch;
    int nMatch;
    int vnum;
    int count;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool fClassRestrict;
    bool fRaceRestrict;
    bool fImmortalOnly;
    bool fPKRestrict;
    bool fSiriusRestrict;
    bool fCrescentRestrict;
    bool fAsperRestrict;
    bool fKujikiriRestrict;
    bool fHordeRestrict;
    bool fGemmaRestrict;
    bool fTristanRestrict;
    bool fTattoo;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fClassRestrict = FALSE;
    fRaceRestrict = FALSE;
    fPKRestrict = FALSE;
    fImmortalOnly  = FALSE;
    fHordeRestrict = FALSE;
    fCrescentRestrict = FALSE;
    fSiriusRestrict = FALSE;
    fKujikiriRestrict = FALSE;
    fAsperRestrict = FALSE;
    fGemmaRestrict = FALSE;
    fTristanRestrict = FALSE;
    vnum = 0;
    fTattoo = FALSE;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
	rgfRace[iRace] = FALSE;


    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if (!str_cmp(arg,"pk"))
	  {
	    fPKRestrict = TRUE;
	    break;
	  }

	if (!str_cmp(arg,"sirius"))
	  {
/*
	    if (ch->kingdom != KINGDOM_SIRIUS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fSiriusRestrict = TRUE;
		break;
	    /* } */
	  }
	if (!str_cmp(arg,"argus"))
	  {
/*
	    if (ch->kingdom != KINGDOM_ARGUS && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fAsperRestrict = TRUE;
		break;
//	      }
	  }
	if (!str_cmp(arg,"horde"))
	  {
/*
	    if (ch->kingdom != KINGDOM_AOYURI && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fHordeRestrict = TRUE;
		break;
//	      }
	  }
	if (!str_cmp(arg,"kujikiri"))
	  {
/*
	    if (ch->kingdom != KINGDOM_KUJIKIRI && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fKujikiriRestrict = TRUE;
		break;
	      }
//	  }
	if (!str_cmp(arg,"mordukhan"))
	  {
/*
	    if (ch->kingdom != KINGDOM_MORDUHAN &&
!IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fCrescentRestrict = TRUE;
		break;
//	      }
	  }
	if (!str_cmp(arg,"gemma"))
	  {
/*
	    if (ch->kingdom != KINGDOM_GEMMA && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fGemmaRestrict = TRUE;
		break;
	      }
//	  }
	if (!str_cmp(arg,"tristan"))
	  {
/*
	    if (ch->kingdom != KINGDOM_TRISTAN && !IS_IMMORTAL(ch))
	      {
		send_to_char("You are not in that kingdom!\n\r",ch);
		return;
	      }
	    else
	      {
*/
		fTristanRestrict = TRUE;
		break;
	      }
//	  }
/* Baxter
	if (!str_cmp(arg, "tattoo"))
	  {
	    send_to_char("This function of who is disabled for now.\n\r", ch);
	    return; 
	  }
 */

	if ( is_number( arg ) && IS_IMMORTAL(ch))
	{
	    switch ( ++nNumber )
	    {
	    case 1: iLevelLower = atoi( arg ); break;
	    case 2: iLevelUpper = atoi( arg ); break; 
	    default:
		send_to_char( "This function of who is for immortals.\n\r",ch);
		return;
	    }
	}
	else
	{

	    /*
	     * Look for classes to turn on.
	     */
	    if ( arg[0] == 'i' )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
	      iClass = class_lookup(arg);
	      if (iClass == -1 || !IS_IMMORTAL(ch))
	      	{
	          iRace = race_lookup(arg);

	          if (iRace == 0 || iRace >= MAX_PC_RACE)
		    {
		      send_to_char("That's not a valid race or kingdom.\n\r",ch);
		      return;
		    }
	          else
		    {
			fRaceRestrict = TRUE;
			rgfRace[iRace] = TRUE;
		    }
		}
	      else
		{
		  fClassRestrict = TRUE;
		  rgfClass[iClass] = TRUE;
		}
	    }
	  }
      }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    immMatch = 0;
    count = 0;
    buf[0] = '\0';
    output = new_buf();
    immput = new_buf();
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
	char const *class;

	/*
	 * Check for match against restrictions. 
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;
	if (!can_see(ch, wch)) /* can't see switched wizi imms */
	  continue;

	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level < LEVEL_HERO )
	|| ( fClassRestrict && !rgfClass[wch->class])
	|| ( fRaceRestrict && !rgfRace[wch->race])
        || ( fPKRestrict && is_safe_nomessage(ch,wch) && !IS_IMMORTAL(wch))
/*	|| ( fTattoo &&(vnum == get_eq_char(wch,WEAR_TATTOO)->pIndexData->vnum))*/
	    || (fSiriusRestrict && wch->kingdom != KINGDOM_GONDOR)
	    || (fCrescentRestrict && wch->kingdom != KINGDOM_MORDUKHAN)
	    || (fHordeRestrict && wch->kingdom != KINGDOM_5)
	    || (fKujikiriRestrict && wch->kingdom != KINGDOM_ALDON)
	    || (fAsperRestrict && wch->kingdom != KINGDOM_ARKADIA)
	    || (fGemmaRestrict && wch->kingdom != KINGDOM_DRAKKON) 
	    || (fTristanRestrict && wch->kingdom != KINGDOM_XELENIA)
	)
	    continue;

if ( wch->level >= LEVEL_IMMORTAL )
      immMatch++;
      else
      nMatch++;

if ( wch->level >= LEVEL_IMMORTAL )
continue;
	/*
	 * Figure out what to print for class.
	 */
	class = class_table[wch->class].who_name;
	switch ( wch->level )
	{
	default: break;
            {
                case MAX_LEVEL - 0 : class = "IMP";     break;
                case MAX_LEVEL - 1 : class = "SYS";     break;
                case MAX_LEVEL - 2 : class = "SUP";     break;
                case MAX_LEVEL - 3 : class = "DEI";     break;
                case MAX_LEVEL - 4 : class = "GOD";     break;
                case MAX_LEVEL - 5 : class = "OSR";     break;
                case MAX_LEVEL - 8 : class = "SAD";     break;
                case MAX_LEVEL - 9 : class = "DSN";     break;
                case MAX_LEVEL - 10 : class = "ADM";     break;
                case MAX_LEVEL - 11 : class = "HAS";     break;
                case MAX_LEVEL - 12 : class = "AST";     break;
                case MAX_LEVEL - 13 : class = "LAS";     break;
                case MAX_LEVEL - 14 : class = "RET";     break;
                case MAX_LEVEL - 15 : class = "EDR";     break;
		case MAX_LEVEL - 16 : class = "NBD";	break;
		case MAX_LEVEL - 17 : class = "TBD";	break;
            }
	}

	/* Kingdom formatting */
	if (((wch->kingdom && (wch->kingdom == ch->kingdom || 
			   IS_TRUSTED(ch,LEVEL_IMMORTAL))) ||
                           wch->level <= LEVEL_HERO ||
			   wch->kingdom == KINGDOM_GONDOR) &&
			   wch->kingdom != KINGDOM_NONE)
  xprintf(kingdombuf,"{W[%s]{x ",kingdom_table[wch->kingdom].short_name);
	else kingdombuf[0] = '\0';

	/*
	 * Format it up.
	 */
if ( wch->level >= LEVEL_IMMORTAL )
     {
if (IS_TRUSTED(ch,(MAX_LEVEL - 14 )) || ch==wch ||
                   wch->level >= LEVEL_HERO)
	  xprintf( buf, "{W[{W%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
	     kingdombuf,
	    IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "", 
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            wch->incog_level >= LEVEL_HERO ? "{M(Incog){x " : "",
            wch->invis_level >= LEVEL_HERO ? "{B(Wizi){x " : "",
	    IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title );
	else
	  xprintf( buf, "{W[{W%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
	     kingdombuf,
	     IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
            IS_SET(wch->act, PLR_LOG)  ? "{r(LOG){x "  : "",
	    wch->pkban == 1  ?  "{R({BPKBAN{R){x":"",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name, 
	    IS_NPC(wch) ? "" : wch->pcdata->title );
	    add_buf(immput,buf);
      }
if ( wch->level < LEVEL_IMMORTAL )
     {
if (IS_TRUSTED(ch,(MAX_LEVEL - 14 )) || ch==wch || wch->level >= LEVEL_HERO)
	  xprintf( buf, "{W[%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
	     ( (ch==wch && ch->exp < PK_MIN) || 
	   is_safe_nomessage(ch,wch) || IS_IMMORTAL(ch) )? 
		  "" : "{r(PK){x ",
	    kingdombuf,
	    IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
            wch->pkban == 1  ?  "{R({BPKBAN{R) {X":"",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title );
	else
	 xprintf( buf, "{W[%3d %s {G%s{W]{x %s%s%s%s%s%s%s%s%s{m%s%s{x\n\r",
	    wch->level,
	    wch->race < MAX_PC_RACE ? pc_race_table[wch->race].who_name 
				    : "     ",
	    class,
          ( (ch==wch && ch->exp < PK_MIN) || 
	   is_safe_nomessage(ch,wch) || IS_IMMORTAL(ch) )? 
		  "" : "{r(PK){x ",
	    kingdombuf,
 	     IS_SET(wch->tag_flags,TAG_RED) ? "{R[RED] {x" :
            (IS_SET(wch->tag_flags,TAG_BLUE) ? "{B[BLUE] {x" : ""),
	    IS_SET(wch->act, PLR_WANTED) ? "{r(WANTED){x " : "",
	    IS_SET(wch->act, PLR_KILLER) ? "{r(KILLER){x " : "",
	    IS_SET(wch->act, PLR_THIEF)  ? "{r(THIEF){x "  : "",
            IS_SET(wch->comm, COMM_OLC) ? "{w[OLC]{x " : "",
            wch->pkban == 1  ?  "{R({BPKBAN{R) {X":"",
	    IS_NPC(wch) ? "" : wch->pcdata->pretitle,
	    wch->name,
	    IS_NPC(wch) ? "" : wch->pcdata->title );
	    add_buf(output,buf);
      }
}
    send_to_char( "{mImmortals Online{x\n\r", ch );
    send_to_char( "{m---------------------{x\n\r", ch );
    page_to_char( buf_string(immput), ch );
    free_buf(immput);
    send_to_char( "{cPlayers Online{x\n\r", ch );
    send_to_char( "{c--------------{x\n\r", ch );
    page_to_char( buf_string(output), ch );
    free_buf(output);
    xprintf( buf2, "{mImmortals Found: %d{x\n\r{cPlayers found: %d{x\n\r\n\r", immMatch, nMatch );
    send_to_char(buf2,ch);
    return;
}

void do_count ( CHAR_DATA *ch, char *argument )
{
    int count;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    count = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING && can_see( ch, d->character ) )
	    count++;

    max_on = UMAX(count,max_on);

    if (max_on == count)
xprintf(buf,"{B%d{M adventurers... the most I've seen today.{x\n\r",
	    count);
    else
xprintf(buf,"{B%d{M adventurers... I saw {m%d{M earlier today.{x\n\r",
	    count,max_on);

    send_to_char(buf,ch);
}

void do_inventory( CHAR_DATA *ch, char *argument )
{
    send_to_char( "{GYou are carrying:{x\n\r", ch );
    send_to_char("{g",ch);
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    send_to_char("{x",ch);
    return;
}


/*[UPDATED BY ZALEV 1/5/08] EQUIPMENT WILL NOW SHOW AS EMPTY SPACES FOR NEWBIES IF NO EQUIPMENT
IS BEING WORN IN THAT SPOT*/
void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;
    char buf[MAX_STRING_LENGTH];
    
    
    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
    	send_to_char( where_name[iWear], ch );
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL ) 
	    { send_to_char("{gnothing {x\n\r",ch);
	    	continue;
	    } 

	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "{x\n\r", ch );
	}
	else
	{
	    send_to_char( "something.{x\n\r", ch );
	}
	found = TRUE;
    }
    
    
    if ( !found )
	send_to_char( "Nothing.{x\n\r", ch );
	
/* [TAKEN OUT BY MAKUS]	if (ch->race == race_lookup("ogre") && !IS_NPC(ch))
     {
  	 xprintf(buf,"{W(*-*){CYou have %d old bones attached to your belt.{W(*-*){x\n\r",ch->pcdata->pkills);
   	 send_to_char(buf,ch);
     }
[TAKEN OUT BY MAKUS] */ 
    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ,NULL) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if (arg2[0] == '\0')
    {
	for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content)
	{
	    if (obj2->wear_loc != WEAR_NONE
	    &&  can_see_obj(ch,obj2)
	    &&  obj1->item_type == obj2->item_type
	    &&  (obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if (obj2 == NULL)
	{
	    send_to_char("You aren't wearing anything comparable.\n\r",ch);
	    return;
	}
    } 

    else if ( (obj2 = get_obj_carry(ch,arg2 , NULL) ) == NULL )
    {
	send_to_char("You do not have that item.\n\r",ch);
	return;
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];
	    break;

	case ITEM_WEAPON:
	    if (obj1->pIndexData->new_format)
		value1 = (1 + obj1->value[2]) * obj1->value[1];
	    else
	    	value1 = obj1->value[1] + obj1->value[2];

	    if (obj2->pIndexData->new_format)
		value2 = (1 + obj2->value[2]) * obj2->value[1];
	    else
	    	value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
	do_help(ch,"diku");
    return;
}

void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim,*dch;
    bool found;
    bool fPKonly = FALSE;

    one_argument( argument, arg );

    if (!check_blind(ch))
      return;

    if (room_is_dark(ch->in_room,NULL) && !IS_SET(ch->act, PLR_HOLYLIGHT)) {
      send_to_char("{DIt is too dark.{x\n\r",ch);
      return;
    }

    if (!str_cmp(arg,"pk"))
      fPKonly = TRUE;

if (  !IS_SET(ch->in_room->room_flags, ROOM_WILDERNESS)   ) 
{
    if ( arg[0] == '\0' || fPKonly)
    {
	send_to_char( "{BPlayers near you:{x\n\r", ch );
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if (IS_NPC(victim)) {
		 if(!IS_MIRROR(victim))
			continue;
		if (victim->in_room == NULL)
			continue;
		if (victim->in_room->area != ch->in_room->area)
			continue;
		if (!can_see(ch, victim))
			continue;

		xprintf( buf, "{r%s{c%-28s %s{x\n\r",
		 (where_safe(ch,victim)  ?
			"     " : "(PK) "),
	         victim->name,
		 victim->in_room->name );
		send_to_char( buf, ch );
		found = TRUE;
	    }
	    else {
		if (victim->desc == NULL)
			continue;
		if (victim->desc->connected != CON_PLAYING)
			continue;
	        if (fPKonly && is_safe_nomessage(ch,victim))
			continue;
		if (victim->in_room == NULL)
			continue;
		if (victim->in_room->area != ch->in_room->area)
			continue;
		if (!can_see(ch, victim))
			continue;

		dch = victim;
		if(is_affected(victim,gsn_doppelganger) 
		&& !IS_SET(ch->act,PLR_HOLYLIGHT))
			dch=victim->doppel; 
		if (ch == victim) dch = victim;
		xprintf( buf, "{r%s{c%-28s %s{x\n\r",
		 (is_safe_nomessage(ch,dch)  ?
			"     " : "(PK) "),dch->name,
		 dch->in_room->name );
		send_to_char( buf, ch );
		found = TRUE;
	}
     }	
	if ( !found )
	    send_to_char( "{cNone{x\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_FADE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		xprintf( buf, "{c%-28s %s{x\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "{cYou didn't find any $T.{x", ch, NULL, arg, TO_CHAR );
    }
}/* end if NON wilderness */
else /*wilderness*/
{
int RANGE=5,
    MAPWIDTH=160,
    VNUM=ch->in_room->vnum,
    VICTIM_FOUND=0,
    i;

	send_to_char("You can't seem to find anyone in this vast wilderness.\n\r",ch);
	return;

    if ( arg[0] == '\0' || fPKonly)
    {
	send_to_char( "{BPlayers close by:{x\n\r", ch );
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if (IS_NPC(victim)) {
		 if(!IS_MIRROR(victim))
			continue;
		if (victim->in_room == NULL)
			continue;
		if (victim->in_room->area != ch->in_room->area)
			continue;
		if (!can_see(ch, victim))
			continue;

/* goes through line sections on map */
for ( i = (-(MAPWIDTH*RANGE+RANGE))   ; /* top left corner     */
      i<= (+(MAPWIDTH*RANGE+RANGE))   ; /* bottom right corner */
      i+= MAPWIDTH ) /* does stuff to MAPWIDTH*2 lines on map */
{
	if ( (victim->in_room->vnum > VNUM+i)
           &&(victim->in_room->vnum < VNUM+(i+RANGE*2)) )
	{VICTIM_FOUND=1;break;}
}
WAIT_STATE(ch,1);
if (!VICTIM_FOUND) continue;

		xprintf( buf, "{r%s{c%-28s %s{x\n\r",
		 (where_safe(ch,victim)  ?
			"     " : "(PK) "),
	         victim->name,
		 victim->in_room->name );
		send_to_char( buf, ch );
		found = TRUE;
	    }
	    else {
		if (victim->desc == NULL)
			continue;
		if (victim->desc->connected != CON_PLAYING)
			continue;
	        if (fPKonly && is_safe_nomessage(ch,victim))
			continue;
		if (victim->in_room == NULL)
			continue;
		if (victim->in_room->area != ch->in_room->area)
			continue;
		if (!can_see(ch, victim))
			continue;

/* goes through line sections on map */
for ( i = (-(MAPWIDTH*RANGE+RANGE))   ; /* top left corner     */
      i<= (+(MAPWIDTH*RANGE+RANGE))   ; /* bottom right corner */
      i+= MAPWIDTH ) /* does stuff to MAPWIDTH*2 lines on map */
{
	if ( (victim->in_room->vnum > VNUM+i)
           &&(victim->in_room->vnum < VNUM+(i+RANGE*2)) )
	{VICTIM_FOUND=1;break;}
}
WAIT_STATE(ch,1);
if (!VICTIM_FOUND) continue;

		dch = victim;
		if(is_affected(victim,gsn_doppelganger) 
		&& !IS_SET(ch->act,PLR_HOLYLIGHT))
			dch=victim->doppel; 
		if (ch == victim) dch = victim;
		xprintf( buf, "{r%s{c%-28s %s{x\n\r",
		 (is_safe_nomessage(ch,dch)  ?
			"     " : "(PK) "),dch->name,
		 dch->in_room->name );
		send_to_char( buf, ch );
		found = TRUE;
	}
     }	
	if ( !found )
	    send_to_char( "{cNone{x\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
/* goes through 20 lines on map */
for ( i = (-(MAPWIDTH*RANGE+RANGE))   ; /* top left corner     */
      i<= (+(MAPWIDTH*RANGE+RANGE))   ; /* bottom right corner */
      i+= MAPWIDTH ) /* does stuff to MAPWIDTH*2 lines on map */
{
	if ( (victim->in_room->vnum > VNUM+i)
           &&(victim->in_room->vnum < VNUM+(i+RANGE*2)) )
	{VICTIM_FOUND=1;break;}
}
WAIT_STATE(ch,1);

	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_FADE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name )
	    && 	 VICTIM_FOUND )
	    {
		found = TRUE;
		xprintf( buf, "{c%-28s %s{x\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "{cYou didn't find any $T.{x", ch, NULL, arg, TO_CHAR );
    }

}/* end wilderness */
    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    char *align;
    int diff;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, NULL, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	send_to_char("Don't even think about it.\n\r",ch);
	return;
    }

    diff = victim->level - ch->level;

         if ( diff <= -10 ) msg = "{gYou could kill $N with your little finger.{x";
    else if ( diff <=  -5 ) msg = "{g$N doesn't have a fighting chance.{x";
    else if ( diff <=  -2 ) msg = "{g$N looks like an easy kill.{x";
    else if ( diff <=   1 ) msg = "{gThe perfect match!{x";
    else if ( diff <=   4 ) msg = "{gA few lucky blows would kill $m.{x";
    else if ( diff <=   9 ) msg = "{g$N shows you $s razor-sharp teeth.{x";
    else                    msg = "{gA hooded figure waits patiently nearby.{x";

    if (IS_EVIL(ch) && IS_EVIL(victim))
      align = "{g$N grins evilly with you.{x";
    else if (IS_GOOD(victim) && IS_GOOD(ch))
      align = "{g$N greets you warmly.{x";
    else if (IS_GOOD(victim) && IS_EVIL(ch))
      align = "{g$N smiles at you, hoping you will turn from your evil path.{x";
    else if (IS_EVIL(victim) && IS_GOOD(ch))
      align = "{g$N grins evilly at you.{x";
    else if (IS_NEUTRAL(ch) && IS_EVIL(victim))
      align = "{g$N grins evilly.{x";
    else if (IS_NEUTRAL(ch) && IS_GOOD(victim))
      align = "{g$N smiles happily.{x";
    else if (IS_NEUTRAL(ch) && IS_NEUTRAL(victim))
      align = "{g$N looks just as disinterested as you.{x";
    else
      align = "{g$N looks very disinterested.{x";

    act( msg, ch, NULL, victim, TO_CHAR );
    act( align, ch, NULL, victim, TO_CHAR);
    return;
}


void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?' )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}






void do_description( CHAR_DATA *ch, char *argument )
{

if(IS_NPC(ch))
return;

string_append(ch, &ch->description);
return;
}

#if 0 /* removes old description editor*/
void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );

    	if (argument[0] == '-')
    	{
            int len;
            bool found = FALSE;
 
            if (ch->description == NULL || ch->description[0] == '\0')
            {
                send_to_char("No lines left to remove.\n\r",ch);
                return;
            }
	
  	    strcpy(buf,ch->description);
 
            for (len = strlen(buf); len > 0; len--)
            {
                if (buf[len] == '\r')
                {
                    if (!found)  /* back it up */
                    {
                        if (len > 0)
                            len--;
                        found = TRUE;
                    }
                    else /* found the second one */
                    {
                        buf[len + 1] = '\0';
			free_string(ch->description);
			ch->description = str_dup(buf);
			send_to_char( "Your description is:\n\r", ch );
			send_to_char( ch->description ? ch->description : 
			    "(None).\n\r", ch );
                        return;
                    }
                }
            }
            buf[0] = '\0';
	    free_string(ch->description);
	    ch->description = str_dup(buf);
	    send_to_char("Description cleared.\n\r",ch);
	    return;
        }
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

	if ( strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 2 )
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}
#endif


void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
	return;

    xprintf( buf,
"{cYou report that you have {C%2d%%{c hp, {C%2d%%{c mana, and {C%2d%%{c \
moves.{x\n\r",
	PERCENT(ch->hit,  ch->max_hit),
	PERCENT(ch->mana, ch->max_mana),
	PERCENT(ch->move, ch->max_move));
    send_to_char( buf, ch );

    xprintf( buf,
"{c$n reports that $e has {C%2d%%{c hp, {C%2d%%{c mana, and {C%2d%%{c \
moves.{x\n\r",
	PERCENT(ch->hit,  ch->max_hit),
	PERCENT(ch->mana, ch->max_mana),
	PERCENT(ch->move, ch->max_move));
    act( buf, ch, NULL, NULL, TO_ROOM );
    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf2[20000];
    char buf[MAX_STRING_LENGTH];
    int sn,temp;

    if ( IS_NPC(ch) )
        return;

    if ( argument[0] == '\0' )
    {
        int col;

        col    = 0;
        strcpy( buf2, "" );
        for ( sn = 0; sn < MAX_SKILL; sn++ )
        {
       if ( skill_table[sn].name == NULL )
                break;
            if ( ch->level < skill_table[sn].skill_level[ch->class] ||
(skill_table[sn].kingdom != ch->kingdom && skill_table[sn].kingdom != KINGDOM_NONE)
              )
                continue;

            xprintf( buf, "{C%-18s {W%3d%%  {w",
                skill_table[sn].name, ch->pcdata->learned[sn] );
            strcat( buf2, buf );
            if ( ++col % 3 == 0 )
                strcat( buf2, "\n\r" );
        }

        if ( col % 3 != 0 )
  strcat( buf2, "\n\r" );

        xprintf( buf, "You have %d practice sessions left.\n\r",
            ch->practice );
        strcat( buf2, buf );

        if ( IS_IMMORTAL( ch ) )
          page_to_char( buf2, ch );
        else
          send_to_char( buf2, ch );
    }
    else
    {
        CHAR_DATA *mob;
        int adept;

        if ( !IS_AWAKE(ch) )
    {
            send_to_char( "In your dreams, or what?\n\r", ch );
            return;
        }

        if ( ch->practice <= 0 )
        {
            send_to_char( "You have no practice sessions left.\n\r", ch );
            return;
        }
     if ( ( sn = find_spell( ch,argument ) ) < 0
        || ( !IS_NPC(ch)
        &&   (ch->level < skill_table[sn].skill_level[ch->class] ||
(skill_table[sn].kingdom != ch->kingdom && skill_table[sn].kingdom != KINGDOM_NONE) ) ) )
        {
            send_to_char( "You can't practice that.\n\r", ch );
            return;
        }
 for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
        {
            if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
              {
                if (skill_table[sn].kingdom == KINGDOM_NONE)
                 {
                  if ( ( mob->pIndexData->practicer == skill_table[sn].group )
		|| mob->pIndexData->practicer != 0 )
                  break;
                 }
     else
                 {
                  if (ch->kingdom == mob->kingdom)      break;
                 }
             }
        }

        if ( mob == NULL )
        {
            send_to_char( "You can't do that here.\n\r",ch);
            return;
        }

        adept = IS_NPC(ch) ? 100 : class_table[ch->class].skill_adept;

        if ( ch->pcdata->learned[sn] >= adept )
        {
       xprintf( buf, "You are already learned at %s.\n\r",
                skill_table[sn].name );
            send_to_char( buf, ch );
        }
        else
        {
            if (!ch->pcdata->learned[sn]) ch->pcdata->learned[sn] = 1;
            ch->practice--;
		/* Make it easier to practice since Kazmir modify the prac too
		 * hard to practice
		 *		Takeda 3-2-00
		 */
			temp = skill_table[sn].rating[ch->class];
			if(temp <= 1)
				temp = 1;
			else
				temp /= 2;

            ch->pcdata->learned[sn] +=
                int_app[get_curr_stat(ch,STAT_INT)].learn /
                UMAX(temp,1);
            if ( ch->pcdata->learned[sn] < adept )
            {
                act( "You practice $T.",
                    ch, NULL, skill_table[sn].name, TO_CHAR );
                act( "$n practices $T.",
                    ch, NULL, skill_table[sn].name, TO_ROOM );
    }
            else
            {
                ch->pcdata->learned[sn] = adept;
                act( "You are now learned at $T.",
                    ch, NULL, skill_table[sn].name, TO_CHAR );
                act( "$n is now learned at $T.",
                    ch, NULL, skill_table[sn].name, TO_ROOM );
            }
        }
    }
    return;
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else  {
	wimpy = atoi( arg );
        if ( ch->level < 40 )  {
          if ( wimpy > 10000 ) wimpy = 10000;
          wimpy = (ch->max_hit*wimpy)/100;
        }
    }
    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit/2 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
      xprintf( buf, "{cWimpy set to {C%d{c hit points.{x\n\r", wimpy );

    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_scan(CHAR_DATA *ch, char *argument)
{
  char dir[MAX_INPUT_LENGTH];
  char *dir2;
  ROOM_INDEX_DATA *in_room;
  ROOM_INDEX_DATA *to_room;
  EXIT_DATA *exit;
  int door;
  int range;
  char buf[MAX_STRING_LENGTH];
  int i;
  CHAR_DATA *person;
  int numpeople;

  one_argument(argument,dir);

  if (dir[0]=='\0')
    {
      send_to_char("Usage: scan <direction>.\n\r",ch);
      return;
    }

  switch (dir[0])
    {
    case 'N':
    case 'n':
      door = 0;
      dir2 = "north";
      break;
    case 'E':
    case 'e':
      door = 1;
      dir2 = "east";
      break;
    case 'S':
    case 's':
      door = 2;
      dir2 = "south";
      break;
    case 'W':
    case 'w':
      door = 3;
      dir2 = "west";
      break;
    case 'U':
    case 'u':
      door = 4;
      dir2 = "up";
      break;
    case 'D':
    case 'd':
      door = 5;
      dir2 = "down";
      break;
    default:
      send_to_char("That's not a direction.\n\r",ch);
      return;
    }
  
  WAIT_STATE(ch,2);
  xprintf(buf, "{gYou scan %s.{x\n\r",dir2);
  send_to_char(buf,ch);
  xprintf(buf, "{g$n scans %s.{x",dir2);
  act(buf,ch,NULL,NULL,TO_ROOM);

  if (!check_blind(ch))
    return;

  range = 1 + (ch->level)/10;
  
  in_room = ch->in_room;

  for (i=1; i <= range; i++)
    {

      if ( (exit = in_room->exit[door]) == NULL
	  || (to_room = exit->u1.to_room) == NULL
	  || IS_SET(exit->exit_info,EX_CLOSED) )
	return;

      for (numpeople = 0,person = to_room->people; person != NULL;
	   person = person->next_in_room)
	if (can_see(ch,person)) numpeople++;
	      
      if (numpeople)
	{
	  xprintf(buf, "{G***** {g%d %s {G*****{g\n\r",i,dir2);
	  send_to_char(buf,ch);
	  show_char_to_char(to_room->people, ch);
	  send_to_char("{x\n\r", ch);
	}
      in_room = to_room;
    }
}

void do_request(CHAR_DATA *ch, char *argument)
{
  char arg1 [MAX_INPUT_LENGTH];
  char arg2 [MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  OBJ_DATA  *obj;
/*  int sn; 
  AFFECT_DATA af;  */

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
 
  if (IS_NPC(ch))
	return;
 
  if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
      send_to_char( "Request what from whom?\n\r", ch );
      return;
    }

  if ( ( victim = get_char_room( ch, NULL, arg2 ) ) == NULL )
    {
      send_to_char( "They aren't here.\n\r", ch );
      return;
    }
  
  if (!IS_NPC(victim))
    {
      send_to_char("Why don't you just ask the player?\n\r", ch);
      return;
    }

  if (!IS_GOOD(ch))
    {
	do_say(victim,"I will not give anything to someone so impure.");
      return;
    }

  if (ch->move < (50 + ch->level))
    {
      do_say(victim,"You look rather tired, why don't you rest a bit first?");
      return;
    }

  WAIT_STATE(ch, 15); 
  ch->move -= 10;
  ch->move = UMAX(ch->move, 0);

  if (victim->level >= ch->level + 10 || victim->level >= ch->level * 2)
    {
	do_say(victim,"In good time, my child");
      return;
    }

  if (( ( obj = get_obj_carry(victim , arg1 ,NULL) ) == NULL 
      && (obj = get_obj_wear(victim, arg1 ,TRUE)) == NULL)
      || IS_SET(obj->extra_flags, ITEM_INVENTORY))
    {
	do_say(victim,"Sorry, I don't have that.");
      return;
    }
  
  if (!IS_GOOD(victim))
    {
	do_say(victim,"I'm not about to give you anything!");
	do_murder(victim,ch->name);
      return;
    }
  
  if ( obj->wear_loc != WEAR_NONE )
    unequip_char(victim, obj);
  
  if ( !can_drop_obj( ch, obj ) )
    {
	do_say(victim,"Sorry, I can't let go of it.  It's cursed.");
      return;
    }
  if (obj->pIndexData->limit >= 0) {
	do_say(victim,"I'm Sorry, but i can't give you that.");
	do_say(victim,"I will kill you now for requesting that!!");
	do_murder(victim,ch->name);
	return;
    }  
  if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
      send_to_char( "Your hands are full.\n\r", ch);
      return;
    }
  
  if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
      send_to_char( "You can't carry that much weight.\n\r", ch);
      return;
    }
  
  if ( !can_see_obj( ch, obj ) )
    {
      act( "You don't see that.", ch, NULL, victim, TO_CHAR );
      return;
    }

  


  obj_from_char( obj );
  obj_to_char( obj, ch );
  act( "$n requests $p from $N.", ch, obj, victim, TO_NOTVICT );
  act( "You request $p from $N.",   ch, obj, victim, TO_CHAR    );
  act( "$n requests $p from you.", ch, obj, victim, TO_VICT    );

  if (IS_SET(obj->progtypes,IPROG_GIVE))
    (obj->pIndexData->iprogs->give_prog) (obj,ch,victim);

 if (IS_SET(victim->progtypes,MPROG_GIVE))
    (victim->pIndexData->moprogs->give_prog) (victim,ch,obj);



  ch->move -= (50 + ch->level);
  ch->move = UMAX(ch->move, 0);
  ch->hit -= 3 * (ch->level / 2);
  ch->hit = UMAX(ch->hit, 0);

  act("You feel grateful for the trust of $N and meditate a while on the goodness you", ch, NULL, victim, TO_CHAR);  
  send_to_char("have seen in the world.\n\r", ch);
  
  WAIT_STATE(ch, 4 * PULSE_TICK);
  return;
}
  

void do_hometown(CHAR_DATA *ch, char *argument)
{
  int amount;
  char buf[MAX_INPUT_LENGTH];

  if (IS_NPC(ch))
    {
      send_to_char("You can't change your hometown!\n\r", ch);
      return;
    }

  if (!IS_SET(ch->in_room->room_flags, ROOM_REGISTRY))
    {
  send_to_char("You have to be in a registry to change your hometown.\n\r",
      ch);
      return;
    }
  
  amount = (ch->level * ch->level * 10) + 1000;

  if (argument[0] == '\0')
    {
      xprintf(buf, "It will cost you %d gold.\n\r", amount);
      send_to_char(buf, ch);
      return;
    }

  if (ch->gold < amount)
    {
      send_to_char("You don't have enough money to change hometowns!\n\r",ch);
      return;
    }
  if (!str_prefix(argument, "sanaria"))
    {
      if (ch->hometown == 0)
        {
	  send_to_char("But you already live in Sanaria!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Sanaria.\n\r", ch);
      ch->hometown = 0;
      return;
    }

  else if (!str_prefix(argument, "newthalos"))
    {
      if (ch->hometown == 1)
        {
	  send_to_char("But you already live in New Thalos!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to New Thalos.\n\r", ch);
      ch->hometown = 1;
      return;
    }
/*
  else if (!str_prefix(argument, "rivendell"))
    {
      if (!IS_GOOD(ch))
	{
	  send_to_char("Only good people can live in Rivendell.\n\r", ch);
	  return;
 	}
      if (ch->hometown == 2)
        {
	  send_to_char("But you already live in Rivendell!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Rivendell.\n\r", ch);
      ch->hometown = 2;
      return;
    }
*/
  else if (!str_prefix(argument, "ofcol"))
    {
      if (!IS_NEUTRAL(ch))
	{
	  send_to_char("Only neutral people can live in Ofcol.\n\r", ch);
	  return;
 	}
      if (ch->hometown == 3)
        {
	  send_to_char("But you already live in Ofcol!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Ofcol.\n\r", ch);
      ch->hometown = 3;
      return;
    }

  else if (!str_prefix(argument, "arkham"))
    {
      if (!IS_EVIL(ch))
	{
	  send_to_char("Only evil people can live in Arkham.\n\r", ch);
	  return;
 	}
      if (ch->hometown == 4)
        {
	  send_to_char("But you already live in Arkham!\n\r", ch);
	  return;
	}
      ch->gold -= amount;
      send_to_char("Your hometown is changed to Arkham.\n\r", ch);
      ch->hometown = 4;
      return;
    }

  else
    {
      send_to_char("That is not a valid choice.\n\r", ch);
send_to_char("Choose from Sanaria, Newthalos, Ofcol and Arkham.\n\r",
      		ch);
      return;
    }
}


void do_detect_hidden( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_detect_hidden].skill_level[ch->class] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

    if ( CAN_DETECT(ch, DETECT_HIDDEN) )
    {
	send_to_char("You are already as alert as you can be. \n\r",ch);
	return;
    }
    if ( number_percent( ) > get_skill( ch, gsn_detect_hidden ) )
    {
    	check_improve(ch,gsn_detect_hidden,FALSE,1);
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    WAIT_STATE(ch, skill_table[gsn_detect_hidden].beats);
    af.where     = TO_DETECTS;
    af.type      = gsn_detect_hidden;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = DETECT_HIDDEN;
    affect_to_char( ch, &af );
    send_to_char( "Your awareness improves.\n\r", ch );
    check_improve(ch,gsn_detect_hidden,TRUE,1);
    return;
}


void do_bear_call( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *gch;
  CHAR_DATA *bear;
  CHAR_DATA *bear2;
  AFFECT_DATA af;
  int i;
  
  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_bear_call].skill_level[ch->class] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

  send_to_char("You call for bears help you.\n\r",ch);
  act("$n shouts a bear call.",ch,NULL,NULL,TO_ROOM);

  if (is_affected(ch, gsn_bear_call))
    {
      send_to_char("You cannot summon the strength to handle more bears right now.\n\r", ch);
      return;
    }
  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
	  gch->pIndexData->vnum == MOB_VNUM_BEAR)
	{
	  send_to_char("What's wrong with the bear you've got?",ch);
	  return;
	}
    }

  if ( ch->in_room != NULL && IS_SET(ch->in_room->room_flags, ROOM_NO_MOB) )
  {
     send_to_char( "No bears listen you.\n\r", ch );
     return;
  }

  if ( number_percent( ) > get_skill( ch, gsn_bear_call) )
  {
	send_to_char( "No bears listen you.\n\r", ch );
	return;
  }
  
  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||
       IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  ||
       (ch->in_room->exit[0] == NULL &&
          ch->in_room->exit[1] == NULL &&
          ch->in_room->exit[2] == NULL &&
          ch->in_room->exit[3] == NULL &&
          ch->in_room->exit[4] == NULL &&
          ch->in_room->exit[5] == NULL) ||
         ( ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS ) )
  {
    send_to_char( "No bears come to your rescue.\n\r", ch );
    return;
  }

  if ( ch->mana < 125 )
  {
     send_to_char( "You don't have enough mana to shout a bear call.\n\r", ch );
     return;
  }
  WAIT_STATE(ch, skill_table[gsn_bear_call].beats);
  ch->mana -= 125;

  bear = create_mobile( get_mob_index(MOB_VNUM_BEAR) );

  for (i=0;i < MAX_STATS; i++)
    {
      bear->perm_stat[i] = UMIN(25,2 * ch->perm_stat[i]);
    }

  bear->max_hit = ((IS_NPC(ch)? ch->max_hit : ch->pcdata->perm_hit)/2);
  bear->hit = bear->max_hit;
  bear->max_mana = IS_NPC(ch)? ch->max_mana : ch->pcdata->perm_mana;
  bear->mana = bear->max_mana;
  bear->alignment = ch->alignment;
  bear->level = ch->level-5;
  for (i=0; i < 3; i++)
    bear->armor[i] = interpolate(bear->level,100,-100);
  bear->armor[3] = interpolate(bear->level,100,0);
  bear->sex = ch->sex;
  bear->gold = 0;

  if(ch->level >= 60) {
  bear2 = create_mobile(bear->pIndexData);
  clone_mobile(bear,bear2);
  SET_BIT(bear2->affected_by, AFF_CHARM);
  char_to_room(bear2,ch->in_room);
  add_follower( bear2, ch );
  bear2->leader = ch;
  SET_BIT(bear->affected_by, AFF_CHARM);
  char_to_room(bear,ch->in_room);
  add_follower( bear, ch );
  bear->leader = ch;
  send_to_char("Two bears come to your rescue!\n\r",ch);
  act("Two bears come to $n's rescue!",ch,NULL,NULL,TO_ROOM);
  }
   else
  {
  SET_BIT(bear->affected_by, AFF_CHARM);
  add_follower( bear, ch );
  bear->leader = ch;
  char_to_room(bear,ch->in_room);
  send_to_char("One bears come to your rescue!\n\r",ch);
  act("One bears come to $n's rescue!",ch,NULL,NULL,TO_ROOM);
  }
  af.where		= TO_AFFECTS;
  af.type               = gsn_bear_call;
  af.level              = ch->level; 
  af.duration           = 24;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);  

}

void do_acute_vision( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_acute_vision].skill_level[ch->class] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

    if ( CAN_DETECT(ch, ACUTE_VISION ) )
    {
	send_to_char("Your vision is already acute. \n\r",ch);
	return;
    }
    if ( number_percent( ) > get_skill( ch, gsn_acute_vision ) )  {
      send_to_char( 
         "You search in the forest, but find nothing.\n\r", 
         ch );
      return;
    }
    WAIT_STATE(ch, skill_table[gsn_acute_vision].beats);
    af.where     = TO_DETECTS;
    af.type      = gsn_acute_vision;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ACUTE_VISION;
    affect_to_char( ch, &af );
    send_to_char( "Your vision sharpens.\n\r", ch );
    return;
}



void do_dark_vision( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_dark_vision].skill_level[ch->class] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }

    if (is_affected(ch, gsn_dark_vision))
    {
	send_to_char("You are already affected dark vision.\n\r",ch);
	return;
    }
    if ( number_percent( ) > get_skill( ch, gsn_dark_vision ) )
    {
	send_to_char( "You failed.\n\r", ch );
	check_improve(ch,gsn_dark_vision,FALSE,1);
	return;
    }

    WAIT_STATE(ch, skill_table[gsn_dark_vision].beats);
    af.where     = TO_AFFECTS;
    af.type      = gsn_dark_vision;
    af.level     = ch->level;
    af.duration  = ch->level / 3;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char( "Your eyes glows red.\n\r", ch );
    check_improve(ch,gsn_dark_vision,TRUE,1);
    return;
}

/*END*/
void do_questinfo(CHAR_DATA *ch, char *argument)
{
send_to_char("You have solved the following quests:\n",ch);
 if ( IS_SET( ch->quest, QUEST_WEAPONSMITH ) )  
	send_to_char("Andor the Weaponmaster\n",ch);
 if ( IS_SET( ch->quest, QUEST_GOLEM ) )  
	send_to_char("Queen of New Thalos\n",ch);
 if(!IS_SET(ch->quest,QUEST_WEAPONSMITH) && !IS_SET(ch->quest,QUEST_GOLEM))
	send_to_char("None\n",ch);
return;
}



void do_marry( CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *victim2;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );


       if ( arg1[0] == '\0' || arg2[0] == '\0' )
          {
             send_to_char("Syntax: marry <char1> <char2>\n\r",ch);
             return;
       }

       if ( ((victim = get_char_world(ch,arg1)) == NULL) ||
            ((victim2 = get_char_world(ch,arg2)) == NULL))
          {
          send_to_char("Both characters must be playing!\n\r", ch);
          return;
          }

       if ( IS_NPC(victim) || IS_NPC(victim2))
          {
          send_to_char("Sorry! Mobs can't get married!\n\r", ch);
          return;
          }
       if (victim->level < 25 || victim2->level < 25)
          {
          send_to_char("They are not of the proper level to marry.\n\r",ch);
          return;
          }

        bug(victim->pcdata->spouse,0);
        bug(victim2->pcdata->spouse,0);
       if ((strlen(victim->pcdata->spouse) < 1) && (strlen(victim2->pcdata->spouse) < 1))
      {
            send_to_char("You pronounce them man and wife!\n\r",ch);
            send_to_char("You say the big 'I do.'\n\r", victim);
            send_to_char("You say the big 'I do.'\n\r",victim2);

            act("$n and $N are now declared married!\n\r", victim,NULL, victim2, TO_ROOM);

            victim->pcdata->spouse = str_dup(victim2->name);

            victim2->pcdata->spouse = str_dup(victim->name);

            return;
          }
        else
        {
           send_to_char("They are Already Married!\n\r", ch);
           return;
	}
       return;
}


void do_divorce( CHAR_DATA *ch, char *argument)
   {
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
   CHAR_DATA *victim2;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

       if ( arg1[0] == '\0' || arg2[0] == '\0' )
          {
          send_to_char("Syntax: divorce <char1> <char2>\n\r",ch);
          return;
          }
       if ( ((victim = get_char_world(ch,arg1)) == NULL) ||
            ((victim2 = get_char_world(ch,arg2)) == NULL))
          {
          send_to_char("Both characters must be playing!\n\r", ch);
          return;
          }

     if ( IS_NPC(victim) || IS_NPC(victim2))
          {
          send_to_char("I don't think they're Married to the Mob!\n\r",ch);
          return;
          }

       if ( !str_cmp( victim->pcdata->spouse, victim2->name ) && !str_cmp(victim2->pcdata->spouse, victim->name ) )
       {
       send_to_char("You hand them their papers.\n\r", ch);
       send_to_char("Your divorce is final.\n\r", victim);
       send_to_char("Your divorce is final.\n\r", victim2);

       act("$n and $N swap divorce papers, they are no-longer married.",victim, NULL, victim2, TO_NOTVICT);

       free_string( victim->pcdata->spouse         );
       free_string( victim2->pcdata->spouse        );
       victim->pcdata->spouse = str_dup( "" );
       victim2->pcdata->spouse = str_dup( "" );
       return;
       }
       else
       {
       send_to_char("They arent married!", ch);
       return;
       }
}

/*
void do_4score( CHAR_DATA *ch, char *argument)
{
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	send_to_char("{c|{W                   God Kazmir, Lord Creation         {c             |\n\r", ch);
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	send_to_char("{c|{W Combat Strategy: {c({yAssault{c)   {c|{W Race:    {c({yHuman{c) {c({yMale{c)            |\n\r", ch);
	send_to_char("{c|{W Position:        {c({yStanding{c)  {c|{W Class:   {c({ywarrior{c)                 |\n\r", ch);
	send_to_char("{c|{W Speed:           {c({yUnlimited{c) {c|{W Kingdom: {c({yTORSANCTUS{c)              |\n\r", ch);
	send_to_char("{c|{W Fatigued:        {c({yLow{c)       {c|{W Rank:    {c({yGod{c)                     |\n\r", ch);
	send_to_char("{c|{W Saves:           {c({yGood{c)      {c|{W {c({y Lawful  Evil {c)                   |\n\r", ch);
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	send_to_char("{c|{W Hp: {c({R99999/99999{c){W Mp: {c({B99999/99999{c){W Mv: {c({G99999/99999{c)              |\n\r", ch);
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	send_to_char("{c|{W Str: {c({yGodly{c)             {c| {WGlory Points:    {c({y000000 {c/ {y000000{c) {c    |\n\r", ch);
	send_to_char("{c|{W Int: {c({yAbsolute{c)          {c| {WRoleplay tokens: {c({y000000 {c/ {y000000{c) {c    |\n\r", ch);
	send_to_char("{c|{W Wis: {c({yMystical{c)          {c| {WCarrying:                             {c|\n\r", ch);
	send_to_char("{c|{W Dex: {c({yUnlimited{c)         {c| {W Items:    {c({y4/{y1000{c)                    {c|\n\r", ch);
	send_to_char("{c|{W Con: {c({yIdeal{c)             {c| {W Weighing: {c({y305149{c/{y10000000{c) stones.   {c|\n\r", ch);
	send_to_char("{c|{W Agi: {c({yUnearthly{c)         {c| {W Level: {c({y100{c) Tnl: {c[{yXXXXXXXXXX{c]        {c|\n\r", ch);
	send_to_char("{c|{W Cha: {c({yAngelic{c)           {c| {W Status: {c({yHungry{c)                      {c|\n\r", ch);
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	send_to_char("{C            You also carry 761118 gold and 6981 silver.\n\r", ch);
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	send_to_char("{c| Holy Light: on  Trust: (100)\n\r", ch);
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
}
*/
