/***************************************************************************
 *  Fate of Kingdom IV                                                     *
 *   By Chrisitan Tusborg (Kazmir).                                        *
 *									   *
 * The use of this code without the permission of Christian Tusborg        *
 * author is prohibited.  You will be punished to the fullest extend       *
 * of the law.                                                             *
 *                                                                         *
 *     Original Fate of Kingdom Mud (Mud.mysteria.dk:4000)		   *
 ***************************************************************************/
/***************************************************************************
 * File: fok_db.c, Lots of save file for olc.                              *
 * Function:                                                               *
 * void save_raceses( void ) Save all races Called by: setuprace fok_onc.c *
 * void save_race( int num ) Save one Race Called by: save_raceses         *
 * void load_race( void )    Load all races
 ***************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "fok_struct.h"
#include "declaration.h"
/*****************************************************************************
 Name:	    save_raceses
 Purpose:   save all raceses
 Called by: File: fok_olc.c Function: do_setup_race(CHAR_DATA *ch ,char * argument)
 ****************************************************************************/
void save_raceses( void )
{
 int count_race;
 // for all races save 
 for (count_race=1;count_race <= MAX_PC_RACE;count_race++)
  save_race(count_race);
 // Send it to logs when we are do with it. 
 log_string("(Action: File: fok_db.c, Function: load_raceses, Race saving Success!)");
 return;
 }
 

/*****************************************************************************
 Name:	    save_race
 Purpose:   Save one Race.
 Called by: save_raceses(fok_db.c).
 ****************************************************************************/
 void save_race( int num )
{
  FILE *fp;
  char buf[MAX_STRING_LENGTH];

 // Check if it a num validated in put
 if ( (num < 0) || (num > MAX_PC_RACE) )
  {
   sprintf (buf, "BUG: File: fok_db.c, Function: save_race, Called with Num %d.",num);
   bug(buf,0);
  }	
 
 // Check to see if can write to file
 sprintf (buf, "%s%s", RACE_DIR,pc_race_table[num].name);
 
 if (!(fp = fopen (buf, "w")))
  {
   sprintf (buf, "Could not open file %s in order to save race.",pc_race_table[num].name);
   bug(buf,0);
   return;
  } 
 
 // Writing to file
 fprintf (fp,"%s~\n",pc_race_table[num].name);
 fprintf (fp,"%s~\n",pc_race_table[num].who_name);
 fprintf (fp,"%s~\n",pc_race_table[num].short_name);
 fprintf (fp,"%d %d %d\n",pc_race_table[num].hpdice,pc_race_table[num].manadice,pc_race_table[num].movedice);
 fprintf (fp,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
  pc_race_table[num].class_mult[0],
  pc_race_table[num].class_mult[1],
  pc_race_table[num].class_mult[2],
  pc_race_table[num].class_mult[3],
  pc_race_table[num].class_mult[4],
  pc_race_table[num].class_mult[5],
  pc_race_table[num].class_mult[6],
  pc_race_table[num].class_mult[7],
  pc_race_table[num].class_mult[8],
  pc_race_table[num].class_mult[9],
  pc_race_table[num].class_mult[10],
  pc_race_table[num].class_mult[11],
  pc_race_table[num].class_mult[12],
  pc_race_table[num].class_mult[13],
  pc_race_table[num].class_mult[14],
  pc_race_table[num].class_mult[15],
  pc_race_table[num].class_mult[16],
  pc_race_table[num].class_mult[17],
  pc_race_table[num].class_mult[18],
  pc_race_table[num].class_mult[19],
  pc_race_table[num].class_mult[20],
  pc_race_table[num].class_mult[21],
  pc_race_table[num].class_mult[22],
  pc_race_table[num].class_mult[23],
  pc_race_table[num].class_mult[24],
  pc_race_table[num].class_mult[25],
  pc_race_table[num].class_mult[26],
  pc_race_table[num].class_mult[27],
  pc_race_table[num].class_mult[28],
  pc_race_table[num].class_mult[29],
  pc_race_table[num].class_mult[30],
  pc_race_table[num].class_mult[31],
  pc_race_table[num].class_mult[32],
  pc_race_table[num].class_mult[33],
  pc_race_table[num].class_mult[34],
  pc_race_table[num].class_mult[35]);
 fprintf (fp,"%d %d %d %d %d %d %d\n",
  pc_race_table[num].max_stats[0],
  pc_race_table[num].max_stats[1],
  pc_race_table[num].max_stats[2],
  pc_race_table[num].max_stats[3],
  pc_race_table[num].max_stats[4],
  pc_race_table[num].max_stats[5],
  pc_race_table[num].max_stats[6]);
 fprintf (fp,"%d %d %d %d %d %d %d\n",
  pc_race_table[num].min_stats[0],
  pc_race_table[num].min_stats[1],
  pc_race_table[num].min_stats[2],
  pc_race_table[num].min_stats[3],
  pc_race_table[num].min_stats[4],
  pc_race_table[num].min_stats[5],
  pc_race_table[num].min_stats[6]);  
 fprintf (fp,"%d\n\r",pc_race_table[num].size);
 fprintf (fp,"%d %d %d\n",
  pc_race_table[num].alignment[0],
  pc_race_table[num].alignment[1],
  pc_race_table[num].alignment[2]);
 fprintf (fp,"%d %d %d\n",
  pc_race_table[num].ethos[0], 
  pc_race_table[num].ethos[1],
  pc_race_table[num].ethos[2]);
 fprintf (fp,"%d\n",pc_race_table[num].disabled); 
 
 fprintf (fp, "-1"); /* EOF -1 */
 // Close the connection to the file
 fclose (fp);
 
 return;
 };

