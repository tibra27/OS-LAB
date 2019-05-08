#include<bits/stdc++.h>
using namespace std;
int main()
{
	int total_tracks;
	cout<<"Enter total no. of tracks present::";
	cin>>total_tracks;
	int n;
	cout<<"Enter total no. of requests::";
	cin>>n;
	int input_requests[n],flag[n];
	for(int i=0;i<n;i++)
	{
		cout<<"Enter request "<<i+1<<":";
		cin>>input_requests[i];
		flag[i]=1;
	}
	int head;
	cout<<"Enter current position of head::";
	cin>>head;
	int total_seek_movements=0;
	for(int i=head;i<total_tracks;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(i==input_requests[j] && flag[j]!=0)
			{
				total_seek_movements+=abs(i-head);
				cout<<"HEAD MOVEMENT: "<<head<<"-->"<<i<<endl;
				head=i;	
				flag[j]=0;
			}			
		}	
	}
	if(head<total_tracks-1)
	{
		total_seek_movements+=abs(total_tracks-1-head);
		cout<<"HEAD MOVEMENT: "<<head<<"-->"<<total_tracks-1<<endl;
		head=total_tracks-1;	
	}
	for(int i=total_tracks-1;i>=0;i--)
	{
		for(int j=0;j<n;j++)
		{
			if(i==input_requests[j] && flag[j]!=0)
			{
				total_seek_movements+=abs(i-head);
				cout<<"HEAD MOVEMENT: "<<head<<"-->"<<i<<endl;
				head=i;	
				flag[j]=0;
			}			
		}
	}
	cout<<"TOTAL SEEK MOVEMENTS = "<<total_seek_movements<<endl;
	return 0;
}
