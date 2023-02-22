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
/*
    To get this to work you have to either modify your area file format
    to have resets for random objects and your DB.C loader.  I'll leave
    that as an exercise for you.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/dir.h>
#include "merc.h"
#include "magic.h"

#define nelems(a) (sizeof (a)/sizeof (a)[0])

/*:: external function delcaration */
void    wear_obj        args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace) );

/* Calculate a meaningful modifier and amount */
void random_apply( OBJ_DATA *obj, CHAR_DATA *mob )
{
   static int attrib_types[] = { APPLY_STR, APPLY_DEX, APPLY_DEX, APPLY_INT,
      APPLY_INT, APPLY_WIS, APPLY_CON, APPLY_CON, APPLY_CON };
   static int power_types[] = { APPLY_MANA, APPLY_HIT, APPLY_MOVE };
   static int combat_types[] = { APPLY_HITROLL, APPLY_HITROLL, APPLY_DAMROLL,
      APPLY_SAVING_SPELL, APPLY_SAVING_SPELL, APPLY_SAVING_BREATH, APPLY_AC };

   AFFECT_DATA *paf = alloc_perm( sizeof(*paf) );
   paf->type	     = -1;
   paf->duration  = -1;
   paf->bitvector = 0;
   paf->next	     = obj->affected;
   obj->affected  = paf;
   switch (number_bits(2)) {
   case 0:
      paf->location  = attrib_types[number_range(0, nelems(attrib_types)-1)];
      paf->modifier  = 1;
      break;
   case 1:
      paf->location  = power_types[number_range(0, nelems(power_types)-1)];
      paf->modifier  = number_range(mob->level/2, mob->level); 
      break;
   case 2:
   case 3:
      paf->location  = combat_types[number_range(0, nelems(combat_types)-1)];
      paf->modifier  = number_range(1, mob->level/6+1); 
      break;
   }

   SET_BIT(obj->extra_flags, ITEM_MAGIC);

   /* Is item cursed? */
   if (number_percent() <= 5)
      {
      paf->modifier = -paf->modifier;
      SET_BIT(obj->extra_flags, ITEM_NODROP);
      if (number_percent() <= 15)
         SET_BIT(obj->extra_flags, ITEM_NOREMOVE);
      }
}

/* Jewelry stuff */
static char *adj1[] = { "splendid", "ancient", "dusty", "scratched",
   "flawed", "burnt", "heavy", "gilded", "spooky", "flaming", "plain",
   "ornate", "inscrutable", "obscene", "wrinkled", "reddish",
   "blue-hued", "dark green", "bright", "lustrous", "gleaming", 
   "coruscant", "lightening", "effulgent", "reflective", "dark",
   "pitch black", "lucrid", "shadowy", "umbrageous", "thin", "pellucid",
   "nacreous", "stained", "pearly", "showy", "flashy", "emblazoned",
   "chromatic", "old", "pallor", "etoliated", "dingy",
   "grey", "frosted", "sallow", "ivory tinted", "blood peppered",
   "kohl-black", "azure colored", "amber tinted", "citrin",
   "cobalt-colored", "drab", "ashy", "charred", "scarlet",
   "exquisite","variegated","irridescent","mottled","blood streaked",
   "rune engraved", "veined", "silver encrusted", "obsidian laced",
   "shining", "sleek", "slime covered", "scratched", "chipped", "etched"  
};
static char *adj2[] = { "diamond", "emerald", "topaz", "wooden", "jade",
   "gold", "onyx", "tin", "glass", "marble", "black", "granite",
"garnet", "amethyst", "bloodstone", "sardonyx", "tourmaline",
"coral","pearl" };

#define MASK_IGNORE     (1<<TAR_IGNORE)
#define MASK_OFFENSIVE  (1<<TAR_CHAR_OFFENSIVE)
#define MASK_DEFENSIVE  (1<<TAR_CHAR_DEFENSIVE)
#define MASK_SELF       (1<<TAR_CHAR_SELF)
#define MASK_INV        (1<<TAR_OBJ_INV)

