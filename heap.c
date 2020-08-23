#include "heap.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void hnodeinit(heapnode *h){
	h->line=NULL;
	h->fln=0;
}

//initialises the heap
void hinit(heap *h){
	h->arr=NULL;
	h->count=0;
}

/*
*inserts a line in heap acc. to stringcomp function
*/
void  heapinsert(heap *h,char *line,unsigned int fln,keyfield *k){
	heapnode *hn;
	int temp,i,c;
	hn=(heapnode*)malloc(sizeof(heapnode));
	if(!hn)
		exit(1);
	hn->line=(char*)malloc(strlen(line)+1);
	strcpy(hn->line,line);
	hn->fln=fln;
	
	
	if(h->arr==NULL || h->count==0){
		h->arr=(heapnode**)malloc(sizeof(heapnode*));
		h->arr[0]=hn;
		h->count++;
	}
	
	else{
		h->arr=realloc(h->arr,sizeof(heapnode*)*(h->count+1));
		if(!h->arr)
			exit(1);
		h->arr[h->count]=hn;
		c=h->count;
		i=(c-1)/2;
		while(i>=0 && c>=0){
			if((stringcomp((h->arr[i])->line,(h->arr[c])->line,k)==1)){
				swapheap(&(h->arr[i]),&(h->arr[c]));
				c=i;
				i=(c-1)/2;
			}
			else
				break;
				
			
		}
		h->count++;
	}
	
	
}

/* swaps heaps*/
void swapheap(heapnode **h1,heapnode **h2){
	heapnode h;
	h=**h1;
	**h1=**h2;
	**h2=h;
}

/*removes the smallest element from the heap*/
heapnode heapremov(heap *h,keyfield *z){
	int i,j,k,temp,c;
	char *temp2;
	static heapnode f;
	hnodeinit(&f);
	if(h->arr){
		f.line=(char*)malloc(strlen(h->arr[0]->line)+1);
		f.fln=h->arr[0]->fln;
		if(!f.line){
			printf("Memory full");
			exit(1);
		}
		strcpy(f.line,h->arr[0]->line);
		free(h->arr[0]->line);
		if(h->count>=1){
			h->arr[0]->line=h->arr[h->count-1]->line;
			h->arr[0]->fln=h->arr[h->count-1]->fln;
			i=0;                   //smallest
			h->count--;
			c=h->count-1;
			j = 2 * i + 1;        //left child
			k = 2 * i + 2;        //right child
			while(j<=c){
				temp=i;           //temp stores smallest element
					
				if(j<=c && stringcomp(h->arr[temp]->line,h->arr[j]->line,z)==1){
					temp=j;
				}
				
				if(k<=c && stringcomp(h->arr[temp]->line,h->arr[k]->line,z)==1)
					temp=k;
					
					
				if(temp!=i){
					swapheap(&h->arr[i],&h->arr[temp]);
					i=temp;
					j = 2 * i + 1;
					k = 2 * i + 2;
					
				}
				
				else
					break;
					
					
				}  //end of while loop
		
			
		}
		
		else
			h->count--;
	}
	
	return f;
}

void heaprint(heap *h) {
	int i;
	for(i = 0; i < h->count; i++)
		printf("%s\t%u\n", h->arr[i]->line,h->arr[i]->fln);
	printf("\n");
}



//for max heap
void  revheapinsert(heap *h,char *line,unsigned int fln,keyfield *k){
	heapnode *hn;
	int temp,i,c;
	hn=(heapnode*)malloc(sizeof(heapnode));
	if(!hn)
		exit(1);
	hn->line=(char*)malloc(strlen(line)+1);
	strcpy(hn->line,line);
	hn->fln=fln;
	
	if(h->arr==NULL || h->count==0){
		h->arr=(heapnode**)malloc(sizeof(heapnode*));
		h->arr[0]=hn;
		h->count++;
	}
	
	else{
		h->arr=realloc(h->arr,sizeof(heapnode*)*(h->count+1));
		if(!h->arr)
			exit(1);
		h->arr[h->count]=hn;
		c=h->count;
		i=(c-1)/2;
		while(i>=0 && c>=0){
			if((stringcomp((h->arr[i])->line,(h->arr[c])->line,k)==-1)){
				swapheap(&(h->arr[i]),&(h->arr[c]));
				c=i;
				i=(c-1)/2;
			}
			else
				break;
				
			
		}
		h->count++;
	}
}

heapnode revheapremov(heap *h,keyfield *z){
	int i,j,k,temp,c;
	char *temp2;
	static heapnode f;
	hnodeinit(&f);
	if(h->arr){
		f.line=(char*)malloc(strlen(h->arr[0]->line)+1);
		f.fln=h->arr[0]->fln;
		if(!f.line)
			exit(1);
		strcpy(f.line,h->arr[0]->line);
		free(h->arr[0]->line);
		if(h->count>=1){
			h->arr[0]->line=h->arr[h->count-1]->line;
			h->arr[0]->fln=h->arr[h->count-1]->fln;
			i=0;                   //largest
			h->count--;
			c=h->count-1;
			j = 2 * i + 1;        //left child
			k = 2 * i + 2;        //right child
			while(j<=c){
				temp=i;           //temp stores largest element index
					
				if(j<=c && stringcomp(h->arr[temp]->line,h->arr[j]->line,z)==-1){
					temp=j;
				}
				
				if(k<=c && stringcomp(h->arr[temp]->line,h->arr[k]->line,z)==-1)
					temp=k;
					
					
				if(temp!=i){
					swapheap(&h->arr[i],&h->arr[temp]);
					i=temp;
					j = 2 * i + 1;
					k = 2 * i + 2;
					
				}
				
				else
					break;
					
					
				}  //end of while loop
		
			
		}
		else
			h->count--;
	}
	
	return f;
}
