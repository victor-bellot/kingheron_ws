#!/usr/bin/env python

from gazebo_msgs.srv import SpawnModel
from heron_srvs.srv import AskModel, AskModelResponse
from geometry_msgs.msg import Pose
import rospkg
import rospy


def spawn_buoy(request):
    initial_pose = Pose()
    initial_pose.position = request.position

    ros_pack = rospkg.RosPack()
    model_path = ros_pack.get_path('heron_mission') + '/models/buoy/buoy.urdf'

    spawn_model_client = rospy.ServiceProxy('/gazebo/spawn_urdf_model', SpawnModel)
    resp = spawn_model_client(
        model_name=request.model_name,
        model_xml=open(model_path, 'r').read(),
        robot_namespace='',
        initial_pose=initial_pose,
        reference_frame=''
    )

    return AskModelResponse(resp.success, resp.status_message)


def spawn_obstacle(request):
    initial_pose = Pose()
    initial_pose.position = request.position

    ros_pack = rospkg.RosPack()
    model_path = ros_pack.get_path('heron_mission') + '/models/obstacle/obstacle.urdf'

    spawn_model_client = rospy.ServiceProxy('/gazebo/spawn_urdf_model', SpawnModel)
    resp = spawn_model_client(
        model_name=request.model_name,
        model_xml=open(model_path, 'r').read(),
        robot_namespace='',
        initial_pose=initial_pose,
        reference_frame=''
    )

    return AskModelResponse(resp.success, resp.status_message)


if __name__ == '__main__':
    rospy.init_node("model_manager")

    buoy_srv = rospy.Service('model_manager/spawn_buoy', AskModel, spawn_buoy)
    obst_srv = rospy.Service('model_manager/spawn_obstacle', AskModel, spawn_obstacle)

    rospy.spin()
