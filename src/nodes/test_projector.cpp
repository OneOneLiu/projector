#include <LightCraftProjector.h>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace amnl;

int main(int argc, char **argv)
{
	CLightCraftProjector* pj = new CLightCraftProjector;
	pj -> Open();
	std::cout<<pj->IsOpen()<<"\n";
	pj->SetFrameParam(130000,9000);
	pj->SetIntensity(125);
	// pj->ProjectCrossPatternContinue();

	ros::init(argc, argv, "Projector_listener");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("chatter", 1000, &CLightCraftProjector::commandCallback, pj);
	ros::spin();

	return 0;
}