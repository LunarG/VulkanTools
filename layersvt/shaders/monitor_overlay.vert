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

// Draws the monitor overlay panel as a 4 vertex triangle strip, with no vertex buffer.
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

// Position inside the panel, in visible (unrotated) pixels
layout(location = 0) out vec2 out_panel;

void main() {
    const vec2 corner = vec2(gl_VertexIndex & 1, gl_VertexIndex >> 1);
    const vec2 visible = mix(vec2(pc.visible_rect.xy), vec2(pc.visible_rect.zw), corner);
    const vec2 visible_extent = vec2(pc.visible_extent & 0xFFFFu, pc.visible_extent >> 16);
    const uint rotation = (pc.params >> 8) & 0x3u;

    // Image content is the visible content rotated clockwise
    vec2 image;
    vec2 image_extent;
    if (rotation == 1u) {
        image = vec2(visible_extent.y - visible.y, visible.x);
        image_extent = visible_extent.yx;
    } else if (rotation == 2u) {
        image = visible_extent - visible;
        image_extent = visible_extent;
    } else if (rotation == 3u) {
        image = vec2(visible.y, visible_extent.x - visible.x);
        image_extent = visible_extent.yx;
    } else {
        image = visible;
        image_extent = visible_extent;
    }

    out_panel = visible - vec2(pc.visible_rect.xy);
    gl_Position = vec4(image / image_extent * 2.0 - 1.0, 0.0, 1.0);
}
