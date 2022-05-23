#pragma once
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <string>

class Exception : public std::exception {
private:
    std::string error;
public:
    Exception(const char* error) {
        this->error = error;
    }
    const char* error_msg() {
        return error.c_str();
    }
};
#endif