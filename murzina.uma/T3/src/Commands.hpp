#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Geometry.hpp"
#include <vector>
#include <string>
#include <functional>
#include <map>

class CommandProcessor {
public:
    using PolygonVector = std::vector<geometry::Polygon>;
    
    explicit CommandProcessor(PolygonVector polygons);
    
    void processCommand(const std::string& commandLine);
    
private:
    PolygonVector polygons_;
    std::map<std::string, std::function<void(std::istringstream&)>> handlers_;
    
    void initHandlers();
    
    void handleArea(std::istringstream& args);
    void handleMax(std::istringstream& args);
    void handleMin(std::istringstream& args);
    void handleCount(std::istringstream& args);
    void handleRects(std::istringstream& args);
    void handleSame(std::istringstream& args);
    
    template<typename Predicate>
    double sumAreaIf(Predicate pred) const;
    
    template<typename Predicate>
    size_t countIf(Predicate pred) const;
};

#endif
