#ifndef _AEONUTIL_
#define _AEONUTIL_
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
