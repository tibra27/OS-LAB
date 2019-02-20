#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
    char c[100];
    char d[100];
    printf("WELCOME.......YOU ARE IN SHELL!!!\n");
    printf(">>");
    //scanf("%[^\n]s",c);
    fgets(c,100,stdin);
    while(1)
    {
        if(strcmp(c,"exit\n")==0)
        {
            printf("EXITING SHELL..........................\n");
            break;
        }
        else
        {
            int p=fork();
            if(p<0)
            {
                printf("Fork failed........\n");
                break;
            }
            else if(p==0)
            {
                char* comm[15];
                int x=strlen(c);
                int i=0,j=0,z=0;
                while(i<x)
                {
                    j=0;
                    while(c[i]!=' ' && c[i]!='\n' && c[i]!='\0' && i<x)
                    {
                        d[j]=c[i];
                        j++;
                        i++;
                    }
                    d[j]='\0';
                    //strcpy(comm[z],d);
                    comm[z]=strdup(d);
                    z++;
                    i++;
                }
                comm[z]=NULL;
                execvp(comm[0],comm);

            }
            else if(p>0)
            {
                wait(NULL);
                printf(">>");
                //canf("%[^\n]s",c);
                fgets(c,100,stdin);
            }
        }
    }
    return 0;
}
