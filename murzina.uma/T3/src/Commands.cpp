#include "Commands.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>

using namespace std::placeholders;

CommandProcessor::CommandProcessor(PolygonVector polygons) 
    : polygons_(std::move(polygons)) {
    initHandlers();
}

void CommandProcessor::initHandlers() {
    handlers_["AREA"] = std::bind(&CommandProcessor::handleArea, this, _1);
    handlers_["MAX"] = std::bind(&CommandProcessor::handleMax, this, _1);
    handlers_["MIN"] = std::bind(&CommandProcessor::handleMin, this, _1);
    handlers_["COUNT"] = std::bind(&CommandProcessor::handleCount, this, _1);
    handlers_["RECTS"] = std::bind(&CommandProcessor::handleRects, this, _1);
    handlers_["SAME"] = std::bind(&CommandProcessor::handleSame, this, _1);
}

void CommandProcessor::processCommand(const std::string& commandLine) {
    std::istringstream iss(commandLine);
    std::string command;
    iss >> command;
    
    auto it = handlers_.find(command);
    if (it != handlers_.end()) {
        it->second(iss);
    } else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

template<typename Predicate>
double CommandProcessor::sumAreaIf(Predicate pred) const {
    if (polygons_.empty()) return 0.0;
    
    std::vector<double> areas(polygons_.size());
    std::transform(polygons_.cbegin(), polygons_.cend(), areas.begin(),
                   std::bind(&geometry::Polygon::area, _1));
    
    std::vector<double> filteredAreas;
    filteredAreas.reserve(polygons_.size());
    
    auto areaIt = areas.cbegin();
    auto polyIt = polygons_.cbegin();
    
    std::copy_if(areas.cbegin(), areas.cend(), std::back_inserter(filteredAreas),
                 [&pred, &polyIt, &areaIt](double) mutable {
                     bool result = pred(*polyIt);
                     ++polyIt;
                     ++areaIt;
                     return result;
                 });
    
    return std::accumulate(filteredAreas.cbegin(), filteredAreas.cend(), 0.0);
}

template<typename Predicate>
size_t CommandProcessor::countIf(Predicate pred) const {
    return std::count_if(polygons_.cbegin(), polygons_.cend(), pred);
}

void CommandProcessor::handleArea(std::istringstream& args) {
    std::string param;
    args >> param;
    
    if (polygons_.empty()) {
        std::cout << "0.0" << std::endl;
        return;
    }
    
    if (param == "ODD") {
        double sum = sumAreaIf(std::bind(std::equal_to<size_t>(),
                                         std::bind(std::modulus<size_t>(),
                                                  std::bind(&geometry::Polygon::vertexCount, _1),
                                                  2),
                                         1));
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    } 
    else if (param == "EVEN") {
        double sum = sumAreaIf(std::bind(std::equal_to<size_t>(),
                                         std::bind(std::modulus<size_t>(),
                                                  std::bind(&geometry::Polygon::vertexCount, _1),
                                                  2),
                                         0));
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    }
    else if (param == "MEAN") {
        if (polygons_.empty()) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        std::vector<double> areas(polygons_.size());
        std::transform(polygons_.cbegin(), polygons_.cend(), areas.begin(),
                       std::bind(&geometry::Polygon::area, _1));
        double sum = std::accumulate(areas.cbegin(), areas.cend(), 0.0);
        double mean = sum / polygons_.size();
        std::cout << std::fixed << std::setprecision(1) << mean << std::endl;
    }
    else {
        size_t vertexCount = std::stoul(param);
        double sum = sumAreaIf(std::bind(std::equal_to<size_t>(),
                                         std::bind(&geometry::Polygon::vertexCount, _1),
                                         vertexCount));
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    }
}

void CommandProcessor::handleMax(std::istringstream& args) {
    std::string param;
    args >> param;
    
    if (polygons_.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    
    if (param == "AREA") {
        auto maxIt = std::max_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<double>(),
                     std::bind(&geometry::Polygon::area, _1),
                     std::bind(&geometry::Polygon::area, _2)));
        std::cout << std::fixed << std::setprecision(1) << maxIt->area() << std::endl;
    }
    else if (param == "VERTEXES") {
        auto maxIt = std::max_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<size_t>(),
                     std::bind(&geometry::Polygon::vertexCount, _1),
                     std::bind(&geometry::Polygon::vertexCount, _2)));
        std::cout << maxIt->vertexCount() << std::endl;
    }
    else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void CommandProcessor::handleMin(std::istringstream& args) {
    std::string param;
    args >> param;
    
    if (polygons_.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    
    if (param == "AREA") {
        auto minIt = std::min_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<double>(),
                     std::bind(&geometry::Polygon::area, _1),
                     std::bind(&geometry::Polygon::area, _2)));
        std::cout << std::fixed << std::setprecision(1) << minIt->area() << std::endl;
    }
    else if (param == "VERTEXES") {
        auto minIt = std::min_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<size_t>(),
                     std::bind(&geometry::Polygon::vertexCount, _1),
                     std::bind(&geometry::Polygon::vertexCount, _2)));
        std::cout << minIt->vertexCount() << std::endl;
    }
    else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void CommandProcessor::handleCount(std::istringstream& args) {
    std::string param;
    args >> param;
    
    if (param == "ODD") {
        size_t count = countIf(std::bind(std::equal_to<size_t>(),
                                         std::bind(std::modulus<size_t>(),
                                                  std::bind(&geometry::Polygon::vertexCount, _1),
                                                  2),
                                         1));
        std::cout << count << std::endl;
    }
    else if (param == "EVEN") {
        size_t count = countIf(std::bind(std::equal_to<size_t>(),
                                         std::bind(std::modulus<size_t>(),
                                                  std::bind(&geometry::Polygon::vertexCount, _1),
                                                  2),
                                         0));
        std::cout << count << std::endl;
    }
    else {
        size_t vertexCount = std::stoul(param);
        size_t count = countIf(std::bind(std::equal_to<size_t>(),
                                         std::bind(&geometry::Polygon::vertexCount, _1),
                                         vertexCount));
        std::cout << count << std::endl;
    }
}

void CommandProcessor::handleRects(std::istringstream& args) {
    size_t count = countIf(std::bind(&geometry::isRectangle, _1));
    std::cout << count << std::endl;
}

void CommandProcessor::handleSame(std::istringstream& args) {
    geometry::Polygon target;
    if (!(args >> target)) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    
    size_t count = countIf(std::bind(&geometry::isSameShape, target, _1));
    std::cout << count << std::endl;
}
