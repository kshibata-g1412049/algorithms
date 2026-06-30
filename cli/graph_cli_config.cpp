#include "graph_cli_config.h"

#include <stdexcept>

#include <yaml-cpp/yaml.h>

GraphCliConfig parseGraphCliConfig(const std::string& path) {
    YAML::Node root;
    try {
        root = YAML::LoadFile(path);
    } catch (const YAML::BadFile&) {
        throw std::runtime_error("config file not found or not readable: " + path);
    } catch (const YAML::ParserException& e) {
        throw std::runtime_error("YAML parse error in " + path + ": " + e.what());
    }

    if (!root.IsMap()) {
        throw std::runtime_error("config file must contain a YAML mapping: " + path);
    }
    if (!root["start"]) {
        throw std::runtime_error("missing required field 'start' in " + path);
    }
    if (!root["adjacency"]) {
        throw std::runtime_error("missing required field 'adjacency' in " + path);
    }

    GraphCliConfig config;
    try {
        config.start = root["start"].as<int>();
    } catch (const YAML::Exception&) {
        throw std::runtime_error("field 'start' must be an integer in " + path);
    }

    if (!root["adjacency"].IsSequence()) {
        throw std::runtime_error("field 'adjacency' must be a list of lists in " + path);
    }
    for (const auto& row : root["adjacency"]) {
        if (!row.IsSequence()) {
            throw std::runtime_error("each entry in 'adjacency' must be a list in " + path);
        }
        std::vector<int> neighbors;
        for (const auto& value : row) {
            try {
                neighbors.push_back(value.as<int>());
            } catch (const YAML::Exception&) {
                throw std::runtime_error("'adjacency' entries must be integers in " + path);
            }
        }
        config.adjacency.push_back(neighbors);
    }

    const int numNodes = static_cast<int>(config.adjacency.size());
    if (numNodes == 0) {
        throw std::runtime_error("'adjacency' must not be empty in " + path);
    }
    if (config.start < 0 || config.start >= numNodes) {
        throw std::runtime_error("'start' (" + std::to_string(config.start) +
                                  ") is out of range [0, " + std::to_string(numNodes) +
                                  ") in " + path);
    }
    for (const auto& neighbors : config.adjacency) {
        for (int neighbor : neighbors) {
            if (neighbor < 0 || neighbor >= numNodes) {
                throw std::runtime_error(
                    "'adjacency' contains out-of-range node id " +
                    std::to_string(neighbor) + " in " + path);
            }
        }
    }

    return config;
}
