/* khepera.h -- Header file for visible Khepera functions
 * Created: Thu Nov  3 19:48:30 1994 by faith@cs.unc.edu
 * Revised: Mon Nov  6 13:30:34 1995 by faith@cs.unc.edu
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
 * $Id: khepera.h,v 1.26 1995/11/08 05:32:06 yakowenk Exp $
 */

#ifndef _KHEPERA_H_
#define _KHEPERA_H_

#include <stdio.h>
#include <stdarg.h>

#ifndef __GCC__
#define __inline__
#define __attribute__(x)
#endif

				/* If KH_MAGIC is non-zero, important
                                   structures will be tagged with a magic
                                   number which will be checked for
                                   integrity at each access.  This uses up
                                   more memory, and is slightly slower, but
                                   helps debugging quite a bit. */
#ifndef KH_MAGIC
#define KH_MAGIC 1
#endif

/* make a seg fault by accessing a null pointer */
#define SEGFAULT (*(int *)NULL)

#if KH_MAGIC
#define HSH_MAGIC               0x01020304
#define HSH_MAGIC_FREED         0x10203040
#define SET_MAGIC               0x02030405
#define SET_MAGIC_FREED         0x20304050
#define LST_MAGIC               0x03040506
#define LST_MAGIC_FREED         0x30405060
#define MEM_STRINGS_MAGIC       0x23232323
#define MEM_STRINGS_MAGIC_FREED 0x32323232
#define MEM_OBJECTS_MAGIC       0x42424242
#define MEM_OBJECTS_MAGIC_FREED 0x24242424
#define TRE_MAGIC               0x012bb0bd
#define TRE_MAGIC_FREED         0x10b20bbd
#define SYM_SCOPE_MAGIC         0x12345678
#define SYM_SCOPE_MAGIC_FREED   0x21436587
#define SYM_SYMBOL_MAGIC        0xfedcba98
#define SYM_SYMBOL_MAGIC_FREED  0xefcdab89
#endif

/* kh.c */

#define KH_FAREY     0xc0000001	/* Farey fraction generation */
#define KH_TRE       0xc0000002	/* Tree libary */
#define KH_SRC       0xc0000004	/* Source library */
#define KH_PP        0xc0000008	/* Prettyprinter library */
#define KH_PPVERBOSE 0xc0000010	/* Prettyprinter library, verbose */
#define KH_LINE      0xc0000020	/* Line number debugging */
#define KH_MEMORY    0xc0000040	/* Print memory usage statistics at exit */
#define KH_TIME      0xc0000080	/* Print timer information at exit */
#define KH_RULES     0xc0000100	/* Rule application */
#define KH_REPLACE   0xc0000200	/* tre_replace */
#define KH_SCOPE     0xc0000400	/* Print scopes in tre_print */
#define KH_ENUM      0xc0000800	/* Use enumerated names in tre_print */
#define KH_TYPE      0xc0001000	/* Print types in tre_print */
#define KH_INFER     0xc0002000	/* Print type inferences */
#define KH_TREES     0xc0004000	/* Print ASTs */

extern void kh_init( void );
extern void kh_shutdown( void );
extern void kh_insert_notice( FILE *str, const char *name, const char *input );

/* xmalloc.c */

#ifndef DMALLOC_FUNC_CHECK
extern __inline__ void *xmalloc( unsigned int size );
extern __inline__ void *xcalloc( unsigned int num, unsigned int size );
extern __inline__ void *xrealloc( void *pt, unsigned int size );
extern __inline__ void xfree( void *pt );
extern __inline__ char *xstrdup( const char *s );
#endif


typedef void *tre_Node;
typedef void *typ_Expr;
typedef int boolean;
#define TRUE 1
#define FALSE 0


/* bit.c */
extern __inline__ void bit_set( unsigned long *flags, int bit );
extern __inline__ void bit_clr( unsigned long *flags, int bit );
extern __inline__ int  bit_tst( unsigned long *flags, int bit );
extern __inline__ int  bit_cnt( unsigned long *flags );

