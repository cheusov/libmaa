/* stack.c -- Stack routines for Khepera
 * Created: Wed Nov  9 19:40:00 1994 by faith@dict.org
 * Copyright 1994, 1995, 2002 Rickard E. Faith (faith@dict.org)
 * Copyright 2002-2008 Aleksey Cheusov (vle@gmx.net)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * \section{Stack Routines}
 *
 * \intro The stack routines provide support for a general stack of
 * pointers to "void".  Because of the simplicity of the stack object, no
 * statistics are maintained.  (Althought the list routines can also be used
 * as a stack, the stack implemented here is more efficient.)
 *
 */

#include "maaP.h"

typedef struct data {
   const void  *datum;
   struct data *prev;
} *dataType;

typedef struct stack {
   struct data    *data;
} *stackType;

/* \doc |stk_create| initializes a stack object. */

stk_Stack stk_create( void )
{
   stackType s;

   s          = xmalloc( sizeof( struct stack ) );
   s->data    = NULL;

   return s;
}

/* \doc |stk_destroy| destroys all memory associated with the |stack|.  The
   memory used by data is \emph{not} freed---this memory is the
   responsibility of the user. */

void stk_destroy( stk_Stack stack )
{
   while (!stk_isempty( stack )){
	   stk_pop( stack );
   }
   xfree( stack );		/* terminal */
}

/* \doc |stk_push| places |datum| on the top of the |stack|. */

void stk_push( stk_Stack stack, void *datum )
{
   stackType s = (stackType)stack;
   dataType  d = (dataType)malloc( sizeof( struct data ) );

   d->datum = datum;
   d->prev  = s->data;
   s->data  = d;
}

/* \doc |stk_pop| removes the top of the |stack| and returns the pointer.
   If the |stack| is empty, |stk_pop| returns "NULL". */

void *stk_pop( stk_Stack stack )
{
   stackType  s     = (stackType)stack;
   void      *datum = NULL;

   if (s->data) {
      dataType old = s->data;

      datum = __UNCONST(old->datum); /* Discard const */
      s->data = s->data->prev;

      free( old );
   }

   return datum;
}

/* \doc |stk_isempty| return 1 if |stack| is empty, or 0 otherwise.
*/

int stk_isempty( stk_Stack stack )
{
   stackType  s     = (stackType)stack;

   if (s->data) {
	  return 0;
   }else{
	  return 1;
   }
}

/* \doc |stk_top| returns a pointer to the datum on the top of the |stack|,
   but does \emph{not} remove this datum from the |stack|.  If the |stack|
   is empty, |stk_pop| returns "NULL". */

void *stk_top( stk_Stack stack )
{
   stackType s = (stackType)stack;

   if (s->data)
      return __UNCONST(s->data->datum);	/* Discard const */

   return NULL;
}
