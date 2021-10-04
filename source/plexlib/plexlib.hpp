#pragma once

#include <iostream>
#include <string>
#include <vector>

int PlexMain(std::vector<std::string>& args,
             std::ostream& out,
             std::ostream& err);

void PrintUsage(std::ostream& out);
