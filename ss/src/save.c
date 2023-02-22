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
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif

/*
 * Global variables.
 */
//bool		    PWipe = FALSE;
/* int rename(const char *oldfname, const char *newfname); */

char *print_flags(int flag)
{
    int count, pos = 0;
    static char buf[52];


    for (count = 0; count < 32;  count++)
    {
        if (IS_SET(flag,1<<count))
        {
            if (count < 26)
                buf[pos] = 'A' + count;
            else
                buf[pos] = 'a' + (count - 26);
            pos++;
        }
    }

    if (pos == 0)
    {
        buf[pos] = '0';
        pos++;
    }

    buf[pos] = '\0';

    return buf;
}


/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];



/*
 * Local functions.
 */
void	fwrite_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fwrite_obj	args( ( CHAR_DATA *ch,  OBJ_DATA  *obj,
			    FILE *fp, int iNest ) );
void	fwrite_kingdom	args( ( int kingdom, FILE *fp) );
void	fread_kingdom	args( ( int kingdom, FILE *fp) );
//void	fwrite_pet	args( ( CHAR_DATA *pet, FILE *fp) );
void	fread_char	args( ( CHAR_DATA *ch,  FILE *fp ) );
//void    fread_pet	args( ( CHAR_DATA *ch,  FILE *fp ) );
void	fread_obj	args( ( CHAR_DATA *ch,  FILE *fp ) );
/*
void pwipe(void)
{
   DESCRIPTOR_DATA *d;
   struct dirent *Dir;
   struct stat Stat;
   DIR *Directory;
   char FName[80];
   char strsave[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char pfile[MAX_STRING_LENGTH];
   
   d = new_descriptor();
   Directory=opendir(PLAYER_DIR);
   Dir=readdir(Directory);
PWipe = TRUE;
   while(Dir!=NULL)
   {
      xprintf(FName, PLAYER_DIR"%s", Dir->d_name);
      stat( FName, &Stat);
      if(S_ISREG(Stat.st_mode)) {
	if (Dir->d_name[0] >= 'A' && Dir->d_name[0] <= 'Z') {
          load_char_obj( d, Dir->d_name);
	  // wipe files older than 60 days or 2*player level days 
	  // exempt HERO+ from wipe.
	  // d->character->login was set equal to lastlogoff ("LogO" in save.c)
 	  if ( d->character->level < LEVEL_HERO &&
	      (current_time - d->character->laston) > (86400 * UMAX(60,2*d->character->level))) 
	      {
               xprintf( buf, "Autodelete: %s",FName);
	       log_string(buf);
               xprintf( strsave, "%s%s", PLAYER_DIR, capitalize( FName ) );
               xprintf(pfile,"/home/fate/.ftpbak/ss/Backup/autodelete/%s.bak",capitalize( FName ) );
               rename(strsave,pfile);s/
	    //unlink (FName); 
	  }
	}
      }
      Dir=readdir(Directory);
   }
   closedir(Directory);
PWipe = FALSE;
}                          
  */                     


/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
/* Lance: part of newly invented registration system */

bool is_allowed(char *name)
{
FILE *fp;
char buf[MAX_INPUT_LENGTH];

  if ( (fp = fopen("allowed", "r")) == NULL ) return FALSE;

  for (;;)
  {
    strcpy(buf, fread_word(fp));
    buf[0] = UPPER(buf[0]);

    if (buf[0] == '$') break;

    if (!strcmp(buf, name))
    {
	fclose(fp);
	return TRUE;
    }
  }

  fclose(fp);
  return FALSE;

}



