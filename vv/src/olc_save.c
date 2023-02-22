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

/**************************************************************************
 *  File: olc_save.c                                                       *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/
/* OLC_SAVE.C
 * This takes care of saving all the .are information.
 * Notes:
 * -If a good syntax checker is used for setting vnum ranges of areas
 *  then it would become possible to just cycle through vnums instead
 *  of using the iHash stuff and checking that the room or reset or
 *  mob etc is part of that area.
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "tables.h"
#include "olc.h"
#include "fok_struct.h"

#define DIF(a,b) (~((~a)|(b)))

/*
 *  Verbose writes reset data in plain english into the comments
 *  section of the resets.  It makes areas considerably larger but
 *  may aid in debugging.
 */

#define VERBOSE 

/*****************************************************************************
 Name:		fix_string
 Purpose:	Returns a string without \r and ~.
 ****************************************************************************/
char *fix_string( const char *str )
{
    static char strfix[MAX_STRING_LENGTH * 2];
    int i;
    int o;

    if ( str == NULL )
        return '\0';

    for ( o = i = 0; str[i+o] != '\0'; i++ )
    {
        if (str[i+o] == '\r' || str[i+o] == '~')
            o++;
        strfix[i] = str[i+o];
    }
    strfix[i] = '\0';
    return strfix;
}



/*****************************************************************************
 Name:		save_area_list
 Purpose:	Saves the listing of files to be loaded at startup.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area_list()
{
    FILE *fp;
    AREA_DATA *pArea;

    if ( ( fp = fopen( AREA_LIST, "w" ) ) == NULL )
    {
	bug( "Save_area_list: fopen", 0 );
	perror( AREA_LIST );
    }
    else
    {
	/*
	 * Add any help files that need to be loaded at
	 * startup to this section.
	 */
	fprintf( fp, "fate.are\n"   );  
//	fprintf( fp, "social.are\n" );    /* ROM OLC */


	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    fprintf( fp, "%s\n", pArea->file_name );
	}

	fprintf( fp, "prog.are\n"  );    /* , will this work? */

	fprintf( fp, "$\n" );
	fclose( fp );
    }

    return;
}


/*
 * ROM OLC
 * Used in save_mobile and save_object below.  Writes
 * flags on the form fread_flag reads.
 * 
 * buf[] must hold at least 32+1 characters.
 *
 * -- Hugin
 */
char *fwrite_flag( long flags, char buf[] )
{
    char offset;
    char *cp;

    buf[0] = '\0';

    if ( flags == 0 )
    {
	strcpy( buf, "0" );
	return buf;
    }

    /* 32 -- number of bits in a long */

    for ( offset = 0, cp = buf; offset < 32; offset++ )
	if ( flags & ( (long)1 << offset ) )
	{
	    if ( offset <= 'Z' - 'A' )
		*(cp++) = 'A' + offset;
	    else
		*(cp++) = 'a' + offset - ( 'Z' - 'A' + 1 );
	}

    *cp = '\0';

    return buf;
}

void save_mobprogs( FILE *fp, AREA_DATA *pArea )
{
//        MPROG_CODE *pMprog;
        PROG_CODE *pMprog;
        int i;

        fprintf(fp, "#MOBPROGS\n");

        for( i = pArea->min_vnum; i <= pArea->max_vnum; i++ )
        {
//          if ( (pMprog = get_mprog_index(i) ) != NULL)
          if ( (pMprog = get_prog_index(i,PRG_MPROG) ) != NULL)          
                {
                          fprintf(fp, "#%d\n", i);
                   fprintf(fp, "%s~\n", fix_string(pMprog->code));
                }
        }

        fprintf(fp,"#0\n\n");
        return;
}



/*****************************************************************************
 Name:		save_mobile
 Purpose:	Save one mobile to file, new format -- Hugin
 Called by:	save_mobiles (below).
 ****************************************************************************/
