#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include<bits/stdc++.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <fstream>

using namespace std;

struct linked_list{
      char p[1024];
      char n[256];
      off_t s;
      struct linked_list *next;
};
struct linked_list *head=NULL;
int  node_size=0;


off_t file_size(const char* path){
	struct stat filestatus;
 	stat(path,&filestatus);
  	return filestatus.st_size;
}

struct linked_list* createNode(const char *route,char *name,off_t size)
{
     struct linked_list*temp=(struct linked_list*)malloc(sizeof(linked_list));
     snprintf(temp->p, sizeof(temp->p), "%s", route);                                          
     snprintf(temp->n, sizeof(temp->n), "%s", name); 
     temp->s=size;
     return temp;
}


struct linked_list* enter(const char *route,char *name,off_t size){
	struct linked_list *pre=head;
	struct linked_list *temp=head->next;
	if((head->s)>=size){
		temp=createNode(route,name,size);	
		temp->next=head;
		return temp;
	}	
        while(temp!=NULL && temp->s<size){
		pre=temp;
		temp=temp->next;
	}
        struct linked_list* temp1=createNode(route,name,size);
        pre->next=temp1;
        temp1->next=temp;
        return head;
}

struct linked_list* insert(const char *route,char *name,off_t size)
{
	if((node_size)==0)
  	{
		(node_size)++;
		return createNode(route,name,size);
  	}
  	else if((node_size)<10){
 		(node_size)++;
		return enter(route,name,size);
  	}
	if((head->s)<size){
		struct linked_list *temp=head->next;
                free(head);
                head=temp;
		return enter(route,name,size);
	}
	return head;
}


void nodeDirectory(const char *name)
{
    	DIR *dir;
    	struct dirent *entry;
    	if (!(dir = opendir(name))){
		cout << "NO SUCH PATH EXISTS!!!" << endl;
        	return ;
	}
    	char path[1024];
    	while ((entry = readdir(dir)) != NULL) {
        	if (entry->d_type == DT_DIR) {
            		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                		continue;
            	snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            	nodeDirectory(path);
        	} 
		else {
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
			off_t x=file_size(path);
	    		head=insert(name,entry->d_name,x);
        	}
    }
    closedir(dir);
}

void copy_cut_file(char* file_source,char* file_destination){
	int n;
	char buff[1024];
	int s_fd=open(file_source,O_RDONLY,S_IRWXU);
    	int d_fd=open(file_destination,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU);
	
    	while ((n=read(s_fd,buff,1024))>0)
    	{
        	write(d_fd,buff,n);
    	}
	close(s_fd);
	remove(file_source);
	close(d_fd);
}





void move_file(char *path){
	char d[1024];
	char f[1024];
	string s;
	s=path;
	string ext=s.substr(s.find_last_of('.')+1);
	char c[ext.size()+1];
	strcpy(c, ext.c_str());
	c[ext.size()]='\0';
	snprintf(d, sizeof(d),"/home/fayaz/Documents/%s",c);
	string n=s.substr(s.find_last_of('/')+1);
	char b[n.size()+1];
	strcpy(b, n.c_str());
	b[n.size()]='\0';
	snprintf(f, sizeof(f),"/home/fayaz/Documents/%s/%s",c,b);
	cout << d << endl;
	cout << f << endl << endl;
	if(b!="desktop"){
		mkdir(d, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		copy_cut_file(path,f);
	}
}


void nodeDirectory_2(char *name)
{
    	DIR *dir;
    	struct dirent *entry;
    	if (!(dir = opendir(name))){
        	return ;
	}
    
    	while ((entry = readdir(dir)) != NULL) {
		char path[1024];
        	if (entry->d_type == DT_DIR) {
            		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                		continue;
            	snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            	nodeDirectory_2(path);
        	} 
		else {
			snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
	    		move_file(path);
        	}
        }
    closedir(dir);
}




int main(void) {
	int n;
        char main_p[1024];
	char file_source[1024];
	char file_destination[1024];
	cout << "CASE-1.Display the top 10 files according to size in the respective directory\n";
	cout << "CASE-2.Cut files from desktop and create folders according to the file type in documents and paste there\n";
	cout << "Choose option 1 Or 2::";
	cin >> n;
	if(n==1){ 
		cout << "Enter the path ::";
        	cin >>main_p;
    		nodeDirectory(main_p);
		struct linked_list *temp=head;
    		while(temp!=NULL){
			cout << temp->p << endl << temp->n << endl << setprecision(8) << (double)((temp->s)/(1024*1024))/* temp->s*/ << "MB" << endl << endl;
			temp=temp->next;
		}
	}
	else if(n==2){
		snprintf(file_source, sizeof(file_source), "/home/fayaz/Desktop/Graphics/hello");
		nodeDirectory_2(file_source);
	}
    	return 0;
}
