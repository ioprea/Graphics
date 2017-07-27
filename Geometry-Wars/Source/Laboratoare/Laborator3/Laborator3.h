#pragma once
#include <vector>
#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

using namespace std;

//Clasa vieti nava utilizator
class Life {
public:
		int cooX;
		int cooY;
		glm::mat3 matrix;
};

//Clasa proiectile nava
class ProjectileX {
	public:
		float cooX;
		float cooY;
		glm::mat3 matrix;
		float angle;

};

//Clasa nava inamic
class enemyShip {
public:
	int type; // 0 - blue || 1 - yellow
	int culoare; // 2 yellow || 1 - red
	float cooX;
	float cooY;
	glm::mat3 matrix;
	float angle; //unghiul pentru deplasarea spre nava utilizator
	float angle2; // unghiul folosit pentru orientarea corecta cu fata spre nava utilizator
	int speed = 3;
	float scale = 45;

};


class Laborator3 : public SimpleScene
{
	public:
		vector<ProjectileX*> vP;
		vector<enemyShip*> eS;
		vector<Life*> lF;
		Mesh * CreateMesh(const char * name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
		Laborator3();
		~Laborator3();

		
		void Init() override;


	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

	protected:
		glm::mat3 modelMatrix;
		glm::mat3 modelMatrix1[1000];
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep;
};
