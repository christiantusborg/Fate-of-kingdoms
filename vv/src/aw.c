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
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (fate.gator.net 2000)		   *
***************************************************************************/

#include <sys/types.h> 
#include <sys/time.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <time.h>
#include "merc.h"  
#include "recycle.h"  
#include "tables.h"  
#include "lookup.h" 
#include "arena.h"
#include "fok_struct.h"
#include "declaration.h"

/* command procedures needed */
DECLARE_DO_FUN(do_save_clans	);
DECLARE_DO_FUN(do_font		);
DECLARE_DO_FUN(do_rstat		);
DECLARE_DO_FUN(do_mstat		);
DECLARE_DO_FUN(do_ostat		);
DECLARE_DO_FUN(do_rset		);
DECLARE_DO_FUN(do_mset		);
DECLARE_DO_FUN(do_oset		);
DECLARE_DO_FUN(do_sset		);
DECLARE_DO_FUN(do_mfind		);
DECLARE_DO_FUN(do_ofind		);
DECLARE_DO_FUN(do_slookup	);
DECLARE_DO_FUN(do_mload		);
DECLARE_DO_FUN(do_oload		);
DECLARE_DO_FUN(do_force		);
DECLARE_DO_FUN(do_quit		);
DECLARE_DO_FUN(do_save		);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_limited       );
DECLARE_DO_FUN(do_rename        );
DECLARE_DO_FUN(do_asave         );
DECLARE_DO_FUN(do_resets	);
/*
 * Local functions.
 */
ROOM_INDEX_DATA *	find_location	args( ( CHAR_DATA *ch, char *arg ) );
bool write_to_descriptor  args(( int desc, char *txt, int length ));
int     execl                   args( ( const char *path, const char *arg, ... ) );
int     close                   args( ( int fd ) );
void    logf            args( ( char * fmt, ...));         

bool check_parse_name (char* name);  /* comm.c */
char *initial( const char *str );    /* comm.c */

void do_resetarena(CHAR_DATA *ch, char *argument)
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

 argument = one_argument(argument,arg);
 
 if ((arg[0] == '\0') || (!is_number(arg)))
    {
	send_to_char("Syntax: ResetArena (# of wait time)\n\r",ch);
      return;
    }
  sprintf(buf, "{R[{YARENA{R] {c%s {Rreseted the arena.{w", ch->name);
  info(buf,NULL,NULL,INFO_ARENA,0,0);
    wartype = 0;
    min_level = 0;
    max_level = 0;
    iswar = FALSE;
    inmiddle = FALSE;
    inwar = 0;
    wartimer = 0;
    wartimeleft = 0;
    arena_counter = atoi(arg);
    sprintf(buf,"Arena will be able start in %i ticks.\n\r",arena_counter);
    send_to_char(buf,ch);
}

void do_guild(CHAR_DATA *ch, char *argument)
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];
   char arg3[MAX_STRING_LENGTH];
   int rank,number,number_of_members;
// int i;
      argument = one_argument( argument, arg1 );
      argument = one_argument( argument, arg2 );
      argument = one_argument( argument, arg3 );

 if ( arg1[0] == '\0' || arg2[0] == '\0'  )
         {
         send_to_char( "Syntax: guild <char> <cln name> <rank #1-10>\n\r",ch);
         return;
         }

         if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
         {
                  send_to_char( "They aren't playing.\n\r", ch );
                  return;
         }

 if (!str_prefix(arg2,"none"))
         {
        send_to_char("They are now kingdomless.\n\r",ch);
        send_to_char("You are now a member of no kingdom!\n\r",victim);
 if (victim->kingdom != 0) {
     if(!IS_IMMORTAL(victim)) {
   subtract_member(victim->name,victim->kingdom);
		}
	}
        victim->kingdom = 0;
        victim->kingdom_rank=0;
	if(!IS_NPC(victim)) {
        free_string(victim->pcdata->pretitle);
        victim->pcdata->pretitle = str_dup("");
        do_save_clans(ch,"");
	}
        return;
         } 
	else if(!str_prefix(arg2,kingdom_table[1].name))  { number=KINGDOM_GONDOR; }
	else if(!str_prefix(arg2,kingdom_table[2].name))    { number=KINGDOM_ALDON; }
	else if(!str_prefix(arg2,kingdom_table[3].name))  { number=KINGDOM_MORDUKHAN; }
        else if(!str_prefix(arg2,kingdom_table[4].name))   {  number=KINGDOM_DRAKKON;}
	else if(!str_prefix(arg2,kingdom_table[5].name))   {number=KINGDOM_5;}
        else if(!str_prefix(arg2,kingdom_table[6].name))    { number=KINGDOM_ARKADIA; }
	else if(!str_prefix(arg2,kingdom_table[7].name)) { number=KINGDOM_XELENIA; }
	else 
	 { 
          send_to_char("You must provide with one of the following kingdom.\n\r",ch);
          sprintf(buf,"{CName: %s, %s, %s, %s, %s, %s, %s{x\n\r",kingdom_table[1].name,
          kingdom_table[2].name,kingdom_table[3].name,kingdom_table[4].name,
          kingdom_table[5].name,kingdom_table[6].name,kingdom_table[7].name);
          send_to_char(buf,ch);
          return;
  	 }

      if(!str_prefix(arg3,"1"))
        rank = 0;
        else if(!str_prefix(arg3,"2"))
        rank = 1;
        else if(!str_prefix(arg3,"3"))
        rank = 2;
        else if(!str_prefix(arg3,"4"))
        rank = 3;
        else if(!str_prefix(arg3,"5"))
        rank = 4;
        else if(!str_prefix(arg3,"6"))
        rank = 5;
        else if(!str_prefix(arg3,"7"))
        rank = 6;
        else if(!str_prefix(arg3,"8"))
        rank = 7;
        else if(!str_prefix(arg3,"9"))
        rank = 8;
	else if(!str_prefix(arg3,"10"))
        rank = 9;
        else {
        send_to_char("No such rank exists in that kingdom.\n\r",ch);
        return;
        }
{
   number_of_members = 0;

/*
        for (i=0 ; i < 10 ; i++ )
        number_of_members += kingdom_table[number].rank_slots[i];
*/
    if (number_of_members >= MAX_KINGDOM_MEMBERS)      {
    send_to_char("{cThe kingdom is full.{x\n\r",ch);return;}

    if (victim->kingdom != 0) {
  if(!IS_IMMORTAL(victim)) {
     subtract_member(victim->name,victim->kingdom);
	}
        }
      victim->kingdom = number;
      victim->kingdom_rank = rank;

      if(!IS_IMMORTAL(victim)) {
      add_member(victim->name,victim->kingdom);
	}

      do_save_clans(ch,"");

     if(victim->sex == SEX_FEMALE)
      sprintf(buf,"They are now a %s of Kingdom %s.\n\r",
      kingdom_table[victim->kingdom].rank_titlesf[victim->kingdom_rank],
                 capitalize(kingdom_table[victim->kingdom].short_name));
	else
      sprintf(buf,"They are now a %s of Kingdom %s.\n\r",
      kingdom_table[victim->kingdom].rank_titles[victim->kingdom_rank],
                 capitalize(kingdom_table[victim->kingdom].short_name));
       send_to_char(buf,ch);

	if(victim->sex == SEX_FEMALE)
        sprintf(buf,"You are now a %s of Kingdom %s.\n\r",
      kingdom_table[victim->kingdom].rank_titlesf[victim->kingdom_rank],
                 capitalize(kingdom_table[victim->kingdom].short_name));
	else
        sprintf(buf,"You are now a %s of Kingdom %s.\n\r",
      kingdom_table[victim->kingdom].rank_titles[victim->kingdom_rank],
                 capitalize(kingdom_table[victim->kingdom].short_name));
        send_to_char(buf,victim);
}

if(!IS_NPC(victim)) {
/*
  for (sn = 0; sn < MAX_SKILL; sn++) {
      if ( (victim->kingdom > 0) &&
        )
        victim->pcdata->learned[sn] = 50;
      else if ( skill_table[sn].kingdom != KINGDOM_NONE &&
               victim->kingdom != skill_table[sn].kingdom )
        victim->pcdata->learned[sn] = 0;
    }
*/
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
}
   return;
}

void do_rename (CHAR_DATA* ch, char* argument)
{
        char old_name[MAX_INPUT_LENGTH],
             new_name[MAX_INPUT_LENGTH],
             strsave [MAX_INPUT_LENGTH];

        CHAR_DATA* victim;
        FILE* file;


       argument = one_argument(argument, old_name); 
/* find new/old name */
       one_argument (argument, new_name);

       /* Trivial checks */
       if (!old_name[0])
       {
               send_to_char ("Rename who?\n\r",ch);

                return;
        }

        victim = get_char_world (ch, old_name);

        if (!victim)
        {
                send_to_char ("There is no such a person online.\n\r",ch);
                return;
        }

       if (IS_NPC(victim))
       {
               send_to_char ("You cannot use Rename on NPCs.\n\r",ch);
               return;
       }

       /* allow rename self new_name,but otherwise only lower level */
       if ( (victim != ch) && (get_trust (victim) >= get_trust (ch)) )
       {
                send_to_char ("You failed.\n\r",ch);
                return;
        }

        if (!victim->desc || (victim->desc->connected != CON_PLAYING) )
        {
send_to_char (
"This player has lost his link or is inside a pager or the like.\n\r",ch);
                return;
        }


      if (!new_name[0])
      {
              send_to_char ("Rename to what new name?\n\r",ch);
              return;
      }

      /* Insert check for clan here!! */
        if (victim->kingdom)
        {
send_to_char ("This player is member of a clan, remove him from there first.\n\r",ch);
                return;
        }

        if (!check_parse_name(new_name))
        {
                send_to_char ("The new name is illegal.\n\r",ch);
                return;
        }

        /* First, check if there is a player named that off-line */
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( new_name ) );

        fclose (fpReserve); /* close the reserve file */
        file = fopen (strsave, "r"); /* attempt to to open pfile */
        if (file)
        {
send_to_char ("A player with that may already exist!.\n\r",ch);
send_to_char ("***READ HELP RENAME***.\n\r",ch);
fclose (file);
fpReserve = fopen( NULL_FILE, "r" ); /* is this really necessary these days? */
                return;

        }
        fpReserve = fopen( NULL_FILE, "r" );  /* reopen the extra file */

        /* Check .gz file ! */
    sprintf( strsave, "%s%s.gz", PLAYER_DIR, capitalize( new_name ) );

        fclose (fpReserve); /* close the reserve file */
        file = fopen (strsave, "r"); /* attempt to to open pfile */
        if (file)
        {
send_to_char ("A player with that may already exist!.\n\r",ch);
send_to_char ("***READ HELP RENAME***.\n\r",ch);

                fclose (file);
        fpReserve = fopen( NULL_FILE, "r" );
                return;

        }
        fpReserve = fopen( NULL_FILE, "r" );  /* reopen the extra file */

        if (get_char_world(ch,new_name))
/* check for playing level-1 non-saved */
        {
send_to_char ("A player with the name you specified already exists!\n\r",ch);
                return;
        }
/* save */

    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );


        /* Rename the character and save him to a new file */
        /* NOTE: Players who are level 1 do NOT get saved under a new name*/

        free_string (victim->name);
        victim->name = str_dup (capitalize(new_name));

        save_char_obj (victim);

        /* unlink the old file */
        unlink (strsave); /* unlink does return a value.. but we do not care */

        /* That's it! */

        send_to_char ("Character renamed.\n\r",ch);

        victim->position = POS_STANDING; /* I am laaazy */
        act ("{W$n has given you a new identity... $N!{x",ch,NULL,victim,TO_VICT);

}


void do_kingdom_scan( CHAR_DATA *ch, char *argument )
{
int i;
char buf[160];

  for(i=1;i<MAX_KINGDOM;i++)
  {
    sprintf(buf, "%s[room %d]> item %d: %s ", 
	kingdom_table[i].short_name, 
        kingdom_table[i].room_vnum, 
        kingdom_table[i].obj_vnum,	
kingdom_table[i].obj_ptr!=NULL?kingdom_table[i].obj_ptr->short_descr:"(NULL)");
    send_to_char( buf, ch );
    if ( kingdom_table[i].obj_ptr!=NULL )
    {
	sprintf(buf, "in_room: (%d)",
	kingdom_table[i].obj_ptr->in_room!=NULL?
 	kingdom_table[i].obj_ptr->in_room->vnum:0);
    }
    strcat( buf, "\n\r");
    send_to_char( buf, ch );
  } 
  return;
}

void do_affections( CHAR_DATA *ch, char *argument )
{
FILE *fp;
OBJ_DATA *obj;
AFFECT_DATA *paf;

   if ( (fp=fopen( "AFFECTIONS.TXT", "w" ) ) == NULL )
   {
        send_to_char( "File error.\n\r", ch );
        return;
   }

   for( obj=object_list; obj!=NULL; obj = obj->next )
   {
     if ( obj->pIndexData->affected != NULL )
     {
       fprintf( fp, "Obj: %s\n", obj->short_descr );
       for( paf=obj->pIndexData->affected; paf != NULL; paf = paf->next )
       {
            if ( paf == NULL ) continue;
            fprintf( fp, "  Affects %s by %d.\n",
                affect_loc_name( paf->location ), paf->modifier );
            if (paf->bitvector)
            {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                        fprintf(fp,"   Adds %s affect.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                        fprintf(fp,"   Adds %s object flag.\n",
                            extra_bit_name(paf->bitvector));
                        break;
                    case TO_IMMUNE:
                        fprintf(fp,"   Adds immunity to %s.\n",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_RESIST:
                        fprintf(fp,"   Adds resistance to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_VULN:
                        fprintf(fp,"   Adds vulnerability to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    default:
                        fprintf(fp,"   Unknown bit %d: %d\n\r",
                            paf->where,paf->bitvector);
                        break;
                }
            }
       }
     }
   }
   fclose( fp );
   return;
}

void do_makelist( CHAR_DATA *ch, char *argument )
{
    return;
/* Baxter
FILE *fp;
int count;
char buf[160];

  fp = fopen( "skill_list", "w" );
  if ( fp == NULL )
  {
    send_to_char( "Can't open file..\n\r", ch );
    return;
  }
  fprintf( fp, "{\n" );

  for( count=0; count < MAX_SKILL; count++ )
  {
	sprintf(buf, "\t{  \"%s\",\t\t%4d\t}%s\n", 
	  skill_table[count].name, skill_table[count].slot,
	  count+1==MAX_SKILL?"":",");

	fprintf( fp, buf );
	send_to_char( buf, ch );
	send_to_char( "\r", ch );
  }
  fprintf( fp, "}\n" );
  fclose(fp);
  return;
*/
}


void do_limited( CHAR_DATA *ch, char *argument )
{

  extern int top_obj_index;
  OBJ_DATA *obj;
  OBJ_INDEX_DATA *obj_index;
  int	lCount = 0;
  int	ingameCount;
  char  buf[2*MAX_STRING_LENGTH];
  char  buf2[2*MAX_STRING_LENGTH];
  BUFFER *pbuf;
  int 	nMatch;
  int 	vnum;

  if ( argument[0] != '\0' )  {
    obj_index = get_obj_index( atoi(argument) );
    if ( obj_index == NULL )  {
      send_to_char( "Not found.\n\r", ch);
      return;
    }
    if ( obj_index->limit == -1 )  {
      send_to_char( "Thats not a limited item.\n\r", ch );
      return;
    }
    nMatch = 0;
          sprintf( buf, "%-35s [%5d]  Limit: %3d  Current: %3d\n\r", 
		   obj_index->short_descr, 
		   obj_index->vnum,
	           obj_index->limit, 
		   obj_index->count );
	  buf[0] = UPPER( buf[0] );
	  send_to_char( buf, ch );
	  ingameCount = 0;

	  for ( obj=object_list; obj != NULL; obj=obj->next )
	    if ( obj->pIndexData->vnum == obj_index->vnum )  {
	      ingameCount++;
	      if ( obj->carried_by != NULL ) 
		if(can_see(ch,obj->carried_by))
		sprintf(buf, "Carried by %-30s\n\r", obj->carried_by->name);
	      if ( obj->in_room != NULL ) 
		sprintf(buf, "At %-20s [%d]\n\r", obj->in_room->name, obj->in_room->vnum);
	      if ( obj->in_obj != NULL ) 
		sprintf(buf, "In %-20s [%d] \n\r", obj->in_obj->short_descr, obj->in_obj->pIndexData->vnum);
	        send_to_char( buf, ch );

	    }

	    sprintf(buf, "  %d found in game. %d should be in pFiles.\n\r", 
			ingameCount, obj_index->count-ingameCount);
    send_to_char( buf, ch ); 
  return;
  }
 
  nMatch = 0;
  pbuf = new_buf();
  for ( vnum = 0; nMatch < top_obj_index; vnum++ )
      if ( ( obj_index = get_obj_index( vnum ) ) != NULL )
      {
        nMatch++;
	if ( obj_index->limit != -1 )  {
	  lCount++;
          sprintf( buf, "%-37s [%5d]  Limit: %3d  Current: %3d\n\r", 
		   obj_index->short_descr, 
		   obj_index->vnum,
	           obj_index->limit, 
		   obj_index->count );
	  buf[0] = UPPER( buf[0] );
	  add_buf(pbuf,buf);
	}
      }
  sprintf( buf2, "\n\r%d of %d objects are limited.\n\r", lCount, nMatch );
  add_buf(pbuf,buf2);
  page_to_char(buf_string(pbuf), ch);
  free_buf(pbuf);
  return;

}
/*
void do_wiznet( CHAR_DATA *ch, char *argument )
{
    int flag;
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] == '\0' )
    {
      	if (IS_SET(ch->wiznet,WIZ_ON))
      	{
            send_to_char("Signing off of Wiznet.\n\r",ch);
            REMOVE_BIT(ch->wiznet,WIZ_ON);
      	}
      	else
      	{
            send_to_char("Welcome to Wiznet!\n\r",ch);
            SET_BIT(ch->wiznet,WIZ_ON);
      	}
      	return;
    }

    if (!str_prefix(argument,"on"))
    {
	send_to_char("Welcome to Wiznet!\n\r",ch);
	SET_BIT(ch->wiznet,WIZ_ON);
	return;
    }

    if (!str_prefix(argument,"off"))
    {
	send_to_char("Signing off of Wiznet.\n\r",ch);
	REMOVE_BIT(ch->wiznet,WIZ_ON);
	return;
    }

   //show wiznet status 
    if (!str_prefix(argument,"status")) 
    {
	buf[0] = '\0';

	if (!IS_SET(ch->wiznet,WIZ_ON))
	    strcat(buf,"off ");

	for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	    if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
	    {
		strcat(buf,wiznet_table[flag].name);
		strcat(buf," ");
	    }

	strcat(buf,"\n\r");

	send_to_char("Wiznet status:\n\r",ch);
	send_to_char(buf,ch);
	return;
    }

    if (!str_prefix(argument,"show"))
    // list of all wiznet options 
    {
	buf[0] = '\0';

	for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	{
	    if (wiznet_table[flag].level <= get_trust(ch))
	    {
	    	strcat(buf,wiznet_table[flag].name);
	    	strcat(buf," ");
	    }
	}

	strcat(buf,"\n\r");

	send_to_char("Wiznet options available to you are:\n\r",ch);
	send_to_char(buf,ch);
	return;
    }
   
    flag = wiznet_lookup(argument);

    if (flag == -1 || get_trust(ch) < wiznet_table[flag].level)
    {
	send_to_char("No such option.\n\r",ch);
	return;
    }
   
    if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
    {
	sprintf(buf,"You will no longer see %s on wiznet.\n\r",
	        wiznet_table[flag].name);
	send_to_char(buf,ch);
	REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
    	return;
    }
    else
    {
    	sprintf(buf,"You will now see %s on wiznet.\n\r",
		wiznet_table[flag].name);
	send_to_char(buf,ch);
    	SET_BIT(ch->wiznet,wiznet_table[flag].flag);
	return;
    }

}*/

void do_wiznet( CHAR_DATA *ch, char *argument )
{
   int flag;
   char buf[MAX_STRING_LENGTH];
   char flagz[MAX_STRING_LENGTH];
   int flagw;
                
   if ( argument[0] == '\0' )
/* Show wiznet options - just like channel command */
   {
        if (!IS_SET(ch->wiznet,WIZ_ON))
        {
        send_to_char("Wiznet is currently off.\n\r",ch);
        send_to_char("Type wiznet 'on' to turn wiznet on.\n\r",ch);
        return;
        }
        else
        {
        send_to_char("Wiznet is currently on.\n\r", ch);
        send_to_char("   Option      Status\n\r",ch);
        send_to_char("---------------------{b\n\r",ch);
        }
        /* list of all wiznet options */
        buf[0] = '\0';

        flagw = 0;        
        for (flag = 0; wiznet_table[flag].name != NULL; flag++)
        {
            if (wiznet_table[flag].level <= get_trust(ch))
            {
                if (flagw == 6)
                {
                send_to_char("\n\r",ch);
                flagw = 0;
                } 
                flagw += 1;

                if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
                sprintf(flagz, "{G%-10s{x ",wiznet_table[flag].name);
                else
                sprintf(flagz, "{g%-10s{x ",wiznet_table[flag].name);
                send_to_char(flagz, ch);   
            }
        }
/* To avoid color bleeding */
     send_to_char("{x\n\r",ch);
     return;
   }    
 
   if (!str_prefix(argument,"on"))
   {     
        send_to_char("{BWelcome to Wiznet!{x\n\r",ch);
        SET_BIT(ch->wiznet,WIZ_ON);
        return;
   }
                
   if (!str_prefix(argument,"off"))
   {
        send_to_char("{BSigning off of Wiznet.{x\n\r",ch);
        REMOVE_BIT(ch->wiznet,WIZ_ON);
        return;
   }
   
       if (!str_prefix(argument,"all"))
    {
        if (!IS_SET(ch->wiznet,WIZ_ON))
        {
        SET_BIT(ch->wiznet,WIZ_ON);
        }
        buf[0] = '\0';
        for (flag = 0; wiznet_table[flag].name != NULL; flag++)
	{
	    if (wiznet_table[flag].level <= get_trust(ch))
	    {
             sprintf(buf,"You will now see %s on wiznet.\n\r",
             wiznet_table[flag].name);
             send_to_char(buf,ch);
             SET_BIT(ch->wiznet,wiznet_table[flag].flag);
            }
        }
	strcat(buf,"\n\r");
        send_to_char("The following options are now enabled\n\r", ch);
             return;
    }
        
   flag = wiznet_lookup(argument);
        
   if (flag == -1 || get_trust(ch) < wiznet_table[flag].level) 
   {
        send_to_char("{MNo such option.{x\n\r",ch);
        return;
   }

   if (IS_SET(ch->wiznet,wiznet_table[flag].flag))
   {
        sprintf(buf,"{GYou will no longer see %s on wiznet.{x\n\r",
                wiznet_table[flag].name);
        send_to_char(buf,ch);
        REMOVE_BIT(ch->wiznet,wiznet_table[flag].flag);
        return;
   }
   else  
   {
        sprintf(buf,"{GYou will now see %s on wiznet.{x\n\r",
                wiznet_table[flag].name);
        send_to_char(buf,ch);
        SET_BIT(ch->wiznet,wiznet_table[flag].flag);
        return;
   }
}/*
void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj,
	    long flag, long flag_skip, int min_level) 
{
    DESCRIPTOR_DATA *d;

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->connected == CON_PLAYING
	&&  IS_IMMORTAL(d->character) 
	&&  IS_SET(d->character->wiznet,WIZ_ON) 
	&&  (!flag || IS_SET(d->character->wiznet,flag))
	&&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
	&&  get_trust(d->character) >= min_level
	&&  d->character != ch)
        {
	    if (IS_SET(d->character->wiznet,WIZ_PREFIX))
	  	send_to_char("--> ",d->character);
            act_new(string,d->character,obj,ch,TO_CHAR,POS_DEAD);
        }
    }
 
    return;
}
*/

void wiznet(char *string, CHAR_DATA *ch, OBJ_DATA *obj,
	    long flag, long flag_skip, int min_level) 
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int cflag;
    
/*    cflag = flag;*/

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if (d->connected == CON_PLAYING
	&&  IS_IMMORTAL(d->character) 
	&&  IS_SET(d->character->wiznet,WIZ_ON) 
	&&  (!flag || IS_SET(d->character->wiznet,flag))
	&&  (!flag_skip || !IS_SET(d->character->wiznet,flag_skip))
	&&  get_trust(d->character) >= min_level
	&&  d->character != ch)
        {
           /* if (IS_SET(d->character->wiznet,WIZ_PREFIX))
                send_to_char("{B[WIZNET]{x{R ",d->character);
            else*/
/*                send_to_char( "{Z", d->character );*/
          
          for (cflag = 0; wiznet_table[cflag].name != NULL; cflag++)  
	   {
	    if (wiznet_table[cflag].flag == flag)
	    {
            sprintf(buf,"%s{x",wiznet_table[cflag].control);
            send_to_char(buf, d->character );
            send_to_char( "{C", d->character );
            act_new(string,d->character,obj,ch,TO_CHAR,POS_DEAD);
	    send_to_char( "{x", d->character );
	    }
	   }
        }
    }
 
    return;
}
void do_tick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
 
    one_argument( argument, arg );
    if ( arg[0] == '\0' )  {
	send_to_char("tick <area|player|room>\n\r", ch );
	return;
    }
    if (!strcmp(arg, "area") )  {
	area_update( );
	send_to_char("Area updated.\n\r", ch );
	return;
    }
    if (!strcmp(arg, "player") )  {
	char_update( );
	send_to_char("Players updated.\n\r", ch );
	return;
    }
	if (!strcmp(arg, "room") )  {
	room_update( );
	send_to_char("Room updated.\n\r", ch );
	return;
    }
    send_to_char("tick <area|player|room>\n\r", ch );
    return;
}

