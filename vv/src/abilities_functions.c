
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "fok_struct.h"
#include "interp.h"
#include "declaration.h"

void do_hunt( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  int direction,i;
  bool fArea,ok;
  

    if ( !IS_NPC(ch)
    /**FIX**/
    &&   ch->level < skill_table[gsn_hunt].abiliti_skill[ch->class] )
    {
	send_to_char("Huh?\n\r", ch );
	return;
    }

  one_argument( argument, arg );

  if( arg[0] == '\0' )
    {
      send_to_char( "Whom are you trying to hunt?\n\r", ch );
      return;
    }

/*  fArea = ( get_trust(ch) < MAX_LEVEL ); */
  fArea = !(IS_IMMORTAL(ch));
  /**FIX**/
  if ( ch->level >= skill_table[gsn_world_find].abiliti_skill[ch->class] )
	{
	 if (number_percent() < get_skill(ch,gsn_world_find) )
	  {
	   fArea = 0;
	   check_improve(ch,gsn_world_find,TRUE,1);
	  }
	 check_improve(ch,gsn_world_find,FALSE,1);
	}

  if( fArea )
    victim = get_char_area( ch, arg);
  else
    victim = get_char_world( ch, arg);

  if( victim == NULL )
    {
      send_to_char("No-one around by that name.\n\r", ch );
      return;
    }

  if( ch->in_room == victim->in_room )
    {
      act( "$N is here!", ch, NULL, victim, TO_CHAR );
      return;
    }

  if( IS_NPC( ch  ) )
  {
    ch->hunting = victim;
    hunt_victim( ch );
    return;
  }


  /*
   * Deduct some movement.
   */
  if (!IS_IMMORTAL(ch))
{
  if( ch->endur > 2 )
    ch->endur -= 3;
  else
    {
      send_to_char( "You're too exhausted to hunt anyone!\n\r", ch );
      return;
    }
}

  act( "$n stares intently at the ground.", ch, NULL, NULL, TO_ROOM );
  
  WAIT_STATE( ch, skill_table[gsn_hunt].abiliti_beats[1] );
  direction = find_path( ch->in_room->vnum, victim->in_room->vnum,
			ch, -40000, fArea );

  if( direction == -1 )
    {
      act( "You couldn't find a path to $N from here.",
	  ch, NULL, victim, TO_CHAR );
      return;
    }

  if( direction < 0 || direction > 5 )
    {
      send_to_char( "Hmm... Something seems to be wrong.\n\r", ch );
      return;
    }

  /*
   * Give a random direction if the player misses the die roll.
   */
  if ( IS_NPC (ch) && number_percent () > 75)        /* NPC @ 25% */
    {
    log_string("Do PC hunt");
    ok=FALSE;
    for(i=0;i<6;i++) {
    	if (ch->in_room->exit[direction]!=NULL) {
    		ok=TRUE;
    		break;
    		}
    	}
    if (ok)	
    {
      do
	{
	  direction = number_door();
	}
      while( ( ch->in_room->exit[direction] == NULL )
	    || ( ch->in_room->exit[direction]->u1.to_room == NULL) );
    }
else {
	  log_string("Do hunt, player hunt, no exits from room!");
  	  ch->hunting=NULL;
  	  send_to_char("Your room has not exits!!!!\n\r",ch);
  	  return;
  	}
  /*
   * Display the results of the search.
   */
  }
  sprintf( buf, "$N is %s from here.", dir_name[direction] );
  act( buf, ch, NULL, victim, TO_CHAR );  
  return;
}

void do_dark_vision( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_dark_vision].abiliti_name[ch->class] )
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
	return;
    }

    WAIT_STATE(ch, skill_table[gsn_dark_vision].abiliti_beats[1]);
    af.where     = TO_AFFECTS;
    af.type      = gsn_dark_vision;
    af.level     = ch->level;
    af.duration  = ch->level / 3;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char( "Your eyes glows red.\n\r", ch );
    return;
}

void do_detect_hidden( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_detect_hidden].abiliti_level[ch->class] )
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
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    WAIT_STATE(ch, skill_table[gsn_detect_hidden].abiliti_beats[1]);
    af.where     = TO_DETECTS;
    af.type      = gsn_detect_hidden;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = DETECT_HIDDEN;
    affect_to_char( ch, &af );
    send_to_char( "Your awareness improves.\n\r", ch );
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
      ch->level < skill_table[gsn_bear_call].abiliti_name[ch->class] )
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
  WAIT_STATE(ch, skill_table[gsn_bear_call].abiliti_beats[1]);
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
  bear->level = (UMIN(70,1 * ch->level-2)-5);
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
  SET_BIT(bear->affected_by, AFF_CHARM);
  char_to_room(bear,ch->in_room);
  bear->master = bear2->master = ch;
  send_to_char("Two bears come to your rescue!\n\r",ch);
  act("Two bears come to $n's rescue!",ch,NULL,NULL,TO_ROOM);
  } else {
  SET_BIT(bear->affected_by, AFF_CHARM);
  bear->master = ch;
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
      ch->level < skill_table[gsn_acute_vision].abiliti_name[ch->class] )
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
    WAIT_STATE(ch, skill_table[gsn_acute_vision].abiliti_beats[1]);
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

void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }
 if (MOUNTED(ch))
    {
        send_to_char("You can't pick while mounted.\n\r", ch);
        return;
    }


    WAIT_STATE( ch, skill_table[gsn_pick_lock].abiliti_beats[1] );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > get_skill(ch,gsn_pick_lock))
    {
	send_to_char( "You failed.\n\r", ch);
	check_improve(ch,gsn_pick_lock,FALSE,2);
	return;
    }

//    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    if ( ( obj = get_obj_here( ch,NULL, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {	
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_PICKPROOF))
	    {
		send_to_char("You failed.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	    check_improve(ch,gsn_pick_lock,TRUE,2);
	    return;
	}

	
	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
        act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
        act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	check_improve(ch,gsn_pick_lock,TRUE,2);
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL(ch))
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 && !IS_IMMORTAL(ch))
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	check_improve(ch,gsn_pick_lock,TRUE,2);

	/* pick the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}

void do_hide( CHAR_DATA *ch, char *argument )
{
int forest;
/*  adds hidden obj code */
    char       arg [ MAX_INPUT_LENGTH ];
    one_argument( argument, arg );

   if (MOUNTED(ch))
    {
        send_to_char("You can't hide while mounted.\n\r", ch);
        return;
    }

    if (RIDDEN(ch))
    {
        send_to_char("You can't hide while being ridden.\n\r", ch);
        return;
    }

    if ( arg[0] != '\0' )
    {
	do_hide_obj(ch,argument);
        return;
    }
/*  end */

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_hide].abiliti_level[ch->class] )
    {
	send_to_char(
	    "You don't know how to hide.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE )  )  {
      send_to_char( "You can not hide while glowing.\n\r", ch );
      return;
    }
    if ( IS_AFFECTED( ch, AFF_FAERIE_FOG )  )  {
      send_to_char( "You can not hide while glowing.\n\r", ch );
      return;
    }
    forest = 0;
    forest += ch->in_room->sector_type == SECT_FOREST?60:0;
    forest += ch->in_room->sector_type == SECT_FIELD?60:0;

    send_to_char( "You attempt to hide.\n\r", ch );

    if ( number_percent( ) < get_skill(ch,gsn_hide)-forest)
    {
	SET_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,TRUE,3);
    }
    else  {
      if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,FALSE,3);
    }

    return;
}


void do_camouflage( CHAR_DATA *ch, char *argument )
{
  if (MOUNTED(ch))
    {
        send_to_char("You can't camouflage while mounted.\n\r", ch);
        return;
    }
    if (RIDDEN(ch))
    {
        send_to_char("You can't camouflage while being ridden.\n\r", ch);
        return;
    }


    if ( IS_NPC(ch) || 
         ch->level < skill_table[gsn_camouflage].abiliti_level[ch->class] )
      {
	send_to_char("You don't know how to camouflage yourself.\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_FAERIE_FIRE ) )  {
      send_to_char( "You can't camouflage yourself while glowing.\n\r", ch );
      return; 
    }
   if ( IS_AFFECTED( ch, AFF_FAERIE_FOG ) )  {
      send_to_char( "You can't camouflage yourself while glowing.\n\r", ch );
      return; 
    }

    if (ch->in_room->sector_type != SECT_FOREST &&
	ch->in_room->sector_type != SECT_HILLS  &&
	ch->in_room->sector_type != SECT_MOUNTAIN)
      {
	send_to_char("There is no cover here.\n\r",ch);
	act("$n tries to camouflage $mself against the lone leaf on the ground.",ch,NULL,NULL,TO_ROOM);
	return;
      }
    send_to_char( "You attempt to camouflage yourself.\n\r", ch );
    WAIT_STATE( ch, skill_table[gsn_camouflage].abiliti_beats[1] );

    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) )
	REMOVE_BIT(ch->affected_by, AFF_CAMOUFLAGE);


    if ( IS_NPC(ch) || 
	number_percent( ) < ch->pcdata->learned[gsn_camouflage] )
    {
	SET_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	check_improve(ch,gsn_camouflage,TRUE,1);
    }
    else
	check_improve(ch,gsn_camouflage,FALSE,1);

    return;
}

void do_track(CHAR_DATA *ch, char *argument)
{
  ROOM_HISTORY_DATA *rh;
  char buf[MAX_STRING_LENGTH];
  static char *door[] = { "north","east","south","west","up","down",
                            "that way" };

  if (!IS_NPC(ch) && get_skill(ch,gsn_track) < 2) {
    send_to_char("There are no train tracks here.\n\r",ch);
    return;
  }

  WAIT_STATE(ch,skill_table[gsn_track].abiliti_beats[1]);
  act("$n checks the ground for tracks.",ch,NULL,NULL,TO_ROOM);

  if (IS_NPC(ch) || number_percent() < get_skill(ch,gsn_track) )
    {
   if (IS_NPC(ch))
        ch->status = 0;
      for (rh = ch->in_room->history;rh != NULL;rh = rh->next)
        if (is_name(argument,rh->name)) {
          check_improve(ch,gsn_track,TRUE,1);
          sprintf(buf,"%s's tracks lead %s.\n\r",capitalize(rh->name),
                  door[URANGE(0,rh->went,7)]);
          send_to_char(buf, ch);
          move_char(ch,rh->went,FALSE);
          return;
      }
    }
  send_to_char("You don't see any tracks.\n\r",ch);
  if (IS_NPC(ch))
    ch->status = 5;
  check_improve(ch,gsn_track,FALSE,1);
}

/* Relock, Coded by Andeus in 3 mins, idea from CF */
void do_relock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Which door or container?\n\r", ch );
        return;
    }


    if ( !IS_NPC(ch) && number_percent( ) > get_skill(ch,gsn_relock))
    {
        send_to_char( "You failed.\n\r", ch);
        check_improve(ch,gsn_relock,FALSE,2);
        return;
    }



//    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    if ( ( obj = get_obj_here( ch,NULL, arg ) ) != NULL )
    {
        /* portal stuff */
        if (obj->item_type == ITEM_PORTAL)
        {
            if (!IS_SET(obj->value[1],EX_ISDOOR)
            ||  IS_SET(obj->value[1],EX_NOCLOSE))
            {
                send_to_char("You can't do that.\n\r",ch);
                return;
            }
            if (!IS_SET(obj->value[1],EX_CLOSED))
            {
                send_to_char("It's not closed.\n\r",ch);
                return;
            }

            if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
            {
                send_to_char("It can't be relocked.\n\r",ch);
                return;
            }
/*
            if (!has_key(ch,obj->value[4]))
            {
                send_to_char("You lack the key.\n\r",ch);
                return;
            }
*/
            if (IS_SET(obj->value[1],EX_LOCKED))
            {
                send_to_char("It's already locked.\n\r",ch);
                return;
            }

            SET_BIT(obj->value[1],EX_LOCKED);
            act("You lock $p.",ch,obj,NULL,TO_CHAR);
            act("$n locks $p.",ch,obj,NULL,TO_ROOM);
            return;
        }

        /* 'lock object' */
        if ( obj->item_type != ITEM_CONTAINER )
            { send_to_char( "That's not a container.\n\r", ch ); return; }
        if ( !IS_SET(obj->value[1], CONT_CLOSED) )
            { send_to_char( "It's not closed.\n\r",        ch ); return; }
        if ( obj->value[2] < 0 )
            { send_to_char( "It can't be relocked.\n\r",     ch ); return; }
/*      if ( !has_key( ch, obj->value[2] ) )
            { send_to_char( "You lack the key.\n\r",       ch ); return; }*/
        if ( IS_SET(obj->value[1], CONT_LOCKED) )
            { send_to_char( "It's already locked.\n\r",    ch ); return; }

        SET_BIT(obj->value[1], CONT_LOCKED);
        act("You lock $p.",ch,obj,NULL,TO_CHAR);
        act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
        return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
        /* 'Relock door' */
        ROOM_INDEX_DATA *to_room;
        EXIT_DATA *pexit;
        EXIT_DATA *pexit_rev;

        pexit   = ch->in_room->exit[door];
        if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
            { send_to_char( "It's not closed.\n\r",        ch ); return; }
        if ( pexit->key < 0 )
            { send_to_char( "It can't be relocked.\n\r",     ch ); return; }
/*      if ( !has_key( ch, pexit->key) && 
             !has_key_ground( ch, pexit->key) )
            { send_to_char( "You lack the key.\n\r",       ch ); return; }*/
        if ( IS_SET(pexit->exit_info, EX_LOCKED) )
            { send_to_char( "It's already locked.\n\r",    ch ); return; }

        SET_BIT(pexit->exit_info, EX_LOCKED);
        send_to_char( "*Click*\n\r", ch );
        act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

        /* lock the other side */
        if ( ( to_room   = pexit->u1.to_room            ) != NULL
        &&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
        &&   pexit_rev->u1.to_room == ch->in_room )
        {
            SET_BIT( pexit_rev->exit_info, EX_LOCKED );
            for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
                act( "The $d clicks.", rch, NULL, pexit_rev->keyword, TO_CHAR );

        }
        return;
    }
    return;
}

