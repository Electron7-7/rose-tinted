#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "getargs/argument.hpp"

namespace Flags
{
    inline Flag Help    ("--help"    , "-h");
    inline Flag Version ("--version" , "-v");

    // Flags & Options don't need both long & short names
    inline Flag OnlyShortFlag("-f");
    inline Flag OnlyLongFlag("--only-long-flag");
}

namespace Options
{
    inline Option SomeOption("--some-option", "-o");
    inline Option SomeOptionThatRequiresAValue("--special-option", "-s", true);

    // Flags & Options don't need both long & short names
    inline Option OnlyShortOption("-s");
    inline Option OnlyLongOption("--only-long-option");
}

constexpr const char* _Help_Printout =
R"(    Usage: getargs [-h|--help] [-v|--version] [--some-option|-o [<some_argument>]] [--special-option|-s <required_argument>]
    Options:
        -h, --help                  print help document
        -v, --version               print program version
        -o, --some-option ARG       do something with "ARG" (but don't freak out if "ARG" wasn't passed)
        -s, --special-option ARG    do something with "ARG" (and error out if "ARG" wasn't passed)

    Example:
        getargs -v
        getargs --some-option --version
        getargs --special-option "required argument"
)";

constexpr const char* _Version_Printout = "getargs v1.1.1";

#endif // ARGUMENTS_H
