/* bit.c -- 
 * Created: Thu Sep 28 18:09:30 1995 by faith@cs.unc.edu
 * Revised: Thu Sep 28 18:58:10 1995 by faith@cs.unc.edu
 * Copyright 1995 Rickard E. Faith (faith@cs.unc.edu)
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
 * $Id: bit.c,v 1.1 1995/09/29 00:55:40 faith Exp $
 * 
 */

#include "kh.h"

__inline__ void bit_set( unsigned long flags, int bit )
{
   flags |= (1 << bit);
}

__inline__ void bit_clr( unsigned long flags, int bit )
{
   flags &= ~(1 << bit);
}

__inline__ int bit_tst( unsigned long flags, int bit )
{
   return (flags & (1 << bit));
}
