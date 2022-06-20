// This program has been developed by students from the bachelor Computer Science at
// Utrecht University within the Software Project course.
// © Copyright Utrecht University (Department of Information and Computing Sciences)

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