# customizable configuration library

ccl is the customizable configuration library, a collection of functions for application programmers wishing to interface with user-editable configuration files containing key/value pairs.

ccl is customizable because it allows the comment, key/value, and string literal delimiters to be programatically specified at runtime.

ccl is designed to be simple and portable; it has a small interface consisting of five functions and is written in ANSI/ISO C. ccl uses avl's implementation of binary search trees for backend storage.

ccl is hosted at https://github.com/sbooth/ccl/

# Example

An example is the best way to understand how ccl works. A configuration file named `example.conf` might contain:

```
## Sample configuration file 
Desktop-Picture = /usr/images/earth.jpg 
Position = Centered 
"Background Color" = Black 
```

The following code demonstrates how to parse and access this file using ccl:

```c
#include "ccl/ccl.h"

struct ccl_t config;
const struct ccl_pair_t *iter;

/* Set configuration file details */
config.comment_char = '#';
config.sep_char = '=';
config.str_char = '"';

/* Parse the file */
ccl_parse(&config, "example.conf");

/* Iterate through all key/value pairs */
while((iter = ccl_iterate(&config)) != 0) {
  printf("(%s,%s)\n", iter->key, iter->value);
}

/* Clean up */
ccl_release(&config);
```

When compiled, the snippet above produces the output

```
(Background Color,Black)
(Desktop-Picture,/usr/images/earth.jpg)
(Position,Centered)
```
