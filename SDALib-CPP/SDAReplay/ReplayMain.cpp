#include "ReplayDriver.h"

int main(int argc, char **path)
{
    std::string replayFileName("ReplayData/bbRecording20220330-123618.txt"));
    for (int i = 1; i < argc; i++)
    {
        if (path[i] == std::string("--bbfile"))
        {
            std::string test(path[++i]);  //"ReplayData/bbRecording20220330-123618.txt");
            replayFileName = test;
        }
    }

    ReplayDriver driver(replayFileName);
    driver.Run();

    return 0;
}
