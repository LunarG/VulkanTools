# Additional Linux Configuration for the Sample Intel Vulkan Driver

The sample intel Vulkan driver in this repo uses DRI3 for its window system interface.
This requires extra configuration of Ubuntu systems.

You may need to install the following packages:
```
sudo apt-get install git subversion cmake libgl1-mesa-dev freeglut3-dev libglm-dev libmagickwand-dev qt5-default libpciaccess-dev libpthread-stubs0-dev libudev-dev bison graphviz libpng-dev python3-lxml
sudo apt-get build-dep mesa
```

## Linux Render Nodes

The render tests depend on access to DRM render nodes.
To make that available, a couple of config files need to be created to set a module option
and make accessible device files.
The system will need to be rebooted with these files in place to complete initialization.
These commands will create the config files.

```
sudo tee /etc/modprobe.d/drm.conf << EOF
# Enable render nodes
options drm rnodes=1
EOF
# this will add the rnodes=1 option into the boot environment
sudo update-initramfs -k all -u
```
```
sudo tee /etc/udev/rules.d/drm.rules << EOF
# Add permissions to render nodes
SUBSYSTEM=="drm", ACTION=="add", DEVPATH=="/devices/*/renderD*", MODE="020666"
EOF
```
## DRI 3
Find your Ubuntu release below:

### Ubuntu 14.04.3 LTS support of DRI 3

Ubuntu 14.04.3 LTS does not ship a xserver-xorg-video-intel package with supported DRI 3 on intel graphics.
The xserver-xorg-video-intel package can be built from source with DRI 3 enabled.
Use the following commands to enable DRI3 on ubuntu 14.04.3 LTS.

- Install packages used to build:
```
sudo apt-get update
sudo apt-get dist-upgrade
sudo apt-get install devscripts
sudo apt-get build-dep xserver-xorg-video-intel-lts-vivid
```

- Get the source code for xserver-xorg-video-intel-lts-vivid
```
mkdir xserver-xorg-video-intel-lts-vivid_source
cd xserver-xorg-video-intel-lts-vivid_source
apt-get source xserver-xorg-video-intel-lts-vivid
cd xserver-xorg-video-intel-lts-vivid-2.99.917
debian/rules patch
quilt new 'enable-DRI3'
quilt edit configure.ac
```

- Use the editor to make these changes.
```
--- a/configure.ac
+++ b/configure.ac
@@ -340,9 +340,9 @@
 	      [DRI2=yes])
 AC_ARG_ENABLE(dri3,
 	      AS_HELP_STRING([--enable-dri3],
-			     [Enable DRI3 support [[default=no]]]),
+			     [Enable DRI3 support [[default=yes]]]),
 	      [DRI3=$enableval],
-	      [DRI3=no])
+	      [DRI3=yes])
 AC_ARG_ENABLE(xvmc, AS_HELP_STRING([--disable-xvmc],
                                   [Disable XvMC support [[default=yes]]]),
```
- Build and install xserver-xorg-video-intel-lts-vivid
```
quilt refresh
debian/rules clean
debuild -us -uc
sudo dpkg -i ../xserver-xorg-video-intel-lts-vivid_2.99.917-1~exp1ubuntu2.2~trusty1_amd64.deb
```
- Prevent updates from replacing this version of the package.
```
sudo bash -c 'echo xserver-xorg-video-intel-lts-vivid hold | dpkg --set-selections'
```
- save your work then restart the X server with the next command.
```
sudo service lightdm restart
```
- After logging in again, check for success with this command and look for DRI3.
```
xdpyinfo | grep DRI
```

### Ubuntu 14.10 support of DRI 3

Warning: Recent versions of 14.10 have **REMOVED** DRI 3.
Version: 2:2.99.914-1~exp1ubuntu4.1 is known to work.
To see status of this package:
```
dpkg -s xserver-xorg-video-intel
```

Note:
Version 2:2.99.914-1~exp1ubuntu4.2 does not work anymore.
To install the working driver from launchpadlibrarian.net:
- Remove the current driver:
```
sudo apt-get purge xserver-xorg-video-intel
```
- Download the old driver:
```
wget http://launchpadlibrarian.net/189418339/xserver-xorg-video-intel_2.99.914-1%7Eexp1ubuntu4.1_amd64.deb
```
- Install the driver:
```
sudo dpkg -i xserver-xorg-video-intel_2.99.914-1~exp1ubuntu4.1_amd64.deb
```
- Pin the package to prevent updates
```
sudo bash -c "echo $'Package: xserver-xorg-video-intel\nPin: version 2:2.99.914-1~exp1ubuntu4.1\nPin-Priority: 1001' > /etc/apt/preferences.d/xserver-xorg-video-intel"
```

