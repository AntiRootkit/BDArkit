#pragma once


#include <iostream>


#ifdef _UNICODE
#ifndef tstring
typedef std::wstring tstring;
#endif
#else
#ifndef tstring
typedef std::string tstring;
#endif
#endif