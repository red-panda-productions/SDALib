#include "IPCPointerManager.h"

/// @brief  Gets the data pointer from shared memory
/// @return The data pointer
SDAData* IPCPointerManager::GetDataPointer()
{
    return static_cast<SDAData*>(m_dataRegion.get_address());
}

/// @brief  Gets the segment pointer from shared memory
/// @return The segment pointer
tTrackSeg* IPCPointerManager::GetSegmentPointer()
{
    return static_cast<tTrackSeg*>(m_segmentRegion.get_address());
}

