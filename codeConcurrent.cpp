//Flood Routing Simulator - Jeremiah Thomas
#include <bits/stdc++.h>
using namespace std;

int networkDiameter = 10; // initially set, updated w user input
std::chrono::time_point<std::chrono::system_clock > startSimTime, endSimTime;

class packet
{
	public:
	int src;
	int dest;
	int seq_no;
	double ttl_left = 5;	//time to live
	double startTime;
	unordered_set<int> nodesVisited;
	int maxhopCount = networkDiameter;
	int maxRetransmit = 3;
};

class node
{
	public:
	vector<int> nbrs;
	unordered_set<int> floodedPackets;
	int id;

};

//Essential Entities:-
node nn[1000];
packet pkt[1000];
bool reached[1000] = { false
};

bool dropped[1000] = { false
};

int noNodes = 1000;
int noPackets = 1000;

void bfs1(int src, int dest, int seq_no)
{
	queue<int> q;

	q.push(src);
	pkt[seq_no].nodesVisited.insert(src);

	while (!q.empty())
	{
		//Here it is meant that seq_no has now been received at curN

		int curN = q.front();	//curN -> current Node
		q.pop();

		//check if current node is the destination
		if (curN == dest)
		{
			reached[seq_no] = true;
			cout << "Destination reached (Node:" << dest << ") \n";
			break;
		}

		//check if packet has exceeded its time to live factor:-

		std::chrono::time_point<std::chrono::system_clock > curTime = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = curTime - startSimTime;

		std::time_t curTimePrint = std::chrono::system_clock::to_time_t(curTime);

		float curTimeInSeconds = elapsed_seconds.count();

		cout << "Packet: " << seq_no << ", Received at Node: " << curN << " at : " << curTimeInSeconds << "s" << endl;

		if (pkt[seq_no].ttl_left < (curTimeInSeconds - pkt[seq_no].startTime))
		{
			//Drop packet
			cout << "Dropping Packet due to exceeding TTL \n";
			dropped[seq_no] = true;
			break;
		}

		//check if hopCount has exhausted to zero, if so drop the packet
		if (--pkt[seq_no].maxhopCount == 0)
		{
			//Drop packet
			cout << "Dropping Packet due to exhausting HopCount \n";
			dropped[seq_no] = true;
			break;
		}

		//Check if the packet has been already flooded by curN or not		
		if (nn[curN].floodedPackets.find(seq_no) == nn[curN].floodedPackets.end())
		{
			nn[curN].floodedPackets.insert(seq_no);	//Insert into set

			//Flooding the packet to nbs nodes on conditions

			//cout<<"Now in node"<<curN<<" nbrs"<<endl;
			cout << "From Node: " << curN << ", we are ... \n";

			for (auto i: nn[curN].nbrs)
			{
				//	cout<<i<<"\t";
				if (pkt[seq_no].nodesVisited.find(i) == pkt[seq_no].nodesVisited.end())
				{
					//this packet has not visited node "i" yet
					q.push(i);
					cout << "Flooding to Node:" << i << endl;
					pkt[seq_no].nodesVisited.insert(i);
				}
				else
				{
					//packet visited node i before, so not visiting
					cout << "Sequence no:" << seq_no << " has visited Node:" << i << " before" << endl;
				}
			}

			cout << endl;

		}
		else
		{
			//already flooded, so ignore
			cout << "Already Flooded! \n";
		}
	}
}

