/*
 *  $Id: ccl_parse.c,v 1.1.1.1 2004-04-10 13:35:03 sbooth Exp $
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
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "libconfig.h"

/* Return 0 on failure */
static struct config_pair_t*
make_pair(const char *key,
	  const char *value)
{
  struct config_pair_t *result;

  if(key == 0 || value == 0)
    return 0;

  result = (struct config_pair_t*) malloc(sizeof(struct config_pair_t));
  if(result == 0)
    return 0;

  result->key = 0;
  result->value = 0;
  result->next = 0;

  result->key = strdup(key);
  result->value = strdup(value);

  if(result->key == 0 || result->value == 0) {
    free(result->key);
    free(result->value);
    free(result);
    return 0;
  }

  return result;
}

static void
my_getstring_sep(const char *src, 
		 char *dst,
		 char sep_char)
{
  /* Skip leading ws */
  while(isspace(*src))
    ++src;
  
  /* Extract quoted string */
  if(*src == '\"') {
    ++src;
    while(*src != '\"')
      *dst++ = *src++;
  }
  /* Extract non-ws */
  else {
    while(!isspace(*src) && *src != sep_char && *src != '\0')
      *dst++ = *src++;
  }
  
  *dst = '\0';
}

static void
my_getstring(const char *src, 
	     char *dst)
{
  /* Skip leading ws */
  while(isspace(*src))
    ++src;
  
  /* Extract quoted string */
  if(*src == '\"') {
    ++src;
    while(*src != '\"')
      *dst++ = *src++;
  }
  /* Extract non-ws */
  else {
    while(!isspace(*src) && *src != '\0')
      *dst++ = *src++;
  }
  
  *dst = '\0';
}

int
config_parse(struct config_t *data, 
	     const char *path)
{
  FILE *f;
  char *buf, *sep, *pos, *limit;
  char key [CONFIG_MAXLEN], value [CONFIG_MAXLEN];
  int result;
  size_t line;
  struct config_pair_t *pair, *current;

  if(data == 0 || path == 0)
    return -1;
  
  /* Setup */
  data->head = 0;
  data->iter = 0;
  data->iter_prev = 0;
  result = 0;
  line = 1;

  f = fopen(path, "r");
  if(f == 0) {
    fprintf(stderr, PACKAGE" error: file '%s' not found\n", path);
    return -1;
  }

  buf = (char*) malloc(sizeof(char) * CONFIG_MAXLEN);
  if(buf == 0) {
    result = ENOMEM;
    goto cleanup;
  }

  /* Parse file */
  for(;;) {
    /* Read single line */
    fgets(buf, CONFIG_MAXLEN, f);
    if(feof(f))
      break;

    /* Strip comments */
    pos = buf;
    limit = buf + strlen(buf);
    while(pos < limit) {
      if(*pos == data->comment_char) {
	*pos = '\0';
	break;
      }
      ++pos;
    }

    /* Process input */
    key[0] = value[0] = '\0';
    my_getstring_sep(buf, key, data->sep_char);
    my_getstring(buf + strlen(key), value);
    
    if(strlen(key) == 0) {
      fprintf(stderr, PACKAGE" error(%s:%i): zero key length\n", 
	      path, line);
    }
    else if(config_get(data, key) != 0) {
      fprintf(stderr, PACKAGE" error(%s:%i): duplicate key '%s'\n", 
	      path, line, key);	
    }
    else {
      /* Create the new key/value pair */
      pair = make_pair(key, value);
      if(pair == 0) {
	result = ENOMEM;
	goto cleanup;
      }
      /* Add it to the linked list */
      if(data->head == 0) {
	data->head = pair;
	current = pair;
      }
      else {
	current->next = pair;
	current = pair;
      }
    }
    ++line;
  }

 cleanup:
  free(buf);
  fclose(f);

  return result;
}
