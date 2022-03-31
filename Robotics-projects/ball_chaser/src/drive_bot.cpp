#include"ros/ros.h"
#include"geometry_msgs/Twist.h"
#include"ball_chaser/DriveToTarget.h"

ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,ball_chaser::DriveToTarget::Response& res  )
{

ROS_INFO("velocites recieved -v1:%1.2f v2:%1.2f",(float)req.linear_x,(float)req.angular_y);

geometry_msgs::Twist motor_command;

motor_command.angular.z = req.angular_y;
motor_command.linear.x = req.linear_x;


motor_command_publisher.publish(motor_command);

res.msg_feedback="velocities set to z : " + std::to_string(motor_command.angular.z) + 
"to x : " + std::to_string(motor_command.linear.x);
ROS_INFO_STREAM(res.msg_feedback);
}


int main(int argc, char** argv)
{
ros::init(argc,argv,"drive_bot");
ros::NodeHandle n;

motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);

ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot",handle_drive_request);




ros::spin();
return 0;

}
