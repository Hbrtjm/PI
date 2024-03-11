#include <stdio.h>

int rand_from_interval(int a, int b) {
        if(a==b)
                return a;
        if(b-a > RAND_MAX)
                return INT_MAX;
        if(a > b)
                return INT_MIN;
        return (rand()%(b-a+1)+a);
}

void swap(int *a,int *b)
{
        int temp = *a;
        *a = *b;
        *b = temp;
}

void rand_permutation(int n, int tab[]) {
        int k;
        for(int i = 0;i < n;i++)
                tab[i] = i;
        for(int i = 0;i < n-1;i++)
        {
                k = rand_from_interval(i,n-1);
                swap(&tab[i],&tab[k]);
        }
}

double cbuff_push(int cli_nr);
int cbuff_pop(void);
int cbuff_state(void);
void cbuff_print(void);

int main()
{
	int seed,version,max_conflicts;
	scanf("%d",seed);
	scanf("%d",version);
	scanf("%d",max_conflicts);
	switch(version)
	{
		case 0:
			
		case 1:
			
		default:
			printf("Podaj liczbę 0 lub 1!");	
	}
	return 0;
}

#define CBUFF_SIZE  10

int cbuff[CBUFF_SIZE];
int out=0, len=0;

double cbuff_push(int cli_nr)
{
        if(len == CBUFF_SIZE)
        {
                return INFINITY;
        }
        cbuff[(out+len)%CBUFF_SIZE] = cli_nr;
        len++;
        return 0.;
}

int cbuff_pop(void)
{
        if(len == 0)
                return -1;
        int value = cbuff[out];
        out = (out + 1) % CBUFF_SIZE;
        len--;
        return value;
}

int cbuff_state(void)
{
        return len;
}

void cbuff_print(void)
{
        printf("%d ",cbuff[out]);
        for(int i = out+1;i != (out+len)%CBUFF_SIZE;i=(i+1)%CBUFF_SIZE)
        {
                printf("%d ",cbuff[i]);
        }
}
