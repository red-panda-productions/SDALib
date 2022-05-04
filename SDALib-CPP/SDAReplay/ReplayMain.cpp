#include "ReplayDriver.h"
#include <boost/program_options.hpp>

using namespace boost::program_options;

int main(int p_argc, char **p_path)
{
    variables_map vm;                                // variables_map that keeps track of the amount of arguments it gets fed through command lines.
    options_description argOptions{"CMDArguments"};  // initialize the command-line options.

    // determines all arguments options (name), the value it should have (s) and a description of what it is (description)
    argOptions.add_options()("bbfile", value<std::string>(), "BlackBoxReplayFile");

    // tries to add all command line arguments, if an argument does not exists as an option in "argOptions": print out an error.
    try
    {
        store(parse_command_line(p_argc, p_path, argOptions), vm);
    }
    catch (const error &er)
    {
        std::cerr << "arguments are invalid, syntax: \"--bbfile <filepath>\"" << std::endl;
        return 0;
    }

    std::string replayFileName;  // initialize the string
    if (vm.count("bbfile"))
    {
        replayFileName = vm["bbfile"].as<std::string>();
    }

    if (replayFileName.empty())  // if you don't give the program a filename, print the following error.
    {
        std::cerr << "no replay file found, please use command \"--bbfile <filename>\"" << std::endl;
        return 0;
    }

    ReplayDriver driver(replayFileName);
    driver.Run();

    return 0;
}
