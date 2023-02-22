#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "merc.h"
#include "magic.h"
#include "fok_struct.h"
#include "tables.h"


int new_lookup_race(const char *strName)
{
// ** This command is a lookup and return an int with race number, and not change anything.

   int iRace;
   
	//Loop races
	for (iRace = 1; iRace < NEW_MAX_PC_RACE; iRace++)
	  {
       	if (LOWER(strName[0]) == LOWER(new_pc_race_table[iRace].name[0])
       	 &&  !str_prefix( strName,new_pc_race_table[iRace].name))
			return iRace; //Return if race found.
		
	  }
//Return if no race by name found.
  return -1;
}

int new_lookup_class(const char *strName)
{
// ** This command is a lookup and return an int with race number, and not change anything.

   int iClass;
   
	//Loop races
	for (iClass = 1; iClass < NEW_MAX_CLASS; iClass++)
	  {
       	if (LOWER(strName[0]) == LOWER(new_class_table[iClass].name[0])
       	 &&  !str_prefix( strName,new_class_table[iClass].name))
			return iClass; //Return if class found.
		
	  }
//Return if no class by name found.
  return -1;
}


void infomation_On_Race( CHAR_DATA *ch, int iRace )
{
// ** This command do send infomation and not change anything.

   char buf[MAX_STRING_LENGTH];
   int col=0;
   int iClass=0;
      
	xprintf( buf, "*** %s ***\n\r",new_pc_race_table[iRace].name);
	send_to_char( buf, ch );
	
	xprintf( buf, "Name:          %s\n\r",new_pc_race_table[iRace].who_name);
	send_to_char( buf, ch );
	
	xprintf( buf, "Short name:    %s\n\r",new_pc_race_table[iRace].short_name);
	send_to_char( buf, ch );
	
	xprintf( buf, "Hitpoint dice: %d\n\r",new_pc_race_table[iRace].hpdice);
	send_to_char( buf, ch );
	
	xprintf( buf, "Mana dice:     %d\n\r",new_pc_race_table[iRace].manadice);
	send_to_char( buf, ch );
	
	xprintf( buf, "Move dice:     %d\n\r",new_pc_race_table[iRace].movedice);
	send_to_char( buf, ch );
	
	xprintf( buf, "Strength:     %2d/%-2d\n\r",new_pc_race_table[iRace].min_stats[0],new_pc_race_table[iRace].max_stats[0]);
	send_to_char( buf, ch );
	
	xprintf( buf, "Intelligence: %2d/%-2d\n\r",new_pc_race_table[iRace].min_stats[1],new_pc_race_table[iRace].max_stats[1]);
	send_to_char( buf, ch );
	
	xprintf( buf, "Wisdom:       %2d/%-2d\n\r",new_pc_race_table[iRace].min_stats[2],new_pc_race_table[iRace].max_stats[2]);
	send_to_char( buf, ch );
	
	xprintf( buf, "Dexterity:    %2d/%-2d\n\r",new_pc_race_table[iRace].min_stats[3],new_pc_race_table[iRace].max_stats[3]);
	send_to_char( buf, ch );
	
	xprintf( buf, "Constitution: %2d/%-2d\n\r",new_pc_race_table[iRace].min_stats[4],new_pc_race_table[iRace].max_stats[4]);
	send_to_char( buf, ch );
	
	xprintf( buf, "Agility:      %2d/%-2d\n\r",new_pc_race_table[iRace].min_stats[5],new_pc_race_table[iRace].max_stats[5]);
	send_to_char( buf, ch );
	
	xprintf( buf, "Charisma:     %2d/%-2d\n\r",new_pc_race_table[iRace].min_stats[6],new_pc_race_table[iRace].max_stats[6]);
	send_to_char( buf, ch );
	
	xprintf( buf, "Size:         %s\n\r",size_table[new_pc_race_table[iRace].size].name);
	send_to_char( buf, ch );


	send_to_char( "Alignment:\n\r", ch );
	if (new_pc_race_table[iRace].alignment[0] == 1)
		send_to_char( "Lawful ", ch );
	if (new_pc_race_table[iRace].alignment[1] == 1)
		send_to_char( "Neutral ", ch );
	if (new_pc_race_table[iRace].alignment[2] == 1)
			send_to_char( "Chaotic ", ch );
	send_to_char( "\n\r", ch );

	send_to_char( "Ethos:\n\r", ch );
	if (new_pc_race_table[iRace].ethos[0] == 1)
		send_to_char( "Good ", ch );
	if (new_pc_race_table[iRace].ethos[1] == 1)
		send_to_char( "Neutral ", ch );
	if (new_pc_race_table[iRace].ethos[2] == 1)
			send_to_char( "Evil ", ch );
	send_to_char( "\n\r", ch );

	if (new_pc_race_table[iRace].disabled == 1)
		send_to_char( "Races is Enabled\n\r", ch );
	else
		send_to_char( "Races is Disabled\n\r", ch );
		

	send_to_char( "Classes:\n\r", ch );
				
	for (iClass = 1; iClass < NEW_MAX_CLASS -4; iClass++) 
	  {
		if(new_pc_race_table[iRace].class_mult[iClass] < 999)
		  {
			
			xprintf( buf, "%-25s [%3d%] ",new_class_table[iClass].name,new_pc_race_table[iRace].class_mult[iClass]);		  
			send_to_char( buf, ch );

			if ( ++col % 3 == 0)
				send_to_char("\n\r",ch);
		}
	  }

	if ( col % 3 != 0)
		send_to_char("\n\r",ch);
/*
    sh_int	hometown[10];
*/
}