void bfs2(int src, int dest, int seq_no)
{
	queue<int> q;

	q.push(src);
	pkt[seq_no].nodesVisited.insert(src);

	while (!q.empty())
	{
		//Here it is meant that seq_no has now been received at curN

		int curN = q.front();	//curN -> current Node
		q.pop();

		//check if current node is the destination
		if (curN == dest)
		{
			reached[seq_no] = true;
			cout << "Destination reached (Node:" << dest << ") \n";
			break;
		}

		//check if packet has exceeded its time to live factor:-

		std::chrono::time_point<std::chrono::system_clock > curTime = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = curTime - startSimTime;

		std::time_t curTimePrint = std::chrono::system_clock::to_time_t(curTime);

		float curTimeInSeconds = elapsed_seconds.count();

		cout << "Packet: " << seq_no << ", Received at Node: " << curN << " at : " << curTimeInSeconds << "s" << endl;

		if (pkt[seq_no].ttl_left < (curTimeInSeconds - pkt[seq_no].startTime))
		{
			//Drop packet
			cout << "Dropping Packet due to exceeding TTL \n";
			dropped[seq_no] = true;
			break;
		}

		//check if hopCount has exhausted to zero, if so drop the packet
		if (--pkt[seq_no].maxhopCount == 0)
		{
			//Drop packet
			cout << "Dropping Packet due to exhausting HopCount \n";
			dropped[seq_no] = true;
			break;
		}

		//Check if the packet has been already flooded by curN or not		
		if (nn[curN].floodedPackets.find(seq_no) == nn[curN].floodedPackets.end())
		{
			nn[curN].floodedPackets.insert(seq_no);	//Insert into set

			//Flooding the packet to nbs nodes on conditions

			thread *arr = new thread[nn[curN].nbrs.size()];	//creating threads for concurrent execution

			cout << "From Node: " << curN << ", we are ... \n";

			for (auto i: nn[curN].nbrs)
			{
				//	cout<<i<<"\t";
				if (pkt[seq_no].nodesVisited.find(i) == pkt[seq_no].nodesVisited.end())
				{
					//this packet has not visited node "i" yet
					q.push(i);
					cout << "Flooding to Node:" << i << endl;
					pkt[seq_no].nodesVisited.insert(i);
					//create a new thread and run bfs(i, dest, seq_no)
					arr[i] = thread(bfs2, i, dest, seq_no);
				}
				else
				{
					//packet visited node i before, so not visiting
					cout << "Sequence no:" << seq_no << " has visited Node:" << i << " before" << endl;
				}
			}

			cout << endl;

		}
		else
		{
			//already flooded, so ignore
			cout << "Already Flooded! \n";
		}
	}
}

void routingIndividualPkt_lvl1(packet pkt)
{
	retransmit: while (!reached[pkt.seq_no] && !dropped[pkt.seq_no])
	{
		//set startTime for curPacket
		std::chrono::time_point<std::chrono::system_clock > curTime = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = curTime - startSimTime;

		//std::time_t end_time = std::chrono::system_clock::to_time_t(endSimTime);

		pkt.startTime = elapsed_seconds.count();

		cout << "\n\nSending Packet:" << pkt.seq_no << " at: " << pkt.startTime << "s\n";

		bfs1(pkt.src, pkt.dest, pkt.seq_no);

		if (reached[pkt.seq_no])
		{
			cout << "\nPacket:" << pkt.seq_no << " has been successfully received!\n";
		}
	}

	if (dropped[pkt.seq_no])
	{
		//Make the current thread sleep for a second before retransmitting...
		std::this_thread::sleep_for(std::chrono::seconds(1));
		//retransmit after sometime
		if (pkt.maxRetransmit--)
		{
			cout << "\nRetransmitting Packet:" << pkt.seq_no;
			goto retransmit;
		}
		else
		{
			cout << "\nRetransmits for Packet:" << pkt.seq_no << " exhausted!\n";
		}
	}
}

