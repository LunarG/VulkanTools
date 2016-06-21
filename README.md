# Vulkan Ecosystem Components

This project provides vktrace capture/replay tool, Intel Ilo sample driver and other layer tools and driver tests.

## Introduction

Branches within this repository include the Vulkan loader, validation layers, header files, and associated tests.  These pieces are mirrored from this Github repository:
https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers
These pieces are required to enable this repository to be built standalone; that is without having to clone the Vulkan-LoaderAndValidationLayers repository.

The following components are available in this repository over and above what is mirrored from Vulkan-LoaderAndValidationLayers repository
- Api_dump, screenshot and example layers (layersvt/)
- Intel sample driver and null driver (icd/)
- tests for the Intel Ilo sample driver (tests/)
- vktrace and vkreplay, API capture and replay  (vktrace/)

## Contributing

If you intend to contribute, the preferred work flow is for you to develop your contribution
in a fork of this repo in your GitHub account and then submit a pull request.
Please see the [CONTRIBUTING](CONTRIBUTING.md) file in this respository for more details.

## How to Build and Run

[BUILDVT.md](BUILDVT.md)
includes directions for building all the components, running the tests and running the demo applications.

Information on how to enable the various layers is in
[layers/README.md](layers/README.md).

Architecture and interface information for the loader is in
[loader/LoaderAndLayerInterface.md](loader/LoaderAndLayerInterface.md).

## License
This work is released as open source under a Apache-style license from Khronos including a Khronos copyright.

See COPYRIGHT.txt for a full list of licenses used in this repository.

## Acknowledgements
While this project has been developed primarily by LunarG, Inc; there are many other
companies and individuals making this possible: Valve Corporation, funding
project development; Google providing significant contributions to the validation layers;
Khronos providing oversight and hosting of the project.

