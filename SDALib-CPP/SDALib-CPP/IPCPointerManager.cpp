#include "IPCPointerManager.h"

SDAData* IPCPointerManager::GetDataPointer()
{
    return static_cast<SDAData*>(m_dataRegion.get_address());
}

tTrackSeg* IPCPointerManager::GetSegmentPointer()
{
    return static_cast<tTrackSeg*>(m_segmentRegion.get_address());
}