/* equips a character */
void do_outfit ( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int sn,vnum;

    if ((ch->level > 5 || IS_NPC(ch)) && !IS_IMMORTAL(ch))
    {
	send_to_char("Find it yourself!\n\r",ch);
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) == NULL )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_BANNER), 0 );
	obj->cost = 0;
        obj_to_char( obj, ch );
/*        equip_char( ch, obj, WEAR_LIGHT ); */
/*  :: light goes in inventory */
    }
 
    if ( ( obj = get_eq_char( ch, WEAR_BODY ) ) == NULL )
    {
	obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_VEST), 0 );
	obj->cost = 0;
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_BODY );
    }


 
    /* do the weapon thing */
if(ch->class != CLASS_MNK)
    if ((obj = get_eq_char(ch,WEAR_WIELD)) == NULL)
    {
    	sn = 0; 
    	vnum = OBJ_VNUM_SCHOOL_SWORD; /* just in case! */
        vnum = class_table[ch->class].weapon;

/*    	for (i = 0; weapon_table[i].name != NULL; i++)
    	{
	    if (ch->pcdata->learned[sn] < 
		ch->pcdata->learned[*weapon_table[i].gsn])
	    {
	    	sn = *weapon_table[i].gsn;
	    	vnum = weapon_table[i].vnum;
	    }
    	}
*/
    	obj = create_object(get_obj_index(vnum),0);
     	obj_to_char(obj,ch);
    	equip_char(ch,obj,WEAR_WIELD);
    }

    if (((obj = get_eq_char(ch,WEAR_WIELD)) == NULL 
    ||   !IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS)) 
    &&  (obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_SHIELD), 0 );
	obj->cost = 0;
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_SHIELD );
    }

    send_to_char("{MYou have been equipped by the gods.{x\n\r",ch);
}

     
/* RT nochannels command, for those spammers */
/*RE Coded by Salindor, Imp By Kazmir*/

void do_nochannels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
 
    one_argument( argument, arg );
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "Nochannel whom?", ch );
        return;
    }
 
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
 
    if (IS_IMMORTAL(victim))
     if ( get_trust( victim ) >= get_trust( ch ) )
      {
       send_to_char( "You failed.\n\r", ch );
       return;
      }
 
    if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
    {
        REMOVE_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "The gods have restored your channel priviliges.\n\r", 
		      victim );
        send_to_char( "NOCHANNELS removed.\n\r", ch );
	sprintf(buf,"$N restores channels to %s",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
        SET_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "The gods have revoked your channel priviliges.\n\r", 
		       victim );
        send_to_char( "NOCHANNELS set.\n\r", ch );
	sprintf(buf,"$N revokes %s's channels.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
 
    return;
}


/* RT nochannels command, for those spammers */
/*RE Coded by Salindor, Imp By Kazmir*/
void do_notick( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH], arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2);
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Nochannel whom?", ch );
        return;
    }
 

    if ( arg2[0] == '\0' )
     {
      send_to_char( "For how many ticks?", ch );
      return;
     }
/*
    if (atoi(arg2)==0)
	{
		send_to_char( arg2, ch);
		send_to_char( "Punishment must be greator than 0",ch);
		return;
	}
*/
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
 
    if (IS_IMMORTAL(victim))
     if ( get_trust( victim ) >= get_trust( ch ) )
      {
       send_to_char( "You failed.\n\r", ch );
       return;
      }
 

    if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
    {
       if (0 == atoi(arg2))
	{
         REMOVE_BIT(victim->comm, COMM_NOCHANNELS);
         send_to_char( "The gods have restored your channel priviliges.\n\r",victim );
         sprintf(buf,"NOCHANNELS you restored %s channel priviliges.\n\r", victim->name);
         send_to_char(buf, ch );
         sprintf(buf,"NOCHANNELS removed by %s on %s\n\r",ch->name, victim->name);
         victim->nochanned =0;
         wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
	} 
       else
	{
	 victim->nochanned=victim->nochanned+atoi(arg2);
	 sprintf(buf,"You increased %s NOCHANNELS time to %d\n\r", victim->name,victim->nochanned);
	 send_to_char(buf, ch );
	 sprintf(buf,"Your NOCHANNELS time increased to %d\n\r",victim->nochanned);
	 send_to_char( buf, victim );
	 sprintf(buf,"%s increased NOCHANNELS time to %d on %s\n\r",ch->name,victim->nochanned, victim->name);
	 wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
	}
    }
    else
    {
	victim->nochanned=atoi(arg2);
        SET_BIT(victim->comm, COMM_NOCHANNELS);
        sprintf(buf,"The gods have revoked your channel priviliges, for %d tick.\n\r",victim->nochanned);
	send_to_char( buf, victim );
	sprintf(buf,"NOCHANNELS Set %s, for %d tick.\n\r", victim->name,victim->nochanned);
	send_to_char(buf, ch );
	sprintf(buf,"%s revokes %s's channels for %d ticks.\n\r",ch->name,victim->name, victim->nochanned);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}


void do_smote(CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char *letter,*name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    int matches = 0;
 
    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "You can't show your emotions.\n\r", ch );
        return;
    }
 
    if ( argument[0] == '\0' )
    {
        send_to_char( "Emote what?\n\r", ch );
        return;
    }
    
    if (strstr(argument,ch->name) == NULL)
    {
	send_to_char("You must include your name in an smote.\n\r",ch);
	return;
    }
   
    send_to_char(argument,ch);
    send_to_char("\n\r",ch);
 
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (vch->desc == NULL || vch == ch)
            continue;
 
        if ((letter = strstr(argument,vch->name)) == NULL)
        {
	    send_to_char(argument,vch);
	    send_to_char("\n\r",vch);
            continue;
        }
 
        strcpy(temp,argument);
        temp[strlen(argument) - strlen(letter)] = '\0';
        last[0] = '\0';
        name = vch->name;
 
        for (; *letter != '\0'; letter++)
        {
            if (*letter == '\'' && matches == strlen(vch->name))
            {
                strcat(temp,"r");
                continue;
            }
 
            if (*letter == 's' && matches == strlen(vch->name))
            {
                matches = 0;
                continue;
            }
 
            if (matches == strlen(vch->name))
            {
                matches = 0;
            }
 
            if (*letter == *name)
            {
                matches++;
                name++;
                if (matches == strlen(vch->name))
                {
                    strcat(temp,"you");
                    last[0] = '\0';
                    name = vch->name;
                    continue;
                }
                strncat(last,letter,1);
                continue;
            }
 
            matches = 0;
            strcat(temp,last);
            strncat(temp,letter,1);
            last[0] = '\0';
            name = vch->name;
        }
 
	send_to_char(temp,vch);
	send_to_char("\n\r",vch);
    }
 
    return;
}

void do_bamfin( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
	smash_tilde( argument );

	if (argument[0] == '\0')
	{
	    sprintf(buf,"Your poofin is %s\n\r",ch->pcdata->bamfin);
	    send_to_char(buf,ch);
	    return;
	}

	if ( strstr(argument,ch->name) == NULL)
	{
	    send_to_char("You must include your name.\n\r",ch);
	    return;
	}
	     
	free_string( ch->pcdata->bamfin );
	ch->pcdata->bamfin = str_dup( argument );

        sprintf(buf,"Your poofin is now %s\n\r",ch->pcdata->bamfin);
        send_to_char(buf,ch);
    }
    return;
}



void do_bamfout( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
 
    if ( !IS_NPC(ch) )
    {
        smash_tilde( argument );
 
        if (argument[0] == '\0')
        {
            sprintf(buf,"Your poofout is %s\n\r",ch->pcdata->bamfout);
            send_to_char(buf,ch);
            return;
        }
 
        if ( strstr(argument,ch->name) == NULL)
        {
            send_to_char("You must include your name.\n\r",ch);
            return;
        }
 
        free_string( ch->pcdata->bamfout );
        ch->pcdata->bamfout = str_dup( argument );
 
        sprintf(buf,"Your poofout is now %s\n\r",ch->pcdata->bamfout);
        send_to_char(buf,ch);
    }
    return;
}



void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->act, PLR_DENY);
    send_to_char( "You are denied access!\n\r", victim );
    sprintf(buf,"$N denies access to %s",victim->name);
    wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    send_to_char( "OK.\n\r", ch );
    save_char_obj(victim);
    stop_fighting(victim,TRUE);
    do_quit( victim, "" );

    return;
}



void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Disconnect whom?\n\r", ch );
	return;
    }

    if (is_number(arg))
    {
	int desc;

	desc = atoi(arg);
    	for ( d = descriptor_list; d != NULL; d = d->next )
    	{
            if ( d->descriptor == desc )
            {
            	close_socket( d );
            	send_to_char( "Ok.\n\r", ch );
            	return;
            }
	}
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "Descriptor not found!\n\r", ch );
    return;
}



void do_pardon( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: pardon <character> <killer|thief>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "killer" ) )
    {
	if ( IS_SET(victim->act, PLR_KILLER) )
	{
	    REMOVE_BIT( victim->act, PLR_KILLER );
	    send_to_char( "Killer flag removed.\n\r", ch );
	    send_to_char( "You are no longer a KILLER.\n\r", victim );
	}
	return;
    }

    if ( !str_cmp( arg2, "thief" ) )
    {
	if ( IS_SET(victim->act, PLR_THIEF) )
	{
	    REMOVE_BIT( victim->act, PLR_THIEF );
	    send_to_char( "Thief flag removed.\n\r", ch );
	    send_to_char( "You are no longer a THIEF.\n\r", victim );
	}
	return;
    }

    send_to_char( "Syntax: pardon <character> <killer|thief>.\n\r", ch );
    return;
}



void do_echo_font( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Global echo (with font) what?\n\r", ch );
	return;
    }
    
    if (strlen(argument) > 30)
    {
	send_to_char( "30 characters of less only please.\n\r", ch );
	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	   do_font(d->character,argument);
    }
    return;
}

void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Global echo what?\n\r", ch );
	return;
    }
    
    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING )
	{
	    if (get_trust(d->character) >= get_trust(ch))
		send_to_char( "{Wglobal> ",d->character);
	    send_to_char( "{c",   d->character );
	    send_to_char( argument, d->character );
	    send_to_char( "{x\n\r",   d->character );
	}
    }

    return;
}



void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Local echo what?\n\r", ch );

	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
            if (get_trust(d->character) >= get_trust(ch))
                send_to_char( "local> ",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_zecho(CHAR_DATA *ch, char *argument)
{
    DESCRIPTOR_DATA *d;

    if (argument[0] == '\0')
    {
	send_to_char("Zone echo what?\n\r",ch);
	return;
    }

    for (d = descriptor_list; d; d = d->next)
    {
	if (d->connected == CON_PLAYING
	&&  d->character->in_room != NULL && ch->in_room != NULL
	&&  d->character->in_room->area == ch->in_room->area)
	{
	    if (get_trust(d->character) >= get_trust(ch))
		send_to_char("zone> ",d->character);
	    send_to_char(argument,d->character);
	    send_to_char("\n\r",d->character);
	}
    }
}

void do_pecho( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument(argument, arg);
 
    if ( argument[0] == '\0' || arg[0] == '\0' )
    {
	send_to_char("Personal echo what?\n\r", ch); 
	return;
    }
   
    if  ( (victim = get_char_world(ch, arg) ) == NULL )
    {
	send_to_char("Target not found.\n\r",ch);
	return;
    }

    if (get_trust(victim) >= get_trust(ch) && get_trust(ch) != MAX_LEVEL)
        send_to_char( "{Wpersonal>{c ",victim);
    else if (!strcmp(victim->name, "Takeda") ) /* if kroudar */
        send_to_char( "{Wpersonal>{c ",victim);

    send_to_char("\n\r{c",victim);
    send_to_char(argument,victim);
    send_to_char("{x\n\r",victim);
    send_to_char( "{Wpersonal>{c ",ch);
    send_to_char(argument,ch);
    send_to_char("{x\n\r",ch);
}


ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL )
	return obj->in_room;

    return NULL;
}



void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Transfer whom (and where)?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

  if ( !str_cmp( arg1, "quest" ) )
    {
        for ( d = descriptor_list; d != NULL; d = d->next )
        {
            if ( d->connected == CON_PLAYING
            &&   d->character != ch
            &&   d->character->in_room != NULL
            &&   can_see( ch, d->character ) 
 	    &&   d->character->pcdata->quest 
            &&   d->character->pcdata->quest->quest_god == ch )
            {
                char buf[MAX_STRING_LENGTH];
                sprintf( buf, "%s %s", d->character->name, arg2 );
                do_transfer( ch, buf );
            }
        }
        return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}

	if ( !is_room_owner(ch,location) && room_is_private( location ) 
	&&  get_trust(ch) < MAX_LEVEL)
	{
	    send_to_char( "That room is private right now.\n\r", ch );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }
    if ((ch->level < victim->level) && !IS_NPC(victim)) 
	{
	send_to_char("You cannot transfer someone from a higher level.\n",ch);
	return;
	}
    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
    act( "{G$n disappears in a mushroom cloud.{x", victim, NULL, NULL,TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, location );
    act( "{G$n arrives from a puff of smoke.{x", victim, NULL, NULL, TO_ROOM);
    if ( ch != victim )
	act( "{G$n has transferred you.{x", ch, NULL, victim, TO_VICT );
    do_look( victim, "auto" );
    send_to_char( "Ok.\n\r", ch );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    OBJ_DATA *on;
    CHAR_DATA *wch;
    
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && room_is_private( location ) 
    &&  get_trust(ch) < MAX_LEVEL)
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    original = ch->in_room;
    on = ch->on;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    ch->on = on;
	    break;
	}
    }

    return;
}



void do_goto( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;
    int count = 0;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    count = 0;
    for ( rch = location->people; rch != NULL; rch = rch->next_in_room )
        count++;

   if ( !IS_BUILDER( ch, location->area ) && (ch->level < 86))
	{
	    send_to_char( "Sorry as builder you can't goto that area.{x\n\r", ch );
	    return;
	}


/*
    if (!is_room_owner(ch,location) && room_is_private(location) 
    &&  (count > 1 || get_trust(ch) < MAX_LEVEL))
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    } */

    if ( ch->fighting != NULL )
	stop_fighting( ch, TRUE );

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
	if (get_trust(rch) >= ch->invis_level)
	{
	    if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
		act("{G$t{x",ch,ch->pcdata->bamfout,rch,TO_VICT);
	    else
		act("{G$n leaves in a swirling mist.{x",ch,NULL,rch,TO_VICT);
	}
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
                act("{G$t{x",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act("{G$n appears in a swirling mist.{x",ch,NULL,rch,TO_VICT);
        }
    }

    do_look( ch, "auto" );
    return;
}

