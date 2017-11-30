#include<ros/ros.h>
#include<std_msgs/UInt64.h>
#include <unistd.h> //for out setting the precision digits in output
#include <stdlib.h> //For rand() and RAND_MAX
#include <time.h> 	//time for srand()

//#include<std_msgs/UInt64MultiArray.h>

//std_msgs::UInt64MultiArray my_tuple;

//uint64 my_tuple [2];
void electionMessage(const std_msgs::UInt64& msg);
//void electionMessage(const std_msgs::UInt64MultiArray& msg);

int main(int argc, char **argv) {
	std_msgs::UInt64 my_tuple;
	
	time_t seconds;					//Declare variable to hold seconds on clock.
	time(&seconds);					//Get value from system clock and place in seconds variable.
	srand((unsigned int) seconds);	//Convert seconds to a unsigned integer.
	
	ros::init(argc, argv, "defaultnode");
	ros::NodeHandle nh;
	
	ros::Subscriber le_sub=nh.subscribe("/election",5,electionMessage);
	ros::Publisher le_pub=nh.advertise<std_msgs::UInt64>("/election",1000);
	//ros::Publisher le_pub=nh.advertise<std_msgs::UInt64MultiArray>("/election",1000);
	my_tuple.data = rand();
	
	ros::Rate my_rate(2);
	while (ros::ok() && nh.ok()){
		
		ros::spinOnce();
		le_pub.publish(my_tuple);
		my_rate.sleep();
	}
}

//void electionMessage(const std_msgs::UInt64MultiArray& msg){
void electionMessage(const std_msgs::UInt64& msg){
	ROS_INFO_STREAM("UID: " << msg.data); 
	//ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "Pose=(" << PoseIn.x << 
}