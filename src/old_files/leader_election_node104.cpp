#include<ros/ros.h>
#include <unistd.h> //for out setting the precision digits in output
#include <stdlib.h> //For rand() and RAND_MAX
#include <time.h> 	//time for srand()
#include<std_msgs/UInt64MultiArray.h>
#include <random>	

std_msgs::UInt64MultiArray my_tuple;

void electionMessage(const std_msgs::UInt64MultiArray& msg);

int main(int argc, char **argv) {

	ros::init(argc, argv, "defaultnode");
	ros::NodeHandle nh;
	
	ros::Subscriber le_sub=nh.subscribe("/election",5,electionMessage);
	ros::Publisher le_pub=nh.advertise<std_msgs::UInt64MultiArray>("/election",1000);
	
	
	//usleep(1000000);
	
	//const std::string PARAM_NAME="~UID";
	
	int myUID;
	bool status=ros::param::get("~UID",myUID);
	//bool status=ros::param::get(PARAM_NAME,myUID);
	//bool status=ros::param::get(PARAM_NAME,myUID);
	if(!status ) {
		ROS_FATAL_STREAM("Could_not_get_Parameter" );
						 //<< PARAM_NAME);
		exit(1);
	}
	
	my_tuple.data.resize(2);
	my_tuple.data[0]=myUID;
	my_tuple.data[1]=23;
	//my_tuple.data={10,12};
		
	ros::Rate my_rate(0.25);
	while (ros::ok() && nh.ok()){
		
		ros::spinOnce();
		le_pub.publish(my_tuple);
		my_rate.sleep();
	}
}

void electionMessage(const std_msgs::UInt64MultiArray& msg){
	if(msg.data[0] != my_tuple.data[0]) {	
		ROS_INFO_STREAM("Received UID: " << msg.data[0] << "    round: " << msg.data[1]); 
	}
}