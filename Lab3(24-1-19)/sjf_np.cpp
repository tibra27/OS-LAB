#include <bits/stdc++.h>
using namespace std;
int global_t=0;            //initial time=0
int np;                   //number of processes

struct process
{
	int pid;                      //process id
	int arrival_t;                //arrival time
	int priority;
	int share;
	vector<pair<char,int> > burst;              //stores all burst times
	int index;                                  //to access the particular burst time
	int timeLeft;                               //remaining time in a burst
	int response_t;                             //response time
	int waiting_t;                              //waiting time
	int turn_around_t;                          //turn around time
	int total_burst_t;                          //total burst time
	bool execution_start_status;                //false if execution is not started

};
typedef struct process process;
process p[10];

bool sort_t(process a,process b);
bool sort_bt(process* a,process* b);
void SJF_NP();


int main()
{
	ifstream file("process.dat");
	string n;
	getline(file,n);
	np=atoi(n.c_str());
	vector< vector<string> > inp;
	for(int j=0;j<np;j++)
	{
		string s;
		getline(file,s);
		vector<string> v;
		int i=0;
		string s1="";
		while(i<s.size()-1)
		{
			if(s[i]!=' ')
			{
				s1=s1+s[i];
				i++;
			}
			else if(s[i]==' ')
			{
				i++;
				v.push_back(s1);
				s1="";
			}
		}
		inp.push_back(v);
	}
	for(int i = 0; i < np; ++i)
	{
		p[i].pid=atoi(inp[i][0].c_str());
		p[i].arrival_t=atoi(inp[i][1].c_str());
		p[i].priority=atoi(inp[i][2].c_str());
	//	p[i].share=atoi(inp[i][3].c_str());
		for(int j=3;j<inp[i].size();j++)
		{
			char type;int dur;
			type=inp[i][j].substr(0,1)[0];
			string s1=inp[i][j].substr(1,inp[i][j].size()-1);
			dur=atoi(s1.c_str());
			p[i].burst.push_back(make_pair(type,dur));
		}
		p[i].execution_start_status=false;
	}
	for(int i=0;i<np;i++)
	{
		int sum = 0;
		for(int j=0;j<p[i].burst.size();j++)
		{
			sum += p[i].burst[j].second;
		}
		p[i].total_burst_t = sum;
	}
	SJF_NP();
	for(int i=0;i<np;i++)
	{
		p[i].waiting_t = p[i].turn_around_t - p[i].total_burst_t;
	}
	printf("-------------------SHORTEST JOB FIRST (NP) CPU SCHEDULING-------------------\n");
	printf("pid\tarriving_time\tresponse_time\twaiting_time\tturn_around_time\n");
	for(int i=0;i<np;i++)
	{
		printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n",p[i].pid,p[i].arrival_t,p[i].response_t,p[i].waiting_t,p[i].turn_around_t);
	}
	return 0;
}




bool sort_at(process a,process b)                       //sort according to arriving time
{
	return(a.arrival_t < b.arrival_t);
}
bool sort_bt(process* a,process* b)                     // sort according to burst time
{
	return(a->burst[a->index].second < b->burst[b->index].second);

}


