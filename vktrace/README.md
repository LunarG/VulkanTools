<article class="markdown-body">

# [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-trace-and-replay-tools)Trace and Replay Tools

This folder contains the source for the Vulkan trace and replay tools, as well as associated libraries. The trace tools are built as part of the top level VulkanTools build. To build the trace tools, follow the build directions for the top level VulkanTools project build in BUILDVT.md.

## [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-tracing-and-trace-files)Tracing and Trace Files

The vktrace tool provides the capability to trace Vulkan applications.  vktrace records an application's Vulkan API calls to a trace file, usually to a file with a `.vktrace` suffix. The call information is stored in the trace file in a compact binary format.

 The options for the `vktrace` command are:

<table>

<thead>

<tr>

<th>Trace Option</th>

<th>Description</th>

<th>Default</th>

</tr>

</thead>

<tbody>

<tr>

<td>-a &lt;string&gt;<br/>
‑‑Arguments &lt;string&gt;</td>

<td>Command line arguments to pass to the application to be traced</td>

<td>none</td>

</tr>

<tr>

<td>-o &lt;string&gt;<br/>
‑‑OutputTrace &lt;string&gt;</td>

<td>Name of the generated trace file</td>

<td>vktrace_out.vktrace</td>

</tr>

<tr>

<td>-p &lt;string&gt;<br/>
‑‑Program &lt;string&gt;</td>

<td>Name of the application to trace</td>

<td>if not provided, server mode tracing is enabled</td>

</tr>

<tr>

<td>-ptm &lt;bool&gt;<br/>
‑‑PrintTraceMessages &lt;bool&gt;</td>

<td>Print trace messages to console</td>

<td>on</td>

</tr>

<tr>

<td>-s &lt;string&gt;<br/>  
‑‑Screenshot &lt;string&gt;</td>

<td>Comma-separated list of frame numbers of which to take screen shots</td>

<td>no screenshots</td>

</tr>

<tr>

<td>-w &lt;string&gt;<br/>  
‑‑WorkingDir &lt;string&gt;</td>

<td>Alternate working directory</td>

<td>the application's directory</td>

</tr>

<tr>

<td>-P &lt;bool&gt;<br/>  
‑‑PMB &lt;bool&gt;</td>

<td>Optimize trace of persistently mapped buffers</td>

<td>on</td>

</tr>

<tr>

<td>-v &lt;string&gt;<br/>  
‑‑Verbosity &lt;string&gt;</td>

<td>Verbosity mode - "quiet", "errors", "warnings", or "full"</td>

<td>errors</td>

</tr>

<td>-tr &lt;string&gt;<br/>  
‑‑TraceTrigger &lt;string&gt;</td>

<td>Start/stop trim by hotkey or frame range.<br/>String arg is one of:<br>&nbsp;&nbsp;&nbsp;&nbsp;hotkey-[F1-F12|TAB|CONTROL]<br>&nbsp;&nbsp;&nbsp;&nbsp;hotkey-[F1-F12|TAB|CONTROL]-&lt;framecount&gt<br>&nbsp;&nbsp;&nbsp;&nbsp;frames-&lt;startframe&gt;-&lt;endframe&gt</td>

<td>no trimming</td>

</tr>

</tbody>

</table>

In local tracing mode, both the `vktrace` and application executables reside on the same system.

An example command to trace the sample `cube` application in local mode follows.

Linux shell:

    $ vktrace -p cube -o cubetrace.vktrace

Windows Powershell:

    PS> vktrace -p cube.exe -o cubetrace.vktrace

**Note:** Subsequent command examples in this document are written using Linux shell commands. These example commands can be translated and used as Windows Powershell commands.

Trace packets are written to the file `cubetrace.vktrace` in the current directory. Output messages from the replay operation are written to `stdout`.

_Important_: Subsequent `vktrace` runs with the same `-o` option value will overwrite the trace file, preventing the generation of multiple, large trace files. Be sure to specify a unique output trace file name for each `vktrace` invocation if you do not desire this behaviour.

