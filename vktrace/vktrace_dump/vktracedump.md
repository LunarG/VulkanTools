<!-- markdownlint-disable MD041 -->
[![LunarG][1]][2]

[1]: https://vulkan.lunarg.com/img/LunarGLogo.png "www.LunarG.com"
[2]: https://www.LunarG.com/

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

Copyright &copy; 2015-2019 LunarG, Inc.

## Dump API Calls from Vulkan Trace File

The vktracedump command dumps Vulkan API calls from a Vulkan application trace as plain text, html, or json.

The  `vktracedump` command-line  options are:

| Option                | Description | Default |
| --------------------- | ----------- | ------- |
| -o &lt;string&gt; | Name of trace file to open and dump | **required** |
| -s &lt;string&gt; | Name of simple dump file to save the outputs of simple/brief API dump. <br> Use 'stdout' to send outputs to stdout. | **optional** |
| -f &lt;string&gt; | Name of full dump file to save the outputs of full/detailed API dump. <br> Use 'stdout' to send outputs to stdout. | **optional** |
| -ds | Dump the shader binary code in pCode to shader dump files shader&lowbar;&lt;index&gt;.hex (when &lt;fullDumpFile&gt; is a file) or to stdout (when &lt;fullDumpFile&gt; is stdout). <br> Only works with "-f &lt;fullDumpFile&gt;" option. <br> The file name shader&lowbar;&lt;index&gt;.hex can be found in pCode in the &lt;fullDumpFile&gt; to associate with vkCreateShaderModule. | disabled |
| -dh | Save full/detailed API dump as HTML format. Only works with "-f &lt;fullDumpFile&gt;" option. | text format |
| -dj | Save full/detailed API dump as JSON format. Only works with "-f &lt;fullDumpFile&gt;" option. | text format |
| -na | Dump string "address" in place of hex addresses. Only works with "-f &lt;fullDumpFile&gt;" option.  | disabled |

To dump API calls from a Vulkan vkcube trace:

```
$ vktracedump -o vkcube.vktrace -s vkcube-sdump.txt -f vkcube-fdump.txt
```
