/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   																					 *
*     									   																								 *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   						 *
***************************************************************************/

/***************************************************************************
                              Administrator
****************************************************************************/



#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"

void list_Xpsp(CHAR_DATA *ch,char *argument,int item_type,int objectDissallow,int lvnum,int hvnum)
{
/*
	int vnum_count = 0;
	int v0123 = 0;
	char buf_color[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	OBJ_INDEX_DATA *pObj;
	bool found = FALSE;

		for (vnum_count = lvnum; vnum_count <= hvnum; vnum_count++)
			if ( ((pObj = get_obj_index(vnum_count)) != NULL && pObj->item_type == item_type) )
			 {				
				xprintf( buf,"VNUM: %5d, Level:%3d",vnum_count,pObj->value[0]);
	
				if ( pObj->value[4] != 
				xprintf( buf_color,"%s {G[v%d]Spell: %s{X ",buf_color,v0123,pObj->value[v0123] != -1 ? skill_table[pObj->value[v0123]].name : "none");
									if (!str_cmp (argument, "G"))
										found= TRUE;
				
			}	
	
*/
}

void list_psp(CHAR_DATA *ch,char *argument,int item_type,int objectDissallow,int lvnum,int hvnum)
{
	int vnum_count = 0;
	int v0123 = 0;
	char buf_color[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	OBJ_INDEX_DATA *pObj;
	bool found = FALSE;

		for (vnum_count = lvnum; vnum_count <= hvnum; vnum_count++)
			if ( ((pObj = get_obj_index(vnum_count)) != NULL && pObj->item_type == item_type) )
			 {				
				xprintf( buf,"VNUM: %5d, Level:%3d",vnum_count,pObj->value[0]);
				
				found = FALSE;
				for ( v0123 = 1; v0123 < 5; v0123++ )
				 {
					switch(skill_table[pObj->value[v0123]].objectDissallow[objectDissallow])
					  {
							case 0:
									xprintf( buf_color," {R[v%d]Spell: %s{X ",v0123,pObj->value[v0123] != -1 ? skill_table[pObj->value[v0123]].name : "none");
									if (!str_cmp (argument, "R"))
										found= TRUE;
							break;
							case 1:
									xprintf( buf_color,"%s {y[v%d]Spell: %s{X ",buf_color,v0123,pObj->value[v0123] != -1 ? skill_table[pObj->value[v0123]].name : "none");
									if (!str_cmp (argument, "y"))
										found= TRUE;

							break;
							case 2:
									xprintf( buf_color,"%s {Y[v%d]Spell: %s{X ",buf_color,v0123,pObj->value[v0123] != -1 ? skill_table[pObj->value[v0123]].name : "none");
									if (!str_cmp (argument, "Y"))
										found= TRUE;

							break;
							
							case 3:
									xprintf( buf_color,"%s {g[v%d]Spell: %s{X ",buf_color,v0123,pObj->value[v0123] != -1 ? skill_table[pObj->value[v0123]].name : "none");
									if (!str_cmp (argument, "g"))
										found= TRUE;

							break;
							case 4:
									xprintf( buf_color,"%s {G[v%d]Spell: %s{X ",buf_color,v0123,pObj->value[v0123] != -1 ? skill_table[pObj->value[v0123]].name : "none");
									if (!str_cmp (argument, "G"))
										found= TRUE;

							break;
							default:
									xprintf(buf_color,"%s {G[v%d]Spell: None{X",buf_color,v0123);
							break;
						}
					
		
				 }
				if (found)

						{
							send_to_char(buf, ch );
							send_to_char(buf_color , ch );
							send_to_char("\n\r" , ch );		
						}
					
				found = FALSE;
				 }
}

void do_check (CHAR_DATA *ch, char *argument)
{
	int lvnum = 0;
	int hvnum = 0;
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	int item_type = -1;
	int objectDissallow = -1;
	
	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);

		if (arg2[0] == '\0')
		 {
			send_to_char( "Syntax: (List disallowed in area or all)\n\r", ch );
			send_to_char( "  check potions (R|y|Y|g|G) (all)\n\r", ch );
			send_to_char( "  check scrolls (R|y|Y|g|G) (all)\n\r", ch );
			send_to_char( "  check wand    (R|y|Y|g|G) (all)\n\r", ch );
			send_to_char( "  check staff   (R|y|Y|g|G) (all)\n\r", ch );
			send_to_char( "  check pill    (R|y|Y|g|G) (all)\n\r", ch );
			send_to_char( "  check mobile  (R|y|Y|g|G) (all)\n\r", ch );
			send_to_char( "  check armor   (R|y|Y|g|G) (all)\n\r", ch );
			send_to_char( "  check armor   (R|y|Y|g|G) (all)\n\r", ch );
			return;
		 }

	if (!str_cmp (argument, "all"))
		{
	  	lvnum = 1;
	  	hvnum = 60000;
	  }
	else
	 {
	  	lvnum = ch->in_room->area->min_vnum;
	  	hvnum = ch->in_room->area->max_vnum;
	 }
	 
	if (!str_cmp (arg1, "potions"))
		{
			item_type = ITEM_POTION;
			objectDissallow = 0;
			list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}

	if (!str_cmp (arg1, "scrolls"))
		{
			item_type = ITEM_SCROLL;
			objectDissallow = 1;
			list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}
			
	if (!str_cmp (arg1, "pill"))
		{
			item_type = ITEM_PILL;
			objectDissallow = 2;
			list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}


	if (!str_cmp (arg1, "wand"))
		{
			item_type = ITEM_WAND;
			objectDissallow = 3;
			//list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}
		
	if (!str_cmp (arg1, "staff"))
		{
			item_type = ITEM_STAFF;
			objectDissallow = 4;
			//list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}
		
	if (!str_cmp (arg1, "armor"))
		{
			item_type = ITEM_ARMOR;
			//objectDissallow = 4;
			//list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}					
					
	if (!str_cmp (arg1, "weapon"))
		{
			item_type = ITEM_WEAPON;
			//objectDissallow = 4;
			//list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}					

	if (!str_cmp (arg1, "mobile"))
		{
			//item_type = ITEM_WEAPON;
			//objectDissallow = 4;
			//list_psp(ch,arg2,item_type,objectDissallow,lvnum,hvnum);
			return;
		}

	do_check(ch,"");
	return;
}

void do_dpxp (CHAR_DATA *ch, char *argument)
{
	if (double_xp)
		{
			double_xp = FALSE;
			send_to_char("double xp off.\n\r", ch);
		}
	else
		{
		double_xp = TRUE;
		send_to_char("double xp on.\n\r", ch);	
	}
}