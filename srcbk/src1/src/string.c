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

/***************************************************************************
 *  File: string.c                                                         *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.                *
 *                                                                         *
 ***************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "tables.h"
#include "olc.h"

char *numlineas( char * );
char * del_last_line( char * string);

/*****************************************************************************
 Name:		string_edit
 Purpose:	Clears string and puts player into editing mode.
 Called by:	none
 ****************************************************************************/
void string_edit( CHAR_DATA *ch, char **pString )
{
    send_to_char( "-========- Entering EDIT Mode -=========-\n\r", ch );
    send_to_char( "    Type .h on a new line for help\n\r", ch );
    send_to_char( "   Terminate with a @ on a blank line.\n\r", ch );
    send_to_char( "-=======================================-\n\r", ch );

    if ( *pString == NULL )
    {
        *pString = str_dup( "" );
    }
    else
    {
        **pString = '\0';
    }

    ch->desc->pString = pString;

    return;
}


void string_append( CHAR_DATA *ch, char **pString )
{
send_to_char( "{b-=======- {WEntering APPEND Mode {b-========-{x\n\r",ch);
send_to_char( "{b    Type {W.h{b on a new line for help{x\n\r", ch );
send_to_char( "{b   Terminate with {W@{b on a blank line.{x\n\r", ch );
send_to_char( "{b-=======================================-{B\n\r", ch );

    if ( *pString == NULL )
    {
        *pString = str_dup( "" );
    }
  send_to_char( numlineas(*pString), ch );
/*  if ( *(*pString + strlen( *pString ) - 1) != '\r' )
       send_to_char( "\n\r", ch ); */

    ch->desc->pString = pString;

    return;
}


/*****************************************************************************
 Name:		string_replace
 Purpose:	Substitutes one string for another.
 Called by:	string_add(string.c) (aedit_builder)olc_act.c.
 ****************************************************************************/
char * string_replace( char * orig, char * old, char * new )
{
    char xbuf[MAX_STRING_LENGTH];
    int i;

    xbuf[0] = '\0';
    strcpy( xbuf, orig );
    if ( strstr( orig, old ) != NULL 
     && strlen( orig )
      - strlen( strstr( orig, old ) )
      + strlen( new ) < MAX_STRING_LENGTH )
    {
        i = strlen( orig ) - strlen( strstr( orig, old ) );
        xbuf[i] = '\0';
        strcat( xbuf, new );
        strcat( xbuf, &orig[i+strlen( old )] );
        free_string( orig );
    }

    return str_dup( xbuf );
}

/*****************************************************************************
 Name:          string_replace2
 Purpose:       Replaces a line of text.
 Called by:     string_add(string.c) (aedit_builder)olc_act.c.

****************************************************************************/
char   *string_replace2 (CHAR_DATA * ch, char *orig, int line, char *new)
{
        char   *rdesc;
        char    xbuf[MAX_STRING_LENGTH];
        int     current_line = 1;
        int     i;
        bool    fReplaced = FALSE;

        xbuf[0] = '\0';
        strcpy (xbuf, orig);

        i = 0;

        for (rdesc = orig; *rdesc; rdesc++)
        {
                if (current_line == line && !fReplaced)
                {
                        xbuf[i] = '\0';

                        if (*new)
                                strcat(xbuf, new);
                        strcat(xbuf, "\n\r");
                        fReplaced = TRUE;
                }

                if (current_line == line + 1)
                {
          strcat(xbuf, &orig[i]);
                        free_string (orig);

                        send_to_char ("Line replaced.\n\r", ch);

                        return str_dup (xbuf);
                }

                i++;

                if (*rdesc == '\r')
                        current_line++;
        }

        if (current_line - 1 != line)
        {
                send_to_char ("That line does not exist.\n\r", ch);
            return str_dup (xbuf);
        }

        free_string (orig);
        send_to_char ("Line replaced.\n\r", ch);

        return str_dup (xbuf);
}

char *getline( char *str, char *buf )
{
        int tmp = 0;
        bool found = FALSE;

        while ( *str )
        {
                if ( *str == '\n' )
                {
                        found = TRUE;
                        break;
                }

                buf[tmp++] = *(str++);
        }

        if ( found )
 {
                if ( *(str + 1) == '\r' )
                        str += 2;
                else
                        str += 1;
        } 

        buf[tmp] = '\0';

        return str;
}

