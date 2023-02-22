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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "merc.h"

DECLARE_DO_FUN( do_say		);
DECLARE_DO_FUN( do_yell		);
DECLARE_DO_FUN( do_split	);
DECLARE_DO_FUN( do_kill		);

/*
 * Local functions.
 */
#define CD CHAR_DATA
#define OD OBJ_DATA
bool	remove_obj	args( (CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
CD *	find_keeper	args( (CHAR_DATA *ch ) );
int	get_cost	args( (CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
void 	obj_to_keeper	args( (OBJ_DATA *obj, CHAR_DATA *ch ) );
OD *	get_obj_keeper	args( (CHAR_DATA *ch,CHAR_DATA *keeper,char *argument));
void    check_look	args( (OBJ_DATA *obj ) );

#undef OD
#undef	CD

/* RT part of the corpse looting code */

bool can_loot(CHAR_DATA *ch, OBJ_DATA *obj)
{
    CHAR_DATA *owner, *wch;

return TRUE;
    if (IS_IMMORTAL(ch))
	return TRUE;

    if (!obj->owner || obj->owner == NULL)
	return TRUE;

    owner = NULL;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
        if (!str_cmp(wch->name,obj->owner))
            owner = wch;

    if (owner == NULL)
	return TRUE;

    if (!str_cmp(ch->name,owner->name))
	return TRUE;

    if (!IS_NPC(owner) && IS_SET(owner->act,PLR_CANLOOT))
	return TRUE;

    if (is_same_group(ch,owner))
	return TRUE;

    return FALSE;
}


void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    char buffer[100];

    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
	send_to_char( "You can't take that.\n\r", ch );
	return;
    }

    if ((IS_OBJ_STAT (obj, ITEM_DIMENSION)) &&  
       (!is_affected(ch, gsn_duo_dimension)))
    {
        send_to_char("You are not in the same dimension with that item.\n\r", ch);
        return;
    }
    
      if ((!IS_OBJ_STAT (obj, ITEM_DIMENSION)) &&  
       (is_affected(ch, gsn_duo_dimension)))
    {
        send_to_char("You are not in the same dimension with that item.\n\r", ch);
        return;
    }
    
if (!IS_IMMORTAL(ch))
{
if (checkgetput(ch, obj) )
      return;
}
else checkgetput(ch, obj);

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
	act( "$d: you can't carry that many items.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }


    if ( get_carry_weight(ch) + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	act( "$d: you can't carry that much weight.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

/*
    if (!can_loot(ch,obj))
    {
	act("Corpse looting is not permitted.",ch,NULL,NULL,TO_CHAR );
	return;
    }
*/

    if (obj->in_room != NULL)
    {
	for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
	    if (gch->on == obj)
	    {
		act("$N appears to be using $p.",
		    ch,obj,gch,TO_CHAR);
		return;
	    }
    }
		

    if ( container != NULL )
    {
/*
    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  get_trust(ch) < obj->level)
	{
	    send_to_char("You are not powerful enough to use it.\n\r",ch);
	    return;
	}
*/
	if((obj->item_type == ITEM_CORPSE_PC) && !IS_IMMORTAL(ch)
	&& str_cmp(ch->name,obj->owner))
	{
	send_to_char("You can't do that.\n",ch);
	return;
	}
/* This code don't realy do any thing so i am takeing out Kazmir
	if(container->item_type == ITEM_CORPSE_PC) { 
    if (str_cmp(ch->name,container->owner))
    // The 150 below represents 150 pieces of looted gear, a roughly impossible
    // amount.  It was changed to remove corpse looting restrictions easily and
    // sloppily while leaving the feature easy to put back into action
    // Derkec 11/23/99
    // Changed again to only allow one item at a time to be taken.
    // Derkec 2/6/00
    // if ((container->lootamount >= 150) && !IS_IMMORTAL(ch)) {
	 if ((container->lootamount >= 1) && !IS_IMMORTAL(ch)) {

	//    send_to_char("You cannot loot anymore.\n\r",ch);
		container->lootamount = 0;	
	      return;
		}
		else
		{container->lootamount += 1;}
	}
*/
      if (container->pIndexData->vnum == OBJ_VNUM_KUJIKIRI_SKULL
       || container->pIndexData->vnum == OBJ_VNUM_SIRIUS_STAND
       || container->pIndexData->vnum == OBJ_VNUM_HORDE_THRONE
       || container->pIndexData->vnum == OBJ_VNUM_CRESCENT_ALTAR
       || container->pIndexData->vnum == OBJ_VNUM_ASPER_ALTAR
        || container->pIndexData->vnum == OBJ_VNUM_GEMMA_ALTAR)
        {
          act( "{WYou get $p from $P.{x", ch, obj, container, TO_CHAR );
          if (!IS_AFFECTED(ch,AFF_SNEAK))
            act( "{W$n gets $p from $P.{x", ch, obj, container, TO_ROOM );
          obj_from_obj( obj );
          act("{W$p fades to black, then disappears!{x", ch, container, NULL,TO_ROOM);
          act("{W$p fades to black, then disappears!{x", ch, container, NULL,TO_CHAR);
          extract_obj(container);
          obj_to_char(obj, ch);

          if (IS_SET(obj->progtypes,IPROG_GET))
            (obj->pIndexData->iprogs->get_prog) (obj,ch);
	  return;
	}

    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  !CAN_WEAR(container, ITEM_TAKE)
	&&  !IS_OBJ_STAT(obj,ITEM_HAD_TIMER))
	    obj->timer = 0;	
	act( "{gYou get $p from $P.{x", ch, obj, container, TO_CHAR );
        if (!IS_AFFECTED(ch,AFF_SNEAK))
	  act( "{g$n gets $p from $P.{x", ch, obj, container, TO_ROOM );
	REMOVE_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	obj_from_obj( obj );
    }
    else
    {
	act( "{gYou get $p.{x", ch, obj, container, TO_CHAR );
        if (!IS_AFFECTED(ch,AFF_SNEAK))
	  act( "{g$n gets $p.{x", ch, obj, container, TO_ROOM );
	obj_from_room( obj );
/*  adds hidden obj code */
   if ( IS_SET( obj->extra_flags, ITEM_HIDDEN ) )
       REMOVE_BIT( obj->extra_flags, ITEM_HIDDEN );
    }

    if ( obj->item_type == ITEM_MONEY)
    {
	ch->silver += obj->value[0];
	ch->gold += obj->value[1];
        if (IS_SET(ch->act,PLR_AUTOSPLIT))
        { /* AUTOSPLIT code */
    	  members = 0;
    	  for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	  {
            if (!IS_AFFECTED(gch,AFF_CHARM) && is_same_group( gch, ch ) )
              members++;
    	  }

	  if ( members > 1 && (obj->value[0] > 1 || obj->value[1]))
	  {
	    sprintf(buffer,"%d %d",obj->value[0],obj->value[1]);
		do_split(ch,buffer);
	  }
        }
 
	extract_obj( obj );
    }
    else
    {
	obj_to_char( obj, ch );
        if (IS_SET(obj->progtypes,IPROG_GET))
          (obj->pIndexData->iprogs->get_prog) (obj,ch);
    }

    return;
}



void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char log_buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;


    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"from"))
	argument = one_argument(argument,arg2);


    /* Get type. */
    if ( arg1[0] == '\0' )
    {
	send_to_char( "{CGet what?{x\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj' */
	    obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    if ( obj == NULL )
	    {
		act( "{CI see no $T here.{x", ch, NULL, arg1, TO_CHAR );
		return;
	    }

     if (strstr(obj->pIndexData->area->builders, "Unlinked")
	&& !IS_IMMORTAL(ch)) {
	sprintf(buf,"%s is not approved yet.\n\r",obj->short_descr);
	send_to_char(buf,ch);
	return;
        } 

	    get_obj( ch, obj, NULL );
	}
	else
	{
	    /* 'get all' or 'get all.obj' */
	    found = FALSE;
	    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
       if (strstr(obj->pIndexData->area->builders, "Unlinked")
	&& !IS_IMMORTAL(ch)) {
	 sprintf(buf,"%s is not approved yet.\n\r",obj->short_descr);
	 send_to_char(buf,ch);
	return;
       }
		    get_obj( ch, obj, NULL );
		}
	    }

	    if ( !found ) 
	    {
		if ( arg1[3] == '\0' )
		    send_to_char( "I see nothing here.\n\r", ch );
		else
		    act( "{CI see no $T here.{x", ch, NULL, &arg1[4], TO_CHAR );
	    }
	}
    }
    else
    {
	/* 'get ... container' */
	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}

	if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    act( "{CI see no $T here.{x", ch, NULL, arg2, TO_CHAR );
	    return;
	}
	switch ( container->item_type )
	{
	default:
	    send_to_char( "{CThat's not a container.{x\n\r", ch );
	    return;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	    break;

	case ITEM_CORPSE_PC:
	    {
	/*	if ( (!str_cmp( arg1, "all" ) 
		     || !str_prefix( "all.", arg1 ))
		     && str_cmp(ch->name,obj->owner))
 		{
	    		send_to_char( "You can't do that.\n\r", ch );
	    		return;
		}
	*/
/*
		if ( (!str_cmp( arg1, "all" ) && str_cmp(ch->name,container->owner) &&  !IS_IMMORTAL(ch)))
 		{
	    		send_to_char( "You can't do that.\n\r", ch );
	    		return;
		}
*/	

       if ( ( (!str_cmp( arg1, "all" ) 
	       || !str_prefix( "all.", arg1 )
	       || !str_prefix( "1.", arg1 )
	       || !str_prefix( "2.", arg1 )
	       || !str_prefix( "3.", arg1 )
	       || !str_prefix( "4.", arg1 )
	       || !str_prefix( "5.", arg1 )
	       || !str_prefix( "6.", arg1 )
	       || !str_prefix( "7.", arg1 )
	       || !str_prefix( "8.", arg1 )
	       || !str_prefix( "9.", arg1 )
	       || !str_prefix( "10.", arg1 )
	       || !str_prefix( "11.", arg1 )
	       || !str_prefix( "12.", arg1 )
	       || !str_prefix( "13.", arg1 )
	       || !str_prefix( "14.", arg1 )
	       || !str_prefix( "15.", arg1 )
	       || !str_prefix( "16.", arg1 )
	       || !str_prefix( "17.", arg1 )
	       || !str_prefix( "18.", arg1 )
	       || !str_prefix( "19.", arg1 )
	       || !str_prefix( "20.", arg1 ))
	       && str_cmp(ch->name,container->owner) )
	       && !IS_IMMORTAL(ch)) 
			{
	    		 send_to_char( "{CDon't be so greedy!.\n\r", ch );
	    		 return;
  		        }


		if (!can_loot(ch,container) &&  !IS_IMMORTAL(ch))
		{
		    send_to_char( "{CYou can't do that.{x\n\r", ch );
		    return;
		}
	    }
	}

	if ( IS_SET(container->value[1], CONT_CLOSED) )
	{
	    act( "{CThe $d is closed.{x", ch, NULL, container->name, TO_CHAR );
	    return;
	}

       if (strstr(container->pIndexData->area->builders, "Unlinked")
	&& !IS_IMMORTAL(ch)) {
	 sprintf(buf,"%s is not approved yet.\n\r",container->short_descr);
	 send_to_char(buf,ch);
	return;
       }


	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj container' */
	    obj = get_obj_list( ch, arg1, container->contains );
	    if ( obj == NULL )
	    {
		act( "{CI see nothing like that in the $T.{x",
		    ch, NULL, arg2, TO_CHAR );
		return;
	    }
         
         if (container->item_type == ITEM_CORPSE_PC)
          {
           sprintf( log_buf, "%s [%d] %s from the corpse of %s",ch->name,obj->pIndexData->vnum,obj->name,container->owner);
           log_string( log_buf );

           if (str_cmp(ch->name,container->owner))
            {
             wiznet(log_buf,ch,NULL,WIZ_FLAGS,0,0);
             }
          }
	    get_obj( ch, obj, container );
	}
	else
	{
	    /* 'get all container' or 'get all.obj container' */
	    found = FALSE;
	    for ( obj = container->contains; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    if (container->pIndexData->vnum == OBJ_VNUM_PIT
		    &&  !IS_IMMORTAL(ch))
		    {
			send_to_char("{CDon't be so greedy!{x\n\r",ch);
			return;
		    }
    	           get_obj( ch, obj, container );
		}
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
		    act( "{CI see nothing in the $T.{x",
			ch, NULL, arg2, TO_CHAR );
		else
		    act( "{CI see nothing like that in the $T.{x",
			ch, NULL, arg2, TO_CHAR );
	    }
	}
    }

    return;
}


void do_hide_obj(CHAR_DATA *ch, char *argument)
{
    char        arg[MAX_INPUT_LENGTH];
    int         chance;
    OBJ_DATA    *obj;

    one_argument(argument,arg);
    if ( arg[0] == '\0' )
    {
        send_to_char("What do you want to hide?\n\r", ch);
        return;
    }
   if ( !( obj = get_obj_carry( ch, arg ) ) )
   {
       send_to_char( "You do not have that item.\n\r", ch );
       return;
   }
   if ( !can_drop_obj( ch, obj ) )
   {
       send_to_char( "You can't let go of it.\n\r", ch );
       return;
   }
   obj_from_char( obj );
   obj_to_room( obj, ch->in_room );

   if (IS_SET(obj->extra_flags, ITEM_HIDDEN)) 
   /* no use in hiding it again */
       return;


   chance = number_range(1, 5);
   if (ch->class == CLASS_RAN)      /* Rangers are better */
       chance += 2;
    if (ch->class == CLASS_THI 
	|| ch->class == CLASS_SPE 
	|| ch->class == CLASS_ASN 
	|| ch->class == CLASS_INQ 
	|| ch->class == CLASS_BRD
	)
        chance++;

    if (chance > 5)                     /* Let's not push it... */
        chance = 5;

    if (ch->level > LEVEL_HERO )        /* Immortals will manage */
        chance = 5;
    switch (chance)                     /* Let's see what we've got */
    {
        case 1:
        act("$n is on all fours trying to hide $p.", ch, obj, NULL,TO_ROOM );
        act("You try to hide $p, but fail misarably.", ch, obj, NULL,TO_CHAR );
                break;
        case 2:
act("$n is on all fours digging in the dirt.", ch, NULL, NULL, TO_ROOM );
act("You hide $p, but you did it quite obvious.", ch, obj, NULL, TO_CHAR);
                SET_BIT(obj->extra_flags, ITEM_HIDDEN);
                break;
        case 3:
        act( "You fail to hide $p.", ch, obj, NULL, TO_CHAR );
                break;
        case 4:
        case 5:
                act( "You hide $p.", ch, obj, NULL, TO_CHAR );
                SET_BIT(obj->extra_flags, ITEM_HIDDEN);
                break;


     }
}


void do_search(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    char buf[MAX_INPUT_LENGTH];
    char buf2[MAX_INPUT_LENGTH];
    int found = FALSE;
    int chance;

    buf[0] = '\0';
    buf2[0] = '\0';

    for ( obj = ch->in_room->contents;obj && (!found); obj =obj->next_content )
    {
        if ( IS_SET(obj->extra_flags, ITEM_HIDDEN) )
        {
                chance = number_range( 1, 5);
                if (
		    (ch->class == CLASS_RAN) 
		 || (ch->class == CLASS_THI)
		 || (ch->class == CLASS_ASN)
		 || (ch->class == CLASS_SPE)
		 || (ch->class == CLASS_INQ)
		 || (ch->class == CLASS_BRD)
		   )
                        chance++;
                if (chance > 3)
                {
                        sprintf(buf, "You find %s.\n\r",obj->short_descr);
                        strcat(buf2, buf);
                        buf[0] = '\0';
                        REMOVE_BIT(obj->extra_flags, ITEM_HIDDEN);
                        found = TRUE;
                }
        }
    }
    if (!found)
        strcat(buf2, "You find nothing.\n\r");
    send_to_char(buf2, ch);
    buf2[0] = '\0';
}



