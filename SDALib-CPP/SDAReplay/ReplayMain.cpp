#include "ReplayDriver.h"

int main()
{
    std::string replayFileName("ReplayData/bbRecording20220330-123618.txt");

    ReplayDriver driver(replayFileName);
    driver.Run();

    return 0;
}