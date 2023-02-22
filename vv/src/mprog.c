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
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "interp.h"
#include "recycle.h"

void raw_kill args( (CHAR_DATA *victim) );
DECLARE_DO_FUN(do_yell	);
DECLARE_DO_FUN(do_gd	);
DECLARE_DO_FUN(do_murder);

DECLARE_MPROG_FUN_BRIBE( bribe_prog_cityguard );
DECLARE_MPROG_FUN_FIGHT( fight_prog_wraith    );
DECLARE_MPROG_FUN_FIGHT( fight_prog_statue    );
DECLARE_MPROG_FUN_FIGHT( fight_prog_enforcer  );
DECLARE_MPROG_FUN_FIGHT( fight_prog_slayer    );
DECLARE_MPROG_FUN_ENTRY( entry_prog_enforcer  );
DECLARE_MPROG_FUN_ENTRY( entry_prog_slayer    );
DECLARE_MPROG_FUN_AREA( area_prog_enflayer );
DECLARE_MPROG_FUN_GREET( greet_prog_kingdom );
DECLARE_MPROG_FUN_SPEECH( speech_prog_dressmaker );
DECLARE_MPROG_FUN_GREET( greet_prog_queen );
DECLARE_MPROG_FUN_GREET( greet_prog_golem );
DECLARE_MPROG_FUN_GIVE( give_prog_golem );
DECLARE_MPROG_FUN_GIVE( give_prog_queen );
DECLARE_MPROG_FUN_SPEECH( speech_prog_sailor );
DECLARE_MPROG_FUN_GREET( greet_prog_sailor );
/*  tries to add generic greet prog */
DECLARE_MPROG_FUN_GREET( greet_prog_generic );
DECLARE_MPROG_FUN_GIVE( give_prog_keeper );
DECLARE_MPROG_FUN_DEATH( death_prog_stalker );
DECLARE_MPROG_FUN_DEATH( death_prog_guardian );
DECLARE_MPROG_FUN_GREET( greet_prog_gandalf );
DECLARE_MPROG_FUN_AREA( area_prog_gandalf );
DECLARE_MPROG_FUN_GREET( greet_prog_shade );
DECLARE_MPROG_FUN_FIGHT( fight_prog_priest );
DECLARE_MPROG_FUN_GREET( greet_prog_keeper );
DECLARE_MPROG_FUN_GIVE( give_prog_dressmaker );

DECLARE_MPROG_FUN_AREA( area_prog_cityguard );
DECLARE_MPROG_FUN_ENTRY( entry_prog_cityguard );
DECLARE_MPROG_FUN_SPEECH( speech_prog_vagabond );

DECLARE_MPROG_FUN_GIVE( give_prog_weaponsmith );
DECLARE_MPROG_FUN_SPEECH( speech_prog_weaponsmith );
DECLARE_MPROG_FUN_DEATH( death_prog_servant );
DECLARE_MPROG_FUN_BRIBE( bribe_prog_seller_dwarf );
DECLARE_MPROG_FUN_GIVE( give_prog_wizard );
DECLARE_MPROG_FUN_GREET( greet_prog_templeman   );
DECLARE_MPROG_FUN_SPEECH( speech_prog_templeman );

int lookup_religion_leader (const char *name);
#define MOB_STAT_RESTING	2
#define MOB_STAT_WAITING_ANSWER	3
#define MOB_STAT_BEATING_SWORD	6
#define MOB_STAT_HEATING_SWORD	7
#define MOB_STAT_COOLING_SWORD	8
#define MOB_STAT_HANGING_SWORD	9

#define MOB_STAT_WAITING_SERVANT 10
#define MOB_STAT_SERVANT_LATE 	11
#define MOB_STAT_MAKE_SWORD 	12
#define MOB_STAT_MAKE_MACE 	13
#define MOB_STAT_MAKE_DAGGER 	14
#define MOB_STAT_WEAPON_READY 	15
#define GIVE_HELP_RELIGION       6
#define RELIG_CHOSEN             7
#define MOB_STAT_SERV_STOP 	6
#define MOB_STAT_SERV_GO 	7

/* time values are in seconds of real time */
#define OBJ_VNUM_WEAPONSMITH_SWORD1  3021 
#define OBJ_VNUM_WEAPONSMITH_SWORD2  3022
#define WEAPONSMITH_NEEDS_IRON_TIME  30*60

