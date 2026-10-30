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

#include <gtest/gtest.h>

#include <atomic>
#include <cctype>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iterator>
#include <set>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "monitor_overlay.h"
#include "monitor_overlay_shaders.h"
#include "monitor_settings.h"

using namespace std::chrono_literals;
using Clock = std::chrono::steady_clock;

// Feeds `count` presents spaced `delta` apart, starting at `start`. Returns the time of the last present.
static Clock::time_point Present(monitor::FrameStats& stats, Clock::time_point start, Clock::duration delta, int count,
                                 std::chrono::milliseconds interval = 500ms) {
    Clock::time_point now = start;
    for (int i = 0; i < count; ++i) {
        monitor::RecordPresent(stats, now, interval);
        now += delta;
    }
    return now - delta;
}

TEST(MonitorOverlay, StatsAt144HzAfterFirstInterval) {
    monitor::FrameStats stats;
    const auto delta = std::chrono::duration_cast<Clock::duration>(std::chrono::duration<double>(1.0 / 144.0));

    Present(stats, Clock::time_point{}, delta, 10);
    EXPECT_FALSE(stats.snapshot.valid);

    Present(stats, Clock::time_point{} + 10 * delta, delta, 144);
    ASSERT_TRUE(stats.snapshot.valid);
    EXPECT_NEAR(stats.snapshot.fps, 144.0, 0.5);
    EXPECT_NEAR(stats.snapshot.avg_ms, 1000.0 / 144.0, 0.01);
}

TEST(MonitorOverlay, StatsUseSubSecondIntervals) {
    // A one-second clock would report 0 or a whole number of frames here.
    monitor::FrameStats stats;
    const auto delta = std::chrono::duration_cast<Clock::duration>(std::chrono::duration<double>(1.0 / 60.0));

    Present(stats, Clock::time_point{}, delta, 17, 250ms);
    ASSERT_TRUE(stats.snapshot.valid);
    EXPECT_NEAR(stats.snapshot.fps, 60.0, 0.1);
}

TEST(MonitorOverlay, StatsReportHitchAsMaxFrameTime) {
    monitor::FrameStats stats;
    const auto delta = std::chrono::duration_cast<Clock::duration>(std::chrono::duration<double>(1.0 / 60.0));

    Clock::time_point last = Present(stats, Clock::time_point{}, delta, 10, 2000ms);
    last = Present(stats, last + 1000ms, delta, 120, 2000ms);

    ASSERT_TRUE(stats.snapshot.valid);
    EXPECT_NEAR(stats.snapshot.max_ms, 1000.0, 0.01);
    EXPECT_NEAR(stats.snapshot.min_ms, 1000.0 / 60.0, 0.01);
    EXPECT_GT(stats.snapshot.avg_ms, 1000.0 / 60.0);
}

TEST(MonitorOverlay, FrameNumbersStartAtZero) {
    std::atomic<uint64_t> counter{0};
    EXPECT_EQ(monitor::NextFrameNumber(counter), 0u);
    EXPECT_EQ(monitor::NextFrameNumber(counter), 1u);
    EXPECT_EQ(monitor::NextFrameNumber(counter), 2u);
}

TEST(MonitorOverlay, FrameNumbersAreUniqueAcrossThreads) {
    std::atomic<uint64_t> counter{0};
    constexpr int kPerThread = 10000;
    std::vector<uint64_t> first_frames;
    std::vector<uint64_t> second_frames;
    first_frames.reserve(kPerThread);
    second_frames.reserve(kPerThread);

    std::thread first_thread([&] {
        for (int i = 0; i < kPerThread; ++i) {
            first_frames.push_back(monitor::NextFrameNumber(counter));
        }
    });
    std::thread second_thread([&] {
        for (int i = 0; i < kPerThread; ++i) {
            second_frames.push_back(monitor::NextFrameNumber(counter));
        }
    });
    first_thread.join();
    second_thread.join();

    std::set<uint64_t> all(first_frames.begin(), first_frames.end());
    all.insert(second_frames.begin(), second_frames.end());
    EXPECT_EQ(all.size(), 2u * kPerThread);
    EXPECT_EQ(*all.rbegin(), (2u * kPerThread) - 1);
}

namespace {
struct GlyphArt {
    char character;
    const char* rows[monitor::kGlyphHeight];
};
}  // namespace

