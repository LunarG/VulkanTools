# Vulkan Ecosystem Components
*Version 1.0, January 25, 2016*

This project provides vktrace capture/replay tool, Intel Ilo sample driver and other layer tools and driver tests.

## Introduction

Branches within this repository include the Vulkan loader, validation layers, header files, and associated tests.  These pieces are mirrored from this Github repository:
https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers
These pieces are required to enable this repository to be built standalone; that is without having to clone the Vulkan-LoaderAndValidationLayers repository.

The following components are available in this repository over and above what is mirrored from Vulkan-LoaderAndValidationLayers repository
- Api_dump, screenshot and example layers (layers/)
- Intel sample driver and null driver (icd/)
- tests for the Intel Ilo sample driver (tests/)
- vktrace and vkreplay, API capture and replay  (vktrace/)


## How to Build and Run

[BUILD.md](BUILD.md)
includes directions for building all the components, running the tests and running the demo applications.

Information on how to enable the various layers is in
[layers/README.md](layers/README.md).


## License
This work is intended to be released as open source under a MIT-style
license once the Vulkan specification is public. Until that time, this work
is covered by the Khronos NDA governing the details of the VK API.

## Acknowledgements
While this project is being developed by LunarG, Inc; there are many other
companies and individuals making this possible: Valve Software, funding
project development; Intel Corporation, providing full hardware specifications
and valuable technical feedback; AMD, providing VK spec editor contributions;
ARM, contributing a Chairman for this working group within Khronos; Nvidia,
providing an initial co-editor for the spec; Qualcomm for picking up the
co-editor's chair; and Khronos, for providing hosting within GitHub.

