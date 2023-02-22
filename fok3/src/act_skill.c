#if defined(macintosh)
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "magic.h"
#include "recycle.h"

/*
void do_skill_list(CHAR_DATA *ch, char *argument)
{
    BUFFER *output;
    char skill_list[LEVEL_HERO][MAX_STRING_LENGTH];
    char skill_columns[LEVEL_HERO];
    int sn,lev;
    bool found = FALSE;
    char buf[MAX_STRING_LENGTH];
    
    if (IS_NPC(ch))
      return;

    output = new_buf();

 
    for (lev = 0; lev < LEVEL_HERO; lev++)
    {
        skill_columns[lev] = 0;
        skill_list[lev][0] = '\0';
    }

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
      if (skill_table[sn].name == NULL )
        break;


      	if ((level = skill_table[sn].skill_level[ch->class]) <= LEVEL_HERO 
         && (skill_table[sn].spell_fun == spell_null))
	  {
		if (ch->level < level)
          		xprintf(buf,"{lW%-18s {C(-----/-----) (N/A){w      ", skill_table[sn].name);
        	else
          		xprintf(buf,"{W%-18s (Base/Base) {C%3d%%{w      ",skill_table[sn].name,
                                         ch->pcdata->learned[sn]);	  	
	  }
      {
        found = TRUE;
        lev = skill_table[sn].skill_level[ch->class];


        if (skill_list[lev][0] == '\0')
          xprintf(skill_list[lev],"\n\r{WLevel %2d: {w%s",lev,buf);
        else 
        {
          if ( ++skill_columns[lev] % 2 == 0)
            strcat(skill_list[lev],"\n\r          ");
          strcat(skill_list[lev],buf);
   }
      }
    }



    if (!found)
    {
      send_to_char("You know no skills.\n\r",ch);
      return;
    }

    for (lev = 0; lev < LEVEL_HERO; lev++)
      if (skill_list[lev][0] != '\0')
	add_buf(output,skill_list[lev]);
	add_buf(output,"\n\r");

	page_to_char(buf_string(output),ch);
	free_buf(output);
//        send_to_char(skill_list[lev],ch);

}
*/
