#include "engine.hpp"

<<<<<<< HEAD
<<<<<<< HEAD
#include <string.h>

=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
namespace gameObject {

//square object
class Square {
    private:
        create::Engine* engine;
    public:
        Square(int x, int y, int s, Color c, create::Engine* eng);
        ~Square();
        void translate(float x, float y);
<<<<<<< HEAD
<<<<<<< HEAD
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

=======
};


}
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
=======
};


}
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
