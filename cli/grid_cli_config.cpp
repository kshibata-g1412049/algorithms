#include "grid_cli_config.h"

#include <stdexcept>

#include <yaml-cpp/yaml.h>

namespace {

std::pair<int, int> parsePoint(const YAML::Node& node, const std::string& fieldName,
                                const std::string& path) {
    if (!node.IsSequence() || node.size() != 2) {
        throw std::runtime_error("field '" + fieldName +
                                  "' must be a 2-element list [row, col] in " + path);
    }
    try {
        return {node[0].as<int>(), node[1].as<int>()};
    } catch (const YAML::Exception&) {
        throw std::runtime_error("field '" + fieldName + "' must contain integers in " + path);
    }
}

}  // namespace

GridCliConfig parseGridCliConfig(const std::string& path) {
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
    for (const std::string& field : {"start", "goal", "grid"}) {
        if (!root[field]) {
            throw std::runtime_error("missing required field '" + field + "' in " + path);
        }
    }

    GridCliConfig config;

    if (!root["grid"].IsSequence() || root["grid"].size() == 0) {
        throw std::runtime_error("field 'grid' must be a non-empty list of rows in " + path);
    }
    std::size_t numCols = 0;
    for (std::size_t r = 0; r < root["grid"].size(); ++r) {
        const YAML::Node& rowNode = root["grid"][r];
        if (!rowNode.IsSequence()) {
            throw std::runtime_error("each row in 'grid' must be a list in " + path);
        }
        if (r == 0) {
            numCols = rowNode.size();
            if (numCols == 0) {
                throw std::runtime_error("'grid' rows must not be empty in " + path);
            }
        } else if (rowNode.size() != numCols) {
            throw std::runtime_error("'grid' must be rectangular (row " + std::to_string(r) +
                                      " has a different length) in " + path);
        }

        std::vector<int> row;
        for (const auto& cellNode : rowNode) {
            int cell = 0;
            try {
                cell = cellNode.as<int>();
            } catch (const YAML::Exception&) {
                throw std::runtime_error("'grid' cells must be integers in " + path);
            }
            if (cell != 0 && cell != 1) {
                throw std::runtime_error("'grid' cells must be 0 or 1 in " + path);
            }
            row.push_back(cell);
        }
        config.grid.push_back(row);
    }

    config.start = parsePoint(root["start"], "start", path);
    config.goal = parsePoint(root["goal"], "goal", path);

    const int numRows = static_cast<int>(config.grid.size());
    const int numColsInt = static_cast<int>(numCols);
    auto checkInBoundsAndFree = [&](const std::pair<int, int>& point,
                                     const std::string& fieldName) {
        const auto [row, col] = point;
        if (row < 0 || row >= numRows || col < 0 || col >= numColsInt) {
            throw std::runtime_error("'" + fieldName + "' (" + std::to_string(row) + ", " +
                                      std::to_string(col) + ") is out of grid bounds in " + path);
        }
        if (config.grid[row][col] != 0) {
            throw std::runtime_error("'" + fieldName + "' (" + std::to_string(row) + ", " +
                                      std::to_string(col) + ") is on an obstacle cell in " + path);
        }
    };
    checkInBoundsAndFree(config.start, "start");
    checkInBoundsAndFree(config.goal, "goal");

    if (root["weight"]) {
        try {
            config.weight = root["weight"].as<double>();
        } catch (const YAML::Exception&) {
            throw std::runtime_error("field 'weight' must be a number in " + path);
        }
        if (config.weight < 1.0) {
            throw std::runtime_error("field 'weight' must be >= 1.0 in " + path);
        }
    }

    return config;
}