## [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-clientserver-mode)Client/Server Mode

The vktrace tools also support tracing Vulkan applications in client/server mode, where the trace server resides on a local or a remote system.

### [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-server)Server

In client/server mode, the `vktrace` _server_ is started without specifying the `-p` or `--Program` option. Invoked in this fashion, `vktrace` enters a waiting mode for trace packets.

    $ mkdir examples/traces
    $ cd examples/traces
    $ vktrace -o cubetrace_s.vktrace
    No program (-p) parameter found: Running vktrace as server.

The trace file will be written to `cubetrace_s.vktrace`. If additional programs are traced with this trace server, subsequent trace files will be named `cubetrace_s-<`_`N`_`>.vktrace`, with the trace server incrementing _`N`_ for each time the application is run.

### [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-client)Client

The tracer is implemented as a Vulkan layer. When tracing in server mode, the local or remote client must enable the `Vktrace` layer. The `Vktrace` layer _must_ be the first layer identified in the `VK_INSTANCE_LAYERS` list.

    $ export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_vktrace

#### [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-local-client)Local Client

Local client/server mode is particularly useful for Vulkan applications with multiple runtime options or complicated startup.

In local client/server mode, the tracer layer must be enabled before the local client application to be traced is launched.

    $ export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_vktrace
    $ ./cube

The generated trace file is found at `examples/traces/cubetrace_s.vktrace`.

_Note_: The `cube` application is used to demonstrate tracing in client/server mode. `cube` is a very simple application, and would not be traced in this mode in practice.

#### [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-remote-client)Remote Client

Remote client/server mode is useful if the client is a mobile device or running in a lightweight environment that might not have the disk or other capabilities to store large trace files.

In remote client/server mode, the remote client must additionally identify the IP address of the trace server system.

    $ cd examples/build
    $ export VKTRACE_LIB_IPADDR=<ip address of trace server system>
    $ export VK_INSTANCE_LAYERS=Vktrace
    $ ./cube

The generated trace file is located at `examples/traces/cubetrace_s.vktrace` on the remote trace server system.

## [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-replay)Replay

The vkreplay command is used to replay a Vulkan application trace.

**Important**: Trace files generated with earlier versions of the vktrace tools may not be replayable with later versions of the tools. The trace file format has evolved over time - vkreplay attempts to identify incompatible versions during replay and can often successfully replay them, but it does not handle all such cases.

**Important**: Trace files generated on one GPU may or may not be replayable on other GPUs, as well as trace files generated on different OSes and different driver versions. vkreplay attempts to replay such trace files, translating API calls as needed for the new platform, but it does not handle all such cases.

The `vkreplay` options are:

<table>

<thead>

<tr>

<th>Replay Option</th>

<th>Description</th>

<th>Default</th>

</tr>

</thead>

<tbody>

<tr>

<td>-o &lt;string&gt;<br/> 
‑‑Open &lt;string&</td>

<td>Name of trace file to open and replay</td>

<td>**required**</td>

</tr>

<tr>

<td>-l &lt;int&gt;<br/> 
‑‑NumLoops &lt;int&gt;</td>

<td>Number of times to replay the trace file</td>

<td>1</td>

</tr>

<tr>

<td>-lsf &lt;int&gt;<br/>  
‑‑LoopStartFrame &lt;int&gt;</td>

<td>The start frame number of the loop range</td>

<td>0</td>

</tr>

<tr>

<td>-lef &lt;int&gt;<br/>  
‑‑LoopEndFrame &lt;int&gt;</td>

<td>The end frame number of the loop range</td>

<td>The last frame in the tracefile</td>

</tr>

<tr>

<td>-pltf &lt;bool&gt;<br/>
‑‑PreloadTraceFile &lt;bool&gt;</td>

<td>Preload tracefile to memory before replay (NumLoops need to be 1)</td>

<td>off</td>

</tr>

<tr>

<td>-s &lt;string&gt;<br/>
‑‑Screenshot &lt;string&gt;</td>