void save_mobile( FILE *fp, MOB_INDEX_DATA *pMobIndex )
{
    int race = pMobIndex->race;
//    MPROG_LIST *pMprog;    
    PROG_LIST *pMprog;    
    char buf[MAX_STRING_LENGTH];
    long temp;

    fprintf( fp, "#%d\n",         pMobIndex->vnum );
    fprintf( fp, "%s~\n",         pMobIndex->player_name );
    fprintf( fp, "%s~\n",         pMobIndex->short_descr );
    fprintf( fp, "%s~\n",         fix_string( pMobIndex->long_descr ) );
    fprintf( fp, "%s~\n",         fix_string( pMobIndex->description) );
    fprintf( fp, "%s~\n",         race_table[race].name );
    fprintf( fp, "%d\n",         pMobIndex->class );    
    fprintf( fp, "%s ",	          fwrite_flag( pMobIndex->act,         buf ) );
    fprintf( fp, "%s ",	          fwrite_flag( pMobIndex->affected_by, buf ) );
    fprintf( fp, "%s ",		  fwrite_flag( pMobIndex->affected_by2,buf ) );
    fprintf( fp, "%s ",		  fwrite_flag( pMobIndex->detection, buf ) );
    fprintf( fp, "%d %d\n",        pMobIndex->alignment , pMobIndex->group);
    fprintf( fp, "%d ",	          pMobIndex->level );
    fprintf( fp, "%d ",	          pMobIndex->hitroll );
    fprintf( fp, "%dd%d+%d ",     pMobIndex->hit[DICE_NUMBER], 
	     	     	          pMobIndex->hit[DICE_TYPE], 
	     	     	          pMobIndex->hit[DICE_BONUS] );
    fprintf( fp, "%dd%d+%d ",     pMobIndex->mana[DICE_NUMBER], 
	     	     	          pMobIndex->mana[DICE_TYPE], 
	     	     	          pMobIndex->mana[DICE_BONUS] );
    fprintf( fp, "%dd%d+%d ",     pMobIndex->damage[DICE_NUMBER], 
	     	     	          pMobIndex->damage[DICE_TYPE], 
	     	     	          pMobIndex->damage[DICE_BONUS] );
    fprintf( fp, "%s\n",          attack_table[pMobIndex->dam_type].name );
    fprintf( fp, "%d %d %d %d\n", pMobIndex->ac[AC_PIERCE] / 10, 
	     	     	          pMobIndex->ac[AC_BASH]   / 10, 
	     	     	          pMobIndex->ac[AC_SLASH]  / 10, 
	     	     	          pMobIndex->ac[AC_EXOTIC] / 10 );
    fprintf( fp, "%s ",           fwrite_flag( pMobIndex->off_flags,  buf ) );
    fprintf( fp, "%s ",	          fwrite_flag( pMobIndex->imm_flags,  buf ) );
    fprintf( fp, "%s ",           fwrite_flag( pMobIndex->res_flags,  buf ) );
    fprintf( fp, "%s\n",          fwrite_flag( pMobIndex->vuln_flags, buf ) );
    fprintf( fp, "%s %s %s %ld\n",
	                          position_table[pMobIndex->start_pos].short_name,
	         	     	  position_table[pMobIndex->default_pos].short_name,
	         	     	  sex_table[pMobIndex->sex].name,
	         	     	  pMobIndex->wealth );
    fprintf( fp, "%s ",           fwrite_flag( pMobIndex->form,  buf ) );
    fprintf( fp, "%s ",      	  fwrite_flag( pMobIndex->parts, buf ) );

/*Res/imm*/
    fprintf (fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        pMobIndex->resistance[0],
        pMobIndex->resistance[1],
        pMobIndex->resistance[2],
        pMobIndex->resistance[3],
        pMobIndex->resistance[4],
        pMobIndex->resistance[5],
        pMobIndex->resistance[6],
        pMobIndex->resistance[7],
        pMobIndex->resistance[8],
        pMobIndex->resistance[9],
        pMobIndex->resistance[10],
        pMobIndex->resistance[11],
        pMobIndex->resistance[12],
        pMobIndex->resistance[13],
        pMobIndex->resistance[14],
        pMobIndex->resistance[15],
        pMobIndex->resistance[16],
        pMobIndex->resistance[17],
        pMobIndex->resistance[18],
        pMobIndex->resistance[19],
        pMobIndex->resistance[20],
        pMobIndex->resistance[21],
        pMobIndex->resistance[22]);

    fprintf (fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        pMobIndex->immuniti[0],
        pMobIndex->immuniti[1],
        pMobIndex->immuniti[2],
        pMobIndex->immuniti[3],
        pMobIndex->immuniti[4],
        pMobIndex->immuniti[5],
        pMobIndex->immuniti[6],
        pMobIndex->immuniti[7],
        pMobIndex->immuniti[8],
        pMobIndex->immuniti[9],
        pMobIndex->immuniti[10],
        pMobIndex->immuniti[11],
        pMobIndex->immuniti[12],
        pMobIndex->immuniti[13],
        pMobIndex->immuniti[14],
        pMobIndex->immuniti[15],
        pMobIndex->immuniti[16],
        pMobIndex->immuniti[17],
        pMobIndex->immuniti[18],
        pMobIndex->immuniti[19],
        pMobIndex->immuniti[20],
        pMobIndex->immuniti[21],
        pMobIndex->immuniti[22]);
/*Res/imm*/            
    
    fprintf( fp, "%s ",           size_table[pMobIndex->size].name );
    fprintf( fp, "%d ",           pMobIndex->nrattack);
  fprintf( fp, "%s\n",        IS_NULLSTR(pMobIndex->material) ? pMobIndex->material : "unknown" );
    if ((temp = DIF(race_table[race].act,pMobIndex->act)))
     	fprintf( fp, "F act %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].aff,pMobIndex->affected_by)))
     	fprintf( fp, "F aff %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].aff2,pMobIndex->affected_by2)))
	fprintf( fp, "F aff2 %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].detect,pMobIndex->detection)))
	fprintf( fp, "F detect %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].off,pMobIndex->off_flags)))
     	fprintf( fp, "F off %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].imm,pMobIndex->imm_flags)))
     	fprintf( fp, "F imm %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].res,pMobIndex->res_flags)))
     	fprintf( fp, "F res %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].vuln,pMobIndex->vuln_flags)))
     	fprintf( fp, "F vul %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].form,pMobIndex->form)))
     	fprintf( fp, "F for %s\n", fwrite_flag(temp, buf) );

    if ((temp = DIF(race_table[race].parts,pMobIndex->parts)))
    	fprintf( fp, "F par %s\n", fwrite_flag(temp, buf) );

  for (pMprog = pMobIndex->mprogs; pMprog; pMprog = pMprog->next)
    {
        fprintf(fp, "M %s %d %s~\n",
//        mprog_type_to_name(pMprog->trig_type), pMprog->vnum,
        prog_type_to_name(pMprog->trig_type), pMprog->vnum,
                pMprog->trig_phrase);
    }

    return;
}
void save_objprogs( FILE *fp, AREA_DATA *pArea )
{
	PROG_CODE *pOprog;
        int i;

        fprintf(fp, "#OBJPROGS\n");

	for( i = pArea->min_vnum; i <= pArea->max_vnum; i++ )
        {
          if ( (pOprog = get_prog_index(i, PRG_OPROG) ) != NULL)
		{
		          fprintf(fp, "#%d\n", i);
		          fprintf(fp, "%s~\n", fix_string(pOprog->code));
		}
        }

        fprintf(fp,"#0\n\n");
        return;
}

void save_roomprogs( FILE *fp, AREA_DATA *pArea )
{
	PROG_CODE *pRprog;
        int i;

        fprintf(fp, "#ROOMPROGS\n");

	for( i = pArea->min_vnum; i <= pArea->max_vnum; i++ )
        {
          if ( (pRprog = get_prog_index(i,PRG_RPROG) ) != NULL)
		{
		          fprintf(fp, "#%d\n", i);
		          fprintf(fp, "%s~\n", fix_string(pRprog->code));
		}
        }

        fprintf(fp,"#0\n\n");
        return;
}

/*****************************************************************************
 Name:		save_mobiles
 Purpose:	Save #MOBILES secion of an area file.
 Called by:	save_area(olc_save.c).
 Notes:         Changed for ROM OLC.
 ****************************************************************************/
void save_mobiles( FILE *fp, AREA_DATA *pArea )
{
    int i;
    MOB_INDEX_DATA *pMob;

    fprintf( fp, "#MOBILES\n" );

    for( i = pArea->min_vnum; i <= pArea->max_vnum; i++ )
    {
	if ( (pMob = get_mob_index( i )) )
	    save_mobile( fp, pMob );
    }

    fprintf( fp, "#0\n\n\n\n" );
    return;
}





/*****************************************************************************
 Name:		save_object
 Purpose:	Save one object to file.
                new ROM format saving -- Hugin
 Called by:	save_objects (below).
 ****************************************************************************/
void save_object( FILE *fp, OBJ_INDEX_DATA *pObjIndex )
{
//    char letter;
    AFFECT_DATA *pAf;
    EXTRA_DESCR_DATA *pEd;
    char buf[MAX_STRING_LENGTH];
    PROG_LIST *pOprog;

    fprintf( fp, "#%d\n",    pObjIndex->vnum );
    fprintf( fp, "%s~\n",    pObjIndex->name );
    fprintf( fp, "%s~\n",    pObjIndex->short_descr );
    fprintf( fp, "%s~\n",    fix_string( pObjIndex->description ) );
    fprintf( fp, "%s~\n",    pObjIndex->material );
    fprintf( fp, "%s ",      item_name(pObjIndex->item_type));
    fprintf( fp, "%s ",	     fwrite_flag( pObjIndex->class_flags, buf ));
    fprintf( fp, "%s\n",     fwrite_flag( pObjIndex->race_flags, buf ));
    fprintf( fp, "%s ",      fwrite_flag( pObjIndex->extra_flags, buf ) );
    fprintf( fp, "%s\n",     fwrite_flag( pObjIndex->wear_flags,  buf ) );

/*
 *  Using fwrite_flag to write most values gives a strange
 *  looking area file, consider making a case for each
 *  item type later.
 */

    switch ( pObjIndex->item_type )
    {
        default:
	    fprintf( fp, "%s ",  fwrite_flag( pObjIndex->value[0], buf ) );
	    fprintf( fp, "%s ",  fwrite_flag( pObjIndex->value[1], buf ) );
	    fprintf( fp, "%s ",  fwrite_flag( pObjIndex->value[2], buf ) );
	    fprintf( fp, "%s ",  fwrite_flag( pObjIndex->value[3], buf ) );
	    fprintf( fp, "%s\n", fwrite_flag( pObjIndex->value[4], buf ) );
	    break;

        case ITEM_LIGHT:
	    fprintf( fp, "0 0 %d 0 0\n",
		     pObjIndex->value[2] < 1 ? 999  /* infinite */
		     : pObjIndex->value[2] );
	    break;

        case ITEM_MONEY:
            fprintf( fp, "%d %d 0 0 0\n",
                     pObjIndex->value[0],
                     pObjIndex->value[1]);
            break;
            
        case ITEM_DRINK_CON:
            fprintf( fp, "%d %d '%s' %d 0\n",
                     pObjIndex->value[0],
                     pObjIndex->value[1],
                     liq_table[pObjIndex->value[2]].liq_name,
/*                   fwrite_flag( pObjIndex->value[3], buf ) ); */
		     pObjIndex->value[3]);
            break;
                    
	case ITEM_FOUNTAIN:
	    fprintf( fp, "%d %d '%s' 0 0\n",
	             pObjIndex->value[0],
	             pObjIndex->value[1],
	             liq_table[pObjIndex->value[2]].liq_name);
	    break;
	    
        case ITEM_CONTAINER:
            fprintf( fp, "%d %s %d %d %d\n",
                     pObjIndex->value[0],
                     fwrite_flag( pObjIndex->value[1], buf ),
                     pObjIndex->value[2],
                     pObjIndex->value[3],
                     pObjIndex->value[4]);
            break;
            
        case ITEM_FOOD:
            fprintf( fp, "%d %d 0 %s 0\n",
                     pObjIndex->value[0],
                     pObjIndex->value[1],
                     fwrite_flag( pObjIndex->value[3], buf ) );
            break;
            
        case ITEM_PORTAL:
            fprintf( fp, "%d %s %s %d 0\n",
                     pObjIndex->value[0],
                     fwrite_flag( pObjIndex->value[1], buf ),
                     fwrite_flag( pObjIndex->value[2], buf ),
                     pObjIndex->value[3]);
            break;
            
        case ITEM_FURNITURE:
            fprintf( fp, "%d %d %s %d %d\n",
                     pObjIndex->value[0],
                     pObjIndex->value[1],
                     fwrite_flag( pObjIndex->value[2], buf),
                     pObjIndex->value[3],
                     pObjIndex->value[4]);
            break;
            
        case ITEM_WEAPON:
            fprintf( fp, "%s %d %d %s %s\n",
                     weapon_name(pObjIndex->value[0]),
                     pObjIndex->value[1],
                     pObjIndex->value[2],
                     attack_table[pObjIndex->value[3]].name,
                     fwrite_flag( pObjIndex->value[4], buf ) );
            break;

        case ITEM_TRAP:
            fprintf( fp, "%s %d %d %d 0\n",
                     fwrite_flag( pObjIndex->value[0], buf ),
                     pObjIndex->value[1],
                     pObjIndex->value[3], /* reads back in as 2 in db.c */
		     pObjIndex->value[3] );
            break;
            
        case ITEM_ARMOR:
            fprintf( fp, "%d %d %d %d %d\n",
                     pObjIndex->value[0],
                     pObjIndex->value[1],
                     pObjIndex->value[2],
                     pObjIndex->value[3],
                     pObjIndex->value[4]);
            break;
            
        case ITEM_PILL:
        case ITEM_POTION:
        case ITEM_SCROLL:
	    fprintf( fp, "%d '%s' '%s' '%s' '%s'\n",
		     pObjIndex->value[0] > 0 ? /* no negative numbers */
		     pObjIndex->value[0]
		     : 0,
		     pObjIndex->value[1] != -1 ?
		     skill_table[pObjIndex->value[1]].abiliti_name
		     : "",
		     pObjIndex->value[2] != -1 ?
		     skill_table[pObjIndex->value[2]].abiliti_name
		     : "",
		     pObjIndex->value[3] != -1 ?
		     skill_table[pObjIndex->value[3]].abiliti_name
		     : "",
		     pObjIndex->value[4] != -1 ?
		     skill_table[pObjIndex->value[4]].abiliti_name
		     : "");
	    break;

        case ITEM_STAFF:
        case ITEM_WAND:
	    fprintf( fp, "%d ", pObjIndex->value[0] );
	    fprintf( fp, "%d ", pObjIndex->value[1] );
	    fprintf( fp, "%d '%s' 0\n",
		     pObjIndex->value[2],
		     pObjIndex->value[3] != -1 ?
		       skill_table[pObjIndex->value[3]].abiliti_name
		       : 0 );
	    break;
    }


    fprintf( fp, "%d ", pObjIndex->level );
    fprintf( fp, "%d ", pObjIndex->weight );
    fprintf( fp, "%d ", pObjIndex->cost );
    fprintf( fp, "%d ", pObjIndex->condition );
    fprintf( fp, "%d ", pObjIndex->repop );
    fprintf (fp, "%d ",pObjIndex->crepop);
    fprintf (fp, "%d\n",pObjIndex->size);
/*Res/imm*/
    fprintf (fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        pObjIndex->resistance[0],
        pObjIndex->resistance[1],
        pObjIndex->resistance[2],
        pObjIndex->resistance[3],
        pObjIndex->resistance[4],
        pObjIndex->resistance[5],
        pObjIndex->resistance[6],
        pObjIndex->resistance[7],
        pObjIndex->resistance[8],
        pObjIndex->resistance[9],
        pObjIndex->resistance[10],
        pObjIndex->resistance[11],
        pObjIndex->resistance[12],
        pObjIndex->resistance[13],
        pObjIndex->resistance[14],
        pObjIndex->resistance[15],
        pObjIndex->resistance[16],
        pObjIndex->resistance[17],
        pObjIndex->resistance[18],
        pObjIndex->resistance[19],
        pObjIndex->resistance[20],
        pObjIndex->resistance[21],
        pObjIndex->resistance[22]);

    fprintf (fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        pObjIndex->immuniti[0],
        pObjIndex->immuniti[1],
        pObjIndex->immuniti[2],
        pObjIndex->immuniti[3],
        pObjIndex->immuniti[4],
        pObjIndex->immuniti[5],
        pObjIndex->immuniti[6],
        pObjIndex->immuniti[7],
        pObjIndex->immuniti[8],
        pObjIndex->immuniti[9],
        pObjIndex->immuniti[10],
        pObjIndex->immuniti[11],
        pObjIndex->immuniti[12],
        pObjIndex->immuniti[13],
        pObjIndex->immuniti[14],
        pObjIndex->immuniti[15],
        pObjIndex->immuniti[16],
        pObjIndex->immuniti[17],
        pObjIndex->immuniti[18],
        pObjIndex->immuniti[19],
        pObjIndex->immuniti[20],
        pObjIndex->immuniti[21],
        pObjIndex->immuniti[22]);
/*Res/imm*/        

/*
         if ( pObjIndex->condition >  90 ) letter = 'P';
    else if ( pObjIndex->condition >  75 ) letter = 'G';
    else if ( pObjIndex->condition >  50 ) letter = 'A';
    else if ( pObjIndex->condition >  25 ) letter = 'W';
    else if ( pObjIndex->condition >  10 ) letter = 'D';
    else if ( pObjIndex->condition >   0 ) letter = 'B';
    else                                   letter = 'R';

    fprintf( fp, "%c\n", letter ); 

         if ( pObjIndex->repop >  90 ) letter = 'P';
    else if ( pObjIndex->repop >  75 ) letter = 'G';
    else if ( pObjIndex->repop >  50 ) letter = 'A';
    else if ( pObjIndex->repop >  25 ) letter = 'W';
    else if ( pObjIndex->repop >  10 ) letter = 'D';
    else if ( pObjIndex->repop >   0 ) letter = 'B';
    else                               letter = 'R';

    fprintf( fp, "%c\n", letter );
*/
    for( pAf = pObjIndex->affected; pAf; pAf = pAf->next )
    {
	if (pAf->where == TO_OBJECT || pAf->bitvector == 0)
	        fprintf( fp, "A\n%d %d\n",  pAf->location, pAf->modifier );
	else
	{
		fprintf( fp, "F\n" );

		switch(pAf->where)
		{
			case TO_AFFECTS:
				fprintf( fp, "A " );
				break;
			case TO_AFFECTS2:
				fprintf( fp, "Z " );
				break;
			case TO_DETECTS:
				fprintf( fp, "D " );
				break;
			case TO_IMMUNE:
				fprintf( fp, "I " );
				break;
			case TO_RESIST:
				fprintf( fp, "R " );
				break;
			case TO_VULN:
				fprintf( fp, "V " );
				break;
			default:
				bug( "olc_save: Invalid Affect->where", 0);
				break;
		}
		
		fprintf( fp, "%d %d %s\n", pAf->location, pAf->modifier,
				fwrite_flag( pAf->bitvector, buf ) );
	}
    }

    for( pEd = pObjIndex->extra_descr; pEd; pEd = pEd->next )
    {
        fprintf( fp, "E\n%s~\n%s~\n", pEd->keyword,
		 fix_string( pEd->description ) );
    }
    for (pOprog = pObjIndex->oprogs; pOprog; pOprog = pOprog->next)
    {
        fprintf(fp, "O %s %d %s~\n",
        prog_type_to_name(pOprog->trig_type), pOprog->vnum,
                pOprog->trig_phrase);
    }


    return;
}
 



/*****************************************************************************
 Name:		save_objects
 Purpose:	Save #OBJECTS section of an area file.
 Called by:	save_area(olc_save.c).
 Notes:         Changed for ROM OLC.
 ****************************************************************************/
void save_objects( FILE *fp, AREA_DATA *pArea )
{
    int i;
    OBJ_INDEX_DATA *pObj;

    fprintf( fp, "#OBJECTS\n" );

    for( i = pArea->min_vnum; i <= pArea->max_vnum; i++ )
    {
	if ( (pObj = get_obj_index( i )) )
	    save_object( fp, pObj );
    }

    fprintf( fp, "#0\n\n\n\n" );
    return;
}
 




/*****************************************************************************
 Name:		save_rooms
 Purpose:	Save #ROOMS section of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_rooms( FILE *fp, AREA_DATA *pArea )
{
    ROOM_INDEX_DATA *pRoomIndex;
    EXTRA_DESCR_DATA *pEd;
    EXIT_DATA *pExit;
    int iHash;
    int door;
    PROG_LIST *pRprog;

    fprintf( fp, "#ROOMS\n" );
    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pRoomIndex = room_index_hash[iHash]; pRoomIndex; pRoomIndex = pRoomIndex->next )
        {
            if ( pRoomIndex->area == pArea )
            {
                fprintf( fp, "#%d\n",		pRoomIndex->vnum );
                fprintf( fp, "%s~\n",		pRoomIndex->name );
                fprintf( fp, "%s~\n",		fix_string( pRoomIndex->description ) );
		fprintf( fp, "0 " );
                fprintf( fp, "%d ",		pRoomIndex->room_flags );
                fprintf( fp, "%d\n",		pRoomIndex->sector_type );

                for ( pEd = pRoomIndex->extra_descr; pEd;
                      pEd = pEd->next )
                {
                    fprintf( fp, "E\n%s~\n%s~\n", pEd->keyword,
                                                  fix_string( pEd->description ) );
                }
                for( door = 0; door < MAX_DIR; door++ )	/* I hate this! */
                {
                    if ( ( pExit = pRoomIndex->exit[door] )
                          && pExit->u1.to_room )
                    {
			int locks = 0;
			if ( IS_SET( pExit->rs_flags, EX_ISDOOR ) 
			&& ( !IS_SET( pExit->rs_flags, EX_PICKPROOF ) ) 
		    	&& ( !IS_SET( pExit->rs_flags, EX_NOPASS ) ) )
			    locks = 1;
			if ( IS_SET( pExit->rs_flags, EX_ISDOOR )
			&& ( IS_SET( pExit->rs_flags, EX_PICKPROOF ) )
		        && ( !IS_SET( pExit->rs_flags, EX_NOPASS ) ) )
			    locks = 2;
			if ( IS_SET( pExit->rs_flags, EX_ISDOOR )
			&& ( !IS_SET( pExit->rs_flags, EX_PICKPROOF ) )
		        && ( IS_SET( pExit->rs_flags, EX_NOPASS ) ) )
			    locks = 3;
			if ( IS_SET( pExit->rs_flags, EX_ISDOOR )
			&& ( IS_SET( pExit->rs_flags, EX_PICKPROOF ) )
		        && ( IS_SET( pExit->rs_flags, EX_NOPASS ) ) )
			    locks = 4;
			
                        fprintf( fp, "D%d\n",      pExit->orig_door );
                        fprintf( fp, "%s~\n",      fix_string( pExit->description ) );
                        fprintf( fp, "%s~\n",      pExit->keyword );
                        fprintf( fp, "%d %d %d\n", locks,
                                                   pExit->key,
                                                   pExit->u1.to_room->vnum );
                    }
                }
		if (pRoomIndex->mana_rate != 100 || pRoomIndex->heal_rate != 100)
		 fprintf ( fp, "M %d H %d\n",pRoomIndex->mana_rate,
		                             pRoomIndex->heal_rate);
/*  removes clan shit
		if (pRoomIndex->clan > 0)
		 fprintf ( fp, "C %s~\n" , clan_table[pRoomIndex->clan].name );
 */		 			     
        if (!IS_NULLSTR(pRoomIndex->owner))
                 fprintf ( fp, "O %s~\n" , pRoomIndex->owner );

for (pRprog = pRoomIndex->rprogs; pRprog; pRprog = pRprog->next)
{
	 fprintf(fp, "R %s %d %s~\n",
	 prog_type_to_name(pRprog->trig_type), pRprog->vnum,
	 pRprog->trig_phrase);
}

		fprintf( fp, "S\n" );
            }
        }
    }
    fprintf( fp, "#0\n\n\n\n" );
    return;
}



/*****************************************************************************
 Name:		save_specials
 Purpose:	Save #SPECIALS section of area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_specials( FILE *fp, AREA_DATA *pArea )
{
    int iHash;
    MOB_INDEX_DATA *pMobIndex;
    
    fprintf( fp, "#SPECIALS\n" );

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pMobIndex = mob_index_hash[iHash]; pMobIndex; pMobIndex = pMobIndex->next )
        {
            if ( pMobIndex && pMobIndex->area == pArea && pMobIndex->spec_fun )
            {
#if defined( VERBOSE )
                fprintf( fp, "M %d %s Load to: %s\n", pMobIndex->vnum,
                                                      spec_name( pMobIndex->spec_fun ),
                                                      pMobIndex->short_descr );
#else
                fprintf( fp, "M %d %s\n", pMobIndex->vnum,
                              spec_name( pMobIndex->spec_fun ) );
#endif
            }
        }
    }

    fprintf( fp, "S\n\n\n\n" );
    return;
}

/*****************************************************************************
 Name:          save_practicer
 Purpose:       Save #PRACTICERS section of area file.
 Called by:     save_area(olc_save.c).
****************************************************************************/
void save_practicer( FILE *fp, AREA_DATA *pArea )
{
    int iHash;
    MOB_INDEX_DATA *pMobIndex;

    fprintf( fp, "#PRACTICERS\n" );

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for( pMobIndex = mob_index_hash[iHash]; pMobIndex; pMobIndex = pMobIndex->next )
        {
            if ( pMobIndex && pMobIndex->area == pArea && pMobIndex->practicer )
            {

                fprintf( fp, "M %d %s   *  %s\n", pMobIndex->vnum,
                        prac_name(pMobIndex->practicer),
			pMobIndex->short_descr );
            }
        }
    }

    fprintf( fp, "S\n\n\n\n" );
    return;
}

/*****************************************************************************
 Name:          save_olimit
 Purpose:       Save #OLIMITS section of area file.
 Called by:     save_area(olc_save.c).
****************************************************************************/

void save_olimit( FILE *fp, AREA_DATA *pArea )
{
    int iHash;
    OBJ_INDEX_DATA *pObjIndex;

    fprintf( fp, "#OLIMITS\n" );

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pObjIndex = obj_index_hash[iHash]; pObjIndex; pObjIndex = pObjIndex->next )
        {
            if ( pObjIndex && pObjIndex->area == pArea && pObjIndex->limit )
            {
	/* Added so no such thing as a -1 limit */
	     if ( pObjIndex->limit > -1) {
      		fprintf( fp, "O %d %d   *  %s\n", pObjIndex->vnum,
                        pObjIndex->limit,
                        pObjIndex->short_descr );
		}
            }
        }
    }

    fprintf( fp, "S\n\n\n\n" );
    return;
}


/*
 * This function is obsolete.  It it not needed but has been left here
 * for historical reasons.  It is used currently for the same reason.
 *
 * I don't think it's obsolete in ROM -- Hugin.
 */
void save_door_resets( FILE *fp, AREA_DATA *pArea )
{
    int iHash;
    ROOM_INDEX_DATA *pRoomIndex;
    EXIT_DATA *pExit;
    int door;

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pRoomIndex = room_index_hash[iHash]; pRoomIndex; pRoomIndex = pRoomIndex->next )
        {
            if ( pRoomIndex->area == pArea )
            {
                for( door = 0; door < MAX_DIR; door++ )
                {
                    if ( ( pExit = pRoomIndex->exit[door] )
                          && pExit->u1.to_room 
                          && ( IS_SET( pExit->rs_flags, EX_CLOSED )
                          || IS_SET( pExit->rs_flags, EX_LOCKED ) ) )
#if defined( VERBOSE )
			fprintf( fp, "D 0 %d %d %d The %s door of %s is %s\n", 
				pRoomIndex->vnum,
				pExit->orig_door,
				IS_SET( pExit->rs_flags, EX_LOCKED) ? 2 : 1,
				dir_name[ pExit->orig_door ],
				pRoomIndex->name,
				IS_SET( pExit->rs_flags, EX_LOCKED) ? "closed and locked"
				    : "closed" );
#endif
#if !defined( VERBOSE )
			fprintf( fp, "D 0 %d %d %d\n", 
				pRoomIndex->vnum,
				pExit->orig_door,
				IS_SET( pExit->rs_flags, EX_LOCKED) ? 2 : 1 );
#endif
		}
	    }
	}
    }
    return;
}




