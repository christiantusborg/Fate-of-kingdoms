/****************************************************************************
*     Fate of Kingdom 4.0 is copyright 2000-???? Christian Tusborg (Kazmir) *
*     By using this code, you have agreed to follow the terms of the        *
*     Fate license.  The use of this code without the permission of the     *
*     author is prohibited.  You will be punished to the fullest extend of  *
*     the law.								    *
*     									    *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		    *
****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"

/****************************************************************************
                                   Armor
*****************************************************************************/
/*
lookup_armor()
     int loc,mod,stat;
     OBJ_DATA *obj;
     AFFECT_DATA *af;
     int i;

	for (loc = 0; loc < MAX_WEAR; loc++)
	  {
		obj = get_eq_char(ch,loc);
		if (obj == NULL)
			continue;

		for (i = 0; i < 4; i++)
			ac_temp -= apply_ac( obj, loc, i );

		for ( af = obj->pIndexData->affected; af != NULL; af = af->next )
			switch(af->location)
			  {
				mod = af->modifier;
				case APPLY_AC:		
					for (i = 0; i < 4; i ++)
					  {
						ac_body += mod; 
						ac_head += mod; 
						ac_arm  += mod; 
						ac_leg  += mod; 
					  }
				  break;
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

*/
