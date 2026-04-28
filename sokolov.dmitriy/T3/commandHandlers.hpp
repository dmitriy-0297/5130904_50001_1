#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include <sstream>
#include <fstream>
#include <vector>
#include "polygon.hpp"

void handleArea(std::istringstream &args, const std::vector<Polygon> &figures);

void handleMax(std::istringstream &args, const std::vector<Polygon> &figures);

void handleMin(std::istringstream &args, const std::vector<Polygon> &figures);

void handleCount(std::istringstream &args, const std::vector<Polygon> &figures);

void handleRMEcho(std::istringstream &args, std::vector<Polygon> &figures);

void handleInFrame(std::istringstream &args, std::vector<Polygon> &figures);

#endif
