/*
 *  $Id: ccl.h,v 1.3 2004-04-14 15:31:57 sbooth Exp $
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

#ifndef _CCL_H_
#define _CCL_H_

#include "config.h"

/** A key-value linked-list entry */
struct ccl_pair_t
{
  /** The key */
  char *key;
  /** The value */
  char *value;

  /** The next entry in the linked list */
  struct ccl_pair_t *next;
};

/** The main data structure */
struct ccl_t
{
  /** The character that indicates the start of a comment */
  char comment_char;
  /** The character that separates keys from values */
  char sep_char;
  /** The character that delineates string literals */
  char str_char;

  /** The head of the linked list */
  struct ccl_pair_t *head;

  /* Iterator functionality */
  struct ccl_pair_t *iter;
  struct ccl_pair_t *iter_prev;
};
typedef struct ccl_t ccl_t;

/**
 * Parse a configuration file.
 * @param data The \c ccl_t in which to store the parsed data
 * @param path The file to parse
 * @return 0 if successful
 */
int
ccl_parse(struct ccl_t *data, 
	  const char *path);

void 
ccl_release(struct ccl_t *data);


const char* 
ccl_get(const struct ccl_t *data, 
	const char *key);

const struct ccl_pair_t* 
ccl_iterate(struct ccl_t *data);

void 
ccl_reset(struct ccl_t *data);


#endif /* ! _CCL_H_ */


