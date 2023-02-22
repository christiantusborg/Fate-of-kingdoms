/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/**************************************************************************
			SOCIAL EDITING
**************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "recycle.h"


#define SOCIAL_FILE "../area/socials.txt" /* or whatever fits you */

#define CONST_SOCIAL /* remove this in Step 2 */

int maxSocial; /* max number of socials */
struct social_type *social_table;	   /* and social table */

void load_social (FILE *fp, struct social_type *social)
{
	social->name =          fread_string (fp);
	social->char_no_arg =   fread_string (fp);
	social->others_no_arg = fread_string (fp);
	social->char_found =    fread_string (fp);
	social->others_found =  fread_string (fp);
	social->vict_found =    fread_string (fp);
	social->char_auto =     fread_string (fp);
	social->others_auto =   fread_string (fp);
}

void load_social_table ()
{
	FILE *fp;
	int i;
	
	
	fp = fopen (SOCIAL_FILE, "r");
	
	if (!fp)
	{
		bug ("Could not open " SOCIAL_FILE " for reading.",0);
		exit(1);
	}
	
	fscanf (fp, "%d\n", &maxSocial);

	/* IMPORTANT to use malloc so we can realloc later on */
		
	social_table = malloc (sizeof(struct social_type) * (maxSocial+1));
	
	for (i = 0; i < maxSocial; i++)
		load_social (fp,&social_table[i]);

	/* For backwards compatibility */
			
	social_table[maxSocial].name = str_dup(""); /* empty! */		
		
	fclose (fp);
	
}

void save_social (const struct social_type *s, FILE *fp)
{
	/* get rid of (null) */
	fprintf (fp, "%s~\n%s~\n%s~\n%s~\n%s~\n%s~\n%s~\n%s~\n\n",
			       s->name 			 ? s->name  : "" , 
			       s->char_no_arg 	 ? s->char_no_arg   : "" , 
			       s->others_no_arg  ? s->others_no_arg : "" ,
			       s->char_found     ? s->char_found    : "" , 
			       s->others_found   ? s->others_found  : "" , 
			       s->vict_found     ? s->vict_found    : "" ,
			       s->char_auto      ? s->char_auto     : "" , 
			       s->others_auto    ? s->others_auto   : ""
			       );
}

void save_social_table()
{
	FILE *fp;
	int i;
	
	fp = fopen (SOCIAL_FILE, "w");
	
	if (!fp)
	{
		bug ("Could not open " SOCIAL_FILE " for writing.",0);
		return;
	}

	
	for (maxSocial = 0 ; social_table[maxSocial].name[0] ; maxSocial++); /* empty */
	
	fprintf (fp, "%d\n", maxSocial);
	
	for ( i = 0 ; i < maxSocial ; i++)
		save_social (&social_table[i], fp);
		
	fclose (fp);
}
/* Find a social based on name */ 
int social_lookup (const char *name)
{
	int i;
	
	for (i = 0; i < maxSocial ; i++)
		if (!str_cmp(name, social_table[i].name))
			return i;
			
	return -1;
}

/*
 * Social editting command
 */