void do_put( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *objc;
    int pcount;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"in") || !str_cmp(arg2,"on"))
	argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Put what in what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	return;
    }

    if ( container->item_type != ITEM_CONTAINER )
    {
	send_to_char( "That's not a container.\n\r", ch );
	return;
    }

    if ( IS_SET(container->value[1], CONT_CLOSED) )
    {
	act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	return;
    }


    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
    {
	/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

if (!IS_IMMORTAL(ch)) {
	if (checkgetput(ch, obj) )
        return;
}
else {
	checkgetput(ch, obj);
}

	if ( obj == container )
	{
	    send_to_char( "You can't fold it into itself.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

  if (IS_NPC(ch))
   if (( obj->pIndexData->limit != -1 )
       && obj->pIndexData->count > obj->pIndexData->limit)
       {
        send_to_char( "{cYou can't find it.{x\n\r", ch );
        return;
      }       	
       
      if(WEIGHT_MULT(obj) !=100)
    	{
           send_to_char("You have a feeling that would be a bad idea.\n\r",ch);
            return;
        }

	if ( obj->pIndexData->limit != -1 &&
             container->pIndexData->limit == -1 && !IS_SET(container->wear_flags, ITEM_TAKE) )  {
	  act( "This unworthy container won't hold $p.", ch,obj,NULL,TO_CHAR);
	  return;
	}

      if ( obj->pIndexData->limit != -1 &&
          IS_NPC(ch))  {
	  act( "You cannot put $p in the bag.", ch,obj,NULL,TO_CHAR);
	  return;
	}

	if (get_obj_weight( obj ) + get_true_weight( container )
	     > (container->value[0] * 10) 
	||  get_obj_weight(obj) > (container->value[3] * 10))
	{
	    send_to_char( "It won't fit.\n\r", ch );
	    return;
	}

      if (strstr(obj->pIndexData->area->builders, "Unlinked")) {
        sprintf(buf,"%s is not approved yet.\n\r",obj->short_descr);
        send_to_char(buf,ch);
        return;
       }
  if ( ( obj->pIndexData->limit != -1 )  && IS_NPC(ch) &&
    ( obj->pIndexData->count >= obj->pIndexData->limit ) )
        {
        send_to_char("NPC cannot put limit eq which is over.\n\r",ch);
        return;
        }

	if ( obj->item_type == ITEM_POTION && 
	     IS_SET(container->wear_flags, ITEM_TAKE) )
	{
	   pcount = 0;
	   for(objc=container->contains; objc!=NULL ; objc=objc->next_content)
	     if (objc->item_type == ITEM_POTION) pcount++;
	   if ( pcount>15 )
	   {
	      act("It is not safe to put more potions into $p.",ch,container,NULL, TO_CHAR);
	      return;
	   }
	}

	if (container->pIndexData->vnum == OBJ_VNUM_PIT &&  !CAN_WEAR(container,ITEM_TAKE))
	 {
	  if (obj->timer)
	   {
	    SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	   }
	  else
	   {
	    obj->timer = number_range(100,200);
	   }
	 } 

	obj_from_char( obj );
	obj_to_obj( obj, container );

	if (IS_SET(container->value[1],CONT_PUT_ON))
	{
	    act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
	    act("You put $p on $P.",ch,obj,container, TO_CHAR);
	}
	else
	{
	    act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
	    act( "You put $p in $P.", ch, obj, container, TO_CHAR );
	}
    }
    else
    {
	/* 'put all container' or 'put all.obj container' */
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   WEIGHT_MULT(obj) == 100
	    &&   obj->wear_loc == WEAR_NONE
	    &&   obj != container
	    &&   can_drop_obj( ch, obj )
	    &&   get_obj_weight( obj ) + get_true_weight( container )
		 <= (container->value[0] * 10) 
	    &&   get_obj_weight(obj) < (container->value[3] * 10))
	    {
	    	if (container->pIndexData->vnum == OBJ_VNUM_PIT &&  !CAN_WEAR(obj, ITEM_TAKE) )
	    	 {
	    	    if (obj->timer)
	    	     {
			SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
		     }
	    	    else
	    	     {
	    	      obj->timer = number_range(100,200);
	    	     } 
                  }
		if ( obj->pIndexData->limit != -1 &&
       	      	     container->pIndexData->limit == -1 && !IS_SET(container->wear_flags, ITEM_TAKE) )  {
		  act( "This unworthy container will not hold $p, it is too powerful.",ch,obj,NULL,TO_CHAR);
		  continue;
		}

		if ( obj->item_type == ITEM_POTION &&
	     	     IS_SET(container->wear_flags, ITEM_TAKE) )
		{
	   	  pcount = 0;
	   	  for(objc=container->contains; objc!=NULL ; objc=objc->next_content)
	     	  if (objc->item_type == ITEM_POTION) pcount++;
	   	  if ( pcount>15 )
	   	  {
	     	    act("It's not safe to put more potions into $p.",ch,container,NULL, TO_CHAR);
	      	    continue;
	   	  }
		}

		obj_from_char( obj );
		obj_to_obj( obj, container );

        	if (IS_SET(container->value[1],CONT_PUT_ON))
        	{
            	    act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
            	    act("You put $p on $P.",ch,obj,container, TO_CHAR);
        	}
		else
		{
		    act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
		    act( "You put $p in $P.", ch, obj, container, TO_CHAR );
		}
	    }
	}
    }

    return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int count = 0;
    bool found;


    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{CDrop what?{x\n\r", ch );
	return;
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
     {
      ++count;
     } 
      if (++count > 5000)
       {
        send_to_char( "You Better not drop any more in this room it will just get lost.\n\r", ch );
        return;
       }	



    if ( is_number( arg ) )
    {
	/* 'drop NNNN coins' */
	int amount, gold = 0, silver = 0;

	amount   = atoi(arg);
	argument = one_argument( argument, arg );
	if ( amount <= 0
	|| ( str_cmp( arg, "coins" ) && str_cmp( arg, "coin" ) && 
	     str_cmp( arg, "gold"  ) && str_cmp( arg, "silver") ) )
	{
	    send_to_char( "{cSorry, you can't do that.{x\n\r", ch );
	    return;
	}

	if ( !str_cmp( arg, "coins") || !str_cmp(arg,"coin") 
	||   !str_cmp( arg, "silver"))
	{
	    if (ch->silver < amount)
	    {
		send_to_char("{cYou don't have that much silver.{x\n\r",ch);
		return;
	    }

	    ch->silver -= amount;
	    silver = amount;
	}

	else
	{
	    if (ch->gold < amount)
	    {
		send_to_char("{cYou don't have that much gold.{x\n\r",ch);
		return;
	    }

	    ch->gold -= amount;
  	    gold = amount;
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    switch ( obj->pIndexData->vnum )
	    {
	    case OBJ_VNUM_SILVER_ONE:
		silver += 1;
		extract_obj(obj);
		break;

	    case OBJ_VNUM_GOLD_ONE:
		gold += 1;
		extract_obj( obj );
		break;

	    case OBJ_VNUM_SILVER_SOME:
		silver += obj->value[0];
		extract_obj(obj);
		break;

	    case OBJ_VNUM_GOLD_SOME:
		gold += obj->value[1];
		extract_obj( obj );
		break;

	    case OBJ_VNUM_COINS:
		silver += obj->value[0];
		gold += obj->value[1];
		extract_obj(obj);
		break;
	    }
	}

	obj = create_money( gold, silver );
	obj_to_room( obj, ch->in_room );
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	  act( "{g$n drops some coins.{x", ch, NULL, NULL, TO_ROOM );
	send_to_char( "{gYou dropped some coins.{x\n\r", ch );
	return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
	ROOM_INDEX_DATA *pToRoom;       /* Line added! */
        int i = 0;                      /* Line added! */

	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "{cYou do not have that item.{x\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "{CYou can't let go of it.{x\n\r", ch );
	    return;
	}

  if ( ( obj->pIndexData->limit != -1 )  && IS_NPC(ch) &&
    ( obj->pIndexData->count >= obj->pIndexData->limit ) )
        {
        send_to_char("NPC cannot drop limit eq which is over.\n\r",ch);
        return;
        }

    if ( IS_SET(obj->extra_flags, ITEM_MOBGIVE)  &&
		!IS_NPC(ch) && !IS_IMMORTAL(ch)) {
	send_to_char("You are not allow to give this away.\n\r",ch);
	return;
    }

	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
  	  act( "{g$n drops $p.{x", ch, obj, NULL, TO_ROOM );
	act( "{gYou drop $p.{x", ch, obj, NULL, TO_CHAR );

        if (IS_SET(obj->progtypes,IPROG_DROP))
          (obj->pIndexData->iprogs->drop_prog) (obj,ch);

	if (IS_OBJ_STAT(obj,ITEM_MELT_DROP) )
	{
	  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
	    act("{g$p dissolves into smoke.{x",ch,obj,NULL,TO_ROOM);
	  act("{g$p dissolves into smoke.{x",ch,obj,NULL,TO_CHAR);
	  extract_obj(obj);
	return;
	}
	/* Phoenix: Start NEW BIT */
        for ( ; obj->in_room->exit[DIR_DOWN]
             && (pToRoom = obj->in_room->exit[DIR_DOWN]->u1.to_room)
             && obj->in_room->sector_type == SECT_AIR
             && i < 10; i++ ) {
           static ROOM_INDEX_DATA *visited_rooms[10];
           int j;
           
           if ( obj->in_room->people )
              act("$p falls away through the air below you.",
              obj->in_room->people, obj, NULL, TO_ALL);
           visited_rooms[i] = obj->in_room;
           for ( j = 0; j <= i; j++ )
              if ( visited_rooms[j] == pToRoom )
                 break;
           obj_from_room(obj);
           obj_to_room(obj, pToRoom);

           if ( obj->in_room->people && obj->in_room->exit[DIR_DOWN]
             && obj->in_room->exit[DIR_DOWN]->u1.to_room
             && obj->in_room->sector_type == SECT_AIR ) {
              if ( obj->in_room->people )
              act( "$p falls through the air.\n\r",
              obj->in_room->people, obj, NULL, TO_ALL);
           } else
              /* switch statement */
              act("$p falls from the sky and lands on the ground in "
              "front of you.", obj->in_room->people, obj, NULL, TO_ALL);
        }
        /* Phoenix: End NEW BIT */
    }
    else
    {
	/* 'drop all' or 'drop all.obj' */
	found = FALSE;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == WEAR_NONE 
	    &&   (!(( obj->pIndexData->limit != -1 )  && IS_NPC(ch) &&
                 (obj->pIndexData->count >= obj->pIndexData->limit ) ))
	    &&   can_drop_obj( ch, obj ) )
	    {
		found = TRUE;
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );
	  	if ( !IS_AFFECTED(ch, AFF_SNEAK) )
		  act( "{g$n drops $p.{x", ch, obj, NULL, TO_ROOM );
		act( "{gYou drop $p.{x", ch, obj, NULL, TO_CHAR );

                if (IS_SET(obj->progtypes,IPROG_DROP))
                  (obj->pIndexData->iprogs->drop_prog) (obj,ch);

        	if (IS_OBJ_STAT(obj,ITEM_MELT_DROP))
        	{
		  if ( !IS_AFFECTED(ch, AFF_SNEAK) )
             	    act("{g$p dissolves into smoke.{x",ch,obj,NULL,TO_ROOM);
            	  act("{g$p dissolves into smoke.{x",ch,obj,NULL,TO_CHAR);
            	  extract_obj(obj);
        	}
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
		act( "{cYou are not carrying anything.{x",
		    ch, NULL, arg, TO_CHAR );
	    else
		act( "{cYou are not carrying any $T.{x",
		    ch, NULL, &arg[4], TO_CHAR );
	}
    }

    return;
}