void SJF_NP()
{
	vector<process*> p_queue;        // process queue
	queue<process*> i_queue;        // input queue
	queue<process*> o_queue;        // output queue
	sort(p,p+np,sort_at);            // sort the processes according to arrival time
	int indexNotYetPushed=0;
	while(true)
	{
		//
		//printf("----%d-----\n",global_t);
		while(indexNotYetPushed<np && p[indexNotYetPushed].arrival_t == global_t)
		{  
		//	printf("%d\n",indexNotYetPushed);
			p_queue.push_back(&p[indexNotYetPushed]);                    // pushing new processes into process_queue
			p[indexNotYetPushed].index=0;
			p[indexNotYetPushed].timeLeft=p[indexNotYetPushed].burst[p[indexNotYetPushed].index].second;
			indexNotYetPushed++;
		}
		if(!p_queue.empty() && p_queue.front()->timeLeft == 0)
		{
			if(p_queue.front()->index == p_queue.front()->burst.size()-1)
			{
		//		printf("process %d terminated\n",p_queue.front()->pid);
				p_queue.front()->turn_around_t = global_t- p_queue.front()->arrival_t;  // process terminated 
			}
			else if(p_queue.front()->burst[p_queue.front()->index + 1].first == 'I')
			{
				i_queue.push(p_queue.front());                      //pushing into input queue
				p_queue.front()->index++;
				p_queue.front()->timeLeft = p_queue.front()->burst[p_queue.front()->index].second;
		//		printf("process %d moved from process queue to input queue\n",p_queue.front()->pid);
			}
			else if(p_queue.front()->burst[p_queue.front()->index + 1].first == 'O')
			{
				o_queue.push(p_queue.front());                     //pushing into output queue
				p_queue.front()->index++;
				p_queue.front()->timeLeft = p_queue.front()->burst[p_queue.front()->index].second;
		//		printf("process %d moved from process queue to output queue\n",p_queue.front()->pid);
			}
			p_queue.erase(p_queue.begin());
			sort(p_queue.begin(),p_queue.end(),sort_bt);
		}

		if(!i_queue.empty() && i_queue.front()->timeLeft == 0){
			if(i_queue.front()->index == i_queue.front()->burst.size()-1)
			{
				i_queue.front()->turn_around_t = global_t- i_queue.front()->arrival_t;
		//		printf("process %d terminated\n",i_queue.front()->pid);
			}
			else if(i_queue.front()->burst[i_queue.front()->index+1].first == 'C')
			{
				p_queue.push_back(i_queue.front());
				i_queue.front()->index ++;
				i_queue.front()->timeLeft = i_queue.front()->burst[i_queue.front()->index].second;
		//		printf("process %d moved from input queue to process queue\n",i_queue.front()->pid);
			}
			else if(i_queue.front()->burst[i_queue.front()->index+1].first == 'O')
			{
				o_queue.push(i_queue.front());
				i_queue.front()->index ++;
				i_queue.front()->timeLeft = i_queue.front()->burst[i_queue.front()->index].second;
		//		printf("process %d moved from input queue to output queue\n",i_queue.front()->pid);
			}
			i_queue.pop();
		}

		if(!o_queue.empty() && o_queue.front()->timeLeft == 0)
		{
			if(o_queue.front()->index == o_queue.front()->burst.size()-1)
			{
				o_queue.front()->turn_around_t = global_t- o_queue.front()->arrival_t;
		//		printf("process %d terminated\n",o_queue.front()->pid);
			}
			else if(o_queue.front()->burst[o_queue.front()->index+1].first == 'C')
			{
				p_queue.push_back(o_queue.front());
				o_queue.front()->index ++;
				o_queue.front()->timeLeft = o_queue.front()->burst[o_queue.front()->index].second;
		//		printf("process %d moved from output queue to process queue\n",o_queue.front()->pid);
			}
			else if(o_queue.front()->burst[o_queue.front()->index+1].first == 'I')
			{
				i_queue.push(o_queue.front());
				o_queue.front()->index ++;
				o_queue.front()->timeLeft = o_queue.front()->burst[o_queue.front()->index].second;
			//	printf("process %d moved from output queue to input queue\n",o_queue.front()->pid);
			}
			o_queue.pop();
		}

		if(indexNotYetPushed == np && p_queue.empty() && i_queue.empty() && o_queue.empty())
		{
			break;
		}

		if(!p_queue.empty())
		{
			if(!p_queue.front()->execution_start_status)
			{
				p_queue.front()->response_t = global_t-p_queue.front()->arrival_t;
				p_queue.front()->execution_start_status = true;
			}
			p_queue.front()->timeLeft--;
		//	printf("process queue   %d %d\n",p_queue.front()->pid,p_queue.front()->timeLeft);
		}
		if(!i_queue.empty())
		{
			i_queue.front()->timeLeft--;
		//	printf("input queue    %d %d \n",i_queue.front()->pid,i_queue.front()->timeLeft);
		}
		if(!o_queue.empty())
		{
			o_queue.front()->timeLeft--;
		//	printf("output queue   %d %d\n",o_queue.front()->pid,o_queue.front()->timeLeft);
		}
			global_t++;
	}
}

