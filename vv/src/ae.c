/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
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
#include "fok_struct.h"
DECLARE_DO_FUN(	do_say		);
DECLARE_DO_FUN( do_enter	);
DECLARE_DO_FUN( do_look		);
DECLARE_DO_FUN( do_stand	);
DECLARE_DO_FUN( do_cast		);
DECLARE_DO_FUN( do_open		);

/* random room generation procedure */
ROOM_INDEX_DATA  *get_random_room(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *room;

    for ( ; ; )
    {
        room = get_room_index( number_range( 0, 65535 ) );
        if ( room != NULL )
        if ( can_see_room(ch,room)
	&&   !IS_SET(room->room_flags, ROOM_ARENA)
	&&   (!(strstr(room->area->builders,"Arena") ) )
	&& ( !( strstr( room->area->builders, "Unlinked" ) ) )
	&&   !room_is_private(room)
        &&   !IS_SET(room->room_flags, ROOM_PRIVATE)
        &&   !IS_SET(room->room_flags, ROOM_SOLITARY) 
	&&   !IS_SET(room->room_flags, ROOM_SAFE) 
	&&   (IS_NPC(ch) || IS_SET(ch->act,ACT_AGGRESSIVE) 
	||   !IS_SET(room->room_flags,ROOM_LAW)))
            break;
    }

    return room;
}

/* RT Enter portals */
void do_enter( CHAR_DATA *ch, char *argument)
{    
    ROOM_INDEX_DATA *location; 

    if ( ch->fighting != NULL ) 
	return;

    /* nifty portal stuff */
    if (argument[0] != '\0')
    {
        ROOM_INDEX_DATA *old_room;
	OBJ_DATA *portal;
	CHAR_DATA *fch, *fch_next;

        old_room = ch->in_room;

	portal = get_obj_list( ch, argument,  ch->in_room->contents );
	
	if (portal == NULL)
	{
	    send_to_char("{cYou don't see that here.{x\n\r",ch);
	    return;
	}

	if (portal->item_type != ITEM_PORTAL 
        ||  (IS_SET(portal->value[1],EX_CLOSED) && !IS_TRUSTED(ch,ANGEL)))
	{
	    send_to_char("{cYou can't seem to find a way in.{x\n\r",ch);
	    return;
	}

	if (!IS_TRUSTED(ch,ANGEL) && !IS_SET(portal->value[2],GATE_NOCURSE)
	&&  (IS_AFFECTED(ch,AFF_CURSE) 
	||   IS_SET(old_room->room_flags,ROOM_NO_RECALL)))
	{
	    send_to_char("{cSomething prevents you from leaving...{x\n\r",ch);
	    return;
	}

	if (IS_SET(portal->value[2],GATE_RANDOM) || portal->value[3] == -1)
	{
	    location = get_random_room(ch);
	    portal->value[3] = location->vnum; /* for record keeping :) */
	}
	else if (IS_SET(portal->value[2],GATE_BUGGY) && (number_percent() < 5))
	    location = get_random_room(ch);
	else
	    location = get_room_index(portal->value[3]);

	if (location == NULL
	||  location == old_room
	||  !can_see_room(ch,location) 
	||  (room_is_private(location) && !IS_TRUSTED(ch,IMPLEMENTOR)))
	{
	   act("$p doesn't seem to go anywhere.",ch,portal,NULL,TO_CHAR);
	   return;
	}

        if (IS_NPC(ch) && IS_SET(ch->act,ACT_AGGRESSIVE)
        &&  IS_SET(location->room_flags,ROOM_LAW))
        {
            send_to_char("{cSomething prevents you from leaving...{x\n\r",ch);
            return;
        }

	act("$n steps into $p.",ch,portal,NULL,TO_ROOM);
	
	if (IS_SET(portal->value[2],GATE_NORMAL_EXIT))
	    act("You enter $p.",ch,portal,NULL,TO_CHAR);
	else
	    act("You walk through $p and find yourself somewhere else...",
	        ch,portal,NULL,TO_CHAR); 

	char_from_room(ch);
	char_to_room(ch, location);

	if (IS_SET(portal->value[2],GATE_GOWITH)) /* take the gate along */
	{
	    obj_from_room(portal);
	    obj_to_room(portal,location);
	}

	if (IS_SET(portal->value[2],GATE_NORMAL_EXIT))
	    act("$n has arrived.",ch,portal,NULL,TO_ROOM);
	else
	    act("$n has arrived through $p.",ch,portal,NULL,TO_ROOM);

	do_look(ch,"auto");

	/* charges */
	if (portal->value[0] > 0)
	{
	    portal->value[0]--;
	    if (portal->value[0] == 0)
		portal->value[0] = -1;
	}

	/* protect against circular follows */
	if (old_room == location)
	    return;

    	for ( fch = old_room->people; fch != NULL; fch = fch_next )
    	{
            fch_next = fch->next_in_room;

            if (portal == NULL || portal->value[0] == -1) 
	    /* no following through dead portals */
                continue;
 
            if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM)
            &&   fch->position < POS_STANDING)
		do_stand(fch,"");

            if ( fch->master == ch && fch->position == POS_STANDING)
            {
 
                if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
                &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
                {
                    act("You can't bring $N into the city.",
                    	ch,NULL,fch,TO_CHAR);
                    act("You aren't allowed in the city.",
                    	fch,NULL,NULL,TO_CHAR);
                    continue;
            	}
 
            	act( "You follow $N.", fch, NULL, ch, TO_CHAR );
		do_enter(fch,argument);
            }
    	}

 	if (portal != NULL && portal->value[0] == -1)
	{
	    act("$p fades out of existence.",ch,portal,NULL,TO_CHAR);
	    if (ch->in_room == old_room)
		act("$p fades out of existence.",ch,portal,NULL,TO_ROOM);
	    else if (old_room->people != NULL)
	    {
		act("$p fades out of existence.", 
		    old_room->people,portal,NULL,TO_CHAR);
		act("$p fades out of existence.",
		    old_room->people,portal,NULL,TO_ROOM);
	    }
	    extract_obj(portal);
	}
       /*
        * If someone is following the char, these triggers get activated
        * for the followers before the char, but it's safer this way...
        */
       if ( IS_NPC( ch ) && HAS_TRIGGER_MOB( ch, TRIG_ENTRY ) )
