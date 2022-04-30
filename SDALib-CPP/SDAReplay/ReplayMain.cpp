#include "ReplayDriver.h"
#include "portability.h"

int main()
{
    std::string replayFileName("ReplayData/decisions.txt");

    ReplayDriver driver(replayFileName);
    driver.Run();

    return 0;
}