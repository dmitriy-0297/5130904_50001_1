#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include <string>
#include "polygon.h"

void processArea(const std::vector<Polygon>& polygons, const std::string& arg);
void processMax(const std::vector<Polygon>& polygons, const std::string& arg);
void processMin(const std::vector<Polygon>& polygons, const std::string& arg);
void processCount(const std::vector<Polygon>& polygons, const std::string& arg);
void processRmecho(std::vector<Polygon>& polygons, const Polygon& target);
void processIntersections(const std::vector<Polygon>& polygons, const Polygon& target);

#endif

