#include<bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
int main()
{
    int r=rand()%4;
    cout<<"Root Id:"<<getpid()<<endl;
    if(r<=0)
        sleep(3);
    else
    {
        int pid1;
        pid1=fork();
        r--;
        if(pid1==0)
        {
            int x=rand()%4;
            cout<<"---child(1) Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
            if(x>0)
            {
                x--;
                int p1=fork();
                if(p1==0)
                    cout<<"------child(1->1) ProcessId:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                else
                {
                    if(x>0)
                    {
                        int p2=fork();
                        x--;
                        if(p2==0)
                            cout<<"------child(1->2) ProcessId:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                        else
                        {
                            if(x>0)
                            {
                            	int p3=fork();
                             	x--;
                             	if(p3==0)
                                 	cout<<"------child(1->3) ProcessId:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                            	else
                                	sleep(1);
							}
                        	else
                                sleep(1);
                    	}
                	}
                    else
                        sleep(1);
                }
            }
            else
                sleep(1);
		}
        else
        {
            sleep(2);
            if(r>0)
            {
                int pid2=fork();
                r--;
                if(pid2==0)
                {
                    int x=rand()%4;
                    cout<<"---child(2) Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                    if(x>0)
                    {
                        x--;
                        int p1=fork();
                        if(p1==0)
                            cout<<"------child(2->1) Process Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                        else
                        {
                            if(x>0)
                            {
                                x--;
                                int p2=fork();
                                if(p2==0)
                                   cout<<"------child(2->2) Process Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                                else
                                {
                                    if(x>0)
                                    {
                                        x--;
                                        int p3=fork();
                                        if(p3==0)
                                        	cout<<"------child(2->3) Process Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                                        else
                                            sleep(1);
                                    }
                                    else
                                        sleep(1);
                                }
                            }
                            else
                                sleep(1);
                        }
                    }
                    else
                         sleep(1);
               }
               else
               {
                    sleep(2);
                    if(r>0)
                    {
                        int pid3=fork();
                        r--;
                        if(pid3==0)
                        {
                             int x=rand()%4;
                             cout<<"---child(3) Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                             if(x>0)
                             {
                                 x--;
                                 int p1=fork();
                                 if(p1==0)
                                   cout<<"------child(3->1) Process Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                                 else
                                 {
                                      if(x>0)
                                      {
                                           x--;
                                           int p2=fork();
                                           if(p2==0)
                                               cout<<"------child(3->2) Process Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                                           else
                                           {
                                                if(x>0)
                                                {
                                                     x--;
                                                     int p3=fork();
                                                     if(p3==0)
                                                         cout<<"------child(3->3) Process Id:"<<getpid()<<endl;//" parent:"<<getppid()<<endl;
                                                     else
                                                         sleep(1);
                                                 }
                                                 else
                                                     sleep(1);
                                            }
                                       }
                                       else
                                           sleep(1);
                                    }
                              }
                    		  else
                    		    sleep(1);
					    }
						else
                            sleep(3);
                    }
                    else
                         sleep(3);
                }
            }
            else
                sleep(3);
        }
    }
}
