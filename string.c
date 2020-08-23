#include "string1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


int l,m;
int ib1,ib2;

/*
*global variables l and m are used so that when result is obtained before terminating this function 
the position values inside both the tokenize functions should be set to 0*/

/*
*should compare the string length by ignoring blank spaces whenever required 
* or by ignoring special characters or
* by ignoring numbers or by ignoring nonprintable characters
*General Case Precedence: According to ascii values of characters whenever any special case is not specified
*/
int stringcomp(char *s,char *t,keyfield *k){
	int i=1,j=0;
	static int l1;
	char ch1[2]={0},ch2[2]={0};
	int x;
	float f1,f2;
	ch1[0]=tokenize1(s,k);
	ch2[0]=tokenize2(t,k);
	if(k->keydef && k->keystartindex){
			
			while(i<k->keystartindex){
				ch1[0]=tokenize1(s,k) ;
				ch2[0]=tokenize2(t,k) ;
				i++;
			}
	}
	//for nonprintable characters
	if(k->ignorenonprint){
			while((ch1[0]>0 && ch1[0]<=31) || ch1[0]==127){
				if((ch1[0]>0 && ch1[0]<=31) || ch1[0]==127){
					ch1[0]=tokenize1(s,k);
				}
			}
			while((ch2[0]>0 && ch2[0]<=31) || ch2[0]==127){
				if((ch2[0]>0 && ch2[0]<=31) || ch2[0]==127){
					ch2[0]=tokenize2(t,k);
				}
			}
			
				
	}
	
	//for numbers
	if(k->num){
		if(k->keydef && k->keystart){
			if(k->fieldsep ){
				i=getcolumn(s,k->keystart,k->fieldsep,k->ignoreblanks);
				j=getcolumn(t,k->keystart,k->fieldsep,k->ignoreblanks);
			}
			
			else{
				i=getcolumn(s,k->keystart,32,k->ignoreblanks);
				j=getcolumn(t,k->keystart,32,k->ignoreblanks);
			}
			
			if(isdigit(s[i]))
				f1=atof(&s[i]);
			else
				f1=0;
			
			if(isdigit(t[j]))
				f2=atof(&t[j]);
			else
				f2=0;
			
			if(f1>f2)
				return 1;
			if(f1<f2)
				return -1;
			
		}
		
		if(!k->keydef){
			f1=atof(s);
			f2=atof(t);
			if(f1>f2)
				return 1;
			if(f1<f2)
				return -1;
		}
	}
	//for non-numeric characters
	if(!k->num){
		while(ch1[0] && ch2[0]){
			//if both are not letters
			if((isalpha(ch1[0])==0 && isalpha(ch2[0])==0)){
				x=strcmp(ch1,ch2);
				if(x!=0){
					if(x>0){
						l=-1;
						m=-1;
						ch1[0]=tokenize1(s,k) ;
						ch2[0]=tokenize2(t,k) ;
						return 1;
					}
					else if(x<0){
						l=-1;
						m=-1;
						ch1[0]=tokenize1(s,k) ;
						ch2[0]=tokenize2(t,k) ;
						return -1;
					}
				}
			}
			
			else {
				x=(strcmp(ch1,ch2));
				if(x!=0){
					if(x>0){
						l=-1;
						m=-1;
						ch1[0]=tokenize1(s,k) ;
						ch2[0]=tokenize2(t,k) ;
						return 1;
					}
					if(x<0){
						l=-1;
						m=-1;
						ch1[0]=tokenize1(s,k) ;
						ch2[0]=tokenize2(t,k) ;
						return -1;
					}
				}
			}
			
			ch1[0]=tokenize1(s,k) ;
			ch2[0]=tokenize2(t,k) ;
			
			//for ignoring non printable characters
			if(k->ignorenonprint){
				while((ch1[0]>0 && ch1[0]<=31) || ch1[0]==127){
					if((ch1[0]>0 && ch1[0]<=31) || ch1[0]==127){
						ch1[0]=tokenize1(s,k);
					}
				}
				while((ch2[0]>0 && ch2[0]<=31) || ch2[0]==127){
					if((ch2[0]>0 && ch2[0]<=31) || ch2[0]==127){
						ch2[0]=tokenize2(t,k);
					}
				}
			}
			
		}
	}
	l=-1;
	m=-1;
	if(!k->num){
		if(ch1[0]==0 && ch2[0]){
			ch1[0]=tokenize1(s,k) ;
			ch2[0]=tokenize2(t,k) ;
			return -1;
		}
		if(ch1[0] && ch2[0]==0){
			ch1[0]=tokenize1(s,k) ;
			ch2[0]=tokenize2(t,k) ;	
			return 1;
		}
	}
	ch1[0]=tokenize1(s,k) ;
	ch2[0]=tokenize2(t,k) ;
	if(!k->keyend){
		if(strcmp(&s[ib1],&t[ib2])>0)
			return 1;
		if(strcmp(&s[ib1],&t[ib2])<0)
			return -1;
		if(strcmp(&s[ib1],&t[ib2])==0)
			return 0;
	}
	
	return 0;
}


