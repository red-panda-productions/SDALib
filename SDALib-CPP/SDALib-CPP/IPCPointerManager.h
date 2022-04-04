#pragma once
#include "boost/interprocess/shared_memory_object.hpp"
#include "boost/interprocess/mapped_region.hpp"
#include "SDAData.hpp"

/// @brief Manages the shared memory pointers
class IPCPointerManager
{
public:
    IPCPointerManager()
    {
		m_dataRegion = boost::interprocess::mapped_region(m_currentDataObject, boost::interprocess::read_write);
		m_segmentRegion = boost::interprocess::mapped_region(m_currentSegmentDataObject, boost::interprocess::read_only);
    }

	SDAData* GetDataPointer();

	tTrackSeg* GetSegmentPointer();

private:
	boost::interprocess::shared_memory_object m_currentDataObject =
		boost::interprocess::shared_memory_object(boost::interprocess::open_only, "SDA_SHARED_MEMORY", boost::interprocess::read_write);
	boost::interprocess::shared_memory_object m_currentSegmentDataObject =
		boost::interprocess::shared_memory_object(boost::interprocess::open_only, "SDA_SHARED_SEGMENT_MEMORY", boost::interprocess::read_only);

	boost::interprocess::mapped_region m_dataRegion;
	boost::interprocess::mapped_region m_segmentRegion;

};