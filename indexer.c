#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <string.h>


//**********************************************************************************************************************************//


int collect_title(char *,int,char *,int);
int get_heading(char *,int,int);
char*	getword(char	*,int	,int	);
void	findwords(char	*,char	**,int*);
void create_list_of_word(char **,int);
int	word_count(char	*,int	,char	**,int	);
char*	geturl(int	,int    ,char*);
void print_complete_list();
void create_hash();
void initialize_hash();
int	generate_key(char*);
void print_hash();
void create_cluster();
void save_to_file();


//************************************************************************************************************************************//

struct word
{
char name[50];
struct word *next;
struct document *doc;
};


struct word *head=NULL;
struct word *current=NULL;

//************************************************************************************************************************************//

struct document
{
int id;
char url[100];
int count;

struct document *down;
};

//struct word *d_head=NULL;

struct	hashtable
{
struct	word *node;
};

struct hashtable hash[256];
struct hashtable *hash_head;



//***************************************************************************************************************************************//
void create_list()
{

struct word *start;
start=head;
while(start!=NULL)
{

//puts(start->name);
int k=1;
struct document *d_current=(struct document *)malloc(sizeof(struct document));
d_current=NULL;
//printf("\n%d\n",k);
//open file
while(k<8)
{

char file[1000]={0};
char num[10];
strcat(file,"/home/muskan/Desktop/sefiles/");
sprintf(num,"%d",k);
strcat(file,num);
strcat(file,".txt");
//puts(file);

char	ch;
int	i=0;
int	file_size;
struct	stat	st;
stat(file,&st);
file_size=st.st_size;

//printf("\n%d\n",file_size);

char	*data=(char *)malloc((file_size+1)*sizeof(char));
FILE	*temp;

temp=fopen(file,"r");

while(!feof(temp))
{

ch=fgetc(temp);

data[i]=ch;
i++;
}
data[i]='\0';
fclose(temp);

char **words=(char**)malloc(5000);
int size=0;



findwords(data,words,&size);


int s=0;
s=word_count(start->name,0,words,size);
//printf("%d.txt  %s %d\n",k,start->name,s);
char *url;
url=(char*)malloc(1000);
if(s>0)
{
url=geturl(k,file_size,data);
//puts(url);


struct document *d_temp=(struct document *)malloc(sizeof(struct document));
d_temp->id=k;
strcpy(d_temp->url,url);
d_temp->count=s;
d_temp->down=NULL;
//puts(d_temp->url);
//printf("%d %d\n",d_temp->id,d_temp->count);



if(start->doc==NULL)
{

start->doc=d_temp;




d_current=d_temp;



}


else
{

d_current->down=d_temp;
d_current=d_current->down;

}

}

k++;
}
start=start->next;
}

//end of fxn

}

//***************************************************************************************************************************************//

int	word_count(char	*word,int	index,char	**words,int	size)
{

int	count=0;

for(int	i=index;i<size;i++)
{

if(word=="" ||  words[i]=="")
continue;
else if(strcmp(word,words[i])==0)
{
count++;
words[i]="";
}
}
return	count;
}

//**************************************************************************************************************************************//

int	generate_key(char	*word)
{
int count=0,i;
count=word[0];

return count;
}
//*****************************************************************************************************************************************//



char*	geturl(int	i,int file_size,char *data)
{

int	m=collect_title(data,file_size,"l",0);
m+=2;
int	n=collect_title(data,file_size,"/l",m);
char	url[100];
int	size=0;
for(int	j=m;j<n-1;j++,size++)
url[size]=data[j];
url[size]='\0';
//puts(url);
char	*result=(char *)malloc(1000);
strcpy(result,url);
return	result;
}

//***************************************************************************************************************************************//
void create_list_of_word(char **titlewords,int size)
{


int i;

for(i=0;i<size;i++)
{
if(strlen(titlewords[i])==0)
continue;
struct word *temp=(struct word*)malloc(sizeof(struct word));
strcpy(temp->name,titlewords[i]);
temp->doc=NULL;

if(head==NULL)
{
temp->next=NULL;
head=temp;
current=temp;
}
else
{
struct word *dummy;
dummy=head;
int flag=0;
while(dummy!=NULL)
{
if(strcmp(dummy->name,temp->name)==0)
{
flag=1;
break;
}
dummy=dummy->next;

}

if(flag==1)
{

}

else
{
current->next=temp;
temp->next=NULL;
current=current->next;
}


}

}

}

//************************************************************************************************************************************//
void print_list()
{

struct word *temp=head;

while(temp!=NULL)
{
int length;
length=strlen(temp->name);
printf("%s \n",temp->name);
temp=temp->next;


}


}

