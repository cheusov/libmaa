/* hash.c -- Hash table routines for Khepera
 * Created: Thu Nov  3 20:07:29 1994 by faith@cs.unc.edu
 * Revised: Thu Mar  9 10:13:05 1995 by faith@cs.unc.edu
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
 * $Id: hash.c,v 1.1 1995/04/21 15:31:47 faith Exp $
 *
 * \section{Hash Table Routines}
 *
 * \intro Generic hash table support is provided for storing generic data
 * associated with keys.  The hash table has prime length, with
 * self-organizing linked lists \cite[pp.~398--9]{faith:Knuth73c} used for
 * collision resolution. The hash table automatically grows as necessary to
 * preserve efficient access.
 *
 */

#include "kh.h"

				/* A few primes, each approximately 3 times
                                   the previous prime.  */

int _hsh_primes[] = { 5, 17, 53, 163, 491, 1481, 4447, 13367, 40111,
		      120349, 361069, 1083211, 3249641, 9748927, 0 };

typedef struct bucket {
   const void    *key;
   unsigned int   hash;
   const void    *datum;
   struct bucket *next;
} *bucketType;

typedef struct table {
   int            prime;
   int            entries;
   bucketType    *buckets;
   int            retrievals;
   int            hits;
   int            misses;
   unsigned int (*hash)( const void * );
   int          (*compare)( const void *, const void * );
} *tableType;
   

