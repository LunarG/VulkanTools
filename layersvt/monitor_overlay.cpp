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

#include "monitor_overlay.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <ratio>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace monitor {

constexpr double kMillisecondsPerSecond = 1000.0;

void RecordPresent(FrameStats &stats, std::chrono::steady_clock::time_point now, std::chrono::milliseconds interval) {
    if (!stats.started) {
        stats.started = true;
        stats.last_present = now;
        stats.interval_start = now;
        return;
    }

    const double frame_ms = std::chrono::duration<double, std::milli>(now - stats.last_present).count();
    stats.last_present = now;

    if (stats.frame_count == 0) {
        stats.min_ms = frame_ms;
        stats.max_ms = frame_ms;
    }
    stats.frame_count++;
    stats.total_ms += frame_ms;
    stats.min_ms = std::min(stats.min_ms, frame_ms);
    stats.max_ms = std::max(stats.max_ms, frame_ms);

    if (now - stats.interval_start < interval || stats.total_ms <= 0.0) {
        return;
    }

    stats.snapshot.valid = true;
    stats.snapshot.avg_ms = stats.total_ms / stats.frame_count;
    stats.snapshot.fps = kMillisecondsPerSecond / stats.snapshot.avg_ms;
    stats.snapshot.min_ms = stats.min_ms;
    stats.snapshot.max_ms = stats.max_ms;

    stats.interval_start = now;
    stats.frame_count = 0;
    stats.total_ms = 0.0;
}

uint64_t NextFrameNumber(std::atomic<uint64_t> &counter) { return counter.fetch_add(1, std::memory_order_relaxed); }

// Characters in glyph order, index 0 being the blank glyph. The font only has the characters the overlay prints:
// digits and punctuation for the numbers, "FPS", and the lowercase letters of "ms" and "Frame".
constexpr std::string_view kGlyphChars = " 0123456789.-()FPSaemrs";
static_assert(kGlyphChars.size() == kGlyphCount);

constexpr std::array<std::array<uint8_t, kGlyphHeight>, kGlyphCount> kFont = {{
    {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000},  // ' '
    {0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110},  // '0'
    {0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110},  // '1'
    {0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111},  // '2'
    {0b11111, 0b00010, 0b00100, 0b00010, 0b00001, 0b10001, 0b01110},  // '3'
    {0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010},  // '4'
    {0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110},  // '5'
    {0b00110, 0b01000, 0b10000, 0b11110, 0b10001, 0b10001, 0b01110},  // '6'
    {0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000},  // '7'
    {0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110},  // '8'
    {0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00010, 0b01100},  // '9'
    {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b01100, 0b01100},  // '.'
    {0b00000, 0b00000, 0b00000, 0b01110, 0b00000, 0b00000, 0b00000},  // '-'
    {0b00010, 0b00100, 0b01000, 0b01000, 0b01000, 0b00100, 0b00010},  // '('
    {0b01000, 0b00100, 0b00010, 0b00010, 0b00010, 0b00100, 0b01000},  // ')'
    {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000},  // 'F'
    {0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000},  // 'P'
    {0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110},  // 'S'
    {0b00000, 0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111},  // 'a'
    {0b00000, 0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b01110},  // 'e'
    {0b00000, 0b00000, 0b11010, 0b10101, 0b10101, 0b10001, 0b10001},  // 'm'
    {0b00000, 0b00000, 0b10110, 0b11001, 0b10000, 0b10000, 0b10000},  // 'r'
    {0b00000, 0b00000, 0b01111, 0b10000, 0b01110, 0b00001, 0b11110},  // 's'
}};

uint8_t GlyphIndex(char character) {
    const size_t index = kGlyphChars.find(character);
    return index == std::string_view::npos ? uint8_t{0} : static_cast<uint8_t>(index);
}

uint8_t GlyphRow(uint8_t glyph, int row) {
    if (glyph >= kGlyphCount || row < 0 || row >= kGlyphHeight) {
        return 0;
    }
    return kFont[glyph][static_cast<size_t>(row)];
}

bool AnyLineVisible(const LineFlags &flags) { return flags.fps || flags.frame_time || flags.frame_number; }

const char *SharedSwapchainsWarning(const LineFlags &flags) {
    return AnyLineVisible(flags) ? "no overlay on swapchains from vkCreateSharedSwapchainsKHR" : nullptr;
}

