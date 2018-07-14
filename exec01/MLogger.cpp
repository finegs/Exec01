#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sys/timeb.h>
#include <string>
#include <sstream>
#include <thread>
#include "MLogger.h"


static char* nowTime(char* str) {
	char t[24];
	struct tm timeinfo;
	struct timeb tm;

	ftime(&tm);
	localtime_s(&timeinfo, &tm.time);

	strftime(t, 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
	sprintf_s(str, 24, "%s.%03u", t, tm.millitm);
	return str;
}

static char* t2s(char* str, timeb tm) {
	char t[24];
	struct tm ti;

	localtime_s(&ti, &tm.time);

	strftime(t, 20, "%Y-%m-%d %H:%M:%S", &ti);
	sprintf_s(str, 24, "%s.%03u", t, tm.millitm);
	return str;
}

MLogMsg::MLogMsg(const MLogMsg& other) {
	if (this == &other) return;
	msg = new char[sizeof(other.msg)];
	strcpy(msg, other.msg);
}

MLogMsg& MLogMsg::operator=(const MLogMsg& other) {
	if (this == &other) return (*this);
	msg = new char[sizeof(other.msg)];
	strcpy(msg, other.msg);
	return (*this);
}

MLogMsg::MLogMsg(MLogMsg&& other) {
	if (this == &other) return;
	delete[] msg;
	tm = other.tm;
	msg = std::move(other.msg);
}

MLogMsg& MLogMsg::operator=(MLogMsg&& other) {
	if (this == &other) return (*this);
	delete[] msg;
	tm = other.tm;
	msg = std::move(other.msg);
	return (*this);
}

MLog::MLog() : logLevel(Info), isRun(true) {};

std::ostringstream& MLog::get(MLogLevel level) {
	char ts[24];
	os << nowTime(ts);
	os << " " << level << ": ";
	os << std::string(level > Debug ? 0 : level - Debug, '\t');
	logLevel = level;
	return os;
}

MLog::~MLog() {
	if (logLevel >= MLog::LogLevel()) {
		os << std::endl;
		fprintf(stdout, "%s", os.str().c_str());
		fflush(stdout);
	}
}


MLog& MLog::operator=(const MLog& logger) {
	if (this == &logger) return (*this);
	this->os.clear();
	this->os << logger.os.str();

	this->logLevel = logger.logLevel; 

	return (*this);
}

void MLog::flush() {
	log(os.str().c_str());
	os.clear();
}

void MLog::clear() {
	os.clear();
}

void MLog::init() {
	if (logWriter) return;
	logWriter = new std::thread(&MLog::handleLog, this);
}

void MLog::pause() {
	std::unique_lock<std::mutex> lck(mtx);
	isPause = true;
	cv.notify_all();
}

void MLog::resume() {
	std::unique_lock<std::mutex> lck(mtx);
	isPause = false;
	cv.notify_all();
}

void MLog::start() {
	isRun = true;
	init();
	resume();
}

void MLog::shutdown() {

}

void MLog::handleLog() {
	while (isRun) {
		std::unique_lock < std::mutex > lck(mtx);
		while (isPause || queue.empty()) {
			cv.wait(lck);
		}

		MLogMsg* msg = queue.top();
		queue.pop();

		std::cout << msg;
	
		std::cout.flush();

		delete msg;
	}
}

void MLog::log(const char* _msg) {
	MLogMsg* msg = new MLogMsg(_msg);

	std::unique_lock<std::mutex> lck(mtx);
	queue.push(msg);
	cv.notify_all();
}

MLogMsg::~MLogMsg()
{
	delete[] msg;
	msg = nullptr;
}

timeb MLogMsg::getTime() const
{
	return tm;
}

char * MLogMsg::getMsg() const
{
	return msg;
}

std::ostream& operator<<(std::ostream& os, const MLogMsg& logmsg) {
	char tsstr[24];
	os << t2s(tsstr, logmsg.getTime()) << " " << logmsg.msg; 
	return os;
}

template<class T>
MLog& MLog::operator<<(const T& obj) {
	os << obj;
	return *this;
}
