#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/kingheron/kingheron_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/kingheron/kingheron_ws/install/lib/python2.7/dist-packages:/home/kingheron/kingheron_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/kingheron/kingheron_ws/build" \
    "/usr/bin/python2" \
    "/home/kingheron/kingheron_ws/src/heron_robot/heron_bringup/setup.py" \
     \
    build --build-base "/home/kingheron/kingheron_ws/build/heron_robot/heron_bringup" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/kingheron/kingheron_ws/install" --install-scripts="/home/kingheron/kingheron_ws/install/bin"
