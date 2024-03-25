#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

#define TEST 1   // 1 dla testowania, 0 dla automatycznej oceny

int count[MAX_DIGRAMS] = { 0 };
int count2[MAX_CHARS] = { 0 };
// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count2[va] == count2[vb]) return va - vb; // sort alphabetically if counts equal
	return count2[vb] - count2[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stream
void wc(int *nl, int *nw, int *nc, FILE *stream)
{
	char character,prevch;
	while ((character = fgetc(stream)) != EOF)
	{
		switch(character)
		{
			case ' ':
				break;
			case NEWLINE:
				*nl += 1;
				break;
			default:
				if (prevch==NEWLINE || prevch==' ')
					*nw+=1;
				break;
		}
		*nc+=1;
		prevch=character;
	}
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stream. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt, FILE *stream)
{
	void swap(int* p1, int* p2)
	{
		int temp;
		temp = *p1;
		*p1 = *p2;
		*p2 = temp;
	}

	int partition(int arr[], int low, int high)
	{
		int pivot = arr[high];
		int i = (low - 1);

		for (int j = low; j <= high; j++) {
			if (cmp(&arr[j], &pivot)>0) {
				i++;
				swap(&arr[i], &arr[j]);
			}
		}
		swap(&arr[i + 1], &arr[high]);
		return (i + 1);
	}
	void qsort(int arr[], int low, int high)
	{
		if (low < high) {
			// pi is the partition return index of pivot

			int pi = partition(arr, low, high);

			// Recursion Call
			// smaller element than pivot goes left and
			// higher element goes right
			qsort(arr, low, pi - 1);
			qsort(arr, pi + 1, high);
		}
	}
	char ch;
	int chars[MAX_CHARS];
	for(int i = 0;i < MAX_CHARS;i++)
	{
		chars[i] = FIRST_CHAR+i;
	}
	while((ch=fgetc(stream))!= EOF)
	{
		count2[ch]+=1;
	}
	qsort(chars,0,MAX_CHARS-1);
	*n_char = chars[MAX_CHARS-char_no];
	*cnt = count2[(int)*n_char];
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stream. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[], FILE *stream)
{
	void swap(int* p1, int* p2)
	{
		int temp;
		temp = *p1;
		*p1 = *p2;
		*p2 = temp;
	}

	int partition(int arr[], int low, int high)
	{
		int pivot = arr[high];
		int i = (low - 1);

		for (int j = low; j <= high; j++) {
			if (cmp_di(&arr[j], &pivot)>0) {
				i++;
				swap(&arr[i], &arr[j]);
			}
		}
		swap(&arr[i + 1], &arr[high]);
		return (i + 1);
	}
	void qsort(int arr[], int low, int high)
	{
		if (low < high) {
			int pi = partition(arr, low, high);
			qsort(arr, low, pi - 1);
			qsort(arr, pi + 1, high);
		}
	}
	char ch,prevchar;
	int diagrams[MAX_DIGRAMS];
	for(int i = 0;i < MAX_CHARS;i++)
	{
		for(int j = 0;j < MAX_CHARS;j++)
		{
			diagrams[i*MAX_CHARS+j] = i*MAX_CHARS+j;
		}
	}
	while((ch=fgetc(stream))!= EOF)
	{
		// count[(prevchar-FIRST_CHAR)*MAX_CHARS+ch-FIRST_CHAR]+=1;
		prevchar=ch;
	}
	qsort(diagrams,0,MAX_CHARS-1);
	digram[0] = diagrams[MAX_DIGRAMS-digram_no] / MAX_CHARS;
	digram[1] = diagrams[MAX_DIGRAMS-digram_no] % MAX_CHARS;
	digram[2] = count[digrams[0]/MAX_CHARS+digram[1]%MAX_CHARS]
}

// Count block and line comments in the text read from stream. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter, FILE *stream)
{
	char prevch,character;
	int type=0;
	while((character = fgetc(stream))!=EOF)
	{
		if(!type && character=='/' && prevch=='/')
		{
			*line_comment_counter+=1;
			type=1;
			type=IN_LINE_COMMENT;
		}
		if(character==NEWLINE && type==IN_LINE_COMMENT && type)
		{
			type=0;
		}
		if(!type && character=='*' && prevch=='/')
		{
			type=IN_BLOCK_COMMENT;
			*block_comment_counter+=1;
		}
		if(character=='/' && prevch=='*' && type==IN_BLOCK_COMMENT)
		{
			type=0;
		}
		prevch=character;
	}
}

#define MAX_LINE 128

int read_line() {
	char line[MAX_LINE];
	int n;

	fgets (line, MAX_LINE, stdin); // to get the whole line
	sscanf (line, "%d", &n);
	return n;
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	char file_name[40];
	FILE *stream;

	if(TEST) printf("Wpisz nr zadania ");
    to_do = read_line();      //    scanf ("%d", &to_do);
    if(TEST)  stream = stdin;
    else {
        scanf("%s",file_name);  read_line();
        stream = fopen(file_name,"r");
        if(stream == NULL) {
            printf("fopen failed\n");
            return -1;
        }
    }

//	to_do = read_line();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc, stream);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
      if(TEST) printf("Wpisz numer znaku ");
      char_no = read_line();  //    scanf("%d",&char_no);
			char_count(char_no, &n_char, &cnt, stream);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // digram_count()
      if(TEST) printf("Wpisz numer digramu ");
      char_no = read_line();   //  scanf("%d",&char_no);
			digram_count(char_no, digram, stream);
			printf("%c%c %d\n", digram[0], digram[1], digram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter, stream);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