constexpr GlyphArt kExpectedGlyphs[] = {
    {' ', {".....", ".....", ".....", ".....", ".....", ".....", "....."}},
    {'0', {".###.", "#...#", "#..##", "#.#.#", "##..#", "#...#", ".###."}},
    {'1', {"..#..", ".##..", "..#..", "..#..", "..#..", "..#..", ".###."}},
    {'2', {".###.", "#...#", "....#", "...#.", "..#..", ".#...", "#####"}},
    {'3', {"#####", "...#.", "..#..", "...#.", "....#", "#...#", ".###."}},
    {'4', {"...#.", "..##.", ".#.#.", "#..#.", "#####", "...#.", "...#."}},
    {'5', {"#####", "#....", "####.", "....#", "....#", "#...#", ".###."}},
    {'6', {"..##.", ".#...", "#....", "####.", "#...#", "#...#", ".###."}},
    {'7', {"#####", "....#", "...#.", "..#..", ".#...", ".#...", ".#..."}},
    {'8', {".###.", "#...#", "#...#", ".###.", "#...#", "#...#", ".###."}},
    {'9', {".###.", "#...#", "#...#", ".####", "....#", "...#.", ".##.."}},
    {'.', {".....", ".....", ".....", ".....", ".....", ".##..", ".##.."}},
    {'-', {".....", ".....", ".....", ".###.", ".....", ".....", "....."}},
    {'(', {"...#.", "..#..", ".#...", ".#...", ".#...", "..#..", "...#."}},
    {')', {".#...", "..#..", "...#.", "...#.", "...#.", "..#..", ".#..."}},
    {'F', {"#####", "#....", "#....", "####.", "#....", "#....", "#...."}},
    {'P', {"####.", "#...#", "#...#", "####.", "#....", "#....", "#...."}},
    {'S', {".####", "#....", "#....", ".###.", "....#", "....#", "####."}},
    {'a', {".....", ".....", ".###.", "....#", ".####", "#...#", ".####"}},
    {'e', {".....", ".....", ".###.", "#...#", "#####", "#....", ".###."}},
    {'m', {".....", ".....", "##.#.", "#.#.#", "#.#.#", "#...#", "#...#"}},
    {'r', {".....", ".....", "#.##.", "##..#", "#....", "#....", "#...."}},
    {'s', {".....", ".....", ".####", "#....", ".###.", "....#", "####."}},
};

static std::string GlyphRowArt(uint8_t glyph, int row) {
    std::string art;
    const uint8_t bits = monitor::GlyphRow(glyph, row);
    for (int x = 0; x < monitor::kGlyphWidth; ++x) {
        art += ((bits & (1u << (monitor::kGlyphWidth - 1 - x))) != 0u) ? '#' : '.';
    }
    return art;
}

TEST(MonitorOverlay, FontDrawsEveryGlyph) {
    std::set<uint8_t> indices;
    for (const GlyphArt& expected : kExpectedGlyphs) {
        const uint8_t glyph = monitor::GlyphIndex(expected.character);
        indices.insert(glyph);
        for (int row = 0; row < monitor::kGlyphHeight; ++row) {
            EXPECT_EQ(GlyphRowArt(glyph, row), expected.rows[row]) << "glyph '" << expected.character << "' row " << row;
        }
    }
    EXPECT_EQ(indices.size(), std::size(kExpectedGlyphs));
    EXPECT_EQ(static_cast<size_t>(monitor::kGlyphCount), std::size(kExpectedGlyphs));
}

TEST(MonitorOverlay, FontDrawsUnknownCharactersBlank) {
    EXPECT_EQ(monitor::GlyphIndex('?'), monitor::GlyphIndex(' '));
    EXPECT_EQ(monitor::GlyphIndex('Z'), monitor::GlyphIndex(' '));
    EXPECT_EQ(monitor::GlyphIndex('\0'), monitor::GlyphIndex(' '));
}

static monitor::StatsSnapshot Snapshot(double fps, double avg_ms, double min_ms, double max_ms) {
    monitor::StatsSnapshot snapshot;
    snapshot.valid = true;
    snapshot.fps = fps;
    snapshot.avg_ms = avg_ms;
    snapshot.min_ms = min_ms;
    snapshot.max_ms = max_ms;
    return snapshot;
}

TEST(MonitorOverlay, TextShowsAllLines) {
    const auto lines = monitor::FormatOverlayLines(Snapshot(143.24, 6.981, 6.1, 9.4), 123456, monitor::LineFlags{});
    const std::vector<std::string> expected = {"FPS 143.2", "6.98 ms (6.10-9.40)", "Frame 123456"};
    EXPECT_EQ(lines, expected);
}

TEST(MonitorOverlay, TextShowsPlaceholdersBeforeFirstInterval) {
    const auto lines = monitor::FormatOverlayLines(monitor::StatsSnapshot{}, 0, monitor::LineFlags{});
    const std::vector<std::string> expected = {"FPS -", "- ms", "Frame 0"};
    EXPECT_EQ(lines, expected);
}

TEST(MonitorOverlay, TextOmitsHiddenLines) {
    monitor::LineFlags flags;
    flags.fps = false;
    flags.frame_time = false;
    EXPECT_EQ(monitor::FormatOverlayLines(Snapshot(60, 16.67, 16, 17), 42, flags), std::vector<std::string>{"Frame 42"});

    flags = monitor::LineFlags{};
    flags.frame_number = false;
    EXPECT_EQ(monitor::FormatOverlayLines(Snapshot(60, 16.67, 16, 17), 42, flags).size(), 2u);

    flags.fps = false;
    flags.frame_time = false;
    EXPECT_TRUE(monitor::FormatOverlayLines(Snapshot(60, 16.67, 16, 17), 42, flags).empty());
}

static const std::vector<std::string> kThreeLines = {"FPS 143.2", "6.98 ms (6.10-9.40)", "Frame 123456"};

TEST(MonitorOverlay, LayoutTopLeftAtDefaultScale) {
    const auto layout = monitor::ComputeOverlayLayout({1920, 1080}, monitor::Position::TopLeft, 100,
                                                      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, kThreeLines);
    // 2 image pixels per font pixel, 4 font pixels of margin, 19 characters by 3 lines plus padding.
    EXPECT_EQ(layout.pixel_size, 2u);
    EXPECT_EQ(layout.visible_rect, (monitor::Rect{8, 8, 8 + 238, 8 + 62}));
    EXPECT_EQ(layout.image_rect, layout.visible_rect);
    EXPECT_EQ(layout.rotation, 0u);
    EXPECT_FALSE(layout.transform_warning);
}

