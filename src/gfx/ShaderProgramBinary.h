#pragma once
#include <string>
using std::string;

struct ShaderProgramBinary{
	int Size;
	GLint Format;
	char Version[128];
};