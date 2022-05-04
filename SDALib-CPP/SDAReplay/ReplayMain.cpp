#include "ReplayDriver.h"
#include <boost/program_options.hpp>

using namespace boost::program_options;

int main(int p_argc, char **p_path)
{
    variables_map vm;
    options_description desc{"CMDagruments"};
    desc.add_options()("bbfile", value<std::string>(), "BlackBoxReplayFile");

    //tries an command line argument, if does not exist store crashes, so it prints out a message.
    try
    {
        store(parse_command_line(p_argc, p_path, desc), vm);   
    }
    catch (const error &er)
    {
        std::cerr << "argument is invalid, try: \"--bbfile\" instead" << std::endl;
        return 0;
    }


    std::string replayFileName;  // initialize the string
    if (vm.count("bbfile"))
    {
        replayFileName = vm["bbfile"].as<std::string>();
    }

    if (replayFileName.empty())
    {
        std::cerr << "no replay file found, please use command \"--bbfile <filename>\"" << std::endl;
        return 0;
    }

    ReplayDriver driver(replayFileName);
    driver.Run();

    return 0;
}
