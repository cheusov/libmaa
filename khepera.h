/* khepera.h -- Header file for visible Khepera functions
 * Created: Thu Nov  3 19:48:30 1994 by faith@cs.unc.edu
 * Revised: Thu Mar  9 14:41:56 1995 by faith@cs.unc.edu
 * Copyright 1994 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: khepera.h,v 1.1 1995/04/21 15:31:47 faith Exp $
 */

#ifndef _KHEPERA_H_
#define _KHEPERA_H_

#include <stdio.h>

#ifndef __GCC__
#define __inline__
#define __attribute__(x)
#endif

/* kh.c */

#define KH_FAREY 0xc0000001	/* Farey fraction generation */
#define KH_TRE   0xc0000002	/* Tree libary  */

extern void kh_init( void );

/* xmalloc.c */

#ifndef DMALLOC_FUNC_CHECK
extern __inline__ void *xmalloc( unsigned int size );
extern __inline__ void *xcalloc( unsigned int num, unsigned int size );
extern __inline__ void *xrealloc( void *pt, unsigned int size );
extern __inline__ void xfree( void *pt );
extern __inline__ char *xstrdup( const char *s );
#endif

/* hash.c */

typedef void *hsh_HashTable;

typedef struct hsh_Stats {
   int size;			/* Size of table */
   int resizings;		/* Number of resizings */
   int entries;			/* Total entries in table */
   int buckets_used;		/* Number of hash buckets in use */
   int singletons;		/* Number of length one lists */
   int maximum_length;		/* Maximum list length */

   int retrievals;		/* Total number of retrievals */
   int hits;			/* Number of retrievals from top of a list */
   int misses;			/* Number of unsuccessful retrievals */
} *hsh_Stats;

extern hsh_HashTable hsh_create( unsigned int (*hash)( const void * ),
				 int (*compare)( const void *,
						 const void * ) );
extern void          hsh_destroy( hsh_HashTable table );
extern int           hsh_insert( hsh_HashTable table,
				 const void *key,
				 const void *datum );
extern int           hsh_delete( hsh_HashTable table, const void *key );
extern const void    *hsh_retrieve( hsh_HashTable table, const void *key );
extern void          hsh_iterate( hsh_HashTable table,
				  int (*iterator)( const void *key,
						   const void *datum ) );
extern hsh_Stats     hsh_get_stats( hsh_HashTable table );
extern void          hsh_print_stats( hsh_HashTable table, FILE *stream );
extern unsigned int  hsh_string_hash( const void *key );
extern unsigned int  hsh_pointer_hash( const void *key );
extern int           hsh_string_compare( const void *key1, const void *key2 );
extern int           hsh_pointer_compare( const void *key1, const void *key2 );

/* set.c */

typedef void *set_Set;

typedef struct set_Stats{
   int size;			/* Size of table */
   int resizings;		/* Number of resizings */
   int entries;			/* Total entries in table */
   int buckets_used;		/* Number of hash buckets in use */
   int singletons;		/* Number of length one lists */
   int maximum_length;		/* Maximum list length */

   int retrievals;		/* Total number of retrievals */
   int hits;			/* Number of retrievals from top of a list */
   int misses;			/* Number of unsuccessful retrievals */
} *set_Stats;

extern set_Set   set_create( unsigned int (*hash)( const void * ),
			     int (*compare)( const void *, const void * ) );
extern void      set_destroy( set_Set set );
extern int       set_insert( set_Set set, const void *elem );
extern int       set_delete( set_Set set, const void *elem );
extern int       set_member( set_Set set, const void *elem );
extern void      set_iterate( set_Set set,
			      int (*iterator)( const void *key ) );
extern set_Set   set_union( set_Set set1, set_Set set2 );
extern set_Set   set_inter( set_Set set1, set_Set set2 );
extern set_Set   set_diff( set_Set set1, set_Set set2 );
extern set_Stats set_get_stats( set_Set set );
extern void      set_print_stats( set_Set set, FILE *stream );

/* stack.c */

typedef void *stk_Stack;

extern stk_Stack stk_create( void );
extern void      stk_destroy( stk_Stack stack );
extern void      stk_push( stk_Stack stack, void *datum );
extern void      *stk_pop( stk_Stack stack );
extern void      *stk_top( stk_Stack stack );

/* error.c */

extern void err_fatal( const char *routine, const char *format, ... )
   __attribute__((noreturn,format(printf, 2, 3)));

extern void err_warning( const char *routine, const char *format, ... )
   __attribute__((format(printf, 2, 3)));

extern void err_internal( const char *routine, const char *format, ... )
   __attribute__((noreturn,format(printf, 2, 3)));

/* memory.c */

typedef void *mem_String;
typedef void *mem_Object;

typedef struct mem_StringStats {
   int count;			/* Number of strings or objects */
   int bytes;			/* Number of bytes allocated */
} *mem_StringStats;

typedef struct mem_ObjectStats {
   int total;			/* Total objects requested */
   int used;			/* Total currently in use */
   int reused;			/* Total reused  */
} *mem_ObjectStats;

extern mem_String      mem_create_strings( void );
extern void            mem_destroy_strings( mem_String info );
extern char            *mem_strcpy( mem_String info, const char *string );
extern char            *mem_strncpy( mem_String info,
				     const char *string,
				     int len );
extern mem_StringStats mem_get_string_stats( mem_String info );
extern void            mem_print_string_stats( mem_String info, FILE *stream );