void do_violate( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;
 
    if ( argument[0] == '\0' )
    {
        send_to_char( "Goto where?\n\r", ch );
        return;
    }
 
    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
        send_to_char( "No such location.\n\r", ch );
        return;
    }

    if (!room_is_private( location ))
    {
        send_to_char( "That room isn't private, use goto.\n\r", ch );
        return;
    }
 
    if ( ch->fighting != NULL )
        stop_fighting( ch, TRUE );
 
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
                act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
            else
                act("$n leaves in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }
 
    char_from_room( ch );
    char_to_room( ch, location );
 
 
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (get_trust(rch) >= ch->invis_level)
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0')
                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act("$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }
 
    do_look( ch, "auto" );
    return;
}

/* RT to replace the 3 stat commands */

void do_stat ( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char *string;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *location;
   CHAR_DATA *victim;

   string = one_argument(argument, arg);
   if ( arg[0] == '\0')
   {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  stat <name>\n\r",ch);
	send_to_char("  stat obj <name>\n\r",ch);
	send_to_char("  stat mob <name>\n\r",ch);
 	send_to_char("  stat room <number>\n\r",ch);
	return;
   }

   if (!str_cmp(arg,"room"))
   {
	do_rstat(ch,string);
	return;
   }
  
   if (!str_cmp(arg,"obj"))
   {
	do_ostat(ch,string);
	return;
   }

   if(!str_cmp(arg,"char")  || !str_cmp(arg,"mob"))
   {
	do_mstat(ch,string);
	return;
   }
   
   /* do it the old way */

   obj = get_obj_world(ch,argument);
   if (obj != NULL)
   {
     do_ostat(ch,argument);
     return;
   }

  victim = get_char_world(ch,argument);
  if (victim != NULL)
  {
    do_mstat(ch,argument);
    return;
  }

  location = find_location(ch,argument);
  if (location != NULL)
  {
    do_rstat(ch,argument);
    return;
  }

  send_to_char("Nothing by that name found anywhere.\n\r",ch);
}

   



void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_HISTORY_DATA *rh;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && ch->in_room != location 
    &&  room_is_private( location ) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }
    if (ch->in_room->affected_by)
    {
	sprintf(buf, "Affected by %s\n\r", 
	    raffect_bit_name(ch->in_room->affected_by));
	send_to_char(buf,ch);
    }

    if (ch->in_room->room_flags)
    {
	sprintf(buf, "Roomflags %s\n\r", 
	    flag_room_name(ch->in_room->room_flags));
	send_to_char(buf,ch);
    }

    sprintf( buf, "Name: '%s'\n\rArea: '%s'\n\r",
	location->name,
	location->area->name );
    send_to_char( buf, ch );

    sprintf( buf,
	"Vnum: %d  Sector: %d  Light: %d  Healing: %d  Mana: %d\n\r",
	location->vnum,
	location->sector_type,
	location->light,
	location->heal_rate,
	location->mana_rate );
    send_to_char( buf, ch );

    sprintf( buf,
	"Room flags: %d.\n\rDescription:\n\r%s",
	location->room_flags,
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	if (can_see(ch,rch))
        {
	    send_to_char( " ", ch );
	    one_argument( rch->name, buf );
	    send_to_char( buf, ch );
	}
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"Door: %d.  To: %d.  Key: %d.  Exit flags: %d.\n\rKeyword: '%s'.  Description: %s",

		door,
		(pexit->u1.to_room == NULL ? -1 : pexit->u1.to_room->vnum),
	    	pexit->key,
	    	pexit->exit_info,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "(none).\n\r" );
	    send_to_char( buf, ch );
	}
    }
    send_to_char("Tracks:\n\r",ch);
    for (rh = location->history;rh != NULL;rh = rh->next) {
      sprintf(buf,"%s took door %i.\n\r",rh->name,rh->went);
      send_to_char(buf,ch);
    }
    return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, argument ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }

    sprintf( buf, "Name(s): %s\n\r",
	obj->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %d  Format: %s  Type: %s  Resets: %d\n\r",
	obj->pIndexData->vnum, obj->pIndexData->new_format ? "new" : "old",
	item_type_name(obj), obj->pIndexData->reset_num );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s\n\rLong description: %s\n\r",
	obj->short_descr, obj->description );
    send_to_char( buf, ch );
    sprintf( buf, "Race bits: %s\n\rClass bits: %s\n\r",
	race_bit_name(obj->race_flags), class_bit_name( obj->class_flags ) );
    send_to_char( buf, ch );
    sprintf( buf, "Wear bits: %s\n\rExtra bits: %s\n\r",
	wear_bit_name(obj->wear_flags), extra_bit_name( obj->extra_flags ) );
    send_to_char( buf, ch );

    sprintf( buf, "Number: %d/%d  Weight: %d/%d/%d (10th pounds)\n\r",
	1,           get_obj_number( obj ),
	obj->weight, get_obj_weight( obj ),get_true_weight(obj) );
    send_to_char( buf, ch );

    sprintf( buf, "Level: %d  Cost: %d  Cond: %d Repop: %d  Timer: %d Count: %d\n\r",
	obj->level, obj->cost, obj->condition, obj->pIndexData->repop, obj->timer, obj->pIndexData->count );
    send_to_char( buf, ch );

    sprintf( buf,
	"In room: %d  In object: %s  Carried by: %s  Wear_loc: %d\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" : 
	    can_see(ch,obj->carried_by) ? obj->carried_by->name
				 	: "someone",
	obj->wear_loc );
    send_to_char( buf, ch );
    
    sprintf( buf, "Values: %d %d %d %d %d\n\r",
	obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	obj->value[4] );
    send_to_char( buf, ch );
    
    /* now give out vital statistics as per identify */
    
    switch ( obj->item_type )
    {
    	case ITEM_SCROLL: 
    	case ITEM_POTION:
    	case ITEM_PILL:
	    sprintf( buf, "Level %d spells of:", obj->value[0] );
	    send_to_char( buf, ch );

	    if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[1]].abiliti_name, ch );
	    	send_to_char( "'", ch );
	    }

	    if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[2]].abiliti_name, ch );
	    	send_to_char( "'", ch );
	    }

	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[3]].abiliti_name, ch );
	    	send_to_char( "'", ch );
	    }

	    if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	    {
		send_to_char(" '",ch);
		send_to_char(skill_table[obj->value[4]].abiliti_name,ch);
		send_to_char("'",ch);
	    }

	    send_to_char( ".\n\r", ch );
	break;

    	case ITEM_WAND: 
    	case ITEM_STAFF: 
	    sprintf( buf, "Has %d(%d) charges of level %d",
	    	obj->value[1], obj->value[2], obj->value[0] );
	    send_to_char( buf, ch );
      
	    if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	    {
	    	send_to_char( " '", ch );
	    	send_to_char( skill_table[obj->value[3]].abiliti_name, ch );
	    	send_to_char( "'", ch );
	    }

	    send_to_char( ".\n\r", ch );
	break;

	case ITEM_DRINK_CON:
	    sprintf(buf,"It holds %s-colored %s.\n\r",
		liq_table[obj->value[2]].liq_color,
		liq_table[obj->value[2]].liq_name);
	    send_to_char(buf,ch);
	    break;
		
    	case ITEM_TRAP:
	    sprintf( buf, 
	    "Trap Triggers(%d)     Trap Attack Damage(%d)    Trap Charges(%d/%d)\n\r",
	        obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	    send_to_char( buf, ch );
	break;

      
    	case ITEM_WEAPON:
 	    send_to_char("Weapon type is ",ch);
	    switch (obj->value[0])
	    {
	    	case(WEAPON_EXOTIC): 
		    send_to_char("exotic\n\r",ch);
		    break;
	    	case(WEAPON_SWORD): 
		    send_to_char("sword\n\r",ch);
		    break;	
	    	case(WEAPON_DAGGER): 
		    send_to_char("dagger\n\r",ch);
		    break;
	    	case(WEAPON_SPEAR):
		    send_to_char("spear/staff\n\r",ch);
		    break;
	    	case(WEAPON_MACE): 
		    send_to_char("mace/club\n\r",ch);	
		    break;
	   	case(WEAPON_AXE): 
		    send_to_char("axe\n\r",ch);	
		    break;
	    	case(WEAPON_FLAIL): 
		    send_to_char("flail\n\r",ch);
		    break;
	    	case(WEAPON_WHIP): 
		    send_to_char("whip\n\r",ch);
		    break;
	    	case(WEAPON_POLEARM): 
		    send_to_char("polearm\n\r",ch);
		    break;
		case(WEAPON_BOW):
		    send_to_char("bow\n\r",ch);
		    break;
		case(WEAPON_ARROW):
		    send_to_char("arrow.\n\r",ch);
		    break;
                case(WEAPON_LANCE): 
		    send_to_char("lance.\n\r",ch);
	            break;
	    	default: 
		    send_to_char("unknown\n\r",ch);
		    break;
 	    }
	    if (obj->pIndexData->new_format)
	    	sprintf(buf,"Damage is %dd%d (average %d)\n\r",
		    obj->value[1],obj->value[2],
		    (1 + obj->value[2]) * obj->value[1] / 2);
	    else
	    	sprintf( buf, "Damage is %d to %d (average %d)\n\r",
	    	    obj->value[1], obj->value[2],
	    	    ( obj->value[1] + obj->value[2] ) / 2 );
	    send_to_char( buf, ch );

	    sprintf(buf,"Damage noun is %s.\n\r",
		attack_table[obj->value[3]].noun);
	    send_to_char(buf,ch);
	    
	    if (obj->value[4])  /* weapon flags */
	    {
	        sprintf(buf,"Weapons flags: %s\n\r",
		    weapon_bit_name(obj->value[4]));
	        send_to_char(buf,ch);
            }
	break;

    	case ITEM_ARMOR:
	    sprintf( buf, 
	    "Armor class is %d pierce, %d bash, %d slash, and %d vs. magic\n\r",
	        obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	    send_to_char( buf, ch );
	break;


        case ITEM_CONTAINER:
            sprintf(buf,"Capacity: %d#  Maximum weight: %d#  flags: %s\n\r",
                obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
            send_to_char(buf,ch);
            if (obj->value[4] != 100)
            {
                sprintf(buf,"Weight multiplier: %d%%\n\r",
		    obj->value[4]);
                send_to_char(buf,ch);
            }
        break;
    }


    if ( obj->extra_descr != NULL || obj->pIndexData->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );

	for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
	    	send_to_char( " ", ch );
	}

	for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}

	send_to_char( "'\n\r", ch );
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d, level %d",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char(buf,ch);
	
	if ( paf->duration > -1)
	    sprintf(buf,", %d hours.\n\r",paf->duration);
	else
	    sprintf(buf,".\n\r");
	send_to_char( buf, ch );
	if (paf->bitvector)
	{
	    switch(paf->where)
	    {
		case TO_AFFECTS:
		    sprintf(buf,"Adds %s affect.\n",
			affect_bit_name(paf->bitvector));
		    break;

		case TO_AFFECTS2:
    		sprintf(buf, "Also adds %s affect.\n",
        	affect2_bit_name(paf->bitvector));
    		break;

		case TO_DETECTS:
		sprintf(buf, "Adds %s effect.\n",
		detect_bit_name(paf->bitvector));
		break;

                case TO_WEAPON:
                    sprintf(buf,"Adds %s weapon flags.\n",
                        weapon_bit_name(paf->bitvector));
		    break;
		case TO_OBJECT:
		    sprintf(buf,"Adds %s object flag.\n",
			extra_bit_name(paf->bitvector));
		    break;
		case TO_IMMUNE:
		    sprintf(buf,"Adds immunity to %s.\n",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_RESIST:
		    sprintf(buf,"Adds resistance to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		case TO_VULN:
		    sprintf(buf,"Adds vulnerability to %s.\n\r",
			imm_bit_name(paf->bitvector));
		    break;
		default:
		    sprintf(buf,"Unknown bit %d: %d\n\r",
			paf->where,paf->bitvector);
		    break;
	    }
	    send_to_char(buf,ch);
	}
    }

    if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf, "Affects %s by %d, level %d.\n\r",
	    affect_loc_name( paf->location ), paf->modifier,paf->level );
	send_to_char( buf, ch );
        if (paf->bitvector)
        {
            switch(paf->where)
            {
                case TO_AFFECTS:
                    sprintf(buf,"Adds %s affect.\n",
                        affect_bit_name(paf->bitvector));
                    break;
		case TO_AFFECTS2:
                sprintf(buf, "Also adds %s affect.\n",
                affect2_bit_name(paf->bitvector));
                break;

                case TO_DETECTS:
                sprintf(buf, "Adds %s effect.\n",
                detect_bit_name(paf->bitvector));
                break;

                case TO_OBJECT:
                    sprintf(buf,"Adds %s object flag.\n",
                        extra_bit_name(paf->bitvector));
                    break;
                case TO_IMMUNE:
                    sprintf(buf,"Adds immunity to %s.\n",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_RESIST:
                    sprintf(buf,"Adds resistance to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                case TO_VULN:
                    sprintf(buf,"Adds vulnerability to %s.\n\r",
                        imm_bit_name(paf->bitvector));
                    break;
                default:
                    sprintf(buf,"Unknown bit %d: %d\n\r",
                        paf->where,paf->bitvector);
                    break;
            }
            send_to_char(buf,ch);
        }
    }
    sprintf( buf, "Item progs: " );
    if ( obj->pIndexData->progtypes != 0 )  {
	if ( IS_SET(obj->progtypes, IPROG_GET ) )
	   strcat( buf, "get " );
	if ( IS_SET(obj->progtypes, IPROG_DROP ) )
	   strcat( buf, "drop " );
	if ( IS_SET(obj->progtypes, IPROG_SAC ) )
	   strcat( buf, "sacrifice " );
	if ( IS_SET(obj->progtypes, IPROG_GIVE ) )
	   strcat( buf, "give " );
	if ( IS_SET(obj->progtypes, IPROG_FIGHT ) )
	   strcat( buf, "fight " );
	if ( IS_SET(obj->progtypes, IPROG_DEATH ) )
	   strcat( buf, "death " );
	if ( IS_SET(obj->progtypes, IPROG_SPEECH ) )
	   strcat( buf, "speech " );
	if ( IS_SET(obj->progtypes, IPROG_AREA ) )
	   strcat( buf, "area " );
    }	
    strcat( buf, "\n\r" );
    send_to_char( buf, ch );
    return;
}



void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[50];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;
  int d, h, m, s = 0;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stat whom?\n\r", ch );
	return;
    }

//    if ( ( victim = get_char_room( ch, argument ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL, argument ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: %s\n\r",
	victim->name);
    send_to_char( buf, ch );

    sprintf( buf, 
	"Vnum: %d  Format: %s  Race: %s  Group: %d  Sex: %s  Room: %d\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	IS_NPC(victim) ? victim->pIndexData->new_format ? "new" : "old" : "pc",
	race_table[victim->race].name,
	IS_NPC(victim) ? victim->group : 0, sex_table[victim->sex].name,
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    if (IS_NPC(victim))
    {
	sprintf(buf,"Count: %d  Killed: %d  ---  Status: %d\n\r",
	    victim->pIndexData->count,victim->pIndexData->killed, victim->status);
	send_to_char(buf,ch);
    }
    else
    {
	sprintf(buf,"{RPkills: %d     {GTotalDeaths: %d     {YQP: %d{x\n\r",
	victim->pcdata->pkills,
	victim->pcdata->total_death,
	victim->questpoints);
	send_to_char(buf,ch);
    }
    sprintf( buf, 
   	"Str: %d(%d)  Int: %d(%d)  Wis: %d(%d)  Dex: %d(%d)  Con: %d(%d)\n\r",
	victim->perm_stat[STAT_STR],
	get_curr_stat(victim,STAT_STR),
	victim->perm_stat[STAT_INT],
	get_curr_stat(victim,STAT_INT),
	victim->perm_stat[STAT_WIS],
	get_curr_stat(victim,STAT_WIS),
	victim->perm_stat[STAT_DEX],
	get_curr_stat(victim,STAT_DEX),
	victim->perm_stat[STAT_CON],
	get_curr_stat(victim,STAT_CON) );
    send_to_char( buf, ch );


    sprintf( buf, "Hp: %d/%d  Mana: %d/%d  Move: %d/%d  Practices: %d\n\r",
	victim->hit,         victim->max_hit,
	victim->mana,        victim->max_mana,
	victim->move,        victim->max_move,
	IS_NPC(ch) ? 0 : victim->practice );
    send_to_char( buf, ch );
	
    if ( IS_NPC(victim) )
      sprintf( buf2, "%d", victim->alignment );
    else  {
      sprintf( buf2, "%s", 
	victim->ethos==1?"Law-":
	victim->ethos==2?"Neut-":
	victim->ethos==3?"Cha-":"none-" );
      strcat( buf2, 
	IS_GOOD(victim)?"Good":
	IS_NEUTRAL(victim)?"Neut":
	IS_EVIL(victim)?"Evil":"Other" );
    }
/*
if (victim->religion > 0) {
  sprintf(buf,"%s believes the religion of %s.\n\r",
      victim->sex == 0 ? "It" : victim->sex == 1 ? "He" : "She",
   IS_NPC(victim) ? "Takeda" : religion_table[victim->religion].leader);
    send_to_char(buf,ch);
}
*/
    sprintf( buf,
	"Lv: %d  Class: %s  Align: %s  Gold: %ld  Silver: %ld  Exp: %d\n\r",
	victim->level,       
	IS_NPC(victim) ? "mobile" : class_table[victim->class].name,            
	buf2,
	victim->gold, victim->silver, victim->exp );
    send_to_char( buf, ch );

    sprintf(buf,"Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
	    GET_AC(victim,AC_PIERCE), GET_AC(victim,AC_BASH),
	    GET_AC(victim,AC_SLASH),  GET_AC(victim,AC_EXOTIC));
    send_to_char(buf,ch);

    sprintf( buf, 
	"Hit: %d  Dam: %d  Saves: %d  Size: %s  Position: %s  Wimpy: %d\n\r",
	GET_HITROLL(victim), GET_DAMROLL(victim), victim->saving_throw,
	size_table[victim->size].name, position_table[victim->position].name,
	victim->wimpy );
    send_to_char( buf, ch );

    if (IS_NPC(victim) && victim->pIndexData->new_format)
    {
	sprintf(buf, "Damage: %dd%d  Message:  %s\n\r",
	    victim->damage[DICE_NUMBER],victim->damage[DICE_TYPE],
	    attack_table[victim->dam_type].noun);
	send_to_char(buf,ch);
    }
    sprintf( buf, "Fighting: %s\n\r",
	victim->fighting ? victim->fighting->name : "(none)" );
    send_to_char( buf, ch );

    if ( !IS_NPC(victim) )
    {
	sprintf( buf,
	    "Thirst: %d  Hunger: %d  Full: %d  Drunk: %d  BloodLust: %d\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_HUNGER],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK],
	     victim->pcdata->condition[COND_BLOODLUST]);
	send_to_char( buf, ch );
    }

    sprintf( buf, "Carry number: %d  Carry weight: %ld\n\r",
	victim->carry_number, get_carry_weight(victim) / 10 );
    send_to_char( buf, ch );


    if (!IS_NPC(victim))
    {
    	sprintf( buf, 
	    "Age: %d  Played: %d  Last Level: %d  Timer: %d\n\r",
	    get_age(victim), 
	    (int) (victim->played + current_time - victim->logon) / 3600, 
	    victim->pcdata->last_level, 
	    victim->timer );
    	send_to_char( buf, ch );
    }

    sprintf(buf, "Act: %s\n\r",act_bit_name(victim->act));
    send_to_char(buf,ch);
    
    if (victim->comm)
    {
    	sprintf(buf,"Comm: %s\n\r",comm_bit_name(victim->comm));
    	send_to_char(buf,ch);
    }

    if (IS_NPC(victim) && victim->off_flags)
    {
    	sprintf(buf, "Offense: %s\n\r",off_bit_name(victim->off_flags));
	send_to_char(buf,ch);
    }

    if (victim->imm_flags)
    {
	sprintf(buf, "Immune: %s\n\r",imm_bit_name(victim->imm_flags));
	send_to_char(buf,ch);
    }
 
    if (victim->res_flags)
    {
	sprintf(buf, "Resist: %s\n\r", imm_bit_name(victim->res_flags));
	send_to_char(buf,ch);
    }

    if (victim->vuln_flags)
    {
	sprintf(buf, "Vulnerable: %s\n\r", imm_bit_name(victim->vuln_flags));
	send_to_char(buf,ch);
    }

    sprintf(buf, "Form: %s\n\rParts: %s\n\r", 
	form_bit_name(victim->form), part_bit_name(victim->parts));
    send_to_char(buf,ch);

    if (victim->affected_by)
    {
	sprintf(buf, "Affected by %s\n\r", 
	    affect_bit_name(victim->affected_by));
	send_to_char(buf,ch);
    }
    if (victim->affected_by2)
    {
	sprintf(buf, "Also Affected by %s\n\r",
	affect2_bit_name(victim->affected_by2));
	send_to_char(buf,ch);
	}
    if (victim->detection)
    {
	sprintf(buf,"Detection: %s\n\r",
	detect_bit_name(victim->detection));
	send_to_char(buf,ch);
	}

    sprintf( buf, "Master: %s  Leader: %s  Pet: %s\n\r",
	victim->master      ? victim->master->name   : "(none)",
	victim->leader      ? victim->leader->name   : "(none)",
	victim->pet 	    ? victim->pet->name	     : "(none)");
    send_to_char( buf, ch );

    if (!IS_NPC(victim))
    {
    sprintf( buf, "Security: %d.\n\r", victim->pcdata->security ); /* OLC */
    send_to_char( buf, ch );					   /* OLC */
    }

    sprintf( buf, "Short description: %s\n\rLong  description: %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "(none)\n\r" );
    send_to_char( buf, ch );

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
    {
	sprintf(buf,"Mobile has special procedure %s.\n\r",
		spec_name(victim->spec_fun));
	send_to_char(buf,ch);
    }

    for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
	sprintf( buf,
	    "Spell: '%s' modifies %s by %d for %d hours with bits %s, level %d.\n\r",
	    skill_table[(int) paf->type].abiliti_name,
	    affect_loc_name( paf->location ),
	    paf->modifier,
	    paf->duration,
	(paf->where == TO_AFFECTS) ? affect_bit_name( paf->bitvector ) :
    (paf->where == TO_AFFECTS2) ? affect2_bit_name( paf->bitvector ):
	detect_bit_name(paf->bitvector),
	    paf->level
	    );
	send_to_char( buf, ch );
    }

    if ( IS_NPC(victim)  )
      if ( victim->pIndexData->progtypes != 0 )  {
        sprintf( buf, "Mobile progs: " );
	if ( IS_SET( victim->progtypes, MPROG_BRIBE ) )
	  strcat( buf, "bribe " );
	if ( IS_SET( victim->progtypes, MPROG_SPEECH ) )
	  strcat( buf, "speech " );
	if ( IS_SET( victim->progtypes, MPROG_GIVE ) )
	  strcat( buf, "give " );
	if ( IS_SET( victim->progtypes, MPROG_DEATH ) )
	  strcat( buf, "death " );
	if ( IS_SET( victim->progtypes, MPROG_GREET ) )
	  strcat( buf, "greet " );
	if ( IS_SET( victim->progtypes, MPROG_ENTRY ) )
	  strcat( buf, "entry " );
	if ( IS_SET( victim->progtypes, MPROG_FIGHT ) )
	  strcat( buf, "fight " );
	if ( IS_SET( victim->progtypes, MPROG_AREA ) )
	  strcat( buf, "area " );
        strcat( buf, "\n\r" );
        send_to_char( buf, ch );
    }	
    sprintf(buf, "Last fought: %10s  Last fight time: %s\n\r", 
	victim->last_fought!=NULL?victim->last_fought->name:"none", 
	ctime( &(victim->last_fight_time) )     );
    send_to_char( buf, ch );
 if(!IS_NPC(victim)) {
    s = current_time - victim->lastpk;
    d = s/86400;
    s-=d*86400;
    h = s/3600;
    s -= h*3600;
    m = s/60;
    s -= m*60;
    sprintf(buf,
    "Last Pk was %d days, %d hours, %d minutes and %d seconds ago.\n\r",
    d, h, m, s);
    send_to_char(buf, ch);
}
 if(!IS_NPC(victim)) {
    s = current_time - victim->lastkingdom;
    d = s/86400;
    s-=d*86400;
    h = s/3600;
    s -= h*3600;
    m = s/60;
    s -= m*60;
    sprintf(buf,
    "Last Kingdom was %d days, %d hours, %d minutes and %d seconds ago.\n\r",
    d, h, m, s);
    send_to_char(buf, ch);
}
    return;
}

/* ofind and mfind replaced with vnum, vnum skill also added */

void do_vnum(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char *string;

    string = one_argument(argument,arg);
 
    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  vnum obj <name>\n\r",ch);
	send_to_char("  vnum mob <name>\n\r",ch);
	send_to_char("  vnum skill <skill or spell>\n\r",ch);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_ofind(ch,string);
 	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    { 
	do_mfind(ch,string);
	return;
    }

    if (!str_cmp(arg,"skill") || !str_cmp(arg,"spell"))
    {
	do_slookup(ch,string);
	return;
    }
    /* do both */
    do_mfind(ch,argument);
    do_ofind(ch,argument);
}


void do_mfind( CHAR_DATA *ch, char *argument )
{
    extern int top_mob_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find whom?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !str_cmp( arg, "all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_mob_index; vnum++ )
    {
	if ( ( pMobIndex = get_mob_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name_imm( argument, pMobIndex->player_name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pMobIndex->vnum, pMobIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "No mobiles by that name.\n\r", ch );

    return;
}



void do_ofind( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find what?\n\r", ch );
	return;
    }

    fAll	= FALSE; /* !str_cmp( arg, "all" ); */
    found	= FALSE;
    nMatch	= 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	    nMatch++;
	    if ( fAll || is_name_imm( argument, pObjIndex->name ) )
	    {
		found = TRUE;
		sprintf( buf, "[%5d] %s\n\r",
		    pObjIndex->vnum, pObjIndex->short_descr );
		send_to_char( buf, ch );
	    }
	}
    }

    if ( !found )
	send_to_char( "No objects by that name.\n\r", ch );

    return;
}


void do_owhere(CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = 200;

    buffer = new_buf();

    if (argument[0] == '\0')
    {
	send_to_char("Find what?\n\r",ch);
	return;
    }
 
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
        if ( !can_see_obj( ch, obj ) || !is_name_imm( argument, obj->name )
        ||   ch->level < obj->level)
            continue;
 
        found = TRUE;
        number++;
 
        for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
            ;
 
        if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by)
	&&   in_obj->carried_by->in_room != NULL)
            sprintf( buf, "%3d) %s is carried by %s [Room %d]\n\r",
                number, obj->short_descr,PERS(in_obj->carried_by, ch),
		in_obj->carried_by->in_room->vnum );
        else if (in_obj->in_room != NULL && can_see_room(ch,in_obj->in_room))
            sprintf( buf, "%3d) %s is in %s [Room %d]\n\r",
                number, obj->short_descr,in_obj->in_room->name, 
	   	in_obj->in_room->vnum);
	else
            sprintf( buf, "%3d) %s is somewhere\n\r",number, obj->short_descr);
 
        buf[0] = UPPER(buf[0]);
        add_buf(buffer,buf);
 
        if (number >= max_found)
            break;
    }
 
    if ( !found )
        send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
    else
        page_to_char(buf_string(buffer),ch);

    free_buf(buffer);
}


