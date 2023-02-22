#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* unlink() */ 
#include <time.h>
#include "merc.h"
#include "interp.h"
#include "strategy.h"
#include "fok_struct.h"


char *get_alingemt(CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];
  
	if (IS_GOOD(ch))
    	xprintf( buf,"Good ");
    else if (IS_EVIL(ch))
    	xprintf( buf,"Evil ");
    else
		xprintf( buf,"Neut ");
  return  str_dup(buf);
}


char* get_ethos(CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];

	if (ch->ethos == 1) 
		xprintf( buf,"Lawful");
	else if (ch->ethos == 2)
    	xprintf( buf,"Neutral");
    else if (ch->ethos == 3)
    	xprintf( buf,"Chaotic");
    else
    	xprintf( buf,"NONE");

  return  str_dup(buf);
}


char* get_saves(CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];

  	xprintf( buf,"Disabled");
  return  str_dup(buf);
  
}



char *get_position(CHAR_DATA *ch)
{
  char buf[MAX_STRING_LENGTH];
  
xprintf( buf,"POS_ %d",ch->position); 
  bug(buf,0);
    switch ( ch->position )
    {
    case POS_DEAD:     
	xprintf( buf,"DEAD");
	return  str_dup(buf);
	break;
    case POS_MORTAL:
	xprintf( buf,"Mortally Wounded");
	return  str_dup(buf);
	break;
    case POS_INCAP:
	xprintf( buf,"Incapacitated");
	return  str_dup(buf);
	break;
    case POS_STUNNED:
	xprintf( buf,"Stunned");
	return  str_dup(buf);
	break;
    case POS_SLEEPING:
	xprintf( buf,"Sleeping");
	return  str_dup(buf);
	break;
    case POS_RESTING:
	xprintf( buf,"Resting");
	return  str_dup(buf);
	break;
    case POS_STANDING:
	xprintf( buf,"Standing");
	return  str_dup(buf);
	break;
    case POS_FIGHTING:
	xprintf( buf,"Fighting");
	return  str_dup(buf);
	break;
    }
  return NULL;
}


