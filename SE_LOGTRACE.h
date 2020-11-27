#ifndef __SE_LOGTRACE__
#define __SE_LOGTRACE__

#include <iostream>
#include <string>
#include <ctime>

/*****************************************************************************************/
class LogTrace {
public:
    virtual ~LogType() {}
    virtual void Trace(std::string) const = 0;
};

/*****************************************************************************************/
class ConcreteConsoleLog : public LogTrace {
public:
    void Trace(std::string toSend) const override {
        std::cout << std::string(ctime(&time(0))) + ": " + toSend << std::endl;
    }
};

/*****************************************************************************************/
class ConcreteFileLog : public LogTrace {
public:
    void Trace(std::string toSend) const override {
        //Open file and write
    }
};

/*****************************************************************************************/
class CreateLogTracer {
public:
    virtual ~CreateLogTracer(){};
    virtual LogTrace* FactoryMethod() const = 0;

    void SendLog() const {
        LogTrace* logtype = this->FactoryMethod();
        logtype->Trace("Hello world");
        delete logtype;
        return result;
    }
};

/*****************************************************************************************/
class GetConsoleLog : public CreateLogTracer {
public:
    LogTrace* FactoryMethod() const override {
        return new ConcreteConsoleLog();
    }
};

/*****************************************************************************************/
class GetConsoleLog : public CreateLogTracer {
public:
    LogTrace* FactoryMethod() const override {
        return new ConcreteFileLog();
    }
};

#endif