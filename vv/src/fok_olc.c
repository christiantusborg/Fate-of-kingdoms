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
 * File: fok_olc.c, olc functions to fate of kingdoms IV                   *
 *                                                                         *
 * Function:                                                               *
 * void do_set_race (CHAR_DATA *ch ,char * argument)                       *
 ***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "tables.h"
#include "recycle.h" 
#include "fok_function.h" 
#include "fok_struct.h" 
#include "declaration.h"

void setup_race (CHAR_DATA *ch ,char * argument)
{
	char race_name[MAX_INPUT_LENGTH]; 
	char type[MAX_INPUT_LENGTH]; 
	char arg[MAX_INPUT_LENGTH]; 	
	char buf[MAX_INPUT_LENGTH]; 	
	char arg1[MAX_INPUT_LENGTH]; 		
	int race_no;
	int nr;
	int stat;
	
	argument = one_argument (argument, race_name);
	argument = one_argument (argument, type);
	argument = one_argument (argument, arg);
	argument = one_argument (argument, arg1);
	
	if (!type[0]) // we allway need 2 arguments
	{
 	 send_to_char("{C-------------------------------------------------------{X\n\r",ch);
 	 send_to_char ("Syntax:\n\r",ch);
 	 send_to_char (" setup race <race> <type> <argument>.\n\r",ch);
 	 send_to_char (" setup race <race> max <stat> <1-30>.\n\r",ch);
 	 send_to_char (" setup race <race> min <stat> <1-30>.\n\r",ch);
 	 send_to_char (" setup race <race> alignment <alignment>.\n\r",ch);
 	 send_to_char (" setup race <race> ethos <ethos>.\n\r",ch);
 	 send_to_char (" setup race <race> show.\n\r",ch); 	 
 	 send_to_char ("  Type: who,short,hpdice,manadice,movedice,size.\n\r",ch);
 	 send_to_char ("  Stat: Str,Int,Wis,Dex,Agi,Con,Cha.\n\r",ch);
 	 send_to_char ("  Alignment: Good, Neutral, Evil.\n\r",ch);
 	 send_to_char ("  Ethos: Lawful,Neutral,Chaotic.\n\r",ch); 	 
	 send_to_char("{C-------------------------------------------------------{X\n\r",ch);
	 return;
	}  
       //Find the Race
	for (race_no = 0; race_no < MAX_PC_RACE; race_no++)
         if (!str_cmp(race_name, pc_race_table[race_no].name))
          break;

	// Did we find one else out!
	if (race_no == MAX_PC_RACE)
	{
	 sprintf(buf, "No Race named '%s' exists, Help Race.\n\r", race_name);
	 send_to_char(buf,ch);
	 return;
	}

       // Edit what?
       if ( !str_cmp(type,"who") )
        {
         strcpy(pc_race_table[race_no].who_name,arg);
         sprintf(buf, "Race: %s who name set to %s", race_name,pc_race_table[race_no].who_name);
        }
       else if ( !str_cmp(type,"short") )
        {
         strcpy(pc_race_table[race_no].short_name,arg);
         sprintf(buf, "Race: %s short name set to %s", race_name,pc_race_table[race_no].short_name);
        }
       else if ( !str_cmp(type,"hpdice") )
        {
         if (!is_number(arg))
          return;
         nr = atoi (arg); 
         pc_race_table[race_no].hpdice = nr;
         sprintf(buf, "Race: %s hp dice set to %d", race_name,pc_race_table[race_no].hpdice);
        }
       else if ( !str_cmp(type,"manadice") )
        {
         if (!is_number(arg))
          return;
         
         nr = atoi (arg); 
         pc_race_table[race_no].manadice = nr;
         sprintf(buf, "Race: %s hp dice set to %d", race_name,pc_race_table[race_no].manadice);
        }
       else if ( !str_cmp(type,"movedice") )
        {
         if (!is_number(arg))
          return;
         nr = atoi (arg); 
         pc_race_table[race_no].movedice = nr;
         sprintf(buf, "Race: %s hp dice set to %d", race_name,pc_race_table[race_no].movedice);
        }	
       else if ( !str_cmp(type,"max") )
        {
         if (!is_number(arg1) )
          {
          send_to_char("A",ch);
          return;
         } 
         nr = atoi (arg1); 
         if( nr < 1 || nr > 30 )
           {
            send_to_char("B",ch);
            return;
           } 
          
         stat = lookup_stat(arg,STAT_SHORT);
         if (stat < 0)
          {
           sprintf(buf, "None stat by name %s Wrong%d\n\r",arg,stat);
           send_to_char(buf,ch);
           return;
          }
         if (pc_race_table[race_no].min_stats[stat] <= nr)
          {
           pc_race_table[race_no].max_stats[stat] = nr;
           sprintf(buf, "Race: %s stat max %s now set to %d\n\r", race_name,arg,pc_race_table[race_no].max_stats[stat]);
          }
         else
          {
          send_to_char("Max stat can NOT be less then MIN stat for a race\n\r",ch);
          }  
        }        
       else if ( !str_cmp(type,"min") )
        {
         if (!is_number(arg1))
          return;
         nr = atoi (arg1); 
         if( nr < 1 || nr > 30 )
          return;
         stat = lookup_stat(arg,STAT_SHORT);

         pc_race_table[race_no].min_stats[stat] = nr;
         sprintf(buf,  "Race: %s stat min %s now set to %d\n\r", race_name,arg,pc_race_table[race_no].min_stats[stat]);
        }        
       else if ( !str_cmp(type,"alignment") )
        {
         if ( !str_cmp(arg,"Good") )
          {
           if ( pc_race_table[race_no].alignment[0] == 0)
            {
             pc_race_table[race_no].alignment[0] = 1;
             sprintf(buf, "Race: %s can now be good aligned", pc_race_table[race_no].name);
            } 
           else
            {
             pc_race_table[race_no].alignment[0] = 0;
             sprintf(buf, "Race: %s can no longer be good aligned",pc_race_table[race_no].name);
            } 
           }            
         if ( !str_cmp(arg,"neutral") )
          {
           if ( pc_race_table[race_no].alignment[1] == 0)
            {
             pc_race_table[race_no].alignment[1] = 1;
             sprintf(buf, "Race: %s can now be neutral aligned",pc_race_table[race_no].name);
            } 
           else
            {
             pc_race_table[race_no].alignment[0] = 0;
             sprintf(buf, "Race: %s can no longer be neutral aligned",pc_race_table[race_no].name);
            } 
           }
         if ( !str_cmp(arg,"evil") )
          {
           if ( pc_race_table[race_no].alignment[1] == 0)
            {
             pc_race_table[race_no].alignment[1] = 1;
             sprintf(buf, "Race: %s can now have be evil aligned",pc_race_table[race_no].name);
            } 
           else
            {
             pc_race_table[race_no].alignment[0] = 0;
             sprintf(buf, "Race: %s can no longer be evil aligned",pc_race_table[race_no].name);
            } 
           }               
        }
       else if ( !str_cmp(type,"ethos") )
        {
         if ( !str_cmp(arg,"lawful") )
          {
           if ( pc_race_table[race_no].ethos[0] == 0)
            {
             pc_race_table[race_no].ethos[0] = 1;
             sprintf(buf, "Race: %s can now have ethos Lawful",pc_race_table[race_no].name);
            } 
           else
            {
             pc_race_table[race_no].ethos[0] = 0;
             sprintf(buf, "Race: %s can no longer have ethos Lawful",pc_race_table[race_no].name);
            } 
           }            
         if ( !str_cmp(arg,"neutral") )
          {
           if ( pc_race_table[race_no].ethos[1] == 0)
            {
             pc_race_table[race_no].ethos[1] = 1;
             sprintf(buf, "Race: %s can now have ethos neutral",pc_race_table[race_no].name);
            } 
           else
            {
             pc_race_table[race_no].ethos[0] = 0;
             sprintf(buf, "Race: %s can no longer have ethos neutral",pc_race_table[race_no].name);
            } 
           }
         if ( !str_cmp(arg,"chaotic") )
          {
           if ( pc_race_table[race_no].ethos[1] == 0)
            {
             pc_race_table[race_no].ethos[1] = 1;
             sprintf(buf, "Race: %s can now have ethos chaotic",pc_race_table[race_no].name);
            } 
           else
            {
             pc_race_table[race_no].alignment[0] = 0;
             sprintf(buf, "Race: %s can no longer have ethos chaotic",pc_race_table[race_no].name);
            } 
           }               
         }
       else if ( !str_cmp(type,"show") )  
        {
           send_to_char("{C-------------------------------------------------------\n\r",ch);
           sprintf (buf,"{WRace: %15s                      Race show\n\r",pc_race_table[race_no].name);
           send_to_char(buf,ch);
           send_to_char("{C-------------------------------------------------------\n\r{x",ch);
           sprintf (buf, " Who name:    %10s     Alignment:\n\r",pc_race_table[race_no].who_name);
           send_to_char(buf,ch);
           sprintf (buf, " Short name:  %3s          - Good     %s \n\r",pc_race_table[race_no].short_name,pc_race_table[race_no].alignment[0]  ? "Yes":"No");
           send_to_char(buf,ch);
           sprintf (buf, " Size         %7s      - Neutral  %s \n\r",size_table[pc_race_table[race_no].size].name,pc_race_table[race_no].alignment[1] ? "Yes":"No");
           send_to_char(buf,ch);
           sprintf (buf, " Strength     %2d %2d        - Evil     %s \n\r",pc_race_table[race_no].min_stats[0],pc_race_table[race_no].max_stats[0],pc_race_table[race_no].alignment[2]  ? "Yes":"No");
           send_to_char(buf,ch);
           sprintf (buf, " Intelligence %2d %2d      Ethos:  \n\r",pc_race_table[race_no].min_stats[1],pc_race_table[race_no].max_stats[1]);
           send_to_char(buf,ch);
           sprintf (buf, " Wisdom       %2d %2d        - Lawful   %s \n\r",pc_race_table[race_no].min_stats[2],pc_race_table[race_no].max_stats[2],pc_race_table[race_no].ethos[0]  ? "Yes":"No");
           send_to_char(buf,ch);
           sprintf (buf, " Agility      %2d %2d        - Neutral  %s \n\r",pc_race_table[race_no].min_stats[3],pc_race_table[race_no].max_stats[3],pc_race_table[race_no].ethos[1]  ? "Yes":"No");
           send_to_char(buf,ch);
           sprintf (buf, " Dexterity    %2d %2d        - Chaotic  %s\n\r",pc_race_table[race_no].min_stats[4],pc_race_table[race_no].max_stats[4],pc_race_table[race_no].ethos[3]  ? "Yes":"No");
           send_to_char(buf,ch);
           sprintf (buf, " Constituion  %2d %2d      Dice \n\r",pc_race_table[race_no].min_stats[5],pc_race_table[race_no].max_stats[5]);
           send_to_char(buf,ch);
           sprintf (buf, " Charisma     %2d %2d        - Hp        %d\n\r",pc_race_table[race_no].min_stats[6],pc_race_table[race_no].max_stats[6],pc_race_table[race_no].hpdice);
           send_to_char(buf,ch);
           sprintf (buf, " Status     %2d             - Mana      %d\n\r",pc_race_table[race_no].disabled,pc_race_table[race_no].manadice);
           send_to_char(buf,ch);
           sprintf (buf, "                           - Move      %d \n\r",pc_race_table[race_no].movedice); 
           send_to_char(buf,ch);
           send_to_char("{C-------------------------------------------------------{X\n\r",ch);
           return;
        }
       else if ( !str_cmp(type,"glfgjdfl") )  
        {
        
        }
       else
       {
       //do none?
        setup_race(ch ,"");
        return;
       }

 // Send out of action to user
 send_to_char(buf,ch);
//Save the races! 
 save_raceses();

 return;
}
 
