#include "fake_twitter/RestServer.h"

int main(int argc, char *argv[]) {
    Port port(8888);
    int thr = 1;
    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));
        if (argc == 3)
            thr = std::stoi(argv[2]);
    }
    Address addr(Ipv4::any(), port);

    auto opts = Http::Endpoint::options()
            .threads(thr)
            .flags(Tcp::Options::ReusePort | Tcp::Options::ReuseAddr);

//    cout << "Cores = " << hardware_concurrency() << endl;
//    cout << "Using " << thr << " threads" << endl;
    std::cout << "Start server" << std::endl;


//    auto config = std::make_shared<sqlpp::postgresql::connection_config>();
//    config->host = "127.0.0.1";
//    config->user = "twituser";
//    config->password = "123";
//    config->dbname = "twitdb";
//    config->debug = true;


    sql::connection_config config;
    config.host = "127.0.0.1";
    config.user = "twituser";
    config.password = "123";
    config.dbname = "twitdb";
    config.debug = true;

    RestServer stats(addr, config);
    stats.init(opts);
    stats.serve();
}
