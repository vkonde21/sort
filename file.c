#include "file.h"
#include "heap.h"
#define _GNU_SOURCE          //for fcloseall function
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<ctype.h>
#define lines 200000        //runsize

int g;
int q;
int lo;
char **a;
char **b;
FILE **p;

unsigned int readlines(FILE *fp);
void sort(int low, int high,keyfield *k);
void merging(int low,int mod,int high,keyfield *k);
void mergefiles(unsigned int nofiles,keyfield *k); //for sorting files in ascending order
void revmergefiles(unsigned int n,keyfield *k);   //for sorting in reverse order
void writelines(FILE *,int);
void removeduplicates(keyfield *k);
void help();

int main(int argc , char *argv[]){
	unsigned int x,i,j,l,l1;
	char ch,*fh;
	FILE *fp,*fp2;
	keyfield k;
	keyinit(&k);
	if(argc<2){
		printf("Bad Arguments.Enter ./sort --help to see more");
		exit(1);
	}
	
	j=1;
	while(argv[j][0]=='-'){
		for(l1=1;l1<strlen(argv[j]);l1++){
			switch(argv[j][l1]){
					case 't':
						k.fieldsep=argv[j][2];
						while(argv[j][l1])
							l1++;
						break;
					case 'k':
						if(isdigit(argv[j][l1+1]))
							k.keystart=atoi(&argv[j][l1+1]);
						else{
							printf("Sort: Invalid number at field start: Invalid count at start of '%c'\n", argv[j][l1+1]);						
							exit(1);
						}
						if(k.keystart>=1){
							k.keydef=1;
						}
						l=l1;
						while(ch!=',' && argv[j][l]!=0){
							ch=argv[j][l];
							if(ch!=',' && isdigit(ch)==0){
								printf("Sort: Invalid number at field start: Invalid count at start of '%c'\n", argv[j][l1+1]);						
								exit(1);
							}
								
							l++;
							l1=l;
						}
						k.keyend=atoi(&argv[j][l]);
						l1=l1-1;
						if(k.keystart>k.keyend && k.keyend!=0){
							k.keydef=0;
						}
						
						while(l1<=strlen(argv[j]))
							l1++;
						break;
						
						case 'n':
							k.num=1;
							k.ignorenonprint=0;          //test this line by entering non printable characters
							if(argv[j][2]=='k'){
								l1++;
								k.keydef=1;
								k.keystart=atoi(&argv[j][3]);  //-nk5
								l1=l1+strlen(&argv[j][3]);
							}
							
							if(!k.keystart){
								k.keystart=1;
								k.keydef=1;
							}
							
							if(k.ignorenonprint==1){
								printf("Please Enter proper option '-ni' is invalid");
								exit(1);
							}
						break;
						
						case 'b':
							k.ignoreblanks=1;
							break;
						case 'r':
							k.reverse=1;
							break;
						case 'd':
							k.keydef=0;
							k.ignorepunct=1;
							k.ignorenonprint=1;
							break;
						case 'i':
							k.ignorenonprint=1;
							if(k.num==1){
								printf("Enter valid option '-ni' is invalid");
								exit(1);
							}
							break;
							
						case 'm':
							k.merge=1;
							break;
						case 'h':
								help();
								exit(1);
						case 'u':
							k.unique=1;
							break;
						default:
							printf("Enter a valid option\n");
							exit(1);
							break;
				}
				
		}
		
		j++;
	}
	
	if(!k.merge){
		if(argv[argc-1])
			fp=fopen(argv[argc-1],"r");
		else{
			printf("Enter a file");
			exit(1);
		}
		if(fp==NULL){
			printf("No such file exists");
			exit(1);
		}
	}
	i=0;
	p=(FILE**)malloc(sizeof(FILE*));
	fh=(char*)malloc(10);
	if(!p || !fh){
				exit(1);
	}
	if(!k.merge){
		do{
			g=0;
			while(g!=-1){
				x=readlines(fp);
			}
			sort(0,x,&k);
			if(i>9){
				fh=realloc(fh,9+i+1);
			}
			sprintf(fh,"%d",i+1);
			
			p[i]=fopen(strcat(fh,".txt"),"w+");
			if(!p[i]){
				printf("Could not open file");
				exit(1);
			}
			writelines(p[i],x);
			p=realloc(p,sizeof(FILE*) *(i+2));
			if(!p){
				exit(1);
			}
			i++;
			
		}while(q!=-1);
	}
	
	else{
		for(i=j;i<argc;i++){
			p[i-j]=fopen(argv[i],"r");
			if(!p[i-j]){
				printf("No such file");
				exit(1);
			}
			p=realloc(p,sizeof(FILE*) *(i-j +2));
			for (ch = getc(p[i-j]); ch != EOF; ch = getc(p[i-j])) 
        		if (ch == '\n') 									// Increment count if this character is newline 
            		lo = lo + 1; 
		}
		i=i-2;
	}
	if(!k.reverse)
		mergefiles(i,&k);
	else
		revmergefiles(i,&k);
		
	fcloseall();
	if(!k.merge){
		for(j=0;j<=i+1;j++){
			sprintf(fh,"%d",j);
			if(remove(strcat(fh,".txt"))==0){
				;
			}
		}
	}
	
	free(p);
	return 0;
	
}

