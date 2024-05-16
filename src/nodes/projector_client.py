#!/usr/bin/env python3

import rospy
from projector.srv import Projection, ProjectionRequest, ProjectionResponse

def request_projection():
    rospy.init_node('projection_client')
    rospy.wait_for_service('make_projections')
    try:
        projection_service = rospy.ServiceProxy('make_projections', Projection)
        request = ProjectionRequest()
        response = projection_service(request)
        if response.success:
            rospy.loginfo("Projection successful")
        else:
            rospy.loginfo("Projection failed")
    except rospy.ServiceException as e:
        rospy.logerr("Service call failed: %s", e)

if __name__ == '__main__':
    request_projection()