/*****************************************************************************
 Name:		save_resets
 Purpose:	Saves the #RESETS section of an area file.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_resets( FILE *fp, AREA_DATA *pArea )
{
    RESET_DATA *pReset;
    MOB_INDEX_DATA *pLastMob = NULL;
    OBJ_INDEX_DATA *pLastObj;
    ROOM_INDEX_DATA *pRoom;
    char buf[MAX_STRING_LENGTH];
    int iHash;
/* put int temp here for one time changing of wear locations for mobs */
/* int temp;  for changed E code when needed... */

    fprintf( fp, "#RESETS\n" );

    save_door_resets( fp, pArea );

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pRoom = room_index_hash[iHash]; pRoom; pRoom = pRoom->next )
        {
            if ( pRoom->area == pArea )
	    {
    for ( pReset = pRoom->reset_first; pReset; pReset = pReset->next )
    {
	switch ( pReset->command )
	{
	default:
	    bug( "Save_resets: bad command %c.", pReset->command );
	    break;

#if defined( VERBOSE )
	case 'M':
            pLastMob = get_mob_index( pReset->arg1 );
	    fprintf( fp, "M 0 %d %d %d %d Load %s\n", 
	        pReset->arg1,
                pReset->arg2,
                pReset->arg3,
		pReset->arg4,
                pLastMob->short_descr );
            break;

	case 'O':
            pLastObj = get_obj_index( pReset->arg1 );
            pRoom = get_room_index( pReset->arg3 );
/*  changes 0 to 1 to match format */
	    fprintf( fp, "O 0 %d 1 %d %s loaded to %s\n", 
	        pReset->arg1,
                pReset->arg3,
                capitalize(pLastObj->short_descr),
                pRoom->name );
            break;

	case 'P':
            pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( fp, "P 0 %d %d %d %d %s put inside %s\n", 
	        pReset->arg1,
	        pReset->arg2,
	        pReset->arg3,
                pReset->arg4,
                pLastObj->short_descr,
                pLastObj->short_descr );
            break;

	case 'G':
/*  adds arg2, was just auto-0 */
	    fprintf( fp, "G 0 %d %d %s is given to %s\n",
	        pReset->arg1,
	        pReset->arg1,
	        capitalize(get_obj_index( pReset->arg1 )->short_descr),
                pLastMob ? pLastMob->short_descr : "!NO_MOB!" );
            if ( !pLastMob )
            {
                sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->file_name );
                bug( buf, 0 );
            }
            break;
/* normal E code */
	case 'E':
	    fprintf( fp, "E 0 %d -1 %d %s is loaded %s of %s\n",
	        pReset->arg1,
                pReset->arg3,
                capitalize(get_obj_index( pReset->arg1 )->short_descr),
                flag_string( wear_loc_strings, pReset->arg3 ),
                pLastMob ? pLastMob->short_descr : "!NO_MOB!" );
            if ( !pLastMob )
            {
                sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->file_name );
                bug( buf, 0 );
            }
            break;

/* NEW e code, just to switch wear locatiosn to new format, run ONCE */
/* I only put it in the verbose section... */
/* must also define int temp; above */
/*	case 'E':

	temp = pReset->arg3;

	if (pReset->arg3 == 20) temp=19;	
	if (pReset->arg3 == 19) temp=20;	

	    fprintf( fp, "E 0 %d -1 %d %s is loaded %s of %s\n",
	        pReset->arg1,
                temp,
                capitalize(get_obj_index( pReset->arg1 )->short_descr),
                flag_string( wear_loc_strings, pReset->arg3 ),
                pLastMob ? pLastMob->short_descr : "!NO_MOB!" );
            if ( !pLastMob )
            {
                sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->file_name );
                bug( buf, 0 );
            }
            break;
 comments out wear locations fix code, may need it again */

	case 'D':
            break;

	case 'R':
/*  do i have this? */
            pRoom = get_room_index( pReset->arg1 );
	    fprintf( fp, "R 0 %d %d Randomize %s\n", 
	        pReset->arg1,
                pReset->arg2,
                pRoom->name );
            break;
            }
#endif
#if !defined( VERBOSE )
	case 'M':
            pLastMob = get_mob_index( pReset->arg1 );
	    fprintf( fp, "M 0 %d %d %d %d\n", 
	        pReset->arg1,
                pReset->arg2,
                pReset->arg3,
                pReset->arg4 );
            break;

	case 'O':
/*  repeats changes from above 0->1 */
            pLastObj = get_obj_index( pReset->arg1 );
            pRoom = get_room_index( pReset->arg3 );
	    fprintf( fp, "O 0 %d 1 %d\n", 
	        pReset->arg1,
                pReset->arg3 );
            break;

	case 'P':
            pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( fp, "P 0 %d %d %d %d\n", 
	        pReset->arg1,
	        pReset->arg2,
                pReset->arg3,
                pReset->arg4 );
            break;

	case 'G':
/*  adds arg2 */
	    fprintf( fp, "G 0 %d %d\n", 
		pReset->arg1, 
		pReset->arg2  );
            if ( !pLastMob )
            {
                sprintf( buf,
                    "Save_resets: !NO_MOB! in [%s]", pArea->file_name );
                bug( buf, 0 );
            }
            break;

	case 'E':
/*  changes -1 to 0 */
	    fprintf( fp, "E 0 %d -1 %d\n",
	        pReset->arg1,
                pReset->arg3 );
            if ( !pLastMob )
            {
                sprintf( buf,
                    "Save_resets: !NO_MOB! in [%s]", pArea->file_name );
                bug( buf, 0 );
            }
            break;

	case 'D':
            break;

	case 'R':
            pRoom = get_room_index( pReset->arg1 );
	    fprintf( fp, "R 0 %d %d\n", 
	        pReset->arg1,
                pReset->arg2 );
            break;
            }
