#include "ReplayDriver.h"

int main(int p_argc, char **p_path)
{
    std::string replayFileName;  // initialize the string
    for (int i = 1; i < p_argc; i++)
    {
        // picks the argument written after --bbfile
        if (strcmp(p_path[i], "--bbfile") == 0)
        {
            replayFileName = p_path[++i];
        }
    }

    ReplayDriver driver(replayFileName);
    driver.Run();

    return 0;
}
