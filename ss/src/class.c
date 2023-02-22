
#include <time.h>
#include <sys/types.h>
#include <errno.h>		
#include <unistd.h>		
#include <sys/time.h>
#include <ctype.h>		
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

/* Save this class */
void save_class (int num)
{
	FILE *fp;
	char buf[MAX_STRING_LENGTH];
	int lev, i;
	
	xprintf (buf, "%s%s", CLASS_DIR, class_table[num].name);
	if (!(fp = fopen (buf, "w")))
	{
xprintf (buf, "Could not open file %s in order to save class.",class_table[num].name);

		return;
	}
	
	for (lev = 0; lev < LEVEL_IMMORTAL; lev++)
		for (i = 0; i < MAX_SKILL; i++)
		{
			if (!skill_table[i].name || !skill_table[i].name[0])
				continue;
				
			if (skill_table[i].skill_level[num] == lev)
				fprintf (fp, "%d %s\n", lev, skill_table[i].name);
		}
	
	fprintf (fp, "-1"); /* EOF -1 */
	fclose (fp);
}



void save_classes()
{
	int i;
	
	for (i = 0; i < MAX_CLASS; i++) {
		save_class (i);
		save_web(i);
	}
}


/* Load a class */
void load_class (int num)
{
	char buf[MAX_STRING_LENGTH];
	int level,n;
	FILE *fp;
	
	xprintf (buf, "%s%s", CLASS_DIR, class_table[num].name);
	
	if (!(fp = fopen (buf, "r")))
	{
xprintf (buf, "Could not open file %s in order to load class.",class_table[num].name);
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
			xprintf (buf2, "Class %s: unknown spell %s", class_table[num].name, buf);
			bug (buf2, 0);
		}
		else
			skill_table[n].skill_level[num] = level;

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
			skill_table[j].skill_level[i] = LEVEL_IMMORTAL;
		
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
  xprintf(buf, "Level range is from 0 to %d.\n\r",LEVEL_IMMORTAL);
	send_to_char(buf,ch);
		return;
	}
	
	
	if ( (sn = skill_lookup (skill_name)) == -1)
	{
 xprintf(buf, "There is no such spell/skill as '%s'.\n\r", skill_name);
	send_to_char(buf,ch);
		return;
	}
	
	for (class_no = 0; class_no < MAX_CLASS; class_no++)
		if (!str_cmp(class_name, class_table[class_no].who_name))
			break;
	
	if (class_no == MAX_CLASS)
	{
	xprintf(buf, "No class named '%s' exists. Use the 3-letter WHO names (Psi, Mag etc.)\n\r", class_name);
	send_to_char(buf,ch);
		return;
	}
	
	skill_table[sn].skill_level[class_no] = level;
	
	xprintf(buf, "OK, %ss will now gain %s at level %d%s.\n\r", class_table[class_no].name,
								skill_table[sn].name, level, level == LEVEL_IMMORTAL ? " (i.e. never)" : "");
	send_to_char(buf,ch);
	
	save_classes();
}


/* end */