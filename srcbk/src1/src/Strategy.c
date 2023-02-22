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
#include "strategy.h"  
#include "skill.h"

struct	strategy_type	strategy_list[]	=
  {
	{"flee"     ,STRATEGY_FLEE ,    75, 75,75 , 75, 75, 75, 75, 75, 75 },
	{"defend"   ,STRATEGY_Defend,   50, 50,50 , 50,125,125,125,125,125 },
	{"engage"   ,STRATEGY_Engage,  100,100,100,100,100,100,100,100,100 },
	{"assault"  ,STRATEGY_Assault, 125,125,125,125, 50, 50, 50, 50, 50 },
	{"berserk"  ,STRATEGY_Berserk, 150,150,150,150, 25, 25, 25, 25, 25 },
	{ NULL      ,STRATEGY_NULL,     0,   0,  0,  0,  0,  0,  0,  0,  0 }
  };


int get_effectiveness( CHAR_DATA *ch, int hand )
{
return 5;
}


int haste_affect( CHAR_DATA *ch, int hand )
{
return 25;
}

int weapon_parry( CHAR_DATA *ch, int hand )
{
   int chance;
   OBJ_DATA *weapon;


	if (hand == 1)
		weapon = get_eq_char( ch, WEAR_WIELD );
	else
		weapon = get_eq_char( ch, WEAR_SECONDARY );  
	 
	 if (weapon == NULL)
	 	return -100;
	 	
	 switch (weapon->value[0])  // type of weapon

	 {
		case WEAPON_SWORD: 
			chance +=8;
			break;
		case WEAPON_DAGGER:
			chance += 3;
			break;
		case WEAPON_AXE:
			chance -= 20;
			break;
		case WEAPON_FLAIL:
			chance -= 7;
			break;
		case WEAPON_WHIP:
			chance -= 7;
			break;
		case WEAPON_POLEARM:
			chance -= 10;
			break;
		case WEAPON_BOW:
			chance += 6;
			break;
		case WEAPON_LANCE:
			chance -= 15;
			break;
		default: break;
	 }
return chance;
}

void strategy_set( CHAR_DATA *ch, int i )
  {
	return;
  }

int strategy_percent(int tag,int strategy,int base)
  {
    int		Normal = 0;    
    float 	Convert = 0.0;


	  switch(tag)
	    {
	  	case 0:
			Normal = base * strategy_list[strategy].primaryproficiency;
	  	break;
	  	case 1:
			Normal = base * strategy_list[strategy].secondaryproficiency;
	  	break;
	  	case 2:
	  		Normal = base * strategy_list[strategy].hitbonus;
	  	break;
	  	case 3:
	  		Normal = base * strategy_list[strategy].damagebonus;
	  	break;
	  	case 4:
	  		Normal = base * strategy_list[strategy].primaryparry;
	  	break;
	  	case 5:
	  		Normal = base * strategy_list[strategy].secondaryparry;
	  	break;
	  	case 6:
	  		Normal = base * strategy_list[strategy].dodge;
	  	break;
	  	case 7:
	  		Normal = base * strategy_list[strategy].avoide;
	  	break;
	  	case 8:
			Normal = base * strategy_list[strategy].shieldblock;
	  	break;
	  }	
	
	Normal = Normal / 100;
	Convert = Normal / 250.0;
	Normal = Convert * 100;
	return Normal;	
  }


bool strategy_allowed( CHAR_DATA *ch, int i )
  {
	return TRUE;
  }