/*#define CLASS_MAGE       0*/
/*#define CLASS_CLERIC     1*/

#define CLASS_MAGEA       CLASS_MAG
#define CLASS_CLERICA     CLASS_CLE

/* Returns a clerical or magical spell of the appropriate (masked) type */
int random_spell( int level, int mask, sh_int *type )
{
   extern	struct skill_type skill_table[MAX_SKILL];
/*  ::  ^----took out const */
   for ( ;; )
      {
      int skill_no = number_range(0, 250);
	/* supposed to go to max skill but i would rather not let
	   people have access to any of the new spells */
	/*eg... brand is sn=254 and remove brand is sn=255 */
      if (/*(skill_table[skill_no].skill_level[CLASS_MAGEA] <= level ||
           skill_table[skill_no].skill_level[CLASS_CLERICA] <= level) &&*/
         (skill_table[skill_no].spell_fun!=spell_null)&& /*an actual spell?*/
         (skill_table[skill_no].kingdom==KINGDOM_NONE)&& /*not a kingdom spell*/
          mask & (1<<skill_table[skill_no].target))  /* appropriate?*/
         {
         *type = skill_table[skill_no].target;
         return skill_no;
         }
      }
}

/* Wands/Staves/Potions/Pills and bags */
void wield_random_magic( CHAR_DATA *mob )
{
   int item_type = number_range(92,97/*92,97*/);
   OBJ_INDEX_DATA *pObjIndex = get_obj_index( item_type );
   OBJ_DATA *obj = create_object( pObjIndex, number_fuzzy( mob->level ) );
   sh_int type;
   int    n_adj1 = number_range(0, nelems(adj1)-1);
   int    n_adj2 = number_range(0, nelems(adj2)-1);

   /* Potion stuff */
   static char *potions[] = { "bottle", "snifter", "jar", "bulb", "potion" };

   /* Pill stuff */
   static char *pills[] = { "bag of dust", "honeycomb", "root", "leaf",
      "crystal", "powder packet", "seed" };

   /* Bag types (no wearable belts/backpacks... yet) */
   static char *skins[] = { "lamia skin", "cloth", "leather", "tough leather",
      "striated", "gilded" };
   static char *bags[] = { "bag", "valise", "pail", "basket", "jar",
      "box", "sack" };

   char buffer[256];
   char buf[MAX_STRING_LENGTH];
   switch (item_type) {
   case 92: /* scroll */
      xprintf(buffer, "%s scroll", adj1[n_adj1]);
      obj->value[0] = number_range(obj->level / 2+1, obj->level * 3 / 2+1); /* level */
      obj->value[0] = URANGE(1, obj->value[0], 36);
      obj->value[1] = random_spell(obj->value[0],
         MASK_IGNORE|MASK_OFFENSIVE|MASK_DEFENSIVE|MASK_SELF, &type); 
      if (number_percent() < 50)
         obj->value[2] = random_spell(obj->value[0], 1<<type, &type);
      break;
   case 93: /* wand */
      xprintf(buffer, "%s wand", adj1[n_adj1]);
      obj->value[0] = number_range(obj->level / 3 + 1, obj->level*3/2);  
      obj->value[0] = URANGE(1, obj->value[0], 36);
      obj->value[1] = number_fuzzy(obj->level / 2 + 3); /*max charges*/
      obj->value[2] = number_range(1, obj->value[1]); /*charges left*/
      obj->value[3] = random_spell(obj->value[0], MASK_OFFENSIVE,
&type); 
      break;
   case 94: /* staff */
      xprintf(buffer, "%s %s staff", adj1[n_adj1], adj2[n_adj2]);
      obj->value[0] = number_range(obj->level / 4 + 1, obj->level * 3 / 2);  /* level */
      obj->value[0] = URANGE(1, obj->value[0], 36);
      obj->value[1] = number_fuzzy(obj->level / 2 + 3); /* max charges */
      obj->value[2] = number_range(1, obj->value[1]);   /* charges left */
      obj->value[3] = random_spell(obj->value[0],
         MASK_IGNORE|MASK_OFFENSIVE|MASK_DEFENSIVE|MASK_SELF, &type); 
      break;
   case 95: /* potion */
      xprintf(buffer, "%s %s", adj1[n_adj1],
         potions[number_range(0, nelems(potions)-1)]);
      obj->value[0] = number_range(obj->level/2+1,obj->level*3/2+1); 
      obj->value[0] = URANGE(1, obj->value[0], 36);
      obj->value[1] = random_spell(obj->value[0], MASK_DEFENSIVE,&type); 
      if (number_percent() < 50)
         obj->value[2] = random_spell(obj->value[0], MASK_DEFENSIVE,&type);

      break;
   case 96: /* pill */
      xprintf(buffer, "%s %s", adj1[n_adj1],
         pills[number_range(0, nelems(pills)-1)]);
      obj->value[0] = number_range(obj->level / 2+1, obj->level * 3 / 2+1); /* level */
      obj->value[0] = URANGE(1, obj->value[0], 36);
      obj->value[1] = random_spell(obj->value[0], MASK_DEFENSIVE,
&type);
      if (number_percent() < 50)
         obj->value[2] = random_spell(obj->value[0], MASK_DEFENSIVE,
&type);
      break;
   case 97: /* bag */
      xprintf(buffer, "%s %s %s", adj1[n_adj1],
         skins[number_range(0, nelems(skins)-1)],
         bags[number_range(0, nelems(bags)-1)]);
      obj->value[0] = number_range(mob->level, mob->level * 10); /* weight */
      obj->value[1] = number_range(0, 1);
      obj->value[2] = -1;
      obj->value[3] = number_range(mob->level/10,mob->level/2); /*capacity*/
/*      obj->value[4] = 100;      */
      break;
   };

   /* Generate the description strings */
   free_string( obj->name );
   obj->name = str_dup( buffer );

   xprintf( buf, "a%s %s",
      (buffer[0] == 'a' || buffer[0] == 'e' || buffer[0] == 'i' ||
       buffer[0] == 'o' || buffer[0] == 'u') ? "n" : "", buffer );
   free_string( obj->short_descr );
   obj->short_descr = str_dup( buf );

   free_string( obj->description );
   xprintf( buf, "%s lies here.", obj->short_descr );
   obj->description = str_dup( buf );

   /* Put the item in the mob's inventory */
   obj_to_char( obj, mob );
}