//*************************************************************************************************************************************//
void openfile()
{
char file[1000]={0};
char num[20];
static int k=1;
strcat(file,"/home/muskan/Desktop/sefiles/");
sprintf(num,"%d",k);
strcat(file,num);
strcat(file,".txt");

//puts(file);

//collect data
char	ch;
int	i=0;
int	file_size;
struct	stat	st;
stat(file,&st);
file_size=st.st_size;

//printf("%d",file_size);

char	*data=(char *)malloc((file_size+1)*sizeof(char));
FILE	*temp;
temp=fopen(file,"r");
while(!feof(temp))
{
ch=fgetc(temp);
data[i]=ch;
i++;
}
data[i]='\0';
fclose(temp);


//collect title
int pos=0;

int in=collect_title(data,file_size,"title",pos);
in+=6;
int end=collect_title(data,file_size,"/title",in);
char title[100];
int j=0;
for(in;in<end-1;in++,j++)
title[j]=data[in];
title[j]='\0';
//puts(title);
//printf("\n");


char	**titlewords;
titlewords=(char	**)malloc(100);
int	size=0;
findwords(title,titlewords,&size);


create_list_of_word(titlewords,size);

k++;
}
//************************************************************************************************************************************//
char*	getword(char	*string,int	a,int	b)
{
//strlwr(string);

char	*word=(char*)malloc(100);
if(b-a>99||b-a==1)
{
word[0]='\0';
return word;
}
int j=0;
//printf("%c\n",str[a]);



for(int	i=a;i<b;i++,j++)
{
//if(string[i]>=65&&string[i]<=122)
//string[i]+=32;

if(string[i]=='<')
{
i++;
while(string[i]!='>'&&i<b)
i++;
continue;
}




else if((string[i]>=65&&string[i]<=90)||(string[i]>=97&&string[i]<=122))
word[j]=string[i];
else
break;
}
word[j]='\0';
if(word[0]==' ')
word[0]='\0';

for(int j=0;j<strlen(word);j++)
if(word[j]>=65&&word[j]<=90)
word[j]+=32;


//puts(word);
return	word;
}
//************************************************************************************************************************************//
int get_heading(char *html,int file_size,int pos)
{

int i,j;
for(pos;html[pos]!='>';pos++)
{
}
pos++;
if(html[pos]!='<')
return pos;
else
return -1;

}
//************************************************************************************************************************************//

void print_complete_list()
{

struct word *start;
start=head;
while(start!=NULL)
{
printf("\ntitle=%s \n",start->name);

struct document *strt;
strt=start->doc;

while(strt!=NULL)
{

printf("page=%d \t",strt->id);
printf("url=%s \t",strt->url);
printf("count=%d \n",strt->count);
 

strt=strt->down;
}
start=start->next;
}


//end of fxn;
}


//***********************************************************************************************************************************//

void	findwords(char	*str,char	**words,int	*s)
{

int	flag=0,a=-1,b=-1,size=0,i;
char	**temp;
temp=(char**)malloc(1000);
for(i=0;i<strlen(str);i++)
{
if(str[i]==' ')
{
if(flag==1)
{
b=i;
}
flag=0;
}
else
{
if(flag==0)
{
a=i;
}
flag=1;
}
if(a!=-1	&&	b!=-1)
{
words[*s+size]=getword(str,a,b);
//temp[size]=getword(str,a,b);
size++;
a=-1;
b=-1;
}
}
if(a!=-1)
{
words[*s+size]=getword(str,a,i);
//temp[size]=getword(str,a,i);
size++;
}
*s=*s+size;
//removeduplicates(temp,words,s,size);
}



//***************************************************************************************************************************************//
int collect_title(char *html,int file_size,char word[],int pos)
{

//file_size=strlen(html);
int len=strlen(word);
char c;

int i,j,flag;

for(pos;pos<file_size;pos++)
{
c=html[pos];
if(c=='<')
{
flag=0;
for(j=0,pos=pos+1;j<len;j++,pos++)
{
if(html[pos]!=word[j])
flag=1;
}
if(flag==0)
return pos-j;


}


}

return -1;
}
//********************************************************************************************************************************************//

void initialize_hash()
{
int i;
for(i=0;i<256;i++)
hash[i].node=NULL;

}
//********************************************************************************************************************************************//

void create_hash()
{
initialize_hash();
while(head!=NULL)
{

int key=generate_key(head->name);

if(hash[key].node==NULL)
{

hash[key].node=head;
head=head->next;
(hash[key].node)->next=NULL;

}


else
{

struct word *current;
current=hash[key].node;
while(current->next!=NULL)
{

current=current->next;


}

current->next=head;
head=head->next;
(current->next)->next=NULL;


}

//end of while
}
//print_hash();
//end of fxn
}

//******************************************************************************************************************************************//

void create_cluster()
{

struct word *h;
struct word *current;
int i;

hash_head=(struct hashtable*)malloc(sizeof(struct hashtable));
for(i=0;i<256;i++)
{
if(hash[i].node!=NULL)
{
hash_head->node=hash[i].node;
struct word *h1;
h1=hash[i].node;
while(h1->next!=NULL)
{

h1=h1->next;

}
current=h1;
break;
}



}


for(i=i+1;i<256;i++)
{
if(hash[i].node!=NULL)
{
current->next=hash[i].node;
h=hash[i].node;

while(h->next!=NULL)
h=h->next;

current=h;

}

}

}

//******************************************************************************************************************************************//
void print_hash()
{

int i;
struct word *temp;

temp=hash_head->node;

while(temp!=NULL)
{

printf("%s\n",temp->name);
temp=temp->next;
}



//end of fxn
}

//*****************************************************************************************************************************************//


void save_to_file()
{

char path[]="/home/muskan/Desktop/indexer.txt";

struct word *start;
start=hash_head->node;

FILE *fp;
fp=fopen(path,"w");

int k=1;

if(fp==NULL)
printf("file not found");

while(start!=NULL)
{
fprintf(fp,"%d",k);
fprintf(fp,"\t");
fprintf(fp,"%s",start->name);
fprintf(fp,"\n");

struct document *d;

d=start->doc;

while(d!=NULL)
{

fprintf(fp,"%s",d->url);
fprintf(fp,"\tcount=");
fprintf(fp,"%d",d->count);
fprintf(fp,"\n");


d=d->down;
}

fprintf(fp,"\n");

start=start->next;
k++;

}





fclose(fp);


}

//********************************************************************************************************************//
int main(char argc,char* argv[])
{
int i=1;
while(i<10)
{openfile();

i++;
}


//print_list();

create_list();

//print_complete_list();

create_hash();

create_cluster();

print_hash();

save_to_file();

return 0;

}