void setup_combination(CHAR_DATA *ch ,char * argument)
{
  char race_name[MAX_INPUT_LENGTH];
  char class_name[MAX_INPUT_LENGTH];
  char multi[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int class_no,race_no,nmulti;
	
  argument = one_argument (argument, race_name);
  argument = one_argument (argument, class_name);
  argument = one_argument (argument, multi);

  if (!multi[0])
   {
    send_to_char ("Syntax is: setup combi <race> <class> <penalty percent>.\n\r",ch);
    return;
   }        
   // find race
   for (race_no = 0; race_no < MAX_PC_RACE; race_no++)
    if (!str_cmp(race_name, pc_race_table[race_no].name))
     break;
   // find class
   for (class_no = 0; class_no < MAX_CLASS; class_no++)
    if (!str_cmp(class_name, class_table[class_no].name))
     break;
     
   // There is no race by that name OUT!
   if (race_no == MAX_PC_RACE)
    {
     sprintf(buf, "No race named '%s'.\n\r", race_name);
     send_to_char(buf,ch);
     return;
    }
  // There is no cless by that name OUT!
   if (race_no == MAX_CLASS)
    {
     sprintf(buf, "No class named '%s'.\n\r", class_name);
     send_to_char(buf,ch);
     return;
    }

   // did he make multi a number?
   if (!is_number(multi))
    {
     send_to_char("Range must be from -1 to 999 ( Can't be 0)",ch);     
     send_to_char("-1 is race can't be the class.",ch);
     return;    
    }  
   nmulti =   atoi (multi);
	
   //Make sure it ok multi
   if ( (nmulti < -1) || (nmulti > 999) || (nmulti == 0) )
    {
     send_to_char("Range must be from -1 to 999 ( Can't be 0)",ch);     
     send_to_char("-1 is race can't be the class.",ch);
     return;
    }

   //set the multi
   pc_race_table[race_no].class_mult[class_no] = nmulti;    	

   //Send final replay to user
   if  (nmulti > -1)
    sprintf(buf, "OK, %ss can now be %s and will have and penalty percent of %d.\n\r", pc_race_table[race_no].name,class_table[class_no].name,nmulti);
   else
    sprintf(buf, "OK, %ss can't be %s now.\n\r", pc_race_table[race_no].name,class_table[class_no].name);   
   
   // Save it!
   save_raceses();

 return;    
}
