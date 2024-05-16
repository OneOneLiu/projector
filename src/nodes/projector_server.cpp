#include <LightCraftProjector.h>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"

using namespace amnl;

#include <ros/ros.h>
#include <projector/Projection.h> //这里的头文件跟srv文件名保持一致

bool handle_projections(projector::Projection::Request &req,
                        projector::Projection::Response &res,
                        CLightCraftProjector* pj) {
    if (pj->IsOpen()) {
        pj->SetFrameParam(130000, 9000);
        pj->SetIntensity(125);

        // 投影一次
        pj->ProjectScanImagesOnce();
		ROS_INFO("Scanned once.");

        res.success = true;
    } else {
        res.success = false;
    }
    return true;
}

int main(int argc, char** argv) {
	ros::init(argc, argv, "projector_server_node");
    ros::NodeHandle nh;

	// 实例化投影仪对象
    CLightCraftProjector pj;
    if (pj.Open()) {
        ROS_INFO("Projector successfully opened.");
    } else {
        ROS_ERROR("Failed to open projector.");
        return 1;  // 退出程序，因为投影仪未能成功打开
    }

    // 创建服务服务器，并传递投影仪对象的指针
    ros::ServiceServer service = nh.advertiseService<projector::Projection::Request, projector::Projection::Response>(
        "make_projections", boost::bind(handle_projections, _1, _2, &pj));

    ros::spin();
    return 0;
}