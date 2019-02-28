#include<bits/stdc++.h>
using namespace std;
vector<int> v;
float h=0,m=0;
void lru(int page,int size)
{
	if(v.size()<size)                                  // if memory is not full
	{
		if(find(v.begin(),v.end(),page)==v.end())        //page not found 
		{
			v.push_back(page);
			cout<<"Miss......."<<endl<<"pages in memory are::";
			m++;
			for(int i=0;i<v.size();i++)
			{
				cout<<v[i]<<" ";
			}
			cout<<endl;
			
		}
		else                                               //page found
		{
			v.erase(find(v.begin(),v.end(),page));
			v.push_back(page);
			cout<<"Hit......."<<endl<<"pages in memory are::";
			h++;
			for(int i=0;i<v.size();i++)
			{
				cout<<v[i]<<" ";
			}
			cout<<endl;
			
		}
	}
	else                                          //if memory is full
	{
		if(find(v.begin(),v.end(),page)==v.end())        //page not found 
		{
			v.erase(v.begin());
			v.push_back(page);
			cout<<"Miss......."<<endl<<"pages in memory are::";
			m++;
			for(int i=0;i<v.size();i++)
			{
				cout<<v[i]<<" ";
			}
			cout<<endl;
			
		}
		else                                               //page found
		{
			v.erase(find(v.begin(),v.end(),page));
			v.push_back(page);
			cout<<"Hit......."<<endl<<"pages in memory are::";
			h++;
			for(int i=0;i<v.size();i++)
			{
				cout<<v[i]<<" ";
			}
			cout<<endl;
		
		}
	}	
}
int main()
{
	int s;  //capacity of memory
	cout<<".....................LRU PAGE REPLACEMENT POLICY..................."<<endl;
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
			lru(p,s);
		}
	}
}
