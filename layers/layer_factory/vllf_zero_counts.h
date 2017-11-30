#pragma once

#include "vector"
#include "string"
#include <algorithm>

class ZeroCounts : public layer_factory {
public:
    // Constructor for state_tracker
    ZeroCounts() : layer_factory(this) {};

    // Intercept CmdDraw calls and check instanceCount
    void PreCallCmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex,
                        uint32_t firstInstance){
        if (instanceCount == 0) {
            Warning("Warning: You are calling vkCmdDraw with an instanceCount of Zero.");
        }
    };

    // Intercept CmdDrawIndexed calls and check instanceCount
    void PreCallCmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                               int32_t vertexOffset, uint32_t firstInstance){
        if (instanceCount == 0) {
            Warning("Warning: You are calling vkCmdDrawIndexed with an instanceCount of Zero.");
        }
    };

    // Intercept CmdDrawIndirect calls and check drawCount
    void PreCallCmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount,
                                uint32_t stride) {
        if (drawCount == 0) {
            Warning("Warning: You are calling vkCmdDrawIndirect with an instanceCount of Zero.");
        }
    };

    // Intercept CmdDrawIndexedIndirect calls and check drawCount
    void CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
        uint32_t drawCount, uint32_t stride) {
        if (drawCount == 0) {
            Warning("Warning: You are calling vkCmdDrawIndexedIndirect with a drawCount of Zero.");
        }
    };
};

ZeroCounts warn_for_zero_counts;