char tokenize1(char *str,keyfield *k){
	static int i;
	static int j=0;
	static int flag=0;
	static int flag2=0;
	static int temp;
	static int temp2;
	static char c;
	int store=k->keyend - k->keystart;
	if(str[i]==0 || l==-1){
		i=0;
		j=0;
		l=0;
		flag=0;
		flag2=0;
		temp=0;
		temp2=0;
		return 0;
	}
	
	//applicable for normal as well as -k sorting
	if(i==0  && k->ignoreblanks){
		if(str[i]==32 || str[i]==9){
			while(str[i]==32 || str[i]==9)
				i++;
		}
		ib1=i;
	}
	if(k->keydef && flag==0){
		while(j!=(k->keystart)-1 && str[i]!=0){
			if(!k->fieldsep){
				if((str[i]==32 || str[i]==9) && str[i]!=0){
					j++;
				}
				
				if(j!=k->keystart-1){
					while((str[i]==32 || str[i]==9) && str[i]!=0){
						i++;
					}
				}
				
				if(k->ignoreblanks){
					if(str[i]==32  || str[i]==9){
						while(str[i]==32 || str[i]==9){
							i++;
					}	
					}
				}
				
			 }
			 
			else if(k->fieldsep){
				c=k->fieldsep;
				if((str[i]==c) && str[i]!=0){
					j++;
				}
				
				if(k->ignoreblanks){
					if(str[i]==32 || str[i]==9){
						while(str[i]==32 || str[i]==9){
							i++;
					}	
					}
				}
		    }
			if(j==k->keystart-1){
				flag=1;
				break;
			}
		    i++;
			
		}
		if(j==k->keystart-1){
				flag=1;
		}
		
	}
	
	if(((k->keydef && flag!=0) || !k->keydef) && str[i]!=0){
		while((ispunct(str[i]) && k->ignorepunct) && str[i]!='\0'){
			i++;
		}
		
		
		if(k->keydef){
			if(k->keyend){
			
				if(temp2>store){
						i=0;
						j=0;
						l=0;
						flag=0;
						flag2=0;
						temp=0;
						temp2=0;
						return 0;
						
				}
				if(k->fieldsep){
					if(str[i]!=k->fieldsep && str[i+1]==k->fieldsep){
						temp2++;
					}
				}
				
				
				if(!k->fieldsep){
					if(str[i]!=32 && str[i]!=9 && str[i+1]==32){
						temp2++;
					}
					
				}
			}
		}
			
		}
	
		if(str[i]!=0){
			if(k->ignorecase && isalpha(str[i]) && isupper(str[i])){  //i.e no diffrence between upper and lowercase
				str[i]=str[i]+32;
			}
				i++;
				
		
			return str[i-1];
		}
	
	
	return 0;
}

char tokenize2(char *str,keyfield *k){
	static int i;
	static int j=0;
	static int flag=0;
	static int flag2=0;
	static int temp;
	static int temp2;
	static char c;
	int store=k->keyend - k->keystart;
	if(str[i]==0 || m==-1){
		i=0;
		j=0;
		m=0;
		flag=0;
		flag2=0;
		temp=0;
		temp2=0;
		return 0;
	}
	
	//applicable for normal as well as -k sorting
	if(i==0  && k->ignoreblanks){
		if(str[i]==32 || str[i]==9){
			while(str[i]==32 || str[i]==9)
				i++;
		}
		ib2=i;
	}
	if(k->keydef && flag==0){
		while(j!=(k->keystart)-1 && str[i]!=0){
			if(!k->fieldsep){
				if((str[i]==32 || str[i]==9) && str[i]!=0){
					j++;
				}
				
				if(j!=k->keystart-1){
					while((str[i]==32 || str[i]==9) && str[i]!=0){
						i++;
					}
				}
				
				if(k->ignoreblanks){
					if(str[i]==32  || str[i]==9){
						while(str[i]==32 || str[i]==9){
							i++;
					}	
					}
				}
				
			 }
			 
			else if(k->fieldsep){
				c=k->fieldsep;
				if((str[i]==c) && str[i]!=0){
					j++;
				}
				
				if(k->ignoreblanks){
					if(str[i]==32 || str[i]==9){
						while(str[i]==32 || str[i]==9){
							i++;
					}	
					}
				}
		    }
			if(j==k->keystart-1){
				flag=1;
				break;
			}
		    i++;
			
		}
		if(j==k->keystart-1){
				flag=1;
		}
		
	}
	
	if(((k->keydef && flag!=0) || !k->keydef) && str[i]!=0){
		while((ispunct(str[i]) && k->ignorepunct) && str[i]!='\0'){
			i++;
		}
		
		
		if(k->keydef){
			if(k->keyend){
			
				if(temp2>store){
						i=0;
						j=0;
						m=0;
						flag=0;
						flag2=0;
						temp=0;
						temp2=0;
						return 0;
						
				}
				if(k->fieldsep){
					if(str[i]!=k->fieldsep && str[i+1]==k->fieldsep){
						temp2++;
					}
				}
				
				
				if(!k->fieldsep){
					if(str[i]!=32 && str[i]!=9 && str[i+1]==32){
						temp2++;
					}
					
				}
			}
		}
			
		}
	
		if(str[i]!=0){
			if(k->ignorecase && isalpha(str[i]) && isupper(str[i])){  //i.e no diffrence between upper and lowercase
				str[i]=str[i]+32;
			}
				i++;
				
		
			return str[i-1];
		}
	
	
	return 0;
}



//counts the number of columns in a particular string
//this function is used only during columnwise comparison of numeric sort
/*
	It gives the position of character from where given column starts 
*/
int getcolumn(char *str,int index,char fieldsep,int blanks){
	int j=0,i=0;
	if(blanks){
		if(str[i]==32 || str[i]==9){
			while(str[i]==32 || str[i]==9){
				i++;
			}
		}
	}
	while(j!=index-1 && str[i]!=0){
		if(str[i]==fieldsep && str[i]!=0){
			while(str[i]==fieldsep && str[i]!=0){
				i++;
			}
			
			if(blanks && fieldsep!=32){
				if(str[i]==32 || str[i]==9){
					while(str[i]==32 || str[i]==9){
						i++;
				}
				}
			}
			
				j++;
		}
		if(j==index-1){
			break;
		}
		i++;
	}
	return i;
}
