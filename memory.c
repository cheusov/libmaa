/* memory.c -- Memory management for Khepera
 * Created: Thu Dec 22 09:58:38 1994 by faith@cs.unc.edu
 * Revised: Mon Jan 23 10:02:58 1995 by faith@cs.unc.edu
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
 * $Id: memory.c,v 1.1 1995/04/21 15:31:47 faith Exp $
 *
 * \section{Memory Management Routines}
 *
 * \intro The memory management routines provide simple support for string
 * object storage.  These routines are generally used as building blocks by
 * other parts of the \khepera library (e.g., string pools and abstract
 * syntax trees).
 *
 */

#include "kh.h"
#include "obstack.h"

#ifdef DMALLOC_FUNC_CHECK
				/* Must be true functions */
#define obstack_chunk_alloc malloc
#define obstack_chunk_free  free
#else
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free  xfree
#endif

typedef struct stringInfo {
   int            count;
   int            bytes;
   struct obstack *obstack;
} *stringInfo;

typedef struct objectInfo {
   int             total;
   int             used;
   int             reused;
   int             size;
   void           *stack;	/* for free list */
   struct obstack *obstack;
} *objectInfo;

/* \doc |mem_create_strings| creates a memory object for storing strings. */

mem_String mem_create_strings( void )
{
   stringInfo info = xmalloc( sizeof( struct stringInfo ) );

   info->count   = 0;
   info->bytes   = 0;
   info->obstack = xmalloc( sizeof( struct obstack ) );
   obstack_init( info->obstack );

   obstack_alignment_mask( info->obstack ) = 0; /* no alignment for chars */

   return info;
}

/* \doc |mem_destroy_strings| destroys the memory object returned from
   |mem_create_strings|.  All memory if freed, including that used for the
   strings.  Therefore, any pointers to strings in the table will be left
   dangling. */

void mem_destroy_strings( mem_String info )
{
   stringInfo i = (stringInfo)info;
   
   obstack_free( i->obstack, NULL );
   xfree( i->obstack );
   xfree( i );
}

/* \doc |mem_strcpy| copies a |string| into the memory object pointed to by
   |info|. */

char *mem_strcpy( mem_String info, const char *string )
{
   stringInfo i   = (stringInfo)info;
   int        len = strlen( string );

   ++i->count;
   i->bytes += len + 1;
   
   return obstack_copy0( i->obstack, string, len );
}

/* \doc |mem_strncpy| copies |len| bytes of |string| into the memory object
   pointed to by |info|.  A null is added to the end of the copied
   sequence. */

char *mem_strncpy( mem_String info,
		   const char *string, int len )
{
   stringInfo i   = (stringInfo)info;

   ++i->count;
   i->bytes += len + 1;
   
   return obstack_copy0( i->obstack, string, len );
}

/* \doc |mem_get_string_stats| returns statistics about the memory object
   pointed to by |info|.  The |mem_StringStats| structure is shown in
   \grind{mem_StringStats}. */

mem_StringStats mem_get_string_stats( mem_String info )
{
   stringInfo      i = (stringInfo)info;
   mem_StringStats s = xmalloc( sizeof( struct mem_StringStats ) );

   s->count = i->count;
   s->bytes = i->bytes;

   return s;
}

/* \doc |mem_print_string_stats| prints the statistics for the memory
   object pointed to by |info| on the specified |stream|.  If |stream| is
   "NULL", then "stdout" will be used. */

void mem_print_string_stats( mem_String info, FILE *stream )
{
   FILE            *str = stream ? stream : stdout;
   mem_StringStats s    = mem_get_string_stats( info );

   fprintf( str, "Statistics for string memory manager at %p:\n", info );
   fprintf( str, "   %d strings, using %d bytes\n", s->count, s->bytes );

   xfree( s );
}

/* \doc |mem_create_objects| creates a memory storage object for object of
   |size| bytes.  */

mem_Object mem_create_objects( int size )
{
   objectInfo info = xmalloc( sizeof ( struct objectInfo ) );

   info->total   = 0;
   info->used    = 0;
   info->reused  = 0;
   info->size    = size;
   info->stack   = stk_create();
   info->obstack = xmalloc( sizeof( struct obstack ) );
   obstack_init( info->obstack );

   return info;
}

/* \doc |mem_destroy_objects| destroys the memory object returned from
   |mem_create_objects|.  All memory if freed, including that used for the
   object.  Therefore, any pointers to objects stored by |info| will be
   left dangling. */

void mem_destroy_objects( mem_Object info )
{
   objectInfo i = (objectInfo)info;
   
   stk_destroy( i->stack );
   obstack_free( i->obstack, NULL );
   xfree( i->obstack );
   xfree( i );
}

/* \doc |mem_get_object| returns a pointer to a block of memory which is
   |size| bytes long (as specified in the call to |mem_create_objects|).
   This block is either newly allocated memory, or is memory which was
   previously allocated by |mem_get_object| and subsequently freed by
   |mem_free_object|. */

void *mem_get_object( mem_Object info )
{
   objectInfo  i   = (objectInfo)info;
   void       *obj = stk_pop( i->stack );

   if (!obj) {
      obj = obstack_alloc( i->obstack, i->size );
      ++i->total;
   } else ++i->reused;

   ++i->used;
   
   return obj;
}

/* \doc |mem_free_object| ``frees'' the object, |obj|, which was previously
   obtained from |mem_get_object|.  The memory associated with the object
   is not actually freed, but the object pointer is stored on a stack, and
   is available for subsequent calls to |mem_get_object|. */

void mem_free_object( mem_Object info, void *obj )
{
   objectInfo i = (objectInfo)info;

   stk_push( i->stack, obj );
   --i->used;
}

/* \doc |mem_get_object_stats| returns statistics about the memory object
   pointed to by |info|.  The |mem_ObjectStats| structure is shown in
   \grind{mem_ObjectStats}. */

mem_ObjectStats mem_get_object_stats( mem_Object info )
{
   objectInfo      i = (objectInfo)info;
   mem_ObjectStats s = xmalloc( sizeof( struct mem_ObjectStats ) );

   s->total  = i->total;
   s->used   = i->used;
   s->reused = i->reused;

   return s;
}

/* \doc |mem_print_object_stats| prints the statistics for the memory
   object pointed to by |info| on the specified |stream|.  If |stream| is
   "NULL", then "stdout" will be used. */

void mem_print_object_stats( mem_Object info, FILE *stream )
{
   FILE            *str = stream ? stream : stdout;
   mem_ObjectStats s    = mem_get_object_stats( info );

   fprintf( str, "Statistics for object memory manager at %p:\n", info );
   fprintf( str,
	    "   %d objects allocated, of which %d are in use\n",
	    s->total, s->used );
   fprintf( str, "   %d objects have been reused\n", s->reused );

   xfree( s );
}
