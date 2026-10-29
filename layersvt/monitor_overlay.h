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

#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include <vulkan/vulkan.h>

namespace monitor {

struct StatsSnapshot {
    bool valid = false;
    double fps = 0.0;
    double avg_ms = 0.0;
    double min_ms = 0.0;
    double max_ms = 0.0;
};

// Present-to-present timing for one swapchain. The snapshot is refreshed once per update interval.
struct FrameStats {
    StatsSnapshot snapshot;

    bool started = false;
    std::chrono::steady_clock::time_point last_present;
    std::chrono::steady_clock::time_point interval_start;
    uint32_t frame_count = 0;
    double total_ms = 0.0;
    double min_ms = 0.0;
    double max_ms = 0.0;
};

void RecordPresent(FrameStats &stats, std::chrono::steady_clock::time_point now, std::chrono::milliseconds interval);

// Returns the number of the frame being presented. Frame 0 is the first presented image.
[[nodiscard]] uint64_t NextFrameNumber(std::atomic<uint64_t> &counter);

// 5x7 bitmap font. The fragment shader holds a copy of the same table.
inline constexpr int kGlyphWidth = 5;
inline constexpr int kGlyphHeight = 7;
inline constexpr int kGlyphCount = 23;

// Panel metrics in font pixels. The fragment shader holds a copy of these too.
inline constexpr int kCharAdvance = kGlyphWidth + 1;
inline constexpr int kLineAdvance = kGlyphHeight + 2;
inline constexpr int kPadding = 3;
inline constexpr int kMargin = 4;

// Characters the font can't draw map to the blank glyph.
[[nodiscard]] uint8_t GlyphIndex(char character);

// Returns one row of a glyph, top row first. Bit 4 is the leftmost pixel.
[[nodiscard]] uint8_t GlyphRow(uint8_t glyph, int row);

struct LineFlags {
    bool fps = true;
    bool frame_time = true;
    bool frame_number = true;
};

// With every line hidden there is no overlay to draw
[[nodiscard]] bool AnyLineVisible(const LineFlags &flags);

// What to say about vkCreateSharedSwapchainsKHR swapchains, which never get the overlay. Null with every line hidden.
[[nodiscard]] const char *SharedSwapchainsWarning(const LineFlags &flags);

// Messages already shown to the user, so each is shown once
using ShownMessages = std::set<std::string, std::less<>>;

// True the first time `message` is seen. Messages seen before cost a lookup, without allocating.
[[nodiscard]] bool FirstTimeShown(ShownMessages &shown, std::string_view message);

// Text lines of the overlay, top to bottom. Hidden lines are left out.
[[nodiscard]] std::vector<std::string> FormatOverlayLines(const StatsSnapshot &snapshot, uint64_t frame_number,
                                                          const LineFlags &flags);

// The same, reusing the strings in `lines`, so it doesn't allocate once they have grown to fit.
void FormatOverlayLines(const StatsSnapshot &snapshot, uint64_t frame_number, const LineFlags &flags,
                        std::vector<std::string> &lines);

enum class Position : uint8_t { TopLeft, TopRight, BottomLeft, BottomRight };

// {x0, y0, x1, y1}, x1 and y1 exclusive. An array rather than named fields, so it has the shaders' ivec4 layout.
using Rect = std::array<int32_t, 4>;

struct OverlayLayout {
    VkExtent2D visible_extent = {};  // Image extent as the user sees it, after preTransform
    Rect visible_rect = {};          // Panel as the user sees it
    Rect image_rect = {};            // Panel in swapchain image pixels
    uint32_t rotation = 0;           // Quarter turns clockwise from visible to image
    uint32_t pixel_size = 1;         // Image pixels per font pixel
    bool transform_warning = false;  // preTransform was mirrored or INHERIT and is drawn unmirrored
};

[[nodiscard]] OverlayLayout ComputeOverlayLayout(VkExtent2D image_extent, Position position, int scale_percent,
                                                 VkSurfaceTransformFlagBitsKHR transform, const std::vector<std::string> &lines);

inline constexpr uint32_t kMaxLines = 3;
inline constexpr uint32_t kMaxColumns = 32;

// Bit layout of the packed push constant fields. The overlay shaders unpack the same layout.
inline constexpr uint32_t kExtentHeightShift = 16;
inline constexpr uint32_t kExtentWidthMask = 0xFFFF;
inline constexpr uint32_t kParamsPixelSizeMask = 0xFF;
inline constexpr uint32_t kParamsRotationShift = 8;
inline constexpr uint32_t kParamsRotationMask = 0x3;
inline constexpr uint32_t kParamsLineCountShift = 10;
inline constexpr uint32_t kParamsOpaqueShift = 12;
inline constexpr uint32_t kGlyphsPerWord = 4;
inline constexpr uint32_t kBitsPerGlyph = 8;

// maxPushConstantsSize every Vulkan implementation supports
inline constexpr size_t kMinMaxPushConstantsSize = 128;

// Everything the overlay shaders need. Must match the push constant block in the overlay shaders.
struct OverlayPushConstants {
    Rect visible_rect;        // Panel as the user sees it
    uint32_t visible_extent;  // width | height << 16
    uint32_t params;          // pixel size (bits 0-7) | rotation (8-9) | line count (10-11) | opaque (12)
    float white;              // Value of white for the swapchain color space
    uint32_t reserved;
    std::array<uint32_t, kMaxLines * kMaxColumns / kGlyphsPerWord> glyphs;  // Glyph indices, one byte each, kMaxColumns per line
};
static_assert(sizeof(OverlayPushConstants) == kMinMaxPushConstantsSize,
              "Push constants must fit in the minimum maxPushConstantsSize");

[[nodiscard]] OverlayPushConstants PackOverlayPushConstants(const OverlayLayout &layout, const std::vector<std::string> &lines,
                                                            float white, bool opaque);

struct WhiteLevel {
    float value = 1.0f;
    bool known = true;  // False if the color space is not recognized and 1.0 was used
};

// Value written for white text, chosen so HDR swapchains show it at about 200 nits.
[[nodiscard]] WhiteLevel WhiteForColorSpace(VkColorSpaceKHR color_space);

struct Eligibility {
    bool ok = true;
    const char *reason = nullptr;  // Why the overlay can't be drawn, when !ok. Null if the user hid every line.
    bool deferred = false;         // Image memory is bound on first acquire, so image views must wait until then
};

[[nodiscard]] Eligibility SwapchainOverlayEligibility(const VkSwapchainCreateInfoKHR &create_info);

// The create info a swapchain is created with, and whether the overlay can draw on it
struct SwapchainPatch {
    VkSwapchainCreateInfoKHR create_info = {};
    Eligibility eligibility;
};

// Returns a copy of the create info that lets the overlay render into the swapchain images, or an unchanged copy if
// the overlay can't draw on them or `lines` hides everything. The pNext chain is shared.
[[nodiscard]] SwapchainPatch PatchSwapchainCreateInfo(const VkSwapchainCreateInfoKHR &create_info, const LineFlags &lines);

[[nodiscard]] bool ShouldDrawOverlay(VkQueueFlags queue_flags, bool any_overlay_swapchain, bool any_line_visible);

// Whether VkImageViewUsageCreateInfo can be used. The device version the app may use is the lower of its
// VkApplicationInfo::apiVersion (1.0 if not given) and the physical device's apiVersion.
[[nodiscard]] bool ImageViewUsageAvailable(uint32_t instance_api_version, uint32_t device_api_version, bool maintenance2_enabled);

inline constexpr int kDefaultScalePercent = 100;
inline constexpr int kMinScalePercent = 50;
inline constexpr int kMaxScalePercent = 400;
inline constexpr int kDefaultUpdateIntervalMs = 500;
inline constexpr int kMinUpdateIntervalMs = 50;

// Layer settings as read, before validation
struct RawSettings {
    std::string position = "TOP_LEFT";
    int32_t scale = kDefaultScalePercent;
    int32_t update_interval_ms = kDefaultUpdateIntervalMs;
    bool show_fps = true;
    bool show_frame_time = true;
    bool show_frame_number = true;
    bool log_warnings = false;
};

struct MonitorSettings {
    Position position = Position::TopLeft;
    int scale_percent = kDefaultScalePercent;
    std::chrono::milliseconds update_interval{kDefaultUpdateIntervalMs};
    LineFlags lines;
    bool log_warnings = false;  // Say why the overlay isn't shown; off unless the user asks
};

[[nodiscard]] MonitorSettings ParseSettings(const RawSettings &raw);

}  // namespace monitor
