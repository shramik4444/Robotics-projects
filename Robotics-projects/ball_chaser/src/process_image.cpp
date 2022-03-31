#include"ros/ros.h"
#include"ball_chaser/DriveToTarget.h"
#include<sensor_msgs/Image.h>


ros::ServiceClient client;


void drive_robot(float lin_x, float ang_z)
{
ROS_INFO_STREAM("Moving with given velocities");
ball_chaser::DriveToTarget srv;
srv.request.linear_x = lin_x;
srv.request.angular_y = ang_z;
	if(!client.call(srv))
    {
	ROS_ERROR("failed to call the service");
    }

}

void process_image_callback(const sensor_msgs::Image img)
{
int max_value = 0;
int white_pixel = 255;


for(int i = 0; i < img.step*img.height; i+=3)
   {
bool ball_present = false;
max_value = img.data[i];
ROS_INFO("inside the loop searching for pixel");
ROS_INFO("max_value = %d and i-value = %d",max_value,i);

	if (img.data[i] == white_pixel)
   {
   ROS_INFO("ball found");
   ball_present = true;
   int section = i%img.step;
while(ball_present)
{
   if(section > 0 && section < img.step/3)
     {
	ROS_INFO(" turn Left");
	drive_robot(0.0,0.5);
     }
   else if(section > img.step*1/3 && section < img.step*2/3)
     {
	drive_robot(0.5,0.0);
	ROS_INFO("Move Forward");
     }
   else if(section > img.step*2/3 && section < img.step)
     {
	drive_robot(0.0,-0.5);
	ROS_INFO("MOve right");
     }
}
   }
else{ROS_INFO("SEARCHING FOR BALL");}
}

ROS_INFO("OUT OF LOOP");
}
int main(int argc, char** argv)
{
ros::init(argc,argv,"process_image");
ros::NodeHandle n;


client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
ros::Subscriber sub1 = n.subscribe("camera/rgb/image_raw",10,process_image_callback);

ros::spin();
return 0;

}









