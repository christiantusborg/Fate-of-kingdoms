/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/****************************************************************************
				Death
****************************************************************************/
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "magic.h"
#include "arena.h"
#include "fok_struct.h"

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char log_buf[MAX_STRING_LENGTH];
    char pfile[MAX_STRING_LENGTH];
    char strsave[MAX_INPUT_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    SHOP_DATA *pShop;
    char *name;
    
    pShop = NULL;

   if (!IS_NPC(ch))
    {
     
     save_char_obj(ch);
     sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
     sprintf(pfile,"/home/fate/.ftpbak/ss/Backup/death/%s.bak",capitalize( ch->name ) );
     rename(strsave,pfile);
     save_char_obj(ch);
    }



    if ( IS_NPC(ch) )
    {
        name            = ch->short_descr;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
        corpse->timer   = number_range( 3, 6 );
        corpse->value[0] = ch->pIndexData->vnum;
        

  /*      ch->gold = 0;
        ch->silver = ch->Wealth * (number_range( 50, 150 ) / 100)
*/

     if((pShop = ch->pIndexData->pShop) != NULL) {
          ch->gold = ch->level/2;
          ch->silver = ch->level;
        }

        if ( ch->gold > 0 )
     {
     	    if (IS_SET(ch->form,FORM_UNDEAD))
     	    obj_to_room( create_money( ch->gold, ch->silver ), ch->in_room );
     	    else
            obj_to_obj( create_money( ch->gold, ch->silver ), corpse );
            ch->gold = 0;
            ch->silver = 0;
        }
        corpse->cost = 0;
    }
    else
    {
        name            = ch->name;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC),0);
        corpse->timer   = number_range( 200, 300 );
        corpse->lootamount = 0;
        corpse->weight = 1000;
        REMOVE_BIT(ch->act,PLR_CANLOOT);
        corpse->owner = str_dup(ch->name);

            if (ch->gold > 1 || ch->silver > 1)
            {
                obj_to_obj(create_money(ch->gold / 2, ch->silver/2),corpse);
                ch->gold -= ch->gold/2;
                ch->silver -= ch->silver/2;
            }

        corpse->cost = 0;
    }

    corpse->level = ch->level;
    
    sprintf( buf, corpse->name, ch->name );
    free_string( corpse->name );
    corpse->name = str_dup( buf );

    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        bool floating = FALSE;
     

        obj_next = obj->next_content;

    if ( !IS_NPC( ch ) )
        {
         sprintf( log_buf, "DEATH LOG EQ: player: %s obj [vnum: %d]NAME: %s SHORTDESC: %s",ch->name,obj->pIndexData->vnum,obj->name,obj->short_descr);
         log_string( log_buf );
        }
        
        if (obj->wear_loc == WEAR_FLOAT)
            floating = TRUE;
        obj_from_char( obj );
        if (obj->item_type == ITEM_POTION)
            obj->timer = number_range(500,1000);
        if (obj->item_type == ITEM_SCROLL)
            obj->timer = number_range(1000,2500);
      if (IS_SET(obj->extra_flags,ITEM_ROT_DEATH) && !floating)
        {
            obj->timer = number_range(5,10);
            REMOVE_BIT(obj->extra_flags,ITEM_ROT_DEATH);
        }
        REMOVE_BIT(obj->extra_flags,ITEM_VIS_DEATH);

      if ( IS_SET( obj->extra_flags, ITEM_INVENTORY )  
          || ( (obj->pIndexData->limit != -1 
          && (obj->pIndexData->count >= obj->pIndexData->limit) ) 
          && (!IS_NPC( ch ) ) ) )
     
          {
            extract_obj( obj );
          }
        else if (floating)
        {
            if (IS_OBJ_STAT(obj,ITEM_ROT_DEATH)) /* get rid of it! */
            {
                if (obj->contains != NULL)
                {
                    OBJ_DATA *in, *in_next;

                    act("$p evaporates, scattering its contents.",
                        ch,obj,NULL,TO_ROOM);
                    for (in = obj->contains; in != NULL; in = in_next)
                    {
                        in_next = in->next_content;
                        obj_from_obj(in);
                        obj_to_room(in,ch->in_room);
                    }
                 }
                 else
                    act("$p evaporates.",
                        ch,obj,NULL,TO_ROOM);
                 extract_obj(obj);
            }
            else
            {
                act("$p falls to the floor.",ch,obj,NULL,TO_ROOM);
          obj_to_room(obj,ch->in_room);
            }
        }
        else
        {
            if (IS_SET(ch->form,FORM_UNDEAD))
            obj_to_room( obj, ch->in_room );
            else
            obj_to_obj( obj, corpse );
        }            
    }
    if (IS_SET(ch->form,FORM_UNDEAD))
    act("$n's body crumbles to dust.",ch, NULL,NULL,TO_ROOM);
    else
    obj_to_room( corpse, ch->in_room );
    return;
}

