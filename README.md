OpenCorsairLink
===============
Linux and Mac OS support for the H100i Water Cooler  
Written using Code and info from: 
http://forum.corsair.com/forums/showthread.php?t=120092  
## Warning: This was a weekend project after buying the H100i for a Linux Server. Some or most things might not work for you!
## Warning: OpenCorsairLinkGui is a WIP, and therefore Does Not Work!

### Dependencies
#### Ubuntu
```bash
sudo apt install libqt4-dev libhidapi-dev gdb
```

### Building  
```bash
git submodule update --init  
qmake OpenCorsairLink.pro  
make
```  

### Running
As a regular user: `sudo ./OpenCorsairLinkCli -p`,
dash i for firmware info of device,
dash p for printing fan, temp, and LED status.  

As root user: `./OpenCorsairLinkCli -p`  
