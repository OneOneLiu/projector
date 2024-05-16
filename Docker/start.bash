xhost local:root
XAUTH=/tmp/.docker.xauth
docker run --rm -it \
    --name=projector_container\
    --env="DISPLAY=$DISPLAY" \
    --env="QT_X11_NO_MITSHM=1" \
    --env="XAUTHORITY=$XAUTH" \
    --volume="/home/$USER/catkin_ws/src/projector:/catkin_ws/src/projector" \
    --volume="/dev/bus/usb:/dev/bus/usb" \
    --net=host \
    --privileged \
    projector \
    bash

echo "Done."