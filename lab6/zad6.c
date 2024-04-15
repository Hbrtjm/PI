#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair*, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair*, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair*, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair*, int);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(pair*, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair*, int);

int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);
int get_domain(pair*, int, int*);

// Case 3:

int composition (pair*, int, pair*, int, pair*);

// Comparator for pair
int cmp_pair (const void *a, const void *b) 
{
    pair *first_pair = (pair *) a;
    pair *second_pair = (pair *) b;
    if (*first_pair.first > *second_pair.first)
    {
        return first_pair;
    }
    else if(*first_pair.first < *second_pair.firs)
    {
        return second_pair;
    }
    else
    {
        if(*first_pair.second < *second_pair.second)
        {
            return first_pair; 
        }
        else if(*first_pair.second > *second_pair.second)
        {
            return second_pair; 
        }
    }
    return first_pair;
}

// Add pair to existing relation if not already there
int add_relation (pair *graph, int n, pair new_pair) {
    int flag = 0;
    // pair *new_graph = malloc(sizeof(pair) * (n+1));
    for(int i = 0;i < n;i++)
    {
        if(new_pair.first == graph[i].first && new_pair.second == graph[i].second)
        {
            return n;
        }
    }
    graph[n] = new_pair;
    return n+1;
}

// Read number of pairs, n, and then n pairs of ints
// Obviously unsafe
int read_relation(pair *relation) {
    int n;
    scanf("%d",n);
    for(int i = 0;i < n;i++)
    {
        scanf("%d%d",relation[i].first,relation[i].second);
    }
}


void print_int_array(int *array, int n) {
    printf("%d\n",n);
    for(int i = 0;i < n;i++)
    {
        printf("%d",array[i]);
    }
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

// This can be done in O(log(n)) if graph is sorted

int is_connected(pair *graph,int n)
{
    int flag = 1;
    int domain;
    int domain_size = get_domain(graph,n,domain);
    for(int i = 0;i < domain_size;i++)
    {
        for(int j = i;j < domain_size;j++)
        {
            for(int k = 0; k < n;k++)
            {
                if(graph[k].first == domain[i] && graph[k].second == domain[j] || graph[k].first == domain[j] && graph[k].second == domain[i])
                {
                    flag = 0;
                }
            }
        }
        if(flag)
        {
            return 0;
        }
    }
    return 1;
}

int get_domain(pair *graph,int n, int *domain)
{
}

int is_reflexive(pair *graph, int n)
{
    int flag = 1; 
    // qsort(graph,n,sizeof(pair),cmp);
    for(int i = 0;i < n;i++)
    {
        flag = 1;
        for(int j = 0;j < n;j++)
        {
            if(i != j)
            {
                if(graph[i].first == graph[j].second && graph[j].first == graph[j].second)
                {
                    flag = 0;
                }
            }
        }
        if(flag)
        {
            return 0;
        }
    }
    return 1;
}

int is_irreflexive(pair *graph, int n)
{
    int flag = 1; 
    // qsort(graph,n,sizeof(pair),cmp);
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            if(i != j)
            {
                if(graph[i].first == graph[j].second && graph[j].first == graph[j].second)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_symmetric(pair *graph,int n)
{
    int flag = 1;
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            if(graph[i].first == graph[j].second && graph[i].second == graph[j].first)
            {
                flag = 0;
            }
        }
        if(flag)
        {
            return 0;
        }
    }
    return 1;
}

int is_antisymmetric(pair *graph,int n)
{
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            if(graph[i].first == graph[j].second && graph[i].second == graph[j].first)
            {
                if(graph[i].first != graph[i].second)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int is_asymmetric(pair *graph,int n)
{
    int flag = 1;
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            if(graph[i].first == graph[j].second && graph[i].second == graph[j].first)
            {
                return 0;
            }
        }
    }
    return 1;
}

int is_transitive(pair *graph,int n)
{
    int flag = 1;
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            flag = 0;
            if(graph[i].second == graph[j].first)
            {
                flag = 1;
                for(int k = 0;k < n;k++)
                {
                    if(graph[i].first == graph[k].first && graph[j].second == graph[k].second)
                    {
                        flag = 0;
                    }
                }
            }
        }
        if(flag)
        {
            return 0;
        }
    }
    return 1;
}


int is_partial_order(pair *graph,int n)
{
    if(is_reflexive(graph,n) && is_transitive(graph,n) && is_antisymmetric(graph,n))
    {
        return 1;
    }
    return 0;
}

int is_total_order(pair *graph,int n)
{
    if(is_partial_order(graph,n) && is_connected(graph,n))
    {
        return 1;
    }
    return 0;
}

int find_max_elements(pair *graph,int n,int *max_elements)
{
    int max_elements_iterator = 0,flag;
    for(int i = 0;i < n;i++)
    {
        flag = 1;
        for(int j = 0;j < n;j++)
        {
            if(graph[i].second == graph[j].first)
            {
                flag = 0;
            }
        }
        if(flag)
        {
            max_elements[max_elements_iterator] = graph[i].second;
            max_elements_iterator++;
        }
    }
    return max_elements_iterator;
}

int find_min_elements(pair *graph,int n,int *min_elements)
{
    int min_elements_iterator = 0,flag;
    for(int i = 0;i < n;i++)
    {
        flag = 1;
        for(int j = 0;j < n;j++)
        {
            if(graph[i].first == graph[j].second)
            {
                flag = 0;
            }
        }
        if(flag)
        {
            min_elements[min_elements_iterator] = graph[i].second;
            min_elements_iterator++;
        }
    }
    return min_elements_iterator;
}

int find_domain(pair *graph, int n, int *domain)
{
    int m = 0, tab[MAX_RANGE] = 0;
    for(int i = 0;i < n;i++)
    {
        tab[domain[i].first]++;
        tab[domain[i].second]++;
    }
    for(int i = 0;i < MAX_RANGE;i++)
    {
        if(tab[i] > 0)
        {
            domain[m] = i;
            m++;
        }
    }
    return m;
}

int composition(pair *R, int n1, pair *S, int n2,pair *S_o_R)
{
    int compositioned = 0;
    for(int i = 0;i < n1;i++)
    {
        for(int j = 0;j < n2;j++)
        {
            if(R[i].first == S[j].second)
            {
                compositioned++;
            }
        }
    }
    return compositioned;
}

