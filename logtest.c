/* sltest.c -- 
 * Created: Fri, 27 Feb 2009 23:21:31 +0200 by vle@gmx.net
 * Copyright 2009 Aleksey Cheusov (vle@gmx.net)
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
 */

#include "maaP.h"

int main( int argc, char **argv )
{
   const char *logfn;

   --argc, ++argv;

   if (!argc)
      return 1;

   logfn = argv [0];

   maa_init (argv [0]);

   log_file ("logtest", logfn);

   log_error ("main", "Error??? No-o-o-o... :-)");

   maa_shutdown ();

   return 0;
}
