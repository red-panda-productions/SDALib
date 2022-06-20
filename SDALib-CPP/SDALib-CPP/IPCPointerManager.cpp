// This program has been developed by students from the bachelor Computer Science at
// Utrecht University within the Software Project course.
// © Copyright Utrecht University (Department of Information and Computing Sciences)

#include "IPCPointerManager.h"

IPCPointerManager::IPCPointerManager()
{
    m_dataRegion = boost::interprocess::mapped_region(m_currentDataObject, boost::interprocess::read_write);
    m_segmentRegion = boost::interprocess::mapped_region(m_currentSegmentDataObject, boost::interprocess::read_only);
}

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
