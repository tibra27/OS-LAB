#include<bits/stdc++.h>
using namespace std;
queue<int> page_queue;
vector<int> v;
float h=0,m=0;
void print_queue(queue<int> page_queue,int s)
{
	for(int i=0;i<s;i++)
	{
		int x=page_queue.front();
		page_queue.pop();
		cout<<x<<" ";
		page_queue.push(x);
	}
	cout<<endl;
}
void fifo(int page,int size)
{
	if(v.size()<size)                                  // if memory is not full
	{
		if(find(v.begin(),v.end(),page)==v.end())        //page not found 
		{
			v.push_back(page);
			page_queue.push(page);
			cout<<"Miss......."<<endl<<"pages in queue are::";
			m++;
			print_queue(page_queue,v.size());
		}
		else                                               //page found
		{
			cout<<"Hit......."<<endl<<"pages in queue are::";
			h++;
			print_queue(page_queue,v.size());
		}
	}
	else                                          //if memory is full
	{
		if(find(v.begin(),v.end(),page)==v.end())        //page not found 
		{
			int page1=page_queue.front();
			page_queue.pop();
			v.erase(find(v.begin(),v.end(),page1));
			v.push_back(page);
			page_queue.push(page);
			cout<<"Miss......."<<endl<<"pages in queue are::";
			m++;
			print_queue(page_queue,v.size());
		}
		else                                               //page found
		{
			cout<<"Hit......."<<endl<<"pages in queue are::";
			h++;
			print_queue(page_queue,v.size());
		}
	}	
}
int main()
{
	int s;  //capacity of memory
	cout<<"...........................FIFO PAGE REPLACEMENT POLICY........................."<<endl;
	cout<<"Enter no. of pages that can be hold by memory i.e. memory capacity::";
	cin>>s;
	int p;
	cout<<"Enter page no. or -1 to exit.."<<endl;
	while(1)
	{
		cin>>p;
		if(p==-1)
		{
			float hr=h/(h+m);
			float mr=1-hr;
			cout<<"Hit Ratio="<<hr<<endl<<"Miss Ratio="<<mr<<endl;
			break;
		}
		else
		{
			fifo(p,s);
		}
	}
}
