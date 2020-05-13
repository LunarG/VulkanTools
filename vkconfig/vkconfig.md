<!-- markdownlint-disable MD041 -->
[![LunarG][1]][2]

[1]: https://vulkan.lunarg.com/img/LunarGLogo.png "www.LunarG.com"
[2]: https://www.LunarG.com/

[![Creative Commons][3]][4]

[3]: https://i.creativecommons.org/l/by-nd/4.0/88x31.png "Creative Commons License"
[4]: https://creativecommons.org/licenses/by-nd/4.0/

Copyright &copy; 2015-2020 LunarG, Inc.

# Vulkan Configurator (vkconfig)

The Vulkan Configurator is a graphical application that allows a user to specify which layers will be loaded by Vulkan applications at runtime.
It provides an alternative to setting layers through environment variables or an application's layer selection.
In addition, it allows using layers from non-standard locations, selecting the ordering for implicit layers, and specifying settings for layers that Vulkan Configurator supports.

## Using the Vulkan Configurator

The Vulkan Configurator is a graphical user interface (GUI), and does not support any functionality through the system console. It may be launched from the console (as `vkconfig`), but no further functionality will be available from the console. The tool is distributed differently, depending on the platform:
- Ubuntu packages: Upon installing the `lunarg-vkconfig` package, the tools will be available from the command line as `vkconfig`.
- Linux tarball: After extracting the SDK, run `./vulkansdk lunarg-tools` to build `vkconfig`. Note that the Core, GUI, and Widgets modules of Qt5 must be installed prior to running the build script.
- Windows: The tool will be present on the start menu, in the Vulkan SDK menu. It will be called `vkconfig`.
- MacOS: The tool is provided as an application bundle. Run it by double clicking the bundle from a file explorer.

The configurator is divided into the following three sections.

### Layer Locations

At the top is the layer location selector.
By default, this list will display each of the standard system directories (or registries, on Windows) where a Vulkan application will search for layers.
Each location is labeled with either an "E", denoting that this location contains explicit layers, or an "I", denoting that this location contains implicit layers.
At the top of the layer locations selector is a checkbox labelled "Use custom layer paths".
When this checkbox is selected, the user gains the ability to choose the locations where layers will be searched.
The user can then add or remove paths with the "Add", "Remove", and "Clear" buttons on the right.
In addition, the "Search" button will allow the user to select a directory and Vulkan Configurator will scan that directory for json files that look like Vulkan layers, then prompt the user to ask if each path should actually be added.
This can be helpful when a user does not wish to manually find the exact location of layers.
When selecting layers manually, the selected layers will be saved each time the application is run, making it unnecessary to reset them every time.

### Active Layers

The bottom of Vulkan Configurator is the active layer selector.
This section allows a user to choose which layers are active for Vulkan applications, as well as selecting the order of those layers.
Any layers specified in this section will be active on any Vulkan application that is run on the current machine, with the current user.
The layer selector consists of three lists and a few other options.
The list on the left is the list of enabled layers.
The two lists on the right are the lists of disabled layers.
The top of those lists contains explicit layers, while the bottom contains implicit layers.
To enable a disabled layer, select it in one of the disabled layer lists and press the arrow button immediately to the left of that list.
The layer will be added to the bottom of the enabled layer list.
The layer can now be reordered by selecting it and pressing the up/down arrow buttons immediately to the left of the active layer list.
Layers at the top of the list will be enabled closest to the Vulkan application, while layers at the bottom will be enabled closest to the Vulkan driver.
This is indicated by the "Application Side" and "Driver Side" labels above and below the list.
In addition, right below the up/down buttons, there is also a "Remove" button and a "Clear" button below that.
The "Remove" button will remove all selected layers from the enabled layer list, putting them back in the disabled layer lists.
The "Clear" button will disable all layers, regardless of selection.

