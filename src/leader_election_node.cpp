#include<ros/ros.h>
#include <unistd.h> //for out setting the precision digits in output
#include <stdlib.h> //For rand() and RAND_MAX
#include <time.h> 	//time for srand()
#include <std_msgs/Int16MultiArray.h>
#include <random>	
#include <chrono>	//for endl

std_msgs::Int16MultiArray mymessage;
std_msgs::Int16MultiArray my_family;
int diam, nround, nParents=0;

void electionmymessage(const std_msgs::Int16MultiArray& msg);
void leaderCheck(const std_msgs::Int16MultiArray& msg);
void countMyParents();

int main(int argc, char **argv) {

	ros::init(argc, argv, "defaultnode");
	ros::NodeHandle nh;
	
	ros::Subscriber le_sub=nh.subscribe("/election",10,electionmymessage);
	ros::Publisher le_pub=nh.advertise<std_msgs::Int16MultiArray>("/election",1000);	
	
	//usleep(1000000);
	
	int myUID=0,child1=0,child2=0,child3=0,parent1=0,parent2=0,parent3=0;
	
	bool status		= ros::param::get("~UID",myUID);
	bool statusc1	= ros::param::get("~child1",child1);
	bool statusp1	= ros::param::get("~parent1",parent1);
	ros::param::get("~child2",child2);
	ros::param::get("~parent2",parent2);
	ros::param::get("~child3",child3);
	ros::param::get("~parent3",parent3);
	ros::param::get("~round",nround); 
	ros::param::get("~diam",diam);
	if(!status || !statusc1 || !statusp1 ) {
		ROS_FATAL_STREAM("Could_not_get_some_critical_Parameter" );
		exit(1);
	}
				
	my_family.data.resize(6);
	my_family.data[0]=child1;
	my_family.data[1]=parent1;
	my_family.data[2]=child2;
	my_family.data[3]=parent2;
	my_family.data[4]=child3;
	my_family.data[5]=parent3;	
	
	countMyParents();
	
	mymessage.data.resize(3);
	mymessage.data[0]=myUID;
	mymessage.data[1]=myUID; 		//as algo proceeds, myUID will be replaced with subsequent higher UIDs received so as to elect leader through flood max
	mymessage.data[2]=0; 			//current status 0-unknown 1-not leader 2-leader. by default, everybody have unknown status. Changes only during leader announcement
	
	//ros::param::get("~round",mymessage.data[2]); 
	//ros::param::get("diam",diam); 
	
	ros::Rate my_rate(0.15);
	while (ros::ok() && nh.ok()){
		
		ros::spinOnce();
		le_pub.publish(mymessage);
		my_rate.sleep();
	}
}

void countMyParents() {
	if(my_family.data[3]!=0 && my_family.data[5]!=0) {
		nParents=3;
	}
	else if (my_family.data[3]!=0) {
		nParents=2;
	}
	else {
		nParents=1;
	}
}

void electionmymessage(const std_msgs::Int16MultiArray& msg){
	// msg.data[0]  owner UID of neighbour
	// msg.data[1]	higest known UID received from neighbour
	
	//if(message.data[2] <= msg.data[2] && 
	if(msg.data[0] != mymessage.data[0]  && mymessage.data[2]==0) {	
		//enter this loop only if everybody finishes the previous round
		//Prevents reading self UID from the message pool
		
		if(msg.data[0] == my_family.data[1] || msg.data[0] == my_family.data[3] || msg.data[0] == my_family.data[5] ){	
			//Prevent listening to unauthorised instances (should only listen to corresponding parents as described in the graph)
			
			ROS_INFO_STREAM("My UID: " << mymessage.data[0] 
							<< ", Round no: " << nround 
							<< "		higest known UID received from neighbour(" << msg.data[0] << "): " << msg.data[1] ); 
			ros::param::set("~round",nround);
			
			if(msg.data[1] > mymessage.data[1]){	
				//if the neighbours's max known UID is higher than mine, update 
				//(only the UIDs authorised for access (as defined in the graph) will pass the previous filter and arrive at this stage)
				
				ROS_INFO_STREAM("My UID: " << mymessage.data[0] 
								<< ", Round no: " << nround 
								<< "		updating my higest known UID to: " << msg.data[1] << std::endl ); 
				mymessage.data[1]=msg.data[1];
			}
			else {
				ROS_INFO_STREAM("My UID: " << mymessage.data[0] 
								<< ", Round no: " << nround 
								<< "		ignoring the higest known UID(" << msg.data[1] << ") passed by neighbour" << std::endl);	
			}	
			leaderCheck(msg);
			nround=nround + 1;
		}
	}
}

void leaderCheck(const std_msgs::Int16MultiArray& msg){
	if(mymessage.data[0] == msg.data[1] && nround == diam){
		//when my UID is equal to neighbour's max known UID and the no:of rounds reached diameter. then self-proclaim myself as leader of the gang
		ROS_INFO_STREAM("My UID: " << mymessage.data[0] 
						<< ", Round no: " << nround 
						<< "		Yahoo! I'm the leader " << std::endl ); 
		mymessage.data[2]=2;
		//ros::shutdown();
	}
	else if(mymessage.data[2]==0 && msg.data[2]!=0 && nround>=diam ) {
		mymessage.data[2]=1;
		ROS_INFO_STREAM("My UID: " << mymessage.data[0] 
							<< ", Round no: " << nround 
							<< "		I am not the leader " << std::endl ); 
		//ros::shutdown();
	}
}