//           mp_percent_trigger( ch, NULL, NULL, NULL, TRIG_ENTRY );
           p_percent_trigger( ch, NULL, NULL, NULL, NULL, NULL, TRIG_ENTRY );
//       if ( !IS_NPC( ch ) )
//           mp_greet_trigger( ch );
        if ( !IS_NPC( ch ) )
        {
            p_greet_trigger( ch, PRG_MPROG );
	    p_greet_trigger( ch, PRG_OPROG );
	    p_greet_trigger( ch, PRG_RPROG );
	}
	return;
    }

    send_to_char("{cNope, can't do it.{x\n\r",ch);
    return;
}

/* REDHAT 5.1 don't like this removed!! 
if	defined(linux)
void bcopy(const void *src,void *dest,int n);
void bzero(void *s,int n);
else
void bcopy(char *s1,char* s2,int len);
void bzero(char *sp,int len);
endif
 Redhat */
struct hash_link
{
  int			key;
  struct hash_link	*next;
  void			*data;
};

struct hash_header
{
  int			rec_size;
  int			table_size;
  int			*keylist, klistsize, klistlen; /* this is really lame,
							  AMAZINGLY lame
*/
  struct hash_link	**buckets;
};

#define WORLD_SIZE	32700
#define	HASH_KEY(ht,key)((((unsigned int)(key))*17)%(ht)->table_size)



struct hunting_data
{
  char			*name;
  struct char_data	**victim;
};

struct room_q
{
  int		room_nr;
  struct room_q	*next_q;
};

struct nodes
{
  int	visited;
  int	ancestor;
};

#define IS_DIR		(get_room_index(q_head->room_nr)->exit[i])
#define GO_OK		(!IS_SET( IS_DIR->exit_info, EX_CLOSED ))
#define GO_OK_SMARTER	1


