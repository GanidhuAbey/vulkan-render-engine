#include "engine.hpp"

#include <string.h>

namespace gameObject {
//empty object
class EmptyObject {
    private:
        create::Engine* engine;
    public:
        EmptyObject(create::Engine* eng);
        ~EmptyObject();
        void addMeshData(const std::string& fileName);
    private:
        //void abstractToVector(std::vector* vector, void* data);
};

//square object
class Square {
    private:
        create::Engine* engine;
    public:
        Square(int x, int y, int s, Color c, create::Engine* eng);
        ~Square();
        void translate(float x, float y);
        void rotate(float angle);
        void material(const char texture);
};

//cube object
class Cube {
    private:
        create::Engine* engine;
    public:
        //for now the z coordinate won't do anything
        Cube(int x, int y, int z, int s, Color c, create::Engine* eng);
        ~Cube();
        void rotate(float angle);
};

//light object
class Light {
    private:
        create::Engine* engine;

    public:
        Light(int x, int y, int z, Color c, create::Engine* eng);
        ~Light();

};

}

