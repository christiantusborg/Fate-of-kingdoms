/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include "Log.h"

void public_ch_socials ( CHAR_DATA *ch, char *argument, char *type, const int bitname,int auth)
{

	static char * const he_she  [] = { "it",  "he",  "she" };
	static char * const him_her [] = { "it",  "him", "her" };
	static char * const his_her [] = { "its", "his", "her" };

        
	char buf[MAX_INPUT_LENGTH + 200];
	char command[MAX_INPUT_LENGTH + 200];
	CHAR_DATA *victim;
	int cmd;
	bool found;
	char argx[MAX_INPUT_LENGTH];
	DESCRIPTOR_DATA *d;
	int counter;
	int count;
	char buf2x[MAX_STRING_LENGTH];
	argument = one_argument(argument,command);
	if (command[0] == '\0')
	  {
		send_to_char("{W<Channel> + is used for channel based socials.{x\n\r",ch);
		return;
	  }
    
	smash_percent(argument);
	found = FALSE;
	
	for (cmd = 0; social_table[cmd].name[0] != '\0'; cmd++)
	  {
		if (command[0] == social_table[cmd].name[0]
		&& !str_prefix( command,social_table[cmd].name ) )
		  {
			found = TRUE;
			break;
		  }
	  }
	
	if (!found)
	  {
		send_to_char("{WWhat kind of social is that?!?!{x\n\r",ch);
		return;
	 }

	one_argument(argument,argx);
	victim = NULL;

	if (argx[0] == '\0')
	  {
		sprintf(buf, "%s %s {x", type, social_table[cmd].char_no_arg );
		act_new(buf,ch,NULL,NULL,TO_CHAR,POS_DEAD);
		sprintf(buf, "%s %s {x",type, social_table[cmd].others_no_arg );
		
		for (d = descriptor_list; d != NULL; d = d->next)
		  {
			CHAR_DATA *vch;
			vch = d->original ? d->original : d->character;
			
			if (d->connected == CON_PLAYING 
			 && d->character != ch &&
			 !IS_SET(vch->comm,COMM_NOGOSSIP)
			 && !IS_SET(vch->comm,COMM_QUIET))
			  { 
				if ( (NEW_IS_AUTHORIZED( vch, auth)) 
				 &&  (bitname != COMM_NOKINGDOM || vch->kingdom == ch->kingdom) )
				act_new(buf,ch,NULL,vch,TO_VICT,POS_DEAD);
			  }
		  }
	  }
	else if ((victim = get_char_world(ch,argx)) == NULL || IS_NPC(victim))
	  {
		send_to_char("They aren't here.\n\r",ch);
		return;
	  }
	else if (victim == ch)
	  {
		sprintf(buf,"%s %s {x",type, social_table[cmd].char_auto);
		act_new(buf,ch,NULL,NULL,TO_CHAR,POS_DEAD);
		sprintf(buf,"%s %s {x",type, social_table[cmd].others_auto);
		
		for (d = descriptor_list; d != NULL; d = d->next)
		  {
			CHAR_DATA *vch;
			vch = d->original ? d->original : d->character;
			
			if (d->connected == CON_PLAYING
			 && d->character != ch
			 && !IS_SET(vch->comm,COMM_NOGOSSIP)
			 && !IS_SET(vch->comm,COMM_QUIET))
			  { 
			
			if ( (NEW_IS_AUTHORIZED( victim, auth)) 
				 &&  (bitname != COMM_NOKINGDOM || victim->kingdom == ch->kingdom) )
				act_new(buf,ch,NULL,vch,TO_VICT,POS_DEAD);
			  }
		  }
	  }                   
	else
	  {
		sprintf(buf,"%s{X %s {x", type, social_table[cmd].char_found);
		act_new(buf,ch,NULL,victim,TO_CHAR,POS_DEAD);
		sprintf(buf,"%s{X %s {x",type, social_table[cmd].vict_found);
		act_new(buf,ch,NULL,victim,TO_VICT,POS_DEAD);

		for (d=descriptor_list; d != NULL; d = d->next)
		  {
			CHAR_DATA *vch;
			vch = d->original ? d->original : d->character;
			
			if (d->connected == CON_PLAYING
			 && d->character != ch
			 && d->character != victim
			 && !IS_SET(vch->comm, COMM_NOGOSSIP)
			 && !IS_SET(vch->comm,COMM_QUIET))
			  {  
				if ( (NEW_IS_AUTHORIZED( vch, auth)) 
				 &&  (bitname != COMM_NOKINGDOM || victim->kingdom == ch->kingdom) )
				  {        
					sprintf(buf,"%s{X %s {x",type,social_table[cmd].others_found); 
					
					for (counter = 0; buf[counter+1] != '\0'; counter++)
					 {
						if (buf[counter] == '$' && buf[counter + 1] == 'N')
						  {
							strcpy(buf2x,buf);
							buf2x[counter] = '\0';
							strcat(buf2x,PERS(victim,vch));

							for (count = 0; buf[count] != '\0'; count++)
							  {
								buf[count] = buf[count+counter+2];
							  }
							strcat(buf2x,buf);
							strcpy(buf,buf2x);
						  }
						else if (buf[counter] == '$' && buf[counter + 1] == 'E')
						  {
							strcpy(buf2x,buf);
							buf2x[counter] = '\0';
							strcat(buf2x,he_she[URANGE(0, victim->sex, 2)]);

							for (count = 0; buf[count] != '\0'; count ++)
							  {
								buf[count] = buf[count+counter+2];
							  }
							strcat(buf2x,buf);
							strcpy(buf,buf2x);
						  }
            					else if (buf[counter] == '$' && buf[counter + 1] == 'M')
						  {     
							buf[counter] = 's';
							buf[counter + 1] = 's';
							strcpy(buf2x,buf);
							buf2x[counter] = '\0';
							strcat(buf2x,him_her[ URANGE(0,victim->sex, 2)]);

							for (count = 0; buf[count] != '\0'; count++)
							  {
								buf[count] = buf[count+counter+2];
							  }
							strcat(buf2x,buf);
							strcpy(buf,buf2x);
						  }
						else if (buf[counter] == '$' && buf[counter + 1] == 'S')
						  {     
							strcpy(buf2x,buf);
							buf2x[counter] = '\0';
							strcat(buf2x,his_her[URANGE(0, victim->sex, 2)]);

							for (count = 0;buf[count] != '\0'; count++)
							  {
								buf[count] = buf[count+counter+2];
							  }
							strcat(buf2x,buf);
							strcpy(buf,buf2x);
                
						  }
					  }  
					act_new(buf,ch,NULL,vch,TO_VICT,POS_DEAD);
				  }
			  }
		  }
		}
return;
}