void do_shoot( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *wield;
    OBJ_DATA *arrow; 
    char arg1[512],arg2[512],buf[512];
    bool success;
    int chance,direction;
    int range = (ch->level / 10) + 1;
    
   if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_bow].abiliti_level[ch->class] )
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

   argument=one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
	send_to_char( "Shoot what direction and whom?\n\r", ch );
	return;
    }

    if (ch->fighting)
    {
	send_to_char("You cannot concentrate on shooting arrows.\n\r",ch);
	return;
    }

   direction = find_exit( ch, arg1 );

   if (direction<0 || direction > 5) 
	{
	 send_to_char("Shoot which direction and whom?\n\r",ch);
	 return;
	}
	
    if ( ( victim = find_char( ch, arg2, direction, range) ) == NULL )
	return;

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
    {
	sprintf(buf,"Gods protect %s.\n\r",victim->name);
	send_to_char(buf,ch);
	return;
    }

   wield = get_eq_char(ch, WEAR_WIELD);
   arrow = get_eq_char(ch, WEAR_HOLD);    

   if (!wield || wield->item_type!=ITEM_WEAPON || wield->value[0]!=WEAPON_BOW)
    	{
	 send_to_char("You need a bow to shoot!\n\r",ch);
	 return;    	
	}

   if (get_eq_char(ch,WEAR_SECONDARY) || get_eq_char(ch,WEAR_SHIELD) )
    	{
	 send_to_char("Your second hand should be free!\n\r",ch);
	 return;    	
	}

   if (!arrow)
    	{
	 send_to_char("You need an arrow holding for your ammunition!\n\r",ch);
	 return;    	
	}	
	
    if (arrow->item_type!=ITEM_WEAPON || arrow->value[0] != WEAPON_ARROW) 
	{
	 send_to_char("That's not the right kind of arrow!\n\r",ch);
	 return;
	}
	
    
    WAIT_STATE( ch, skill_table[gsn_bow].abiliti_beats[1] );
   
    chance = (get_skill(ch,gsn_bow) - 50) * 2;
    if (ch->position == POS_SLEEPING)
	chance += 30;
    if (ch->position == POS_RESTING)
	chance += 10;
    if (victim->position == POS_FIGHTING)
	chance -= 40;
    chance += GET_HITROLL(ch)/ 7;

    sprintf( buf, "You shoot $p to %s.", dir_name[ direction ] );
    act( buf, ch, arrow, NULL, TO_CHAR );
    sprintf( buf, "$n shoots $p to %s.", dir_name[ direction ] );
    act( buf, ch, arrow, NULL, TO_ROOM );

    obj_from_char(arrow);
    success = send_arrow(ch,victim,arrow,direction,chance,
		dice(wield->value[1],wield->value[2]) );
    check_improve(ch,gsn_bow,TRUE,1);
}

/*
void do_camp( CHAR_DATA *ch, char *argument )
{
  AFFECT_DATA af,af2;

  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_camp].abiliti_level[ch->class] )
    {
      send_to_char( "Huh?\n\r", ch );
      return;
    }
 if (is_affected(ch, gsn_camp))
    {
      send_to_char("You don't have enough power to handle more camp areas.\n\r",ch);
      return;
    }


  if ( number_percent( ) > get_skill( ch, gsn_camp) )
  {
        send_to_char( "You failed to make your camp.\n\r", ch );
        check_improve(ch,gsn_camp,TRUE,4);
        return;
  }

  if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)      ||
       IS_SET(ch->in_room->room_flags, ROOM_PRIVATE)   ||

    IS_SET(ch->in_room->room_flags, ROOM_SOLITARY)  ||
         ( ch->in_room->sector_type != SECT_FIELD &&
           ch->in_room->sector_type != SECT_FOREST &&
           ch->in_room->sector_type != SECT_MOUNTAIN &&
           ch->in_room->sector_type != SECT_HILLS ) )
  {
    send_to_char( "There are not enough leaves to camp here.\n\r", ch );
    return;
  }

  if ( ch->mana < 150 )
  {
     send_to_char( "You don't have enough mana to make a camp.\n\r", ch );
     return;
  }

  check_improve(ch,gsn_camp,TRUE,4);
 ch->mana -= 150;

  WAIT_STATE(ch,skill_table[gsn_camp].abiliti_beats[1]);

  send_to_char("You succeeded to make your camp.\n\r",ch);
  act("$n succeeded to make $s camp.",ch,NULL,NULL,TO_ROOM);

  af.where              = TO_AFFECTS;
  af.type               = gsn_camp;
  af.level              = ch->level;
  af.duration           = 12;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

  af2.where             = TO_ROOM_CONST;
 af2.type               = gsn_camp;
  af2.level              = ch->level;
  af2.duration           = ch->level / 20;
  af2.bitvector          = 0;
  af2.modifier           = 2 * ch->level;
  af2.location           = APPLY_ROOM_HEAL;
  affect_to_room(ch->in_room, &af2);

  af2.modifier           = ch->level;
  af2.location           = APPLY_ROOM_MANA;
  affect_to_room(ch->in_room, &af2);

}
*/

void do_layhands(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_layhands].abiliti_level[ch->class] )
    {
        send_to_char("You lack the skill to heal others with touching.\n\r",ch);
        return;
    }

//    if ( (victim = get_char_room(ch,argument)) == NULL)
    if ( (victim = get_char_room(ch,NULL,argument)) == NULL)    
      {
        send_to_char("You do not see that person here.\n\r",ch);
        return;
      }

 if ( is_affected(ch, gsn_layhands))
        {
         send_to_char("You can't concentrate enough.\n\r",ch);
         return;
        }

    WAIT_STATE(ch,skill_table[gsn_layhands].abiliti_beats[1]);

 if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_layhands])
  {
    af.type = gsn_layhands;
    af.where = TO_AFFECTS;
    af.level = ch->level;
//    af.duration = 21 - (ch->level/4);
    af.duration = 15;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = 0;
    affect_to_char (ch,&af );

    victim->hit += ch->max_hit/3;

//    victim->hit = UMIN( victim->hit + 100 , victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    check_improve(ch,gsn_layhands,TRUE,1);
	}
	else
	send_to_char("You fail miserablely.\n\r",ch);
}


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
	    WAIT_STATE(ch,skill_table[gsn_envenom].abiliti_beats[1]);
	    return;
	}

	act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	if (!obj->value[3])
	    check_improve(ch,gsn_envenom,FALSE,4);
	WAIT_STATE(ch,skill_table[gsn_envenom].abiliti_beats[1]);
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
	    WAIT_STATE(ch,skill_table[gsn_envenom].abiliti_beats[1]);
            return;
        }
	else
	{
	    act("You fail to envenom $p.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,FALSE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].abiliti_beats[1]);
	    return;
	}
    }
 
    act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
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
//    if ((vch = get_char_room(ch,arg)) == NULL)
    if ((vch = get_char_room(ch,NULL,arg)) == NULL)    
          {
           if (ch->position == POS_FIGHTING)
            {
             if (skill_table[sn].abiliti_target == TAR_CHAR_SELF)
              {
               vch = ch;
              }
            else
             {
              vch = ch;
             }  
            }
          }     
	    switch ( skill_table[sn].abiliti_target )
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

//    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg2 ) ) == NULL )
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

    WAIT_STATE( ch, skill_table[gsn_steal].abiliti_beats[1] );
    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );
    percent += can_see( victim, ch ) ? -10 : 0;
    percent -= 5;

    if ( IS_NPC(victim) )
        if ( victim->pIndexData->pShop != NULL )
           percent -= 50;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_steal].abiliti_level[ch->class] )
    {
     percent = -20;
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

    if ( ( obj = get_obj_carry( victim, arg1 ,NULL) ) == NULL )
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
//  else if ( (victim = get_char_room(ch,arg)) == NULL)
  else if ( (victim = get_char_room(ch,NULL,arg)) == NULL)
    {
      send_to_char("They're not here.\n\r",ch);
      return;
    }
  WAIT_STATE( ch, skill_table[gsn_herbs].abiliti_beats[1] );

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
// AFFECT_DATA *paf;
//  int chance;
  int value0, value1, value2;

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
//  if ( (obj = get_obj_here(ch,arg)) == NULL )
  if ( (obj = get_obj_here(ch,NULL,arg)) == NULL )
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

    WAIT_STATE(ch, skill_table[gsn_sharpen].abiliti_beats[1]);
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

void do_smithing(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *hammer;

    if ( IS_NPC(ch)
    ||   ch->level < skill_table[gsn_smithing].abiliti_level[ch->class] )
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

    if (( obj = get_obj_carry(ch, arg,NULL)) == NULL)
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

void do_berserk( CHAR_DATA *ch, char *argument)
{
    int chance, hp_percent;

    if ((chance = get_skill(ch,gsn_berserk)) == 0
    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    ||  (!IS_NPC(ch)
    &&   ch->level < skill_table[gsn_berserk].abiliti_level[ch->class]))
    {
	send_to_char("{cYou turn red in the face, but nothing happens.{x\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
    ||  is_affected(ch,skill_lookup("frenzy")))
    {
	send_to_char("{cYou get a little madder.{x\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
	send_to_char("{cYou're feeling too mellow to berserk.{x\n\r",ch);
	return;
    }

    if (ch->mana < 50)
    {
	send_to_char("{cYou can't get up enough energy.{x\n\r",ch);
	return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
	chance += 10;

    /* damage -- below 50% of hp helps, above hurts */
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->mana -= 50;
	ch->move /= 2;

	/* heal a little damage */
	ch->hit += ch->level * 2;
	ch->hit = UMIN(ch->hit,ch->max_hit);

	send_to_char("{cYour pulse races as you are consumned by rage!{x\n\r",ch);
	act("{c$n gets a wild look in $s eyes.{x",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_berserk,TRUE,2);

	af.where	= TO_AFFECTS;
	af.type		= gsn_berserk;
	af.level	= ch->level;
	af.duration	= number_fuzzy(ch->level / 8);
	af.modifier	= UMAX(1,ch->level/5);
	af.bitvector 	= AFF_BERSERK;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= UMAX(10,10 * (ch->level/5));
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,3 * PULSE_VIOLENCE);
	ch->mana -= 25;
	ch->move /= 2;

	send_to_char("{cYour pulse speeds up, but nothing happens.{x\n\r",ch);
	check_improve(ch,gsn_berserk,FALSE,2);
    }
}

void do_bash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    int Xchance = 0;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_bash)) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_bash].abiliti_level[ch->class]))
    {
	send_to_char("{cBashing? What's that?\n\r{x",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }

//    else if ((victim = get_char_room(ch,arg)) == NULL)
    else if ((victim = get_char_room(ch,NULL,arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].abiliti_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= chance)
        {
         check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_bash].abiliti_beats[1] * 3/2);
         send_to_char( "{cSomeone trying to sneek up on you.{x\n\r", victim );
	 return;
        }
        
     }
 

    if (victim->position < POS_FIGHTING)
    {
	act("{cYou'll have to let $M get back up first.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cYou try to bash your brains out, but fail.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;
#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif
    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is your friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim, gsn_protective_shield))
     {
	act("{cYour bash seems to slide around $N.{x", ch, NULL, victim, TO_CHAR);
	act("{c$n's bash slides off your protective shield.{x", ch, NULL, victim,
	    TO_VICT);
	act("{c$n's bash seems to slide around $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }
/*
    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_bash].abiliti_beats[1] * 3/2);
       return;
      }
*/
/*
    if(victim->kingdom == KINGDOM_DRAKKON)
    if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_bash].abiliti_beats[1] * 3/2);
       return;
      }
*/
    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
//    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;
    chance -= get_curr_stat(victim,STAT_DEX);
    
    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
//    chance += (ch->level - victim->level) * 2;
    chance += (ch->level - victim->level);
    /* now the attack */

    if (number_percent() < chance)
    {

	act("{c$n sends you sprawling with a powerful bash!{x",
		ch,NULL,victim,TO_VICT);
	act("{cYou slam into $N, and send $M flying!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n sends $N sprawling with a powerful bash.{x",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_bash,TRUE,1);

	wait = 3;

	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

if (!IS_NPC(victim))     
      if(number_percent() < victim->pcdata->learned[gsn_reaction])
	{
	check_improve(victim,gsn_reaction,TRUE,1);
	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	act("{cYou reacted and dodge $n's bash!{x",victim,NULL,ch,TO_VICT);
	act("{c$N, reacts and dodges your bash!{x",victim,NULL,ch,TO_CHAR);
	act("{c$n reacts and dodges $N's powerful bash.{x",victim,NULL,ch,TO_NOTVICT);
	return;
	}
	WAIT_STATE(ch,skill_table[gsn_bash].abiliti_beats[1]);
	victim->position = POS_RESTING;
	/*setting lags*/
        if (victim->lagattack < 5)
	  victim->lagattack = 5;
	if (victim->lagflee   < 5)
	 victim->lagflee   = 5;
	if (victim->lagstand   <9)
	  victim->lagstand  = 9;
	if (victim->lagspell   < 5)
	  victim->lagspell = 5;

	damage(ch,victim,number_range(4,4 + 2 * ch->size + chance/10),gsn_bash,
	    DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_bash,DAM_BASH, TRUE);
	act("{cYou fall flat on your face!{x",
	    ch,NULL,victim,TO_CHAR);
	act("{c$n falls flat on $s face.{x",
	    ch,NULL,victim,TO_NOTVICT);
	act("{cYou evade $n's bash, causing $m to fall flat on $s face.{x",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_bash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_bash].abiliti_beats[1] * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	do_yell(victim,"Help! Someone is bashing me!");
	else
	  {
	    sprintf(buf, "Help! %s is bashing me!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	do_yell(victim,buf);
	  }
      }
}

void do_dirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK_DIRT))
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_dirt].abiliti_level[ch->class]))
    {
	send_to_char("{cYou get your feet dirty.{x\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't in combat!{x\n\r",ch);
	    return;
	}
    }

