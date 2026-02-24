#include "./program_arguments.hpp"
#include "app/rose_tinted.hpp"
#include <getargs/argument_parser.hpp>
#include <Nostalgia/filesystem/filesystem.hpp>
#include <Nostalgia/application/application.hpp>
#include <Nostalgia/settings/engine.hpp>

#ifdef _WIN32
#   include <windows.h>
#   include <winbase.h>
#   include <shellapi.h>
#endif // _WIN32

int DedicatedMain(int argc, char** argv)
{
    global_ArgumentParser->AddFlag(&Flags::Help);
    global_ArgumentParser->AddFlag(&Flags::Version);

    int parser_status{global_ArgumentParser->ParseArguments(argc, argv)};
    if(parser_status == ARG_STATUS_FAILED)
        { return 1; }

    std::string program_name{"RoseTinted"};

    if(Flags::Help.IsActive())
    {
        std::println("{}", GetHelpMessage(program_name.data()));
        return 0;
    }
    else if(Flags::Version.IsActive())
    {
        std::println("{}", GetVersionMessage(program_name.data()));
        return 0;
    }

    RoseTinted application{};
    return Application()->Main();
}

// System-specific main functions
#ifdef _WIN32
int main(int argc, char* argv[])
{
    int windows_return = DedicatedMain(argc, argv);
    return windows_return;
}
#else // end of _WIN32, start of LINUX
int main(int argc, char* argv[])
{
    int linux_return = DedicatedMain(argc, argv);
    return linux_return;
}
#endif // end of LINUX