bool FirstTimeShown(ShownMessages &shown, std::string_view message) {
    if (shown.find(message) != shown.end()) {
        return false;
    }
    shown.emplace(message);
    return true;
}

void FormatOverlayLines(const StatsSnapshot &snapshot, uint64_t frame_number, const LineFlags &flags,
                        std::vector<std::string> &lines) {
    size_t count = 0;
    const auto next_line = [&]() -> std::string & {
        if (count == lines.size()) {
            lines.emplace_back();
        }
        return lines[count++];
    };
    // Longer than any line the overlay can show; snprintf truncates the rest
    std::array<char, size_t{kMaxColumns} * 2> text = {};

    if (flags.fps) {
        if (snapshot.valid) {
            (void)snprintf(text.data(), text.size(), "FPS %.1f", snapshot.fps);
            next_line().assign(text.data());
        } else {
            next_line().assign("FPS -");
        }
    }
    if (flags.frame_time) {
        if (snapshot.valid) {
            const int length =
                snprintf(text.data(), text.size(), "%.2f ms (%.2f-%.2f)", snapshot.avg_ms, snapshot.min_ms, snapshot.max_ms);
            // After a very long frame, such as a debugger break, drop the decimals rather than cut off the longest time
            if (length > static_cast<int>(kMaxColumns)) {
                (void)snprintf(text.data(), text.size(), "%.0f ms (%.0f-%.0f)", snapshot.avg_ms, snapshot.min_ms, snapshot.max_ms);
            }
            next_line().assign(text.data());
        } else {
            next_line().assign("- ms");
        }
    }
    if (flags.frame_number) {
        (void)snprintf(text.data(), text.size(), "Frame %llu", static_cast<unsigned long long>(frame_number));
        next_line().assign(text.data());
    }
    lines.resize(count);
}

std::vector<std::string> FormatOverlayLines(const StatsSnapshot &snapshot, uint64_t frame_number, const LineFlags &flags) {
    std::vector<std::string> lines;
    FormatOverlayLines(snapshot, frame_number, flags, lines);
    return lines;
}

// Every 540 pixels of the image's shorter side add one image pixel per font pixel at 100% scale
constexpr uint32_t kImagePixelsPerSize = 540;
constexpr uint32_t kMinBasePixelSize = 2;
constexpr uint32_t kPercent = 100;

// At 100% scale a font pixel is the shorter side of the image divided by 540, rounded, and at least 2 image pixels.
// That is 2 until the shorter side reaches 1350, then 3 at 1728p and 4 at 2160p.
static uint32_t PixelSize(VkExtent2D visible_extent, int scale_percent) {
    const uint32_t shorter_side = std::min(visible_extent.width, visible_extent.height);
    const uint32_t base = std::max(kMinBasePixelSize, (shorter_side + (kImagePixelsPerSize / 2)) / kImagePixelsPerSize);
    const auto scale = static_cast<uint32_t>(std::clamp(scale_percent, kMinScalePercent, kMaxScalePercent));
    return std::max(1u, ((base * scale) + (kPercent / 2)) / kPercent);
}

namespace {
struct Point {
    int32_t x;
    int32_t y;
};
}  // namespace

// Maps a point from visible space to image space. Image content is the visible content rotated clockwise.
[[nodiscard]] static Point VisibleToImage(uint32_t rotation, VkExtent2D visible, int32_t vx, int32_t vy) {
    const auto vw = static_cast<int32_t>(visible.width);
    const auto vh = static_cast<int32_t>(visible.height);
    switch (rotation) {
        case 1:
            return {vh - vy, vx};
        case 2:
            return {vw - vx, vh - vy};
        case 3:
            return {vy, vw - vx};
        default:
            return {vx, vy};
    }
}