//    else if ((victim = get_char_room(ch,arg)) == NULL)
    else if ((victim = get_char_room(ch,NULL,arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
	act("{c$e's already been blinded.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cVery funny.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;
#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is such a good friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */

    /* dexterity */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_DEX);

    /* speed  */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,OFF_FAST))
	chance -= 25;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* sloppy hack to prevent false zeroes */
    if (chance % 5 == 0)
	chance += 1;

    /* terrain */

    switch(ch->in_room->sector_type)
    {
	case(SECT_INSIDE):		chance -= 20;	break;
	case(SECT_CITY):		chance -= 10;	break;
	case(SECT_FIELD):		chance +=  5;	break;
	case(SECT_FOREST):				break;
	case(SECT_HILLS):				break;
	case(SECT_JUNGLE):				break;
	case(SECT_SWAMP):				break;
	case(SECT_ROCK_MOUNTAIN):			break;
	case(SECT_SNOW_MOUNTAIN):			break;
	case(SECT_MOUNTAIN):		chance -= 10;	break;
	case(SECT_DESERT):		chance += 10;   break;
	case(SECT_CAVERN):		chance += 2;	break;
	default:			chance  =  0;   break;
    }

    if (chance == 0)
    {
	send_to_char("{cThere isn't any dirt to kick.{x\n\r",ch);
	return;
    }

    /* now the attack */
    if (number_percent() < chance)
    {
	AFFECT_DATA af;
	act("{c$n is blinded by the dirt in $s eyes!{x",victim,NULL,NULL,TO_ROOM);
        damage(ch,victim,number_range(2,5),gsn_dirt,DAM_NONE, TRUE);
	send_to_char("{cYou can't see a thing!{x\n\r",victim);
	check_improve(ch,gsn_dirt,TRUE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].abiliti_beats[1]);

	af.where	= TO_AFFECTS;
	af.type 	= gsn_dirt;
	af.level 	= ch->level;
	af.duration	= 0;
	af.location	= APPLY_HITROLL;
	af.modifier	= -4;
	af.bitvector 	= AFF_BLIND;

	affect_to_char(victim,&af);
    }
    else
    {
	damage(ch,victim,0,gsn_dirt,DAM_NONE, TRUE);
	check_improve(ch,gsn_dirt,FALSE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].abiliti_beats[1]);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
    	if (!can_see(victim,ch))
	do_yell(victim,"Someone just kicked dirt in my eyes!");
	else
	  {
	    sprintf(buf, "Die, %s!  You dirty fool!{x", (is_affected(ch,
		gsn_doppelganger) && !IS_IMMORTAL(victim)) ? ch->doppel->name
		: ch->name);
	do_yell(victim,buf);
	  }
      }

}

void do_trip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_trip)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TRIP) &&
	ch->master != NULL)
    ||   (!IS_NPC(ch)
	  && ch->level < skill_table[gsn_trip].abiliti_level[ch->class]))
    {
	send_to_char("{cTripping?  What's that?{x\n\r",ch);
	return;
    }


    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{c{cBut you aren't fighting anyone!{x{x\n\r",ch);
	    return;
 	}
    }

//    else if ((victim = get_char_room(ch,arg)) == NULL)
    else if ((victim = get_char_room(ch,NULL,arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;
#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
	send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
	return;
    }
#endif
    if (IS_AFFECTED(victim,AFF_FLYING))
    {
	act("{c$S feet aren't on the ground.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
	act("{c$N is already down.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cYou fall flat on your face!{x\n\r",ch);
	WAIT_STATE(ch,2 * skill_table[gsn_trip].abiliti_beats[1]);
	act("{c$n trips over $s own feet!{x",ch,NULL,NULL,TO_ROOM);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{c$N is your beloved master.{x",ch,NULL,victim,TO_CHAR);
	return;
    }
/*
    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_trip].abiliti_beats[1]);
       return;
      }

    if(victim->kingdom == KINGDOM_DRAKKON)
     if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_trip].abiliti_beats[1]);
       return;
      }

*/

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;
    chance -= ch->lagattack * 6;

    /* now the attack */
    if (number_percent() < chance)
    {
	act("{c$n trips you and you go down!{x",ch,NULL,victim,TO_VICT);
	act("{cYou trip $N and $N goes down!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n trips $N, sending $M to the ground.{x",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_trip,TRUE,1);

//	WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        WAIT_STATE(ch,skill_table[gsn_trip].abiliti_beats[1]);
	victim->position = POS_RESTING;
        if (victim->lagattack < 2)
	  victim->lagattack = 2;
	if (victim->lagflee   < 9)
	 victim->lagflee   = 9;
	if (victim->lagstand   <3)
	  victim->lagstand  = 3;
	if (victim->lagspell   < 2)
	  victim->lagspell = 2;
	damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip,
	    DAM_BASH, TRUE);
    }
    else
    {
	damage(ch,victim,0,gsn_trip,DAM_BASH, TRUE);
	WAIT_STATE(ch,skill_table[gsn_trip].abiliti_beats[1]*2/3);
	check_improve(ch,gsn_trip,FALSE,1);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	do_yell(victim," Help! Someone just tripped me!");
	else
	  {
	    sprintf(buf, "Help! %s just tripped me!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	do_yell(victim,buf);
	  }
      }
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "{cRescue whom?{x\n\r", ch );
	return;
    }

//    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cWhat about fleeing instead?{x\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
	send_to_char( "{cDoesn't need your help!{x\n\r", ch );
	return;
    }

    if ( ch->fighting == victim )
    {
	send_to_char( "{cToo late.{x\n\r", ch );
	return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
	send_to_char( "{cThat person is not fighting right now.{x\n\r", ch );
	return;
    }
    if (IS_NPC(ch) && ch->master != NULL && IS_NPC(victim))
      return;

    if (is_safe(ch, fch))
	return;

    if (ch->master != NULL)
      {
	if (is_safe(ch->master, fch)) return;
      }

    WAIT_STATE( ch, skill_table[gsn_rescue].abiliti_beats[1] );
    if ( !IS_NPC(ch) && number_percent( ) > ch->pcdata->learned[gsn_rescue] )
    {
	send_to_char( "{cYou fail the rescue.{x\n\r", ch );
	check_improve(ch,gsn_rescue,FALSE,1);
	return;
    }

    act( "{cYou rescue $N!{x",  ch, NULL, victim, TO_CHAR    );
    act( "{c$n rescues you!{x", ch, NULL, victim, TO_VICT    );
    act( "{c$n rescues $N!{x",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    check_killer( ch, fch );
    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}



void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_kick].abiliti_level[ch->class] )
    {
	send_to_char(
	    "{cYou better leave the martial arts to fighters.{x\n\r", ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_kick].abiliti_beats[1] );
    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_kick] )
    {
	damage( ch,victim,number_range(1,1.5 * ch->level),gsn_kick,DAM_BASH, TRUE
);
	check_improve(ch,gsn_kick,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_kick,DAM_BASH, TRUE );
	check_improve(ch,gsn_kick,FALSE,1);
    }

    return;
}

void do_circle( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *person;


    if ( IS_NPC(ch)
    ||   ch->level < skill_table[gsn_circle].abiliti_level[ch->class] )
    {
	send_to_char("{cYou don't know how to circle.{x\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    if ((get_eq_char(ch,WEAR_WIELD) == NULL) ||
	attack_table[get_eq_char(ch,WEAR_WIELD)->value[3]].damage
	   != DAM_PIERCE)
      {
       send_to_char("{cYou must wield a piercing weapon to circle stab.{x\n\r",ch);
       return;
     }
    
    // Removed for safety. DERKEC 6/25/00
    //if (is_safe(ch,victim)) return;

    WAIT_STATE( ch, skill_table[gsn_circle].abiliti_beats[1] );

    for (person = ch->in_room->people;person != NULL;
	 person = person->next_in_room)
      {
	if (person->fighting == ch)
	  {
	    send_to_char("{cYou can't circle while defending yourself.{x\n\r",ch);
	    return;
	  }
      }

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_circle] )
    {
	one_hit(ch, victim, gsn_circle,FALSE);
	check_improve(ch,gsn_circle,TRUE,3);
    }
    else
    {
	damage( ch, victim, 0, gsn_circle,TYPE_UNDEFINED, TRUE);
	check_improve(ch,gsn_circle,FALSE,3);
    }

    return;
}




void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon;

    hth = 0;

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ((chance = get_skill(ch,gsn_disarm)) == 0)
    {
	send_to_char( "{cYou don't know how to disarm opponents.{x\n\r", ch );
	return;
    }

    if ( get_eq_char( ch, WEAR_WIELD ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM))))
    {
	send_to_char( "{cYou must wield a weapon to disarm.{x\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    {
	send_to_char( "{cYour opponent is not wielding a weapon.{x\n\r", ch );
	return;
    }
  if (obj->pIndexData->limit >= 0) {
	send_to_char("You can't disarm them\n\r",ch);
        return;
    }


    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));

    /* modifiers */

    /* skill */
    if ( get_eq_char(ch,WEAR_WIELD) == NULL)
	chance = chance * hth/150;
    else
	chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_STR);

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* and now the attack */
    if (number_percent() < chance)
    {
    	WAIT_STATE( ch, skill_table[gsn_disarm].abiliti_beats[1] );
	disarm( ch, victim );
	check_improve(ch,gsn_disarm,TRUE,1);
    }
    else
    {
	WAIT_STATE(ch,skill_table[gsn_disarm].abiliti_beats[1]);
	act("{cYou fail to disarm $N.{x",ch,NULL,victim,TO_CHAR);
	act("{c$n tries to disarm you, but fails.{x",ch,NULL,victim,TO_VICT);
	act("{c$n tries to disarm $N, but fails.{x",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_disarm,FALSE,1);
    }
    return;
}

void do_nerve(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  one_argument(argument,arg);
  if (ch->level < skill_table[gsn_nerve].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }
  if (ch->fighting == NULL)
    {
      send_to_char("{cYou aren't fighting anyone.{x\n\r",ch);
      return;
    }

  victim = ch->fighting;

  if (is_safe(ch,victim)) return;

  if (is_affected(ch,gsn_nerve))
    {
      send_to_char("{cYou cannot weaken that character any more.{x\n\r",ch);
      return;
    }
  check_killer(ch,victim);
  WAIT_STATE( ch, skill_table[gsn_nerve].abiliti_beats[1] );

  if (IS_NPC(ch) ||
      number_percent() < (ch->pcdata->learned[gsn_nerve] + ch->level
                         + get_curr_stat(ch,STAT_DEX))/2)
    {
      AFFECT_DATA af,af2,tohit,todam;
      af.where  = TO_AFFECTS;
      af.type 	= gsn_nerve;
      af.level 	= ch->level;
      af.duration = ch->level * PULSE_VIOLENCE/PULSE_TICK;
      af.location = APPLY_STR;
      af.modifier = -3;
      af.bitvector = 0;
      affect_to_char(victim,&af);
      
      af2.where  = TO_AFFECTS;
      af2.type 	= gsn_nerve;
      af2.level 	= ch->level;
      af2.duration = ch->level * PULSE_VIOLENCE/PULSE_TICK;
      af2.location = APPLY_DEX;
      af2.modifier = -3;
      af2.bitvector = 0;
      affect_to_char(victim,&af2);

      tohit.where     = TO_AFFECTS;
      tohit.type      = gsn_nerve;
      tohit.level     = ch->level;
      tohit.duration  = ch->level * PULSE_VIOLENCE/PULSE_TICK;
      tohit.location  = APPLY_HITROLL;
      tohit.modifier  = -5;
      tohit.bitvector = 0;
      affect_to_char( victim, &tohit );

      todam.where = TO_AFFECTS;
      todam.type = gsn_nerve;
      todam.level = ch->level;
      todam.duration = ch->level * PULSE_VIOLENCE/PULSE_TICK;
      todam.location = APPLY_DAMROLL;
      todam.modifier = -5;
      todam.bitvector = 0;
      affect_to_char( victim, &todam);


      act("{cYou weaken $N with your nerve pressure.{x",ch,NULL,victim,TO_CHAR);
      act("{c$n weakens you with $s nerve pressure.{x",ch,NULL,victim,TO_VICT);
      act("{c$n weakens $N with $s nerve pressure.{x",ch,NULL,victim,TO_NOTVICT);
      check_improve(ch,gsn_nerve,TRUE,1);
    }
  else
    {
      send_to_char("{cYou press the wrong points and fail.{x\n\r",ch);
      act("{c$n tries to weaken you with nerve pressure, but fails.{x",
	  ch,NULL,victim,TO_VICT);
      act("{c$n tries to weaken $N with nerve pressure, but fails.{x",
	  ch,NULL,victim,TO_NOTVICT);
      check_improve(ch,gsn_nerve,FALSE,1);
    }

    multi_hit(victim,ch,TYPE_UNDEFINED);

    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position != POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	do_yell(victim,"Help! I'm being attacked by someone!");
	else
	{
	  sprintf( buf, "Help! I'm being attacked by %s!{x",
		  (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		  ch->doppel->name : ch->name );
	do_yell(victim,buf);
	}
      }
  return;
}

void do_endure(CHAR_DATA *ch, char *arg)
{
  AFFECT_DATA af;

  if (IS_NPC(ch))
    {
      send_to_char("{cYou have no endurance whatsoever.{x\n\r",ch);
      return;
    }

  if ( ch->level < skill_table[gsn_endure].abiliti_level[ch->class] ||
       ch->pcdata->learned[gsn_endure] <= 1 )
    {
      send_to_char("{cYou lack the concentration.{x\n\r",ch);
      return;
    }

  if (is_affected(ch,gsn_endure))
    {
      send_to_char("{cYou cannot endure more concentration.{x\n\r",ch);
      return;
    }


  WAIT_STATE( ch, skill_table[gsn_endure].abiliti_beats[1] );

  af.where 	= TO_AFFECTS;
  af.type 	= gsn_endure;
  af.level 	= ch->level;
  af.duration = ch->level / 4;
  af.location = APPLY_SAVING_SPELL;
  af.modifier = -1 * (ch->pcdata->learned[gsn_endure] / 10);
  af.bitvector = 0;

  affect_to_char(ch,&af);

  send_to_char("{cYou prepare yourself for magical encounters.{x\n\r",ch);
  act("{c$n concentrates for a moment, then resumes $s position.{x",
      ch,NULL,NULL,TO_ROOM);
  check_improve(ch,gsn_endure,TRUE,1);
}

