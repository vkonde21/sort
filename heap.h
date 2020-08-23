//this is a heap node
//#include "key.h"
#include "string1.h"
typedef struct heapnode {
	char *line;
	unsigned int fln;
}heapnode;


typedef struct heap{
	heapnode **arr;                  //arr is an array of heapnodes
	int count ;
}heap;

void hnodeinit(heapnode *h);
void  heapinsert(heap *h,char *line,unsigned int fln,keyfield *k);
void  revheapinsert(heap *h,char *line,unsigned int fln,keyfield *k);

void hinit(heap *h);
void heaprint(heap *h);
heapnode heapremov(heap *h,keyfield *k);
heapnode revheapremov(heap *h,keyfield *k);
void swapheap(heapnode **h1,heapnode **h2);
