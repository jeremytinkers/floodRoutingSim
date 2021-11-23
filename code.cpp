//Flood Routing Simulator - Jeremiah Thomas
#include<bits/stdc++.h>
using namespace std;

int networkDiameter = 4; //for now size of the network 

class packet{

public:
int src;
int dest;
int seq_no;
unordered_set<int> nodesVisited;
int hopCount= networkDiameter;

};

class node{

public:
vector<int> nbrs;
unordered_set<int> floodedPackets;
int id;	

};

node nn[6];
packet pkt[3];
bool reached[3] = {false};
bool dropped[3] = {false};

void bfs(int src, int dest, int seq_no){
	
	queue<int> q;
	
	q.push(src);
	pkt[seq_no].nodesVisited.insert(src);
	
	while(!q.empty()){
		
		int curN = q.front();
		//packet -> seq_no has been received at curN
		q.pop();
		
		cout<<"curN:"<<curN<<endl;
	
		
		if(curN==dest){
			reached[seq_no]=true;
			cout<<"Destination reached \n";
			break;
		}
		
	    if(--pkt[seq_no].hopCount==0){
	    	//Drop packet
	    	cout<<"Dropping Packet due to exhausting HopCount \n";
	    	dropped[seq_no] =true;
	    	break;
		}
								
					
		//Check if the packet has been already flooded or not
		
		if(nn[curN].floodedPackets.find(seq_no) == nn[curN].floodedPackets.end()){
		
		//Insert into set
		nn[curN].floodedPackets.insert(seq_no);
					
		//Flooding packet
		//cout<<"Now in node"<<curN<<" nbrs"<<endl;
			
		cout<<"From CurN: "<<curN<<", we are ... \n";
		
		for(auto i: nn[curN].nbrs){
		//	cout<<i<<"\t";
		if(pkt[seq_no].nodesVisited.find(i) == pkt[seq_no].nodesVisited.end()){
			//this packet has not visited node "i" yet
			q.push(i);
			cout<<"Flooding to Node:"<<i<<endl;
			pkt[seq_no].nodesVisited.insert(i);
			//create a new thread and run bfs(i, dest, nn, pkt)
		}
		else{
			//packet visited node i before, so not visiting
			cout<<"Sequence no:"<<seq_no<<" has visited Node:"<<i<<" before"<<endl;
		}
			
		}
		
		cout<<endl;
		
		}
		
		else{
			//already flooded, so ignore
			cout<<"Already Flooded! \n";
		}
		
	}
}


int main(){
	
	//Pre building Network
	//node nn[6];
	
	for(int i=0;i<6;i++){
	nn[i].id=i;
	}
	
	nn[0].nbrs = {1,2,4};
	nn[1].nbrs = {0,5};
	nn[2].nbrs = {0,4};
	nn[3].nbrs = {4};
	nn[4].nbrs = {0,3,5};
	nn[5].nbrs = {1,4};	
	
	
	pkt[0].src = 0;
	pkt[0].dest = 5;
	
	
	
	while(!reached[0] && !dropped[0]){
		
	bfs(pkt[0].src,pkt[0].dest, pkt[0].seq_no);
	
	if(reached[0]){
		cout<<"\n Packet reached! \n";
	}else{
		cout<<"\n Packet Dropped! \n";
	}

	}


	return 0;
}
