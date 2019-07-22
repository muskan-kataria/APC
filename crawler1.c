#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <string.h>

#define	Baseurl "https://www.chitkara.edu.in"
#define urllength 1000
#define	pathlength	300



struct	Node
{
struct Node *next;
char *url;
int	depth;
int	isvisited;
int	key;
};
struct	Node	*head=NULL;
struct	Node	*tail=NULL;



struct Hash
{
struct	Node	*first;
struct	Node	*last;
int count;
}hash[50];


//***********************************************************************************************************************************//

void initialise()
{
        static int c=0;
        if(c)
        return;
        c=1;
        int i;
        for(i=0;i<50;i++)
        hash[i].first=NULL;
        hash[i].last=NULL;
        hash[i].count=0;
}

//************************************************************************************************************************************//


void removeWhiteSpace(char* html) 
{
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++) 
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer); free(p);
}

//*************************************************************************************************************************************//


int GetNextURL(char* html, char* urlofthispage, char* result, int pos) 
{
  char c;
  int len, i, j;
  char* p1;  //!< pointer pointed to the start of a new-founded URL.
  char* p2;  //!< pointer pointed to the end of a new-founded URL.

  // NEW
  // Clean up \n chars
  if(pos == 0) {
    removeWhiteSpace(html);
  }
  // /NEW

  // Find the <a> <A> HTML tag.
  while (0 != (c = html[pos])) 
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  //! Find the URL it the HTML tag. They usually look like <a href="www.abc.com">
  //! We try to find the quote mark in order to find the URL inside the quote mark.
  if (c) 
  {  
    // check for equals first... some HTML tags don't have quotes...or use single quotes instead
    p1 = strchr(&(html[pos+1]), '=');
    
    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10)) 
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;    

    p2 = strpbrk(p1, "\'\">");
    if (!p2) 
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#') 
    { // Why bother returning anything here....recursively keep going...

      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4)) 
    {
      //! Nice! The URL we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
      if (p1[0] == '.') {
        //! Some URLs are like <a href="../../../a.txt"> I cannot handle this. 
	// again...probably good to recursively keep going..
	// NEW
        
        return GetNextURL(html,urlofthispage,result,pos+1);
	// /NEW
      }
      if (p1[0] == '/') {
        //! this means the URL is the absolute path
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);        
      } else {
        //! the URL is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.abc.com/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthis page is like http://www.abc.com/~xyz
          //! or http://www.abc.com
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }    
  return -1;
}

//*********************************************************************************************************************************//


void NormalizeWord(char* word) {
  int i = 0;
  while (word[i]) {
      // NEW
    if (word[i] < 91 && word[i] > 64) // Bounded below so this funct. can run on all urls
      // /NEW
      word[i] += 32;
    i++;
  }
}


//***********************************************************************************************************************************//


int NormalizeURL(char* URL) 
{
  int len = strlen(URL);
  if (len <= 1 )
    return 0;
  //! Normalize all URLs.
  if (URL[len - 1] == '/') 
  {
    URL[len - 1] = 0;
    len--;
  }
  int i, j;
  len = strlen(URL);
  //! Safe check.
  if (len < 2)
    return 0;
  //! Locate the URL's suffix.
  for (i = len - 1; i >= 0; i--)
    if (URL[i] =='.')
      break;
  for (j = len - 1; j >= 0; j--)
    if (URL[j] =='/')
      break;
  //! We ignore other file types.
  //! So if a URL link is to a file that are not in the file type of the following
  //! one of four, then we will discard this URL, and it will not be in the URL list.
  if ((j >= 7) && (i > j) && ((i + 2) < len)) 
  {
    if ((!strncmp((URL + i), ".htm", 4))
        ||(!strncmp((URL + i), ".HTM", 4))
        ||(!strncmp((URL + i), ".php", 4))
        ||(!strncmp((URL + i), ".jsp", 4))
        ) 
    {
      len = len; // do nothing.
    } 
    else 
    {
      return 0; // bad type
    }
  }
  return 1;
}


//******************************************************************************************************************************************//

//int checkurl1(char *seedurl);

void	DataTransfer(char	*ptr,char	*url,char	*depth);
void print_list(struct Node *);

