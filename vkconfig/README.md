# vkconfig2
The vkConfig 2.0 Reboot




Currently this is ad hoc documentation while I work just so I only have to do this once ;-)



Test Environment
When test environment is run from the main window it is a modeless dialog. It can remain open while VKConfig is used normally.

When test environment is run from the profile editor, it is different. In the profile editor, you are working on a profile on the fly, and it is not saved. When you launch test environmeent, it pushes the currently active profile on a stack, launches the test enviornment tool modally. You can not leave the dialog without closing it. When the dialog is closed, the temporary profile is taken off the stack, and the original profile is resaved.

In addition, if any of the layer settings logging output is being directed anywhere other than stdout, it is changed to stdout so that it will be captured by this window.





layer_info.json
This file is a template that contains a list of layers that have settings that are user editble. Each such layer is listed individually with full descriptions of the fields, the data types, and options for each. These are driven by the layers themselves and we are not free to modify the data types or the specific enumerants we write out to layer_settings.txt.

An optinal field has been added, but only to the VK_LAYER_KHRONOS_validation layer.
"common edit": true

If this field is present (and true), then the field is displayed on the FRONT dialog settings, but not on the advanced configuration (profile) editor. Any user created configuration (profile) will have these fields displayed on the main dialog for easy editing (so long as they contain the Khronos layer).

Canned configs/profiles

BestPracticesValidation.json
GPU-AssistedValidation.json
LightweightValidation.json
StandardValidation.json

These json files contain the profile/configuration, which is a list of layers that follow the template layed out in layer_info.json. These canned configurations only contain the khronos layer. Some profiles will have additional settings fields marked as "common edit", and all other fields are considered read only and are not displayed.

Fixed profiles cannot be edited in the advanced configuration editor, and the read only fields are not displayed on the front panel for user editing. 