/*
*readline function reads the data byte by byte until '\n' or eof character is encountered.
*It also counts the number of lines and runsize.
*Once runsize is finished it sets g=-1
*If the whole file is finished reading then it sets q=-1
*/
unsigned int readlines(FILE *fp){
	static unsigned int i;                //for number of lines
	unsigned int temp;
	unsigned int j=0;      		  		  //for number of characters of each line
	static int line ;
	unsigned int x;	
	char ch;
	g=0;			         
	if(i==0 ){                           //for first time
		a=(char**)malloc(sizeof(char*) * 1024);
		b=(char**)malloc(sizeof(char*) * 1024);
		if(!a || !b){
			printf("Memory Full 1");
			exit(1);
		}
	}
	if(i>=1024){
		a=(char**)realloc(a,sizeof(char*)*(i+1));
		b=(char**)realloc(b,sizeof(char*)*(i+1));
		if(!a || !b){
			printf("Memory Full 2");
			exit(1);
		}
	}
	
	
	a[i]=(char*)malloc(1024);
	b[i]=(char*)malloc(1024);
	x=fread(&ch,1,1,fp);
	while(x!=0 && ch!='\n'){
		a[i][j]=ch;
		x=fread(&ch,1,1,fp);
		if(j>=1024){
			a[i]=realloc(a[i],sizeof(char)*(j+1));
			b[i]=realloc(b[i],sizeof(char)*(j+1));
			if(!a[i] || !b[i]){
				printf("Memory Full 3");
				exit(1);
			}
			
		}
		j++;
	}
	
	a[i][j]=0;
	if(x!=0){
		
		i++;
		lo++;
	}
	line++;
	
	temp=i;
	if(x==0 || line==lines){
		g=-1;
		i=0;
		line=0;
		if(x==0){
			q=-1;
		}
	}
	return temp-1;
}

/*writelines writes the sorted data in the given file*/
void writelines(FILE *fp,int x){
	int i=0;
	char ch='\n';
	for(i=0;i<=x;i++){
		fwrite(a[i],1,strlen(a[i]),fp);
		fwrite(&ch,1,1,fp);
		free(a[i]);
		free(b[i]);
	}
	free(a);
	free(b);
	
}

//merge sort
void sort(int low, int high,keyfield *k) {
	int mid;

	if(low < high) {
		mid = (low + high) / 2;
		sort(low, mid,k);
		sort(mid + 1, high,k);
		merging(low, mid, high,k);
	}
	else
		return;
}

