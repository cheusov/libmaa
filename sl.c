/* sl.c -- Skip lists
 * Created: Sun Feb 18 11:51:06 1996 by faith@cs.unc.edu
 * Revised: Sun Feb 25 11:11:45 1996 by faith@cs.unc.edu
 * Copyright 1996 Rickard E. Faith (faith@cs.unc.edu)
 * Copyright 1996 Lars Nyland (nyland@cs.unc.edu)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 1, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: sl.c,v 1.3 1996/02/25 16:22:00 faith Exp $
 *
 * \section{Skip List Routines}
 *
 * \intro Skip list support is provided as an alternative to balanced
 * trees.  Skip lists have the advantage that an inorder walk through the
 * list is possible in the face of additions and deletions from the list.
 * Balanced tree, algorithms, in contrast, make this sort of traversal
 * impossible because of the rotations that are necessary for the
 * balancing.
 *
 * For these lists, the |key| is derivable from the |datum| that is stored
 * in the list.  This makes it possible for the actualy keys to change, as
 * long as the ordering of the data stay the same.  This is essential for
 * the use of skip lists for \khepera trees.
 *
 * This code is derived from articles written by William Pugh [CITATION
 * HERE] and from a skip list implementation by Lars Nyland.
 *
 */

#include "maaP.h"

#define SL_DEBUG

typedef struct _sl_Entry {
#if MAA_MAGIC
   int              magic;
#endif
   const void       *datum;
#ifdef SL_DEBUG
   int              levels;	/* levels for this entry */
#endif
   struct _sl_Entry *backward;	/* parent */
   struct _sl_Entry *forward[1]; /* variable sized array */
} *_sl_Entry;

typedef struct _sl_List {
#if MAA_MAGIC
   int              magic;
#endif
   int              level;
   struct _sl_Entry *head;
   int              (*compare)( const void *key1, const void *key2 );
   const void       *(*key)( const void *datum );
   const char       *(*print)( const void *datum );
} *_sl_List;

static mem_Object _sl_Memory;
static _sl_Entry  _sl_Current;

#define _sl_MaxLevel  16

#define KEY(l,d) ((l)->key ? (l)->key(d) : (d))
#define PRINT(l,d) ((l)->print ? (l)->print(d) : _sl_print(d))

static void _sl_check_list( _sl_List l, const char *function )
{
   if (!l) err_internal( function, "skip list is null\n" );
#if MAA_MAGIC
   if (l->magic != SL_LIST_MAGIC)
      err_internal( function,
		    "Bad magic: 0x%08x (should be 0x%08x)\n",
		    l->magic,
		    SL_LIST_MAGIC );
#endif
}

static int _sl_check_entry( _sl_Entry e, const char *function )
{
   if (!e) err_internal( function, "entry is null\n" );
#if MAA_MAGIC
   if (e->magic != SL_ENTRY_MAGIC) {
      err_warning( function,
		   "Bad magic: 0x%08x (should be 0x%08x)\n",
		   e->magic,
		   SL_ENTRY_MAGIC );
      return 1;
   }
#endif
   return 0;
}

#ifndef SL_DEBUG
#define _sl_check(x) /* */
#else
static void _sl_check( sl_List list )
{
   _sl_List  l = (_sl_List)list;
   _sl_Entry pt;
   
   _sl_check_list( list, __FUNCTION__ );
   for (pt = l->head->forward[0]; pt; pt = pt->forward[0] ) {
      if (pt && pt->forward[0]
	  && l->key( pt->datum ) >= l->key( pt->forward[0]->datum )) {
	 _sl_dump( list );
	 err_internal( __FUNCTION__,
		       "Datum 0x%p=%lu >= 0x%p=%lu\n",
		       l->key( pt->datum ),
		       (unsigned long)l->key( pt->datum ),
		       l->key( pt->forward[0]->datum ),
		       (unsigned long)l->key( pt->forward[0]->datum ) );
      }
      if (pt && pt->forward[0] && pt->forward[0]->backward != pt) {
	 _sl_dump( list );
	 err_internal( __FUNCTION__,
		       "Backward pointer corrupt for 0x%p=%lu\n",
		       l->key( pt->datum ),
		       (unsigned long)l->key( pt->datum ) );
      }
   }
}
#endif