TEST(MonitorOverlay, LayoutPlacesEachCorner) {
    constexpr VkExtent2D extent = {1920, 1080};
    const auto top_left =
        monitor::ComputeOverlayLayout(extent, monitor::Position::TopLeft, 100, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, kThreeLines);
    const int32_t w = top_left.visible_rect[2] - top_left.visible_rect[0];
    const int32_t h = top_left.visible_rect[3] - top_left.visible_rect[1];

    const auto top_right =
        monitor::ComputeOverlayLayout(extent, monitor::Position::TopRight, 100, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, kThreeLines);
    EXPECT_EQ(top_right.visible_rect, (monitor::Rect{1920 - 8 - w, 8, 1920 - 8, 8 + h}));

    const auto bottom_left = monitor::ComputeOverlayLayout(extent, monitor::Position::BottomLeft, 100,
                                                           VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, kThreeLines);
    EXPECT_EQ(bottom_left.visible_rect, (monitor::Rect{8, 1080 - 8 - h, 8 + w, 1080 - 8}));

    const auto bottom_right = monitor::ComputeOverlayLayout(extent, monitor::Position::BottomRight, 100,
                                                            VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, kThreeLines);
    EXPECT_EQ(bottom_right.visible_rect, (monitor::Rect{1920 - 8 - w, 1080 - 8 - h, 1920 - 8, 1080 - 8}));
}

TEST(MonitorOverlay, LayoutClampsScale) {
    const auto layout = [](int scale) {
        return monitor::ComputeOverlayLayout({1920, 1080}, monitor::Position::TopLeft, scale, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                             kThreeLines);
    };
    EXPECT_EQ(layout(10).pixel_size, 1u);
    EXPECT_EQ(layout(50).pixel_size, 1u);
    EXPECT_EQ(layout(75).pixel_size, 2u);
    EXPECT_EQ(layout(200).pixel_size, 4u);
    EXPECT_EQ(layout(400).pixel_size, 8u);
    EXPECT_EQ(layout(1000).pixel_size, 8u);
}

TEST(MonitorOverlay, LayoutGrowsWithResolution) {
    const auto pixel_size = [](VkExtent2D extent, int scale, VkSurfaceTransformFlagBitsKHR transform) {
        return monitor::ComputeOverlayLayout(extent, monitor::Position::TopLeft, scale, transform, kThreeLines).pixel_size;
    };
    const VkSurfaceTransformFlagBitsKHR identity = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    EXPECT_EQ(pixel_size({500, 500}, 100, identity), 2u);
    EXPECT_EQ(pixel_size({1920, 1080}, 100, identity), 2u);
    EXPECT_EQ(pixel_size({3072, 1728}, 100, identity), 3u);
    EXPECT_EQ(pixel_size({3840, 2160}, 100, identity), 4u);

    // The shorter side counts, so a tall portrait image doesn't get huge text
    EXPECT_EQ(pixel_size({1080, 2400}, 100, identity), 2u);
    EXPECT_EQ(pixel_size({1080, 2400}, 100, VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR), 2u);

    // The scale setting applies on top
    EXPECT_EQ(pixel_size({1920, 1080}, 50, identity), 1u);
    EXPECT_EQ(pixel_size({3840, 2160}, 200, identity), 8u);
}

TEST(MonitorOverlay, LayoutStaysOnScreenWhenPanelIsTooBig) {
    const auto layout = monitor::ComputeOverlayLayout({64, 32}, monitor::Position::BottomRight, 400,
                                                      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, kThreeLines);
    EXPECT_GE(layout.visible_rect[0], 0);
    EXPECT_GE(layout.visible_rect[1], 0);
}

TEST(MonitorOverlay, LayoutClipsPanelToTheImage) {
    // The image rect becomes a scissor, which must stay inside the image.
    for (const VkSurfaceTransformFlagBitsKHR transform :
         {VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR, VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR,
          VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR}) {
        constexpr VkExtent2D image = {48, 100};
        const auto layout = monitor::ComputeOverlayLayout(image, monitor::Position::TopLeft, 400, transform, kThreeLines);
        EXPECT_LE(layout.visible_rect[2], static_cast<int32_t>(layout.visible_extent.width)) << transform;
        EXPECT_LE(layout.visible_rect[3], static_cast<int32_t>(layout.visible_extent.height)) << transform;
        EXPECT_GE(layout.image_rect[0], 0) << transform;
        EXPECT_GE(layout.image_rect[1], 0) << transform;
        EXPECT_LE(layout.image_rect[2], static_cast<int32_t>(image.width)) << transform;
        EXPECT_LE(layout.image_rect[3], static_cast<int32_t>(image.height)) << transform;
    }
}

