# VK\_LAYER\_LUNARG\_screenshot
The `VK_LAYER_LUNARG_screenshot` layer records frames to image files. The environment variable `VK_SCREENSHOT_FRAMES` can be set to a comma-separated list of frame numbers. When the frames corresponding to these numbers are presented, the screenshot layer will record the image buffer to PPM files in the working directory. For example, if `VK_SCREENSHOT_FRAMES` is set to "4,8,15,16,23,42", the files created will be: 4.ppm, 8.ppm, 15.ppm, etc.

Checks include:
 - validating that handles used are valid
 - if an extension's function is used, it must have been enabled (including for the appropriate `VkInstance` or `VkDevice`)
 - the query functions must be called before creating a swapchain
 - the results of the query functions are compared with the values used to create a swapchain
 - all `VkSwapchainKHR`s associated with a `VkDevice` must be destroyed before the `VkDevice` is destroyed
 - applications should not call `vkAcquireNextImageKHR()` so many times that the call cannot succeed
 - applications must own an image (i.e. by calling `vkAcquireNextImageKHR()`) in order to present
 - image indices must be within range.

### Android
To enable, set a property that contains target frame:
```
adb shell setprop debug.vulkan.screenshot <framenumber>
```
For production builds, be sure your application has access to read and write to external storage by adding the following to AndroidManifest.xml:
```
<!-- This allows writing log files to sdcard -->
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE"/>
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
```
You may also need to grant it access with package manager:
```
adb shell pm grant com.example.Cube android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.Cube android.permission.WRITE_EXTERNAL_STORAGE
```
Result screenshot will be in:
```
/sdcard/Android/<framenumber>.ppm
```