void public_ch ( CHAR_DATA *ch, char *argument, char *type, const int bitname,int auth)
{ 
      
	char buf[MAX_INPUT_LENGTH + 200];
	char command[MAX_INPUT_LENGTH + 200];
	char *buf3;
	DESCRIPTOR_DATA *d;
	
	buf3 = str_dup(argument);



    
	if (argument[0] == '\0' )
	  {
		if (IS_SET(ch->comm,(bitname)))
		  {
			sprintf( buf, "%s channel is now ON.{x\n\r", type);
			send_to_char( buf ,ch);
			REMOVE_BIT(ch->comm,(bitname));
		  }
		else
		  {
			sprintf( buf, "%s channel is now OFF.{x\n\r", type);
			send_to_char(buf,ch);
			SET_BIT(ch->comm,(bitname));
		  }
	  }
	else  
	  {
		if (IS_SET(ch->comm,COMM_QUIET))
		  {
			send_to_char("You must turn off quiet mode first.\n\r",ch);
			return;
		  }
 
		if (IS_SET(ch->comm,COMM_NOCHANNELS))
		  {
			send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
			return;
		  }

               REMOVE_BIT(ch->comm,(bitname));


		argument = one_argument( argument, command );

		if (!str_cmp(command, "+"))
		  {
			public_ch_socials (ch,argument,type,bitname,auth);
			return;
		  }

		if (!str_cmp(command, "emote") || !str_cmp(command, "em") ||!str_cmp(command, "!"))
		  {
			if (argument[0] == '\0')
			  { 
				send_to_char("{WEmote what?{x\n\r", ch);
				 return;
			  }
			sprintf( buf, "%s %s {X%s{x\n\r", type,ch->name, argument );
			send_to_char( buf, ch);
			sprintf( buf, "%s $n {X%s{x", type, argument );
		  }
		else
		  {             
		      	sprintf( buf, "%s You say{X: '%s'{x\n\r", type, buf3 );
			send_to_char(buf,ch);
			sprintf( buf, "%s $n:{X '%s'{x", type, buf3 );
		  } 

		for ( d = descriptor_list; d != NULL; d = d->next )
		  {
			CHAR_DATA *victim;
			victim = d->original ? d->original : d->character;
			
			if ( d->connected == CON_PLAYING
			 && d->character != ch
			 && !IS_SET(victim->comm,(bitname))
			 && !IS_SET(victim->comm,COMM_QUIET) )
			  {  
				if ( (NEW_IS_AUTHORIZED( victim, auth)) 
				 &&  (bitname != COMM_NOKINGDOM || victim->kingdom == ch->kingdom) )
					act_new(buf,ch,NULL,victim,TO_VICT,POS_DEAD);
			  }
		 }
	}
}