void do_race( CHAR_DATA *ch, char *argument )
 { // Command show list and showinfomation races.

// ** This command do only send infomation and not change anything.

   char buf[MAX_STRING_LENGTH];
   char race[MAX_STRING_LENGTH];
   int col=0;
   int iRace=1;
   
	//Get race name
	argument = one_argument( argument, race ); 	

	if (!str_cmp(race,"config"))
	  {
		config_race(ch,argument );
		return;
 	  }
	//If no race name given list races.
	if ( race[0] == '\0')
	  {

		for (iRace = 1; iRace < NEW_MAX_PC_RACE; iRace++)
		  {
			xprintf( buf, "%-20s",new_pc_race_table[iRace].name);
			send_to_char( buf, ch );	
			if ( ++col % 4 == 0)
				send_to_char("\n\r",ch);
		  }	
 	   	send_to_char("\n\r",ch);
 	   	return;
 	   }

	//Lookup rase.
	iRace = new_lookup_race(race);
	
	//Check is a real race name, if not show list of races
	if (iRace == -1)
	  {
		do_race(ch,"");
		return;
	  }
	
	//Show infomation on race.
	infomation_On_Race(ch,iRace);
	
  return;
}

void config_race( CHAR_DATA *ch, char *argument )
{
/*   char buf[MAX_STRING_LENGTH];
   char race[MAX_STRING_LENGTH];
   char cmd[MAX_STRING_LENGTH];
   char subcmd[MAX_STRING_LENGTH];   
   char strinput1[MAX_STRING_LENGTH];   
   char strinput2[MAX_STRING_LENGTH];   
   
   int numinput1=0;
   int numinput2=0;
   int iRace=1;
   
   
	//Get race name
	argument = one_argument( argument, race );
	argument = one_argument( argument, cmd );
	argument = one_argument( argument, subcmd );

	argument = one_argument( argument, strinput1 );
	argument = one_argument( argument, strinput2 );


	//Lookup rase.
	iRace = new_lookup_race(race);

	if (!str_cmp(cmd,"dice"))
	  {
		if (!is_number( strinput1 ))
			{ send_to_char("Value most be a number",ch); return;}
		
		numinput1 = atoi( strinput1 );

		if (!str_cmp(subcmd,"hp"))
			{ 
				new_pc_race_table[iRace].hpdice = numinput1; 
				xprintf( buf, "Hitpoint dice changed on %s to %2d.\n\r",race,numinput1);
			}
		
		else if (!str_cmp(subcmd,"mana"))
			{ 
				new_pc_race_table[iRace].manadice = numinput1; 
				xprintf( buf, "Manapoint dice changed on %s to %2d.\n\r",race,numinput1);
			}

		else if (!str_cmp(subcmd,"move"))
			{ 
				new_pc_race_table[iRace].movedice = numinput1; 
				xprintf( buf, "Movepoint dice changed on %s to %2d.\n\r",race,numinput1);
			}
		else
			xprintf( buf, "You only change hp/mana/move dice.");


		send_to_char( buf, ch );
		return;
	  }

	else if (!str_cmp(cmd,"stat"))
	  {

		if (!is_number( strinput1 ) && !is_number( strinput2 ))
			{ send_to_char("Value most be a number",ch); return;}


		numinput1 = atoi( strinput1 );
		numinput2 = atoi( strinput2 );
		
		if (numinput1 >= numinput2)
			{ send_to_char("Min stat must be less then max stat",ch); return;}


		
		if (!str_cmp(subcmd,"str"))
		  {
				new_pc_race_table[iRace].min_stats[0] = numinput1;
				new_pc_race_table[iRace].max_stats[0] = numinput2;
				xprintf( buf, "Strength changed on %s to %d/%d .\n\r",race,numinput1,numinput2);
		  }
		else if (!str_cmp(subcmd,"int"))
		  {
				new_pc_race_table[iRace].min_stats[1] = numinput1;
				new_pc_race_table[iRace].max_stats[1] = numinput2;
				xprintf( buf, "Intelligence changed on %s to %d/%d .\n\r",race,numinput1,numinput2);
		  }
		else if (!str_cmp(subcmd,"wis"))
		  {
				new_pc_race_table[iRace].min_stats[2] = numinput1;
				new_pc_race_table[iRace].max_stats[2] = numinput2;
				xprintf( buf, "Wisdom changed on %s to %d/%d .\n\r",race,numinput1,numinput2);
		  }
		else if (!str_cmp(subcmd,"dex"))
		  {
				new_pc_race_table[iRace].min_stats[3] = numinput1;
				new_pc_race_table[iRace].max_stats[3] = numinput2;
				xprintf( buf, "Dexterity changed on %s to %d/%d .\n\r",race,numinput1,numinput2);
		  }
		else if (!str_cmp(subcmd,"con"))
		  {
				new_pc_race_table[iRace].min_stats[4] = numinput1;
				new_pc_race_table[iRace].max_stats[4] = numinput2;
				xprintf( buf, "Constitution changed on %s to %d/%d .\n\r",race,numinput1,numinput2);
		  }
		else if (!str_cmp(subcmd,"agi"))
		  {
				new_pc_race_table[iRace].min_stats[5] = numinput1;
				new_pc_race_table[iRace].max_stats[5] = numinput2;
				xprintf( buf, "Agility changed on %s to %d/%d .\n\r",race,numinput1,numinput2);
		  }
		else if (!str_cmp(subcmd,"cha"))
		  {
				new_pc_race_table[iRace].min_stats[6] = numinput1;
				new_pc_race_table[iRace].max_stats[6] = numinput2;
				xprintf( buf, "Charisma changed on %s to %d/%d .\n\r",race,numinput1,numinput1);
		  }
		else
			xprintf( buf, "You only change str/int/wis/dex/con/agi/int.");


		send_to_char( buf, ch );
		return;
	  }
	 
	if (!str_cmp(cmd,"alignment"))
	  {

		if (!str_cmp(subcmd,"lawful"))
			{ 
				if (new_pc_race_table[iRace].alignment[0] == 1) 
					new_pc_race_table[iRace].alignment[0] = 0;
				else
					new_pc_race_table[iRace].alignment[0] = 1; 
				
				xprintf( buf, "Then can now %s be lawful.\n\r",new_pc_race_table[iRace].alignment[0] != 0 ? "" : "NOT");
			}
		
		else if (!str_cmp(subcmd,"neutral"))
			{ 
				if (new_pc_race_table[iRace].alignment[1] == 1) 
					new_pc_race_table[iRace].alignment[1] = 0;
				else
					new_pc_race_table[iRace].alignment[1] = 1; 
				
				xprintf( buf, "Then can now %s be neutral.\n\r",new_pc_race_table[iRace].alignment[1] != 0 ? "" : "NOT");
			}

		else if (!str_cmp(subcmd,"chaotic"))
			{ 
				if (new_pc_race_table[iRace].alignment[2] == 1) 
					new_pc_race_table[iRace].alignment[2] = 0;
				else
					new_pc_race_table[iRace].alignment[2] = 1; 
				
				xprintf( buf, "Then can now %s be chaotic.\n\r",new_pc_race_table[iRace].alignment[2] != 0 ? "" : "NOT");
			}
		else
			xprintf( buf, "You only change lawful/neutral/chaotic.");


		send_to_char( buf, ch );
		return;
	  }

	if (!str_cmp(cmd,"ethos"))
	  {

		if (!str_cmp(subcmd,"good"))
			{ 
				if (new_pc_race_table[iRace].ethos[0] == 1) 
					new_pc_race_table[iRace].ethos[0] = 0;
				else
					new_pc_race_table[iRace].ethos[0] = 1; 
				
				xprintf( buf, "Then can now %s be lawful.\n\r",new_pc_race_table[iRace].ethos[0] != 0 ? "" : "NOT");
			}
		else if (!str_cmp(subcmd,"neutral"))
			{ 
				if (new_pc_race_table[iRace].ethos[1] == 1) 
					new_pc_race_table[iRace].ethos[1] = 0;
				else
					new_pc_race_table[iRace].ethos[1] = 1; 
				
				xprintf( buf, "Then can now %s be neutral.\n\r",new_pc_race_table[iRace].ethos[1] != 0 ? "" : "NOT");
			}
		else if (!str_cmp(subcmd,"chaotic"))
			{ 
				if (new_pc_race_table[iRace].ethos[2] == 1) 
					new_pc_race_table[iRace].ethos[2] = 0;
				else
					new_pc_race_table[iRace].ethos[2] = 1; 
				
				xprintf( buf, "Then can now %s be evial.\n\r",new_pc_race_table[iRace].ethos[2] != 0 ? "" : "NOT");
			}
		else
			xprintf( buf, "You only change good/neutral/evilc.");


		send_to_char( buf, ch );
		return;
	  }

	if (!str_cmp(cmd,"class"))
	  {
		iClass = new_lookup_class(subcommand);
		
		if(iClass == -1)
		  {
			do_race(ch,"config");
			return;

		  }
		
	  }
*/
}
