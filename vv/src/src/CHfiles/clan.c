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
#include <sys/time.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include "merc.h"  
#include "recycle.h" 
#include "tables.h"  
#include "lookup.h"  
#include "olc.h" 

DECLARE_DO_FUN( do_anon_shout	);
DECLARE_DO_FUN( do_font		);
void save_area( AREA_DATA *pArea );

/* returns number of your own guardians in the whole AREA */
int number_of_guardians_area(CHAR_DATA *ch)
{
CHAR_DATA *pChar;
CHAR_DATA *pChar_next;
AREA_DATA *pArea=ch->in_room->area;
ROOM_INDEX_DATA *room;
int guardians=0,room_vnum;

/*for every room in area*/ 
for ( room_vnum = pArea->min_vnum; room_vnum <= pArea->max_vnum; room_vnum++ )
  {
   room = get_room_index(room_vnum);
   if (room != NULL) { 
   for ( pChar = room->people; pChar; pChar = pChar_next )
     {
      pChar_next = pChar->next_in_room;
      if (IS_NPC(pChar))
      if (pChar->pIndexData->vnum == ch->kingdom*10+100)
         guardians+=1;
     }
    }
  }

return guardians;
}


/* returns number of your own guardians in your room */
int number_of_guardians(CHAR_DATA *ch)
{
CHAR_DATA *pChar;
CHAR_DATA *pChar_next;
int guardians=0;

   for ( pChar = ch->in_room->people; pChar; pChar = pChar_next )
   {
      pChar_next = pChar->next_in_room;
      if (IS_NPC(pChar))
      if (pChar->pIndexData->vnum == ch->kingdom*10+100)
         guardians+=1;
   }

return guardians;
}


/* will delete the first reset of the room ch is in*/
/* guardian MUST be in first reset position in rooms */
/* This should ONLY be called if guardian resets #1 in the room */
/* guardians must not move from room they are reset in under ANY circumstance */
/* call this from a death_prog for guardians */
void remove_guardians(CHAR_DATA *ch) {

  ROOM_INDEX_DATA *pRoom = ch->in_room;
  RESET_DATA *pReset = NULL;
  CHAR_DATA *pChar;int i;int
  original=number_of_guardians(ch);

for (i=1,pChar = ch->in_room->people; i <= original; i++,pChar=pChar->next_in_room )
	{
if (!ch->in_room->reset_first ) return;
              pReset = pRoom->reset_first;
              pRoom->reset_first= pRoom->reset_first->next;
              if ( !pRoom->reset_first ) pRoom->reset_last = NULL;
      if(IS_NPC(pChar))
/*      if(pChar->pIndexData->vnum == ch->kingdom*10+100)
		extract_char(pChar,TRUE); *//* purge existing guardians */
		free_reset_data(pReset );
	}/*for*/
	return;
}
/* will add the guardian as the rooms first reset and load the mobs there */
/* can not place guardians with symbol */
/* on the other side of this : symbols should never even have a chance */
/* of being placed with the guardian, i think it is mechanically impossible */
/* if not, then changes to the code will have to be made */
/* this will place this many guardians in the room */
void place_guardians(CHAR_DATA *ch,int number)
{
    RESET_DATA *pReset = NULL;
    int kingdom = ch->kingdom;
    int i;
    MOB_INDEX_DATA *pMobIndex=get_mob_index(ch->kingdom*10+100);
    CHAR_DATA *victim;

	for (i=1 ;i <= number ; i++ )
	{

	pReset = new_reset_data();
	pReset->arg1= kingdom*10+100; 

	if (pMobIndex == NULL)
	{
	send_to_char( "{cStrange, the guardian does not exist...{x\n\r",ch);
	return;
	}

	pReset->command  = 'M';
	pReset->arg2 = 999;	/* MAX # */
	pReset->arg3 = ch->in_room->vnum;
	pReset->arg4 = number;	/* MIN # */

	add_reset( ch->in_room, pReset, i ); /*always at position 1 to start*/

    /* loads the mob */
    	victim = create_mobile( pMobIndex );
    	char_to_room( victim, ch->in_room );
    /* done loading mob */

}/*for*/

return;
}

/* will delete the first reset of the room ch is in*/
/* symbol MUST be in first reset position in rooms */
/* This should ONLY be called if symbol is in the room */

void remove_symbol(CHAR_DATA *ch)
{
ROOM_INDEX_DATA *pRoom = ch->in_room;
RESET_DATA *pReset = NULL;

  if ( !ch->in_room->reset_first ) {
   send_to_char( "{cSomething went wrong...there is nothing here.{x\n\r",ch);
  return;
	}

        pReset = pRoom->reset_first;
        pRoom->reset_first = pRoom->reset_first->next;
            if ( !pRoom->reset_first )
                  pRoom->reset_last = NULL;
  
	free_reset_data( pReset );

	return;
	}

/* will add the ch's kingdom symbol as the rooms first reset */
void place_symbol(CHAR_DATA *ch) {

	RESET_DATA *pReset = NULL;
	int kingdom = ch->kingdom;

	pReset = new_reset_data();
	pReset->arg1= kingdom+190; 

  if (get_obj_index(ch->kingdom+190) == NULL) {
	send_to_char( "{cStrange, the symbol does not exist...{x\n\r",ch);
  return;
  }

pReset->command  = 'O';
pReset->arg2 = 0;
pReset->arg3 = ch->in_room->vnum;
pReset->arg4 = 0;

add_reset( ch->in_room, pReset, 1 ); /*always at position 1*/
return;
}

bool warring(int kingdom, int enemy)
{
if (kingdom==enemy) return FALSE;
if (kingdom_table[kingdom].at_war_with[enemy]==1) return TRUE;
if (kingdom_table[enemy].at_war_with[kingdom]==1) return TRUE;
if (kingdom_table[kingdom].at_war_with[enemy]==-1) return TRUE;
if (kingdom_table[enemy].at_war_with[kingdom]==-1) return TRUE;
return FALSE;
}

/* MLK */
/* returns true if area is clear of enemies (pcs and guardians)     */
bool check_desecration(CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int x, room_vnum;
CHAR_DATA *pChar;
CHAR_DATA *pChar_next;
AREA_DATA *pArea=ch->in_room->area;
ROOM_INDEX_DATA *room;

      if (!str_cmp (ch->name, "Kazmir"))
                return TRUE;
	if (!str_cmp (ch->name, "Derkec"))
                return TRUE;     
	if (!str_cmp (ch->name, "Fletcher"))
                return TRUE;          

/*for every room in area*/
for ( room_vnum = pArea->min_vnum; room_vnum <= pArea->max_vnum; room_vnum++ )
  {
   room = get_room_index(room_vnum);
   if (room != NULL) {
   for ( pChar = room->people; pChar; pChar = pChar_next )
     {
      pChar_next = pChar->next_in_room;
      if (IS_NPC(pChar)) {
	      if ((pChar->pIndexData->vnum >= 110) 
		 && (pChar->pIndexData->vnum <= 169))
		{
		   x=(pChar->pIndexData->vnum-100)/10; 

		   if (ch->kingdom!=x) {
		       sprintf(buf,"{c%s is still guarding the area.{x\n\r",pChar->short_descr);		
 		       send_to_char(buf,ch);
		       return FALSE; /* enemy guardian found */
		   }
		}
	   } /* NPC */
	   if(!IS_NPC(pChar)) /* enemy pc found -----------v */
	      if ( warring(ch->kingdom,pChar->kingdom) ) 
	      {
	        sprintf(buf,"{cYou can not do that with %s in the area.{x\n\r",pChar->name);		
 		send_to_char(buf,ch);
		return FALSE; /* enemy pc found */
	      }
   }
}
}
return TRUE;
}