static hsh_HashTable _hsh_create( int prime,
				  unsigned int (*hash)( const void * ),
				  int (*compare)( const void *,
						  const void * ))
{
   tableType t;
   int       i;
   int       size = _hsh_primes[ prime ];
   
   t             = xmalloc( sizeof( struct table ) );
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

/* \doc The |hsh_create| function initilizes a generic hash table.  Keys
   and data are pointers to "void".

   The internal representation of the hash table will grow automatically
   when an insertion is performed and the table is more than half full.
   The table size progresses through the following series of primes:
   \val{_hsh_primes}.

   The |hash| function should take a pointer to a |key| and return an
   "unsigned int".  If |hash| is "NULL", then the |key| is assumed to be a
   pointer to a null-terminated string, and the function shown in
   \grind{hsh_string_hash} will be used for |hash| (the algorithm for this
   function is from \cite[p.~435]{faith:Aho88}).

   The |compare| function should take a pair of pointers to keys and return
   zero if the keys are equal and non-zero if the keys are not equal.  If
   |compare| is "NULL", then the keys are assumed to point to
   null-terminated strings, and the |strcmp| function will be used for
   |compare|.

   Additionally, the |hsh_pointer_hash| and |hsh_pointer_compare| functions
   are available and can be used to treat the \emph{value} of the "void"
   pointer as the key.  These functions are often useful for maintaining
   sets of objects. */

hsh_HashTable hsh_create( unsigned int (*hash)( const void * ),
			  int (*compare)( const void *,
					  const void * ) )
{
   return _hsh_create( 0, hash, compare );
}

static void _hsh_destroy_buckets( hsh_HashTable table )
{
   int       i;
   tableType t    = (tableType)table;
   int       size = _hsh_primes[ t->prime ];

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

static void _hsh_destroy_table( hsh_HashTable table )
{
   xfree( table );
}

/* \doc |hsh_destroy| frees all of the memory associated with the hash
   table.

   The memory used by keys and data is \emph{not} freed---this memory is
   the responsibility of the user.  However, a call to |hsh_iterate|
   can be used to free this memory \emph{immediately} before a call to
   |hsh_destroy|. */

void hsh_destroy( hsh_HashTable table )
{
   _hsh_destroy_buckets( table );
   _hsh_destroy_table( table );
}

static void _hsh_insert( hsh_HashTable table,
			 unsigned int hash,
			 const void *key,
			 const void *datum )
{
   tableType    t    = (tableType)table;
   int          size = _hsh_primes[ t->prime ];
   unsigned int h    = hash % size;
   bucketType   b;
   
   b        = xmalloc( sizeof( struct bucket ) );
   b->key   = key;
   b->hash  = hash;
   b->datum = datum;
   b->next  = NULL;
   
   if (t->buckets[h]) b->next = t->buckets[h];
   t->buckets[h] = b;
   ++t->entries;
}

/* \doc |hsh_insert| inserts a new |key| into the |table|.  If the
   insertion is successful, zero is returned.  If the |key| already exists,
   1 is returned.  Hence, the way to change the |datum| associated with a
   |key| is first to call |hsh_delete|.

   If the internal representation of the hash table becomes more than half
   full, its size is increased automatically.  At present, this requires
   that all of the key pointers are copied into a new table.  Rehashing is
   not required, however, since the hash values are stored for each key. */

int hsh_insert( hsh_HashTable table,
		const void *key,
		const void *datum )
{
   tableType    t         = (tableType)table;
   int          size      = _hsh_primes[ t->prime ];
   unsigned int hashValue = t->hash( key );
   unsigned int h;

				/* Keep table less than half full */
   if (t->entries * 2 > size && _hsh_primes[ t->prime + 1 ]) {
      tableType new = _hsh_create( t->prime + 1, t->hash, t->compare );
      int       i;

      for (i = 0; i < size; i++) {
	 if (t->buckets[i]) {
	    bucketType pt;

	    for (pt = t->buckets[i]; pt; pt = pt->next)
		  _hsh_insert( new, pt->hash, pt->key, pt->datum );
	 }
      }

				/* fixup values */
      _hsh_destroy_buckets( t );
      ++t->prime;
      t->buckets = new->buckets;
      _hsh_destroy_table( new );
      
      size       = _hsh_primes[ t->prime ];
   }

   h = hashValue % size;

   if (t->buckets[h]) {		/* Assert uniqueness */
      bucketType pt;
      
      for (pt = t->buckets[h]; pt; pt = pt->next)
	    if (!t->compare( pt->key, key )) return 1;
   }

   _hsh_insert( t, hashValue, key, datum );
   return 0;
}

/* \doc |hsh_delete| removes a |key| and the associated datum from the
   |table|.  Zero is returned if the |key| was present.  Otherwise, 1 is
   returned. */

int hsh_delete( hsh_HashTable table, const void *key )
{
   tableType    t    = (tableType)table;
   int          size = _hsh_primes[ t->prime ];
   unsigned int h    = t->hash( key ) % size;

   if (t->buckets[h]) {
      bucketType pt;
      bucketType prev;
      
      for (prev = NULL, pt = t->buckets[h]; pt; prev = pt, pt = pt->next)
	    if (!t->compare( pt->key, key )) {
	       --t->entries;
	       
	       if (!prev) t->buckets[h] = NULL;
	       else       prev->next = pt->next;
	       
	       xfree( pt );
	       return 0;
	    }
   }
   
   return 1;
}


/* \doc |hsh_retrieve| retrieves the datum associated with a |key|.  If the
   |key| is not present in the |table|, then "NULL" is returned. */

const void *hsh_retrieve( hsh_HashTable table,
			  const void *key )
{
   tableType    t    = (tableType)table;
   int          size = _hsh_primes[ t->prime ];
   unsigned int h    = t->hash( key ) % size;

   ++t->retrievals;
   if (t->buckets[h]) {
      bucketType pt;
      bucketType prev;
      
      for (prev = NULL, pt = t->buckets[h]; pt; prev = pt, pt = pt->next)
	    if (!t->compare( pt->key, key )) {
	       if (!prev) {
		  ++t->hits;
	       } else {
				/* Self organize */
		  prev->next    = pt->next;
		  pt->next      = t->buckets[h];
		  t->buckets[h] = pt;
	       }
	       return pt->datum;
	    }
   }

   ++t->misses;
   return NULL;
}

/* \doc |hsh_iterate| is used to iterate a function over every value in the
   |table|.  The function, |iterator|, is passed the |key| and |datum| pair
   for each entry in the table.  If |iterator| returns a non-zero value,
   the iterations stop, and |hsh_iterate| returns.  Note that the keys are
   in some arbitrary order, and that this order may change between two
   successive calls to |hsh_iterate|. */

void hsh_iterate( hsh_HashTable table,
		  int (*iterator)( const void *key,
				   const void *datum ) )
{
   tableType    t    = (tableType)table;
   int          size = _hsh_primes[ t->prime ];
   unsigned int i;

   for (i = 0; i < size; i++) {
      if (t->buckets[i]) {
	 bucketType pt;
	 
	 for (pt = t->buckets[i]; pt; pt = pt->next)
	       if (iterator( pt->key, pt->datum ))
		     return;
      }
   }
}

/* \doc |hsh_get_stats| returns statistics about the |table|.  The
   |hsh_Stats| structure is shown in \grind{hsh_Stats}. */

hsh_Stats hsh_get_stats( hsh_HashTable table )
{
   tableType    t    = (tableType)table;
   int          size = _hsh_primes[ t->prime ];
   hsh_Stats    s    = xmalloc( sizeof( struct hsh_Stats ) );
   int          i;
   int          count;

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
   if (t->entries != s->entries )
	 err_internal( __FUNCTION__,
		       "Incorrect count for entries: %d vs. %d\n",
		       t->entries,
		       s->entries );

   return s;
}

/* \doc |hsh_print_stats| prints the statistics for |table| on the
   specified |stream|.  If |stream| is "NULL", then "stdout" will be
   used. */

void hsh_print_stats( hsh_HashTable table, FILE *stream )
{
   FILE      *str = stream ? stream : stdout;
   hsh_Stats s    = hsh_get_stats( table );

   fprintf( str, "Statistics for hash table at %p:\n", table );
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

unsigned int hsh_string_hash( const void *key )
{
   const char   *pt = (char *)key;
   unsigned int  h  = 0;

   while (*pt) {
      h += *pt++;
      h *= 65599L;		/* prime near %$2^{16}$% */
   }

   return h;
}

unsigned int hsh_pointer_hash( const void *key )
{
   const char   *pt = (char *)&key;
   unsigned int  h  = 0;
   int          i;

   for (i = 0; i < SIZEOF_VOID_P; i++) {
      h += *pt++;
      h *= 65599L;		/* prime near %$2^{16}$% */
   }

   return h;
}

int hsh_string_compare( const void *key1, const void *key2 )
{
   return strcmp( key1, key2 );
}

int hsh_pointer_compare( const void *key1, const void *key2 )
{
   const char *p1 = (const char *)&key1;
   const char *p2 = (const char *)&key2;
   int  i;

   for (i = 0; i < SIZEOF_VOID_P; i++) if (*p1++ != *p2++) return 1;
   return 0;
}