void merging(int low,int mid,int high,keyfield *f){
	int i = low, j = mid+1, k = 0;
	while(i <= mid && j <= high) {
		
		if(!f->reverse){
			if(stringcomp(a[i],a[j],f)==1) {
				strcpy(b[k],a[j]);
				k += 1;
				j += 1;
			}
			else {
				strcpy(b[k],a[i]);
				k += 1; 
				i += 1;
			}
		}
		
		else{
			if(stringcomp(a[i],a[j],f)==-1) {
				strcpy(b[k],a[j]);
				k += 1;
				j += 1;
			}
			else {
				strcpy(b[k],a[i]);
				k += 1; 
				i += 1;
			}
		}
	}

	// add elements left in the first interval 
	while(i <= mid) {
		strcpy(b[k],a[i]);
		k += 1; i += 1;
	}

	// add elements left in the second interval 
	while(j <= high) {
		strcpy(b[k],a[j]);
		k += 1; j += 1;
	}

	// copy temp to original interval
	for(i = low; i <= high; i += 1) {
		strcpy(a[i],b[i-low]);
	}
}

/*
*read the first line from all (n) files and then create a minheap.
*from this minheap write the root element in the output file and then rearrange the heap.
*Also note the file number from which the least element was removed.
*Next time it inserts the next line from this file.
*/
void mergefiles(unsigned int n,keyfield *k){
	
    unsigned int i=0,ln,fln,j=0;
    int x,flag=0,flag2=0,f;
    char *ch,c;
    ch=(char*)malloc(n+1);
    FILE *fp2;
    heap h;
    heapnode h1;
    hinit(&h);
    fp2=fopen("output.txt","w");
    if(!fp2){
    	printf("could not open file");
    	exit(1);
    }
    i=0;
   for(i=0;i<n;i++){
    	fseek(p[i],0,SEEK_SET);
    }
    
    a=(char**)malloc(sizeof(char*));
	a[0]=(char*)malloc(255);
	if(flag==0){
		 for(i=0;i<n;i++){
		 		j=0;
		 		x=fread(&c,1,1,p[i]);
		 		while(x!=0 && c!='\n'){
		 			a[0][j]=c;
		 			j++;
		 			x=fread(&c,1,1,p[i]);
		 		}
		 		if(x!=0){
		 			a[0][j]='\n';
		 			a[0][j+1]=0;
		 			heapinsert(&h,a[0],i+1,k);
		 		}
		 }
		 flag=1;
	}
	 
	 
	h1=heapremov(&h,k);
	if(!k->unique)
		printf("%s",h1.line);
	if(k->unique)
		fwrite(h1.line,1,strlen(h1.line),fp2);
	free(h1.line);
	fln=h1.fln;
	
	if(flag!=0){
		 	do{
		 		j=0;
			 			x=fread(&c,1,1,p[fln-1]);
						while(x!=0 && c!='\n'){
						 		a[0][j]=c;
						 		j++;
						 		x=fread(&c,1,1,p[fln-1]);
						}
						
						if(x!=0){
							a[0][j]='\n';
			 		   	    a[0][j+1]=0;
				 			heapinsert(&h,a[0],fln,k);
				 		}
			 		
				 		if(feof(p[fln-1])){
				 			flag2++;
				 		}
				
			 		if(h.count){
			 			h1=heapremov(&h,k);
			 			if(!k->unique)
			 				printf("%s",h1.line);
			 			if(k->unique)
			 				fwrite(h1.line,1,strlen(h1.line),fp2);
			 			free(h1.line);
						fln=h1.fln;
				 		
				 	}
				 	
			 	
			 }while(flag2<=lo-2);
	
		 
	}
	fclose(fp2);
	if(k->unique){
		removeduplicates(k);
		
	}
	free(ch);
	free(a[0]);
	free(a);
	remove("output.txt");
	 
}

/*
	for -u option : This function removes the duplicate elements
*/
void removeduplicates(keyfield *k){
	int fd,j;
	FILE *fp;
	char c;
	fp=fopen("output.txt","r");
	if(!fp){
		printf("Could not open file");
		exit(1);
	}
	a=realloc(a,sizeof(char*)*2);
	a[1]=(char*)malloc(255);
	c=getc(fp);
	j=0;           															
	while(c!='\n'){
		a[0][j]=c;
		j++;
		c=getc(fp);
	}
	a[0][j]=0;
	j=0;
	while(!feof(fp)){
		j=0;
		c=getc(fp);
		
		while(c!='\n' && !feof(fp)){
			a[1][j]=c;
			j++;
			c=getc(fp);
		}
		a[1][j]=0;

		if(stringcomp(a[0],a[1],k)!=0){
			printf("%s\n",a[0]);
		}
		
		strcpy(a[0],a[1]);
	}
	
	free(a[1]);
	remove("output.txt");
}


