#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <fstream>
#include <cstdarg>

void error(const char *format, ...);
std::string ReadFile(std::string filename);

#endif

