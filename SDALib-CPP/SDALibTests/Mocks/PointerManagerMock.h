#pragma once
#include "SDAData.hpp"
class PointerManagerMock
{
public:
    PointerManagerMock()
    {
        
    };

    SDAData* GetDataPointer()
    {
        return &Data;
    }

    tTrackSeg* GetSegmentPointer()
    {
        return Segments;
    }

    SDAData Data;

    tTrackSeg Segments[10];

};