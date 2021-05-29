#include "Icon_Renderer.h"

IconRenderer::IconRenderer(const Shader &shader)
{
	this->shader = shader;
    this->initQuadRenderData();
}

IconRenderer::~IconRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void IconRenderer::DrawIcon(Texture2D &texture, glm::vec2 position, glm::vec2 size)
{
    // prepare transformations
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f)); 
  
    this->shader.SetMatrix4("model", model);
  
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void IconRenderer::initQuadRenderData()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
	};

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