void do_tame(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument,arg);

  if (arg[0] == '\0')
    {
      send_to_char("{cYou are beyond taming.{x\n\r",ch);
      act("{c$n tries to tame $mself but fails miserably.{x",ch,NULL,NULL,TO_ROOM);
      return;
    }

//  if ( (victim = get_char_room(ch,arg)) == NULL)
  if ( (victim = get_char_room(ch,NULL,arg)) == NULL)
    {
      send_to_char("{cThey're not here.{x\n\r",ch);
      return;
    }

  if (IS_NPC(ch))
    {
      send_to_char("{cWhy don't you tame yourself first?{x",ch);
      return;
    }

  if (!IS_NPC(victim))
    {
      act("{c$N is beyond taming.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (!IS_SET(victim->act,ACT_AGGRESSIVE))
    {
      act("{c$N is not usually aggressive.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_tame].abiliti_beats[1] );

  if (number_percent() < ch->pcdata->learned[gsn_tame] + 15
                         + 4*(ch->level - victim->level))
    {
      REMOVE_BIT(victim->act,ACT_AGGRESSIVE);
      SET_BIT(victim->affected_by,AFF_CALM);
      send_to_char("{cYou calm down.{x\n\r",victim);
      act("{cYou calm $N down.{x",ch,NULL,victim,TO_CHAR);
      act("{c$n calms $N down.{x",ch,NULL,victim,TO_NOTVICT);
      stop_fighting(victim,TRUE);
      check_improve(ch,gsn_tame,TRUE,1);
    }
  else
    {
      send_to_char("{cYou failed.{x\n\r",ch);
      act("{c$n tries to calm down $N but fails.{x",ch,NULL,victim,TO_NOTVICT);
      act("{c$n tries to calm you down but fails.{x",ch,NULL,victim,TO_VICT);
      check_improve(ch,gsn_tame,FALSE,1);
    }
}

void do_assassinate( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_assassinate].abiliti_level[ch->class] )
      {
	send_to_char("{cYou don't know how to assassinate.{x\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "{cYou don't want to kill your beloved master.{x\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cAssassinate whom?{x\n\r", ch );
	return;
    }

//    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Suicide is against your way.{x\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( IS_IMMORTAL( victim ) && !IS_NPC(victim) )
    {
	send_to_char( "{cYour hands pass through.{x\n\r", ch );
	return;
    }
#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{cKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't assassinate a fighting person.{x\n\r", ch );
	return;
    }

    if ( (get_eq_char( ch, WEAR_WIELD ) != NULL) ||
	 (get_eq_char( ch, WEAR_HOLD  ) != NULL) )  {
	send_to_char(
	"{cYou need both hands free to assassinate somebody.{x\n\r", ch );
	return;
    }

    if ( (victim->hit < victim->max_hit) &&
	 (can_see(victim, ch)) )

    {
	act( "{c$N is hurt and suspicious ... you can't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

/*
    if (IS_SET(victim->imm_flags, IMM_WEAPON))
      {
	act("{c$N seems immune to your assassination attempt.{x", ch, NULL,
		 victim, TO_CHAR);
	act("{c$N seems immune to $n's assassination attempt.{x", ch, NULL,
		victim, TO_ROOM);
	return;
      }
*/
    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_assassinate].abiliti_beats[1] );
    if ( IS_NPC(ch) ||
	!IS_AWAKE(victim)
	||   number_percent( ) < ch->pcdata->learned[gsn_assassinate])
      multi_hit(ch,victim,gsn_assassinate);
    else
      {
	check_improve(ch,gsn_assassinate,FALSE,1);
	damage( ch, victim, 0, gsn_assassinate,DAM_NONE, TRUE );
      }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING)
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! Someone tried to assassinate me!");
	else
	  {
	    sprintf( buf, "Help! %s tried to assassinate me!{x",
		    (is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		    ch->doppel->name : ch->name );
	do_yell(victim,buf);
	  }
      }
    return;
  }


void do_caltraps(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim = ch->fighting;

  if (IS_NPC(ch) || ch->pcdata->learned[gsn_caltraps] < 1)
    {
      send_to_char("{cCaltraps? Is that a dance step?{x\n\r",ch);
      return;
    }

  if (victim == NULL)
    {
      send_to_char("{cYou must be in combat.{x\n\r",ch);
      return;
    }

  if (is_safe(ch,victim))
    return;

  act("{cYou throw a handful of sharp spikes at the feet of $N.{x",
      ch,NULL,victim,TO_CHAR);
  act("{c$n throws a handful of sharp spikes at your feet!{x",
      ch,NULL,victim,TO_VICT);

  WAIT_STATE(ch,skill_table[gsn_caltraps].abiliti_beats[1]);

  if (!IS_NPC(ch) && number_percent() >= ch->pcdata->learned[gsn_caltraps])
    {
      damage(ch,victim,0,gsn_caltraps,DAM_PIERCE, TRUE);
      check_improve(ch,gsn_caltraps,FALSE,1);
      return;
    }

  damage(ch,victim,ch->level,gsn_caltraps,DAM_PIERCE, TRUE);

  if (!is_affected(victim,gsn_caltraps))
    {
      AFFECT_DATA tohit,todam,todex;

      tohit.where     = TO_AFFECTS;
      tohit.type      = gsn_caltraps;
      tohit.level     = ch->level;
      tohit.duration  = -1; /* update removes it after combat */
      tohit.location  = APPLY_HITROLL;
      tohit.modifier  = -5;
      tohit.bitvector = 0;
      affect_to_char( victim, &tohit );

      todam.where = TO_AFFECTS;
      todam.type = gsn_caltraps;
      todam.level = ch->level;
      todam.duration = -1;
      todam.location = APPLY_DAMROLL;
      todam.modifier = -5;
      todam.bitvector = 0;
      affect_to_char( victim, &todam);

      todex.type = gsn_caltraps;
      todex.level = ch->level;
      todex.duration = -1;
      todex.location = APPLY_DEX;
      todex.modifier = -5;
      todex.bitvector = 0;
      affect_to_char( victim, &todex);

      act("{c$N starts limping.{x",ch,NULL,victim,TO_CHAR);
      act("{cYou start to limp.{x",ch,NULL,victim,TO_VICT);
      check_improve(ch,gsn_caltraps,TRUE,1);
    }
}


void do_throw( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_throw].abiliti_level[ch->class] )
    {
	send_to_char(
	    "{cA clutz like you couldn't throw down a worm.{x\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
	return;
    }

    if (get_eq_char (ch, WEAR_SECONDARY) != NULL)
    {
	send_to_char( "{cLets see, two weapons and an enemy.. you have 3 arms?{x\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
      return;


    WAIT_STATE( ch, skill_table[gsn_throw].abiliti_beats[1] );

    if (is_affected(victim, gsn_protective_shield))
     {
	act("{cYou fail to reach $s arm.{x", ch, NULL, victim, TO_CHAR);
	act("{c$n fails to throw you.{x", ch, NULL, victim, TO_VICT);
	act("{c$n fails to throw $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }
/*
    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE( ch, skill_table[gsn_throw].abiliti_beats[1] );
       return;
      }
      
    if(victim->kingdom == KINGDOM_DRAKKON)
     if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE( ch, skill_table[gsn_throw].abiliti_beats[1] );
       return;
      }
      
*/

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_throw] - ch->lagattack * 6)
    {
      act("{cYou throw $N to the ground with stunning force.{x",
	  ch,NULL,victim,TO_CHAR);
      act("{c$n throws you to the ground with stunning force.{x",
	  ch,NULL,victim,TO_VICT);
      act("{c$n throws $N to the ground with stunning force.{x",
	  ch,NULL,victim,TO_NOTVICT);
//      WAIT_STATE(victim,2 * PULSE_VIOLENCE);
	victim->position = POS_RESTING;
        if (victim->lagattack < 4)
	  victim->lagattack = 4;
	if (victim->lagflee   < 4)
	 victim->lagflee   = 4;
	if (victim->lagstand   < 4)
	  victim->lagstand  = 4;
	if (victim->lagspell   < 10)
	  victim->lagspell = 10;

      damage( ch, victim,ch->level + get_curr_stat(ch,STAT_STR),
	     gsn_throw,DAM_BASH, TRUE );
      check_improve(ch,gsn_throw,TRUE,1);
    }
    else
    {
	act( "{cYou fail to grab your opponent.{x", ch, NULL, NULL, TO_CHAR);
	act( "{c$N tries to throw you, but fails.{x", victim, NULL, ch, TO_CHAR);
	act( "{c$n tries to grab $N's arm.{x", ch, NULL, victim, TO_NOTVICT);
	check_improve(ch,gsn_throw,FALSE,1);
    }

    return;
}

void do_strangle(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];
    int bonus = 0;
    int temp = 0;
    int temp1 = 0;
    int Xchance = 0;
    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_strangle].abiliti_level[ch->class] )
    {
	send_to_char("{cYou lack the skill to strangle.{x\n\r",ch);
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )  {
	send_to_char( "{cYou don't want to grap your beloved masters' neck.{x\n\r",ch);
	return;
    }

//    if ( (victim = get_char_room(ch,argument)) == NULL)
    if ( (victim = get_char_room(ch,NULL,argument)) == NULL)
      {
	send_to_char("{cYou do not see that person here.{x\n\r",ch);
	return;
      }

    if (ch==victim)
      {
	send_to_char("{cEven you are not that stupid.{x\n\r",ch);
	return;
      }

    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't strangle a fighting person.{x\n\r", ch );
	return;
    }

if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].abiliti_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= Xchance)
        {
        check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_strangle].abiliti_beats[1] * 3/2);
         send_to_char( "{cSomeone trying to sneek up on you.{x\n\r", victim );
	 return;
        }
        
     }

    if (is_affected(ch,gsn_strangle))
      return;

    if (is_safe(ch,victim))
      return;

    victim->last_fight_time = current_time;
    ch->last_fight_time = current_time;

    WAIT_STATE(ch,skill_table[gsn_strangle].abiliti_beats[1]);

if (IS_AFFECTED(victim,AFF_FLYING) && !(IS_AFFECTED(ch,AFF_FLYING)))
	{
	send_to_char("Strangle someone who is flying... try jump higher.\n\r",ch);
	return;
	}

/*
    if(ch->kingdom != KINGDOM_KUJIKIRI)
	bonus = -20;
	else
	bonus = 10;
*/


/* Kazmir adds 30/11/99 */	
     bonus += URANGE( 0, (get_curr_stat(ch,STAT_DEX)-20)*2, 10);	
     bonus -= URANGE( 0, (get_curr_stat(victim,STAT_CON)-20)*2, 10);	
/* Kazmir adds 30/11/99 */


    if ( IS_NPC(victim) )
	if ( victim->pIndexData->pShop != NULL )
	   bonus -= 40;
	   
//	   	   bonus += 100;
/* Kazmir out
    if (IS_NPC(ch) ||
	number_percent() < 0.4 * (ch->pcdata->learned[gsn_strangle]+bonus))
*/	
    temp += bonus;
//    sprintf(buf, "A %d\n\r",temp );
//    send_to_char(buf,ch);
    temp += ch->pcdata->learned[gsn_strangle];
//    sprintf(buf, "B %d\n\r",temp );
//    send_to_char(buf,ch);
    temp = temp * 0.5;
//    sprintf(buf, "C %d\n\r",temp );
//    send_to_char(buf,ch);
    temp += 2 * (ch->level - victim->level);
    temp1 = number_range(1,100);
//    sprintf(buf, "D %d\n\r",temp1 );
//    send_to_char(buf,ch);
    
//    if (IS_NPC(ch) || (number_range(1,100) < (0.5 * (ch->pcdata->learned[gsn_strangle]+bonus) ) ) )	
    if (IS_NPC(ch) || (temp1 < temp ))

      {
	act("{cYou grab hold of $N's neck and put $M to sleep.{x",
	    ch,NULL,victim,TO_CHAR);
	act("{c$n grabs hold of your neck and puts you to sleep.{x",
	    ch,NULL,victim,TO_VICT);
	act("{c$n grabs hold of $N's neck and puts $M to sleep.{x",
	    ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_strangle,TRUE,1);

	af.type = gsn_strangle;
        af.where = TO_AFFECTS;
	af.level = ch->level;
/*	af.duration = 1; Kazmir out*/
        af.duration = 2;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_SLEEP;
	affect_join ( victim,&af );

	if (IS_AWAKE(victim))
	  victim->position = POS_SLEEPING;
      }
    else
      {
//	char buf[MAX_STRING_LENGTH];

	damage(ch,victim,0,gsn_strangle,DAM_NONE, TRUE);
	check_improve(ch,gsn_strangle,FALSE,1);
	if (!can_see(victim, ch))
	do_yell(victim, "Help! I'm being strangled by someone!");
	else
	  {
	    sprintf(buf, "Help! I'm being strangled by %s!{x",
		    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim))?
		    ch->doppel->name : ch->name );
	    if (!IS_NPC(victim)) 
	do_yell(victim,buf);
	  }
      }
}