void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    BUFFER *buffer;
    CHAR_DATA *victim;
    bool found;
    int count = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    found = FALSE;
    buffer = new_buf();

    if ( arg1[0] == '\0' )
    {
        DESCRIPTOR_DATA *d;

        /* show characters logged */

        for (d = descriptor_list; d != NULL; d = d->next)
        {
            if (d->character != NULL && d->connected == CON_PLAYING
            &&  d->character->in_room != NULL && can_see(ch,d->character)
            &&  can_see_room(ch,d->character->in_room))
            {
                victim = d->character;
                count++;
                if (d->original != NULL)
                    sprintf(buf,"%3d) %s (in the body of %s) is in %s [%d]\n\r",
                        count, d->original->name,victim->short_descr,
                        victim->in_room->name,victim->in_room->vnum);
                else
         sprintf(buf,"%3d) %s is in %s [%d]\n\r",
                        count, victim->name,victim->in_room->name,
                        victim->in_room->vnum);
                add_buf(buffer,buf);
            }
        }

        page_to_char(buf_string(buffer),ch);
        free_buf(buffer);
        return;
    }
    else if(arg2[0] == '\0' )
    {
      for ( victim = char_list; victim != NULL; victim = victim->next )
      {
        if ( victim->in_room != NULL
        &&   is_name( arg1, victim->name ) 
	&&   can_see(ch,victim))
      {
            found = TRUE;
            count++;
            sprintf( buf, "%3d) [%5d] %-28s [%5d] %s\n\r", count,
                IS_NPC(victim) ? victim->pIndexData->vnum : 0,
                IS_NPC(victim) ? victim->short_descr : victim->name,
                victim->in_room->vnum,
                victim->in_room->name );
            add_buf(buffer,buf);
        }
      }
    }
    else if(!str_cmp(arg1,"all") && !str_cmp(arg2,"area"))
    {
      found = FALSE;
      buffer = new_buf();
      for ( victim = char_list; victim != NULL; victim = victim->next )
  {
        if ( victim->in_room != NULL
        &&   victim->in_room->area == ch->in_room->area
        &&   IS_NPC(victim) 
	&&   can_see(ch,victim) )
        {
            found = TRUE;
            count++;
            sprintf( buf, "%3d) [%5d] %-28s [%5d] %s\n\r", count,
                IS_NPC(victim) ? victim->pIndexData->vnum : 0,
                IS_NPC(victim) ? victim->short_descr : victim->name,
                victim->in_room->vnum,
                victim->in_room->name );
            add_buf(buffer,buf);
        }
      }
    }
    else if(!str_cmp(arg2,"area"))
  {
      found = FALSE;
      buffer = new_buf();
      for ( victim = char_list; victim != NULL; victim = victim->next )
      {
        if ( victim->in_room != NULL
        &&   victim->in_room->area == ch->in_room->area
        &&   can_see(ch,victim) 
        &&   is_name( arg1, victim->name ) )
        {
            found = TRUE;
            count++;
            sprintf( buf, "%3d) [%5d] %-28s [%5d] %s\n\r", count,
                IS_NPC(victim) ? victim->pIndexData->vnum : 0,
                IS_NPC(victim) ? victim->short_descr : victim->name,
                victim->in_room->vnum,
                victim->in_room->name );
            add_buf(buffer,buf);
     }
      }
    }
    if ( !found )
    {
      if ( !str_cmp(arg2,"area") )
         act("You didn't find any $T in this area.", ch, NULL, arg1,TO_CHAR );
      else
         act("You didn't find any $T.", ch, NULL, arg1, TO_CHAR );
    }
    else
    {
        page_to_char(buf_string(buffer),ch);
    }
  free_buf(buffer);

    return;
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    return;
}



void do_reboot( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;

    if (ch->invis_level < LEVEL_HERO)
    {
    	sprintf( buf, "Reboot by %s.", ch->name );
    	do_echo( ch, buf );
    }
    do_force ( ch, "all save");
    do_save (ch, "");
    do_save_clans(ch,"");
    merc_down = TRUE;
    for ( d = descriptor_list; d != NULL; d = d_next )
    {
	d_next = d->next;
    	close_socket(d);
    }
    
    return;
}



void do_shutdow( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}



void do_shutdown( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;

    if (ch->invis_level < LEVEL_HERO)
    sprintf( buf, "Shutdown by %s.", ch->name );
    append_file( ch, SHUTDOWN_FILE, buf );
    strcat( buf, "\n\r" );
    if (ch->invis_level < LEVEL_HERO)
    	do_echo( ch, buf );
    do_force ( ch, "all quit");
    merc_down = TRUE;
    for ( d = descriptor_list; d != NULL; d = d_next)
    {
	d_next = d->next;
	close_socket(d);
    }
    return;
}

void do_protect( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;

    if (argument[0] == '\0')
    {
	send_to_char("Protect whom from snooping?\n\r",ch);
	return;
    }

    if ((victim = get_char_world(ch,argument)) == NULL)
    {
	send_to_char("You can't find them.\n\r",ch);
	return;
    }

    if (IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	act_new("$N is no longer snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
        if (strcmp (ch->name, "Takeda"))
	send_to_char("Your snoop-proofing was just removed.\n\r",victim);
	REMOVE_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
    else
    {
	act_new("$N is now snoop-proof.",ch,NULL,victim,TO_CHAR,POS_DEAD);
	send_to_char("You are now immune to snooping.\n\r",victim);
	SET_BIT(victim->comm,COMM_SNOOP_PROOF);
    }
}
  


void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	wiznet("$N stops being such a snoop.",
		ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == ch->desc )
		d->snoop_by = NULL;
	}
	return;
    }

    if ( victim->desc->snoop_by != NULL )
    {
	send_to_char( "Busy already.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room 
    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
        send_to_char("That character is in a private room.\n\r",ch);
        return;
    }

    if (strcmp (ch->name, "Takeda") || strcmp(ch->name, "Kazmir"))
    if ( get_trust( victim ) >= get_trust( ch ) 
    ||   IS_SET(victim->comm,COMM_SNOOP_PROOF))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL )
    {
	for ( d = ch->desc->snoop_by; d != NULL; d = d->snoop_by )
	{
	    if ( d->character == victim || d->original == victim )
	    {
		send_to_char( "No snoop loops.\n\r", ch );
		return;
	    }
	}
    }

    victim->desc->snoop_by = ch->desc;
    if (strcmp (ch->name, "Takeda") || strcmp(ch->name, "Kazmir"))
    {
    sprintf(buf,"$N starts snooping on %s",
	(IS_NPC(ch) ? victim->short_descr : victim->name));
    wiznet(buf,ch,NULL,WIZ_SNOOPS,WIZ_SECURE,get_trust(ch));
    }
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;
    
    if ( ch->desc->original != NULL )
    {
	send_to_char( "You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim))
    {
	send_to_char("You can only switch into mobiles.\n\r",ch);
	return;
    }

    if (!is_room_owner(ch,victim->in_room) && ch->in_room != victim->in_room 
    &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
	send_to_char("That character is in a private room.\n\r",ch);
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( "Character in use.\n\r", ch );
	return;
    }

    sprintf(buf,"$N switches into %s",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));

    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    ch->desc            = NULL;
    /* change communications to match */
    if (ch->prompt != NULL)
        victim->prompt = str_dup(ch->prompt);
    victim->comm = ch->comm;
    victim->lines = ch->lines;
    send_to_char( "Ok.\n\r", victim );
    return;
}



void do_return( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char( 
"You return to your original body. Type replay to see any missed tells.\n\r", 
	ch );
    if (ch->prompt != NULL)
    {
	free_string(ch->prompt);
	ch->prompt = NULL;
    }

    sprintf(buf,"$N returns from %s.",ch->short_descr);
    wiznet(buf,ch->desc->original,0,WIZ_SWITCHES,WIZ_SECURE,get_trust(ch));
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc; 
    ch->desc                  = NULL;
    return;
}

/* trust levels for load and clone */
bool obj_check (CHAR_DATA *ch, OBJ_DATA *obj)
{
    if (IS_TRUSTED(ch,GOD)
	|| (IS_TRUSTED(ch,IMMORTAL) && obj->level <= 20 && obj->cost <= 1000)
	|| (IS_TRUSTED(ch,DEMI)	    && obj->level <= 10 && obj->cost <= 500)
	|| (IS_TRUSTED(ch,ANGEL)    && obj->level <=  5 && obj->cost <= 250)
	|| (IS_TRUSTED(ch,AVATAR)   && obj->level ==  0 && obj->cost <= 100))
	return TRUE;
    else
	return FALSE;
}

/* for clone, to insure that cloning goes many levels deep */
void recursive_clone(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone)
{
    OBJ_DATA *c_obj, *t_obj;


    for (c_obj = obj->contains; c_obj != NULL; c_obj = c_obj->next_content)
    {
	if (obj_check(ch,c_obj))
	{
	    t_obj = create_object(c_obj->pIndexData,0);
	    clone_object(c_obj,t_obj);
	    obj_to_obj(t_obj,clone);
	    recursive_clone(ch,c_obj,t_obj);
	}
    }
}

/* command that is similar to load */
void do_clone(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char *rest;
    CHAR_DATA *mob;
    OBJ_DATA  *obj;

    rest = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Clone what?\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	mob = NULL;
//	obj = get_obj_here(ch,rest);
	obj = get_obj_here(ch,NULL,rest);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	obj = NULL;
//	mob = get_char_room(ch,rest);
	mob = get_char_room(ch,NULL,rest);
	if (mob == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else /* find both */
    {
//	mob = get_char_room(ch,argument);
	mob = get_char_room(ch,NULL,argument);
//	obj = get_obj_here(ch,argument);
	obj = get_obj_here(ch,NULL,argument);
	if (mob == NULL && obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }

    /* clone an object */
    if (obj != NULL)
    {
	OBJ_DATA *clone;

	if (!obj_check(ch,obj))
	{
	    send_to_char(
		"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	clone = create_object(obj->pIndexData,0); 
	clone_object(obj,clone);
	if (obj->carried_by != NULL)
	    obj_to_char(clone,ch);
	else
	    obj_to_room(clone,ch->in_room);
 	recursive_clone(ch,obj,clone);

	act("$n has created $p.",ch,clone,NULL,TO_ROOM);
	act("You clone $p.",ch,clone,NULL,TO_CHAR);
	wiznet("$N clones $p.",ch,clone,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
	return;
    }
    else if (mob != NULL)
    {
	CHAR_DATA *clone;
	OBJ_DATA *new_obj;
	char buf[MAX_STRING_LENGTH];

	if (!IS_NPC(mob))
	{
	    send_to_char("You can only clone mobiles.\n\r",ch);
	    return;
	}

	if ((mob->level > 20 && !IS_TRUSTED(ch,GOD))
	||  (mob->level > 10 && !IS_TRUSTED(ch,IMMORTAL))
	||  (mob->level >  5 && !IS_TRUSTED(ch,DEMI))
	||  (mob->level >  0 && !IS_TRUSTED(ch,ANGEL))
	||  !IS_TRUSTED(ch,AVATAR))
	{
	    send_to_char(
		"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	clone = create_mobile(mob->pIndexData);
	while ( clone->affected != NULL )
        affect_strip( clone, clone->affected->type );

	clone_mobile(mob,clone); 
	
	for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	{
	    if (obj_check(ch,obj))
	    {
		new_obj = create_object(obj->pIndexData,0);
		clone_object(obj,new_obj);
		recursive_clone(ch,obj,new_obj);
		obj_to_char(new_obj,clone);
		new_obj->wear_loc = obj->wear_loc;
	    }
	}
	char_to_room(clone,ch->in_room);
        act("$n has created $N.",ch,NULL,clone,TO_ROOM);
        act("You clone $N.",ch,NULL,clone,TO_CHAR);
	sprintf(buf,"$N clones %s.",clone->short_descr);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
        return;
    }
}

/* RT to replace the two load commands */

void do_load(CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  load mob <vnum>\n\r",ch);
	send_to_char("  load obj <vnum> <level>\n\r",ch);
	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    {
	do_mload(ch,argument);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_oload(ch,argument);
	return;
    }
    /* echo syntax */
    do_load(ch,"");
}


void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    
    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: load mob <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

   if ( !IS_BUILDER( ch, get_mob_index( atoi( arg ) )->area ) && (ch->level < 86))
	{
	    send_to_char( "Sorry as builder you can't load from this area.{x\n\r", ch );
	    return;
	}

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
    sprintf(buf,"$N loads %s.",victim->short_descr);
    wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH] ,arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;
    
    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number(arg1))
    {
	send_to_char( "Syntax: load obj <vnum> <level>.\n\r", ch );
	return;
    }
    
    level = get_trust(ch); /* default */
  
    if ( arg2[0] != '\0')  /* load with a level */
    {
	if (!is_number(arg2))
        {
	  send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
	  return;
	}
        level = atoi(arg2);
        if (level < 0 || level > get_trust(ch))
	{
	  send_to_char( "Level must be be between 0 and your level.\n\r",ch);
  	  return;
	}
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

   if ( (!IS_BUILDER( ch, get_obj_index( atoi( arg1 )  )->area )) && (ch->level < 86))
	{
	    send_to_char( "Sorry as builder you can't load from this area.{x\n\r", ch );
	    return;
	}

    obj = create_object( pObjIndex, level );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
	obj_to_char( obj, ch );
    else
	obj_to_room( obj, ch->in_room );
    act( "$n has created $p!", ch, obj, NULL, TO_ROOM );

    if ( ( pObjIndex->limit != -1 ) && ( pObjIndex->count > pObjIndex->limit ) )
     {
         send_to_char( "{B[{RWARNING{B]:{X The Object you load is over MAX{x\n\r", ch );
         wiznet("$N Loading a limited Object over MAX $p.",ch,obj,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
         return;
     }
    else
     { 
      wiznet("$N loads $p.",ch,obj,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
      send_to_char( "Ok.\n\r", ch );
      return;
     } 
}



void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if ( IS_NPC(victim) && !IS_SET(victim->act,ACT_NOPURGE) 
	    &&   victim != ch /* safety precaution */ )
		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))
	      extract_obj( obj );
	}

	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) )
    {

	if (ch == victim)
	{
	  send_to_char("Ho ho ho.\n\r",ch);
	  return;
	}

	if (get_trust(ch) <= get_trust(victim))
	{
	  send_to_char("Maybe that wasn't a good idea...\n\r",ch);
	  sprintf(buf,"%s tried to purge you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}

	act("$n disintegrates $N.",ch,0,victim,TO_NOTVICT);

    	if (victim->level > 1)
	    save_char_obj( victim );
    	d = victim->desc;
    	extract_char( victim, TRUE );
    	if ( d != NULL )
          close_socket( d );

	return;
    }

    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    extract_char( victim, TRUE );
    return;
}


void do_trust( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int level;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: trust <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 0 || level > MAX_LEVEL )
    {
	sprintf(buf, "Level must be 0 (reset) or 1 to %d.\n\r",MAX_LEVEL);
	send_to_char(buf,ch);
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust.\n\r", ch );
	return;
    }

    victim->trust = level;
    return;
}



void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    DESCRIPTOR_DATA *d;

    one_argument( argument, arg );
    if (arg[0] == '\0' || !str_cmp(arg,"room"))
    {
    /* cure room */
    	
        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            affect_strip(vch,gsn_plague);
            affect_strip(vch,gsn_poison);
            affect_strip(vch,gsn_blindness);
            affect_strip(vch,gsn_sleep);
            affect_strip(vch,gsn_curse);
            
            vch->hit 	= vch->max_hit;
            vch->mana	= vch->max_mana;
            vch->move	= vch->max_move;
            update_pos( vch);
            act("$n has restored you.",ch,NULL,vch,TO_VICT);
        }

        sprintf(buf,"$N restored room %d.",ch->in_room->vnum);
        wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));
        
        send_to_char("Room restored.\n\r",ch);
        return;

    }
    
    if ( get_trust(ch) >=  MAX_LEVEL - 10 && !str_cmp(arg,"all"))
    {
    /* cure all */
    	
        for (d = descriptor_list; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;
                
            affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blindness);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);
            
            victim->hit 	= victim->max_hit;
            victim->mana	= victim->max_mana;
            victim->move	= victim->max_move;
            update_pos( victim);
	    if (victim->in_room != NULL)
                act("$n has restored you.",ch,NULL,victim,TO_VICT);
        }
	send_to_char("All active players restored.\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    affect_strip(victim,gsn_plague);
    affect_strip(victim,gsn_poison);
    affect_strip(victim,gsn_blindness);
    affect_strip(victim,gsn_sleep);
    affect_strip(victim,gsn_curse);
    victim->hit  = victim->max_hit;
    victim->mana = victim->max_mana;
    victim->move = victim->max_move;
    update_pos( victim );
    act( "$n has restored you.", ch, NULL, victim, TO_VICT );
    sprintf(buf,"$N restored %s",
	IS_NPC(victim) ? victim->short_descr : victim->name);
    wiznet(buf,ch,NULL,WIZ_RESTORE,WIZ_SECURE,get_trust(ch));
    send_to_char( "Ok.\n\r", ch );
    return;
}

 	
void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->act, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can play again.\n\r", victim );
	send_to_char( "FREEZE removed.\n\r", ch );
	sprintf(buf,"$N thaws %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->act, PLR_FREEZE);
	send_to_char( "You can't do ANYthing!\n\r", victim );
	send_to_char( "FREEZE set.\n\r", ch );
	sprintf(buf,"$N puts %s in the deep freeze.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    save_char_obj( victim );

    return;
}



void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->act, PLR_LOG) )
    {
	REMOVE_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->act, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOEMOTE) )
    {
	REMOVE_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can emote again.\n\r", victim );
	send_to_char( "NOEMOTE removed.\n\r", ch );
	sprintf(buf,"$N restores emotes to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can't emote!\n\r", victim );
	send_to_char( "NOEMOTE set.\n\r", ch );
	sprintf(buf,"$N revokes %s's emotes.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_noshout( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noshout whom?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOSHOUT) )
    {
	REMOVE_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can shout again.\n\r", victim );
	send_to_char( "NOSHOUT removed.\n\r", ch );
	sprintf(buf,"$N restores shouts to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can't shout!\n\r", victim );
	send_to_char( "NOSHOUT set.\n\r", ch );
	sprintf(buf,"$N revokes %s's shouts.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOTELL) )
    {
	REMOVE_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can tell again.\n\r", victim );
	send_to_char( "NOTELL removed.\n\r", ch );
	sprintf(buf,"$N restores tells to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can't tell!\n\r", victim );
	send_to_char( "NOTELL set.\n\r", ch );
	sprintf(buf,"$N revokes %s's tells.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}

void do_nonote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Nonote whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( get_trust( victim ) >= get_trust( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NONOTE) )
    {
	REMOVE_BIT(victim->comm, COMM_NONOTE);
	send_to_char( "You can write notes again.\n\r", victim );
	send_to_char( "NONOTE removed.\n\r", ch );
	sprintf(buf,"$N restores nonotes to %s.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }
    else
    {
	SET_BIT(victim->comm, COMM_NONOTE);
	send_to_char( "You can't write note!\n\r", victim );
	send_to_char( "NONOTES set.\n\r", ch );
	sprintf(buf,"$N revokes %s's nonotes.",victim->name);
	wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
	    rch->last_fought = NULL;
	    stop_fighting( rch, TRUE );
	if (IS_NPC(rch) && IS_SET(rch->act,ACT_AGGRESSIVE))
	    REMOVE_BIT(rch->act,ACT_AGGRESSIVE);
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_wizlock( CHAR_DATA *ch, char *argument )
{
    extern bool wizlock;
    wizlock = !wizlock;

    if ( wizlock )
    {
	wiznet("$N has wizlocked the game.",ch,NULL,0,0,0);
	send_to_char( "Game wizlocked.\n\r", ch );
    }
    else
    {
	wiznet("$N removes wizlock.",ch,NULL,0,0,0);
	send_to_char( "Game un-wizlocked.\n\r", ch );
    }

    return;
}

/* RT anti-newbie code */

void do_newlock( CHAR_DATA *ch, char *argument )
{
    extern bool newlock;
    newlock = !newlock;
 
    if ( newlock )
    {
	wiznet("$N locks out new characters.",ch,NULL,0,0,0);
        send_to_char( "New characters have been locked out.\n\r", ch );
    }
    else
    {
	wiznet("$N allows new characters back in.",ch,NULL,0,0,0);
        send_to_char( "Newlock removed.\n\r", ch );
    }
 
    return;
}


void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Lookup which skill or spell?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].abiliti_name == NULL )
		break;
	    sprintf( buf, "Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",
		sn, skill_table[sn].abiliti_slot, skill_table[sn].abiliti_name );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %3d  Slot: %3d  Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].abiliti_slot, skill_table[sn].abiliti_name );
	send_to_char( buf, ch );
    }

    return;
}

/* RT set replaces sset, mset, oset, and rset */

void do_set( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set mob   <name> <field> <value>\n\r",ch);
	send_to_char("  set obj   <name> <field> <value>\n\r",ch);
	send_to_char("  set room  <room> <field> <value>\n\r",ch);
        send_to_char("  set skill <name> <spell or skill> <value>\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	do_mset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"skill") || !str_prefix(arg,"spell"))
    {
	do_sset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	do_oset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"room"))
    {
	do_rset(ch,argument);
	return;
    }
    /* echo syntax */
    do_set(ch,"");
}


void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set skill <name> <spell or skill> <value>\n\r", ch);
	send_to_char( "  set skill <name> all <value>\n\r",ch);  
	send_to_char("   (use the name of the skill, not the number)\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( "Value range is 0 to 100.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].abiliti_name != NULL )
		victim->pcdata->learned[sn]	= value;
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
    }

    return;
}


void do_string( CHAR_DATA *ch, char *argument )
{
    char type [MAX_INPUT_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    smash_tilde( argument );
    argument = one_argument( argument, type );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( type[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  string char <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long desc title spec\n\r",ch);
	send_to_char("  string obj  <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long extended\n\r",ch);
	send_to_char("    Also you can string games\n\r",ch);
	return;
    }
    
    if (!str_prefix(type,"character") || !str_prefix(type,"mobile"))
    {
    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
    	}

	/* clear zone for mobs */
	victim->zone = NULL;

     	if ( !str_prefix( arg2, "name" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( "Not on PC's.\n\r", ch );
	    	return;
	    }
	    free_string( victim->name );
	    victim->name = str_dup( arg3 );
	    return;
    	}
    	
    	if ( !str_prefix( arg2, "description" ) )
    	{
    	    free_string(victim->description);
    	    victim->description = str_dup(arg3);
    	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    free_string( victim->short_descr );
	    victim->short_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    free_string( victim->long_descr );
	    strcat(arg3,"\n\r");
	    victim->long_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "title" ) )
    	{
	    if ( IS_NPC(victim) )
	    {
	    	send_to_char( "Not on NPC's.\n\r", ch );
	    	return;
	    }

	    set_title( victim, arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "spec" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( "Not on PC's.\n\r", ch );
	    	return;
	    }

	    if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	    {
	    	send_to_char( "No such spec fun.\n\r", ch );
	    	return;
	    }

	    return;
    	}
    }
    
    if (!str_prefix(type,"object"))
    {
    	/* string an obj */
    	
   	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	    return;
    	}
    	
        if ( !str_prefix( arg2, "name" ) )
    	{
	    free_string( obj->name );
	    obj->name = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    free_string( obj->short_descr );
	    obj->short_descr = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    free_string( obj->description );
	    obj->description = str_dup( arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "ed" ) || !str_prefix( arg2, "extended"))
    	{
	    EXTRA_DESCR_DATA *ed;

	    argument = one_argument( argument, arg3 );
	    if ( argument == NULL )
	    {
	    	send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		    ch );
	    	return;
	    }

 	    strcat(argument,"\n\r");

	    ed = new_extra_descr();

	    ed->keyword		= str_dup( arg3     );
	    ed->description	= str_dup( argument );
	    ed->next		= obj->extra_descr;
	    obj->extra_descr	= ed;
	    return;
    	}
    }
    
    	
    /* echo bad use message */
    do_string(ch,"");
}