void save_char_obj( CHAR_DATA *ch )
{
    char strsave[MAX_INPUT_LENGTH];
    FILE *fp;
/*  makes it so lev 1 cant save */
if (ch->level < 2)
    return;

if (IS_SET(ch->act, PLR_ARENA))
    return;


    if ( IS_NPC(ch) )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

#if defined(unix)
    /* create god log */
    if (IS_IMMORTAL(ch) || ch->level >= LEVEL_IMMORTAL)
    {
	fclose(fpReserve);
	xprintf(strsave, "%s%s",GOD_DIR, capitalize(ch->name));
	if ((fp = fopen(strsave,"w")) == NULL)
	{
	    bug("Save_char_obj: fopen",0);
	    perror(strsave);
 	}

	fprintf(fp,"Lev %2d Trust %2d  %s%s%s\n",
	    ch->level, get_trust(ch), 
	ch->pcdata->pretitle,ch->name, ch->pcdata->title);
	fclose( fp );
	fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug("save_char_obj: Can't open null file.", 0 );

    }
#endif

    fclose( fpReserve );
    xprintf( strsave, "%s%s", PLAYER_DIR, capitalize( ch->name ) );
    if ( ( fp = fopen( TEMP_FILE, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
	fwrite_char( ch, fp );
	if ( ch->carrying != NULL )
	    fwrite_obj( ch, ch->carrying, fp, 0 );
	/* save the pets */
	/*if (ch->pet != NULL && ch->pet->in_room == ch->in_room)
	    fwrite_pet(ch->pet,fp);*/
	fprintf( fp, "#END\n" );
    }
    fclose( fp );
    rename(TEMP_FILE,strsave);
    fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug("save_char_obj: Can't open null file.", 0 );

    return;
}




/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    AFFECT_DATA *paf;
    int sn, pos, i; /* "i" int added for board code */    

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"	);

    fprintf( fp, "Name %s~\n",	ch->name		);
    fprintf( fp, "Id   %ld\n", ch->id			);
    fprintf( fp, "LogO %ld\n",	current_time		);
    if(ch->pcdata->host != NULL)
    fprintf( fp, "Host %s~\n",	ch->pcdata->host	);
    fprintf( fp, "Vers %d\n",   8			);
    fprintf( fp, "Etho %d\n",   ch->ethos		);
    fprintf( fp, "Home %d\n",   ch->hometown		);
    fprintf( fp, "Sign %s~\n",   ch->pcdata->sign	);
    fprintf( fp, "Pks  %d\n",  ch->pcdata->pkills	);
    fprintf( fp, "Bnty %d\n",  ch->pcdata->bounty	);
    fprintf( fp, "Owed %d\n",  ch->pcdata->owed 	);
    fprintf( fp, "Cab  %d\n",   ch->kingdom		);
    fprintf( fp, "Rank %d\n",   ch->kingdom_rank	);
    fprintf( fp, "Dead %d\n",   ch->pcdata->death	);
    fprintf( fp, "TD   %d\n",   ch->pcdata->total_death );
    fprintf( fp, "TSwap   %d\n",   ch->pcdata->gainswap );
    fprintf( fp, "Ques %d\n", 	ch->quest		);
 if (ch->kincom)
        fprintf( fp, "Kincom %s\n",   print_flags(ch->kincom));

    if (ch->short_descr[0] != '\0')
      	fprintf( fp, "ShD  %s~\n",	ch->short_descr	);
    if( ch->long_descr[0] != '\0')
	fprintf( fp, "LnD  %s~\n",	ch->long_descr	);
    if (ch->description[0] != '\0')
    	fprintf( fp, "Desc %s~\n",	ch->description	);
    if (ch->immtitle != NULL)
        fprintf( fp, "Immtitle %s~\n",  ch->immtitle);
    if (ch->pcdata->background[0] != '\0')
    	fprintf( fp, "Back %s~\n",	ch->pcdata->background	);
    if (ch->prompt != NULL || !str_cmp(ch->prompt,"%1 {Rhp %2 {Bm %3 {Gmv{w "))
        fprintf( fp, "Prom %s~\n",      ch->prompt  	);
    fprintf( fp, "Race %s~\n", pc_race_table[ch->race].name );
    fprintf( fp, "Sex  %d\n",	ch->sex			);
    fprintf( fp, "Class  %s~\n",	class_table[ch->class].name		);
    fprintf( fp, "Levl %d\n",	ch->level		);
    if (ch->trust != 0)
	fprintf( fp, "Tru  %d\n",	ch->trust	);
    fprintf( fp, "Sec  %d\n",    ch->pcdata->security	);	/* OLC */
    fprintf( fp, "Plyd %d\n",
	ch->played + (int) (current_time - ch->logon)	);
    fprintf( fp, "Scro %d\n", 	ch->lines		);
    fprintf( fp, "Room %d\n",
        (  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
        && ch->was_in_room != NULL )
            ? ch->was_in_room->vnum
            : ch->in_room == NULL ? 3001 : ch->in_room->vnum );

    fprintf( fp, "HMV  %d %d %d %d %d %d\n",
	ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move );
    if (ch->gold > 0)
      fprintf( fp, "Gold %ld\n",	ch->gold		);
    else
      fprintf( fp, "Gold %d\n", 0			);
    if (ch->silver > 0)
	fprintf( fp, "Silv %ld\n",ch->silver		);
    else
	fprintf( fp, "Silv %d\n",0			);
    if (ch->pkban == 1)
    fprintf( fp, "Pkban %d\n", ch->pkban);
    if ( (ch->tbh > 0) && (ch->tbh < 3) )
    fprintf( fp, "Ptbh %d\n", ch->tbh);    
    fprintf( fp, "Banks %ld\n", ch->pcdata->bank_s );
    fprintf( fp, "Bankg %ld\n", ch->pcdata->bank_g );
    fprintf( fp, "Exp  %d\n",	ch->exp			);
    if (ch->act != 0)
	fprintf( fp, "Act  %s\n",   print_flags(ch->act));
    if (ch->affected_by != 0)
	fprintf( fp, "AfBy %s\n",   print_flags(ch->affected_by));
    if (ch->affected_by2 != 0)
	fprintf( fp, "AfBy2 %s\n", print_flags(ch->affected_by2));
    if (ch->detection != 0)
	fprintf( fp, "AfDetect %s\n", print_flags(ch->detection));
    fprintf( fp, "Comm %s\n",       print_flags(ch->comm));
    if (ch->wiznet)
    	fprintf( fp, "Wizn %s\n",   print_flags(ch->wiznet));
  if (ch->info)
        fprintf( fp, "Info %s\n",   print_flags(ch->info));
    if (ch->invis_level)
	fprintf( fp, "Invi %d\n", 	ch->invis_level	);
    if (ch->incog_level)
	fprintf(fp,"Inco %d\n",ch->incog_level);
    if (ch->lastpk)
	fprintf(fp,"LastPk %ld\n",ch->lastpk);
	else
	fprintf(fp,"LastPk 8000000\n"); // set it so it's over 24 hours
    if (ch->lastkingdom)
	fprintf(fp,"LastKi %ld\n",ch->lastkingdom);
	else
	fprintf(fp,"LastKi 8000000\n"); // set it so it's over 24 hours
    if (ch->pcdata->spouse)
    fprintf( fp, "Spouse %s~\n", ch->pcdata->spouse );	
    fprintf( fp, "Pos  %d\n",
	ch->position == POS_FIGHTING ? POS_STANDING : ch->position );
    if (ch->practice != 0)
    	fprintf( fp, "Prac %d\n",	ch->practice	);
    if (ch->train != 0)
	fprintf( fp, "Trai %d\n",	ch->train	);
    if (ch->saving_throw != 0)
	fprintf( fp, "Save  %d\n",	ch->saving_throw);
    fprintf( fp, "Alig  %d\n",	ch->alignment		);
    if (ch->hitroll != 0)
	fprintf( fp, "Hit   %d\n",	ch->hitroll	);
    if (ch->damroll != 0)
	fprintf( fp, "Dam   %d\n",	ch->damroll	);
    fprintf( fp, "ACs %d %d %d %d\n",
	ch->armor[0],ch->armor[1],ch->armor[2],ch->armor[3]);
    if (ch->wimpy !=0 )
	fprintf( fp, "Wimp  %d\n",	ch->wimpy	);
    if (ch->rptoken != 0 && ch->rptoken < 500000)
	fprintf( fp, "rpt  %d\n",	ch->rptoken); 	
if (ch->trptoken != 0 && ch->trptoken < 500000)
	fprintf( fp, "trpt  %d\n",	ch->trptoken); 		
    if (ch->msp != 0)
	fprintf( fp, "MSP  %d\n",	ch->msp		);
    fprintf( fp, "Attr %d %d %d %d %d %d %d\n",
	ch->perm_stat[STAT_STR],
	ch->perm_stat[STAT_INT],
	ch->perm_stat[STAT_WIS],
	ch->perm_stat[STAT_DEX],
	ch->perm_stat[STAT_CON],
	ch->perm_stat[STAT_AGI],
	ch->perm_stat[STAT_CHA]	 );

    fprintf (fp, "AMod %d %d %d %d %d %d %d\n",
	ch->mod_stat[STAT_STR],
	ch->mod_stat[STAT_INT],
	ch->mod_stat[STAT_WIS],
	ch->mod_stat[STAT_DEX],
	ch->mod_stat[STAT_CON],
	ch->mod_stat[STAT_AGI],
	ch->mod_stat[STAT_CHA] );

    if ( IS_NPC(ch) )
    {
	fprintf( fp, "Vnum %d\n",	ch->pIndexData->vnum	);
    }
    else
    {
	fprintf( fp, "Pass %s~\n",	ch->pcdata->pwd		);
	if(ch->pcdata->real_name != NULL)
	fprintf( fp, "Real %s~\n",	ch->pcdata->real_name	);
	if(ch->pcdata->email != NULL)
	fprintf( fp, "Email %s~\n",	ch->pcdata->email	);
	if (ch->pcdata->bamfin[0] != '\0')
	    fprintf( fp, "Bin  %s~\n",	ch->pcdata->bamfin);
	if (ch->pcdata->bamfout[0] != '\0')
		fprintf( fp, "Bout %s~\n",	ch->pcdata->bamfout);
	fprintf( fp, "Titl %s~\n",	ch->pcdata->title	);
	fprintf( fp, "Pretit %s~\n",	ch->pcdata->pretitle	);
    	fprintf( fp, "Pnts %d\n",   	ch->pcdata->points      );
	fprintf( fp, "TSex %d\n",	ch->pcdata->true_sex	);
	fprintf( fp, "LLev %d\n",	ch->pcdata->last_level	);
	if(ch->sitelock)
	fprintf( fp, "Sitelock %s~\n",  ch->sitelock		);
	fprintf( fp, "HMVP %d %d %d\n", ch->pcdata->perm_hit,
						   ch->pcdata->perm_mana,
						   ch->pcdata->perm_move);
	fprintf( fp, "Cnd4  %d %d %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2],
	    ch->pcdata->condition[3],
	    ch->pcdata->condition[4]);


  fprintf( fp, "Score %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	ch->pcdata->score[1],
	ch->pcdata->score[2],
	ch->pcdata->score[3],
	ch->pcdata->score[4],
	ch->pcdata->score[5],
	ch->pcdata->score[6],
	ch->pcdata->score[7],
	ch->pcdata->score[8],
	ch->pcdata->score[9],
	ch->pcdata->score[10],
	ch->pcdata->score[11],
	ch->pcdata->score[12],
	ch->pcdata->score[13],
	ch->pcdata->score[14],
	ch->pcdata->score[15],
	ch->pcdata->score[16],
	ch->pcdata->score[17],
	ch->pcdata->score[18],
	ch->pcdata->score[19],
	ch->pcdata->score[20],
	ch->pcdata->score[21],
	ch->pcdata->score[22],
	ch->pcdata->score[23],
	ch->pcdata->score[24],
	ch->pcdata->score[25],
	ch->pcdata->score[26],
	ch->pcdata->score[27],
	ch->pcdata->score[28],
	ch->pcdata->score[29],
	ch->pcdata->score[30],
	ch->pcdata->score[31],
	ch->pcdata->score[32]);

/*Fixed grant by Kazmir*/
  fprintf( fp, "NAuth %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n", ch->pcdata->new_authorized[0],
                                           ch->pcdata->new_authorized[1],
                                           ch->pcdata->new_authorized[2],
                                           ch->pcdata->new_authorized[3],
                                           ch->pcdata->new_authorized[4],
                                           ch->pcdata->new_authorized[5],
                                           ch->pcdata->new_authorized[6],
                                           ch->pcdata->new_authorized[7],
                                           ch->pcdata->new_authorized[8],
                                           ch->pcdata->new_authorized[9],
                                           ch->pcdata->new_authorized[10],
                                           ch->pcdata->new_authorized[11],
                                           ch->pcdata->new_authorized[12],
                                           ch->pcdata->new_authorized[13],
                                           ch->pcdata->new_authorized[14],
                                           ch->pcdata->new_authorized[15],
                                           ch->pcdata->new_authorized[16],
                                           ch->pcdata->new_authorized[17],
                                           ch->pcdata->new_authorized[18],
                                           ch->pcdata->new_authorized[19]);

/* tak added  auth */
  fprintf( fp, "Auth %ld %ld %ld\n", ch->pcdata->authorized[0],
                                           ch->pcdata->authorized[1],
                                           ch->pcdata->authorized[2]);

	  	fprintf( fp, "trophyvnum %d %d %d %d %d\n",
	  		ch->pcdata->trophyvnum[0],
	  		ch->pcdata->trophyvnum[1],
	  		ch->pcdata->trophyvnum[2],
	  		ch->pcdata->trophyvnum[3],
	  		ch->pcdata->trophyvnum[4]);

	  	fprintf( fp, "trophycount %d %d %d %d %d\n",
	  		ch->pcdata->trophycount[0],
	  		ch->pcdata->trophycount[1],
	  		ch->pcdata->trophycount[2],
	  		ch->pcdata->trophycount[3],
	  		ch->pcdata->trophycount[4]);




/*  board code added, in right place? do pfiles have to be ordered? */
 	/* Save note board status */
 	/* Save number of boards in case that number changes */
 	fprintf (fp, "Boards       %d ", MAX_BOARD);
 	for (i = 0; i < MAX_BOARD; i++)
 		fprintf (fp, "%s %ld ", boards[i].short_name, ch->pcdata->last_note[i]);
 	fprintf (fp, "\n");

	/* write alias */
        for (pos = 0; pos < MAX_ALIAS; pos++)
	{
	    if (ch->pcdata->alias[pos] == NULL
	    ||  ch->pcdata->alias_sub[pos] == NULL)
		break;

	    fprintf(fp,"Alias %s %s~\n",ch->pcdata->alias[pos],
		    ch->pcdata->alias_sub[pos]);
	}

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
	    {
		fprintf( fp, "Sk %d %d %d '%s'\n",
		    ch->pcdata->learned[sn], 
		    ch->pcdata->learned_effectiveness[sn],
    		    ch->pcdata->learned_memory[sn],
		    skill_table[sn].name );

	    }
	}
/* : quest code added */
    if (ch->questpoints != 0)
        fprintf( fp, "QuestPnts %d\n",  ch->questpoints );
    if(ch->quest_curr != 0)
	fprintf(fp, "QuestCurr %d\n", ch->quest_curr );
    if(ch->quest_accum != 0)
	fprintf(fp, "QuestAcc %d\n", ch->quest_accum );
    if (ch->nextquest != 0)
        fprintf( fp, "QuestNext %d\n",  ch->nextquest   );
    else if (ch->countdown != 0)
        fprintf( fp, "QuestNext %d\n",  10              );
}

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < 0 || paf->type>= MAX_SKILL ||
            paf->type == gsn_doppelganger)
	    continue;

	fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
	    skill_table[paf->type].name,
	    paf->where,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector
	    );
    }
   fprintf( fp ,"Relig %d\n",          ch->religion            );
   
   if (ch->nochanned!=0)
    fprintf( fp, "NoChanned %d\n", ch->nochanned); //TA nochan2

   if (ch->wantedtime!=0)
    fprintf( fp, "Wantedtime %d\n", ch->wantedtime);
    
    if (ch->ironskincount!=0)
    fprintf( fp, "Ironskincount %d\n", ch->ironskincount);
    
   if (ch->crime!=0)
    fprintf( fp, "Crime %d\n", ch->crime); //TA nochan2

