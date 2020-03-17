<!-- markdownlint-disable MD041 -->
[![LunarG][1]][2]

[1]: https://vulkan.lunarg.com/img/LunarGLogo.png "www.LunarG.com"
[2]: https://www.LunarG.com/

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

Copyright &copy; 2015-2019 LunarG, Inc.

# Trace and Replay Tools
This document describes the vktrace and vkreplay tools which are used for tracing and replaying Vulkan API calls.

## Tracing and Trace Files
The trace program is named `vktrace`. It is used to record an application's Vulkan API calls to a trace file. The call information is stored in the trace file in a compact binary format. The trace files normally have a  `.vktrace` suffix. The application can be either a local or remote application.

Options for the `vktrace` command are:

| Trace Option         | Description |  Default |
| -------------------- | ----------------- | --- |
| -a&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;Arguments&nbsp;&lt;string&gt; | Command line arguments to pass to the application to be traced | none |
| -o&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;OutputTrace&nbsp;&lt;string&gt; | Name of the generated trace file | `vktrace_out.vktrace` |
| -p&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;Program&nbsp;&lt;string&gt; | Name of the application to trace  | if not provided, server mode tracing is enabled |
| -ptm&nbsp;&lt;bool&gt;<br>&#x2011;&#x2011;PrintTraceMessages&nbsp;&lt;bool&gt; | Print trace messages to console | on |
| -s&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;Screenshot&nbsp;&lt;string&gt; | Frame numbers of which to take screen shots. String arg is one of:<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;comma separated list of frames<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;start&gt;-&nbsp;&lt;count&gt;-&nbsp;&lt;interval&gt; <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"all"  | no screenshots |
| -w&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;WorkingDir&nbsp;&lt;string&gt; | Alternate working directory | the application's directory |
| -P&nbsp;&lt;bool&gt;<br>&#x2011;&#x2011;PMB&nbsp;&lt;bool&gt; | Trace persistently mapped buffers | true |
| -tr&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;TraceTrigger&nbsp;&lt;string&gt; | Start/stop trim by hotkey or frame range. String arg is one of:<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;hotkey-[F1-F12\|TAB\|CONTROL]<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;frames-&lt;startframe&gt;-&lt;endframe&gt;| on |
| -tpp&nbsp;&lt;bool&gt;<br>&#x2011;&#x2011;TrimPostProcessing&nbsp;&lt;bool&gt; | Enable trim post-processing to make trimmed trace file smaller, see description of `VKTRACE_TRIM_POST_PROCESS` below | false |
| -tl&nbsp;&lt;bool&gt;<br>&#x2011;&#x2011;TraceLock&nbsp;&lt;bool&gt; | Enable locking of API calls during trace. Default is TRUE if trimming is enabled, FALSE otherwise. See description of `VKTRACE_ENABLE_TRACE_LOCK` below | See description |
| -v&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;Verbosity&nbsp;&lt;string&gt; | Verbosity mode - `quiet`, `errors`, `warnings`, `full`, or `max` | `errors` | The level of messages that should be logged.  The named level and below will be included.  The special value `max` always prints out all information available, and is generally equivalent to `full`.
| -tbs&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;TrimBatchSize&nbsp;&lt;string&gt; | Set the maximum trim commands batch size per command buffer, see description of `VKTRACE_TRIM_MAX_COMMAND_BATCH_SIZE` below  |  device memory allocation limit divided by 100 |

In local tracing mode, both the `vktrace` and application executables reside on the same system.

An example command to trace the sample Vulkan Cube application in local mode follows.

Linux shell:

```
$ vktrace -p ./vkcube -o cubetrace.vktrace
```

Windows Powershell:

```
PS> vktrace -p vkcube.exe -o cubetrace.vktrace
```
**Note:** Subsequent command examples in this document are written using Linux shell commands. These example commands can be translated and used as Windows Powershell commands.

Trace packets are written to the file `cubetrace.vktrace` in the local directory.  Output messages from the replay operation are written to `stdout`.

*Important*:  Subsequent `vktrace` runs with the same `-o` option value will overwrite the trace file, preventing the generation of multiple, large trace files.  Be sure to specify a unique output trace file name for each `vktrace` invocation if you do not desire this behaviour.

## Client/Server Mode
The tools also support tracing Vulkan applications in client/server mode, where the trace server resides on a local or a remote system.

### Server

In client/server mode, the `vktrace` *server* is started without specifying the `-p` or `--Program` option.  Invoked in this fashion, `vktrace` enters a waiting mode for trace packets.

```
$ mkdir examples/traces
$ cd examples/traces
$ vktrace -o cubetrace_s.vktrace
No program (-p) parameter found: Running vktrace as server.
```

The trace file will be written to `cubetrace_s.vktrace`. If additional programs are traced with this trace server, subsequent trace files will be named `cubetrace_s-<`_`N`_`>.vktrace`, with the trace server incrementing _`N`_ for each time the application is run.


