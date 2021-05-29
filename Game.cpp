#include "Game.h"


/*
// collision functions
// -------------------
bool CheckCollision(GameObject &one, GameObject &two); // AABB - AABB collision
Collision CheckCollision(BallObject &one, GameObject &two); // AABB - Circle collision
Direction VectorDirection(glm::vec2 target); 


// Mouse functions
// ---------------
bool CheckHover(double mousePosX, double mousePosY, Button &button);
*/


// game class functions
// --------------------
Game::Game(GLFWwindow* window, unsigned int width, unsigned int height) : Keys(), Width(width), Height(height), window(window)
{ 
	int winWidth = 0;
	int winHeight = 0;
	glfwGetWindowSize(window, &winWidth, &winHeight);
	if (winWidth > 0 && winHeight > 0)
	{
		this->wRatio = static_cast<float>(width) / winWidth;
		this->hRatio = static_cast<float>(height) / winHeight;
	}
	else
	{
		this->wRatio = 1;
		this->hRatio = 1;
	}
	this->camera = new Camera(glm::vec3(0.0f, 0.5f, 0.0f));
}

Game::~Game()
{

}

void Game::Init()
{
	// load shaders
	ResourceManager::LoadShader("Shaders/Object_vShader.txt", "Shaders/Object_fShader.txt", nullptr, "objectShader");
	ResourceManager::LoadShader("Shaders/Icon_vShader.txt", "Shaders/Icon_fShader.txt", nullptr, "iconShader");
    
    // configure shaders


    // set render-specific controls
    Cubes = new ObjectRenderer(ResourceManager::GetShader("objectShader"));
    Icons = new IconRenderer(ResourceManager::GetShader("iconShader"));
    
    // load textures
	ResourceManager::LoadTexture("Textures/wood.png", false, "wood");
	ResourceManager::LoadTexture("Textures/Center.png", true, "cross");
    
    // text
    //Text = new TextRenderer(this->Width, this->Height);
    //Text->Load("nasalization.ttf", 24);
	
	
	//Basic Cubes
	Block block1;
	block1.Position = glm::vec3(-5.0f, 0.0f, 0.0f);
	block1.Size =  glm::vec3(1.0f);
	this->Blocks.push_back(block1);
	
	Block block2;
	block2.Position = glm::vec3(0.0f, 0.0f, -5.0f);
	block2.Size = glm::vec3(1.0f);
	this->Blocks.push_back(block2);
	
	Block block3;
	block3.Position = glm::vec3(0.0f, 5.0f, 0.0f);
	block3.Size = glm::vec3(1.0f);
	this->Blocks.push_back(block3);
	
	Block block4;
	block4.Position = glm::vec3(0.0f, -5.0f, 0.0f);
	block4.Size = glm::vec3(1.0f);
	this->Blocks.push_back(block4);
	
	Block block5;
	block5.Position = glm::vec3(0.0f, 0.0f, 5.0f);
	block5.Size = glm::vec3(1.0f);
	this->Blocks.push_back(block5);
	
	Block block6;
	block6.Position = glm::vec3(5.0f, 0.0f, 0.0f);
	block6.Size = glm::vec3(1.0f);
	this->Blocks.push_back(block6);
}

void Game::DoCollisions()
{
	for (int i = 0; i < this->Projectiles.size(); i++)
	{
		if (!this->Projectiles[i].Collided)
		{
			//std::cout << this->Projectiles[i].Collided << std::endl;
			for (int j = 0; j < this->Blocks.size(); j++)
			{
				if (this->CheckCollision(this->Projectiles[i], this->Blocks[j]))
				{
					this->Projectiles[i].Collided = true;
					//std::cout << "collided\n";
					break;
				}
			}
		}
	}
}  

