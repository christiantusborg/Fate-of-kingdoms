#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/dir.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "merc.h"  
#include "recycle.h"  
#include "tables.h"  
#include "lookup.h" 
#include "arena.h"
#include "db.h"
#include "olc.h"

struct	top_type	top_table	[21]	=
{
   { "NONE",	0}, //1
   { "NONE",	0}, //2
   { "NONE",	0}, //3
   { "NONE",	0}, //4
   { "NONE",	0}, //5
   { "NONE",	0}, //6
   { "NONE",	0}, //7
   { "NONE",	0}, //8
   { "NONE",	0}, //9
   { "NONE",	0}, //10
   { "NONE",	0}, //1
   { "NONE",	0}, //2
   { "NONE",	0}, //3
   { "NONE",	0}, //4
   { "NONE",	0}, //5
   { "NONE",	0}, //6
   { "NONE",	0}, //7
   { "NONE",	0}, //8
   { "NONE",	0}, //9
   { "NONE",	0}, //10   
   { "NONE",	0}, //Not in list
};

void update_top_rp( CHAR_DATA *ch)
 {
  int TopI;
  int TopJ;
  int Tmpnumber;
  char * TmpName;

if (IS_IMMORTAL(ch))
      return;
      
      
for (TopI = 0; TopI <= 20; TopI++)
  if (!str_cmp (ch->name, top_table[TopI].name))
   {
    top_table[TopI].name = str_dup("NONE");
    top_table[TopI].number = 0;
   }


  top_table[20].name   = str_dup(ch->name);
  top_table[20].number = ch->trptoken;




  for (TopI = 20; TopI >= 1; TopI--)
   {
//    for (TopJ = 21;TopJ > TopI+1;TopJ--)
      TopJ = TopI -1;
      if (top_table[TopI].number > top_table[TopJ].number)
       {
        Tmpnumber = top_table[TopJ].number;
        TmpName   = top_table[TopJ].name;
        
        top_table[TopJ].number = top_table[TopI].number;
        top_table[TopJ].name   = str_dup(top_table[TopI].name);

        top_table[TopI].number = Tmpnumber;
        top_table[TopI].name   = str_dup(TmpName);
       }
 }

 save_top();
 save_top_html();
 return;
 }

void do_show_top_rp( CHAR_DATA *ch, char *argument)
 {
  int Top;
  char buf[MAX_STRING_LENGTH];
  
  send_to_char("-----------------------------------------------------\n\r",ch);
  send_to_char("              Top Roleplaying \n\r",ch);
  send_to_char("-----------------------------------------------------\n\r",ch);
  for (Top = 0; Top < 20 ; Top++)
   {
    sprintf(buf,"%2d * %-25s * %d\n\r",Top+1,top_table[Top].name,top_table[Top].number);
    send_to_char(buf,ch); 
   } 
  send_to_char("-----------------------------------------------------\n\r",ch);    
return;
 }
 
void save_top(void) 
  {   
    FILE *fp; 
   int Top;
   char buf[MAX_STRING_LENGTH];    
    
    
fclose(fpReserve);

  if ( (fp = fopen(TOP_FILE , "w") ) == NULL)
  {
     bug( "top.dat: fopen", 0 );
     perror( "top.dat" );
     fpReserve = fopen( NULL_FILE, "r" );
  }
  else
  {    
   for (Top = 0; Top < 21 ; Top++)
    {
     sprintf(buf,"%d %s~\n",top_table[Top].number,top_table[Top].name);
     fprintf(fp,buf);
    } 
  }  

  sprintf(buf,"-1\n");
  fprintf(fp,buf);
  fclose( fp ); 
  fpReserve = fopen( NULL_FILE, "r" );
  return;
}  

void load_top(void) 
 {

	char buf[MAX_STRING_LENGTH];
//	int number;
	int TopI;
	FILE *fp;
  if (!(fp = fopen (TOP_FILE, "r")))
   {
          sprintf (buf, "Could not open file in order to load top.");
		return;
	}
	
	TopI = 0;
	
	while (TopI <= 20) 
	{

		top_table[TopI].number = fread_number( fp ); 
		top_table[TopI].name   = fread_string( fp );
		 
		TopI++;
//		fscanf (fp, "%d", &number);
	}
	
	fclose (fp);
	return;
} 
/*
void do_wipe_top( CHAR_DATA *ch, char *argument)
 {

 }
 */
 
 void save_top_html(void) 
 {   
    FILE *fp; 
   int Top;
   char buf[MAX_STRING_LENGTH];    
    
    
fclose(fpReserve);

  if ( (fp = fopen(TOP_HTML , "w") ) == NULL)
  {
     bug( "top.html: fopen", 0 );
     perror( "top.htmp" );
     fpReserve = fopen( NULL_FILE, "r" );
  }
  else
  {    
  	
    fprintf(fp, "<html>\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "<META HTTP-EQUIV=Refresh CONTENT=\"40; URL=webwho.html\">\n");
    fprintf(fp, "<link rel=stylesheet href=\"fok.css\" type=\"text/css\">\n");
    fprintf(fp, "<title>Top Roleplay)</title>\n");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
    fprintf(fp, "<br><br>\n");
    fprintf(fp, "<div align=center>\n");
    fprintf(fp, "<span style=\"font-size:15pt\">Top Roleplaying.</span><br><br>\n");
    fprintf(fp, "<table width=80 cellspacing=0 cellpadding=4 border=0>\n");

   for (Top = 0; Top < 10 ; Top++)
    {

     sprintf( buf,"<tr><td><span style=\"font-size:11pt\">%d &nbsp;&nbsp;&nbsp;&nbsp; %s </span></td><td> <span style=\"font-size:11pt\"> %d",
     Top+1,top_table[Top].name,top_table[Top].number);
     fprintf(fp, "</span></td></tr>\n");
     fprintf(fp,buf);
    } 
  }  
    fprintf(fp, "</table>\n");
    sprintf(buf, "<span>This file last updated at %s GMT Time.</span><br>\n", ((char *) ctime( &current_time )));
    fprintf(fp, buf);
    fprintf(fp, "</div>\n");
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");

  fclose( fp ); 
  fpReserve = fopen( NULL_FILE, "r" );
  return;
}
