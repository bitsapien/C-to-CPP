#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "converter_helpers.h"

int converter();


int main() {
    int success;
    success = converter();

    if (success != 0) {
        printf("Conversion failed.\n");
    }
    return 0;
}

int converter() {
    char *tmp, tmp2[80], Line[80], *Tvalue, **strvalues, **strstrvalues, *strtemp;
    int value_count, i;
    FILE *fpopen, *fpwrite;

    fpopen = fopen("test.dat", "r");
    fpwrite = fopen("output.cpp", "w+");

    while (!feof(fpopen)) {
        fgets(Line, 80, fpopen);
        tmp = strtok(Line, "<");

        if (strstr(tmp, "include") != NULL) {
            fprintf(fpwrite, "%s<", tmp);
            tmp = strtok(NULL, ">");

            if (tmp != NULL) {
                if (strcmp(tmp, "stdio.h") == 0)
                    fprintf(fpwrite, "%s>\n", "iostream.h");
                else
                    fprintf(fpwrite, "%s>\n", tmp);
            }
        }

        else if (strstr(tmp, "printf") != NULL) {
            if (strstr(tmp, "%") != NULL) {

                value_count = strcount(tmp, '%');

                strcpy(tmp2, Line);
                tmp = strtok(tmp2, "printf");
                fprintf(fpwrite, "%s", tmp);

                strcpy(tmp2, Line);

                tmp = strtok(tmp2, "\"");
                tmp = strtok(NULL, "%");

                if (value_count == 1) {

                    if (strstr(tmp, ");") == NULL && value_count < 3)
                        fprintf(fpwrite, "cout<<\"%s\"", tmp);
                    else
                        fprintf(fpwrite, "%s", "cout");

                    strcpy(tmp2, Line);
                    Tvalue = strtok(tmp2, ",");
                    Tvalue = strtok(NULL, ")");
                    fprintf(fpwrite, "<<%s", Tvalue);

                    strcpy(tmp2, Line);
                    tmp = strtok(tmp2, "%");
                    tmp = strtok(NULL, "\"");

                    if (strlen(tmp) == 1)
                        fprintf(fpwrite, "%s\n", ";");
                    else {
                        tmp++;
                        fprintf(fpwrite, "<<\"%s\"%s\n", tmp, ";");
                    }
                }
                else {                                                        //"%d ...blah...blah..." does not work when number of %d > 2
                    strvalues = malloc(value_count * sizeof(char *));

                    strcpy(tmp2, Line);
                    Tvalue = strtok(tmp2, ",");
                    Tvalue = strtok(NULL, ")");

                    strtemp = strtok(Tvalue, ",");
                    strvalues[0] = (char *) malloc(strlen(strtemp) + 1);
                    strcpy(strvalues[0], strtemp);

                    strcpy(tmp2, Line);
                    Tvalue = strtok(tmp2, ",");
                    Tvalue = strtok(NULL, ")");

                    strtemp = strrev(Tvalue);
                    strtemp = strtok(strtemp, ",");
                    strtemp = strrev(strtemp);

                    strvalues[value_count - 1] = (char *) malloc(strlen(strtemp) + 1);
                    strcpy(strvalues[value_count - 1], strtemp);

                    if (value_count > 2) {
                        strcpy(tmp2, Line);
                        Tvalue = strtok(tmp2, ",");
                        Tvalue = strtok(NULL, ",");

                        for (i = 1; i < value_count - 1; i++) {
                            strtemp = strtok(NULL, ",");
                            strvalues[i] = (char *) malloc(strlen(strtemp) + 1);
                            strcpy(strvalues[i], strtemp);
                        }
                    }

                    strstrvalues = malloc((value_count + 1) * sizeof(char *));

                    strcpy(tmp2, Line);
                    tmp = strtok(tmp2, "\"");
                    tmp = strtok(NULL, "\"");

                    if (tmp[0] == '%') {
                        strstrvalues[0] = (char *) malloc(strlen("") + 1);
                        strcpy(strstrvalues[0], "");
                    }
                    else {
                        strtemp = strtok(tmp, "%");
                        strstrvalues[0] = (char *) malloc(strlen(strtemp) + 1);
                        strcpy(strstrvalues[0], strtemp);
                    }

                    for (i = 1; i < value_count; i++) {
                        strtemp = strtok(NULL, "%");
                        strstrvalues[i] = (char *) malloc(strlen(strtemp) + 1);
                        strtemp++;
                        strcpy(strstrvalues[i], strtemp);
                    }

                    strcpy(tmp2, Line);
                    tmp = strtok(tmp2, "\"");
                    tmp = strtok(NULL, "\"");

                    strtemp = strrev(tmp);
                    strtemp = strtok(strtemp, "%");
                    strtemp = strrev(strtemp);
                    strtemp[strlen(strtemp)] = '\0';

                    strstrvalues[value_count] = (char *) malloc(strlen(strtemp) + 1);
                    strtemp++;
                    strcpy(strstrvalues[value_count], strtemp);

                    if (strlen(strstrvalues[0]) != 0) {
                        fprintf(fpwrite, "cout<<\"%s\"", strstrvalues[0]);
                        fprintf(fpwrite, "<<%s", strvalues[0]);
                    }
                    else
                        fprintf(fpwrite, "cout<<%s", strvalues[0]);

                    for (i = 1; i < value_count; i++) {
                        if (strlen(strstrvalues[i]) != 0)
                            fprintf(fpwrite, "<<\"%s\"", strstrvalues[i]);
                        fprintf(fpwrite, "<<%s", strvalues[i]);
                    }
                    if (strlen(strstrvalues[i]) != 0)
                        fprintf(fpwrite, "<<\"%s\";\n", strstrvalues[i]);
                    else
                        fprintf(fpwrite, "%s;\n", strstrvalues[i]);
                }
            }
            else {
                strcpy(tmp2, Line);
                tmp = strtok(tmp2, "printf");
                fprintf(fpwrite, "%s", tmp);

                tmp = strtok(Line, "\"");
                tmp = strtok(NULL, "\"");
                fprintf(fpwrite, "cout<<\"%s\"%s\n", tmp, ";");
            }
        } else if (strstr("scanf", tmp)) {

        }

        else
            fprintf(fpwrite, "%s", tmp);
    }
    return 0;
}