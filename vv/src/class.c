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

/* Online setting of skill/spell levels, 
 * (c) 1996 Erwin S. Andreasen <erwin@pip.dknet.dk>
 *
 */

#include "include.h" /* This includes all the usual include files */
#include "fok_struct.h"
/* Save this class */
void save_class (int num)
{
	FILE *fp;
	char buf[MAX_STRING_LENGTH];
	int lev, i;
	
	sprintf (buf, "%s%s", CLASS_DIR, class_table[num].name);
	if (!(fp = fopen (buf, "w")))
	{
sprintf (buf, "Could not open file %s in order to save class.",class_table[num].name);

		return;
	}
	
	for (lev = 0; lev < LEVEL_IMMORTAL; lev++)
		for (i = 0; i < MAX_SKILL; i++)
		{
			if (!skill_table[i].abiliti_name || !skill_table[i].abiliti_name[0])
				continue;
				
			if (skill_table[i].abiliti_level[num] == lev)
				fprintf (fp, "%d %s\n", lev, skill_table[i].abiliti_name);
		}
	
	fprintf (fp, "-1"); /* EOF -1 */
	fclose (fp);
}



void save_classes()
{
	int i;
	
	for (i = 0; i < MAX_CLASS; i++)
		save_class (i);
}


/* Load a class */
void load_class (int num)
{
	char buf[MAX_STRING_LENGTH];
	int level,n;
	FILE *fp;
	
	sprintf (buf, "%s%s", CLASS_DIR, class_table[num].name);
	
	if (!(fp = fopen (buf, "r")))
	{
sprintf (buf, "Could not open file %s in order to load class.",class_table[num].name);
		return;
	}
	
	fscanf (fp, "%d", &level);
	
	while (level != -1)
	{
		fscanf (fp, " %[^\n]\n", buf); /* read name of skill into buf */
		
		n = skill_lookup (buf); /* find index */
		
		if (n == -1)
		{
			char buf2[200];
			sprintf (buf2, "Class %s: unknown spell %s", class_table[num].name, buf);
			bug (buf2, 0);
		}
		else
			skill_table[n].abiliti_level[num] = level;

		fscanf (fp, "%d", &level);
	}
	
	fclose (fp);
}
	
void load_classes ()
{
	int i,j;

	for (i = 0; i < MAX_CLASS; i++)
	{
		for (j = 0; j < MAX_SKILL; j++)
			skill_table[j].abiliti_level[i] = LEVEL_IMMORTAL;
		
		load_class (i);
	}
}

/*  commented out, too many errors, command not needed */
void do_setskill (CHAR_DATA *ch ,char * argument)
{
	char class_name[MAX_INPUT_LENGTH], 
	skill_name[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int sn, level, class_no;
	
	argument = one_argument (argument, class_name);
	argument = one_argument (argument, skill_name);
	
	if (!argument[0])
	{
		
	send_to_char ("Syntax is: SETSKILL <class> <skill> <level>.\n\r",ch);
		return;
	}
	
	level = atoi (argument);
	
	if (!is_number(argument) || level < 0 || level > LEVEL_IMMORTAL)
	{
  sprintf(buf, "Level range is from 0 to %d.\n\r",LEVEL_IMMORTAL);
	send_to_char(buf,ch);
		return;
	}
	
	
	if ( (sn = skill_lookup (skill_name)) == -1)
	{
 sprintf(buf, "There is no such spell/skill as '%s'.\n\r", skill_name);
	send_to_char(buf,ch);
		return;
	}
	
	for (class_no = 0; class_no < MAX_CLASS; class_no++)
		if (!str_cmp(class_name, class_table[class_no].who_name))
			break;
	
	if (class_no == MAX_CLASS)
	{
	sprintf(buf, "No class named '%s' exists. Use the 3-letter WHO names (Psi, Mag etc.)\n\r", class_name);
	send_to_char(buf,ch);
		return;
	}
	
	skill_table[sn].abiliti_level[class_no] = level;
	
	sprintf(buf, "OK, %ss will now gain %s at level %d%s.\n\r", class_table[class_no].name,
								skill_table[sn].abiliti_name, level, level == LEVEL_IMMORTAL ? " (i.e. never)" : "");
	send_to_char(buf,ch);
	
	save_classes();
}

/* end */
