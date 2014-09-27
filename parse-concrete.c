/* parse-concrete.c -- Support for calling parsers from Libmaa, continued
 * Created: Mon Apr 24 17:40:51 1995 by faith@dict.org
 * Copyright 1995, 1997, 2002 Rickard E. Faith (faith@dict.org)
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
 */

#include "maaP.h"

static hsh_HashTable _prs_hash;

void prs_register_concrete( const char *symbol, const char *concrete )
{
   if (!_prs_hash) _prs_hash = hsh_create( NULL, NULL );

   hsh_insert( _prs_hash, str_find( symbol ), str_find( concrete ) );
}

const char *prs_concrete( const char *symbol )
{
   if (!_prs_hash) return NULL;
   return hsh_retrieve( _prs_hash, symbol );
}

void _prs_shutdown( void )
{
   if (_prs_hash) hsh_destroy( _prs_hash );
}
