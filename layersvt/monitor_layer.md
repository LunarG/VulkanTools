<!-- markdownlint-disable MD041 -->
<p align="left"><img src="https://vulkan.lunarg.com/img/NewLunarGLogoBlack.png" alt="LunarG" width=263 height=113 /></p>
<p align="left">Copyright &copy; 2015-2026 LunarG, Inc.</p>

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

# VK\_LAYER\_LUNARG\_monitor
The `VK_LAYER_LUNARG_monitor` utility layer draws a small overlay in the corner of every image the application presents, showing:

- the frame rate in frames per second
- the average frame time in milliseconds, with the shortest and longest frame time in parentheses
- the frame number

The overlay is drawn into the presented images, so it works in fullscreen and doesn't depend on the window system. The application's window title is left alone. The layer is available on Windows, Linux, macOS, iOS and Android, and the overlay stays upright on Android devices that pre-rotate the swapchain.

The frame rate and frame times are measured between presents of each swapchain, and refresh twice a second by default. The frame number updates every frame: it starts at 0 for the first presented image and counts every `vkQueuePresentKHR` call of the `VkInstance`.

For an overview of how to configure layers, refer to the [Layers Overview and Configuration](https://vulkan.lunarg.com/doc/sdk/latest/windows/layer_configuration.html) document.

The Monitor Layer can be enabled using the [Vulkan Configurator](https://vulkan.lunarg.com/doc/sdk/latest/windows/vkconfig.html) included with the Vulkan SDK.

## Layer Settings

| Setting | Default | Description |
|---|---|---|
| `position` | `TOP_LEFT` | Corner the overlay is drawn in: `TOP_LEFT`, `TOP_RIGHT`, `BOTTOM_LEFT` or `BOTTOM_RIGHT`. |
| `scale` | `100` | Size of the overlay text as a percentage of the default size, from 50 to 400. The default size grows with the resolution of the presented images. |
| `update_interval_ms` | `500` | How often the frame rate and frame times are updated, at least 50. |
| `show_fps` | `true` | Show the frame rate. |
| `show_frame_time` | `true` | Show the frame times. |
| `show_frame_number` | `true` | Show the frame number. |
| `log_warnings` | `false` | Print why the overlay isn't shown on some swapchains, once per reason, to stderr or the Android log. |

The settings can be set with the Vulkan Configurator, `vk_layer_settings.txt`, environment variables such as `VK_LUNARG_MONITOR_POSITION=BOTTOM_RIGHT`, or `VK_EXT_layer_settings`. The layer option details are in the [monitor layer documentation](https://vulkan.lunarg.com/doc/sdk/latest/windows/monitor_layer.html#user-content-layer-details).

If every line is hidden, nothing is drawn and swapchains are created exactly as the application asks.

## Limitations

In these cases the frame rate isn't shown, and the application's frames are presented exactly as they would be without the layer:

- Presents from a queue that can't do graphics work, such as a compute or transfer only queue. The overlay is drawn on the queue the application presents from.
- Swapchains with a shared present mode. The application and the display use the same image all the time, so there is no moment the overlay could be drawn without getting in the way.
- Protected swapchains. The layer can't draw into protected images.
- Swapchains created with `vkCreateSharedSwapchainsKHR`.

With `VK_KHR_incremental_present`, the layer marks the whole image as changed so the overlay is updated. If `VkPresentRegionsKHR` comes after an extension struct the layer doesn't know in the `pNext` chain, it can't, and the overlay may only update where the application's rectangles cover it.

Turn on `log_warnings` to have the layer say when any of these happens.

To draw into the swapchain images, the layer adds `VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT` to the swapchain image usage.