/* prime.c */

extern int           prm_is_prime( unsigned int value );
extern unsigned long prm_next_prime( unsigned int start );

/* hash.c */

typedef void *hsh_HashTable;

typedef struct hsh_Stats {
   unsigned long size;		 /* Size of table */
   unsigned long resizings;	 /* Number of resizings */
   unsigned long entries;	 /* Total entries in table */
   unsigned long buckets_used;	 /* Number of hash buckets in use */
   unsigned long singletons;	 /* Number of length one lists */
   unsigned long maximum_length; /* Maximum list length */

   unsigned long retrievals;	 /* Total number of retrievals */
   unsigned long hits;		 /* Number of retrievals from top of a list */
   unsigned long misses;	 /* Number of unsuccessful retrievals */
} *hsh_Stats;

extern hsh_HashTable hsh_create( unsigned long (*hash)( const void * ),
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
extern unsigned long hsh_string_hash( const void *key );
extern unsigned long hsh_pointer_hash( const void *key );
extern int           hsh_string_compare( const void *key1, const void *key2 );
extern int           hsh_pointer_compare( const void *key1, const void *key2 );
extern void          hsh_key_strings(hsh_HashTable);

/* set.c */

typedef void *set_Set;
typedef void *set_Position;

typedef struct set_Stats{
   unsigned long size;		 /* Size of table */
   unsigned long resizings;	 /* Number of resizings */
   unsigned long entries;	 /* Total entries in table */
   unsigned long buckets_used;	 /* Number of hash buckets in use */
   unsigned long singletons;	 /* Number of length one lists */
   unsigned long maximum_length; /* Maximum list length */

   unsigned long retrievals;	 /* Total number of retrievals */
   unsigned long hits;		 /* Number of retrievals from top of a list */
   unsigned long misses;	 /* Number of unsuccessful retrievals */
} *set_Stats;

typedef unsigned long (*set_HashFunction)( const void * );
typedef int           (*set_CompareFunction)( const void *, const void * );

extern set_Set             set_create( set_HashFunction hash,
				       set_CompareFunction compare );
extern set_HashFunction    set_get_hash( set_Set set );
extern set_CompareFunction set_get_compare( set_Set set );
extern void                set_destroy( set_Set set );
extern int                 set_insert( set_Set set, const void *elem );
extern int                 set_delete( set_Set set, const void *elem );
extern int                 set_member( set_Set set, const void *elem );
extern void                set_iterate( set_Set set,
					int (*iterator)( const void *key ) );
extern set_Set             set_union( set_Set set1, set_Set set2 );
extern set_Set             set_inter( set_Set set1, set_Set set2 );
extern set_Set             set_diff( set_Set set1, set_Set set2 );
extern int                 set_equal( set_Set set1, set_Set set2 );
extern set_Stats           set_get_stats( set_Set set );
extern void                set_print_stats( set_Set set, FILE *stream );
extern int                 set_count( set_Set set );
extern set_Position        set_init_position( set_Set set );
extern set_Position        set_next_position( set_Set set,
					      set_Position position );
extern void                *set_get_position( set_Position position );
extern int                 set_readonly( set_Set set, int flag );

#define SET_POSITION_INIT(P,S) ((P)=set_init_position(S))
#define SET_POSITION_NEXT(P,S) ((P)=set_next_position(S,P))
#define SET_POSITION_OK(P)     (P)
#define SET_POSITION_GET(P,E)  ((E)=set_get_position(P))

/* iterate over all entries E in set S */
#define SET_ITERATE(S,P,E)                                                   \
   for (SET_POSITION_INIT((P),(S));                                          \
	SET_POSITION_OK(P) && (SET_POSITION_GET((P),(E)),1);                 \
	SET_POSITION_NEXT((P),(S)))

/* if the SET_ITERATE loop is exited before all element in the set are
   seen, then SET_ITERATE_END should be called.  Calling this function
   after complete loops does no harm.*/
#define SET_ITERATE_END(S) set_readonly(S,0)

/* stack.c */

typedef void *stk_Stack;

extern stk_Stack stk_create( void );
extern void      stk_destroy( stk_Stack stack );
extern void      stk_push( stk_Stack stack, void *datum );
extern void      *stk_pop( stk_Stack stack );
extern void      *stk_top( stk_Stack stack );

/* list.c */

typedef void *lst_List;
typedef void *lst_Position;

extern lst_List     lst_create( void );
extern void         lst_destroy( lst_List list );
extern void         lst_append( lst_List list, const void *datum );
extern void         lst_push( lst_List list, const void *datum );
extern void         *lst_pop( lst_List list );
extern void         *lst_top( lst_List list );
extern void         *lst_nth_get( lst_List list, unsigned int n );
extern void         lst_nth_set( lst_List list, unsigned int n,
				 const void *datum );
extern int          lst_member( lst_List list, const void *datum );
extern unsigned int lst_length( lst_List list );
extern void         lst_iterate( lst_List list,
				 int (*iterator)( const void *datum ) );
extern void         lst_truncate( lst_List list, unsigned int length );
extern void         lst_truncate_position( lst_List list,
					   lst_Position position );
extern lst_Position lst_init_position( lst_List list );
extern lst_Position lst_last_position( lst_List list );
extern lst_Position lst_next_position( lst_Position position );
extern lst_Position lst_nth_position( lst_List list, unsigned int n );
extern void         *lst_get_position( lst_Position position );
extern void         lst_set_position( lst_Position position,
				      const void *datum );
extern void         _lst_shutdown( void );

#define LST_POSITION_INIT(P,L) ((P)=lst_init_position(L))
#define LST_POSITION_NEXT(P)   ((P)=lst_next_position(P))
#define LST_POSITION_OK(P)     (P)
#define LST_POSITION_GET(P,E)  ((E)=lst_get_position(P))

/* iterate over all entries E in list L */
#define LST_ITERATE(L,P,E)                                                  \
   for (LST_POSITION_INIT((P),(L));                                         \
        LST_POSITION_OK(P) && (LST_POSITION_GET((P),(E)),1);                \
        LST_POSITION_NEXT(P))

/* iterate over all entries in lists L1 and L2 */
#define LST_ITERATE2(L1,L2,P1,P2,E1,E2)                                      \
   for (LST_POSITION_INIT((P1),(L1)), LST_POSITION_INIT((P2),(L2));          \
	LST_POSITION_OK(P1) && LST_POSITION_OK(P2)                           \
	   && (LST_POSITION_GET((P1),(E1)),LST_POSITION_GET((P2),(E2)),1);   \
	LST_POSITION_NEXT(P1), LST_POSITION_NEXT(P2))

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
   int size;			/* Size of each object */
} *mem_ObjectStats;