### Client
The tracer is implemented as a Vulkan layer.  When tracing in server mode, the local or remote client must enable the `Vktrace` layer.  The `Vktrace` layer *must* be the first layer identified in the `VK_INSTANCE_LAYERS` lists.

```
$ export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_vktrace
```

#### Local Client
Local client/server mode is particularly useful for Vulkan applications with multiple runtime options or complicated startup.

In local client/server mode, the tracer layer must be enabled before the local client application to be traced is launched.


```
$ cd examples/build
$ export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_vktrace
$ ./vkcube
```
The generated trace file is found at `examples/traces/cubetrace_s.vktrace`.

*Note*:  The Vulkan Cube application is used to demonstrate tracing in client/server mode.  Vulkan Cube is a very simple application, and would not be traced in this mode in practice.

#### Remote Client
Remote client/server mode is useful if the client is a mobile device or running in a lightweight environment that might not have the disk or other capabilities to store large trace files.

In remote client/server mode, the remote client must additionally identify the IP address of the trace server system.

```
$ cd examples/build
$ export VKTRACE_LIB_IPADDR=<ip address of trace server system>
$ export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_vktrace
$ ./vkcube
```

The generated trace file is located at `examples/traces/cubetrace_s.vktrace` on the remote trace server system.

## Replay
The vkreplay command is used to replay a Vulkan application trace.

**Important**: Trace files generated with earlier versions of the vktrace tools may not be replayable with later versions of the tools. The trace file format has evolved over time - vkreplay attempts to identify incompatible versions during replay and can often successfully replay them, but it does not handle all such cases.

**Important**: Trace files generated on one GPU may or may not be replayable on other GPUs, as well as trace files generated on different OSes and different driver versions. vkreplay attempts to replay such trace files, translating API calls as needed for the new platform, but it does not handle all such cases.

The  `vkreplay` command-line  options are:

| Replay Option         | Description | Default |
| --------------------- | ----------- | ------- |
| -o&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;Open&nbsp;&lt;string&gt; | Name of trace file to open and replay | **required** |
| -l&nbsp;&lt;int&gt;<br>&#x2011;&#x2011;NumLoops&nbsp;&lt;int&gt; | Number of times to replay the trace file  | 1 |
| -lsf&nbsp;&lt;int&gt;<br>&#x2011;&#x2011;LoopStartFrame&nbsp;&lt;int&gt; | The start frame number of the loop range | 0 |
| -lef&nbsp;&lt;int&gt;<br>&#x2011;&#x2011;LoopEndFrame&nbsp;&lt;int&gt; | The end frame number of the loop range | the last frame in the tracefile |
| -c&nbsp;&lt;bool&gt;<br>&#x2011;&#x2011;CompatibilityMode&nbsp;&lt;bool&gt; | Enable compatibility mode - modify api calls as needed when replaying trace file created on different platform than replay platform. For example: Convert trace file memory indices to replay device memory indices. | true |
| -s&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;Screenshot&nbsp;&lt;string&gt; | Comma-separated list of frame numbers of which to take screen shots  | no screenshots |
| -sf&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;ScreenshotFormat&nbsp;&lt;string&gt; | Color Space format of screenshot files. Formats are UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB  | Format of swapchain image |
| -x&nbsp;&lt;bool&gt;<br>&#x2011;&#x2011;ExitOnAnyError&nbsp;&lt;bool&gt; | Exit if an error occurs during replay | false |
| -v&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;Verbosity&nbsp;&lt;string&gt; | Verbosity mode - "quiet", "errors", "warnings", or "full" | errors |
| Linux Only |  |  |
| -ds&nbsp;&lt;string&gt;<br>&#x2011;&#x2011;DisplayServer&nbsp;&lt;string&gt; | Display server - "xcb", or "wayland" | xcb |

To replay the Vulkan Cube application trace captured in the example above:

```
$ cd examples/build
$ vkreplay -o cubetrace.vktrace -l 5
```

If the trace is rather short, the replay may finish quickly.  Specify the `-l` or `--NumLoops` option to replay the trace `NumLoops` option value times.

Output messages from the replay operation are written to `stdout`.

#### Linux Display Server Support

To run vkreplay with a different display server implementation than XCB, the command-line option --DisplayServer (-ds) can be set. Currently, the available options are XCB and WAYLAND.

Example for running on Wayland:
```
vkreplay -o <tracefile> -ds wayland
```


## Replayer Interaction with Layers

The Vulkan validation layers may be enabled for trace replay.  Replaying a trace with layers activated provides many benefits.  Developers can take advantage of new validation capabilities as they are developed with older and existing trace files.

To activate specific layers on a trace replay, set the `VK_INSTANCE_LAYERS` environment variable to a colon-separated list of layer names before replaying the trace. Refer to the [Layers Overview and Configuration](https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/LAYER_CONFIGURATION.md) guide for additional information on layers and how to configure layer output options.