OverlayLayout ComputeOverlayLayout(VkExtent2D image_extent, Position position, int scale_percent,
                                   VkSurfaceTransformFlagBitsKHR transform, const std::vector<std::string> &lines) {
    OverlayLayout layout;

    switch (transform) {
        case VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR:
            layout.rotation = 1;
            break;
        case VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR:
            layout.rotation = 2;
            break;
        case VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR:
            layout.rotation = 3;
            break;
        case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR:
            layout.rotation = 1;
            layout.transform_warning = true;
            break;
        case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR:
            layout.rotation = 2;
            layout.transform_warning = true;
            break;
        case VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR:
            layout.rotation = 3;
            layout.transform_warning = true;
            break;
        case VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:
            break;
        default:
            layout.transform_warning = true;
            break;
    }

    layout.visible_extent = image_extent;
    if (layout.rotation % 2 == 1) {
        std::swap(layout.visible_extent.width, layout.visible_extent.height);
    }

    const size_t line_count = std::min(lines.size(), static_cast<size_t>(kMaxLines));
    size_t columns = 0;
    for (size_t i = 0; i < line_count; ++i) {
        columns = std::max(columns, std::min(lines[i].size(), static_cast<size_t>(kMaxColumns)));
    }

    layout.pixel_size = PixelSize(layout.visible_extent, scale_percent);
    const auto px = static_cast<int32_t>(layout.pixel_size);
    const int32_t width = ((2 * kPadding) + (static_cast<int32_t>(columns) * kCharAdvance) - 1) * px;
    const int32_t height = ((2 * kPadding) + (static_cast<int32_t>(line_count) * kLineAdvance) - 2) * px;
    const int32_t margin = kMargin * px;
    const auto vw = static_cast<int32_t>(layout.visible_extent.width);
    const auto vh = static_cast<int32_t>(layout.visible_extent.height);

    const bool right = position == Position::TopRight || position == Position::BottomRight;
    const bool bottom = position == Position::BottomLeft || position == Position::BottomRight;
    // A panel bigger than the image is clipped, so the image rect is always a valid scissor.
    const int32_t x0 = std::clamp(right ? vw - margin - width : margin, 0, vw);
    const int32_t y0 = std::clamp(bottom ? vh - margin - height : margin, 0, vh);
    layout.visible_rect[0] = x0;
    layout.visible_rect[1] = y0;
    layout.visible_rect[2] = std::min(x0 + width, vw);
    layout.visible_rect[3] = std::min(y0 + height, vh);

    const Point first = VisibleToImage(layout.rotation, layout.visible_extent, layout.visible_rect[0], layout.visible_rect[1]);
    const Point second = VisibleToImage(layout.rotation, layout.visible_extent, layout.visible_rect[2], layout.visible_rect[3]);
    layout.image_rect[0] = std::min(first.x, second.x);
    layout.image_rect[1] = std::min(first.y, second.y);
    layout.image_rect[2] = std::max(first.x, second.x);
    layout.image_rect[3] = std::max(first.y, second.y);

    return layout;
}

OverlayPushConstants PackOverlayPushConstants(const OverlayLayout &layout, const std::vector<std::string> &lines, float white,
                                              bool opaque) {
    OverlayPushConstants push_constants = {};
    push_constants.visible_rect = layout.visible_rect;
    push_constants.visible_extent =
        (layout.visible_extent.width & kExtentWidthMask) | (layout.visible_extent.height << kExtentHeightShift);

    const uint32_t line_count = std::min(static_cast<uint32_t>(lines.size()), kMaxLines);
    push_constants.params = (layout.pixel_size & kParamsPixelSizeMask) |
                            ((layout.rotation & kParamsRotationMask) << kParamsRotationShift) |
                            (line_count << kParamsLineCountShift) | ((opaque ? 1u : 0u) << kParamsOpaqueShift);
    push_constants.white = white;

    for (uint32_t line = 0; line < line_count; ++line) {
        const uint32_t columns = std::min(static_cast<uint32_t>(lines[line].size()), kMaxColumns);
        for (uint32_t column = 0; column < columns; ++column) {
            const uint32_t byte = (line * kMaxColumns) + column;
            push_constants.glyphs[byte / kGlyphsPerWord] |= static_cast<uint32_t>(GlyphIndex(lines[line][column]))
                                                            << ((byte % kGlyphsPerWord) * kBitsPerGlyph);
        }
    }
    return push_constants;
}

constexpr double kHdrWhiteNits = 200.0;
// scRGB 1.0 is 80 nits
constexpr double kScRgbWhiteNits = 80.0;
// HLG reference white is 75% signal (ITU-R BT.2408)
constexpr float kHlgReferenceWhite = 0.75f;

// SMPTE ST 2084 (PQ) constants, named as in the standard
constexpr double kPqPeakNits = 10000.0;
constexpr double kPqM1 = 2610.0 / 16384.0;
constexpr double kPqM2 = 2523.0 / 4096.0 * 128.0;
constexpr double kPqC1 = 3424.0 / 4096.0;
constexpr double kPqC2 = 2413.0 / 4096.0 * 32.0;
constexpr double kPqC3 = 2392.0 / 4096.0 * 32.0;

