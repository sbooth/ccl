/*
 *  $Id: ccl_release.c,v 1.3 2004-04-14 21:21:36 sbooth Exp $
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

#include <stdlib.h>

#include "ccl.h"

static void
ccl_bst_item_func(void *bst_item,
		  void *bst_param)
{
  struct ccl_pair_t *pair = (struct ccl_pair_t*) bst_item;
  free(pair->key);
  free(pair->value);
}

void
ccl_release(struct ccl_t *data)
{
  if(data == 0)
    return;

  bst_destroy(data->table, ccl_bst_item_func);
}