extern mem_String      mem_create_strings( void );
extern void            mem_destroy_strings( mem_String info );
extern const char      *mem_strcpy( mem_String info, const char *string );
extern const char      *mem_strncpy( mem_String info,
				     const char *string,
				     int len );
extern void            mem_grow( mem_String info,
				 const char *string,
				 int len );
extern const char      *mem_finish( mem_String info );
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

typedef struct str_Stats {
   int count;			/* Number of strings created */
   int bytes;			/* Number of bytes allocated */
   int retrievals;		/* Total number of retrievals */
   int hits;			/* Number of retrievals from top of a list */
   int misses;			/* Number of unsuccessful retrievals */
} *str_Stats;

extern str_Pool   str_pool_create( void );
extern void       str_pool_destroy( str_Pool pool );
extern const char *str_pool_find( str_Pool pool, const char *s );
extern void       str_pool_grow( str_Pool pool, const char *s, int length );
extern const char *str_pool_finish( str_Pool pool );
extern str_Stats  str_pool_get_stats( str_Pool pool );
extern void       str_pool_print_stats( str_Pool pool, FILE *stream );

extern const char *str_find( const char *s );
extern const char *str_findn( const char *s, int length );
extern void       str_grow( const char *s, int length );
extern const char *str_finish( void );
extern const char *str_unique( const char *prefix );
extern void       str_destroy( void );
extern str_Stats  str_get_stats( void );
extern void       str_print_stats( FILE *stream );

