#include <string>
#include "anole.h"
#include "utils.h"

int main(int argc, char * argv[])
{
    if (2 != argc)
    {
        printf("invalid argument\n");
        return -1;
    }

    slothjson::config_t config;
    if (!slothjson::load(argv[1], config))
    {
        printf("load config error\n");
        return -1;
    }

    anole::init_config(config);

    anole_t anole(config);
    anole.run();
    return 0;
}
