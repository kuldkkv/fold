#include<stdio.h>
#include<unistd.h>

#define MAX_WIDTH 8190
#define DEFAULT_WIDTH 30

int fold(char line[], int max_width);
char* substr(char line[], int *from_index, int *to_index);
void parse_args(int argc, char *argv[], int *arg_width);

int main(int argc, char *argv[])
{
	char *line = NULL;
	size_t len = 0;
	ssize_t chars_read;
	int arg_width = DEFAULT_WIDTH;

	parse_args(argc, argv, &arg_width);
	while ((chars_read = getline(&line, &len, stdin)) != -1)	{
		//printf("%zu: %s", chars_read, line);
		fold(line, arg_width);
	}
	return 0;
}

void parse_args(int argc, char *argv[], int *arg_width)
{
	int opt;

	while ((opt = getopt(argc, argv, "w:")) != -1) {
		switch(opt) {
		case 'w':
			*arg_width = atoi(optarg);
			if (*arg_width < 4 || *arg_width > MAX_WIDTH) {
				fprintf(stderr, "error: width should be between 4 and %d\n",
                    MAX_WIDTH);
                exit(1);
            }
			break;
		default:
			fprintf(stderr, "Usage: %s [-w linewidth]\n",
				argv[0]);
			exit(1);
		}
	}
}


	
int fold(char line[], int max_width)
{
	size_t len;
	int from_index, to_index;

	len = strlen(line);
	//printf("line is of %d width\n", len);
	from_index = 0;
	while (from_index < len) {
		to_index = from_index + max_width - 2;
		//printf("in fold [%s]\n", substr(line, from_index, to_index));
		printf("[%s]\n", substr(line, &from_index, &to_index));
		from_index = to_index;
	}
}

char* substr(char line[], int *from_index, int *to_index)
{
	char sub[MAX_WIDTH];
	int i = 0, j;

	// ignore all blank spaces from the start
	for ( ; isspace(line[*from_index]); (*from_index)++, (*to_index)++)
		;

	while ((i + (*from_index)) < *to_index)	{
		sub[i] = line[i + (*from_index)];
		i = i + 1;
	}
	// check for '-'
	j = i + *from_index;
	if (isalpha(line[j-2]) && isalpha(line[j-1]) && isalpha(line[j])) {
		sub[i-1] = '-';
		(*to_index)--;
	}
	sub[i] = '\0';
	return sub;
}
