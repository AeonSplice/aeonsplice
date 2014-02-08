#ifndef _AEONUTIL_
#define _AEONUTIL_

#define EXIT_CRITICAL_FAILURE 9001

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif // EXIT_FAILURE

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#define AEON_INFO 3
#define AEON_WARNING 2
#define AEON_ERROR 1
#define AEON_FATAL 0

#include <string>

namespace aeon
{
    std::string getUserDir();
    std::string getAeonDir();

    bool equals(std::string first, std::string second);

    std::string toString(int number);
    int toInt(std::string number);
    bool toBoolean(std::string boolean);

    void initAeonDirectories();

    double getCurrentTimeMilli();
    const std::string currentDateTime();
}
#endif // _AEONUTIL_
