<!-- markdownlint-disable MD041 -->
[![LunarG][1]][2]

[1]: https://vulkan.lunarg.com/img/LunarGLogo.png "www.LunarG.com"
[2]: https://www.LunarG.com/

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

Copyright &copy; 2015-2019 LunarG, Inc.

# VK\_LAYER\_LUNARG\_screenshot
The `VK_LAYER_LUNARG_screenshot` layer records frames to image files.

####VK_SCREENSHOT_FRAMES
The environment variable `VK_SCREENSHOT_FRAMES` can be set to a comma-separated list of frame numbers. When the frames corresponding to these numbers are presented, the screenshot layer will record the image buffer to PPM files. For example, if `VK_SCREENSHOT_FRAMES` is set to "4,8,15,16,23,42", the files created will be: 4.ppm, 8.ppm, 15.ppm, etc. `VK_SCREENSHOT_FRAMES` can also be set to a range of frames by specifying two numbers separated by a dash. The first number is the first frame and the second number is the number of frames. For example, if it is set to "20-3", the files created will be 20.ppm, 21.ppm, and 22.ppm.

####VK_SCREENSHOT_DIR
The environment variable `VK_SCREENSHOT_DIR` can be set to specify the directory in which to create the screenshot files. If it is not set or is set to null, the files will be created in the current working directory.

## Android

Frame numbers can be specified with the debug.vulkan.screenshot property:

```
adb shell setprop debug.vulkan.screenshot <framenumbers>
```

The directory in which to create the image files can be specified with the debug.vulkan.dir property:

```
adb shell setprop debug.vulkan.dir <directory>
```
If debug.vulkan.dir is not set or it is set to an empty string, the value of debug.vulkan.dir will default to "/sdcard/Android".

For production builds, if the files are to be written to external storage, make sure your application is able to read and write external storage by adding the following to AndroidManifest.xml:

```xml
<!-- This allows writing log files to sdcard -->
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
```

You may also need to grant it access with package manager.  For example, using the
Vulkan Cube demo from [Khronos/Vulkan-Tools](https://github.com/KhronosGroup/Vulkan-Tools):

```
adb shell pm grant com.example.VkCube android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.VkCube android.permission.WRITE_EXTERNAL_STORAGE
```