/*  end */
    fprintf( fp, "End\n\n" );
    return;
}


/*  adds for clan writing */
void fwrite_kingdom( int kingdom, FILE *fp )
{
int i;
if (fp==NULL) 
  bug("Fwrite_kingdom::fp is NULL",0);

    fprintf( fp, "%s~",   	kingdom_table[kingdom].short_name	);
    fprintf( fp, "%s~",	kingdom_table[kingdom].long_name	);
    fprintf( fp, "%s~", kingdom_table[kingdom].name		);
    fprintf( fp, "%d ",   	kingdom_table[kingdom].obj_vnum	);
    fprintf( fp, "%d ",   	kingdom_table[kingdom].room_vnum	);
    fprintf( fp, "%d\n",   	kingdom_table[kingdom].gold		);

for (i=0;i <= MAX_KINGDOM_MEMBERS ; i++ )
    fprintf( fp, "%s~\n",   kingdom_table[kingdom].members[i] );
for (i=0;i < 10 ; i++ )
    fprintf( fp, "%s~\n",   kingdom_table[kingdom].rank_titles[i] );
for (i=0;i < 10 ; i++ )
    fprintf( fp, "%s~\n",   kingdom_table[kingdom].rank_titlesf[i] );
for (i=0;i < 10 ; i++ )
    fprintf( fp, "%d ",     kingdom_table[kingdom].rank_slots[i] );

fprintf(fp, "\n" );

for (i=0;i < MAX_KINGDOM-1 ; i++ )
    fprintf( fp, "%d ",   kingdom_table[kingdom].at_war_with[i] );
fprintf( fp, "%d",   kingdom_table[kingdom].at_war_with[MAX_KINGDOM-1] );
/* , last on has no space or it gets buggy readin the next string */

    return;
}
/*  end */

/* write a pet */
/*void fwrite_pet( CHAR_DATA *pet, FILE *fp)
{
    AFFECT_DATA *paf;

    fprintf(fp,"#PET\n");

    fprintf(fp,"Vnum %d\n",pet->pIndexData->vnum);

    fprintf(fp,"Name %s~\n", pet->name);
    fprintf(fp,"LogO %ld\n", current_time);
    fprintf(fp,"Cab  %d\n", pet->kingdom );
    fprintf(fp,"Rank %d\n", pet->kingdom_rank );
    if (pet->short_descr != pet->pIndexData->short_descr)
    	fprintf(fp,"ShD  %s~\n", pet->short_descr);
    if (pet->long_descr != pet->pIndexData->long_descr)
    	fprintf(fp,"LnD  %s~\n", pet->long_descr);
    if (pet->description != pet->pIndexData->description)
    	fprintf(fp,"Desc %s~\n", pet->description);
    if (pet->race != pet->pIndexData->race)
    	fprintf(fp,"Race %s~\n", race_table[pet->race].name);
    fprintf(fp,"Sex  %d\n", pet->sex);
    if (pet->level != pet->pIndexData->level)
    	fprintf(fp,"Levl %d\n", pet->level);
    fprintf(fp, "HMV  %d %d %d %d %d %d\n",
    	pet->hit, pet->max_hit, pet->mana, pet->max_mana, pet->move, pet->max_move);
    if (pet->gold > 0)
    	fprintf(fp,"Gold %ld\n",pet->gold);
    if (pet->silver > 0)
	fprintf(fp,"Silv %ld\n",pet->silver);
    if (pet->exp > 0)
    	fprintf(fp, "Exp  %d\n", pet->exp);
    if (pet->act != pet->pIndexData->act)
    	fprintf(fp, "Act  %s\n", print_flags(pet->act));
    if (pet->affected_by != pet->pIndexData->affected_by)
    	fprintf(fp, "AfBy %s\n", print_flags(pet->affected_by));
    if (pet->affected_by2 != 0)
        fprintf( fp, "AfBy2 %s\n", print_flags(pet->affected_by2));
    if (pet->detection != 0)
        fprintf( fp, "AfDetect %s\n", print_flags(pet->detection));
    if (pet->comm != 0)
    	fprintf(fp, "Comm %s\n", print_flags(pet->comm));
    fprintf(fp,"Pos  %d\n", pet->position = POS_FIGHTING ? POS_STANDING : pet->position);
    if (pet->saving_throw != 0)
    	fprintf(fp, "Save %d\n", pet->saving_throw);
    if (pet->alignment != pet->pIndexData->alignment)
    	fprintf(fp, "Alig %d\n", pet->alignment);
    if (pet->hitroll != pet->pIndexData->hitroll)
    	fprintf(fp, "Hit  %d\n", pet->hitroll);
    if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
    	fprintf(fp, "Dam  %d\n", pet->damroll);
    fprintf(fp, "ACs  %d %d %d %d\n",
    	pet->armor[0],pet->armor[1],pet->armor[2],pet->armor[3]);
    fprintf(fp, "Attr %d %d %d %d %d\n",
    	pet->perm_stat[STAT_STR], pet->perm_stat[STAT_INT],
    	pet->perm_stat[STAT_WIS], pet->perm_stat[STAT_DEX],
    	pet->perm_stat[STAT_CON]);
    fprintf(fp, "AMod %d %d %d %d %d\n",
    	pet->mod_stat[STAT_STR], pet->mod_stat[STAT_INT],
    	pet->mod_stat[STAT_WIS], pet->mod_stat[STAT_DEX],
    	pet->mod_stat[STAT_CON]);

    for ( paf = pet->affected; paf != NULL; paf = paf->next )
    {
    	if (paf->type < 0 || paf->type >= MAX_SKILL ||
          paf->type == gsn_doppelganger)
    	    continue;

    	fprintf(fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
    	    skill_table[paf->type].name,
    	    paf->where, paf->level, paf->duration, paf->modifier,paf->location,
    	    paf->bitvector);
    }

    fprintf(fp,"End\n");
    return;
}*/

/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
    int i;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */

    if ( obj->next_content != NULL )
	fwrite_obj( ch, obj->next_content, fp, iNest );

if(ch)
{

    if (obj->pIndexData->vnum == 6419 || obj->pIndexData->vnum == 146) {
	extract_obj(obj);
	return;
    }

    if(strstr(obj->pIndexData->area->builders, "Unlinked") && 
		!IS_IMMORTAL(ch)) {
	extract_obj(obj);
	return;
    }
   
    for (i=1;i < MAX_KINGDOM;i++)
      if (obj->pIndexData->vnum == kingdom_table[i].obj_vnum)
        return;

    /*
     * Castrate storage characters.
     */
/* Removed camelot.are and darkforest.are objects -Baxter */
/* this is how you get rid of object numbers on characters */
/*  wonders if this is even needed anymore
    if ((obj->pIndexData->vnum > 17299 && obj->pIndexData->vnum < 17352)
    ||  (obj->pIndexData->vnum > 2399  && obj->pIndexData->vnum < 2425 ))
    {
	extract_obj(obj);
	return;
    }
 end */
    if ((obj->item_type == ITEM_KEY && obj->value[0] == 0)
    ||  (obj->item_type == ITEM_MAP && !obj->value[0]))
    {
	extract_obj( obj );
	return;
    }

}




    fprintf( fp, "#O\n" );
    fprintf( fp, "Vnum %d\n",   obj->pIndexData->vnum        );
    if (!obj->pIndexData->new_format)
	fprintf( fp, "Oldstyle\n");
    if (obj->enchanted)
	fprintf( fp,"Enchanted\n");
    fprintf( fp, "Nest %d\n",	iNest	  	     );

    /* these data are only used if they do not match the defaults */
   if(!ch && obj->in_room)
        fprintf(fp, "Inrm %d\n", obj->in_room->vnum );
        
    if ( obj->name != obj->pIndexData->name)
    	fprintf( fp, "Name %s~\n",	obj->name		     );
    if ( obj->short_descr != obj->pIndexData->short_descr)
        fprintf( fp, "ShD  %s~\n",	obj->short_descr	     );
    if ( obj->description != obj->pIndexData->description)
        fprintf( fp, "Desc %s~\n",	obj->description	     );
    if ( obj->extra_flags != obj->pIndexData->extra_flags)
        fprintf( fp, "ExtF %d\n",	obj->extra_flags	     );
    if ( obj->wear_flags != obj->pIndexData->wear_flags)
        fprintf( fp, "WeaF %d\n",	obj->wear_flags		     );
    if ( obj->item_type != obj->pIndexData->item_type)
        fprintf( fp, "Ityp %d\n",	obj->item_type		     );
    if ( obj->weight != obj->pIndexData->weight)
        fprintf( fp, "Wt   %d\n",	obj->weight		     );
