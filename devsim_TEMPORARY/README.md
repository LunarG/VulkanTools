# Proposed JSON schema for Devsim layer configuration

https://github.com/LunarG/VulkanTools/tree/mikew_devsim_schema/devsim_TEMPORARY

This branch contains a proposed JSON schema, for use in validating the configuration files consumed by the [LunarG device simulation Vulkan layer](https://github.com/LunarG/VulkanTools/blob/mikew_devsim_schema/layersvt/device_simulation.md).

## Background
The original devsim configuration language was adopted directly from the [vkjson_info](https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers/tree/master/libs/vkjson) tool.  While functional for a proof-of-concept, that JSON format is ad-hoc, and parts of its structure are not amenable to JSON schema validation.  One particularly error-prone aspect of that format is the reuse of keywords between Vulkan structure names and JSON structuring keywords (e.g. `properties`).

As the devsim configuration files come into wider use, the configuration files will probably be viewed and manually edited by users, and become assets in themselves.  It is useful to revise the design of that original JSON format, and create a schema to ensure the data conforms to basic validation.

## Terminology
It is easy to get confused by the use of similar terms across the contexts of C++, JSON, and Vulkan.  For example:
* In JSON, an `object` is an unordered collection of name/value pairs, what might be called an unordered_map in C++.  Those name/value pairs in an `object` are called `properties`.
* In JSON, an `array` is an ordered collection of `items`, accessed by an ordinal index.

## Design approach
For this initial proposal, several rules were applied to keep the scope of the effort focused.  Those rules may certainly be revisited for future schema versions in the future.
1. Only the data currently output by the vkjson_info tool is considered in this initial proposal.  We do not consider adding support for additional Vulkan data structures yet.  Also, we do not address adding datatypes that might support future capabilities of the devsim layer.
1. Use JSON schema's datatypes.  The most used datatypes are:
   1. `object` : An unordered container, mapping name/value pairs.  Used to represent structure members and their values.
   1. `array` : An ordered container, addressed by an ordinal index.
   1. `integer` : An integral base-10 numeric value of arbitrary magnitude.  Min/max constraints are used to enforce wordsizes and signed/unsigned.
   1. `number` : A real base-10 numeric value.
1. As much as practical, leverage the enforcement capabilities of JSON schema.  That includes: using JSON `definitions` to define the set of Vulkan types; min/max bounding of values; enforcement of whether additional properties are permitted or not; maximum length of strings and arrays; etc.
1. As much as possible, conform strictly to the structure naming and member naming of Vulkan, as defined by `vulkan.h`  That means adhering to the name of Vulkan structures and their members, and not adding structures or structure members that are not defined by Vulkan.  There are two exceptions:
   1. A `formatID` member was added to the `VkFormatProperties` structure, as Vulkan uses indexing that is not included in the structure itself.
   1. Several types with the prefix of `ArrayOf` were added when Vulkan does not define a container type to hold multiple instances of a structure.
1. Do not attempt to express capabilities beyond what is defined by JSON.  Since JSON defines only base-10 numeric values, do not attempt to use "0x" hexadecimal values, even if that might be more appropriate for some Vulkan datatypes such as flags.
1. It should be possible to reconstruct Vulkan data structures with minimal information beyond a given JSON document.  Therefore we express C++ enumerations as their numeric values, rather than as symbolic tokens.
1. To avoid accidental misspelling of structure members, the contents of Vulkan structures may not have any members that are not defined in the schema.
1. The top-level members of the JSON document are not mandatory, nor are they exclusive.  Top-level members may be omitted, and top-level members may be added as desired.  (In contrast to the members of Vulkan structures, which are not mandatory, but _are_ exclusive.)
1. To provide flexibility, there is no restriction on adding top-level structures to the JSON document.  For example, a free-form `comments` object has been defined to permit flexible addition of annotations.  (The JSON specification does not support the concept of comments)

## Contents
1. `schema.json` : This is the proposed schema.
1. `GeForceGTX950_ORIGINAL.json` : The verbatim output of the current version of `vkjson_info`.
1. `GeForceGTX950_MODIFIED.json` : A hand-edited conversion of the `vkjson_info` output, modified to pass schema validation.
1. `getreportjson_1456_ORIGINAL.json` : The verbatim result of querying http://vulkan.gpuinfo.org/api/v2/getreportjson.php?id=1456
1. `getreportjson_1456_MODIFIED.json` : A hand-edited conversion of the gpuinfo.org query, modified to pass schema validation.

## Resources
1. http://json.org/
1. http://json-schema.org/
1. https://tools.ietf.org/html/draft-wright-json-schema-00
1. https://tools.ietf.org/html/draft-wright-json-schema-validation-01
1. https://spacetelescope.github.io/understanding-json-schema/
1. https://jsonlint.com/

### JSON validation
The following web-based validators have been used to confirm the MODIFIED files validate against the schema, and verify the ORIGINAL files fail validation.  Unfortunately, each website gives somewhat different results when validating, but usually have similar pass/fail assessment.
1. http://www.jsonschemavalidator.net/
1. https://jsonschemalint.com/#/version/draft-04/markup/json
1. https://json-schema-validator.herokuapp.com/

## Questions
1. Is this really a devsim schema, or is it really a Vulkan schema?
1. What should be the name of the schema file?  From other schemas on the web, options could vary among:
   1. devsim/version-0/schema
   1. devsim/version-0/schema.json
   1. version-0/devsim
   1. version-0/devsim.json
1. When officially released, where should the schema file be located?  I.e.: What should be the official and immutable URL for the schema?  Options include:
   1. Directly from a repository tree on GitHub
   1. A top-level path under http://khronos.org
   1. A top-level path under http://lunarg.org
   1. A general warehouse site like http://schemastore.org/json/
