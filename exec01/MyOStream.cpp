#include <iostream>

#include "MyOStream.h"

using namespace std;

LoggedStream::LoggedStream(ostream& _out) : out(_out) {}

template<typename T>
const LoggedStream& LoggedStream::operator<<(const T& v) const {
    log(); out << v; return *this;
}

void MyLogger::log() {
    char ts[24];
    out << timestamp(ts) << endl;
}

static char* timestamp(char* str) {
    char temp[24];
    struct tm timeinfo;
    struct timeb tm;

    ftime(&tm);
    localtime_s(&timeinfo, &tm.time);

    strftime(temp, 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
    sprintf_s(str, 24, "%s.%03u", temp, tm.millitm);
    return str;
}