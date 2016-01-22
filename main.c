#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "converter_helpers.h"

#define BUFF_CPY_STR(start,end) ({\
                strcpy(tmp2, buffer_read);\
                temp_str = strtok(tmp2,start);\
                temp_str = strtok(NULL,end);\
})

#define BUFF_CPY_VALUE(start,end) ({\
                strcpy(tmp2, buffer_read);\
                temp_value = strtok(tmp2,start);\
                temp_value = strtok(NULL,end);\
})

#define BUFF_CPY_TMP(start,end) ({\
                strcpy(tmp2, buffer_read);\
                tmp = strtok(tmp2,start);\
                tmp = strtok(NULL,end);\
})

int converter();
char * spcadjust(char **,int);
char ** argument_values(char *,int);

int main() {
    int success;
    success = converter();

    if (success != 0)
        printf("Conversion failed.\n");
    return 0;
}

char ** argument_values(char *buffer_read, int value_count)
{
    char **arguments,tmp2[80],*temp_value,*temp_str;
    int i;
    arguments = malloc(value_count * sizeof(char *));

    if(value_count == 1) {
        BUFF_CPY_STR(",",")");
        arguments[0] = (char *) malloc(strlen(temp_str) + 1);
        strcpy(arguments[0], temp_str);
        return arguments;
    }

    BUFF_CPY_VALUE(",",")");

    temp_str = strtok(temp_value, ",");
    arguments[0] = (char *) malloc(strlen(temp_str) + 1);
    strcpy(arguments[0], temp_str);

    BUFF_CPY_VALUE(",",")");

    temp_str = strrev(temp_value);
    temp_str = strtok(temp_str, ",");
    temp_str = strrev(temp_str);
    arguments[value_count - 1] = (char *) malloc(strlen(temp_str) + 1);
    strcpy(arguments[value_count - 1], temp_str);

    if (value_count > 2) {
        BUFF_CPY_VALUE(",",",");
        for (i = 1; i < value_count - 1; i++) {
            temp_str = strtok(NULL, ",");
            arguments[i] = (char *) malloc(strlen(temp_str) + 1);
            strcpy(arguments[i], temp_str);
        }
    }
    return arguments;
}

int converter() {
    char *tmp, tmp2[80], buffer_read[80], *temp_value, **strvalues, **strstrvalues, *strtemp;
    int value_count, i;
    FILE *fpopen, *fpwrite;

    fpopen = fopen("test.dat", "r");
    fpwrite = fopen("output.cpp", "w+");

    while (!feof(fpopen)) {
        fgets(buffer_read, 80, fpopen);
        tmp = strtok(buffer_read, "<");

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
            handle_spaces("printf",buffer_read,fpwrite);

            if (strstr(tmp, "%") != NULL) {
                value_count = strcount(tmp, '%');

                BUFF_CPY_TMP("\"","%");

                if (value_count == 1) {
                    if (strstr(tmp, ");") == NULL)
                        fprintf(fpwrite, "cout<<\"%s\"", tmp);
                    else
                        fprintf(fpwrite, "%s", "cout");

                    BUFF_CPY_VALUE(",",")");

                    fprintf(fpwrite, "<<%s", temp_value);

                    BUFF_CPY_TMP("%","\"");

                    if (strlen(tmp) == 1)
                        fprintf(fpwrite, "%s\n", ";");
                    else
                        fprintf(fpwrite, "<<\"%s\"%s\n", ++tmp, ";");
                }
                else {                                                        //"%d ...blah...blah..." does not work when number of %d > 2
                    strcpy(tmp2, buffer_read);
                    strvalues = argument_values(tmp2,value_count);
                    strstrvalues = malloc((value_count + 1) * sizeof(char *));

                    BUFF_CPY_TMP("\"","\"");

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
                        strcpy(strstrvalues[i], ++strtemp);
                    }

                    BUFF_CPY_TMP("\"","\"");

                    strtemp = strrev(tmp);
                    strtemp = strtok(strtemp, "%");
                    strtemp = strrev(strtemp);
                    strtemp[strlen(strtemp)] = '\0';

                    strstrvalues[value_count] = (char *) malloc(strlen(strtemp) + 1);
                    strcpy(strstrvalues[value_count], ++strtemp);

                    if (strlen(strstrvalues[0]) != 0) {
                        fprintf(fpwrite, "cout<<\"%s\"", strstrvalues[0]);
                        fprintf(fpwrite, "<<%s", spcadjust(strvalues,0));
                    }
                    else
                        fprintf(fpwrite, "cout<<%s", spcadjust(strvalues,0));

                    for (i = 1; i < value_count; i++) {
                        if (strlen(strstrvalues[i]) != 0)
                            fprintf(fpwrite, "<<\"%s\"", strstrvalues[i]);
                        fprintf(fpwrite, "<<%s", spcadjust(strvalues,i));
                    }
                    if (strlen(strstrvalues[i]) != 0)
                        fprintf(fpwrite, "<<\"%s\";\n", strstrvalues[i]);
                    else
                        fprintf(fpwrite, "%s;\n", strstrvalues[i]);
                }
            }
            else {
                BUFF_CPY_TMP("\"","\"");
                fprintf(fpwrite, "cout<<\"%s\"%s\n", tmp, ";");
            }
        }

        else if (strstr(tmp,"scanf") != NULL) {
            handle_spaces("scanf",buffer_read,fpwrite);
            strcpy(tmp2,buffer_read);
            value_count = strcount(tmp2,'%');
            strvalues = argument_values(tmp2,value_count);
            fprintf(fpwrite, "%s", "cin");

            for(i = 0 ; i < value_count ; i++)
                fprintf(fpwrite, ">>%s", spcadjust(strvalues,i));
            fprintf(fpwrite, "%s\n", ";");
        }

        else
            fprintf(fpwrite, "%s", tmp);
    }
    return 0;
}

char * spcadjust(char **strvalues, int i)
{
    while(strvalues[i][0] == '&' || strvalues[i][0] == ' ')
        shiftleft(strvalues[i],1);
    return strvalues[i];
}