void do_blackjack(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int chance  = 0;
    int Xchance = 0;
    int durachance; /* Kazmir in*/

    if ( IS_NPC(ch) ||
	 ch->level < skill_table[gsn_blackjack].abiliti_level[ch->class] )

    {
	send_to_char("{CHuh?{x\n\r",ch);
	return;
    }

//    if ( (victim = get_char_room(ch,argument)) == NULL)
    if ( (victim = get_char_room(ch,NULL,argument)) == NULL)
      {
	send_to_char("{cYou do not see that person here.{x\n\r",ch);
	return;
      }

    if (ch==victim)
      {
	send_to_char("{cYou idiot?! Blackjack your self?!{x\n\r",ch);
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )  {
	send_to_char( "{cYou don't want to hit your beloved masters' head with a full filled jack.{x\n\r",ch);
	return;
    }

    if (IS_AFFECTED(victim,AFF_SLEEP))  {
      act("{c$E is already asleep.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't blackjack a fighting person.{x\n\r", ch );
	return;
    }
if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].abiliti_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= chance)
        {
        check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_blackjack].abiliti_beats[1] * 3/2);
         send_to_char( "{cSomeone trying to sneek up on you.{x\n\r", victim );
	 return;
        }
        
     }

    if (is_safe(ch,victim))
      return;
    victim->last_fight_time = current_time;
    ch->last_fight_time = current_time;

    WAIT_STATE(ch,skill_table[gsn_blackjack].abiliti_beats[1]);

/*    chance = 0.4 * ch->pcdata->learned[gsn_blackjack]; */
      chance = 0.7 * ch->pcdata->learned[gsn_blackjack];
    
/*    chance += URANGE( 0, (get_curr_stat(ch,STAT_DEX)-20)*2, 10); Kazmir  out*/
/* Kazmir add 30/11/99*/
 chance += URANGE( 0, (get_curr_stat(ch,STAT_DEX)-20)*2, 10);
 chance -= URANGE( 0, (get_curr_stat(victim,STAT_CON)-20)*2, 10);
/* Kazmir add 30/11/99*/     

    chance += can_see(victim, ch) ? 0 : 5;
/*
  if(ch->kingdom != KINGDOM_KUJIKIRI)
        chance -= 20;
        else
        chance += 10;
*/
if (IS_AFFECTED(victim,AFF_FLYING) && !(IS_AFFECTED(ch,AFF_FLYING)))
	{
	send_to_char("Blackjack someone who is flying... try jump higher.\n\r",ch);
	return;
	}

    if ( IS_NPC(victim) )
	if ( victim->pIndexData->pShop != NULL )
	   chance -= 40;

/* Kazmir add 29/11/99*/
  if (get_curr_stat(ch,STAT_STR) < 24) 
     durachance = 3;
     
  if (get_curr_stat(ch,STAT_STR) < 20)
     durachance = 2;
     
  if (get_curr_stat(ch,STAT_STR) < 15)
     durachance = 1;          
/* Kazmir add 29/11/99*/     



    if (IS_NPC(ch) ||
	number_percent() < chance)
      {
	act("{cYou hit $N's head with a lead filled sack.{x",
	    ch,NULL,victim,TO_CHAR);
	act("{cYou feel a sudden pain erupt through your skull!{x",
	    ch,NULL,victim,TO_VICT);
	act("{c$n whacks $N at the back of $S head with a heavy looking sack! Ouch!",
	    ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_blackjack,TRUE,1);

	af.type = gsn_blackjack;
        af.where = TO_AFFECTS;
	af.level = ch->level;
	af.duration = 2; /* Kazmir out */
/*	af.duration = durachance;*/ /* Kazmir in*/
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = AFF_SLEEP;
	affect_join ( victim,&af );

	if (IS_AWAKE(victim))
	  victim->position = POS_SLEEPING;
      }
    else
      {
	char buf[MAX_STRING_LENGTH];

	damage(ch,victim,20,gsn_blackjack,DAM_NONE, TRUE);
	check_improve(ch,gsn_blackjack,FALSE,1);
	if (!IS_NPC(victim))
	 {
	  if (!can_see(victim, ch))
	   do_yell(victim,"Help! I'm being blackjacked by someone!");
	  else
	   {
	    sprintf(buf, "Help! I'm being blackjacked by %s!{x",
	    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim))?
	    ch->doppel->name : ch->name );
	    if (!IS_NPC(victim)) 
	    do_yell(victim,buf);
	   }
	 }  
  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Nerveless Nuisance");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

      }
}

/*  adds */
void do_whirlwind( CHAR_DATA *ch, char *argument )
{ 
   CHAR_DATA *pChar; 
   CHAR_DATA *pChar_next;
   OBJ_DATA *wield;
   bool found = FALSE;

   if (    !IS_NPC( ch )
        && ch->level < skill_table[gsn_whirlwind].abiliti_level[ch->class] )
   {
      send_to_char( "You do not know how to do that...\n\r", ch );
      return;
   }

   if ( ( wield = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
   {
      send_to_char( "You need to wield a weapon first...\n\r", ch );
      return;
   }

   act( "{g$n holds $p firmly, and swings in a huge arc...{x", ch,wield,NULL, TO_ROOM );
   act( "{gYou hold $p firmly, and swings in a huge arc...{x",  ch,wield,NULL, TO_CHAR );

   pChar_next = NULL;
   for ( pChar = ch->in_room->people; pChar; pChar = pChar_next )
   {
      pChar_next = pChar->next_in_room;
      if ( IS_NPC( pChar ) )
      {
         found = TRUE;
         act( "{C$n turns towards YOU!{x", ch, NULL, pChar, TO_VICT    );
         one_hit( ch, pChar, gsn_whirlwind, FALSE );
      }
   }

   if ( !found )
   {
      act( "{g$n looks dizzy, and a tiny bit embarassed.{x", ch, NULL,NULL,TO_ROOM );
      act( "{gYou feel dizzy, and a tiny bit embarassed.{x", ch, NULL,NULL,TO_CHAR );
   }

   WAIT_STATE( ch, skill_table[gsn_whirlwind].abiliti_beats[1] );

   if ( !found && number_percent() < 25 )
   {
      act( "$n loses $s balance and falls into a heap.",  ch, NULL, NULL,TO_ROOM );
      act( "You lose your balance and fall into a heap.", ch, NULL, NULL,TO_CHAR );
      ch->position = POS_STUNNED;
   }

   return;
}


void do_bloodthirst( CHAR_DATA *ch, char *argument)
{
    int chance, hp_percent;

    if (!kingdom_ok(ch,gsn_bloodthirst))
      return;

    if ((chance = get_skill(ch,gsn_bloodthirst)) == 0
	||  IS_NPC(ch) ||
	(!IS_NPC(ch)
	 && ch->level < skill_table[gsn_bloodthirst].abiliti_level[ch->class]))
    {
	send_to_char("{cYou're not that thirsty.{x\n\r",ch);
	return;
    }

  if (IS_AFFECTED(ch,AFF_BLOODTHIRST))
    {
        send_to_char("Your thirst for blood continues.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
        send_to_char("You're feeling to mellow to be bloodthirsty.\n\r",ch);
        return;
    }

    if (ch->fighting == NULL)
      {
	send_to_char("{cYou need to be fighting.{x\n\r",ch);
	return;
      }

    /* modifiers */

    /* damage -- below 50% of hp helps, above hurts */
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch, PULSE_VIOLENCE);


	send_to_char("{cYou hunger for blood!{x\n\r",ch);
	act("{c$n gets a bloodthirsty look in $s eyes.{x",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_bloodthirst,TRUE,2);

	af.where        = TO_AFFECTS;
        af.type         = gsn_bloodthirst;
        af.level        = ch->level;
        af.duration     = ch->level / 5;
        af.modifier     = UMIN(15,ch->level);
        af.bitvector    = AFF_BLOODTHIRST;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= - UMIN(ch->level - 5,35);
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,3 * PULSE_VIOLENCE);

	send_to_char("{cYou feel bloodthirsty for a moment, but it passes.{x\n\r",
		     ch);
	check_improve(ch,gsn_bloodthirst,FALSE,2);
    }
}