void routingIndividualPkt_lvl2(packet pkt)
{
	retransmit: while (!reached[pkt.seq_no] && !dropped[pkt.seq_no])
	{
		//set startTime for curPacket
		std::chrono::time_point<std::chrono::system_clock > curTime = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = curTime - startSimTime;

		pkt.startTime = elapsed_seconds.count();

		cout << "\n\nSending Packet:" << pkt.seq_no << " at: " << pkt.startTime << "s\n";

		bfs2(pkt.src, pkt.dest, pkt.seq_no);

		if (reached[pkt.seq_no])
		{
			cout << "\nPacket:" << pkt.seq_no << " has been successfully received!\n";
		}
	}

	if (dropped[pkt.seq_no])
	{
		//Make the current thread sleep for a second before retransmitting...
		std::this_thread::sleep_for(std::chrono::seconds(1));
		//retransmit after sometime
		if (pkt.maxRetransmit--)
		{
			cout << "\nRetransmitting Packet:" << pkt.seq_no;
			goto retransmit;
		}
		else
		{
			cout << "\nRetransmits for Packet:" << pkt.seq_no << " exhausted!\n";
		}
	}
}

//Check if a node exists in the neighbour set of another node
bool isAlreadyNbr(int nodeToCheck, int nodeParent)
{
	for (auto nbr: nn[nodeParent].nbrs)
	{
		if (nbr == nodeToCheck)
		{
			return true;
		}
	}

	return false;

}