char *numlineas( char *string )
{
        int cnt = 1;
        static char buf[MAX_STRING_LENGTH*2];
        char buf2[MAX_STRING_LENGTH], tmpb[MAX_STRING_LENGTH];

        buf[0] = '\0';

        while ( *string )
        {
        string = getline( string, tmpb );
                xprintf( buf2, "%2d. %s\n\r", cnt++, tmpb );
                strcat( buf, buf2 );
        }

        return buf;
}

/*****************************************************************************
 Name:          string_insertline
 Purpose:       Inserts a line, blank or containing text.
 Called by:     string_add(string.c) (aedit_builder)olc_act.c.

****************************************************************************/
char   *string_insertline (CHAR_DATA * ch, char *orig, int line, char
*addstring)
{
        char   *rdesc;
        char    xbuf[MAX_STRING_LENGTH];
        int     current_line = 1;
        int     i;

        xbuf[0] = '\0';
        strcpy (xbuf, orig);
    i = 0;

        for (rdesc = orig; *rdesc; rdesc++)
        {
                if (current_line == line)
                        break;

                i++;

                if (*rdesc == '\r')
                        current_line++;
        }

        if (!*rdesc)
        {
                send_to_char ("That line does not exist.\n\r", ch);
      return str_dup (xbuf);
        }

        xbuf[i] = '\0';

        if (*addstring)
                strcat(xbuf, addstring);
        strcat(xbuf, "\n\r");

        strcat(xbuf, &orig[i]);
        free_string (orig);

        send_to_char ("Line inserted.\n\r", ch);

        return str_dup (xbuf);
}

/*****************************************************************************
 Name:          string_deleteline
 Purpose:       Deletes a specified line of the string.
 Called by:     string_add(string.c) (aedit_builder)olc_act.c.

****************************************************************************/
char   *string_deleteline (char *orig, int line)
{
        char   *rdesc;
        char    xbuf[MAX_STRING_LENGTH];
        int     current_line = 1;
        int     i = 0;

        xbuf[0] = '\0';

        for (rdesc = orig; *rdesc; rdesc++)
        {
          if (current_line != line)
                {
                        xbuf[i] = *rdesc;
                        i++;
                }

                if (*rdesc == '\r')
                        current_line++;
        }

        free_string (orig);
        xbuf[i] = 0;

        return str_dup (xbuf);
}


/*****************************************************************************
 Name:		string_add
 Purpose:	Interpreter for string editing.
 Called by:	game_loop_xxxx(comm.c).
 ****************************************************************************/
