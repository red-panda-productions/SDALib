#pragma once
#include "SDADataMock.h"
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