/* debug.c */

typedef unsigned long int dbg_Type;

extern void           dbg_register( dbg_Type flag, const char *name );
extern void           _dbg_register( dbg_Type flag, const char *name );
extern void           dbg_destroy( void );
extern void           dbg_set( const char *name );
extern __inline__ int dbg_test( dbg_Type flag );
extern void           dbg_list( FILE *stream );

#define PRINTF(flag,arg) if (dbg_test(flag)) printf arg

/* source.c */

typedef void *src_Type;

typedef struct src_Stats {
   int lines_used;		/* Lines used */
   int lines_allocated;		/* Lines allocated */
   int lines_bytes;		/* Bytes required to store lines */
   int tokens_total;		/* Tokens used */
   int tokens_reused;		/* Tokens reused */
   int tokens_size;		/* Size of a token information object */
} *src_Stats;

extern void       src_create( void );
extern void       src_destroy( void );
extern const char *src_line( const char *line, int len );
extern void       src_new_file( const char *filename );
extern void       src_new_line( int line );
extern void       src_advance( int length );
extern src_Type   src_get( int length );
extern const char *src_filename( src_Type source );
extern int        src_linenumber( src_Type source );
extern int        src_offset( src_Type source );
extern int        src_length( src_Type source );
extern const char *src_source_line( src_Type source );
extern void       src_parse_error( FILE *str, src_Type source,
				   const char *message );
extern src_Stats  src_get_stats( void );
extern void       src_print_stats( FILE *stream );

/* parse.c */

extern void   prs_set_debug( int debug_flag );
extern void   prs_set_cpp_options( const char *cpp_options );
extern void   prs_file( const char *filename );
extern int    prs_make_integer( const char *string, int length );
extern double prs_make_double( const char *string, int length );

/* parse-concrete.c */

extern void   prs_register_concrete( const char *symbol,
				     const char *concrete );
extern const  char *prs_concrete( const char *symbol );
extern void   _prs_shutdown( void );


/* symbol.c */

#define _SYM_HEADER       \
   const char *name;      \
   sym_Scope  scope;      \
   tre_Node   definition; \
   typ_Expr   typeExpr

#if KH_MAGIC
#define SYM_HEADER int magic; _SYM_HEADER
#else
#define SYM_HEADER _SYM_HEADER
#endif

typedef void *sym_Entry;
typedef void *sym_Scope;

/* Delimiter used in mangling names prior to searching the symbol table.
 * Naturally, this character should be outlawed from user-defined identifiers.
 */
#define SYM_DELIM_CHAR ':'

extern void       sym_set_size( int size );
extern sym_Entry  sym_add( sym_Scope scope, const char *name );
extern sym_Entry  sym_new( sym_Scope scope, const char *name );
extern sym_Entry  sym_unique( sym_Scope scope, const char *prefix );
extern sym_Entry  sym_find( sym_Scope scope, const char *name );
extern sym_Entry  sym_find_down( sym_Scope scope, const char *name );
extern sym_Entry  sym_find_local( sym_Scope scope, const char *name );
extern sym_Scope  sym_parent( sym_Scope scope );
extern sym_Scope  sym_top( sym_Scope scope );
extern sym_Scope  sym_push( sym_Scope scope );
extern sym_Scope  sym_pop( sym_Scope scope );
extern int        sym_scope_level( sym_Scope scope );
extern int        sym_symbol_level( sym_Entry symbol );
extern const char *sym_symbol_name( sym_Entry symbol );
extern sym_Scope  sym_scope( sym_Entry symbol );
extern tre_Node   sym_definition( sym_Entry symbol );
extern void       sym_definition_set( sym_Entry symbol, tre_Node tree );
extern typ_Expr   sym_type_expr( sym_Entry symbol );
extern void       sym_type_expr_set( sym_Entry symbol, typ_Expr expr );
extern void       _sym_shutdown( void );
extern void       _sym_scope_check( sym_Scope scope, const char *function );
extern void       _sym_symbol_check( sym_Entry symbol, const char *function );
 
