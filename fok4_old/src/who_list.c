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
#include "lookup.h"
#include "tables.h"
 #include "fok_struct.h"
#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif


void who_html_update (void)
{
  FILE *fp; 
  DESCRIPTOR_DATA *d;
  char buf[MAX_INPUT_LENGTH];
  char buf2[MAX_INPUT_LENGTH];
  

  buf[0] = '\0';
  buf2[0] = '\0';
  
  fclose(fpReserve);

  if ( (fp = fopen(WEBWHO_FILE , "w") ) == NULL)
  {
     bug( "online.htm: fopen", 0 );
     perror( "online.htm" );
  }
  else
  {
  fprintf(fp, "<html>\n");
  fprintf(fp, "<head>\n");
  fprintf(fp, "<link rel=stylesheet href=\"fok.css\" type=\"text/css\">\n");
  fprintf(fp, "<title>");
  fprintf(fp, "Players currently on Fate of Kingdoms III, (By Kazmir & Scarpia)");
  fprintf(fp, "</title>\n");
  fprintf(fp, "<body>\n");
  fprintf(fp, "<div align=center>\n");
  fprintf(fp, "<big>Fate of Kingdoms WHO List. Updates every 30 seconds.</big>\n");
  fprintf(fp, "<table background=#6699CC width=80 cellspacing=4 cellpadding=3 border=0>\n");
  fprintf(fp, "<tr>\n");
  fprintf(fp, "<td colspan=2><span style=\"font-size:15pt;\">(Visible) Immortals Online</span></td>\n");
  fprintf(fp, "</tr>\n");
   
  for ( d = descriptor_list; d != NULL ; d = d->next )
  {
    CHAR_DATA *wch;
    
    if ( d->connected != CON_PLAYING)
        continue;
    wch   = ( d->original != NULL ) ? d->original : d->character;

    if (!(wch->invis_level > LEVEL_HERO) && 
	!IS_AFFECTED(wch,AFF_INVISIBLE) && 
	!IS_AFFECTED(wch, AFF_SNEAK) && 
	!IS_AFFECTED(wch,AFF_CAMOUFLAGE) && 
	!IS_AFFECTED(wch, AFF_HIDE) &&
	!IS_AFFECTED(wch, AFF_FADE) &&
	!is_affected(wch,gsn_duo_dimension))
    {
      sprintf(buf2, "%6s", pc_race_table[wch->race].who_name);
      get_who_data(buf, buf2, strlen(buf2));  
      fprintf(fp, buf);
      sprintf(buf2, "%s", wch->pcdata->pretitle);
      get_who_data(buf, buf2, strlen(buf2));
      fprintf(fp, buf);
      sprintf(buf, "%s", wch->name);
      fprintf(fp, buf);
      buf2[0] = '\0';
      sprintf(buf2, "%s", wch->pcdata->title);

      get_who_data(buf, buf2, strlen(buf2));
      fprintf(fp, buf);

      fprintf(fp, "\n");
 
    }  /*end if */
  }    /*end for */
  




  fprintf(fp, "<font color=""#FFFFFF"" face=""Times New Roman"">\n");
  sprintf(buf, "This file last updated at %s Eastern Time.\n", ((char *) ctime( &current_time )));
  fprintf(fp, buf);
  fprintf(fp, "<br>\n");
  fprintf(fp, "If this is more than a few moments old, the mud is likely to be down.\n");
  fprintf(fp, "<br>\n");
  fprintf(fp, "You will not be able to see any invis player or immortals on.\n");
  fprintf(fp, "<br>\n");
  fprintf(fp, "<a href=""index.html"">Back to main page.</a>\n");
  fprintf(fp, "<br>\n");
  fprintf(fp, "<br>\n");
  fprintf(fp, "<hr>\n");

  fprintf(fp, "</p>\n");
  fprintf(fp, "</body>\n");
  fprintf(fp, "</html>\n");
  fclose( fp ); 
  fpReserve = fopen( NULL_FILE, "r" );
  } /*end if */ 
  
  return;
}/* end function */
