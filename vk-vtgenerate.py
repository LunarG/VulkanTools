#!/usr/bin/env python3
#
# Copyright (C) 2015-2016 Valve Corporation
# Copyright (c) 2015-2016 LunarG, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
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
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
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
        for proto in self.protos:
            if proto.name == "CreateAndroidSurfaceKHR":
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
        for proto in self.protos:
            if proto.name == 'CreateAndroidSurfaceKHR':
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