void do_sedit (CHAR_DATA *ch, char *argument)
{
	char cmd[MAX_INPUT_LENGTH], social[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int iSocial;
	
	smash_tilde (argument);
	
	argument = one_argument (argument,cmd);
	argument = one_argument (argument,social);
	
	if (!cmd[0])
	{
		send_to_char ("Huh? Type HELP SEDIT to see syntax.\n\r",ch);
         send_to_char ("sedit delete         <social>\n\r",ch);
         send_to_char ("sedit new            <social>\n\r",ch);
         send_to_char ("sedit show           <social>\n\r",ch);
         send_to_char ("sedit [string-name]  <social> <string>\n\r",ch);
         send_to_char ("[string-name]-> cnoarg,onoarg,cfound,ofound,vfound,cself,oself\n\r",ch);
 	 return;
	}
		
	if (!social[0])
	{
		send_to_char ("What social do you want to operate on?\n\r",ch);
		return;
	}
	
	iSocial = social_lookup (social);
	
	if (str_cmp(cmd,"new") && (iSocial == -1))
	{
		send_to_char ("No such social exists.\n\r",ch);
		return;
	}

	if (!str_cmp(cmd, "delete")) /* Remove a social */
	{
		int i,j;
		struct social_type *new_table = malloc (sizeof(struct social_type) * maxSocial);
		
		if (!new_table)
		{
			send_to_char ("Memory allocation failed. Brace for impact...\n\r",ch);
			return;
		}
		
		/* Copy all elements of old table into new table, except the deleted social */
		for (i = 0, j = 0; i < maxSocial+1; i++)
			if (i != iSocial) /* copy, increase only if copied */
			{
				new_table[j] = social_table[i];
				j++;
			}
	
		free (social_table);
		social_table = new_table;
		
		maxSocial--; /* Important :() */
		
		send_to_char ("That social is history now.\n\r",ch);
				
	}
	
	else if (!str_cmp(cmd, "new")) /* Create a new social */
	{
		struct social_type *new_table;
		
		if (iSocial != -1)
		{
			send_to_char ("A social with that name already exists\n\r",ch);
			return;
		}
		
		/* reallocate the table */
		/* Note that the table contains maxSocial socials PLUS one empty spot! */
		
		maxSocial++;
		new_table = realloc (social_table, sizeof(struct social_type) * (maxSocial + 1));
		
		if (!new_table) /* realloc failed */
		{
			send_to_char ("Memory allocation failed. Brace for impact.\n\r",ch);
			return;
		}
		
		social_table = new_table;
		
		social_table[maxSocial-1].name = str_dup (social);
		social_table[maxSocial-1].char_no_arg = str_dup ("");
		social_table[maxSocial-1].others_no_arg = str_dup ("");
		social_table[maxSocial-1].char_found = str_dup ("");
		social_table[maxSocial-1].others_found = str_dup ("");
		social_table[maxSocial-1].vict_found = str_dup ("");
		social_table[maxSocial-1].char_auto = str_dup ("");
		social_table[maxSocial-1].others_auto = str_dup ("");
		
		social_table[maxSocial].name = str_dup (""); /* 'terminating' empty string */
		
		send_to_char ("New social added.\n\r",ch);
			
	}
	
	else if (!str_cmp(cmd, "show")) /* Show a certain social */
	{
		sprintf (buf, "{C-------------------------------------------------------\n\r"
		              "{WSocial: %15s                      sedit show\n\r"
		              "{C-------------------------------------------------------\n\r"
		              "{D(cnoarg) DO - you see:\n\r"
		              "{w  --->    %s\n\r"
		              "{D(onoarg) DO - others see:\n\r"
		              "{w  --->    %s\n\r"
		              "{D(cfound) DO SOMEONE - you see:\n\r"
		              "{w  --->    %s\n\r"
		              "{D(ofound) DO SOMEONE - others see:\n\r"
		              "{w  --->    %s\n\r"
		              "{D(vfound) DO SOMEONE - victim sees:\n\r"
		              "{w  --->    %s\n\r"
		              "{D(cself)  DO YOURSELF - you see:\n\r"
		              "{w  --->    %s\n\r"
		              "{D(oself)  DO YOURSELF - others see:\n\r"
		              "{w  --->    %s\n\r"
		              "{C-------------------------------------------------------\n\r",
		              
		              social_table[iSocial].name,
		              social_table[iSocial].char_no_arg,
		              social_table[iSocial].others_no_arg,
		              social_table[iSocial].char_found,
		              social_table[iSocial].others_found,
		              social_table[iSocial].vict_found,
		              social_table[iSocial].char_auto,
		              social_table[iSocial].others_auto);

		send_to_char (buf,ch);		          
		return; /* return right away, do not save the table */
	}
	
	else if (!str_cmp(cmd, "cnoarg")) /* Set that argument */
	{
		free_string (social_table[iSocial].char_no_arg);
		social_table[iSocial].char_no_arg = str_dup(argument);		

		if (!argument[0])
			send_to_char ("Character will now see nothing when this social is used without arguments.\n\r",ch);
		else
			printf_to_char (ch,"New message is now:\n\r%s\n\r", argument);
	}
	
	else if (!str_cmp(cmd, "onoarg"))
	{
		free_string (social_table[iSocial].others_no_arg);
		social_table[iSocial].others_no_arg = str_dup(argument);		

		if (!argument[0])
			send_to_char ("Others will now see nothing when this social is used without arguments.\n\r",ch);
		else
			printf_to_char (ch,"New message is now:\n\r%s\n\r", argument);
			
	}
	
	else if (!str_cmp(cmd, "cfound"))
	{
		free_string (social_table[iSocial].char_found);
		social_table[iSocial].char_found = str_dup(argument);		

		if (!argument[0])
			send_to_char ("The character will now see nothing when a target is found.\n\r",ch);
		else
			printf_to_char (ch,"New message is now:\n\r%s\n\r", argument);
			
	}
	
	else if (!str_cmp(cmd, "ofound"))
	{
		free_string (social_table[iSocial].others_found);
		social_table[iSocial].others_found = str_dup(argument);		

		if (!argument[0])
			send_to_char ("Others will now see nothing when a target is found.\n\r",ch);
		else
			printf_to_char (ch,"New message is now:\n\r%s\n\r", argument);
			
	}
	
	else if (!str_cmp(cmd, "vfound"))
	{
		free_string (social_table[iSocial].vict_found);
		social_table[iSocial].vict_found = str_dup(argument);		

		if (!argument[0])
			send_to_char ("Victim will now see nothing when a target is found.\n\r",ch);
		else
			printf_to_char (ch,"New message is now:\n\r%s\n\r", argument);
	}
	
	else if (!str_cmp(cmd, "cself"))
	{
		free_string (social_table[iSocial].char_auto);
		social_table[iSocial].char_auto = str_dup(argument);		

		if (!argument[0])
			send_to_char ("Character will now see nothing when targetting self.\n\r",ch);
		else
			printf_to_char (ch,"New message is now:\n\r%s\n\r", argument);

	}
	
	else if (!str_cmp(cmd, "oself"))
	{
		free_string (social_table[iSocial].others_auto);
		social_table[iSocial].others_auto = str_dup(argument);		

		if (!argument[0])
			send_to_char ("Others will now see nothing when character targets self.\n\r",ch);
		else
			printf_to_char (ch,"New message is now:\n\r%s\n\r", argument);
	}
	
	else
	{
		send_to_char ("Huh? Try HELP SEDIT.\n\r",ch);
		return;
	}
	
	/* We have done something. update social table */	
	save_social_table();
}
