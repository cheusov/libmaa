/* set.c -- Set routines for Khepera
 * Created: Wed Nov  9 13:31:24 1994 by faith@cs.unc.edu
 * Revised: Thu Mar  9 09:56:31 1995 by faith@cs.unc.edu
 * Copyright 1994, 1995 Rickard E. Faith (faith@cs.unc.edu)
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * $Id: set.c,v 1.1 1995/04/21 15:31:47 faith Exp $
 *
 * \section{Set Routines}
 *
 * \intro The set implementation is similar to the hash table
 * implementation, except that the set object does not associate a |datum|
 * with a |key|.  For sets, keys are called elements.  All of the hash
 * table functions are supported, with the addition of a membership test
 * and several set operations.
 *
 * The underlying data structure is a hash table of prime length, with
 * self-organizing linked lists \cite[pp.~398--9]{faith:Knuth73c} used for
 * collision resolution.  The hash table automatically grows as necessary
 * to preserve efficient access.
 *
 */

#include "kh.h"

typedef struct bucket {
   const void    *elem;
   unsigned int   hash;
   struct bucket *next;
} *bucketType;

typedef struct set {
   int            prime;
   int            entries;
   bucketType    *buckets;
   int            retrievals;
   int            hits;
   int            misses;
   unsigned int (*hash)( const void * );
   int          (*compare)( const void *, const void * );
} *setType;
   

static set_Set _set_create( int prime,
			    unsigned int (*hash)( const void * ),
			    int (*compare)( const void *, const void * ) )
{
   setType t;
   int     i;
   int     size = _hsh_primes[ prime ];
   
   t             = xmalloc( sizeof( struct set ) );
   t->prime      = prime;
   t->entries    = 0;
   t->buckets    = xmalloc( size * sizeof( struct bucket ) );
   t->retrievals = 0;
   t->hits       = 0;
   t->misses     = 0;
   t->hash       = hash ? hash : hsh_string_hash;
   t->compare    = compare ? compare : hsh_string_compare;

   for (i = 0; i < size; i++) t->buckets[i] = NULL;

   return t;
}

/* \doc The |set_create| function initilizes a set object.  Elements are
   pointers to "void".

   The internal representation of the set will grow automatically when an
   insertion is performed and the table is more than half full.  The table
   size progresses through the following series of primes:
   \val{_hsh_primes}.

   The |hash| function should take a pointer to a |elem| and return an
   "unsigned int".  If |hash| is "NULL", then the |elem| is assumed to be a
   pointer to a null-terminated string, and the function shown in
   \grindref{fig:hshstringhash} will be used for |hash|.

   The |compare| function should take a pair of pointers to elements and
   return zero if the elements are the same and non-zero if they are
   different.  If |compare| is "NULL", then the elements are assumed to
   point to null-terminated strings, and the |strcmp| function will be used
   for |compare|.

   Additionally, the |hsh_pointer_hash| and |hsh_pointer_compare| functions
   are available and can be used to treat the \emph{value} of the "void"
   pointer as the element.  These functions are often useful for
   maintaining sets of objects. */

set_Set set_create( unsigned int (*hash)( const void * ),
		    int (*compare)( const void *,
				    const void * ) )
{
   return _set_create( 0, hash, compare );
}

static void _set_destroy_buckets( set_Set set )
{
   int     i;
   setType t = (setType)set;
   int     size = _hsh_primes[ t->prime ];

   for (i = 0; i < size; i++) {
      bucketType b = t->buckets[i];

      while (b) {
	 bucketType next = b->next;
	 
	 xfree( b );
	 b = next;
      }
   }

   xfree( t->buckets );
   t->buckets = NULL;
}

static void _set_destroy_table( set_Set set )
{
   xfree( set );
}

/* \doc |set_destroy| frees all of the memory associated with the set
   object.

   The memory used by elements is \emph{not} freed---this memory is the
   responsibility of the user.  However, a call to |set_iterate| can be
   used to free this memory \emph{immediately} before a call to
   |set_destroy|. */

void set_destroy( set_Set set )
{
   _set_destroy_buckets( set );
   _set_destroy_table( set );
}

static void _set_insert( set_Set set, unsigned int hash, const void *elem )
{
   setType    t    = (setType)set;
   int        size = _hsh_primes[ t->prime ];
   int        h    = hash % size;
   bucketType b;

   b        = xmalloc( sizeof( struct bucket ) );
   b->hash  = hash;
   b->elem  = elem;
   b->next  = NULL;
   
   if (t->buckets[h]) b->next = t->buckets[h];
   t->buckets[h] = b;
   ++t->entries;
}

/* \doc |set_insert| inserts a new |elem| into the |set|.  If the insertion
   is successful, zero is returned.  If the |elem| already exists, 1 is
   returned.

   If the internal representation of the set becomes more than half full,
   its size is increased automatically.  At present, this requires that all
   of the element pointers are copied into a new set.  Rehashing is not
   required, however, since the hash values are stored for each element. */

