#ifndef EXCEPTION
#define EXCEPTION

#include<string>

class Exception {
private:
  std::string msg;
public:
  Exception(const std::string & m = "") : msg(m) {

  }
};

class ExceptionIntersection : public Exception {
public:
  ExceptionIntersection(const std::string & m = "") : Exception(m) {

  }
};

class ExceptionOutside : public Exception {
public:
  ExceptionOutside(const std::string & m = "") : Exception(m) {

  }
};

#endif
