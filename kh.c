/* kh.c -- General Support for Khepera
 * Created: Fri Jan 20 14:56:18 1995 by faith@cs.unc.edu
 * Revised: Sat Jan 28 20:12:11 1995 by faith@cs.unc.edu
 * Copyright 1995 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: kh.c,v 1.1 1995/04/21 15:31:47 faith Exp $
 *
 * \section{General Support}
 *
 *
 */

#include "kh.h"

void kh_init( void )
{
   _dbg_register( KH_FAREY, "farey" );
   _dbg_register( KH_TRE,   "tre" );
}
