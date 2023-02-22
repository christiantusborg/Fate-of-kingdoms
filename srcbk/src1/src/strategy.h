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

void do_strategy	args ( (CHAR_DATA *ch, char *argument) );
extern  struct  strategy_type	strategy_list	[];
#define STRATEGY_NULL    -1
#define STRATEGY_FLEE    0
#define STRATEGY_Defend  1
#define STRATEGY_Engage  2
#define STRATEGY_Assault 3
#define STRATEGY_Berserk 4
#define STRATEGY_MAX 	 5


struct	strategy_type
{
	char * const	name;
	sh_int		bit;
	sh_int		primaryproficiency;
	sh_int		secondaryproficiency;
	sh_int		hitbonus;
	sh_int		damagebonus;
	sh_int		primaryparry;
	sh_int		secondaryparry;
	sh_int		dodge;
	sh_int		avoide;
	sh_int		shieldblock;
};
