#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <stdlib.h>

#include "Resource_Manager.h"
//#include "Post_Processor.h"
#include "Text_Renderer.h"
#include "Object_Renderer.h"
#include "Icon_Renderer.h"
#include "Camera.h"

// Game class. General framework to potentially use for other games in the future.


struct Projectile
{
	glm::vec3 CurrentPos;
	glm::vec3 OriginalPos;
	glm::vec3 Velocity0;
	float Radius;
	float Time;
	bool Collided;
};

struct Block
{
	glm::vec3 Position;
	glm::vec3 Size;
};

class Game
{
    public:
        // game state
		bool           Keys[1024];
        bool 		   KeysProcessed[1024];
        unsigned int Width, Height;
        double wRatio, hRatio;
        GLFWwindow* window;
        
        float Force  = 0.0;
        bool  loaded = false;
        
        // class objects
        Camera *camera;
        ObjectRenderer *Cubes;
        IconRenderer *Icons;
        
        // object state
        std::vector <Projectile> Projectiles;
        std::vector <Block>		 Blocks;
        
        // constructor/destructor
        Game(GLFWwindow* window, unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        void DoCollisions();
        CollisionDirections/*glm::vec3*/ DoCameraCollisions();
        void ProcessInput(float dt);
        void CheckButtons(double mousePosX, double mousePosY);
        bool CheckCollision(Projectile projectile, Block block);
        glm::vec3 CheckCollision(glm::vec3 position, Block block);
        void Update(float dt);
        void Render();
        void initProjectile(float force, float mass, float pitch, float yaw, glm::vec3 playerPos, float radius, float dt);
        glm::vec3 updateProjectilePos(float dt, unsigned int id);
        std::vector<Block> getClosestBlocks();
        std::vector<Block> getBlocksNearCentre(std::vector<Block> closestBlocks);
        void addBlock(std::vector<Block> filtered);
        void printCurrentBlockInfo(std::vector<Block> closestBlocks);
};

#endif
