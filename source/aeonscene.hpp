#ifndef _AEONSCENE_
#define _AEONSCENE_

namespace aeon
{
    class Scene
    {
    public:
        virtual Scene();
        virtual ~Scene();

        virtual void load();

        virtual void createModel(std::string handle, std::string file);
        virtual void createNode(std::string handle, double x, double y, double z);
        // TODO: Create child node

        virtual void attachModel(std::string node, std::string model);

        virtual void processInput();
        virtual void render();
    protected:
        // TODO: Vector of models/nodes/cameras
        //Camera cameraMan;
        InputHandler * aInput;
    }
}

#endif // _AEONSCENE_
