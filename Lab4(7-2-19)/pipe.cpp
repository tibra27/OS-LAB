#include<bits/stdc++.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
using namespace std;
int main()
{
    int pipe1[2];
    int pipe2[2];
    int pipe3[2];

    pid_t x;

    if(pipe(pipe1)==-1)
    {
        fprintf(stderr,"PIPE FAILED...");
        return 1;
    }
    if(pipe(pipe2)==-1)
    {
        fprintf(stderr,"PIPE FAILED...");
        return 1;
    }
    if(pipe(pipe3)==-1)
    {
        fprintf(stderr,"PIPE FAILED...");
        return 1;
    }

    char file_path[100];
    cout<<"ENTER FILE PATH::";
    cin>>file_path;
    FILE *f=fopen(file_path,"r");
    char data[1000];
    fgets(data,1000,f);

    x=fork();
    if(x<0)
    {
        fprintf(stderr,"FORK FAILED.......");
        return 1;
    }
    else if(x>0)
    {
        cout<<"process 1 (pid= "<<getpid()<<" ) write into pipe1... "<<endl<<data<<endl;
        write(pipe1[1],data,strlen(data)+1);
        wait(NULL);
        int words;
        read(pipe3[0],&words,4);
        cout<<"process 1 (pid= "<<getpid()<<" ) read from pipe3... "<<endl<<words<<endl;
        cout<<"Total no. of words="<<words<<endl;
    }
    else
    {
        char data1[1000];
        read(pipe1[0],data1,1000);
        cout<<"process 2 (pid= "<<getpid()<<" ) raed from pipe1... "<<endl<<data1<<endl;
        int len=strlen(data1);
        char data2[1000];
        int i=0,j=0;
        while(i<len)
        {
            while((data1[i]==' ' || data1[i]=='\t') && i<len)
            {
                i++;
                continue;
            }
            while(data1[i]!=' ' && data1[i]!='\t' && i<len)
            {
                data2[j]=data1[i];
                j++;
                i++;
            }
            data2[j]=' ';
            j++;
        }
        data2[j]='\0';
        len=j;
        j=0;
        i=0;
        char data3[1000];
        while(i<len)
        {
            if(data2[i]==' ' && i<len-1 && ispunct(data2[i+1]))
            {
                data3[j]=data2[i+1];
                j++;
                i++;
                data3[j]=' ';
                j++;
            }
            else if(ispunct(data2[i]))
            {
                data3[j]=data2[i];
                j++;
                data3[j]=' ';
                j++;
            }
            else
            {
                data3[j]=data2[i];
                j++;
            }
            i++;
        }
        data3[j]='\0';
        cout<<"process 2 (pid= "<<getpid()<<" ) write into pipe2... "<<endl<<data3<<endl;
        write(pipe2[1],data3,strlen(data3)+1);
        pid_t x1;
        x1=fork();
        if(x1<0)
        {
            fprintf(stderr,"FORK FAILED.......");
            return 1;
        }
        else if(x1>0)
        {
            wait(NULL);
        }
        else
        {
            char data4[1000];
            read(pipe2[0],data4,1000);
            cout<<"process 3 (pid= "<<getpid()<<" ) read from pipe2... "<<endl<<data4<<endl;
            int len=strlen(data4);
            int i=0,words=0;
            while(i<len)
            {
                char c=toupper(data4[i]);
                cout<<c;
                i++;
                while(i<len && data4[i]!=' ')
                {
                    c=tolower(data4[i]);
                    cout<<c;
                    i++;
                }
                cout<<" ";
                i++;
                words++;
            }
            cout<<endl<<"process 3 (pid= "<<getpid()<<" ) write into pipe3... "<<endl<<words<<endl;
            write(pipe3[1],&words,4);
        }
    }

    return 0;
}