// SMPTE ST 2084 (PQ) inverse EOTF
[[nodiscard]] static float EncodePq(double nits) {
    const double y = std::pow(nits / kPqPeakNits, kPqM1);
    return static_cast<float>(std::pow((kPqC1 + (kPqC2 * y)) / (1.0 + (kPqC3 * y)), kPqM2));
}

WhiteLevel WhiteForColorSpace(VkColorSpaceKHR color_space) {
    switch (color_space) {
        case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
        case VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT:
        case VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT:
        case VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT:
        case VK_COLOR_SPACE_BT709_LINEAR_EXT:
        case VK_COLOR_SPACE_BT709_NONLINEAR_EXT:
        case VK_COLOR_SPACE_BT2020_LINEAR_EXT:
        case VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT:
        case VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT:
        case VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT:
            return {1.0f, true};
        case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:
            return {static_cast<float>(kHdrWhiteNits / kScRgbWhiteNits), true};
        case VK_COLOR_SPACE_HDR10_ST2084_EXT:
            return {EncodePq(kHdrWhiteNits), true};
        case VK_COLOR_SPACE_HDR10_HLG_EXT:
            return {kHlgReferenceWhite, true};
        default:
            return {1.0f, false};
    }
}

SwapchainPatch PatchSwapchainCreateInfo(const VkSwapchainCreateInfoKHR &create_info, const LineFlags &lines) {
    SwapchainPatch patch;
    patch.create_info = create_info;
    patch.eligibility = SwapchainOverlayEligibility(create_info);
    if (!AnyLineVisible(lines)) {
        patch.eligibility.ok = false;
        patch.eligibility.reason = nullptr;
    }
    if (patch.eligibility.ok) {
        // Every surface supports color attachment usage, so this is always valid.
        patch.create_info.imageUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    return patch;
}

Eligibility SwapchainOverlayEligibility(const VkSwapchainCreateInfoKHR &create_info) {
    Eligibility eligibility;
    if (create_info.presentMode == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR ||
        create_info.presentMode == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR) {
        eligibility.ok = false;
        eligibility.reason = "shared present modes are not supported";
    } else if ((create_info.flags & VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR) != 0u) {
        eligibility.ok = false;
        eligibility.reason = "protected swapchains are not supported";
    }
    eligibility.deferred = (create_info.flags & VK_SWAPCHAIN_CREATE_DEFERRED_MEMORY_ALLOCATION_BIT_KHR) != 0;
    return eligibility;
}

bool ShouldDrawOverlay(VkQueueFlags queue_flags, bool any_overlay_swapchain, bool any_line_visible) {
    return ((queue_flags & VK_QUEUE_GRAPHICS_BIT) != 0u) && any_overlay_swapchain && any_line_visible;
}

bool ImageViewUsageAvailable(uint32_t instance_api_version, uint32_t device_api_version, bool maintenance2_enabled) {
    // Patch versions add no functionality
    const auto major_minor = [](uint32_t version) {
        return VK_MAKE_API_VERSION(0, VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version), 0);
    };
    return maintenance2_enabled ||
           std::min(major_minor(instance_api_version), major_minor(device_api_version)) >= VK_API_VERSION_1_1;
}

MonitorSettings ParseSettings(const RawSettings &raw) {
    MonitorSettings settings;

    if (raw.position == "TOP_RIGHT") {
        settings.position = Position::TopRight;
    } else if (raw.position == "BOTTOM_LEFT") {
        settings.position = Position::BottomLeft;
    } else if (raw.position == "BOTTOM_RIGHT") {
        settings.position = Position::BottomRight;
    } else {
        settings.position = Position::TopLeft;
    }

    settings.scale_percent = std::clamp(raw.scale, kMinScalePercent, kMaxScalePercent);
    settings.update_interval = std::chrono::milliseconds(std::max(raw.update_interval_ms, kMinUpdateIntervalMs));
    settings.lines.fps = raw.show_fps;
    settings.lines.frame_time = raw.show_frame_time;
    settings.lines.frame_number = raw.show_frame_number;
    settings.log_warnings = raw.log_warnings;
    return settings;
}

}  // namespace monitor
