#pragma once

#define ASSERT_ALMOST_EQ(f1, f2) ASSERT_LE(abs(f1-f2), 0.01) 