extern mem_Object      mem_create_objects( int size );
extern void            mem_destroy_objects( mem_Object info );
extern void            *mem_get_object( mem_Object info );
extern void            mem_free_object( mem_Object info, void *obj );
extern mem_ObjectStats mem_get_object_stats( mem_Object info );
extern void            mem_print_object_stats( mem_Object info, FILE *stream );

/* string.c */

typedef void *str_Pool;

extern str_Pool   str_pool_create( void );
extern void       str_pool_destroy( str_Pool pool );
extern const char *str_pool_find( str_Pool pool, const char *s );
extern const char *str_find( const char *s );
extern const char *str_findn( const char *s, int length );
extern void       str_destroy( void );

/* debug.c */

typedef unsigned long int dbg_Type;

extern void           dbg_register( dbg_Type flag, const char *name );
extern void           _dbg_register( dbg_Type flag, const char *name );
extern void           dbg_destroy( void );
extern void           dbg_set( const char *name );
extern __inline__ int dbg_test( dbg_Type flag );

#define PRINTF(flag,arg) if (dbg_test(flag)) printf arg

/* source.c */

typedef void *src_Type;

extern void     src_create( void );
extern void     src_destroy( void );
extern char     *src_line( const char *line, int len );
extern void     src_new_file( const char *filename );
extern void     src_new_line( int line );
extern void     src_advance( int length );
extern src_Type src_get( int length );

/* tree.c */

				/* These names are provided for debugging
				   purposes only.  You are absolutely
				   forbidden to access any of the fields in
				   TRE_HEADER! */
#ifndef TRE_STRUCT_NAME
#define TRE_STRUCT_NAME Node
#endif

#define TRE_HEADER                              \
   short                  id;                   \
   unsigned short         head:1;               \
   unsigned short         farey:1;              \
   unsigned short         red:1;                \
   short                  ln, ld, n, d, rn, rd; \
   struct TRE_STRUCT_NAME *right;               \
   struct TRE_STRUCT_NAME *left;                \
   struct TRE_STRUCT_NAME *down;                \
   struct TRE_STRUCT_NAME *parent;              \
   struct TRE_STRUCT_NAME *rb_left;             \
   struct TRE_STRUCT_NAME *rb_right;            \
   src_Type               src

typedef void *tre_Node;

typedef struct tre_Stats {
   int created;			/* Number of nodes created */
   int inuse;			/* Number of nodes currently in use */
   int reused;			/* Number of nodes reused */
} *tre_Stats;

extern void       tre_set_size( int size );
extern void       tre_register( int id,
				const char *name,
				void (*create_callback)( tre_Node node ),
				void (*destroy_callback)( tre_Node node ) );
extern void       tre_register_name( int id, const char *name );
extern void       tre_register_callbacks( int id,
				  void (*create_callback)( tre_Node node ),
				  void (*destroy_callback)( tre_Node node ));
extern const char *tre_name( int id );
extern tre_Node   tre_parent( tre_Node child );
extern tre_Node   tre_child( tre_Node parent );
extern tre_Node   tre_right( tre_Node node );
extern tre_Node   tre_left( tre_Node node );
extern int        tre_id( tre_Node node );
extern tre_Node   tre_has_id( tre_Node node, int id );
extern int        parent_id( tre_Node node );
extern tre_Node   parent_has_id( tre_Node node, int id );
extern tre_Node   ancestor_has_id( tre_Node node, int id );
extern tre_Node   child_has_id( tre_Node node, int id );
extern tre_Node   descendent_has_id( tre_Node node, int id );
extern tre_Node   tre_find_ancestor( tre_Node current,
				     int (*f)( tre_Node node ) );
extern tre_Node   tre_find_right( tre_Node current,
				  int (*f)( tre_Node node ) );
extern tre_Node   tre_find_left( tre_Node current, int (*f)( tre_Node node ) );
extern tre_Node   tre_find_descendant( tre_Node current,
				       int (*f)( tre_Node node ) );
extern int        same_id( tre_Node node1, tre_Node node2 );
extern tre_Node   tre_create( int id );
extern void       tre_append( tre_Node parent, tre_Node child );
extern void       tre_prepend( tre_Node parent, tre_Node child );
extern void       tre_insert_before( tre_Node sibling, tre_Node child );
extern void       tre_insert_after( tre_Node sibling, tre_Node child );
extern void       tre_destroy( tre_Node node );
extern tre_Node   tre_disconnect( tre_Node node );
extern void       tre_delete( tre_Node node );
extern tre_Node   tre_mk( int id, ... );
extern tre_Node   tre_copy( tre_Node node );
extern int        tre_match( tre_Node node1, tre_Node node2 );
extern void       tre_fast_init( tre_Node root );
extern int        tre_fast_postorder( tre_Node root,
				      int targetID,
				      int (*f)( tre_Node node ) );
extern int        tre_preorder( tre_Node parent, int (*f)( tre_Node node ) );
extern int        tre_preorder_children( tre_Node parent,
					 int (*f)( tre_Node node ) );
extern int        tre_preorder_right( tre_Node node,
				      int (*f)( tre_Node node ) );
extern int        tre_postorder( tre_Node parent, int (*f)( tre_Node node ) );
extern int        tre_postorder_children( tre_Node parent,
					  int (*f)( tre_Node node ) );
extern int        tre_postorder_right( tre_Node node,
				       int (*f)( tre_Node node ) );
extern int        tre_iterate( tre_Node parent,
			       int (*f)( tre_Node node, int position ) );
extern void       tre_print( tre_Node parent, FILE *stream );
extern void       _tre_rb_dump( int id );
extern tre_Stats  tre_get_stats( void );
extern void       tre_print_stats( FILE *stream );

#endif
