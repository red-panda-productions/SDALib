project(SDATests)

set(INSTALL_GTEST OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(
  SDATests "SDADataTest.cpp"   "SDAActionTest.cpp" "Utils.h" "SDADriverTest.cpp")
target_link_libraries(
  SDATests
  SDALib
  gtest_main
)
target_include_directories(SDATests PUBLIC "../SDALib-CPP")

#add_custom_command(TARGET SDATests POST_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:SDATests> $<TARGET_FILE_DIR:SDATests>
#       COMMAND_EXPAND_LISTS
#)

add_custom_command(TARGET SDATests POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy "${IPCLIB_BIN_DIR}/IPCLib.dll" $<TARGET_FILE_DIR:SDATests>
       COMMAND_EXPAND_LISTS
)

include(GoogleTest)
gtest_discover_tests(SDATests)