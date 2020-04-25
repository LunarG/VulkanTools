# vkconfig2
The vkConfig 2.0 Reboot


Fix confusion between "true" and true... use one and be consistent?


Currently this is ad hoc documentation while I work just so I only have to do this once ;-)

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