extern boolean    sym_rule_find(tre_Node, tre_Node *);
extern void       sym_rule_install(tre_Node, tre_Node);
extern const char *sym_mangle_name(tre_Node);
extern const char *sym_mangle_function_name(tre_Node);
extern void       sym_dump( FILE *stream, sym_Scope scope );

   
/* tree.c */

#define TRE_TABLE_SIZE 1000	/* Hard limit for efficient access.  Ick. */

				/* These names are provided for debugging
				   purposes only.  You are absolutely
				   forbidden to access any of the fields in
				   TRE_HEADER! */
#ifndef TRE_STRUCT_NAME
#define TRE_STRUCT_NAME Node
#endif

				/* Khepera maintainers: if you add things
                                   to this table, you "must" check
                                   tree.c:tre_replace to make sure it does
                                   the right thing when replacing a tree. */

#define _TRE_HEADER                             \
   enum tre_Id { tre_IdList,                    \
		 tre_IdMax = TRE_TABLE_SIZE }   \
                          id;                   \
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
   src_Type               src;                  \
   const char             *string;              \
   int                    integer;              \
   double                 real;                 \
   unsigned long          bits;                 \
   sym_Scope              scope;                \
   sym_Entry              symbol;               \
   typ_Expr               exprType;             \
   tre_SetTree            monomorphicVars;      \
   tre_Node               typeTree
   

#if KH_MAGIC
#define TRE_HEADER int magic; _TRE_HEADER
#else
#define TRE_HEADER _TRE_HEADER
#endif

typedef void *pp_Format;

typedef struct tre_Stats {
   int created;			/* Number of nodes created */
   int inuse;			/* Number of nodes currently in use */
   int reused;			/* Number of nodes reused */
} *tre_Stats;

typedef struct tre_SetTree {
   set_Set vars;                /* set of elements at this node */
   struct tre_SetTree *parent;  /* parent node in tree */
} *tre_SetTree;

typedef const char *(*tre_Mangler)(tre_Node);


extern void tre_set_size( int size );
extern void tre_set_guard( int guard );
extern void tre_set_id_flag( int id, int flag );
extern void tre_clr_id_flag( int id, int flag );
extern int  tre_tst_id_flag( int id, int flag );
extern void tre_register( int id, const char *name, const char *enumName,
			  void (*create_callback)( tre_Node node ),
			  void (*destroy_callback)( tre_Node node ) );
extern void tre_register_name( int id, const char *enumName, const char *name);
extern int  tre_lookup( const char *enumName );
extern void tre_register_callbacks( int id,
				    void (*create_callback)( tre_Node node ),
				    void (*destroy_callback)( tre_Node node ));
extern void tre_add_create_callback( int id,
				     void (*create_callback)( tre_Node node ));
extern void tre_add_destroy_callback( int id,
				      void (*destroy_callback)(tre_Node node));
extern void tre_register_format( int id, int count, const char *label,
				 const char *format, ... );

