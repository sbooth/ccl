/*
 *  $Id: ccl_parse.c,v 1.2 2004-04-14 06:58:34 sbooth Exp $
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

#define CONFIG_BUFSIZE 1024
#define CONFIG_TOKSIZE 32

enum {
  CONFIG_PARSE_INITIAL,
  CONFIG_PARSE_COMMENT,
  CONFIG_PARSE_QUOTED,
  CONFIG_PARSE_UNQUOTED,
  CONFIG_HANDLE_NEWLINE,
  CONFIG_HANDLE_SEP,
  CONFIG_PARSE_VALUE
};

int
config_parse(struct config_t *data, 
	     const char *path)
{
  FILE 			*f;
  char 			*buf, *p;
  char 			*token, *t, *tok_limit;
  int 			result, state, got_tok, tok_cap;
  size_t 		count, line;
  struct config_pair_t 	*pair, *current;


  if(data == 0 || path == 0)
    return -1;
  
  /* Setup */
  data->head = 0;
  data->iter = 0;
  data->iter_prev = 0;
  result = 0;
  line = 1;
  pair = 0;
  current = 0;

  /* Open file */
  f = fopen(path, "r");
  if(f == 0) {
    fprintf(stderr, PACKAGE" error: file '%s' not found\n", path);
    return -1;
  }

  /* Initialize buffer */
  buf = (char*) malloc(sizeof(char) * CONFIG_BUFSIZE);
  token = (char*) malloc(sizeof(char) * CONFIG_TOKSIZE);
  if(buf == 0 || token == 0) {
    result = ENOMEM;
    goto cleanup;
  }

  /* Parse file */
  state = CONFIG_PARSE_INITIAL;
  got_tok = 0;
  tok_cap = CONFIG_TOKSIZE;
  tok_limit = token + tok_cap;
  
  do {

    /* Read a chunk */
    count = fread(buf, sizeof(char), CONFIG_BUFSIZE, f);

    /* Parse the input - manually increment p since not all
       transitions should automatically consume a character */
    for(p = buf; p < buf + count; ) {

      switch(state) {

	/* ==================== Initial parsing state */
      case CONFIG_PARSE_INITIAL:
	if(*p == data->comment_char) {
	  state = CONFIG_PARSE_COMMENT;
	  ++p;
	}
	else if(*p == '"') {
	  t = token;
	  state = CONFIG_PARSE_QUOTED;
	  ++p;
	}
	else if(*p == '\n') {
	  state = CONFIG_HANDLE_NEWLINE;
	}
	else if(*p == data->sep_char) {
	  state = CONFIG_HANDLE_SEP;
	  ++p;
	}
	else if(isspace(*p)) {
	  /* Skip ws */
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

	  state = CONFIG_PARSE_UNQUOTED;
	}
	break;

	/* ==================== Parse comments */
      case CONFIG_PARSE_COMMENT:
	if(*p == '\n') {
	  state = CONFIG_HANDLE_NEWLINE;
	}
	else {
	  ++p;
	}
	break;

	/* ==================== Parse quoted strings */
      case CONFIG_PARSE_QUOTED:
	if(*p == '"') {
	  got_tok = 1;
	  *t = '\0';
	  state = CONFIG_PARSE_INITIAL;
	  ++p;
	}
	else if(*p == '\n') {
	  fprintf(stderr, PACKAGE" error(%s:%i): unterminated string\n",
		  path, line);
	  
	  state = CONFIG_HANDLE_NEWLINE;
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
      case CONFIG_PARSE_UNQUOTED:
	if(*p == data->comment_char) {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }
	  state = CONFIG_PARSE_COMMENT;
	  ++p;
	}
	else if(*p == '\n') {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }
	  state = CONFIG_HANDLE_NEWLINE;
	}
	else if(*p == data->sep_char) {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }

	  state = CONFIG_HANDLE_SEP;
	  ++p;
	}
	/* In this mode a space ends the current token */
	else if(isspace(*p)) {
	  if(t != token) {
	    got_tok = 1;
	    *t = '\0';
	  }
	  
	  state = CONFIG_PARSE_INITIAL;
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
      case CONFIG_HANDLE_SEP:
	if(got_tok == 0) {
	  fprintf(stderr, PACKAGE" error(%s:%i): missing key\n", path, line);
	}
	else {
	  //fprintf(stderr, "sep: token = %s\n", token);

	  pair = (struct config_pair_t*) malloc(sizeof(struct config_pair_t));
	  if(pair == 0) {
	    result = ENOMEM;
	    goto cleanup;
	  }

	  pair->key = strdup(token);
	  pair->value = 0;
	  pair->next = 0;
	  if(pair->key == 0) {
	    result = ENOMEM;
	    goto cleanup;
	  }
	}

	got_tok = 0;
	state = CONFIG_PARSE_INITIAL;
	break;

	/* ==================== Process newlines */
      case CONFIG_HANDLE_NEWLINE:
	if(got_tok == 1 && pair != 0) {
	  //fprintf(stderr, "newline: token = %s\n", token);

	  pair->value = strdup(token);
	  if(pair->value == 0) {
	    result = ENOMEM;
	    goto cleanup;
	  }

	  if(data->head == 0) {
	    data->head = pair;
	  }

	  if(current != 0) {
	    current->next = pair;
	  }

	  current = pair;
	  pair = 0;
	}

	got_tok = 0;
	state = CONFIG_PARSE_INITIAL;
	++line;
	++p;
	break;
      } 
    }    
  } while(feof(f) == 0);

 cleanup:
  free(buf);
  free(token);
  fclose(f);

  return result;
}