#endif
        }
	    }	/* End if correct area */
	}	/* End for pRoom */
    }	/* End for iHash */
    fprintf( fp, "S\n\n\n\n" );
    return;
}



/*****************************************************************************
 Name:		save_shops
 Purpose:	Saves the #SHOPS section of an area file.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_shops( FILE *fp, AREA_DATA *pArea )
{
    SHOP_DATA *pShopIndex;
    MOB_INDEX_DATA *pMobIndex;
    int iTrade;
    int iHash;
    
    fprintf( fp, "#SHOPS\n" );

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pMobIndex = mob_index_hash[iHash]; pMobIndex; pMobIndex = pMobIndex->next )
        {
            if ( pMobIndex && pMobIndex->area == pArea && pMobIndex->pShop )
            {
                pShopIndex = pMobIndex->pShop;

                fprintf( fp, "%d ", pShopIndex->keeper );
                for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
                {
                    if ( pShopIndex->buy_type[iTrade] != 0 )
                    {
                       fprintf( fp, "%d ", pShopIndex->buy_type[iTrade] );
                    }
                    else
                       fprintf( fp, "0 ");
                }
                fprintf( fp, "%d %d ", pShopIndex->profit_buy, pShopIndex->profit_sell );
                fprintf( fp, "%d %d\n", pShopIndex->open_hour, pShopIndex->close_hour );
            }
        }
    }

    fprintf( fp, "0\n\n\n\n" );
    return;
}

void save_helps()
{  /* thnx to everyone who helped debug this, less buggy and faster now */
	FILE *fp;
	HELP_DATA *pHelp;

	/*rename to create backup copy! */
	rename( "fate.are", "fate.bak");

	// fclose(fpReserve); 

       if((fp=fopen( "fate.are", "w")) == NULL)
	{	
	bug( "save_helps: fopen", 0);
	perror( "fate.are" );
        fpReserve = fopen( NULL_FILE, "r" );
	      return;

}
	fprintf( fp, "#HELPS\n\n");

	for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next ){

		fprintf( fp, "%d ", pHelp->level);
	fprintf(fp, "%s~\n", pHelp->keyword);
	fprintf(fp, "%s~\n\n", fix_string(pHelp->text));

	}
	fprintf(fp, "0 $~\n\n#$\n");
	fclose( fp );
}

