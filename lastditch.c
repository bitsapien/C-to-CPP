#include<stdio.h>
#include<string.h>

int main()
{
	char *Temp,Temp2[80],Line[80],*Tvalue;
	FILE *fpopen,*fpwrite;
	
	fpopen = fopen("Test.C","r");
	fpwrite = fopen("output.CPP","w");
	
	while(!feof(fpopen))
	{
		fgets(Line,80,fpopen);
		Temp = strtok(Line,"<");
		//stdio
		if(strstr(Temp,"include") != NULL)
		{
			fprintf(fpwrite,"%s<",Temp);
			Temp = strtok(NULL,">");
		
			if(Temp != NULL)
			{	
				if(strcmp(Temp,"stdio.h") == 0)
					fprintf(fpwrite,"%s>\n","iostream.h");	
				else
					fprintf(fpwrite,"%s>",Temp);					
			}
		}
		
		else  if(strstr(Temp,"printf") != NULL)
		{
			if(strstr(Temp,"%") != NULL)
			{
				strcpy(Temp2,Line);
				Temp = strtok(Temp2,"printf");
				fprintf(fpwrite,"%s",Temp);	
				
				strcpy(Temp2,Line);
				
				Temp = strtok(Temp2,"\"");
				Temp = strtok(NULL,"%");
				fprintf(fpwrite,"cout<<\"%s\"",Temp);
				
				//while(strstr(Temp,"%") != NULL)
				//{
					strcpy(Temp2,Line);
					Tvalue = strtok(Temp2,",");
					Tvalue = strtok(NULL,")");
					fprintf(fpwrite,"<<%s",Tvalue);
					
					strcpy(Temp2,Line);
					Temp = strtok(Temp2,"%");
					Temp = strtok(NULL,"\"");
					 
					printf("%s",Temp); 
					if(strlen(Temp) == 1)
						fprintf(fpwrite,"%s\n",";");
					else
					{	
						Temp += 2;
						fprintf(fpwrite,"<<\"%s\"%s\n",Temp,";");
					}
				//}	
			}
			else
			{
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