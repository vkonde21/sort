typedef struct keyfield {
	int reverse;
	int keydef;
	int keystart;
	int keystartindex;  //eg. 3.4 then start index=4
	int keyend;
	int keyendindex;
	
	char fieldsep;
	int unique;
	int num;
	
	int ignoreblanks;
	int ignorepunct;
	int ignorecase;
	int ignorenonprint;
	
	char **p;
	int nfiles;
	char *output_file;
	
	int merge;
	int index;
    int dict;
	int check;
	
}keyfield;

void keyinit(keyfield *k);