//    if ( obj->limlevel != obj->pIndexData->limlevel)
//       fprintf( fp, "LL   %d\n",       obj->limlevel );

	fprintf( fp, "Cond %d\n",	obj->condition	     ); 

    /* variable data */

    fprintf( fp, "Wear %d\n",   obj->wear_loc                );
    if (obj->level != obj->pIndexData->level)
        fprintf( fp, "Lev  %d\n",	obj->level		     );
    if (obj->timer != 0)
        fprintf( fp, "Time %d\n",	obj->timer	     );
    fprintf( fp, "Cost %d\n",	obj->cost		     );
    if (obj->value[0] != obj->pIndexData->value[0]
    ||  obj->value[1] != obj->pIndexData->value[1]
    ||  obj->value[2] != obj->pIndexData->value[2]
    ||  obj->value[3] != obj->pIndexData->value[3]
    ||  obj->value[4] != obj->pIndexData->value[4])
    	fprintf( fp, "Val  %d %d %d %d %d\n",
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3],
	    obj->value[4]	     );

    switch ( obj->item_type )
    {
    case ITEM_POTION:
    case ITEM_SCROLL:
	if ( obj->value[1] > 0 )
	{
	    fprintf( fp, "Spell 1 '%s'\n",
		skill_table[obj->value[1]].name );
	}

	if ( obj->value[2] > 0 )
	{
	    fprintf( fp, "Spell 2 '%s'\n",
		skill_table[obj->value[2]].name );
	}

	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3 '%s'\n",
		skill_table[obj->value[3]].name );
	}

	break;

    case ITEM_PILL:
    case ITEM_STAFF:
    case ITEM_WAND:
	if ( obj->value[3] > 0 )
	{
	    fprintf( fp, "Spell 3 '%s'\n",
		skill_table[obj->value[3]].name );
	}

	break;
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < 0 || paf->type >= MAX_SKILL)
	    continue;
        fprintf( fp, "Affc '%s' %3d %3d %3d %3d %3d %10d\n",
            skill_table[paf->type].name,
            paf->where,
            paf->level,
            paf->duration,
            paf->modifier,
            paf->location,
            paf->bitvector
            );
    }

    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
    {
	fprintf( fp, "ExDe %s~ %s~\n",
	    ed->keyword, ed->description );
    }

    fprintf( fp, "End\n\n" );

    if ( obj->contains != NULL )
	fwrite_obj( ch, obj->contains, fp, iNest + 1 );

    return;
}



