
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

#include "merc.h"


void safe_printf(const char *file, const char *function, int line,int size, char *str, char *fmt, ...)
{
  char buf [MAS];
  va_list args;
  va_start (args, fmt);
  vsprintf (buf, fmt, args);
  va_end (args);

  /*Max Alloc Size is allot!*/
  if(size > MAS)
  {
    char egbug[MSL];
  //  logf("xprintf size greater then MAS!!!!\n\r");
  //  logf("Warning: Overflow has been caught by xprintf.\n\r");
  //  logf("Memcheck: xprintf:File %s, Function %s, Line %d.\n\r",file,function,line);
    xprintf(egbug, "Memcheck: System memory corrupted by overflow through xprintf: File: %s Function: %s Line: %d",file, function,line);
    wiznet(egbug,NULL,NULL,WIZ_MEMCHECK,0,0);
    return;
  }

  if ((unsigned)size < strlen(buf)+1) 
  {
    char egbug[MSL];
   // logf("XPRINTF error: fmt %s.\n\r",fmt);
   // logf("Warning: Overflow has been caught by xprintf.\n\r");
   // logf("Memcheck: xprintf: File %s, Function %s, Line %d.\n\r",file,function,line);
    /*Yes, this is a potential loop bug if infact the xprintf does collapse in on itself..*/
    xprintf(egbug, "Memcheck: System memory corrupted by overflow through xprintf: File: %s Function: %s Line: %d",file,function,line);
    wiznet(egbug,NULL,NULL,WIZ_MEMCHECK,0,0);
  }
  else
  {
	    strcpy(str,buf);
	    //Disabled for now.
	    //strlcpy(str,buf,size-2);
	    //str[size-1] = '\0';

	    /*Just double checking.*/
	   if( strlen( str ) > (unsigned)size - 1 )
	   {
		 char egbug[MSL];
//	   	 logf("XPRINTF error: fmt %s.\n\r",fmt);
///		     logf("Warning: Overflow has been caught by xprintf.\n\r");
//		 logf("Memcheck: Xprintf: File %s, Function %s, Line %d.\n\r",file, function,line);

		 /*Yes, this is a potential loop bug if infact the xprintf does collapse in on itself..*/
		 xprintf(egbug, "Memcheck: System memory corrupted by overflow through xprintf: File: %s Function: %s Line: %d",file, function,line);
		 wiznet(egbug,NULL,NULL,WIZ_MEMCHECK,0,0);
	   }
  }
}

void safe_strcatf (const char *file, const char *function, int line,int size, char *prev, char *next, ... )
{
    char buf [MAS];
    va_list args;

    va_start ( args, next );
    vsnprintf ( buf, MAS, next, args );
    va_end ( args );

  /*Max Alloc Size is allot!*/
  if(size > MAS)
  {
    char egbug[MSL];
   // logf( "xcatf size greater then MAS!!!!\n\r");
   // logf( "ERROR: System Memory Corrupted by Overflow, through xcatf.\n\r");
   // logf( "Memcheck: xcatf:File %s, Function %s, Line %d.\n\r",file,function,line);
    xprintf(egbug, "Memcheck: System memory corrupted by overflow through xcatf: File: %s Function: %s Line: %d",file, function,line);
    wiznet(egbug,NULL,NULL,WIZ_MEMCHECK,0,0);
    return;
  }

  if ((unsigned)size < strlen(buf)+1) 
  {
    char egbug[MSL];
  //  logf( "XCATF error: fmt %s.\n\r",next);
  //  logf( "ERROR: System Memory Corrupted by Overflow, through xcatf.\n\r");
  //  logf( "Memcheck: xcatf: File %s, Function %s, Line %d.\n\r",file,function,line);
    /*Yes, this is a potential loop bug if infact the xcatf does collapse in on itself..*/
    xprintf(egbug, "Memcheck: System memory corrupted by overflow through xcatf: File: %s Function: %s Line: %d",file,function,line);
    wiznet(egbug,NULL,NULL,WIZ_MEMCHECK,0,0);
    return;
  }

    strcpy( prev, buf );

    /*Just double checking.*/
   if( strlen( prev ) > (unsigned)size - 1 )
   {
	 char egbug[MSL];
   //	 logf( "XCATF error: fmt %s.\n\r",next);
//	 logf( "ERROR: System Memory Corrupted by Overflow, through xcatf.\n\r");
//	 logf( "Memcheck: Xcatf: File %s, Function %s, Line %d.\n\r",file, function,line);
	 /*Yes, this is a potential loop bug if infact the xcatf does collapse in on itself..*/
	 xprintf(egbug, "Memcheck: System memory corrupted by overflow through xcatf: File: %s Function: %s Line: %d",file, function,line);
	 wiznet(egbug,NULL,NULL,WIZ_MEMCHECK,0,0);
   }

}