/* returns true if area is clear of mobs and enemy guardians        */
bool check_consecration(CHAR_DATA *ch)
{
char buf[MAX_STRING_LENGTH];
int x, room_vnum;
CHAR_DATA *pChar;
CHAR_DATA *pChar_next;
AREA_DATA *pArea=ch->in_room->area;
ROOM_INDEX_DATA *room;

/*for every room in area*/
           if ((!str_cmp (ch->name, "Kazmir")) || 
              (!str_cmp (ch->name, "Fletcher"))|| 
              (!str_cmp (ch->name, "Derkec")))
                return TRUE;     


for ( room_vnum = pArea->min_vnum; room_vnum <= pArea->max_vnum; room_vnum++ )
  {
   room = get_room_index(room_vnum);
   if (room != NULL) {
   for ( pChar = room->people; pChar; pChar = pChar_next )
     {
      pChar_next = pChar->next_in_room;
      if (IS_NPC(pChar)) {
	      if ((pChar->pIndexData->vnum < 110) 
		 || (pChar->pIndexData->vnum > 169)) {
		  sprintf(buf,"{c%s is still in the area.{x\n\r",pChar->short_descr);		
		  send_to_char(buf,ch);
		  return FALSE; /*regular mob found*/
              }

	      if ((pChar->pIndexData->vnum >= 110) 
		 && (pChar->pIndexData->vnum <= 169))
		{
		   x=(pChar->pIndexData->vnum-100)/10; 

		   if (ch->kingdom!=x) {
		       sprintf(buf,"{c%s is still loitering about in the area.{x\n\r",pChar->short_descr);		
 		       send_to_char(buf,ch);
		       return FALSE; /* enemy guardian found */
		   }
		}
	   } /* NPC */
	}
   }
}	
return TRUE;
}

/* returns whether of not ch is in a room with any clan symbol */
bool with_symbol(CHAR_DATA *ch)
{
OBJ_DATA *obj;
OBJ_DATA *obj_next;

   for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
   {
   obj_next = obj->next_content;
   if ((obj->pIndexData->vnum >= 190) && (obj->pIndexData->vnum <= 196))
      return TRUE;
   }
return FALSE;
}

/* should only be called when there is room for members */
void add_member (char *name, int kingdom)
{
int i;
int fixadd;
   fixadd = 0;
   for ( i=0 ; i < MAX_KINGDOM_MEMBERS ; i++ ) {
      if (is_full_name (kingdom_table[kingdom].members[i],"NONE") && (fixadd < 1)) {
      strcpy(kingdom_table[kingdom].members[i],name);
      fixadd = 1;
      break; /* added the name in place of first NONE */
      }	
   }
bug("add_member: No slots empty in kingdom_table[%d].members[]",kingdom);
return;
}

/* should only be called when name is in memberlist */
void subtract_member (char *name, int kingdom)
{
int i;
char bugbuf[200];

   for ( i=0 ; i < MAX_KINGDOM_MEMBERS ; i++ ) {
      if (is_full_name (kingdom_table[kingdom].members[i],name)) {
      strcpy(kingdom_table[kingdom].members[i],"NONE");
      return; /* added NONE in place name */
      }	
   }

sprintf(bugbuf,"subtract_member: kingdom_table[%d].members[] --> %s not found",kingdom,name);
bug(bugbuf,0);
return;
}

