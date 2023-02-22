#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

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
             strcat(buf2,"");
             break;
           case 'M':
             strcat(buf2, "");
             break;
           case 'r':
             strcat(buf2, "");
             break;
           case 'R':
             strcat(buf2, "");
             break;
           case 'g':
             strcat(buf2, "");
             break;
           case 'G':
             strcat(buf2, "");
             break;
           case 'c':
             strcat(buf2, "");
             break;
           case 'C':
             strcat(buf2, "");
             break;
           case 'y':
             strcat(buf2, "");
             break;
           case 'Y':
             strcat(buf2, "");
             break;
           case 'w':
             strcat(buf2, "");
             break;
           case 'W':
             strcat(buf2, "");
             break;
           case 'D':
             strcat(buf2, "");
             break;
           case 'b':
             strcat(buf2, "");
             break;
           case 'B':
             strcat(buf2, "");
             break;
           case '{':
             strcat(buf2, "");
             break;
           case 'x':
             strcat(buf2 ,"");
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



void web( void )
 {
    FILE *fp; 
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   char kingdombuf[MAX_STRING_LENGTH];
   int nMatch = 0;
   int immMatch = 0;
   int count = 0;
   DESCRIPTOR_DATA *d;   

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
    fprintf(fp, "<META HTTP-EQUIV=Refresh CONTENT=\"40; URL=webwho.html\">\n");
    fprintf(fp, "<link rel=stylesheet href=\"fok.css\" type=\"text/css\">\n");
    fprintf(fp, "<title>Web Who List (cool! by Kazmir & Scarpia)</title>\n");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
    fprintf(fp, "<br><br>\n");
    fprintf(fp, "<div align=center>\n");
    fprintf(fp, "<span style=\"font-size:15pt\">Fate of Kingdoms WHO List. Updates every 30 seconds.</span><br><br>\n");
    fprintf(fp, "<table width=80% cellspacing=0 cellpadding=4 
border=0>\n");
    fprintf(fp, "<tr>\n");
    fprintf(fp, "<td colspan=2><big>(Visible) Immortals Online</big></td>\n");
    fprintf(fp, "</tr>\n");

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
    CHAR_DATA *wch;
//    char const *class;
    
	if ( d->connected != CON_PLAYING )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;    
    

  if (  (wch->invis_level > LEVEL_HERO)
    || (wch->incog_level > LEVEL_HERO)
    || IS_AFFECTED(wch, AFF_INVISIBLE)
    || is_affected(wch,gsn_duo_dimension)
    || is_affected(wch,gsn_spell_illusion_fields)
    || IS_AFFECTED(wch,AFF_CAMOUFLAGE)
    || IS_AFFECTED(wch, AFF_HIDE)
    || IS_AFFECTED(wch, AFF_FADE) )
   continue;

   /*Kingdom*/
   if (wch->kingdom > 1)
    sprintf(kingdombuf," [%s] ",kingdom_table[wch->kingdom].short_name);
   else 
    sprintf(kingdombuf," ");
	
 if ( wch->level >= LEVEL_IMMORTAL )
  {
   sprintf( buf,"<tr><td align=right><span style=\"font-size:11pt\"> %s </span></td><td> <span style=\"font-size:11pt\"> %s %s %s",
     kingdombuf,
     wch->pcdata->pretitle,
     wch->name,
     wch->pcdata->title);
   get_who_data(buf2, buf, strlen(buf));
   fprintf(fp,buf2);
   fprintf(fp, "</span></td></tr>\n");
  immMatch++; 
 }
}

    fprintf(fp, "<tr>\n");
    fprintf(fp, "<td colspan=2><big>(Visible) Players Online</big></td>\n");
    fprintf(fp, "</tr>\n");


    for ( d = descriptor_list; d != NULL; d = d->next )
    {
    CHAR_DATA *wch;
//    char const *class;
    
	if ( d->connected != CON_PLAYING )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;    
    

  if (  (wch->invis_level > LEVEL_HERO)
    || (wch->incog_level > LEVEL_HERO)
    || IS_AFFECTED(wch, AFF_INVISIBLE)
    || is_affected(wch,gsn_duo_dimension)
    || is_affected(wch,gsn_spell_illusion_fields)
    || IS_AFFECTED(wch,AFF_CAMOUFLAGE)
    || IS_AFFECTED(wch, AFF_HIDE)
    || IS_AFFECTED(wch, AFF_FADE) )
   continue;

   /*Kingdom*/
   if (wch->kingdom > 1)
    sprintf(kingdombuf," [%s] ",kingdom_table[wch->kingdom].short_name);
   else 
    kingdombuf[0] = '\0';
	
 if ( wch->level < LEVEL_IMMORTAL )
  {
   sprintf( buf,"<tr><td align=right><span style=\"font-size:11pt\"> %s </span></td><td> <span style=\"font-size:11pt\"> %s %s %s",
     kingdombuf,
     wch->pcdata->pretitle,
     wch->name,
     wch->pcdata->title);
   get_who_data(buf2, buf, strlen(buf));
   fprintf(fp,buf2);
   fprintf(fp, "</span></td></tr>\n");
  nMatch++; 
  count++;
  }
 }


   sprintf( buf, "<tr><td colspan=2 align=center><big>Immortals Found: %d &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Players found: %d </big></td></tr>\n", immMatch, nMatch);
    fprintf(fp, buf);
    fprintf(fp, "</table>\n");
    sprintf(buf, "<span>This file last updated at %s GMT Time.</span><br>\n", ((char *) ctime( &current_time )));
    fprintf(fp, buf);
    fprintf(fp, "<span>You will not see any invis players or immortals on.</span>\n");
    fprintf(fp, "</div>\n");
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");

 
  fclose( fp ); 
  fpReserve = fopen( NULL_FILE, "r" );
  } /*end if */ 


    return;
}
