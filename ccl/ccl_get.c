/*
 *  $Id: ccl_get.c,v 1.2 2004-04-14 15:31:57 sbooth Exp $
 *
 *  Copyright (C) 2004 Stephen F. Booth
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <string.h>

#include "ccl.h"

const char* 
ccl_get(const struct ccl_t *data, 
	const char *key)
{
  const struct ccl_pair_t *current;

  if(data == 0 || key == 0)
    return 0;
  
  current = data->head;
  while(current != 0) {
    if(strcmp(current->key, key) == 0)
      return current->value;
    current = current->next;
  }

  return 0;
}
