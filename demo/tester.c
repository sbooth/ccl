#include <stdio.h>
#include "libconfig.h"

void
usage(const char *prog_name)
{
  printf("usage: %s config_file_name\n", prog_name);
}

int
main(int argc,
     char **argv)
{
  struct config_t config;
  const struct config_pair_t *iter;

  if(argc == 1) {
    usage(argv[0]);
    return 0;
  }

  config.comment_char = '#';
  config.sep_char = '=';

  config_parse(&config, argv[1]);

  config_iterate_reset(&config);
  while((iter = config_iterate(&config)) != 0) {
    printf("(%s,%s)\n", iter->key, iter->value);
  }

  config_release(&config);  
}
