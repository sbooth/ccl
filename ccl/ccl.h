/*
 *  $Id: ccl.h,v 1.2 2004-04-14 06:58:34 sbooth Exp $
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

#ifndef _LIBCONFIG_H_
#define _LIBCONFIG_H_

#include "config.h"

/** A key-value linked-list entry */
struct config_pair_t
{
  /** The key */
  char *key;
  /** The value */
  char *value;

  /** The next entry in the linked list */
  struct config_pair_t *next;
};

/** The main data structure */
struct config_t
{
  /** The character that indicates the start of a comment */
  char comment_char;
  /** The character that separates keys from values */
  char sep_char;

  /** The head of the linked list */
  struct config_pair_t *head;

  /* Iterator functionality */
  struct config_pair_t *iter;
  struct config_pair_t *iter_prev;
};
typedef struct config_t config_t;

/**
 * Parse a configuration file.
 * @param data The \c config_t in which to store the parsed data
 * @param path The file to parse
 * @return 0 if successful
 */
int
config_parse(struct config_t *data, 
	     const char *path);

void 
config_release(struct config_t *data);


const char* 
config_get(const struct config_t *data, 
	   const char *key);

const struct config_pair_t* 
config_iterate(struct config_t *data);

void 
config_iterate_reset(struct config_t *data);


#endif /* ! _LIBCONFIG_H_ */


