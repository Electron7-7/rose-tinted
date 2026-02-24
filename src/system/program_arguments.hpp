#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "version.hpp"
#include <format>
#include <getargs/argument.hpp>

namespace Flags
{
    inline Flag Help     {"--help", "-h"};
    inline Flag Version  {"--version", "-v"};
}

namespace Options
{}

constexpr const char* _Help_Format_String{
R"(Usage: {} [-h|--help] [-v|--version]
    Options:
        -h, --help      print help document
        -v, --version   print program version
)"};

constexpr const char* _Version_Format_String{"{} v{}"};

inline std::string GetHelpMessage(const char* inProgramName)
{ return std::format(_Help_Format_String, inProgramName); }

inline std::string GetVersionMessage(const char* inProgramName)
{
    return std::format(_Version_Format_String,
        inProgramName,
        ROSE_TINTED_VERSION_FULL);
}

#endif // ARGUMENTS_H
