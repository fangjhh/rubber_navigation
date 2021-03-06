//
// Created by xcy on 2019/12/12.
//
#include "ros/ros.h"
#include "rubber_navigation/BaseController.h"
#include "ros/package.h"
#include "../../Visual-Servo/include//visual_servo/JoyTeleop.h"
#include "../../Visual-Servo/include/visual_servo/parameterTeleop.h"
int main(int argc,char* argv[])
{
    ros::init(argc,argv,"baseOnly");
    ros::NodeHandle nh_("~");
    const std::string parameter_addr{ros::package::getPath("rubber_navigation")+"/config/BaseModel.yaml"};
    std::string base_foot_print,odom_frame,map_frame,serial_addr;
    bool publish_tf;

    nh_.param("publish_tf",publish_tf,(bool)false);
    nh_.param("base_foot_print",base_foot_print,(std::string)"base_link");
    nh_.param("odom_frame",odom_frame,(std::string)"odom");
    nh_.param("serial_addr",serial_addr,(std::string)"/dev/ttyS1");

	const std::vector<std::string> parameterNames{"/visual_servo/clockGo","/visual_servo/antiClockGo","/visual_servo/knifeOn","/visual_servo/knifeOff","/visual_servo/knifeUnplug","/visual_servo/singleClockGo","/visual_servo/singleAntiClockGo"};

 	ParameterListener parameterListener(40,8);
	parameterListener.registerParameterCallback(parameterNames,false);

    BaseController baseController(serial_addr,B115200,base_foot_print,odom_frame,publish_tf);
    baseController.setBaseModel(parameter_addr);

    ros::AsyncSpinner spinner(2);
    spinner.start();
    ros::Rate loop_rate(30);



    while(ros::ok())
    {

		if((bool)parameterListener.parameters()[0])
		{
		    baseController.sendCommand(BaseController::CLOCK_GO);
			ros::param::set(parameterNames[0],(double)false);
		}
		else if((bool)parameterListener.parameters()[1])
		{
		    baseController.sendCommand(BaseController::ANTI_CLOCK_GO);
		    ros::param::set(parameterNames[1],(double)false);
		}
		else if((bool)parameterListener.parameters()[2])
		{
		    baseController.sendCommand(BaseController::KNIFE_ON);
		    ros::param::set(parameterNames[2],(double)false);
		}
		else if((bool)parameterListener.parameters()[3])
		{
		    baseController.sendCommand(BaseController::KNIFE_OFF);
		    ros::param::set(parameterNames[3],(double)false);
		}
		else if((bool)parameterListener.parameters()[4])
		{
		    baseController.sendCommand(BaseController::KNIFE_UNPLUG);
		    ros::param::set(parameterNames[4],(double)false);
		}
		else if((bool)parameterListener.parameters()[5])
		{
		    baseController.sendCommand(BaseController::SINGLE_CLOCK_GO);
			ROS_INFO("get single clock go");
		    ros::param::set(parameterNames[5],(double)false);
		}
		else if((bool)parameterListener.parameters()[6])
		{
		    baseController.sendCommand(BaseController::SINGLE_ANTI_CLOCK_GO);
		    ros::param::set(parameterNames[6],(double)false);
		}

        ros::spinOnce();
        loop_rate.sleep();
    }

}