/*****************************************************************************
 Name:	    load_raceses
 Purpose:   load all raceses
 Called by: File: db.c Function: boot_db()
 ****************************************************************************/
void load_raceses( void )
{
 int count_race;
 
 // for all races load
 for (count_race=1;count_race <= MAX_PC_RACE;count_race++)
  load_race(count_race);
 
 // Send it to logs when we are do with it.
 log_string("(Action: File: fok_db.c, Function: load_raceses, Race loading Success!)");
 return;
 }

/*****************************************************************************
 Name:	    load_race
 Purpose:   load one Race.
 Called by: load_raceses(fok_db.c).
 ****************************************************************************/
void load_race( int num )
{
  char buf[MAX_STRING_LENGTH];
  FILE *fp;

 // Check if it a num validated in put 
 if ( (num < 0) || (num > MAX_PC_RACE) )
  {
   sprintf (buf, "BUG: File: fok_db.c, Function: load_race, Called with Num %d.",num);
   bug(buf,0);
  }
// Check to see if can read to file	
  sprintf (buf, "%s%s", RACE_DIR, pc_race_table[num].name);
	
  if (!(fp = fopen (buf, "r")))
   {
    sprintf (buf, "Could not open file %s in order to load class.",pc_race_table[num].name);
    return;
   }

// Reading file 
 pc_race_table[num].name = fread_string( fp );
 pc_race_table[num].who_name = fread_string( fp ); 
 pc_race_table[num].short_name = fread_string( fp ); 
 pc_race_table[num].hpdice          = fread_number( fp );
 pc_race_table[num].manadice        = fread_number( fp );
 pc_race_table[num].movedice        = fread_number( fp );
 pc_race_table[num].class_mult[0]   = fread_number( fp );
 pc_race_table[num].class_mult[1]   = fread_number( fp );
 pc_race_table[num].class_mult[2]   = fread_number( fp );
 pc_race_table[num].class_mult[3]   = fread_number( fp );
 pc_race_table[num].class_mult[4]   = fread_number( fp );
 pc_race_table[num].class_mult[5]   = fread_number( fp );
 pc_race_table[num].class_mult[6]   = fread_number( fp );
 pc_race_table[num].class_mult[7]   = fread_number( fp );
 pc_race_table[num].class_mult[8]   = fread_number( fp );
 pc_race_table[num].class_mult[9]   = fread_number( fp );
 pc_race_table[num].class_mult[10]  = fread_number( fp );
 pc_race_table[num].class_mult[11]  = fread_number( fp );
 pc_race_table[num].class_mult[12]  = fread_number( fp );
 pc_race_table[num].class_mult[13]  = fread_number( fp );
 pc_race_table[num].class_mult[14]  = fread_number( fp );
 pc_race_table[num].class_mult[15]  = fread_number( fp );
 pc_race_table[num].class_mult[16]  = fread_number( fp );
 pc_race_table[num].class_mult[17]  = fread_number( fp );
 pc_race_table[num].class_mult[18]  = fread_number( fp );
 pc_race_table[num].class_mult[19]  = fread_number( fp );
 pc_race_table[num].class_mult[20]  = fread_number( fp );
 pc_race_table[num].class_mult[21]  = fread_number( fp );
 pc_race_table[num].class_mult[22]  = fread_number( fp );
 pc_race_table[num].class_mult[23]  = fread_number( fp );
 pc_race_table[num].class_mult[24]  = fread_number( fp );
 pc_race_table[num].class_mult[25]  = fread_number( fp );
 pc_race_table[num].class_mult[26]  = fread_number( fp );
 pc_race_table[num].class_mult[27]  = fread_number( fp );
 pc_race_table[num].class_mult[28]  = fread_number( fp );
 pc_race_table[num].class_mult[29]  = fread_number( fp );
 pc_race_table[num].class_mult[30]  = fread_number( fp );
 pc_race_table[num].class_mult[31]  = fread_number( fp );
 pc_race_table[num].class_mult[32]  = fread_number( fp );
 pc_race_table[num].class_mult[33]  = fread_number( fp );
 pc_race_table[num].class_mult[34]  = fread_number( fp );
 pc_race_table[num].class_mult[35]  = fread_number( fp );
 pc_race_table[num].max_stats[0]    = fread_number( fp );
 pc_race_table[num].max_stats[1]    = fread_number( fp );
 pc_race_table[num].max_stats[2]    = fread_number( fp );
 pc_race_table[num].max_stats[3]    = fread_number( fp );
 pc_race_table[num].max_stats[4]    = fread_number( fp );
 pc_race_table[num].max_stats[5]    = fread_number( fp );
 pc_race_table[num].max_stats[6]    = fread_number( fp );
 pc_race_table[num].min_stats[0]    = fread_number( fp );
 pc_race_table[num].min_stats[1]    = fread_number( fp );
 pc_race_table[num].min_stats[2]    = fread_number( fp );
 pc_race_table[num].min_stats[3]    = fread_number( fp );
 pc_race_table[num].min_stats[4]    = fread_number( fp );
 pc_race_table[num].min_stats[5]    = fread_number( fp );
 pc_race_table[num].min_stats[6]    = fread_number( fp );
 pc_race_table[num].size            = fread_number( fp );
 pc_race_table[num].alignment[0]    = fread_number( fp );
 pc_race_table[num].alignment[1]    = fread_number( fp );
 pc_race_table[num].alignment[2]    = fread_number( fp );
 pc_race_table[num].ethos[0]        = fread_number( fp );
 pc_race_table[num].ethos[1]        = fread_number( fp );
 pc_race_table[num].ethos[2]        = fread_number( fp );
 pc_race_table[num].disabled        = fread_number( fp );
//Close Connection to file.
 fclose (fp);
 return;
}