void do_strategy( CHAR_DATA *ch, char *argument )
  { //do_strategy
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_STRING_LENGTH];	
    int i = 1;
    int col = 0;
    
    
	one_argument( argument, arg );


	if (ch->pcdata->strategy >= STRATEGY_MAX)
		ch->pcdata->strategy = 2;			
	
	if ( arg[0] == '\0' )
	  { //When No argument
		send_to_char( "---------------------------------------------------------------------\n\r", ch );

		xprintf(buf,  "-               Combat Strategy:     %10s                     -\n\r", strategy_list[ch->pcdata->strategy].name);
		send_to_char(buf,  ch );
		send_to_char( "---- Offense: -------------------------------------------------------\n\r", ch );
		xprintf(buf, "- Primary hand proficiency:    %s   -\n\r", bar(strategy_percent(1,ch->pcdata->strategy,base_primary_proficiency(ch))) );
		send_to_char(buf,  ch );
		xprintf(buf, "- Secondary hand proficiency:  %s   -\n\r", bar(strategy_percent(2,ch->pcdata->strategy,base_secondary_proficiency(ch))) );
		send_to_char(buf,  ch );
		xprintf(buf, "- Hit Bonus:                   %s   -\n\r", bar(strategy_percent(3,ch->pcdata->strategy,base_hit(ch))) );
		send_to_char(buf,  ch );
		xprintf(buf, "- Damage Bonus:                %s   -\n\r", bar(strategy_percent(4,ch->pcdata->strategy,base_dam(ch))) );
		send_to_char( "---- Defense: -------------------------------------------------------\n\r", ch );
		xprintf(buf, "- Effective Primary Parry:     %s   -\n\r", bar(strategy_percent(5,ch->pcdata->strategy,base_parry_primary(ch))) );
		send_to_char(buf,  ch );
		xprintf(buf, "- Effective Secondary Parry:   %s   -\n\r", bar(strategy_percent(6,ch->pcdata->strategy,base_parry_secondary(ch))) );
		send_to_char(buf,  ch );
		xprintf(buf, "- Effective Dodge:             %s   -\n\r", bar(strategy_percent(7,ch->pcdata->strategy,base_dodge(ch))) );
		send_to_char(buf,  ch );
		xprintf(buf, "- Effective Avoide:            %s   -\n\r", bar(strategy_percent(8,ch->pcdata->strategy,base_avoide(ch))) );
		send_to_char(buf,  ch );
		xprintf(buf, "- Effective Shield block:      %s   -\n\r", bar(strategy_percent(9,ch->pcdata->strategy,base_shieldblock(ch))) );
		send_to_char( "---------------------------------------------------------------------\n\r", ch );
		return;
	  } //When No argument
	
	if ( !str_cmp( arg, "list" ))
	  { //When argument list
		for ( i = 0; strategy_list[i].name != NULL; i++ )
		  { //Looping strategy list
       			if (strategy_allowed(ch,i))
       			  { // is allowed
       				xprintf( buf, "%-10s ", strategy_list[i].name);
       				send_to_char( buf, ch );
       				if ( ++col % 4 == 0 )
					send_to_char( "\n\r", ch );
			  } // is allowed
		 } //Looping strategy list
		send_to_char( "\n\r", ch );
		return;
	  } //When argument list

	for ( i = 0; strategy_list[i].name != NULL; i++ )
		if (!str_prefix( arg, strategy_list[i].name ))
		  {
		  	if (strategy_allowed(ch,i))
		  	  {
		  	 	ch->pcdata->strategy = i;
		  	 	xprintf(buf,  "You strategy is now %s\n\r", strategy_list[ch->pcdata->strategy].name);
				send_to_char(buf,  ch );
		  	 }
			else
		  	 send_to_char( "You do know any strategy \n\r", ch );
		  
		  	break;
		  }
	
	if (i == STRATEGY_MAX)
		send_to_char( "You do know any strategy \n\r", ch );
	
    return;
  } //do_strategy


int base_parry_primary( CHAR_DATA *ch)
  { // base parry
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;


	chance = ch->pcdata->learned[gsn_parry] / 4;
/*
	switch(get_effectiveness(ch,gsn_parry))
	  { //switch get_skill
		case SKILL_BS:
			chance += 25;	
		break;
		case SKILL_SK:
			chance += 40;
		break;
		case SKILL_EX:
			chance += 50;
		break;
		case SKILL_MS:
			chance += 75;
		break;
		case SKILL_GM:
			chance += 100;
		break;
		default:
			wiznet("File: Strategy.c Function: base_parry_primary Bug: Wrong effectiveness", NULL, NULL, WIZ_MEMCHECK,0,0) ;
		break;
	  } //switch get_skill
*/
	
	chance += dex_app[get_curr_stat(ch,STAT_DEX)].parry;
	chance += weapon_parry(ch,0);


	if (chance <= 0)
		return 0;
    
    if (chance > 100)
    	chance = 100;
    	
    return chance;
    	
} // base parry

int base_parry_secondary( CHAR_DATA *ch)
  { // base parry
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;

	if ( get_eq_char( ch, WEAR_SECONDARY ) == NULL )
		return 0;


	switch(get_effectiveness(ch,gsn_parry))
	  { //switch get_skill
		case SKILL_BS:
			chance += 25;	
		break;
		case SKILL_SK:
			chance += 40;
		break;
		case SKILL_EX:
			chance += 50;
		break;
		case SKILL_MS:
			chance += 75;
		break;
		case SKILL_GM:
			chance += 100;
		break;
		default:
			wiznet("File: Strategy.c Function: base_parry_primary Bug: Wrong effectiveness", NULL, NULL, WIZ_MEMCHECK,0,0) ;
		break;
	  } //switch get_skill

	
	chance += dex_app[get_curr_stat(ch,STAT_DEX)].parry;
	chance += weapon_parry(ch,0);
	chance += haste_affect(ch,0);

	if (chance <= 0)
		return 0;
    
    return chance;
} // base parry

int base_hit( CHAR_DATA *ch)
  { // base hit
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;

    
    return GET_HITROLL(ch);
} // base hit

int base_dam( CHAR_DATA *ch)
  { // base hit
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;

    
    return GET_DAMROLL(ch);
} // base hit

int base_dodge( CHAR_DATA *ch)
  { // base hit
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;

    
    return 25;
} // base hit

int base_avoide( CHAR_DATA *ch)
  { // base hit
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;

    
    return 25;
} // base hit

int base_shieldblock( CHAR_DATA *ch)
  { // base hit
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;

    
    return 25;
} // base hit



int base_primary_proficiency( CHAR_DATA *ch)
  { // base hit
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;



    
    return 25;
} // base hit

int base_secondary_proficiency( CHAR_DATA *ch)
  { // base hit
    int chance = 0;

	if ( !IS_AWAKE(ch) )
		return 0;

    
    return 25;
} // base hit
