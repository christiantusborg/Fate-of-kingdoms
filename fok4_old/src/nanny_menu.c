/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/****************************************************************************
				Menu
****************************************************************************/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>                /* OLC -- for close read write etc */
#include <stdarg.h>                /* printf_to_char */

#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"




void menu_main(CHAR_DATA *ch)
{
char buf[MAX_INPUT_LENGTH];

 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{WFate of Kingdoms IIV Main Menu:{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{W[{B1{W] {BEnter the land of {WFate of Kingdoms{B.{x\n\r",ch);
// send_to_char("{W[{B2{W] {BNote Write{B.{x\n\r",ch);
// send_to_char("{W[{B3{W] {BWrite Description{B.{x\n\r",ch);
// send_to_char("{W[{B4{W] {BWrite Background{B.{x\n\r",ch);
// send_to_char("{W[{B5{W] {BRead Fate of Kingdom story.{x\n\r",ch);
// send_to_char("{W[{B6{W] {BChange your password.{x\n\r",ch);
send_to_char("{WMore is comming soon.{x\n\r",ch);
// send_to_char("{W[{B7{W] {BConfigure In Real Life Data.{x\n\r",ch);

 if ( IS_IMMORTAL(ch) )
  {
   sprintf( buf,"{W[{BW{W] {BSet Invisible Level (%d).{x\n\r",ch->invis_level);
   send_to_char( buf,ch );
  } 
 send_to_char("{W[{BQ{W] {BQuit Logoff.{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("\n\r",ch);
 send_to_char("{WMake your selection: {x",ch);
 return;
}

void menu_notewrite(CHAR_DATA *ch)
{
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{WFate of Kingdoms IIV Note Write Menu:{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{W[{B1{W] {BAnnounce Board{B.{x\n\r",ch);
 send_to_char("{W[{B2{W] {BRoleplay Board{B.{x\n\r",ch);
 send_to_char("{W[{B3{W] {BPersonal Board{B.{x\n\r",ch);
 send_to_char("{W[{B4{W] {BSubmit{B.{x\n\r",ch);
 send_to_char("{W[{B5{W] {BIdea.{x\n\r",ch);
 send_to_char("{W[{B6{W] {BPlayer Kill.{x\n\r",ch);
 send_to_char("{W[{BB{W] {BBack to Main Menu.{x\n\r",ch); 
 send_to_char("{WMake your selection: {x",ch);
 return;
}

void menu_irl(CHAR_DATA *ch)
{
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{WFate of Kingdoms IIV IRL Date Menu:{x\n\r",ch);
 send_to_char("{W------------------------------------------------{x\n\r",ch);
 send_to_char("{W[{B1{W] {BReal Name{B.{x\n\r",ch);
 send_to_char("{W[{B2{W] {BE-mail address{B.{x\n\r",ch);
 send_to_char("{W[{B3{W] {BICQ{B.{x\n\r",ch);
 send_to_char("{W[{B4{W] {BPassword Remember Word.{x\n\r",ch);
 send_to_char("{W[{BB{W] {BBack to Main Menu.{x\n\r",ch); 
 send_to_char("{WMake your selection: {x",ch);
 return;
}
/*Race Class stuff*/

/*
case CON_NOTEWRITE:
switch( argument[0])
  {
   case '1':
   //Announce
   return;

   case '2':
    //RP
   return; 
    
   case '3':
    //Personal
   return; 
       
   case '4':
    //Submit
   return; 
          
   case '5':
    //Idea
   return; 
             
   case '6':
    //pk
   return;              
             
   case 'B': case 'b':
//    d->connected = CON_MAINMENU;
//    do_clear_login(ch);
//    menu_main(ch);

        if ( IS_IMMORTAL(ch) )
        {   
            do_help( ch, "imotd" );
            d->connected = CON_READ_IMOTD;
        }
        else
        {
            do_help( ch, "motd" );
            d->connected = CON_READ_MOTD;
        }  

   break;   

   default:
    do_clear_login(ch);
    menu_notewrite(ch);
    d->connected = CON_NOTEWRITE;
   break;                 
}
break;   

case CON_MAINMENU:

switch( argument[0])
  {
   case '1':
	if ( IS_IMMORTAL(ch) )
	{
	    do_help( ch, "imotd" );
	    d->connected = CON_READ_IMOTD;
 	}
	else
	{
	    do_help( ch, "motd" );
	    d->connected = CON_READ_MOTD;
	}
   break; 

   case '2':
    do_clear_login(ch);
    menu_notewrite(ch);
    d->connected = CON_NOTEWRITE;
   return; 
    
   case '3':
    send_to_char("{WFunction Not Ready (Description).{x\n\r",ch);
   return; 
       
   case '4':
    send_to_char("{WFunction Not Ready (Background).{x\n\r",ch);
   return; 
          
   case '5':
    send_to_char("{WFunction Not Ready (Story).{x\n\r",ch);
   return; 
             
   case '6':
    send_to_char("{WFunction Not Ready (New Password).{x\n\r",ch);
   return;              
             
   case '7':
    do_clear_login(ch);
    menu_irl(ch);
    d->connected = CON_IRL;
   break;
   
   case 'W': case 'w':
     if ( IS_IMMORTAL(ch) )
      {
       send_to_char("{WSelect your invis level:.{x\n\r",ch);
       do_clear_login(ch);
       d->connected = CON_SETWIZILEVEL;
      } 
   break;   
                 
  case 'Q': case 'q':                 
    close_socket( d );
   break;

}    	
break;

case CON_SETWIZILEVEL:
  
if ((arg[0] == 'B') || (arg[0] == 'b'))
 {
  menu_main(ch);
  d->connected = CON_MAINMENU;
  break;
 }
   
  if (arg[0] == '\0')
   if (ch->invis_level)
    {
     ch->invis_level = 0;
     send_to_char ("Your fade back into existence.\n\r", ch);
     menu_main(ch);
     d->connected = CON_MAINMENU;
     break;
    }
   else
    {
     ch->invis_level = get_trust (ch);
     send_to_char ("You slowly vanish into thin air.\n\r", ch);
     do_clear_login(ch);
     menu_main(ch);
     d->connected = CON_MAINMENU;
     break;
    }
  else
   {
    level = atoi (arg);
     if (level < 0 || level > get_trust (ch))
       {
        send_to_char ("{WInvis level must be between 0 and your level.\n\r",ch);
        d->connected = CON_SETWIZILEVEL;
        break;
        }
      else
       {
        ch->reply = NULL;
        ch->invis_level = level;
        send_to_char ("You slowly vanish into thin air.\n\r", ch);
        do_clear_login(ch);
        menu_main(ch);
        d->connected = CON_MAINMENU;
        break;
        }   
   }
d->connected = CON_MAINMENU;
break;

case CON_IRL:
switch( argument[0])
  {
   case '1':
   //irl name
   return;

   case '2':
    //email
   return; 
    
   case '3':
    //icq
   return; 
       
   case '4':
    //password remember word
   return; 
          
            
   case 'B': case 'b':
    d->connected = CON_MAINMENU;
    do_clear_login(ch);
    menu_main(ch);
   break;   

   default:
    do_clear_login(ch);
    menu_notewrite(ch);
    d->connected = CON_IRL;
   break;                 
}
break;
*/
