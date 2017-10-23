#include<ros/ros.h>
#include<std_msgs/UInt64.h>
#include <unistd.h>
//#include<std_msgs/UInt64MultiArray.h>

//std_msgs::UInt64MultiArray my_tuple;
std_msgs::UInt64 my_tuple;
//uint64 my_tuple [2];
void electionMessage(const std_msgs::UInt64& msg);
//void electionMessage(const std_msgs::UInt64MultiArray& msg);

int main(int argc, char **argv) {
	ros::init(argc, argv, "defaultnode");
	ros::NodeHandle nh;
	
	ros::Subscriber le_sub=nh.subscribe("/election",5,electionMessage);
	ros::Publisher le_pub=nh.advertise<std_msgs::UInt64>("/election",1000);
	//ros::Publisher le_pub=nh.advertise<std_msgs::UInt64MultiArray>("/election",1000);
	
	my_tuple.data = 16;
		
	while (ros::ok() && nh.ok()){
		
		ros::spinOnce();
		le_pub.publish(my_tuple);
	}
}

//void electionMessage(const std_msgs::UInt64MultiArray& msg){
void electionMessage(const std_msgs::UInt64& msg){
	ROS_INFO_STREAM("UID: " << msg.data); 
	//ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "Pose=(" << PoseIn.x << 
}