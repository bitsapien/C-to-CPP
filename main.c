#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "converter_helpers.h"

int converter();
char ** argument_values(char *,int);

int main() {
    int success;
    success = converter();

    if (success != 0) {
        printf("Conversion failed.\n");
    }
    return 0;
}

char ** argument_values(char *buffer_read, int value_count)
{
    char **arguments,tmp2[80],*temp_value,*temp_str;
    int i;
    arguments = malloc(value_count * sizeof(char *));

    if(value_count == 1) {
        strcpy(tmp2, buffer_read);
        temp_str = strtok(tmp2,",");
        temp_str = strtok(NULL,")");
        arguments[0] = (char *) malloc(strlen(temp_str) + 1);
        strcpy(arguments[0], temp_str);

        return arguments;
    }

    strcpy(tmp2, buffer_read);
    temp_value = strtok(tmp2, ",");
    temp_value = strtok(NULL, ")");

    temp_str = strtok(temp_value, ",");
    arguments[0] = (char *) malloc(strlen(temp_str) + 1);
    strcpy(arguments[0], temp_str);

    strcpy(tmp2, buffer_read);
    temp_value = strtok(tmp2, ",");
    temp_value = strtok(NULL, ")");

    temp_str = strrev(temp_value);
    temp_str = strtok(temp_str, ",");
    temp_str = strrev(temp_str);

    arguments[value_count - 1] = (char *) malloc(strlen(temp_str) + 1);
    strcpy(arguments[value_count - 1], temp_str);

    if (value_count > 2) {
        strcpy(tmp2, buffer_read);
        temp_value = strtok(tmp2, ",");
        temp_value = strtok(NULL, ",");

        for (i = 1; i < value_count - 1; i++) {
            temp_str = strtok(NULL, ",");
            arguments[i] = (char *) malloc(strlen(temp_str) + 1);
            strcpy(arguments[i], temp_str);
        }
    }

    return arguments;
}

int converter() {
    char *tmp, tmp2[80], Line[80], *Tvalue, **strvalues, **strstrvalues, *strtemp;
    int value_count, i, tmp_flag = 0;
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
                for (i = 0; i <= strlen(tmp); i++) {
                    if (tmp[i] != ' ' || tmp[i] != '\t') {
                        tmp_flag = 1;
                        break;
                    }
                }
                if (tmp_flag == 0)
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
                    strcpy(tmp2, Line);
                    strvalues = argument_values(tmp2,value_count);

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
                for (i = 0; i <= strlen(tmp); i++) {
                    if (tmp[i] != ' ' || tmp[i] != '\t') {
                        tmp_flag = 1;
                        break;
                    }
                }
                if (tmp_flag == 0)
                    fprintf(fpwrite, "%s", tmp);

                tmp = strtok(Line, "\"");
                tmp = strtok(NULL, "\"");
                fprintf(fpwrite, "cout<<\"%s\"%s\n", tmp, ";");
            }
        }

        else if (strstr(tmp,"scanf") != NULL) {
            strcpy(tmp2,Line);
            value_count = strcount(tmp2,'%');
            strvalues = argument_values(tmp2,value_count);
            fprintf(fpwrite, "%s", "cin");
            for(i = 0 ; i < value_count ; i++) {
                shiftleft(strvalues[i],1);
                fprintf(fpwrite, ">>%s", strvalues[i]);
            }
            fprintf(fpwrite, "%s\n", ";");
        }

        else
            fprintf(fpwrite, "%s", tmp);
    }
    return 0;
}