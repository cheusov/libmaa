/* primetest.c -- 
 * Created: Thu Jul 20 22:15:54 1995 by r.faith@ieee.org
 * Revised: Thu Aug 24 11:38:32 1995 by faith@cs.unc.edu
 * Copyright 1995 Rickard E. Faith (r.faith@ieee.org)
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
 * $Id: primetest.c,v 1.2 1995/08/24 15:41:34 faith Exp $
 * 
 */

#include "kh.h"

int main( int argc, char **argv )
{
   if (argc != 2) {
      fprintf( stderr, "usage: primetest value\n" );
      return 1;
   }
   printf( "%lu\n", prm_next_prime( atol( argv[1] ) ) );
   return 0;
}
