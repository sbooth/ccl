/*
 *  $Id: tester.c,v 1.4 2004-04-15 18:30:59 sbooth Exp $
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
#include <stdio.h>

#include "ccl/ccl.h"

void
usage(const char *prog_name)
{
  printf("usage: %s config_file\n", prog_name);
}

int
main(int argc,
     char **argv)
{
  struct ccl_t 			config;
  const struct ccl_pair_t 	*iter;

  if(argc == 1) {
    usage(argv[0]);
    return 0;
  }

  config.comment_char = '#';
  config.sep_char = '=';
  config.str_char = '"';

  ccl_parse(&config, argv[1]);

  while((iter = ccl_iterate(&config)) != 0) {
    printf("(%s,%s)\n", iter->key, iter->value);
  }

  ccl_release(&config);

  return 0;
}