- Either restart Ubuntu or just X11.


### Ubuntu 15.04 support of DRI 3

Ubuntu 15.04 has never shipped a xserver-xorg-video-intel package with supported DRI 3 on intel graphics.
The xserver-xorg-video-intel package can be built from source with DRI 3 enabled.
Use the following commands to enable DRI3 on ubuntu 15.04.

- Install packages used to build:
```
sudo apt-get update
sudo apt-get dist-upgrade
sudo apt-get install devscripts
sudo apt-get build-dep xserver-xorg-video-intel
```

- Get the source code for xserver-xorg-video-intel
```
mkdir xserver-xorg-video-intel_source
cd xserver-xorg-video-intel_source
apt-get source xserver-xorg-video-intel
cd xserver-xorg-video-intel-2.99.917
debian/rules patch
quilt new 'enable-DRI3'
quilt edit configure.ac
```

- Use the editor to make these changes.
```
--- a/configure.ac
+++ b/configure.ac
@@ -340,9 +340,9 @@
 	      [DRI2=yes])
 AC_ARG_ENABLE(dri3,
 	      AS_HELP_STRING([--enable-dri3],
-			     [Enable DRI3 support [[default=no]]]),
+			     [Enable DRI3 support [[default=yes]]]),
 	      [DRI3=$enableval],
-	      [DRI3=no])
+	      [DRI3=yes])
 AC_ARG_ENABLE(xvmc, AS_HELP_STRING([--disable-xvmc],
                                   [Disable XvMC support [[default=yes]]]),
```
- Build and install xserver-xorg-video-intel
```
quilt refresh
debian/rules clean
debuild -us -uc
sudo dpkg -i ../xserver-xorg-video-intel_2.99.917-1~exp1ubuntu2.2_amd64.deb
```
- Prevent updates from replacing this version of the package.
```
sudo bash -c 'echo xserver-xorg-video-intel hold | dpkg --set-selections'
```
- save your work then restart the X server with the next command.
```
sudo service lightdm restart
```
- After logging in again, check for success with this command and look for DRI3.
```
xdpyinfo | grep DRI
```
### Ubuntu 15.10 support of DRI 3

Ubuntu 15.10 has never shipped a xserver-xorg-video-intel package with supported DRI 3 on intel graphics.
The xserver-xorg-video-intel package can be built from source with DRI 3 enabled.
Use the following commands to enable DRI3 on ubuntu 15.10.

- Install packages used to build:
```
sudo apt-get update
sudo apt-get dist-upgrade
sudo apt-get install devscripts
sudo apt-get build-dep xserver-xorg-video-intel
```

- Get the source code for xserver-xorg-video-intel
```
mkdir xserver-xorg-video-intel_source
cd xserver-xorg-video-intel_source
apt-get source xserver-xorg-video-intel
cd xserver-xorg-video-intel-2.99.917+git20150808
debian/rules patch
quilt new 'enable-DRI3'
quilt edit configure.ac
```

- Use the editor to make these changes.
```
Index: xserver-xorg-video-intel-2.99.917+git20150808/configure.ac
===================================================================
--- xserver-xorg-video-intel-2.99.917+git20150808.orig/configure.ac
+++ xserver-xorg-video-intel-2.99.917+git20150808/configure.ac
@@ -356,7 +356,7 @@ AC_ARG_WITH(default-dri,
            AS_HELP_STRING([--with-default-dri],
                           [Select the default maximum DRI level [default 2]]),
              [DRI_DEFAULT=$withval],
-             [DRI_DEFAULT=2])
+             [DRI_DEFAULT=3])
 if test "x$DRI_DEFAULT" = "x0"; then
        AC_DEFINE(DEFAULT_DRI_LEVEL, 0,[Default DRI level])
 else
```
- Build and install xserver-xorg-video-intel
```
quilt refresh
debian/rules clean
debuild -us -uc
sudo dpkg -i ../xserver-xorg-video-intel_2.99.917+git20150808-0ubuntu4_amd64.deb
```
- Prevent updates from replacing this version of the package.
```
sudo bash -c 'echo xserver-xorg-video-intel hold | dpkg --set-selections'
```
- save your work then restart the X server with the next command.
```
sudo service lightdm restart
```
- After logging in again, check for success with this command and look for DRI3.
```
xdpyinfo | grep DRI
```