CollisionDirections/*glm::vec3*/ Game::DoCameraCollisions()
{
	for (int i = 0; i < this->Blocks.size(); i++) 
	{
		glm::vec3 direction = this->CheckCollision(this->camera->Position, this->Blocks[i]);
		//return(direction);
		
		if (direction.x < 0.0) {
			return (NORTH);
		} else if (direction.x > 0.0) {
			return (SOUTH);
		} else {
			if (direction.z < 0.0) {
				return (EAST);
			} else if (direction.z > 0.0) {
				return (WEST);
			} else {
				if (direction.y < 0.0) {
					return (TOP);
				} else if (direction.y > 0.0) {
					return (BOTTOM);
				}
			}
		}
		
	}
	
	return (NONE);
}  

void Game::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_W])
    {
    	this->camera->ProcessKeyboard(FORWARD, this->DoCameraCollisions(), dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    	//this->camera->UpdateCamPos(this->DoCameraCollisions());
    }
    if (this->Keys[GLFW_KEY_S])
    {
    	this->camera->ProcessKeyboard(BACKWARD, this->DoCameraCollisions(), dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    	//this->camera->UpdateCamPos(this->DoCameraCollisions());
    }
    if (this->Keys[GLFW_KEY_A])
    {
    	this->camera->ProcessKeyboard(LEFT, this->DoCameraCollisions(), dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    	//this->camera->UpdateCamPos(this->DoCameraCollisions());
    }
    if (this->Keys[GLFW_KEY_D])
    {
    	this->camera->ProcessKeyboard(RIGHT, this->DoCameraCollisions(), dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    	//this->camera->UpdateCamPos(this->DoCameraCollisions());
    }
    if (this->Keys[GLFW_KEY_SPACE])
    {
    	this->Force += 10.0;
    	this->loaded = true;
    	//std::cout << this->Force << "\t";
    }
    if (!this->Keys[GLFW_KEY_SPACE] && this->loaded)
    {
    	initProjectile(this->Force, 0.5f, this->camera->Pitch, this->camera->Yaw, this->camera->Position, 0.1f, dt);
    	this->Force = 0.0;
    	this->loaded = false;
    	//std::cout << "Projectile initialized\n";
    }
}

void Game::CheckButtons(double mousePosX, double mousePosY)
{
	/*
	srand(glfwGetTime());
	float x = (float)(rand() % 15 - 8);
	float y = (float)(rand() % 8);
	float z = (float)(rand() % 15 - 8);
	
	Block newBlock;
	newBlock.Position = glm::vec3(x, y, z);
	newBlock.Size = glm::vec3(1.0);
	
	Blocks.push_back(newBlock);
	*/
	std::vector<Block> closestBlocks = this->getClosestBlocks();
	this->printCurrentBlockInfo(closestBlocks);
	std::cout << "Function 1 passed." << std::endl;
	
	closestBlocks = this->getBlocksNearCentre(closestBlocks);
	this->printCurrentBlockInfo(closestBlocks);
	std::cout << "Function 2 passed." << std::endl;
	
	this->addBlock(closestBlocks);
	std::cout << "Function 3 passed." << std::endl;
}


bool Game::CheckCollision(Projectile projectile, Block block) // AABB - Circle collision
{
    // get center point circle first 
    glm::vec3 center(projectile.CurrentPos);
    // calculate AABB info (center, half-extents)
    glm::vec3 aabb_half_extents(block.Size.x / 2.0f, block.Size.y / 2.0f, block.Size.z / 2.0f);
    glm::vec3 aabb_center(block.Position);
    // get difference vector between both centers
    glm::vec3 difference = center - aabb_center;
    glm::vec3 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec3 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    
    if (glm::length(difference) <= projectile.Radius)
        return true;
    else
        return false;
}

glm::vec3 Game::CheckCollision(glm::vec3 position, Block block) // AABB - Circle collision
{
    // calculate AABB info (center, half-extents)
    glm::vec3 aabb_half_extents(block.Size.x / 2.0f, block.Size.y / 2.0f, block.Size.z / 2.0f);
    glm::vec3 aabb_center(block.Position);
    // get difference vector between both centers
    glm::vec3 difference = position - aabb_center;
    glm::vec3 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec3 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - position;
    
    if (glm::length(difference) <= 0.75) 
    {
    	float scale = glm::length(glm::vec3(0.75)) - glm::length(difference);
    	glm::vec3 direction = glm::normalize(closest);
    	glm::vec3 offset = direction * scale;
        return (/*offset*/direction);
    }
    else
        return (glm::vec3(0.0));
}


void Game::Update(float dt)
{
	// update objects
	// --------------
	// update camera position
	ResourceManager::GetShader("objectShader").Use();
    float fov = this->camera->fov;
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
    glm::mat4 view = this->camera->GetViewMatrix();
    ResourceManager::GetShader("objectShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("objectShader").SetMatrix4("view", view);
    ResourceManager::GetShader("objectShader").SetVector3f("viewPos", this->camera->Position);
	ResourceManager::GetShader("objectShader").SetInteger("nr_point_lights", this->Projectiles.size());
	
    // update projectile position				
    std::vector <unsigned int> expiredProjectiles;
	
	char buf[30];	
    for (int i = 0; i < this->Projectiles.size(); i++)
	{
		sprintf(buf, "lightPositions[%d]", i);
		ResourceManager::GetShader("objectShader").SetVector3f(buf, this->Projectiles[i].CurrentPos);
		if (this->Projectiles[i].CurrentPos.y > 0 && !this->Projectiles[i].Collided)
		{
			this->Projectiles[i].CurrentPos = updateProjectilePos(dt, i);
			//std::cout << this->Projectiles[i].CurrentPos.x << " " << this->Projectiles[i].CurrentPos.y << " " << this->Projectiles[i].CurrentPos.z << std::endl;
		}
		else
		{
			this->Projectiles[i].Time += dt;
			if (this->Projectiles[i].Time > 100.0f)
				expiredProjectiles.push_back(i);
		}
	}
	
	for (int i = 0; i < expiredProjectiles.size(); i++)
	{
		this->Projectiles.erase(Projectiles.begin() + expiredProjectiles[i] + i);
		//std::cout << "Projectile erased\n";
	}
	
	expiredProjectiles.clear();
	
	
	// check for collisions
	this->DoCollisions();
}

void Game::Render()
{
	auto wood = ResourceManager::GetTexture("wood");
	auto cross = ResourceManager::GetTexture("cross");
	// Set up post processing framebuffers
	// -----------------------------------
	//Effects->BeginRender();
	
	// Render scene
	// ------------
    // draw background

    // draw objects
    for (int i = 0; i < this->Blocks.size(); i++)
    {
		this->Cubes->DrawCube(wood, this->Blocks[i].Position, this->Blocks[i].Size, 0.0f, false, glm::vec3(0.5f, 0.8f, 0.0f));
	}
	
	this->Cubes->DrawSphere(glm::vec3(5.0f, 2.5f, 0.0f), 0.1f, glm::vec3(0.0f, 1.0f, 1.0f), true);
	
	for (int i = 0; i < this->Projectiles.size(); i++)		
	{
		//if (this->Projectiles[i].CurrentPos.y > 0)
			this->Cubes->DrawSphere(this->Projectiles[i].CurrentPos, this->Projectiles[i].Radius, glm::vec3(1.0f), true);
		//std::cout << "Sphere drawn\n";
	}
    
    // Render Text

    
    // Do post processing effects
    // --------------------------
    ResourceManager::GetShader("iconShader").Use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("iconShader").SetMatrix4("projection", projection);
    Icons->DrawIcon(cross, glm::vec2(this->Width/2.0, this->Height/2.0), glm::vec2(10.0f, 10.0f));
}

void Game::initProjectile(float force, float mass, float pitch, float yaw, glm::vec3 originalPos, float radius, float dt)	
{
	float acceleration = force / mass;
	const float g = 9.8;
	//float Yaw = yaw * -1.0;
	float velocity0 = acceleration * dt;
	float velocity0_x = velocity0 * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	
	float velocity0_y = velocity0 * sin(glm::radians(pitch));
	float velocity0_z = velocity0 * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	//if (abs(yaw) > 90.0)
	//	velocity0_x *= -1.0;
	glm::vec3 projectilePos;
	projectilePos.x = (velocity0_x * dt) + originalPos.x;
	projectilePos.y = ((velocity0_y - g * dt) * dt) + originalPos.y;
	projectilePos.z = (velocity0_z * dt) + originalPos.z;
	
	Projectile projectile;
	projectile.OriginalPos = originalPos;
	projectile.CurrentPos = projectilePos;
	projectile.Velocity0 = glm::vec3(velocity0_x, velocity0_y, velocity0_z);
	projectile.Radius = radius;
	projectile.Time = dt;
	projectile.Collided = false;
	
	this->Projectiles.push_back(projectile);
}

glm::vec3 Game::updateProjectilePos(float dt, unsigned int id)
{
	this->Projectiles[id].Time += dt;
	const float g = 9.8;
	glm::vec3 projectilePos;
	
	projectilePos.x = (this->Projectiles[id].Velocity0.x * this->Projectiles[id].Time) + this->Projectiles[id].OriginalPos.x;
	projectilePos.y = ((this->Projectiles[id].Velocity0.y - g * this->Projectiles[id].Time) * this->Projectiles[id].Time) + this->Projectiles[id].OriginalPos.y;
	projectilePos.z = (this->Projectiles[id].Velocity0.z * this->Projectiles[id].Time) + this->Projectiles[id].OriginalPos.z;
	
	/*
	projectilePos.x = (this->Projectiles[id].Velocity0.x * dt) + this->Projectiles[id].CurrentPos.x;
	projectilePos.y = ((this->Projectiles[id].Velocity0.y - g * this->Projectiles[id].Time) * this->Projectiles[id].Time) + this->Projectiles[id].OriginalPos.y;
	projectilePos.z = (this->Projectiles[id].Velocity0.z * dt) + this->Projectiles[id].CurrentPos.z;
	*/
	return projectilePos;
}

std::vector<Block> Game::getClosestBlocks()
{
	std::vector<Block> closestCubes;
	for (int i = 0; i < Blocks.size(); i++) 
	{
		glm::vec3 pos = Blocks[i].Position;
		if (abs(glm::length(pos - this->camera->Position)) <= 3.0) 
		{
			closestCubes.push_back(Blocks[i]);
		}
	}
	return (closestCubes);
}

std::vector<Block> Game::getBlocksNearCentre(std::vector<Block> closestBlocks)
{
	std::vector<Block> filtered;
	float fov = this->camera->fov;
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
    glm::mat4 view = this->camera->GetViewMatrix();
	for (int i = 0; i < closestBlocks.size(); i++)
	{
		glm::vec4 clipPos = glm::vec4(projection * view * glm::vec4(closestBlocks[i].Position, 1.0));
		glm::vec3 realPos = glm::vec3(clipPos.x / clipPos.w, clipPos.y / clipPos.w, clipPos.z / clipPos.w);
		if (realPos.z > 0.0)
		{
			if (realPos.x < 1.0 && realPos.x > -1.0 && realPos.y < 1.0 && realPos.y > -1.0)
			{
				filtered.push_back(closestBlocks[i]);
				std::cout << "Block added to filterted list" << std::endl;
			}
		}
	}
	
	return (filtered);
}

void Game::addBlock(std::vector<Block> filtered)
{
	float fov = this->camera->fov;
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
    glm::mat4 view = this->camera->GetViewMatrix();
    std::cout << "Prelim calcs completed" << std::endl;
    
    if (filtered.size() > 0)
    {
		for (int i = 0; i < filtered.size()-1; i++)
		{
			for (int j = i+1; j < filtered.size(); j++)
			{
				if (filtered[j].Position.z < filtered[i].Position.z)
				{
					Block temp = filtered[i];
					filtered[i] = filtered[j];
					filtered[j] = temp;
				}
			}
		}
		
		glm::vec3 orientation = glm::vec3(this->camera->Front.x / abs(this->camera->Front.x), this->camera->Front.y / abs(this->camera->Front.y), this->camera->Front.z / abs(this->camera->Front.z));
		float reverse;
		if (orientation.x == orientation.z)
		{
			reverse = -1.0;
		}
		else
		{
			reverse = 1.0;
		}
		
		float totalReversal = reverse * orientation.y * -1.0;
		
		for (int i = 0; i < filtered.size(); i++)
		{	
			glm::vec3 centre = filtered[i].Position;
			
			//Front-facing Cube vertices
			std::vector<glm::vec3> vertices;
			glm::vec3 _1 = centre + glm::vec3(0.5) * glm::vec3( 1.0				   , -1.0,  1.0		  		   ) * orientation;
			vertices.push_back(_1);
			glm::vec3 _2 = centre + glm::vec3(0.5) * glm::vec3( 1.0 * totalReversal, -1.0, -1.0 * totalReversal) * orientation;			//_1 + glm::vec3(0.0, 0.0, 1.0);
			vertices.push_back(_2);
			glm::vec3 _3 = centre + glm::vec3(0.5) * glm::vec3(-1.0				   , -1.0, -1.0		  		   ) * orientation;			//_2 + glm::vec3(-1.0, 0.0, 0.0);
			vertices.push_back(_3);
			glm::vec3 _4 = centre + glm::vec3(0.5) * glm::vec3(-1.0 * totalReversal, -1.0,  1.0 * totalReversal) * orientation;			//_3 + glm::vec3(0.0, 0.0, -1.0);
			vertices.push_back(_4);
			glm::vec3 _5 = centre + glm::vec3(0.5) * glm::vec3(-1.0 * totalReversal,  1.0,  1.0 * totalReversal) * orientation;			//_4 + glm::vec3(0.0, -1.0, 0.0);
			vertices.push_back(_5);
			glm::vec3 _6 = centre + glm::vec3(0.5) * glm::vec3(-1.0				   ,  1.0, -1.0				   ) * orientation;			//_5 + glm::vec3(0.0, 0.0, 1.0);
			vertices.push_back(_6);
			glm::vec3 _7 = centre + glm::vec3(0.5) * glm::vec3( 1.0 * totalReversal,  1.0, -1.0 * totalReversal) * orientation;			//_6 + glm::vec3(1.0, 0.0, 0.0);
			vertices.push_back(_7);
			
			for (int j = 0; j < 7; j++)
			{
				std::cout << vertices[j].x << " " << vertices[j].y << " " << vertices[j].z << std::endl;
				glm::vec4 clipPos = glm::vec4(projection * view * glm::vec4(vertices[j], 1.0));
				vertices[j] = glm::vec3(clipPos.x / clipPos.w, clipPos.y / clipPos.w, clipPos.z / clipPos.w);
				std::cout << vertices[j].x << " " << vertices[j].y << " " << vertices[j].z << std::endl;
			}
			
			
			
			//Face 1
			// -----
			std::cout << "Beginning Face 1" << std::endl;
			bool check1 = false, check2 = false, check3 = false, check4 = false;
			
			float Xg = -1 * vertices[0].y * ((vertices[1].x - vertices[0].x) / (vertices[1].y - vertices[0].y)) + vertices[0].x;
			if ((Xg * orientation.y * -1.0) > 0.0)
			{
				check1 = true;
				std::cout << "Check 1 passed" << std::endl;
			}
			else
				std::cout << "Check 1 not passed" << std::endl;
			float Yg1 = vertices[1].y - ((vertices[2].y - vertices[1].y) / (vertices[2].x - vertices[1].x)) * vertices[1].x;
			if ((Yg1 * orientation.y * -1.0) < 0.0)
			{
				check2 = true;
				std::cout << "Check 1 passed" << std::endl;
			}
			else
				std::cout << "Check 1 not passed" << std::endl;
			Xg = -1 * vertices[2].y * ((vertices[3].x - vertices[2].x) / (vertices[3].y - vertices[2].y)) + vertices[2].x;
			if ((Xg * orientation.y * -1.0) < 0.0)
			{
				check3 = true;
				std::cout << "Check 1 passed" << std::endl;
			}
			else
				std::cout << "Check 1 not passed" << std::endl;
			float Yg = vertices[3].y - ((vertices[0].y - vertices[3].y) / (vertices[0].x - vertices[3].x)) * vertices[3].x;
			if ((Yg * orientation.y * -1.0) > 0.0)
			{
				check4 = true;
				std::cout << "Check 1 passed" << std::endl;
			}
			else
				std::cout << "Check 1 not passed" << std::endl;
				
			if (check1 && check2 && check3 && check4)
			{
				glm::vec3 collided = CheckCollision(this->camera->Position, filtered[i]);
				if (collided.x == 0.0 && collided.y == 0.0 && collided.z == 0.0)
				{
					Block newBlock;
					newBlock.Position = glm::vec3(centre.x, centre.y - orientation.y, centre.z);
					newBlock.Size = glm::vec3(1.0);
					this->Blocks.push_back(newBlock);
					std::cout << "New block added on face 1" << std::endl;
				}
				break;
			} 
			else
			{
				//Face2
				//-----
				std::cout << "Beginning Face 2" << std::endl;
				check1 = false;
				check2 = false;
				check3 = false;
				check4 = false;
				
				Xg = -1 * vertices[1].y * ((vertices[6].x - vertices[1].x) / (vertices[6].y - vertices[1].y)) + vertices[1].x;
				if ((Xg * orientation.y * -1.0) > 0.0)
				{
					check1 = true;
					std::cout << "Check 1 passed" << std::endl;
				}
				else
					std::cout << "Check 1 not passed" << std::endl;
				Yg = vertices[6].y - ((vertices[5].y - vertices[6].y) / (vertices[5].x - vertices[6].x)) * vertices[6].x;
				if ((Yg * orientation.y * -1.0) < 0.0)
				{
					check2 = true;
					std::cout << "Check 1 passed" << std::endl;
				}
				else
					std::cout << "Check 1 not passed" << std::endl;
				float Xg1 = -1 * vertices[5].y * ((vertices[2].x - vertices[5].x) / (vertices[2].y - vertices[5].y)) + vertices[5].x;
				if ((Xg1 * orientation.y * -1.0) < 0.0)
				{
					check3 = true;
					std::cout << "Check 1 passed" << std::endl;
				}
				else
					std::cout << "Check 1 not passed" << std::endl;
				if ((Yg1 * orientation.y * -1.0) > 0.0)
				{
					check4 = true;
					std::cout << "Check 1 passed" << std::endl;
				}
				else
					std::cout << "Check 1 not passed" << std::endl;
					
				if (check1 && check2 && check3 && check4)
				{
					glm::vec3 collided = CheckCollision(this->camera->Position, filtered[i]);
					if (collided.x == 0.0 && collided.y == 0.0 && collided.z == 0.0)
					{
						Block newBlock;
						if (totalReversal == 1.0)
						{
							newBlock.Position = glm::vec3(centre.x, centre.y, centre.z - orientation.z);
						}
						else
						{
							newBlock.Position = glm::vec3(centre.x - orientation.x, centre.y, centre.z);
						}
						newBlock.Size = glm::vec3(1.0);
						this->Blocks.push_back(newBlock);
						std::cout << "New block added at face 2" << std::endl;
					}
					break;
				} 
				else
				{
					//Face3
					//-----
					std::cout << "Beginning Face 3" << std::endl;
					check1 = false;
					check2 = false;
					check3 = false;
					check4 = false;
					
					if ((Xg1 * orientation.y * -1.0) > 0.0)
					{
						check1 = true;
						std::cout << "Check 1 passed" << std::endl;
					}
					else
						std::cout << "Check 1 not passed" << std::endl;
					Yg = vertices[5].y - ((vertices[4].y - vertices[5].y) / (vertices[4].x - vertices[5].x)) * vertices[5].x;
					if ((Yg * orientation.y * -1.0) < 0.0)
					{
						check2 = true;
						std::cout << "Check 1 passed" << std::endl;
					}
					else
						std::cout << "Check 1 not passed" << std::endl;
					Xg = -1 * vertices[4].y * ((vertices[3].x - vertices[4].x) / (vertices[3].y - vertices[4].y)) + vertices[4].x;
					if ((Xg * orientation.y * -1.0) < 0.0)
					{
						check3 = true;
						std::cout << "Check 1 passed" << std::endl;
					}
					else
						std::cout << "Check 1 not passed" << std::endl;
					Yg = vertices[3].y - ((vertices[2].y - vertices[3].y) / (vertices[2].x - vertices[3].x)) * vertices[3].x;
					if ((Yg * orientation.y * -1.0) > 0.0)
					{
						check4 = true;
						std::cout << "Check 1 passed" << std::endl;
					}
					else
						std::cout << "Check 1 not passed" << std::endl;
					
					if (check1 && check2 && check3 && check4)
					{
						glm::vec3 collided = CheckCollision(this->camera->Position, filtered[i]);
						if (collided.x == 0.0 && collided.y == 0.0 && collided.z == 0.0)
						{
							Block newBlock;
							if (totalReversal == 1.0)
							{
								newBlock.Position = glm::vec3(centre.x - orientation.x, centre.y, centre.z);
							}
							else
							{
								newBlock.Position = glm::vec3(centre.x, centre.y, centre.z - orientation.z);
							}
							newBlock.Size = glm::vec3(1.0);
							this->Blocks.push_back(newBlock);
							std::cout << "New Block added at face 3" << std::endl;
						}
						break;
					}
				}
			}
		}
				
	}
}

void Game::printCurrentBlockInfo(std::vector<Block> closestBlocks)
{
	float fov = this->camera->fov;
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
    glm::mat4 view = this->camera->GetViewMatrix();
	for (int i = 0; i < closestBlocks.size(); i++)
	{
		glm::vec4 clipPos = glm::vec4(projection * view * glm::vec4(closestBlocks[i].Position, 1.0));
		glm::vec3 realPos = glm::vec3(clipPos.x / clipPos.w, clipPos.y / clipPos.w, clipPos.z / clipPos.w);
		std::cout << closestBlocks[i].Position.x << " " << closestBlocks[i].Position.y << " " << closestBlocks[i].Position.z << "\t" << realPos.x << " " << realPos.y << " " << realPos.z << std::endl;
	}
}

/*
// collision functions
// -------------------
bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}



Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}    



// Mouse functions
// ---------------
bool CheckHover(double mousePosX, double mousePosY, Button &button)
{
	// mouse x-axis within button?
    bool collisionX = button.Position.x + button.Size.x >= mousePosX && button.Position.x <= mousePosX;
    // mouse y-axis within?
    bool collisionY = button.Position.y + button.Size.y >= mousePosY && button.Position.y <= mousePosY;
    // collision only if on both axes
    
    if (collisionX && collisionY)
    {
		button.Hover = true;
		button.TextColor = glm::vec3(0.0f);
		return true;
	}
	else
	{
		button.Hover = false;
		button.TextColor = glm::vec3(1.0f);
		return false;
	}
}
*/