static _sl_Entry _sl_create_entry( int maxLevel, const void *datum )
{
   _sl_Entry e;

   if (maxLevel > _sl_MaxLevel)
      err_internal( __FUNCTION__,
		    "level %d > %d\n", maxLevel, _sl_MaxLevel );

   e = xmalloc( sizeof( struct _sl_Entry )
		+ (maxLevel + 1) * sizeof( _sl_Entry ) );
#if MAA_MAGIC
   e->magic  = SL_ENTRY_MAGIC;
#endif
   e->datum  = datum;
   e->levels = maxLevel + 1;

   return e;
}

/* \doc |sl_create| initializes a skip list.  The |compare| function
   returns -1, 0, or 1 depending on the ordering of |key1| and |key2|.  The
   |key| function converts a |datum| into a |key|.  The |print| function
   returns a string representation of |datum|, and is allowed to always
   return a pointer to the same static buffer.

   |compare| must be provided.  If |key| is not provided, then |datum| will
   be used as the key.  If |print| is not provided, then the |datum|
   pointer will be printed when necessary. */


sl_List sl_create( int (*compare)( const void *key1, const void *key2 ),
		   const void *(*key)( const void *datum ),
		   const char *(*print)( const void *datum ) )
{
   _sl_List l;
   int      i;

   if (!_sl_Memory) {
      _sl_Memory = mem_create_objects( sizeof( struct _sl_List ) );
   }

   if (!compare)
      err_internal( __FUNCTION__, "compare fuction is NULL\n" );

   l          = mem_get_object( _sl_Memory );
#if MAA_MAGIC
   l->magic   = SL_LIST_MAGIC;
#endif
   l->level   = 0;
   l->head    = _sl_create_entry( _sl_MaxLevel, NULL );
   l->compare = compare;
   l->key     = key;
   l->print   = print;

   for (i = 0; i <= _sl_MaxLevel; i++) l->head->forward[i] = NULL;

   return l;
}

void sl_destroy( sl_List list )
{
   _sl_List  l = (_sl_List)list;
   _sl_Entry e;
   _sl_Entry next;

   _sl_check_list( list, __FUNCTION__ );
   for (e = l->head; e; e = next) {
      next = e->forward[0];
#if MAA_MAGIC
      e->magic = SL_ENTRY_MAGIC_FREED;
#endif
      xfree( e );
   }
#if MAA_MAGIC
   l->magic = SL_LIST_MAGIC_FREED;
#endif
   mem_free_object( _sl_Memory, l );
}

void _sl_shutdown( void )
{
   if (_sl_Memory) mem_destroy_objects( _sl_Memory );
   _sl_Memory = NULL;
}

static int _sl_random_level( void )
{
   int level = 1;
				/* FIXME! Assumes random() is.  We should
                                   use our own random() to make sure.  This
                                   also assumes that p == 0.5, which is
                                   probably reasonable, but maybe should be
                                   a user-defined parameter. */
   while ((random() & 1) &&  level < _sl_MaxLevel) ++level;
   return level;
}

static const char *_sl_print( const void *datum )
{
   static char buf[1024];

   sprintf( buf, "%p", datum );

   return buf;
}

static _sl_Entry _sl_locate_previous( _sl_List l, const void *key,
				      _sl_Entry update[] )
{
   int       i;
   _sl_Entry pt;
   
   _sl_check( l );
   for (i = l->level, pt = l->head; i >= 0; i--) {
      while (pt->forward[i]
	     && l->compare( KEY(l,pt->forward[i]->datum), key ) < 0)
	 pt = pt->forward[i];
      update[i] = pt;
   }
   
   return pt;			/* Caller must do pt = pt->forward[0] */
}

void sl_insert( sl_List list, const void *datum )
{
   _sl_List         l = (_sl_List)list;
   _sl_Entry        update[_sl_MaxLevel + 1];
   _sl_Entry        backward;
   _sl_Entry        pt;
   const void       *key;
   int              i;
   int              level = _sl_random_level();
   _sl_Entry        entry;

   _sl_check_list( list, __FUNCTION__ );
   
   key = KEY(l,datum);

   backward = _sl_locate_previous( l, key, update );
   pt       = backward->forward[0];

   if (pt && !l->compare( KEY(l,pt->datum), key ))
      err_internal( __FUNCTION__,
		    "Datum \"%s\" is already in list\n", PRINT(l,datum) );

   if (level > l->level) {
      level = ++l->level;
      update[level] = l->head;
   }
   
   entry = _sl_create_entry( level, datum );

				/* Fixup forward pointers */
   for (i = 0; i <= level; i++) {
      entry->forward[i]     = update[i]->forward[i];
      update[i]->forward[i] = entry;
   }

				/* Fixup backward pointers */
   entry->backward = backward;
   if (entry->forward[0]) entry->forward[0]->backward = entry;
   
   _sl_check( list );
}

