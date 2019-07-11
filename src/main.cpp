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

    if (0 != zlog_init(config.logger.conf.c_str()))
    {
        printf("zlog_init fail\n");
        return -1;
    }
    anole::init_logger(config.logger.default_category.c_str());

    anole::set_mdc("anole");
    zlog_debug(anole::cat(), "init anole");

    anole_t anole(config);

    boost::asio::signal_set sig(anole.service());
    sig.add(SIGINT);
    sig.add(SIGTERM);
    sig.async_wait([&](const boost::system::error_code err, int signum){
        if (err)
        {
            return;
        }
        switch (signum)
        {
            case SIGINT:
            case SIGTERM:
                anole.stop();
                zlog_fini();
                break;
        }
    });

    zlog_debug(anole::cat(), "start anole");
    anole.run();
    return 0;
}
