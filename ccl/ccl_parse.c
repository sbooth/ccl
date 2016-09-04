/*
 *  Copyright (C) 2004 Stephen F. Booth <me@sbooth.org>
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

#include <stdlib.h> 	/* malloc, free */
#include <stdio.h>	/* fopen, fread, fclose */
#include <string.h>	/* strcmp, strdup */
#include <ctype.h>	/* isspace */
#include <errno.h>	/* ENOMEM, EINVAL, ENOENT */

#include "ccl/ccl.h"

#define CCL_BUFSIZE 1024
#define CCL_TOKSIZE 32

enum {
  CCL_PARSE_INITIAL,
  CCL_PARSE_COMMENT,
  CCL_PARSE_QUOTED,
  CCL_PARSE_UNQUOTED,
  CCL_HANDLE_NEWLINE,
  CCL_HANDLE_SEP
};

static int
ccl_bst_comparison_func(const void *bst_a,
			const void *bst_b,
			void *bst_param)
{
  const struct ccl_pair_t *a = (const struct ccl_pair_t*) bst_a;
  const struct ccl_pair_t *b = (const struct ccl_pair_t*) bst_b;

  return strcmp(a->key, b->key);
}

int
ccl_parse(struct ccl_t *data, 
	  const char *path)
{
  FILE 			*f;
  char 			*buf, *p;
  char 			*token, *t, *tok_limit;
  int 			result, state, got_tok, tok_cap;
  size_t 		count, line;
  struct ccl_pair_t 	*pair;


  /* Validate arguments */
  if(data == 0 || path == 0)
    return EINVAL;
  
  /* Setup local variables */
  result = 0;
  pair = 0;
  line = 1;

  /* Setup data struct */
  data->table = bst_create(ccl_bst_comparison_func, 0, 0);
  if(data->table == 0) {
    return ENOMEM;    
    goto cleanup;
  }
  bst_t_init(&data->traverser, data->table);
  data->iterating = 0;

  /* Open file */
  f = fopen(path, "r");
  if(f == 0) {
    fprintf(stderr, PACKAGE": Unable to open '%s'\n", path);
    return ENOENT;
  }

  /* Initialize file and token buffers */
  buf = (char*) malloc(sizeof(char) * CCL_BUFSIZE);
  token = (char*) malloc(sizeof(char) * CCL_TOKSIZE);
  if(buf == 0 || token == 0) {
    result = ENOMEM;
    goto cleanup;
  }

  /* Parse file */
  state = CCL_PARSE_INITIAL;
  got_tok = 0;
  tok_cap = CCL_TOKSIZE;
  tok_limit = token + tok_cap;
  
  do {

    /* Read a chunk */
    count = fread(buf, sizeof(char), CCL_BUFSIZE, f);

    /* Parse the input - manually increment p since not all
       transitions should automatically consume a character */
    for(p = buf; p < buf + count; /* ++p */ ) {

      switch(state) {

	/* ==================== Initial parsing state */
      case CCL_PARSE_INITIAL:
	if(*p == data->comment_char) {
	  state = CCL_PARSE_COMMENT;
	  ++p;
	}
	else if(*p == data->str_char) {
	  t = token;
	  state = CCL_PARSE_QUOTED;
	  ++p;
	}
	else if(*p == '\n') {
	  state = CCL_HANDLE_NEWLINE;
	}
	else if(*p == data->sep_char) {
	  state = CCL_HANDLE_SEP;
	  ++p;
	}
	else if(isspace(*p)) {
	  ++p;
	}
	else {
	  t = token;

	  /* Enlarge buffer, if needed */
	  if(t > tok_limit) {
	    int count = t - token;
	    token = (char*) realloc(token, tok_cap * 2);
	    if(token == 0) {
	      result = ENOMEM;
	      goto cleanup;
	    }
	    tok_cap *= 2;
	    tok_limit = token + tok_cap;
	    t = token + count;
	  }

	  *t++ = *p++;

	  state = CCL_PARSE_UNQUOTED;
	}
	break;

	/* ==================== Parse comments */
      case CCL_PARSE_COMMENT:
	if(*p == '\n') {
	  state = CCL_HANDLE_NEWLINE;
	}
	else {
	  ++p;
	}
	break;

	/* ==================== Parse quoted strings */
      case CCL_PARSE_QUOTED:
	if(*p == data->str_char) {
	  got_tok = 1;
	  *t = '\0';
	  state = CCL_PARSE_INITIAL;
	  ++p;
	}
	else if(*p == '\n') {
	  fprintf(stderr, PACKAGE": Unterminated string (%s:%zi)\n",
		  path, line);
	  
	  state = CCL_HANDLE_NEWLINE;
	}
	else {
	  /* Enlarge buffer, if needed */
	  if(t > tok_limit) {
	    int count = t - token;
	    token = (char*) realloc(token, tok_cap * 2);
	    if(token == 0) {
	      result = ENOMEM;
	      goto cleanup;
	    }
	    tok_cap *= 2;
	    tok_limit = token + tok_cap;
	    t = token + count;
	  }

	  *t++ = *p++;
	}
	break;
	
	/* ==================== Parse unquoted strings */
      case CCL_PARSE_UNQUOTED:
	if(*p == data->comment_char) {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }
	  state = CCL_PARSE_COMMENT;
	  ++p;
	}
	else if(*p == '\n') {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }
	  state = CCL_HANDLE_NEWLINE;
	}
	else if(*p == data->sep_char) {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }

	  state = CCL_HANDLE_SEP;
	  ++p;
	}
	/* In this mode a space ends the current token */
	else if(isspace(*p)) {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }
	  
	  state = CCL_PARSE_INITIAL;
	  ++p;
	}
	else {
	  /* Enlarge buffer, if needed */
	  if(t > tok_limit) {
	    int count = t - token;
	    token = (char*) realloc(token, tok_cap * 2);
	    if(token == 0) {
	      result = ENOMEM;
	      goto cleanup;
	    }
	    tok_cap *= 2;
	    tok_limit = token + tok_cap;
	    t = token + count;
	  }

	  *t++ = *p++;
	}
	break;

	/* ==================== Process separator characters */
      case CCL_HANDLE_SEP:
	if(got_tok == 0) {
	  pair = 0;
	  fprintf(stderr, PACKAGE": Missing key (%s:%zi)\n", path, line);
	}
	else if(ccl_get(data, token) != 0) {
	  pair = 0;
	  fprintf(stderr, PACKAGE": Ignoring duplicate key '%s' (%s:%zi)\n", 
		  token, path, line);
	}
	else {
	  pair = (struct ccl_pair_t*) malloc(sizeof(struct ccl_pair_t));
	  if(pair == 0) {
	    result = ENOMEM;
	    goto cleanup;
	  }

	  pair->key = strdup(token);
	  pair->value = 0;
	  if(pair->key == 0) {
	    result = ENOMEM;
	    goto cleanup;
	  }
	}

	got_tok = 0;
	state = CCL_PARSE_INITIAL;
	break;

	/* ==================== Process newlines */
      case CCL_HANDLE_NEWLINE:
	if(pair != 0) {
	  /* Some type of token was parsed */
	  if(got_tok == 1) {
	    pair->value = strdup(token);
	  }
	  else {
	    /* In this case we read a key but no value */
	    pair->value = strdup("");
	  }
	  
	  if(pair->value == 0) {
	    result = ENOMEM;
	    goto cleanup;
	  }

	  if(bst_probe(data->table, pair) == 0) {
	    result = ENOMEM;
	    goto cleanup;
	  }
	}

	got_tok = 0;
	pair = 0;
	state = CCL_PARSE_INITIAL;
	++line;
	++p;
	break;
      } 
    }    
  } while(feof(f) == 0);

 cleanup:
  if(pair != 0) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
  free(buf);
  free(token);
  fclose(f);

  return result;
}
