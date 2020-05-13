# vkconfig2
The vkConfig 2.0 Reboot

Bugs should be reported on the LunarG project tracker, or you can email me at richard@lunarg.com.

General Use and Overview
--------------------------------------------

vkConfig has undergone a significant reworking. There are some important ideas that you need to know if you are transitioning from the previous version, or if this is your first time using this tool.

The purpose of vkConfig is to provide an easy means of specifying a set of override layers that are implicity loaded with your Vulkan applications. These layers override the default implicit layers you may have installed or any layers your applictaion may be loading explicitly. vkConfig allows you to create and save specific layer configurations or profiles that consists of one more layers and their associated settings.

In addition, starting with the loader available with the next update to the Vulkan SDK from LunarG, the effects of the override layers will apply only to applications that are registered with the vkConfig application. In fact, vkConfig will not even operate until you have added at least one application via the "Edit List..." button at the upper left hand corner of the main vkConfig dialog. Once an application list has been started, you may select from one of the preconfigured layer profiles listed on the front dialog, or create your own via the "Edit..." button.

To activate one of the preconfigured layers (the most common use case), click on it in the list box and press the "Activate" button. Once activated, a specific configuration will remain active and will affect the Vulkan applications in the application list even after vkConfig has been closed. To deactivate any layer overrides, simply click the "Deactivate" button (is is the same physical button as "Activate", but now with bright red text).

The fixed preconfigured layer configurations consist of the VK_LAYER_KHRONOS_validation layer and certain common scenarios for validation and debugging. Output options for the Khronos layer are shown in the bottom panel, and any user defined configurations that contain the Khronos layer will also have these same settings exposed for easy access on the front dialog. The pre configured layer configurations may not be edited, however you can duplicate them by selecting one, clicking "Edit..." and saving your changes as a new layer configuration profile. The name you give this configuration will then appear on the front dialog. 

It is now possible to launch the Vulkan application of your choice directly from vkConfig by clicking the "Launch Test Environment..." button at the top of the main dialog. When an appliciaon is run from this tool, all output to stdio and stderr is captured and optionally logged to a file. In this way, you can monitor validation layer output directly from the vkConfig tool.

The editor for new configurations is an advanced feature that should not be needed by most users of this tool. All Vulkan Layers detected on your system are presented here (and you have the option of adding additional search folders in case you have some custom layers of your own you wish to use). By default they are all set to "App Controlled" which means they are not part of any override. You can change this selection to "Force Off", which essentially blacklists the layer and prevents any appliction from seeing it, or "Force On" which makes th layer a part of your override group. You can rearrange the order of the layers with this dialog, and if any of the layers have user editable settings, they are displayed in the lower half of the editor dialog. Give your new configuration profile a name and description and click "Save". 

Certain system layers may be required, and some layers may be incompatible with one another. vkConfig makes no effort (at this time) to warn you of this and as an advanced feature, the developer making use of this tool should exercise care when selecting layers for a particular configuration. The current working configuration represented by the editor can be temporarily activated by clicking the "Test Environment..." button which will initiate the override files, and allow you to launch and monitor a Vulkan application of your choice.

----------------------------------------------------------
Known issues with this Beta and ongoing work
----------------------------------------------------------
There is no help text as yet.

Users need to be warned when there is a local layer settings file for an application in the app list. Layers will use the override layer settings and ignore the local file and users need to be aware of this.

Each setting has a text description that explains it's purpose. This will be displayed along side the settings in the editors.

GPU specific settings for the Khronos Validation Layer settings will be merged with the tree to the left of it.

Layer paths may not be duplicated in the layer override .json file. They currently are.

Saving profiles needs to be reworked slightly. There is no way to rename a profile or "Save As".

We need to warn users that they have an old loader on their system and thus changes made with this tool will affect ALL Vulkan applications on their system.

front panel for user editing. 