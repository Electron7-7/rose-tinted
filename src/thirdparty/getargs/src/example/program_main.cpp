#include "getargs/argument_parser.hpp"
#include "program_arguments.hpp"

#include <cstdio>

// You could (and probably should) write a specialized function/class for handling arguments; I'm using the main function as an example.
int main(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentParser->AddFlag(&Flags::Help);
    global_ArgumentParser->AddFlag(&Flags::Version);

    // Add valid options
    global_ArgumentParser->AddOption(&Options::SomeOption);
    global_ArgumentParser->AddOption(&Options::SomeOptionThatRequiresAValue);

    // Parse all arguments
    int parser_status = global_ArgumentParser->ParseArguments(argc, argv);

    if(parser_status == ARG_STATUS_FAILED)
        return 1;

    if(Flags::Help.IsActive())
    {
        printf("%s\n    %s\n", _Help_Printout, _Version_Printout);
        return 0;
    }

    if(Flags::Version.IsActive())
    {
        printf("    %s\n", _Version_Printout);
        return 0;
    }

    if(Options::SomeOption.IsActive())
    {
        if(Options::SomeOption.HasValue())
            printf("You passed '%s' to '--some-option'\n", Options::SomeOption.GetValue());
        else
            printf("You didn't pass anything to '--some-option'\n");
    }

    if(Options::SomeOptionThatRequiresAValue.IsActive())
    {
        if(Options::SomeOptionThatRequiresAValue.HasValue())
            printf("You passed '%s' to '--special-option'\n", Options::SomeOptionThatRequiresAValue.GetValue());
        else
            printf("Uh... you really shouldn't be seeing this...\n");
    }

    return 0;
}
