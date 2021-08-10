#include "engine.hpp"

#include <glm/glm.hpp>

namespace camera {

typedef enum ProjectionType {
    PERSPECTIVE_PROJECTION,
    ORTHOGONAL_PROJECTION,

} ProjectionType;


class Camera {
    private:
        glm::mat4 perspective;
        glm::mat4 worldToCamera;

        create::Engine* engine;

        glm::vec3 eye;
        glm::vec3 target;
        glm::vec3 up;

    public:
        Camera(ProjectionType projection, glm::vec3 eye, glm::vec3 target, glm::vec3 camera_up, create::Engine* eng);
        ~Camera();
        
        //retrieve neccesary data
        glm::mat4 getProjectionMatrix();
        glm::mat4 getCameraMatrix();

        //update the camera
        void updateCameraEye(glm::vec3 newEye);
        void updateCameraTarget(glm::vec3 newTarget);
        void updateCameraOrientation(glm::vec3 camera_up);

    private:
        glm::mat4 createCameraMatrix(glm::vec3 cameraPos, glm::vec3 lookAt, glm::vec3 up);
        glm::mat4 createOrthogonalMatrix(int width, int height, float n, float f);
        glm::mat4 createPerspectiveMatrix(float angle, float aspect, float n, float f);
};

}