//reversing a file
/*same as mergefiles function except that it uses a max heap instead of minheap*/
void revmergefiles(unsigned int n,keyfield *k){
	unsigned int i=0,ln,fln,j=0;
    int x,flag=0,flag2=0,f;
    char ch[n+1],c;
    FILE *fp2;
    heap h;
    heapnode h1;
    hinit(&h);
    fp2=fopen("output.txt","w");
    if(!fp2){
    	printf("could not open file");
    	exit(1);
    }
    i=0;
   for(i=0;i<n;i++){
    	fseek(p[i],0,SEEK_SET);
    }
    
    a=(char**)malloc(sizeof(char*));
	a[0]=(char*)malloc(255);
	if(flag==0){
		 for(i=0;i<n;i++){
		 		j=0;
		 		x=fread(&c,1,1,p[i]);
		 		while(x!=0 && c!='\n'){
		 			a[0][j]=c;
		 			j++;
		 			x=fread(&c,1,1,p[i]);
		 		}
		 		if(x!=0){
		 			a[0][j]='\n';
		 			a[0][j+1]=0;
		 			revheapinsert(&h,a[0],i+1,k);
		 		}
		
		 }
		 flag=1;
	}
	 
	 
	h1=revheapremov(&h,k);
	if(!k->unique)
		printf("%s",h1.line);
	if(k->unique)
		fwrite(h1.line,1,strlen(h1.line),fp2);
	free(h1.line);
	fln=h1.fln;
	if(flag!=0){
		 	do{
		 		j=0;
			 			x=fread(&c,1,1,p[fln-1]);
						while(x!=0 && c!='\n'){
						 		a[0][j]=c;
						 		j++;
						 		x=fread(&c,1,1,p[fln-1]);
						}
						
						if(x!=0){
							a[0][j]='\n';
			 		   	    a[0][j+1]=0;
				 			revheapinsert(&h,a[0],fln,k);
				 		}
			 		
				 		if(feof(p[fln-1])){
				 			flag2++;
				 		}
				
			 		if(h.count){
			 			h1=revheapremov(&h,k);
			 			if(!k->unique)
			 				printf("%s",h1.line);
			 			if(k->unique)
			 				fwrite(h1.line,1,strlen(h1.line),fp2);
			 			free(h1.line);
						fln=h1.fln;
				 	}
				 	
			 	
			 }while(flag2<=lo-2);
		
	}
	fclose(fp2);
	if(k->unique){
		removeduplicates(k);
		
	}
	free(a[0]);
	free(a);
	remove("output.txt");
}


void help() {
	printf("Usage: ./sort [OPTION]... [FILE]...\n");
	printf("Write sorted concatenation of all FILE(s) to standard output.\n");
	printf("Ordering Options:\n");
	printf("%7s, ignore leading blanks\tIgnore leading blanks\n", "-b");
	printf("%7s, numersic sort\t\tCompare according to string numeric value\n", "-n");
	printf("%7s, reverse\t\tReverse the result of comparisions\n", "-r");
	printf("%7s, dictionary order\t\tArrange in a dictionary order\n", "-d");
	printf("Other Options: \n");
	printf("%7s, key=KEYDEF\t\tSort via a key. KEYDEF gives location\n", "-k");
	printf("%7s, merge\t\t\tMerge already sorted files; Do not sort.\n", "-m");
	printf("%7s, unique\t\t\tRemove duplicates from result\n", "-u");
	printf("%7s, ignore-nonprint\t\t\tignore the nonprintable characters while sorting\n", "-i");	
	printf("%7s,--help\t\t\tPrint this usage and exit\n", "-h");
	exit(0);
}