int set_insert( set_Set set, const void *elem )
{
   setType      t         = (setType)set;
   int          size      = _hsh_primes[ t->prime ];
   unsigned int hashValue = t->hash( elem );
   unsigned int h;

				/* Keep table less than half full */
   if (t->entries * 2 > size && _hsh_primes[ t->prime + 1 ]) {
      setType new = _set_create( t->prime + 1, t->hash, t->compare );
      int     i;

      for (i = 0; i < size; i++) {
	 if (t->buckets[i]) {
	    bucketType pt;

	    for (pt = t->buckets[i]; pt; pt = pt->next)
		  _set_insert( new, pt->hash, pt->elem );
	 }
      }

				/* fixup values */
      _set_destroy_buckets( t );
      ++t->prime;
      t->buckets = new->buckets;
      _set_destroy_table( new );
      
      size       = _hsh_primes[ t->prime ];
   }
   
   h = hashValue % size;

   if (t->buckets[h]) {		/* Assert uniqueness */
      bucketType pt;
      
      for (pt = t->buckets[h]; pt; pt = pt->next)
	    if (!t->compare( pt->elem, elem )) return 1;
   }

   _set_insert( t, hashValue, elem );
   return 0;
}

/* \doc |set_delete| removes an |elem| from the |set|.  Zero is returned if
   the |elem| was present.  Otherwise, 1 is returned. */

int set_delete( set_Set set, const void *elem )
{
   setType    t    = (setType)set;
   int        size = _hsh_primes[ t->prime ];
   int        h    = t->hash( elem ) % size;

   if (t->buckets[h]) {
      bucketType pt;
      bucketType prev;
      
      for (prev = NULL, pt = t->buckets[h]; pt; prev = pt, pt = pt->next)
	    if (!t->compare( pt->elem, elem )) {
	       --t->entries;
	       
	       if (!prev) t->buckets[h] = NULL;
	       else       prev->next = pt->next;
	       
	       xfree( pt );
	       return 0;
	    }
   }
   
   return 1;
}

/* \doc |set_member| returns 1 if |elem| is in |set|.  Otherwise, zero is
   returned. */

int set_member( set_Set set, const void *elem )
{
   setType      t    = (setType)set;
   int          size = _hsh_primes[ t->prime ];
   unsigned int h    = t->hash( elem ) % size;

   ++t->retrievals;
   if (t->buckets[h]) {
      bucketType pt;
      bucketType prev;
      
      for (prev = NULL, pt = t->buckets[h]; pt; prev = pt, pt = pt->next)
	    if (!t->compare( pt->elem, elem )) {
	       if (!prev) {
		  ++t->hits;
	       } else {
				/* Self organize */
		  prev->next    = pt->next;
		  pt->next      = t->buckets[h];
		  t->buckets[h] = pt;
	       }
	       return 1;
	    }
   }

   ++t->misses;
   return 0;
}

/* \doc |set_iterate| is used to iterate a function over every |elem| in
   the |set|.  The function, |iterator|, is passed each |elem|.  If
   |iterator| returns a non-zero value, the iterations stop, and
   |set_iterate| returns.  Note that the elements are in some arbitrary
   order, and that this order may change between two successive calls to
   |set_iterate|. */

void set_iterate( set_Set set,
		  int (*iterator)( const void *elem ) )
{
   setType   t    = (setType)set;
   int       size = _hsh_primes[ t->prime ];
   int       i;

   for (i = 0; i < size; i++) {
      if (t->buckets[i]) {
	 bucketType pt;
	 
	 for (pt = t->buckets[i]; pt; pt = pt->next)
	       if (iterator( pt->elem ))
		     return;
      }
   }
}

/* \doc |set_union| returns a new set which is the union of |set1| and
   |set2|.  Only pointers to elements are copied, \emph{not} the data
   pointed (this has memory management implications).  The |hash| and
   |compare| functions must be identical for the two sets. */

set_Set set_union( set_Set set1, set_Set set2 )
{
   setType t1    = (setType)set1;
   setType t2    = (setType)set2;
   int     size1 = _hsh_primes[ t1->prime ];
   int     size2 = _hsh_primes[ t2->prime ];
   set_Set set;
   int     i;

   if (t1->hash != t2->hash)
	 err_fatal( __FUNCTION__,
		    "Sets do not have identical hash functions\n" );

   if ( t1->compare != t2->compare )
	 err_fatal( __FUNCTION__,
		    "Sets do not have identical comparison functions\n" );

   set = set_create( t1->hash, t1->compare );

   for (i = 0; i < size1; i++) {
      if (t1->buckets[i]) {
	 bucketType pt;
	 
	 for (pt = t1->buckets[i]; pt; pt = pt->next)
	       set_insert( set, pt->elem );
      }
   }
   
   for (i = 0; i < size2; i++) {
      if (t2->buckets[i]) {
	 bucketType pt;
	 
	 for (pt = t2->buckets[i]; pt; pt = pt->next)
	       set_insert( set, pt->elem );
      }
   }

   return set;
}

