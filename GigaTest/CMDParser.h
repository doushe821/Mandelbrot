
#ifndef CMD_PARSER_H_INCLUDED
#define CMD_PARSER_H_INCLUDED

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "RunParametersStruct.h"
#include "ErrorParser.h"

__attribute((unused)) static const int NUMBER_OF_COMMANDS = 7;

struct flags_t
{
    bool isHelp;

    bool isDimensions;
    int ScreenX;
    int ScreenY;

    bool isProbeNumber;
    int ProbeNumber;

    bool isGraphic;

    bool isTest;
    int TestNumber;
    unsigned long long CPUFrequency;

    bool isStep;
    float step;
};

static const option options[NUMBER_OF_COMMANDS]
{
    {"dim"   , required_argument, NULL, 'd'},
    {"help"  , no_argument      , NULL, 'h'},
    {"pn", required_argument, NULL, 'n'},
    {"graphics" , no_argument, NULL, 'g'},
    {"test" , required_argument, NULL, 't'},
    {"step", required_argument, NULL, 's'},
    {"frequency", required_argument, NULL, 'f'},

};

enum ErrorCodes InitParams(flags_t CMDFlags, RunParameters* params);
enum ErrorCodes FileNamesInit(RunParameters* params);
enum ErrorCodes ParseCMD(int argc, char** argv, flags_t* flags);

__attribute((unused)) static const char* HELP_MESSAGE = "This program calculates Mandelbrot set and is cool\n";
__attribute((unused)) static const char* UNKNOWN_COMMAND_MESSAGE = "Unknown option, it will be ignored\n";
__attribute((unused)) static const char* MISSING_ARGUMENT = "Option missing argument, it will be set to default or ignored\n";
__attribute((unused)) static const char* PARSER_FAILURE = "Command line options parser failed\n";
#endif
