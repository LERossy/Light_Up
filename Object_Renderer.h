#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include "shader.h"
#include "Texture.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "vector"

class ObjectRenderer
{
    public:
        ObjectRenderer(const Shader &shader);
        ~ObjectRenderer();

        void DrawCube(Texture2D &texture, glm::vec3 position, glm::vec3 size = glm::vec3(1.0f), float rotate = 0.0f /*, glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f)*/ , bool color = false, glm::vec3 objectColor = glm::vec3(1.0f));
        void DrawSphere(glm::vec3 position, float radius = 0.1f, glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f), bool light = false);
        
    private:
        Shader       shader; 
        unsigned int cubeVAO;
        unsigned int sphereVAO;
        int indexCount;

        void initCubeRenderData();
        void initSphereRenderData();
};

#endif