void init_hash_table(struct hash_header	*ht,int rec_size,int table_size)
{
  ht->rec_size	= rec_size;
  ht->table_size= table_size;
  ht->buckets	= (void*)calloc(sizeof(struct hash_link**),table_size);
  ht->keylist	= (void*)malloc(sizeof(ht->keylist)*(ht->klistsize=128));
  ht->klistlen	= 0;
}

void init_world(ROOM_INDEX_DATA *room_db[])
{
  /* zero out the world */
  bzero((char *)room_db,sizeof(ROOM_INDEX_DATA *)*WORLD_SIZE);
}

CHAR_DATA *get_char_area( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   CHAR_DATA *ach;
   int number;
   int count;

   if( argument[0] == '\0' )
     return NULL;

   number = number_argument( argument, arg );
/*    if (arg[0] == NULL) return NULL; */
   if (arg[0] == '\0') return NULL;
   count = 0;     

//   if( ( ach = get_char_room( ch, argument ) ) != NULL )
   if( ( ach = get_char_room( ch,NULL, argument ) ) != NULL )
     return ach;



   for( ach = char_list; ach != NULL; ach = ach->next )
   { 
     if( ach->in_room && (ach->in_room->area != ch->in_room->area
	|| !can_see( ch, ach ) || !is_name( arg, ach->name )) )
	continue;
     if( ++count == number )
       return ach;
   }
   return NULL;
}


void destroy_hash_table(struct hash_header *ht,void (*gman)())
{
  int			i;
  struct hash_link	*scan,*temp;

  for(i=0;i<ht->table_size;i++)
    for(scan=ht->buckets[i];scan;)
      {
	temp = scan->next;
	(*gman)(scan->data);
	free(scan);
	scan = temp;
      }
  free(ht->buckets);
  free(ht->keylist);
}

void _hash_enter(struct hash_header *ht,int key,void *data)
{
  /* precondition: there is no entry for <key> yet */
  struct hash_link	*temp;
  int			i;

  temp		= (struct hash_link *)malloc(sizeof(struct hash_link));
  temp->key	= key;
  temp->next	= ht->buckets[HASH_KEY(ht,key)];
  temp->data	= data;
  ht->buckets[HASH_KEY(ht,key)] = temp;
  if(ht->klistlen>=ht->klistsize)
    {
      ht->keylist = (void*)realloc(ht->keylist,sizeof(*ht->keylist)*
				   (ht->klistsize*=2));
    }
  for(i=ht->klistlen;i>=0;i--)
    {
      if(ht->keylist[i-1]<key)
	{
	  ht->keylist[i] = key;
	  break;
	}
      ht->keylist[i] = ht->keylist[i-1];
    }
  ht->klistlen++;
}

ROOM_INDEX_DATA *room_find(ROOM_INDEX_DATA *room_db[],int key)
{
  return((key<WORLD_SIZE&&key>-1)?room_db[key]:0);
}

void *hash_find(struct hash_header *ht,int key)
{
  struct hash_link *scan;

  scan = ht->buckets[HASH_KEY(ht,key)];

  while(scan && scan->key!=key)
    scan = scan->next;

  return scan ? scan->data : NULL;
}

int room_enter(ROOM_INDEX_DATA *rb[],int key,ROOM_INDEX_DATA *rm)
{
  ROOM_INDEX_DATA *temp;
   
  temp = room_find(rb,key);
  if(temp) return(0);

  rb[key] = rm;
  return(1);
}

int hash_enter(struct hash_header *ht,int key,void *data)
{
  void *temp;

  temp = hash_find(ht,key);
  if(temp) return 0;

  _hash_enter(ht,key,data);
  return 1;
}

ROOM_INDEX_DATA *room_find_or_create(ROOM_INDEX_DATA *rb[],int key)
{
  ROOM_INDEX_DATA *rv;

  rv = room_find(rb,key);
  if(rv) return rv;

  rv = (ROOM_INDEX_DATA *)malloc(sizeof(ROOM_INDEX_DATA));
  rb[key] = rv;
    
  return rv;
}

