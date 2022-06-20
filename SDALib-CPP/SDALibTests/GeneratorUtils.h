// This program has been developed by students from the bachelor Computer Science at
// Utrecht University within the Software Project course.
// © Copyright Utrecht University (Department of Information and Computing Sciences)

#pragma once
#include <gtest/gtest.h>
#include <vector>
#include "SDADriver.hpp"
#include "Utils.h"

struct TestSegments
{
    tTrackSeg* NextSegments;
    int NextSegmentsCount;
};

tTrackSeg GenerateSegment();

TestSegments GenerateSegments();

void DestroySegments(TestSegments& p_segments);

tCarElt GenerateCar(TestSegments& p_segments);

void DestroyCar(tCarElt& p_car);

tSituation GenerateSituation();

void DestroySituation(tSituation& p_situation);