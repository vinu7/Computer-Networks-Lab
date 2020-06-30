#include<stdio.h>

int main()
{
	FILE *file;
	file=fopen("RollC.txt","r");
	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		printf("%s",line);
	}
	fclose(file);
}