void	PathCreated(char	*url,char	*path,char	*depth)
{

printf("\npath %s\n",path);

//printf("\n in path created fxn\n");
char	string_file[20];
char	*p1=(char	*)malloc(20);
static	int	file_no=0;

//file counter
FILE	*File_No;
File_No=fopen("FileNOs.txt","wb+");
fscanf(File_No,"%s",p1);
sscanf(p1,"%d",&file_no);
file_no++;
fclose(File_No);
File_No=fopen("FileNOs.txt","wb+");
fprintf(File_No,"%d",file_no);
fclose(File_No);

//path creation

sprintf(string_file,"%d",file_no);
//file_no++;
char	p[pathlength+100]={0},ptr[pathlength+100]={0};
strcat(p,"touch ");
strcat(p,"/home/muskan/Downloads/APC/");
strcat(ptr,"/home/muskan/Downloads/APC/");
strcat(p,path);
strcat(ptr,path);
strcat(p,"/temp");
strcat(ptr,"/temp");
strcat(p,string_file);
strcat(ptr,string_file);
strcat(p,".txt");
strcat(ptr,".txt");
puts(p);
puts(ptr);
system(p);
printf("Path Created\n"); 
puts(p);
puts(ptr);
//printf("\n%d\n",file_no);

//file transfer
DataTransfer(ptr,url,depth);
}

void	collectlinks(char	*data,int	file_size,char	*url);
//void filling_links(char *links[100]);

void	DataTransfer(char	*ptr,char	*url,char	*depth)
{
char	ch;
int	i=0;
int	file_size;
struct	stat	st;
stat("/home/muskan/Downloads/APC/C/temp.txt",&st);
file_size=st.st_size;

char	*data=(char *)malloc((file_size+1)*sizeof(char));
FILE	*temp;
temp=fopen("/home/muskan/Downloads/APC/C/temp.txt","r");
while(!feof(temp))
{
ch=fgetc(temp);
data[i]=ch;
i++;
}
data[i]='\0';

FILE	*new;
new=fopen(ptr,"wb+");
fprintf(new,"%s",url);
fprintf(new,"%c",'\n');
fprintf(new,"%s",depth);
fprintf(new,"%s",data);
fclose(new);
fclose(temp);

printf("\n now time for collect links\n");
collectlinks(data,file_size,url);
}


//***************************************************************************************************************************************//

int	generatekey(char	*url)
{
int	key=0;
for(int	i=0;i<strlen(url);i++)
key+=url[i];

while(key>50)
{
key/=10;
}
return	key;
}





//*************************************************************************************************************************//

int checkurl1(char *);
int checkurl(char *);
void save_links_in_file(struct Node*);

void put_links_in_list(char **,int ,int );



void	collectlinks(char	*data,int	file_size,char	*url)
{
static int depth=1;
char	*resulturl[10];
for(int	i=0;i<10;i++)
resulturl[i]=(char	*)malloc(10000);
int	pos=0,a=0;
char	*result=(char	*)malloc(10000);

file_size=strlen(data);

//printf("%d\n",file_size);

while(pos<file_size)
{


//int	i=0;
//pos=GetNextURL(data,url,result,pos);

if(pos==-1)
break;

else
{
int i=0;
pos=GetNextURL(data,url,result,pos);
int	res=checkurl(result);
int	duplicate=0;

for(i=0;i<a;i++)
if(strcmp(resulturl[i],result)==0)
duplicate=1;

if(res==1 && duplicate==0 )
{
strcpy(resulturl[a],result);
printf("\n %d %s\n",a,resulturl[a]);
a++;
}
if(a==10)
break;


}
}

free(data);
//dept++;
put_links_in_list(resulturl,a,depth);
depth++;


}

//***********************************************************************************************************************************//





//*********************************************************************************************************************************//






//*******************************************************************************************************************************//


int checkdepth(char *depth)
{
if(depth[0]>=49 && depth[0]<=53 && depth[1]=='\0')
return	1;
else
return 0;
}


//******************************************************************************************************************************//

int checkurl1(char *seedurl)
{

for(int	i=0;Baseurl[i]!='\0';i++)
	if(seedurl[i]!=Baseurl[i])
		return	0;
return	1;
}


//******************************************************************************************************************************//

int checkurl(char *seedurl)
{

for(int	i=0;Baseurl[i]!='\0';i++)
	if(seedurl[i]!=Baseurl[i])
		return	0;

char *result=(char	*)malloc(1000);
strcat(result,"wget --spider ");
strcat(result,seedurl);
if(!system(result))
return	1;
else
return	0;
}

//******************************************************************************************************************************//


int checktarget(char *target)
{
struct	stat	statbuf;
if(stat(target,&statbuf)==-1)
return	0;
if(!S_ISDIR(statbuf.st_mode))
return	0;
if((statbuf.st_mode & S_IWUSR)!=S_IWUSR)
return	0;
return	1;
}

