#include <stdio.h>
#include <stdlib.h>

#define SPACE		' '

int decrypt(char const * str1)
{
	return *str1 ^ 42;
}

int frobcmp(char const * str1, char const * str2)
{
	while (1)
	{
		//if both characters are space, return 0

		if (*str1 == SPACE && *str2 == SPACE)
			return 0;
		// if str1 is less than str2, return -1
		else if (*str1 == SPACE || (decrypt(str1) < decrypt(str2)))
			return -1;
		// if str2 is less than str1, return 1
		else if (*str2 == SPACE || (decrypt(str1) > decrypt(str2)))
			return 1;

		str1++;
		str2++;
	}
}

int cmpWrap(const void* a, const void* b)
{
	return frobcmp(*(const char**)a, *(const char**)b);
}

void checkInputError()
{
	if (ferror(stdin))
	{
		fprintf(stderr, "Input error reading from stdin.");
	}
}

int main(void)
{
	int(*cmp) (const void*, const void*);	//declare a pointer to the compare function

	cmp = &cmpWrap;

	char* line;
	line = (char*)malloc(sizeof(char));		//allocate memory for line

	char ** lineArray;
	lineArray = (char**)malloc(sizeof(char*));	//allocate memory for lineArray

	if (line == NULL || lineArray == NULL)
	{
		fprintf(stderr, "Memory allocation error.");	
			exit(1);
	}

	char currChar = getchar();
	checkInputError();

	int lineIndex;
	int arrayIndex;

	lineIndex = 0;
	arrayIndex = 0;

	while (currChar != EOF)
	{
		//add current char to end of line
		*(line + lineIndex) = currChar;
		char *templine = realloc(line, sizeof(char)*(lineIndex + 2));
		if (templine == NULL)
		{
			free(line);
			fprintf((stderr), "Memory allocation error");
			exit(1);
		}
		else
			line = templine;
		lineIndex++;

		if (currChar == SPACE)
		{
			//if it reaches space, add line
			*(lineArray + arrayIndex) = line;
			char **tempArray = realloc(lineArray, sizeof(char*)*(arrayIndex + 2));
			if (tempArray == NULL)
			{
				free(lineArray);
				fprintf(stderr, "Memory allocation error");
				exit(1);
			}
			else
			{
				lineArray = tempArray;
				//set line to point to new memory location for new string of line
				line = NULL;
				line = (char*)malloc(sizeof(char));
				lineIndex = 0;
				arrayIndex++;
			}
		}

		char nextChar = getchar();
		checkInputError();

		//to check if the file has reached the end

		if (nextChar == EOF && currChar == SPACE)
		{
			break;
		}
		else if (nextChar == EOF)
			currChar = SPACE;
		else if (nextChar == SPACE && currChar == SPACE) 	// remove extra spaces
		{
			while (currChar == SPACE)
			{
				currChar = getchar();
				checkInputError();
			}
		}
		else
			currChar = nextChar;
	}

	qsort(lineArray, arrayIndex, sizeof(char*), cmp);
	//void qsort (void* base, size_t num, size_t size, int (*compar)(const void*,const void*));

	//print all characters

	int i;
	for (i = 0; i < arrayIndex; i++)
	{
		int j;
		j = 0;
		while (1)
		{
			if (putchar(lineArray[i][j]) == EOF)
			{
				fprintf(stderr, "Stdout error.");
				exit(1);
			}

			if (lineArray[i][j] == SPACE )
				break;
			j++;
		}
	}

	for (i = 0; i < arrayIndex; i++)
	{
		free(lineArray[i]);
	}
	free(lineArray);

	exit(0);
}
