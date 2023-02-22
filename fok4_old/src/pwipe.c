
#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h> 
#include <unistd.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"

void archive( char*, char*, int );

void wipe_pfiles()
{
    DIR *dp;
    struct dirent *ep;
    char letter ;
    char dir[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
	sprintf( dir , "%s", PLAYER_DIR);
    	dp = opendir (dir);
     
    	if (dp != NULL)
    	{
            while ( (ep = readdir (dp)) )
            {
            	if( ep->d_name[0] != '.' ) 
            	  {
		    archive( dir, ep->d_name, PFILE_EXPIRE );
		 //   sprintf( buf , "%s", ep->d_name);
//		    bug( buf, 0 );
		   }
            }	
            (void) closedir (dp);
    	} 
    	else
	    bugf( "Could not open %s.", dir );

}        
 
void archive( char* dir, char* name , int days)
{
    char old_file[MAX_STRING_LENGTH];
    char new_file[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    
    struct stat fst;
    
    sprintf( old_file, "%s%s",dir ,name );
    

    if ( stat( old_file, &fst ) != -1 )
    {
    	if( difftime( current_time , fst.st_mtime ) > days*60.0*60.0*24.0 )
	{
	    logf( "Archiving %s" , name );
	    sprintf( new_file, "%s%s" , ARCHIVE_DIR, capitalize(name) );
  	    rename( old_file, new_file );
//	    sprintf( buf, "gzip -fq %s", new_file );
//	    system( buf );
	}
    }
}    

void remove_inactive( char* dir , int days)
{
    DIR *dp;
    struct dirent *ep;
    struct stat fst;
    char file[MAX_STRING_LENGTH];
    
    dp = opendir (dir);
                     
    if (dp != NULL)
    {
    	while ( (ep = readdir (dp)) )
    	{
    	   if( ep->d_name[0] != '.' )
    	   {
		sprintf( file, "%s",ep->d_name );
    		if( (stat( file , &fst ) != -1)
    		  && difftime( current_time , fst.st_mtime ) > days*60*60*24 ) 
		{
	     		logf( "Removing %s", ep->d_name );
    	      		unlink(file);
   	     	}
    	   }
    	}
        (void) closedir (dp);
    }
    else
    	bugf( "Could not open %s.", dir );
}