int main()
{
	//I/O from user:-

	int userChoice = 1;
	int levelConcurrency = 1;

	cout << "\nDo you want to load PreBuilt Network for this simulation? If yes -> 1, If no ->0 \n";
	cin >> userChoice;

	l11:
		cout << "\nWhat level of concurreny do you prefer? (1 -> Level 1, 2 -> Level 2)";
	cin >> levelConcurrency;

	if (levelConcurrency != 1 && levelConcurrency != 2)
	{
		cout << "\nEnter a valid level of concurrency -> 1 or 2";
		goto l11;
	}

	if (!userChoice)
	{
		//Custom Network as per User's requirement
		l1: cout << "\nEnter network size(no of nodes) : ";
		cin >> noNodes;
		if (noNodes > 1000 || noNodes < 2)
		{
			cout << "\nEnter Valid no of nodes from 2 to 1000";
			goto l1;

		}

		l2: cout << "\nEnter no of packets: ";
		cin >> noPackets;
		if (noPackets > 1000 || noPackets < 1)
		{
			cout << "\nEnter Valid no of packets from 1 to 1000";
			goto l2;
		}

		//building user requested network. Adding neighbours as required for each node:-
		for (int i = 0; i < noNodes; i++)
		{
			nn[i].id = i;
			int nbrsSize = 0;

			if ((noNodes - nn[i].nbrs.size()) <= 1)
			{
				cout << "\nCurrent Node:" << i << " is full, no more neighbours possible!";
			}
			else
			{
				if (nn[i].nbrs.size())
				{
					cout << "\nCurrent Neighbours of node: " << i << " are:[";
					for (auto x: nn[i].nbrs)
					{
						cout << x << ",";
					}

					cout << "]\n";
				}
				else
				{
					cout << "\nCurrent Node:" << i << " has no assigned neighbours";
				}

				l3:
					cout << "\nEnter No of New Neighbours for Node:" << i << "\n";
				cin >> nbrsSize;

				if (nbrsSize >= noNodes || nbrsSize < 0)
				{
					cout << "\nInvalid No of Neighbours. Must be less than " << noNodes << " and greater than 0";
					goto l3;
				}

				if (nbrsSize > (noNodes - nn[i].nbrs.size() - 1))
				{
					cout << "\nCur Node can only accomodate a max of : " << (noNodes - nn[i].nbrs.size() - 1) << " nodes more";
					goto l3;
				}

				if (nbrsSize)
				{
					for (int j = 0; j < nbrsSize; j++)
					{
						int tempNbr;

						l4:
							cout << "\nEnter Neigbhour: ";
						cin >> tempNbr;	//temporary Neighbour

						//Validating Neighbour entered
						if (tempNbr == i)
						{
							cout << "\nA node can't be the neighbour of itself!";
							goto l4;
						}

						if (tempNbr >= noNodes || tempNbr < 0)
						{
							cout << "\nInvalid Node Id. Node Ids range from : 0 - " << (noNodes - 1);
							goto l4;
						}

						if (isAlreadyNbr(tempNbr, i))
						{
							cout << "\nAlready a neighbour";
							goto l4;

						}

						//Neighbour entered has been validated, now push into "nbrs" array for i and tempNbr as well
						nn[i].nbrs.push_back(tempNbr);
						nn[tempNbr].nbrs.push_back(i);
					}
				}
			}

			cout << "\n\n";
		}

		//building user custom packets

		networkDiameter = noNodes;
		int maxHopCount = networkDiameter;
		double ttl_left = 3;
		int retransmitLimit = 3;

		cout << "\nEnter maxHopCount permitted for a packet: ";
		cin >> maxHopCount;

		cout << "\nEnter Time To Live(TTL) desired for a packet(in Seconds, double type so decimal allowed): ";
		cin >> ttl_left;

		cout << "\nEnter Max No of Retransmissions possible for a packet before it will be discarded: ";
		cin >> retransmitLimit;

		for (int i = 0; i < noPackets; i++)
		{
			pkt[i].seq_no = i;
			pkt[i].maxhopCount = maxHopCount;
			pkt[i].ttl_left = ttl_left;
			pkt[i].maxRetransmit = retransmitLimit;

			l5:
				l6:
				cout << "\nEnter Source Node for Packet " << i << " :";
			cin >> pkt[i].src;

			if (pkt[i].src < 0 || pkt[i].src >= noNodes)
			{
				cout << "\nInvalid Node Id. Node Ids range from : 0 - " << (noNodes - 1);
				goto l6;
			}

			l7:
				cout << "\nEnter Destination Node for Packet " << i << " :";
			cin >> pkt[i].dest;

			if (pkt[i].dest < 0 || pkt[i].dest >= noNodes)
			{
				cout << "\nInvalid Node Id. Node Ids range from : 0 - " << (noNodes - 1);
				goto l7;
			}

			if (pkt[i].dest == pkt[i].src)
			{
				cout << "\nWarning: It is inadvisable to set src and dest as the same.";
				goto l5;
			}
		}
	}
	else
	{
		//build below prebuilt network:-
		//node nn[6];

		for (int i = 0; i < 6; i++)
		{
			nn[i].id = i;
		}

		nn[0].nbrs = { 1,
			2,
			4
		};
		nn[1].nbrs = { 0,
			5
		};
		nn[2].nbrs = { 0,
			4
		};
		nn[3].nbrs = { 4 };
		nn[4].nbrs = { 0,
			3,
			5
		};
		nn[5].nbrs = { 1,
			4
		};

		pkt[0].src = 0;
		pkt[0].dest = 5;
		pkt[0].seq_no = 0;

		pkt[1].src = 1;
		pkt[1].dest = 4;
		pkt[1].seq_no = 1;

		pkt[2].src = 2;
		pkt[2].dest = 3;
		pkt[2].seq_no = 2;

	}

	thread noTransmissions[1000];

	startSimTime = std::chrono::system_clock::now();

	std::time_t start_time = std::chrono::system_clock::to_time_t(startSimTime);

	cout << "\nSimulation started at : " << ctime(&start_time) << endl;

	for (int i = 0; i < 3; i++)
	{
		cout << "\nAssigning new Thread for Packet:" << i << " .... \n";
		if (levelConcurrency == 1)
		{
			noTransmissions[i] = thread(routingIndividualPkt_lvl1, pkt[i]);
		}
		else
		{
			noTransmissions[i] = thread(routingIndividualPkt_lvl2, pkt[i]);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		cout << "\nJoining Thread of Packet : " << i << "\n";
		noTransmissions[i].join();

	}

	endSimTime = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = endSimTime - startSimTime;

	std::time_t end_time = std::chrono::system_clock::to_time_t(endSimTime);

	cout << "\n\nTotal Simulation time: " << elapsed_seconds.count() << "s\n";
	cout << "Simulation Finished at : " << std::ctime(&end_time) << endl;

	return 0;
}