void string_add( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    /*
     * Thanks to James Seng
     */
    smash_tilde( argument );

    if ( *argument == '.' )
    {
        char arg1 [MAX_INPUT_LENGTH];
        char arg2 [MAX_INPUT_LENGTH];
        char arg3 [MAX_INPUT_LENGTH];
	char tmparg3 [MAX_INPUT_LENGTH];

        argument = one_argument( argument, arg1 );

          if (!str_cmp (arg1, "./"))
                {
                        interpret(ch, argument);
                        send_to_char ("Command performed.\n\r", ch);
                        return;
                }

        argument = first_arg( argument, arg2, FALSE );
        argument = first_arg( argument, arg3, FALSE );
        strcpy( tmparg3, argument );
        if ( !str_cmp( arg1, ".c" ) )
        {
            send_to_char( "String cleared.\n\r", ch );
	    free_string(*ch->desc->pString);
           *ch->desc->pString = str_dup( "" );
            return;
        }

        if ( !str_cmp( arg1, ".s" ) )
        {
            send_to_char( "String so far:\n\r", ch );
	    send_to_char( numlineas(*ch->desc->pString), ch );
            return;
        }

        if ( !str_cmp( arg1, ".r" ) )
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char(
                    "usage:  .r \"old string\" \"new string\"\n\r", ch );
                return;
            }
            *ch->desc->pString =
                string_replace( *ch->desc->pString, arg2, arg3 );
            xprintf( buf, "'%s' replaced with '%s'.\n\r", arg2, arg3 );
            send_to_char( buf, ch );
            return;
        }
      if (!str_cmp (arg1, ".rl"))
                {
                        if (arg2[0] == '\0' || !is_number (arg2))
                        {
                                send_to_char (
                                   "usage:  .rl <line> <text>\n\r", ch);
                                return;
                        }

                        smash_tilde (arg3);     /* Just to be sure -- Hugin */
                        *ch->desc->pString =
               string_replace2 (ch, *ch->desc->pString, atoi(arg2), arg3);
                        return;
     }


                if (!str_cmp (arg1, ".i"))
                {
                        if (arg2[0] == '\0' || !is_number (arg2))
                        {
                                send_to_char (
                                   "usage:  .i <line> {text}\n\r", ch);
                                return;
                        }

                        smash_tilde (arg3);     /* Just to be sure -- Hugin */
                        *ch->desc->pString =
   string_insertline (ch, *ch->desc->pString, atoi (arg2), arg3);
                        return;
    }


                if (!str_cmp (arg1, ".dl"))
                {
                        if (arg2[0] == '\0' || !is_number (arg2))
                        {
                                send_to_char (
                                   "usage:  .dl <line>\n\r", ch);
                                return;
                        }

                        *ch->desc->pString =
                           string_deleteline (*ch->desc->pString, atoi (arg2));
                        xprintf (buf, "Line %d deleted.\n\r", atoi (arg2));
                        send_to_char (buf, ch);
                        return;
    }

	if ( !str_cmp( arg1, ".d" ) )
	{
	    *ch->desc->pString = del_last_line(*ch->desc->pString );
  	    send_to_char( " Line removed.\n\r", ch );
	    return;
	}

        if ( !str_cmp( arg1, ".f" ) )
        {
            *ch->desc->pString = format_string( *ch->desc->pString );
            send_to_char( "String formatted.\n\r", ch );
            return;
        }
        
        if ( !str_cmp( arg1, ".h" ) )
        {
            send_to_char( "Sedit help (commands on blank line):   \n\r", ch );
            send_to_char( ".r 'old' 'new'   - replace a substring \n\r", ch );
            send_to_char( "                   (requires '', \"\") \n\r", ch );
            send_to_char( ".h               - get help (this info)\n\r", ch );
            send_to_char( ".s               - show string so far  \n\r", ch );
            send_to_char( ".f               - (word wrap) string  \n\r", ch );
            send_to_char( ".c               - clear string so far \n\r", ch );
	    send_to_char( ".d               - delete last line    \n\r",ch );
            send_to_char(".dl <line>        - deletes a line      \n\r", ch );
            send_to_char(".i <line> {{text} - inserts a line	  \n\r",ch );
            send_to_char ("./ <command>      - do a regular command\n\r",ch );
 	    send_to_char(".rl <line> <text> - replaces a line     \n\r", ch );
            send_to_char( "@                - end string          \n\r", ch );
            return;
        }
            

        send_to_char( "SEdit:  Invalid dot command.\n\r", ch );
        return;
    }

    if ( *argument == '~' || *argument == '@' )
    {
      if ( ch->desc->editor == ED_MPCODE ) /* para los mobprogs */
        {
                MOB_INDEX_DATA *mob;
                int hash;
                PROG_LIST *mpl;
                PROG_CODE *mpc;
        EDIT_MPCODE(ch, mpc);

                if ( mpc != NULL )
                        for ( hash = 0; hash < MAX_KEY_HASH; hash++ )
                                for ( mob = mob_index_hash[hash]; mob; mob = mob->next )
                                        for ( mpl = mob->mprogs; mpl; mpl = mpl->next )
                                                if ( mpl->vnum == mpc->vnum )
                                                {
                                                        xprintf( buf,"Arreglando mob %d.\n\r", mob->vnum );
                                                        send_to_char( buf,ch );
                                                        mpl->code = mpc->code;
                                                }
        }

	if ( ch->desc->editor == ED_OPCODE ) /* for the objprogs */
	{
		OBJ_INDEX_DATA *obj;
		int hash;
		PROG_LIST *opl;
		PROG_CODE *opc;

		EDIT_OPCODE(ch, opc);

		if ( opc != NULL )
			for ( hash = 0; hash < MAX_KEY_HASH; hash++ )
				for ( obj = obj_index_hash[hash]; obj; obj = obj->next )
					for ( opl = obj->oprogs; opl; opl = opl->next )
						if ( opl->vnum == opc->vnum )
						{
							xprintf( buf, "Fixing object %d.\n\r", obj->vnum );
							send_to_char( buf, ch );
							opl->code = opc->code;
						}
	}

	if ( ch->desc->editor == ED_RPCODE ) /* for the roomprogs */
	{
		ROOM_INDEX_DATA *room;
		int hash;
		PROG_LIST *rpl;
		PROG_CODE *rpc;

		EDIT_RPCODE(ch, rpc);

		if ( rpc != NULL )
			for ( hash = 0; hash < MAX_KEY_HASH; hash++ )
				for ( room = room_index_hash[hash]; room; room = room->next )
					for ( rpl = room->rprogs; rpl; rpl = rpl->next )
						if ( rpl->vnum == rpc->vnum )
						{
							xprintf( buf, "Fixing room %d.\n\r", room->vnum );
							send_to_char( buf, ch );
							rpl->code = rpc->code;
						}
	}

        ch->desc->pString = NULL;
        return;
    }

    strcpy( buf, *ch->desc->pString );

    /*
     * Truncate strings to MAX_STRING_LENGTH.
     * --------------------------------------
     */
    if ( strlen( buf ) + strlen( argument ) >= ( MAX_STRING_LENGTH - 10 ) )
    {
        send_to_char( "String too long, last line skipped.\n\r", ch );

	/* Force character out of editing mode. */
        ch->desc->pString = NULL;
        return;
    }

    /*
     * Ensure no tilde's inside string.
     * --------------------------------
     */
    smash_tilde( argument );

    strcat( buf, argument );
    strcat( buf, "\n\r" );
    free_string( *ch->desc->pString );
    *ch->desc->pString = str_dup( buf );
    return;
}



