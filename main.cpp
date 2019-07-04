#include <string>
#include <boost/asio/signal_set.hpp>
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

    boost::asio::signal_set sig(anole.service());
    sig.add(SIGINT);
    sig.add(SIGTERM);
    sig.async_wait([&](const boost::system::error_code error, int signum){
        if (error)
        {
            return;
        }
        switch (signum)
        {
            case SIGINT:
            case SIGTERM:
                anole.stop();
                break;
        }
    });

    anole.run();
    return 0;
}
