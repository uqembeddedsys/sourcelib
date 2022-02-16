sudo apt-get update
sudo apt-get install gcc-arm-none-eabi	#Install ARM GCC EABI compiler
sudo apt-get install gdb-arm-none-eabi
sudo apt-get install libnewlib-arm-none-eabi	#Install ARM new library
sudo apt-get install gitg		#Git management
sudo apt-get remove modemmanager brltty	#Remove serial terminal interference
sudo adduser $USER dialout		#Add user permissions for dialout
sudo apt-get install python3-pip
mkdir $HOME/Downloads
wget -O $HOME/Downloads/JLink_Linux_V696_x86_64.deb https://cloudstor.aarnet.edu.au/plus/s/1nCIZgLudE9s9Dw/download
sudo dpkg -i $HOME/Downloads/JLink_Linux_V696_x86_64.deb
sudo python3 -m pip install pylink-square
sudo ln -s /usr/bin/gdb-multiarch /usr/local/bin/arm-none-eabi-gdb
$PWD/sourcelib_env.sh