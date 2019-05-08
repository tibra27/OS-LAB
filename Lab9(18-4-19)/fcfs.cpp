#include<bits/stdc++.h>
using namespace std;
int main()
{
	int total_tracks=200;
	int n;
	cout<<"Enter total no. of requests::";
	cin>>n;
	int input_requests[n];
	for(int i=0;i<n;i++)
	{
		cout<<"Enter request "<<i+1<<":";
		cin>>input_requests[i];
	}
	int head;
	cout<<"Enter current position of head::";
	cin>>head;
	int total_seek_movements=0;
	for(int i=0;i<n;i++)
	{
		total_seek_movements+=abs(input_requests[i]-head);
		cout<<"HEAD MOVEMENT: "<<head<<"-->"<<input_requests[i]<<endl;
		head=input_requests[i];
	}
	cout<<"TOTAL SEEK MOVEMENTS = "<<total_seek_movements<<endl;
	return 0;
}