// Image content is the visible content rotated clockwise by the swapchain's preTransform.
TEST(MonitorOverlay, LayoutRotatesIntoImageSpace) {
    struct Case {
        VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        uint32_t rotation = 0;
        monitor::Rect image_rect = {};
    };
    // Portrait-native 1080x2400 image shown in landscape; visible panel is {8, 8, 246, 70}.
    const Case cases[] = {
        {VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR, 1, {1080 - 70, 8, 1080 - 8, 246}},
        {VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR, 3, {8, 2400 - 246, 70, 2400 - 8}},
    };
    for (const Case& test_case : cases) {
        const auto layout =
            monitor::ComputeOverlayLayout({1080, 2400}, monitor::Position::TopLeft, 100, test_case.transform, kThreeLines);
        EXPECT_EQ(layout.visible_extent.width, 2400u);
        EXPECT_EQ(layout.visible_extent.height, 1080u);
        EXPECT_EQ(layout.visible_rect, (monitor::Rect{8, 8, 246, 70}));
        EXPECT_EQ(layout.rotation, test_case.rotation);
        EXPECT_EQ(layout.image_rect, test_case.image_rect);
        EXPECT_FALSE(layout.transform_warning);
    }

    const auto upside_down = monitor::ComputeOverlayLayout({1920, 1080}, monitor::Position::TopLeft, 100,
                                                           VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR, kThreeLines);
    EXPECT_EQ(upside_down.rotation, 2u);
    EXPECT_EQ(upside_down.image_rect, (monitor::Rect{1920 - 246, 1080 - 70, 1920 - 8, 1080 - 8}));
}

TEST(MonitorOverlay, LayoutDrawsMirrorAndInheritTransformsUnmirrored) {
    struct Case {
        VkSurfaceTransformFlagBitsKHR transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        uint32_t rotation = 0;
    };
    const Case cases[] = {
        {VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR, 0},
        {VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR, 1},
        {VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR, 2},
        {VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR, 3},
        {VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR, 0},
    };
    for (const Case& test_case : cases) {
        const auto layout =
            monitor::ComputeOverlayLayout({1920, 1080}, monitor::Position::TopLeft, 100, test_case.transform, kThreeLines);
        EXPECT_EQ(layout.rotation, test_case.rotation) << test_case.transform;
        EXPECT_TRUE(layout.transform_warning) << test_case.transform;
    }
}

// Glyph k of line i is byte (i * kMaxColumns + k) of glyphs[], little-endian within each uint32_t.
static uint8_t PackedGlyph(const monitor::OverlayPushConstants& push_constants, uint32_t line, uint32_t column) {
    const uint32_t byte = (line * monitor::kMaxColumns) + column;
    return static_cast<uint8_t>(push_constants.glyphs[byte / 4] >> ((byte % 4) * 8));
}

TEST(MonitorOverlay, PushConstantsFitTheGuaranteedLimit) { EXPECT_EQ(sizeof(monitor::OverlayPushConstants), 128u); }

TEST(MonitorOverlay, PushConstantsEncodeLayout) {
    const auto layout = monitor::ComputeOverlayLayout({1080, 2400}, monitor::Position::TopLeft, 200,
                                                      VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR, kThreeLines);
    const auto push_constants = monitor::PackOverlayPushConstants(layout, kThreeLines, 2.5f, true);

    EXPECT_EQ(push_constants.visible_rect, layout.visible_rect);
    EXPECT_EQ(push_constants.visible_extent & 0xFFFF, 2400u);
    EXPECT_EQ(push_constants.visible_extent >> 16, 1080u);
    EXPECT_EQ(push_constants.params & 0xFF, 4u);         // pixel size
    EXPECT_EQ((push_constants.params >> 8) & 0x3, 1u);   // rotation
    EXPECT_EQ((push_constants.params >> 10) & 0x3, 3u);  // line count
    EXPECT_EQ((push_constants.params >> 12) & 0x1, 1u);  // opaque
    EXPECT_FLOAT_EQ(push_constants.white, 2.5f);

    const auto blended = monitor::PackOverlayPushConstants(layout, kThreeLines, 1.0f, false);
    EXPECT_EQ((blended.params >> 12) & 0x1, 0u);
}

TEST(MonitorOverlay, PushConstantsEncodeGlyphs) {
    const auto layout = monitor::ComputeOverlayLayout({1920, 1080}, monitor::Position::TopLeft, 100,
                                                      VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, kThreeLines);
    const auto push_constants = monitor::PackOverlayPushConstants(layout, kThreeLines, 1.0f, false);

    for (uint32_t line = 0; line < kThreeLines.size(); ++line) {
        for (uint32_t column = 0; column < monitor::kMaxColumns; ++column) {
            const char expected = column < kThreeLines[line].size() ? kThreeLines[line][column] : ' ';
            EXPECT_EQ(PackedGlyph(push_constants, line, column), monitor::GlyphIndex(expected))
                << "line " << line << " column " << column;
        }
    }
}

TEST(MonitorOverlay, PushConstantsTruncateLongText) {
    const std::vector<std::string> lines = {std::string(40, '1'), "2", "3", "4"};
    const auto layout =
        monitor::ComputeOverlayLayout({1920, 1080}, monitor::Position::TopLeft, 100, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, lines);
    const auto push_constants = monitor::PackOverlayPushConstants(layout, lines, 1.0f, false);

    EXPECT_EQ((push_constants.params >> 10) & 0x3, 3u);
    EXPECT_EQ(PackedGlyph(push_constants, 0, monitor::kMaxColumns - 1), monitor::GlyphIndex('1'));
    EXPECT_EQ(PackedGlyph(push_constants, 1, 0), monitor::GlyphIndex('2'));
    EXPECT_EQ(PackedGlyph(push_constants, 2, 0), monitor::GlyphIndex('3'));

    // The panel is sized for the text that is actually drawn.
    const std::vector<std::string> drawn = {std::string(monitor::kMaxColumns, '1'), "2", "3"};
    const auto drawn_layout =
        monitor::ComputeOverlayLayout({1920, 1080}, monitor::Position::TopLeft, 100, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, drawn);
    EXPECT_EQ(layout.visible_rect, drawn_layout.visible_rect);
}

