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

