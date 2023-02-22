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
/* WorldMaker by Kroudar, Fall97, mlkesl@stthomas.edu */
#include <stdio.h>
#include <stdlib.h>

void Xmain()
{
int WIDTH=160,
    HEIGHT=60,
    VNUM_START=20001, /* must NOT be zero */
    VNUM_END=WIDTH*HEIGHT+(VNUM_START-1),
    vnum,n,e,s,w,line_pos;

FILE *fp=fopen("newworld.are","w");

printf("#*********************************#\n");
printf("# GENERATING %3d BY %3d AREA FILE #\n",WIDTH,HEIGHT );
printf("#*********************************#\n");

fprintf(fp,
"\
#AREADATA\n\
Name Thera~\n\
Builders None~\n\
VNUMs %d %d\n\
Credits -=GLOBE=- The World of Thera~\n\
Security 1\n\
Worth 0\n\
Owner NONE~\n\
End\n\
",
VNUM_START,
VNUM_END);

fprintf(fp,"\n\n\n#MOBILES\n#0\n");
fprintf(fp,"\n\n\n#OBJECTS\n#0\n");
fprintf(fp,"\n\n\n#ROOMS\n");

   for ( vnum=VNUM_START ; vnum<=VNUM_END  ; vnum++)
   { /* for every room */

   n=vnum-WIDTH;
   s=vnum+WIDTH;
   e=vnum+1;
   w=vnum-1;

   /*where it is on the line 0 to (WIDTH-1)*/
   line_pos=(vnum-VNUM_START+1)%(WIDTH);
   if (line_pos==0) line_pos=WIDTH;

   /*north border*/
   if ( (vnum >= VNUM_START) && (vnum < VNUM_START+WIDTH) ) {   
      n=(VNUM_END-WIDTH)+(line_pos);
/*      printf("vnum %5d NORTH %5d   [line_pos %3d]\n",vnum,n,line_pos);*/
   }

   /*south border*/
   if ( (vnum > VNUM_END-WIDTH) && (vnum <= VNUM_END) ) {
      s=(VNUM_START)+(line_pos-1);
/*      printf("vnum %5d SOUTH %5d   [line_pos %3d]\n",vnum,s,line_pos);*/
   }
     
   /*east border*/
   if ( (vnum-VNUM_START+1)%(WIDTH)==0 ) {   
      e=vnum-WIDTH+1;
/*      printf("vnum %5d EAST  %5d   [line_pos %3d]\n",vnum,e,line_pos);*/
   }

   /*west border*/
   if ( (vnum-VNUM_START+1)%(WIDTH)==1 ) {   
      w=vnum+WIDTH-1;
/*      printf("vnum %5d WEST  %5d   [line_pos %3d]\n",vnum,w,line_pos);*/
   }

   fprintf(fp,"#%d\nNAME~\nDESC\n~\n0 1073741824 3\n",vnum);
   fprintf(fp,"D0\n~\n~\n0 0 %d\n",n);
   fprintf(fp,"D1\n~\n~\n0 0 %d\n",e);
   fprintf(fp,"D2\n~\n~\n0 0 %d\n",s);
   fprintf(fp,"D3\n~\n~\n0 0 %d\n",w);
   fprintf(fp,"S\n");

/*   printf("VNUM(%5d) N>%5d S>%5d E>%5d W>%5d  LINE_POS[%3d]\n",
	   vnum,n,s,e,w,line_pos);*/
   }     

fprintf(fp,"#0\n");
fprintf(fp,"\n\n\n#SPECIALS\nS\n");
fprintf(fp,"\n\n\n#RESETS\nS\n");
fprintf(fp,"\n\n\n#SHOPS\n0\n");
fprintf(fp,"\n\n\n#$\n");

}