void death_cry( CHAR_DATA *ch )
{
  death_cry_org( ch, -1 );
}

/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry_org( CHAR_DATA *ch, int part )
{
    ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;
    int vnum;
      
    vnum = 0;
    msg = "{cYou hear $n's death cry.{x";
     
    if ( part == -1 )
      part = number_bits(4);

    switch ( part )
    {

    case  0: msg  = "{c$n hits the ground ... DEAD.{x";			break;
    case  1:
	if (ch->material == 0)
	{
	    msg  = "{c$n splatters blood on your armor.{x";
	    break;
	}
    case  2:
	if (IS_SET(ch->parts,PART_GUTS))
	{
	    msg = "{c$n spills $s guts all over the ground.{x";
	    vnum = OBJ_VNUM_GUTS;
	}
	break;
    case  3:
	if (IS_SET(ch->parts,PART_HEAD))
	{
	    msg  = "{c$n's severed head plops onto the ground.{x";
	    vnum = OBJ_VNUM_SEVERED_HEAD;
	}
	break;
    case  4:
	if (IS_SET(ch->parts,PART_HEART))
	{
	    msg  = "{c$n's heart is torn from $s chest.{x";
	    vnum = OBJ_VNUM_TORN_HEART;
	}
	break;
    case  5:
	if (IS_SET(ch->parts,PART_ARMS))
	{
	    msg  = "{c$n's arm is sliced from $s dead body.{x";
	    vnum = OBJ_VNUM_SLICED_ARM;
	}
	break;
    case  6:
	if (IS_SET(ch->parts,PART_LEGS))
	{
	    msg  = "{c$n's leg is sliced from $s dead body.{x";
	    vnum = OBJ_VNUM_SLICED_LEG;
	}
	break;
    case 7:
	if (IS_SET(ch->parts,PART_BRAINS))
	{
	    msg = "{c$n's head is shattered, and $s brains splash all over you.{x";
	    vnum = OBJ_VNUM_BRAINS;
	}
    }

    act( msg, ch, NULL, NULL, TO_ROOM );

    if ( vnum != 0 )
    {
	char buf[MAX_STRING_LENGTH];
	OBJ_DATA *obj;
	char *name;

	name		= IS_NPC(ch) ? ch->short_descr : ch->name;
	obj		= create_object( get_obj_index( vnum ), 0 );
	obj->timer	= number_range( 4, 7 );

	sprintf( buf, obj->short_descr, name );
	free_string( obj->short_descr );
	obj->short_descr = str_dup( buf );

	sprintf( buf, obj->description, name );
	free_string( obj->description );
	obj->description = str_dup( buf );

	obj->from = str_dup(name);

	if (obj->item_type == ITEM_FOOD)
	{
	    if (IS_SET(ch->form,FORM_POISON))
		obj->value[3] = 1;
	    else if (!IS_SET(ch->form,FORM_EDIBLE))
		obj->item_type = ITEM_TRASH;
	}

	obj_to_room( obj, ch->in_room );
    }

    if ( IS_NPC(ch) )
	msg = "{cYou hear something's death cry.{x";
    else
	msg = "{cYou hear someone's death cry.{x";

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = was_in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   pexit->u1.to_room != was_in_room )
	{
	    ch->in_room = pexit->u1.to_room;
	    act( msg, ch, NULL, NULL, TO_ROOM );
	}
    }
    ch->in_room = was_in_room;

    return;
}

/*
 *  For death cry, improved by GG(). part = deathcry()
 */