/* \doc |set_inter| returns a new set which is the intersection of |set1|
   and |set2|.  Only pointers to elements are copied, \emph{not} the data
   pointed (this has memory management implications).  The |hash| and
   |compare| functions must be identical for the two sets. */

set_Set set_inter( set_Set set1, set_Set set2 )
{
   setType t1    = (setType)set1;
   setType t2    = (setType)set2;
   int     size1 = _hsh_primes[ t1->prime ];
   set_Set set;
   int     i;

   if (t1->hash != t2->hash)
	 err_fatal( __FUNCTION__,
		    "Sets do not have identical hash functions\n" );

   if ( t1->compare != t2->compare )
	 err_fatal( __FUNCTION__,
		    "Sets do not have identical comparison functions\n" );

   set = set_create( t1->hash, t1->compare );

   for (i = 0; i < size1; i++) {
      if (t1->buckets[i]) {
	 bucketType pt;
	 
	 for (pt = t1->buckets[i]; pt; pt = pt->next)
	       if (set_member( t2, pt->elem ))
		   set_insert( set, pt->elem );
      }
   }

   return set;
}

/* \doc |set_diff| returns a new set which is the difference resulting from
   removing every element in |set2| from the elements in |set1|.  Only
   pointers to elements are copied, \emph{not} the data pointed (this has
   memory management implications).  The |hash| and |compare| functions
   must be identical for the two sets. */

set_Set set_diff( set_Set set1, set_Set set2 )
{
   setType t1    = (setType)set1;
   setType t2    = (setType)set2;
   int     size1 = _hsh_primes[ t1->prime ];
   set_Set set;
   int     i;

   if (t1->hash != t2->hash)
	 err_fatal( __FUNCTION__,
		    "Sets do not have identical hash functions\n" );

   if ( t1->compare != t2->compare )
	 err_fatal( __FUNCTION__,
		    "Sets do not have identical comparison functions\n" );

   set = set_create( t1->hash, t1->compare );

   for (i = 0; i < size1; i++) {
      if (t1->buckets[i]) {
	 bucketType pt;
	 
	 for (pt = t1->buckets[i]; pt; pt = pt->next)
	       if (!set_member( t2, pt->elem ))
		   set_insert( set, pt->elem );
      }
   }

   return set;
}

/* \doc |set_get_stats| returns statistics about the |set|.  The
   |set_Stats| structure is shown in \grind{set_Stats}. */

set_Stats set_get_stats( set_Set set )
{
   setType   t    = (setType)set;
   int       size = _hsh_primes[ t->prime ];
   set_Stats s    = xmalloc( sizeof( struct set_Stats ) );
   int       i;
   int       count;

   s->size           = size;
   s->resizings      = t->prime;
   s->entries        = 0;
   s->buckets_used   = 0;
   s->singletons     = 0;
   s->maximum_length = 0;
   s->retrievals     = t->retrievals;
   s->hits           = t->hits;
   s->misses         = t->misses;
   
   for (i = 0; i < size; i++) {
      if (t->buckets[i]) {
	 bucketType pt;
	 
	 ++s->buckets_used;
	 for (count = 0, pt = t->buckets[i]; pt; ++count, pt = pt->next);
	 if (count == 1) ++s->singletons;
	 s->maximum_length = max( s->maximum_length, count );
	 s->entries += count;
      }
   }

   if ( t->entries != s->entries )
	 err_internal( __FUNCTION__,
		       "Incorrect count for entries: %d vs. %d\n",
		       t->entries,
		       s->entries );


   return s;
}

/* \doc |set_print_stats| prints the statistics for |set| on the specified
   |stream|.  If |stream| is "NULL", then "stdout" will be used. */

void set_print_stats( set_Set set, FILE *stream )
{
   setType    t   = (setType)set;
   FILE      *str = stream ? stream : stdout;
   set_Stats  s   = set_get_stats( t );

   fprintf( str, "Statistics for set at %p:\n", set );
   fprintf( str, "   %d resizings to %d total\n", s->resizings, s->size );
   fprintf( str, "   %d entries (%d buckets used, %d without overflow)\n",
	    s->entries, s->buckets_used, s->singletons );
   fprintf( str, "   maximum list length is %d", s->maximum_length );
   if (s->buckets_used)
	 fprintf( str, " (optimal is %.1f)\n",
		  (double)s->entries / (double)s->buckets_used );
   else
	 fprintf( str, "\n" );
   fprintf( str, "   %d retrievals (%d from top, %d failed)\n",
	    s->retrievals, s->hits, s->misses );

   xfree( s );
}
