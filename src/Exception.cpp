#include "Exception.h"
#include <sstream>
using namespace std;

template<class DataTypeT>
string Exception<DataTypeT>::getError() const {
    ostringstream outStr;
    outStr << msg << " :(value: " << value << ")";
    return outStr.str();
}
