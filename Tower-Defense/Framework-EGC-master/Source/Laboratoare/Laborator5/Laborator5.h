#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
using namespace std;

class Tower {
public:
	
	float cooX;
	float cooY;
	float cooZ;
	float angle;
	glm::mat4 matrix;

};

class Weapon {
public:
	int type; // 1 - pistol , 2 - shotgun , 3 - grenade launcher
	float cooX;
	float cooY;
	float cooZ;
	float angle;
	float distance = 0;
	int playerWeaponY = 0;
	int hit = 0; //0 - nu a nimerit inca , 1 - a nimerit
	glm::mat4 matrix;

};

class Enemy {
public:
	double cooX;
	double cooY;
	double cooZ;
	int line1, line2, line3;
	float angle;
	int hp = 100;
	int animation = 100; //folosit pentru animatia de cadere
	float scale = 0.5;
	glm::mat4 matrix;
};

class Laborator5 : public SimpleScene
{
	public:

		vector<Enemy*> enemyVector;
		vector<Weapon*> weaponVector;
		vector<Tower*> towerVector;
		vector<Weapon*> towerBulletVector;
		Laborator5();
		~Laborator5();
		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
};
