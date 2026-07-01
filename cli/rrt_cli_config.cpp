#include "rrt_cli_config.h"

#include <stdexcept>

#include <yaml-cpp/yaml.h>

namespace {

double asDouble(const YAML::Node& node, const std::string& fieldName, const std::string& path) {
    try {
        return node.as<double>();
    } catch (const YAML::Exception&) {
        throw std::runtime_error("field '" + fieldName + "' must be a number in " + path);
    }
}

std::pair<double, double> parsePoint(const YAML::Node& node, const std::string& fieldName,
                                      const std::string& path) {
    if (!node.IsSequence() || node.size() != 2) {
        throw std::runtime_error("field '" + fieldName +
                                  "' must be a 2-element list [x, y] in " + path);
    }
    return {asDouble(node[0], fieldName, path), asDouble(node[1], fieldName, path)};
}

bool pointInBounds(const std::pair<double, double>& point, const std::array<double, 4>& bounds) {
    return point.first >= bounds[0] && point.first <= bounds[2] && point.second >= bounds[1] &&
           point.second <= bounds[3];
}

bool pointInsideObstacle(const std::pair<double, double>& point,
                          const std::vector<std::tuple<double, double, double>>& obstacles) {
    for (const auto& [cx, cy, radius] : obstacles) {
        const double dx = point.first - cx;
        const double dy = point.second - cy;
        if (dx * dx + dy * dy < radius * radius) {
            return true;
        }
    }
    return false;
}

}  // namespace

RrtCliConfig parseRrtCliConfig(const std::string& path) {
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
    for (const std::string& field :
         {"start", "goal", "bounds", "goal_radius", "step_size", "max_iterations"}) {
        if (!root[field]) {
            throw std::runtime_error("missing required field '" + field + "' in " + path);
        }
    }

    RrtCliConfig config;

    if (!root["bounds"].IsSequence() || root["bounds"].size() != 4) {
        throw std::runtime_error(
            "field 'bounds' must be a 4-element list [xmin, ymin, xmax, ymax] in " + path);
    }
    for (int i = 0; i < 4; ++i) {
        config.bounds[i] = asDouble(root["bounds"][i], "bounds", path);
    }
    if (config.bounds[0] >= config.bounds[2] || config.bounds[1] >= config.bounds[3]) {
        throw std::runtime_error(
            "field 'bounds' must satisfy xmin < xmax and ymin < ymax in " + path);
    }

    config.start = parsePoint(root["start"], "start", path);
    config.goal = parsePoint(root["goal"], "goal", path);

    config.goalRadius = asDouble(root["goal_radius"], "goal_radius", path);
    if (config.goalRadius <= 0.0) {
        throw std::runtime_error("field 'goal_radius' must be > 0 in " + path);
    }

    config.stepSize = asDouble(root["step_size"], "step_size", path);
    if (config.stepSize <= 0.0) {
        throw std::runtime_error("field 'step_size' must be > 0 in " + path);
    }

    try {
        config.maxIterations = root["max_iterations"].as<int>();
    } catch (const YAML::Exception&) {
        throw std::runtime_error("field 'max_iterations' must be an integer in " + path);
    }
    if (config.maxIterations <= 0) {
        throw std::runtime_error("field 'max_iterations' must be > 0 in " + path);
    }

    if (root["obstacles"]) {
        if (!root["obstacles"].IsSequence()) {
            throw std::runtime_error("field 'obstacles' must be a list in " + path);
        }
        for (const auto& obstacleNode : root["obstacles"]) {
            if (!obstacleNode["center"] || !obstacleNode["radius"]) {
                throw std::runtime_error(
                    "each entry in 'obstacles' must have 'center' and 'radius' in " + path);
            }
            const auto [cx, cy] = parsePoint(obstacleNode["center"], "obstacles.center", path);
            const double radius = asDouble(obstacleNode["radius"], "obstacles.radius", path);
            if (radius <= 0.0) {
                throw std::runtime_error("'obstacles' radius must be > 0 in " + path);
            }
            config.obstacles.emplace_back(cx, cy, radius);
        }
    }

    if (!pointInBounds(config.start, config.bounds)) {
        throw std::runtime_error("'start' is outside of 'bounds' in " + path);
    }
    if (!pointInBounds(config.goal, config.bounds)) {
        throw std::runtime_error("'goal' is outside of 'bounds' in " + path);
    }
    if (pointInsideObstacle(config.start, config.obstacles)) {
        throw std::runtime_error("'start' is inside an obstacle in " + path);
    }
    if (pointInsideObstacle(config.goal, config.obstacles)) {
        throw std::runtime_error("'goal' is inside an obstacle in " + path);
    }

    if (root["seed"]) {
        try {
            config.seed = root["seed"].as<unsigned int>();
        } catch (const YAML::Exception&) {
            throw std::runtime_error("field 'seed' must be a non-negative integer in " + path);
        }
    }

    return config;
}