void do_spellbane(CHAR_DATA *ch, char *argument)
{
  if (!kingdom_ok(ch,gsn_spellbane))
    return;
  if (ch->level < skill_table[gsn_spellbane].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }
  if (is_affected(ch,gsn_spellbane))
    {
      send_to_char("{cYou are already deflecting spells.{x\n\r",ch);
      return;
    }

  if (ch->mana < 50)
    {
      send_to_char("{cYou cannot muster up the energy.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_spellbane].abiliti_beats[1] );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_spellbane])
    {
      AFFECT_DATA af;

      af.where = TO_AFFECTS;
      af.type 	= gsn_spellbane;
      af.level 	= ch->level;
      af.duration = ch->level+5;
      af.location = APPLY_SAVING_SPELL;
      af.modifier = -ch->level/4;
      af.bitvector = 0;

      affect_to_char(ch,&af);
      ch->mana -= 50;

      act("{cYour hatred of magic surrounds you.{x",ch,NULL,NULL,TO_CHAR);
      act("{c$n fills the air with $s hatred of magic.{x",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_spellbane,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

      send_to_char("{cYou get red in the face, but nothing happens.{x\n\r",ch);
      act("{c$n makes some wild and angry gestures, but nothing happens.{x",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_spellbane,FALSE,1);
    }

}

void do_resistance(CHAR_DATA *ch, char *argument)
{
  if (!kingdom_ok(ch,gsn_resistance))
    return;
  if (ch->level < skill_table[gsn_resistance].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }
  if (is_affected(ch,gsn_resistance))
    {
      send_to_char("{cYou are as resistant as you will get.{x\n\r",ch);
      return;
    }
/*    
    if ( IS_AFFECTED(ch, AFF_SANCTUARY) )
     {
      send_to_char("You are already in sanctuary or resistance you can't
be both.\n\r",ch);
      return;
     }    
*/
  if (ch->mana < 50)
    {
      send_to_char("{cYou cannot muster up the energy.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_resistance].abiliti_beats[1] );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_resistance])
    {
      AFFECT_DATA af;

      af.where	= TO_AFFECTS;
      af.type 	= gsn_resistance;
      af.level 	= ch->level;
      af.duration = ch->level / 6;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = 0;

      affect_to_char(ch,&af);
      ch->mana -= 50;

      act("{cYou feel tough!{x",ch,NULL,NULL,TO_CHAR);
      act("{c$n looks tougher.{x",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_resistance,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

     send_to_char("{cYou flex your muscles, but you don't feel tougher.{x\n\r",ch);
      act("{c$n flexes $s muscles, trying to look tough.{x",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_resistance,FALSE,1);
    }

}

void do_stronghold(CHAR_DATA *ch, char *argument)
{
 if (!kingdom_ok(ch, gsn_stronghold))
    return;

  if (IS_NPC(ch) || ch->level < skill_table[gsn_stronghold].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (ch->move < 50)
    {
	send_to_char("You feel exhausted to go back to the stronghold.\n\r",ch);
      return;
    }
}
void do_trophy(CHAR_DATA *ch, char *argument)
{
  int trophy_vnum;
  OBJ_DATA *trophy;
  AFFECT_DATA af;
  OBJ_DATA *part;
  char arg[MAX_INPUT_LENGTH];
  int level;
  one_argument( argument, arg );

  if (!kingdom_ok(ch, gsn_trophy))
    return;

  if (IS_NPC(ch) || ch->level < skill_table[gsn_trophy].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (is_affected(ch, gsn_trophy))
    {
      send_to_char("{cBut you've already got one trophy!{x\n\r", ch);
      return;
    }

  if (ch->mana < 30)
    {
      send_to_char("{cYou feel too weak to concentrate on a trophy.{x\n\r", ch);
      return;
    }

  if ( arg[0] == '\0' )
    {
      send_to_char( "{cMake a trophy of what?{x\n\r", ch );
      return;
    }

  if ( ( part = get_obj_carry( ch, arg,NULL ) ) == NULL )
    {
      send_to_char( "{cYou do not have that body part.{x\n\r", ch );
      return;
    }

    if ( number_percent( ) < (get_skill( ch, gsn_trophy )/3)*2 )
    {
      send_to_char("{cYou failed and destroyed it.{x\n\r", ch);
      extract_obj(part);
      return;
    }

  WAIT_STATE(ch, skill_table[gsn_trophy].abiliti_beats[1]);

  if (part->pIndexData->vnum == OBJ_VNUM_SLICED_ARM)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_SLICED_LEG)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_SEVERED_HEAD)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_TORN_HEART)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_GUTS)
    trophy_vnum = OBJ_VNUM_HORDE_PONCHO;
  else if (part->pIndexData->vnum == OBJ_VNUM_BRAINS)
    {
      send_to_char("{cWhy don't you just eat those instead?{x\n\r", ch);
      return;
    }
  else
    {
      send_to_char("{cYou can't make a trophy out of that!{x\n\r", ch);
      return;
    }

  if (part->from[0] == '\0')
    {
      send_to_char("{cInvalid body part.{x\n\r", ch);
      return;
    }

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_trophy])
    {
      af.where  = TO_AFFECTS;
      af.type	= gsn_trophy;
      af.level	= ch->level;
      af.duration	= ch->level;
      af.modifier	= 0;
      af.bitvector 	= 0;

      af.location	= 0;
      affect_to_char(ch,&af);

      if ( trophy_vnum != 0 )
	{
	  char buf[MAX_STRING_LENGTH];
	  level = UMIN(part->level + 5, 60);

	  trophy = create_object( get_obj_index( trophy_vnum ), level );
	  trophy->timer = ch->level * 2;

	  sprintf( buf, trophy->short_descr, part->from );
	  free_string( trophy->short_descr );
	  trophy->short_descr = str_dup( buf );

	  sprintf( buf, trophy->description, part->from );
	  free_string( trophy->description );
	  trophy->description = str_dup( buf );
	  trophy->cost  = 0;
	  ch->mana -= 30;
	  af.where	= TO_OBJECT;
	  af.type 	= -1;
	  af.level	= level;
	  af.duration	= -1;
	  af.location	= APPLY_DAMROLL;
	  af.modifier  = ch->level / 5;
	  af.bitvector	= 0;
	  affect_to_obj( trophy, &af );
	  af.location	= APPLY_HITROLL;
	  af.modifier  = ch->level / 5;
	  af.bitvector	= 0;
	  affect_to_obj( trophy, &af );
/*	  af.location	= APPLY_HIT;
	  affect_to_obj( trophy, &af );*/
	  af.location	= APPLY_INT;
	  af.modifier	= level>20?-2:-1;
	  affect_to_obj( trophy, &af );
	  af.location	= APPLY_STR;
	  af.modifier	= level>20?2:1;
	  affect_to_obj( trophy, &af );

/*	  affect_to_obj( trophy, &af ); */
	  trophy->value[0] = ch->level;
	  trophy->value[1] = ch->level;
	  trophy->value[2] = ch->level;
	  trophy->value[3] = ch->level;


	  obj_to_char(trophy, ch);
          check_improve(ch, gsn_trophy, TRUE, 1);

	  act("{cYou make a poncho from $p!{x",ch,part,NULL,TO_CHAR);
	  act("{c$n makes a poncho from $p!{x",ch,part,NULL,TO_ROOM);

	  extract_obj(part);
	  return;
	}
    }
    else
    {
      send_to_char("{cYou destroyed it.{x\n\r", ch);
      extract_obj(part);
      ch->mana -= 15;
      check_improve(ch, gsn_trophy, FALSE, 1);
    }
}


void do_truesight(CHAR_DATA *ch, char *argument)
{
  if (!kingdom_ok(ch,gsn_truesight))
    return;

  if (IS_NPC(ch) ||
        ch->level < skill_table[gsn_truesight].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (is_affected(ch,gsn_truesight))
    {
      send_to_char("{cYour eyes are as sharp as they will get.{x\n\r",ch);
      return;
    }

  if (ch->mana < 50)
    {
      send_to_char("{cYou cannot seem to focus enough.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_truesight].abiliti_beats[1] );

  if (!IS_NPC(ch) && number_percent() < ch->pcdata->learned[gsn_truesight])
    {
      AFFECT_DATA af;

      af.where  = TO_DETECTS;
      af.type 	= gsn_truesight;
      af.level 	= ch->level;
      af.duration = ch->level;
      af.location = APPLY_NONE;
      af.modifier = 0;
      af.bitvector = DETECT_HIDDEN;
      affect_to_char(ch, &af);

      af.bitvector = DETECT_INVIS;
      affect_to_char(ch, &af);

      af.bitvector = DETECT_MAGIC;
      affect_to_char(ch,&af);

      ch->mana -= 50;

      act("{cYou look around sharply!{x",ch,NULL,NULL,TO_CHAR);
      act("{c$n looks more enlightened.{x",ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_truesight,TRUE,1);
    }
  else
    {
      ch->mana -= 25;

     send_to_char("{cYou look about sharply, but you don't see anything new.{x\n\r"
		  ,ch);
      act("{c$n looks around sharply but doesn't seem enlightened.{x",
	  ch,NULL,NULL,TO_ROOM);
      check_improve(ch,gsn_truesight,FALSE,1);
    }

}

void do_vanish(CHAR_DATA *ch, char *argument)
{
  ROOM_INDEX_DATA *pRoomIndex;

  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_vanish].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (ch->mana < 25)
    {
      send_to_char("{cYou lack the mental power for now.{x\n\r",ch);
      return;
    }

  if (is_affected(ch, gsn_charm_person))
    {
      send_to_char("{cYour master wouldn't like that.{x\n\r",ch);
      return;
    }
  if (number_percent() > ch->pcdata->learned[gsn_vanish])
    {
      send_to_char("{cYou throw down a small globe, which produces some smoke.{x\n\r", ch);
      act("{c$n throws a small globe, resulting only some smoke.{x", ch, NULL, NULL, TO_ROOM);
      return;
    }
  if ( ch->in_room == NULL
      ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL))
    {
      send_to_char( "{cYou failed.{x\n\r", ch );
      return;
    }

  for ( ; ; )
    {
      pRoomIndex = get_room_index( number_range( 0, 65535 ) );
      if ( pRoomIndex != NULL )
	if ( can_see_room(ch,pRoomIndex) && !room_is_private(pRoomIndex)
	    && ch->in_room->area == pRoomIndex->area)

	  break;
    }


  act( "{c$n throws down a small globe.{x", ch, NULL, NULL, TO_ROOM );
  send_to_char("{cYou throw down a small globe, and vanish in a puff of smoke.{x\n\r",ch);

  if (!IS_NPC(ch) && ch->fighting != NULL && number_bits(1) == 1) {
    send_to_char("{cYou failed.{x\n\r",ch);
    return;
  }

  act( "{c$n is gone!{x",ch,NULL,NULL,TO_ROOM);

  char_from_room( ch );
  char_to_room( ch, pRoomIndex );
  act( "{c$n appears from nowhere.{x", ch, NULL, NULL, TO_ROOM );
	do_look(ch,"auto");
  stop_fighting(ch,TRUE);
  WAIT_STATE( ch, skill_table[gsn_vanish].abiliti_beats[1] );
  return;
}


void do_warcry(CHAR_DATA *ch, char *argument)
{
  AFFECT_DATA af;

  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_warcry].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (is_affected(ch,gsn_bless) || is_affected(ch, gsn_warcry))
    {
      send_to_char("{cYou are blessed though.{x\n\r",ch);
      return;
    }

  if (ch->mana < 30)
    {
      send_to_char("{cYou can not concentrate enough right now.{x\n\r",ch);
      return;
    }

  WAIT_STATE( ch, skill_table[gsn_warcry].abiliti_beats[1] );

  if (number_percent() > ch->pcdata->learned[gsn_warcry])
    {
      send_to_char("{cYou grunt softly.{x\n\r", ch);
      act("{c$n makes some soft grunting noises.{x", ch, NULL, NULL, TO_ROOM);
      return;
    }

  ch->mana -= 30;

  af.where	= TO_AFFECTS;
  af.type      = gsn_warcry;
  af.level	 = ch->level;
  af.duration  = 6+ch->level;
  af.location  = APPLY_HITROLL;
  af.modifier  = ch->level / 8;
  af.bitvector = 0;
  affect_to_char( ch, &af );

  af.location  = APPLY_SAVING_SPELL;
  af.modifier  = 0 - ch->level / 8;
  affect_to_char( ch, &af );
  send_to_char( "{cYou feel righteous as you yell out your warcry.{x\n\r", ch );
  return;
}

void do_barkskin(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;

    if ( is_affected( ch, gsn_barkskin ) )
    {
	  send_to_char("{cYour skin is already covered in bark.{x\n\r",ch);
	return;
    }

 if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_barkskin].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if (number_percent() > ch->pcdata->learned[gsn_barkskin])
    {
      send_to_char("{cYou try to cover in bark, but fail.{x\n\r", ch);
      act("{c$n takes some leaves from the floor and puts them on his head.{x", ch, NULL, NULL, TO_ROOM);
      return;
    }

  ch->mana -= 40;


    af.where	 = TO_AFFECTS;
    af.type      = gsn_barkskin;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.location  = APPLY_AC;
    af.modifier  = -(ch->level * 1.5);
    af.bitvector = 0;
    affect_to_char( ch, &af );
    act( "{c$n's skin becomes covered in bark.{x", ch, NULL, NULL, TO_ROOM );
    send_to_char( "{cYour skin becomes covered in bark.{x\n\r", ch );
    return;
}

void do_guard(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;
  one_argument(argument, arg);

  if (!kingdom_ok(ch,gsn_guard))
    return;

  if (IS_NPC(ch) || ch->pcdata->learned[gsn_guard] < 40 ||
        ch->level < skill_table[gsn_guard].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }

  if ( arg[0] == '\0' )
    {
      send_to_char("{cGuard whom?{x\n\r", ch);
      return;
    }

//  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
  if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
      send_to_char( "{cThey aren't here.{x{x\n\r", ch );
      return;
    }

  if (IS_NPC(victim))
    {
      act("{c$N doesn't need any of your help!{x", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (!str_cmp(arg, "none") || !str_cmp(arg, "self") || victim == ch)
    {
	if (ch->guarding == NULL)
	  {
	    send_to_char("{cYou can't guard yourself!{x\n\r", ch);
	    return;
	  }
	else
	{
      act("{cYou stop guarding $N.{x", ch, NULL, ch->guarding, TO_CHAR);
      act("{c$n stops guarding you.{x", ch, NULL, ch->guarding, TO_VICT);
      act("{c$n stops guarding $N.{x", ch, NULL, ch->guarding, TO_NOTVICT);
      ch->guarding->guarded_by = NULL;
      ch->guarding             = NULL;
      return;
        }
    }

  if (ch->guarding == victim)
    {
      act("{cYou're already guarding $N!{x", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (ch->guarding != NULL)
    {
      send_to_char("{cBut you're already guarding someone else!{x\n\r",ch);
      return;
    }

  if (victim->guarded_by != NULL)
    {
      act("{c$N is already being guarded by someone.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (victim->guarding == ch)
    {
      act("{cBut $N is guarding you!{x", ch, NULL, victim, TO_CHAR);
      return;
    }

  if (!is_same_group(victim, ch))
    {
      act("{cBut you aren't in the same group as $N.{x",ch,NULL,victim,TO_CHAR);
      return;
    }

  if (IS_AFFECTED(ch,AFF_CHARM))
    {
      act("{cYou like your master too much to bother guarding $N!{x",
	  ch,NULL,victim,TO_VICT);
      return;
    }

 if (victim->fighting != NULL)
   {
     send_to_char("{cWhy don't you let them stop fighting first?{c\n\r", ch);
     return;
   }

  if (ch->fighting != NULL)
    {
      send_to_char
    ("{cBetter finish your own battle before you worry about guarding someone else.{x\n\r",
     ch);
      return;
    }

  act("{cYou are now guarding $N.{x", ch, NULL, victim, TO_CHAR);
  act("{cYou are being guarded by $n.{x", ch, NULL, victim, TO_VICT);
  act("{c$n is now guarding $N.{x", ch, NULL, victim, TO_NOTVICT);

  ch->guarding = victim;
  victim->guarded_by = ch;

}

CHAR_DATA *check_guard(CHAR_DATA *ch, CHAR_DATA *mob)
{
  int chance;

  if (ch->guarded_by == NULL ||
//      get_char_room(ch,ch->guarded_by->name) == NULL)
      get_char_room(ch,NULL,ch->guarded_by->name) == NULL)
      return ch;
  else
    {
      chance = (get_skill(ch->guarded_by,gsn_guard) -
		(1.5 * (ch->level - mob->level)));
      if (number_percent() < UMIN(100,chance))
	{
	  act("{c$n jumps in front of $N!{x",ch->guarded_by,NULL,ch,TO_NOTVICT);
	  act("{c$n jumps in front of you!{x",ch->guarded_by,NULL,ch,TO_VICT);
	  act("{cYou jump in front of $N!{x",ch->guarded_by,NULL,ch,TO_CHAR);
	  check_improve(ch->guarded_by,gsn_guard,TRUE,3);
	  return ch->guarded_by;
	}
      else
	{
	  check_improve(ch->guarded_by, gsn_guard, FALSE, 3);
	  return ch;
	}

    }
}


void do_shieldbash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    int Xchance = 0;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    OBJ_DATA *obj;

     wait = 0;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_shieldbash)) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_shieldbash].abiliti_level[ch->class]))
    {
	send_to_char("{cShield Bashing? What's that?{x\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }

//    else if ((victim = get_char_room(ch,arg)) == NULL)
    else if ((victim = get_char_room(ch,NULL,arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x{x\n\r",ch);
	return;
    }
if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].abiliti_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= chance)
        {
        check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_bash].abiliti_beats[1] * 3/2);
         send_to_char( "{cSomeone trying to sneek up on you.{x\n\r", victim );
	 return;
        }
        
     }
        
if (!IS_NPC(victim))     
      if(number_percent() < victim->pcdata->learned[gsn_reaction])
	{
	check_improve(victim,gsn_reaction,TRUE,1);
	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	act("{cYou reacted and dodge $n's bash!{x",victim,NULL,ch,TO_VICT);
	act("{c$N, reacts and dodges your bash!{x",victim,NULL,ch,TO_CHAR);
	act("{c$n reacts and dodges $N's powerful bash.{x",victim,NULL,ch,TO_NOTVICT);
	return;
	}

    if (victim->position < POS_FIGHTING)
    {
	act("{cYou'll have to let $M get back up first.{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("{cYou try to hit your head with your shield, but fail.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( ( obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
{
	send_to_char("{cYou need a shield to bash with it!{x\n\r",ch);
	return;
 }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is your friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }
/*
    if (is_affected(victim, gsn_protective_shield))
     {
	act("{cYour shield bash seems to slide around $N.{x", ch, NULL, victim, TO_CHAR);
	act("{c$n's bash slides off your protective shield.{x", ch, NULL, victim,
	    TO_VICT);
	act("{c$n's bash seems to slide around $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }
 *//*
    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_shieldbash].abiliti_beats[1] * 3/2);
       return;
      }
    

    if(victim->kingdom == KINGDOM_DRAKKON)
     if ( check_draggon_speed( ch, victim) )
      {
       WAIT_STATE(ch,skill_table[gsn_shieldbash].abiliti_beats[1] * 3/2);
       return;
      }
*/
    /* modifiers */





    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 10;

    /* level */
    chance += (ch->level - victim->level) * 2;
    chance -= ch->lagattack * 5;
    /* now the attack */
    if (number_percent() < chance)
    {

	act("{c$n sends you sprawling with a powerful shield bash!{x",
		ch,NULL,victim,TO_VICT);
	act("{cYou slam into $N with your shield, and send $M flying!{x",ch,NULL,victim,TO_CHAR);
	act("{c$n sends $N sprawling with a powerful bash with his shield.{x",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_shieldbash,TRUE,1);

	wait = 3;

	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

//	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_shieldbash].abiliti_beats[1]);
	victim->position = POS_RESTING;
        if (victim->lagattack < 9)
	  victim->lagattack = 9;
	if (victim->lagflee   < 5)
	 victim->lagflee   = 5;
	if (victim->lagstand   <5)
	  victim->lagstand  = 5;
	if (victim->lagspell   < 5)
	  victim->lagspell = 5;
	damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/10),gsn_shieldbash,
	    DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_shieldbash,DAM_BASH, TRUE);
	act("{cYou fall flat on your face!{x",
	    ch,NULL,victim,TO_CHAR);
	act("{c$n falls flat on $s face.{x",
	    ch,NULL,victim,TO_NOTVICT);
	act("{cYou evade $n's shield bash, causing $m to fall flat on $s face.{x",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_shieldbash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_shieldbash].abiliti_beats[1] * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! Someone is bashing me with a shield!");
	else
	  {
	    sprintf(buf, "Help! %s is bashing me with a shield!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name);
	do_yell(victim,buf);
	  }
      }
}

void do_blindness( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *tmp_vict;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)
    || ch->level < skill_table[gsn_blindness_dust].abiliti_level[ch->class])
    {
	send_to_char("{CHuh?{x\n\r",ch);
	return;
    }

    if (ch->position < skill_table[gsn_blindness_dust].abiliti_minimum_position)
    {
	send_to_char("{cYou can't concentrage enough.{x\n\r",ch);
	return;
    }

    if (ch->mana < 20)
    {
	send_to_char("{cYou don't have enought mana.{x\n\r",ch);
	return;
    }

    send_to_char("{CA cloud of dust fills in the room.{x\n\r",ch);
    act("{CA cloud of dust fills the room.{x",ch,NULL,NULL,TO_ROOM);
    WAIT_STATE(ch, skill_table[gsn_blindness_dust].abiliti_beats[1]);
    if (number_percent() > ch->pcdata->learned[gsn_blindness_dust])
    {
	send_to_char("{cYou lost your concentration.{x\n\r",ch);
	check_improve(ch, gsn_blindness_dust, FALSE, 1);
	ch->mana -= 10;
	return;
    }
    else
    {
	for (tmp_vict = ch->in_room->people; tmp_vict != NULL;
	    tmp_vict = tmp_vict->next_in_room)

	    if (!is_safe_nomessage(ch, tmp_vict))
	    {
		if (!IS_NPC(ch) && tmp_vict != ch && ch->fighting != tmp_vict
		&& tmp_vict->fighting != ch
		&& (IS_SET(tmp_vict->affected_by, AFF_CHARM)
		|| !IS_NPC(tmp_vict)))
		{
		    if (!can_see(tmp_vict, ch))
	do_yell(tmp_vict,"Help! Someone is attacking me!");
		    else
		    {
			sprintf(buf,"Die, %s, you sorcerous dog!{x",
			(is_affected(ch, gsn_doppelganger)
			&& !IS_IMMORTAL(tmp_vict)) ? ch->doppel->name :
			ch->name);
	do_yell(tmp_vict,buf);
		    }
		}
		if (tmp_vict != ch)
		{
		    spell_blindness(gsn_blindness, ch->level, ch,
			tmp_vict, TARGET_CHAR);
		    multi_hit(tmp_vict, ch, TYPE_UNDEFINED);
		}
	    }
	ch->mana -= 20;
	check_improve(ch, gsn_blindness_dust, TRUE, 1);
	return;
    }
}


void do_poison( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *tmp_vict;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)
    || ch->level < skill_table[gsn_poison_smoke].abiliti_level[ch->class])
    {
	send_to_char("{CHuh?{x\n\r",ch);
	return;
    }

    if (ch->position < skill_table[gsn_poison_smoke].abiliti_minimum_position)
    {
	send_to_char("{cYou can't concentrage enough.{x\n\r",ch);
	return;
    }

    if (ch->mana < 20)
    {
	send_to_char("{cYou don't have enought mana.{x\n\r",ch);
	return;
    }

    send_to_char("{CA cloud of poison smoke fills in the room.{x\n\r",ch);
    act("{CA cloud of poison smoke fills the room.{x",ch,NULL,NULL,TO_ROOM);
    WAIT_STATE(ch, skill_table[gsn_poison_smoke].abiliti_beats[1]);
    if (number_percent() > ch->pcdata->learned[gsn_poison_smoke])
    {
	send_to_char("{cYou lost your concentration.{x\n\r",ch);
	check_improve(ch, gsn_poison_smoke, FALSE, 1);
	ch->mana -= 10;
	return;
    }
    else
    {
	for (tmp_vict = ch->in_room->people; tmp_vict != NULL;
	    tmp_vict = tmp_vict->next_in_room)

	    if (!is_safe_nomessage(ch, tmp_vict))
	    {
		if (!IS_NPC(ch) && tmp_vict != ch && ch->fighting != tmp_vict
		&& tmp_vict->fighting != ch
		&& (IS_SET(tmp_vict->affected_by, AFF_CHARM)
		|| !IS_NPC(tmp_vict)))
		{
		    if (!can_see(tmp_vict, ch))
	do_yell(tmp_vict,"Help! Someone is attacking me!");
		    else
		    {
			sprintf(buf,"Die, %s, you sorcerous dog!{x",
			(is_affected(ch, gsn_doppelganger)
			&& !IS_IMMORTAL(tmp_vict)) ? ch->doppel->name :
			ch->name);
	do_yell(tmp_vict,buf);
		    }
		}
		if (tmp_vict != ch)
		{
		    spell_poison(gsn_poison, ch->level, ch,
			tmp_vict, TARGET_CHAR);
		    multi_hit(tmp_vict, ch, TYPE_UNDEFINED);
		}
	    }
	ch->mana -= 20;
	check_improve(ch, gsn_poison_smoke, TRUE, 1);
	return;
    }
}


