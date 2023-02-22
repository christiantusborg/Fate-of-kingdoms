/****************************************************************************
*     Fate of Kingdom 4.0 is copyright 2000-???? Christian Tusborg (Kazmir) *
*     By using this code, you have agreed to follow the terms of the        *
*     Fate license.  The use of this code without the permission of the     *
*     author is prohibited.  You will be punished to the fullest extend of  *
*     the law.								    *
*     									    *
*     Original Fate of Kingdom Mud (mud.kingdom.dk 4000)		    *
****************************************************************************/

#define 	NAV 	90

//group act
#define		GROUP_ACT_NONE	0
#define		GROUP_ACT_SKILL	1
#define		GROUP_ACT_CAST	2
#define		GROUP_ACT_SONG	3
#define		GROUP_ACT_DANCE	4
#define		GROUP_ACT_FOCUS	5

//group sort
#define		GROUP_SORT_NONE			0
#define		GROUP_SORT_CURA			1
#define		GROUP_SORT_HEALING		2
#define		GROUP_SORT_DETECTION		3
#define		GROUP_SORT_ENCHANTMENT		4
#define		GROUP_SORT_MIND			5
#define		GROUP_SORT_NECRO		6
#define		GROUP_SORT_ATTACK		7
#define		GROUP_SORT_PROTECTIVE		8
#define		GROUP_SORT_ILLUSION		9
#define		GROUP_SORT_NATURE		10
#define		GROUP_SORT_DEFENSIVE		11
#define		GROUP_SORT_CREATION		12
#define		GROUP_SORT_MALADICTIONS		13
#define		GROUP_SORT_TRANSPORTATION	14
#define		GROUP_SORT_GROUP_WIZARD		15
#define		GROUP_SORT_STEALTH		16
#define		GROUP_SORT_SUDDENDEATH		17
#define		GROUP_SORT_BEGUILING		18
#define		GROUP_SORT_WEAPON		19


struct	abillities_type
{
	char *		name;
	sh_int		class_level[MAX_CLASS];
	sh_int		class_max[MAX_CLASS];
	sh_int		class_min[MAX_CLASS];
  sh_int		rating[MAX_CLASS];
	sh_int		race_max[MAX_RACE];    
  SPELL_FUN *	spell_fun;
  sh_int		target;
  sh_int		position;
  sh_int *	pgsn;
  sh_int		slot;
  sh_int		mana;
  sh_int		beats[5];
  char *		noun_damage;
  char *		msg_off;
  char *		msg_obj;
  sh_int		group_act;
  sh_int		group_sort;
};