void *hash_find_or_create(struct hash_header *ht,int key)
{
  void *rval;

  rval = hash_find(ht, key);
  if(rval) return rval;

  rval = (void*)malloc(ht->rec_size);
  _hash_enter(ht,key,rval);

  return rval;
}

int room_remove(ROOM_INDEX_DATA *rb[],int key)
{
  ROOM_INDEX_DATA *tmp;

  tmp = room_find(rb,key);
  if(tmp)
    {
      rb[key] = 0;
      free(tmp);
    }
  return(0);
}

void *hash_remove(struct hash_header *ht,int key)
{
  struct hash_link **scan;

  scan = ht->buckets+HASH_KEY(ht,key);

  while(*scan && (*scan)->key!=key)
    scan = &(*scan)->next;

  if(*scan)
    {
      int		i;
      struct hash_link	*temp, *aux;

      temp	= (*scan)->data;
      aux	= *scan;
      *scan	= aux->next;
      free(aux);

      for(i=0;i<ht->klistlen;i++)
	if(ht->keylist[i]==key)
	  break;

      if(i<ht->klistlen)
	{
	  bcopy((char *)ht->keylist+i+1,(char *)ht->keylist+i,(ht->klistlen-i)
		*sizeof(*ht->keylist));
	  ht->klistlen--;
	}

      return temp;
    }

  return NULL;
}

void room_iterate(ROOM_INDEX_DATA *rb[],void (*func)(),void *cdata)
{
  register int i;

  for(i=0;i<WORLD_SIZE;i++)
    {
      ROOM_INDEX_DATA *temp;
  
      temp = room_find(rb,i);
      if(temp) (*func)(i,temp,cdata);
    }
}

void hash_iterate(struct hash_header *ht,void (*func)(),void *cdata)
{
  int i;

  for(i=0;i<ht->klistlen;i++)
    {
      void		*temp;
      register int	key;

      key = ht->keylist[i];
      temp = hash_find(ht,key);
      (*func)(key,temp,cdata);
      if(ht->keylist[i]!=key) /* They must have deleted this room */
	i--;		      /* Hit this slot again. */
    }
}



int exit_ok( EXIT_DATA *pexit )
{
  ROOM_INDEX_DATA *to_room;

  if ( ( pexit == NULL )
  ||   ( to_room = pexit->u1.to_room ) == NULL )
    return 0;

  return 1;
}

void donothing()
{
  return;
}

