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
				Arena
****************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"  
#include "recycle.h"  
	

void do_armor( CHAR_DATA *ch, char *argument )
{
char buf[MAX_STRING_LENGTH];
     int loc;
     int mod = 0;
     OBJ_DATA *obj;
     AFFECT_DATA *af;
      int ac_temp = 0;
     int i;
	int ac_body = 0;
	int ac_head = 0;
	float wow = 0;
	int ac_arm = 0;
	int ac_leg  = 0;


     i = 0;

	send_to_char( "{C------------------------------------------------------{X\n\r", ch );
	xprintf(buf, "-    Combat Armor:     %15s               -\n\r",ch->name);
	send_to_char(buf, ch );     
for (i = 0; i < 4; i++)
{

	  	ac_body = 0;
		ac_head = 0;
		ac_arm  = 0;
		ac_leg  = 0;
	for (loc = 0; loc < MAX_WEAR; loc++)
	  {
		obj = get_eq_char(ch,loc);
		if (obj == NULL)
			continue;

		ac_temp = 0;



	  	ac_temp = apply_ac( obj, loc, i );

		wow = ac_temp/100.0;
		ac_temp = wow * obj->condition;

		mod = 0;
		for ( af = obj->affected; af != NULL; af = af->next )
			if (af->location == APPLY_AC)
			  {
				mod = (af->modifier * -1);
				ac_body += mod; 
				ac_head += mod; 
				ac_arm  += mod; 
				ac_leg  += mod; 

			  }

	  	switch(loc)
	  	  {
			//Body
			case WEAR_NECK_1:
			case WEAR_NECK_2:
			case WEAR_BODY:
			case WEAR_WAIST:
			 	ac_body += ac_temp;
			 break;

			//Both Body and Leg
			case WEAR_ABOUT:
				ac_body += ac_temp;
				ac_leg += ac_temp;
			   break;

			//Legs
			case WEAR_LEGS:
			case WEAR_FEET:
			 	ac_leg += ac_temp;
			 break;
			 
			//Head
			case WEAR_HEAD:
				ac_head += ac_temp;

			  break;
			//Arms
			case WEAR_HANDS:
			case WEAR_ARMS:
			case WEAR_WRIST_L:
			case WEAR_WRIST_R:
			 	ac_arm += ac_temp;
			 break;
	  	  
	  	  }
	  }


		
    	for (af = ch->affected; af != NULL; af = af->next)
	  {
	
			if (af->location == APPLY_AC)
			  {
				mod = (af->modifier *-1);
				ac_body += mod; 
				ac_head += mod; 
				ac_arm  += mod; 
				ac_leg  += mod; 

			  }
	  }

		if (ac_head > 0)
		  {
			wow = ac_head/430.0;
			ac_head = wow * 100;
		  }
		else
			ac_head = 0;	

		if (ac_body > 0)
		  {
			wow = ac_body/550.0;
			ac_body = wow * 100;
		  }
		else	
			ac_body = 0;	

		if (ac_leg > 0)
		  {
			wow = ac_leg/260.0;
			ac_leg = wow * 100;
		  }
		else	
			ac_leg = 0;	

		
		if (ac_arm > 0)
		  {
			wow = ac_arm/460.0;
			ac_arm = wow * 100;
		  }
		else	
			ac_arm = 0;				

	  	switch(i)
	  	{
	  	case 0:
	send_to_char( "{C----{c Pierce Damage: {C----------------------------------{X\n\r", ch );
	xprintf(buf, "- Head Armor:  %s    -\n\r", bar(ac_head) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Body Armor:  %s    -\n\r", bar(ac_body) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Arms Armor:  %s    -\n\r", bar(ac_arm) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Legs Armor:  %s    -\n\r", bar(ac_leg) );
	send_to_char(buf,  ch );
	  	break;
	  	case 1:
	send_to_char( "{C----{c   Bash Damage: {C----------------------------------{X\n\r", ch );
	xprintf(buf, "- Head Armor:  %s    -\n\r", bar(ac_head) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Body Armor:  %s    -\n\r", bar(ac_body) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Arms Armor:  %s    -\n\r", bar(ac_arm) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Legs Armor:  %s    -\n\r", bar(ac_leg) );
	send_to_char(buf,  ch );
		break;
	  	case 2:
	send_to_char( "{C----{c  Slash Damage: {C----------------------------------{X\n\r", ch );
	xprintf(buf, "- Head Armor:  %s    -\n\r", bar(ac_head) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Body Armor:  %s    -\n\r", bar(ac_body) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Arms Armor:  %s    -\n\r", bar(ac_arm) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Legs Armor:  %s    -\n\r", bar(ac_leg) );
	send_to_char(buf,  ch );
	  	break;
	  	case 3:
	send_to_char( "{C----{c Exotic Damage: {C----------------------------------{X\n\r", ch );
	xprintf(buf, "- Head Armor:  %s    -\n\r", bar(ac_head) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Body Armor:  %s    -\n\r", bar(ac_body) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Arms Armor:  %s    -\n\r", bar(ac_arm) );
	send_to_char(buf,  ch );
	xprintf(buf, "- Legs Armor:  %s    -\n\r", bar(ac_leg) );
	send_to_char(buf,  ch );
	send_to_char( "{C------------------------------------------------------{X\n\r", ch );
	  	break;
}	  		  		  	

}

}
