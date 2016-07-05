#!/usr/bin/env python3
#
# Copyright (C) 2015-2016 Valve Corporation
# Copyright (c) 2015-2016 LunarG, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Author: Chia-I Wu <olv@lunarg.com>
# Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
# Author: Jon Ashburn <jon@lunarg.com>

import sys
import vulkan

def generate_get_proc_addr_check(name):
    return "    if (!%s || %s[0] != 'v' || %s[1] != 'k')\n" \
           "        return NULL;" % ((name,) * 3)

class Subcommand(object):
    def __init__(self, argv):
        self.argv = argv
        self.headers = vulkan.headers
        self.protos = vulkan.protos

    def run(self):
        print(self.generate())

    def generate(self):
        copyright = self.generate_copyright()
        header = self.generate_header()
        body = self.generate_body()
        footer = self.generate_footer()

        contents = []
        if copyright:
            contents.append(copyright)
        if header:
            contents.append(header)
        if body:
            contents.append(body)
        if footer:
            contents.append(footer)

        return "\n\n".join(contents)

    def generate_copyright(self):
        return """/* THIS FILE IS GENERATED.  DO NOT EDIT. */

/*
 *
 * Copyright (C) 2015-2016 Valve Corporation
 * Copyright (c) 2015-2016 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Courtney Goeltzenleuchter <courtney@LunarG.com>
 */"""

    def generate_header(self):
        return "\n".join(["#include <" + h + ">" for h in self.headers])

    def generate_body(self):
        pass

    def generate_footer(self):
        pass

class IcdDummyEntrypointsSubcommand(Subcommand):
    def run(self):
        if len(self.argv) == 1:
            self.prefix = self.argv[0]
            self.qual = "static"
        else:
            self.prefix = "vk"
            self.qual = ""

        super().run()

    def generate_header(self):
        return "#include \"icd.h\""

    def _generate_stub_decl(self, proto):
        if proto.name == "GetInstanceProcAddr":
            return proto.c_pretty_decl(self.prefix + "_icd" + proto.name, attr="ICD_EXPORT VKAPI")
        else:
            return proto.c_pretty_decl(self.prefix + proto.name, attr="VKAPI")

    def _generate_stubs(self):
        stubs = []
        exclusions = [ 'CreateAndroidSurfaceKHR', 'CreateWaylandSurfaceKHR', 'CreateMirSurfaceKHR',
                       'GetPhysicalDeviceWaylandPresentationSupportKHR', 'GetPhysicalDeviceMirPresentationSupportKHR',
                       'GetPhysicalDeviceDisplayPropertiesKHR', 'GetPhysicalDeviceDisplayPlanePropertiesKHR',
                       'GetDisplayPlaneSupportedDisplaysKHR', 'GetDisplayModePropertiesKHR',
                       'CreateDisplayModeKHR', 'GetDisplayPlaneCapabilitiesKHR', 'CreateDisplayPlaneSurfaceKHR']
        for proto in self.protos:
            if proto.name in exclusions:
                continue
            decl = self._generate_stub_decl(proto)
            if proto.ret != "void":
                stmt = "    return VK_ERROR_UNKNOWN;\n"
            else:
                stmt = ""

            stubs.append("%s %s\n{\n%s}" % (self.qual, decl, stmt))

        return "\n\n".join(stubs)

    def generate_body(self):
        return self._generate_stubs()

class IcdGetProcAddrSubcommand(IcdDummyEntrypointsSubcommand):
    def generate_header(self):
        return "\n".join(["#include <string.h>", "#include \"icd.h\""])

    def generate_body(self):
        for proto in self.protos:
            if proto.name == "GetDeviceProcAddr":
                gpa_proto = proto
            if proto.name == "GetInstanceProcAddr":
                gpa_instance_proto = proto

        gpa_instance_decl = self._generate_stub_decl(gpa_instance_proto)
        gpa_decl = self._generate_stub_decl(gpa_proto)
        gpa_pname = gpa_proto.params[-1].name

        lookups = []
        exclusions = [ 'CreateAndroidSurfaceKHR', 'CreateWaylandSurfaceKHR', 'CreateMirSurfaceKHR',
                       'GetPhysicalDeviceWaylandPresentationSupportKHR', 'GetPhysicalDeviceMirPresentationSupportKHR',
                       'GetPhysicalDeviceDisplayPropertiesKHR', 'GetPhysicalDeviceDisplayPlanePropertiesKHR',
                       'GetDisplayPlaneSupportedDisplaysKHR', 'GetDisplayModePropertiesKHR',
                       'CreateDisplayModeKHR', 'GetDisplayPlaneCapabilitiesKHR', 'CreateDisplayPlaneSurfaceKHR']

        for proto in self.protos:
            if proto.name in exclusions:
                continue
            lookups.append("if (!strcmp(%s, \"%s\"))" %
                    (gpa_pname, proto.name))
            if proto.name != "GetInstanceProcAddr":
                lookups.append("    return (%s) %s%s;" %
                    (gpa_proto.ret, self.prefix, proto.name))
            else:
                lookups.append("    return (%s) %s%s;" %
                    (gpa_proto.ret, self.prefix, "_icdGetInstanceProcAddr"))

        body = []
        body.append("%s %s" % (self.qual, gpa_instance_decl))
        body.append("{")
        body.append(generate_get_proc_addr_check(gpa_pname))
        body.append("")
        body.append("    %s += 2;" % gpa_pname)
        body.append("    %s" % "\n    ".join(lookups))
        body.append("")
        body.append("    return NULL;")
        body.append("}")
        body.append("")

        body.append("%s %s" % (self.qual, gpa_decl))
        body.append("{")
        body.append(generate_get_proc_addr_check(gpa_pname))
        body.append("")
        body.append("    %s += 2;" % gpa_pname)
        body.append("    %s" % "\n    ".join(lookups))
        body.append("")
        body.append("    return NULL;")
        body.append("}")

        return "\n".join(body)

def main():

    wsi = {
            "Win32",
            "Android",
            "Xcb",
            "Xlib",
            "Wayland",
            "Mir"
    }

    subcommands = {
            "icd-dummy-entrypoints": IcdDummyEntrypointsSubcommand,
            "icd-get-proc-addr": IcdGetProcAddrSubcommand
    }

    if len(sys.argv) < 3 or sys.argv[1] not in wsi or sys.argv[2] not in subcommands:
        print("Usage: %s <subcommand> [options]" % sys.argv[0])
        print
        print("Available sucommands are: %s" % " ".join(subcommands))
        exit(1)

    subcmd = subcommands[sys.argv[2]](sys.argv[3:])
    subcmd.run()

if __name__ == "__main__":
    main()
