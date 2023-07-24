# Installation for WSL2
sudo apt-get update
sudo apt install linux-tools-virtual hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip `ls /usr/lib/linux-tools/*/usbip | tail -n1` 20
sudo apt-get install -y gcc-arm-none-eabi	#Install ARM GCC EABI compiler
sudo apt-get install -y gdb-arm-none-eabi
sudo apt-get install -y libnewlib-arm-none-eabi	#Install ARM new library
#sudo apt-get remove modemmanager brltty	#Remove serial terminal interference
sudo adduser $USER dialout		#Add user permissions for dialout
sudo apt --fix-broken install -y
sudo apt-get install -y python3-pip
mkdir /mnt/c/csse3010/Downloads
wget -O /mnt/c/csse3010/Downloads/JLink_Linux_V762_x86_64.deb https://cloudstor.aarnet.edu.au/plus/s/alRMOQKscp2JX74/download
sudo dpkg -i /mnt/c/csse3010/Downloads/JLink_Linux_V762_x86_64.deb
sudo python3 -m pip install pylink-square
sudo ln -s /usr/bin/gdb-multiarch /usr/local/bin/arm-none-eabi-gdb
/bin/bash $PWD/sourcelib_env_linux.sh
