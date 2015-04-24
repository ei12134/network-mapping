#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
using namespace std;

template<class DataTypeT>
class Exception {
private:
    string msg;
    DataTypeT value;
public:

    Exception(string str, DataTypeT value) : msg(str), value(value) {
    }

    string getError() const;

    string getMessage() const {
        return msg;
    }
    
    DataTypeT getValue() const {
        return value;
    }
};

#endif