/*****************************************************************************
 Name:		save_area
 Purpose:	Save an area, note that this format is new.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area( AREA_DATA *pArea )
{
    FILE *fp;

    fclose( fpReserve );
    if ( !( fp = fopen( pArea->file_name, "w" ) ) )
    {
	bug( "Open_area: fopen", 0 );
	perror( pArea->file_name );
    }

    fprintf( fp, "#AREADATA\n" );
    fprintf( fp, "Name %s~\n",        pArea->name );
    fprintf( fp, "Builders %s~\n",        fix_string( pArea->builders ) );
    fprintf( fp, "VNUMs %d %d\n",      pArea->min_vnum, pArea->max_vnum );
    fprintf( fp, "Credits %s~\n",	 pArea->credits );
    fprintf( fp, "Security %d\n",         pArea->security );
    fprintf( fp, "Worth %d\n", 		pArea->worth); 
    fprintf( fp, "Owner %s~\n",		pArea->owner); 
    fprintf( fp, "End\n\n\n\n" );

    save_mobiles( fp, pArea );
    save_objects( fp, pArea );
    save_rooms( fp, pArea );
    save_specials( fp, pArea );
    save_practicer( fp, pArea );
    save_resets( fp, pArea );
    save_shops( fp, pArea );
    save_olimit( fp, pArea );
    save_mobprogs( fp, pArea );
    save_objprogs( fp, pArea );
    save_roomprogs( fp, pArea );
    fprintf( fp, "#$\n" );

    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

/*****************************************************************************
 Name:		do_asave
 Purpose:	Entry point for saving area data.
 Called by:	interpreter(interp.c)

****************************************************************************/
void do_asave( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    AREA_DATA *pArea;
    FILE *fp;
    int value, sec;

    fp = NULL;

    if ( !ch )       /* Do an autosave */
	sec = 9;
    else if ( !IS_NPC(ch) )
    	sec = ch->pcdata->security;
    else
    	sec = 0;

/*    {
	save_area_list();
	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    save_area( pArea );
	    REMOVE_BIT( pArea->area_flags, AREA_CHANGED );
	}
	return;
    } */

    smash_tilde( argument );
    strcpy( arg1, argument );

    if ( arg1[0] == '\0' )
    {
	if (ch)
	{
		send_to_char( "Syntax:\n\r", ch );
		send_to_char( "  asave <vnum>   - saves a particular area\n\r",	ch );
		send_to_char( "  asave list     - saves the area.lst file\n\r",	ch );
		send_to_char( "  asave area     - saves the area being edited\n\r",	ch );
		send_to_char( "  asave changed  - saves all changed zones\n\r",	ch );
		send_to_char( "  asave world    - saves the world! (db dump)\n\r",	ch );
		send_to_char( "  asave helps    - saves the help file\n\r",	ch );
		send_to_char( "\n\r", ch );
	}

	return;
    }

    /* Snarf the value (which need not be numeric). */
    value = atoi( arg1 );
    if ( !( pArea = get_area_data( value ) ) && is_number( arg1 ) )
    {
	if (ch)
		send_to_char( "That area does not exist.\n\r", ch );
	return;
    }

    /* Save area of given vnum. */
    /* ------------------------ */
    if ( is_number( arg1 ) )
    {
	if ( ch && !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "You are not a builder for this area.\n\r", ch);
	    return;
	}

	save_area_list();
	save_area( pArea );

	return;
    }

    /* Save the world, only authorized areas. */
    /* -------------------------------------- */
    if ( !str_cmp( "world", arg1 ) )
    {
	save_area_list();
	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    /* Builder must be assigned this area. */
	    if ( ch && !IS_BUILDER( ch, pArea ) )
		continue;

	    save_area( pArea );
	    REMOVE_BIT( pArea->area_flags, AREA_CHANGED );
	}

	if ( ch )
		send_to_char( "You saved the world.\n\r", ch );
	return;
    }

    /* Save changed areas, only authorized areas. */
    /* ------------------------------------------ */
    if ( !str_cmp( "changed", arg1 ) )
    {
	char buf[MAX_INPUT_LENGTH];

	save_area_list();

	if ( ch )
		send_to_char( "Saved zones:\n\r", ch );
	else
		log_string( "Saved zones:" );

	sprintf( buf, "None.\n\r" );

	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    /* Builder must be assigned this area. */
	    if ( ch && !IS_BUILDER( ch, pArea ) )
		continue;

	    /* Save changed areas. */
	    if ( IS_SET(pArea->area_flags, AREA_CHANGED) )
	    {
		save_area( pArea );
		sprintf( buf, "%24s - '%s'", pArea->name, pArea->file_name);
		if ( ch )
		{
			send_to_char( buf, ch );
			send_to_char( "\n\r", ch );
		}
		else
			log_string( buf );
		REMOVE_BIT( pArea->area_flags, AREA_CHANGED );
	    }
	}

	if ( !str_cmp( buf, "None.\n\r" ) )
	 {
		if ( ch )
			send_to_char( buf, ch );
		else
			log_string( "None." );
          }
	return;
    }

    /* Save the area.lst file. */
    /* ----------------------- */
    if ( !str_cmp( arg1, "list" ) )
    {
	save_area_list();
	return;
    }
    if (!str_cmp(arg1, "helps"))
	{		
		save_helps();
		send_to_char("{RHelps saved.{x\n\r", ch);
		return;
	}
    /* Save area being edited, if authorized. */
    /* -------------------------------------- */
    if ( !str_cmp( arg1, "area" ) )
    {
	if ( !ch || !ch->desc )
		return;

	/* Is character currently editing. */
	if ( ch->desc->editor == ED_NONE )
	{
	    send_to_char( "You are not editing an area, "
		"therefore an area vnum is required.\n\r", ch );
	    return;
	}
	
	/* Find the area to save. */
	switch (ch->desc->editor)
	{
	    case ED_AREA:
		pArea = (AREA_DATA *)ch->desc->pEdit;
		break;
	    case ED_ROOM:
		pArea = ch->in_room->area;
		break;
	    case ED_OBJECT:
		pArea = ( (OBJ_INDEX_DATA *)ch->desc->pEdit )->area;
		break;
	    case ED_MOBILE:
		pArea = ( (MOB_INDEX_DATA *)ch->desc->pEdit )->area;
		break;
	    default:
		pArea = ch->in_room->area;
		break;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "You are not a builder for this area.\n\r", ch);
	    return;
	}

	save_area_list();
	save_area( pArea );
	REMOVE_BIT( pArea->area_flags, AREA_CHANGED );
	send_to_char( "Area saved.\n\r", ch );
	return;
    }

    /* Show correct syntax. */
    /* -------------------- */
    if (ch)
	do_asave( ch, "" );

    return;
}