void getpage(char *url)
{
printf("\n url=%s \n",url);
char urlbuffer[urllength+300]={0};
strcat(urlbuffer,"wget ");
strcat(urlbuffer,url);
strcat(urlbuffer," -O ~/Downloads/APC/C/temp.txt");
system(urlbuffer);
}

//******************************************************************************************************************************//


void save_links_in_file(struct Node* head)
{
        FILE *ptr=fopen("/home/muskan/Desktop/links.txt","wb+");
        struct Node *temp=head;
        char div[100]={0};
        while(temp!=NULL)
        {
        /*
                strcat(div,temp->Link_url);
                strcat(div,","); 
                strcat(div,(temp->Link_key));
                strcat(div,",");    
                */
                fprintf(ptr,"%s ",temp->url);
                fprintf(ptr,"%d ",temp->key);
                fprintf(ptr,"%d ",temp->depth);
                fprintf(ptr,"%d\n",temp->isvisited);  
                temp=temp->next;
        }
fclose(ptr);
        
}


//*****************************************************************************************************************************//

void put_links_in_list(char **result,int l,int depth)
{
       
printf("\n its time for put link in list\n");

 initialise();

printf("after initialzing\n");
        static int c=1;
        int i;
        struct Node *temp,*trav;
        for(i=0;i<l;i++)
        {
                temp=(struct Node*)calloc(1,sizeof(struct Node));
                temp->key=generatekey(result[i]);
                int key=temp->key;
            temp->url=result[i];
printf("\nurl\n");
puts(temp->url);
                temp->depth=depth;
                if(head==NULL)
                {     
                  
                  
                        temp->next=NULL;
                        temp->isvisited=0;
                        head=temp;                     
                        tail=temp;
                        hash[key].first=temp;
                        hash[key].last=temp;
                        hash[key].count=1;
                        continue;
                }
                else
                {
                        if(hash[key].first==NULL)
                        {
                    
                               tail->next=temp;
                               temp->next=NULL;
                               temp->isvisited=0;
                               tail=temp;
                               hash[key].first=temp;
                               hash[key].last=temp;
                               hash[key].count=1;
                        }
                        else
                        {
                            trav=hash[key].first;
                            if((strcmp(hash[key].first->url,temp->url)==0)||(strcmp(hash[key].last->url,temp->url)==0))
                                {
                                    free(temp);
                                    continue;
                                }
                            int f=0;
                            while(trav->next!=NULL&&trav->next->key==key)
                            {
                                if(strcmp(trav->url,temp->url)==0)
                                {
                                    free(temp);
                                    f=1;
                                    break;
                                }
                                trav=trav->next;
                            }
                            if(f)
                            continue;
                            if(trav->next==NULL)
                            tail=temp;
                      
                            temp->next=trav->next;
                            temp->isvisited=0;
                            trav->next=temp;
                            hash[key].last=temp;
                            hash[key].count++;
                        }
                }
        }


//free(trav);
//free(result);
  save_links_in_file(head);
         print_list(head);

}



//******************************************************************************************************************************//
void print_list(struct Node *head)
{
    struct Node *temp=head;
int i=1;
    while(temp!=NULL)
    {
        printf("%d key = %d Link = %s depth = %d visit=%d\n",i,temp->key,temp->url,temp->depth,temp->isvisited);
i++;
        temp=temp->next;
    }
free(temp);
}





//*****************************************************************************************************************************//

int main(int argc,char *argv[])
{


char	*seedurl=argv[1];
char	*depth=argv[2];
char	*target=argv[3];



if(argc!=4)
{
printf("Insufficient parameters\n");
return 0;
}
else{


if(checkdepth(depth)==0)
printf("Depth invalid\n");

else if(checkurl(seedurl)==0)
printf("URL invalid\n");

else if(checktarget(target)==0)
printf("Target Directory invalid\n");

else{
printf("Valid\n");
getpage(seedurl);



		
PathCreated(seedurl,target,depth);


}
}

struct Node *temp=(struct Node*)calloc(1,sizeof(struct Node));
temp=head->next;
head=NULL;

while(temp!=NULL)
{

printf("\nin while\n seed url");
seedurl=temp->url;
puts(seedurl);
if(checkurl(seedurl)==1)
{
printf("\n in if ");
getpage(seedurl);
		printf("\n after page get\n");

//target=argv[3];
//puts(target);
PathCreated(seedurl,target,depth);
}

temp->isvisited=1;


temp=temp->next;
}

//free(temp);





return	0;
}