void sl_delete( sl_List list, const void *datum )
{
   _sl_List         l = (_sl_List)list;
   _sl_Entry        update[_sl_MaxLevel + 1];
   _sl_Entry        backward;
   _sl_Entry        pt;
   const void       *key;
   int              i;

   _sl_check_list( list, __FUNCTION__ );
   
   key = KEY(l,datum);

   backward = _sl_locate_previous( l, key, update );
   pt       = backward->forward[0];

   if (!pt || l->compare( KEY(l,pt->datum), key )) {
      _sl_dump( list );
      err_internal( __FUNCTION__,
		    "Datum \"%s\" is not in list\n", PRINT(l,datum) );
   }

				/* Fixup sl_iterate's position pointer */
   if (_sl_Current && _sl_Current == pt) {
      _sl_Current = pt->backward;
      assert( _sl_Current );
      PRINTF(MAA_SL,("delete: %lu => %lu\n",
		     (unsigned long)l->key(pt->datum),
		     _sl_Current?(unsigned long)l->key(_sl_Current->datum):0));
   }

				/* Fixup forward pointers */
   for (i = 0; i <= l->level; i++) {
      if (update[i]->forward[i] == pt)
	 update[i]->forward[i] = pt->forward[i];
   }
				/* Fixup backward pointer */
   if (pt->forward[0]) pt->forward[0]->backward = backward;
   
   xfree( pt );
   while (l->level && !l->head->forward[ l->level ])
      --l->level;
   _sl_check( list );
}

const void *sl_find( sl_List list, const void *key )
{
   _sl_List         l = (_sl_List)list;
   _sl_Entry        update[_sl_MaxLevel + 1];
   _sl_Entry        backward;
   _sl_Entry        pt;

   _sl_check_list( list, __FUNCTION__ );

   backward = _sl_locate_previous( l, key, update );
   pt       = backward->forward[0];

   if (pt && !l->compare( KEY(l,pt->datum), key )) return pt->datum;
   return NULL;
}

int sl_iterate( sl_List list, int (*f)( const void *datum ) )
{
   _sl_List   l = (_sl_List)list;
   _sl_Entry  pt;
   _sl_Entry  next;
   int        retcode;
   _sl_Entry  oldCurrent = _sl_Current; /* Make sl_iterate re-entrant */
   const void *key;

   if (!list) return 0;
   _sl_check_list( list, __FUNCTION__ );

   if (dbg_test(MAA_SL)) {
      printf( __FUNCTION__ ": " );
      for (pt = l->head->forward[0]; pt; pt = next) {
	 next = pt->forward[0];
	 printf( "%lu ", (unsigned long)l->key( pt->datum ) );
      }
      printf( "\n" );
   }

   for (pt = l->head->forward[0], _sl_Current = l->head; pt; pt = next) {
      _sl_check_entry( pt, __FUNCTION__ );
      key = l->key( pt->datum ); /* Save current key for comparison */
      
      if ((retcode = f(pt->datum))) {
	 _sl_Current = oldCurrent; /* Restore old pointer */
	 return retcode;
      }
      assert( _sl_Current );

      for (next = _sl_Current->forward[0]; next; next = next->forward[0])
	 if (l->compare( l->key( next->datum ), key ) > 0) break;

      PRINTF(MAA_SL,("next = 0x%p/%lu\n",
		     next?(unsigned long)l->key(next->datum):0,
		     next?(unsigned long)l->key(next->datum):0));
   }

   _sl_check( list );
   _sl_Current = oldCurrent;	/* Restore old pointer */
   
   return 0;
}

void _sl_dump( sl_List list )
{
   _sl_List  l = (_sl_List)list;
   _sl_Entry pt;
   int       i;
   int       count = 0;

   _sl_check_list( list, __FUNCTION__ );

   printf( "Level = %d\n", l->level );
   for (pt = l->head; pt; pt = pt->forward[0]) {
      printf( "  Entry %p (%d/%p/0x%p=%lu) has 0x%x levels (backward = %p):\n",
	      pt, count++, pt->datum,
	      pt->datum ? l->key( pt->datum ) : 0,
	      (unsigned long)(pt->datum ? l->key( pt->datum ) : 0),
	      pt->levels,
	      pt->backward );
      for (i = 0; i < pt->levels; i++)
	 printf( "    %p\n", pt->forward[i] );
   }
}
