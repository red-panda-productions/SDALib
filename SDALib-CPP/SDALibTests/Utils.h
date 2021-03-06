/*
 * This program has been developed by students from the bachelor Computer Science at
 * Utrecht University within the Software Project course.
 * © Copyright Utrecht University (Department of Information and Computing Sciences)
 */

#pragma once

#include <future>
#include <csetjmp>  // is needed for ASSERT_DURATION_LE
#include <csignal>

/// @brief compares two values. If their difference is lower than the tolerance, return true.
#define ASSERT_ALMOST_EQ(f1, f2) ASSERT_LE(abs(f1 - f2), 0.01)

/// @brief Tests if 2 messages are equal to each other
/// @param msg1 The first message
/// @param msg2 The second message
/// @param size The size of the 2 messages
/// @return     Whether the test failed or succeeded
inline bool TestMessageEqual(const char* p_msg1, const char* p_msg2, int p_size)
{
    bool len1 = p_size == static_cast<int>(strlen(p_msg1));
    if (!len1) return false;
    int length = static_cast<int>(strlen(p_msg2));
    bool len2 = p_size == length;
    if (!len2) return false;

    for (int i = 0; i < p_size; i++)
    {
        if (p_msg1[i] != p_msg2[i]) return false;
    }
    return true;
}

/// @brief       Executes a statement and fails if it takes longer than the secs parameter
/// @param  secs the timeout parameter
/// @param  stmt the statement that needs to be executed
#define ASSERT_DURATION_LE(secs, stmt)                                                       \
    {                                                                                        \
        std::promise<bool> completed;                                                        \
        auto stmt_future = completed.get_future();                                           \
        std::thread([&](std::promise<bool>& completed) { \
    stmt; \
    completed.set_value(true); }, std::ref(completed)).detach();   \
        if (stmt_future.wait_for(std::chrono::seconds(secs)) == std::future_status::timeout) \
            GTEST_FATAL_FAILURE_("       timed out (> " #secs                                \
                                 " seconds). Check code for infinite loops");                \
    }

// test cases, how to use:
// TEST_CASE(TestSuite,ExampleTest,ExampleFunc,(1,"hi"));

/// @brief					Creates a parameterized test case
/// @param  test_suite_name The name of the test suite
/// @param  test_name		The name of this test
/// @param  methodName		The name of the method that needs to be called
/// @param  params			The parameters
#define TEST_CASE(test_suite_name, test_name, methodName, params) \
    TEST(test_suite_name, test_name)                              \
    {                                                             \
        methodName params;                                        \
    }

// combinatorial testing, how to use:
// BEGIN_TEST_COMBINATORIAL(suite,name)				   // start the test
// int ages[3] {42,69,22};							   // Define arrays or vectors
// const char* names[3] {"Sem","Debra","Elliot"};
// END_TEST_COMBINATORIAL(PrintAgeName,ages,3,names,3) // end the test with the given arrays

/// @brief					Begin a combinatorial test
/// @param  test_suite_name The name of the test suite
/// @param  test_name		The name of the combinatorial test
#define BEGIN_TEST_COMBINATORIAL(test_suite_name, test_name) \
    TEST(test_suite_name, test_name)                         \
    {
/// @brief				End a combinatorial test with 2 arrays
/// @param  method_name The method that needs to be called in the test
/// @param  arrname1	The array name of the first array
/// @param  count1		The amount of elements in the first array
/// @param  arrname2	The array name of the second array
/// @param  count2		The amount of elements in the second array
#define END_TEST_COMBINATORIAL2(method_name, arrname1, count1, arrname2, count2) \
    for (int a = 0; a < (count1); a++)                                           \
    {                                                                            \
        for (int b = 0; b < (count2); b++)                                       \
        {                                                                        \
            method_name((arrname1)[a], (arrname2)[b]);                           \
        }                                                                        \
    }                                                                            \
    }

/// @brief				End a combinatorial test with 3 arrays
/// @param  method_name The method that needs to be called in the test
/// @param  arrname1	The array name of the first array
/// @param  count1		The amount of elements in the first array
/// @param  arrname2	The array name of the second array
/// @param  count2		The amount of elements in the second array
/// @param  arrname3	The array name of the third array
/// @param  count3		The amount of elements in the third array
#define END_TEST_COMBINATORIAL3(method_name, arrname1, count1, arrname2, count2, arrname3, count3) \
    for (int a = 0; a < (count1); a++)                                                             \
    {                                                                                              \
        for (int b = 0; b < (count2); b++)                                                         \
        {                                                                                          \
            for (int c = 0; c < (count3); c++)                                                     \
            {                                                                                      \
                method_name((arrname1)[a], (arrname2)[b], (arrname3)[c]);                          \
            }                                                                                      \
        }                                                                                          \
    }                                                                                              \
    }

/// @brief				End a combinatorial test with 4 arrays
/// @param  method_name The method that needs to be called in the test
/// @param  arrname1	The array name of the first array
/// @param  count1		The amount of elements in the first array
/// @param  arrname2	The array name of the second array
/// @param  count2		The amount of elements in the second array
/// @param  arrname3	The array name of the third array
/// @param  count3		The amount of elements in the third array
/// @param  arrname4	The array name of the fourth array
/// @param  count4		The amount of elements in the fourth array
#define END_TEST_COMBINATORIAL4(method_name, arrname1, count1, arrname2, count2, arrname3, count3, arrname4, count4) \
    for (int a = 0; a < (count1); a++)                                                                               \
    {                                                                                                                \
        for (int b = 0; b < (count2); b++)                                                                           \
        {                                                                                                            \
            for (int c = 0; c < (count3); c++)                                                                       \
            {                                                                                                        \
                for (int d = 0; d < (count4); d++)                                                                   \
                {                                                                                                    \
                    method_name((arrname1)[a], (arrname2)[b], (arrname3)[c], (arrname4)[d]);                         \
                }                                                                                                    \
            }                                                                                                        \
        }                                                                                                            \
    }                                                                                                                \
    }

/// @brief				End a combinatorial test with 5 arrays
/// @param  method_name The method that needs to be called in the test
/// @param  arrname1	The array name of the first array
/// @param  count1		The amount of elements in the first array
/// @param  arrname2	The array name of the second array
/// @param  count2		The amount of elements in the second array
/// @param  arrname3	The array name of the third array
/// @param  count3		The amount of elements in the third array
/// @param  arrname4	The array name of the fourth array
/// @param  count4		The amount of elements in the fourth array
/// @param  arrname5	The array name of the fifth array
/// @param  count5		The amount of elements in the fifth array
#define END_TEST_COMBINATORIAL5(method_name, arrname1, count1, arrname2, count2, arrname3, count3, arrname4, count4, arrname5, count5) \
    for (int a = 0; a < (count1); a++)                                                                                                 \
    {                                                                                                                                  \
        for (int b = 0; b < (count2); b++)                                                                                             \
        {                                                                                                                              \
            for (int c = 0; c < (count3); c++)                                                                                         \
            {                                                                                                                          \
                for (int d = 0; d < (count4); d++)                                                                                     \
                {                                                                                                                      \
                    for (int e = 0; e < (count5); e++)                                                                                 \
                    {                                                                                                                  \
                        method_name((arrname1)[a], (arrname2)[b], (arrname3)[c], (arrname4)[d], (arrname5)[e]);                        \
                    }                                                                                                                  \
                }                                                                                                                      \
            }                                                                                                                          \
        }                                                                                                                              \
    }                                                                                                                                  \
    }
