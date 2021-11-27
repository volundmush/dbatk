//
// Created by volund on 11/26/21.
//

#include "sysdep.h"
#include "ringmud/core.h"
#include "argh.h"
#include <iostream>

int main(int argc, char* argv[])
{
    int pos = 1;
    argh::parser cmdl(argc, argv);
    std::string st;
    bool recovering = false;
    uint16_t port = 1280;

    if(cmdl[{"-h", "--help"}]) {
        std::cout << "Usage: %s [-c] [-m] [-x] [-q] [-r] [-s] [-d pathname] [port #]" << std::endl <<
                     "  -c             Copyover mode. For internal use only!" << std::endl <<
                     "  -t <port>      Port to run telnet listener on." << std::endl <<
                     "  -d <directory> Specify profile directory for world data, logs, saves, etc." << std::endl;
        exit(0);
    }

    cmdl({"-d", "--dir"}) >> st;
    if(!st.empty()) {
        ring::core::profile_path = fs::path(st);
        if(!fs::exists(ring::core::profile_path))
            throw std::runtime_error(fmt::format("Cannot access path: {}", ring::core::profile_path.string()));
        st.clear();
    }

    // This might be a copyover...
    recovering = cmdl["c"];

    cmdl("-t") >> st;
    if(!st.empty()) {
        port = std::strtoul(st.c_str(), nullptr, 10);
    }



    return (0);
}