int find_path( int in_room_vnum, int out_room_vnum, CHAR_DATA *ch, 
	       int depth, int in_zone )
{
  struct room_q		*tmp_q, *q_head, *q_tail;
  struct hash_header	x_room;
  int			i, tmp_room, count=0, thru_doors;
  ROOM_INDEX_DATA	*herep;
  ROOM_INDEX_DATA	*startp;
  EXIT_DATA		*exitp;

  if ( depth <0 )
    {
      thru_doors = TRUE;
      depth = -depth;
    }
  else
    {
      thru_doors = FALSE;
    }

  startp = get_room_index( in_room_vnum );

  init_hash_table( &x_room, sizeof(int), 2048 );
  hash_enter( &x_room, in_room_vnum, (void *) - 1 );

  /* initialize queue */
  q_head = (struct room_q *) malloc(sizeof(struct room_q));
  q_tail = q_head;
  q_tail->room_nr = in_room_vnum;
  q_tail->next_q = 0;

  while(q_head)
    {
      herep = get_room_index( q_head->room_nr );
      /* for each room test all directions */
      if (herep==NULL) fprintf(stderr,"BUG:  Null herep in hunt.c, room #%d",q_head->room_nr);
      if( herep && (herep->area == startp->area || !in_zone) )
		{
	  /* only look in this zone...
	     saves cpu time and  makes world safer for players  */
	  for( i = 0; i <= 5; i++ )
	    {
	      exitp = herep->exit[i];
	      if( exit_ok(exitp) && ( thru_doors ? GO_OK_SMARTER : GO_OK )
)
		{
		  /* next room */
		  tmp_room = herep->exit[i]->u1.to_room->vnum;
		  if( tmp_room != out_room_vnum )
		    {
		      /* shall we add room to queue ?
			 count determines total breadth and depth */
		      if( !hash_find( &x_room, tmp_room )
			 && ( count < depth ) )
			/* && !IS_SET( RM_FLAGS(tmp_room), DEATH ) ) */
			{
			  count++;
			  /* mark room as visted and put on queue */
			  
			  tmp_q = (struct room_q *)
			    malloc(sizeof(struct room_q));
			  tmp_q->room_nr = tmp_room;
			  tmp_q->next_q = 0;
			  q_tail->next_q = tmp_q;
			  q_tail = tmp_q;
	      
			  /* ancestor for first layer is the direction */
			  hash_enter( &x_room, tmp_room,

((int)hash_find(&x_room,q_head->room_nr)
				      == -1) ? (void*)(i+1)
				     :
hash_find(&x_room,q_head->room_nr));
			}
		    }
		  else
		    {
		      /* have reached our goal so free queue */
		      tmp_room = q_head->room_nr;
		      for(;q_head;q_head = tmp_q)
			{
			  tmp_q = q_head->next_q;
			  free(q_head);
			}
		      /* return direction if first layer */
		      if ((int)hash_find(&x_room,tmp_room)==-1)
			{
			  if (x_room.buckets)
			    {
			      /* junk left over from a previous track */
			      destroy_hash_table(&x_room, donothing);
			    }
			  return(i);
			}
		      else
			{
			  /* else return the ancestor */
			  int i;
			  
			  i = (int)hash_find(&x_room,tmp_room);
			  if (x_room.buckets)
			    {
			      /* junk left over from a previous track */
			      destroy_hash_table(&x_room, donothing);
			    }
			  return( -1+i);
			}
		    }
		}
	    }
	}
      
      /* free queue head and point to next entry */
      tmp_q = q_head->next_q;
      free(q_head);
      q_head = tmp_q;
    }

  /* couldn't find path */
  if( x_room.buckets )
    {
      /* junk left over from a previous track */
      destroy_hash_table( &x_room, donothing );
    }
  return -1;
}






/*
 * revised by chronos.
 */