void do_charge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int dam, wait;

    one_argument(argument,arg);

    if (!IS_NPC(ch)
    && ch->level < skill_table[gsn_charge].abiliti_level[ch->class])
    {
	send_to_char("{cYou are not trained in this skill.{x\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("{cBut you aren't fighting anyone!{x\n\r",ch);
	    return;
	}
    }
//    else if ((victim = get_char_room(ch,arg)) == NULL)
    else if ((victim = get_char_room(ch,NULL,arg)) == NULL)
    {
	send_to_char("{cThey aren't here.{x\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You can't reach.{x\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    obj = get_eq_char(ch, WEAR_WIELD);
    if (obj == NULL)
	{
	send_to_char("You cannot charge without an weapon!\n\r",ch);
	return;
	}

    if (obj->value[0] != WEAPON_POLEARM && obj->value[0] != WEAPON_LANCE)
      {
	send_to_char("You need to be wielding a polearm or lance!!\n\r",ch);
	return;
	}

    if ( ch->fighting == victim )
    {
	send_to_char( "{cYou can't charge someone you fighting face to face.{x\n\r", ch );
	return;
    }

    if (is_affected(victim, gsn_protective_shield))
     {
	act("{cYour charge seems to bounce off $N.{x", ch, NULL, victim,TO_CHAR);
	act("{c$n's charge seems to bounce off you.{x", ch, NULL, victim,
	    TO_VICT);
	act("{c$n's charge seems to bounce off $N.{x",ch,NULL,victim,TO_NOTVICT);
	return;
     }
/*
    if(victim->kingdom == KINGDOM_MORDUKHAN)
     if ( check_lightning_reflexes( ch, victim) )
      return;
 */     
      
    
    dam = GET_DAMROLL(ch);
    dam += number_range(10, ch->level * 4);
    dam += number_range (GET_DAMROLL(ch) /5, GET_DAMROLL(ch));

     

 if (MOUNTED(ch))
     dam = dam * 2;

    WAIT_STATE(ch, skill_table[gsn_charge].abiliti_beats[1]);
    if (IS_NPC(ch) || number_percent() < get_skill(ch, gsn_charge))
    {
	act("{c$n charges $N, and knocks them over!{x",
	    ch, NULL, victim, TO_NOTVICT);
	act("{c$n charges right into you!{x",
	    ch, NULL, victim, TO_VICT);
	act("{cYou charge right into $N, and knock $M over!{x",
	    ch, NULL, victim, TO_CHAR);
	damage( ch, victim, dam, gsn_charge, DAM_BASH, TRUE );
	check_improve(ch, gsn_charge, TRUE, 1);
	wait = 2;

 if (MOUNTED(ch))
    wait = wait * 2;
    
	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	victim->position = POS_RESTING;

    }
    else
    {
	act("{c$n charges $N, but runs right past!{x",
	    ch, NULL, victim, TO_NOTVICT);
	act("{c$n charges you, but runs right past!{x",
	    ch, NULL, victim, TO_VICT);
	act("{cYou try to charge $N, but run past $M!{x",
	    ch, NULL, victim, TO_CHAR);
	damage( ch, victim, 0, gsn_charge, DAM_BASH, TRUE );
	check_improve(ch, gsn_charge, FALSE, 1);
    }
    return;
}

/*
 * critical strike
*/
int critical_strike( CHAR_DATA *ch, CHAR_DATA *victim,   int dam )
{
  int diceroll;
  AFFECT_DATA baf;

/*
  if ( get_eq_char(ch,WEAR_WIELD) == NULL
        && get_eq_char(ch,WEAR_SECONDARY) == NULL
      && number_percent() > ( (ch->hit * 100) / ch->max_hit ) )
      return 0;
*/

  if ( !IS_NPC(ch) && /* IS_SET( ch->off_flags, OFF_CRIT ) ) ||*/
         get_skill( ch, gsn_critical ) > 0 )
  {

    diceroll = number_range( 0, 100 );
    if( victim -> level > ch -> level )
      diceroll += ( victim->level - ch->level ) * 2;
    if( victim -> level < ch -> level )
      diceroll -= ( ch->level - victim->level );

    if ( (!IS_NPC(ch)) && (diceroll <= (get_skill( ch, gsn_critical )/2))
)
    {
      check_improve( ch, gsn_critical, TRUE, 2 );
      dam += dam * diceroll/200;
  }

    if ( (IS_NPC(ch) && (diceroll <= (ch->level / 13))) ||
         (!IS_NPC(ch) && (diceroll <= (get_skill(ch, gsn_critical)/13)) )
)
    {
      diceroll = number_percent( );
      if( diceroll < 75 )
      {
        act_new( "$n takes you down with a weird judo move!",
             ch, NULL, victim, TO_VICT,POS_RESTING);
        act_new( "You take $N down with a weird judo move!",
             ch, NULL, victim, TO_CHAR,POS_RESTING);
        if (!IS_NPC(ch))
          check_improve( ch, gsn_critical, TRUE, 3 );
       // WAIT_STATE( victim, 2 * PULSE_VIOLENCE );
       
       	victim->position = POS_RESTING;
	/*setting lags*/
        if (victim->lagattack < 6)
	  victim->lagattack = 6;
	if (victim->lagflee   < 6)
	 victim->lagflee   = 6;
	if (victim->lagstand   <6)
	  victim->lagstand  = 6;
	if (victim->lagspell   < 6)
	  victim->lagspell = 6;
        dam += (dam * number_range( 2, 5 )) / 5;
        return(dam);
      }
   else if( diceroll > 75 && diceroll < 95 )
      {
        act_new( "You are blinded by $n's attack!", ch, NULL, victim,
                        TO_VICT ,POS_RESTING);
        act_new( "You blind $N with your attack!", ch, NULL, victim,
                        TO_CHAR,POS_RESTING);
        if (!IS_NPC(ch))
          check_improve( ch, gsn_critical, TRUE, 4 );
        if (!IS_AFFECTED(victim,AFF_BLIND))
        {
          baf.type = gsn_dirt;
          baf.level = ch->level;
          baf.location = APPLY_HITROLL;
          baf.modifier = -4;
          baf.duration = number_range(1,5);
          baf.bitvector = AFF_BLIND;
          affect_to_char( victim, &baf );
    }
        dam += dam * number_range( 1, 2 );
        return(dam);
      }
      else if( diceroll > 95 )
      {
        act_new( "$n does a flying jump kick.  OUCH!!",
             ch, NULL, victim, TO_VICT ,POS_RESTING);
        act_new( "You does a flying jump kick at $N!  I bet that hurt!",
             ch, NULL, victim, TO_CHAR ,POS_RESTING);
        if (!IS_NPC(ch))
          check_improve( ch, gsn_critical, TRUE, 5 );
        dam += dam * number_range( 2, 5 );
        return(dam);
      }
    }
  }

  return dam;
}



void do_bandage( CHAR_DATA *ch, char *argument)
{
    int heal;

    if (!kingdom_ok(ch,gsn_bandage))
      return;

    if ( get_skill(ch,gsn_bandage) == 0
        ||  IS_NPC(ch) ||
        (!IS_NPC(ch)
         && ch->level < skill_table[gsn_bandage].abiliti_level[ch->class]))
    {
        send_to_char("Huh?.\n\r",ch);
        return;
  }

    if (IS_AFFECTED(ch,AFF_REGENERATION))
    {
        send_to_char("You have already using your bandage.\n\r",ch);
        return;
    }


    if (number_percent() < get_skill(ch,gsn_bandage) )
    {
        AFFECT_DATA af;

        WAIT_STATE(ch, PULSE_VIOLENCE);


        send_to_char("You place your bandage to your shoulder!\n\r",ch);
   act("$n places a bandage to $s shulder.",ch,NULL,NULL,TO_ROOM);
        check_improve(ch,gsn_bandage,TRUE,2);

        heal = dice(4, 8) + ch->level / 2;
        ch->hit = UMIN( ch->hit + heal, ch->max_hit );
        update_pos( ch );
        send_to_char( "You feel better!\n\r", ch);
 
        af.where        = TO_AFFECTS;
        af.type         = gsn_bandage;
        af.level        = ch->level;
        af.duration     = ch->level / 10;
        af.modifier     = UMIN(15,ch->level/2);
        af.bitvector    = AFF_REGENERATION;
        af.location     = 0;
        affect_to_char(ch,&af);
 }

    else
    {
     WAIT_STATE(ch, PULSE_VIOLENCE);

     send_to_char("You failed to place your bandage to your shoulder.\n\r", ch);
     //check_improve(ch,gsn_bloodthirst,FALSE,2);
	check_improve(ch,gsn_bandage,FALSE,2);   // Derkec
    }
}

void do_vild( CHAR_DATA *ch, char *argument)
{
    int chance;
    //hp_percent;

/*
    if (ch->level < skill_table[gsn_aim].skill_level[ch->class])
    {
	send_to_char("{cYou turn red in the face, but nothing happens.{x\n\r",ch);
	return;
    }
*/

    /* damage -- below 50% of hp helps, above hurts */
     chance = ch->pcdata->learned[gsn_aim] / 2;



  if (is_affected(ch,gsn_aim))
    { 
      send_to_char("You cannot plan you attacks any better.\n\r",ch);
      return;
    }

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);

	send_to_char("{cYour attacks are now planned!{x\n\r",ch);
	check_improve(ch,gsn_aim,TRUE,2);

	af.where	= TO_AFFECTS;
	af.type		= gsn_aim;
	af.level	= ch->level;
	af.duration	= 3;
	af.modifier	= number_range(ch->level/2,ch->level +1);
 	af.bitvector = 0;
	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

    }

    else
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->mana -= 25;
	ch->move -= 10;

	send_to_char("{cYou failed to plan you attacks.{x\n\r",ch);
	check_improve(ch,gsn_aim,FALSE,2);
    }
}


