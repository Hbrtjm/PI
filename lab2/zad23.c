#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#define CARD_AMOUNT 52

int rand_from_interval(int a, int b) {
        if(a==b)
                return a;
        if(b-a > 2147483647) // RAND_MAX required
                return 2147483647; // INT_MAX required
        if(a > b)
                return -2147483648; // INT_MIN required
        return (rand()%(b-a+1)+a);
}

void swap(int *a,int *b)
{
        int temp = *a;
        *a = *b;
        *b = temp;
}

void rand_permutation(int n, int tab[],int seed) {
        int k;
        srand(seed);
        for(int i = 0;i < n;i++)
                tab[i] = i;
        for(int i = 0;i < n-1;i++)
        {
                k = rand_from_interval(i,n-1);
                swap(&tab[i],&tab[k]);
        }
}

typedef struct Deck
{
        int len;
        int out;
        int array[CARD_AMOUNT];
} Deck;

double queue_push(struct Deck *deck,int cli_nr);
int queue_pop(struct Deck *deck);
int queue_state(struct Deck *deck);
void queue_print(struct Deck *deck);

int main()
{
	int seed,version,war=0,max_conflicts,cardA,cardB,conflicts=0,conflict_size=0;
        Deck deckA, deckB,tableA,tableB;
        deckA.len=0;
        deckA.out=0;
        deckB.len=0;
        deckB.out=0;
        tableA.len=0;
        tableA.out=0;
        tableB.len=0;
        tableB.out=0;
	scanf("%d %d %d",&seed,&version,&max_conflicts);
	int deck[52];
        for(int i = 0;i < CARD_AMOUNT;i++)
        {
                deck[i] = i;
        }
        rand_permutation(CARD_AMOUNT,deck,seed);
        for(int i = 0;i < CARD_AMOUNT/2;i++)
        {
                queue_push(&deckA,deck[i]);
                queue_push(&deckB,deck[i+CARD_AMOUNT/2]);
        }
         queue_print(&deckA);
         printf("\n");
         queue_print(&deckB);
         printf("\n");
        switch(version)
	{
		case 1:
                        while(conflicts < max_conflicts)
                        {
                                if(queue_state(&deckA)==0)
                                {
                                        printf("3\n");
                                        // printf("%d",conflicts);
                                        queue_print(&deckB);
                                        return 0;
                                }
                                else if (queue_state(&deckB)==0)
                                {
                                        printf("2 %d",conflicts);
                                        return 0;
                                }
                                cardA = queue_pop(&deckA);
                                cardB = queue_pop(&deckB);
                                if(cardA>>2 > cardB>>2)
                                {
                                        queue_push(&deckA,cardA);
                                        queue_push(&deckA,cardB);
                                }
                                else if (cardA>>2 < cardB>>2)
                                {
                                        queue_push(&deckB,cardB);
                                        queue_push(&deckB,cardA);
                                }
                                else
                                {
                                        queue_push(&deckA,cardA);
                                        queue_push(&deckB,cardB);
                                }
                                conflicts++;
                        }
                        break;
		case 0:
                        while(conflicts < max_conflicts && queue_state(&deckA)!=0 && queue_state(&deckB)!=0)
                        {
                                 printf("\n%d : %d\n",cardA,cardB);
                                 queue_print(&deckB);
                                 printf("\n");
                                 queue_print(&deckA);
                                if(!war)
                                {
                                        if(queue_state(&deckA)==0 && queue_state(&tableA)==0)
                                        {
                                                printf("3\n");
                                                queue_print(&deckB);
                                                return 0;
                                        }
                                        else if (queue_state(&deckB)==0&&queue_state(&tableB)==0)
                                        {
                                                printf("2 %d",conflicts);
                                                return 0;
                                        }
                                        cardA = queue_pop(&deckA);
                                        cardB = queue_pop(&deckB);
                                        queue_push(&tableA,cardA);
                                        queue_push(&tableB,cardB);
                                        if(cardA>>2 > cardB>>2)
                                        {
                                                while(queue_state(&tableA)!=0)
                                                        queue_push(&deckA,queue_pop(&tableA));
                                                while(queue_state(&tableB)!=0)
                                                        queue_push(&deckA,queue_pop(&tableB));
                                                war = 0;
                                                conflicts++;
                                        }
                                        else if (cardA>>2 < cardB>>2)
                                        {
                                                while(queue_state(&tableB)!=0)
                                                        queue_push(&deckB,queue_pop(&tableB));
                                                while(queue_state(&tableA)!=0)
                                                        queue_push(&deckB,queue_pop(&tableA));
                                                war = 0;
                                                conflicts++;
                                        }
                                        else
                                        {
                                                conflicts++;
                                                war = 1;
                                        }
                                }
                                else
                                {
                                        queue_push(&tableA,queue_pop(&deckA));
                                        queue_push(&tableB,queue_pop(&deckB));
                                        war=0;
                                }
                        }
                        break;
		default:
			printf("Podaj liczbę 0 lub 1!");
                        break;
        }
        while(queue_state(&tableB)!=0)
                queue_push(&deckB,queue_pop(&tableB));
        while(queue_state(&tableA)!=0)
                queue_push(&deckA,queue_pop(&tableA));
        if(queue_state(&deckA)==0 && queue_state(&tableA)==0)
        {
                printf("3\n");
                queue_print(&deckB);
                return 0;
        }
        else if (queue_state(&deckB)==0&&queue_state(&tableB)==0)
        {
                printf("2 %d",conflicts);
                return 0;
        }
        if(conflicts==max_conflicts)
                printf("0 %d %d",queue_state(&deckA),queue_state(&deckB));
	else
                printf("1 %d %d",queue_state(&deckA),queue_state(&deckB));
        return 0;
}

#define CBUFF_SIZE  CARD_AMOUNT

int deckA[CBUFF_SIZE],deckB[CBUFF_SIZE],conflict_deck[CARD_AMOUNT];
int outA=0, lenA=0,outB=0,lenB=0;

double queue_push(struct Deck *deck,int cli_nr)
{
        if(deck->len == CBUFF_SIZE)
        {
                return -1.;
        }
        deck->array[(deck->len+deck->out)%CBUFF_SIZE] = cli_nr;
        deck->len++;
        return 0.;
}

int queue_pop(struct Deck *deck)
{
        if(deck->len == 0)
                return -1;
        int value = deck->array[deck->out];
        deck->out = (deck->out + 1) % CBUFF_SIZE;
        deck->len--;
        return value;
}

int queue_state(struct Deck *deck)
{
        return deck->len;
}

void queue_print(struct Deck *deck)
{
        printf("%d ",deck->array[ deck->out ]);
        if(deck->len>1)
        for(int i = deck->out+1;i != (deck->out+deck->len)%CBUFF_SIZE;i=(i+1)%CBUFF_SIZE)
        {
                printf("%d ",deck->array[ i ]);
        }
}


