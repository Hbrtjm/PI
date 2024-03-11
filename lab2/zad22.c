#include <stdio.h>
#include <math.h>

#define TEST    1        // 1 dla testowania, 0 dla oceny automatycznej

// Deklaracje (prototypy) funkcji:

double stack_push(double x);
double stack_pop(void);
int stack_state(void);

double queue_push(int how_many);
int queue_pop(int how_many);
int queue_state(void);
void queue_print(void);

double cbuff_push(int cli_nr);
int cbuff_pop(void);
int cbuff_state(void);
void cbuff_print(void);

///////////////////////

int main(void) {
    int to_do, i, n, ans;
    int nr_klienta, kod_operacji;
    double x,f_ans;
    if(TEST) printf("Nr testu = ");
    scanf("%d",&to_do);
    switch(to_do) {
case 1:
        if(TEST) printf("sekwencja operacji (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan stosu i konczy sekwencje operacji
        do {
            scanf("%lf",&x);
            if(x > 0) {
                f_ans = stack_push(x);
                if(f_ans == INFINITY) printf("%f ",INFINITY);
            }
            else {
                if(x < 0) printf("%.2f ",stack_pop());
                else      printf("\n%d\n",stack_state());
            }
        } while(x != 0.0);
        break;
case 2:
//  kolejka kolejnych liczb naturlnych > 0 (naplywajacych grupowo) - implementacja w tablicy z przesunieciami
        if(TEST) printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
        do {
            scanf("%d",&n);
            if(n > 0) {
                f_ans = queue_push(n);
                if(f_ans== INFINITY) printf("%f ",INFINITY);
            }
            else {
                if(n < 0) {
                    ans = queue_pop(-n);
                    if(ans < 0) printf("-1 ");
                }
                else  {
                    printf("\n%d\n",queue_state());
                    queue_print();
                }
            }
        } while(n != 0);
        break;
case 3:
//  kolejka kolejnych liczb naturalnych > 0 (naplywajacych pojedynczo - implementacja w buforze cyklicznym
        if(TEST) printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
        nr_klienta = 0;
        do {
            scanf("%d",&kod_operacji);
            if(kod_operacji > 0) {
                f_ans = cbuff_push(++nr_klienta);
                if(f_ans == INFINITY) printf("%f ",INFINITY);
            }
            else {
                if(kod_operacji < 0) {
                    printf("%d ",cbuff_pop());
//                    ans = cbuff_pop();
//                    if(ans < 0) printf("%f ",NAN);
//                    else        printf("%d ",ans);
                }
                else  {
                    printf("\n%d\n",cbuff_state());
                    cbuff_print();
                }
            }
        } while(kod_operacji != 0);
        break;
default:
        if(TEST) printf("NOTHING TO DO!\n");
    }
    return 0;
}

///////////////////////////////////////////////////////

// Definicje funkcji i zmiennych o zasięgu plikowym (file scope):

// Stos

#define STACK_SIZE  10

double stack[STACK_SIZE];
int top=0;   // jawne zerowanie nie jest konieczne

double stack_push(double x) {
	if(top == STACK_SIZE)
		return INFINITY;
	stack[top] = x;
	top++;
	return 0;
}

double stack_pop(void) {
	if(top == 0)
		return NAN;
	double val = stack[top-1];
	stack[top-1] = 0;
	top = top-- < 0 ? 0 : top;
	return val;
}

int stack_state(void) {
	return top;
}

//Kolejka z przesuwaniem w tablicy

#define QUEUE_SIZE  10

int queue[QUEUE_SIZE];
int in=0, curr_nr=0;  // 1. klient dostanie nr = 1

double queue_push(int how_many) 
{
	for(int i = 0; i < how_many;i++)
	{
		if(i + in >= QUEUE_SIZE)
		{
			in = QUEUE_SIZE;
			curr_nr += how_many-i;
			return INFINITY;
		}
		curr_nr++;
		queue[i+in] = curr_nr;
	}
	in+=how_many;
	return 0.;
}

int queue_pop(int how_many) 
{
	if(how_many > QUEUE_SIZE)
	{
		for(int i = 0;i < QUEUE_SIZE;i++)
		{
			queue[i] = 0;
		}
		in = 0;
		return -1;		
	}
	int temp = in - how_many;
	for(int i = 0;i < how_many;i++)
	{
		if(in == 0)
		{
			return -1;
		}
		queue[i] = 0;
		in--;
	}
	for(int i = 0;i < QUEUE_SIZE;i++)
	{
		if(how_many+i >= QUEUE_SIZE)
			break;
		queue[i] = queue[how_many+i];
	}
	return in;
}

int queue_state(void) 
{
	return in;
}

void queue_print(void) 
{
	for(int i = 0;i < in;i++)
		printf("%d ",queue[i]);
}


//Kolejka w buforze cyklicznym

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