TEST(MonitorOverlay, WhiteIsOneForSdrColorSpaces) {
    constexpr VkColorSpaceKHR sdr[] = {
        VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,          VK_COLOR_SPACE_DISPLAY_P3_NONLINEAR_EXT, VK_COLOR_SPACE_DISPLAY_P3_LINEAR_EXT,
        VK_COLOR_SPACE_DCI_P3_NONLINEAR_EXT,        VK_COLOR_SPACE_BT709_LINEAR_EXT,         VK_COLOR_SPACE_BT709_NONLINEAR_EXT,
        VK_COLOR_SPACE_BT2020_LINEAR_EXT,           VK_COLOR_SPACE_ADOBERGB_LINEAR_EXT,      VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT,
        VK_COLOR_SPACE_EXTENDED_SRGB_NONLINEAR_EXT,
    };
    for (const VkColorSpaceKHR color_space : sdr) {
        const auto white = monitor::WhiteForColorSpace(color_space);
        EXPECT_FLOAT_EQ(white.value, 1.0f) << color_space;
        EXPECT_TRUE(white.known) << color_space;
    }
}

TEST(MonitorOverlay, WhiteIsAbout200NitsForHdrColorSpaces) {
    const auto scrgb = monitor::WhiteForColorSpace(VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT);
    EXPECT_FLOAT_EQ(scrgb.value, 2.5f);  // 1.0 is 80 nits
    EXPECT_TRUE(scrgb.known);

    const auto pq_white = monitor::WhiteForColorSpace(VK_COLOR_SPACE_HDR10_ST2084_EXT);
    EXPECT_NEAR(pq_white.value, 0.5791f, 0.0001f);  // ST 2084 encoding of 200 nits
    EXPECT_TRUE(pq_white.known);

    const auto hlg = monitor::WhiteForColorSpace(VK_COLOR_SPACE_HDR10_HLG_EXT);
    EXPECT_FLOAT_EQ(hlg.value, 0.75f);  // BT.2408 reference white
    EXPECT_TRUE(hlg.known);
}

TEST(MonitorOverlay, WhiteFallsBackToOneForUnknownColorSpaces) {
    for (const VkColorSpaceKHR color_space :
         {VK_COLOR_SPACE_PASS_THROUGH_EXT, VK_COLOR_SPACE_DISPLAY_NATIVE_AMD, static_cast<VkColorSpaceKHR>(0x7FFF0000)}) {
        const auto white = monitor::WhiteForColorSpace(color_space);
        EXPECT_FLOAT_EQ(white.value, 1.0f) << color_space;
        EXPECT_FALSE(white.known) << color_space;
    }
}

static VkSwapchainCreateInfoKHR SwapchainInfo() {
    VkSwapchainCreateInfoKHR info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    info.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
    info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    info.imageExtent = {1920, 1080};
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    return info;
}

TEST(MonitorOverlay, PatchAddsColorAttachmentUsage) {
    VkImageFormatListCreateInfo format_list = {VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO};
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.pNext = &format_list;
    info.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    const VkSwapchainCreateInfoKHR patched = monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{}).create_info;
    EXPECT_EQ(patched.imageUsage, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    EXPECT_EQ(patched.pNext, &format_list);
    EXPECT_EQ(patched.imageFormat, info.imageFormat);
    EXPECT_EQ(patched.presentMode, info.presentMode);
    EXPECT_EQ(info.imageUsage, VK_IMAGE_USAGE_TRANSFER_DST_BIT);  // The app's struct is untouched
}

// vkCreateSharedSwapchainsKHR swapchains never get the overlay; that's only worth saying if there was one to draw
TEST(MonitorOverlay, SharedSwapchainsWarningOnlyWhenALineIsVisible) {
    const char* warning = monitor::SharedSwapchainsWarning(monitor::LineFlags{});
    ASSERT_NE(warning, nullptr);
    EXPECT_NE(std::string(warning).find("vkCreateSharedSwapchainsKHR"), std::string::npos);
    EXPECT_EQ(monitor::SharedSwapchainsWarning(monitor::LineFlags{false, false, false}), nullptr);
}

// With every line hidden nothing is drawn, so the app's swapchain stays as it asked for, and there's nothing to warn about
TEST(MonitorOverlay, PatchLeavesSwapchainsAloneWhenEveryLineIsHidden) {
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    const monitor::LineFlags hidden = {false, false, false};
    EXPECT_FALSE(monitor::AnyLineVisible(hidden));
    EXPECT_TRUE(monitor::AnyLineVisible(monitor::LineFlags{}));

    const monitor::SwapchainPatch patch = monitor::PatchSwapchainCreateInfo(info, hidden);
    EXPECT_FALSE(patch.eligibility.ok);
    EXPECT_EQ(patch.eligibility.reason, nullptr);
    EXPECT_EQ(patch.create_info.imageUsage, VK_IMAGE_USAGE_TRANSFER_DST_BIT);

    const monitor::LineFlags frame_number_only = {false, false, true};
    EXPECT_TRUE(monitor::PatchSwapchainCreateInfo(info, frame_number_only).eligibility.ok);

    // Not even the reasons the overlay couldn't be drawn are worth a message then
    info.presentMode = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;
    EXPECT_EQ(monitor::PatchSwapchainCreateInfo(info, hidden).eligibility.reason, nullptr);
}