void do_4score( CHAR_DATA *ch, char *argument)
{
  char buf[MAX_STRING_LENGTH];
  char tmp1_buf[MAX_STRING_LENGTH];
  char tmp2_buf[MAX_STRING_LENGTH];
  char tmp3_buf[MAX_STRING_LENGTH];
  
	
	send_to_char("\n\r{c--------------------------------------------------------------------\n\r", ch);
	
	//pretitle, Name, title
	xprintf( tmp1_buf,"%s %s %s",IS_NPC(ch) ? "" : ch->pcdata->pretitle,ch->name,IS_NPC(ch) ? "" : ch->pcdata->title);
	xprintf( buf,"{c|{W          %-55s {c|{x\n\r",tmp1_buf);
	send_to_char(buf,ch);	
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	
	//strategy, race
	xprintf(tmp1_buf,"({y%s{c)",IS_NPC(ch) ? "Mobile" : strategy_list[ch->pcdata->strategy].name);
	xprintf(tmp2_buf,"({y%s{c)",pc_race_table[ch->race].name);
	xprintf( buf,"{c|{W Combat Strategy: {c%-10s   {c|{W Race:    {c%-10s                  {c|\n\r",tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);
	
	//position, class
	xprintf(tmp1_buf,"({y%s{c)",get_position(ch));
	xprintf(tmp2_buf,"({y%s{c)",class_table[ch->class].name);
	xprintf( buf,"{c|{W Position:        {c%10s  {c|{W Class:   {c%-10s                   {c|\n\r",tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);
	
	//speed, kingdom
	xprintf(tmp1_buf,"({yDisabled{c)");
	xprintf(tmp2_buf,"({y%s{c)",ch->kingdom == 0 ? "None" : kingdom_table[ch->kingdom].long_name);
	xprintf( buf,"{c|{W Speed:           {c%-10s  {c|{W Kingdom: {c%-10s             {c|\n\r",tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);

	//fatigued, rank
	xprintf(tmp1_buf,"({yDisabled{c)");
	xprintf(tmp2_buf,"({y%s{c)",ch->kingdom == 0 ? "None" : kingdom_table[ch->kingdom].rank_titles[ch->kingdom_rank]);
	xprintf( buf,"{c|{W Fatigued:        {c%-10s  {c|{W Rank:    {c%-10s          {c|\n\r",tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);

	//saves, alingemt, ethos
	xprintf(tmp1_buf,"({y%s{c)",get_saves(ch));
	xprintf(tmp2_buf,"({y %s  %s {c)",get_ethos(ch),get_alingemt(ch));
	xprintf( buf,"{c|{W Saves:           {c%-10s  {c|{W {c%-10s                 {c|\n\r",tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);


	send_to_char("{c--------------------------------------------------------------------\n\r", ch);
 
	//Hp, mana, move, blodpoints
	xprintf(tmp1_buf,"({R%d/%d)",ch->hit,ch->max_hit);
	xprintf(tmp2_buf,"({B%d/%d)",ch->mana,ch->max_mana);
	xprintf(tmp3_buf,"({G%d/%d)",ch->move,ch->max_move);
	xprintf(buf,"{c|{W Hp: {c%-11s{c Mp: {c%-11s{c Mv: {c%-11s{c            {c|\n\r",tmp1_buf,tmp2_buf,tmp3_buf);
	send_to_char(buf,ch);
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);


	xprintf(tmp1_buf,"{c({y%s{c)",get_stat_aliasn_curr(ch,STAT_STR));
	xprintf(tmp2_buf,"{c({y%d{c/{y%d{c)",ch->quest_curr,ch->quest_accum);
	xprintf(buf,"{c|{W Str: %-20s {c| {WGlory Points:    %-20s {c              |\n\r", tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);

	xprintf(tmp1_buf,"{c({y%s{c)",get_stat_aliasn_curr(ch,STAT_INT));
	xprintf(tmp2_buf,"{c({y%d{c/{y%d{c)",ch->rptoken,ch->trptoken);
	xprintf(buf,"{c|{W Int: %-20s {c| {WRoleplay tokens: %s {c                 |\n\r", tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);
	
	xprintf(tmp1_buf,"{c({y%s{c)",get_stat_aliasn_curr(ch,STAT_WIS));
	xprintf(buf,"{c|{W Wis: %-20s {c| {WCarrying:                                {c  |\n\r", tmp1_buf);
	send_to_char(buf,ch);

	xprintf(tmp1_buf,"{c({y%s{c)",get_stat_aliasn_curr(ch,STAT_DEX));
	xprintf(tmp2_buf,"{c({y%d{c/{y%d{c)",ch->carry_number, can_carry_n(ch));
	xprintf(buf,"{c|{W Dex: %-20s {c| {W Items:    %s                        {c|\n\r", tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);

	xprintf(tmp1_buf,"{c({y%s{c)",get_stat_aliasn_curr(ch,STAT_CON));
	xprintf(tmp2_buf,"{c({y%d{c/{y%d{c)",get_carry_weight(ch), can_carry_w(ch));
	xprintf(buf,"{c|{W Con: %-20s {c| {W Weighing: %s stones.    {c      |\n\r", tmp1_buf,tmp2_buf);
	send_to_char(buf,ch);

	xprintf(tmp1_buf,"{c({y%s{c)",get_stat_aliasn_curr(ch,STAT_AGI));
	xprintf(tmp2_buf,"{c({y%d{c)",ch->level);	
	xprintf(tmp3_buf,"{c[{yXXXXXXXXXX{c]");	
	xprintf(buf,"{c|{W Agi: %-20s {c| {W Level:    %-3s Tnl: %s         {c|\n\r", tmp1_buf,tmp2_buf,tmp3_buf);
	send_to_char(buf,ch);

	xprintf(tmp1_buf,"{c({y%s{c)",get_stat_aliasn_curr(ch,STAT_CHA));
	xprintf(tmp2_buf,"{c({y%d{c)",ch->wimpy);	
	xprintf(buf,"{c|{W Cha: %-20s {c| {W Wimpy:    %-4s                          {c|\n\r", tmp1_buf,tmp2_buf);	
	send_to_char(buf,ch);

	send_to_char("{c--------------------------------------------------------------------\n\r", ch);

	xprintf(buf,"{W Status: ");	
	send_to_char(buf,ch);
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
			send_to_char( " {c({yDrunk{c)",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
			send_to_char( " {c({yThirsty{c)", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   <=  0 )
			send_to_char( " {c({yHungry{c)",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_BLOODLUST]   <=  0 )
			send_to_char( "{c({yYou are hungry for blood.{c)",  ch );
		xprintf(buf,"\n\r");
		send_to_char(buf,ch);
	
	xprintf(buf,"{c            You also carry %d gold and %d silver.\n\r", ch->gold,ch->silver);
	send_to_char(buf,ch);
	
	send_to_char("{c--------------------------------------------------------------------\n\r", ch);

	if (ch->level > 90)
	  {
		xprintf(buf,"{c| {WHoly Light: {c({y%s{c){W  Trust: {c({y%d{c)\n\r", IS_SET(ch->act,PLR_HOLYLIGHT) ? "on" : "off",ch->trust);
		send_to_char(buf,ch);
		send_to_char("{c--------------------------------------------------------------------\n\r", ch);
	}
}
