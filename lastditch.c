#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int strcount(char *myString, char char2find) {
	int count = 0, len = strlen(myString), i;
	for (i = 0; i <= len; i++)
		if (myString[i] == char2find)
			count++;
	return count;
}

int main() {
	char *Temp, Temp2[80], Line[80], *Tvalue, **strvalues, **strstrvalues, *strtemp;
	int value_count, i, placedcount = 0;
	FILE *fpopen,*fpwrite;
	
	fpopen = fopen("Test.C","r");
	fpwrite = fopen("output.CPP","w");

	while (!feof(fpopen)) {
		fgets(Line,80,fpopen);
		Temp = strtok(Line,"<");

		if (strstr(Temp, "include") != NULL) {
			fprintf(fpwrite,"%s<",Temp);
			Temp = strtok(NULL,">");

			if (Temp != NULL) {
				if(strcmp(Temp,"stdio.h") == 0)
					fprintf(fpwrite,"%s>\n","iostream.h");	
				else
					fprintf(fpwrite,"%s>\n",Temp);					
			}
		}

		else if (strstr(Temp, "printf") != NULL) {
			if (strstr(Temp, "%") != NULL) {

				value_count = strcount(Temp, '%');

				strcpy(Temp2,Line);
				Temp = strtok(Temp2,"printf");
				if (strstr(Temp,");") == NULL)
					fprintf(fpwrite,"%s",Temp);	
				
				strcpy(Temp2,Line);
				
				Temp = strtok(Temp2,"\"");
				Temp = strtok(NULL,"%");

				if (value_count == 1) {

					if (strstr(Temp, ");") == NULL && value_count < 3)
						fprintf(fpwrite, "cout<<\"%s\"", Temp);
					else
						fprintf(fpwrite, "%s", "cout");

					strcpy(Temp2, Line);
					Tvalue = strtok(Temp2, ",");
					Tvalue = strtok(NULL, ")");
					fprintf(fpwrite, "<<%s", Tvalue);

					strcpy(Temp2, Line);
					Temp = strtok(Temp2, "%");
					Temp = strtok(NULL, "\"");

					if (strlen(Temp) == 1)
						fprintf(fpwrite, "%s\n", ";");
					else {
						Temp++;
						fprintf(fpwrite, "<<\"%s\"%s\n", Temp, ";");
					}
				}
				else {                                                        //"%d ...blah...blah..." does not work when number of %d > 2
					strvalues = malloc(value_count * sizeof(char *));

					strcpy(Temp2, Line);
					Tvalue = strtok(Temp2, ",");
					Tvalue = strtok(NULL, ")");

					strtemp = strtok(Tvalue, ",");
					strvalues[0] = (char *) malloc(strlen(strtemp) + 1);
					strcpy(strvalues[0], strtemp);

					strcpy(Temp2, Line);
					Tvalue = strtok(Temp2, ",");
					Tvalue = strtok(NULL, ")");

					strtemp = strrev(Tvalue);
					strtemp = strtok(strtemp, ",");
					strtemp = strrev(strtemp);

					strvalues[value_count - 1] = (char *) malloc(strlen(strtemp) + 1);
					strcpy(strvalues[value_count - 1], strtemp);

					if (value_count > 2) {
						strcpy(Temp2, Line);
						Tvalue = strtok(Temp2, ",");
						Tvalue = strtok(NULL, ",");

						for (i = 1; i < value_count - 1; i++) {
							strtemp = strtok(NULL, ",");
							strvalues[i] = (char *) malloc(strlen(strtemp) + 1);
							strcpy(strvalues[i], strtemp);
						}
					}

					strstrvalues = malloc((value_count + 1) * sizeof(char *));

					strcpy(Temp2, Line);
					Temp = strtok(Temp2, "\"");
					Temp = strtok(NULL, "\"");

					if (Temp[0] == '%') {
						strstrvalues[0] = (char *) malloc(strlen("") + 1);
						strcpy(strstrvalues[0], "");
					}
					else {
						strtemp = strtok(Temp, "%");
						strstrvalues[0] = (char *) malloc(strlen(strtemp) + 1);
						strcpy(strstrvalues[0], strtemp);
					}

					for (i = 1; i < value_count; i++) {
						strtemp = strtok(NULL, "%");
						strstrvalues[i] = (char *) malloc(strlen(strtemp) + 1);
						strtemp++;
						strcpy(strstrvalues[i], strtemp);
					}

					strcpy(Temp2, Line);
					Temp = strtok(Temp2, "\"");
					Temp = strtok(NULL, "\"");

					strtemp = strrev(Temp);
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
						fprintf(fpwrite, ";\n", strstrvalues[i]);
				}
			}
			else {
				strcpy(Temp2,Line);
				Temp = strtok(Temp2,"printf");
				fprintf(fpwrite,"%s",Temp);	
				
				Temp = strtok(Line,"\"");
				Temp = strtok(NULL,"\"");
				fprintf(fpwrite,"cout<<\"%s\"%s\n",Temp,";");
			}
		}
		
		else
			fprintf(fpwrite,"%s",Temp);
	}
	return 0;
}