TEST(MonitorOverlay, PatchLeavesSwapchainsTheOverlayCantDrawOnAlone) {
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    info.presentMode = VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR;

    const monitor::SwapchainPatch patch = monitor::PatchSwapchainCreateInfo(info, monitor::LineFlags{});
    EXPECT_FALSE(patch.eligibility.ok);
    EXPECT_EQ(patch.create_info.imageUsage, VK_IMAGE_USAGE_TRANSFER_DST_BIT);
}

TEST(MonitorOverlay, EligibleForRegularSwapchains) {
    const auto eligibility = monitor::SwapchainOverlayEligibility(SwapchainInfo());
    EXPECT_TRUE(eligibility.ok);
    EXPECT_FALSE(eligibility.deferred);
}

TEST(MonitorOverlay, NotEligibleForSharedPresentModes) {
    for (const VkPresentModeKHR mode : {VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR, VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR}) {
        VkSwapchainCreateInfoKHR info = SwapchainInfo();
        info.presentMode = mode;
        const auto eligibility = monitor::SwapchainOverlayEligibility(info);
        EXPECT_FALSE(eligibility.ok);
        EXPECT_NE(eligibility.reason, nullptr);
    }
}

TEST(MonitorOverlay, NotEligibleForProtectedSwapchains) {
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.flags = VK_SWAPCHAIN_CREATE_PROTECTED_BIT_KHR;
    const auto eligibility = monitor::SwapchainOverlayEligibility(info);
    EXPECT_FALSE(eligibility.ok);
    EXPECT_NE(eligibility.reason, nullptr);
}

TEST(MonitorOverlay, EligibleWithDeferredMemoryAllocation) {
    VkSwapchainCreateInfoKHR info = SwapchainInfo();
    info.flags = VK_SWAPCHAIN_CREATE_DEFERRED_MEMORY_ALLOCATION_BIT_KHR;
    const auto eligibility = monitor::SwapchainOverlayEligibility(info);
    EXPECT_TRUE(eligibility.ok);
    EXPECT_TRUE(eligibility.deferred);
}

TEST(MonitorOverlay, DrawsOnlyOnGraphicsQueuesWithSomethingToShow) {
    EXPECT_TRUE(monitor::ShouldDrawOverlay(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, true, true));
    EXPECT_FALSE(monitor::ShouldDrawOverlay(VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT, true, true));
    EXPECT_FALSE(monitor::ShouldDrawOverlay(VK_QUEUE_GRAPHICS_BIT, false, true));
    EXPECT_FALSE(monitor::ShouldDrawOverlay(VK_QUEUE_GRAPHICS_BIT, true, false));
}

TEST(MonitorOverlay, SettingsDefaults) {
    const monitor::MonitorSettings settings = monitor::ParseSettings(monitor::RawSettings{});
    EXPECT_EQ(settings.position, monitor::Position::TopLeft);
    EXPECT_EQ(settings.scale_percent, 100);
    EXPECT_EQ(settings.update_interval, 500ms);
    EXPECT_TRUE(settings.lines.fps);
    EXPECT_TRUE(settings.lines.frame_time);
    EXPECT_TRUE(settings.lines.frame_number);
}

TEST(MonitorOverlay, SettingsParsePositions) {
    const std::pair<const char*, monitor::Position> cases[] = {
        {"TOP_LEFT", monitor::Position::TopLeft},       {"TOP_RIGHT", monitor::Position::TopRight},
        {"BOTTOM_LEFT", monitor::Position::BottomLeft}, {"BOTTOM_RIGHT", monitor::Position::BottomRight},
        {"MIDDLE", monitor::Position::TopLeft},         {"", monitor::Position::TopLeft},
    };
    for (const auto& test_case : cases) {
        monitor::RawSettings raw;
        raw.position = test_case.first;
        EXPECT_EQ(monitor::ParseSettings(raw).position, test_case.second) << test_case.first;
    }
}

TEST(MonitorOverlay, SettingsClampNumbers) {
    monitor::RawSettings raw;
    raw.scale = 10;
    raw.update_interval_ms = 1;
    monitor::MonitorSettings settings = monitor::ParseSettings(raw);
    EXPECT_EQ(settings.scale_percent, 50);
    EXPECT_EQ(settings.update_interval, 50ms);

    raw.scale = 1000;
    raw.update_interval_ms = -5;
    settings = monitor::ParseSettings(raw);
    EXPECT_EQ(settings.scale_percent, 400);
    EXPECT_EQ(settings.update_interval, 50ms);

    raw.scale = 150;
    raw.update_interval_ms = 2000;
    settings = monitor::ParseSettings(raw);
    EXPECT_EQ(settings.scale_percent, 150);
    EXPECT_EQ(settings.update_interval, 2000ms);
}

