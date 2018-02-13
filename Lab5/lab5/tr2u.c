#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s 'from' 'to' \n", argv[0]);
		exit(1);
	}

	char *from = argv[1];
	char *to = argv[2];

	int from_len;
	int to_len;

	from_len =strlen(from);
	to_len = strlen(to);

	if(from_len != to_len)
	{
		fprintf(stderr, "Error: 'from' and 'to' are not the same length\n.");
		exit(1);
	}

	int i, j;
	
	for(i = 0; i < from_len	; i++)
	{
		for(j = i+1; j < to_len	; j++)
		{
			if(from[i] == from[j])
			{
				fprintf(stderr, "Error: 'from' cannot have duplicate bytes\n");
				exit(1);
			}	
		}
	}

	int exists = 0;
	char curr;
	while (read(0, &curr, 1) != 0)
	{
		for(i = 0; i < from_len; i++)
		{
			if (curr == from[i])
			{
				write(1, to+i, 1);
				exists = 1;
				break;
			}
			else
				exists = 0;
		}

		if (!exists)
			write(1, &curr, 1);
	}
}