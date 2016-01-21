//
//  converter_helpers.c
//  cpp_converter
//
//  Created by Ojas Shirekar on 14/01/16.
//  Copyright © 2016 Ojas Shirekar. All rights reserved.
//

#include <string.h>
#include "converter_helpers.h"

int write_as_is(FILE *wp, const char *to_be_written){
    fprintf(wp, "%s", to_be_written);
    return 0;
}

char *strrev(char *str){
  char *p1, *p2;

  if(!str || ! *str){
    return str;
  }
  for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2 ) {
    *p1 ^= *p2;   //Binary XOR operator helps copy. Look it up.
    *p2 ^= *p1;
    *p1 ^= *p2;
  }

  return str;
}

int strcount(char *myString, char char2find) {
  int count = 0, len = strlen(myString), i;
  for (i = 0; i <= len; i++)
    if (myString[i] == char2find)
      count++;
  return count;
}