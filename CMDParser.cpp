
#include <string.h>

#include "CMDParser.h"

int ParseCMD(int argc, char** argv, flags_t* flags)
{
    if(argc <= 2)
    {
        return 0;
    }

    int opt = 0;
    int optid = 0;
    for(int i = 0; i < argc - 1; i++)
    {
        if((opt = getopt_long(argc, argv, "", options, &optid)) != -1)
        {
            switch(opt)
            {
                case 'h':
                {
                    flags->isHelp = true;
                    break;
                }
                case 'd':
                {
                    flags->isDimensions = true;
                    flags->ScreenX = (int)atoi(optarg);
                    flags->ScreenY = (int)atoi(optarg);
                    optid++;
                    break;
                }
                case 'n':
                {
                    flags->isProbeNumber = true;
                    flags->ProbeNumber = (int)atoi(optarg);

                    break;
                }
                case 't':
                {
                    flags->isTest = true;
                    flags->TestNumber = (int)atoi(optarg);

                    break;
                }
                case 'g':
                {
                    flags->isGraphic = true;

                    break;
                }
                case 's':
                {
                    flags->isStep = true;
                    flags->step = (float)atof(optarg);
                }
                case 'f':
                {
                    flags->CPUFrequency = (unsigned long long)atoll(optarg);
                }
                case '?':
                {
                    fprintf(stderr, "%s", UNKNOWN_COMMAND_MESSAGE);
                    break;
                }
                case ':':
                {
                    fprintf(stderr, "%s", MISSING_ARGUMENT);
                    break;
                }
                default:
                {
                    fprintf(stderr, "%s", PARSER_FAILURE);
                    return -1;
                }
            }
        }
    }
    return 0;
}
