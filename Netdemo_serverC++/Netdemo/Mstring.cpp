#include"Mstring.h"
#include<fstream>
#include<assert.h>
#include <windows.h>
std::vector<Mstring> Mstring::split(char c) {
    std::vector<Mstring> stringvector;
	Mstring tempstr;
	stringvector.clear();
	auto beg = this->begin(), end = this->end();
	for (; beg != end; beg++)
	{
		if (*beg !=c) {
			tempstr.push_back(*beg);
		}
		else {
			stringvector.push_back(tempstr);
			tempstr.clear();
		}
	}
	stringvector.push_back(tempstr);
	return stringvector;
};

std::vector<Mstring> Mstring::split(const char &c,const unsigned &n) {
	assert(n);
	std::vector<Mstring> stringvector;
	Mstring tempstr;
	unsigned i = 1;
	stringvector.clear();
	auto beg = this->begin(), end = this->end();
	for (; beg != end&&i!=n; beg++)
	{
		if (*beg != c) {
			tempstr.push_back(*beg);
		}
		else {
			stringvector.push_back(tempstr);
			tempstr.clear();
			++i;
		}
	}
	for (; beg != end; beg++)
	{
		tempstr.push_back(*beg);
	}
	stringvector.push_back(tempstr);
	return stringvector;
}
unsigned Mstring::save(const std::string &str) {
	std::ofstream File;
	File.open(str, std::ios::app);
	if (File.is_open()) {
		File << *this << std::endl;
		File.close();
		return 1;
	}
	else {
		File.close();
		return 0;
	}
}

std::string TCHAR2STRING(TCHAR* str)
{
	std::string strstr;
	try
	{
		int iLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen * sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, str, -1, chRtn, iLen, NULL, NULL);
		strstr = chRtn;
	}
	catch (std::exception e)
	{
	}
	return strstr;

}
std::string GetProgramDir()
{
	TCHAR exeFullPath[MAX_PATH]; // Full path
	std::string strPath = "";
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = TCHAR2STRING(exeFullPath);    // Get full path of the file
	int pos = strPath.find_last_of('\\', strPath.length());
	return strPath.substr(0, pos);  // Return the directory without the file name
}