<td>Comma-separated list of frame numbers of which to take screen shots</td>

<td>No screenshots</td>

</tr>

<tr>

<td>-sf &lt;string&gt;<br/>
‑‑ScreenshotFormat &lt;string&gt;</td>

<td>Color Space format of screenshot files. Formats are UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB</td>

<td>Format of swapchain image</td>

</tr>

<tr>

<td>-v &lt;string&gt;<br/>  
‑‑Verbosity &lt;string&gt;</td>

<td>Verbosity mode - "quiet", "errors", "warnings", or "full"</td>

<td>errors</td>

</tr>

</tbody>

</table>

To replay the cube application trace first captured in the example above:

    $ cd examples/build
    $ vkreplay -o cubetrace.vktrace -l 5

If the trace is rather short, the replay may finish quickly. Specify the `-l` or `--NumLoops` option to replay the trace `NumLoops` option value times.

Output messages from the replay operation are written to `stdout`.

## [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-replayer-interaction-with-layers)Replayer Interaction with Layers

The Vulkan validation layers may be enabled for trace replay. Replaying a trace with layers activated provides many benefits. Developers can take advantage of new validation capabilities as they are developed with older and existing trace files.

To activate specific layers on a trace replay, set the `VK_INSTANCE_LAYERS` environment variable to a colon-separated list of layer names before replaying the trace. Refer to the [Vulkan Validation and Debugging Layers](./layers.html) guide for additional information on layers and how to configure layer output options.

## [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-vktraceviewer)vktraceviewer

The vktraceviewer tool allows interactive creation and viewing of Vulkan trace files. In the future, it will include support for interactively playing back trace files. This is alpha software.

In order to run vktraceviewer on Linux, Qt5 libraries need to be installed on the system.

## [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-persistently-mapped-buffers-and-vktrace)Persistently Mapped Buffers and vktrace

If a Vulkan program uses persistently mapped buffers (PMB) that are allocated via vkMapMemory, vktrace can track changes to PMB and automatically copy modified PMB pages to the trace file, rather than requiring that the Vulkan program call vkFlushMappedMemoryRanges to specify what PMB buffers should be copied. On Windows, the trace layer detects changes to PMB pages by setting the PAGE_GUARD flag for mapped memory pages and installing an exception handler for PAGE_GUARD that keeps track of which pages have been modified. On Linux, the trace layer detects changes to PMB pages by examining /proc/self/pagemap.

Tracking of changes to PMB using the above techniques is enabled by default. If you wish to disable PMB tracking, it can be disabled by with the `--PMB false` option to the vktrace command. Disabling PMB tracking can result in some mapped memory changes not being detected by the trace layer, a larger trace file, and/or slower trace/replay.

## [<span aria-hidden="true" class="octicon octicon-link"></span>](#user-content-trace-tools-enviroment-variables)Trace Tools Enviroment Variables

Several enviroment variables can be set to change the behavior of vktrace/vkrepay:

*   VKTRACE_PMB_ENABLE

    VKTRACE_PMB_ENABLE enables tracking of PMB if its value is 1\. Other values disable PMB tracking. If this environment variable is not set, PMB tracking is enabled. When creating a trace using client/server mode, set this variable to 0 when starting the client if you wish to disable PMB tracking.

*   VKTRACE_PAGEGUARD_ENABLE_READ_PMB

    VKTRACE_PAGEGUARD_ENABLE_READ_PMB enables read PMB support if set to a non-NULL value. If PMB data changes comes from the GPU side, PMB tracking does not usually capture those changes. This environment variable is used to enable capture of such GPU initiated PMB data changes. It is supported only on Windows.

*   VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS

    VKTRACE_PAGEGUARD_ENABLE_READ_POST_PROCESS, when set to a non-null value, enables post processing when read PMB support is enabled. When VKTRACE_PAGEGUARD_ENABLE_READ_PMB is set, PMB processing will sometimes miss writes following reads if writes occur on the same page as a read. Set this environment variable to enable post processing to fix missed pmb writes. It is supported only on Windows.
</article>