extern const char *tre_name( int id );
extern const char *tre_enum_name( int id );
extern const char *tre_node_name( tre_Node node );
extern const char *tre_node_enum_name( tre_Node node );
extern pp_Format  tre_format( int id, int count, const char *label );
extern tre_Node   tre_parent( tre_Node child );
extern tre_Node   tre_child( tre_Node parent );
extern tre_Node   tre_right( tre_Node node );
extern tre_Node   tre_left( tre_Node node );
extern int        tre_id( tre_Node node );
extern src_Type   tre_src( tre_Node node );
extern const char *tre_string( tre_Node node );
extern int        tre_integer( tre_Node node );
extern double     tre_real( tre_Node node );
extern int        tre_child_count( tre_Node node );
extern tre_Node   tre_nth_child( tre_Node node, int n );
extern tre_Node   tre_has_id( tre_Node node, int id );
extern int        tre_parent_id( tre_Node node );
extern tre_Node   tre_parent_has_id( tre_Node node, int id );
extern tre_Node   tre_ancestor_has_id( tre_Node node, int id );
extern tre_Node   tre_child_has_id( tre_Node node, int id );
extern tre_Node   tre_descendent_has_id( tre_Node node, int id );
extern tre_Node   tre_find_ancestor( tre_Node current,
				     int (*f)( tre_Node node ) );
extern tre_Node   tre_find_right( tre_Node current,
				  int (*f)( tre_Node node ) );
extern tre_Node   tre_find_left( tre_Node current, int (*f)( tre_Node node ) );
extern tre_Node   tre_find_descendant( tre_Node current,
				       int (*f)( tre_Node node ) );
