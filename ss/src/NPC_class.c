/****************************************************************************
*     Fate of Kingdom 4.0 is copyright 2000-???? Christian Tusborg (Kazmir) *
*     By using this code, you have agreed to follow the terms of the        *
*     Fate license.  The use of this code without the permission of the     *
*     author is prohibited.  You will be punished to the fullest extend of  *
*     the law.								                                              *
*     									                                                    *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		                *
****************************************************************************/
/****************************************************************************
*                             NPC Classes                                   *
*                                                                           *
*****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "tables.h"
#include "olc.h"
#include "recycle.h"


//struct	npc_class_type
//{
//    char *	name;	
//    sh_int	hitpoint;
//    sh_int	manapoint;
//};
const	struct	npc_class_type	npc_class_table	[]	=
{
	{
		"none",
		9999,
		1
	},
	{
		"warrior",
		12,
		1
	},
	{
		"shaman",
		6,
		1
	},	
	{
		"paladin",
		10,
		1
	},
	{
		"black-knight",
		10,
		1
	},
	{
		"thief",
		6,
		1
	},
	{
		"inquisitor",
		8,
		1
	},
	{
		"spellfilcher",
		5,
		1
	},
	{
		"cleric",
		6,
		1
	},
	{
		"mage",
		4,
		1
	},
	{
		"ranger",
		8,
		1
	},
	{
		"assassin",
		7,
		1
	},
	{
		"bard",
		7,
		1
	},
	{
		"alchemist",
		4,
		1
	},
	{
		"druid",
		5,
		1
	},
	{
		"monk",
		8,
		1
	},
	{
		"necromancer",
		4,
		1
	},
	{
		"animal",
		4,
		1
	},
	{
		NULL,
		4,
		1
	}
};

