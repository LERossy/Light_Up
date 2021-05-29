#ifndef ICON_RENDERER_H
#define ICON_RENDERER_H

#include "shader.h"
#include "Texture.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "vector"

class IconRenderer
{
    public:
        IconRenderer(const Shader &shader);
        ~IconRenderer();

        void DrawIcon(Texture2D &texture, glm::vec2 position, glm::vec2 size);
        
    private:
        Shader       shader; 
        unsigned int quadVAO;

        void initQuadRenderData();
};

#endif