/*
 * Thanks to Kalgen for the new procedure (no more bug!)
 * Original wordwrap() written by Surreality.
 */
/*****************************************************************************
 Name:		format_string
 Purpose:	Special string formating and word-wrapping.
 Called by:	string_add(string.c) (many)olc_act.c
 ****************************************************************************/
char *format_string( char *oldstring /*, bool fSpace */)
{
  char xbuf[MAX_STRING_LENGTH];
  char xbuf2[MAX_STRING_LENGTH];
  char *rdesc;
  int i=0;
  bool cap=TRUE;
  
  xbuf[0]=xbuf2[0]=0;
  
  i=0;
  
  for (rdesc = oldstring; *rdesc; rdesc++)
  {
    if (*rdesc=='\n')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc=='\r') ;
    else if (*rdesc==' ')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc==')')
    {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!'))
      {
        xbuf[i-2]=*rdesc;
        xbuf[i-1]=' ';
        xbuf[i]=' ';
        i++;
      }
      else
      {
        xbuf[i]=*rdesc;
        i++;
      }
    }
    else if (*rdesc=='.' || *rdesc=='?' || *rdesc=='!') {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!')) {
        xbuf[i-2]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i-1]=' ';
          xbuf[i]=' ';
          i++;
        }
        else
        {
          xbuf[i-1]='\"';
          xbuf[i]=' ';
          xbuf[i+1]=' ';
          i+=2;
          rdesc++;
        }
      }
      else
      {
        xbuf[i]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i+1]=' ';
          xbuf[i+2]=' ';
          i += 3;
        }
        else
        {
          xbuf[i+1]='\"';
          xbuf[i+2]=' ';
          xbuf[i+3]=' ';
          i += 4;
          rdesc++;
        }
      }
      cap = TRUE;
    }
    else
    {
      xbuf[i]=*rdesc;
      if ( cap )
        {
          cap = FALSE;
          xbuf[i] = UPPER( xbuf[i] );
        }
      i++;
    }
  }
  xbuf[i]=0;
  strcpy(xbuf2,xbuf);
  
  rdesc=xbuf2;
  
  xbuf[0]=0;
  
  for ( ; ; )
  {
    for (i=0; i<70; i++)
    {
      if (!*(rdesc+i)) break;
    }
    if (i<70)
    {
      break;
    }
    for (i=(xbuf[0]?69:66) ; i ; i--)
    {
      if (*(rdesc+i)==' ') break;
    }
    if (i)
    {
      *(rdesc+i)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"\n\r");
      rdesc += i+1;
      while (*rdesc == ' ') rdesc++;
    }
    else
    {
      bug ("No spaces (format_string)", 0);
      *(rdesc+68)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"-\n\r");
      rdesc += 69;
    }
  }
  while (*(rdesc+i) && (*(rdesc+i)==' '||
                        *(rdesc+i)=='\n'||
                        *(rdesc+i)=='\r'))
    i--;
  *(rdesc+i+1)=0;
  strcat(xbuf,rdesc);
  if (xbuf[strlen(xbuf)-2] != '\n')
    strcat(xbuf,"\n\r");

  free_string(oldstring);
  return(str_dup(xbuf));
}

