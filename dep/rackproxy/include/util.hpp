// Common utilities

#include <string>
#include <vector>

/** Surrounds raw text with quotes
Example:
	#define NAME "world"
	printf("Hello " TOSTRING(NAME))
expands to
	printf("Hello " "world")
and of course the C++ lexer/parser then concatenates the string literals.
*/
#define TOSTRING_LITERAL(x) #x
#define TOSTRING(x) TOSTRING_LITERAL(x)

bool systemIsFile(std::string path);
bool systemIsDirectory(std::string path);
std::vector<std::string> systemListEntries(std::string path);
