#include<ros/ros.h>
#include <unistd.h> //for out setting the precision digits in output
#include <stdlib.h> //For rand() and RAND_MAX
#include <time.h> 	//time for srand()
#include<std_msgs/Int16MultiArray.h>
#include <random>	

std_msgs::Int16MultiArray mymessage;
std_msgs::Int16MultiArray my_family;
int diam=0, round=0;

void electionmymessage(const std_msgs::Int16MultiArray& msg);

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
	ros::param::get("~round",round); 
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
	
	mymessage.data.resize(2);
	mymessage.data[0]=myUID;
	mymessage.data[1]=myUID; 		//as algo proceeds, myUID will be replaced with subsequent higher UIDs received so as to elect leader through flood max
	
	ros::Rate my_rate(0.15);
	while (ros::ok() && nh.ok()){
		
		ros::spinOnce();
		le_pub.publish(mymessage);
		my_rate.sleep();
	}
}

void electionmymessage(const std_msgs::Int16MultiArray& msg){
	// msg.data[0]  owner UID
	// msg.data[1]	higest unknown UID from owner
	
	if(msg.data[0] != mymessage.data[0]) {	//Prevents reading self UID
		ROS_INFO_STREAM("Owner UID: " << mymessage.data[0] << "	higest known UID: " << msg.data[1] ); 
		if(msg.data[0] == my_family.data[1] || msg.data[0] == my_family.data[3] || msg.data[0] == my_family.data[5] ){	//Prevent listening to unauthorised instances (should only listen to corresponding parents)
			
			if(msg.data[1] > mymessage.data[1]){	//find max UID from the recevied UIDs (only the UIDs authorised for access (defined in the graph) will pass the previous filter and arrive at this stage)				mymessage.data[1]=msg.data[1];
				ROS_INFO_STREAM("Owner UID: " << mymessage.data[0] << " updating its higest known UID to: " << mymessage.data[1] ); 
			}
		}
	}
}