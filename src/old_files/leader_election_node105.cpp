#include<ros/ros.h>
#include <unistd.h> //for out setting the precision digits in output
#include <stdlib.h> //For rand() and RAND_MAX
#include <time.h> 	//time for srand()
#include<std_msgs/Int16MultiArray.h>
#include <random>	

std_msgs::Int16MultiArray my_tuple;

void electionMessage(const std_msgs::Int16MultiArray& msg);

int main(int argc, char **argv) {

	ros::init(argc, argv, "defaultnode");
	ros::NodeHandle nh;
	
	//ros::Subscriber le_sub=nh.subscribe("/election",10,electionMessage);
	ros::Publisher le_pub=nh.advertise<std_msgs::Int16MultiArray>("/election",1000);	
	
	//usleep(1000000);
	
	int myUID=0,child1=0,child2=0,child3=0,parent1=0,parent2=0,parent3=0;
	
	bool status=ros::param::get("~UID",myUID);
	bool statusc1=ros::param::get("~child1",child1);
	bool statusp1=ros::param::get("~parent1",parent1);
	bool statusc2=ros::param::get("~child2",child2);
	bool statusp2=ros::param::get("~parent2",parent2);
	bool statusc3=ros::param::get("~child3",child3);
	bool statusp3=ros::param::get("~parent3",parent3);
	
	if(!status || !statusc1 || !statusp1 ) {
		ROS_FATAL_STREAM("Could_not_get_some_critical_Parameter" );
		exit(1);
	}
	
	my_tuple.data.resize(7);
	my_tuple.data[0]=myUID;
	my_tuple.data[1]=child1;
	my_tuple.data[2]=parent1;
	my_tuple.data[3]=child2;
	my_tuple.data[4]=parent2;
	my_tuple.data[5]=child3;
	my_tuple.data[6]=parent3;
	my_tuple.data[7]=myUID;		//as algo proceeds, myUID will be replaced with subsequent higher UIDs received so as to elect leader through flood max
	
	ros::Rate my_rate(0.1);
	while (ros::ok() && nh.ok()){
		
		ros::spinOnce();
		le_pub.publish(my_tuple);
		my_rate.sleep();
	}
}

void electionMessage(const std_msgs::Int16MultiArray& msg){
	if(msg.data[0] != my_tuple.data[0]) {	//Prevents reading own UID
		ROS_INFO_STREAM("Received UID: " << msg.data[0] << "	child: (" << msg.data[1] << "," << msg.data[3] << "," << msg.data[5] << ")	Parent: (" 
						 << msg.data[2] << "," << msg.data[4] << "," << msg.data[6] << ")"); 
		/*if(){	//Prevent listening to unauthorised instances
			
			if(){	//find max UID from the recevied UIDs (only the authorised ones pass the previous filter and arrive at this stage)
				
			}
		}*/
	}
}