TEST(MonitorOverlay, SettingsLogWarningsOnlyWhenAsked) {
    monitor::RawSettings raw;
    EXPECT_FALSE(monitor::ParseSettings(raw).log_warnings);
    raw.log_warnings = true;
    EXPECT_TRUE(monitor::ParseSettings(raw).log_warnings);
}

TEST(MonitorOverlay, SettingsHideLines) {
    monitor::RawSettings raw;
    raw.show_fps = false;
    raw.show_frame_number = false;
    const monitor::MonitorSettings settings = monitor::ParseSettings(raw);
    EXPECT_FALSE(settings.lines.fps);
    EXPECT_TRUE(settings.lines.frame_time);
    EXPECT_FALSE(settings.lines.frame_number);
}

// The fragment shader has its own copy of the font, as "const uint kFont[...] = uint[](...)" with one row per value.
TEST(MonitorOverlay, ShaderFontMatchesFont) {
    std::ifstream file(MONITOR_SHADER_DIR "/monitor_overlay.frag");
    ASSERT_TRUE(file.is_open());
    const std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    const size_t begin = source.find("uint[](");
    ASSERT_NE(begin, std::string::npos);
    const size_t end = source.find(");", begin);
    ASSERT_NE(end, std::string::npos);

    std::vector<uint32_t> rows;
    const std::string values = source.substr(begin + 7, end - begin - 7);
    for (size_t i = 0; i < values.size();) {
        if (std::isdigit(static_cast<unsigned char>(values[i])) != 0) {
            size_t length = 0;
            rows.push_back(static_cast<uint32_t>(std::stoul(values.substr(i), &length, 0)));
            i += length;
        } else if (values.compare(i, 2, "//") == 0) {
            i = values.find('\n', i);
        } else {
            ++i;
        }
    }

    ASSERT_EQ(rows.size(), static_cast<size_t>(monitor::kGlyphCount * monitor::kGlyphHeight));
    for (int glyph = 0; glyph < monitor::kGlyphCount; ++glyph) {
        for (int row = 0; row < monitor::kGlyphHeight; ++row) {
            EXPECT_EQ(rows[static_cast<size_t>((glyph * monitor::kGlyphHeight) + row)],
                      monitor::GlyphRow(static_cast<uint8_t>(glyph), row))
                << "glyph " << glyph << " row " << row;
        }
    }
}

// The fragment shader repeats the panel metrics as "const int kName = value;".
TEST(MonitorOverlay, ShaderMetricsMatchLayout) {
    std::ifstream file(MONITOR_SHADER_DIR "/monitor_overlay.frag");
    ASSERT_TRUE(file.is_open());
    const std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    const auto shader_constant = [&](const std::string& name) -> int {
        const std::string declaration = "const int " + name + " = ";
        const size_t pos = source.find(declaration);
        if (pos == std::string::npos) {
            ADD_FAILURE() << name << " not found in the shader";
            return -1;
        }
        return std::stoi(source.substr(pos + declaration.size()));
    };

    EXPECT_EQ(shader_constant("kGlyphWidth"), monitor::kGlyphWidth);
    EXPECT_EQ(shader_constant("kGlyphHeight"), monitor::kGlyphHeight);
    EXPECT_EQ(shader_constant("kGlyphCount"), monitor::kGlyphCount);
    EXPECT_EQ(shader_constant("kCharAdvance"), monitor::kCharAdvance);
    EXPECT_EQ(shader_constant("kLineAdvance"), monitor::kLineAdvance);
    EXPECT_EQ(shader_constant("kPadding"), monitor::kPadding);
    EXPECT_EQ(shader_constant("kMaxColumns"), static_cast<int>(monitor::kMaxColumns));
}

// FNV-1a over the shader sources with carriage returns dropped, the same as scripts/generate_monitor_shaders.py.
// The files are hashed in the order of SHADERS in that script. Returns false if a source can't be read.
static bool HashShaderSources(uint64_t& hash) {
    hash = 0xcbf29ce484222325ull;
    for (const char* name : {"/monitor_overlay.vert", "/monitor_overlay.frag"}) {
        std::ifstream file(std::string(MONITOR_SHADER_DIR) + name, std::ios::binary);
        if (!file.is_open()) {
            ADD_FAILURE() << "can't read " << name;
            return false;
        }
        for (std::istreambuf_iterator<char> it(file), end; it != end; ++it) {
            if (*it != '\r') {
                hash = (hash ^ static_cast<uint8_t>(*it)) * 0x100000001b3ull;
            }
        }
    }
    return true;
}

// The SPIR-V is checked in; a shader edited without running the generator leaves it stale.
TEST(MonitorOverlay, ShaderSpirvMatchesTheShaderSources) {
    uint64_t hash = 0;
    ASSERT_TRUE(HashShaderSources(hash));
    EXPECT_EQ(hash, kMonitorOverlayShaderSourceHash) << "Run scripts/generate_monitor_shaders.py after changing layersvt/shaders";
}

