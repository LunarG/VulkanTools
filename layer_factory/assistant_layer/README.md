<!-- markdownlint-disable MD041 -->
[![LunarG][1]][2]

[1]: https://vulkan.lunarg.com/img/LunarGLogo.png "www.LunarG.com"
[2]: https://www.LunarG.com/

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

Copyright &copy; 2015-2019 LunarG, Inc.

# VK\_LAYER\_LUNARG\_assistant\_layer
The `VK_LAYER_LUNARG_assistant_layer` functions as a Vulkan 'best practices' layer, and is intended to highlight
potential performance issues, questionable usage patterns, common mistakes, and items not specifically prohibited by
the Vulkan specification but that may lead to application problems.

This layer will ideally be run periodically along with normal validation checks so that issues may be addressed in early stages of development.

The specific areas covered by this layer are currently tracked in
[Github Issue #24: Best Practices/Assistant/Performance Layer](https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/24).
Requests for additional checks can be posted through the same issue, or by creating a new Github issue, and current status is tracked there
as well.

This layer has been created with the Vulkan Layer Factory. It will be deprecated in the near future, and will be replaced by Best Practices Validation in the VK_LAYER_KHRONOS_validation_layer. See https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/best_practices.md
