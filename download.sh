#!/bin/bash
curl -s https://packagecloud.io/install/repositories/dirk-thomas/vcstool/script.deb.sh | sudo bash
sudo apt-get update
sudo apt-get install python3-vcstool
vcs import --input https://raw.githubusercontent.com/ros2/ros2/kilted/ros2.repos src