/* Anything wearable, and trinkets */
void wield_random_armor( CHAR_DATA *mob )
{
   int item_type = number_range(4, MAX_WEAR - 3); /* template from LIMBO.ARE */
   OBJ_INDEX_DATA *pObjIndex = get_obj_index( item_type + 71 );
   OBJ_DATA *obj = create_object( pObjIndex, number_fuzzy( mob->level ) );
   int    n_adj1 = number_range(0, nelems(adj1)-1);
   int    n_adj2 = number_range(0, nelems(adj2)-1);
   char *name = "#random#";
   int chance=number_percent();

   /* Armor stuff */
   static char *armor_types[] = { "tin","copper","brass", 
      "russet", "pinchbeck" , "bronze", "chainmail",
      "ringmail", "ferric", "mithril" };
   static int armor_mul[] = { 2, 2, 3, 3, 4, 4, 4, 5, 6, 6 };
   static int armor_div[] = { 1, 2, 1, 2, 2, 1, 3, 3, 1, 3  };

   /* Weapon stuff */
   static char *weapon_types[] = { " heavy sword", " thin sword",
" gleaming broadsword", "n everyday sword", " well-forged sword",
" stout sword", " runed dagger", " thin wavy dagger", " heavy hammer", 
" massive mace", " bloodied mace", " thin whip" };
   static int weapon_dam[] = { 3, 3, 3, 3, 3, 11, 11, 11, 0, 7, 7, 4 };

   /* Trinket stuff */
   static char *noun[] = { "pebble", "bauble", "stone", "charm", "fetish",
      "bone", "trinket" };
   char buffer[64];

   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];

   if (obj->item_type == ITEM_ARMOR)
      {
      int ac_type = URANGE(0, mob->level/5, nelems(armor_types)-1);

chance=number_percent();
if (chance<25)
      xprintf(buffer, "%s %s", adj1[n_adj1], adj2[n_adj2]);
else if (chance<5) 
      xprintf(buffer, "%s", armor_types[ac_type]);
else
      xprintf(buffer, "%s", adj1[n_adj1]);

      name = buffer;
      obj->weight *= armor_mul[ac_type];
      obj->weight /= armor_div[ac_type];
      obj->value[0] = /*pierce*/
number_fuzzy(armor_mul[ac_type])+(number_fuzzy(armor_div[ac_type])/2);
      obj->value[1] = /*bash*/
number_fuzzy(armor_mul[ac_type])-(number_fuzzy(armor_div[ac_type])/3);
      obj->value[2] = /*slash*/
number_fuzzy(armor_mul[ac_type])+((number_fuzzy(armor_div[ac_type]))/3);
      obj->value[3] = (number_fuzzy(armor_mul[ac_type]))/4; /*exotic*/

      }
   else if (obj->item_type == ITEM_WEAPON)
      {
      int wea_type = number_range(0, nelems(weapon_types)-1);
      xprintf(buffer, "%s", weapon_types[wea_type]);

      name = buffer;
		   free_string( obj->name );
		   obj->name = str_dup( name );

	obj->value[1] = URANGE(1 ,(mob->level/4+1), 5);
	obj->value[2] = 2 + URANGE(1, (mob->level/12.5), 4);
	obj->value[3] = weapon_dam[wea_type];

      }
   else if (obj->item_type == ITEM_TREASURE)
      {
      if (number_percent() < mob->level)
         {
         random_apply(obj, mob);

         if (number_percent() < mob->level / 3)
            random_apply(obj, mob);
         }

      if (IS_SET(obj->wear_flags, ITEM_HOLD)) { /*trinket*/
         xprintf(buffer, "%s %s %s", adj1[n_adj1],
            adj2[n_adj2],
            noun[number_range(0, nelems(noun)-1)]);
		   free_string( obj->name );
		   obj->name = str_dup( buffer );
      }
      else { /*no, necklace or something*/
         xprintf(buffer, "%s %s", adj1[n_adj1],
            adj2[n_adj2]);
		   free_string( obj->name );
		   obj->name = str_dup( buffer );
      }

      name = buffer;
      }

