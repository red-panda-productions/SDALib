#pragma once
#include "SDAData.hpp"
class PointerManagerMock
{
public:
    PointerManagerMock() = default;

    SDAData* GetDataPointer()
    {
        return &Data;
    }

    tTrackSeg* GetSegmentPointer()
    {
        return Segments;
    }

    SDAData Data = {};

    tTrackSeg Segments[10] = {};
};