void mprog_set(MOB_INDEX_DATA *mobindex,const char *progtype, const char *name)
{
   if (!str_cmp(progtype, "bribe_prog"))
     {
       if (!str_cmp(name, "bribe_prog_cityguard"))
	 mobindex->moprogs->bribe_prog = bribe_prog_cityguard;
       else if (!str_cmp(name, "bribe_prog_seller_dwarf"))
	 mobindex->moprogs->bribe_prog = bribe_prog_seller_dwarf;

       else 
	 {
	   bug("Load_improgs: 'M': Bribe Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }

       SET_BIT(mobindex->progtypes, MPROG_BRIBE);
       return;
     }

   if (!str_cmp(progtype, "entry_prog"))
     {
       if (!str_cmp(name, "entry_prog_enforcer"))
	 mobindex->moprogs->entry_prog = entry_prog_enforcer;
       else if (!str_cmp(name, "entry_prog_slayer"))
	 mobindex->moprogs->entry_prog = entry_prog_slayer;
       else if (!str_cmp(name, "entry_prog_cityguard"))
	 mobindex->moprogs->entry_prog = entry_prog_cityguard;

       else
	 {
	   bug("Load_improgs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_ENTRY);
       return;
     }
   if (!str_cmp(progtype, "greet_prog"))
     {

       if (!str_cmp(name, "greet_prog_kingdom"))
	 mobindex->moprogs->greet_prog = greet_prog_kingdom;
       else if (!str_cmp(name, "greet_prog_queen"))
	 mobindex->moprogs->greet_prog = greet_prog_queen;
       else if (!str_cmp(name, "greet_prog_golem"))
	 mobindex->moprogs->greet_prog = greet_prog_golem;
       else if (!str_cmp(name,"greet_prog_sailor"))
	 mobindex->moprogs->greet_prog = greet_prog_sailor;
/*  adds 2 lines for generic greet prog */
       else if (!str_cmp(name,"greet_prog_generic"))
	 mobindex->moprogs->greet_prog = greet_prog_generic;
       else if (!str_cmp(name, "greet_prog_gandalf"))
	 mobindex->moprogs->greet_prog = greet_prog_gandalf;
       else if (!str_cmp(name, "greet_prog_shade"))
	 mobindex->moprogs->greet_prog = greet_prog_shade;
       else if (!str_cmp(name, "greet_prog_keeper"))
	 mobindex->moprogs->greet_prog = greet_prog_keeper;
       else if (!str_cmp(name, "greet_prog_templeman"))
         mobindex->moprogs->greet_prog = greet_prog_templeman;
       else
	 {
	   bug("Load_improgs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }

       SET_BIT(mobindex->progtypes, MPROG_GREET);
       return;
     }

   if (!str_cmp(progtype, "fight_prog"))
     {
       if (!str_cmp(name, "fight_prog_wraith"))
	 mobindex->moprogs->fight_prog = fight_prog_wraith;
       
       else if (!str_cmp(name, "fight_prog_statue"))
	 mobindex->moprogs->fight_prog = fight_prog_statue;

       else if (!str_cmp(name, "fight_prog_enforcer"))
	 mobindex->moprogs->fight_prog = fight_prog_enforcer;

       else if (!str_cmp(name, "fight_prog_slayer"))
	 mobindex->moprogs->fight_prog = fight_prog_slayer;
       else if (!str_cmp(name, "fight_prog_priest"))
	 mobindex->moprogs->fight_prog = fight_prog_priest;
       else 
	 {
	   bug("Load_improgs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_FIGHT);
       return;
     }
   if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
     {
       if (!str_cmp(name, "death_prog_stalker"))
	 mobindex->moprogs->death_prog = death_prog_stalker;
       else if (!str_cmp(name, "death_prog_servant"))
	 mobindex->moprogs->death_prog = death_prog_servant;
       else if (!str_cmp(name, "death_prog_guardian"))
	mobindex->moprogs->death_prog = death_prog_guardian;
       else 
	 {
	   bug("Load_improgs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_DEATH);
       return;
     }
   if (!str_cmp(progtype, "area_prog"))
     {
       if (!str_cmp(name, "area_prog_enflayer"))
	 mobindex->moprogs->area_prog = area_prog_enflayer;
       else if (!str_cmp(name, "area_prog_gandalf"))
	 mobindex->moprogs->area_prog = area_prog_gandalf;
       else if (!str_cmp(name, "area_prog_cityguard"))
	 mobindex->moprogs->area_prog = area_prog_cityguard;
       else
	 {
	   bug("Load_improgs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_AREA);
       return;
     }
   if (!str_cmp(progtype, "speech_prog"))
     {
       if (!str_cmp(name, "speech_prog_dressmaker"))
	 mobindex->moprogs->speech_prog = speech_prog_dressmaker;
       else if(!str_cmp(name, "speech_prog_sailor"))
	 mobindex->moprogs->speech_prog = speech_prog_sailor;
       else if(!str_cmp(name, "speech_prog_vagabond"))
	 mobindex->moprogs->speech_prog = speech_prog_vagabond;
       else if(!str_cmp(name, "speech_prog_weaponsmith"))
	 mobindex->moprogs->speech_prog = speech_prog_weaponsmith;
	else if(!str_cmp(name, "speech_prog_templeman"))
         mobindex->moprogs->speech_prog = speech_prog_templeman;
       else
	 {
	   bug("Load_improgs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
       SET_BIT(mobindex->progtypes, MPROG_SPEECH);
       return;
     }
   if (!str_cmp(progtype, "give_prog"))
     {
       if (!str_cmp(name, "give_prog_queen"))
	 mobindex->moprogs->give_prog = give_prog_queen;
       else if (!str_cmp(name, "give_prog_golem"))
	 mobindex->moprogs->give_prog = give_prog_golem;
       else if (!str_cmp(name, "give_prog_keeper"))
 	 mobindex->moprogs->give_prog = give_prog_keeper;
       else if (!str_cmp(name, "give_prog_dressmaker"))
	 mobindex->moprogs->give_prog = give_prog_dressmaker;
       else if (!str_cmp(name, "give_prog_weaponsmith"))
	 mobindex->moprogs->give_prog = give_prog_weaponsmith;
       else if (!str_cmp(name, "give_prog_wizard"))
	 mobindex->moprogs->give_prog = give_prog_wizard;
       else
	 {
	   bug("Load_improgs: 'M': Function not found for vnum %d",
	       mobindex->vnum);
	   exit(1);
	 }
	 
       SET_BIT(mobindex->progtypes, MPROG_GIVE);
       return;
     }
   bug( "Load_improgs: 'M': invalid program type for vnum %d",mobindex->vnum);
   exit(1);
 }

/******************  Functions *********************/

time_t last_time_weaponsmith = -1; /* global for weaponsmith quest */

void give_prog_weaponsmith(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	char log_buf[MAX_STRING_LENGTH];
  /*
   *  Servant is the giver.
   */
  if ( IS_NPC( ch ) || IS_IMMORTAL(ch))  {
    if ( obj->pIndexData->vnum != 1405 ) {
	do_say(mob, "What is this? I don't need this.");
	obj_from_char( obj );
	obj_to_char( obj, ch );
	act("$N gives you $p back.", ch, obj, mob, TO_CHAR);
	act("$N gives $p back to $n.", ch, obj, mob, TO_ROOM);
	act("You give $p back to $N", mob, obj, ch, TO_CHAR);
   if(IS_IMMORTAL(ch)) {
	do_say(mob,"You are an immortal go back to your own business.");
	do_say(mob,"Your attempt have been logged.");
      sprintf(log_buf,"%s has tried to solve the weaponsmith quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
	}
	return;
    }

   if(IS_IMMORTAL(ch)) {
	do_say(mob,"You are an immortal go back to your own business.");
	do_say(mob,"Your attempt have been logged.");
      sprintf(log_buf,"%s has tried to solve the weaponsmith quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
	}
    do_say( mob, "Dwarves are getting off work. I must reduce the price.");
    act( "$n puts $p over the bright fire.", mob, obj, NULL, TO_ROOM );
    do_say(mob,  "Now, back to work.");
    extract_obj( obj );
    last_time_weaponsmith = current_time;
    mob->status = MOB_STAT_HEATING_SWORD;
    return;
  }

  /*  
   *  Giver is a player.
   */

   if ( mob->status != MOB_STAT_SERVANT_LATE )   {
    if ( obj->pIndexData->vnum == 1405 ) {
	if ( mob->status != MOB_STAT_WAITING_SERVANT )
	  do_say(mob, "I have enough iron in my supplies. I don't need more.");
	else 
	  do_say(mob, "I've just send the boy to get me some iron, he'll soon return." );
   }
   else
	do_say(mob, "What is this? I don't need this.");

   obj_from_char( obj );
   obj_to_char( obj, ch );
   act("$N gives you $p back.", ch, obj, mob, TO_CHAR);
   act("$N gives $p back to $n.", ch, obj, mob, TO_ROOM);
   act("You give $p back to $N", mob, obj, ch, TO_CHAR);
   return;
  }

  /* 
   * things are not good, the servant is late and we are out of iron.
   */

    if ( obj->pIndexData->vnum != 1405 ) {
	do_say(mob, "Thank you, but i don't need this.");
	obj_from_char( obj );
	obj_to_char( obj, ch );
	switch(dice(1, 4) )  {
	  case 1:
	  case 2:
	    do_say(mob, "Well, i am out of iron, and the boy is late.");
	    do_say(mob, "If you find me some iron i promise you'll get a nice weapon from me.");
	    break;
	  case 3:
	    interpret(mob, "emote looks at the clock." );
	    do_say(mob, "Where the hell is this stupid boy?!");
	    break;
	  case 4:
	    interpret(mob, "emote looks at the clock." );
	    do_say(mob, "That boy needs a good beating. He's late AGAIN!");
	    break;
	}
  	return;
  }
  
  /*
   * What? somebody made us a favor? Oh dear!
   */

  do_say( mob, "Great! You brought me iron, that's very nice of you." );
  do_say( mob, "I'll make a weapon for you? What do you need?");
  do_say( mob, "Sword? Mace? or a dagger?");
  extract_obj( obj );
  mob->char_in_mind = ch;
  mob->status = MOB_STAT_WAITING_ANSWER;
  return;
}

void give_prog_wizard(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	char log_buf[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	OBJ_DATA *new;

  if ( obj->pIndexData->vnum == OBJ_VNUM_SPEC_WEAPON_S  ||
       obj->pIndexData->vnum == OBJ_VNUM_SPEC_WEAPON_M  ||
       obj->pIndexData->vnum == OBJ_VNUM_SPEC_WEAPON_D  )
    if ( !IS_SET( obj->extra_flags, ITEM_HUM ) )  
    {
      do_say( mob, "I've been waiting for you." );
      interpret( mob, "emote utters the words 'Wgrisma qwemnet judicara'." );
      act( "Suddenly the weapon glows extreamly bright, then fades.", mob, NULL, NULL, TO_ROOM );
      act( "The weapon starts humming.", mob, NULL, NULL, TO_ROOM );
      act( "Wizard gives your weapon back.", ch, NULL, NULL, TO_CHAR );
      act( "Wizard gives $n's weapon back.", ch, NULL, NULL, TO_ROOM );
      if ( obj->pIndexData->vnum == OBJ_VNUM_SPEC_WEAPON_S )
        new = create_object(get_obj_index(OBJ_VNUM_SPEC_WEAPON_S+1), 0);
  else    if ( obj->pIndexData->vnum == OBJ_VNUM_SPEC_WEAPON_M )
        new = create_object(get_obj_index(OBJ_VNUM_SPEC_WEAPON_M+1), 0);
  else    if ( obj->pIndexData->vnum == OBJ_VNUM_SPEC_WEAPON_D )
        new = create_object(get_obj_index(OBJ_VNUM_SPEC_WEAPON_D+1),0);
  else  new = create_object(get_obj_index(OBJ_VNUM_SPEC_WEAPON_S+1), 0);
      new->extra_descr = new_extra_descr();
      new->extra_descr->keyword = str_dup(obj->extra_descr->keyword);
      new->extra_descr->description = str_dup(obj->extra_descr->description);
      new->extra_descr->next = NULL;
      extract_obj( obj );
      obj_to_char( new, ch );
      do_say( mob, "It will only serve you." );
      if ( !IS_SET( ch->quest, QUEST_WEAPONSMITH ) )  {
         SET_BIT( ch->quest, QUEST_WEAPONSMITH );
	sprintf(log_buf,"%s has solved the weaponsmith quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
  	 act_color( "{Y( Well done!  You receive 2500 experience points.){w\n\r",
             ch, NULL, NULL, TO_CHAR, POS_SLEEPING, 0 );
	sprintf(buf,"{YAndor shouts: Thanks, %s for helping me!!\n",ch->name);
	do_echo(ch,buf);
         gain_exp( ch, 2500 );
      }
      return;
   }
   do_say( mob, "What is this?  I don't have any use of this." );
   obj_from_char( obj );
   obj_to_char( obj, ch );
   return;
 }

void speech_prog_weaponsmith(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
char buf[160];
OBJ_DATA *weapon;

  if ( (ch == mob->char_in_mind) && (mob->status==MOB_STAT_WAITING_ANSWER) )
  {
    if ( !str_cmp( speech, "sword" ) )
	mob->status = MOB_STAT_MAKE_SWORD;
    else if ( !str_cmp( speech, "mace" ) )
	mob->status = MOB_STAT_MAKE_MACE;
    else if ( !str_cmp( speech, "dagger" ) )
	mob->status = MOB_STAT_MAKE_DAGGER;
    else  {
	do_say(mob, "What? Sorry i can only make you a sword, mace or dagger");
	do_say(mob, "Now, which one would you prefer?");
	return;
    }
	do_say(mob, "Here.  Take this weapon to the Tatorious the wizard in the High Tower.");
        if ( mob->status == MOB_STAT_MAKE_SWORD )
          weapon = create_object( get_obj_index( OBJ_VNUM_SPEC_WEAPON_S ), 0 );
        else if ( mob->status == MOB_STAT_MAKE_MACE )
          weapon = create_object( get_obj_index( OBJ_VNUM_SPEC_WEAPON_M ), 0 );
        else if ( mob->status == MOB_STAT_MAKE_DAGGER )
          weapon = create_object( get_obj_index( OBJ_VNUM_SPEC_WEAPON_D ), 0 );
        else return;

        sprintf( buf, weapon->pIndexData->extra_descr->description, ch->name );
        weapon->extra_descr = new_extra_descr();
        weapon->extra_descr->keyword = 
                  str_dup( weapon->pIndexData->extra_descr->keyword );
        weapon->extra_descr->description = str_dup( buf );
        weapon->extra_descr->next = NULL;
        obj_to_char( weapon, ch );
	do_say(mob, "He's my friend and he will finish this weapon.");
	sprintf( buf, "thank %s", ch->name );
	interpret(mob, buf );
        mob->status = MOB_STAT_RESTING;
	return;
  }	

  if ( IS_NPC(ch) )
    if ( ch->pIndexData->vnum == 3307 )
      if ( !str_cmp( speech, "Do you need anything, master?") )  {
        if ( current_time-last_time_weaponsmith > WEAPONSMITH_NEEDS_IRON_TIME )  {
          do_say(mob, "Yes, go and get me iron bars. You know where it is.");
          do_say(mob, "Here's some money.");
          if ( mob->silver < 1000 ) mob->silver += 1000;
	  do_give( mob, "1000 silver novice");
          ch->status = MOB_STAT_SERV_GO;
          last_time_weaponsmith = current_time;
        }
      }
  return;

}


void speech_prog_templeman(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
/*
char buf[160];
int chosen = 0,correct = 1;

    if ( !str_cmp( speech, "religion" ) )
        mob->status = GIVE_HELP_RELIGION;
    else if (( chosen = lookup_religion_leader( speech)) != 0 )
        mob->status = RELIG_CHOSEN;
    else  return;

   if ( mob->status == RELIG_CHOSEN )
   {
    if (( ch->religion > 0) && (ch->religion < MAX_RELIGION) )
        {
       sprintf(buf,"You are already in the way of %s",
                religion_table[ch->religion].leader);
         do_say(mob,buf);
         return;
        }
    switch( chosen )
    {
        case RELIGION_ARIANEIRA:
         if ( !IS_GOOD(ch) && ch->ethos != 1) correct = 0;
         break;
        case RELIGION_SILVARA:
      if ( !IS_GOOD(ch) && ch->ethos != 2) correct = 0;
         break;
        case RELIGION_BAYARD:
         if ( !IS_EVIL(ch) && ch->ethos != 3) correct = 0;
         break;
        case RELIGION_CEDRIC:
         if ( !IS_EVIL(ch) && ch->ethos != 2) correct = 0;
         break;
        case RELIGION_EXPLORER:
         if ( !IS_NEUTRAL(ch) && ch->ethos != 2) correct = 0;
         break;
     }
    if (!correct)
      {
        do_say(mob, "That religion doesn't match your ethos and alignment.");
        return;
      }

    ch->religion = chosen;
    sprintf(buf,"From now on and forever, you are in the way of %s",
                religion_table[ch->religion].leader);
    do_say(mob,buf);
    return;
   }
   do_say(mob,"Himm yes, religion.Do you really interested in that?.");
   do_say(mob,"Read the help first.Type 'help religion'");
   do_say(mob,"Do not forget that once you choose your religion. ");
   do_say(mob,"You can not change anoymore."); */
   return;
}

void greet_prog_templeman(CHAR_DATA *mob, CHAR_DATA *ch)
{

  char arg[100];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;

  sprintf(arg,"smile %s",ch->name);
  interpret(mob, arg);
}

int lookup_religion_leader (const char *name)
{
/*   int value;

   for ( value = 0; value < MAX_RELIGION ; value++)
   {
        if (LOWER(name[0]) == LOWER(religion_table[value].leader[0])
        &&  !str_prefix( name,religion_table[value].leader))
            return value;
   }
*/
   return 0;
}

bool death_prog_servant(CHAR_DATA *mob)
{
  ROOM_INDEX_DATA *room;
  CHAR_DATA *ch;

  room = get_room_index( 3011 );
  if (room == NULL)  {
    bug("death_servant: can't find room 3011", 0);
    return FALSE;
  }

  for(ch=room->people; ch!=NULL; ch=ch->next_in_room)  {
    if ( !IS_NPC(ch)) continue;
    if ( ch->pIndexData->vnum == 3003 ) break;
  }
  if ( ch == NULL )
    return FALSE;          /* weaponsmith does not exist */
  ch->status = MOB_STAT_SERVANT_LATE;
  return FALSE;
}


void entry_prog_cityguard(CHAR_DATA *mob)
{
CHAR_DATA *tmp;

  for ( tmp = mob->in_room->people; tmp != NULL; tmp = tmp->next_in_room )
  {
    if ( IS_NPC(tmp) && (tmp->pIndexData->vnum == MOB_VNUM_VAGABOND ) ) {
       mob->status = 9;
    }
  }
  if ( mob->status != 9 ) mob->status = 10;

}


void area_prog_cityguard(CHAR_DATA *mob)
{
CHAR_DATA *tmp;


  if ( mob->fighting != NULL ) return;
  if ( mob->status == 8 ) return;  
  for ( tmp = mob->in_room->people; tmp != NULL; tmp = tmp->next_in_room )
  {
    if ( IS_NPC(tmp) && (tmp->pIndexData->vnum == MOB_VNUM_VAGABOND ) ) {
       mob->status = 9;
    }
  }
  if (mob->status != 9) mob->status = 10;

  if ( mob->status == 10 )  {
    switch( dice(1,180) )  {
	case 1:
	  interpret( mob, "sigh" );
	  do_say( mob, "The city vagabonds need to be taken care of." );
	  break;
	case 4:
	  do_say( mob, "The Mayor needs to do something or I will." );
	  break;
	case 8:
	  do_say( mob, "Hmm.. I should make the rounds, gotta keep this place safe.");
	 break;
	case 12:
	  interpret( mob, "yawn" );
	  do_say( mob, "Sometimes I wonder if this is really the job for me...");
	 break;
    }
    return;
  }
  if ( mob->status == 9 )  {
    switch( dice(1,11) )  {
    case 1:
    case 2:
    case 3:
    case 4:
      interpret( mob, "emote eyes the vagabond suspiciously and grins." );
      do_say( mob, "Go ahead slime, try it.. the people will not mind");
      mob->status = 8;
      break;
    case 5:
    case 6:
    case 7:
      interpret( mob, "emote looks at the vagabond angrily." );
      do_say( mob, "One more ruse like your last, and you will spend the rest of your life hoping for death." );
      mob->status = 8;
      break;
    case 8:
    case 9:
      interpret( mob, "stare vagabond" );
      do_say( mob, "A tourist was killed last night, stripped of her valuables.");
      do_say( mob, "Any idea who it may have been?");
      interpret( mob, "emote takes a deep breath and hums in thought." );
      mob->status = 8;
      break;
    case 10:
      do_say( mob, "YOU! YOU KILLED THAT TOURIST!");
      do_kill( mob, "vagabond" );
      do_say( mob, "...and justice will be served");
      mob->status = 1;
      return;
    }
    return;
  }


}

void speech_prog_vagabond(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
  int vCount;
  CHAR_DATA *tmp;

  if ( !IS_NPC(ch) )
      return;

/* 
 *  Mob's words are trigger.
 */
  vCount = 0;
  for ( tmp = mob->in_room->people; tmp != NULL; tmp = tmp->next_in_room )
  {
    if ( IS_NPC(tmp) && (tmp->pIndexData->vnum == MOB_VNUM_VAGABOND ) )
       vCount++;
  }


  if ( ch->pIndexData->vnum == MOB_VNUM_CITYGUARD )  {
    if (!str_cmp(speech, "You'd better watch your next steps.") 
	&& (ch->status!=6))  
    {
      ch->status = 6;
      do_say(mob, "Sure. I will");
      if ( vCount > 1 )  {
        do_say(mob, "Check this out.");
        do_kill(mob, "cityguard");
	switch(dice(1,3)) {
        case 1:
	  do_say(mob, "KILL THE CITYGUARD!");
          break;
        case 2:
	  do_say(mob, "THIS GUARD WON'T LEAVE HERE ALIVE!");
          break;
        case 3:
          do_say(mob, "EVERYBODY ATTACK!");
          break;
        }
      }
      else  {
        if (dice(1,2) == 1) do_west(mob, "");
	else do_east(mob, "");
      }
    }

    if (!str_cmp(speech, "Stupid, you are responsable for that terrible look in dump")
	&& ch->status != 6)  
    {
      ch->status = 6;
      switch(dice(1,3))  {
      case 1:
        do_say(mob, "YOU ARE CITYGUARD. Why don't you lick clean it up?");
	break;
      case 2:
	do_say(mob, "At least it smells better than you.");
	break;
      case 3:
        if ( ch->sex == 1 )  {
  	  do_say(mob, 
	"Maybe you should check your wife, instead of talking about the dump.");
	  interpret( mob, "grin cityguard" );
	}
        if ( ch->sex == 2 )  {
  	  do_say(mob, 
	"Maybe you wanna visit the dump with me, babe? We could really have fun.");
	  interpret( mob, "stroke cityguard" );
	}
	break;
      }	
      switch(dice(1, 2))  {
      case 1:
	do_say( ch, "YOU FOOL! I'M GONNA TEACH YOU HOW TO TALK WITH A CITYGUARD!");
	break;
      case 2:
	do_say( ch, "YOU'RE GONNA PAY FOR THAT!" );
	break;
      }
      do_kill( ch, "vagabond" );
    }
	  
    if (!str_cmp(speech, "I think you did it. Who did that?")
	)  
      switch(dice(1, 3) )  {
      case 1:
        interpret( mob, "shake" );
	break;
      case 2:
	do_say( mob, "No man, i dunno." );
	break;
      case 3:
	interpret( mob, "Bored. Send your doughter or leave me alone." );
	break;
      }
  }



  if ( ch->pIndexData->vnum == MOB_VNUM_VAGABOND )
    if (  !str_cmp(speech, "KILL THE CITYGUARD") ||
	  !str_cmp(speech, "THIS GUARD WON'T") ||
	  !str_cmp(speech, "EVERYBODY ATTACK") )
    {
      switch(dice(1, 4))  {
      case 1:
	do_say(mob, "Yeah!");
	interpret( mob, "emote screams and attacks!");
        do_trip( mob, "cityguard" );
	break;      
      case 2:
	do_say(mob, "Die fool! Taste my boots.");
	interpret( mob, "emote attacks and kicks the cityguard.");
	do_kick( mob, "cityguard" );
	break;      
      case 3:
	do_say(mob, "Slayer will reward us for this.");
	interpret( mob, "emote screams and attacks!");
	do_dirt( mob, "cityguard" );
	break;      
      case 4:
	break;
      }
    }
}

void bribe_prog_seller_dwarf(CHAR_DATA *mob, CHAR_DATA *ch, int amount)
{

  if ( !IS_NPC(ch) )
    if ( amount < 9 )  {
      do_say( mob, "That is not enough." );
      return;
    }

/*
  if ( ch->pIndexData->vnum != 3164 )
    return;
*/
  obj_to_char(create_object( get_obj_index(OBJ_VNUM_IRON_BAR), -1 ), mob);   
  do_drop(mob, "iron" );
  do_say(mob, "Here they are.");
/* new iron bars */
  return;
}

void bribe_prog_cityguard(CHAR_DATA *mob, CHAR_DATA *ch, int amount)
{
  if (amount < 10)
    {
      do_say(mob, "You cheapskate!!!");
      do_murder(mob, ch->name);
      return;
    }
  else if (amount >= 50)
    {
      interpret(mob, "smile");
      do_sleep(mob, "");
      return;
    }
  else 
    {
      do_say(mob, "Trying to bribe me, eh? It'll cost ya more than that.");
      return;
    }
}

void fight_prog_wraith(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (number_bits(1) == 0)
    return;

  act("You touch $N with your slimy fingers!",mob,NULL,ch,TO_CHAR);
  act("$n touches you with $s slimy fingers!",mob,NULL,ch,TO_VICT);
  act("$n touches $N with $s slimy fingers!",mob,NULL,ch,TO_NOTVICT);

  gain_exp(ch,- number_range(150,500));
}

void fight_prog_statue(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (number_bits(4) == 0) {
    act("You jump in the air and land on $N, killing $M instantly!",
	mob,NULL,ch,TO_CHAR);
    act("$n jumps in the air and lands on $N, killing $M instantly!",
	mob,NULL,ch,TO_NOTVICT);
    act("$n jumps in the air and lands on you, killing you instantly!",
	mob,NULL,ch,TO_VICT);

    raw_kill(ch);
  }
}

void fight_prog_enforcer(CHAR_DATA *mob,CHAR_DATA *ch)
{
  if (IS_NPC(ch) && 
      (ch->pIndexData->vnum == 3165 || ch->pIndexData->vnum == 3011)
      && (float) mob->hit / (float) mob->max_hit < 0.2) {
   /* spell_vanish( gsn_vanish,mob->level,mob,(void *) mob, TARGET_CHAR );*/
    do_yell(mob, "Justice will prevail in the end!");
    mob->hit = mob->max_hit;
    ch->hit  = ch->max_hit;
  }
  else 
    switch(number_bits(4)) {
    case 0:
    case 1:
      do_say(mob,"He who fools with Truth suffers Death!");
      break;
    case 2:
    case 3:
      do_say(mob,"Justice always prevails!");
      break;
    case 4:
    case 5:
      do_say(mob,"It is a good day for slaying Slayers.");
      break;
    case 6:
      do_say(mob,"Look! Your shoe's untied!");
      break;
    default:
      break;
    }
}

void fight_prog_slayer(CHAR_DATA *mob,CHAR_DATA *ch)
{
  if (IS_NPC(ch) && 
      (ch->pIndexData->vnum == 3165 || ch->pIndexData->vnum == 3011)
      && (float) mob->hit / (float) mob->max_hit < 0.1) {
/*    spell_vanish( gsn_vanish,mob->level,mob,(void *) mob, TARGET_CHAR );*/
    do_yell(mob, "Evil is defeated, but I shall return!");
    mob->hit = mob->max_hit;
    ch->hit  = ch->max_hit;
  }
  else 
    switch(number_bits(4)) {
    case 0:
    case 1:
      do_say(mob,"I feel your despair. Surrender!");
      break;
    case 2:
    case 3:
      do_say(mob,"I will slice you like cheese!");
      break;
    case 4:
    case 5:
      do_say(mob,"Enforcer! What a stupid name!");
      interpret(ch,"snicker");
      break;
    case 6:
      do_say(mob,"What's that behind you?");
      break;
    default:
      break;
    }
}

void entry_prog_enforcer(CHAR_DATA *mob)
{
  switch(number_bits(2)) {
  case 0:
    do_say(mob, "Have you seen the Slayer?");
    break;
  case 1:
    do_say(mob, "Beware! The Slayer is loose.");
    break;
  case 2:
    do_say(mob, "Stand aside. I have a job to do.");
    break;
  case 3:
    do_say(mob, "That evil man has escaped me for the last time!");
    break;
  }
}

void entry_prog_slayer(CHAR_DATA *mob)
{
  switch(number_bits(2)) {
  case 0:
    do_say(mob, "I have come to reclaim my power!");
    break;
  case 1:
    do_say(mob, "The Enforcer must die!");
    break;
  case 2:
    do_say(mob, "Join the forces of evil, and strength shall be yours!");
    break;
  case 3:
    do_say(mob, "Never again will that Enforcer defeat me!");
    break;
  }
}

void area_prog_enflayer(CHAR_DATA *mob)
{
  sh_int door;
  EXIT_DATA *pexit;
  
  door = number_range(0,5);

  if ( ( pexit = mob->in_room->exit[door] ) != NULL
      &&   pexit->u1.to_room != NULL
      &&   !IS_SET(pexit->exit_info, EX_CLOSED)
      &&   !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB)
      && ( !IS_SET(mob->act, ACT_STAY_AREA)
	  ||   pexit->u1.to_room->area == mob->in_room->area ) )
    {
      move_char( mob, door, FALSE );
    }
}

void give_prog_keeper(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
  OBJ_DATA *rug;
  char log_buf[MAX_STRING_LENGTH];
   if(IS_IMMORTAL(ch)) {
	do_say(mob,"You are an immortal go back to your own business.");
	do_say(mob,"Your attempt have been logged.");
      sprintf(log_buf,"%s has tried to solve the dress quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
	return;
	}

  if (obj->pIndexData->vnum == 9751)
    {
      do_say(mob, "Finally, the dress I sent for!");
      act("$n tucks the dress away under her desk.",mob,NULL,NULL,TO_ROOM);
      obj_from_char(obj);
      extract_obj(obj);
      if ((rug = get_obj_carry(ch,"rug",NULL)) != NULL)
	{
	  do_say(mob, "I suppose you'll want to see the Queen now");
	  do_say(mob, "Be careful, she's been in a nasty mood.");
	  do_unlock (mob, "north");
	  do_open (mob, "north");
	}
      else
	{
  do_say(mob, "It doesn't look like you have any business with the Queen.");
  do_say(mob, "I suggest you leave and find some before coming here again.");
	}
    }
}


void speech_prog_dressmaker(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
  OBJ_DATA *obj;
  if(mob->status == 1) {
  if (!str_cmp(speech, "shelena") && !IS_NPC(ch) )
    {
      obj = create_object(get_obj_index(9751), 0);
      free_string(obj->name);
      obj->name = str_dup("advisor dress");
      act("$n fashions a white gown out of the bolt of silk.", mob, NULL, 
	NULL, TO_ROOM);
      act("You make a white gown for the Shelena.", mob, NULL, NULL, TO_CHAR);
      do_say(mob, "Here is the dress for the shelena.");
      obj_to_char(obj, ch);
    }
  }
 mob->status = 0;
}

void greet_prog_queen(CHAR_DATA *mob, CHAR_DATA *ch)
{
  char buf[100];
  OBJ_DATA *obj;

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;

  if ((obj = get_obj_carry(ch,"rug",NULL)) == NULL)
    {
      do_say(mob, "I don't want to see that worthless rug anywhere near me.");
    do_say(mob, "Why don't you give it to that silly Green sister from Tear.");
    do_emote(mob,"unlocks a shinny box.");
    do_emote(mob,"open a shinny box.");
    do_emote(mob,"gets paper from box.");
      do_say(mob, "These papers might help you.");
      act("$n sneers at you.", mob, NULL, ch, TO_VICT);
      act("You sneer at $N.", mob, NULL, ch, TO_CHAR);
      act("$n sneers at $N.", mob, NULL, ch, TO_NOTVICT);
      do_load(mob, "obj 9743");
      sprintf(buf, "papers %s", ch->name);
      do_give(mob, buf);
    do_emote(mob,"closes a shinny box.");
    do_emote(mob,"locks a shinny box.");
    }
}

void give_prog_queen(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
  char buf[100];
  ROOM_INDEX_DATA *location;
  char log_buf[MAX_STRING_LENGTH];

   if(IS_IMMORTAL(ch)) {
	do_say(mob,"You are an immortal go back to your own business.");
	do_say(mob,"Your attempt have been logged.");
      sprintf(log_buf,"%s has tried to solve the dress quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
	return;
	}

  if (!can_see(mob,ch))
       do_say(mob, "Is someone there?");
  else if (IS_NPC(ch))
      do_say(mob, "How strange, an animal delivering something.");
      
  else if (obj->pIndexData->vnum != 6419)
    {
      do_say(mob, "How interesting!  ...what's it for?");
      interpret(mob, "giggle");
    }
  else
    {
      do_say(mob, "What a wonderful rug!  Let's see....where shall I put it?");
      act("$n starts wandering about the room, mumbling to $mself.", mob, 
	NULL, NULL, TO_ROOM);
      act("$n sticks $s hands in $s pockets.",mob,NULL,NULL,TO_ROOM);
      do_load(mob, "obj 9759");
      do_say(mob, "What's this?  A key?  Here, you can have it.");
      sprintf(buf, "xxx %s",ch->name);
      do_give(mob, buf);
      act("$n absently pushes the rug under a chair.",mob,NULL,NULL,TO_ROOM);
      obj_from_char(obj);
      extract_obj(obj);
      do_say(mob, "I don't need to talk to you anymore.");
      location = get_room_index(9699);
      char_from_room(ch);
      char_to_room(ch, location);
      do_look(ch,"auto");
      do_close (mob, "south");
      do_lock (mob, "south");
    }
}

void greet_prog_golem(CHAR_DATA *mob, CHAR_DATA *ch)
{
  
  OBJ_DATA *obj;
  char arg[100];

  if (!can_see(mob,ch) || IS_NPC(ch) || IS_IMMORTAL(ch))
    return;

  sprintf(arg,"xxx");

  if ((obj = get_obj_carry(ch, arg,NULL)) == NULL)
    {
      interpret(mob, "grin");
      do_say(mob, "You forgot to bring the key in....");
      do_murder(mob, ch->name);
    }
}  

void give_prog_golem(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)

{
  char buf[MAX_STRING_LENGTH];
  OBJ_DATA *saangreal; 
  char log_buf[MAX_STRING_LENGTH];
   if(IS_IMMORTAL(ch)) {
	do_say(mob,"You are an immortal go back to your own business.");
	do_say(mob,"Your attempt have been logged.");
      sprintf(log_buf,"%s has tried to solve the dress quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
	return;
	}

  if (obj->pIndexData->vnum == 9759)
    {
      if ( !IS_SET( ch->quest, QUEST_GOLEM ) )  {
         SET_BIT( ch->quest, QUEST_GOLEM );
	sprintf(log_buf,"%s has solved the golem quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
  	 act_color( "{Y( Well done!  You receive 3000 experience points.){w\n\r",
             ch, NULL, NULL, TO_CHAR, POS_SLEEPING, 0 );
         gain_exp( ch, 3000 );

      do_say(mob, "Here is your reward, I won't kill you....this time.");
     saangreal = create_object(get_obj_index(OBJ_VNUM_SAANGREAL), 0);
      obj->timer = 500;
      obj_from_char(obj);
      obj_to_char(saangreal, mob);
      sprintf(buf,"saangreal %s",ch->name);
      do_give(mob, buf);
      do_say(mob, "This has special powers, use it well.");
      sprintf(buf,"{YAriana shouts: Thanks brave %s!!\n",ch->name);
	do_echo(ch,buf);
	}
      obj_from_char(obj);
      extract_obj(obj);
    }
}

void greet_prog_sailor(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (!IS_NPC(ch))
    {
if (number_percent() > 33)
      do_say(mob,
	   "Hey, want to dice?  I could use some of that gold in your pouch.");
    }
}

void greet_prog_generic(CHAR_DATA *mob, CHAR_DATA *ch)
{
int number;
char buf[MAX_STRING_LENGTH];

  if (!IS_NPC(ch))
    {	number = number_percent();
switch (mob->pIndexData->vnum)
	{

case 400 :	/* Sylescia */
	if (number < 90 && number > 30) 
	{
	interpret(mob, "emote dries a small shot glass with a tattered cloth.");
	do_say(mob, "Care for something? Some absinthe perhaps?"); 
	}
	if (number < 30)
	{
	interpret(mob,"emote mumbles...\n\r     '{GI really wish somebody would fix that hole in the Liquor Vault.{g'" );
	}
	break;
case 3160:    /* melancholy map man */
	if (number < 50)
	{
	interpret(mob,"emote smiles and puts down a scroll.");
	do_say(mob,"Why hello there, care to buy a map?");
	}
	if (number < 10 )
	{
	interpret(mob,"emote shrugs and sighs.");
	do_say(mob,"Just between us...I do not think the business will last...");
	do_say(mob,"The last map I got came from the Scribe himself...");
	}
	break;
case 20: /* outland stranger handles arena */
	if (number < 90)
	{
	do_say(mob,"Tidings stranger...interested in arena combat?");
	sprintf(buf,"I can bring you to one for %d gold.",ch->level*2);
	do_say(mob, buf);
	interpret(mob,"emote smiles and waits patiently.");
	}
	if ((number < 101) && (number > 89))
	{
	do_say(mob,"You know...I would be nothing without the great anger here.");
	do_say(mob,"A shame people treat their lives with so little respect.");
	interpret(mob,"emote sighs heavily.");
	do_say(mob,"I hope my business can help to remedy both of these problems");
	}
	break;
case 30:	/* outland stranger, handles jobs */
	if (number < 90)
	{
	do_say(mob,"Ho there young one!");
	do_say(mob,"It seems my lands' problems have migrated.");
	do_say(mob,"If ever need a job, I may have one for you.");
	}
	if ((number < 101) && (number > 89))
	{
	do_say(mob,"People here are so lively and ambitious...");
	do_say(mob,"Things are much different where I come from.");
	interpret(mob,"emote runs a hand through his grey hair and sighs.");
	}
	break;
default:
do_say(mob, "BUG: Tell Takeda, my vnum is not assigned a case in greet_prog_generic switch statement");
	}

    }
}

	 
void speech_prog_sailor(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
  int sroll;
  int proll;
  int amount;
  char buf[100];
  char arg[100];

  if (!str_cmp(speech, "yes"))
    do_say(mob, "How much?");

  else if (is_number(speech))
    {
      amount = atoi(speech);
      if (amount < 0)
	{
  	  sprintf(buf, "You win %d gold!!\n\r", 2*amount);
	  send_to_char(buf, ch);
	  act("$n does a very foolish thing....",ch, NULL, NULL, TO_ROOM);
	  ch->gold = UMAX(0, (ch->gold + (2*amount)));
	  return;
	}
      if (ch->gold < amount)
	{
	  do_say(mob, "You don't have that much gold!");
	  do_say(mob, "I won't dice with the likes of you!");
	}
      else
	{
	  act("$n rolls $s dice.", mob, NULL, NULL, TO_ROOM);
	  act("You roll your dice.", mob, NULL, NULL, TO_CHAR);
	  sroll = dice(5, 6);
	  act("$n rolls $s dice.", ch, NULL, NULL, TO_ROOM);
	  act("You roll your dice.", ch, NULL, NULL, TO_CHAR);
	  proll = dice(5, 6);
	  
	  if (sroll > proll)
	    {
	      sprintf(buf, "$N lost %d gold!", amount);
	      sprintf(arg, "You lost %d gold!!", amount);
	      act("$n has won some gold!!", mob, NULL, ch, TO_NOTVICT);
	      act(buf, mob, NULL, ch, TO_CHAR);
	      act(arg, mob, NULL, ch, TO_VICT);
	      ch->gold -= amount;
	      do_say(mob, "Would you like to try again?");
	      interpret(mob, "grin");
	    }
	  
	  else if (sroll == proll)
	    {
	      act("You tied with $N!  You don't lose any money.",
		  ch, NULL, mob, TO_CHAR);
	      act("You tied with $n!  You don't lose any money.",
		  ch, NULL, mob, TO_VICT);
	      act("$n tied with $N!  Neither wins the game.",
		  ch, NULL, mob, TO_NOTVICT);
	      do_say(mob, "Would you like to try that again?");
	    }
	  else
	    {
	      sprintf(buf, "$n won %d gold!", amount);
	      sprintf(arg, "You won %d gold!!", amount);
	      act("$n has won some gold!!", ch, NULL, mob, TO_NOTVICT);
	      act(buf, ch, NULL, mob, TO_VICT);
	      act(arg, ch, NULL, mob, TO_CHAR);
	      ch->gold += amount;
	      do_say(mob, "Next time you won't be so lucky.");
	    }
	}
    }
}

bool death_prog_stalker(CHAR_DATA *mob)
{
  char buf[100];
/*
  REMOVE_BIT(mob->comm,COMM_NOGD);
*/
  mob->kingdom = KINGDOM_XELENIA;
  mob->kingdom_rank = 0;
  sprintf(buf, "I have failed trying to kill %s, I gasp my last breath.",
	  mob->last_fought->name);
  do_gd(mob, buf);
  return FALSE;
}

bool death_prog_guardian(CHAR_DATA *mob)
{
  char buf[100];
  AREA_DATA *pArea=mob->in_room->area;

if(mob->pIndexData->vnum == 110) 
mob->kingdom = KINGDOM_GONDOR;
else if (mob->pIndexData->vnum == 120)
mob->kingdom = KINGDOM_ALDON;
else if (mob->pIndexData->vnum == 130)
mob->kingdom = KINGDOM_MORDUKHAN;
else if (mob->pIndexData->vnum == 140)
mob->kingdom = KINGDOM_ARKADIA;
else if (mob->pIndexData->vnum == 150)
mob->kingdom = KINGDOM_5;
else if (mob->pIndexData->vnum == 160)
mob->kingdom = KINGDOM_DRAKKON;
else if (mob->pIndexData->vnum == 170)
mob->kingdom = KINGDOM_XELENIA;
else mob->kingdom = KINGDOM_NONE;

  mob->kingdom_rank = 0;
  sprintf(buf,"%s is being attacked!!!",pArea->name);
  do_gd(mob, buf);
  remove_guardians(mob);
  return FALSE;
}
 
void area_prog_gandalf(CHAR_DATA *mob)
{
  switch(number_bits(5)) {
  case 0:
    do_say(mob, "I don't think you'll find much that is wholesome to eat or drink in Mirkwood.");
    break;
  }
}

void greet_prog_gandalf(CHAR_DATA *mob, CHAR_DATA *ch)
{
  do_say(mob, "And Remember!  DON'T STRAY OFF THE PATH!!");

}

void greet_prog_kingdom(CHAR_DATA *mob, CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];

  if ( IS_NPC(ch) )
    return;

if(mob->pIndexData->vnum == 200)
mob->kingdom = KINGDOM_GONDOR;
else if (mob->pIndexData->vnum == 260)
mob->kingdom = KINGDOM_ALDON;
else if (mob->pIndexData->vnum == 251)
mob->kingdom = KINGDOM_MORDUKHAN;
else if (mob->pIndexData->vnum == 230)
mob->kingdom = KINGDOM_ARKADIA;
else if (mob->pIndexData->vnum == 220)
mob->kingdom = KINGDOM_5;
else if (mob->pIndexData->vnum == 241)
mob->kingdom = KINGDOM_DRAKKON;
else if (mob->pIndexData->vnum == 271)
mob->kingdom = KINGDOM_XELENIA;

else mob->kingdom = KINGDOM_NONE;

mob->kingdom_rank = 5;

  if ((ch->kingdom == KINGDOM_DRAKKON) && (mob->kingdom == KINGDOM_DRAKKON)){
    do_say(mob, "Brother to Brother");
    return;
  }
/*
  if ((ch->kingdom == KINGDOM_5) && (mob->kingdom ==KINGDOM_5))
{
    do_say(mob, kingdom_table[5].mobgreet);
    return;
  }
*/
  if ((ch->kingdom == KINGDOM_GONDOR) && (mob->kingdom == KINGDOM_GONDOR)){
    sprintf(buf,"bow %s",ch->name);
    interpret(mob,buf);
    return;
  }

  if ((ch->kingdom == KINGDOM_ALDON) && (mob->kingdom == KINGDOM_ALDON)){
    sprintf(buf,"Hail brother of magic, %s",ch->name);
    do_say(mob,buf);
    return;
  }

  if ((ch->kingdom == KINGDOM_MORDUKHAN) && (mob->kingdom ==
KINGDOM_MORDUKHAN)) {
  do_say(mob, "Hail to the Mordukhan Empire!");
    return;
  }

if ((ch->kingdom == KINGDOM_ARKADIA) && (mob->kingdom == KINGDOM_ARKADIA)) {
    do_say(mob, "Greetings, wise one.");
    return;
  }

  if (ch->last_death_time != -1 && current_time - ch->last_death_time < 600)
    {
      do_say(mob, "Ghosts are not allowed in this place.");
      do_slay(mob, ch->name);
      return;
    }

  do_gd(mob, "Intruder! Intruder!");
  do_murder(mob, ch->name);
 if ((!IS_NPC(ch)) && (number_percent() < 25)) {  
    sprintf(buf, "the Intruder");
    free_string( ch->pcdata->title );ch->pcdata->title = str_dup( buf );
}

}

void greet_prog_shade(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (!can_see(mob, ch) || IS_NPC(ch))
    return;

  if (mob->master != NULL)
    do_follow(mob, ch->name);
}

void fight_prog_priest(CHAR_DATA *mob, CHAR_DATA *ch)
{
  switch(number_bits(6)) {
  case 0:
    do_say(mob, "Xiang Long Shi Ba Zhang!");
    break;
  case 1:
    do_say(mob, "Qian Kun Da Nuo Yi!");
    break;
  case 2:
    do_say(mob, "Da Li Jing Kang Zi !");
    break;
  case 3:
    do_say(mob, "Ni Jing Gan Gen Wo Tou");
    break;
  case 4:
    do_say(mob, "Wo Gei Ni Yan Se Kan");
    break;
  case 5:
    do_say(mob, "Wan Xiang Shen Gong");
    break;
  }
}

void give_prog_dressmaker(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{
	char log_buf[MAX_STRING_LENGTH];

  if (IS_NPC(ch))
    return;

   if(IS_IMMORTAL(ch)) {
	do_say(mob,"You are an immortal go back to your own business.");
	do_say(mob,"Your attempt have been logged.");
      sprintf(log_buf,"%s has tried to solve the dress quest.",ch->name);
            wiznet(log_buf,NULL,NULL,WIZ_QUESTS,0,0);
      log_string( log_buf );
	return;
	}

  if (!can_see(mob, ch))
    {
      do_say(mob, "Where did this come from?");
      return;
    }

  if (obj->pIndexData->vnum != 9750)
    {
      do_say(mob, "I can't do anything with this, I need silk.");
      do_drop(mob, obj->name);
      return;
    }

  else
    {
      mob->status = 1;
      do_say(mob, "Who am I making this dress for?");
      obj_from_char(obj);
      extract_obj(obj);
    }
}

void greet_prog_keeper(CHAR_DATA *mob, CHAR_DATA *ch)
{
  if (IS_NPC(ch))
    return;

  if (!can_see(mob, ch))
    return;

  do_say(mob, "What business do you have here?  Is it that dress I ordered?");
}
/*
Snake-charmer mobile spec-fun


  Ok, here is a spec-fun example for a nasty mobile known as the snake
   charmer.  I rather like this guy, but I never could find a MUD to
   put this code in.

   Here is what happens.  Player attacks snake charmer mobile.  Snake
   charmer then casts CHARM spell on player, and forces player to give
   charmer weapons, sing, and he can even order the player to CHAT that
   the snake charmer is this really great mob to attack.

   I figure this should liven up any egypt or india zone.
*/

bool spec_snake_charm( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
/*    char *spell; */
/*    int sn; */

    if ( ch->position != POS_FIGHTING )
       {
       switch ( number_bits( 3 ) ) {
       case 0:
          do_order( ch, "all sing charmer" ); /* a chance to get free here */
          break;
       case 1:
          do_order( ch, "all yell 'The snake charmer area is pretty cool.");
          do_order( ch, "all yell 'I'm getting a lot of experience really fast!" );
          break;
       case 2:
          do_order( ch,
             "all yell 'YES!  I just got 327xp for killing the snake charmer!") ;
          break;
       case 3:
          do_order( ch, "all remove dagger" );
          do_order( ch, "all give dagger charmer" );
          break;
       case 4:
          do_order( ch, "all remove sword" );
          do_order( ch, "all give sword charmer" );
          break;
       case 5:
          do_order( ch, "all remove mace" );
          do_order( ch, "all give mace charmer" );
          break;
       case 6:
          do_order( ch, "all drop all" );
          break;
       case 7:
          do_order( ch, "all cast 'cure light' charmer" );
          break;
       };

       return TRUE;
       }

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
        v_next = victim->next_in_room;
        if ( victim->fighting == ch && number_bits( 2 ) == 0 )
            break;
    }

    if ( victim == NULL )
        return FALSE;

    act( "$n begins playing a new, beautiful song.", ch, NULL, NULL, TO_ROOM );
spell_charm_person( skill_lookup( "charm" ), ch->level,ch,victim,TARGET_CHAR);
/* spell_charm_person(gsn_charm_person, ch->level, ch, victim );*/
    if (IS_AFFECTED(victim, AFF_CHARM))
       stop_fighting( victim, TRUE );

    return TRUE;
}


void give_prog_cook(CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj)
{

OBJ_DATA *obj1;
OBJ_DATA *obj2;
OBJ_DATA *obj3;
 /*Cheesecake*/

 if (( obj->pIndexData->vnum == 51474 )
   && ( obj->pIndexData->vnum == 51422 )
   && ( obj->pIndexData->vnum == 51340 ))
     {
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"fresh",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"cup",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
     }
  /*Beef terriyaki*/     
 if (( obj->pIndexData->vnum == 51331 )
   && ( obj->pIndexData->vnum == 51420 ))
        {
      if (((obj1 = get_obj_carry(ch,"sauce",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"meat",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
}
  /*Frosty Flan*/     
 if (( obj->pIndexData->vnum == 51422 )
   && ( obj->pIndexData->vnum == 51430 )
   && ( obj->pIndexData->vnum == 51464 ))
     {
      if (((obj1 = get_obj_carry(ch,"cup",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"egg",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"watermellon",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
}
  /*An egg roll*/     
 if (( obj->pIndexData->vnum == 51474 )
   && ( obj->pIndexData->vnum == 51430 ))
     {
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"egg",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
} 
 /*Liver and onions*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51347 ))
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"onion",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
} 
 /*Misosoup*/     
 if (( obj->pIndexData->vnum == 51331 )
   && ( obj->pIndexData->vnum == 51479 ))
     {
      if (((obj1 = get_obj_carry(ch,"sauce",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"mushroom",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
} 
 /*A club sandwich*/     
 if (( obj->pIndexData->vnum == 51474 )
   && ( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51477 ))
     {
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"meat",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"lettuce",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
}
  /*Pumpkin Pie*/     
 if (( obj->pIndexData->vnum == 51474 )
   && ( obj->pIndexData->vnum == 51478 ))
     {
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"pumpkin",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
} 
 /*Blueberry pie*/     
 if (( obj->pIndexData->vnum == 51474 )
   && ( obj->pIndexData->vnum == 51461 ))
     {
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"blueberries",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
}
  /*Apple Cream Puff*/     
 if (( obj->pIndexData->vnum == 51422 )
   && ( obj->pIndexData->vnum == 51474 )
   && ( obj->pIndexData->vnum == 51467 ))
     {
      if (((obj1 = get_obj_carry(ch,"cup",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"apple",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");
       }
}
  /*French Fries*/     
 if (( obj->pIndexData->vnum == 51476 )
   && ( obj->pIndexData->vnum == 51422 ))
     {
      if (((obj1 = get_obj_carry(ch,"potato",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"cup",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*Sashiuni Obento*/     
 if (( obj->pIndexData->vnum == 51422 )
   && ( obj->pIndexData->vnum == 51430 )
   && ( obj->pIndexData->vnum == 51464 ))
     {
      if (((obj1 = get_obj_carry(ch,"cup",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"egg",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"watermellon",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*Okoniomiyaki */     
 if (( obj->pIndexData->vnum == 51474 )
   && ( obj->pIndexData->vnum == 51430 )
   && ( obj->pIndexData->vnum == 51331 ))
     {
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"egg",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*Tomago Yati*/     
 if (( obj->pIndexData->vnum == 51331 )
   && ( obj->pIndexData->vnum == 51430 ))
     {
      if (((obj1 = get_obj_carry(ch,"sauce",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"egg",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))  
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))      
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*Sezchaun Pork*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 ))
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   

  /*Pudding*/     
 if (( obj->pIndexData->vnum == 51341 )
   && ( obj->pIndexData->vnum == 51333 ))
     {
      if (((obj1 = get_obj_carry(ch,"yogurt",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"oil",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*Tempura */     
 if (( obj->pIndexData->vnum == 51333 )
   && ( obj->pIndexData->vnum == 51420 ))
     {
      if (((obj1 = get_obj_carry(ch,"oil",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"fish",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*Shrimp Pilaf*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51475 ))
     {
      if (((obj1 = get_obj_carry(ch,"fish",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"pack",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }  
  /*BGourmet Rice Ball*/     
 if (( obj->pIndexData->vnum == 51475 )
   && ( obj->pIndexData->vnum == 51332 ))
     {
      if (((obj1 = get_obj_carry(ch,"pack",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL))
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))   
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))  
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))  
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))  
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))  
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))   
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))  
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
  /*BBQ teriyaki Steak*/     
 if (( obj->pIndexData->vnum == 51420 )
   && ( obj->pIndexData->vnum == 51332 )
   && ( obj->pIndexData->vnum == 51331 ))  
     {
      if (((obj1 = get_obj_carry(ch,"meat",NULL)) != NULL)
      && ((obj2 = get_obj_carry(ch,"herbs",NULL)) != NULL)
      && ((obj3 = get_obj_carry(ch,"sauce",NULL)) != NULL))     
       {
        do_say(mob, "Like me to cook it?.");       	
       }
    }   
 
       

 return;
}


void speech_prog_cook(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
  OBJ_DATA *obj;
  OBJ_DATA *obj1;
  OBJ_DATA *obj2;
  OBJ_DATA *obj3;
  if (!str_cmp(speech, "cook"))
    { /*Cheesecake*/
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"fresh",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"cup",NULL)) != NULL))
        {
         obj = create_object(get_obj_index(51342), 0);
         extract_obj( obj1);
         extract_obj( obj2);
         extract_obj( obj3);
         act("$n i act this to romm.", mob, NULL,NULL, TO_ROOM);
         do_say(mob, "say when iu give you?.");
         obj_to_char(obj, ch);
        }
       /*Next1*/
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"fresh",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"cup",NULL)) != NULL))
        {
         obj = create_object(get_obj_index(51342), 0);
         extract_obj( obj1);
         extract_obj( obj2);
         extract_obj( obj3);
         act("$n i act this to romm.", mob, NULL,NULL, TO_ROOM);
         do_say(mob, "say when iu give you?.");
         obj_to_char(obj, ch);
        }        

       /*Next2*/
      if (((obj1 = get_obj_carry(ch,"bread",NULL)) != NULL)
       && ((obj2 = get_obj_carry(ch,"fresh",NULL)) != NULL)
       && ((obj3 = get_obj_carry(ch,"cup",NULL)) != NULL))
        {
         obj = create_object(get_obj_index(51342), 0);
         extract_obj( obj1);
         extract_obj( obj2);
         extract_obj( obj3);
         act("$n i act this to romm.", mob, NULL,NULL, TO_ROOM);
         do_say(mob, "say when iu give you?.");
         obj_to_char(obj, ch);
        }        
    }

return;
}

/* Damn you gotta match up the ( and the )'s, and also a { ends with a }
   anyway i fixed it for you, so messy you could have used a case 
   statement or something like that.  make the person say the food they
   want to make and then do the check.

				- Takeda 2/14/2002
btw you didn't finish it yet
*/