/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )
{
    char strsave[MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *ch;
    FILE *fp;
    bool found;
    int stat;
    int i = 0;
/*    extern char * daPrompt; */

    ch = new_char();
    ch->pcdata = new_pcdata();

    d->character			= ch;
    ch->desc				= d;
    ch->name				= str_dup( name );
    ch->id				= get_pc_id();
    ch->race				= race_lookup("human");
    ch->kingdom                           = 0;
    ch->kingdom_rank                      = 0;
    ch->hometown                        = 0;
    ch->pcdata->sign                    = 0;
    ch->pcdata->pkills                  = 0;
    ch->prompt 				= str_dup("<{R%1 {M%2 {G%3>{w ");
    ch->pcdata->total_death             = 0;
    ch->pcdata->bounty			= 0;
    ch->pcdata->owed			= 0;
    ch->ethos                           = 0;
    ch->affected_by                     = 0;
    ch->affected_by2                    = 0;
    ch->ironskincount		        = 0;
    ch->wantedtime		        = 0;
   ch->pcdata->score[1]  =0;
   ch->pcdata->score[2]  =0;
   ch->pcdata->score[3]  =0;
   ch->pcdata->score[4]  =0;
   ch->pcdata->score[5]  =0;
   ch->pcdata->score[6]  =0;
   ch->pcdata->score[7]  =0;
   ch->pcdata->score[8]  =0;
   ch->pcdata->score[9]  =0;
   ch->pcdata->score[10] =0;
   ch->pcdata->score[11] =0;
   ch->pcdata->score[12] =0;
   ch->pcdata->score[13] =0;
   ch->pcdata->score[14] =0;
   ch->pcdata->score[15] =0;
   ch->pcdata->score[16] =0;
   ch->pcdata->score[17] =0;
   ch->pcdata->score[18] =0;
   ch->pcdata->score[19] =0;
   ch->pcdata->score[20] =0;
   ch->pcdata->score[21] =0;
   ch->pcdata->score[22] =0;
   ch->pcdata->score[23] =0;
   ch->pcdata->score[24] =0;
   ch->pcdata->score[25] =0;
   ch->pcdata->score[26] =0;
   ch->pcdata->score[27] =0;
   ch->pcdata->score[28] =0;
   ch->pcdata->score[29] =0;
   ch->pcdata->score[30] =0;
   ch->pcdata->score[31] =0;
   ch->pcdata->score[32] =0;
   for ( i = 0; i < 30; i++ )
    {
    	ch->pcdata->trophyvnum[i] = 0;
    	ch->pcdata->trophycount[i] = 0;
    	
    }
 
   for ( i = 0; i < MAX_SKILL; i++ )
    {
    	ch->pcdata->learned_effectiveness[i] = 0;
    	ch->pcdata->learned_memory[i] = 0;
    	
    }
 
       
       ch->nochanned           = 0; //TA nochan2
       ch->crime           = 0; //TA nochan2
       ch->anchor              = 0;
       ch->act				= PLR_NOSUMMON
   					| PLR_COLOR
   					| PLR_AUTOEXIT
   					| PLR_AUTOSAC
					| PLR_AUTOGOLD
					| PLR_AUTOSPLIT
					| PLR_AUTOASSIST;


 	/* every characters starts at default board from login.. this board
 	   should be read_level == 0 !
 	    board code added...was it even here before?? */
 	ch->pcdata->board		= &boards[DEFAULT_BOARD];

    ch->comm				= COMM_COMBINE
					| COMM_PROMPT
					| COMM_COMPACT;
    ch->msp				= 0;
    ch->invis_level                     = 0;
    ch->practice                        = 0;
    ch->train                           = 0;
    ch->hitroll                         = 0;
    ch->damroll                         = 0;
    ch->trust                           = 0;
    ch->wimpy                           = 0;
    ch->rptoken				= 0;
    ch->trptoken				= 0;
    ch->lastpk				= 800000;
    ch->lastkingdom			= 800000;
    ch->saving_throw                    = 0;
    ch->progtypes                       = 0; /* PCs don't have programs */
    ch->extracted                       = FALSE; /* check for two extracts */
    ch->pcdata->points                  = 0;

    ch->pcdata->confirm_delete		= FALSE;
    ch->pcdata->pwd			= str_dup( "" );
    ch->pcdata->bamfin			= str_dup( "" );
    ch->pcdata->bamfout			= str_dup( "" );
    ch->pcdata->title			= str_dup( "" );
    ch->pcdata->pretitle		= str_dup( "" );
    ch->pcdata->spouse       		= str_dup( "" );
    ch->religion 			= RELIGION_NONE;
    for (stat =0; stat < MAX_STATS; stat++)
	ch->perm_stat[stat]		= 13;
    ch->pcdata->condition[COND_DRUNK]	= -6;
    ch->pcdata->condition[COND_THIRST]	= 48;
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->condition[COND_HUNGER]	= 48;
    ch->pcdata->condition[COND_BLOODLUST] = 48;
    ch->pcdata->condition[COND_TOXIC]	= 0;
    ch->pcdata->security		= 0;	/* OLC */
    ch->pcdata->authorized[0]           = 0;
    ch->pcdata->authorized[1]           = 0;
    ch->pcdata->authorized[2]           = 0;
    ch->pcdata->new_authorized[0]           = 0;
    ch->pcdata->new_authorized[1]           = 0;
    ch->pcdata->new_authorized[2]           = 0;
    ch->pcdata->new_authorized[3]           = 0;
    ch->pcdata->new_authorized[4]           = 0;
    ch->pcdata->new_authorized[5]           = 0;
    ch->pcdata->new_authorized[6]           = 0;
    ch->pcdata->new_authorized[7]           = 0;
    ch->pcdata->new_authorized[8]           = 0;
    ch->pcdata->new_authorized[9]           = 0;
    ch->pcdata->new_authorized[10]           = 0;
    ch->pcdata->new_authorized[11]           = 0;
    ch->pcdata->new_authorized[12]           = 0;
    ch->pcdata->new_authorized[13]           = 0;
    ch->pcdata->new_authorized[14]           = 0;
    ch->pcdata->new_authorized[15]           = 0;
    ch->pcdata->new_authorized[16]           = 0;
    ch->pcdata->new_authorized[17]           = 0;
    ch->pcdata->new_authorized[18]           = 0;
    ch->pcdata->new_authorized[19]           = 0;

    
    ch->tbh				= 0;
    ch->riding  = FALSE;
    ch->mount   = NULL;
    ch->hunting = NULL;
    ch->nextquest = 0;
    ch->questpoints = 0;
    ch->quest_curr = 0;
    ch->quest_accum = 0;
    ch->countdown = 0;
    ch->questobj = 0;
    ch->questmob = 0;
    ch->timer   = 0;
    ch->pking	= 0;
    ch->pcdata->gainswap = 0;
    ch->pcdata->strategy = 2;
    
    AUTHORIZE(ch, auth_mortal);

    found = FALSE;
    fclose( fpReserve );

    #if defined(unix)
    /* decompress if .gz file exists */
    xprintf( strsave, "%s%s%s", PLAYER_DIR, capitalize(name),".gz");
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	fclose(fp);
	xprintf(buf,"gzip -dfq %s",strsave);
	system(buf);
    }
    #endif

    xprintf( strsave, "%s%s", PLAYER_DIR, capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYER" ) ) fread_char ( ch, fp );
//            else if ( PWipe ) return 0;
	    else if ( !str_cmp( word, "OBJECT" ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "O"      ) ) fread_obj  ( ch, fp );
//	    else if ( !str_cmp( word, "PET"    ) ) fread_pet  ( ch, fp );
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    if ( fpReserve == NULL )
      bug("load_char: Can't open null file.", 0 );


    /* initialize race */
    if (found)
    {
	if (ch->race == 0)
	    ch->race = race_lookup("human");

	ch->size = pc_race_table[ch->race].size;
	ch->dam_type = 17; /*punch */

	ch->affected_by = ch->affected_by|race_table[ch->race].aff;
	ch->affected_by2= ch->affected_by2|race_table[ch->race].aff2;
	ch->detection   = ch->detection|race_table[ch->race].detect;
	ch->imm_flags	= ch->imm_flags | race_table[ch->race].imm;
	ch->res_flags	= ch->res_flags | race_table[ch->race].res;
	ch->vuln_flags	= ch->vuln_flags | race_table[ch->race].vuln;
	ch->form	= race_table[ch->race].form;
	ch->parts	= race_table[ch->race].parts;
    }



    /* fix levels */
    if (found && ch->version < 3 && (ch->level > 35 || ch->trust > 35))
    {
	switch (ch->level)
	{
	    case(40) : ch->level = 60;	break;  /* imp -> imp */
	    case(39) : ch->level = 58; 	break;	/* god -> supreme */
	    case(38) : ch->level = 56;  break;	/* deity -> god */
	    case(37) : ch->level = 53;  break;	/* angel -> demigod */
	}

        switch (ch->trust)
        {
            case(40) : ch->trust = 60;  break;	/* imp -> imp */
            case(39) : ch->trust = 58;  break;	/* god -> supreme */
            case(38) : ch->trust = 56;  break;	/* deity -> god */
            case(37) : ch->trust = 53;  break;	/* angel -> demigod */
            case(36) : ch->trust = 51;  break;	/* hero -> hero */
        }
    }

    /* ream gold */
    if (found && ch->version < 4)
    {
	ch->gold   /= 100;
    }

    if (found && ch->version < 6 )
    {
	ch->pcdata->learned[gsn_spell_craft] = 1;
    }

    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    int count = 0;
    int lastlogoff = current_time;
    int percent;
 //if ( !PWipe )
    xprintf(buf,"Loading %s.",ch->name);
    log_string(buf);
    ch->pcdata->bank_s = 0;
    ch->pcdata->bank_g = 0;
    ch->pcdata->background =
    strdup("Character made before Background Code :)");
    ch->immtitle = str_dup( "Immortal");
    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( "Act",		ch->act,		fread_flag( fp ) );
	    KEY( "AffectedBy",	ch->affected_by,	fread_flag( fp ) );
	    KEY( "AfBy",	ch->affected_by,	fread_flag( fp ) );
	    KEY( "AfBy2",	ch->affected_by2,	fread_flag( fp ) );
	    KEY( "AfDetect",	ch->detection,		fread_flag( fp ) );
	    KEY( "Alignment",	ch->alignment,		fread_number( fp ) );
	    KEY( "Alig",	ch->alignment,		fread_number( fp ) );

	    if(!str_cmp( word, "Auth"))
            {
               ch->pcdata->authorized[0] = fread_number( fp );
               ch->pcdata->authorized[1] = fread_number( fp );
               ch->pcdata->authorized[2] = fread_number( fp );
               fMatch = TRUE;
            }

	    if (!str_cmp( word, "Alia"))
	    {
		if (count >= MAX_ALIAS)
		{
		    fread_to_eol(fp);
		    fMatch = TRUE;
		    break;
		}

		ch->pcdata->alias[count] 	= str_dup(fread_word(fp));
		ch->pcdata->alias_sub[count]	= str_dup(fread_word(fp));
		count++;
		fMatch = TRUE;
		break;
	    }

            if (!str_cmp( word, "Alias"))
            {
                if (count >= MAX_ALIAS)
                {
                    fread_to_eol(fp);
                    fMatch = TRUE;
                    break;
                }

                ch->pcdata->alias[count]        = str_dup(fread_word(fp));
                ch->pcdata->alias_sub[count]    = fread_string(fp);
                count++;
                fMatch = TRUE;
                break;
            }

	    if (!str_cmp( word, "AC") || !str_cmp(word,"Armor"))
	    {
		fread_to_eol(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!str_cmp(word,"ACs"))
	    {
		int i;

		for (i = 0; i < 4; i++)
		    ch->armor[i] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!str_cmp(word, "AffD"))
	    {
		AFFECT_DATA *paf;
		int sn;

		paf = new_affect();

		sn = skill_lookup(fread_word(fp));
		if (sn < 0)
		    bug("Fread_char: unknown skill.",0);
		else
		    paf->type = sn;

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= ch->affected;
		ch->affected	= paf;
		fMatch = TRUE;
		break;
	    }

            if (!str_cmp(word, "Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_char: unknown skill.",0);
                else
                    paf->type = sn;

                paf->where  = fread_number(fp);
                paf->level      = fread_number( fp );
                paf->duration   = fread_number( fp );
                paf->modifier   = fread_number( fp );
                paf->location   = fread_number( fp );
                paf->bitvector  = fread_number( fp );
                paf->next       = ch->affected;
                ch->affected    = paf;
                fMatch = TRUE;
                break;
            }

	  if (ch->version < 9)
	  {
	    if ( !str_cmp( word, "AttrMod"  ) || !str_cmp(word,"AMod"))
	    {
		int stat;
		for (stat = 0; stat < OLD_MAX_STATS; stat ++)
		   ch->mod_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrPerm" ) || !str_cmp(word,"Attr"))
	    {
		int stat;

		for (stat = 0; stat < OLD_MAX_STATS; stat++)
		    ch->perm_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	   }
	  else
	  {
	    if ( !str_cmp( word, "AttrMod"  ) || !str_cmp(word,"AMod"))
	    {
		int stat;
		for (stat = 0; stat < MAX_STATS; stat ++)
		   ch->mod_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrPerm" ) || !str_cmp(word,"Attr"))
	    {
		int stat;

		for (stat = 0; stat < MAX_STATS; stat++)
		    ch->perm_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	   }

	    break;

	case 'B':
	    KEY( "Back",	ch->pcdata->background,	fread_string( fp ) );  
	    KEY( "Bamfin",	ch->pcdata->bamfin,	fread_string( fp ) );
            KEY( "Banks",       ch->pcdata->bank_s,     fread_number( fp ) );
            KEY( "Bankg",       ch->pcdata->bank_g,     fread_number( fp ) );
	    KEY( "Bamfout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    KEY( "Bin",		ch->pcdata->bamfin,	fread_string( fp ) );
	    KEY( "Bout",	ch->pcdata->bamfout,	fread_string( fp ) );
	    KEY( "Bnty",        ch->pcdata->bounty,	fread_number( fp ) );

/*  board code added */
 		/* Read in board status */
 	    if (!str_cmp(word, "Boards" ))
 	    {
 			int i,num = fread_number (fp); /* number of boards saved */
 			char *boardname;

 			for (; num ; num-- ) /* for each of the board saved */
 			{
 				boardname = fread_word (fp);
 				i = board_lookup (boardname); /* find board number */

 				if (i == BOARD_NOTFOUND) /* Does board still exist ? */
 				{
 					xprintf (buf, "fread_char: %s had unknown board name: %s. Skipped.", ch->name, boardname);
 					log_string (buf);
 					fread_number (fp); /* read last_note and skip info */
 				}
 				else /* Save it */
 					ch->pcdata->last_note[i] = fread_number (fp);
 			}	 /* for */

 			fMatch = TRUE;
 	    } /* Boards */
/*  end */
        break;
        

	case 'C':
//	    KEY( "Class",	ch->class,		fread_number( fp ) );
	    KEY( "Cla",		ch->class,		fread_number( fp ) );
	    KEY( "Class",	ch->class,		class_lookup_load(fread_string( fp )) );
	    KEY( "Cab",		ch->kingdom,		fread_number( fp ) );

	    if ( !str_cmp( word, "Condition" ) || !str_cmp(word,"Cond"))
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
            if (!str_cmp(word,"Cnd"))
            {
                ch->pcdata->condition[0] = fread_number( fp );
                ch->pcdata->condition[1] = fread_number( fp );
                ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
                fMatch = TRUE;
                break;
            }
            
            if (!str_cmp(word,"Cnd4"))
            {
                ch->pcdata->condition[0] = fread_number( fp );
                ch->pcdata->condition[1] = fread_number( fp );
                ch->pcdata->condition[2] = fread_number( fp );
		ch->pcdata->condition[3] = fread_number( fp );
		ch->pcdata->condition[4] = fread_number( fp );
                fMatch = TRUE;
                break;
            }            
	    KEY("Comm",		ch->comm,		fread_flag( fp ) );

	    break;

	case 'D':
	    KEY( "Damroll",	ch->damroll,		fread_number( fp ) );
	    KEY( "Dam",		ch->damroll,		fread_number( fp ) );
	    KEY( "Description",	ch->description,	fread_string( fp ) );
	    KEY( "Desc",	ch->description,	fread_string( fp ) );
	    KEY( "Dead",	ch->pcdata->death,	fread_number( fp ) );
	    break;

	case 'E':
	    KEY( "Email",	ch->pcdata->email,	fread_string( fp ) );
	    if ( !str_cmp( word, "End" ) )
	    {
    		/* adjust hp mana move up  -- here for speed's sake */
    		percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

		percent = UMIN(percent,100);

    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))
    		{
        	    ch->hit	+= (ch->max_hit - ch->hit) * percent / 100;
        	    ch->mana    += (ch->max_mana - ch->mana) * percent / 100;
        	    ch->move    += (ch->max_move - ch->move)* percent / 100;
    		}
		return;
	    }
	    KEY( "Exp",		ch->exp,		fread_number( fp ) );
	    KEY( "Etho",	ch->ethos,		fread_number( fp ) );
	    break;

	case 'G':
	    KEY( "Gold",	ch->gold,		fread_number( fp ) );
         if ( !str_cmp( word, "Group" )  || !str_cmp(word,"Gr"))
            {
		  fread_word( fp ) ;
                fMatch = TRUE;
            }
	    break;

	case 'H':
	    KEY( "Hitroll",	ch->hitroll,		fread_number( fp ) );
	    KEY( "Hit",		ch->hitroll,		fread_number( fp ) );
	    KEY( "Home",	ch->hometown,		fread_number( fp ) );
	    KEY( "Host",	ch->pcdata->host,	fread_string( fp ) );
	    if ( !str_cmp( word, "HpManaMove" ) || !str_cmp(word,"HMV"))
	    {
		ch->hit		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		ch->mana	= fread_number( fp );
		ch->max_mana	= fread_number( fp );
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

            if ( !str_cmp( word, "HpManaMovePerm" ) || !str_cmp(word,"HMVP"))
            {
                ch->pcdata->perm_hit	= fread_number( fp );
                ch->pcdata->perm_mana   = fread_number( fp );
                ch->pcdata->perm_move   = fread_number( fp );
                fMatch = TRUE;
                break;
            }

	    break;

	case 'I':
	    KEY( "Id",		ch->id,			fread_number( fp ) );
	    KEY( "InvisLevel",	ch->invis_level,	fread_number( fp ) );
	    KEY( "Inco",	ch->incog_level,	fread_number( fp ) );
  	    KEY( "Info",        ch->info,               fread_flag( fp ));   
	    KEY( "Invi",	ch->invis_level,	fread_number( fp ) );
	    KEY( "Immtitle",    ch->immtitle,           fread_string( fp ) );
	    KEY( "Ironskincount", ch->ironskincount,    fread_number( fp ) );
	    break;

	case 'K':
	    KEY( "Kincom",	ch->kincom,  fread_flag( fp ));
	    break;

	case 'L':
	    KEY( "LastLevel",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "LastKi",	ch->lastkingdom,	fread_number( fp ) );		
	    KEY( "LastPk",	ch->lastpk, 		fread_number( fp ) );		
	    KEY( "LLev",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    KEY( "Lev",		ch->level,		fread_number( fp ) );
	    KEY( "Levl",	ch->level,		fread_number( fp ) );
	    KEY( "LogO",	ch->laston,		fread_number( fp ) );
//            ch->laston = lastlogoff;
	    KEY( "LongDescr",	ch->long_descr,		fread_string( fp ) );
	    KEY( "LnD",		ch->long_descr,		fread_string( fp ) );
	    break;

	case 'M':
	    KEY( "MSP",		ch->msp,		fread_number(fp ) );
	    break;
	case 'N':
	    KEY( "Name",	ch->name,		fread_string( fp ) );


	    if(!str_cmp( word, "NAuth"))
            {
               ch->pcdata->new_authorized[0] = fread_number( fp );
               ch->pcdata->new_authorized[1] = fread_number( fp );
               ch->pcdata->new_authorized[2] = fread_number( fp );
               ch->pcdata->new_authorized[3] = fread_number( fp );
               ch->pcdata->new_authorized[4] = fread_number( fp );
               ch->pcdata->new_authorized[5] = fread_number( fp );
               ch->pcdata->new_authorized[6] = fread_number( fp );
               ch->pcdata->new_authorized[7] = fread_number( fp );
               ch->pcdata->new_authorized[8] = fread_number( fp );
               ch->pcdata->new_authorized[9] = fread_number( fp );
               ch->pcdata->new_authorized[10] = fread_number( fp );
               ch->pcdata->new_authorized[11] = fread_number( fp );
               ch->pcdata->new_authorized[12] = fread_number( fp );
               ch->pcdata->new_authorized[13] = fread_number( fp );
               ch->pcdata->new_authorized[14] = fread_number( fp );
               ch->pcdata->new_authorized[15] = fread_number( fp );
               ch->pcdata->new_authorized[16] = fread_number( fp );
               ch->pcdata->new_authorized[17] = fread_number( fp );
               ch->pcdata->new_authorized[18] = fread_number( fp );
               ch->pcdata->new_authorized[19] = fread_number( fp );
               fMatch = TRUE;
            }
            
            KEY( "NoChanned", ch->nochanned, fread_number( fp) ); // TA nochan2
            KEY( "crime", ch->crime, fread_number( fp) );

	    break;		

	case 'O':
	    KEY( "Owed",	ch->pcdata->owed,	fread_number(fp));
	    break;		

	case 'P':
	    KEY( "Password",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Pass",	ch->pcdata->pwd,	fread_string( fp ) );
	    KEY( "Played",	ch->played,		fread_number( fp ) );
	    KEY( "Plyd",	ch->played,		fread_number( fp ) );
	    KEY( "Points",	ch->pcdata->points,	fread_number( fp ) );
	    KEY( "Pnts",	ch->pcdata->points,	fread_number( fp ) );
	    KEY( "Position",	ch->position,		fread_number( fp ) );
	    KEY( "Pos",		ch->position,		fread_number( fp ) );
	    KEY( "Practice",	ch->practice,		fread_number( fp ) );
	    KEY( "Prac",	ch->practice,		fread_number( fp ) );
            KEY( "Prompt",      ch->prompt,             fread_string( fp ) );
 	    KEY( "Prom",	ch->prompt,		fread_string( fp ) );
	    KEY( "Pkban",	ch->pkban,		fread_number( fp ) );
	    KEY( "Ptbh",	ch->tbh,		fread_number( fp ) );
	    KEY( "Pks",        ch->pcdata->pkills,	fread_number( fp ) );
	  if ( !str_cmp( word, "Pretit" )  || !str_cmp( word, "Pretitl"))
            {
                ch->pcdata->pretitle = fread_string( fp );
      if (ch->pcdata->pretitle[0] != '.' && ch->pcdata->pretitle[0] != ','
      &&  ch->pcdata->pretitle[0] != '!' && ch->pcdata->pretitle[0] != '?')
                {
                    xprintf( buf, "%s", ch->pcdata->pretitle );
                    free_string( ch->pcdata->pretitle );
                    ch->pcdata->pretitle = str_dup( buf );
                }
                fMatch = TRUE;
                break;
            }

	    break;
	case 'Q':
	    KEY( "Ques",	ch->quest, fread_number( fp ) );
            KEY( "QuestPnts",   ch->questpoints, fread_number( fp) );
            KEY( "QuestNext",   ch->nextquest,fread_number( fp ) );
            KEY( "QuestCurr",   ch->quest_curr,fread_number( fp ) );
            KEY( "QuestAcc",   ch->quest_accum,fread_number( fp ) );
            break;

	case 'R':
	    KEY( "Relig",       ch->religion,           fread_number( fp) );
	    KEY( "Race",        ch->race,
				race_lookup(fread_string( fp )) );
	    KEY( "Rank",	ch->kingdom_rank,fread_number( fp ) );
	    KEY( "Real",	ch->pcdata->real_name,	fread_string( fp ) );
	    KEY( "rpt",	ch->rptoken,		fread_number( fp ) );
	    KEY( "trpt",	ch->trptoken,		fread_number( fp ) );
	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'S':
	    KEY( "SavingThrow",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Save",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Scro",	ch->lines,		fread_number( fp ) );
	    KEY( "Sec",         ch->pcdata->security,	fread_number( fp ) );	/* OLC */
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    KEY( "ShortDescr",	ch->short_descr,	fread_string( fp ) );
	    KEY( "ShD",		ch->short_descr,	fread_string( fp ) );
 	    KEY( "Sign",        ch->pcdata->sign,	fread_string( fp ) );
            KEY( "Silv",        ch->silver,             fread_number( fp ) );
	    KEY( "Sitelock",	ch->sitelock,		fread_string( fp ) );
	    KEY( "Spouse",	ch->pcdata->spouse,	fread_string( fp ) );

	    if(!str_cmp( word, "Score")) 
		{
    ch->pcdata->score[1]  =fread_number( fp );
    ch->pcdata->score[2]  =fread_number( fp );
    ch->pcdata->score[3]  =fread_number( fp );
    ch->pcdata->score[4]  =fread_number( fp );
    ch->pcdata->score[5]  =fread_number( fp );
    ch->pcdata->score[6]  =fread_number( fp );
    ch->pcdata->score[7]  =fread_number( fp );
    ch->pcdata->score[8]  =fread_number( fp );
    ch->pcdata->score[9]  =fread_number( fp );
    ch->pcdata->score[10] =fread_number( fp );
    ch->pcdata->score[11] =fread_number( fp );
    ch->pcdata->score[12] =fread_number( fp );
    ch->pcdata->score[13] =fread_number( fp );
    ch->pcdata->score[14] =fread_number( fp );
    ch->pcdata->score[15] =fread_number( fp );
    ch->pcdata->score[16] =fread_number( fp );
    ch->pcdata->score[17] =fread_number( fp );
    ch->pcdata->score[18] =fread_number( fp );
    ch->pcdata->score[19] =fread_number( fp );
    ch->pcdata->score[20] =fread_number( fp );
    ch->pcdata->score[21] =fread_number( fp );
    ch->pcdata->score[22] =fread_number( fp );
    ch->pcdata->score[23] =fread_number( fp );
    ch->pcdata->score[24] =fread_number( fp );
    ch->pcdata->score[25] =fread_number( fp );
    ch->pcdata->score[26] =fread_number( fp );
    ch->pcdata->score[27] =fread_number( fp );
    ch->pcdata->score[28] =fread_number( fp );
    ch->pcdata->score[29] =fread_number( fp );
    ch->pcdata->score[30] =fread_number( fp );
    ch->pcdata->score[31] =fread_number( fp );
    ch->pcdata->score[32] =fread_number( fp );

		}

	    if ( !str_cmp( word, "Skill" ) || !str_cmp(word,"Sk"))
	    {
		int sn;
		int value,value1,value2;
		char *temp;

		if (ch->version < 8)
		 {
			value = fread_number( fp );
			temp = fread_word( fp ) ;
			sn = skill_lookup(temp);
			/* sn    = skill_lookup( fread_word( fp ) ); */
			if ( sn < 0 )
			  {
		    		fprintf(stderr,"%s",temp);
		    		bug( "Fread_char: unknown skill. ", 0 );
			  }
			else
		    		ch->pcdata->learned[sn] = value;
		  }
		 else
		  {
			value = fread_number( fp );
			value1 = fread_number( fp );
			value2 = fread_number( fp );
			temp = fread_word( fp ) ;
			sn = skill_lookup(temp);
			/* sn    = skill_lookup( fread_word( fp ) ); */
			if ( sn < 0 )
			  {
		    		fprintf(stderr,"%s",temp);
		    		bug( "Fread_char: unknown skill. ", 0 );
			  }
			else
		    		ch->pcdata->learned[sn] = value;
		    		ch->pcdata->learned_effectiveness[sn] = value1;
    				ch->pcdata->learned_memory[sn] = value2;
		  }
		fMatch = TRUE;
	    }

	    break;

	case 'T':
            KEY( "TrueSex",     ch->pcdata->true_sex,  	fread_number( fp ) );
	    KEY( "TSex",	ch->pcdata->true_sex,   fread_number( fp ) );
	    KEY( "Trai",	ch->train,		fread_number( fp ) );
	    KEY( "Trust",	ch->trust,		fread_number( fp ) );
	    KEY( "Tru",		ch->trust,		fread_number( fp ) );
	    KEY( "TD",	ch->pcdata->total_death,	fread_number(fp));
            KEY( "Tswap",	ch->pcdata->gainswap,	fread_number(fp));
	    if ( !str_cmp( word, "Title" )  || !str_cmp( word, "Titl"))
	    {
		  ch->pcdata->title = fread_string( fp );
    		if (ch->pcdata->title[0] != '.' && ch->pcdata->title[0] != ','
		&&  ch->pcdata->title[0] != '!' && ch->pcdata->title[0] != '?')
		{
		    xprintf( buf, " %s", ch->pcdata->title );
		    free_string( ch->pcdata->title );
		    ch->pcdata->title = str_dup( buf );
		}

		fMatch = TRUE;
		break;
	    }

	    if(!str_cmp( word, "trophyvnum"))
            {
               ch->pcdata->trophyvnum[0] = fread_number( fp );
               ch->pcdata->trophyvnum[1] = fread_number( fp );
               ch->pcdata->trophyvnum[2] = fread_number( fp );
               ch->pcdata->trophyvnum[3] = fread_number( fp );
               ch->pcdata->trophyvnum[4] = fread_number( fp );
            }

if(!str_cmp( word, "trophycount"))
            {
               ch->pcdata->trophycount[0] = fread_number( fp );
               ch->pcdata->trophycount[1] = fread_number( fp );
               ch->pcdata->trophycount[2] = fread_number( fp );
               ch->pcdata->trophycount[3] = fread_number( fp );
               ch->pcdata->trophycount[4] = fread_number( fp );
            }

	    break;

	case 'V':
	    KEY( "Version",     ch->version,		fread_number ( fp ) );
	    KEY( "Vers",	ch->version,		fread_number ( fp ) );
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "Wimpy",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wimp",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wizn",	ch->wiznet,		fread_flag( fp ) );
	    KEY( "Wantedtime", 	ch->wantedtime,		fread_number( fp) );
	    break;
	}

	if ( !fMatch )
	{
	    bug( "Fread_char: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}

/*  adds reading of KINGDOM_DATA */
void fread_kingdom( int kingdom, FILE *fp)
{
int i;

kingdom_table[kingdom].short_name		= fread_string( fp );
//log_string("                   (short_name)");
kingdom_table[kingdom].long_name		= fread_string( fp );
//log_string("                   (long_name)");
kingdom_table[kingdom].name			= fread_string( fp );
//log_string("		       (name)");
kingdom_table[kingdom].obj_vnum		= fread_number( fp );
//log_string("                   (obj_vnum)");
kingdom_table[kingdom].room_vnum		= fread_number( fp );
//log_string("                   (room_vnum)");
kingdom_table[kingdom].gold			= fread_number( fp );
//log_string("                   (gold)");

for (i=0;i <= MAX_KINGDOM_MEMBERS ; i++ ) 
	   kingdom_table[kingdom].members[i]=fread_string( fp );
//log_string("                   (members)");

for (i=0;i < 10 ; i++ ) 
	   kingdom_table[kingdom].rank_titles[i]=fread_string( fp );
//log_string("                   (rank_titles)");

for (i=0;i < 10 ; i++ ) 
	   kingdom_table[kingdom].rank_titlesf[i]=fread_string( fp );
//log_string("                   (female_rank_titles)");

for (i=0;i < 10 ; i++ ) 
	   kingdom_table[kingdom].rank_slots[i]=fread_number( fp );
//log_string("                   (rank_slots)");

for (i=0;i < MAX_KINGDOM ; i++ ) 
	   kingdom_table[kingdom].at_war_with[i]=fread_number( fp );
//log_string("                   (at_war_with)");

return;
}


/* load a pet from the forgotten reaches */
/*void fread_pet( CHAR_DATA *ch, FILE *fp )
{
    char *word;
    CHAR_DATA *pet;
    bool fMatch;
    int lastlogoff = current_time;
    int percent;*/

    /* first entry had BETTER be the vnum or we barf */
/*    word = feof(fp) ? "END" : fread_word(fp);
    if (!str_cmp(word,"Vnum"))
    {
    	int vnum;

    	vnum = fread_number(fp);
    	if (get_mob_index(vnum) == NULL)
	{
    	    bug("Fread_pet: bad vnum %d.",vnum);
	    pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
	}
    	else
    	    pet = create_mobile(get_mob_index(vnum));
    }
    else
    {
        bug("Fread_pet: no vnum in file.",0);
        pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
    }

    for ( ; ; )
    {
    	word 	= feof(fp) ? "END" : fread_word(fp);
    	fMatch = FALSE;

    	switch (UPPER(word[0]))
    	{
    	case '*':
    	    fMatch = TRUE;
    	    fread_to_eol(fp);
    	    break;

    	case 'A':
    	    KEY( "Act",		pet->act,		fread_flag(fp));
    	    KEY( "AfBy",	pet->affected_by,	fread_flag(fp));
	    KEY( "AfBy2",       ch->affected_by2,       fread_flag( fp ) );
            KEY( "AfDetect",    ch->detection,          fread_flag( fp ) );
    	    KEY( "Alig",	pet->alignment,		fread_number(fp));

    	    if (!str_cmp(word,"ACs"))
    	    {
    	    	int i;

    	    	for (i = 0; i < 4; i++)
    	    	    pet->armor[i] = fread_number(fp);
    	    	fMatch = TRUE;
    	    	break;
    	    }

    	    if (!str_cmp(word,"AffD"))
    	    {
    	    	AFFECT_DATA *paf;
    	    	int sn;

    	    	paf = new_affect();

    	    	sn = skill_lookup(fread_word(fp));
    	     	if (sn < 0)
    	     	    bug("Fread_char: unknown skill.",0);
    	     	else
    	     	   paf->type = sn;

    	     	paf->level	= fread_number(fp);
    	     	paf->duration	= fread_number(fp);
    	     	paf->modifier	= fread_number(fp);
    	     	paf->location	= fread_number(fp);
    	     	paf->bitvector	= fread_number(fp);
    	     	paf->next	= pet->affected;
    	     	pet->affected	= paf;
    	     	fMatch		= TRUE;
    	     	break;
    	    }

            if (!str_cmp(word,"Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_char: unknown skill.",0);
                else
                   paf->type = sn;

		paf->where	= fread_number(fp);
                paf->level      = fread_number(fp);
                paf->duration   = fread_number(fp);
                paf->modifier   = fread_number(fp);
                paf->location   = fread_number(fp);
                paf->bitvector  = fread_number(fp);
                paf->next       = pet->affected;
                pet->affected   = paf;
                fMatch          = TRUE;
                break;
            }

    	    if (!str_cmp(word,"AMod"))
    	    {
    	     	int stat;

    	     	for (stat = 0; stat < MAX_STATS; stat++)
    	     	    pet->mod_stat[stat] = fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	    }

    	    if (!str_cmp(word,"Attr"))
    	    {
    	         int stat;

    	         for (stat = 0; stat < MAX_STATS; stat++)
    	             pet->perm_stat[stat] = fread_number(fp);
    	         fMatch = TRUE;
    	         break;
    	    }
    	    break;

    	 case 'C':
             KEY( "Cab",	pet->kingdom,             fread_number(fp));
    	     KEY( "Comm",	pet->comm,		fread_flag(fp));
    	     break;

    	 case 'D':
    	     KEY( "Dam",	pet->damroll,		fread_number(fp));
    	     KEY( "Desc",	pet->description,	fread_string(fp));
    	     break;

    	 case 'E':
    	     if (!str_cmp(word,"End"))
	     {
		pet->leader = ch;
		pet->master = ch;
		ch->pet = pet;*/
    		/* adjust hp mana move up  -- here for speed's sake */
    	/*	percent = (current_time - lastlogoff) * 25 / ( 2 * 60 * 60);

    		if (percent > 0 && !IS_AFFECTED(ch,AFF_POISON)
    		&&  !IS_AFFECTED(ch,AFF_PLAGUE))
    		{
		    percent = UMIN(percent,100);
    		    pet->hit	+= (pet->max_hit - pet->hit) * percent / 100;
        	    pet->mana   += (pet->max_mana - pet->mana) * percent / 100;
        	    pet->move   += (pet->max_move - pet->move)* percent / 100;
    		}
    	     	return;
	     }
    	     KEY( "Exp",	pet->exp,		fread_number(fp));
    	     break;

    	 case 'G':
    	     KEY( "Gold",	pet->gold,		fread_number(fp));
    	     break;

    	 case 'H':
    	     KEY( "Hit",	pet->hitroll,		fread_number(fp));

    	     if (!str_cmp(word,"HMV"))
    	     {
    	     	pet->hit	= fread_number(fp);
    	     	pet->max_hit	= fread_number(fp);
    	     	pet->mana	= fread_number(fp);
    	     	pet->max_mana	= fread_number(fp);
    	     	pet->move	= fread_number(fp);
    	     	pet->max_move	= fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	     }
    	     break;

     	case 'L':
    	     KEY( "Levl",	pet->level,		fread_number(fp));
    	     KEY( "LnD",	pet->long_descr,	fread_string(fp));
	     KEY( "LogO",	lastlogoff,		fread_number(fp));
    	     break;

    	case 'N':
    	     KEY( "Name",	pet->name,		fread_string(fp));
    	     break;

    	case 'P':
    	     KEY( "Pos",	pet->position,		fread_number(fp));
    	     break;

	case 'R':
    	    KEY( "Race",	pet->race, race_lookup(fread_string(fp)));
    	    KEY( "Rank",	pet->kingdom_rank,	fread_number(fp));
    	    break;

    	case 'S' :
    	    KEY( "Save",	pet->saving_throw,	fread_number(fp));
    	    KEY( "Sex",		pet->sex,		fread_number(fp));
    	    KEY( "ShD",		pet->short_descr,	fread_string(fp));
            KEY( "Silv",        pet->silver,            fread_number( fp ) );
    	    break;

    	if ( !fMatch )
    	{
    	    bug("Fread_pet: no match.",0);
    	    fread_to_eol(fp);
    	}

    	}
    }
}*/

extern	OBJ_DATA	*obj_free;

void fread_obj( CHAR_DATA *ch, FILE *fp )
{
    OBJ_DATA *obj;
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    bool first;
    bool new_format;  /* to prevent errors */
    bool make_new;    /* update object */

    fVnum = FALSE;
    obj = NULL;
    first = TRUE;  /* used to counter fp offset */
    new_format = FALSE;
    make_new = FALSE;

    word   = feof( fp ) ? "End" : fread_word( fp );
    if (!str_cmp(word,"Vnum" ))
    {
        int vnum;
	first = FALSE;  /* fp will be in right place */

        vnum = fread_number( fp );
        if (  get_obj_index( vnum )  == NULL )
	{
            bug( "Fread_obj: bad vnum %d.", vnum );
	}
        else
	{
	    obj = create_object_nocount(get_obj_index(vnum),-1);
	    new_format = TRUE;
	}

    }

    if (obj == NULL)  /* either not found or old style */
    {
    	obj = new_obj();
    	obj->name		= str_dup( "" );
    	obj->short_descr	= str_dup( "" );
    	obj->description	= str_dup( "" );
    }

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	if (first)
	    first = FALSE;
	else
	    word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if (!str_cmp(word,"AffD"))
	    {
		AFFECT_DATA *paf;
		int sn;

		paf = new_affect();

		sn = skill_lookup(fread_word(fp));
		if (sn < 0)
		    bug("Fread_obj: unknown skill.",0);
		else
		    paf->type = sn;

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->next	= obj->affected;
		obj->affected	= paf;
		fMatch		= TRUE;
		break;
	    }
            if (!str_cmp(word,"Affc"))
            {
                AFFECT_DATA *paf;
                int sn;

                paf = new_affect();

                sn = skill_lookup(fread_word(fp));
                if (sn < 0)
                    bug("Fread_obj: unknown skill.",0);
                else
                    paf->type = sn;

		paf->where	= fread_number( fp );
                paf->level      = fread_number( fp );
                paf->duration   = fread_number( fp );
                paf->modifier   = fread_number( fp );
                paf->location   = fread_number( fp );
                paf->bitvector  = fread_number( fp );
                paf->next       = obj->affected;
                obj->affected   = paf;
                fMatch          = TRUE;
                break;
            }
	    break;

	case 'C':
	    KEY( "Cond",	obj->condition,		fread_number( fp ));
	    KEY( "Cost",	obj->cost,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Description",	obj->description,	fread_string( fp ) );
	    KEY( "Desc",	obj->description,	fread_string( fp ) );
	    break;

	case 'E':

	    if ( !str_cmp( word, "Enchanted"))
	    {
		obj->enchanted = TRUE;
	 	fMatch 	= TRUE;
		break;
	    }

	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
	    KEY( "ExtF",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe"))
	    {
		EXTRA_DESCR_DATA *ed;

		ed = new_extra_descr();

		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || ( fVnum && obj->pIndexData == NULL ) )
		{
		    bug( "Fread_obj: incomplete object.", 0 );
		    free_obj(obj);
		    return;
		}
		else
	        {
		    if ( !fVnum )
		    {
			free_obj( obj );
			obj = create_object( get_obj_index( OBJ_VNUM_DUMMY ), 0 );
		    }

		    if (!new_format)
		    {
		    	obj->next	= object_list;
		    	object_list	= obj;
		    	obj->pIndexData->count++;
		    }

		    if (!obj->pIndexData->new_format
		    && obj->item_type == ITEM_ARMOR
		    &&  obj->value[1] == 0)
		    {
			obj->value[1] = obj->value[0];
			obj->value[2] = obj->value[0];
		    }
		    if (make_new)
		    {
			int wear;

			wear = obj->wear_loc;
			extract_obj(obj);

			obj = create_object(obj->pIndexData,0);
			obj->wear_loc = wear;
		    }

//		    if ( iNest == 0 || rgObjNest[iNest] == NULL )
//			obj_to_char( obj, ch );
//		    else
//			obj_to_obj( obj, rgObjNest[iNest-1] );
			if ( iNest == 0 || rgObjNest[iNest] == NULL )
			  {   
			  	if(ch)
					obj_to_char( obj, ch );
				else
					obj_to_room(obj, obj->in_room );
			  }
			else
				obj_to_obj( obj, rgObjNest[iNest-1] );
		    return;
		}
	    }
	    break;

	case 'I':

	if(!str_cmp(word, "Inrm" ) )
	  {  
	  	 obj->in_room = get_room_index(fread_number(fp) );
		fMatch = TRUE;
		break;
	  }

	    KEY( "ItemType",	obj->item_type,		fread_number( fp ) );
	    KEY( "Ityp",	obj->item_type,		fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "Level",	obj->level,		fread_number( fp ) );
	    KEY( "Lev",		obj->level,		fread_number( fp ) );
	    break;

	case 'N':
	    KEY( "Name",	obj->name,		fread_string( fp ) );

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_obj: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    break;

   	case 'O':
	    if ( !str_cmp( word,"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL && obj->pIndexData->new_format)
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    break;

	case 'R':
	    if ( !str_cmp( word,"Repo" ) )
	    fread_number( fp ); 
	break;

	case 'S':
	    KEY( "ShortDescr",	obj->short_descr,	fread_string( fp ) );
	    KEY( "ShD",		obj->short_descr,	fread_string( fp ) );

	    if ( !str_cmp( word, "Spell" ) )
	    {
		int iValue;
		int sn;

		iValue = fread_number( fp );
		sn     = skill_lookup( fread_word( fp ) );
		if ( iValue < 0 || iValue > 4 )
		{
		    bug( "Fread_obj: bad iValue %d.", iValue );
		}
		else if ( sn < 0 )
		{
		    bug( "Fread_obj: unknown skill.", 0 );
		}
		else
		{
		    obj->value[iValue] = sn;
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Values" ) || !str_cmp(word,"Vals"))
	    {
		obj->value[0]	= fread_number( fp );
		obj->value[1]	= fread_number( fp );
		obj->value[2]	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		if (obj->item_type == ITEM_WEAPON && obj->value[0] == 0)
		   obj->value[0] = obj->pIndexData->value[0];
		fMatch		= TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Val" ) )
	    {
		obj->value[0] 	= fread_number( fp );
	 	obj->value[1]	= fread_number( fp );
	 	obj->value[2] 	= fread_number( fp );
		obj->value[3]	= fread_number( fp );
		obj->value[4]	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Vnum" ) )
	    {
		int vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( "Fread_obj: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "WearFlags",	obj->wear_flags,	fread_number( fp ) );
	    KEY( "WeaF",	obj->wear_flags,	fread_number( fp ) );
	    KEY( "WearLoc",	obj->wear_loc,		fread_number( fp ) );
	    KEY( "Wear",	obj->wear_loc,		fread_number( fp ) );
	    KEY( "Weight",	obj->weight,		fread_number( fp ) );
	    KEY( "Wt",		obj->weight,		fread_number( fp ) );
	    break;

	}

	if ( !fMatch )
	{
	    bug( "Fread_obj: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}
