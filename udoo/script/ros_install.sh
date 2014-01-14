sudo sh -c 'echo "deb http://packages.ros.org/ahendrix-mirror/ubuntu precise main" > /etc/apt/sources.list.d/ros-latest.list'
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
sudo apt-get update
sudo apt-get install ros-groovy-ros-base
sudo apt-get install python-rosdep
sudo rosdep init
rosdep update
sudo apt-get install python-rosinstall

# Can be customized
echo "source /opt/ros/groovy/setup.bash" >> ~/.bashrc
source ~/.bashrc
