#version 450
/*
 * Copyright (c) 2026 LunarG, Inc.
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
 */

// Draws the monitor overlay text in white over a translucent black panel.
// After changing this file, run scripts/generate_monitor_shaders.py.

// Must match OverlayPushConstants in monitor_overlay.h
layout(push_constant) uniform PushConstants {
    ivec4 visible_rect;
    uint visible_extent;
    uint params;
    float white;
    uint reserved;
    uint glyphs[24];
}
pc;

layout(location = 0) in vec2 in_panel;
layout(location = 0) out vec4 out_color;

// Same 5x7 font as monitor_overlay.cpp, one row per value, top row first, bit 4 is the leftmost pixel
const uint kFont[161] = uint[](
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  // ' '
    0x0Eu, 0x11u, 0x13u, 0x15u, 0x19u, 0x11u, 0x0Eu,  // '0'
    0x04u, 0x0Cu, 0x04u, 0x04u, 0x04u, 0x04u, 0x0Eu,  // '1'
    0x0Eu, 0x11u, 0x01u, 0x02u, 0x04u, 0x08u, 0x1Fu,  // '2'
    0x1Fu, 0x02u, 0x04u, 0x02u, 0x01u, 0x11u, 0x0Eu,  // '3'
    0x02u, 0x06u, 0x0Au, 0x12u, 0x1Fu, 0x02u, 0x02u,  // '4'
    0x1Fu, 0x10u, 0x1Eu, 0x01u, 0x01u, 0x11u, 0x0Eu,  // '5'
    0x06u, 0x08u, 0x10u, 0x1Eu, 0x11u, 0x11u, 0x0Eu,  // '6'
    0x1Fu, 0x01u, 0x02u, 0x04u, 0x08u, 0x08u, 0x08u,  // '7'
    0x0Eu, 0x11u, 0x11u, 0x0Eu, 0x11u, 0x11u, 0x0Eu,  // '8'
    0x0Eu, 0x11u, 0x11u, 0x0Fu, 0x01u, 0x02u, 0x0Cu,  // '9'
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x0Cu, 0x0Cu,  // '.'
    0x00u, 0x00u, 0x00u, 0x0Eu, 0x00u, 0x00u, 0x00u,  // '-'
    0x02u, 0x04u, 0x08u, 0x08u, 0x08u, 0x04u, 0x02u,  // '('
    0x08u, 0x04u, 0x02u, 0x02u, 0x02u, 0x04u, 0x08u,  // ')'
    0x1Fu, 0x10u, 0x10u, 0x1Eu, 0x10u, 0x10u, 0x10u,  // 'F'
    0x1Eu, 0x11u, 0x11u, 0x1Eu, 0x10u, 0x10u, 0x10u,  // 'P'
    0x0Fu, 0x10u, 0x10u, 0x0Eu, 0x01u, 0x01u, 0x1Eu,  // 'S'
    0x00u, 0x00u, 0x0Eu, 0x01u, 0x0Fu, 0x11u, 0x0Fu,  // 'a'
    0x00u, 0x00u, 0x0Eu, 0x11u, 0x1Fu, 0x10u, 0x0Eu,  // 'e'
    0x00u, 0x00u, 0x1Au, 0x15u, 0x15u, 0x11u, 0x11u,  // 'm'
    0x00u, 0x00u, 0x16u, 0x19u, 0x10u, 0x10u, 0x10u,  // 'r'
    0x00u, 0x00u, 0x0Fu, 0x10u, 0x0Eu, 0x01u, 0x1Eu  // 's'
);

// Must match the panel metrics in monitor_overlay.cpp, in font pixels
const int kGlyphWidth = 5;
const int kGlyphHeight = 7;
const int kGlyphCount = 23;
const int kCharAdvance = 6;
const int kLineAdvance = 9;
const int kPadding = 3;
const int kMaxColumns = 32;

void main() {
    const uint pixel_size = pc.params & 0xFFu;
    const int line_count = int((pc.params >> 10) & 0x3u);
    const bool opaque = ((pc.params >> 12) & 0x1u) != 0u;

    out_color = vec4(0.0, 0.0, 0.0, opaque ? 1.0 : 0.6);

    const ivec2 font = ivec2(floor(in_panel / float(pixel_size))) - ivec2(kPadding);
    if (font.x < 0 || font.y < 0) {
        return;
    }

    const int line = font.y / kLineAdvance;
    const int column = font.x / kCharAdvance;
    const int x = font.x % kCharAdvance;
    const int y = font.y % kLineAdvance;
    if (line >= line_count || column >= kMaxColumns || x >= kGlyphWidth || y >= kGlyphHeight) {
        return;
    }

    const uint index = uint(line * kMaxColumns + column);
    const uint glyph = min((pc.glyphs[index / 4u] >> ((index % 4u) * 8u)) & 0xFFu, uint(kGlyphCount - 1));
    const uint row = kFont[glyph * uint(kGlyphHeight) + uint(y)];
    if (((row >> uint(kGlyphWidth - 1 - x)) & 1u) != 0u) {
        out_color = vec4(pc.white, pc.white, pc.white, 1.0);
    }
}
