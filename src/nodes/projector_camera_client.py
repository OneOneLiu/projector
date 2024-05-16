#!/usr/bin/env python3

import rospy
from projector.srv import Projection, ProjectionRequest, ProjectionResponse
from basler_camera.srv import Reset_parameters, Reset_parametersRequest

def request_projection():
    rospy.init_node('projection_client')
    # 等待投影服务
    rospy.wait_for_service('make_projections')
    # 等待相机参数重置服务
    rospy.wait_for_service('reset_parameters')
    try:
        projection_service = rospy.ServiceProxy('make_projections', Projection)
        request = ProjectionRequest()
        response = projection_service(request)
        if response.success:
            # 如果投影成功，调用相机参数重置服务
            rospy.loginfo("Projection successful")
            try:
                reset_parameters = rospy.ServiceProxy('reset_parameters', Reset_parameters)
                request = Reset_parametersRequest()
                response = reset_parameters(request)
                if response.success:
                    rospy.loginfo("Successfully reset parameters")
                else:
                    rospy.loginfo("Failed to reset parameters")
            except rospy.ServiceException as e:
                rospy.logerr("Service call failed: %s", e)
        else:
            rospy.loginfo("Projection failed")
    except rospy.ServiceException as e:
        rospy.logerr("Service call failed: %s", e)

if __name__ == '__main__':
    request_projection()