/* memtest.c -- 
 * Created: Sat Jul  1 22:42:09 1995 by faith@dict.org
 * Revised: Sat Mar 30 12:06:35 2002 by faith@dict.org
 * Copyright 1995, 2002 Rickard E. Faith (faith@dict.org)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
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
 * $Id: memtest.c,v 1.5 2006/04/08 21:22:09 cheusov Exp $
 * 
 */

#include <stdio.h>
#include <stdlib.h>

int main( void )
{
   char *buf;
   int  i;

#ifdef KH_GNU_MALLOC
   mcheck(0);
#endif

   buf = malloc( 6 );

   for (i = 0; i < 7; i++) {
      printf( "i = %d\n", i );
      buf[i] = i;
   }

   for (i = -1; i < 7; i++) printf( "buf[%d] = %d\n", i, buf[i] );

   free( buf );

   return 0;
}
