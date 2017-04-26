// This file contains fixed code which is included by a generated file:  vkreplay_vk_objmapper.h
// Warning: Do not include directly

class objMemory {
   public:
    objMemory() : m_numAllocations(0), m_pMemReqs(NULL) {}
    ~objMemory() { free(m_pMemReqs); }
    void setCount(const uint32_t num) { m_numAllocations = num; }

    void setReqs(const VkMemoryRequirements *pReqs, const uint32_t num) {
        if (m_numAllocations != num && m_numAllocations != 0) {
            vktrace_LogError("objMemory::setReqs, internal mismatch on number of allocations.");
        }
        if (m_pMemReqs == NULL && pReqs != NULL) {
            m_pMemReqs = (VkMemoryRequirements *)vktrace_malloc(num * sizeof(VkMemoryRequirements));
            if (m_pMemReqs == NULL) {
                vktrace_LogError("objMemory::setReqs out of memory.");
                return;
            }
            memcpy(m_pMemReqs, pReqs, num * sizeof(VkMemoryRequirements));
        }
    }

   private:
    uint32_t m_numAllocations;
    VkMemoryRequirements *m_pMemReqs;
};

class gpuMemory {
   public:
    gpuMemory() : m_pendingAlloc(false) { m_allocInfo.allocationSize = 0; }
    ~gpuMemory() {}
    // memory mapping functions for app writes into mapped memory
    bool isPendingAlloc() { return m_pendingAlloc; }

    void setAllocInfo(const VkMemoryAllocateInfo *info, const bool pending) {
        m_pendingAlloc = pending;
        m_allocInfo = *info;
    }

    void setMemoryDataAddr(void *pBuf) {
        if (m_mapRange.empty()) {
            return;
        }
        MapRange mr = m_mapRange.back();
        if (mr.pData != NULL)
            vktrace_LogWarning("gpuMemory::setMemoryDataAddr() data already mapped overwrite old mapping.");
        else if (pBuf == NULL)
            vktrace_LogWarning("gpuMemory::setMemoryDataAddr() adding NULL pointer.");
        mr.pData = (uint8_t *)pBuf;
    }

    void copyMappingDataPageGuard(const void *pSrcData) {
        if (m_mapRange.empty()) {
            return;
        }
        MapRange mr = m_mapRange.back();
        if (!pSrcData || !mr.pData) {
            if (!pSrcData)
                vktrace_LogError("gpuMemory::copyMappingData() null src pointer.");
            else
                vktrace_LogError("gpuMemory::copyMappingData() null dest pointer totalSize=%u.", m_allocInfo.allocationSize);
            m_mapRange.pop_back();
            return;
        }

        PageGuardChangedBlockInfo *pChangedInfoArray = (PageGuardChangedBlockInfo *)pSrcData;
        if (pChangedInfoArray[0].length) {
            PBYTE pChangedData = (PBYTE)(pSrcData) + sizeof(PageGuardChangedBlockInfo) * (pChangedInfoArray[0].offset + 1);
            DWORD CurrentOffset = 0;
            for (DWORD i = 0; i < pChangedInfoArray[0].offset; i++) {
                if ((size_t)pChangedInfoArray[i + 1].length) {
                    memcpy(mr.pData + (size_t)pChangedInfoArray[i + 1].offset, pChangedData + CurrentOffset,
                           (size_t)pChangedInfoArray[i + 1].length);
                }
                CurrentOffset += pChangedInfoArray[i + 1].length;
            }
        }
    }

    void setMemoryMapRange(void *pBuf, const size_t size, const size_t offset, const bool pending) {
        MapRange mr;
        mr.pData = (uint8_t *)pBuf;
        if (size == 0)
            mr.size = (size_t)m_allocInfo.allocationSize - offset;
        else
            mr.size = size;
        mr.offset = offset;
        mr.pending = pending;
        m_mapRange.push_back(mr);
        assert((size_t)m_allocInfo.allocationSize >= (size + offset));
    }

    void copyMappingData(const void *pSrcData, bool entire_map, size_t size, size_t offset) {
        if (m_mapRange.empty()) {
            return;
        }
        MapRange mr = m_mapRange.back();
        if (!pSrcData || !mr.pData) {
            if (!pSrcData)
                vktrace_LogError("gpuMemory::copyMappingData() null src pointer.");
            else
                vktrace_LogError("gpuMemory::copyMappingData() null dest pointer totalSize=%u.", m_allocInfo.allocationSize);
            m_mapRange.pop_back();
            return;
        }
        if (entire_map) {
            size = mr.size;
            offset = 0;  // pointer to mapped buffer is from offset 0
        } else {
            assert(offset >= mr.offset);
            assert(size <= mr.size && (size + offset) <= (size_t)m_allocInfo.allocationSize);
        }
        memcpy(mr.pData + offset, pSrcData, size);
        if (!mr.pending && entire_map) m_mapRange.pop_back();
    }

    size_t getMemoryMapSize() { return (!m_mapRange.empty()) ? m_mapRange.back().size : 0; }

   private:
    bool m_pendingAlloc;
    struct MapRange {
        bool pending;
        size_t size;
        size_t offset;
        uint8_t *pData;
    };
    std::vector<MapRange> m_mapRange;
    VkMemoryAllocateInfo m_allocInfo;
};

typedef struct _imageObj {
    objMemory imageMem;
    VkImage replayImage;
} imageObj;

typedef struct _bufferObj {
    objMemory bufferMem;
    VkBuffer replayBuffer;
} bufferObj;

typedef struct _devicememoryObj {
    gpuMemory *pGpuMem;
    VkDeviceMemory replayDeviceMemory;
} devicememoryObj;

class vkReplayObjMapper {
   public:
    vkReplayObjMapper() {}
    ~vkReplayObjMapper() {}

    bool m_adjustForGPU;  // true if replay adjusts behavior based on GPU
    void init_objMemCount(const uint64_t handle, const VkDebugReportObjectTypeEXT objectType, const uint32_t &num) {
        switch (objectType) {
            case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT: {
                std::map<VkBuffer, bufferObj>::iterator it = m_buffers.find((VkBuffer)handle);
                if (it != m_buffers.end()) {
                    objMemory obj = it->second.bufferMem;
                    obj.setCount(num);
                    return;
                }
                break;
            }
            case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT: {
                std::map<VkImage, imageObj>::iterator it = m_images.find((VkImage)handle);
                if (it != m_images.end()) {
                    objMemory obj = it->second.imageMem;
                    obj.setCount(num);
                    return;
                }
                break;
            }
            default:
                break;
        }
        return;
    }

    void init_objMemReqs(const uint64_t handle, const VkDebugReportObjectTypeEXT objectType, const VkMemoryRequirements *pMemReqs,
                         const unsigned int num) {
        switch (objectType) {
            case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT: {
                std::map<VkBuffer, bufferObj>::iterator it = m_buffers.find((VkBuffer)handle);
                if (it != m_buffers.end()) {
                    objMemory obj = it->second.bufferMem;
                    obj.setReqs(pMemReqs, num);
                    return;
                }
                break;
            }
            case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT: {
                std::map<VkImage, imageObj>::iterator it = m_images.find((VkImage)handle);
                if (it != m_images.end()) {
                    objMemory obj = it->second.imageMem;
                    obj.setReqs(pMemReqs, num);
                    return;
                }
                break;
            }
            default:
                break;
        }
        return;
    }
