#pragma once
#ifndef __Mstring__
#define __Mstring__
#include <string>
#include <vector>

class Mstring :public std::string
{
public:
	Mstring() :std::string() {}
	Mstring(const Mstring &A) :std::string(A) {}
	Mstring(const char * S) :std::string(S) {}
	Mstring(const std::string & A) :std::string(A) {}
	std::vector<Mstring> split(const char c=' ');
	std::vector<Mstring> split(const char &c,const unsigned &n);
	unsigned save(const std::string &str);
};
std::string GetProgramDir();
#endif // !__Mstring__