void do_ooc( CHAR_DATA *ch, char *argument ) {
    public_ch( ch, argument, "{W[OOC]{w", COMM_NOQUESTION,auth_ooc);
}

void do_music( CHAR_DATA *ch, char *argument ) {
    public_ch( ch, argument, "{C[{cMUSIC{C]{w", COMM_NOMUSIC,auth_music);
}

void do_newbie( CHAR_DATA *ch, char *argument ) {
    public_ch( ch, argument, "{G[NEWBIE]{w", COMM_NONEWBIE,auth_newbie);
}

void do_questch( CHAR_DATA *ch, char *argument ) {
    public_ch( ch, argument, "{R[QUEST]{w", COMM_NOQUEST,auth_questtalk);
}

void do_gossip( CHAR_DATA *ch, char *argument ) {
    public_ch( ch, argument, "{M[GOSSIP]{w", COMM_NOGOSSIP,auth_gossip);
    Log_system (ch,argument,LOG_ROLEPLAY_GOSSIP,0,0);
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
        public_ch( ch, argument, "{W[{CImmortal{W]{w", COMM_NOWIZ,auth_immtalk);
}

void do_admintalk( CHAR_DATA *ch, char *argument )
{
	public_ch( ch, argument, "{W[{YAdministrator{W]{w", COMM_NOWIZ,auth_admint);
}

void do_kingdomchannel( CHAR_DATA *ch, char *argument )
{
	
    char buf[MAX_STRING_LENGTH];

    if ( !(ch->kingdom) )
      {
	send_to_char("{cYou are not in a Kingdom{x.\n\r",ch);
	return;
      }
     if(ch->sex == SEX_FEMALE)
    sprintf(buf, "{R%s{R",
	kingdom_table[ch->kingdom].rank_titlesf[ch->kingdom_rank]);
	else
    sprintf(buf, "{R%s{R",
	kingdom_table[ch->kingdom].rank_titles[ch->kingdom_rank]);

     if (IS_IMMORTAL(ch) && (!str_cmp (ch->name, "Kazmir") ) )
        sprintf(buf, "{RGod{R");	

	public_ch( ch, argument, buf, COMM_NOKINGDOM,auth_gd);
}

