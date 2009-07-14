/*
 *  Copyright (C) 2004 - 2005 Stephen F. Booth <me@sbooth.org>
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

#ifndef CCL_H
#define CCL_H

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "ccl/bst.h"

#ifdef __cplusplus
  extern "C" {
#endif

/*! \file ccl.h
 * \brief The ccl library interface
 */

/*! \brief A key/value pair found in a configuration file */
struct ccl_pair_t
{
  /*! \brief The key */
  char *key;
  /*! \brief The value */
  char *value;
};

/*!
 * \brief Data structure encapsulating a parsed configuration file 
 *
 * In the file:
 *  - comments begin with \c comment_char and continue to a newline
 *  - Keys are separated from values by \c sep_char
 *  - String literals enclosed in \c str_char may contain any character,
 * including spaces.
 */
struct ccl_t
{
  /*! \brief Character that indicates the start of a comment */
  char comment_char;
  /*! \brief Character that separates keys from values */
  char sep_char;
  /** \brief Character that delineates string literals */
  char str_char;


  /* ========== Implementation details below, subject to change */

  /* The parsed file data, stored as a binary tree*/
  struct bst_table *table;

  /* The table traverser */
  struct bst_traverser traverser;

  /* Currently traversing? */
  int iterating;
};
typedef struct ccl_t ccl_t;

/*!
 * \brief Parse a configuration file
 *
 * This function will attempt to parse the configuration file
 * \c path, using the comment, separator, and quote characters
 * specified in \c data.
 * This function allocates memory; use ccl_release to clean up.
 * \param data The ccl_t in which to store the parsed data
 * \param path The file to parse
 * \return 0 if successful, nonzero otherwise
 */
int
ccl_parse(struct ccl_t *data, 
	  const char *path);

/*!
 * \brief Release memory associated with a configuration file
 *
 * This function frees any dynamically-allocated memory in \c data.
 * \param data The ccl_t that is no longer needed
 */
void 
ccl_release(struct ccl_t *data);

/*!
 * \brief Extract a value from a configuration file
 *
 * This function searches the parsed configuration file data for \c key,
 * and returns the value associated with that key.<br>
 * If \c key was found in the configuration file, the returned value 
 * will never be 0.<br>
 * The value returned belongs to ccl, and should not be free'd.
 * \param data The ccl_t to query
 * \param key The key to query
 * \return The value associated with \c key, or 0 if \c key was not found
 */
const char* 
ccl_get(const struct ccl_t *data, 
	const char *key);

/*!
 * \brief Iterate through all key/value pairs in a configuration file
 *
 * This function allows iteration through all key/value pairs in a
 * configuration file.<br>
 * This function maintains internal state; to reset the iterator call
 * ccl_reset.<br>
 * The value returned belongs to ccl, and should not be free'd.<br>
 * \param data The ccl_t to query
 * \return A key/value pair, or 0 if no more exist in \c data
 */
const struct ccl_pair_t* 
ccl_iterate(struct ccl_t *data);

/*!
 * \brief Reset a configuration file iterator
 *
 * This function resets the internal iterator in \c data to the first
 * key/value pair.
 * \param data The ccl_t to reset
 */
void 
ccl_reset(struct ccl_t *data);

#ifdef __cplusplus
  }
#endif

#endif /* ! CCL_H */