void hunt_victim( CHAR_DATA *ch )
{
  int		dir;
  bool		found;
  CHAR_DATA	*tmp;
  char		tBuf[MAX_INPUT_LENGTH];

  /*
   * Make sure the victim still exists.
   */
  for( found = 0, tmp = char_list; tmp && !found; tmp = tmp->next )
    if( ch->hunting == tmp )
      found = 1;

  if( !found || !can_see( ch, ch->hunting ) )
    {
     if( get_char_area( ch, ch->hunting->name) != NULL )
        {
           sprintf( tBuf, "portal %s", ch->hunting->name );
           log_string("mob portal");
	   do_cast(ch,tBuf);
           log_string("do_enter1");
	   do_enter(ch,"portal");
	   if (ch->in_room==NULL || ch->hunting==NULL) return;
	   if( ch->in_room == ch->hunting->in_room )
	    {
	      act( "$n glares at $N and says, 'Ye shall DIE!'",
		  ch, NULL, ch->hunting, TO_NOTVICT );
	      act( "$n glares at you and says, 'Ye shall DIE!'",
		  ch, NULL, ch->hunting, TO_VICT );
	      act( "You glare at $N and say, 'Ye shall DIE!",
		  ch, NULL, ch->hunting, TO_CHAR);
	      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
	      ch->hunting = NULL;
	      return;
	    }  
	   log_string("done1");  
	   return;
        } 
       else 
	{ 
	do_say(ch,"Ahhhh!  My prey is gone!!");
         ch->hunting = NULL;
         return;
        }  
    }   /* end if !found or !can_see */ 



  dir = find_path( ch->in_room->vnum, ch->hunting->in_room->vnum,
		  ch, -40000, TRUE );

  if( dir < 0 || dir > 5 )
  {
/* 1 */ 
    if( get_char_area( ch, ch->hunting->name) != NULL  
        && ch-> level > 35 )
    {
      sprintf( tBuf, "portal %s", ch->hunting->name );
      log_string("mob portal");
	do_cast(ch,tBuf);
      log_string("do_enter2");
	do_enter(ch,"portal");
      if (ch->in_room==NULL || ch->hunting==NULL) return;
      if( ch->in_room == ch->hunting->in_room )
       {
      	act( "$n glares at $N and says, 'Ye shall DIE!'",
		  ch, NULL, ch->hunting, TO_NOTVICT );
	act( "$n glares at you and says, 'Ye shall DIE!'",
		  ch, NULL, ch->hunting, TO_VICT );
        act( "You glare at $N and say, 'Ye shall DIE!",
		  ch, NULL, ch->hunting, TO_CHAR);
        multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
        ch->hunting = NULL;
        return;
       }  
      log_string("done2"); 
      return;
    }
    else
    { 
      act( "$n says 'I have lost $M!'", ch, NULL, ch->hunting, TO_ROOM );
      ch->hunting = NULL;
      return;
    }
   } /* if dir < 0 or > 5 */  


  if( ch->in_room->exit[dir] && IS_SET( ch->in_room->exit[dir]->exit_info,EX_CLOSED ) )
    {
	do_open(ch,(char *)dir_name[dir]);
      return;
    }
  if (!ch->in_room->exit[dir]) {
		log_string("BUG:  hunt through null door");
		ch->hunting = NULL;
		return;
		}
  move_char( ch, dir, FALSE );
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( "$n glares at $N and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( "$n glares at you and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( "You glare at $N and say, 'Ye shall DIE!",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }  
  return;
}

void hunt_victim_old( CHAR_DATA *ch )
{
  int		dir,i;
  bool		found,ok;
  CHAR_DATA	*tmp;
  char		tBuf[MAX_INPUT_LENGTH];

  if( ch == NULL || ch->hunting == NULL || !IS_NPC(ch) ) 
   {
    if (IS_NPC(ch))
      {
    	if ((ROOM_INDEX_DATA*)ch->logon!=ch->in_room)
    	log_string("HUNT: Return creature to original home!");
       	act("\n\rA glowing portal appears.",ch,NULL,NULL,TO_ROOM);	    	
    	act("$n steps through a glowing portal.\n\r",ch,NULL,NULL,TO_ROOM);
       	char_from_room(ch);
    	char_to_room(ch,(ROOM_INDEX_DATA*)ch->logon);
        }
    return;
   }

  /*
   * Make sure the victim still exists.
   */
  for( found = 0, tmp = char_list; tmp && !found; tmp = tmp->next )
    if( ch->hunting == tmp )
      found = 1;

  if( !found || !can_see( ch, ch->hunting ) )
    {
/*1 */  if( get_char_world( ch, ch->hunting->name) != NULL  
            && ch-> level > 35 )
        {
           sprintf( tBuf, "portal %s", ch->hunting->name );
           log_string("mob portal");
		do_cast(ch,tBuf);
           log_string("do_enter1");
		do_enter(ch,"portal");
  /* Deth...this shouldn't have to be here..but it got
  here in a core file with ch->hunting==null.. */
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( "$n glares at $N and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( "$n glares at you and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( "You glare at $N and say, 'Ye shall DIE!",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }  
	   log_string("done1");  
	   	return;
        } 
       else { 
       if (IS_NPC(ch)) 
	 {
          if ( (ROOM_INDEX_DATA*)ch->logon!=ch->in_room) 
          {
     	   log_string("HUNT: Send mob home");
    	   act("\n\rA glowing portal appears.",ch,NULL,NULL,TO_ROOM);	    	
    	   act("$n steps through a glowing portal.\n\r",ch,NULL,NULL,TO_ROOM); 
       	   char_from_room(ch);
    	   char_to_room(ch,(ROOM_INDEX_DATA*)ch->logon);
    	  }
         }
	do_say(ch,"Ahhhh!  My prey is gone!!");
         ch->hunting = NULL;
         return;
        }  
    }   /* end if !found or !can_see */ 



  dir = find_path( ch->in_room->vnum, ch->hunting->in_room->vnum,
		  ch, -40000, TRUE );

  if( dir < 0 || dir > 5 )
  {
/* 1 */ 
    if( get_char_area( ch, ch->hunting->name) != NULL  
        && ch-> level > 35 )
    {
      sprintf( tBuf, "portal %s", ch->hunting->name );
      log_string("mob portal");
	do_cast(ch,tBuf); 
      log_string("do_enter2");
	do_enter(ch,"portal");
  /* Deth...this shouldn't have to be here..but it got
  here in a core file with ch->hunting==null.. */
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( "$n glares at $N and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( "$n glares at you and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( "You glare at $N and say, 'Ye shall DIE!",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }  
      log_string("done2"); 
      return;
    }
    else
    { 
     if (IS_NPC(ch)) 
       {
    	if ((ROOM_INDEX_DATA*)ch->logon!=ch->in_room)
        {  
          log_string("HUNT: return creature to original room");
	  act("\n\rA glowing portal appears.",ch,NULL,NULL,TO_ROOM);	    	
	  act("$n steps through a glowing portal.\n\r",ch,NULL,NULL,TO_ROOM);
	  char_from_room(ch);
    	  char_to_room(ch,(ROOM_INDEX_DATA*)ch->logon);
    	}
      }   

      act( "$n says 'I have lost $M!'", ch, NULL, ch->hunting, TO_ROOM );
      ch->hunting = NULL;
      return;
    }
   } /* if dir < 0 or > 5 */  

  /*
   * Give a random direction if the mob misses the die roll.
   */
  if( number_percent () > 75 )        /* @ 25% */
    {
    	ok=FALSE;
        for(i=0;i<6;i++) {
    	if (ch->in_room->exit[dir]!=NULL) {
    		ok=TRUE;
    		break;
    		}
    	}
	if(ok) {
      do
        {
	  dir = number_door();
        }
      while( ( ch->in_room->exit[dir] == NULL )
	    || ( ch->in_room->exit[dir]->u1.to_room == NULL ) );
	   }
	  else {
	  log_string("Do hunt, player hunt, no exits from room!");
  	  ch->hunting=NULL;
  	  send_to_char("Your room has not exits!!!!\n\r",ch);
  	  return;
  	}	    
    }
    


  if( ch->in_room->exit[dir] && IS_SET( ch->in_room->exit[dir]->exit_info,EX_CLOSED ) )
    {
	do_open(ch,(char *)dir_name[dir]);
      return;
    }
	if (!ch->in_room->exit[dir]) {
		log_string("BUG:  hunt through null door");
		return;
		}
  move_char( ch, dir, FALSE );
  /* Deth...this shouldn't have to be here..but it got
  here in a core file with ch->hunting==null.. */
  if (ch->in_room==NULL || ch->hunting==NULL) return;
  if( ch->in_room == ch->hunting->in_room )
    {
      act( "$n glares at $N and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_NOTVICT );
      act( "$n glares at you and says, 'Ye shall DIE!'",
	  ch, NULL, ch->hunting, TO_VICT );
      act( "You glare at $N and say, 'Ye shall DIE!",
	  ch, NULL, ch->hunting, TO_CHAR);
      multi_hit( ch, ch->hunting, TYPE_UNDEFINED );
      ch->hunting = NULL;
      return;
    }  
  return;
}
/**************************************************************************
			SOCIAL EDITING
**************************************************************************/

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
		sprintf (buf, "Social: %s\n\r"
		              "(cnoarg) No argument given, character sees:\n\r"
		              "%s\n\r\n\r"
		              "(onoarg) No argument given, others see:\n\r"
		              "%s\n\r\n\r"
		              "(cfound) Target found, character sees:\n\r"
		              "%s\n\r\n\r"
		              "(ofound) Target found, others see:\n\r"
		              "%s\n\r\n\r"
		              "(vfound) Target found, victim sees:\n\r"
		              "%s\n\r\n\r"
		              "(cself) Target is character himself:\n\r"
		              "%s\n\r\n\r"
		              "(oself) Target is character himself, others see:\n\r"
		              "%s\n\r",
		              
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


