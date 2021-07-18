#include "engine.hpp"

namespace gameObject {

//square object
class Square {
    private:
        create::Engine* engine;
    public:
        Square(int x, int y, int s, Color c, create::Engine* eng);
        ~Square();
        void translate(float x, float y);
};


}