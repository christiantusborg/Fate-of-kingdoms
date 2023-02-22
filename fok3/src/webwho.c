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
    default: buf3[0] = arg[i]; xprintf(buf, buf3); break;
      {
        case '{':
          buf2[0] = '\0';
          i = i + 1;
       switch (arg[i])
       {
         
           {
           case 'm':           
             xprintf(buf2,"");
             break;
           case 'M':
             xprintf(buf2, "");
             break;
           case 'r':
             xprintf(buf2, "");
             break;
           case 'R':
             xprintf(buf2, "");
             break;
           case 'g':
             xprintf(buf2, "");
             break;
           case 'G':
             xprintf(buf2, "");
             break;
           case 'c':
             xprintf(buf2, "");
             break;
           case 'C':
             xprintf(buf2, "");
             break;
           case 'y':
             xprintf(buf2, "");
             break;
           case 'Y':
             xprintf(buf2, "");
             break;
           case 'w':
             xprintf(buf2, "");
             break;
           case 'W':
             xprintf(buf2, "");
             break;
           case 'D':
             xprintf(buf2, "");
             break;
           case 'b':
             xprintf(buf2, "");
             break;
           case 'B':
             xprintf(buf2, "");
             break;
           case '{':
             xprintf(buf2, "");
             break;
           case 'x':
             xprintf(buf2 ,"");
             break;
          } 
          default: xprintf(buf2, "");break;
        }      
        xprintf(buf, buf2);
        break;
      case '\0':
          buf3[0] = arg[i];
          xprintf(buf, buf3);
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
    fprintf(fp, "<link rel=\"stylesheet\" href=\"fokStyle.css\">");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
    fprintf(fp, "<br><br>\n");
    fprintf(fp, "<div align=center>\n");
    fprintf(fp, "<h1>Fate of Kingdoms WHO List. Updates every 30 seconds.</h1><br><br>\n");
    fprintf(fp, "<table width=400 cellspacing=0 cellpadding=2 border=0>\n");
    fprintf(fp, "<tr>\n");
    fprintf(fp, "<td colspan=2><h2>(Visible) Immortals Online<h2></td>\n");
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
    xprintf(kingdombuf," ");
	
 if ( wch->level >= LEVEL_IMMORTAL )
  {
   xprintf( buf,"<tr><td align=right><h3> %s </h3></td><td><h3> %s %s %s",
     kingdombuf,
     wch->pcdata->pretitle,
     wch->name,
     wch->pcdata->title);
   get_who_data(buf2, buf, strlen(buf));
   fprintf(fp,buf2);
   fprintf(fp, "</h3></td></tr>\n");
  immMatch++; 
 }
}

    fprintf(fp, "<tr>\n");
    fprintf(fp, "<td colspan=2><h2>(Visible) Players Online<h2></td>\n");
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
    xprintf(kingdombuf," [%s] ",kingdom_table[wch->kingdom].short_name);
   else 
    kingdombuf[0] = '\0';
	
 if ( wch->level < LEVEL_IMMORTAL )
  {
   xprintf( buf,"<tr><td align=right><h3> %s </h3></td><td> <h3> %s %s %s",
     kingdombuf,
     wch->pcdata->pretitle,
     wch->name,
     wch->pcdata->title);
   get_who_data(buf2, buf, strlen(buf));
   fprintf(fp,buf2);
   fprintf(fp, "</h3></td></tr>\n");
  nMatch++; 
  count++;
  }
 }


   xprintf( buf, "<tr><td colspan=2 align=center><h2>Immortals Found: %d &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Players found: %d </h3></td></tr>\n", immMatch, nMatch);
    fprintf(fp, buf);
    fprintf(fp, "</table>\n");
    xprintf(buf, "<h4>This file last updated at %s GMT Time.</h4><br>\n", ((char *) ctime( &current_time )));
    fprintf(fp, buf);
    fprintf(fp, "<h4>You will not see any invisible players or immortals on.</h4>\n");
    fprintf(fp, "</div>\n");
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");

 
  fclose( fp ); 
  fpReserve = fopen( NULL_FILE, "r" );
  } /*end if */ 


    return;
}
