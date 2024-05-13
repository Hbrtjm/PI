#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define NULLTERMINATOR 0
#define NEWLINE '\n'

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) 
{
    return A[row * cols + col];
}

void set(int cols, int row, int col, int *A, int value) 
{
    A[row * cols + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB)
{
	int sum;
    for(int i = 0;i < rowsA;i++)
    {
        for(int j = 0;j < colsB;j++)
        {
			sum = 0;
            for(int k = 0;k < colsA;k++)
            {
                sum += get(colsA,i,k,A) * get(colsB,k,j,B);
            }
			set(rowsA,i,j,AB,sum);
        }
    }
}

void read_mat(int rows, int cols, int *t) 
{
    for(int i = 0; i < rows; i++)
	{
        for(int j = 0; j < cols; j++)
		{
            scanf("%i", &t[i*cols + j]);
        }
    }
}

void print_mat(int rows, int cols, int *t) 
{
    for(int i = 0;i < rows;i++)
    {
        for(int j = 0;j < cols;j++)
        {
            printf("%d ",t[i * cols + j]);
        }
		printf("\n");
    }
}

int read_char_lines(char *array[]) 
{    
	char buffer[BUF_SIZE] = {0};
    int lines_counter = 0, length;
    while(!feof(stdin)){
		length = strlen(stdin);
        fgets(buffer, BUF_SIZE, stdin);
        array[lines_counter] = malloc((length + 1) * sizeof(char));
        memcpy(array[lines_counter], buffer, length + 1);
        lines_counter++;
    }
    array[lines_counter] = NULL;
    return lines_counter;
}

void write_char_line(char *array[], int n) 
{
	printf("%s \n", array[n]);
}

void delete_lines(char *array[]) 
{
	int index = 0;
	while(array[index] != NULL)
	{
		free(array[index++]);
	}
}

int* read_int_line(int* size){
    int* line = NULL, result, buffer_counter = 0;
    *size = 0;
    char buffer[TAB_SIZE] = {0}, c = fgetc(stdin);

    while(!feof(stdin) && c != '\n'){
        if(c == ' '){
            result = atoi(buffer);
            memset(buffer, 0, BUF_SIZE);
            line = realloc(line, (*size + 1)*sizeof(int));
            line[*size] = result;
            buffer_counter = 0;
            (*size)++;
        } else {
            buffer[buffer_counter] = c;
            buffer_counter++;
        }

        c = fgetc(stdin);
    }

    if(buffer_counter == 0)
        return NULL;
    result = atoi(buffer);
    line = realloc(line,  (*size + 1)*sizeof(int));
    line[*size] = result;
    (*size)++;
    return line;
}

int read_int_lines_cont(int *ptr_array[]) {
    int elements_counter = 0, lines_counter = 0,* tab = ptr_array[0], line_size,* line_array;
    while(!feof(stdin)){
        line_size = 0;
        line_array = read_int_line(&line_size);
        if(line_array == NULL)
            break;
        memcpy(&tab[elements_counter], line_array, sizeof(int)*line_size);
        free(line_array);
        ptr_array[lines_counter] = &tab[elements_counter];
        elements_counter += line_size;
        lines_counter++;
    }
    ptr_array[lines_counter] = ptr_array[0] + elements_counter;
    return lines_counter;
}

void write_int_line_cont(int *ptr_array[], int n)
{
    int* temp_ptr = ptr_array[n];

    while(temp_ptr != ptr_array[n + 1]){
        printf("%i ", *temp_ptr);
        temp_ptr++;
    }
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) 
{
	int lines_count = 0,line_length,*line_array;
	while(!feof(stdin))
	{
		line_length = 0;
		line_array = read_int_lines(&line_length);
		if(line_array == NULL)
		{
			return lines_count;
		}
		lines_array[lines_count].values = malloc(sizeof(int)*line_length);
		lines_array[lines_count].len = line_length;
	}
	return lines_count;
}

void write_int_line(line_type lines_array[], int n) 
{
    for(int i = 0; i < lines_array[n].len; i++){
        printf("%i ", lines_array[n].values[i]);
    }
    printf("\n%.2lf \n", lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) 
{
    for(int i = 0; i < line_count; i++)
        free(array[i].values);
}

int cmp (const void *a, const void *b) 
{
	line_type first = *((line_type*)a);
	line_type second = *((line_type*)b);
	if(first.average < second.average)
	{
		return -1;
	}	
	else if(first.average > second.average)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void sort_by_average(line_type lines_array[], int line_count) 
{
	qsort(lines_array,line_count,sizeof(line_type),cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) 
{
    for(int i = 0;i < n_triplets;i++)
	{
		scanf("%i %i %i", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
	}
	return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) 
{
	triplet a = *((triplet*)t1);
	triplet b = *((triplet*)t2);
	if(a.r < b.r)
	{
		return -1;
	}
	else if(a.r > b.r)
	{
		return 1;
	}
	else
	{
		if(a.c < b.c)
		{
			return -1;
		}
		else if(a.c > b.c)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) 
{
	qsort(triplet_array,n_triplets,sizeof(triplet),cmp_triplets);
	memset(R, 0, (rows+1)*sizeof(int));
	for(int i = 0;i < n_triplets;i++)
	{
		V[i] = triplet_array[i].v;
		C[i] = triplet_array[i].c;
		for(int j = triplet_array[i].r + 1;j < rows + 1;j++)
		{
			R[j] += 1;
		}
	}
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) 
{
    for(int i = 0; i < rows; i++)
	{
        y[i] = 0;
        for(int j = R[i]; j <= R[i+1] - 1; j++)
		{
            y[i] += V[j]*x[C[j]];
        }
    }
}

void read_vector(int *v, int n) 
{
    for(int i = 0;i < n;i++)
    {
        scanf("%i",&v[i]);
    }
}

void write_vector(int *v, int n)
{
    for(int i = 0;i < n;i++)
    {
        printf("%i ",v[i]);
    }
	printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}


