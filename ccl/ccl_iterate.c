/*
 *  $Id: ccl_iterate.c,v 1.4 2004-04-15 17:48:34 sbooth Exp $
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

#include "ccl/ccl.h"

const struct ccl_pair_t*
ccl_iterate(struct ccl_t *data)
{
  struct ccl_pair_t *pair;

  if(data == 0)
    return 0;

  if(data->iterating) {
    pair = bst_t_next(&data->traverser);
  }
  else {
    data->iterating = 1;
    pair = bst_t_first(&data->traverser, data->table);
  }

  return pair;
}
