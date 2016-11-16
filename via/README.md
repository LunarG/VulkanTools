#![LunarG's Vulkan Installation Analyzer (VIA)](images/lunarg_via_title.png)
This document is an overview of how to use the LunarG Vulkan Installation Analyzer (VIA).
VIA is a tool that can:
 1. Determine the state of Vulkan components on your system
 2. Validate that your Vulkan Loader and drivers are installed properly
 3. Capture your system state in a a form that can be used as an attachment when submitting bugs

 This document describes how to run it, and how to understand the resulting command line output that is generated.

<BR />

## Building
VIA is part of the LunarG [VulkanTools](https://github.com/LunarG/VulkanTools) GitHub respository.
Since it is one of many tools in this repository, it is built with all the other
tools by following the instructions in the [BuilidVT.md](https://github.com/LunarG/VulkanTools/blob/master/BUILDVT.md) file at the top of the
source tree.

<BR />

## Running
To run VIA, simply call the executable.

```
via
```

If you are having a problem running a particular Vulkan application, then on Windows you can run VIA
from the start menu (in which case it will output to the desktop), or on any OS you can change to
the directory of the application and run VIA from where it is installed on your system. The latter
choice will make sure VIA sees exactly what the Vulkan application sees.

This will output two things:
 - A command-line ouptut indicating the overall status
 - An HTML file (called via.html) containing the details which will be output to one of two locations:
  1. If the current directory is writeable, the HTML will be placed in that location.
  2. Otherwise, it will be saved to your home folder.

Your home folder is the following location (based on your OS):
 - Windows: Wherever your environment variables %HOMEDRIVE%\%HOMEPATH% point to.
 - Linux: It will be placed in your home folder ("~/.").

#### Additional command-line arguments
There are additional command-line parameters which can be used.  These simply augment existing behavior and do not capture any more information.
The available command-line arguments are:

##### --unique_output
The --unique_output argument, if provided, will cause the output html to be generated with a date/time suffix.  This will allow you to perform
multiple state captures on your system without accidentally erasing previous results.  The new file has the following format:

_via_YYYY_MM_DD_HH_MM.html_

Where each component stands for the numeric values for year (YYYY), month (MM), day (DD), hour (HH), and minute (MM).

#### --output_path
The --output_path arument allows the user to specify a location for the output html file. For
example, if the user runs `via --output_path /home/me/Documents`, then the output file will be
`/home/me/Documents/via.html`.

<BR />

## Common Command-Line Outputs

#### "SUCCESS: Validation completed properly"

##### Problem:
LunarG's VIA could detect no problems with your setup.

##### Possible Reason:
Your system is likely setup properly.  If you have trouble running Vulkan from another location, it could be that your environment variables aren't setup properly.

##### Next Step:
Re-run VIA from the location your Vulkan application/game is supposed to run.


#### "ERROR: Failed to find Vulkan Driver JSON in registry"

##### Problem:
This is a Windows-specific error that indicates that no Vulkan Driver JSON files were referenced in the appropriate place in the Windows Registry.

##### Possible Reason:
This can indicate that a Vulkan driver failed to install properly.

##### Next Step:
You should follow up with your Graphics Driver.  See more details below.


#### "ERROR: Failed to find Vulkan Driver JSON"

##### Problem:
The Vulkan loader on your system failed to find any Vulkan Driver JSON files in the appropriate places.

##### Possible Reason:
This can indicate that a Vulkan driver failed to install properly.

##### Next Step:
You should follow up with your Graphics Driver.  See more details below.


#### "ERROR: Failed to find Vulkan Driver Lib"

##### Problem:
The Vulkan loader on your system found at least one Vulkan Driver JSON file, but the Driver library referenced in that file appears invalid.

##### Possible Reason:
This can indicate that a Vulkan driver failed to install properly.

##### Next Step:
You should follow up with your Graphics Driver.  See more details below.


#### "ERROR: Vulkan failed to find a compatible driver"

##### Problem:
All the components appeared to be in place to allow Vulkan to load (from an external stand-point), but the loader still failed to find
a Vulkan compatible driver.

##### Possible Reason:
This can indicate that either a Vulkan driver failed to install properly, or the run-time is failing for some reason.

##### Next Step:
First, attempt to re-install the run-time by visiting [LunarXchange](https://vulkan.lunarg.com/), and install the latest Vulkan SDK.
If that does not work, try to see if there is a newer Vulkan driver available and install that.  If that still fails, file an issue on
LunarXchange.


#### [WINDOWS] Dialog box pop's up indicating "vulkan-1.dll is missing from your computer."

##### Problem:
The Vulkan loader "vulkan-1.dll" couldn't be found on your system.  This file is typically installed with some Vulkan driver installs,
some Vulkan-capable games, or the LunarG Vulkan SDK.

##### Possible Reason:
The last Vulkan Runtime install that executed on your system failed to behave properly.  Or, you have never installed a Vulkan loader
on your system.

##### Next Step:
To rememdy this, visit [LunarXchange](https://vulkan.lunarg.com/), and install the latest Vulkan SDK.  If that does not work, file an
Issue to have one of their engineers assist you.


#### [LINUX] Message indicating "error while loading shared libraries: libvulkan.so.1"

##### Problem:
The Vulkan loader "libvulkan.so.1" couldn't be found on your system.  This file is typically installed with some Vulkan driver installs,
some Vulkan-capable games, or the LunarG Vulkan SDK.

##### Possible Reason:
The last Vulkan Runtime install that executed on your system failed to behave properly.  Or, you have never installed a Vulkan loader

<BR />

## Vulkan Graphics Driver Problems
If the problem is possibly related to your Graphics Driver, it could be for several reasons:
 1. The hardware you have doesn't support Vulkan.
 2. Your hardware supports Vulkan, but you haven't yet installed a driver with Vulkan support.
 3. There is no Vulkan driver with support for the OS on which you are currently running.
    - Sometimes, the company may provide Vulkan support for some devices on one Operatings System (say Windows), while still waiting to complete Vulkan on other systems.
 4. Everything supports Vulkan, but the driver failed to install properly.

Before approaching your Graphics driver vendor, it would help if you verified that your current driver for your current hardware on your current
operating system **does** support Vulkan.