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
 
#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif

void get_who_data (char buf[MAX_INPUT_LENGTH], char arg[MAX_INPUT_LENGTH], int len)
{
  char buf2[22];
  char buf3[1];
  
  int i;
  
  

for (i=0; i < MAX_INPUT_LENGTH; ++i)
{
   buf[i]= 'G';
}

for (i=0; i < 22; ++i)
{
  buf2[i] = 'G';
}

buf[0] = '\0';
buf2[0] = '\0';

for (i=0; i <= len; ++i) 
{
  
  switch (arg[i])
  {
    default: buf3[0] = arg[i]; strcat(buf, buf3); break;
      {
        case '{':
          buf2[0] = '\0';
          i = i + 1;
       switch (arg[i])
       {
         
           {
           case 'm':           
             strcat(buf2,"<font color=""#8B008B"">");
             break;
           case 'M':
             strcat(buf2, "<font color=""#FF00FF"">");
             break;
           case 'r':
             strcat(buf2, "<font color=""#8B0000"">");
             break;
           case 'R':
             strcat(buf2, "<font color=""#FF0000"">");
             break;
           case 'g':
             strcat(buf2, "<font color=""#006400"">");
             break;
           case 'G':
             strcat(buf2, "<font color=""#00FF00"">");
             break;
           case 'c':
             strcat(buf2, "<font color=""#008B8B"">");
             break;
           case 'C':
             strcat(buf2, "<font color=""#00FFFF"">");
             break;
           case 'y':
             strcat(buf2, "<font color=""#808000"">");
             break;
           case 'Y':
             strcat(buf2, "<font color=""#FFFF00"">");
             break;
           case 'w':
             strcat(buf2, "<font color=""#808080"">");
             break;
           case 'W':
             strcat(buf2, "<font color=""#FFFFFF"">");
             break;
           case 'D':
             strcat(buf2, "<font color=""#636363"">");
             break;
           case 'b':
             strcat(buf2, "<font color=""#00008B"">");
             break;
           case 'B':
             strcat(buf2, "<font color=""#0000FF"">");
             break;
           case '{':
             strcat(buf2, "{");
             break;
           case 'x':
             strcat(buf2 ,"<font color=""#006400"">");
             break;
          }
          default: strcat(buf2, "");break;
        }      
        strcat(buf, buf2);
        break;
      case '\0':
          buf3[0] = arg[i];
          strcat(buf, buf3);
          break;
      } 
     }
}
 
 return;
 }

void who_html_update (void)
{
  FILE *fp; 
  DESCRIPTOR_DATA *d;
  char buf[MAX_INPUT_LENGTH];
  char buf2[MAX_INPUT_LENGTH];
  

  buf[0] = '\0';
  buf2[0] = '\0';
  
  fclose(fpReserve);

  if ( (fp = fopen("/home/fok/public_html/online.htm", "w") ) == NULL)
  {
     bug( "online.htm: fopen", 0 );
     perror( "online.htm" );
  }
  else
  {
  fprintf(fp, "<html>\n");
  fprintf(fp, "<head>\n");
  fprintf(fp, "<title>\n");
  fprintf(fp, "Players currently on Fate of Kingdoms II\n");
  fprintf(fp, "</title>\n");
  fprintf(fp, "<body bgcolor=""#000000"" link=""#FF0000"" vlink=""#FFA500"" alink=""#FFFF00"">\n");
  fprintf(fp, "<h1><font color=""#FF2222"">\n");
  fprintf(fp, "Who's on Fate of Kingdom II\n");  
    /*  Change to your mud name.  Also customize the title, background color, whatever
     *    --Valatar
     */
  fprintf(fp, "</font></h1>\n");
  
  fprintf(fp, "<pre>\n");

  fprintf(fp, "<font color=""#FF00FF"">[");
  fprintf(fp, "<font color=""#00FFFF"">Race");
  fprintf(fp, "<font color=""#FF00FF"">] [");
  fprintf(fp, "<font color=""#00FFFF"">Player Name");
  fprintf(fp, "<font color=""#FF00FF"">]\n");
  fprintf(fp, "<font color=""#0000FF"">_________________________________________________________________\n");
  
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
      fprintf(fp, "<font color=""#006400"">[");
      fprintf(fp, "<font color=""#0000FF"">");
	buf2[0] = '\0';
      sprintf(buf2, "%6s", pc_race_table[wch->race].who_name);
      get_who_data(buf, buf2, strlen(buf2));  
      fprintf(fp, buf);
      fprintf(fp, "<font color=""#006400"">] ");
      buf2[0] = '\0';
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
  
  fprintf(fp, "</font>\n");
  fprintf(fp, "</pre><br>\n");
  fprintf(fp, "<p>\n"); 
  fprintf(fp, "<br>\n");
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


