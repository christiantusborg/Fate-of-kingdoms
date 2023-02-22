/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/***************************************************************************
                                   Command
****************************************************************************/

//void do_disengage( CHAR_DATA *ch, char *argument )

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"

void do_disengage( CHAR_DATA *ch, char *argument )
{
//Stop the fight, if you not tanking or victim AFF_STUN or AFF2_HOLD
    CHAR_DATA *victim;
    CHAR_DATA *person;

	//Wait befor we do any thing
	WAIT_STATE(ch, PULSE_VIOLENCE / 3);
	//if we are not fighting any one
	if ( ( victim = ch->fighting ) == NULL )
	  {
		send_to_char( "{cYou aren't fighting anyone.{x\n\r", ch );
		return;
	  }
    
	//Looping aææ on the room to see if we fight them
	for (person = ch->in_room->people;person != NULL; person = person->next_in_room)
	  {
		//To check if have to ftop or not
		if ((person->fighting == ch) && (!IS_AFFECTED(person,AFF_STUN)) && (!IS_AFFECTED2(person, AFF2_HOLD)))
		  {
			send_to_char("{cYou can't disengage when you're being targetted.{x\n\r",ch);
			return;
		  }
	  }
	//Stop the fight
	send_to_char( "{cYou disengage from the fight.{x\n\r", ch );
	act( "$n disengages from the fight.", ch, NULL, NULL, TO_ROOM );
	stop_fighting( ch, TRUE );
	victim->last_fought = NULL;
	if (IS_NPC(victim) && IS_SET(victim->act,ACT_AGGRESSIVE))
	    REMOVE_BIT(victim->act,ACT_AGGRESSIVE);	
  return;
}

