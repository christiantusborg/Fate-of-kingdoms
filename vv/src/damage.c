/***************************************************************************
 *  Fate of Kingdom IV                                                     *
 *   By Chrisitan Tusborg (Kazmir).                                        *
 *									   *
 * The use of this code without the permission of Christian Tusborg        *
 * author is prohibited.  You will be punished to the fullest extend       *
 * of the law.                                                             *
 *                                                                         *
 *     Original Fate of Kingdom Mud (Mud.mysteria.dk:4000)		   *
 ***************************************************************************/
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "merc.h"
#include "fok_struct.h"
#include "declaration.h"

#define MAX_DAMAGE_MESSAGE 45

const struct battleskill_type battleskill_table[] =
{
    { "feeble"   },
    { "weak"     },
    { "poor"     },
    { ""         },
    { "fine"     },
    { "mighty"   },
    { "powerful" },
    { "supreme"  },
    { NULL       }
};

const struct battlepart_type battlepart_table[] =
{
    { "head"   },
    { "leg"    },
    { "body"   },
    { "arm"    },
    { NULL       }
};

void damage_message(CHAR_DATA *ch,CHAR_DATA *victim,int damage,int battleskill,int dt,int part)
{
    char buf1[256], buf2[256], buf3[256], buf4[256], buf5[256], pbuf1[256], pbuf2[256], pbuf3[256];
    const char *vs_damgetype;
    const char *vs_battleskill;
    const char *vs_battlepart;
    const char *attack;
    int damage_percent;
    char punct;
    int pblood1, pblood2;
    int blood_vnum;
    OBJ_DATA *blood;
    
  if (ch == NULL || victim == NULL)
        return;

 vs_battleskill = battleskill_table[battleskill].name;
 vs_battlepart  = battlepart_table[part].name;

	if ( dt >= 0 && dt < MAX_SKILL )
	    vs_damgetype	= skill_table[dt].abiliti_noun_damage;
	else if ( dt >= TYPE_HIT && dt <= TYPE_HIT + MAX_DAMAGE_MESSAGE) 
	    vs_damgetype	= attack_table[dt - TYPE_HIT].noun;
	else
	{
	    bug( "Dam_message: bad dt %d.{x", dt );
	    dt  = TYPE_HIT;
	    vs_damgetype  = attack_table[0].name;
	}

//   (Your feeble slash hits a training dummy)
 damage_percent = damage * 125 / victim->hit;  //(victim->hit * 100 / damage);
if (ch != victim)
 {
  sprintf( buf1, "%d and %d",damage_percent,damage);
   send_to_char(buf1,ch);
   send_to_char(buf1,victim);
  if (damage_percent < 1)
   {
    //To Ch
    sprintf( buf1, "Your %s %s nicks %s's %s",vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
    //To Victim
    sprintf( buf2, "%s's %s %s nicks your %s",ch->name,vs_battleskill,vs_damgetype,vs_battlepart);
    //To Room
    sprintf( buf3, "%s's %s %s nicks %s's %s",ch->name,vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
   } 
  else if (damage_percent < 2)
   {
    //To Ch
    sprintf( buf1, "Your %s %s scratches %s %s",vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
    //To Victim
    sprintf( buf2, "%s's %s %s scratches your %s",ch->name,vs_battleskill,vs_damgetype,vs_battlepart);
    //To Room
    sprintf( buf3, "%s's %s %s scratches %s %s",ch->name,vs_battleskill,vs_damgetype,victim->name,vs_battlepart); 
   }
  else if (damage_percent < 3)
   {
    //To Ch
    sprintf( buf1, "Your %s %s grazes %s's %s",vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
    //To Victim
    sprintf( buf2, "%s's %s %s grazes your %s",ch->name,vs_battleskill,vs_damgetype,vs_battlepart);
    //To Room
    sprintf( buf3, "%s's %s %s grazes %s's %s",ch->name,vs_battleskill,vs_damgetype,victim->name,vs_battlepart); 
   }
  else if (damage_percent < 5)
   {
    //To Ch
    sprintf( buf1, "Your %s %s hits %s's %s",vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
    //To Victim
    sprintf( buf2, "%s's %s %s hits your %s",ch->name,vs_battleskill,vs_damgetype,vs_battlepart);
    //To Room
    sprintf( buf3, "%s's %s %s hits %s's %s",ch->name,vs_battleskill,vs_damgetype,victim->name,vs_battlepart); 
   }
  else if (damage_percent < 8)
   {
    //To Ch
    sprintf( buf1, "Your %s %s hits %s's %s very hard",vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
    //To Victim
    sprintf( buf2, "%s's %s %s hits your %s very hard",ch->name,vs_battleskill,vs_damgetype,vs_battlepart);
    //To Room
    sprintf( buf3, "%s's %s %s hits %s's %s very hard.",ch->name,vs_battleskill,vs_damgetype,victim->name,vs_battlepart); 
   }
  else if (damage_percent < 10)
   {
    //To Ch
    sprintf( buf1, "Your %s %s wounds %s's %s",vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
    //To Victim
    sprintf( buf2, "%s's %s %s wounds your %s",ch->name,vs_battleskill,vs_damgetype,vs_battlepart);
    //To Room
    sprintf( buf3, "%s's %s %s wounds %s %s",ch->name,vs_battleskill,vs_damgetype,victim->name,vs_battlepart); 
   }
  else if (damage_percent < 15)
   {
    //To Ch
    sprintf( buf1, "Your %s %s seriously wounds %s's %s",vs_battleskill,vs_damgetype,victim->name,vs_battlepart);
    //To Victim
    sprintf( buf2, "%s's %s %s seriously wounds your %s",ch->name,vs_battleskill,vs_damgetype,vs_battlepart);
    //To Room
    sprintf( buf3, "%s's %s %s seriously wounds %s's %s",ch->name,vs_battleskill,vs_damgetype,victim->name,vs_battlepart); 
   } 
  else if (damage_percent < 20)
   {
    //To Ch
    sprintf( buf1, "Your %s %s enshrouds %s in a mist of blood!",vs_battleskill,vs_damgetype,victim->name);
    //To Victim
    sprintf( buf2, "%s's %s %s enshrouds you in a mist of blood!",ch->name,vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s enshrouds %s in a mist of blood!",ch->name,vs_battleskill,vs_damgetype,victim->name); 
   }  
  else if (damage_percent < 50)
   {
    //To Ch
    sprintf( buf1, "Your %s %s beats the crap out of %s",vs_battleskill,vs_damgetype,victim->name);
    //To Victim
    sprintf( buf2, "%s's %s %s beats the crap out of you",ch->name,vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s beats the crap out of %s.",ch->name,vs_battleskill,vs_damgetype,victim->name); 
   }
  else if (damage_percent < 75)
   {
    //To Ch
    sprintf( buf1, "Your %s %s nearly slaughters %s",vs_battleskill,vs_damgetype,victim->name);
    //To Victim
    sprintf( buf2, "%s's %s %s nearly slaughters you",ch->name,vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s nearly slaughters %s.",ch->name,vs_battleskill,vs_damgetype,victim->name); 
   }
  else if (damage_percent < 95)
   {
    //To Ch
    sprintf( buf1, "Your %s %s absolutely destroys %s",vs_battleskill,vs_damgetype,victim->name);
    //To Victim
    sprintf( buf2, "%s's %s %s absolutely destroys you",ch->name,vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s absolutely destroys %s.",ch->name,vs_battleskill,vs_damgetype,victim->name); 
   }
  else if (damage_percent < 100)
   {
    //To Ch
    sprintf( buf1, "Your %s %s almost kills him %s",vs_battleskill,vs_damgetype,victim->name);
    //To Victim
    sprintf( buf2, "%s's %s %s almost kills you",ch->name,vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s almost kills him %s",ch->name,vs_battleskill,vs_damgetype,victim->name); 
   }
  else
   {
    //To Ch
    sprintf( buf1, "Your %s %s beats %s to the ground",vs_battleskill,vs_damgetype,victim->name);
    //To Victim
    sprintf( buf2, "%s's %s %s beats you to the ground",ch->name,vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s beats %s to the ground",ch->name,vs_battleskill,vs_damgetype,victim->name);
   }
  } 
else
 {
  if (damage_percent < 1)
   {
    //To Self
    sprintf( buf1, "Your %s %s nicks You",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s nicks $mself",ch->name,vs_battleskill,vs_damgetype);   } 
  else if (damage_percent < 2)
   {
    //To Self
    sprintf( buf1, "Your %s %s scratches you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s scratches $mself",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 3)
   {
    //To self
    sprintf( buf1, "Your %s %s grazes you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s grazes $mself",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 5)
   {
    //To Self
    sprintf( buf1, "Your %s %s hits you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s hits $mself",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 8)
   {
    //To Ch
    sprintf( buf1, "Your %s %s hits you very hard",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s hits $mself very hard.",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 10)
   {
    //To Ch
    sprintf( buf1, "Your %s %s wounds you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s wounds $mself.",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 15)
   {
    //To Ch
    sprintf( buf1, "Your %s %s seriously wounds you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s seriously wounds $mself",ch->name,vs_battleskill,vs_damgetype); 
   } 
  else if (damage_percent < 20)
   {
    //To Ch
    sprintf( buf1, "Your %s %s enshrouds you in a mist of blood!",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s enshrouds $mself in a mist of blood!",ch->name,vs_battleskill,vs_damgetype); 
   }  
  else if (damage_percent < 50)
   {
    //To Ch
    sprintf( buf1, "Your %s %s beats the crap out of you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s beats the crap out of $mself.",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 75)
   {
    //To Ch
    sprintf( buf1, "Your %s %s nearly slaughters you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s nearly slaughters $mself.",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 95)
   {
    //To Ch
    sprintf( buf1, "Your %s %s absolutely destroys you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s absolutely destroys $mself.",ch->name,vs_battleskill,vs_damgetype); 
   }
  else if (damage_percent < 100)
   {
    //To Ch
    sprintf( buf1, "Your %s %s almost kills him you",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s almost kills him $mself",ch->name,vs_battleskill,vs_damgetype); 
   }
  else
   {
    //To Ch
    sprintf( buf1, "Your %s %s beats you to the ground",vs_battleskill,vs_damgetype);
    //To Room
    sprintf( buf3, "%s's %s %s beats $mself to the ground",ch->name,vs_battleskill,vs_damgetype); 
   }
 }  



  if (!IS_NPC( victim ))
   {
    pblood1 = ((victim->max_hit / 100) * 2 );
    pblood2 = ((victim->max_hit / 100) * 35);

    if ( (damage > pblood1) && (victim->hit < pblood2) )
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
       act( pbuf1, ch, NULL, victim, TO_ROOM);
       act( pbuf2, ch, NULL, victim, TO_CHAR);
       act( pbuf3, ch, NULL, victim, TO_VICT);
      }
       
     }
   }
    if (ch == victim)
    {
	act(buf3,ch,NULL,NULL,TO_ROOM);
	act(buf1,ch,NULL,NULL,TO_CHAR);
    }
    else
    {
    	act( buf3, ch, NULL, victim, TO_NOTVICT);
    	act( buf1, ch, NULL, victim, TO_CHAR);
    	act( buf2, ch, NULL, victim, TO_VICT);
    }
    return;
}