There are two more tools at the top of the active layer selector: the expiration duration and the refresh button.
The refresh button is the simpler of these tools &mdash; it simply searches the paths specified by the layer locations widget for layers.
This can be useful when adding or removing layers in these locations while Vulkan Configurator is running, since it will not pick up these changes by default.
The expiration duration is a little more complex.
The expiration provides a mechanism so that the layer selections provided by Vulkan Configurator will expire (that is, they will stop having any effect) after a given time.
The prevents a user from setting specific layers as overrides and forgetting that these layers will still be enabled, days, months, or even years later.
The default expiration is 12 hours, meaning that all layer selections will no longer work 12 hours after the layer selections are saved.
Resaving layer selections does extend the expiration to 12 hours from the new save.
The expiration can be set to any number value, and the dropdown menu lets the user select an expiration in minutes, hours, or days.
In addition, the expiration can be disabled entirely by selecting "Never" from the dropdown.
Note that disabling the expiration entirely is not recommended as it makes it very easy to leave settings active and forget about them days or weeks into the future.
It's no fun debugging issues only to find that the wrong layers were enabled.

### Layer Settings

The pane on the right side is the layer settings selector.
Currently, Vulkan Configurator provides support for changing the settings of all LunarG layers, as well as other layers in standard validation.
Support for changing settings for other layers may be added in the future.

The panel provides a list of layers for which settings can be selected.
If the "Show enabled layers only" button is not checked, this will show all supported layers that are found.
If the checkbox is selected, the panel will only show settings for each layer that is currently in the enabled list from the active layers selector. A layer may be selected by clicking on it, to open the details for that layer.
Different layers will provide different settings, so it is impractical to give a list of settings for each layer, but a layer can have five types of settings:

* A true/false pairing &mdash; The user can select either true or false, but not both.
* A dropdown box &mdash; The user can select any one from a number of options.
* A multi-selection dropdown box &mdash; The user can select any number of options from a dropdown menu.
* A text field &mdash; The user can enter any text.
* A file &mdash; The user can enter any text as a filename, but a file selector button is present for convenience.

At the present time, layer settings will only work for layers written by Khronos or LunarG.
In the future, settings may work for other layers, but this feature has not currently been developed.

## Saving and Restoring

Finally, at the bottom of Vulkan Configurator, there are four buttons to control the tool: "Save", "Restore", "Clear", and "Exit".
The "Save" button saves all changes that have been made in the tool.
No changes made in Vulkan Configurator will have any effect until the user hits that save button.
The "Restore" button does the opposite.
It discards all changes made since the last time the user saved.
The "Clear" button will remove all of the files that the Vulkan Configurator saves to change an application's behavior.
After using this button, Vulkan applications will behave as if Vulkan Configurator were never run.
Finally, the "Exit" button will quit the application (without saving).
It is effectively the same as pressing the "Reset" button and exiting the application.
Note that while all unsaved changes will be lost, all saved settings in the tool will be remembered the next time Vulkan Configurator is run.

When any of these buttons is run, a timestamped message will appear at the bottom of the Vulkan Configurator indicating the action that was taken.
This can be useful as a confirmation of what you did.
In addition, it can give you information like when you last saved, since the message will stay there until another command is given.

Keep in mind that any changes made to a system will persist indefinitely (unless an expiration is set).
As a result, it is very important that the user remember to clear the state of the tool when finishing using it.
Otherwise, applications will continue to run with the layers that were specified by the configurator.

## System Modifications

The Vulkan Configurator does not make any system-wide changes to a system, but it does make user-specific changes.
These changes are documented below:

### Linux/Mac

Unix systems store files in the following paths:

- `$HOME/.local/share/vulkan/implicit_layer.d/VkLayer_override.json` tells a Vulkan application which layers to use
- `$HOME/.local/share/vulkan/settings.d/vk_layer_settings.txt` tells Vulkan layers which settings to use
- `$HOME/.config/LunarG/vkconfig.conf` stores the application settings for `vkconfig`

### Windows

Windows systems store files in the following paths:

- `%TEMP%\VulkanLayerManager\VkLayerOverride.json` tells a Vulkan application which layers to use
- `%TEMP%\VulkanLayerManager\vk_layer_settings.json` tells Vulkan layers which settings to use

In addition, Windows system create registry entries in the following locations:

- `HKEY_CURRENT_USER\Software\Khronos\Vulkan\ImplicitLayers` will have an entry that points to the JSON file above
- `HKEY_CURRENT_USER\Software\Khronos\Vulkan\Settings` will have an entry that points to the text file above
- `HKEY_CURRENT_USER\Software\LunarG\vkconfig` stores the application settings for `vkconfig`