extern int        tre_same_id( tre_Node node1, tre_Node node2 );
extern tre_Node   tre_create( int id );
extern tre_Node   tre_append( tre_Node parent, tre_Node child );
extern tre_Node   tre_prepend( tre_Node parent, tre_Node child );
extern void       tre_insert_before( tre_Node sibling, tre_Node child );
extern void       tre_insert_after( tre_Node sibling, tre_Node child );
extern void       tre_destroy( tre_Node node );
extern tre_Node   tre_disconnect( tre_Node node );
extern void       _tre_destroy( tre_Node node );
extern void       tre_delete_children( tre_Node node );
extern void       tre_delete( tre_Node node );
extern tre_Node   tre_replace( tre_Node old, tre_Node new );
extern tre_Node   tre_mk( int id, src_Type src, ... );
extern tre_Node   tre_set_src( tre_Node node, src_Type src );
extern tre_Node   tre_set_flag( tre_Node node, int flag );
extern tre_Node   tre_clr_flag( tre_Node node, int flag );
extern int        tre_tst_flag( tre_Node node, int flag );
extern tre_Node   tre_set_string( tre_Node node, const char *string );
extern tre_Node   tre_set_integer( tre_Node node, int i );
extern tre_Node   tre_set_real( tre_Node node, double r );
extern tre_Node   tre_copy( tre_Node node );
extern int        tre_match( tre_Node node1, tre_Node node2 );
extern void       tre_fast_init( tre_Node root );
extern int        tre_fast_postorder( tre_Node root, int targetID,
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
extern int        tre_apply_rule( tre_Node root,
				  int (*f)( int *count, tre_Node node ) );
extern void       tre_print( tre_Node parent, FILE *stream );
extern void       tre_print_offset(tre_Node parent,int offset,FILE *stream);
extern void       _tre_rb_dump( int id );
extern tre_Stats  tre_get_stats( void );
extern void       tre_print_stats( FILE *stream );
extern void       _tre_shutdown( void );

extern boolean     tre_wild(tre_Node);
extern tre_SetTree tre_monomorphic_vars(tre_Node);
extern void        tre_monomorphic_vars_set(tre_Node,tre_SetTree);
extern typ_Expr    tre_type_expr(tre_Node);
extern void        tre_type_expr_set(tre_Node,typ_Expr);
extern tre_Node    tre_type_tree( tre_Node n );
extern void        tre_type_tree_set( tre_Node n, tre_Node tree );
extern sym_Scope   tre_symtab(tre_Node);
extern void        tre_symtab_set(tre_Node,sym_Scope);
extern sym_Entry   tre_symbol( tre_Node n );
extern void        tre_set_symbol( tre_Node n, sym_Entry s );
extern void        tre_register_name_mangler(int,tre_Mangler);
extern tre_Mangler tre_name_mangler(tre_Node);


/* pp.c */

typedef void *pp_Stream;

extern pp_Stream pp_open( FILE *stream, int width, int lineFlag );
extern void      pp_close( pp_Stream stream );
extern void      pp_begin( pp_Stream stream, int offset, int consistent );
extern void      pp_end( pp_Stream stream );
extern void      pp_break( pp_Stream stream, int offset, int blankSpace );
extern void      pp_string( pp_Stream stream, const char *string,
			    src_Type source );
extern void      pp_print_node( pp_Stream s, tre_Node node,
				const char *label );
extern void      pp_print_tree( FILE *stream, int width, const char *label,
				tre_Node root, int LineFlag );
extern void      pp_print( pp_Stream s, tre_Node node, const char *label,
			   const char *format, ... );
extern pp_Format pp_vmake_format( const char *format, va_list *ap );
extern pp_Format pp_make_format( const char *format, ... );
extern void      pp_free_format( pp_Format format );



/* dct.c */

typedef lst_List                        dct_Dict;
typedef struct { lst_Position a,z; }    dct_Position;
typedef void                           *dct_Element;
typedef void                           *dct_ElementPtr;
typedef void                          (*dct_Destr)(dct_Element);

#define DCT_ARITY 3

#define _X3(X) X,X,X
extern dct_Dict   dct_create( void );
extern boolean    dct_find(dct_Dict, _X3(dct_ElementPtr));
extern void       dct_add(dct_Dict, _X3(dct_Element) );
extern int        dct_length(dct_Dict);
extern dct_Position dct_last_position(dct_Dict);
extern void       dct_truncate(dct_Dict,int, _X3(dct_Destr));
extern void       dct_truncate_position(dct_Dict,dct_Position, _X3(dct_Destr));
extern void       dct_destroy(dct_Dict, _X3(dct_Destr));
extern void       dct_iterate(dct_Dict, int (*)( _X3(dct_Element)));
extern dct_Position dct_next_position(dct_Position);
#undef _X3(X)


#define DCT_POSITION_INIT(P,D) (LST_POSITION_INIT((P).a,(D)),(P).z=NULL)
#define DCT_POSITION_NEXT(P)  ((P)=dct_next_position(P))
#define DCT_POSITION_OK(P)   (LST_POSITION_OK((P).a) || LST_POSITION_OK((P).z))
#define DCT_POSITION_GET(P,X,Y,Z) \
                     ((P).z=(P).a),             LST_POSITION_GET((P).z,(X)), \
                     LST_POSITION_NEXT((P).z),  LST_POSITION_GET((P).z,(Y)), \
                     LST_POSITION_NEXT((P).z),  LST_POSITION_GET((P).z,(Z))

/* iterate over all entries (X,Y,Z) in dictionary D */
#define DCT_ITERATE(D,P,X,Y,Z) \
     for ( DCT_POSITION_INIT((P),(D)); \
           DCT_POSITION_OK(P) && (DCT_POSITION_GET((P),(X),(Y),(Z)),1); \
           DCT_POSITION_NEXT(P) )



/* inf.c */

#define INF_PRETYP	"predef.txt"
extern src_Type   inf_CurrentSourcePosition;
extern void       inf_infer(tre_Node);
extern boolean    inf_infer_definition(tre_Node,tre_Node);


/* lnk.c */

typedef void                     *lnk_Link;
typedef void                     *lnk_LinkPtr;

#define NEW(N,T)          (T *)xmalloc((N)*sizeof(T))
#define LNK_NEWP(PTR)     PTR=(lnk_Link)lnk_new_(sizeof(*PTR))
#define lnk_xfer(P)       lnk_xfer_(&P)
#define lnk_delete(P,D)   lnk_delete_(&(P),D) 

extern lnk_Link    lnk_new_(int);
extern lnk_Link    lnk_copy(lnk_Link);
extern lst_List    lnk_copy_list(lst_List);
extern set_Set     lnk_copy_set(set_Set);
extern lnk_Link    lnk_xfer_(lnk_LinkPtr);
extern lnk_Link    lnk_delete_(  lnk_LinkPtr, void (*)(lnk_Link));
extern lst_List    lnk_delete_list( lst_List, void (*)(lnk_Link));
extern set_Set     lnk_delete_set(   set_Set, void (*)(lnk_Link));
extern void        lnk_decrement(   lnk_Link, void (*)(lnk_Link));



/* typ.c */

enum typ_Tag {
   TYP_VARI, TYP_LINK, TYP_OPER, TYP_CONS, TYP_CSET, TYP_INTC, TYP_PLUS
};


extern typ_Expr      typ_new_vari(void);
extern typ_Expr      typ_new_link(typ_Expr);
extern typ_Expr      typ_new_oper(const char *,lst_List);
extern typ_Expr      typ_new_cons(typ_Expr,typ_Expr);
extern typ_Expr      typ_new_cset(set_Set);
extern typ_Expr      typ_new_intc(int);
extern typ_Expr      typ_new_plus(typ_Expr,typ_Expr);
extern typ_Expr      typ_new_base_type(const char *, int);
extern typ_Expr      typ_instance(typ_Expr, tre_SetTree, dct_Dict);
extern boolean       typ_mgu(typ_Expr, typ_Expr, lst_List, set_Set);
extern void          typ_apply_mgu(dct_Dict);
extern boolean       typ_unify(typ_Expr, typ_Expr);
extern boolean       typ_unify_ast_types(tre_Node, tre_Node);
extern typ_Expr      typ_clash1(void);
extern typ_Expr      typ_clash2(void);
extern void          typ_read_typerule(tre_Node);
extern tre_Node      typ_ast_instance(tre_Node);
extern void          typ_ast_dump(tre_Node);
extern void          typ_unlink(void *);
extern void          typ_destroy(typ_Expr);
extern int           typ_tre_unlink(tre_Node);
extern unsigned long typ_hash(const typ_Expr);
extern int           typ_compare(const typ_Expr, const typ_Expr);
extern void          typ_dump(typ_Expr);
extern void          typ_print(typ_Expr);
extern lst_List      typ_constraints_get(typ_Expr);
extern void          typ_constraints_set(typ_Expr, lst_List);
extern void          typ_constraint_add(typ_Expr);
extern src_Type      typ_trace_info_get(typ_Expr);
extern void          typ_trace_info_set(typ_Expr, src_Type);
extern typ_Expr      typ_dup_expr_tree(typ_Expr);
extern typ_Expr      typ_dup_expr_node(typ_Expr te, boolean dup_constraints,
				       boolean dup_u);
extern tre_Node      typ_dup_ast(tre_Node call);


/* timer.c */

extern void tim_start( const char *name );
extern void tim_stop( const char *name );
extern void tim_reset( const char *name );
extern double tim_get_real( const char *name );
extern double tim_get_user( const char *name );
extern double tim_get_system( const char *name );
extern void tim_print_timer( FILE *str, const char *name );
extern void tim_print_timers( FILE *str );
extern void _tim_shutdown( void );

/* xfm.c */

typedef int (*xfm_Rule)( int *_kh_flag, tre_Node _kh_node );

typedef struct xfm_SetEntry {
   xfm_Rule rule;
   int      match;		/* Go here on a match */
   int      otherwise;		/* Go here otherwise, -1 terminates */
} xfm_SetEntry;

typedef xfm_SetEntry *xfm_Set;

typedef struct xfm_SetListEntry {
   xfm_Set set;
   int matchHere;
   int matchPrevious;
   int otherwise;
} xfm_SetListEntry;

typedef xfm_SetListEntry *xfm_SetList;

typedef struct xfm_RuleListEntry {
   xfm_Rule rule;
   int      is_fast_postorder;
   int      matchHere;
   int      matchPrevious;
   int      otherwise;
} xfm_RuleListEntry;

typedef xfm_RuleListEntry *xfm_RuleList;

#endif
