//
// Created by kwh44 on 10/1/19.
//

#ifndef MYCAT_PARSE_OPTIONS_H
#define MYCAT_PARSE_OPTIONS_H

#include <memory>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

std::pair<bool, std::shared_ptr<std::vector<std::string>>> parse_options(int argc, const char *argv[]) {
    auto filenames = std::make_shared<std::vector<std::string>>();
    bool special_mode_status = false;
    try {
        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Help screen")
                ("special_symbol_on,A", "Special symbols mode turned on")
                ("filename", po::value<std::vector<std::string>>()->
                        multitoken()->zero_tokens()->composing(), "filename");
        po::positional_options_description pos_desc;
        pos_desc.add("filename", -1);
        po::command_line_parser parser{argc, argv};
        parser.options(desc).positional(pos_desc).allow_unregistered();
        po::parsed_options parsed_options = parser.run();
        po::variables_map vm;
        po::store(parsed_options, vm);
        if (vm.count("help")) {
            std::cout << desc << '\n';
            return std::make_pair(special_mode_status, filenames);
        }
        if (vm.count("filename")) {
            std::for_each(vm["filename"].as<std::vector<std::string>>().begin(),
                          vm["filename"].as<std::vector<std::string>>().end(),
                          [&](auto &v) { filenames->emplace_back(v); });
        }
        if (vm.count("special_symbol_on")) {
            special_mode_status = true;
        }
    }
    catch (const po::error &ex) {
        std::cerr << ex.what() << '\n';
    }
    return std::make_pair(special_mode_status, filenames);
}

#endif //MYCAT_PARSE_OPTIONS_H
