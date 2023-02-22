/***************************************************************************
*     Fate 4.0 is copyright 2000-???? Christian Tusborg (Kazmir)           *
*     By using this code, you have agreed to follow the terms of the       *
*     Fate license.  The use of this code without the permission of the    *
*     author is prohibited.  You will be punished to the fullest extend of *
*     the law.								   *
*     									   *
*     Original Fate of Kingdom Mud (mud.mysteria.dk 4000)		   *
***************************************************************************/

/***************************************************************************
                         Damage message
****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "merc.h"
#include "fight.h"

#define MAX_DAMAGE_MESSAGE 45

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune )
{
    char buf1[256], buf2[256], buf3[256], buf4[256], buf5[256], pbuf1[256], pbuf2[256], pbuf3[256];
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;
    int pblood1, pblood2;
    int blood_vnum;
    OBJ_DATA *blood;
    
  if (ch == NULL || victim == NULL)
        return;

/*  adds his own damage messages */

if ( dam == 0 ) { vs = "miss{c"; vp = "misses";}
 else if ( dam <= 4 ) { vs = "{gnick{w"; vp = "{gnicks{w"; }
 else if ( dam <= 8 ) { vs = "{gscratch{w";vp = "{gscratches{w"; }
 else if ( dam <= 13 ) { vs = "{ggraze{w";vp = "grazes{w";}
 else if ( dam <=17 ) { vs = "{gbruise{w"; vp = "bruises{w"; }
 else if ( dam <=21 ) { vs = "{Ghit{w";vp = "{Ghits{w";}
 else if ( dam <=25 ) { vs = "{Ginjure{w"; vp = "{Ginjures{w"; }
 else if ( dam <=30 ) { vs = "{Gwound{w";vp = "{Gwounds{w";}
 else if ( dam <=35 ) { vs = "{Gmaul{w"; vp = "{Gmauls{w"; }
 else if ( dam <=40 ) { vs = "{Gdecimate{w"; vp = "{Gdecimates{w"; }
 else if ( dam <=47 ) { vs = "{Gdevastate{w";vp = "{Gdevastates{w";}
 else if ( dam <=52 ) { vs = "{rMUTILATE{w"; vp = "{rMUTILATES{w"; }
 else if ( dam <=57 ) { vs = "{rDISEMBOWEL{w"; vp = "{rDISEMBOWELS{w"; }
 else if ( dam <=65 ) { vs = "{rEVISCERATE{w";vp = "{rEVISCERATES{w";}
 else if ( dam <=75 ) { vs = "{rDISMEMBER{w";vp = "{rDISMEMBERS{w";}
 else if ( dam <= 95 ) { vs = "{rMASSACRE{w"; vp = "{rMASSACRES{w"; }
 else if ( dam <= 110 ) { vs = "{rMANGLE{w";vp = "{rMANGLES{w";}
 else if ( dam <= 135 ) { vs = "{R*DEMOLISH*{w";
vp = "{R*DEMOLISHES*{w"; }
 else if ( dam <= 150 ) { vs = "{R**SLAUGHTER**{w";
vp = "{R**SLAUGHTERS**{w"; }
 else if ( dam <= 170 ) { vs = "{R***OBLITERATE***{w";
vp = "{R***OBLITERATES***{w";}
 else if ( dam <= 200 ) { vs = "{r<={RCREMATE{r=>{w";
vp = "{r<={RCREMATES{r=>{w";}
 else if ( dam <= 250 ) { vs = "{r<=<={RANNIHILATE{r=>=>{w";
 vp = "{r<=<={RANNIHILATES{r=>=>{w"; }
 else if ( dam <= 300 ) { vs = "{r<<=<={RERADICATE{r=>=>>{w";
 vp = "{r<<=<={RERADICATES{r=>=>>{w"; }
 else if ( dam <= 375 ) { vs = "{r<=><=>{RPULVERIZE{r<=><=>{w";
 vp = "{r<=><=>{RPULVERIZES{r<=><=>{w"; }
 else if ( dam <= 450 ) { vs = "{r<=><=><=>{RVAPORIZE{r<=><=><=>{w";
 vp = "{r<=><=><=>{RVAPORIZES{r<=><=><=>{w"; }
 else if ( dam <= 525 ) { vs = "{r<==><==><==>{RNUKE{r<==><==><==>{w";
 vp = "{r<==><==><==>{RNUKES{r<==><==><==>{w"; }
 else { vs = "do {WUNSPEAKABLE{w things to";
 vp = "does {WUNSPEAKABLE{w things to"; }

     punct   = (dam <= 24) ? '.' : '!';
    sprintf(buf4, " {R({Y%d{R){w", dam);

    if ( dt == TYPE_HIT )
    {
	if (ch  == victim)
	{
	sprintf( buf1, "$n %s $melf%c",vp,punct);
	sprintf( buf2, "You %s yourself%c%s",vs,punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : ""));
	sprintf( buf5, "%s %s %s",ch->name,vp,victim->name);
	}
	else
	{
   	sprintf( buf1, "$n %s $N%c",  vp, punct );
        sprintf( buf2, "You %s $N%c%s", vs, punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ));
        sprintf( buf3, "$n %s you%c%s", vp, punct,
	((IS_SET(victim->act,PLR_AUTODAMAGE) && 
	IS_SET(ch->act,PLR_AUTODAMAGE))? buf4 : "" ));
        sprintf( buf5, "%s %s %s",ch->name,vp,victim->name);
	}
    }
    else
    {
	if ( dt >= 0 && dt < MAX_SKILL )
	    attack	= skill_table[dt].noun_damage;
	else if ( dt >= TYPE_HIT
	&& dt <= TYPE_HIT + MAX_DAMAGE_MESSAGE) 
	    attack	= attack_table[dt - TYPE_HIT].noun;
	else
	{
	    bug( "Dam_message: bad dt %d.{x", dt );
	    dt  = TYPE_HIT;
	    attack  = attack_table[0].name;
	}

	if (immune)
	{
	    if (ch == victim)
	    {
		sprintf(buf1,"$n is unaffected by $s own %s.",attack);
		sprintf(buf2,"Luckily, you are immune to that.");
		sprintf(buf5,"%s is unaffected by %s.",ch->name,attack);
	    }
	    else
	    {
	    	sprintf(buf1,"$N is unaffected by $n's %s!",attack);
	    	sprintf(buf2,"$N is unaffected by your %s!",attack);
	    	sprintf(buf3,"$n's %s is powerless against you.",attack);
		sprintf(buf5,"%s is unaffected by %s's %s!",victim->name,ch->name,attack);
	    }
	}
	else
	{
	    if (ch == victim)
	    {
		sprintf( buf1, "$n's %s %s $m%c",attack,vp,punct);
                sprintf( buf2, "Your %s %s you%c%s",attack,vp,punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ));
                sprintf( buf5, "%s's %s %s %s",ch->name,attack,vp,victim->name);
	    }
	    else
	    {
         sprintf( buf1, "$n's %s %s $N%c%s",  attack, vp, punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ) );
                sprintf( buf2, "Your %s %s $N%c%s",  attack, vp, punct,(IS_SET(ch->act,PLR_AUTODAMAGE) ? buf4 : "" ));
                sprintf( buf3, "$n's %s %s you%c%s", attack, vp, punct,
		((IS_SET(victim->act,PLR_AUTODAMAGE) &&
		IS_SET(ch->act,PLR_AUTODAMAGE))? buf4 : "" ));
                sprintf( buf5, "%s's %s %s %s",ch->name,attack,vp,victim->name);
	    }
	}
    }

  if (!IS_NPC( victim ))
   {
    pblood1 = ((victim->max_hit / 100) * 2 );
    pblood2 = ((victim->max_hit / 100) * 35);

    if ( (dam > pblood1) && (victim->hit < pblood2) )
     {
      blood_vnum = OBJ_VNUM_BLOOD;
      blood = create_object( get_obj_index(blood_vnum), victim->level);
      blood->timer = number_range(1,10);
      blood->cost  = 0;
      obj_to_room(blood, victim->in_room);
      
      sprintf( pbuf1, "{R%s's blood flies everywhere{X",victim->name);
      sprintf( pbuf2, "{R%s's blood flies everywhere{X",victim->name);
      sprintf( pbuf3, "{RYour blood flies everywhere{X");
      
      if (ch == victim)
       {
	act(pbuf1,ch,NULL,NULL,TO_ROOM);
	act(pbuf2,ch,NULL,NULL,TO_CHAR);
       }
      else
      {
       act( pbuf1, ch, NULL, victim, TO_NOTVICT);
       act( pbuf2, ch, NULL, victim, TO_CHAR);
       act( pbuf3, ch, NULL, victim, TO_VICT);
      }
       
     }
   }
    if (ch == victim)
    {
	act(buf1,ch,NULL,NULL,TO_ROOM);
	act(buf2,ch,NULL,NULL,TO_CHAR);
    }
    else
    {
    	act( buf1, ch, NULL, victim, TO_NOTVICT);
    	act( buf2, ch, NULL, victim, TO_CHAR);
    	act( buf3, ch, NULL, victim, TO_VICT);
    }
    return;
}
