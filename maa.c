/* maa.c -- General Support for libmaa
 * Created: Sun Nov 19 13:24:35 1995 by faith@cs.unc.edu
 * Revised: Sun Jan 28 15:44:11 1996 by faith@cs.unc.edu
 * Copyright 1995, 1996 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: maa.c,v 1.6 1996/01/28 22:33:31 faith Exp $
 *
 * \section{General Support}
 *
 */

#include "maaP.h"
#include "maa.h"

/* \doc |maa_init| should be called at the start of "main()", and serves to
   initialize debugging and other support for \maa. */

void maa_init( const char *programName )
{
   tim_start( "total" );
   
   err_set_program_name( programName );
   
   _dbg_register( KH_MEMORY,    ".memory" );
   _dbg_register( KH_TIME,      ".time" );
   _dbg_register( MAA_PR,       ".pr" );

#ifndef __CHECKER__
#ifdef HAVE_ATEXIT
      atexit( maa_shutdown );
#else
# ifdef HAVE_ON_EXIT
      on_exit( maa_shutdown, NULL );
# endif
#endif
#endif
}

void maa_shutdown( void )
{
   if (dbg_test(KH_MEMORY) || dbg_test(KH_TIME))
      fprintf( stderr, "%s\n", maa_version() );
   if (dbg_test(KH_MEMORY)) {
      str_print_stats( stderr );
   }

   _pr_shutdown();
   str_destroy();
   _lst_shutdown();

   tim_stop( "total" );
   if (dbg_test(KH_TIME)) {
      tim_print_timers( stderr );
   }
   _tim_shutdown();
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

const char *maa_version( void )
{
   static char buffer[80];
   char        *pt;
   char        *colon;
   char        *dot;
   
   sprintf( buffer, "libmaa, version %d.%d.", MAA_MAJOR, MAA_MINOR );
   if ((colon = strchr( maa_revision_string, ':') )) {
				/* Assume an RCS Revision string */
      if ((dot = strchr( colon, '.' )))
	 strcat( buffer, dot + 1 );
      else
	 strcat( buffer, colon + 2 );
      pt = strrchr( buffer, '$') - 1;
      if (!pt)                  /* Stripped RCS Revision string? */
            pt = buffer + strlen( buffer ) - 1;
      if (*pt != ' ')
            ++pt;
      *pt = '\0';
   } else {                     /* Assume a simple number */
      if (maa_revision_string[0] == '$')
	 strcat( buffer, "0" );
      else
	 strcat( buffer, maa_revision_string );
   }
      
   return buffer;

}