void raw_kill( CHAR_DATA *ch,CHAR_DATA *victim )
{
   OBJ_DATA *corpse;
   CHAR_DATA *mch;
  
	if (check_death_door(victim))
		return;
	
	if (arena_kill(ch,victim))
		return;
/*		
	if (IS_NPC(ch) && !IS_NPC(victim))
	{
		victim->pcdata->death += 1;
		victim->pcdata->score[MOB_DEATH]++;
		mch = ch->master;
		if (!IS_NPC(mch) && (mch != NULL))
		mch->pcdata->score[PLAYER_KILL]++;				
	}
	
	if (!IS_NPC(ch) && IS_NPC(victim))
		ch->pcdata->score[KILLED_MOB]++;
		
*/

	


	if (!IS_NPC(ch) && !IS_NPC(victim) )
	  {
		victim->pcdata->score[PLAYER_DEATH]++;
		ch->pcdata->score[PLAYER_KILL]++;
  		if ((!IS_NPC(ch)) && (number_percent() < 5))
  		  {
    			char buf[MAX_STRING_LENGTH];
    			sprintf(buf, " the Nefarious %s", capitalize(class_table[ch->class].name ));
    			free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );

		 }		
	  }
	
	gain_exp( victim, - UMIN(1000,victim->exp * 0.08));	
  	raw_kill_org( victim, -1 );
     
    if (!IS_NPC(ch)
	&&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
	&&  corpse->item_type == ITEM_CORPSE_NPC &&
	can_see_obj(ch,corpse))
	{
	    corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 

	    if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
		 corpse && corpse->contains) /* exists and not empty */
		do_get(ch,"all corpse");

	   if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                    corpse && corpse->contains  && /* exists and not empty */
                    !IS_SET(ch->act,PLR_AUTOLOOT))  {
		do_get(ch,"gold corpse");
		do_get(ch,"silver corpse");
                }
            
	    if ( IS_SET(ch->act, PLR_AUTOSAC) )
	     {
       	      if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
		return;
	      else
		do_sacrifice(ch,"corpse");
	     }
	}
  return; 
}

 void raw_kill_org( CHAR_DATA *victim, int part )
{

  CHAR_DATA *tmp_ch;
  OBJ_DATA *obj;
  int i;
  OBJ_DATA *tattoo; 

  if(victim->msp == 1)
  send_to_char("!!MUSIC(music/fokdeath.mid T=death)",victim);

  stop_fighting( victim, TRUE );
  if (victim->assassin != NULL)
   {
    victim->assassin->assassin = NULL;
    victim->assassin = NULL;
   }
   
   if ( IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) ||
   (victim->in_room == get_room_index(ROOM_VNUM_AFTERLIFE)))
   {
   	victim->position	= POS_RESTING;
  	victim->hit		= 1;
  	victim->mana		= 1;
  	send_to_char("{cYou have been eliminated!.{x\n\r",victim);
  	return;
   }

/* work on m/i progs  */
  for (obj = victim->carrying;obj != NULL;obj = obj->next_content)
    {
      if (IS_SET(obj->progtypes,IPROG_DEATH))
	if ((obj->pIndexData->iprogs->death_prog) (obj,victim))
	  {
	    victim->position = POS_STANDING;
	    return;
	  }
    }
  victim->last_fight_time = -1;
  if ( IS_SET(victim->progtypes,MPROG_DEATH))
    if ((victim->pIndexData->moprogs->death_prog) (victim))
      {
	victim->position = POS_STANDING;
	return;
      }
/* Okay enough of that i/m prog work */

//  if (ch->last_fought != NULL)
//   ch->last_fought = NUL;
  victim->last_death_time = current_time;

//  if arena_kill

  tattoo = get_eq_char(victim, WEAR_TATTOO);
  if (tattoo != NULL)
    obj_from_char(tattoo);

if ( !IS_SET ( victim->in_room->room_flags, ROOM_ARENA ) ) { 

    death_cry_org( victim, part );
    make_corpse( victim );
}

  if ( IS_NPC(victim) )
    {
      victim->pIndexData->killed++;
      kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
      extract_char( victim, TRUE );

      return;
    }

  send_to_char("{cYou turn into an invincible ghost for a few minutes,{x\n\r",victim);
  send_to_char("{cas long as you do not attack anything.{x\n\r", victim);
    extract_char( victim, FALSE );

  while ( victim->affected )
    affect_remove( victim, victim->affected );
victim->affected_by = race_table[victim->race].aff;
victim->affected_by2 = race_table[victim->race].aff2;
victim->detection = race_table[victim->race].detect;
  for (i = 0; i < 4; i++)
    victim->armor[i]= 100;
  victim->position	= POS_RESTING;
  victim->hit		= 1;
  victim->mana	= 1;
  victim->move	= victim->level>25?UMAX( 1, victim->move): victim->max_move;

  /* RT added to prevent infinite deaths */
  REMOVE_BIT(victim->act, PLR_KILLER);
  REMOVE_BIT(victim->act, PLR_THIEF);
//  REMOVE_BIT(victim->act, PLR_WANTED);
  REMOVE_BIT(victim->act, PLR_BOUGHT_PET);

  victim->pcdata->condition[COND_THIRST] = 40;
  victim->pcdata->condition[COND_HUNGER] = 40;
  victim->pcdata->condition[COND_FULL] = 40;



  if (tattoo != NULL)
    {
      obj_to_char(tattoo, victim);
      equip_char(victim, tattoo, WEAR_TATTOO);
    }
    save_char_obj( victim ); 

  /* Calm the mad mobs down */
  for (tmp_ch = char_list; tmp_ch != NULL; tmp_ch = tmp_ch->next)
    if (tmp_ch->last_fought == victim)
      tmp_ch->last_fought = NULL;

  return;
}
