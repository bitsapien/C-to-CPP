//
//  converter_helpers.c
//  cpp_converter
//
//  Created by Ojas Shirekar on 14/01/16.
//  Copyright © 2016 Ojas Shirekar. All rights reserved.
//

#include <string.h>
#include "converter_helpers.h"

int write_as_is(FILE *wp, const char *to_be_written) {
    fprintf(wp, "%s", to_be_written);
    return 0;
}

int strcount(char *myString, char char2find) {
    int count = 0, len = strlen(myString), i;
    for (i = 0; i <= len; i++)
        if (myString[i] == char2find)
            count++;
    return count;
}

char *strrev(char *str) {
    char *p1, *p2;

    if (!str || !*str) {
        return str;
    }
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;   //Binary XOR operator helps copy. Look it up.
        *p2 ^= *p1;
        *p1 ^= *p2;
    }

    return str;
}

void shiftleft(char *Str, int amt) {
    int i;
    for (; amt > 0; amt--) {
        i = 0;
        while (Str[i] != '\0') {
            Str[i] = Str[i + 1];
            i++;
        }
    }
}

void handle_spaces(char *delimiter, char *buffer, FILE *write) {
    char temp_str[80],*temp;
    int tmp_flag = 0,i;

    strcpy(temp_str, buffer);
    temp = strtok(temp_str, delimiter);
    for (i = 0; i < strlen(temp); i++) {
        if (temp[i] != ' ' && temp[i] != '\t')
            tmp_flag = 1;
    }
    if (tmp_flag == 0)
        fprintf(write, "%s", temp);
}
