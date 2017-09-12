#ifndef IOEXCEPTION_H
#define IOEXCEPTION_H

#include <stdexcept>
#include <string>

class IOException : public std::runtime_error {
public:
    IOException(const std::string& message);
    virtual ~IOException();
    
    const std::string& message() const;

private:
    std::string message_;
};

#endif /* IOEXCEPTION_H */