void do_jab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );


  if (IS_NPC(ch) ||
      ch->level < skill_table[gsn_jab].abiliti_level[ch->class] )
    {
      send_to_char( "{CHuh?{x\n\r", ch );
      return;
    }


    if (arg[0] == '\0')
    {
  	  victim = ch->fighting;
  	  if (victim == NULL)
      {
  	    send_to_char("But you aren't fighting anyone!\n\r",ch);
  	    return;
  	  }
    }
 
//    else if ((victim = get_char_room(ch, arg)) == NULL)
    else if ((victim = get_char_room(ch,NULL, arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    

    if ( victim == ch )
    {
	   send_to_char( "How are you going to jab yourself exactly???\n\r", ch );
	   return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("{cBut $N is your friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }


    if ( is_safe( ch, victim ) )
      return;

    if ( (( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL) || 
	      (obj->value[0]!=WEAPON_SPEAR)
	   )
    {
	  send_to_char( "You need to wield a spare in order to jab.\n\r", ch );
	  return;
    }

    WAIT_STATE( ch, skill_table[gsn_jab].abiliti_beats[1] );

    if ( number_percent( ) < get_skill(ch,gsn_jab)
    || ( get_skill(ch,gsn_jab) >= 2 && !IS_AWAKE(victim) ) )
     { 
      check_improve(ch,gsn_jab,TRUE,1);
      one_hit( ch, victim, gsn_jab ,FALSE);
     }
    else
     {
     check_improve(ch,gsn_jab,FALSE,1);
     damage( ch, victim, 0, gsn_jab,DAM_NONE,TRUE);
     }
  return;
}

/* Zardic temp steal fix end*/

void do_bagpeek( CHAR_DATA *ch, char *argument )
{    
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    sprintf(buf,"in");
    one_argument( buf, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );


   if (    !IS_NPC( ch )
        && ch->level < skill_table[gsn_bagpeek].abiliti_level[ch->class] )
   {
      send_to_char( "You do not know how to do that...\n\r", ch );
      return;
   }

    
//    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;


    if ( arg3[0] == '\0' )
     {
      send_to_char( "At what will you greedy small eye look?\n\r", ch );
      return;
     } 

    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    { 
	/* 'look in' */

   if ( victim != ch
    &&   !IS_NPC(ch)
    &&   number_percent( ) < get_skill(ch,gsn_bagpeek))
    {
/*	send_to_char( "\n\r{rYou {Wpeek {rat the inventory:{x\n\r", ch ); */
	check_improve(ch,gsn_bagpeek,TRUE,3);

    

//	if ( ( obj = get_obj_here( victim, arg3 ) ) == NULL )
	if ( ( obj = get_obj_here( victim,NULL, arg3 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }
	
	
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    sprintf( buf, "It's %sfilled with  a %s liquid.\n\r",
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
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p holds:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
     send_to_char( "You did not get a look in the container.\n\r", ch );
     	return;
    }
         	return;
    }

void do_bagsteal( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
//    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    CHAR_DATA *victim;
//    CHAR_DATA *gch;
//    bool found;
    int percent;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    
    
    send_to_char( "Bagsteal <name> <obj> <container>\n\r", ch );
    
//    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
        
    if (is_safe(ch,victim))
	return;


    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
     {
      send_to_char("Don't be so greedy!\n\r",ch);
      return;
     }
      	

//	if ( ( container = get_obj_here( victim, arg3 ) ) == NULL )
	if ( ( container = get_obj_here( victim,NULL, arg3 ) ) == NULL )
	{
	    act( "I see no $T here.", ch, NULL, arg3, TO_CHAR );
	    return;
	}
     switch ( container->item_type )
	{
	default:
	    send_to_char( "That's not a container.\n\r", ch );
	    return;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	    break;

	case ITEM_CORPSE_PC:
	    {

		if (!can_loot(ch,container))
		{
		    send_to_char( "You can't do that.\n\r", ch );
		    return;
		}
	    }
	}

	if ( IS_SET(container->value[1], CONT_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	    return;
	}

	    obj = get_obj_list( ch, arg2, container->contains );
	    if ( obj == NULL )
	    {
		act( "I see nothing like that in the $T.",
		    ch, NULL, arg3, TO_CHAR );
		return;
	    }


    WAIT_STATE( ch, skill_table[gsn_bagsteal].abiliti_beats[1] );

    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );
    percent += can_see( victim, ch ) ? -10 : 0;
    percent -= 5;

    if ( /* ch->level + 5 < victim->level || */
         victim->position == POS_FIGHTING
    || ( !IS_NPC(ch) && percent > ch->pcdata->learned[gsn_bagsteal] ) )
    {
	/*
	 * Failure.
	 */
	send_to_char( "Oops.\n\r", ch );
	affect_strip(ch,gsn_sneak);
	REMOVE_BIT(ch->affected_by,AFF_SNEAK);
        sprintf(buf,"%s tries to steal from %s!",ch->name,victim->name);
        wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);

	act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );
	switch(number_range(0,3))
	{
	case 0 :
	   sprintf( buf, "%s is a lousy thief!", ch->name );
	   break;
        case 1 :
	   sprintf( buf, "%s couldn't rob %s way out of a paper bag!",
		    ch->name,(ch->sex == 2) ? "her" : "his");
	   break;
	case 2 :
	    sprintf( buf,"%s tried to rob me!",ch->name );
	    break;
	case 3 :
	    sprintf(buf,"Keep your hands out of there, %s!",ch->name);
	    
	    break;
        }
        if (!IS_AWAKE(victim))
            do_wake(victim,"");
	if (IS_AWAKE(victim))
            do_yell( victim, buf );
	if ( !IS_NPC(ch) )
	{
	    if ( IS_NPC(victim) )
	    {
	        check_improve(ch,gsn_bagsteal,FALSE,2);
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	    else
	    {
 /*	     get_objx( ch, obj, container ); */
 	     wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
		if ( !IS_SET(ch->act, PLR_THIEF) )
		{
/*		    SET_BIT(ch->act, PLR_THIEF);
		    send_to_char( "*** You are now a THIEF!! ***\n\r", ch ); */
		    return;
                 }
           return;
               }
    return;         
              } 
              
    return;
}
get_objx( ch, obj, container );

sprintf(buf,"%s steals from %s [%d] %s!",ch->name,victim->name,obj->pIndexData->vnum,obj->name);
 if (!IS_NPC(victim))
   log_string( buf );
wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
save_char_obj( ch );
return; 
}

void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int Xchance = 0;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_backstab].abiliti_level[ch->class] )
      {
	send_to_char("{cYou don't know how to backstab.{x\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cBackstab whom?{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }

if (!IS_NPC(victim))
    if ( (victim->kingdom == KINGDOM_ARKADIA) && (victim->level > skill_table[gsn_6th_Sense].abiliti_level[victim->class]) && (victim->fighting == NULL))
     {
      Xchance	= victim->pcdata->learned[gsn_6th_Sense] / 2;
       if (number_percent( ) <= Xchance)
        {
        check_improve(victim,gsn_strangle,TRUE,1);
         send_to_char( "{cHe is looking at you.{x\n\r", ch );
         WAIT_STATE(ch,skill_table[gsn_backstab].abiliti_beats[1] * 3/2);
         send_to_char( "{cSomeone trying to sneek up on you.{x\n\r", victim );
	 return;
        }
        
     }


    if ( victim == ch )
    {
	send_to_char( "{cHow can you sneak up on yourself?{x\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
	send_to_char( "{cYou need to wield a weapon to backstab.{x\n\r", ch );
	return;
    }

    if (obj->value[0] != WEAPON_DAGGER)
      {
	send_to_char("{cYou need to wield a dagger to backstab.{x\n\r",
		     ch);
	return;
      }

   if (IS_AFFECTED(victim,AFF_FLYING) && !(IS_AFFECTED(ch,AFF_FLYING)) && (victim->position != POS_SLEEPING))    // Modified by Derkec 11/21/99
	{
	send_to_char("Backstabbing someone who is flying... try throwing the dagger.\n\r",ch);
	return;
	}

    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't backstab a fighting person.{x\n\r", ch );
	return;
    }


    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_backstab].abiliti_beats[1] );

    if ( victim->hit < (0.75 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "{c$N is hurt and suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
/*
    if ( current_time-victim->last_fight_time<300 && (IS_AWAKE(victim) &&
	ch->kingdom != KINGDOM_CRESCENT))
    {
	act( "{c$N is suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }
*/
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < ch->pcdata->learned[gsn_backstab] )
    {
	check_improve(ch,gsn_backstab,TRUE,1);
	if (!IS_NPC(ch) &&
	    number_percent( ) < (ch->pcdata->learned[gsn_dual_backstab]))
	  {
	    check_improve(ch,gsn_dual_backstab,TRUE,1);
	    multi_hit(ch,victim,gsn_backstab);
	    one_hit(ch,victim,gsn_dual_backstab,FALSE);
	  }
	else
	  {
	    check_improve(ch,gsn_dual_backstab,FALSE,1);
	    multi_hit( ch, victim, gsn_backstab);
	  }
    }
    else
    {
	check_improve(ch,gsn_backstab,FALSE,1);
	damage( ch, victim, 0, gsn_backstab,DAM_NONE, TRUE );
    }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! I've been backstabbed!");
	else
	  {
  	    sprintf( buf, "Die, %s, you backstabbing scum!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name );
	do_yell(victim,buf);

  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Backstabbing Scum");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

          }
      }
    return;
}

void do_cleave( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

	if (ch->master != NULL && IS_NPC(ch))
	return;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_cleave].abiliti_level[ch->class] )
      {
	send_to_char("{cYou don't know how to cleave.{x\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cCleave whom?{x\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cHow can you sneak up on yourself?{x\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
    {
	act("{cBut $N is such a good friend!{x",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{CKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
	send_to_char( "{cYou need to wield a weapon to cleave.{x\n\r", ch );
	return;
    }


    if ( victim->fighting != NULL )
    {
	send_to_char( "{cYou can't cleave a fighting person.{x\n\r", ch );
	return;
    }

    if ( (victim->hit < (0.9 * victim->max_hit)) &&
	 (IS_AWAKE(victim)) )
    {
	act( "{c$N is hurt and suspicious ... you can't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_cleave].abiliti_beats[1] );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < ch->pcdata->learned[gsn_cleave] )
      {
	check_improve(ch,gsn_cleave,TRUE,1);
	multi_hit(ch,victim,gsn_cleave);
      }
    else
      {
	check_improve(ch,gsn_cleave,FALSE,1);
	damage( ch, victim, 0, gsn_cleave,DAM_NONE, TRUE );
      }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position == POS_FIGHTING)
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! Someone is attacking me!");
	else
	  {
	    sprintf( buf, "Die, %s, you butchering fool!{x",
		(is_affected(ch,gsn_doppelganger) && !IS_IMMORTAL(victim)) ?
		ch->doppel->name : ch->name );
	do_yell(victim,buf);

  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Butcher");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

	  }
      }
    return;
}

void do_ambush( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( IS_NPC(ch) ||
         ch->level < skill_table[gsn_ambush].abiliti_level[ch->class] )
      {
	send_to_char("{cYou don't know how to ambush.{x\n\r",ch);
	return;
      }

    if ( arg[0] == '\0' )
    {
	send_to_char( "{cAmbush whom?{x\n\r", ch );
	return;
    }


    if ( ( victim = get_char_room( ch,NULL, arg ) ) == NULL )
    {
	send_to_char( "{cThey aren't here.{x{x\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "{cHow can you ambush yourself?{x\n\r", ch );
	return;
    }

	 if ( victim->hit < (0.75 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "{c$N is hurt and suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }


    if (can_see(victim,ch))
      {
	send_to_char("{cBut they can see you.{x\n\r",ch);
	return;
      }

    if ( victim->hit < (0.9 * victim->max_hit) &&
	 (IS_AWAKE(victim) ) )
    {
	act( "{c$N is hurt and suspicious ... you couldn't sneak up.{x",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

#if 0
    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting))
    {
        send_to_char("{cKill stealing is not permitted.{x\n\r",ch);
        return;
    }
#endif

    check_killer( ch, victim );
    WAIT_STATE( ch, skill_table[gsn_ambush].abiliti_beats[1] );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < ch->pcdata->learned[gsn_ambush] )
      {
	check_improve(ch,gsn_ambush,TRUE,1);
	multi_hit(ch,victim,gsn_ambush);
      }
    else
      {
	check_improve(ch,gsn_ambush,FALSE,1);
	damage( ch, victim, 0, gsn_ambush,DAM_NONE, TRUE );
      }
    /* Player shouts if he doesn't die */
    if (!(IS_NPC(victim)) && !(IS_NPC(ch))
	&& victim->position == POS_FIGHTING )
      {
	if (!can_see(victim, ch))
	do_yell(victim, "Help! I've been ambushed by someone!");
	else
	  {
	    sprintf( buf, "Help! I've been ambushed by %s!{x",
		    (is_affected(ch,gsn_doppelganger)&& !IS_IMMORTAL(victim)) ?
		    ch->doppel->name : ch->name );
	do_yell(victim,buf);
  if ((!IS_NPC(ch)) && (number_percent() < 5)){
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, " the Woodland Assailant");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );}

	  }
      }
    return;
}
