#include "utils.hpp"

void error(const char *format, ...)
{
	va_list list;
	va_start(list, format);
	vfprintf(stderr, format, list);
	va_end(list);
	fprintf(stderr, "\n");
	exit(1);
}

std::string readFile(std::string fileName)
{
	std::ifstream ifs(fileName, std::ifstream::in);
	if (!ifs)
		error("Failed to open file \"%s\": ", fileName.c_str());
	std::string strBuf, out = "";
	while (getline(ifs, strBuf)) {
		out += strBuf + '\n';
	}
	ifs.close();
	return out;
}

