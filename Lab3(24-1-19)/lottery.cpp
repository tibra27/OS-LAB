#include<bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cout<<"Enter total number of processes:\n";
    cin>>n;
    int prob[n];
    for(int i=0;i<n;i++)
    {  
        prob[i]=0;
    }
    vector<pair<int,int> > v;
    int x=1,y;
    for(int i=0;i<n;i++)
    { 
    	cout<<"Enter proportion of process "<<i<<": ";
        cin>>y;
        y=x+y-1;
        v.push_back(make_pair(x,y));
        x=y+1;
    }
    int size=v.size();
    for(int i=0;i<100;i++)
    {
        int t=rand()%100;
        for(int j=0;j<v.size();j++)
        {
            int start=v[j].first;
            int end=v[j].second;
            if(t>=start && t<=end)
            {
                prob[j]++;
                cout<<"Ticket number::"<<i+1<<"\t"<<t<<"\t"<<"Process"<<j+1<<endl;
                break;
            }
        }
    }
    for(int i=0;i<n;i++)
    {  
        float z=prob[i]*1.0/100;
        cout<<"Probability of processor for process:"<<i+1<<" = "<<z<<endl;
    }
    return 0;
}