if (obj->item_type == ITEM_WEAPON)
   xprintf( buf, obj->short_descr, name );
else if (obj->item_type == ITEM_TREASURE)
   xprintf( buf, "a%s %s",
      (name[0] == 'a' || name[0] == 'e' || name[0] == 'i' ||
       name[0] == 'o' || name[0] == 'u') ? "n" : "", name );
else
   xprintf( buf, "a%s %s %s",
      (name[0] == 'a' || name[0] == 'e' || name[0] == 'i' ||
       name[0] == 'o' || name[0] == 'u') ? "n" : "", name, 
	obj->short_descr );

   free_string( obj->short_descr );
   obj->short_descr = str_dup( buf );


if (obj->item_type == ITEM_WEAPON)
   xprintf( buf2, "%s lies here, ready to do some damage.",capitalize(obj->short_descr));
else if (obj->item_type == ITEM_TREASURE )
   xprintf( buf2, "%s lies here in splendor.", capitalize(obj->short_descr) );
else
   xprintf( buf2, "%s lies here, ready to be worn.", capitalize(obj->short_descr) );

   free_string( obj->description );
   obj->description = str_dup( buf2 );

      if (number_percent() < mob->level / 1.5)
         random_apply(obj, mob);
      if (number_percent() < mob->level / 3)
         random_apply(obj, mob);
      if (number_percent() < mob->level / 6)
         random_apply(obj, mob);
   

      obj_to_char( obj, mob ); 
      wear_obj (mob, obj, FALSE ); 

}