## Persistently Mapped Buffers and vktrace

If a Vulkan program uses persistently mapped buffers (PMB) that are allocated via vkMapMemory, vktrace can track changes to PMB and automatically copy modified PMB pages to the trace file, rather than requiring that the Vulkan program call vkFlushMappedMemoryRanges to specify what PMB buffers should be copied. On Windows, the trace layer detects changes to PMB pages by setting the `PAGE_GUARD` flag for mapped memory pages and installing an exception handler for `PAGE_GUARD` that keeps track of which pages have been modified.  On Linux, the trace layer detects changes by using mprotect to disable writes to mapped memory pages and installing a signal handler for SIGSEGV.

Tracking of changes to PMB using the above techniques is enabled by default. If you wish to disable PMB tracking, it can be disabled by with the `--PMB false` option to the vktrace command. Disabling PMB tracking can result in some mapped memory changes not being detected by the trace layer, a larger trace file, and/or slower trace/replay.

## Trace Tools Enviroment Variables

Several environment variables can be set to change the behavior of vktrace/vktrace layer:

 - `VKTRACE_PMB_ENABLE`

    VKTRACE_PMB_ENABLE enables tracking of PMB if its value is 1 or 2.  Other values disable PMB tracking.  Currently 2 is only used to enable using external host memory extension and memory write watch to capture PMB on Windows platform.  If this environment variable is not set, PMB tracking is enabled, same as setting VKTRACE_PMB_ENABLE to 1.  When creating a trace using client/server mode, set this variable to 0 when starting the client if you wish to disable PMB tracking.

 - `VKTRACE_PAGEGUARD_ENABLE_READ_PMB`

    VKTRACE_PAGEGUARD_ENABLE_READ_PMB enables read PMB support if set to a non-NULL value.  If PMB data changes comes from the GPU side, PMB tracking does not usually capture those changes. This environment  variable is used to enable capture of such GPU initiated PMB data changes. It is supported only on Windows.

 - `VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS`

    VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS, when set to a non-null value, enables post-processing  when read PMB support is enabled.  When VKTRACE_PAGEGUARD_ENABLE_READ_PMB is set, PMB processing will sometimes miss writes following reads if writes occur on the same page as a read. Set this environment variable to enable post-processing to fix missed PMB writes. It is supported only on Windows.

 - `VKTRACE_TRIM_POST_PROCESS`

    VKTRACE_TRIM_POST_PROCESS enables post-processing of trim if its value is 1.  Other values disable trim post-processing.  Disable post-processing means the trimmed trace file will record all the not destroyed objects whether they are used/referenced in the trim frame range or not.  Enable post-processing will drop most of the pre-trim objects which are not used/referenced in the trim frame range.  Set this environment variable to 1 to enable post-processing of trim to generate a smaller trace file and eliminate most useless pre-trim objects and Vulkan calls.  Do NOT enable trim post-processing when there's a large trim frame range because both the referenced pre-trim data and in-trim data are kept in memory until writing to trace file in the trim end frame which may exceeds the system memory.

 - `VKTRACE_TRIM_MAX_COMMAND_BATCH_SIZE`

    VKTRACE_TRIM_MAX_COMMAND_BATCH_SIZE sets the maximum number of commands batched during trim resources upload (images and buffers recreation). The range is 1 - device memory allocation limit. This enviroment variable is used to reduce the number of  command buffers allocated  by batching the commands execution according to the size set. 

 - `VKTRACE_ENABLE_TRACE_LOCK`
 
    VKTRACE_ENABLE_TRACE_LOCK enables locking of API calls during trace if set to a non-null value. Not setting this variable will sometimes result in race conditions and remap errors during replay. Setting this variable will avoid those errors, with a slight performance loss during tracing. Locking of API calls is always enabled when trimming is enabled.

## Android

### vktrace

To record a trace on Android, enable port forwarding from the device to the host:
```
adb reverse localabstract:vktrace tcp:34201
```
Start up vktrace on the host in server mode:
```
vktrace -v full -o cube.vktrace
```
Run your application with the following layer enabled:
```
VK_LAYER_LUNARG_vktrace
```
The trace will be recorded on the host.

### vkreplay

To replay a trace, push the trace to your device
```
adb push cube.vktrace /sdcard/cube.vktrace
```
Grant vkreplay the ability to read it
```
adb shell pm grant com.example.vkreplay android.permission.READ_EXTERNAL_STORAGE
adb shell pm grant com.example.vkreplay android.permission.WRITE_EXTERNAL_STORAGE
```
And start the native activity
```
adb shell am start -a android.intent.action.MAIN -c android-intent.category.LAUNCH -n com.example.vkreplay/android.app.NativeActivity --es args "-v\ full\ -t\ /sdcard/cube.vktrace"
```
