/*
 *  $Id: ccl_release.c,v 1.5 2004-04-16 23:58:08 sbooth Exp $
 *
 *  Copyright (C) 2004 Stephen F. Booth
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>	/* free */

#include "ccl/ccl.h"

static void
ccl_bst_item_func(void *bst_item,
		  void *bst_param)
{
  struct ccl_pair_t *pair = (struct ccl_pair_t*) bst_item;
  free(pair->key);
  free(pair->value);
  free(pair);
}

void
ccl_release(struct ccl_t *data)
{
  if(data == 0)
    return;

  bst_destroy(data->table, ccl_bst_item_func);
}