void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    int goldamount;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "{cGive what to whom?{x\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	/* 'give NNNN coins victim' */
	int amount;
	bool silver;

	amount   = atoi(arg1);
	if ( amount <= 0
	|| ( str_cmp( arg2, "coins" ) && str_cmp( arg2, "coin" ) && 
	     str_cmp( arg2, "gold"  ) && str_cmp( arg2, "silver")) )
	{
	    send_to_char( "{cSorry, you can't do that.{x\n\r", ch );
	    return;
	}

	silver = str_cmp(arg2,"gold");

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "{cGive what to whom?{x\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "{cThey aren't here.{x\n\r", ch );
	    return;
	}

	if ( (!silver && ch->gold < amount) || (silver && ch->silver < amount) )
	{
	    send_to_char( "{cYou haven't got that much.{x\n\r", ch );
	    return;
	}

	if (silver)
	{
	    ch->silver		-= amount;
	    victim->silver 	+= amount;
	}
	else
	{
	    ch->gold		-= amount;
	    victim->gold	+= amount;
	}

	sprintf(buf,"{g$n gives you %d %s.{x",amount, silver ? "silver" : "gold");
	act( buf, ch, NULL, victim, TO_VICT    );
	act( "$n gives $N some coins.",  ch, NULL, victim, TO_NOTVICT );
	sprintf(buf,"{gYou give $N %d %s.{x",amount, silver ? "silver" : "gold");
	act( buf, ch, NULL, victim, TO_CHAR    );

       /*
        * Bribe trigger
        */
       if ( IS_NPC(victim) && HAS_TRIGGER( victim, TRIG_BRIBE ) )
           mp_bribe_trigger( victim, ch, silver ? amount : amount * 100);

	goldamount = (silver ? 100 * amount / 100 / 100 
		 	     : 100 * amount);

        if (IS_SET(victim->progtypes,MPROG_BRIBE))
          (victim->pIndexData->moprogs->bribe_prog) (victim,ch,goldamount);

	if (IS_NPC(victim) && IS_SET(victim->act,ACT_IS_CHANGER))
	{
	    int change;

	    change = (silver ? 95 * amount / 100 / 100 
		 	     : 95 * amount);


	    if (!silver && change > victim->silver)
	    	victim->silver += change;

	    if (silver && change > victim->gold)
		victim->gold += change;

	    if (change < 1 && can_see(victim,ch))
	    {
		act(
	"$n tells you 'I'm sorry, you did not give me enough to change.'"
		    ,victim,NULL,ch,TO_VICT);
		ch->reply = victim;
		sprintf(buf,"%d %s %s", 
			amount, silver ? "silver" : "gold",ch->name);
		do_give(victim,buf);
	    }
	    else if (can_see(victim,ch))
	    {
		sprintf(buf,"%d %s %s", 
			change, silver ? "gold" : "silver",ch->name);
		do_give(victim,buf);
		if (silver)
		{
		    sprintf(buf,"%d silver %s", 
			(95 * amount / 100 - change * 100),ch->name);
		    do_give(victim,buf);
		}
		act("{B$n{b tells you '{BThank you, come again.{b'{x",
		    victim,NULL,ch,TO_VICT);
		ch->reply = victim;
	    }
	}
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "{cYou do not have that item.{x\n\r", ch );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
    {
	send_to_char( "{cYou must remove it first.{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }


    if (  (IS_NPC(victim) && victim->pIndexData->pShop != NULL ) &&
	   !IS_SET(victim->progtypes, MPROG_GIVE) )
    {
	act("{B$N{b tells you '{BSorry, you'll have to sell that.{b'{x",
	    ch,NULL,victim,TO_CHAR);
	ch->reply = victim;
	return;
    }

    if ( !can_drop_obj( ch, obj ) 
    &&   !IS_SET(obj->extra_flags, ITEM_MOBGIVE)  
    &&   !IS_NPC(ch) )
    {
	send_to_char( "{cYou can't let go of it.{x\n\r", ch );
	return;
    }

    if ( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
    {
	act( "{c$N has $S hands full.{x", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (get_carry_weight(victim) + get_obj_weight(obj) > can_carry_w( victim ) )
    {
	act( "{c$N can't carry that much weight.{x", ch, NULL, victim,TO_CHAR );
	return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
	act( "{c$N can not see it.{x", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (strstr(obj->pIndexData->area->builders, "Unlinked") &&
	!IS_IMMORTAL(victim)) {
	sprintf(buf,"%s is not approved yet.\n\r",obj->short_descr);
	send_to_char(buf,ch);
	return;
    }

      if ( obj->pIndexData->limit != -1 &&
          IS_NPC(ch))  {
	  act( "You cannot give $p away.", ch,obj,NULL,TO_CHAR);
	  return;
	}


    if ( IS_SET(obj->extra_flags, ITEM_MOBGIVE)  &&
		!IS_NPC(ch) && !IS_IMMORTAL(ch)) {
	send_to_char("You are not allow to give this away.\n\r",ch);
	return;
    }

    if ( ( obj->pIndexData->limit != -1 )  && IS_NPC(ch) &&
    ( obj->pIndexData->count >= obj->pIndexData->limit ) )
	{
	send_to_char("NPC cannot give limit eq which is over.\n\r",ch);
	return;
	}
	
    obj_from_char( obj );
    obj_to_char( obj, victim );
    MOBtrigger = FALSE;
    act( "{g$n gives $p to $N.{x", ch, obj, victim, TO_NOTVICT );
    act( "{g$n gives you $p.{x",   ch, obj, victim, TO_VICT    );
    act( "{gYou give $p to $N.{x", ch, obj, victim, TO_CHAR    );
    MOBtrigger = TRUE;

    /*
     * Give trigger
     */
    if ( IS_NPC(victim) && HAS_TRIGGER( victim, TRIG_GIVE ) )
       mp_give_trigger( victim, ch, obj );

    if (IS_SET(obj->progtypes,IPROG_GIVE))
      (obj->pIndexData->iprogs->give_prog) (obj,ch,victim);

    if (IS_SET(victim->progtypes,MPROG_GIVE))
      (victim->pIndexData->moprogs->give_prog) (victim,ch,obj);

    return;
}


/* for poisoning weapons and food/drink */
void do_envenom(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    AFFECT_DATA af;
    int percent,skill;

    /* find out what */
    if (argument[0] == '\0')
    {
	send_to_char("Envenom what item?\n\r",ch);
	return;
    }

    obj =  get_obj_list(ch,argument,ch->carrying);

    if (obj== NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if ((skill = get_skill(ch,gsn_envenom)) < 1)
    {
	send_to_char("Are you crazy? You'd poison yourself!\n\r",ch);
	return;
    }

    if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
    {
	if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	{
	    act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (number_percent() < skill)  /* success! */
	{
	    act("$n treats $p with deadly poison.",ch,obj,NULL,TO_ROOM);
	    act("You treat $p with deadly poison.",ch,obj,NULL,TO_CHAR);
	    if (!obj->value[3])
	    {
		obj->value[3] = 1;
		check_improve(ch,gsn_envenom,TRUE,4);
	    }
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}

	act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	if (!obj->value[3])
	    check_improve(ch,gsn_envenom,FALSE,4);
	WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	return;
     }

    if (obj->item_type == ITEM_WEAPON)
    {/*
        if (IS_WEAPON_STAT(obj,WEAPON_FLAMING)
        ||  IS_WEAPON_STAT(obj,WEAPON_FROST)
        ||  IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC)
        ||  IS_WEAPON_STAT(obj,WEAPON_SHARP)
        ||  IS_WEAPON_STAT(obj,WEAPON_VORPAL)
        ||  IS_WEAPON_STAT(obj,WEAPON_SHOCKING)
	||  IS_WEAPON_STAT(obj,WEAPON_MANADRAIN)
        ||  IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
        {
            act("You can't seem to envenom $p.",ch,obj,NULL,TO_CHAR);
            return;
        }
      */
	if (obj->value[3] < 0 
	||  attack_table[obj->value[3]].damage == DAM_BASH)
	{
	    send_to_char("You can only envenom edged weapons.\n\r",ch);
	    return;
	}

        if (IS_WEAPON_STAT(obj,WEAPON_POISON))
        {
            act("$p is already envenomed.",ch,obj,NULL,TO_CHAR);
            return;
        }

	percent = number_percent();
	if (percent < skill)
	{
 
            af.where     = TO_WEAPON;
            af.type      = gsn_poison;
            af.level     = ch->level * percent / 100;
            af.duration  = ch->level * percent / 100;
            af.location  = 0;
            af.modifier  = 0;
            af.bitvector = WEAPON_POISON;
            affect_to_obj(obj,&af);

	    if ( !IS_AFFECTED( ch, AFF_SNEAK ) ) 
              act("$n coats $p with deadly venom.",ch,obj,NULL,TO_ROOM);
	    act("You coat $p with venom.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,TRUE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
            return;
        }
	else
	{
	    act("You fail to envenom $p.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,FALSE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}
    }
 
    act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
    return;
}

void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Fill what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    found = FALSE;
    for ( fountain = ch->in_room->contents; fountain != NULL;
	fountain = fountain->next_content )
    {
	if ( fountain->item_type == ITEM_FOUNTAIN )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
    {
	send_to_char( "There is no fountain here!\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "You can't fill that.\n\r", ch );
	return;
    }

    if ( obj->value[1] != 0 && obj->value[2] != fountain->value[2] )
    {
	send_to_char( "There is already another liquid in it.\n\r", ch );
	return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
	send_to_char( "Your container is full.\n\r", ch );
	return;
    }

    sprintf(buf,"You fill $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act( buf, ch, obj,fountain, TO_CHAR );
    sprintf(buf,"$n fills $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act(buf,ch,obj,fountain,TO_ROOM);
    obj->value[2] = fountain->value[2];
    obj->value[1] = obj->value[0];
    check_look( obj );

}

void do_pour (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_STRING_LENGTH],buf[MAX_STRING_LENGTH];
    OBJ_DATA *out, *in;
    CHAR_DATA *vch = NULL;
    int amount;

    argument = one_argument(argument,arg);
    
    if (arg[0] == '\0' || argument[0] == '\0')
    {
	send_to_char("Pour what into what?\n\r",ch);
	return;
    }
    

    if ((out = get_obj_carry(ch,arg)) == NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if (out->item_type != ITEM_DRINK_CON)
    {
	send_to_char("That's not a drink container.\n\r",ch);
	return;
    }

    if (!str_cmp(argument,"out"))
    {
	if (out->value[1] == 0)
	{
	    send_to_char("It's already empty.\n\r",ch);
	    return;
	}

	out->value[1] = 0;
	out->value[3] = 0;
	sprintf(buf,"You invert $p, spilling %s in to the water.",
		liq_table[out->value[2]].liq_name);
	act(buf,ch,out,NULL,TO_CHAR);
	
	sprintf(buf,"$n inverts $p, spilling %s in to the water.",
		liq_table[out->value[2]].liq_name);
	act(buf,ch,out,NULL,TO_ROOM);
	return;
    }

    if ((in = get_obj_here(ch,argument)) == NULL)
    {
	vch = get_char_room(ch,argument);

	if (vch == NULL)
	{
	    send_to_char("Pour into what?\n\r",ch);
	    return;
	}

	in = get_eq_char(vch,WEAR_HOLD);

	if (in == NULL)
	{
	    send_to_char("They aren't holding anything.",ch);
 	    return;
	}
    }

    if (in->item_type != ITEM_DRINK_CON)
    {
	send_to_char("You can only pour into other drink containers.\n\r",ch);
	return;
    }
    
    if (in == out)
    {
	send_to_char("You cannot change the laws of physics!\n\r",ch);
	return;
    }

    if (in->value[1] != 0 && in->value[2] != out->value[2])
    {
	send_to_char("They don't hold the same liquid.\n\r",ch);
	return;
    }

    if (out->value[1] == 0)
    {
	act("There's nothing in $p to pour.",ch,out,NULL,TO_CHAR);
	return;
    }

    if (in->value[1] >= in->value[0])
    {
	act("$p is already filled to the top.",ch,in,NULL,TO_CHAR);
	return;
    }

    amount = UMIN(out->value[1],in->value[0] - in->value[1]);

    in->value[1] += amount;
    out->value[1] -= amount;
    in->value[2] = out->value[2];
    
    if (vch == NULL)
    {
    	sprintf(buf,"You pour %s from $p into $P.",
	    liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,in,TO_CHAR);
    	sprintf(buf,"$n pours %s from $p into $P.",
	    liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,in,TO_ROOM);
    }
    else
    {
        sprintf(buf,"You pour some %s for $N.",
            liq_table[out->value[2]].liq_name);
        act(buf,ch,NULL,vch,TO_CHAR);
	sprintf(buf,"$n pours you some %s.",
	    liq_table[out->value[2]].liq_name);
	act(buf,ch,NULL,vch,TO_VICT);
        sprintf(buf,"$n pours some %s for $N.",
            liq_table[out->value[2]].liq_name);
        act(buf,ch,NULL,vch,TO_NOTVICT);
    }
    check_look( in );
    check_look( out );

}

void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int amount;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->item_type == ITEM_FOUNTAIN )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char( "{cDrink what?{x\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( "{cYou can't find it.{x\n\r", ch );
	    return;
	}
    }

    if ( ch->fighting != NULL )
    {
	send_to_char( "{cYou are too busy to fighting drink anything.{x\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 20 )
    {
	send_to_char( "{cYou fail to reach your mouth.  *Hic*{x\n\r", ch );
	return;
    }

    switch ( obj->item_type )
    {
    default:
	send_to_char( "You can't drink from that.\n\r", ch );
	return;

    case ITEM_FOUNTAIN:
        if ( ( liquid = obj->value[2] )  < 0 )
        {
            bug( "Do_drink: bad liquid number %d.", liquid );
            liquid = obj->value[2] = 0;
        }
	amount = liq_table[liquid].liq_affect[4] * 3;
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] )  < 0 )
	{
	    bug( "Do_drink: bad liquid number %d.", liquid );
	    liquid = obj->value[2] = 0;
	}

        amount = liq_table[liquid].liq_affect[4];
        amount = UMIN(amount, obj->value[1]);
	break;
     }
    if (!IS_NPC(ch) && !IS_IMMORTAL(ch) 
    &&  ch->pcdata->condition[COND_FULL] > 80)
    {
	send_to_char("You're too full to drink more.\n\r",ch);
	return;
    }

    act( "$n drinks $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_ROOM );
    act( "You drink $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_CHAR );

    if ( ch->fighting != NULL )
      WAIT_STATE( ch, 3 * PULSE_VIOLENCE );

    gain_condition( ch, COND_DRUNK,
	amount * liq_table[liquid].liq_affect[COND_DRUNK] / 10 );
				  /* ::changed from 36?!-^ */
    gain_condition( ch, COND_FULL,
	amount * liq_table[liquid].liq_affect[COND_FULL] / 2 );
    gain_condition( ch, COND_THIRST,
	amount * liq_table[liquid].liq_affect[COND_THIRST] / 5 );
    gain_condition(ch, COND_HUNGER,
	amount * liq_table[liquid].liq_affect[COND_HUNGER] / 1 );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	send_to_char( "You feel drunk.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   > 60 )
	send_to_char( "You are full.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > 60 )
	send_to_char( "Your thirst is quenched.\n\r", ch );
	
    if ( obj->value[3] != 0 )
    {
	/* The drink was poisoned ! */
	AFFECT_DATA af;

	act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You choke and gag.\n\r", ch );
	af.where     = TO_AFFECTS;
	af.type      = gsn_poison;
	af.level     = number_fuzzy(amount); 
	af.duration  = 3 * amount;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_POISON;
	affect_join( ch, &af );
    }
	
    if (obj->value[0] > 0)
        obj->value[1] -= amount;
    check_look( obj );
    return;
}



void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;


    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "{cEat what?{x\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "{cYou do not have that item.{x\n\r", ch );
	return;
    }

    if ( !IS_IMMORTAL(ch) )
    {
	if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL )
	{
	    send_to_char( "{cThat's not edible.{x\n\r", ch );
	    return;
	}

	if ( ch->fighting != NULL )
	{
	    send_to_char(
		"{cYou are too busy to fighting to worry about food.{x\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL] > 80 )
	{   
	    send_to_char( "{cYou are too full to eat more.{x\n\r", ch );
	    return;
	}
    }

    act( "{g$n eats $p.{x",  ch, obj, NULL, TO_ROOM );
    act( "{gYou eat $p.{x", ch, obj, NULL, TO_CHAR );
    if ( ch->fighting != NULL )
      WAIT_STATE( ch, 3 * PULSE_VIOLENCE );

    switch ( obj->item_type )
    {

    case ITEM_FOOD:
	if ( !IS_NPC(ch) )
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_HUNGER];
	    gain_condition( ch, COND_FULL, obj->value[0]*2 );
	    gain_condition( ch, COND_HUNGER, obj->value[1]*2 );
	    if ( condition == 0 && ch->pcdata->condition[COND_HUNGER] > 0 )
		send_to_char( "{cYou are no longer hungry.{x\n\r", ch );
	    else if ( ch->pcdata->condition[COND_FULL] > 60 )
		send_to_char( "{cYou are full.{x\n\r", ch );
	}

	if ( obj->value[3] != 0 )
	{
	    /* The food was poisoned! */
	    AFFECT_DATA af;

	    act( "{g$n chokes and gags.{x", ch, 0, 0, TO_ROOM );
	    send_to_char( "{gYou choke and gag.{x\n\r", ch );

	    af.where	 = TO_AFFECTS;
	    af.type      = gsn_poison;
	    af.level 	 = number_fuzzy(obj->value[0]);
	    af.duration  = 2 * obj->value[0];
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	break;

    case ITEM_PILL:
	obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[4], obj->value[0], ch, ch, NULL );
	break;
    }

    extract_obj( obj );
    return;
}



/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	return TRUE;

    if ( !fReplace )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
	act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }
/* disabled for now...waiting to get extended....
    if ((IS_OBJ_STAT (obj, ITEM_DIMENSION)) &&  
       (!is_affected(ch, gsn_duo_dimension)))
    {
        send_to_char("You are not in the same dimension with that item.\n\r", ch);
        return;
    }
    
      if ((!IS_OBJ_STAT (obj, ITEM_DIMENSION)) &&  
       (is_affected(ch, gsn_duo_dimension)))
    {
        send_to_char("You are not in the same dimension with that item.\n\r", ch);
        return;
    }
*/
    unequip_char( ch, obj );
    act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
    act( "You stop using $p.", ch, obj, NULL, TO_CHAR );
    return 2;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
  char buf[MAX_STRING_LENGTH];
  int max_count = 2;
  int hand_count= 0;
  int donus=0;
  int wearnow,racenow;

  --max_count;
  if (ch->level < 10 && obj->pIndexData->limit != -1)
  {
    send_to_char("The use of limited equipment is allowed only when you are level 10 or above.\n\r",ch);
    return;
  }
/* Restriction */
  if (ch->level+20 < obj->level &&  obj->pIndexData->limit != -1)
	{
        sprintf( buf, "You must be level %d to use this object.\n\r",obj->level-20 );
        send_to_char( buf, ch );
        act( "{g$n tries to use $p, but is too inexperienced.", ch, obj, NULL, TO_ROOM );
	return;
	}
/*
 * Class Restriction for Armors 
 * Coded by Takeda
 * 2/26/00
 */
      wearnow = 0;
   if((ch->class == CLASS_WAR) &&
                (!IS_SET(obj->class_flags,CLASS_WARRIOR)))
        wearnow = 1;
   if((ch->class == CLASS_MER) &&
                (!IS_SET(obj->class_flags,CLASS_MERCENARY)))
        wearnow = 1;
   if((ch->class == CLASS_PDN) &&
                (!IS_SET(obj->class_flags,CLASS_PALADIN)))
        wearnow = 1;
   if((ch->class == CLASS_BKT) &&
                (!IS_SET(obj->class_flags,CLASS_BLACK_KNIGHT)))
        wearnow = 1;
   if((ch->class == CLASS_THI) &&
                (!IS_SET(obj->class_flags,CLASS_THIEF)))
        wearnow = 1;
   if((ch->class == CLASS_INQ) &&
                (!IS_SET(obj->class_flags,CLASS_INQUISITOR)))
        wearnow = 1;
   if((ch->class == CLASS_CLE) &&
                (!IS_SET(obj->class_flags,CLASS_CLERIC)))
        wearnow = 1;
   if((ch->class == CLASS_PRO) &&
                (!IS_SET(obj->class_flags,CLASS_PRODIGY)))
        wearnow = 1;
   if((ch->class == CLASS_MAG) &&
                (!IS_SET(obj->class_flags,CLASS_MAGE)))
        wearnow = 1;
   if((ch->class == CLASS_RAN) &&
               (!IS_SET(obj->class_flags,CLASS_RANGER)))
        wearnow = 1;
   if((ch->class == CLASS_ASN) &&
                (!IS_SET(obj->class_flags,CLASS_ASSASSIN)))
        wearnow = 1;
   if((ch->class == CLASS_BRD) &&
                (!IS_SET(obj->class_flags,CLASS_BARD)))
        wearnow = 1;
   if((ch->class == CLASS_MAL) &&
                (!IS_SET(obj->class_flags,CLASS_MALIGNER)))
        wearnow = 1;
   if((ch->class == CLASS_ALC) &&
                (!IS_SET(obj->class_flags,CLASS_ALCHEMIST)))
        wearnow = 1;
   if((ch->class == CLASS_DRD) &&
                (!IS_SET(obj->class_flags,CLASS_DRUID)))
        wearnow = 1;
   if((ch->class == CLASS_MNK) && ((!IS_SET(obj->class_flags,CLASS_MONK))||
			(obj->item_type == ITEM_WEAPON)))
        wearnow = 1;

   if(wearnow == 1) {
	sprintf(buf,"%s hinders your abilities.\n",
		obj->short_descr);
        send_to_char(buf,ch);
        return;
        }
/*
 * Race Restriction for Armors 
 * Coded by Takeda
 * 2/26/00
 */ 
       racenow = 0;
   if((ch->race == race_lookup("human")) &&
                (!IS_SET(obj->race_flags,RACE_HUMAN)))
        racenow = 1;
   if((ch->race == race_lookup("elf")) &&
                (!IS_SET(obj->race_flags,RACE_ELF)))
        racenow = 1;
   if((ch->race == race_lookup("drow")) &&
                (!IS_SET(obj->race_flags,RACE_DROW)))
        racenow = 1;
   if((ch->race == race_lookup("dwarf")) &&
                (!IS_SET(obj->race_flags,RACE_DWARF)))
        racenow = 1;
   if((ch->race == race_lookup("duergar")) &&
                (!IS_SET(obj->race_flags,RACE_DUERGAR)))
        racenow = 1;
   if((ch->race == race_lookup("pixie")) &&
                (!IS_SET(obj->race_flags,RACE_PIXIE)))
        racenow = 1;
   if((ch->race == race_lookup("gargoyle")) &&
                (!IS_SET(obj->race_flags,RACE_GARGOYLE)))
        racenow = 1;
   if((ch->race == race_lookup("draconian")) &&
                (!IS_SET(obj->race_flags,RACE_DRACONIAN)))
        racenow = 1;
   if((ch->race == race_lookup("arial")) &&
                (!IS_SET(obj->race_flags,RACE_ARIAL)))
        racenow = 1;
   if((ch->race == race_lookup("felar")) &&
                (!IS_SET(obj->race_flags,RACE_FELAR)))
        racenow = 1;
   if((ch->race == race_lookup("merman")) &&
                (!IS_SET(obj->race_flags,RACE_MERMAN)))
        racenow = 1;
   if((ch->race == race_lookup("giant")) &&
                (!IS_SET(obj->race_flags,RACE_GIANT)))
        racenow = 1;
   if((ch->race == race_lookup("troll")) &&
                (!IS_SET(obj->race_flags,RACE_TROLL)))
        racenow = 1;
   if((ch->race == race_lookup("ogre")) &&
                (!IS_SET(obj->race_flags,RACE_OGRE)))
        racenow = 1;
   if((ch->race == race_lookup("orc")) &&
                (!IS_SET(obj->race_flags,RACE_ORC)))
        racenow = 1;
   if((ch->race == race_lookup("darnok")) &&
                (!IS_SET(obj->race_flags,RACE_DARNOK)))
        racenow = 1;
   if((ch->race == race_lookup("illithid")) &&
                (!IS_SET(obj->race_flags,RACE_ILLITHID)))
        racenow = 1;
   if(racenow == 1) {
	sprintf(buf,"Your race hinders you from wearing %s.\n",
		obj->short_descr);
        send_to_char(buf,ch);
	return;
   }
   if (get_eq_char(ch,WEAR_LIGHT) != NULL) hand_count++;
   if (get_eq_char(ch,WEAR_SHIELD)!= NULL) hand_count++; 
   if (get_eq_char(ch,WEAR_HOLD)  != NULL) hand_count++;  
   if (get_eq_char(ch,WEAR_WIELD) != NULL) hand_count++;
   if (get_eq_char(ch,WEAR_SECONDARY) != NULL) hand_count++;	

    if ( obj->item_type == ITEM_LIGHT )
    {
	OBJ_DATA *weapon;

	if ( !(donus=remove_obj( ch, WEAR_LIGHT, fReplace )) ) 
	    return;
       else if (donus==2) hand_count--;     
   if (hand_count > max_count) {
        act("You don't have free hand to hold something.",ch,obj,NULL,TO_CHAR);
        return;

    }

	weapon = get_eq_char(ch,WEAR_WIELD);

/* Modified by Takeda
 * it's null then it would be false, if true then it would be true
 */
	if ((get_eq_char(ch,WEAR_WIELD) != NULL) && ch->size < SIZE_LARGE 
	&& IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
	{
	    send_to_char("Your hands are tied up with your weapon!\n\r",ch);
	    return;
	}


	act( "$n lights $p and holds it.", ch, obj, NULL, TO_ROOM );
	act( "You light $p and hold it.",  ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LIGHT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
    {
	if ( get_eq_char( ch, WEAR_FINGER_L ) != NULL
	&&   get_eq_char( ch, WEAR_FINGER_R ) != NULL
	&&   !remove_obj( ch, WEAR_FINGER_L, fReplace )
	&&   !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_FINGER_L ) == NULL )
	{
	    act( "{g$n wears $p on $s left finger.{x",    ch, obj, NULL,TO_ROOM );
 	    act( "{gYou wear $p on your left finger.{x",  ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_L );
	    return;
	}

	if ( get_eq_char( ch, WEAR_FINGER_R ) == NULL )
	{
	    act( "{g$n wears $p on $s right finger.{x",   ch, obj, NULL, TO_ROOM );
	    act( "{gYou wear $p on your right finger.{x", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_R );
	    return;
	}

	bug( "Wear_obj: no free finger.", 0 );
	send_to_char( "{cYou already wear two rings.{x\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) )
    {
	if ( get_eq_char( ch, WEAR_NECK_1 ) != NULL
	&&   get_eq_char( ch, WEAR_NECK_2 ) != NULL
	&&   !remove_obj( ch, WEAR_NECK_1, fReplace )
	&&   !remove_obj( ch, WEAR_NECK_2, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_NECK_1 ) == NULL )
	{
	    act( "{g$n wears $p around $s neck.{x",   ch, obj, NULL, TO_ROOM);
	    act( "{gYou wear $p around your neck.{x", ch, obj, NULL,TO_CHAR);
	    equip_char( ch, obj, WEAR_NECK_1 );
	    return;
	}

	if ( get_eq_char( ch, WEAR_NECK_2 ) == NULL )
	{
	    act( "{g$n wears $p around $s neck.{x",   ch, obj, NULL,TO_ROOM );
	    act( "{gYou wear $p around your neck.{x", ch, obj, NULL, TO_CHAR);
	    equip_char( ch, obj, WEAR_NECK_2 );
	    return;
	}

	bug( "Wear_obj: no free neck.", 0 );
	send_to_char( "{cYou already wear two neck items.{x\n\r", ch );
	return;
    }
/*  adds ornament code */
    if ( CAN_WEAR( obj, ITEM_WEAR_ORNAMENT ) )
    {
	if ( get_eq_char( ch, WEAR_ORNAMENT_1 ) != NULL
	&&   get_eq_char( ch, WEAR_ORNAMENT_2 ) != NULL
	&&   !remove_obj( ch, WEAR_ORNAMENT_1, fReplace )
	&&   !remove_obj( ch, WEAR_ORNAMENT_2, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_ORNAMENT_1 ) == NULL )
	{
	    act( "{g$n wears $p.{x",   ch, obj, NULL, TO_ROOM );
	    act( "{gYou wear $p.{x", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_ORNAMENT_1 );
	    return;
	}

	if ( get_eq_char( ch, WEAR_ORNAMENT_2 ) == NULL )
	{
	    act( "{g$n wears $p.{x",   ch, obj, NULL, TO_ROOM );
	    act( "{gYou wear $p.{x", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_ORNAMENT_2 );
	    return;
	}

	bug( "Wear_obj: no free neck.", 0 );
	send_to_char( "{cYou already wear enough ornamentation.{x\n\r", ch );
	return;
    }
/*  end */

    if ( CAN_WEAR( obj, ITEM_WEAR_BODY ) )
    {
	if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
	    return;
	act( "{g$n wears $p on $s torso.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p on your torso.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_BODY );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {
	if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
	    return;
	act( "{g$n wears $p on $s head.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p on your head.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HEAD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
	if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
	    return;
	act( "{g$n wears $p on $s legs.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p on your legs.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LEGS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {
	if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
	    return;
	act( "{g$n wears $p on $s feet.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p on your feet.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FEET );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
	if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
	    return;
	act( "{g$n wears $p on $s hands.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p on your hands.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HANDS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
	if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
	    return;
	act( "{g$n wears $p on $s arms.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p on your arms.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ARMS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
	if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
	    return;
	act( "{g$n wears $p about $s torso.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p about your torso.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ABOUT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
	if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
	    return;
	act( "{g$n wears $p about $s waist.{x",   ch, obj, NULL, TO_ROOM );
	act( "{gYou wear $p about your waist.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_WAIST );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
    {
	if ( get_eq_char( ch, WEAR_WRIST_L ) != NULL
	&&   get_eq_char( ch, WEAR_WRIST_R ) != NULL
	&&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
	&&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_WRIST_L ) == NULL )
	{
	    act( "{g$n wears $p around $s left wrist.{x",
		ch, obj, NULL, TO_ROOM );
	    act( "{gYou wear $p around your left wrist.{x",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_L );
	    return;
	}

	if ( get_eq_char( ch, WEAR_WRIST_R ) == NULL )
	{
	    act( "{g$n wears $p around $s right wrist.{x",
		ch, obj, NULL, TO_ROOM );
	    act( "{gYou wear $p around your right wrist.{x",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_R );
	    return;
	}

	bug( "Wear_obj: no free wrist.", 0 );
	send_to_char( "{cYou already wear two wrist items.{x\n\r", ch );
	return;
    }



    if ( CAN_WEAR( obj, ITEM_WEAR_SHIELD ) )
    {
	OBJ_DATA *weapon;
	if ( !(donus=remove_obj( ch, WEAR_SHIELD, fReplace )) )
	    return;
	else if (donus==2) hand_count--;
	   if (hand_count > max_count) 
	{
        act("{cYou don't have free hand to use shield.{x",ch,obj,NULL,TO_CHAR);
        return;
         }
   if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
        {
            send_to_char ("{cYou need to wield a primary before wielding a secondary!!{x\n\r",ch);
            return;
        }
/*	
	if ( weapon->value[0] == WEAPON_ARROW)
	  send_to_char("{cYou can't wield arrow!{x\n\r",ch);
	
*/	
	weapon = get_eq_char(ch,WEAR_WIELD);
	
	if (weapon != NULL && ch->size < SIZE_LARGE 
	&&  IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
	{
	    send_to_char("{cYour hands are tied up with your weapon!{x\n\r",ch);
	    return;
	}

        if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
        {
            send_to_char ("{cYou cannot use a shield while using two weapons.{x\n\r",ch);
            return;
        }

        if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
            return;
        act( "{g$n wears $p as a shield.{x", ch, obj, NULL, TO_ROOM );
        act( "{gYou wear $p as a shield.{x", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_SHIELD );
        return;
    }


/*
    if ( CAN_WEAR( obj, ITEM_WIELD ) )
*/  if ( CAN_WEAR( obj, ITEM_WIELD ) )  
{
	int sn,skill;

	if ( !(donus=remove_obj( ch, WEAR_WIELD, fReplace ) ))
	    return;
        else if (donus==2) hand_count--;
   if (hand_count > max_count) {
        act("{cYou don't have free hand to wield a weapon.{x",ch,obj,NULL,TO_CHAR);
        return;

    }
	if ( !IS_NPC(ch) 
	&& get_obj_weight(obj) > (str_app[get_curr_stat(ch,STAT_STR)].wield  
		* 10))
	{
	    send_to_char( "{cIt is too heavy for you to wield.{x\n\r", ch );
	    return;
	}

	if (!IS_NPC(ch) && ch->size < SIZE_LARGE 
	&&  IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS)
 	&&  get_eq_char(ch,WEAR_SHIELD) != NULL)
	{
	    send_to_char("{cYou need two hands free for that weapon.{x\n\r",ch);
	    return;
	}
    if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
    {
        send_to_char ("{cYou need to wield a primary weapon, before using a secondary one.{x\n\r",ch);
        return;
    }
    
	act( "{g$n wields $p.{x", ch, obj, NULL, TO_ROOM );
	act( "{gYou wield $p.{x", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_WIELD );

        sn = get_weapon_sn(ch);

	if (sn == gsn_hand_to_hand)
	   return;

        skill = get_weapon_skill(ch,sn);
 
        if (skill >= 100)
            act("{G$p feels like a part of you!{x",ch,obj,NULL,TO_CHAR);
        else if (skill > 85)
            act("{gYou feel quite confident with $p.{x",ch,obj,NULL,TO_CHAR);
        else if (skill > 70)
            act("{gYou are skilled with $p.{x",ch,obj,NULL,TO_CHAR);
        else if (skill > 50)
            act("{gYour skill with $p is adequate.{x",ch,obj,NULL,TO_CHAR);
        else if (skill > 25)
            act("{g$p feels a little clumsy in your hands.{x",ch,obj,NULL,TO_CHAR);
        else if (skill > 1)
            act("{gYou fumble and almost drop $p.{x",ch,obj,NULL,TO_CHAR);
        else
            act("{gYou don't even know which end is up on $p.{x",
                ch,obj,NULL,TO_CHAR);

	return;
    }


    if ( CAN_WEAR( obj, ITEM_HOLD ) )
    {
	OBJ_DATA *weapon;

	if ( !(donus=remove_obj( ch, WEAR_HOLD, fReplace ) ))
	    return;
	else if (donus==2) hand_count--;
   if (hand_count > max_count) {
        act("{cYou don't have free hand to hold that item.{x",ch,obj,NULL,TO_CHAR);
        return;
        }
	weapon = get_eq_char(ch,WEAR_WIELD);
	if (weapon != NULL && ch->size < SIZE_LARGE 
	&&  IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
	{
	    send_to_char("{cYour hands are tied up with your weapon!{x\n\r",ch);
	    return;
	}


        if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
        {
            send_to_char ("{cYou cannot hold an item while using two weapons.{x\n\r",ch);
            return;
        }

        if ( !remove_obj( ch, WEAR_HOLD, fReplace ) )
            return;
        act( "{g$n holds $p in $s hands.{x",   ch, obj, NULL, TO_ROOM );
        act( "{gYou hold $p in your hands.{x", ch, obj, NULL, TO_CHAR );
        equip_char( ch, obj, WEAR_HOLD );
        return;
    }


    if ( CAN_WEAR(obj,ITEM_WEAR_FLOAT) )
    {
	if (!remove_obj(ch,WEAR_FLOAT, fReplace) )
	    return;
	act("{g$n releases $p to float next to $m.{x",ch,obj,NULL,TO_ROOM);
	act("{gYou release $p and it floats next to you.{x",ch,obj,NULL,TO_CHAR);
	equip_char(ch,obj,WEAR_FLOAT);
	return;
    }

    if ( fReplace )
	send_to_char( "{cYou can't wear, wield, or hold that.{x\n\r", ch );

    return;
}



void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cWear, wield, or hold what?{x\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
		wear_obj( ch, obj, FALSE );
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "{cYou do not have that item.{x\n\r", ch );
	    return;
	}

	wear_obj( ch, obj, TRUE );
    }

    return;
}

/*  adds dual wield code */

void do_second (CHAR_DATA *ch, char *argument)
/* wear object as a secondary weapon */
{
    OBJ_DATA *obj,*weapon;
    int sn,skill,wearnow,racenow;
    char buf[MAX_STRING_LENGTH];		
    int hand_count=0;            // too see if the char has a hand avaliable. Derkec		     
/*	removed with level restriction, because that is what used it */

    if ( (get_skill(ch,gsn_dual_wield) == 0) && (ch->kingdom != KINGDOM_ARKADIA))
    {
	send_to_char("{cHuh?{x\n\r",ch);
	return;
    }
/*
    if(ch->class == CLASS_MNK)
     {
      send_to_char("{cMonks don't use weapons?{x\n\r",ch);
      return;
     } 
*/
    if (argument[0] == '\0') /* empty */
    {
        send_to_char ("{cWear which weapon in your off-hand?{x\n\r",ch);
        return;
    }
    obj = get_obj_carry (ch, argument); /* find the obj withing ch's inventory */

    if (obj == NULL)
    {
        send_to_char ("{cYou have no such thing.{x\n\r",ch);
        return;
    }                                       /*Zardic edit 16/03/00*/
/*  if ((obj->item_type != ITEM_WEAPON) || ( obj->value[0] == WEAPON_ARROW))
      {
        send_to_char("{cThat is not a weapon.{x\n\r",
                     ch);
        return;
      }
*/if (obj->item_type != ITEM_WEAPON)
      {
        send_to_char("{cThat is not a weapon.{x\n\r",
                     ch);
        return;
      }

  if (!CAN_WEAR(obj, ITEM_WIELD))
     {
        send_to_char("{cThat is not a weapon.{x\n\r",ch);
        return;
      }   
 /*
 * Class Restriction for Armors
 * Coded by Takeda
 * 2/26/00
 */
       wearnow = 0;
   if((ch->class == CLASS_WAR) &&
		(!IS_SET(obj->class_flags,CLASS_WARRIOR)))
	wearnow = 1;
   if((ch->class == CLASS_MER) &&
		(!IS_SET(obj->class_flags,CLASS_MERCENARY)))
	wearnow = 1;
   if((ch->class == CLASS_PDN) &&
		(!IS_SET(obj->class_flags,CLASS_PALADIN)))
	wearnow = 1;
   if((ch->class == CLASS_BKT) &&
		(!IS_SET(obj->class_flags,CLASS_BLACK_KNIGHT)))
	wearnow = 1;
   if((ch->class == CLASS_THI) &&
		(!IS_SET(obj->class_flags,CLASS_THIEF)))
	wearnow = 1;
   if((ch->class == CLASS_INQ) &&
		(!IS_SET(obj->class_flags,CLASS_INQUISITOR)))
	wearnow = 1;
   if((ch->class == CLASS_CLE) &&
		(!IS_SET(obj->class_flags,CLASS_CLERIC)))
	wearnow = 1;
   if((ch->class == CLASS_PRO) &&
		(!IS_SET(obj->class_flags,CLASS_PRODIGY)))
	wearnow = 1;
   if((ch->class == CLASS_MAG) &&
		(!IS_SET(obj->class_flags,CLASS_MAGE)))
	wearnow = 1;
   if((ch->class == CLASS_RAN) &&
		(!IS_SET(obj->class_flags,CLASS_RANGER)))
	wearnow = 1;
   if((ch->class == CLASS_ASN) &&
		(!IS_SET(obj->class_flags,CLASS_ASSASSIN)))
	wearnow = 1;
   if((ch->class == CLASS_BRD) &&
		(!IS_SET(obj->class_flags,CLASS_BARD)))
	wearnow = 1;
   if((ch->class == CLASS_MAL) &&
		(!IS_SET(obj->class_flags,CLASS_MALIGNER)))
	wearnow = 1;
   if((ch->class == CLASS_ALC) &&
		(!IS_SET(obj->class_flags,CLASS_ALCHEMIST)))
	wearnow = 1;
   if((ch->class == CLASS_DRD) &&
		(!IS_SET(obj->class_flags,CLASS_DRUID)))
	wearnow = 1;
   if((ch->class == CLASS_MNK) &&
		(!IS_SET(obj->class_flags,CLASS_MONK)))
	wearnow = 1;
   
   if(wearnow == 1) {
	sprintf(buf,"%s hinders your abilities.\n",
		obj->short_descr);
        send_to_char(buf,ch);
	}

/*
 * Race Restriction for Armors
 * Coded by Takeda
 * 2/26/00
 */
       racenow = 0;
   if((ch->race == race_lookup("human")) &&
                (!IS_SET(obj->race_flags,RACE_HUMAN)))
        racenow = 1;
   if((ch->race == race_lookup("elf")) &&
                (!IS_SET(obj->race_flags,RACE_ELF)))
        racenow = 1;
   if((ch->race == race_lookup("drow")) &&
                (!IS_SET(obj->race_flags,RACE_DROW)))
        racenow = 1;
   if((ch->race == race_lookup("dwarf")) &&
               (!IS_SET(obj->race_flags,RACE_DWARF)))
        racenow = 1;
   if((ch->race == race_lookup("duergar")) &&
                (!IS_SET(obj->race_flags,RACE_DUERGAR)))
        racenow = 1;
   if((ch->race == race_lookup("pixie")) &&
                (!IS_SET(obj->race_flags,RACE_PIXIE)))
        racenow = 1;
   if((ch->race == race_lookup("gargoyle")) &&
                (!IS_SET(obj->race_flags,RACE_GARGOYLE)))
        racenow = 1;
   if((ch->race == race_lookup("draconian")) &&
                (!IS_SET(obj->race_flags,RACE_DRACONIAN)))
        racenow = 1;
   if((ch->race == race_lookup("arial")) &&
                (!IS_SET(obj->race_flags,RACE_ARIAL)))
        racenow = 1;
   if((ch->race == race_lookup("felar")) &&
                (!IS_SET(obj->race_flags,RACE_FELAR)))
        racenow = 1;
   if((ch->race == race_lookup("merman")) &&
                (!IS_SET(obj->race_flags,RACE_MERMAN)))
        racenow = 1;
   if((ch->race == race_lookup("giant")) &&
                (!IS_SET(obj->race_flags,RACE_GIANT)))
        racenow = 1;
   if((ch->race == race_lookup("troll")) &&
                (!IS_SET(obj->race_flags,RACE_TROLL)))
        racenow = 1;
   if((ch->race == race_lookup("ogre")) &&
                (!IS_SET(obj->race_flags,RACE_OGRE)))
        racenow = 1;
   if((ch->race == race_lookup("orc")) &&
                (!IS_SET(obj->race_flags,RACE_ORC)))
       racenow = 1;
   if((ch->race == race_lookup("darnok")) &&
                (!IS_SET(obj->race_flags,RACE_DARNOK)))
        racenow = 1;
   if((ch->race == race_lookup("illithid")) &&
                (!IS_SET(obj->race_flags,RACE_ILLITHID)))
        racenow = 1;
   if(racenow == 1) {
	sprintf(buf,"Your race hinders you from wearing %s.\n",
		obj->short_descr);
	send_to_char(buf,ch);
        return;
   }

    /* check if the char is using a shield or a held weapon */
   if (get_eq_char(ch,WEAR_LIGHT) != NULL) hand_count++; // borrowed from do_wear
   if (get_eq_char(ch,WEAR_SHIELD)!= NULL) hand_count++; 
   if (get_eq_char(ch,WEAR_HOLD)  != NULL) hand_count++;  

    if ( hand_count > 0)
    {
        send_to_char ("{cYou cannot use a secondary weapon while using a shield or holding an item.{x\n\r",ch);
        return;
    }


  if (get_eq_char (ch, WEAR_WIELD) == NULL)
    {
        send_to_char ("{cYou need to wield a primary weapon, before using a secondary one.{x\n\r",ch);
        return;
    }

/* removed to allow using eq at any level
    if ( ch->level < obj->level )
    {
        sprintf( buf, "You must be level %d to use this object.\n\r", obj->level );
        send_to_char( buf, ch );
        act( "{g$n tries to use $p, but is too inexperienced.", ch, obj, NULL, TO_ROOM );
        return;
    }
*/
  if (ch->level+10 < obj->level &&  obj->pIndexData->limit != -1)
	{
        sprintf( buf, "You must be level %d to use this object.\n\r",obj->level-10 );
        send_to_char( buf, ch );
        act( "{g$n tries to use $p, but is too inexperienced.", ch, obj, NULL, TO_ROOM );
	return;
	}

/* check that the character is using a first weapon at all */

 if(ch->kingdom != KINGDOM_ARKADIA)
  {
/* check for str - secondary weapons have to be lighter */
    if ( get_obj_weight( obj ) > (str_app[get_curr_stat(ch,STAT_STR)].wield / 2) )
    {
        send_to_char( "{cThis weapon is too heavy to be used as a secondary weapon by you.{x\n\r", ch );
        return;
    }

/* check if the secondary weapon is at least half as light as the primary weapon */
    if ( (get_obj_weight (obj)*2) > get_obj_weight(get_eq_char(ch,WEAR_WIELD)) )
    {
        send_to_char ("{cYour secondary weapon has to be considerably lighter than the primary one.{x\n\r",ch);
        return;
    }
  }
    if (IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS)) {
	send_to_char("{cYou can't wield a two-handed weapon your second hand.\n",ch);
	return;
    }
	weapon = get_eq_char(ch,WEAR_WIELD);
	if (weapon != NULL && IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS)) {
	send_to_char("{cYou are wielding a two-handed weapon on your primary.\n",ch);
	return; 
	}

/* at last - the char uses the weapon */

    if (!remove_obj(ch, WEAR_SECONDARY, TRUE)) /* remove the current weapon if any */
        return;                                /* remove obj tells about any no_remove */

/* char CAN use the item! that didn't take long at aaall */

    act ("{g$n wields $p in $s off-hand.{x",ch,obj,NULL,TO_ROOM);
    act ("{gYou wield $p in your off-hand.{x",ch,obj,NULL,TO_CHAR);
    equip_char ( ch, obj, WEAR_SECONDARY);
    sn = get_second_sn(ch);

    if  (sn)
      {
        skill = get_weapon_skill(ch,sn);

        if (skill >= 100)
            act("$p feels like a part of you!",ch,obj,NULL,TO_CHAR);
        else if (skill > 85)
            act("You feel quite confident with $p.",ch,obj,NULL,TO_CHAR);
        else if (skill > 70)
            act("You are skilled with $p.",ch,obj,NULL,TO_CHAR);
        else if (skill > 50)
            act("Your skill with $p is adequate.",ch,obj,NULL,TO_CHAR);
        else if (skill > 25)
            act("$p feels a little clumsy in your hands.",ch,obj,NULL,TO_CHAR);
        else if (skill > 1)
            act("You fumble and almost drop $p.",ch,obj,NULL,TO_CHAR);
    else
            act("You don't even know which end is up on $p.",
                ch,obj,NULL,TO_CHAR);
      }


    return;
}

/*  end */

void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cRemove what?{x\n\r", ch );
	return;
    }


    if ( !str_cmp( arg, "all" ) )
    {
        OBJ_DATA *obj_next;
      
        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;
         if (IS_NPC(ch))
          {
            if (obj->wear_loc != WEAR_NONE && can_see_obj( ch, obj ) )
             {
              if (( obj->pIndexData->limit != -1 ) 
               && (obj->pIndexData->count > obj->pIndexData->limit) 
               && (IS_NPC(ch)))
                { 
                 send_to_char( "{cYou can't find it.{x\n\r", ch );
                } 
               else  
                {
                 remove_obj( ch, obj->wear_loc, TRUE );
                }
             }    
         }
       else
        {
         if (obj->wear_loc != WEAR_NONE && can_see_obj( ch, obj ))
                remove_obj( ch, obj->wear_loc, TRUE );
        }
        }
        return;
    }

    if ( ( obj = get_obj_wear( ch, arg ) ) == NULL )
    {
	send_to_char( "{cYou do not have that item.{x\n\r", ch );
	return;
    }
     if (IS_NPC(ch))
      if(( obj->pIndexData->limit != -1 ) && (obj->pIndexData->count > obj->pIndexData->limit))
       {
        send_to_char( "{cYou can't find it.{x\n\r", ch );
        return;
       }   
    remove_obj( ch, obj->wear_loc, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    int silver;

    
    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    char buffer[100];


    one_argument( argument, arg );

    if ( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
    {
	act( "{g$n offers $mself to the gods, who graciously declines.{x",
	    ch, NULL, NULL, TO_ROOM );
	send_to_char(
	    "{cThe gods appreciate your offer and may accept it later.{x\n\r", ch );
	return;
    }

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	send_to_char( "{cYou can't find it.{x\n\r", ch );
	return;
    }

    if ((IS_OBJ_STAT (obj, ITEM_DIMENSION)) &&
       (!is_affected(ch, gsn_duo_dimension)))
    {
        send_to_char("{cYou are not in the same dimension with that item.{x\n\r", ch);
        return;
    }
    if ((!IS_OBJ_STAT (obj, ITEM_DIMENSION)) &&
       (is_affected(ch, gsn_duo_dimension)))
    {
        send_to_char("{cYou are not in the same dimension with that item.{x\n\r", ch);
        return;
    }


    if ( obj->item_type == ITEM_CORPSE_PC)
    {
	if (obj->contains)
        {
	   send_to_char("{cThe gods refuse your sacrifice!{x\n\r",ch);
	   return;
        }
    }


    if ( !CAN_WEAR(obj, ITEM_TAKE) || CAN_WEAR(obj, ITEM_NO_SAC))
    {
	act( "{c$p is not an acceptable sacrifice.{x", ch, obj, 0, TO_CHAR );
	return;
    }

    silver = UMAX(1,number_fuzzy(obj->level));

    if (obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
    	silver = UMIN(silver,obj->cost);

    if (silver == 1)
        send_to_char(
	    "{cThe gods give you one silver coin for your sacrifice.\n\r{x",ch );
    else
    {
	sprintf(buf,"{cThe gods give you %d silver coins for your sacrifice.\n\r{x",silver);
	send_to_char(buf,ch);
    }

    ch->silver += silver;
    
    if (IS_SET(ch->act,PLR_AUTOSPLIT) )
    { /* AUTOSPLIT code */
    	members = 0;
	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	{
    	    if ( is_same_group( gch, ch ) )
            members++;
    	}

	if ( members > 1 && silver > 1)
	{
	    sprintf(buffer,"%d",silver);
	do_split(ch,buffer);
	}
    }

    act( "{g$n sacrifices $p to the gods.{x", ch, obj, NULL, TO_ROOM );

    if (IS_SET(obj->progtypes,IPROG_SAC))
      if ( (obj->pIndexData->iprogs->sac_prog) (obj,ch) )
        return;

    wiznet("$N sends up $p as a burnt offering.",
	   ch,obj,WIZ_SACCING,0,0);

    if ( (obj->item_type == ITEM_CORPSE_NPC ) || 
	  (obj->item_type == ITEM_CORPSE_PC  ) )
    {
      for ( obj_content = obj->contains; obj_content; obj_content = obj_next )
      {
  	obj_next = obj_content->next_content;
	obj_from_obj( obj_content );
	obj_to_room( obj_content, ch->in_room );
      }
    }

    extract_obj( obj );
    return;
}



void do_quaff( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );
/*
    if ( ch->kingdom == KINGDOM_HORDE )
    {
	send_to_char("{CPotions are for foul mages!{x\n\r",ch);
	return;
    }
 */
    if ( arg[0] == '\0' )
    {
	send_to_char( "{cQuaff what?{x\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "{cYou do not have that potion.{x\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "{cYou can quaff only potions.{x\n\r", ch );
	return;
    }

/*   Grrrr!
    if (ch->level < obj->level)
    {
	send_to_char("This liquid is too powerful for you to drink.\n\r",ch);
	return;
    }
*/
   if ( ch->fighting != NULL )
     WAIT_STATE( ch, 3 * PULSE_VIOLENCE );

    act( "{g$n quaffs $p.{x", ch, obj, NULL, TO_ROOM );
    act( "{gYou quaff $p.{x", ch, obj, NULL ,TO_CHAR );

    obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[4], obj->value[0], ch, ch, NULL );
    extract_obj( obj );
    obj_to_char( create_object(get_obj_index(OBJ_VNUM_EMPTY_VIAL),0), ch);
    return;
}



void do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;
/*
    if ( ch->kingdom == KINGDOM_HORDE )
    {
	send_to_char("{CScrolls are for disgraceful mages!{x\n\r",ch);
	return;
    }
*/
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "{cYou do not have that scroll.{x\n\r", ch );
	return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
	send_to_char( "{cYou can recite only scrolls.{x\n\r", ch );
	return;
    }


    if ( ch->level+7 < scroll->level)
    {
	send_to_char(
		"{cThis scroll is too complex for you to comprehend.{x\n\r",ch);
	return;
    }

    obj = NULL;
    if ( arg2[0] == '\0' )
    {
	victim = ch;
        sprintf( arg2, "%s",ch->name );
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "{cYou can't find it.{x\n\r", ch );
	    return;
	}
    }
    act( "{g$n recites $p.{x", ch, scroll, NULL, TO_ROOM );
    act( "{gYou recite $p.{x", ch, scroll, NULL, TO_CHAR );

    if (!IS_NPC(ch)) /* mobs never fail :> */
     {
      if (number_percent() >= get_skill(ch,gsn_scrolls) * 4/5 )
       {
	send_to_char("{gYou mispronounce a syllable.{x\n\r",ch);
	check_improve(ch,gsn_scrolls,FALSE,2);
       }
      else
       {
        if (( ( victim = get_char_room ( ch, arg2 ) ) != NULL) || ( obj    = get_obj_here  ( ch, arg2 ) ) != NULL )
     	 obj_cast_spell( scroll->value[1], scroll->value[0], ch, victim, obj );
        if (( ( victim = get_char_room ( ch, arg2 ) ) != NULL) || ( obj    = get_obj_here  ( ch, arg2 ) ) != NULL )
    	 obj_cast_spell( scroll->value[2], scroll->value[0], ch, victim, obj );
        if ( (( victim = get_char_room ( ch, arg2 ) ) != NULL) || ( obj    = get_obj_here  ( ch, arg2 ) ) != NULL )
    	 obj_cast_spell( scroll->value[3], scroll->value[0], ch, victim, obj );
        if ( (( victim = get_char_room ( ch, arg2 ) ) != NULL) || ( obj    = get_obj_here  ( ch, arg2 ) ) != NULL )
    	 obj_cast_spell( scroll->value[4], scroll->value[0], ch, victim, obj );

	check_improve(ch,gsn_scrolls,TRUE,2);
        WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
       }
     }

    extract_obj( scroll );
    return;
}



void do_brandish( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char arg[MAX_INPUT_LENGTH];
//    CHAR_DATA *vch_next;
    OBJ_DATA *staff;
    int sn;
/*
    if ( ch->kingdom == KINGDOM_HORDE )
    {
	send_to_char( "{CStaves are for disreputable mages!{x\n\r", ch );
	return;
    }
*/
    if ( ( staff = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
	send_to_char( "{cYou hold nothing in your hand.{x\n\r", ch );
	return;
    }

    if ( staff->item_type != ITEM_STAFF )
    {
	send_to_char( "{cYou can brandish only with a staff.{x\n\r", ch );
	return;
    }

    if ( ( sn = staff->value[3] ) < 0
    ||   sn >= MAX_SKILL
    ||   skill_table[sn].spell_fun == 0 )
    {
	bug( "Do_brandish: bad sn %d.", sn );
	return;
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( staff->value[2] > 0 )
    { //if 1
	act( "{g$n brandishes $p.{x", ch, staff, NULL, TO_ROOM );
	act( "{gYou brandish $p.{x",  ch, staff, NULL, TO_CHAR );

     if (!IS_NPC(ch)) /* mobs never fail :> */
      {   //If2
	if ( ch->level+3 < staff->level
	   ||   (number_percent() >= 10 + get_skill(ch,gsn_staves) * 4/5))
 	 { //if3
	    act ("{gYou fail to invoke $p.{x",ch,staff,NULL,TO_CHAR);
	    act ("{g...and nothing happens.{x",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_staves,FALSE,2);
	  } //if3
	
	else 
	 { //else 1
//	 for ( vch = ch->in_room->people; vch; vch = vch_next )
//	  { //for 1
//	    vch_next	= vch->next_in_room;

    
    one_argument(argument,arg);
    if ((vch = get_char_room(ch,arg)) == NULL)
          {
           if (ch->position == POS_FIGHTING)
            {
             if (skill_table[sn].target == TAR_CHAR_SELF)
              {
               vch = ch;
              }
            else
             {
              vch = ch;
             }  
            }
          }     
	    switch ( skill_table[sn].target )
	    { // swi 1
	    default:
		bug( "Do_brandish: bad target for sn %d.", sn );
		return;

	    case TAR_IGNORE:
		if ( vch != ch )
		break;

	    case TAR_CHAR_OFFENSIVE:
		if ( IS_NPC(ch) ? IS_NPC(vch) : !IS_NPC(vch) )
		break;
		
	    case TAR_CHAR_DEFENSIVE:
		if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
		break;

	    case TAR_CHAR_SELF:
		if ( vch != ch )
		break;
	    } // swi 1

	    obj_cast_spell( staff->value[3], staff->value[0], ch, vch, NULL );
	    check_improve(ch,gsn_staves,TRUE,2);
//	} //for
    }  //else
   } //if 2
    } //if 1

    if ( --staff->value[2] <= 0 )
    {
	act( "{c$n's $p blazes bright and is gone.{x", ch, staff, NULL, TO_ROOM );
	act( "{cYour $p blazes bright and is gone.{x", ch, staff, NULL, TO_CHAR );
	extract_obj( staff );
    }

    return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wand;
    OBJ_DATA *obj;
/*
    if ( ch->kingdom == KINGDOM_HORDE )
    {
	send_to_char( "{CWands are for crooked mages!{x\n\r", ch );
	return;
    }
*/
    one_argument( argument, arg );
    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "{cZap whom or what?{x\n\r", ch );
	return;
    }

    if ( ( wand = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
	send_to_char( "{cYou hold nothing in your hand.{x\n\r", ch );
	return;
    }

    if ( wand->item_type != ITEM_WAND )
    {
	send_to_char( "{cYou can zap only with a wand.{x\n\r", ch );
	return;
    }

    obj = NULL;
    if ( arg[0] == '\0' )
    {
	if ( ch->fighting != NULL )
	{
	    victim = ch->fighting;
	}
	else
	{
	    send_to_char( "{cZap whom or what?{x\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg ) ) == NULL )
	{
	    send_to_char( "{cYou can't find it.{x\n\r", ch );
	    return;
	}
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( wand->value[2] > 0 )
    {
	if ( victim != NULL )
	{
	    act( "{g$n zaps $N with $p.{x", ch, wand, victim, TO_ROOM );
	    act( "{gYou zap $N with $p.{x", ch, wand, victim, TO_CHAR );
	}
	else
	{
	    act( "{g$n zaps $P with $p.{x", ch, wand, obj, TO_ROOM );
	    act( "{gYou zap $P with $p.{x", ch, wand, obj, TO_CHAR );
	}

	if (!IS_NPC(ch)) /* mobs never fail :> */
	{
 	if (ch->level+5 < wand->level 
	||  number_percent() >= 20 + get_skill(ch,gsn_wands) * 4/5) 
	{
	    act( "{gYour efforts with $p produce only smoke and sparks.{x",
		 ch,wand,NULL,TO_CHAR);
	    act( "{g$n's efforts with $p produce only smoke and sparks.{x",
		 ch,wand,NULL,TO_ROOM);
	    check_improve(ch,gsn_wands,FALSE,2);
	}
	else
	{
	    obj_cast_spell( wand->value[3], wand->value[0], ch, victim, obj );
	    check_improve(ch,gsn_wands,TRUE,2);
	}
        }
    }

    if ( --wand->value[2] <= 0 )
    {
	act( "{c$n's $p explodes into fragments.{x", ch, wand, NULL, TO_ROOM );
	act( "{cYour $p explodes into fragments.{x", ch, wand, NULL, TO_CHAR );
	extract_obj( wand );
    }

    return;
}

void do_steal( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *tmp_ch;
    OBJ_DATA *obj;
    OBJ_DATA *obj_inve;
    int percent;
    SHOP_DATA *pShop;
    pShop = NULL;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Steal what from whom?\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) && IS_SET(ch->affected_by, AFF_CHARM) 
	&& (ch->master != NULL))
      {
	send_to_char( "You are to dazed to steal anything.\n\r", ch);
	return;
      }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim) && victim->desc == NULL)
    {
	send_to_char("You can't do that.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (victim->position == POS_FIGHTING)
    {
	send_to_char("You'd better not -- you might get hit.\n\r",ch);
	return;
    }
    if (IS_SET(victim->in_room->room_flags,ROOM_ARENA)) {
	send_to_char("You can't steal in the arena!\n\r",ch);
	return;
	}
/*
    if (!IS_NPC(ch) && 
	(number_percent() > .4 * ch->pcdata->learned[gsn_steal] ) ) {
	do_kill(victim,ch->name);
	return;
}	
*/
/*
        if ( IS_NPC(victim))
	if((pShop = victim->pIndexData->pShop) != NULL) {
	send_to_char("You cannot steal from shop keepers.\n\r",ch);
	do_murder(victim,ch->name);
	return;
        }
*/

    if (IS_SET(victim->detection,DETECT_STEAL)) {
	send_to_char("This person is watching you.\n\r",ch);
	return;
	}

    ch->last_death_time = -1;
    ch->last_fight_time = current_time;

    tmp_ch = (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
     ch->doppel : ch;

    WAIT_STATE( ch, skill_table[gsn_steal].beats );
    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );
    percent += can_see( victim, ch ) ? -10 : 0;
    percent -= 5;

    if ( IS_NPC(victim) )
        if ( victim->pIndexData->pShop != NULL )
           percent -= 50;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_steal].skill_level[ch->class] )
    {
     percent = 1;
    }

    if ( /* ch->level + 5 < victim->level || */
         victim->position == POS_FIGHTING
    || ( !IS_NPC(ch) && percent > ch->pcdata->learned[gsn_steal] ) )
    {
	/*
	 * Failure.
	 */
	send_to_char( "Oops.\n\r", ch );
        if ( !IS_AFFECTED( victim, AFF_SLEEP ) ) {
          victim->position= victim->position==POS_SLEEPING? POS_STANDING:victim->position;
	  act( "{g$n tried to steal from you.\n\r", ch, NULL, victim,TO_VICT  );
        }
	act( "{g$n tried to steal from $N.\n\r",  ch, NULL, victim,TO_NOTVICT);

        sprintf(buf,"%s tries to steal from %s!",ch->name,victim->name);
      
      if (!IS_NPC(victim))
         log_string( buf );
        wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
	switch(number_range(0,3))
	{
	case 0 :
	   sprintf( buf, "%s is a lousy thief!", tmp_ch->name );
	   break;
        case 1 :
	   sprintf( buf, "%s couldn't rob %s way out of a paper bag!",
		    tmp_ch->name,(tmp_ch->sex == 2) ? "her" : "his");
	   break;
	case 2 :
	    sprintf( buf,"%s tried to rob me!",tmp_ch->name );
	    break;
	case 3 :
	    sprintf(buf,"Keep your hands out of there, %s!",tmp_ch->name);
	    break;
        }
	if ( IS_AWAKE( victim ) )
	do_yell(victim,buf);
	if ( !IS_NPC(ch) )
	{

  if ((!IS_NPC(victim)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the craven pickpocket" );
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

	    if ( IS_NPC(victim) )
	    {
	        check_improve(ch,gsn_steal,FALSE,2);
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	}

	return;
    }

    if ( !str_cmp( arg1, "coin"  )
    ||   !str_cmp( arg1, "coins" )
    ||   !str_cmp( arg1, "silver")
    ||   !str_cmp( arg1, "gold"  ) )
    {
	int amount_s = 0;
	int amount_g = 0;
        if ( !str_cmp( arg1, "silver" ) || 
             !str_cmp(arg1, "coin" ) ||
             !str_cmp(arg1, "coins" ) )
	  amount_s = victim->silver * number_range(1, 20) / 100;
        else if ( !str_cmp( arg1, "gold" ) )
	  amount_g = victim->gold * number_range(1, 7) / 100;

	if ( amount_s <= 0 && amount_g <= 0 )
	{
	    send_to_char( "You couldn't get any coins.\n\r", ch );
	    return;
	}

	ch->gold     += amount_g;
	victim->gold -= amount_g;
	ch->silver     += amount_s;
	victim->silver -= amount_s;
	sprintf( buf, "Bingo!  You got %d %s coins.\n\r", 
           amount_s!=0?amount_s:amount_g,
           amount_s!=0?"silver":"gold" );

	send_to_char( buf, ch );
	check_improve(ch,gsn_steal,TRUE,2);

  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the cunning coin collector");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

	return;
    }

    if ( ( obj = get_obj_carry( victim, arg1 ) ) == NULL )
    {
	send_to_char( "{cYou can't find it.{x\n\r", ch );
	return;
    }
  if (IS_NPC(ch))
   if (( obj->pIndexData->limit != -1 ) 
       && obj->pIndexData->count > obj->pIndexData->limit) 
       {
        send_to_char( "{cYou can't find it.{x\n\r", ch );
        return;
      }   
  if ( !can_drop_obj( ch, obj )
   /* ||   IS_SET(obj->extra_flags, ITEM_INVENTORY)*/
   /* ||  obj->level > ch->level */)
    {
	send_to_char( "You can't pry it away.\n\r", ch );
	return;
    }
/*
    if (obj->item_type == ITEM_CONTAINER)
	{
	send_to_char("You can't steal that!!\n\r",ch);
	return;
	}
*/
    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
	send_to_char( "You have your hands full.\n\r", ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	send_to_char( "You can't carry that much weight.\n\r", ch );
	return;
    }
/* Don't work 100, hope it do now */         
    if ( (obj->pIndexData->limit != -1) 
     && (obj->pIndexData->limit < obj->pIndexData->count)
     && (IS_NPC(victim)))   {
      send_to_char( "Thats in maxed you can't steal itlimited.\n\r", ch );
      return;
    }

    if ( !IS_SET( obj->extra_flags, ITEM_INVENTORY ) )  {
      obj_from_char( obj );
      obj_to_char( obj, ch );
      send_to_char( "You got it!\n\r", ch );
      check_improve(ch,gsn_steal,TRUE,2);

  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the sly lifter");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

    }
    else  {
      obj_inve = NULL;
      obj_inve = create_object( obj->pIndexData, 0 ); 
      clone_object( obj, obj_inve );
      REMOVE_BIT( obj_inve->extra_flags, ITEM_INVENTORY );
      obj_to_char( obj_inve, ch );
      send_to_char( "You got one of them!\n\r", ch );
      check_improve(ch,gsn_steal,TRUE,1);
        sprintf(buf,"%s steals from %s [%d] %s!",ch->name,victim->name,obj_inve->pIndexData->vnum,obj_inve->name);
       if (!IS_NPC(victim))
        log_string( buf );
        wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);

    }


    return;
}


/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;

    pShop = NULL;
    for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
    {
	if ( IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) != NULL )
	    break;
    }

    if ( pShop == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return NULL;
    }

    /*
     * Undesirables.
     *
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_KILLER) )
    {
	do_say(keeper, "Killers are not welcome!" );
	sprintf( buf, "%s the KILLER is over here!\n\r", ch->name );
	do_yell(keeper,buf);
	return NULL;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_THIEF) )
    {
	do_say(keeper, "Thieves are not welcome!" );
	sprintf( buf, "%s the THIEF is over here!\n\r", ch->name );
	do_yell(keeper, buf );
	return NULL;
    }
	*/
/*
    if (!IS_NPC(ch) && IS_SET(ch->act, PLR_WANTED))
    {
	do_say(keeper, "Criminals are not welcome!");
	sprintf( buf, "%s the CRIMINAL is over here!\n\r", ch->name );
	do_yell(keeper, buf );
	return NULL;
    }
*/
    /*
     * Shop hours.
     */
    if ( time_info.hour < pShop->open_hour )
    {
	do_say(keeper, "Sorry, I am closed. Come back later.");
	return NULL;
    }
    
    if ( time_info.hour > pShop->close_hour )
    {
	do_say(keeper, "Sorry, I am closed. Come back tomorrow.");
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) && !IS_IMMORTAL( ch ) )
    {
	do_say(keeper, "I don't trade with folks I can't see." );
	return NULL;
    }

    return keeper;
}

/* insert an object at the right spot for the keeper */
void obj_to_keeper( OBJ_DATA *obj, CHAR_DATA *ch )
{
    OBJ_DATA *t_obj, *t_obj_next;

    /* see if any duplicates are found */
    for (t_obj = ch->carrying; t_obj != NULL; t_obj = t_obj_next)
    {
	t_obj_next = t_obj->next_content;

	if (obj->pIndexData == t_obj->pIndexData 
	&&  !str_cmp(obj->short_descr,t_obj->short_descr))
	{
	    /* if this is an unlimited item, destroy the new one */
	    if (IS_OBJ_STAT(t_obj,ITEM_INVENTORY))
	    {
		extract_obj(obj);
		return;
	    }
	    obj->cost = t_obj->cost; /* keep it standard */
	    break;
	}
    }

    if (t_obj == NULL)
    {
	obj->next_content = ch->carrying;
	ch->carrying = obj;
    }
    else
    {
	obj->next_content = t_obj->next_content;
	t_obj->next_content = obj;
    }

    obj->carried_by      = ch;
    obj->in_room         = NULL;
    obj->in_obj          = NULL;
    ch->carry_number    += get_obj_number( obj );
    ch->carry_weight    += get_obj_weight( obj );
}

/* get an object from a shopkeeper's list */
OBJ_DATA *get_obj_keeper( CHAR_DATA *ch, CHAR_DATA *keeper, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;
 
    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = keeper->carrying; obj != NULL; obj = obj->next_content )
    {
        if (obj->wear_loc == WEAR_NONE
        &&  can_see_obj( keeper, obj )
	&&  can_see_obj(ch,obj)
        &&  is_name( arg, obj->name ) )
        {
            if ( ++count == number )
                return obj;
	
	    /* skip other objects of the same name */
	    while (obj->next_content != NULL
	    && obj->pIndexData == obj->next_content->pIndexData
	    && !str_cmp(obj->short_descr,obj->next_content->short_descr))
		obj = obj->next_content;
        }
    }
 
    return NULL;
}

int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
    SHOP_DATA *pShop;
    int cost;

    if ( obj == NULL || ( pShop = keeper->pIndexData->pShop ) == NULL )
	return 0;

    if ( fBuy )
    {
	cost = obj->cost * pShop->profit_buy  / 100;
    }
    else
    {
	OBJ_DATA *obj2;
	int itype;

	cost = 0;
	for ( itype = 0; itype < MAX_TRADE; itype++ )
	{
	    if ( obj->item_type == pShop->buy_type[itype] )
	    {
		cost = obj->cost * pShop->profit_sell / 100;
		break;
	    }
	}

	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
	    for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
	    {
	    	if ( obj->pIndexData == obj2->pIndexData
		&&   !str_cmp(obj->short_descr,obj2->short_descr) )
		  return 0;
/*
	 	    if (IS_OBJ_STAT(obj2,ITEM_INVENTORY))
			cost /= 2;
		    else
                    	cost = cost * 3 / 4;
*/
	    }
    }

    if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
    {
	if (obj->value[1] == 0)
	    cost /= 4;
	else
	    cost = cost * obj->value[2] / obj->value[1];
    }

    return cost;
}



void do_buy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cost,roll;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Buy what?\n\r", ch );
	return;
    }

 if (ch->position == POS_FIGHTING)
   {
      send_to_char(" You're {Yfighting!{x\n\r",ch);
      return;
   }

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *pet;
	ROOM_INDEX_DATA *pRoomIndexNext;
	ROOM_INDEX_DATA *in_room;

	smash_tilde(argument);

	if ( IS_NPC(ch) )
	    return;



	argument = one_argument(argument,arg);

	/* hack to make new thalos pets work */

	if (ch->in_room->vnum == 9621)
	    pRoomIndexNext = get_room_index(9706);
	else
	    pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	in_room     = ch->in_room;
	ch->in_room = pRoomIndexNext;
	pet         = get_char_room( ch, arg );
	ch->in_room = in_room;

	if ( pet == NULL || !IS_SET(pet->act, ACT_PET) )
	{
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	if ( ch->pet != NULL )
	{
	    send_to_char("You already own a pet.\n\r",ch);
	    return;
	}

 	cost = 10 * pet->level * pet->level;

	if ( (ch->silver + 100 * ch->gold) < cost )
	{
	    send_to_char( "You can't afford it.\n\r", ch );
	    return;
	}

	if ( ch->level < pet->level )
	{
	    send_to_char(
		"You're not powerful enough to master this pet.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (roll < get_skill(ch,gsn_haggle))
	{
	    cost -= cost / 2 * roll / 100;
	    sprintf(buf,"You haggle the price down to %d coins.\n\r",cost);
	    send_to_char(buf,ch);
	    check_improve(ch,gsn_haggle,TRUE,4);
	
	}

	deduct_cost(ch,cost);
	pet			= create_mobile( pet->pIndexData );
	SET_BIT(pet->act, ACT_PET);
	SET_BIT(pet->affected_by, AFF_CHARM);
	pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

	argument = one_argument( argument, arg );
	if ( arg[0] != '\0' )
	{
	    sprintf( buf, "%s %s", pet->name, arg );
	    free_string( pet->name );
	    pet->name = str_dup( buf );
	}

	sprintf( buf, "%sA neck tag says 'I belong to %s'.\n\r",
	    pet->description, ch->name );
	free_string( pet->description );
	pet->description = str_dup( buf );

	char_to_room( pet, ch->in_room );
	add_follower( pet, ch );
	pet->leader = ch;
	ch->pet = pet;
	send_to_char( "Enjoy your pet.\n\r", ch );
	act( "{g$n bought $N as a pet.", ch, NULL, pet, TO_ROOM );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj,*t_obj;
	char arg[MAX_INPUT_LENGTH];
	int number, count = 1;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;


	number = mult_argument(argument,arg);
	obj  = get_obj_keeper( ch,keeper, arg );
	cost = get_cost( keeper, obj, TRUE );

	if ( cost <= 0 || !can_see_obj( ch, obj ) )
	{
	    act( "$n tells you 'I don't sell that -- try 'list'.'",
		keeper, NULL, ch, TO_VICT);
	    ch->reply = keeper;
	    return;
	}

	if (number < 0 || number > 200)
	{
	act("$n tells you 'Get Real!",keeper,NULL,ch,TO_VICT);
	return;
        }

	if (!IS_OBJ_STAT(obj,ITEM_INVENTORY))
	{
	    for (t_obj = obj->next_content;
	     	 count < number && t_obj != NULL; 
	     	 t_obj = t_obj->next_content) 
	    {
	    	if (t_obj->pIndexData == obj->pIndexData
	    	&&  !str_cmp(t_obj->short_descr,obj->short_descr))
		    count++;
	    	else
		    break;
	    }

	    if (count < number)
	    {
	    	act("$n tells you 'I don't have that many in stock.'",
		    keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
	}

	if (obj->pIndexData->limit != -1)
	{
	act("$n tells you 'This is too valuable for me to sell to you.'",
		keeper,obj,ch,TO_VICT);
	return;
	}

	if ( (ch->silver + ch->gold * 100) < cost * number )
	{
	    if (number > 1)
		act("$n tells you 'You can't afford to buy that many.'",
		    keeper,obj,ch,TO_VICT);
	    else
	    	act( "$n tells you 'You can't afford to buy $p.'",
		    keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}
	
/*	if ( obj->level > ch->level )
	{
	    act( "$n tells you 'You can't use $p yet'.",
		keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	} */

	if (ch->carry_number +  number * get_obj_number(obj) > can_carry_n(ch))
	{
	    send_to_char( "You can't carry that many items.\n\r", ch );
	    return;
	}

	if ( ch->carry_weight + number * get_obj_weight(obj) > can_carry_w(ch))
	{
	    send_to_char( "You can't carry that much weight.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT) 
	&& roll < get_skill(ch,gsn_haggle))
	{
	    cost -= obj->cost / 2 * roll / 100;
	    act("You haggle with $N.",ch,NULL,keeper,TO_CHAR);
	    check_improve(ch,gsn_haggle,TRUE,4);
	}

	if (number > 1)
	{
	    sprintf(buf,"$n buys $p[%d].",number);
	    act(buf,ch,obj,NULL,TO_ROOM);
	    sprintf(buf,"You buy $p[%d] for %d silver.",number,cost * number);
	    act(buf,ch,obj,NULL,TO_CHAR);
	}
	else
	{
	    act( "$n buys $p.", ch, obj, NULL, TO_ROOM );
	    sprintf(buf,"You buy $p for %d silver.",cost);
	    act( buf, ch, obj, NULL, TO_CHAR );
	}
	deduct_cost(ch,cost * number);
	keeper->gold += cost * number/100;
	keeper->silver += cost * number - (cost * number/100) * 100;

	for (count = 0; count < number; count++)
	{
	    if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	    	t_obj = create_object( obj->pIndexData, obj->level );
	    else
	    {
		t_obj = obj;
		obj = obj->next_content;
	    	obj_from_char( t_obj );
	    }

	    if (t_obj->timer > 0 && !IS_OBJ_STAT(t_obj,ITEM_HAD_TIMER))
	    	t_obj->timer = 0;
	    REMOVE_BIT(t_obj->extra_flags,ITEM_HAD_TIMER);
	    obj_to_char( t_obj, ch );
	    if (cost < t_obj->cost)
	    	t_obj->cost = cost;
	}
    }
}



void do_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	ROOM_INDEX_DATA *pRoomIndexNext;
	CHAR_DATA *pet;
	bool found;



        /* hack to make new thalos pets work */
        if (ch->in_room->vnum == 9621)
            pRoomIndexNext = get_room_index(9706);
        else
            pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	found = FALSE;
	for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
	{
	    if ( IS_SET(pet->act, ACT_PET) )
	    {
		if ( !found )
		{
		    found = TRUE;
		    send_to_char( "Pets for sale:\n\r", ch );
		}
		sprintf( buf, "[%2d] %8d - %s\n\r",
		    pet->level,
		    10 * pet->level * pet->level,
		    pet->short_descr );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "Sorry, we're out of pets right now.\n\r", ch );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost,count;
	bool found;
	char arg[MAX_INPUT_LENGTH];

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;


        one_argument(argument,arg);

	found = FALSE;
	for ( obj = keeper->carrying; obj; obj = obj->next_content )
	{
	    if ( obj->wear_loc == WEAR_NONE
	    &&   can_see_obj( ch, obj )
	    &&   ( cost = get_cost( keeper, obj, TRUE ) ) > 0 
	    &&   ( arg[0] == '\0'  
 	       ||  is_name(arg,obj->name) ))
	    {
		if ( !found )
		{
		    found = TRUE;
		    send_to_char( "[Lv Price Qty] Item\n\r", ch );
		}

		if (IS_OBJ_STAT(obj,ITEM_INVENTORY))
		    sprintf(buf,"[%2d %5d -- ] %s\n\r",
			obj->level,cost,obj->short_descr);
		else
		{
		    count = 1;

		    while (obj->next_content != NULL 
		    && obj->pIndexData == obj->next_content->pIndexData
		    && !str_cmp(obj->short_descr,
			        obj->next_content->short_descr))
		    {
			obj = obj->next_content;
			count++;
		    }
		    sprintf(buf,"[%2d %5d %2d ] %s\n\r",
			obj->level,cost,count,obj->short_descr);
		}
		send_to_char( buf, ch );
	    }
	}

	if ( !found )
	    send_to_char( "You can't buy anything here.\n\r", ch );
	return;
    }
}



void do_sell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost,roll;
    int gold, silver;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Sell what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

 if (ch->position == POS_FIGHTING)
   {
      send_to_char(" You're {Yfighting!{x\n\r",ch);
      return;
   }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item.'",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
	act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }
    if ( cost > (keeper->silver + 100 * keeper->gold) )
    {
	act("$n tells you 'I'm afraid I don't have enough wealth to buy $p.'",
	    keeper,obj,ch,TO_VICT);
	return;
    }

  if ((!IS_NPC(ch)) && (number_percent() < 3)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Merchandiser");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

    act( "$n sells $p.", ch, obj, NULL, TO_ROOM );
    /* haggle */
    roll = number_percent();
    if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT) && roll < get_skill(ch,gsn_haggle))
    {
        roll = get_skill(ch, gsn_haggle) + number_range(1, 20) - 10;
        send_to_char("You haggle with the shopkeeper.\n\r",ch);
        cost += obj->cost / 2 * roll / 100;
        cost = UMIN(cost,95 * get_cost(keeper,obj,TRUE) / 100);
	cost = UMIN(cost,(keeper->silver + 100 * keeper->gold));
        check_improve(ch,gsn_haggle,TRUE,4);
    }

    silver = cost - (cost/100) * 100;
    gold   = cost/100;
 
    sprintf( buf, "You sell $p for %d silver and %d gold piece%s.\n\r",
	silver, gold, cost == 1 ? "" : "s" );

    sprintf( buf2, "You sell $p for %s %s%spiece%s.",
      silver!=0?"%d silver":"",                         /* silvers  */
      (silver!=0 && gold != 0)?"and ":"",		/*   and    */
      gold!=0?"%d gold ":"",				/*  golds   */
      silver+gold>1?"s":"" );				/* piece(s) */
/*    sprintf( buf, buf2, silver, gold ); */
           
    act( buf, ch, obj, NULL, TO_CHAR );
    ch->gold     += gold;
    ch->silver 	 += silver;
    deduct_cost(keeper,cost);
    if ( keeper->gold < 0 )
	keeper->gold = 0;
    if ( keeper->silver< 0)
	keeper->silver = 0;

    if ( obj->item_type == ITEM_TRASH || IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
    {
	extract_obj( obj );
    }
    else
    {
	obj_from_char( obj );
	if (obj->timer)
	    SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	else
	    obj->timer = number_range(50,100);
	obj_to_keeper( obj, keeper );
    }

    return;
}



void do_value( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Value what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
        act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
        return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }

    sprintf( buf, 
	"$n tells you 'I'll give you %d silver and %d gold coins for $p.'", 
	cost - (cost/100) * 100, cost/100 );
    act( buf, keeper, obj, ch, TO_VICT);
    ch->reply = keeper;

    return;
}

void do_wanted( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
 /*
    if (!kingdom_ok(ch,gsn_wanted))
      return;
*/
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
/*
    if (IS_NPC(ch) || ch->pcdata->learned[gsn_wanted] < 50)
      {
	send_to_char( "Huh?\n\r",ch);
	return;
      }
    if (IS_SET(ch->affected_by, AFF_CHARM) && (ch->master != NULL))
      {
	send_to_char( "Your mind is too confused about judging people.\n\r", ch);
        act( "$n's mind is too confused about judging people",ch, NULL, NULL, TO_ROOM);
	return;
      }
*/
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
      {
        send_to_char( "Usage: wanted <player> <Y|N>\n\r", ch );
        return;
      }

    victim = get_char_world(ch, arg1);

    if ( (victim == NULL) || 
	!(can_see(ch, victim)) )
      {
        send_to_char( "They aren't here.\n\r", ch );
        return;
      }
    
    if (victim->level >= LEVEL_IMMORTAL && (ch->level < victim->level)) 
      {
	act( "You do not have the power to arrest $N.", ch, NULL, victim, 
	    TO_CHAR);
	return;
      }
    
    if (victim == ch)
      {
	send_to_char( "You cannot do that to yourself.\n\r", ch);
	return;
      }
     if(IS_NPC(victim))
	{
	send_to_char("Your victim is a NPC.\n\r",ch);
	send_to_char("People are using pets and renaming their own so they don't get wanted.\n\r",ch);
	send_to_char("Please double check using wanted 2.whoever.\n\r",ch);
	}

    switch(arg2[0]) 
      {
      case 'Y':
      case 'y':
	if (victim->act & PLR_WANTED)
	  act( "$n is already wanted.", ch, NULL, NULL, TO_CHAR);
	else 
	  {
	    SET_BIT(victim->act, PLR_WANTED);
	    act( "$n is now WANTED!!!",victim, NULL, ch, TO_NOTVICT);
	    send_to_char( "You are now WANTED!!!\n\r", victim);
	    send_to_char( "Ok.\n\r", ch);
	  }
	break;
      case 'N':
      case 'n':
	if (!(victim->act & PLR_WANTED))
	  act( "$N is not wanted.", ch, NULL, victim, TO_CHAR);
	else
	  {
	    REMOVE_BIT(victim->act, PLR_WANTED);
	    act( "$n is no longer wanted.",victim, NULL, ch, TO_NOTVICT);
	    send_to_char( "You are no longer wanted.\n\r", victim);
	    send_to_char( "Ok.\n\r", ch);
	}
	break;
      default:
        send_to_char( "Usage: wanted <player> <Y|N>\n\r", ch );
	break;
      }
  }

void do_herbs(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  
  one_argument(argument,arg);
  
  if (IS_NPC(ch))
    return;

  if (IS_AFFECTED2(ch,AFF2_HERBS))
    {
      send_to_char("You can't find any more herbs.\n\r",ch);
      return;
    }

  if (arg[0] == '\0')
    victim = ch;
  else if ( (victim = get_char_room(ch,arg)) == NULL)
    {
      send_to_char("They're not here.\n\r",ch);
      return;
    }
  WAIT_STATE( ch, skill_table[gsn_herbs].beats );

  if (ch->in_room->sector_type != SECT_INSIDE && 
      ch->in_room->sector_type != SECT_CITY && 
      (IS_NPC(ch) || number_percent() < ch->pcdata->learned[gsn_herbs]))
    {
      AFFECT_DATA af;
      af.where  = TO_AFFECTS2;
      af.type 	= gsn_herbs;
      af.level 	= ch->level;
      af.duration = 5;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = AFF2_HERBS;

      affect_to_char(ch,&af);

      send_to_char("You gather some beneficial herbs.\n\r",ch);
      act("$n gathers some herbs.",ch,NULL,NULL,TO_ROOM);
      
      if (ch != victim)
	{
	  act("{g$n gives you some herbs to eat.{x",ch,NULL,victim,TO_VICT);
	  act("{gYou give the herbs to $N.{x",ch,NULL,victim,TO_CHAR);
	  act("{g$n gives the herbs to $N.{x",ch,NULL,victim,TO_NOTVICT);
	}
	      
      if (victim->hit < victim->max_hit)
	{
	  send_to_char("You feel better.\n\r",victim);
	  act("$n looks better.",victim,NULL,NULL,TO_ROOM);
	}
      victim->hit = UMIN(victim->max_hit,victim->hit + 5 * ch->level);
      check_improve(ch,gsn_herbs,TRUE,1);
      if (is_affected(victim, gsn_plague))
	{
	  if (check_dispel(ch->level,victim,gsn_plague))
	    { 
	      send_to_char("Your sores vanish.\n\r",victim);
	      act("$n looks relieved as $s sores vanish.",victim,NULL,NULL,TO_ROOM);
	    }
	}
    }
  else
    {
      send_to_char("You search for herbs but find none here.\n\r",ch);
      act("$n looks around for herbs.",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_herbs,FALSE,1);
    }
}

void do_lore( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
  char buf[MAX_STRING_LENGTH];
  AFFECT_DATA *paf;
  int chance;
  int value0, value1, value2, value3;

  argument = one_argument( argument, arg1 );
  
  obj =  get_obj_list(ch,arg1,ch->carrying);

    if (obj== NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }
 if (obj->item_type == ITEM_WEAPON)
  {
      send_to_char("Weapon type is ",ch);

            value0 = obj->value[0];
	    value1 = obj->value[1];
            value2 = obj->value[2];

      switch (value0)
	{
	case(WEAPON_EXOTIC) :sprintf(buf,"exotic.\n\r");	break;
	case(WEAPON_SWORD)  :sprintf(buf,"sword.\n\r");	break;
	case(WEAPON_DAGGER) :sprintf(buf,"dagger.\n\r");	break;
	case(WEAPON_SPEAR)	: sprintf(buf,"spear/staff.\n\r");	break;
	case(WEAPON_MACE) 	: sprintf(buf,"mace/club.\n\r");	break;
	case(WEAPON_AXE)	: sprintf(buf,"axe.\n\r");		break;
	case(WEAPON_FLAIL)	: sprintf(buf,"flail.\n\r");	break;
	case(WEAPON_WHIP)	: sprintf(buf,"whip.\n\r");		break;
	case(WEAPON_POLEARM): sprintf(buf,"polearm.\n\r");	break;
	case(WEAPON_BOW)  : sprintf(buf,"bow.\n\r");  break;
 	case(WEAPON_ARROW)      : sprintf(buf,"arrow.\n\r"); break;
        case(WEAPON_LANCE)      : sprintf(buf,"lance.\n\r"); break;
	default		: sprintf(buf,"unknown.\n\r");	 break;
 	}
      do_say(ch,buf);
      if (obj->pIndexData->new_format)
	sprintf(buf,"Damage is %dd%d (average %d).\n\r",value1,value2,(1 + value2) * value1 / 2);
      else
	sprintf( buf, "Damage is %d to %d (average %d).\n\r",value1, value2,( value1 + value2 ) / 2 );
      do_say(ch,buf);
    
  check_improve(ch,gsn_lore,TRUE,5);
  return;
}
else 
   send_to_char("That is not a weapon.\n\r",ch);
return;
   
}



void do_butcher(CHAR_DATA *ch, char *argument)
{
  OBJ_DATA *obj;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  OBJ_DATA *tmp_obj;
  OBJ_DATA *tmp_next;

  if (IS_NPC(ch))
    return;

  one_argument(argument,arg);
  if ( arg[0]=='\0' )
  {
    send_to_char( "Butcher what?\n\r", ch );
    return;
  }
  if ( (obj = get_obj_here(ch,arg)) == NULL )
    {
      send_to_char("You do not see that here.\n\r",ch);
      return;
    }

  if (obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
    {
      send_to_char("You can't butcher that.\n\r",ch);
      return;
    }

  if (obj->butcher >= 1) 
	{
	send_to_char("The corpse already been butchered.\n\r",ch);
	return;
	}

  if (obj->carried_by != NULL)
    {
      send_to_char("Put it down first.\n\r",ch);
      return;
    }

  if ( !IS_NPC(ch) && ch->pcdata->learned[gsn_butcher] < 1)
    {
      send_to_char("You don't have the precision instruments for that.", ch);
      return;
    }
  if(obj->item_type == ITEM_CORPSE_NPC) {
  obj_from_room(obj);
  
  /* empty the corpse into the room */
  for (tmp_obj = obj->contains;tmp_obj != NULL;
       tmp_obj = tmp_next)
    {
      tmp_next = tmp_obj->next_content;
      obj_from_obj(tmp_obj);
      obj_to_room(tmp_obj,ch->in_room);
    }
  
  }

  if (IS_NPC(ch) || number_percent() < ch->pcdata->learned[gsn_butcher])
    {
      int numsteaks;
      int i;
      OBJ_DATA *steak;
        
      numsteaks = number_bits(2) + 1; /* 1-4 */
      
      if (numsteaks > 1)
	{
	  sprintf(buf, "$n butchers $p and creates %i steaks.",numsteaks);
	  act(buf,ch,obj,NULL,TO_ROOM);

	  sprintf(buf, "You butcher $p and create %i steaks.",numsteaks);
	  act(buf,ch,obj,NULL,TO_CHAR);
	}

      else 
	{
	  act("$n butchers $p and creates a steak."
	      ,ch,obj,NULL,TO_ROOM);

	  act("You butcher $p and create a steak."
	      ,ch,obj,NULL,TO_CHAR);
	}
      check_improve(ch,gsn_butcher,TRUE,1);

      for (i=0; i < numsteaks; i++)
	{
	  steak = create_object(get_obj_index(OBJ_VNUM_STEAK),0);
	  sprintf( buf, steak->short_descr, obj->short_descr);
	  free_string( steak->short_descr );
	  steak->short_descr = str_dup( buf );

	  sprintf( buf, steak->description, obj->short_descr );
	  free_string( steak->description );
	  steak->description = str_dup( buf );

	  obj_to_room(steak,ch->in_room);
	}
    }	
  else 
    {
      act("You fail and destroy $p.",ch,obj,NULL,TO_CHAR);
      act("$n fails to butcher $p and destroys it.",
	  ch,obj,NULL,TO_ROOM);

      check_improve(ch,gsn_butcher,FALSE,1);
    }
  if(obj->item_type == ITEM_CORPSE_NPC)   
		       extract_obj(obj);
	else
			obj->butcher = 1;
}


void do_balance(CHAR_DATA *ch, char *argument)
{
  char buf[160];
  char buf2[100];
  long bank_g;
  long bank_s;

  if (IS_NPC(ch))
    {
      send_to_char("You don't have a bank account.\n\r",ch);
      return;
    }
  
  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char("You are not in a bank.\n\r",ch);
      return;
    }


  if ( ch->pcdata->bank_s + ch->pcdata->bank_g == 0 )  {
    send_to_char( "You don't have any money in the bank.\n\r", ch );
    return;
  }

  bank_g = ch->pcdata->bank_g;
  bank_s = ch->pcdata->bank_s;
  sprintf( buf, "You have %s%s%s coin%s in the bank.\n\r",
    bank_g!=0?"%ld gold":"",
    (bank_g!=0)&&(bank_s!=0)?" and ":"",
    bank_s!=0?"%ld silver":"",
    bank_s+bank_g>1?"s":"" );
  if (bank_g == 0)
    sprintf( buf2, buf, bank_s );
  else
    sprintf( buf2, buf, bank_g, bank_s );

  send_to_char(buf2, ch);
}

void do_withdraw(CHAR_DATA *ch, char *argument)
{
  long  amount_s;
  long  amount_g;
  char arg[MAX_INPUT_LENGTH];
  char buf[100];

  if (IS_NPC(ch))
    {
      send_to_char("You don't have a bank account.\n\r",ch);
      return;
    }
  
  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char("The mosquito by your feet will not give you any money.\n\r",ch);
      return;
    }  

  argument = one_argument( argument, arg );
  if ( arg[0] == '\0' )  {
    send_to_char( "Withdraw how much?\n\r", ch );
    return;
  }
    
  amount_s = labs (atol(arg));
  if ( !str_cmp( argument, "silver") || argument[0] == '\0' )
    amount_g = 0;
  else if ( !str_cmp( argument, "gold" ) )  {
    amount_g = amount_s;
    amount_s = 0;
  }
  else {
    send_to_char( "You can withdraw gold and silver coins only.", ch );
    return;
  }

  if ( amount_g > ch->pcdata->bank_g)
    {
      send_to_char("Sorry, we don't give loans.\n\r",ch);
      return;
    }

  if ( amount_s > ch->pcdata->bank_s)
    {
      send_to_char("Sorry, we don't give loans.\n\r",ch);
      return;
    }

  ch->pcdata->bank_g -= amount_g;
  ch->pcdata->bank_s -= amount_s;
  ch->gold += 0.98 * amount_g;
  ch->silver += 0.90 * amount_s;
  if (amount_s > 0  && amount_s < 10 )  {
    if ( amount_s == 1 )
      sprintf(buf, "One coin??!!! You cheapskate!\n\r");
    else
      sprintf(buf, "%ld coins??!!! You cheapskate!\n\r", amount_s);
  }
  else 
    sprintf(buf, 
	    "Here are your %ld %s coins, minus a %ld coin withdrawal fee.\n\r",
	    amount_s!=0?amount_s:amount_g, 
	    amount_s!=0?"silver":"gold",
	    amount_s!=0?(long) UMAX(1, (0.10 * amount_s)):
                        (long) UMAX(1, (0.02 * amount_g)) );
  send_to_char(buf, ch);
  act("$n steps up to the teller window.",ch,NULL,NULL,TO_ROOM);
}

void do_deposit(CHAR_DATA *ch, char *argument)
{
  long amount_s;
  long amount_g;
  char buf[100];
  char arg[200];

  if (IS_NPC(ch))
    {
      send_to_char("You don't have a bank account.\n\r",ch);
      return;
    }
  
  if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {
      send_to_char("The ant by your feet can't carry your gold.\n\r",ch);
      return;
    }  

  argument = one_argument( argument, arg );
  if ( arg[0] == '\0' )  {
    send_to_char( "Deposit how much?\n\r", ch );
    return;
  }
  amount_s = labs (atol(arg));
  if ( !str_cmp( argument, "silver" ) || argument[0] == '\0' )
    amount_g = 0;
  else if ( !str_cmp( argument, "gold" ) )  {
    amount_g = amount_s;
    amount_s = 0;
  }
  else {
    send_to_char( "You can deposit gold and silver coins only.", ch );
    return;
  }
  
  if (amount_g > ch->gold)
    {
      send_to_char("That's more than you've got.\n\r",ch);
      return;
    }
  if (amount_s > ch->silver)
    {
      send_to_char("That's more than you've got.\n\r",ch);
      return;
    }

  ch->pcdata->bank_s += amount_s;
  ch->pcdata->bank_g += amount_g;
  ch->gold -= amount_g;
  ch->silver -= amount_s;

  if (amount_s == 1)
    sprintf(buf, "Oh boy! One gold coin!\n\r");
  else sprintf(buf, "%ld %s coins deposited. Come again soon!\n\r",
                amount_s!=0?amount_s:amount_g,
                amount_s!=0?"silver":"gold" );

  send_to_char(buf, ch);
  act("$n steps up to the teller window.",ch,NULL,NULL,TO_ROOM);
}


void check_look( OBJ_DATA *obj )
{
char buf[200];
char liq_name[90];

    /*  change the look  GG */
    if ( obj->pIndexData->vnum != OBJ_VNUM_EMPTY_VIAL )
      return;
  if ( obj->value[1] > 1 )  {
    if ( obj->value[2] == 0 ||
         obj->value[2] == 13 )
      sprintf(liq_name, "%s", obj->value[2]==0?"water":"blood" );
    else
      sprintf(liq_name, "a%s %s liquid", 
	liq_table[obj->value[2]].liq_color[0]=='a'?"n":
	liq_table[obj->value[2]].liq_color[0]=='h'?"n":"",
	liq_table[obj->value[2]].liq_color );
    free_string( obj->short_descr );
    sprintf(buf, "vial of %s", liq_name);
    obj->short_descr = str_dup( buf );

    free_string( obj->name );
    sprintf(buf, "vial %s", liq_table[obj->value[2]].liq_color);
    obj->name = str_dup( buf );

    free_string( obj->description );
    sprintf(buf,  "A vial of %s rests on the ground.", liq_name );
    obj->description = str_dup( buf );
    return;
  }

    free_string( obj->short_descr );
    obj->short_descr = str_dup( "an empty vial" );

    free_string( obj->name );
    obj->name = str_dup( "empty vial" );

    free_string( obj->description );
    obj->description = str_dup( "An empty vial is forgotten here." );
    return;
  
}

void do_sharpen( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    AFFECT_DATA af;
    int percent, skill;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument, arg);

    if ((skill = get_skill(ch, gsn_sharpen)) < 1)
    {
	send_to_char("You have no idea how to sharpen weapons.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	send_to_char("Sharpen what weapon?\n\r",ch);
	return;
    }

    if ((obj = get_obj_list(ch, arg, ch->carrying)) == NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("You can only sharpen weapons.\n\r",ch);
	return;
    }

    if (IS_WEAPON_STAT(obj, WEAPON_SHARP))
    {
	act("$p has already been sharpened.",ch,obj,NULL,TO_CHAR);
	return;
    }

    WAIT_STATE(ch, skill_table[gsn_sharpen].beats);
    percent = number_percent();
    if (percent < (0.30 * skill))
    {
	af.where	= TO_WEAPON;
	af.type		= gsn_sharpen;
	af.level	= ch->level * percent / 100;
	af.duration	= (obj->level + ch->level) / 5;
	af.location	= 0;
	af.modifier	= 0;
	af.bitvector	= WEAPON_SHARP;
	affect_to_obj(obj, &af);
	act("$n pulls out a piece of stone and begins sharpening $p.",
	    ch,obj,NULL,TO_ROOM);
	act("You sharpend $p.",ch,obj,NULL,TO_CHAR);
	check_improve(ch, gsn_sharpen, TRUE, 3);
	return;
    }
    else
    {
	if (obj->value[2] > 0)
	    obj->value[2]--;
	send_to_char("You fail to sharpen weapon.\n\r",ch);
	check_improve(ch, gsn_sharpen, FALSE, 3);
	return;
    }
}

void damage_to_obj(CHAR_DATA *ch,OBJ_DATA *wield, OBJ_DATA *worn, int damage);

void damage_to_obj(CHAR_DATA *ch,OBJ_DATA *wield, OBJ_DATA *worn, int damage)
{
 char buf[MAX_STRING_LENGTH];

 if (IS_SET(wield->extra_flags,ITEM_INDESTRUCT))
    return;
 if (IS_SET(worn->extra_flags,ITEM_INDESTRUCT))
    return;

 if ( damage == 0) return;
 worn->condition -= (damage/4);
 act_new("The $p inflicts damage on $P.",
        ch,wield,worn,TO_ROOM,POS_RESTING);

 if (worn->condition < 1)
        {
 act_new("The $P breaks into pieces.",
        ch,wield,worn,TO_ROOM,POS_RESTING);
        extract_obj( worn );
        return;
        }

 if ( (IS_SET(wield->extra_flags,ITEM_ANTI_EVIL)
        && IS_SET(wield->extra_flags,ITEM_ANTI_NEUTRAL) )
     && (IS_SET(worn->extra_flags,ITEM_ANTI_EVIL)
        && IS_SET(worn->extra_flags,ITEM_ANTI_NEUTRAL) ) )
 {
  sprintf(buf,"$p doesn't want to fight against $P.");
  act_new(buf,ch,wield,worn,TO_ROOM,POS_RESTING);
  sprintf(buf,"$p removes itself from you!.");
  act_new(buf,ch,wield,worn,TO_CHAR,POS_RESTING);
  sprintf(buf,"$p removes itself from $n.");
 act_new(buf,ch,wield,worn,TO_ROOM,POS_RESTING);
  unequip_char( ch, wield );
  return;
 }

 if (IS_SET(wield->extra_flags,ITEM_ANTI_EVIL)
        && IS_SET(worn->extra_flags,ITEM_ANTI_EVIL))
 {
  sprintf(buf,"The $p worries for the damage to $P.");
  act_new(buf,ch,wield,worn,TO_ROOM,POS_RESTING);
  return;
 }
 return;
}

void do_repair(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *mob;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
  OBJ_DATA *obj;
   int cost;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if (!IS_NPC(mob)) continue;
        if (mob->spec_fun == spec_lookup("spec_repairman") )
            break;
    }

    if ( mob == NULL )
    {
        send_to_char( "You can't do that here.\n\r", ch );
        return;
    }

    one_argument(argument,arg);
 if (arg[0] == '\0')
    {
       do_say(mob,"I will repair a weapon for you, for a price.");
     send_to_char("Type estimate <weapon> to be assessed for damage.\n\r",ch);
        return;
    }
    if (( obj = get_obj_carry(ch, arg)) == NULL)
    {
        do_say(mob,"You don't have that item");
        return;
    }


    if (obj->condition >= 100)
    {
        do_say(mob,"But that item is not broken.");
        return;
    }

    if (obj->condition == 0)
    {
        sprintf(buf ,"%s is beyond repair.\n\r", obj->short_descr);
        do_say(mob,buf);
        return;
    }

    cost = ( (obj->level * 10) +
                ((obj->cost * (100 - obj->condition)) /100)    );
  cost /= 100;

    if (cost > ch->gold)
    {
        do_say(mob,"You do not have enough gold for my services.");
        return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);

    ch->gold -= cost;
    mob->gold += cost;
    obj->cost = obj->cost/10;
    sprintf(buf, "$N takes %s from $n, repairs it, and returns it to $n",
        obj->short_descr);
    act(buf,ch,NULL,mob,TO_ROOM);
    sprintf(buf, "%s takes %s, repairs it, and returns it\n\r",
        mob->short_descr, obj->short_descr);
    send_to_char(buf, ch);
    obj->condition = 100;
}

void do_estimate(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *mob;
  char arg[MAX_INPUT_LENGTH];
    int cost;

    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if (!IS_NPC(mob)) continue;
        if (mob->spec_fun == spec_lookup("spec_repairman") )
            break;
    }

    if ( mob == NULL )
    {
        send_to_char( "You can't do that here.\n\r", ch );
        return;
   }

    one_argument(argument, arg);

    if (arg[0] == '\0')
    {
        do_say(mob,"Try estimate <item>");
        return;
    }
    if ((obj = (get_obj_carry(ch, arg))) == NULL)
    {
        do_say(mob,"You don't have that item");
        return;
    }
    if (obj->condition >= 100)
  {
        do_say(mob,"But that item's not broken");
        return;
    }
    if (obj->cost == 0)
    {
        do_say(mob,"That item is beyond repair");
        return;
    }

    cost = ( (obj->level * 10) +
                ((obj->cost * (100 - obj->condition)) /100)    );
    cost /= 100;

    sprintf(buf, "It will cost %d to fix that item", cost);
    do_say(mob,buf);
}

/***************************************************************************
 ************************      auction.c      ******************************
***************************************************************************/

DECLARE_SPELL_FUN(	spell_identify	);

void talk_auction(char *argument)
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *original;

    sprintf (buf,"{R[{YAUCTION{R]: {Y%s{w", argument);

    for (d = descriptor_list; d != NULL; d = d->next)
    {
        original = d->original ? d->original : d->character; /* if switched */
        if ((d->connected == CON_PLAYING) && !IS_SET(original->comm,COMM_NOAUCTION) )
            act (buf, original, NULL, NULL, TO_CHAR);
    }
}


/*
  This function allows the following kinds of bets to be made:

  Absolute bet
  ============

  bet 14k, bet 50m66, bet 100k

  Relative bet
  ============

  These bets are calculated relative to the current bet. The '+' symbol adds
  a certain number of percent to the current bet. The default is 25, so
  with a current bet of 1000, bet + gives 1250, bet +50 gives 1500 etc.
  Please note that the number must follow exactly after the +, without any
  spaces!

  The '*' or 'x' bet multiplies the current bet by the number specified,
  defaulting to 2. If the current bet is 1000, bet x  gives 2000, bet x10
  gives 10,000 etc.

*/

int advatoi (const char *s)
/*
  util function, converts an 'advanced' ASCII-number-string into a number.
  Used by parsebet() but could also be used by do_give or do_wimpy.

  Advanced strings can contain 'k' (or 'K') and 'm' ('M') in them, not just
  numbers. The letters multiply whatever is left of them by 1,000 and
  1,000,000 respectively. Example:

  14k = 14 * 1,000 = 14,000
  23m = 23 * 1,000,0000 = 23,000,000

  If any digits follow the 'k' or 'm', the are also added, but the number
  which they are multiplied is divided by ten, each time we get one left.
  This is best illustrated in an example :)

  14k42 = 14 * 1000 + 14 * 100 + 2 * 10 = 14420

  Of course, it only pays off to use that notation when you can skip many 0's.
  There is not much point in writing 66k666 instead of 66666, except maybe
  when you want to make sure that you get 66,666.

  More than 3 (in case of 'k') or 6 ('m') digits after 'k'/'m' are automatically
  disregarded. Example:

  14k1234 = 14,123

  If the number contains any other characters than digits, 'k' or 'm', the
  function returns 0. It also returns 0 if 'k' or 'm' appear more than
  once.
*/

{

/* the pointer to buffer stuff is not really necessary, but originally I
   modified the buffer, so I had to make a copy of it. What the hell, it 
   works:) (read: it seems to work:)
*/

  char string[MAX_INPUT_LENGTH]; /* a buffer to hold a copy of the argument */
  char *stringptr = string; /* a pointer to the buffer so we can move around */
  char tempstring[2];       /* a small temp buffer to pass to atoi*/
  int number = 0;           /* number to be returned */
  int multiplier = 0;       /* multiplier used to get the extra digits right */

  strcpy (string,s);        /* working copy */

  while ( isdigit (*stringptr)) /* as long as the current character is a digit */
  {
      strncpy (tempstring,stringptr,1);           /* copy first digit */
      number = (number * 10) + atoi (tempstring); /* add to current number */
      stringptr++;                                /* advance */
  }

  switch (UPPER(*stringptr)) {
      case 'K'  : multiplier = 1000;    number *= multiplier; stringptr++;
	break;
      case 'M'  : multiplier = 1000000; number *= multiplier; stringptr++;
	break;
      case '\0' : break;
      default   : return 0; /* not k nor m nor NUL - return 0! */
  }

  while ( isdigit (*stringptr) && (multiplier > 1)) 
	/* if any digits follow k/m, add those too */
  {
      strncpy (tempstring,stringptr,1);           /* copy first digit */
      multiplier = multiplier / 10;  
	/* the further we get to right, the less are the digit 'worth' */
      number = number + (atoi (tempstring) * multiplier);
      stringptr++;
  }

  if (*stringptr != '\0' && !isdigit(*stringptr)) 
   /* a non-digit character was found, other than NUL */
    return 0; 
   /* If a digit is found, it means the multiplier is 1 - i.e.
 extra digits that just have to be ignore, liked 14k4443 -> 3 is ignored */

  return (number);
}


int parsebet (const int currentbet, const char *argument)
{

  int newbet = 0;                /* a variable to temporarily hold the new bet */
  char string[MAX_INPUT_LENGTH]; /* a buffer to modify the bet string */
  char *stringptr = string;      /* a pointer we can move around */
  char buf2[MAX_STRING_LENGTH];

  strcpy (string,argument);      /* make a work copy of argument */


  if (*stringptr)               /* check for an empty string */
  {

    if (isdigit (*stringptr)) /* first char is a digit assume e.g. 433k */
      newbet = advatoi (stringptr); /* parse and set newbet to that value */

    else
      if (*stringptr == '+') /* add ?? percent */
      {
        if (strlen (stringptr) == 1) /* only + specified, assume default */
          newbet = (currentbet * 125) / 100; /* default: add 25% */
        else
          newbet = (currentbet * (100 + atoi (++stringptr))) / 100; /* cut off the first char */
      }
      else
        {
        sprintf (buf2,"considering: * x \n\r");
        if ((*stringptr == '*') || (*stringptr == 'x')) /* multiply */
         {
          if (strlen (stringptr) == 1) /* only x specified, assume default */
            newbet = currentbet * 2 ; /* default: twice */
          else /* user specified a number */
            newbet = currentbet * atoi (++stringptr); /* cut off the first char */
         }   
        }
  }
  return newbet;        /* return the calculated bet */
}



void auction_update (void)
{
    char buf[MAX_STRING_LENGTH];
   OBJ_DATA *pit;
   ROOM_INDEX_DATA *original;

    if (auction->item != NULL)
        if (--auction->pulse <= 0) /* decrease pulse */
        {
            auction->pulse = PULSE_AUCTION;
            switch (++auction->going) /* increase the going state */
            {
            case 1 : /* going once */
            case 2 : /* going twice */
            if (auction->bet > 0)
                sprintf (buf, "%s: going %s for %d.",
			auction->item->short_descr,
                     ((auction->going == 1) ? "once" : "twice"),auction->bet);
            else
                sprintf (buf, "%s: going %s (not bet received yet).",
			auction->item->short_descr,
                     ((auction->going == 1) ? "once" : "twice"));
            talk_auction (buf);
            break;

            case 3 : /* SOLD! */

            if (auction->bet > 0)
            {
                sprintf (buf, "%s sold to %s for %d.",
                    auction->item->short_descr,
                    IS_NPC(auction->buyer) ? auction->buyer->short_descr : auction->buyer->name,
                    auction->bet);
                talk_auction(buf);
                obj_to_char (auction->item,auction->buyer);
  act ("The auctioneer appears before you in a puff of smoke and hands you $p.",
                     auction->buyer,auction->item,NULL,TO_CHAR);
                act ("The auctioneer appears before $n, and hands $m $p",
                     auction->buyer,auction->item,NULL,TO_ROOM);

                auction->seller->gold += auction->bet; /* give him the money */

                auction->item = NULL; /* reset item */

            }
            else /* not sold */
            {
   sprintf (buf, "No bets received for %s - object has been removed.",auction->item->short_descr);
                talk_auction(buf);
                sprintf (buf, "The auctioneer puts the unsold item to his pit.");
                talk_auction(buf);
//                extract_obj(auction->item);
//********************************************************************************
         original = auction->seller->in_room;
         char_from_room(auction->seller);
         char_to_room(auction->seller,get_room_index(ROOM_VNUM_PIT));
         pit = get_obj_list(auction->seller, "pit", auction->seller->in_room->contents);
        if (pit != NULL) {
         obj_to_obj(auction->item, pit); }
        else {
        extract_obj(auction->item);
        }
         char_from_room(auction->seller);
         char_to_room(auction->seller,original);
//*******************************************************************************
            auction->item = NULL; /* clear auction */
            } /* else */

            } /* switch */
        } /* if */
} /* func */


void do_auction (CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char betbuf[MAX_STRING_LENGTH];

    argument = one_argument (argument, arg1);

	if (IS_NPC(ch))    /* NPC extracted can't auction! */
		return;

    if (IS_SET(ch->comm,COMM_NOAUCTION))
	{
    	 if (!str_cmp(arg1,"on") )
	 {
	  send_to_char("Auction channel is now ON.\n\r",ch);
	  REMOVE_BIT(ch->comm,COMM_NOAUCTION);
	  return;
	 }
	 else
	 {
	  send_to_char("Your auction channel is OFF.\n\r",ch);
	  send_to_char("You must first change auction channel ON.\n\r",ch);
	  return;
	 }
	}

    if (arg1[0] == '\0')
     {
        if (auction->item != NULL)
        {
            /* show item data here */
            if (auction->bet > 0)
                sprintf (buf, "Current bet on this item is %d gold.\n\r",auction->bet);
            else
                sprintf (buf, "No bets on this item have been received.\n\r");
            send_to_char (buf,ch);
	    spell_identify(0, 0, ch, auction->item,0);
            return;
        }
        else
        {
	    sprintf(buf,"Auction WHAT?\n\r");
            send_to_char (buf,ch);
            return;
        }
      }
    if (!str_cmp(arg1,"off") )
	{
	 send_to_char("Auction channel is now OFF.\n\r",ch);
	 SET_BIT(ch->comm,COMM_NOAUCTION);
	 return;
	}

    if (IS_IMMORTAL(ch) && !str_cmp(arg1,"stop"))
    {
    if (auction->item == NULL)
    {
        send_to_char ("There is no auction going on you can stop.\n\r",ch);
        return;
    }
    else /* stop the auction */
    {
        sprintf(buf,"Sale of %s has been stopped by God. Item confiscated.",
                        auction->item->short_descr);
        talk_auction(buf);
        obj_to_char(auction->item, auction->seller);
        auction->item = NULL;
        if (auction->buyer != NULL) /* return money to the buyer */
        {
            auction->buyer->gold += auction->bet;
            send_to_char ("Your money has been returned.\n\r",auction->buyer);
        }
        return;
    }
   }
    if  (!str_cmp(arg1,"bet") ) 
     {
	if (auction->item != NULL)
        {
            int newbet;

	if ( ch == auction->seller )
	 {
	  send_to_char("You cannot bet on your own selling equipment...:)\n\r",ch);
	  return;
	 }
            /* make - perhaps - a bet now */
         if (argument[0] == '\0')
            {
                send_to_char ("Bet how much?\n\r",ch);
                return;
            }

            newbet = parsebet (auction->bet, argument);
            sprintf (betbuf,"Bet: %d\n\r",newbet);

            if (newbet < (auction->bet + 1))
            {
     send_to_char ("You must at least bid 1 {Ygold{w over the current bet.\n\r",ch);
                return;
            }

            if (newbet > ch->gold)
            {
                send_to_char ("You don't have that much money!\n\r",ch);
                return;
            }

            /* the actual bet is OK! */

            /* return the gold to the last buyer, if one exists */
            if (auction->buyer != NULL)
                auction->buyer->gold += auction->bet;

            ch->gold -= newbet; /* substract the gold - important :) */
            auction->buyer = ch;
            auction->bet   = newbet;
            auction->going = 0;
            auction->pulse = PULSE_AUCTION; /* start the auction over again */

            sprintf (buf,"A bet of %d {Ygold{w has been received on %s.\n\r",
		newbet,auction->item->short_descr);
            talk_auction (buf);
            return;


        }
        else
        {
      send_to_char ("There isn't anything being auctioned right now.\n\r",ch);
            return;
        }
   }
    /* finally... */

    obj = get_obj_carry (ch, arg1 ); /* does char have the item ? */ 

    if (obj == NULL)
    {
        send_to_char ("You aren't carrying that.\n\r",ch);
        return;
    }

    if ( !can_drop_obj( ch, obj ) )
        {
            send_to_char( "You can't let go of it.\n\r", ch );
            return;
        }

    if (ch->level == 84)
	{
	    send_to_char("Builders are not allow to auction items.\n\r",ch);
	    return;
	}

    if (auction->item == NULL)
    switch (obj->item_type)
    {

    default:
        act_new ("You cannot auction $Ts.",ch, NULL,item_type_name(obj), 
		TO_CHAR,POS_SLEEPING);
        return;

    case ITEM_WEAPON:
    case ITEM_ARMOR:
    case ITEM_STAFF:
    case ITEM_WAND:
    case ITEM_SCROLL:
        obj_from_char (obj);
        auction->item = obj;
        auction->bet = 0; 	/* obj->cost / 100 */
        auction->buyer = NULL;
        auction->seller = ch;
        auction->pulse = PULSE_AUCTION;
        auction->going = 0;

        sprintf(buf, "A new item has been received: %s.",
		obj->short_descr);
        talk_auction(buf);

        return;

    } /* switch */
    else
    {
        act ("Try again later - $p is being auctioned right now!",ch,auction->item,NULL,TO_CHAR);
        return;
    }
}

/* Repair skill */
void do_smithing(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *hammer;

    if ( IS_NPC(ch)
    ||   ch->level < skill_table[gsn_smithing].skill_level[ch->class] )
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }


    if ( ch->fighting )
    {
     send_to_char( "Wait until the fight finishes.\n\r", ch );
        return;
    }

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        send_to_char("Which object do you want to repair.\n\r",ch);
        return;
    }

    if (( obj = get_obj_carry(ch, arg)) == NULL)
    {
        send_to_char("You are not carrying that.\n\r",ch);
        return;
    }

   if (obj->condition >= 100)
    {
        send_to_char("But that item is not broken.\n\r",ch);
        return;
    }

    if (( hammer = get_eq_char(ch, WEAR_HOLD)) == NULL)
    {
        send_to_char("You are not holding a hammer.\n\r",ch);
        return;
    }

    if ( hammer->pIndexData->vnum != OBJ_VNUM_HAMMER )
    {
        send_to_char("That is not the correct hammer.\n\r",ch);
        return;
 }

    WAIT_STATE(ch,2 * PULSE_VIOLENCE);
    if ( number_percent() > get_skill(ch,gsn_smithing) )
     {
      check_improve(ch,gsn_smithing,FALSE,8);
    sprintf(buf, "$n try to repair %s with the hammer.But $n fails.",obj->short_descr);
    act(buf,ch,NULL,obj,TO_ROOM);
    sprintf(buf, "You failed to repair %s\n\r", obj->short_descr);
    send_to_char(buf, ch);
    hammer->condition -= 25;
     }
    else
     {
    check_improve(ch,gsn_smithing,TRUE,4);
    sprintf(buf, "$n repairs %s with the hammer.", obj->short_descr);
    act(buf,ch,NULL,NULL,TO_ROOM);
 sprintf(buf, "You repair %s\n\r", obj->short_descr);
    send_to_char(buf, ch);
    obj->condition = UMAX( 100 ,
         obj->condition + ( get_skill(ch,gsn_smithing) / 2) );
    hammer->condition -= 25;
    if(obj->condition > 100)
		obj->condition = 100;
     }
   if (hammer->condition < 1)  extract_obj( hammer );
   return;
}


void do_donate( CHAR_DATA *ch, char *argument)
{
   OBJ_DATA *pit;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *original;
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int amount;

   argument = one_argument(argument, arg);


   if (arg[0] == '\0' )
   {
      send_to_char("Donate what?\n\r",ch);
      return;
   }

   original = ch->in_room;
 if (ch->position == POS_FIGHTING)
   {
      send_to_char(" You're {Yfighting!{x\n\r",ch);
      return;
   }

	 if( !( obj = get_obj_carry( ch, arg ) ) )
   {
      send_to_char("You do not have that!\n\r",ch);
      return;
   }
   else
   {
      if (!can_drop_obj(ch, obj) && ch->level < 81)
      {
         send_to_char("Its stuck to you.\n\r",ch);
         return;
  }
 	if (ch->level == 84)
	{
	send_to_char("Builders at level 84 are not allow to donate.\n\r",ch);
	return;
	}

  if (IS_NPC(ch))
   if (( obj->pIndexData->limit != -1 ) && obj->pIndexData->count > obj->pIndexData->limit)
       {
        send_to_char( "{cHmm you don't donate maxed eq!.{x\n\r", ch );
        return;
      }        
      if ((obj->item_type == ITEM_CORPSE_NPC) ||
         (obj->item_type == ITEM_CORPSE_PC))
      {
         send_to_char("You cannot donate that!\n\r",ch);
         return;
      }

    if ( !CAN_WEAR(obj, ITEM_TAKE) || IS_OBJ_STAT(obj,ITEM_NODONATE))
    {
        act( "{cYou cannot donate $p.{x", ch, obj, 0, TO_CHAR);
        return;
    }


      if (obj->timer > 0)
      {
         send_to_char("You cannot donate that.\n\r",ch);
         return;
      }

      if (ch->in_room != get_room_index(ROOM_VNUM_PIT))
         act("$n donates {Y$p{x.",ch,obj,NULL,TO_ROOM);
      act("You donate {Y$p{x.",ch,obj,NULL,TO_CHAR);

      if ((!IS_OBJ_STAT(obj ,ITEM_ANTI_EVIL) && IS_EVIL(ch)) ||
 	(!IS_OBJ_STAT(obj ,ITEM_ANTI_GOOD) && IS_GOOD(ch)) ||
         IS_NEUTRAL(ch))
         if (obj->cost > 0 && obj->level > 0)
         {
            amount = UMAX(1, obj->cost/6);
		
		// No silver for donating, bug exploiting this way... Derkec 02/01/01
		amount = 0;
		sprintf(buf, "{xThank you for your charity.");
		send_to_char(buf,ch);
		/*
            if (amount == 1)
            {
               sprintf(buf, "You receive {Mone silver{x for your donation.");
               send_to_char(buf,ch);
            }
            else
            {
               sprintf( buf, "You receive {M%d silver{x for your donation.",amount);
               send_to_char(buf,ch);
            }

            ch->silver += amount;
		*/
		
  }

    if ( (obj->pIndexData->limit != -1) 
     && (obj->pIndexData->limit < obj->pIndexData->count)
     && (IS_NPC(ch)))   {
      send_to_char( "Thats in maxed you can't steal itlimited.\n\r", ch );
      return;
    }
         char_from_room(ch);
         char_to_room(ch,get_room_index(ROOM_VNUM_PIT));
         pit = get_obj_list(ch, "pit", ch->in_room->contents);
        if (pit != NULL) {
	obj->cost = 0;
        obj_from_char(obj);
         obj_to_obj(obj, pit); }
        else {
        extract_obj(obj);
        }
         char_from_room(ch);
         char_to_room(ch,original);
         return;
     }
}

void check_weapon_destroy(CHAR_DATA *ch, CHAR_DATA *victim,bool second)
{
 OBJ_DATA *wield,*destroy;
 int skill,chance=0,sn,i;

 if (IS_NPC(victim) || number_percent() < 94 )  return;

 if (!second)
	{
	 if ( (wield = get_eq_char(ch,WEAR_WIELD)) == NULL)
	 return;
 	 sn = get_weapon_sn(ch);
 	 skill = get_skill(ch, sn );
	}
  else  {
	 if ( (wield = get_eq_char(ch,WEAR_SECONDARY)) == NULL)
	 return;
 	 sn = get_second_sn(ch);
 	 skill = get_skill(ch, sn );
	}
 if (is_metal(wield))
      {
	for (i=0;i < MAX_WEAR; i++) 
	{
	 if ( ( destroy = get_eq_char(victim,i) ) == NULL 
		|| number_percent() > 95
		|| number_percent() > 94
	 	|| number_percent() > skill
		|| ch->level < (victim->level - 10) 
		|| check_material(destroy,"platinum") 
		|| destroy->pIndexData->limit != -1
		|| (i == WEAR_WIELD || i== WEAR_SECONDARY
			|| i == WEAR_TATTOO || i == WEAR_STUCK_IN) )
	 continue;
	
	 chance += 20;
	 if ( check_material(wield, "platinium") ||
	      check_material(wield, "titanium") )
	 chance += 5;

	 if ( is_metal(destroy) )  chance -= 20;
	 else 			chance += 20; 

	 chance += ( (ch->level - victim->level) / 5);

	 chance += ( (wield->level - destroy->level) / 2 );

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;
	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;
	 
	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/*	 chance /= 2;	*/
	 if (number_percent() < chance && chance > 50)
		{
		 damage_to_obj(ch,wield,destroy, (chance / 5) );
		 break;
		}
	}
      }
 else {
	for (i=0;i < MAX_WEAR;i++) 
	{
	 if ( ( destroy = get_eq_char(victim,i) ) == NULL 
		|| number_percent() > 95
		|| number_percent() > 94
	 	|| number_percent() < skill
		|| ch->level < (victim->level - 10) 
		|| check_material(destroy,"platinum") 
		|| destroy->pIndexData->limit != -1
		|| (i == WEAR_WIELD || i== WEAR_SECONDARY
	         || i == WEAR_TATTOO || i == WEAR_STUCK_IN ) )
	 continue;
	
	 chance += 10;

	 if ( is_metal(destroy) )  chance -= 20;

	 chance += (ch->level - victim->level);

	 chance += (wield->level - destroy->level);

	/* sharpness	*/
	 if ( IS_WEAPON_STAT(wield,WEAPON_SHARP) )
		chance += 10;

	 if ( sn == gsn_axe ) chance += 10;

	/* spell affects */
	 if ( IS_OBJ_STAT( destroy, ITEM_BLESS) ) chance -= 10;
	 if ( IS_OBJ_STAT( destroy, ITEM_MAGIC) ) chance -= 20;
	 
	 chance += skill - 85 ;
	 chance += get_curr_stat( ch, STAT_STR);

/*	 chance /= 2;	*/
	 if (number_percent() < chance && chance > 50)
		{
		 damage_to_obj(ch,wield,destroy, chance / 5 );
		 break;
		}
	}
      }

 return;
}

