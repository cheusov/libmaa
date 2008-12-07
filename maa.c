/* maa.c -- General Support for libmaa
 * Created: Sun Nov 19 13:24:35 1995 by faith@dict.org
 * Revised: Sat Mar 30 12:07:16 2002 by faith@dict.org
 * Copyright 1995, 1996, 2002 Rickard E. Faith (faith@dict.org)
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
 * $Id: maa.c,v 1.18 2008/12/07 13:42:06 cheusov Exp $
 *
 * \section{General Support}
 *
 */

#include "maaP.h"
#include "maa.h"

/* \doc |maa_init| should be called at the start of "main()", and serves to
   initialize debugging and other support for \libmaa. */

void maa_init( const char *programName )
{
   tim_start( "total" );
   
   err_set_program_name( programName );
   
   _dbg_register( MAA_MEMORY,    ".memory" );
   _dbg_register( MAA_TIME,      ".time" );
   _dbg_register( MAA_PR,        ".pr" );
   _dbg_register( MAA_SL,        ".sl" );
   _dbg_register( MAA_SRC,       ".src" );
   _dbg_register( MAA_PARSE,     ".parse" );
}

void maa_shutdown( void )
{
   if (dbg_test(MAA_MEMORY) || dbg_test(MAA_TIME))
      fprintf( stderr, "%s\n", maa_version() );
   if (dbg_test(MAA_MEMORY)) {
      str_print_stats( stderr );
   }

   _pr_shutdown();
   str_destroy();
   _lst_shutdown();
   _sl_shutdown();

   tim_stop( "total" );
   if (dbg_test(MAA_TIME)) {
      tim_print_timers( stderr );
   }
   _tim_shutdown();
   flg_destroy();
   dbg_destroy();
}

int maa_version_major( void )
{
   return MAA_MAJOR;
}

int maa_version_minor( void )
{
   return MAA_MINOR;
}

int maa_version_teeny( void )
{
   return MAA_MINOR;
}

const char *maa_version( void )
{
   static char buffer[80];

   sprintf( buffer, "Libmaa %d.%d.%d", MAA_MAJOR, MAA_MINOR, MAA_TEENY );
      
   return buffer;
}