// The device can use what both the app's VkApplicationInfo::apiVersion and the physical device support.
TEST(MonitorOverlay, ImageViewUsageNeedsVulkan11OrMaintenance2) {
    EXPECT_FALSE(monitor::ImageViewUsageAvailable(VK_API_VERSION_1_0, VK_API_VERSION_1_3, false));
    EXPECT_TRUE(monitor::ImageViewUsageAvailable(VK_API_VERSION_1_0, VK_API_VERSION_1_3, true));
    EXPECT_FALSE(monitor::ImageViewUsageAvailable(VK_API_VERSION_1_3, VK_API_VERSION_1_0, false));
    EXPECT_TRUE(monitor::ImageViewUsageAvailable(VK_API_VERSION_1_1, VK_API_VERSION_1_2, false));
    EXPECT_TRUE(monitor::ImageViewUsageAvailable(VK_MAKE_API_VERSION(0, 1, 1, 0), VK_MAKE_API_VERSION(0, 1, 4, 321), false));
}

// Only a line that wouldn't fit loses its decimals
TEST(MonitorOverlay, LongFrameTimesFitOnTheLine) {
    const auto lines = monitor::FormatOverlayLines(Snapshot(0.01, 100000.0, 100000.0, 100000.0), 0, monitor::LineFlags{});
    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "100000 ms (100000-100000)");

    const auto fits = monitor::FormatOverlayLines(Snapshot(0.1, 10000.0, 10000.0, 10000.0), 0, monitor::LineFlags{});
    EXPECT_EQ(fits[1], "10000.00 ms (10000.00-10000.00)");
    EXPECT_LE(fits[1].size(), monitor::kMaxColumns);
}

TEST(MonitorOverlay, TextUsesOnlyDrawableCharacters) {
    const auto lines = monitor::FormatOverlayLines(Snapshot(1234.5, 0.81, 0.5, 12345.67), UINT64_MAX, monitor::LineFlags{});
    for (const std::string& line : lines) {
        for (const char character : line) {
            EXPECT_TRUE(character == ' ' || monitor::GlyphIndex(character) != monitor::GlyphIndex(' '))
                << "'" << character << "' in " << line;
        }
    }
}

// Settings as an application would pass them with VK_EXT_layer_settings
static monitor::RawSettings ReadSettings(const std::vector<VkLayerSettingEXT>& layer_settings) {
    VkLayerSettingsCreateInfoEXT settings_info = {VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT};
    settings_info.settingCount = static_cast<uint32_t>(layer_settings.size());
    settings_info.pSettings = layer_settings.data();
    VkuLayerSettingSet settings = VK_NULL_HANDLE;
    vkuCreateLayerSettingSet("VK_LAYER_LUNARG_monitor", &settings_info, nullptr, nullptr, &settings);
    const monitor::RawSettings raw = monitor::ReadRawSettings(settings);
    vkuDestroyLayerSettingSet(settings, nullptr);
    return raw;
}

TEST(MonitorOverlay, SettingsDefaultWithoutASettingSet) {
    // What the layer gets if the layer settings library couldn't create a setting set
    const monitor::RawSettings raw = monitor::ReadRawSettings(VK_NULL_HANDLE);
    const monitor::RawSettings defaults;
    EXPECT_EQ(raw.position, defaults.position);
    EXPECT_EQ(raw.scale, defaults.scale);
    EXPECT_EQ(raw.update_interval_ms, defaults.update_interval_ms);
    EXPECT_EQ(raw.show_fps, defaults.show_fps);
    EXPECT_EQ(raw.show_frame_time, defaults.show_frame_time);
    EXPECT_EQ(raw.show_frame_number, defaults.show_frame_number);
    EXPECT_FALSE(raw.log_warnings);  // Nothing is printed unless the user asks
}

TEST(MonitorOverlay, SettingsAreReadFromTheApplication) {
    constexpr const char* position = "BOTTOM_RIGHT";
    constexpr int32_t scale = 200;
    constexpr int32_t interval = 1000;
    constexpr VkBool32 off = VK_FALSE;
    constexpr VkBool32 enabled = VK_TRUE;
    const std::vector<VkLayerSettingEXT> layer_settings = {
        {"VK_LAYER_LUNARG_monitor", "position", VK_LAYER_SETTING_TYPE_STRING_EXT, 1, static_cast<const void*>(&position)},
        {"VK_LAYER_LUNARG_monitor", "scale", VK_LAYER_SETTING_TYPE_INT32_EXT, 1, &scale},
        {"VK_LAYER_LUNARG_monitor", "update_interval_ms", VK_LAYER_SETTING_TYPE_INT32_EXT, 1, &interval},
        {"VK_LAYER_LUNARG_monitor", "show_fps", VK_LAYER_SETTING_TYPE_BOOL32_EXT, 1, &off},
        {"VK_LAYER_LUNARG_monitor", "show_frame_time", VK_LAYER_SETTING_TYPE_BOOL32_EXT, 1, &off},
        {"VK_LAYER_LUNARG_monitor", "show_frame_number", VK_LAYER_SETTING_TYPE_BOOL32_EXT, 1, &off},
        {"VK_LAYER_LUNARG_monitor", "log_warnings", VK_LAYER_SETTING_TYPE_BOOL32_EXT, 1, &enabled},
    };
    const monitor::RawSettings raw = ReadSettings(layer_settings);
    EXPECT_EQ(raw.position, "BOTTOM_RIGHT");
    EXPECT_EQ(raw.scale, 200);
    EXPECT_EQ(raw.update_interval_ms, 1000);
    EXPECT_FALSE(raw.show_fps);
    EXPECT_FALSE(raw.show_frame_time);
    EXPECT_FALSE(raw.show_frame_number);
    EXPECT_TRUE(raw.log_warnings);
}
