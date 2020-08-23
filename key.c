#include "key.h"
#include<stdio.h>

//initializes the structure 
void keyinit(keyfield *k) {
	k->reverse = 0;
	k->keydef = 0;
	k->keystart = 0;
	k->keyend=0;
	k->keystartindex = 0;
	k->keyendindex=0;
	k->unique = 0;
	k->num = 0;
	k->ignoreblanks = 0;
	k->ignorecase = 0;
	k->nfiles = 0;
	k->merge = 0;
	k->index = 1;
	k->check = 0;
	k->fieldsep=0;
	k->ignorepunct=0;
	k->ignorenonprint=0;
	k->dict=0;
}

