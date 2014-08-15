#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(){
/*
char* string;
string = malloc(50);
fgets(string, 50, stdin);
int o;
o=noEscape(string);
printf("%d\n",o);
string[strlen(string)-1] = '\0';
printf("%s", string);
printf("loooolz");
*/
char* array;
array = malloc (128);
fgets (array, 1024, stdin);
array[strlen(array) - 1] = '\0';
printf("%s", array);
printf("loooolz");

}

int noEscape(char *s)
{
	int i=0;
	int len=strlen(s)-1;
	for(i=0;i<len;i++)
	{
		if(!(s[i]>=32 && s[i]<127))
			return 0;
			
	}
	return 1;
}
