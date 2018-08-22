# VK\_LAYER\_LUNARG\_assistant\_layer
The `VK_LAYER_LUNARG_assistant_layer` functions as a Vulkan 'best practices' layer, and is intended to highlight
potential performance issues, questionable usage patterns, common mistakes, and items not specifically prohibited by
the Vulkan specification but that may lead to application problems.

This layer will ideally be run periodically along with normal validation checks so that issues may be addressed in early stages of development.

The specific areas covered by this layer are currently tracked in
[Github Issue #1612: Best Practices/Assistant/Performance Layer](https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/24).
Requests for additional checks can be posted through the same issue, or by creating a new Github issue, and current status is tracked there
as well.

This layer has been created with the Vulkan Layer Factory, and third-party contributions are welcome!

