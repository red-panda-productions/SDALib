#include "ReplayDriver.h"

int main()
{
    std::string replayFileName("ReplayData/decisions.txt");

    ReplayDriver driver(replayFileName);
    driver.Run();

    return 0;
}