/*****************************************************************************
 Name:          wrap_string
 Purpose:       String word-wrapping for those whose terms don't have it.
 Called by:     (many)act_comm.c (act_new)comm.c

****************************************************************************/

char   *wrap_string ( char *oldstring, int length )
{
        char    xbuf[MAX_STRING_LENGTH];
        static char    xbuf2[MAX_STRING_LENGTH];
        char   *rdesc;
        int     i = 0;
        int     end_of_line;

        if (!length)
        {
                strcpy (xbuf2, oldstring);
                return xbuf2;
        }
   xbuf[0] = xbuf2[0] = '\0';

        i = 0;

        rdesc = oldstring;

        for (;;)
        {
                end_of_line = length;
                for (i = 0; i < end_of_line; i++)
                {
                        if (*(rdesc + i) == '`')
                        {
                                end_of_line += 2;
                                i++;
                        }
                   if (!*(rdesc + i))
                                break;

                        if (*(rdesc + i) == '\r')
                                end_of_line = i;
                }
                if (i < end_of_line)
                {
                        break;
                }
                if (*(rdesc + i - 1) != '\r')
                {
                        for (i = (xbuf[0] ? (end_of_line - 1) : (end_of_line - 4)); i; i--)
                        {
                                if (*(rdesc + i) == ' ')
                                        break;
          }
                        if (i)
                        {
                                *(rdesc + i) = 0;
                                strcat(xbuf, rdesc);
                                strcat(xbuf, "\n\r");
                                rdesc += i + 1;
                                while (*rdesc == ' ')
                                        rdesc++;
                        }
                        else
                        {
                                bug ("No spaces",0);
                                *(rdesc + (end_of_line - 2)) = 0;
                                strcat(xbuf, rdesc);
                                strcat(xbuf, "-\n\r");
                                rdesc += end_of_line - 1;
           }
                }
                else
                {
                        *(rdesc + i - 1) = 0;
                        strcat(xbuf, rdesc);
                        strcat(xbuf, "\r");
                        rdesc += i;
                        while (*rdesc == ' ')
                                rdesc++;
                }
        }
        while (*(rdesc + i) && (*(rdesc + i) == ' ' ||
                  *(rdesc + i) == '\n' ||
                  *(rdesc + i) == '\r'))
                i--;
        *(rdesc + i + 1) = 0;
  strcat(xbuf, rdesc);
        if (xbuf[strlen (xbuf) - 2] != '\n')
                strcat(xbuf, "\n\r");
        strcpy(xbuf2, xbuf);
        return (xbuf2);
}

/*****************************************************************************
 Name:          spell_check
 Purpose:       Spell-check strings with ispell (*nix only)
 Called by:     (string_add)string.c

****************************************************************************/

