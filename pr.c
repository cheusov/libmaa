/* pr.c -- Process creation and tracking support
 * Created: Sun Jan  7 13:34:08 1996 by r.faith@ieee.org
 * Revised: Sun Jan  7 14:51:17 1996 by r.faith@ieee.org
 * Copyright 1996 Rickard E. Faith (r.faith@ieee.org)
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
 * $Id: pr.c,v 1.1 1996/01/08 03:26:51 faith Exp $
 *
 * \section{Process Management Routines}
 *
 * \intro The process management routines are designed to facilitate the
 * creation and management of low-level pipelines.
 *
 */

#include "maaP.h"