void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set obj <object> <field> <value>\n\r",ch);
	send_to_char("  Field being one of:\n\r",				ch );
	send_to_char("    value0 value1 value2 value3 value4 (v1-v4)\n\r",	ch );
	send_to_char("    extra wear level weight cost timer\n\r",		ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_cmp( arg2, "value0" ) || !str_cmp( arg2, "v0" ) )
    {
	obj->value[0] = UMIN(50,value);
	return;
    }

    if ( !str_cmp( arg2, "value1" ) || !str_cmp( arg2, "v1" ) )
    {
	obj->value[1] = value;
	return;
    }

    if ( !str_cmp( arg2, "value2" ) || !str_cmp( arg2, "v2" ) )
    {
	obj->value[2] = value;
	return;
    }

    if ( !str_cmp( arg2, "value3" ) || !str_cmp( arg2, "v3" ) )
    {
	obj->value[3] = value;
	return;
    }

    if ( !str_cmp( arg2, "value4" ) || !str_cmp( arg2, "v4" ) )
    {
	obj->value[4] = value;
	return;
    }

    if ( !str_prefix( arg2, "extra" ) )
    {
	obj->extra_flags = value;
	return;
    }

    if ( !str_prefix( arg2, "wear" ) )
    {
	obj->wear_flags = value;
	return;
    }

    if ( !str_prefix( arg2, "level" ) )
    {
	obj->level = value;
	return;
    }
	
    if ( !str_prefix( arg2, "weight" ) )
    {
	obj->weight = value;
	return;
    }

    if ( !str_prefix( arg2, "cost" ) )
    {
	obj->cost = value;
	return;
    }

    if ( !str_prefix( arg2, "timer" ) )
    {
	obj->timer = value;
	return;
    }
	
    /*
     * Generate usage message.
     */
    do_oset( ch, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set room <location> <field> <value>\n\r",ch);
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    flags sector\n\r",				ch );
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if (!is_room_owner(ch,location) && ch->in_room != location 
    &&  room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR))
    {
        send_to_char("That room is private right now.\n\r",ch);
        return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_prefix( arg2, "flags" ) )
    {
	location->room_flags	= value;
	return;
    }

    if ( !str_prefix( arg2, "sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_rset( ch, "" );
    return;
}

void do_sockets( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA       *vch;
    DESCRIPTOR_DATA *d;
    char            buf  [ MAX_STRING_LENGTH ];
    char            buf2 [ MAX_STRING_LENGTH ];
    int             count;
    char *          st;
    char            s[100];
    char            idle[10];
    char *	    abc;
    count       = 0;
    buf[0]      = '\0';
    buf2[0]     = '\0';

    strcat( buf2,
"\n\r{C:{B==========================================================================={C:{n\n\r");
    strcat( buf2,
"{B|<><><><><><><><><><><><><><>     {Y Sockets {B	<><><><><><><><><><><><><><>|{n\n\r");
    strcat( buf2,
"{C:{B==========================================================================={C:{n\n\r");
        strcat( buf2,
"{B| [{CNum State    Login{B@{C  Idl{B] [{CPlayer Name{B] [{CHosts{B]			    |{n\n\r");
        strcat( buf2,
"{C:{B==========================================================================={C:{n\n\r");

    for ( d = descriptor_list; d; d = d->next )
    {
        if ( d->character && can_see( ch, d->character ) )
        {
           switch( d->connected )
           {
              case CON_PLAYING:              st = "PLAYING ";break;
              case CON_GET_NAME:             st = "Get Name";break;
              case CON_GET_OLD_PASSWORD:     st = "Passwd  ";break;
              case CON_CONFIRM_NEW_NAME:     st = "New Nam ";break;
              case CON_GET_NEW_PASSWORD:     st = "New Pwd ";break;
              case CON_CONFIRM_NEW_PASSWORD: st = "Con Pwd ";break;
              case CON_GET_NEW_SEX:          st = "New Sex ";break;
              case CON_GET_NEW_CLASS:        st = "New Clas";break;
              case CON_GET_NEW_RACE:         st = "New Race";break;
              case CON_GET_ALIGNMENT:        st = "New Aln "; break;
              case CON_PICK_WEAPON:          st = "Pick Wea"; break;
              case CON_READ_IMOTD:           st = " IMOTD  "; break;
              case CON_READ_MOTD:	     st = "  MOTD  "; break;
              case CON_NOTE_TO:       	     st = " Wr Note"; break;
              case CON_NOTE_SUBJECT:  	     st = " Wr Subj"; break;
              case CON_NOTE_EXPIRE:   	     st = " Wr Expi"; break;
              case CON_NOTE_TEXT:     	     st = " Wr Text"; break;
              case CON_NOTE_FINISH:  	     st = " Wr Fin "; break;
              case CON_ROLL_STATS:    	     st = "Rol Stat"; break;
	      case CON_PICK_HOMETOWN:        st = "Pic Home"; break;
              default:                       st = "UNKNOWN!";    break;
           }
           count++;

           /* Format "login" value... */
           vch = d->original ? d->original : d->character;
           strftime( s, 100, "%I:%M%p", localtime( &vch->logon ) );
          abc = (d->original ) ? d->original->name : ( d->character )  ? d->character->name : "(None!)";

           if ( vch->timer > 0 )
              sprintf( idle, "%-2d", vch->timer );
           else
              sprintf( idle, "  " );
      sprintf( buf, "{B| [{C%-3d %-8s %7s  %2s{B] {C%-12s %-33s {B|{n\n\r",
              d->descriptor,
              st,
              s,
              idle,
              ( d->original ) ? d->original->name
                              : ( d->character )  ? d->character->name
					: "(None!)",d->host);
           strcat( buf2, buf );

        }
    }
    strcat( buf2,
"{C:{B										    |{n");
   sprintf( buf,
"\n\r{B| Users:{C%-2d 								    {B|{n\n \r", count );
    strcat( buf2, buf );
    strcat( buf2,
"{C:{B==========================================================================={C:{n\n");
    send_to_char( buf2, ch );
    return;
}
/*

	if ( d->character != NULL && can_see( ch, d->character ) 
	&& (arg[0] == '\0' || is_name_imm(arg,d->character->name)
			   || (d->original && is_name_imm(arg,d->original->name))))
*/
/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    one_argument(argument,arg2);
  
    if (!str_cmp(arg2,"delete") || !str_prefix(arg2,"mob"))
    {
	send_to_char("That will NOT be done.\n\r",ch);
	return;
    }

    sprintf( buf, "$n forces you to '%s'.", argument );

    if ( !str_cmp( arg, "all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	   if (vch->desc->connected != CON_PLAYING)
	    {
	     send_to_char("Don't force link dead mortals!\n\r", ch);
	     return;
	    }


	    if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) )
	    {
		act( buf, ch, NULL, vch, TO_VICT );
		interpret( vch, argument );
	    }
	}
    }
    else if (!str_cmp(arg,"players"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
 
        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;
 
 	   if (vch->desc->connected != CON_PLAYING)
	    {
	     send_to_char("Don't force link dead mortals!\n\r", ch);
	     return;
	    }
 
            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch ) 
	    &&	 vch->level < LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else if (!str_cmp(arg,"gods"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
 
        if (get_trust(ch) <= MAX_LEVEL - 5)
        {
            send_to_char("Not at your level!\n\r",ch);
            return;
        }
 
        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;
 	   if (vch->desc->connected != CON_PLAYING)
	    {
	     send_to_char("Don't force link dead mortals!\n\r", ch);
	     return;
	    }
 
            if ( !IS_NPC(vch) && get_trust( vch ) < get_trust( ch )
            &&   vch->level >= LEVEL_HERO)
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

    	if (!is_room_owner(ch,victim->in_room) 
	&&  ch->in_room != victim->in_room 
        &&  room_is_private(victim->in_room) && !IS_TRUSTED(ch,IMPLEMENTOR))
    	{
            send_to_char("That character is in a private room.\n\r",ch);
            return;
        }

	if ( get_trust( victim ) >= get_trust( ch ) )
	{
	    if (strcmp(ch->name, "Takeda") )  { /* if not kroudar */
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	    }
	}
/*
	if ( (!IS_NPC(victim) && get_trust(ch) <= MAX_LEVEL-11) &&
		get_trust(ch) != 87)
	{
	    send_to_char("Not at your level!\n\r",ch);
	    return;
	}
*/
	act( buf, ch, NULL, victim, TO_VICT );
	interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, char *argument )
{
    int level;
    char arg[MAX_STRING_LENGTH];

    /* RT code for taking a level argument */
    one_argument( argument, arg );

    if ( arg[0] == '\0' ) 
    /* take the default path */

      if ( ch->invis_level)
      {
	  ch->invis_level = 0;
	  act( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM );
	  send_to_char( "You slowly fade back into existence.\n\r", ch );
      }
      else
      {
	  ch->invis_level = get_trust(ch);
	  act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
	  send_to_char( "You slowly vanish into thin air.\n\r", ch );
      }
    else
    /* do the level thing */
    {
      level = atoi(arg);
      if (level < 2 || level > get_trust(ch))
      {
	send_to_char("Invis level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
	  ch->reply = NULL;
          ch->invis_level = level;
          act( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You slowly vanish into thin air.\n\r", ch );
      }
    }

    return;
}


void do_incognito( CHAR_DATA *ch, char *argument )
{
    int level;
    char arg[MAX_STRING_LENGTH];
 
    /* RT code for taking a level argument */
    one_argument( argument, arg );
 
    if ( arg[0] == '\0' )
    /* take the default path */
 
      if ( ch->incog_level)
      {
          ch->incog_level = 0;
          act( "$n is no longer cloaked.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You are no longer cloaked.\n\r", ch );
      }
      else
      {
          ch->incog_level = get_trust(ch);
          act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You cloak your presence.\n\r", ch );
      }
    else
    /* do the level thing */
    {
      level = atoi(arg);
      if (level < 2 || level > get_trust(ch))
      {
        send_to_char("Incog level must be between 2 and your level.\n\r",ch);
        return;
      }
      else
      {
          ch->reply = NULL;
          ch->incog_level = level;
          act( "$n cloaks $s presence.", ch, NULL, NULL, TO_ROOM );
          send_to_char( "You cloak your presence.\n\r", ch );
      }
    }
 
    return;
}



void do_holylight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode on.\n\r", ch );
    }

    return;
}

/* prefix command: it will put the string typed on each line typed */

void do_prefi (CHAR_DATA *ch, char *argument)
{
    send_to_char("You cannot abbreviate the prefix command.\r\n",ch);
    return;
}

void do_prefix (CHAR_DATA *ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];

    if (argument[0] == '\0')
    {
	if (ch->prefix[0] == '\0')
	{
	    send_to_char("You have no prefix to clear.\r\n",ch);
	    return;
	}

	send_to_char("Prefix removed.\r\n",ch);
	free_string(ch->prefix);
	ch->prefix = str_dup("");
	return;
    }

    if (ch->prefix[0] != '\0')
    {
	sprintf(buf,"Prefix changed to %s.\r\n",argument);
	free_string(ch->prefix);
    }
    else
    {
	sprintf(buf,"Prefix set to %s.\r\n",argument);
    }

    ch->prefix = str_dup(argument);
}


/*       CF  WIZ BLOCK    */

/* RT nochannels command, for those spammers */

void do_advance( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int level;
    int iLevel;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: advance <char> <level>.\n\r", ch );
	return;
    }

//    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL ,arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

	if (get_trust(ch) < get_trust(victim))
	{
	  send_to_char("Maybe that wasn't a good idea...\n\r",ch);
	  sprintf(buf,"%s tried to advance you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}


    if ( ( level = atoi( arg2 ) ) < 1 || level > MAX_LEVEL )
    {
        sprintf(buf,"Level must be 1 to %d.\n\r",MAX_LEVEL);
	send_to_char(buf,ch);
	return;
    }

    if ( level > get_trust( ch ) )
    {
	send_to_char( "Limited to your trust level.\n\r", ch );
	return;
    } 

/* Explorer plz when you do changes make sureyou tell me cuz what you did
 * before, i couldn't even advance people above 90....
 *						- Takeda
 */
    if ((ch->level <= 95) && (level > 90)) {
	send_to_char("Sorry you are not allow to promote someone above 90.\n\r",ch);
	return;
}


    /* Level counting */
    if (ch->level <= 5 || ch->level > LEVEL_HERO) { /* didn't count before */

      if (5 < level && level <= LEVEL_HERO) /* counts now */
	total_levels += level - 5;

    }
    else { /* counted before */
      
      if (5 < level && level <= LEVEL_HERO) /* counts now */
	total_levels += level - ch->level;
      
      else total_levels -= (ch->level - 5); /* doesn't count anymore */
    }

    /*
     * Lower level:
     *   Reset to level 1.
     *   Then raise again.
     *   Currently, an imp can lower another imp.
     *   -- Swiftest
     */
    if ( level <= victim->level )
    {
        int temp_prac;

	send_to_char( "Lowering a player's level!\n\r", ch );
	send_to_char( "{r**** OOOOHHHHHHHHHH  NNNNOOOO ****{x\n\r", victim );
	temp_prac = victim->practice;
	victim->level    = 1;
/*	victim->exp      = exp_per_level(victim,victim->pcdata->points);*/
	victim->exp      = exp_to_level(victim,victim->pcdata->points);
	victim->max_hit  = 10;
	victim->max_mana = 100;
	victim->max_move = 100;
	victim->practice = 0;
	victim->hit      = victim->max_hit;
	victim->mana     = victim->max_mana;
	victim->move     = victim->max_move;
	advance_level( victim );
	victim->practice = temp_prac;
    }
    else
    {
	send_to_char( "Raising a player's level!\n\r", ch );
	send_to_char( "{b**** OOOOHHHHHHHHHH  YYYYEEEESSS ****{x\n\r", victim );
    }

    for ( iLevel = victim->level ; iLevel < level; iLevel++ )
    {
	send_to_char( "{WYou attain a new level of knowledge.{x\n\r", victim );
	victim->exp += exp_to_level(victim,victim->pcdata->points);
	victim->level += 1;
	advance_level( victim );
    }
/*    victim->exp   =  exp_per_level(victim,victim->pcdata->points) 
		  * UMAX( 1, victim->level ); */
    victim->trust = 0;
    save_char_obj(victim);
    return;
}

void do_builder( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;
    int iLevel;

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: builder <char>.\n\r", ch );
	return;
    }

//    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    if ( ( victim = get_char_room( ch,NULL,arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }



    level = 84;
	send_to_char("Advancing levels............\n\r",ch);
    for ( iLevel = victim->level ; iLevel < level; iLevel++ )
    {
	send_to_char( "{WYou attain a new level of knowledge.{x\n\r", victim );
	victim->exp += exp_to_level(victim,victim->pcdata->points);
	victim->level += 1;
	advance_level( victim );
    }
	send_to_char("Granting commands.......\n\r",ch);
	NEW_AUTHORIZE(victim,auth_holylight);
	NEW_AUTHORIZE(victim,auth_oedit);
	NEW_AUTHORIZE(victim,auth_immtalk);
	NEW_AUTHORIZE(victim,auth_incognito);
	NEW_AUTHORIZE(victim,auth_wizinvis);
	NEW_AUTHORIZE(victim,auth_peace);
	NEW_AUTHORIZE(victim,auth_asave);
	NEW_AUTHORIZE(victim,auth_redit);
	NEW_AUTHORIZE(victim,auth_medit);
	NEW_AUTHORIZE(victim,auth_aedit);
	NEW_AUTHORIZE(victim,auth_alist);
	NEW_AUTHORIZE(victim,auth_edit);
  send_to_char("Builder advanced: Remember to put him where his area is.\n\r",ch);
    save_char_obj(victim);
    return;
}

void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *victim;
    int value;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set char <name> <field> <value>\n\r",ch); 
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    str int wis dex con sex class level\n\r",	ch );
	send_to_char( "    race gold hp mana move practice align\n\r",	ch );
	send_to_char( "    train thirst drunk full hometown ethos\n\r",	ch );
 	send_to_char( "    security bloodlust hunger\n\r",	ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */

    if ( !str_cmp( arg2, "str" ) )
    {
	if ( value < 1 || value > get_max_train(victim,STAT_STR) )
	{
	    sprintf(buf,
		"Strength range is 1 to %d\n\r.",
		get_max_train(victim,STAT_STR));
	    send_to_char(buf,ch);
	    return;
	}

	victim->perm_stat[STAT_STR] = value;
	return;
    }

    if ( !str_cmp( arg2, "security" ) )	/* OLC */
    {
	if ( IS_NPC( ch ) )
	{
	   send_to_char("Return first!\n\r", ch);
	return;
	}

        if ( IS_NPC( victim ) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }

	if ( value > ch->pcdata->security || value < 0 )
	{
	    if ( ch->pcdata->security != 0 )
	    {
		sprintf( buf, "Valid security is 0-%d.\n\r",
		    ch->pcdata->security );
		send_to_char( buf, ch );
	    }
	    else
	    {
		send_to_char( "Valid security is 0 only.\n\r", ch );
	    }
	    return;
	}
	victim->pcdata->security = value;
	return;
    }

    if ( !str_cmp( arg2, "int" ) )
    {
        if ( value < 1 || value > get_max_train(victim,STAT_INT) )
        {
            sprintf(buf,
		"Intelligence range is 1 to %d.\n\r",
		get_max_train(victim,STAT_INT));
            send_to_char(buf,ch);
            return;
        }
 
        victim->perm_stat[STAT_INT] = value;
        return;
    }

    if ( !str_cmp( arg2, "wis" ) )
    {
	if ( value < 1 || value > get_max_train(victim,STAT_WIS) )
	{
	    sprintf(buf,
		"Wisdom range is 1 to %d.\n\r",get_max_train(victim,STAT_WIS));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_WIS] = value;
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( value < 1 || value > get_max_train(victim,STAT_DEX) )
	{
	    sprintf(buf,
		"Dexterity ranges is 1 to %d.\n\r",
		get_max_train(victim,STAT_DEX));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_DEX] = value;
	return;
    }

    if ( !str_cmp( arg2, "con" ) )
    {
	if ( value < 1 || value > get_max_train(victim,STAT_CON) )
	{
	    sprintf(buf,
		"Constitution range is 1 to %d.\n\r",
		get_max_train(victim,STAT_CON));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_CON] = value;
	return;
    }

    if ( !str_prefix( arg2, "sex" ) )
    {
	if ( value < 0 || value > 2 )
	{
	    send_to_char( "Sex range is 0 to 2.\n\r", ch );
	    return;
	}
	victim->sex = value;
	if (!IS_NPC(victim))
	    victim->pcdata->true_sex = value;
	return;
    }

    if ( !str_prefix( arg2, "class" ) )
    {
	int class;

	if (IS_NPC(victim))
	{
	    send_to_char("Mobiles have no class.\n\r",ch);
	    return;
	}

	class = class_lookup(arg3);
	if ( class == -1 )
	{
	    char buf[MAX_STRING_LENGTH];

        	strcpy( buf, "Possible classes are: " );
        	for ( class = 0; class < MAX_CLASS; class++ )
        	{
            	    if ( class > 0 )
                    	strcat( buf, " " );
            	    strcat( buf, class_table[class].name );
        	}
            strcat( buf, ".\n\r" );

	    send_to_char(buf,ch);
	    return;
	}

	victim->class = class;
	return;
    }

    if ( !str_prefix( arg2, "level" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > MAX_LEVEL )
	{
	sprintf(buf,"Level range is 0 to %d.\n\r",MAX_LEVEL);
	send_to_char(buf,ch);
	    return;
	}
	victim->level = value;
	return;
    }

    if ( !str_prefix( arg2, "gold" ) )
    {
	victim->gold = value;
	return;
    }

    if ( !str_prefix( arg2, "hp" ) )
    {
	if ( value < -10 || value > 30000 )
	{
	    send_to_char( "Hp range is -10 to 30,000 hit points.\n\r", ch );
	    return;
	}
	victim->max_hit = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_hit = value;
	return;
    }

    if ( !str_prefix( arg2, "mana" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Mana range is 0 to 30,000 mana points.\n\r", ch );
	    return;
	}
	victim->max_mana = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_mana = value;
	return;
    }

    if ( !str_prefix( arg2, "move" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Move range is 0 to 30,000 move points.\n\r", ch );
	    return;
	}
	victim->max_move = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_move = value;
	return;
    }

    if ( !str_prefix( arg2, "practice" ) )
    {
	if ( value < 0 || value > 250 )
	{
	    send_to_char( "Practice range is 0 to 250 sessions.\n\r", ch );
	    return;
	}
	victim->practice = value;
	return;
    }

    if ( !str_prefix( arg2, "train" ))
    {
	if (value < 0 || value > 50 )
	{
	    send_to_char("Training session range is 0 to 50 sessions.\n\r",ch);
	    return;
	}
	victim->train = value;
	return;
    }

    if ( !str_prefix( arg2, "align" ) )
    {
	if ( value < -1000 || value > 1000 )
	{
	    send_to_char( "Alignment range is -1000 to 1000.\n\r", ch );
	    return;
	}
	victim->alignment = value;
	send_to_char("Remember to check their hometown.\n\r", ch);
	return;
    }

    if (!str_prefix(arg2, "ethos"))
      {
	if (IS_NPC(victim))
	  {
	    send_to_char("Mobiles don't have an ethos.\n\r", ch);
	    return;
	  }
	if (value < 0 || value > 3)
	  {
	send_to_char("The values are Lawful - 1, Neutral - 2, Chaotic - 3\n\r",
			ch);
	    return;
	  }

	victim->ethos = value;
	return;
      }

    if ( !str_prefix( arg2, "hometown" ) )
    {
        if ( IS_NPC(victim) )
	{
	    send_to_char( "Mobiles don't have hometowns.\n\r", ch );
	    return;
	}
        if ( value < 0 || value > 4 )
        { 
            send_to_char( "Please choose one of the following :.\n\r", ch );
            send_to_char( "Town        Alignment       Value\n\r", ch);
            send_to_char( "----        ---------       -----\n\r", ch);
            send_to_char( "Sanaria      Any              0\n\r", ch);
            send_to_char( "New Thalos   Any              1\n\r", ch);
            send_to_char( "Rivendell    Good             2\n\r", ch);
            send_to_char( "Ofcol        Neutral          3\n\r", ch);
            send_to_char( "Arkham       Evil             4\n\r", ch);
            return;
        }

        if ((value == 2 && !IS_GOOD(victim)) || (value == 3 &&
	!IS_NEUTRAL(victim)) || (value == 4 && !IS_EVIL(victim)))
        { 
            send_to_char( "The hometown doesn't match this character's alignment.\n\r", ch );
            return;
        }    
        
        victim->hometown = value;
        return;
    }

    if ( !str_prefix( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Thirst range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }
    
    if ( !str_prefix( arg2, "hunger" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "hunger range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_HUNGER] = value;
	return;
    }

    if ( !str_prefix( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Drunk range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	return;
    }

    if ( !str_prefix( arg2, "full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Full range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	return;
    }

   if ( !str_prefix( arg2, "bloodlust" ) )
    {
        if ( IS_NPC(victim) )
        {
            send_to_char( "Not on NPC's.\n\r", ch );
            return;
        }

        if ( value < -1 || value > 100 )
        {
            send_to_char( "Full range is -1 to 100.\n\r", ch );
            return;
        }

        victim->pcdata->condition[COND_BLOODLUST] = value;
        return;
    }

    if (!str_prefix( arg2, "race" ) )
    {
	int race;

	race = race_lookup(arg3);

	if ( race == 0)
	{
	    send_to_char("That is not a valid race.\n\r",ch);
	    return;
	}

	if (!IS_NPC(victim) && !race_table[race].pc_race)
	{
	    send_to_char("That is not a valid player race.\n\r",ch);
	    return;
	}

	victim->race = race;
	return;
    }


    /*
     * Generate usage message.
     */
    do_mset( ch, "" );
    return;
}

void do_desocket(CHAR_DATA *ch, char *argument)
{
  DESCRIPTOR_DATA *d;
  int socket;
  char arg[MAX_INPUT_LENGTH];

  one_argument(argument, arg);

  if (!is_number(arg))

    {
      send_to_char("The argument must be a number.\n\r", ch);
      return;
    }

  if (arg[0] == '\0')
    {
      send_to_char("Disconnect which socket?\n\r", ch);
      return;
    }

  else
    {
      socket = atoi(arg);
      for ( d = descriptor_list; d != NULL; d = d->next )      
	{
	  if (d->descriptor == socket)
	    {
	      if (d->character == ch)
		{
		  send_to_char("It would be foolish to disconnect yourself.\n\r", ch);
		  return;
		}
	      if (d->connected == CON_PLAYING)
		{
		  send_to_char("Why don't you just use disconnect?\n\r", ch);
		  return;
		}
	      write_to_descriptor(d->descriptor,
				  "You are being disconnected by an immortal.",
				  0);
	      close_socket(d);
	      send_to_char("Done.\n\r", ch);
	      return;
	    }
	}
      send_to_char("No such socket is connected.\n\r", ch);
      return;
    }
}

void do_smite(CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;

  if (argument[0] == '\0')
    {
      send_to_char("You are so frustrated you smite yourself!  OWW!\n\r", 
		ch);
      return;
    }

  if ((victim = get_char_world(ch, argument)) == NULL)
    {
      send_to_char("You'll have to smite them some other day.\n\r", ch);
      return;
    }

  if (IS_NPC(victim))
    {
      send_to_char("That poor mob never did anything to you.\n\r", ch);
      return;
    }

  if (victim->trust > ch->trust)
    {
      send_to_char("How dare you!\n\r", ch);
      return;
    }

  if (victim->position < POS_SLEEPING)
    {
      send_to_char("Take pity on the poor thing.\n\r", ch);
      return;
    }

  act("A bolt comes down out of the heavens and smites you!", victim, NULL,
	ch, TO_CHAR);
  act("You reach down and smite $n!", victim, NULL, ch, TO_VICT);
  act("A bolt from the heavens smites $n!", victim, NULL, ch, TO_NOTVICT);
  victim->hit -= (victim->hit / 2);
  return;
}

void do_popularity( CHAR_DATA *ch, char *argument )
{
char buf[MAX_STRING_LENGTH];
char buf2[160];
AREA_DATA *area;
extern AREA_DATA *area_first;
int i;

    sprintf(buf,"Area popularity statistics (in char * ticks)\n\r" );

    for (area = area_first,i=0; area != NULL; area = area->next,i++) {
      if (area->count >= 5000000)
        sprintf(buf2,"%-20s overflow       ",area->name);
      else
        sprintf(buf2,"%-20s %-8lu       ",area->name,area->count);
      if ( i % 2 == 0) 
	strcat( buf, "\n\r" );
      strcat( buf, buf2 );
    }
    strcat( buf, "\n\r\n\r");
    page_to_char( buf, ch );
    return;
}

void do_seize (CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( arg1[0] == '\0' ) || ( arg2[0] == '\0' ) )
  {
        send_to_char( "Syntax : seize <object> <player>\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
    {
        send_to_char( "They are not here!\n\r", ch );
        return;
    }

   if ( ( obj = get_obj_list( ch, arg1, victim->carrying ) ) == NULL )
    {
        send_to_char( "They do not have that item.\n\r", ch );
        return;
    }
   if ( victim->level >= ch->level && victim != ch)
    {
        send_to_char( "You Failed.\r\n", ch );
        return;
    }

    if ( obj->wear_loc != WEAR_NONE )
        unequip_char( victim, obj );

    obj_from_char( obj );
    obj_to_char( obj, ch );

    send_to_char( "Ok.\r\n", ch );

    sprintf( buf, "%s seizes %s from %s.", ch->name,obj->short_descr,victim->name);
    wiznet(buf,ch,NULL,WIZ_PENALTIES,WIZ_SECURE,0);
   return;
}

void do_copyover( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  argument = one_argument(argument,arg);

 if (arg[0] == '\0')
    {
      send_to_char("Usage: copyover now\n\r",ch);
      send_to_char("Usage: copyover <ticks to copyover>\n\r",ch);
      send_to_char("Usage: copyover cancel\n\r",ch);
      send_to_char("Usage: copyover status\n\r",ch);
      return;
    }

    if (is_name(arg,"cancel"))
     {
      copyover_counter = -1;
      send_to_char("Copyover canceled.\n\r",ch);
      return;
    }

    if (is_name(arg, "now"))
     {
       sprintf(buf, "Copyover by {C%s.{w\n\r",ch->name);
     copyover_fate();
      return;
    }

    if (is_name(arg, "status"))
    {
      if (copyover_counter == -1)
        sprintf(buf, "Automatic Copyover is inactive.\n\r");
      else
        sprintf(buf,"Copyover in %i minutes.\n\r",copyover_counter);
      send_to_char(buf,ch);
      return;
    }

    if (is_number(arg))
    {
     copyover_counter = atoi(arg);
     sprintf(buf,"Fate will copyover in %i ticks.\n\r",copyover_counter);
     send_to_char(buf,ch);
     return;
    }

 do_copyover(ch,"");
}


#define COPYOVER_FILE "copyover.data"

#define EXE_FILE        "../src/fate"

void copyover_fate ( void )
{

        FILE *fp;
        DESCRIPTOR_DATA *d, *d_next;
        char buf [100], buf2[100];
        extern int port, control;

        fp = fopen (COPYOVER_FILE, "w");

        if (!fp)
        {
        logf("Could not write to copyover file: %s", COPYOVER_FILE);
        perror("do_copyover:fopen");
       return;
        }
        sprintf(buf,"\n\r *** COPYOVER ***\n\r");
        do_asave(NULL, "");
        for(d = descriptor_list; d; d = d_next)
        {
        CHAR_DATA * och = CH (d);
        d_next = d->next;


        if(!d->character || d->connected > CON_PLAYING)
        {
        write_to_descriptor(d->descriptor,"\n\rSorry, we are rebooting, Come back in a few minutes.\n\r",0);
        close_socket(d);
        }
        else
        {
        fprintf(fp,"%d %s %s\n",d->descriptor, och->name, d->host);

                         /* This is not necessary for ROM */
                        if (och->level == 1)

           {
                                write_to_descriptor (d->descriptor, "Since you are level 1, you get a free level!\n\r",0);
                                advance_level (och);
                                och->level++;
                        }


        save_char_obj(och);
        write_to_descriptor(d->descriptor,buf,0);
        }
}
        fprintf(fp, "-1\n");
        fclose(fp);

        fclose(fpReserve);

        sprintf(buf, "%d",port);
        sprintf(buf2, "%d", control);
        execl(EXE_FILE, "fate", buf, "copyover", buf2, (char *) NULL);

        perror("do_copyover: execl");

        fpReserve = fopen(NULL_FILE, "r");
}

void copyover_recover ()
{
        DESCRIPTOR_DATA *d;
        FILE *fp;
        char name[100];
        char host[MAX_STRING_LENGTH];
        int desc;
        bool fOld;

        logf("Copyover recovery initiated");
        fp = fopen (COPYOVER_FILE, "r");

        if (!fp)
        {
        perror ("copyover_recover:fopen");
        logf("Copyover file not found. Exitting.\n\r");
        exit (1);
        }

        unlink(COPYOVER_FILE);



        for (;;)
        {
        fscanf (fp,"%d %s %s\n", &desc, name, host);
        if (desc == -1)
        break;

        if (!write_to_descriptor(desc,"\n\rRestoring from copyover...\n\r",0))
        {
        close(desc);
        continue;
        }
        d = new_descriptor();
        d->descriptor = desc;

        d->host = str_dup (host);
        d->next = descriptor_list;
        descriptor_list = d;
        d->connected = CON_COPYOVER_RECOVER;

        fOld = load_char_obj(d, name);

        if (!fOld)
        {
        write_to_descriptor(desc, "\n\rSomehow, your character was lost in the copyover.  Please speak to the immortals.\n\r",0);
        close_socket(d);
        }
        else
        {
        write_to_descriptor(desc,"\n\rCopyover recovery complete.\n\r",0);

        if(!d->character->in_room)
        d->character->in_room = get_room_index (ROOM_VNUM_TEMPLE);

        d->character->next = char_list;
        char_list = d->character;

        char_to_room(d->character, d->character->in_room);
        do_look(d->character, "auto");
        act("$n materializes!",d->character,NULL,NULL,TO_ROOM);

        d->connected = CON_PLAYING;

        if (d->character->pet != NULL)
        {
        char_to_room(d->character->pet,d->character->in_room);
        act("&n materializes!.",d->character->pet,NULL,NULL,TO_ROOM);
     }
}
}
fclose(fp);
}

void do_mlevel(CHAR_DATA *ch, char *argument)
{
        extern int top_mob_index;
        char buf[MAX_STRING_LENGTH];
        char arg[MAX_INPUT_LENGTH];
        BUFFER *buffer;
        MOB_INDEX_DATA *pMobIndex;
        int vnum, level;
   int nMatch;
        bool found;

        argument=one_argument(argument, arg);
        if (!is_number(arg) )
        {
            send_to_char("Syntax: mlevel [level]\n\r",ch);
            return;
        }
        level=atoi(arg);
        buffer=new_buf( );
        found=FALSE;
        nMatch=0;

        for(vnum=0;nMatch<top_mob_index;vnum++)
     {
           if ( (pMobIndex=get_mob_index(vnum)) !=NULL)
            nMatch++;
           else continue;
           if (level == pMobIndex->level)
           {
              found=TRUE;
              sprintf(buf, "[%5d] %s\n\r",pMobIndex->vnum,
                        pMobIndex->short_descr);
              add_buf(buffer,buf);
            }
         }
         if (!found)
             send_to_char("No mobiles by that level.\n\r",ch);
         else
             page_to_char(buf_string(buffer),ch);
         free_buf(buffer);
         return;
}

void do_olevel(CHAR_DATA *ch, char *argument)
{
        extern int top_obj_index;
        char buf[MAX_STRING_LENGTH];
        char arg[MAX_INPUT_LENGTH];
        BUFFER *buffer;
        OBJ_INDEX_DATA *pObjIndex;
        int vnum, level;
        int nMatch;
        bool found;

        argument=one_argument(argument, arg);
        if (!is_number(arg) )
        {
            send_to_char("Syntax: olevel [level]\n\r",ch);
            return;
        }
    level=atoi(arg);
        buffer=new_buf( );
        found=FALSE;
        nMatch=0;

        for(vnum=0;nMatch<top_obj_index;vnum++)
        {
           if ( (pObjIndex=get_obj_index(vnum)) !=NULL)
              nMatch++;
           else continue;
           if (level == pObjIndex->level)
           {
              found=TRUE;
              sprintf(buf, "[%5d] %s\n\r",pObjIndex->vnum,
                        pObjIndex->short_descr);
              add_buf(buffer,buf);
            }
      }
         if (!found)
             send_to_char("No objects by that level.\n\r",ch);
         else
             page_to_char(buf_string(buffer),ch);
         free_buf(buffer);
    return;
}

void do_immtitle(CHAR_DATA *ch, char *argument)
{
  if (argument[0] == '\0')
  {
    ch->immtitle = NULL;
    send_to_char("Immtitle cleared.\n\r", ch);
    return;
  }

  if (strlencolor(argument) > 13)
  {
    send_to_char("Immtitle must be 13 (or under) characters long.\n\r", ch); 
	 return;
  }

  ch->immtitle = str_dup(argument);
  send_to_char("Immtitle set.\n\r", ch);
}

void do_affrooms(CHAR_DATA *ch, char *argument)
 {
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *room_next;
    char buf[MAX_STRING_LENGTH];
    int count=0;

    if ( !top_affected_room) 
	{
	 send_to_char("No affected room.\n\r",ch);
	}
    for ( room = top_affected_room; room ; room = room_next )
    {
	room_next = room->aff_next;
	count++;
	sprintf(buf,"%d) [Vnum : %5d] %s\n\r",
		count, room->vnum , room->name);
	send_to_char(buf,ch);
    }
  return;
 }

void do_noquit(CHAR_DATA * ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;

    one_argument(argument, arg);

    if (arg[0] == '\0')
	 {
	  send_to_char("No quit whom?\n\r", ch);
	  return;
	 }
    if ((victim = get_char_world(ch, arg)) == NULL)
	 {
	  send_to_char("They aren't here.\n\r", ch);
	  return;
	 }
    if (IS_NPC(victim))
	 {
	  send_to_char("Not on NPC's.\n\r", ch);
	  return;
	 }
    if (get_trust(victim) >= get_trust(ch))
	 {
	  send_to_char("You failed.\n\r", ch);
	  return;
	 }
    if (victim->qflag == 1)
	 {
	  victim->qflag = 0;
	  send_to_char("You may quit again, when it suits you.\n\r",victim);
	  send_to_char("NoQuit removed.\n\r", ch);
	  return;
	 }
    victim->qflag = 1;

    send_to_char("You have lost the ability to quit.\n\r", victim);
    send_to_char("NoQuit set.\n\r", ch);
    sprintf(buf, "$N disallows %s from quiting.", victim->name);
    wiznet(buf, ch, NULL, WIZ_PENALTIES, WIZ_SECURE, 0);


    return;
}

void do_pload( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA d;
  bool isChar = FALSE;
  char name[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

 if (argument[0] == '\0')
  {
    send_to_char("Load who?\n\r", ch);
    return;
  }
  argument[0] = UPPER(argument[0]);
  argument = one_argument(argument, name);

  /* Dont want to load a second copy of a player who's allready online! */
  if (get_char_world( ch, name ) != NULL )
  {
    send_to_char( "That person is already connected!\n\r", ch );
    return;
  }

  isChar = load_char_obj(&d, name); /* char pfile exists? */

  if (!isChar)
  {
    send_to_char("Load Who? Are you sure? I cant seem to find them.\n\r",ch);
    return;
  }
  d.character->desc     = NULL;
  d.character->next     = char_list;
  char_list             = d.character;
  d.connected           = CON_PLAYING;
  reset_char(d.character);

  /* bring player to imm */
  if ( d.character->in_room != NULL )
  {
    d.character->load_room = d.character->in_room;
    char_to_room( d.character, ch->in_room); /* put in room imm is in */
  }
  sprintf(buf,"You bring %s into the game.\n\r",name);
  send_to_char(buf,ch);

  act( "$n has pulled $N from the pattern!", ch, NULL, d.character, TO_ROOM );

  if (d.character->pet != NULL)
 {
     char_to_room(d.character->pet,d.character->in_room);
     act("$n has entered the game.",d.character->pet,NULL,NULL,TO_ROOM);
   }

}

void do_punload( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char who[MAX_INPUT_LENGTH];
  char here[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  sprintf(buf,"A0 %s.\n\r",ch->name);
  send_to_char(buf,ch);

  argument = one_argument(argument, who);
  argument = one_argument(argument, here);

  if ( ( victim = get_char_world( ch, who ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }

  /** Person is legitametly logged on... was not ploaded.   */
  if (victim->desc != NULL)
  {
    send_to_char("I dont think that would be a good idea...\n\r", ch);
  return;
  }
  sprintf(buf,"A1 %s.\n\r",victim->name);
  send_to_char(buf,ch);


 if (!str_cmp (here, "here"))
  victim->load_room = ch->in_room;

  sprintf(buf,"A2.%s \n\r",victim->name);
  send_to_char(buf,ch);

  if (victim->load_room != NULL) /* return player and pet to orig room*/
  {
    char_from_room( victim );
    char_to_room(victim, victim->load_room);
//    if (victim->pet != NULL)
//       {
//        char_from_room( ch );
//      char_to_room(victim->pet, victim->load_room);
//       }
  sprintf(buf,"B2.%s \n\r",victim->name);
  send_to_char(buf,ch);

  }

  sprintf(buf,"You release %s back into the Patterns.\n\r",victim->name);
  send_to_char(buf,ch);
  act("$n has released $N back to the Pattern.",ch, NULL, victim, TO_ROOM);

  sprintf(buf,"C1.%s\n\r",victim->name);
  send_to_char(buf,ch);
  save_char_obj(victim);
  sprintf(buf,"D1.%s\n\r",victim->name);
  send_to_char(buf,ch);
  do_quit(victim,"");
return;
}

void do_wrlist( CHAR_DATA *ch, char *argument )
{
  ROOM_INDEX_DATA *room;
  ROOM_INDEX_DATA *in_room;
  MOB_INDEX_DATA *mob;
  OBJ_INDEX_DATA *obj;
  char arg[MAX_STRING_LENGTH];
  char arg1[MAX_STRING_LENGTH];
  char arg2[MAX_STRING_LENGTH];
  int uvnum;
  int lvnum;
  int MR = 60000;
  int type = -1;
  
  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  uvnum = ( is_number( arg2 ) ) ? atoi( arg2 ) : 0;
  lvnum = ( is_number( arg1 ) ) ? atoi( arg1 ) : 0;  

  if ( !str_cmp( arg, "o" ) )
    type = 2;
  if ( !str_cmp( arg, "m" ) )
    type = 1;
  if ( !str_cmp( arg, "r" ) )
    type = 0;
  if ( !str_cmp( arg, "p" ) )
    type = 3;

  if ( ( uvnum - lvnum ) > 200 )
  {
    send_to_char( "{WThat range is too large.\n\r{x", ch );
    return;
  }
  
  if ( ( ( uvnum == 0 ) && ( lvnum == 0 ) ) || ( arg[0] == '\0' ) 
   || ( type == -1 ) )
  {
    send_to_char( "{MSyntax: wrlist [type] [lvnum] [uvnum]\n\r{x", ch );
    return;
  }

  if ( uvnum > MR || uvnum < 1 || lvnum > MR || lvnum < 1 || lvnum >uvnum )
  {
    send_to_char( "{WInvalid level(s).\n\r{x", ch );
    return;
  }

  in_room = ch->in_room;  
  if ( type == 0 )
  {
    char_from_room( ch );
  }
  for ( MR = lvnum; MR <= uvnum; MR++ )
  {
    if ( type == 0 )
    {
      if ( ( room = get_room_index( MR ) ) )
      {
        sprintf( log_buf, "{R%-5d  {w%-20s\n\r", room->vnum, room->name);
        send_to_char( log_buf, ch );
        char_to_room( ch, room );
        do_resets( ch, "" );
        char_from_room( ch );
      }
    }
    if ( type == 2 )
    {
      if ( ( obj = get_obj_index( MR ) ) )
      {
        sprintf( log_buf, "{R%-5d  {w%-20s\n\r",  obj->vnum, obj->name );
        send_to_char( log_buf, ch );
      }
    }
    if ( type == 1 )
    {
      if ( ( mob = get_mob_index( MR ) ) )
      {
        sprintf( log_buf, "{R%-5d  {w%-20s\n\r", mob->vnum,mob->player_name );
        send_to_char( log_buf, ch );
      }
    }
    if ( type == 3 )
    {
      if ( ( mob = get_mob_index( MR ) ) )
      {
      } 
  }  
  }
  if ( type == 0 )
    char_to_room( ch, in_room );
  return;
 }

void do_rpbonus( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char       buf  [ MAX_STRING_LENGTH ];
    char       arg1 [ MAX_INPUT_LENGTH ];
    char       arg2 [ MAX_INPUT_LENGTH ];
    int      value,t1;
    
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
        send_to_char( "Syntax: bonus <char> <Exp>.\n\r", ch );
        send_to_char( "Syntax: bonus rp <char> <rptoken>.\n\r", ch );
        send_to_char( "Syntax: bonus rp <char> 0 return the current rp tokens.\n\r", ch );
    }
    
    if (( victim = get_char_world ( ch, arg1 ) ) == NULL )
    {
      send_to_char("That Player is not here.\n\r", ch);
      return;
    }
       
    if ( IS_NPC( victim ) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "You may not bonus yourself.\n\r", ch );
        return;
    }
/*
    if (IS_IMMORTAL(victim) || victim->level >= LEVEL_IMMORTAL)
    {
    send_to_char("You can't bonus immortals silly!\n\r", ch);
    return;
    }
*/
    value = atoi( arg2 );

    if ( value < -5000 || value > 5000 )
    {
        send_to_char( "Value range is -5000 to 5000.\n\r", ch );
        return;
    }

    if ( value == 0 )
    {
     sprintf( buf,"%s have %d rptokens.\n\r", victim->name,victim->rptoken);
     send_to_char(buf, ch);
     return;
    }

    if (value > 0)
     victim->rptoken += value;
    else
     {
     t1 = victim->rptoken + value;
     if ((t1) >= 0 )
      victim->rptoken += value;
     else
      {
       send_to_char( "Hmm you don't wanna give rp toknes on credit.\n\r", ch );
       return;
      } 
     }
     
    sprintf( buf,"You have bonused %s a whopping %d rptokens points.\n\r",
                victim->name, value);
                send_to_char(buf, ch);
    sprintf( buf,"%s  now have %d rptokens points.\n\r",
                victim->name, victim->rptoken);
                send_to_char(buf, ch);

    if ( value > 0 )
    { 
      sprintf( buf,"You have been bonused %d rptokens points.\n\r",value );
      send_to_char( buf, victim );
    }
    else
    {
      sprintf( buf,"You have been penalized %d rptokens points.\n\r",value );
      send_to_char( buf, victim );
    }
 
    return;
}  

void do_bonus( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    char       buf  [ MAX_STRING_LENGTH ];
    char       arg1 [ MAX_INPUT_LENGTH ];
    char       arg2 [ MAX_INPUT_LENGTH ];
    int      value;
    
    argument = one_argument( argument, arg1 );
   if(!str_cmp(arg1, "rp") )
     {
      do_rpbonus(ch,argument);
      return;
     }
    argument = one_argument( argument, arg2 );


    
    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
        send_to_char( "Syntax: bonus <char> <Exp>.\n\r", ch );
        send_to_char( "Syntax: bonus rp <char> <rptoken>.\n\r", ch );
        return;
    }
    
    if (( victim = get_char_world ( ch, arg1 ) ) == NULL )
    {
      send_to_char("That Player is not here.\n\r", ch);
      return;
    }
       
    if ( IS_NPC( victim ) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "You may not bonus yourself.\n\r", ch );
        return;
    }

    if (IS_IMMORTAL(victim) || victim->level >= LEVEL_IMMORTAL)
    {
    send_to_char("You can't bonus immortals silly!\n\r", ch);
    return;
    }

    value = atoi( arg2 );

    if ( value < -5000 || value > 5000 )
    {
        send_to_char( "Value range is -5000 to 5000.\n\r", ch );
        return;
    }

    if ( value == 0 )
    {
        send_to_char( "The value must not be equal to 0.\n\r", ch );
        return;
    }

    gain_exp(victim, value);
   
    sprintf( buf,"You have bonused %s a whopping %d experience points.\n\r",
                victim->name, value);
                send_to_char(buf, ch);

    if ( value > 0 )
    { 
      sprintf( buf,"You have been bonused %d experience points.\n\r",value );
      send_to_char( buf, victim );
    }
    else
    {
      sprintf( buf,"You have been penalized %d experience points.\n\r",value );
      send_to_char( buf, victim );
    }
 
    return;
}  


void do_check( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    CHAR_DATA *victim;
    /* AFFECT_DATA *paf; */
    int count = 1;
    /* int last_room = 0; */
    
    one_argument( argument, arg );
    
    if (arg[0] == '\0'|| !str_prefix(arg,"stats"))
    {
        buffer = new_buf();
        for (victim = char_list; victim != NULL; victim = victim->next)
        {
            if (IS_NPC(victim) || !can_see(ch,victim)) 
                continue;
                
            if (victim->desc == NULL)
            {
                sprintf(buf,"%3d) %s is linkdead.\n\r", count, victim->name);
                add_buf(buffer, buf);
                count++;
                continue;               
            }
            
            if (victim->desc->connected >= CON_GET_NEW_RACE
             && victim->desc->connected <= CON_PICK_WEAPON)
            {
                sprintf(buf,"%3d) %s is being created.\n\r",
                    count, victim->name);
                add_buf(buffer, buf);
                count++;
                continue;
            }
            
            if ( (victim->desc->connected == CON_GET_OLD_PASSWORD
               || victim->desc->connected >= CON_READ_IMOTD)
             && get_trust(victim) <= get_trust(ch) )
            {
                sprintf(buf,"%3d) %s is connecting.\n\r",
                    count, victim->name);
                add_buf(buffer, buf);
                count++;
                continue;                        
            }
            
            if (victim->desc->connected == CON_PLAYING)
            {
                if (get_trust(victim) > get_trust(ch))
                    sprintf(buf,"%3d) %s.\n\r", count, victim->name);
                else
                {
                    sprintf(buf,"%3d) %s, Level %d connected since %d hours (%d total hours)\n\r",
                        count, victim->name,victim->level,
                        ((int)(current_time - victim->logon)) /3600, 
                        (victim->played + (int)(current_time - victim->logon)) /3600 );
                    add_buf(buffer, buf);
                    if (arg[0]!='\0' && !str_prefix(arg,"stats"))
                    {
                      sprintf(buf,"  %d HP %d Mana (%d %d %d %d %d) %ld golds %d Tr %d Pr %d Qpts.\n\r",
                        victim->max_hit,
victim->max_mana,victim->perm_stat[STAT_STR],
victim->perm_stat[STAT_INT],victim->perm_stat[STAT_WIS],
victim->perm_stat[STAT_DEX],victim->perm_stat[STAT_CON],
                        victim->gold + victim->silver/100,
                        victim->train, victim->practice,victim->questpoints);
                      add_buf(buffer, buf);
                    }
                    count++;
                }
                continue;
            }
            
            sprintf(buf,"%3d) bug (oops)...please report to Takeda: %s %d\n\r",
                count, victim->name, victim->desc->connected);
            add_buf(buffer, buf);
            count++;   
        }
        page_to_char(buf_string(buffer),ch);
        free_buf(buffer);
        return;
    }
    
    if (!str_prefix(arg,"eq"))
    {
        buffer = new_buf();
        for (victim = char_list; victim != NULL; victim = victim->next)
        {
            if (IS_NPC(victim) 
             || victim->desc->connected != CON_PLAYING
             || !can_see(ch,victim)
             || get_trust(victim) > get_trust(ch) )
                continue;
                
            sprintf(buf,"%3d) %s, %d items (weight %d) Hit:%d Dam:%d Save:%d AC:%d %d %d %d.\n\r",
                count, victim->name, victim->carry_number,victim->carry_weight, 
                victim->hitroll, victim->damroll, victim->saving_throw,
                victim->armor[AC_PIERCE], victim->armor[AC_BASH],
                victim->armor[AC_SLASH], victim->armor[AC_EXOTIC]);
            add_buf(buffer, buf);
            count++;  
        }
        page_to_char(buf_string(buffer),ch);
        free_buf(buffer);       
        return;
    }

  if (!str_prefix(arg,"snoop")) /* this part by jerome */
    {
        char bufsnoop [100];

        if(ch->level < MAX_LEVEL )
          {
            send_to_char("You can't use this check option.\n\r",ch);
            return;
          }
        buffer = new_buf();

        for (victim = char_list; victim != NULL; victim = victim->next)
        {
            if (IS_NPC(victim)
             || victim->desc->connected != CON_PLAYING
             || !can_see(ch,victim)
             || get_trust(victim) > get_trust(ch) )
                continue;

            if(victim->desc->snoop_by != NULL)
              sprintf(bufsnoop," %15s.",victim->desc->snoop_by->character->name);
            else
              sprintf(bufsnoop,"     (none)      ." );

            sprintf(buf,"%3d %15s : %s \n\r",count,victim->name, bufsnoop);
            add_buf(buffer, buf);
            count++;
        }
        page_to_char(buf_string(buffer),ch);
        free_buf(buffer);
        return;
    }

        
    send_to_char("Syntax: 'check'       display info about players\n\r",ch);
    send_to_char("        'check stats' display info and resume stats\n\r",ch);
    send_to_char("        'check eq'    resume eq of all players\n\r",ch);
    send_to_char("Use the stat command in case of doubt about someone...\n\r",ch);
    return;
}

void do_cpassword( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char *pwdnew;
    char *p;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);


 	if ( arg1[0] == '\0' || arg2[0] == '\0'  )
         {
         send_to_char( "Cpassword <character> <password>\n\r",ch);
         return;
         }

   if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
         {
                  send_to_char( "They aren't playing.\n\r", ch );
                  return;
         }

    if ( IS_NPC(ch) || IS_NPC(victim) )
	{
	send_to_char("Not on NPC's",ch);
        return;
	}


	if (get_trust(ch) < get_trust(victim))
	{
	  send_to_char("Maybe that wasn't a good idea...\n\r",ch);
	  sprintf(buf,"%s tried give new password you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}


    if ( strlen(arg2) < 5 )
    {
        send_to_char(
            "New password must be at least five characters long.\n\r", ch );
    return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, victim->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
        if ( *p == '~' )
        {
            send_to_char(
                "New password not acceptable, try again.\n\r", ch );
            return;
        }
    }

    free_string( victim->pcdata->pwd );
    victim->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    sprintf(buf,"You have changed %s's password to: %s\n\r",
	 victim->name,arg2);
    send_to_char(buf,ch);
send_to_char("-----------------------------------------------------\n\r",victim);
    sprintf(buf,"%s have changed your password to: %s\n\r",
	ch->name,arg2);
    send_to_char(buf,victim);
send_to_char("-----------------------------------------------------\n\r",victim);
    return;
}

void do_balefir(CHAR_DATA *ch, char *argument)
{
  send_to_char("If you want to Balefire spell it out.\n\r",ch);
  return;
}

void do_balefire( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];
   CHAR_DATA *victim;
   char arg[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char pfile[MAX_STRING_LENGTH];
   one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Syntax: Balefire <name> \n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


   if (IS_NPC(victim))
    {
      send_to_char( "You cannot BALEFIRE mobiles!\n\r", ch);
      return;
    }

	if (get_trust(ch) <= get_trust(victim))
	{
	  send_to_char("Maybe that wasn't a good idea...\n\r",ch);
	  sprintf(buf,"%s tried to belefire you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}


   if (victim->level > 89 && (!is_name(ch->name,"Takeda") || !is_name(ch->name,"Kazmir"))) {
	send_to_char("You are not allow to balefire anyone higher than 90.\n",ch);
        return;
    }

    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );
    stop_fighting(victim,TRUE);
    sprintf( buf,"%s points down at %s{x!!",ch->name,victim->name);
	do_echo(ch,buf);
    sprintf( buf,"{wSuddenly a {yg{Ylowin{yg {wd{Wivin{we {rb{Rea{rm{x of {wl{Wigh{wt{x descends from the {ch{Ceaven{cs{x and strikes %s.{x",victim->name);
	do_echo( ch,buf);
    sprintf( buf,"{w%s suddenly {rb{Rurn{rs{x into {da{Dshe{ds{x and is no more.{x",victim->name);
	do_echo(ch,buf);
    sprintf(buf,"***********You been deleted by %s.***********\n\r",ch->name);
        send_to_char(buf,victim);
    do_save(victim,"");
    do_purge(ch,victim->name);
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );
    sprintf(pfile,"/home/fate/.ftpbak/ss/Backup/balefire/%s.bak",capitalize( victim->name ) );
    rename(strsave,pfile);
   /* unlink(strsave); */
    return;
}

void do_openvlist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *buffer;
    int loop = 1, x, lowvnum = 0;
    bool found = FALSE;
    bool havehighvnum = FALSE;
    AREA_DATA *pArea;

    buffer = new_buf();

    for (x = 1; x <= MAX_VNUM ; x++)
    {
     int count = 0;
        havehighvnum = FALSE;

        for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
        {
           if (x <= pArea->max_vnum && x >= pArea->min_vnum)
           {
               count++;                 /* count adds 1 if vnum in area */
               havehighvnum = TRUE;     /* Might be the high vnum, trap it */
           }
        }

        if (x == MAX_VNUM && count == 0)  /* Lame hack to catch last vnum */
            havehighvnum = TRUE;

        if (count == 0 && lowvnum == 0)   /* no areas found in and no low */
            lowvnum = x;                  /* vnum yet, assign this one */

        if (lowvnum > 0 && havehighvnum)  /* Have low and high vnum now */
        {
            sprintf(buf, "[%d] Unassigned block of vnums from %d to %d\n\r"
                    ,loop, lowvnum, x - 1);
            add_buf(buffer,buf);
            lowvnum = 0;                  /* Reset low vnum */
            loop++;
            found = TRUE;
        }
    }

    if (!found)
        send_to_char("No free range of vnums was found.\n\r",ch);
    else
        page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

  return;

}

void do_illegal (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if( arg[0] == '\0' )
    {
        list_illegal( ch );
        return;
    }

    if ( check_illegal( arg ) )
        remove_illegal( ch, arg );
    else
        add_illegal( ch, arg );

    return;
}

void add_illegal (CHAR_DATA *ch, char *name)
{
    char buf[MAX_STRING_LENGTH];
    FILE *fp;

    fclose( fpReserve );
    if ( ( fp = fopen( ILLEGAL_FILE, "a" ) ) == NULL )
    {
        send_to_char("File Error.\r\n", ch);
        fpReserve = fopen( NULL_FILE, "r" );
        return;
    }
    else
    {
        fprintf( fp, "%s\n", name );
        sprintf(buf, "%s is now an illegal name.\r\n", name);
        send_to_char(buf, ch);
        fclose( fp );
        fpReserve = fopen( NULL_FILE, "r" );
    }

    return;
}

void remove_illegal (CHAR_DATA *ch, char *iname)
{
    char buf[MAX_STRING_LENGTH];
    char *name;
    FILE *fp;
    FILE *out;

    fclose( fpReserve );
    out = fopen( TEMP_FILE, "w" );
    fp =  fopen( ILLEGAL_FILE, "r" );

            while( !feof( fp ) )
            {
                name = fread_word(fp);
                fread_to_eol(fp);
                if (str_cmp( name, iname ) )
                    fprintf( out, "%s\n", name );
            }
        fclose( fp );
        fclose( out );
        fpReserve = fopen( NULL_FILE, "r" );
        unlink( ILLEGAL_FILE );
        sprintf(buf, "cp %s %s", TEMP_FILE, ILLEGAL_FILE);
        system(buf);
        sprintf(buf, "%s can now be used again.\r\n", iname);
        send_to_char(buf, ch);

    return;
}

bool check_illegal (char *iname)
{
    char *name;
    FILE *fp;
    bool found=FALSE;

    fclose( fpReserve );
    if ( ( fp = fopen( ILLEGAL_FILE, "r" ) ) == NULL )
    {
        perror( ILLEGAL_FILE );
        return FALSE;
    }
    else
    {
        if ( feof( fp ) )
        {
            fclose(fp);
            unlink( ILLEGAL_FILE );
            return FALSE;
        }
        else
        {
            while( !feof( fp ) )
            {
                name = fread_word(fp);
                fread_to_eol(fp);
                if (!str_cmp( name, iname ) )
                   found = TRUE;
            }
        }
        fclose( fp );
        fpReserve = fopen( NULL_FILE, "r" );
    }

    if ( found )
       return TRUE;
    return FALSE;
}

void list_illegal ( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char *name;
    FILE *fp;
    int col=0;

    fclose( fpReserve );
    if ( ( fp = fopen( ILLEGAL_FILE, "r" ) ) == NULL )
    {
        send_to_char("There are no illegal names.\r\n", ch);
        fpReserve = fopen( NULL_FILE, "r" );
        return;
    }
    else
    {
        if ( feof( fp ) )
        {
            fclose(fp);
            unlink( ILLEGAL_FILE );
            list_illegal( ch );
        }
        else
       {
            send_to_char("These names are Illegal:\r\n\r\n", ch);
            while( !feof( fp ) )
            {
                name = fread_word(fp);
                fread_to_eol(fp);
                sprintf( buf, "%-12s", name );
                send_to_char( buf, ch );
                if ( ++col % 6 == 0 )
                    send_to_char( "\n\r", ch );
            }
        }
        send_to_char("\r\n",ch);
        fclose( fp );
        fpReserve = fopen( NULL_FILE, "r" );
    }
    return;
}

void do_pkban( CHAR_DATA *ch, char *argument )
{
     char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "PKBan Who?\n\r", ch );
	return;
    }
    
   if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }   
	if(victim->pkban == 1)
    {
	victim->pkban = 0;
	send_to_char( "PKban removed.\n\r", ch );
	send_to_char( "PKban removed.\n\r", victim);
    }
    else
    {
	victim->pkban = 1;
	send_to_char( "PKban set.\n\r", ch );
	send_to_char( "PKban set.\n\r", victim);
    }

    return;
}

void do_tbh( CHAR_DATA *ch, char *argument )
{
     char arg[MAX_INPUT_LENGTH];
     char arg1[MAX_INPUT_LENGTH];
     CHAR_DATA *victim;
     char buf[MAX_INPUT_LENGTH];
     argument = one_argument( argument, arg );
     argument = one_argument( argument, arg1 );

  if ( (ch->tbh < 2) && !IS_IMMORTAL(ch) )
   { 
    send_to_char( "You not {wT{Dhe {wB{Dlack {wH{Dand{X leader\n\r", ch );
    return;
   }
   
  if ( (ch->tbh > 2) && !IS_IMMORTAL(ch) )
   { 
    send_to_char( "You not {wT{Dhe {wB{Dlack {wH{Dand{X leader\n\r", ch );
    return;
   }   
   
  if ( arg[0] == '\0')
    {
	send_to_char( "Syntax: tbh induct <victim>\n\r", ch );
	send_to_char( "        tbh leader <victim>\n\r", ch );
	send_to_char( "        tbh boot   <victim>\n\r", ch );
/*	send_to_char( "        tbh who     \n\r", ch ); */
	send_to_char( "TBH What Who?\n\r", ch );
	return;
    }
    
   if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }   

     
   if (!str_prefix(arg,"leader"))
     {
     if ( arg1[0] == '\0')
      do_tbh(ch,"");
     
     if(victim->tbh == 0)
        {
         sprintf(buf,"%s is not even member in {wT{Dhe {wB{Dlack {wH{Dand{X\n\r",victim->name);
         send_to_char( buf , ch);
         return;
        }
         sprintf(buf,"%s is now leader in {wT{Dhe {wB{Dlack {wH{Dand{X\n\r",victim->name);
         send_to_char( buf , ch);
         send_to_char( "You leader in {wT{Dhe {wB{Dlack {wH{Dand{X.\n\r", victim);
       victim->tbh = 2;
       return;
     }


   if (!str_prefix(arg,"boot") )
     {
     if ( arg1[0] == '\0')
      do_tbh(ch,"");
     if (victim->tbh < 1)
   	{
         sprintf(buf,"%s is not member of {wT{Dhe {wB{Dlack {wH{Dand{X\n\r",victim->name);
         send_to_char( buf , ch);
         return;
   	} 
   	 victim->tbh = 0;
         sprintf(buf,"%s is now booted from {wT{Dhe {wB{Dlack {wH{Dand{X\n\r",victim->name);
         send_to_char( buf , ch);
	 send_to_char( "You booted from {wT{Dhe {wB{Dlack {wH{Dand{X.\n\r", victim);
	 return;
     }
    

if (!str_prefix(arg,"induct"))
    {
     if ( arg1[0] == '\0')
      do_tbh(ch,"");

	if(victim->tbh < 1 )
         {
	  victim->tbh = 1;
          sprintf(buf,"%s is now member of {wT{Dhe {wB{Dlack {wH{Dand{X\n\r",victim->name);
          send_to_char( buf , ch);
	  send_to_char( "You are now member of {wT{Dhe {wB{Dlack {wH{Dand{X.\n\r", victim);
	  return;
         }
        else
        {
          sprintf(buf,"%s is all ready member of {wT{Dhe {wB{Dlack {wH{Dand{X\n\r",victim->name);
          send_to_char( buf , ch);
          return;
        }
     }
/*
if (!str_prefix(arg,"who"))
 {
  send_to_char( "{wT{Dhe {wB{Dlack {wH{Dand{X who list.\n\r", ch );	
     for(d = descriptor_list; d; d = d_next)
      {

	och = CH (d);
        d_next = d->next;
        if(och->tbh > 0 && !IS_IMMORTAL(och) )
         {
          if(och->tbh == 1)
          sprintf(buf,"Member %s\n\r",och->name);
          if(och->tbh == 2)
           sprintf(buf,"Leader %s\n\r",och->name); 
          send_to_char(buf, ch );	
         }

       }
 }
*/
  do_tbh(ch,"");
  return;
}

void do_fvlist (CHAR_DATA *ch, char *argument)
{
  int i,j;
  char arg[MAX_INPUT_LENGTH];
  char *string;
  int COLUMNS = 100;

  string = one_argument(argument,arg);
 
  if (arg[0] == '\0')
    {
      send_to_char("Syntax:\n\r",ch);
      send_to_char("  fvlist obj\n\r",ch);
      send_to_char("  fvlist mob\n\r",ch);
      send_to_char("  fvlist room\n\r",ch);
      return;
    }
  j=1;
  if (!str_cmp(arg,"obj"))
    {
      printf_to_char(ch,"{WFree {C%s{W vnum listing for area {C%s{x\n\r",arg,
		     ch->in_room->area->name);
      printf_to_char(ch,"{Y=============================================================================={C\n\r");
      for (i = ch->in_room->area->min_vnum; i <= ch->in_room->area->max_vnum; i++) {
	if (get_obj_index(i) == NULL) {
	  printf_to_char(ch,"%8d, ",i);
	  if (j == COLUMNS) {
	    send_to_char("\n\r",ch);
	    j=0;
	  }
	  j++;
	}
      }
      send_to_char("{x\n\r",ch);
      return;
    }

  if (!str_cmp(arg,"mob"))
    { 
      printf_to_char(ch,"{WFree {C%s {Wvnum listing for area {C%s{x\n\r",arg,
		     ch->in_room->area->name);
      printf_to_char(ch,"{Y=============================================================================={C\n\r");
      for (i = ch->in_room->area->min_vnum; i <= ch->in_room->area->max_vnum; i++) {
	if (get_mob_index(i) == NULL) {
	  printf_to_char(ch,"%8d, ",i);
	  if (j == COLUMNS) {
	    send_to_char("\n\r",ch);
	    j=0;
	  }
	  else j++;
	}
      }
      send_to_char("{x\n\r",ch);
      return;
    }
  if (!str_cmp(arg,"room"))
    { 
      printf_to_char(ch,"{WFree {C%s {Wvnum listing for area {C%s{x\n\r",arg,
		     ch->in_room->area->name);
      printf_to_char(ch,"{Y=============================================================================={C\n\r");
      for (i = ch->in_room->area->min_vnum; i <= ch->in_room->area->max_vnum; i++) {
	if (get_room_index(i) == NULL) {
	  printf_to_char(ch,"%8d, ",i);
	  if (j == COLUMNS) {
	    send_to_char("\n\r",ch);
	    j=0;
	  } 
	  else j++;
	}
      }
      send_to_char("{x\n\r",ch);
      return;
    }
  send_to_char("WHAT??? \n\r",ch);
  send_to_char("Syntax:\n\r",ch);
  send_to_char("  fvlist obj\n\r",ch);
  send_to_char("  fvlist mob\n\r",ch);
  send_to_char("  fvlist room\n\r",ch);
}

/* Written by takeda (takeda@mathlab.sunysb.edu) */
void do_avator( CHAR_DATA *ch, char *argument ) /* Procedure Avator */
{  /* Declaration */
    char buf[MAX_STRING_LENGTH];	/* buf */
    char arg1[MAX_INPUT_LENGTH];	/* arg1 */
    int level;				/* level */


    argument = one_argument( argument, arg1 );
/* Check statements */
   if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
        send_to_char( "Syntax: avator <level>.\n\r", ch );
        return;
    }

    if ( IS_NPC(ch) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

        if ( ( level = atoi( arg1 ) ) < 1 || level > MAX_LEVEL )
        {
       sprintf(buf, "Level must be 1 to %d.\n\r", MAX_LEVEL );
        send_to_char( buf, ch );
      return;
    }

     if(ch->trust == 0)
      {
       ch->trust = ch->level;
      } 

    if (level > ch->trust)
    {
        send_to_char( "Limited to your trust level.\n\r", ch );
        sprintf(buf, "Your Trust is %d.\n\r",ch->trust);
        send_to_char(buf,ch);
        return;
    }
     
     if (ch->trust == level)
       {
        ch->trust = 0;
       }
         
       ch->level = level;        
       
         sprintf(buf, "You did avator to level %d.\n\r",ch->level);
         send_to_char(buf,ch);
    
     save_char_obj(ch); /* save character */
return;
}

void do_omni( CHAR_DATA *ch, char *argument )

{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    BUFFER *output;
    DESCRIPTOR_DATA *d;
    int immmatch;
    int mortmatch;
    int hptemp;

 /*
  * Initalize Variables.
  */
         immmatch = 0;
         mortmatch = 0;
         buf[0] = '\0';
         output = new_buf();
 /*
  * Count and output the IMMs.
  */
        sprintf( buf, "\n\r   {M----{YImmortals{M----{x\n\r");
        add_buf(output,buf);
        sprintf( buf, "Name          Level   Wiz   Incog   [ Vnum ]\n\r");
        add_buf(output,buf);
        for ( d = descriptor_list; d != NULL; d = d->next )
        {
                  CHAR_DATA *wch;

                  if ( d->connected != CON_PLAYING && (ch->level <= 100 ))
                      
                          continue;
                  
                  wch   = ( d->original != NULL ) ? d->original :
d->character;

      if (!can_see(ch,wch))
                  continue;

                  if( wch->level <=82)
                          continue;
                  immmatch++;
                  sprintf( buf, 
"%-14s %-3d    %3d    %3d     {w%-5d{x\n\r",
                        wch->name,
                        wch->level,
                        wch->invis_level,
                        wch->incog_level,
                        wch->in_room->vnum);
                        add_buf(output,buf);
         }

 /*
  * Count and output the Morts.
  */
        sprintf( buf, " \n\r
{Y----{MMortals{Y----{x\n\r");
        add_buf(output,buf);
        sprintf( buf, "Name           Race/Class   Position        Lev   %%hp    [ Vnum ]\n\r");
        add_buf(output,buf);
        hptemp = 0;

        for ( d = descriptor_list; d != NULL; d = d->next )
        {
                  CHAR_DATA *wch;
                  char const *class;

                  if ( d->connected != CON_PLAYING )
                         continue;
                  wch   = ( d->original != NULL ) ? d->original :
d->character;
                  if(wch->level > 82)
                         continue;
                  mortmatch++;

                if ((wch->max_hit != wch->hit) && (wch->hit > 0))
                  hptemp = wch->hit * 100 /wch->max_hit;
                else if (wch->max_hit == wch->hit)
                  hptemp = 100;
                else if (wch->hit < 0)
                  hptemp = 0;

                class = class_table[wch->class].who_name;
                sprintf( buf, "%-14s {B%5s{x/{W%3s   {x%-15s %-3d {r%-5d{x  %-5d\n\r",
                wch->name,
                wch->race < MAX_PC_RACE ?
pc_race_table[wch->race].who_name : "     ",
                                         class,
                capitalize( position_table[wch->position].name),
                wch->level, hptemp, wch->in_room->vnum);
                add_buf(output,buf);
         }
/*
 * Tally the counts and send the whole list out.
 */
        sprintf( buf2, "\n\r{g IMMs found: {r%d{x\n\r", immmatch );
        add_buf(output,buf2);
        sprintf( buf2, "{gMorts found: {r%d{x\n\r", mortmatch );
        add_buf(output,buf2);
        page_to_char( buf_string(output), ch );
        free_buf(output);
        return;
}

void do_religion_follower( CHAR_DATA *ch, char *argument )
 {
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );
    
   if ( IS_NPC(ch) )
     {
      send_to_char( "NPC's don't do this!.\n\r", ch );
      return;
     }

   if ( arg1[0] == '\0')
     {
      send_to_char( "Syntax: follower induct <victim>\n\r", ch );
      send_to_char( "        follower banish <victim>\n\r", ch );
     
      if(IS_IMMORTAL(ch))
       {
        send_to_char( "        follower accept <victim>\n\r", ch );
        send_to_char( "        follower priest <victim>\n\r", ch );
       }	
       return;
     }

    if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
     {
      send_to_char( "They aren't here.\n\r", ch );
      return;
     }

    if ( IS_NPC(victim) )
     {
      send_to_char( "Not on NPC's.\n\r", ch );
      return;
     }

    if (ch == victim)
     {
      send_to_char( "AYE right away.\n\r", ch );
      return;
     }

   if(!IS_IMMORTAL(victim))
    {
     send_to_char( "You failed.\n\r", ch );
     return;
    }
  
  if (!str_prefix(arg1,"induct"))
   {
    
    if ((victim->religion > 0) && (victim->religion == ch-> religion))
     {  
      send_to_char( "He is already a member of a gods guild\n\r", ch );    
      return;
     }

    if ((victim->rank_religion > 0))
     {  
      send_to_char( "He is already a member of your gods guild\n\r", ch );    
      return;
     }
    victim->religion = ch->religion; 
    victim->rank_religion = 1;
    return;
   }
  
    if (!str_prefix(arg1,"banish"))
   {
    if ((victim->religion == ch-> religion) && (!IS_IMMORTAL(ch)) )
     {  
      send_to_char( "He not member of you gods guild\n\r", ch );    
      return;
     }

    if ((victim->rank_religion > 1) && (!IS_IMMORTAL(ch)))
     {  
      send_to_char( "Your god have accepted him\n\r", ch );    
      return;
     }    
    
    if ( !NEW_IS_AUTHORIZED( victim, auth_religion_follower ))
      NEW_REVOKE(victim, auth_religion_follower);
    victim->religion = 0;
    victim->rank_religion = 0;
    return;
   }
   
   if (!str_prefix(arg1,"accept"))
    {
     if (!IS_IMMORTAL(ch))
      {  
       return;
      }

    if ((victim->rank_religion > 1))
     {  
      send_to_char( "He is accepted\n\r", ch );    
      return;
     }    
    victim->rank_religion = 2;
    return;
   }   
   
   if (!str_prefix(arg1,"accept"))
    {
     if (!IS_IMMORTAL(ch))
      {  
       return;
      }

    if ((victim->rank_religion > 2))
     {  
      send_to_char( "He is priest\n\r", ch );    
      return;
     }    
    victim->rank_religion = 3;
    if ( !NEW_IS_AUTHORIZED( victim, auth_religion_follower ))
      NEW_AUTHORIZE(victim, auth_religion_follower);  
    return;
   }      
return;    
 }

//********
void do_mlimited( CHAR_DATA *ch, char *argument )
{

  extern int top_obj_index;
  OBJ_INDEX_DATA *obj_index;
  int	lCount = 0;
  char  buf[2*MAX_STRING_LENGTH];
  char  buf2[2*MAX_STRING_LENGTH];
  BUFFER *pbuf;
  int 	nMatch;
  int 	vnum;

  if ( argument[0] != '\0' )  {
    obj_index = get_obj_index( atoi(argument) );
    if ( obj_index == NULL )  {
      send_to_char( "Not found.\n\r", ch);
      return;
    }
    if ( obj_index->limit == -1 )  {
      send_to_char( "Thats not a limited item.\n\r", ch );
      return;
    }
    nMatch = 0;
          sprintf( buf, "%-35s Limit: %3d  Current: %3d\n\r", 
		   obj_index->short_descr, 
	           obj_index->limit, 
		   obj_index->count );
	  buf[0] = UPPER( buf[0] );
	  send_to_char( buf, ch );
	        send_to_char( buf, ch );
    send_to_char( buf, ch ); 
  return;
  }

  nMatch = 0;
  pbuf = new_buf();
  for ( vnum = 0; nMatch < top_obj_index; vnum++ )
      if ( ( obj_index = get_obj_index( vnum ) ) != NULL )
      {
        nMatch++;
	if ( obj_index->limit != -1 )  {
	  lCount++;
          sprintf( buf, "%-37s  Limit: %3d  Current: %3d\n\r", 
		   obj_index->short_descr, 
//		   obj_index->vnum,
	           obj_index->limit, 
		   obj_index->count );
	  buf[0] = UPPER( buf[0] );
	  add_buf(pbuf,buf);
	}
      }
  sprintf( buf2, "\n\r%d of %d objects are limited.\n\r", lCount, nMatch );
  add_buf(pbuf,buf2);
  page_to_char(buf_string(pbuf), ch);
  free_buf(pbuf);
  return;

}

void do_skill_info( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;
    int iClass;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Lookup which skill or spell?\n\r", ch );
	return;
    }
    
   if ( ( sn = skill_lookup( arg ) ) < 0 )
    {
     send_to_char( "No such skill or spell.\n\r", ch );
     return;
    }    
    
   send_to_char("{C-----------------------------------------------------{X\n\r",ch);
   sprintf( buf, "{GSn: {W%3d  {GSlot:{W %3d  {GSkill/spell: {W'%s'{X\n\r",
   sn, skill_table[sn].abiliti_slot, skill_table[sn].abiliti_name );
   send_to_char( buf, ch );
   send_to_char("{C-----------------------------------------------------{X\n\r",ch);
   for (iClass = 0; iClass < MAX_CLASS; iClass++)
    {
     
    if  (skill_table[sn].abiliti_level[iClass] >= 82)
     {
      sprintf( buf, "%s -> n/a\n\r",
      class_table[iClass].name);
      send_to_char( buf, ch );
     }
    else
     {
      sprintf( buf, "%s -> %d\n\r",
      class_table[iClass].name, skill_table[sn].abiliti_level[iClass] );
      send_to_char( buf, ch );
     }
   

  }
   send_to_char("{C-----------------------------------------------------{X\n\r",ch);
 return;   
}        

void do_uvlist (CHAR_DATA *ch, char *argument)
{
  int i,j;
  char arg[MAX_INPUT_LENGTH];
  char *string;
  int COLUMNS =100;

  string = one_argument(argument,arg);
 
  if (arg[0] == '\0')
    {
      send_to_char("Syntax:\n\r",ch);
      send_to_char("  uvlist obj\n\r",ch);
      send_to_char("  uvlist mob\n\r",ch);
      send_to_char("  uvlist room\n\r",ch);
      send_to_char("  uvlist mprog\n\r",ch);
      return;
    }
  j=1;
  if (!str_cmp(arg,"obj"))
    {
      printf_to_char(ch,"{WFree {C%s{W vnum listing for area {C%s{x\n\r",arg,
		     ch->in_room->area->name);
      printf_to_char(ch,"{Y=============================================================================={C\n\r");
      for (i = ch->in_room->area->min_vnum; i <= ch->in_room->area->max_vnum; i++) {
	if (get_obj_index(i) != NULL) {
	  printf_to_char(ch,"%8d, ",i);
	  if (j == COLUMNS) {
	    send_to_char("\n\r",ch);
	    j=0;
	  }
	  j++;
	}
      }
      send_to_char("{x\n\r",ch);
      return;
    }

  if (!str_cmp(arg,"mob"))
    { 
      printf_to_char(ch,"{WFree {C%s {Wvnum listing for area {C%s{x\n\r",arg,
		     ch->in_room->area->name);
      printf_to_char(ch,"{Y=============================================================================={C\n\r");
      for (i = ch->in_room->area->min_vnum; i <= ch->in_room->area->max_vnum; i++) {
	if (get_mob_index(i) != NULL) {
	  printf_to_char(ch,"%8d, ",i);
	  if (j == COLUMNS) {
	    send_to_char("\n\r",ch);
	    j=0;
	  }
	  else j++;
	}
      }
      send_to_char("{x\n\r",ch);
      return;
    }
  if (!str_cmp(arg,"room"))
    { 
      printf_to_char(ch,"{WFree {C%s {Wvnum listing for area {C%s{x\n\r",arg,
		     ch->in_room->area->name);
      printf_to_char(ch,"{Y=============================================================================={C\n\r");
      for (i = ch->in_room->area->min_vnum; i <= ch->in_room->area->max_vnum; i++) {
	if (get_room_index(i) != NULL) {
	  printf_to_char(ch,"%8d, ",i);
	  if (j == COLUMNS) {
	    send_to_char("\n\r",ch);
	    j=0;
	  } 
	  else j++;
	}
      }
      send_to_char("{x\n\r",ch);
      return;
    }

  if (!str_cmp(arg,"mprog"))
    { 
      printf_to_char(ch,"{WFree {C%s {Wvnum listing for area {C%s{x\n\r",arg,
		     ch->in_room->area->name);
      printf_to_char(ch,"{Y=============================================================================={C\n\r");
      for (i = ch->in_room->area->min_vnum; i <= ch->in_room->area->max_vnum; i++) {
//	if (get_mprog_index(i) != NULL) {
	if (get_prog_index(i,PRG_MPROG) != NULL) {

	  printf_to_char(ch,"%8d, ",i);
	  if (j == COLUMNS) {
	    send_to_char("\n\r",ch);
	    j=0;
	  } 
	  else j++;
	}
      }
      send_to_char("{x\n\r",ch);
      return;
    }


  send_to_char("WHAT??? \n\r",ch);
  send_to_char("Syntax:\n\r",ch);
  send_to_char("  uvlist obj\n\r",ch);
  send_to_char("  uvlist mob\n\r",ch);
  send_to_char("  uvlist room\n\r",ch);
  send_to_char("  uvlist mprog\n\r",ch);
}

void do_ofb( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA d;
  bool isChar = FALSE;
  char name[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
    char       arg2 [ MAX_INPUT_LENGTH ];
  int      value;

 if (argument[0] == '\0')
  {
     send_to_char( "Syntax: ofb <char> <rptoken>.\n\r", ch );
    return;
  }


  argument[0] = UPPER(argument[0]);
  argument = one_argument(argument, name);
  argument = one_argument( argument, arg2 );  

    if (arg2[0] == '\0' || !is_number( arg2 ) )
    {
     send_to_char( "Syntax: ofb <char> <rptoken>.\n\r", ch );
     return;
    }

  /* Dont want to load a second copy of a player who's allready online! */
  if ( get_char_world( ch, name ) != NULL )
  {
    send_to_char( "That person is already connected!\n\r", ch );
    return;
  }

  isChar = load_char_obj(&d, name); /* char pfile exists? */

  if (!isChar)
  {
    send_to_char("Load Who? Are you sure? I cant seem to find them.\n\r",ch);
    return;
  }
  d.character->desc     = NULL;
  d.character->next     = char_list;
  char_list             = d.character;
  d.connected           = CON_PLAYING;
  reset_char(d.character);

    value = atoi( arg2 );

    if ( value < 1 || value > 5000 )
    {
        send_to_char( "Value range is 1 to 5000.\n\r", ch );
        return;
    }

     d.character->rptoken += value;

  sprintf(buf,"You off-line bonused %s %d rptokens.\n\r",d.character->name,value);
  send_to_char(buf,ch);

  save_char_obj(d.character);
  do_quit(d.character,"");
} 
char *fgetf( char *s, int n, register FILE *iop )
{
    register int c;
    register char *cs;

    c = '\0';
    cs = s;
    while( --n > 0 && (c = getc(iop)) != EOF)
	if ((*cs++ = c) == '\0')
	    break;
    *cs = '\0';
    return((c == EOF && cs == s) ? NULL : s);
}

void do_crestored( CHAR_DATA *ch, char *argument )
{     
    char pfile[MAX_STRING_LENGTH];
    char strsave[MAX_INPUT_LENGTH];
    char pcopy[MAX_INPUT_LENGTH];
    char Tbackup[MAX_INPUT_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
     char buf[5000];
    FILE *fp;

    argument = one_argument(argument,Tbackup);

        
    if ( argument[0] == '\0' || Tbackup[0] == '\0' )
    {
     send_to_char("crestor death       <player>\n\r", ch); 
     send_to_char("crestor delete      <player>\n\r", ch); 
     send_to_char("crestor balefire    <player>\n\r", ch); 
//     send_to_char("crestor <days back> <player>\n\r", ch);           
//     send_to_char("days back 1 - 6 \n\r", ch);           
     return;
    }

    if ( get_char_world( ch, argument ) != NULL )
     {
      send_to_char( "That person is connected!\n\r", ch );
      return;
     }

     sprintf( strsave, "/home/fate/.ftpbak/ss/player/%s",capitalize( argument ) );        
     
    if ( (!str_cmp( Tbackup, "1" ))
         || (!str_cmp( Tbackup, "2" ))
         || (!str_cmp( Tbackup, "3" ))
         || (!str_cmp( Tbackup, "4" ))
         || (!str_cmp( Tbackup, "5" ))
         || (!str_cmp( Tbackup, "6" )) )
        {
          sprintf(pcopy,"tar -zxvf /home/fate/.ftpbak/ss/Backup/%s.ply.tgz %s",Tbackup,capitalize( argument));
          fp = popen( pcopy, "r" );
          fgetf( buf, 5000, fp );
          page_to_char( buf, ch );
          pclose( fp );
          sprintf(pfile,"/home/fate/.ftpbak/ss/area/%s",capitalize( argument ) );
          sprintf(buf1,"%s is restored from backup %s days back.\n\r",capitalize( argument ),Tbackup );
          if (  fopen( pfile, "r" ) != NULL )
           {
            sprintf(pcopy,"mv %s %s",pfile,strsave);
//send_to_char(pfile,ch);
//send_to_char(,ch);   
//         pclose( fp );
 //         sprintf(pcopy,"rm %s",pfile);
//            fp = popen( pcopy, "r" );
            pclose( fp );
            send_to_char(buf1,ch);
            return;
           }
          else
          send_to_char("Who? There no back of that name.\n\r",ch);                
         return;
        }
    
     if ( !str_cmp( Tbackup, "death" ) )
      {
       sprintf(pfile,"/home/fate/.ftpbak/ss/Backup/death/%s.bak",capitalize( argument ) );
       sprintf(buf1,"%s is restored from death backup.\n\r",capitalize( argument ) );
      } 

     if ( !str_cmp( Tbackup, "delete" ) )
      {
       sprintf(pfile,"/home/fate/.ftpbak/ss/Backup/delete/%s.bak",capitalize( argument ) );
       sprintf(buf1,"%s is restored from delete backup.\n\r",capitalize( argument ) );
      } 

     if ( !str_cmp( Tbackup, "balefire" ) )
      {
       sprintf(pfile,"/home/fate/.ftpbak/ss/Backup/balefire/%s.bak",capitalize( argument ) );
       sprintf(buf1,"%s is restored from balefire backup.\n\r",capitalize( argument ) );
      } 

     if (  fopen( pfile, "r" ) != NULL )
      {
        sprintf(pcopy,"cp %s %s",pfile,strsave);
        fp = popen( pcopy, "r" );
        pclose( fp );
        send_to_char(buf1,ch);
      } 
     else 
      send_to_char("Who? There no back of that name.\n\r",ch);
      

 return;
}

void do_doas (CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vic;
    DESCRIPTOR_DATA *orig;
    char arg1[MAX_STRING_LENGTH];
   
    argument = one_argument(argument, arg1);

    if ( ( vic = get_char_world( ch, arg1 ) ) == NULL )
      {
       send_to_char( "They aren't playing.\n\r", ch );
       return;
      }

    if ( (vic != ch) && (get_trust (vic) >= get_trust (ch)) )
     {
      send_to_char ("You failed.\n\r",ch);
      return;
     }

    orig = vic->desc;
        
    vic->desc = ch->desc;
    ch->desc = NULL;
       
    interpret (vic, argument);

    ch->desc = vic->desc;   
    vic->desc = orig;
   
   return;    
}

void do_mark (CHAR_DATA *ch, char *argument)
{/*
  OBJ_DATA *tattoo;
  CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
  int i;


  argument = one_argument(argument, arg1);
  argument = one_argument(argument, arg2);

  if (arg2[0] == '\0')
   {
     send_to_char( "Syntax: mark <char> <mark>.\n\r", ch );
     send_to_char( "Syntax: mark <char> remove.\n\r", ch );
     send_to_char( "mark: wisdom,justice,faith,deception,hate,nature,creation,
neutraility,war,honor,understanding,quests,law,
leadership,hope,knowledge,order,peace,chaos,death,
life,mercy,magic \n\n\r", ch );
    return;
   }

   if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


  if (IS_NPC(victim))
    {
      act("$N is too dumb to worship you!", ch, NULL, victim, TO_CHAR);
      return;
    }
 if (str_cmp(arg2, "remove"))
  for (i = 0; i < MAX_RELIGION; i++)
    {
      if (!str_cmp(arg2, religion_table[i].order))
	{
  	  tattoo = get_eq_char(victim, WEAR_TATTOO);
   	  if (tattoo != NULL)
    	    {
     	      act("$N is already branded!",
		ch, NULL, victim, TO_CHAR);
      	      act("$n tried to give you another brand but failed.",
		ch, NULL, victim, TO_VICT);
              act("$n tried to give $N another brand but failed.",
		ch, NULL, victim, TO_NOTVICT);
      	      return;
    	    }
  	  else
    	    { 
//      	      tattoo = create_object(get_obj_index(religion_table[i].vnum),60);
//       	      act("You brand $N with $p!",ch, tattoo, victim, TO_CHAR);
//      	      act("$n brands $N with $p!",ch,tattoo,victim,TO_NOTVICT);
//      	      act("$n brands you painfully with $p!",ch,tattoo,victim,TO_VICT);
//      	      obj_to_char(tattoo,victim);
      	      equip_char(victim, tattoo, WEAR_TATTOO);
	      return;
	    }
	}    
    }
  else  
   {
    tattoo = get_eq_char(victim, WEAR_TATTOO);
    if (tattoo != NULL)
     {
      extract_obj(tattoo);
      act("Through a painful process, your brand has been destroyed by $n.",
	ch, NULL, victim, TO_VICT);
      act("You remove the brand from $N.", ch, NULL, victim, TO_CHAR);
      act("$N's brand is destroyed by $n.", ch, NULL, victim, TO_NOTVICT);
     }
    else
     {
      act("$N does not bear a brand.", ch, NULL, victim, TO_CHAR);
     } 
   }
send_to_char("Complete.\n\r", ch); */
 return;    
}

void do_setup(CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];

   argument = one_argument (argument, arg);
   
   if (!arg[0])
    {
    setup_race(ch,"");
    return;
    } 

   if ( !str_cmp(arg,"race") )
    setup_race(ch,argument);
  else
   setup_race(ch,"");
return;   
}
