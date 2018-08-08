#ifndef _MY_OSTREAM
#define _MY_OSTREAM

#include <iostream>

#include "MLogger.h"

using namespace std;

static char* timestamp(char* str);

class LoggedStream {
public:
    LoggedStream(ostream& _out);
    template<typename T>
    const LoggedStream& operator<<(const T& v) const;
protected:
    ostream & out;
};

class MyLogger : LoggedStream {
public:
    void log();
};

#endif
