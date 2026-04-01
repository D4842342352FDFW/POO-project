#pragma once
#include <exception>
#include <stdexcept>
#include <string>

class Exception : public std::exception {

    std::string message;

    public:
    Exception() : message("VFS Error") {}
    Exception(const std::string& msg) : message(msg) {}
    Exception(const char* msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class NotFoundException : public Exception {
    
    public:
    NotFoundException(const std::string& name) 
    : Exception("Error: Element " + name + " not found") {}
};


