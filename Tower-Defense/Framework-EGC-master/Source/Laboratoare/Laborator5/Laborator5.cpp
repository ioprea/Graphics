#include "Laborator5.h"
#include <math.h>
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

int counter = 0;
glm::vec3 transX;
float rotateObj = RADIANS(180);
float fov = RADIANS(45);
glm::vec3 translate = glm::vec3(0, 0, 0);
int weaponType = 1;
int playerLives = 3;

//Transfrom3D
// Translate matrix
inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
{
	// TODO implement translate matrix
	return glm::transpose(
		glm::mat4(1, 0, 0, translateX,
			0, 1, 0, translateY,
			0, 0, 1, translateZ,
			0, 0, 0, 1)
	);
}

// Scale matrix
inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
{
	// TODO implement scale matrix
	return glm::transpose(
		glm::mat4(scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, scaleZ, 0,
			0, 0, 0, 1)
	);
}

// Rotate matrix relative to the OZ axis
inline glm::mat4 RotateOZ(float radians)
{
	// TODO implement rotate matrix
	return glm::transpose(
		glm::mat4(cos(radians), -sin(radians), 0, 0,
			sin(radians), cos(radians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	);
}

// Rotate matrix relative to the OY axis
inline glm::mat4 RotateOY(float radians)
{
	// TODO implement rotate matrix
	return glm::transpose(
		glm::mat4(cos(radians), 0, -sin(radians), 0,
			0, 1, 0, 0,
			sin(radians), 0, cos(radians), 0,
			0, 0, 0, 1)
	);
}

// Rotate matrix relative to the OX axis
inline glm::mat4 RotateOX(float radians)
{
	// TODO implement rotate matrix
	return glm::transpose(
		glm::mat4(1, 0, 0, 0,
			0, cos(radians), -sin(radians), 0,
			0, sin(radians), cos(radians), 0,
			0, 0, 0, 1)
	);
}
//Transform3D

Laborator5::Laborator5()
{
}

Laborator5::~Laborator5()
{
}

void Laborator5::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::Camera();
	//initializare camera
	camera->Set(glm::vec3(0, 0.3, 2), glm::vec3(0,0.3,0), glm::vec3(0, 1, 0));
	

	//creare obiecte
	{
		Mesh* mesh = new Mesh("MonsterLowPoly");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "MonsterLowPoly.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}


	{
		Mesh* mesh = new Mesh("Flash");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "Flash.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("tower");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "tower.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);

	//Creare turnuri
	Tower* t1 = new Tower();
	t1->cooX = 5;
	t1->cooY = 0;
	t1->cooZ = 2;

	Tower* t2 = new Tower();
	t2->cooX = -5;
	t2->cooY = 0;
	t2->cooZ = 2;

	Tower* t3 = new Tower();
	t3->cooX = 0 ;
	t3->cooY = 0 ;
	t3->cooZ = -5;
	
	//Adaugam turnurile in vector
	towerVector.push_back(t1);
	towerVector.push_back(t2);
	towerVector.push_back(t3);

}



void Laborator5::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Laborator5::Update(float deltaTimeSeconds)
{ 
		//setare zoom-ului
		if (weaponType == 1) projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
		else if(weaponType == 2) projectionMatrix = glm::perspective(RADIANS(20), window->props.aspectRatio, 0.01f, 200.0f);
		else projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);

		//Randare turnuri
		for each(Tower* t in towerVector) {
			t->matrix = glm::mat4(1);
			t->matrix *= Translate(t->cooX, t->cooY, t->cooZ);
			t->matrix *= Scale(0.004, 0.004, 0.004);
			RenderMesh(meshes["tower"], shaders["VertexNormal"], t->matrix);
		}

		//Creare inamici
		if (counter % 200 == 0) {
			Enemy* en = new Enemy();
			en->cooX = -24;
			en->cooZ = -24;
			en->cooY = 0.6;	
			en->line1 = 0;
			en->line2 = 0;
			en->line3 = 0;
			
			enemyVector.push_back(en);
		}

		counter++;
		if (counter == 1000) counter = 0;

		//Randare inamici
		for (vector <Enemy*>::iterator i = enemyVector.begin(); i != enemyVector.end(); ) {
		
			Enemy* en = *i;
			en->matrix = glm::mat4(1);
			
			if (en->hp <= 0 && en->animation != 0) {
				en->matrix *= Translate(en->cooX, en->cooY, en->cooZ);
				en->matrix *= RotateOY(en->angle);
				en->matrix *= RotateOZ(115 - RADIANS(en->animation));
				en->matrix *= Scale(en->scale, en->scale, en->scale);
				en->scale -= 0.005;
				en->animation-= 1;
				en->cooY -= 0.005;
				Sleep(1);
			}
			else {
				if (en->line1 == 0) { en->cooX += 0.1; en->angle = RADIANS(270); }
				if (en->cooX > 23.9) { en->line1 = 1; }
				if (en->line1 == 1 && en->line2 == 0) { en->angle = RADIANS(45); en->cooX -= 0.05; en->cooZ += 0.05; }
				if (en->cooX < -23.9 && en->cooZ > 23.9) en->line2 = 1;
				if (en->line2 == 1 && en->line3 == 0) { en->angle = RADIANS(270); en->cooX += 0.1; }
				if (en->cooX > 23.9 && en->cooZ > 23.9) { en->line3 = 1, en->line2 = 1, en->line1 = 1; }
				en->matrix *= Translate(en->cooX, en->cooY, en->cooZ);
				en->matrix *= Scale(0.5, 0.5, 0.5);
				en->matrix *= RotateOY(en->angle);
			}

			RenderMesh(meshes["MonsterLowPoly"], shaders["VertexNormal"], en->matrix);

			//Verificare coliziuni turn - inamic
			for each(Tower* t in towerVector) {
			

				if (abs(t->cooX - en->cooX) < 10 && abs(t->cooZ - en->cooZ) < 10 && counter % 100 == 0) {
					Weapon* bullet = new Weapon();
					bullet->cooX = t->cooX;
					bullet->cooY = 0.3;
					bullet->cooZ = t->cooZ;
					bullet->angle = atan2(t->cooZ - en->cooZ , t->cooX - en->cooX) - RADIANS(180);
					towerBulletVector.push_back(bullet);
				}
			}

			//Arme turn 
			for (vector <Weapon*>::iterator i = towerBulletVector.begin(); i != towerBulletVector.end(); ) {
				Weapon* wp2 = *i;
				wp2->matrix = glm::mat4(1);
				wp2->matrix *= Translate(wp2->cooX, wp2->cooY, wp2->cooZ);
				wp2->matrix *= Scale(0.5, 0.5, 0.5);
				wp2->cooX = wp2->cooX + cos(wp2->angle) /20;
				wp2->cooZ = wp2->cooZ + sin(wp2->angle) /20;
				if (abs(wp2->cooX - en->cooX) < 1 && abs(wp2->cooZ - en->cooZ) < 1) {
					en->hp -= 50; //dead if 0
					wp2->hit = 1;
				}
				RenderMesh(meshes["sphere"], shaders["VertexNormal"], wp2->matrix);
				if (abs(wp2->cooX) > 25 || abs(wp2->cooZ) > 25) i = towerBulletVector.erase(i);
				else if(wp2->hit == 1) i = towerBulletVector.erase(i);
				else i++;
			}

			//Arme player
			for (vector <Weapon*>::iterator i = weaponVector.begin(); i != weaponVector.end(); ) {
				Weapon* wp = *i;
				wp->matrix = glm::mat4(1);
				wp->matrix *= Translate(wp->cooX, wp->cooY, wp->cooZ);
				wp->matrix *= Scale(0.1, 0.1, 0.1);


				if (weaponType == 1) {
					
					wp->cooX = wp->cooX + cos(wp->angle) / 10;
					wp->cooZ = wp->cooZ + sin(wp->angle) / 10;
				}
				else if (weaponType == 2) {
					
					wp->cooX = wp->cooX + cos(wp->angle) / 3;
					wp->cooZ = wp->cooZ + sin(wp->angle) / 3;
				}
				else {
					
					wp->cooX = wp->cooX + cos(wp->angle) / 20;
					wp->cooZ = wp->cooZ + sin(wp->angle) / 20;
					wp->cooY = sin(RADIANS(wp->playerWeaponY)) + 0.08;
					wp->playerWeaponY++;

				}
				wp->distance++;
				if (abs(wp->cooX - en->cooX) < 0.6 && abs(wp->cooZ - en->cooZ) < 0.6) {
					wp->hit = 1;
					if (weaponType == 1) en->hp -= 35;
					else if (weaponType == 2) en->hp -= 50;
					else en->hp -= 100;
				}


				RenderMesh(meshes["sphere"], shaders["VertexNormal"], wp->matrix);

				//Verificare eliminare glont player
				if (wp->distance == 40 && weaponType == 1) { i = weaponVector.erase(i); }
				else if (weaponType == 2 && wp->distance == 30 || wp->cooY <= 0.05) { i = weaponVector.erase(i); }
				else if(wp->hit == 1) i = weaponVector.erase(i);
				else if (abs(wp->cooX) > 25 || abs(wp->cooZ) > 25) {  i = weaponVector.erase(i); }
				else i++;
			}

			//Verificare eliminare inamic 
			if (en->line1 == 1 && en->line2 == 1 && en->line3 == 1) { playerLives--; i = enemyVector.erase(i); }
			else if (en->hp <= 0 && en->animation <= 0) {
				
				i = enemyVector.erase(i);
			}
			else i++;
		}
		
		//Pozitionare si randare player
		glm::mat4 modelMatrix = glm::mat4(1);
		transX = camera->GetTargetPosition();
		transX.y = 0;
		modelMatrix = glm::translate(modelMatrix, transX);
		modelMatrix *= Scale(0.1, 0.1, 0.1);
		modelMatrix *= RotateOY(rotateObj);
		RenderMesh(meshes["Flash"], shaders["VertexNormal"], modelMatrix);

		if (playerLives == 0) {
			cout << "GAME OVER" << endl; exit(1);
		}
	
}

void Laborator5::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Laborator5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator5::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(cameraSpeed*deltaTime);
			
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-cameraSpeed*deltaTime);
		//	translate -= glm::normalize(rightObj) *cameraSpeed*deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-cameraSpeed*deltaTime);

		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(cameraSpeed*deltaTime);

		}

	}
}

void Laborator5::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_1) { weaponType = 1; }
	if (key == GLFW_KEY_2) { weaponType = 2; }
	if (key == GLFW_KEY_3) { weaponType = 3; }

}

void Laborator5::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;
		
		camera->RotateThirdPerson_OX(sensivityOX*deltaY);
		camera->RotateThirdPerson_OY(-sensivityOY*deltaX);
		rotateObj -= -sensivityOY*deltaX;

	}
}

void Laborator5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == 1) {
		
		Weapon *bullet = new Weapon();
		bullet->cooX = camera->GetTargetPosition().x;
		bullet->cooZ = camera->GetTargetPosition().z;
		bullet->cooY = 0.3;
		bullet->angle = rotateObj - RADIANS(-90);
		weaponVector.push_back(bullet);
	}
}

void Laborator5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator5::OnWindowResize(int width, int height)
{
}