void    spell_check (CHAR_DATA * ch, char *string)
{
        char    buf[MAX_STRING_LENGTH];
        char    newstr[MAX_STRING_LENGTH];
   char    line[MAX_STRING_LENGTH];
        char    chr;
        FILE   *fp;
        char   *rstr;
        int     i = 0;

        newstr[0] = 0;
        line[0] = 0;

        if (string == NULL)
                return;

        for (rstr = string; *rstr; rstr++)
        {
                if (*rstr != '\r' && *rstr != '~')
                {
                        if (*rstr == '`')
            {
                                rstr++;
                        }
                        else if (*rstr == '\n')
                        {
                                newstr[i] = ' ';
                                i++;
                        }
                        else
                        {
                                newstr[i] = *rstr;
                                i++;
                        }
                }
        }
        newstr[i] = 0;
    fp = fopen ("spell_check.txt", "w");
        fprintf (fp, "!\n\r%s\n\r", newstr);
        fclose (fp);
        xprintf (buf, "cat spell_check.txt | ispell -a -f spell_check.txt");
        system (buf);
        if ((fp = fopen ("spell_check.txt", "r")) != NULL)
        {
                i = 0;
                while (!feof (fp))
                {
                        chr = getc (fp);
                        line[i] = chr;
                        i++;
                }
                line[i] = 0;
                fclose (fp);
                system ("rm spell_check.txt");

                newstr[0] = 0;
                i = 0;
                for (rstr = str_dup (line); *rstr; rstr++)
                {
                        if (*rstr == '\n' && *(rstr + 1) == '\n')
                        {
                                rstr++;
                        }
                        else
                        {
                                if (*rstr == '\r')
                                        send_to_char ("`5*``", ch);
                                newstr[i] = *rstr;
                                i++;
                        }
                }
           free_string(rstr);
                newstr[i] = 0;
                strcat(newstr, "\n\r\n\r");
                send_to_char (newstr, ch);
        }
        return;
}



/*
 * Used above in string_add.  Because this function does not
 * modify case if fCase is FALSE and because it understands
 * parenthesis, it would probably make a nice replacement
 * for one_argument.
 */
/*****************************************************************************
 Name:		first_arg
 Purpose:	Pick off one argument from a string and return the rest.
 		Understands quates, parenthesis (barring ) ('s) and
 		percentages.
 Called by:	string_add(string.c)
 ****************************************************************************/
char *first_arg( char *argument, char *arg_first, bool fCase )
{
    char cEnd;

    while ( *argument == ' ' )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"'
      || *argument == '%'  || *argument == '(' )
    {
        if ( *argument == '(' )
        {
            cEnd = ')';
            argument++;
        }
        else cEnd = *argument++;
    }

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
    if ( fCase ) *arg_first = LOWER(*argument);
            else *arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( *argument == ' ' )
	argument++;

    return argument;
}




/*
 * Used in olc_act.c for aedit_builders.
 */
char * string_unpad( char * argument )
{
    char buf[MAX_STRING_LENGTH];
    char *s;

    s = argument;

    while ( *s == ' ' )
        s++;

    strcpy( buf, s );
    s = buf;

    if ( *s != '\0' )
    {
        while ( *s != '\0' )
            s++;
        s--;

        while( *s == ' ' )
            s--;
        s++;
        *s = '\0';
    }

    free_string( argument );
    return str_dup( buf );
}



/*
 * Same as capitalize but changes the pointer's data.
 * Used in olc_act.c in aedit_builder.
 */
char * string_proper( char * argument )
{
    char *s;

    s = argument;

    while ( *s != '\0' )
    {
        if ( *s != ' ' )
        {
            *s = UPPER(*s);
            while ( *s != ' ' && *s != '\0' )
                s++;
        }
        else
        {
            s++;
        }
    }

    return argument;
}

/*****************************
 Name:      del_last_line
 Purpose:   Removes last line from string
 Called by: many.
 ******************/
char * del_last_line( char * string)
{
	int len;
	bool found = FALSE;

	char xbuf[MAX_STRING_LENGTH];

	xbuf[0] = '\0';
	if (string == NULL || string[0] == '\0')
	return(str_dup(xbuf));
	
	strcpy(xbuf, string);

	for (len = strlen(xbuf); len > 0; len--)
	{
		if (xbuf[len] == '\r')
		{
			if (!found) /* back it up */
			{
			if ( len > 0)
				len--;
			found = TRUE;
			}
		    else /* found second one */
			{
			xbuf[len +1] = '\0';
			free_string(string);
			return( str_dup(xbuf));
			}
		}
	}
    xbuf[0] = '\0';
	free_string(string);
	return( str_dup(xbuf));
}
