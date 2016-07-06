#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "FancyHeap.h"
unsigned char explaining;
char *filename;
void explanation(void)
{
	printf("Written in Spring 2016 by Wesley Shiflet\n\nMaintains a time complexity of O(n lg n) by utilizing two linked lists:  a queue of empty leaves and a stack of processed & marked nodes.\nEach insertion consists of exactly one dequeue and push (except the first, which just pushes), two enqueues and an assignment to the dequeued node-\nall constant-time operations.\n");
	printf("After these steps, the down-up heapify operation (here called 'bubble') performs a maximum of lg n comparisons and swaps.\nThis means each insertion costs a maximum of T(n)=4+2lg(n) steps or a minimum of T(n)=4+lg(n) steps, making insertion a theta(log n) operation.\nSince the insertion operation would be executed on n items, the process of iterating over a list effectively multiplies this step by n\ncoming out to the desired time complexity of O(n log n).\n");
	printf("\nAll auxillary data structures are implemented via the heaphead structure, which also points to the root of the heap.\nThis method of pointer-based heap construction was compared against a seemingly less memory-intensive alternative,\nwhich determined the position of the next leaf by interpreting the current logical size of the heap.\nAlthough this led to extremely simple, rapid traversal every other insertion based on a bitwise comparison, see for example:\n     if !(size && 1), next=last->p->r,    where last is the previously inserted node\nthe fact is when (size && 1), both the path between root and next and that between last and next grew continuously more complex,\nwhich betrayed the original intention of constant access.\n");
	printf("\nFor the following test cases of size n, the integers 1 through n were used for the test; a sorted input provides worst-case behavior for insertion,\n but best-case behavior for extraction, while reverse-sorted input provides the opposite.\nTimes are given in the cycles returned by clock() because these all run in less than a second:\n");
	printf("# of items      Sorted:  Insert -   Sort    Reverse:  Insert -   Sort\n");
	printf("      1                    ~0cc -   ~0cc                ~0cc -   ~0cc\n");
	printf("     75                    ~0cc -   ~0cc                ~0cc -   ~0cc\n");
	printf("   1000                    ~1cc -   ~1cc                ~1cc -   ~1cc\n");
	printf("   2000                    ~1cc -   ~1cc                ~1cc -   ~1cc\n");
	printf("   5000                     2cc -    2cc                 3cc -    3cc\n");
	printf("  20000                     8cc -    8cc                 8cc -    8cc\n");
	printf("  60000                    37cc -   37cc                24cc -   28cc\n");
	printf(" 200000                   121cc -  121cc                83cc -   87cc\n");
	printf(" 500000                   306cc -  306cc               204cc -  220cc\n");
	printf("1000000                   642cc -  642cc               411cc -  413cc\n");
	printf("\nAs one can see, this version of heapsort runs extremely quickly- within a single clock() cycle for lists up to 2000 items!\nFor sorted lists, experimental T(n) most easily corresponds to (6.353^-4)n - .48856, a linear model.\nFor reverse-sorted lists, experimental T(n) most easily corresponds to the also linear (4.346^-4)n + .535868.\nFrom this we can assume that from a practical standpoint, the linear elements of the program contribute more to runtime than the logarithmic ones.");
}
int processOptions(int argc, char **argv)
{
    int argIndex,mode;
    argIndex = 1;
    mode = 0;
    while (argIndex < argc)
        {
        if (*argv[argIndex] == '-')
        {
        	if (argv[argIndex][1] == '\0') return mode;
        	switch (argv[argIndex][1])
        	{
            	case 'd':
                	mode = 1;
                	break;
            	case 'v':
                	explanation();
                	exit(0);
                	break;
            	default:
                	printf("option %s not understood\n",argv[argIndex]);
            }
        }
        else if (strlen(argv[argIndex])>2) filename = argv[argIndex];
        ++argIndex;
        }
    return mode;
}
int main(int argc, char **argv) 
{
	heaphead * h = (heaphead *)malloc(sizeof(heaphead));
	FILE * infile;
	int a = 0;
	h->realhead=NULL;
	if (argc==1)
	{
		printf("Not enough arguments!\n");
		exit(-1);
	}
	h->abnormality = processOptions(argc,argv);
	infile = fopen(filename,"r");
	do
    {
    	fscanf(infile,"%d",&a);
    	insert(h,a);	
    }while (!feof (infile));
    fclose(infile);
	free(infile);
	while (h->realhead!=NULL) printf("%d ",extract(h));
	free(h);
	return 0;
}
