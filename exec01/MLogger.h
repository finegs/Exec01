#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <sys/timeb.h>
#include <condition_variable>

#ifdef __cplusplus
extern "C" {
#endif
    extern char* timestamp(char* str);
    //char* timestamp(char* str);
    extern char* t2s(char* str, timeb tm);

#ifdef __cplusplus 
    }
#endif

enum MLogLevel {
	Error
	, Warning
	, Info
	, Debug
	, Debug1
	, Debug2
	, Debug3
	, Debug4
};

class MLog;
class MLogMsg {
	friend class MLog;
private:
	timeb tm;
	char* msg;
public:
	MLogMsg(const char* _msg)
	{
		ftime(&tm);
		msg = new char[sizeof(_msg)];
		strcpy_s(msg, sizeof(_msg), _msg);
	}
	~MLogMsg();
	timeb getTime() const;
	char* getMsg() const;
	MLogMsg(const MLogMsg& other);
	MLogMsg& operator=(const MLogMsg& msg);
	MLogMsg(MLogMsg&& other);
	MLogMsg& operator=(MLogMsg&& other);
	//friend std::ostream& operator<<(std::ostream& os, const MLogMsg& logmsg);
    //friend MLog& operator<<(MLog& logger, const MLogMsg & logmsg);
};


class MLog {

	//friend class ostream;

public:
	MLog();
	virtual ~MLog();
	std::ostringstream& get(MLogLevel level = Info);
	MLogLevel& LogLevel() {
		return logLevel;
	}
	void flush();
	void clear();
	void init();
	void pause();
	void resume();
	void start();
	void shutdown();
	void handleLog();

	void log(const char* _msg);

	template<typename T>
	const MLog& operator<<(const T& obj) const;

protected:
	std::ostringstream os;

private:
	MLog(const MLog&);
	MLog& operator=(const MLog&);
	MLogLevel logLevel;

	std::atomic_bool isRun;
	std::atomic_bool isPause;
	std::condition_variable cv;
	std::priority_queue<MLogMsg*> queue;
	std::mutex mtx;
	std::thread* logWriter;
};