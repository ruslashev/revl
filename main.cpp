#include <fstream>
#include "lexer.hpp"

#include <cstdarg>
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

int main()
{
	std::string fileContents = readFile("test.revl");

	std::vector<token_t> tokens;
	tokens = lex(fileContents);
	for (auto &t : tokens) {
		switch (t.kind) {
			case TOKEN_WORD:
				printf("WORD: %s\n", t.word.c_str());
				break;
			case TOKEN_INTEGER:
				printf("INTEGER: %d\n", t.integer);
				break;
			case TOKEN_OPENING_BRACE:
				printf("OPENING_BRACE\n");
				break;
			case TOKEN_CLOSING_BRACE:
				printf("CLOSING_BRACE\n");
				break;
			case TOKEN_EQUALS:
				printf("EQUALS\n");
				break;
		}
	}

	return 0;
}

