#ifndef _AEONBASICOBJECTS_
#define _AEONBASICOBJECTS_
#include <string>

namespace aeon
{
    class aeonrenderobject
    {
    public:
        virtual void render() = 0;
    };
    class aeonstring : public aeonrenderobject
    {
    public:
        aeonstring();
        aeonstring(std::string);
        ~aeonstring();
        virtual void render();

        // Setters
        void setString(std::string);
        void setPosition(int x, int y);
        void setColor(float red, float green, float blue);

        // Getters
        std::string getString();
        void getPosition(int * x, int * y);
        int getPositionX();
        int getPositionY();
    private:
        int xPos,yPos;
        float r,g,b;
    };
}

#endif // _AEONBASICOBJECTS_
