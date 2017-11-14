/* maa.c -- General Support for libmaa
 * Created: Sun Nov 19 13:24:35 1995 by faith@dict.org
 * Copyright 1995, 1996, 2002 Rickard E. Faith (faith@dict.org)
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
   return MAA_TEENY;
}

const char *maa_version( void )
{
   static char buffer[80];

   sprintf( buffer, "Libmaa %d.%d.%d", MAA_MAJOR, MAA_MINOR, MAA_TEENY );
      
   return buffer;
}
