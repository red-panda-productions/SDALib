#pragma once

#include <future>
#include <csetjmp> // is needed for ASSERT_DURATION_LE
#include <csignal>

/// @brief compares two values. If their difference is lower than the tolerance, return true.
#define ASSERT_ALMOST_EQ(f1, f2) ASSERT_LE(abs(f1-f2), 0.01)

/// @brief Tests if 2 messages are equal to each other
/// @param msg1 The first message 
/// @param msg2 The second message 
/// @param size The size of the 2 messages
/// @return     Whether the test failed or succeeded
inline bool TestMessageEqual(const char* msg1, const char* msg2, int size)
{
	bool len1 = size == strlen(msg1);
	if (!len1) return false;
	int length = strlen(msg2);
	bool len2 = size == length;
	if (!len2) return false;

	for (int i = 0; i < size; i++)
	{
		if (msg1[i] != msg2[i]) return false;
	}
	return true;
}

/// @brief       Executes a statement and fails if it takes longer than the secs parameter
/// @param  secs the timeout parameter
/// @param  stmt the statement that needs to be executed
#define ASSERT_DURATION_LE(secs, stmt) { \
  std::promise<bool> completed; \
  auto stmt_future = completed.get_future(); \
  std::thread([&](std::promise<bool>& completed) { \
    stmt; \
    completed.set_value(true); \
  }, std::ref(completed)).detach(); \
  if(stmt_future.wait_for(std::chrono::seconds(secs)) == std::future_status::timeout) \
    GTEST_FATAL_FAILURE_("       timed out (> " #secs \
    " seconds). Check code for infinite loops"); \
}