void do_clan(CHAR_DATA *ch, char *argument)
{
char arg1[MAX_STRING_LENGTH];
char arg2[MAX_STRING_LENGTH];
char  buf[MAX_STRING_LENGTH];
int enemy;
long time;
int timeleft;

    if (argument[0] == '\0')
    {
    send_to_char("{CKINGDOM commands: INFO BALANCE DEPOSIT WITHDRAW AREAS MEMBERS{x\n\r",ch);
    send_to_char("{C                  INDUCT BANISH PROMOTE DEMOTE WAR PEACE{x\n\r",ch);
    send_to_char("{C                  RESIGN DESECRATE CONSECRATE GUARDIAN PROSPECTS{x\n\r",ch);
        return;
    }

argument = one_argument(argument, arg1);
argument = one_argument(argument, arg2);

    if (arg1[0] == '\0')
    {
    send_to_char("{CKINGDOM commands: INFO BALANCE DEPOSIT WITHDRAW AREAS MEMBERS{x\n\r",ch);
    send_to_char("{C               INDUCT BANISH PROMOTE DEMOTE WAR PEACE{x\n\r",ch);
    send_to_char("{C               RESIGN DESECRATE CONSECRATE GUARDIAN PROSPECTS{x\n\r",ch);
        return;
    }

    if (!strcmp(arg1, "info"))
    {    
    AREA_DATA *pArea;
    int clan,i;
    char buf[100];

    if (arg2[0] == '\0'){send_to_char("{cYou must provide a name.{x\n\r",ch);return;}
	     if (!strcmp(arg2, "gondor")) 	{clan=1;}
	else if (!strcmp(arg2, "aldon")) 	{clan=2;}
	else if (!strcmp(arg2, "mordukhan")) 	{clan=3;}
	else if (!strcmp(arg2, "drakkon")) 	{clan=4;}
	else if (!strcmp(arg2, "narssius")) 	{clan=5;}
	else if (!strcmp(arg2, "arkadia")) 	{clan=6;}
	else if (!strcmp(arg2, "talastin"))	{clan=7;}
	else {send_to_char("{cNo such kingdom.{x\n\r",ch);return;}

    send_to_char("\n\r",ch);
    sprintf(buf,"{CName: %s{x\n\r",kingdom_table[clan].name);
    send_to_char(buf,ch);
    sprintf(buf,"{C%s, %s{x\n\r",kingdom_table[clan].short_name,kingdom_table[clan].long_name);
    send_to_char(buf,ch);

    sprintf(buf,"{cRank 10: %-20s %-20s",kingdom_table[clan].rank_titles[9],
		kingdom_table[clan].rank_titlesf[9]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 9:  %-20s %-20s",kingdom_table[clan].rank_titles[8],
		kingdom_table[clan].rank_titlesf[8]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 8:  %-20s %-20s",kingdom_table[clan].rank_titles[7],
		kingdom_table[clan].rank_titlesf[7]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 7:  %-20s %-20s",kingdom_table[clan].rank_titles[6],
		kingdom_table[clan].rank_titlesf[6]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 6:  %-20s %-20s",kingdom_table[clan].rank_titles[5],
		kingdom_table[clan].rank_titlesf[5]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 5:  %-20s %-20s",kingdom_table[clan].rank_titles[4],
		kingdom_table[clan].rank_titlesf[4]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 4:  %-20s %-20s",kingdom_table[clan].rank_titles[3],
		kingdom_table[clan].rank_titlesf[3]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 3:  %-20s %-20s",kingdom_table[clan].rank_titles[2],
		kingdom_table[clan].rank_titlesf[2]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 2:  %-20s %-20s",kingdom_table[clan].rank_titles[1],
		kingdom_table[clan].rank_titlesf[1]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);
    sprintf(buf,"{cRank 1:  %-20s %-20s",kingdom_table[clan].rank_titles[0],
		kingdom_table[clan].rank_titlesf[0]);
    send_to_char(buf,ch);send_to_char("\n\r",ch);

    send_to_char("\n\r",ch);
    send_to_char("{BAt war with:{x\n\r{b",ch);
    for ( i=1 ; i < MAX_KINGDOM ; i++ ) {
	if (warring(clan,i)) {

	if (kingdom_table[ch->kingdom].at_war_with[i]==-1)
		send_to_char("(",ch); 
		/* parenthesis around peace treaty clans */

	send_to_char(kingdom_table[i].short_name,ch);

	if (kingdom_table[ch->kingdom].at_war_with[i]==-1)
		send_to_char(")",ch);
		/* parenthesis around peace treaty clans */

	send_to_char("   ",ch);
	} /*if*/
	} /*for*/
    send_to_char("{x\n\r",ch);

    /* areas */
    send_to_char("{BAreas:{x\n\r{b",ch);
    for ( pArea = area_first; pArea; pArea = pArea->next ) {
	if (is_full_name(pArea->owner,kingdom_table[clan].short_name)) {
	send_to_char(pArea->name,ch);send_to_char("   ",ch);
	} /*if*/
	} /*for*/
    send_to_char("{x\n\r\n\r",ch);


    WAIT_STATE(ch,3);    
    return;
    } /*info*/

    else if (!strcmp(arg1, "peace"))
    {    
    char enemy_name[100];

    if (ch->kingdom == 0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);
	return;
	}

    if (!IS_SET(ch->kincom,KINGDOM_C_PEACE)) {
	send_to_char("{cYou can not do that.{x\n\r",ch);
	return;
	}

    if (arg2[0] == '\0') {
	send_to_char("{cYou must provide a name.{x\n\r",ch);
	return;
	}

	     if (!strcmp(arg2, "gondor")) 	{ enemy = 1; }
	else if (!strcmp(arg2, "aldon")) 	{ enemy = 2; }
	else if (!strcmp(arg2, "mordukhan")) 	{ enemy = 3; }
	else if (!strcmp(arg2, "drakkon")) 	{ enemy = 4; }
	else if (!strcmp(arg2, "narssius")) 	{ enemy = 5; }
	else if (!strcmp(arg2, "arkadia")) 	{ enemy = 6; }
	else if (!strcmp(arg2, "talastin"))	{ enemy = 7; }

	else { send_to_char("{cNo such kingdom.{x\n\r",ch); return; }

	strcpy(enemy_name,capitalize(kingdom_table[enemy].short_name));

    if (enemy==ch->kingdom) 
	 {
	send_to_char("{gYou hear some laughing.{x\n\r",ch);return;
	}
    if (kingdom_table[ch->kingdom].at_war_with[enemy]==0){send_to_char("{cYou are already at peace with them.\n\r{x",ch);return;}
    if (kingdom_table[ch->kingdom].at_war_with[enemy]==1) {
	if (kingdom_table[enemy].at_war_with[ch->kingdom]==-1) { /* they propose? */
	sprintf(buf,"{G%s accepted peace with %s!{x",
		capitalize(kingdom_table[ch->kingdom].short_name),
		enemy_name);
	kingdom_table[ch->kingdom].at_war_with[enemy]=0; /* both at peace */
	kingdom_table[enemy].at_war_with[ch->kingdom]=0;	
	do_anon_shout(ch,buf);
	return;
	} /* end if they propose */
    sprintf(buf,"%s has proposed peace with %s!",
		capitalize(kingdom_table[ch->kingdom].short_name),
		enemy_name);
    kingdom_table[ch->kingdom].at_war_with[enemy]=-1;
	do_anon_shout(ch,buf);
    return;
    }
    if (kingdom_table[ch->kingdom].at_war_with[enemy]==-1) 
    {send_to_char("{cYou have already proposed peace with them\n\r{x",ch);return;}
    return;
    } /*peace*/

    else if (!strcmp(arg1, "war"))
    {    
    char enemy_name[100];

    if (ch->kingdom==0)
	{
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);
	return;
	}
    if (!IS_SET(ch->kincom,KINGDOM_C_WAR)) {
    send_to_char("{cYou can not do that.{x\n\r",ch);return;}

    if (arg2[0] == '\0'){send_to_char("{cYou must provide a name.{x\n\r",ch);return;}
  	if (!strcmp(arg2, "gondor"))      { enemy = 1; }
        else if (!strcmp(arg2, "aldon"))     { enemy = 2; }
        else if (!strcmp(arg2, "mordukhan"))     { enemy = 3; }
        else if (!strcmp(arg2, "drakkon"))        { enemy = 4; }
        else if (!strcmp(arg2, "narssius"))        { enemy = 5; }
        else if (!strcmp(arg2, "arkadia"))        { enemy = 6; }
	else if (!strcmp(arg2, "talastin"))	{ enemy = 7; }

	else {send_to_char("{cNo such kingdom.{x\n\r",ch);return;}

	strcpy(enemy_name,capitalize(kingdom_table[enemy].short_name));

    if (enemy==ch->kingdom) {
	send_to_char("{gYou hear some laughing.{x\n\r",ch);return;}
    if (kingdom_table[ch->kingdom].at_war_with[enemy]==1){
	    if (kingdom_table[enemy].at_war_with[ch->kingdom]==-1) {
	    sprintf(buf,"{R%s rejected peace with %s!{x",
		capitalize(kingdom_table[ch->kingdom].short_name),
		enemy_name);
	    kingdom_table[ch->kingdom].at_war_with[enemy]=1; /* both at war */
            kingdom_table[enemy].at_war_with[ch->kingdom]=1;	
	do_anon_shout(ch,buf);
 	    return;
	    }
       send_to_char("{cYou are already at war with them.\n\r{x",ch);
       return;
    }
    if (kingdom_table[ch->kingdom].at_war_with[enemy]==0) {
	sprintf(buf,"{R%s has declared war on %s!{x",
		capitalize(kingdom_table[ch->kingdom].short_name),
		enemy_name);
    kingdom_table[ch->kingdom].at_war_with[enemy]=1; /* put both at war */
    kingdom_table[enemy].at_war_with[ch->kingdom]=1;
	do_anon_shout(ch,buf);
    return;
    }
    if (kingdom_table[ch->kingdom].at_war_with[enemy]==-1)
    {
    send_to_char("{cYou can not do that, you have asked them for peace.\n\r{x",ch);
    return;
    }
    return;
    } /*war*/

    else if (!strcmp(arg1, "promote"))
    {    
	char promote_message[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

    if (ch->kingdom==0){send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
    if (arg2[0] == '\0'){send_to_char("{cYou must provide a name.{x\n\r",ch);return;}

	victim=get_char_world(ch,arg2);
	if (ch==victim) {send_to_char("{cOn yourself!?.{x\n\r",ch);return;}

    if ((victim=get_char_room( ch, arg2))==NULL)              {
        send_to_char( "{cThey are not here.{x\n\r", ch);return;}
	
	if (victim->kingdom == 0)				   {
	send_to_char("{cThey are not in a kingdom.{x\n\r", ch);return; }

	if (victim->kingdom != ch->kingdom) 	  		      {
	send_to_char("{cThey are not in your kingdom.{x\n\r", ch);
	return;}

	if (ch->kingdom_rank <= (victim->kingdom_rank+1) ) {
	send_to_char("You can't promote someone to your rank.\n\r",ch);
	return; }

	if (IS_NPC(victim)) 						{
	send_to_char("{cThey would not last long.{x\n\r", ch);	return;  }

    /* check for open positons, array numbers are 1 less than rank */
/*
    if (victim->kingdom_rank == 7 )
	if (kingdom_table[ch->kingdom].rank_slots[7] >= 2) {
	send_to_char("{cThat position has been filled.{x\n\r",ch);return;}
    if (victim->kingdom_rank == 8 )
	if (kingdom_table[ch->kingdom].rank_slots[8] >= 1) {
	send_to_char("{cThat position has been filled.{x\n\r",ch);return;}
    if (victim->kingdom_rank == 9 )
	if (kingdom_table[ch->kingdom].rank_slots[9] >= 1) {
	send_to_char("{cThat position has been filled.{x\n\r",ch);return;}

    if (victim->kingdom_rank == 9 ) 					 {
    send_to_char("{cThey are already the highest rank.{x\n\r",ch);return; }
*/	
    /* mark the records */    
/*
    kingdom_table[ch->kingdom].rank_slots[victim->kingdom_rank-1] -= 1;
    kingdom_table[ch->kingdom].rank_slots[victim->kingdom_rank]   += 1;
*/
    victim->kingdom_rank += 1;

    /* promote victim */    
    if(victim->sex == SEX_FEMALE) 
    sprintf(promote_message,"{CYou have been promoted to %s.{x\n\r",
		kingdom_table[ch->kingdom].rank_titlesf[victim->kingdom_rank] );	
    else
    sprintf(promote_message,"{CYou have been promoted to %s.{x\n\r",kingdom_table[ch->kingdom].rank_titles[victim->kingdom_rank] );	

    send_to_char(promote_message,victim);

    if(victim->sex == SEX_FEMALE) 
    sprintf(promote_message,"{C%s has been promoted to %s.{x\n\r",victim->name,kingdom_table[ch->kingdom].rank_titlesf[victim->kingdom_rank]);
    else
    sprintf(promote_message,"{C%s has been promoted to %s.{x\n\r",victim->name,kingdom_table[ch->kingdom].rank_titles[victim->kingdom_rank]);

    send_to_char(promote_message,ch);

    if (victim->kingdom_rank > 6){
    if(victim->sex == SEX_FEMALE)
    sprintf(buf,"%s ",kingdom_table[victim->kingdom].rank_titlesf[victim->kingdom_rank]);
    else
    sprintf(buf,"%s ",kingdom_table[victim->kingdom].rank_titles[victim->kingdom_rank]);
    free_string( victim->pcdata->pretitle );
    victim->pcdata->pretitle = str_dup(buf);
    }


    return;
    } /*promote*/

    else if (!strcmp(arg1, "demote"))
    {    
	char demote_message[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

    if (ch->kingdom==0){send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}

    if (!IS_SET(ch->kincom,KINGDOM_C_DEMOTE)) {
    send_to_char("{cYou can not do that.{x\n\r",ch);return;}

    if (arg2[0] == '\0'){send_to_char("{cYou must provide a name.{x\n\r",ch);return;}

	victim=get_char_world(ch,arg2);
	if (ch==victim) {send_to_char("{cOn yourself!?.{x\n\r",ch);return;}

    if ((victim=get_char_room( ch, arg2))==NULL)              {
        send_to_char( "{cThey are not here.{x\n\r", ch);return;}
	
	if (victim->kingdom_rank==0)				   {
	send_to_char("{cThey are not in a kingdom.{x\n\r", ch);return; }

	if (victim->kingdom != ch->kingdom) 	  		      {
	send_to_char("{cThey are not in your kingdom.{x\n\r", ch);return;}

	if (ch->kingdom_rank <= victim->kingdom_rank) {
	send_to_char("You can't demote someone who is above u.\n\r",ch);
	return; }

    if (victim->kingdom_rank <= 0 ) 				        {
    send_to_char("{cThey are already the lowest rank.{x\n\r",ch);return; }

	if (IS_NPC(victim)) 						{
	send_to_char("{cThey would not last long.{x\n\r", ch);	return;  }

    /* check for open positons, array numbers are 1 less than rank */
/*
    if (victim->kingdom_rank == 9 )
	if (kingdom_table[ch->kingdom].rank_slots[7] >= 2)  {
	send_to_char("{cThat position has been filled.{x\n\r",ch);return;}
    if (victim->kingdom_rank == 9 )
	if (kingdom_table[ch->kingdom].rank_slots[8] >= 1)  {
	send_to_char("{cThat position has been filled.{x\n\r",ch);return;}
*/
	
    /* mark the records */    
    victim->kingdom_rank -= 1;
/*
    kingdom_table[ch->kingdom].rank_slots[victim->kingdom_rank]   -= 1;
    kingdom_table[ch->kingdom].rank_slots[victim->kingdom_rank-1] += 1;
*/
    /* demote victim */    
    if(victim->sex == SEX_FEMALE)
    sprintf(demote_message,"{CYou have been demoted to %s.{x\n\r",kingdom_table[ch->kingdom].rank_titlesf[victim->kingdom_rank-1]);	
	else
    sprintf(demote_message,"{CYou have been demoted to %s.{x\n\r",kingdom_table[ch->kingdom].rank_titles[victim->kingdom_rank-1]);	
    send_to_char(demote_message,victim);

    if(victim->sex == SEX_FEMALE)
    sprintf(demote_message,"{C%s has been demoted to %s.{x\n\r",victim->name,kingdom_table[ch->kingdom].rank_titlesf[victim->kingdom_rank-1]);
	else
    sprintf(demote_message,"{C%s has been demoted to %s.{x\n\r",victim->name,kingdom_table[ch->kingdom].rank_titles[victim->kingdom_rank-1]);
    send_to_char(demote_message,ch);
    if (victim->kingdom_rank > 6){
	if(victim->sex == SEX_FEMALE)
    sprintf(buf,"%s ",kingdom_table[victim->kingdom].rank_titlesf[victim->kingdom_rank]);
	else
    sprintf(buf,"%s ",kingdom_table[victim->kingdom].rank_titles[victim->kingdom_rank]);
    free_string( victim->pcdata->pretitle );
    victim->pcdata->pretitle = str_dup(buf);
    }
    else {
    free_string(victim->pcdata->pretitle);
    victim->pcdata->pretitle = str_dup("");
    }
    return;
    } /*demote*/

    else if (!strcmp(arg1, "induct"))
    {    
	char induct_message[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int number_of_members,sn;
	// int i;
    if (ch->kingdom==0) {
    send_to_char("{cYou are not in a kingdom.{x\n\r",ch);
	return;
	}
    if (!IS_SET(ch->kincom,KINGDOM_C_ADD)) {
    send_to_char("{cYou can not do that.{x\n\r",ch);return;}
    if (arg2[0] == '\0'){send_to_char("{cYou must provide a name.{x\n\r",ch);return;}

	victim=get_char_world(ch,arg2);
	if (ch==victim) {send_to_char("{cOn yourself!?.{x\n\r",ch);return;}

    if ((victim=get_char_room( ch, arg2))==NULL)              {
        send_to_char( "{cThey are not here.{x\n\r", ch);return;}
	
	if (victim->level < 20) {send_to_char("{cThey are too young.{x\n\r",ch);return;}

	if (victim->kingdom_rank!=0)	     			       {
	send_to_char("{cThey are already in a kingdom.{x\n\r", ch);return;}

	if (IS_NPC(victim)) 						{
	send_to_char("{cThey would not last long.{x\n\r", ch);	return;  }

	if ((strlen(victim->description) < 250 )) {
	send_to_char("They need a better description.\n\r",ch);
	return;
	}

if ((strlen(victim->pcdata->background) < 500 )) {
send_to_char("They need a better background.\n\r",ch);
return;
}


    /* check for open positons */
    number_of_members = 0;
/*
	for (i=0 ; i < 10 ; i++ )	
	number_of_members += kingdom_table[ch->kingdom].rank_slots[i];
*/
    if (number_of_members >= MAX_KINGDOM_MEMBERS)      {
    send_to_char("{cThe kingdom is full.{x\n\r",ch);return;}
	
  time = current_time - victim->lastkingdom;
  timeleft = time/86400;

   if((timeleft < 14) && !IS_IMMORTAL(ch))
	 {
    		send_to_char("{cThey still feel too attatched to their old kingdom to join yours.{x\n\r",ch);
     		return;
	 }

    /* induct victim */    
    sprintf(induct_message,"{CYou have been inducted into %s.{x\n\r",
	kingdom_table[ch->kingdom].long_name );	
    send_to_char(induct_message,victim);

    sprintf(induct_message,"{C%s has been inducted into %s.{x\n\r",victim->name,kingdom_table[ch->kingdom].long_name);
    send_to_char(induct_message,ch);

    /* mark the records */    
    victim->kingdom_rank = 0;

/*
    kingdom_table[ch->kingdom].rank_slots[victim->kingdom_rank-1] += 1;
*/
    victim->kingdom = ch->kingdom;

    if(!IS_IMMORTAL(victim)) {
    add_member(victim->name,ch->kingdom);
    }

    /* inducting sets kingdom skills to 50, removes other kingdom spells */
    for (sn = 0; sn < MAX_SKILL; sn++) {
      if ( (victim->kingdom > 0) && 
	(skill_table[sn].kingdom == victim->kingdom))
	victim->pcdata->learned[sn] = 50;
      else if ( skill_table[sn].kingdom != KINGDOM_NONE &&
	       victim->kingdom != skill_table[sn].kingdom )
	victim->pcdata->learned[sn] = 0;
    }
    return;
    } /*induct*/

    else if (!strcmp(arg1, "banish"))
    {    
	char banish_message[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int sn;
    if (ch->kingdom==0) { 
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
    if (!IS_SET(ch->kincom,KINGDOM_C_BAN)) {
	send_to_char("{cYou can not do that.{x\n\r",ch);return;}
    if (arg2[0] == '\0'){send_to_char("{cYou must provide a name.{x\n\r",ch);return;}

	victim=get_char_world(ch,arg2);
	if (ch==victim) {send_to_char("{cOn yourself!?.{x\n\r",ch);return;}

    if ((victim=get_char_world( ch, arg2))==NULL)              {
        send_to_char( "{cThey are not here.{x\n\r", ch);return;}
	/*
	if (victim->kingdom_rank==0)	     			       {
	send_to_char("{cThey are not in a kingdom.{x\n\r", ch);return; }
*/
	if (victim->kingdom != ch->kingdom) 	  		      {
	send_to_char("{cThey are not in your kingdom.{x\n\r", ch);
	return;}

	if (ch->kingdom_rank <= victim->kingdom_rank) {
	send_to_char("You can't banish above your rank.\n\r",ch);
	return; }

    /* banish victim */    
    sprintf(banish_message,"{CYou have been banished from %s.{x\n\r",
	kingdom_table[ch->kingdom].long_name );	
    send_to_char(banish_message,victim);

    sprintf(banish_message,"{C%s has been banished from %s.{x\n\r",
	victim->name,kingdom_table[ch->kingdom].long_name);
    send_to_char(banish_message,ch);

    /* banishing removes kingdom spells */
    for (sn = 0; sn < MAX_SKILL; sn++) {
      if ( (victim->kingdom) && (skill_table[sn].kingdom == victim->kingdom))
	victim->pcdata->learned[sn] = 0;

      else if ( skill_table[sn].kingdom != KINGDOM_NONE &&
	       victim->kingdom != skill_table[sn].kingdom )
	victim->pcdata->learned[sn] = 0;
    }

    /* mark the records and actually take them out */    
/*
    kingdom_table[ch->kingdom].rank_slots[victim->kingdom_rank-1] -= 1;
*/
    victim->kingdom_rank = 0;
    victim->kingdom = 0; 
    victim->lastkingdom = current_time;
  if(!IS_IMMORTAL(victim)) { 
   subtract_member(victim->name,ch->kingdom);
   }

    free_string( victim->pcdata->pretitle );
    victim->pcdata->pretitle = str_dup("");
    return;
    } /*banish*/

    else if (!strcmp(arg1, "resign"))
    {    
	char resign_message[MAX_STRING_LENGTH];
	int sn;
    if (ch->kingdom==0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
    if (ch->kingdom_rank >= 9) {
	send_to_char("{cAs leader, you can not do that.{x\n\r",ch);return;} 
send_to_char("I am sorry but this command have been disabled, you have to ask your kingdom leader to get rid of you.\n\r",ch);
return;
    /* resign */    
    sprintf(resign_message,"{CYou have resigned from %s.{x\n\r",
	kingdom_table[ch->kingdom].long_name );	
    send_to_char(resign_message,ch);

    /* resigning removes kingdom spells */
    for (sn = 0; sn < MAX_SKILL; sn++) {
      if ( (ch->kingdom) && (skill_table[sn].kingdom == ch->kingdom))
	ch->pcdata->learned[sn] = 0;

      else if ( skill_table[sn].kingdom != KINGDOM_NONE &&
	       ch->kingdom != skill_table[sn].kingdom )
	ch->pcdata->learned[sn] = 0;
    }

    /* mark the records and actually take the ch out */    
     if(!IS_IMMORTAL(ch)) {
    subtract_member(ch->name,ch->kingdom);
    }
/*
    kingdom_table[ch->kingdom].rank_slots[ch->kingdom_rank-1] -= 1;
*/
    ch->kingdom_rank = 0;
    ch->kingdom = 0; 
    ch->lastkingdom = current_time;
    free_string( ch->pcdata->pretitle );
    ch->pcdata->pretitle = str_dup("");
    return;
    } /*resign*/

    else if (!strcmp(arg1, "prospects"))
    {    
    AREA_DATA *pArea;
    char area_line[100];
    if (ch->kingdom==0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}

	do_font(ch,"area prospects");	
	send_to_char("{x\n\r",ch);
    for ( pArea = area_first; pArea; pArea = pArea->next ) {
	if (pArea->worth > 0) {
	sprintf(area_line,"{B(%-24s) owned by {W%s{x\n\r",pArea->name,pArea->owner);
	center_to_char(area_line,ch,72);
	} /*if*/
	} /*for*/
    send_to_char("{x\n\r",ch);
    WAIT_STATE(ch,4);
    return;
    } /*prospect*/

    else if (!strcmp(arg1, "areas"))
    {    
    AREA_DATA *pArea;
    char area_line[100];
    if (ch->kingdom==0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
    sprintf(buf,"%s areas",kingdom_table[ch->kingdom].name);
	do_font(ch,buf);    
	send_to_char("{x\n\r",ch);
    for ( pArea = area_first; pArea; pArea = pArea->next ) {
	if(is_full_name(pArea->owner,kingdom_table[ch->kingdom].short_name)) {
	sprintf(area_line,"{C%-18s  Worth:{W%3d{C  Guardians:{W%d{C({W%d{W){x\n\r",pArea->name,pArea->worth,number_of_guardians_area(ch),pArea->worth/5+5);
	center_to_char(area_line,ch,72);
	} /*if*/
	} /*for*/
    send_to_char("{x\n\r",ch);
    WAIT_STATE(ch,2);
    return;
    } /*areas*/

    else if (!strcmp(arg1, "members"))
    {    
    char name[100],buf[100];
    int i,members=0;
	if(!IS_IMMORTAL(ch)) { send_to_char("This command is begin revamped.\n\r",ch); return; }
    if (ch->kingdom==0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
	
    sprintf(buf,"%s members",kingdom_table[ch->kingdom].name);
	do_font(ch,buf);    
	send_to_char("{x\n\r",ch);
	for ( i=0 ; i < MAX_KINGDOM_MEMBERS ; i++ ) {
	   if (is_full_name(kingdom_table[ch->kingdom].members[i],"NONE"))
		continue;
	members += 1;
	sprintf(name,"  {B%-24s",kingdom_table[ch->kingdom].members[i]);
	send_to_char(name,ch);
	if ((members%3)==0) send_to_char("{x\n\r",ch);
	}
    send_to_char("{x\n\r",ch);
    return;
    } /*members*/

    else if (!strcmp(arg1, "balance"))
    {    
    char buf[160];
    long bank;

    if (ch->kingdom==0){
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}

    if (IS_NPC(ch)){send_to_char("{cYou don't have a bank account.{x\n\r",ch);return;}

    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {send_to_char("{cYou are not in a bank.{x\n\r",ch);return;}

    bank = kingdom_table[ch->kingdom].gold;

    if ( bank == 0 )
    {send_to_char( "{cYour kingdom does not have any money in the bank.{x\n\r",ch);return;}

    sprintf( buf, "{CYour kingdom has %ld gold coins in the bank.{x\n\r",bank);

    send_to_char(buf, ch);
    return;
    } /*balance*/

    else if (!strcmp(arg1, "deposit"))
    {    
    long amount;
    char buf[100];

    if (ch->kingdom==0){ 
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
  
    if (IS_NPC(ch)){send_to_char("{cYou don't have a bank account.{x\n\r",ch);return;}

    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK))
    {send_to_char("{cThe ant by your feet can not carry your gold.{x\r",ch);return;}

    if ( arg2[0] == '\0' )  {send_to_char( "{cDeposit how much?{x\n\r", ch);return;}

    amount = labs (atol(arg2));

    if (amount > ch->gold){send_to_char("{cThat's more than you've got.{x\n\r",ch);return;}
 
    kingdom_table[ch->kingdom].gold += amount;
    ch->gold -= amount;

    sprintf(buf, "{g%ld gold coins deposited. Come again soon!{x\n\r",amount);
    send_to_char(buf, ch);
    act("{g$n steps up to the teller window.{x",ch,NULL,NULL,TO_ROOM);
 
    return;
    } /*deposit*/

    else if (!strcmp(arg1, "withdraw"))
    {    
    long amount;
    char buf[100];

    if (ch->kingdom==0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
    if (!IS_SET(ch->kincom,KINGDOM_C_WITHDRAW)) {
	send_to_char("{cYou can not do that.{x\n\r",ch);return;}

    if (IS_NPC(ch)) {send_to_char("{cYou don't have a bank account.{x\n\r",ch);return;}

    if (!IS_SET(ch->in_room->room_flags, ROOM_BANK)) {
      send_to_char("{cThe mosquito by your feet will not give you any money.{x\n\r",ch);return;}

    if ( arg2[0] == '\0' )  {send_to_char( "{cWithdraw how much?{x\n\r",ch);return;}

    amount = labs (atol(arg2));

    if ( amount > kingdom_table[ch->kingdom].gold) {
      send_to_char("{cYour kingdom does not have that much.{x\n\r",ch);
      return;}

    kingdom_table[ch->kingdom].gold -= amount;

    sprintf(buf,"{gHere are your %ld gold coins.{x\n\r",amount);
    send_to_char(buf, ch);
    ch->gold += amount;
    act("{g$n steps up to the teller window.{x",ch,NULL,NULL,TO_ROOM);
    return;
    } /*withdraw*/

    else if (!strcmp(arg1, "desecrate"))
    {    
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char desecrate_buf[MAX_STRING_LENGTH];
    AREA_DATA *pArea=ch->in_room->area;

    pArea=ch->in_room->area;
    if(!IS_SET(ch->kincom,KINGDOM_C_DE)) {
	send_to_char("{cYou can not do that.{x\n\r",ch);return;}
    if (ch->kingdom==0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
    if (pArea->worth==0){send_to_char("{cThis place will never have another owner.{x\n\r",ch);return;}
    if (is_full_name(pArea->owner,"NONE")){send_to_char("{cNo one controls this area.{x\n\r",ch);return;}
	     if (!strcmp(pArea->owner, kingdom_table[1].short_name)){enemy=1;}
	else if (!strcmp(pArea->owner, kingdom_table[2].short_name)){enemy=2;}
	else if (!strcmp(pArea->owner, kingdom_table[3].short_name)){enemy=3;}
	else if (!strcmp(pArea->owner, kingdom_table[4].short_name)){enemy=4;}
	else if (!strcmp(pArea->owner, kingdom_table[5].short_name)){enemy=5;}
	else if (!strcmp(pArea->owner, kingdom_table[6].short_name)){enemy=6;}
	else {send_to_char("{cNo kingdom owns this area.{x\n\r",ch);return;}
    if (!(warring(ch->kingdom,enemy))) { 
    send_to_char("{cYou are not at war with the owner.{x\n\r",ch);return;}
    if (!(with_symbol(ch))){send_to_char("{cFind the enemy symbol.{x\n\r",ch);return;}
    if (!(check_desecration(ch))) return;
    WAIT_STATE(ch,7);
/*go through all objects in room, removes #1 reset & purge all enemy symbols*/ 
    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next ) {
       obj_next = obj->next_content;
       if (obj->pIndexData->vnum == (enemy+190)) /* if is enemy symbol */
	  { remove_symbol(ch);
	extract_obj(obj);break; }   
    }   
	    
    sprintf(desecrate_buf,"{C%s has been routed from %s!{x",
	capitalize(kingdom_table[enemy].short_name), 
	ch->in_room->area->name);
	do_anon_shout(ch,desecrate_buf);

    free_string(ch->in_room->area->owner);
    ch->in_room->area->owner = strdup( "NONE" );
    save_area(ch->in_room->area);
    return;
    } /* desecrate */

    else if (!strcmp(arg1, "consecrate"))
    {    
    OBJ_DATA *obj;
    char consecrate_buf[MAX_STRING_LENGTH];
    AREA_DATA *pArea=ch->in_room->area;

    if (ch->kingdom==0) {
	send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
    if(!IS_SET(ch->kincom,KINGDOM_C_CON)) {
    send_to_char("{cYou can not do that.{x\n\r",ch);return;}

    if (pArea->worth==0){send_to_char("{cThis place will never have another owner.{x\n\r",ch);return;}
    if (!is_full_name(pArea->owner,"NONE")){send_to_char("{cSomeone already controls this area.{x\n\r",ch);return;}
    
    
    if (!(check_consecration(ch))) return;
    WAIT_STATE(ch,7);

   if (!str_cmp (ch->name, "Paladar"))
    obj       = create_object( get_obj_index(3+190), 60 );           
   else 
    obj       = create_object( get_obj_index(ch->kingdom+190), 60 );

    obj_to_room(obj,ch->in_room);
    place_symbol(ch); /* add symbol to resets */
    
   if (!str_cmp (ch->name, "Paladar"))
    {
    sprintf(consecrate_buf,"{C%s has been taken over by %s!{x",
 	ch->in_room->area->name,
     capitalize(kingdom_table[3].short_name));
     do_anon_shout(ch,consecrate_buf);
    }
   else	
   {
    sprintf(consecrate_buf,"{C%s has been taken over by %s!{x",
 	ch->in_room->area->name,


     capitalize(kingdom_table[ch->kingdom].short_name));
	do_anon_shout(ch,consecrate_buf);
   }
    free_string(ch->in_room->area->owner);
       if (!str_cmp (ch->name, "Paladar"))
       ch->in_room->area->owner = strdup(kingdom_table[3].short_name);
       else
    ch->in_room->area->owner = strdup(kingdom_table[ch->kingdom].short_name);
    save_area(ch->in_room->area);
    return;
    } /* consecrate */

    else if (!strcmp(arg1, "guardian"))
    {    
    AREA_DATA *pArea=ch->in_room->area;
    int number,max_room,max_area;
    int original=number_of_guardians(ch);
    char buf[MAX_STRING_LENGTH];   
    buf[0] = '\0';
    number=atoi(arg2);

    if (number < 1)
	{
	send_to_char("{cYou can not do that.{x\n\r",ch);
	send_to_char("You must have a valid integer more than 1.\n\r",ch);
	return;
	}
    if (ch->kingdom==0){send_to_char("{cYou are not in a kingdom.{x\n\r",ch);return;}
	if(!IS_SET(ch->kincom,KINGDOM_C_GUARD)) {
	send_to_char("{cYou can not do that.{x\n\r",ch);return;}
    if (with_symbol(ch)){send_to_char("{cGuardians can not be placed so close to kingdom symbols.{x\n\r",ch);return;}
    if (pArea->worth==0){
	send_to_char("{cThis place is not allow to be taken over.{x\n\r",ch);return;}
    if (!is_full_name(pArea->owner,kingdom_table[ch->kingdom].short_name))
	{send_to_char("{cYou do not control this area.{x\n\r",ch);return;}
    if (number <= original)
	{send_to_char("{cYou already have that many here.{x\n\r",ch);return;}
    if ( kingdom_table[ch->kingdom].gold < (5000*(number-original)) )
	{send_to_char("{cYour kingdom can not afford that right now.{x\n\r",ch);return;}


max_room = URANGE(3,ch->in_room->area->worth/10,10);
if (number > max_room)
{
sprintf(buf,"{cThis area only allows for a maximum of %d guardians per room.{x",max_room);
send_to_char(buf,ch);
return;
}

max_area = (ch->in_room->area->worth/5+5);
if (number+(number_of_guardians_area(ch))-original > max_area)
{
sprintf(buf,"{cThis area only allows for a maximum of %d guardians.{x",max_area);
send_to_char(buf,ch);
return;
}

if (number > 0)
    sprintf(buf,"{gYou relay the order for %d guardians to defend this room...\n\rIt will take some time before they all show up.\n\r{x",number); 
else
    sprintf(buf,"{gYou relay the order for a single guardian to defend this room...\n\rIt will take some time...\n\r{x");
    send_to_char(buf,ch);
    kingdom_table[ch->kingdom].gold -= (5000*(number-original));
    remove_guardians(ch);
    place_guardians(ch,number);
    save_area(pArea);
    WAIT_STATE(ch,20);
    return;
    } /* guardian */
  
   else /* default, error */
   {
   send_to_char("{CKINGDOM commands: INFO BALANCE DEPOSIT WITHDRAW AREAS MEMBERS{x\n\r",ch);
   send_to_char("{C               INDUCT BANISH PROMOTE DEMOTE WAR PEACE{x\n\r",ch);
   send_to_char("{C               RESIGN DESECRATE CONSECRATE GUARDIAN PROSPECTS{x\n\r",ch);
   return;
   }

return;
}

void    fwrite_kingdom    args( ( int kingdom, FILE *fp) );
void    fread_kingdom     args( ( int kingdom, FILE *fp) );

void do_save_clans(CHAR_DATA *ch, char *argument)
{
FILE *fp;
int i;
char *filename[MAX_KINGDOM]= {
"../clans/NONE",
"../clans/GONDOR", 
"../clans/ALDON", 
"../clans/MORDUKHAN",
"../clans/DRAKKON", 
"../clans/NARSSIUS", 
"../clans/ARKADIA",
"../clans/TALASTIN" };

fclose(fpReserve);

for( i=1 ; i < MAX_KINGDOM  ; i++ ) {
	fp = fopen ( filename[i] , "w");
	fwrite_kingdom(i,fp);
	fclose(fp);
}

fpReserve = fopen( NULL_FILE, "r" );

return;
}

void load_clans()
{
FILE *fp;
int i;
char buf[100];
char *filename[MAX_KINGDOM]= {
"../clans/NONE",
"../clans/GONDOR",
"../clans/ALDON",
"../clans/MORDUKHAN",
"../clans/DRAKKON",
"../clans/NARSSIUS",
"../clans/ARKADIA",
"../clans/TALASTIN" };

fclose(fpReserve);

for( i=1 ; i < MAX_KINGDOM  ; i++ ) {
	fp = fopen ( filename[i], "r");
	sprintf(buf,"           %s",filename[i]);
        log_string(buf);
	fread_kingdom(i,fp);
	fclose(fp);
}

fpReserve = fopen( NULL_FILE, "r" );

return;
}

void do_kgrant(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
 if (argument[0] == '\0')
    {
	send_to_char("Syntax: kgrant (who) (command)\n\r",ch);
	send_to_char("Commands are:\n\r",ch);
	send_to_char("WAR PEACE WITHDRAW INDUCT BANISH CONSECRATE DESECRATE\n\r",ch);
	send_to_char("GUARDIAN\n\r",ch);
        return;
    }          
argument = one_argument(argument, arg1);
argument = one_argument(argument, arg2);

    if (ch->kingdom==0) {
    send_to_char("{cYou are not in a kingdom.{x\n\r",ch);
        return;   
	}

    if (ch->kingdom_rank <= 8) {
	send_to_char("{cYou are not rank 10 of the kingdom.{x\n\r",ch);
	return;
	}
    if (arg1[0] == '\0')
    {
	send_to_char("Syntax: kgrant (who) (command)\n\r",ch);
	send_to_char("Commands are:\n\r",ch);
	send_to_char("WAR PEACE WITHDRAW INDUCT BANISH CONSECRATE DESECRATE\n\r",ch);
	send_to_char("GUARDIAN\n\r",ch);
        return;
    }               

    if ((victim=get_char_room( ch, arg1))==NULL)              {
        send_to_char( "{cThey are not here.{x\n\r", ch);return;}   


    if (ch->kingdom != victim->kingdom) {
    send_to_char("{cYou are not in the same kingdom.{x\n\r",ch);
	return;
	}

    if(!strcmp(arg2,"induct")) {
	if(IS_SET(victim->kincom,KINGDOM_C_ADD)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_ADD);
    send_to_char("{cThey now don't have the kingdom induct command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom induct command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_ADD);
    send_to_char("{cThey now have the kingdom induct command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom induct command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"banish")) {
	if(IS_SET(victim->kincom,KINGDOM_C_BAN)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_BAN);
    send_to_char("{cThey now don't have the kingdom banish command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom banish command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_BAN);
    send_to_char("{cThey now have the kingdom banish command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom banish command.{x\n\r",victim);
	}
    return;
	}

    if(!strcmp(arg2,"promote")) {
	if(IS_SET(victim->kincom,KINGDOM_C_PROMOTE)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_PROMOTE);
    send_to_char("{cThey now don't have the kingdom promote command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom promote command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_PROMOTE);
    send_to_char("{cThey now have the kingdom promote command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom promote command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"demote")) {
	if(IS_SET(victim->kincom,KINGDOM_C_DEMOTE)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_DEMOTE);
    send_to_char("{cThey now don't have the kingdom demote command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom demote command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_DEMOTE);
    send_to_char("{cThey now have the kingdom demote command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom demote command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"withdraw")) {
	if(IS_SET(victim->kincom,KINGDOM_C_WITHDRAW)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_WITHDRAW);
    send_to_char("{cThey now don't have the kingdom withdraw command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom withdraw command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_WITHDRAW);
    send_to_char("{cThey now have the kingdom withdraw command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom withdraw command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"consecrate")) {
	if(IS_SET(victim->kincom,KINGDOM_C_CON)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_CON);
    send_to_char("{cThey now don't have the kingdom consecrate command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom consecrate command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_CON);
    send_to_char("{cThey now have the kingdom consecrate command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom consecrate command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"desecrate")) {
	if(IS_SET(victim->kincom,KINGDOM_C_DE)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_DE);
    send_to_char("{cThey now don't have the kingdom desecrate command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom desecrate command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_DE);
    send_to_char("{cThey now have the kingdom desecrate command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom desecrate command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"war")) {
	if(IS_SET(victim->kincom,KINGDOM_C_WAR)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_WAR);
    send_to_char("{cThey now don't have the kingdom war command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom war command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_WAR);
    send_to_char("{cThey now have the kingdom war command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom war command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"peace")) {
	if(IS_SET(victim->kincom,KINGDOM_C_PEACE)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_PEACE);
    send_to_char("{cThey now don't have the kingdom peace command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom peace command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_PEACE);
    send_to_char("{cThey now have the kingdom peace command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom peace command.{x\n\r",victim);
	}
    return;
	}
    if(!strcmp(arg2,"guardian")) {
	if(IS_SET(victim->kincom,KINGDOM_C_GUARD)) {
	REMOVE_BIT(victim->kincom,KINGDOM_C_GUARD);
    send_to_char("{cThey now don't have the kingdom guardian command.{x\n\r",ch);
    send_to_char("{cYou have been detrusted the kingdom guardian command.{x\n\r",victim);
	}
	else {
	SET_BIT(victim->kincom,KINGDOM_C_GUARD);
    send_to_char("{cThey now have the kingdom guardian command.{x\n\r",ch);
    send_to_char("{cYou have been entrusted the kingdom guardian command.{x\n\r",victim);
	}
    return;
	}
	send_to_char("Syntax: kgrant (who) (command)\n\r",ch);
	send_to_char("Commands are:\n\r",ch);
	send_to_char("WAR PEACE WITHDRAW INDUCT BANISH CONSECRATE DESECRATE\n\r",ch);
	send_to_char("GUARDIAN\n\r",ch);
	return;
} /* End ggrant */
