#include "Laborator3.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include <string>


int shootProjectile = 0;
float fixedAngle;
#define PI 3.14159265
int lives = 3;
float coordX = 640;
float coordY = 360;
int spawnTime = 100;
int i = 1;
float angle = 0;

Mesh* Laborator3::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
	// ========================================================================
	// This is section is strongly linked to how the GPU Shader program treats data
	// It will be learned later, when GLSL shaders will introduces
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	cameraInput->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-0.5, -0.5, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(-0.5, 0.5, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0.5, 0.5, 0), glm::vec3(1, 1, 1)),
			VertexFormat(glm::vec3(0.5, -0.5, 0), glm::vec3(1, 1, 1))
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,	// indices for first triangle
			2, 3, 4,	// indices for second triangle
						// TODO: Complete indices data
		};

		vector<VertexFormat> verProjectile
		{
			VertexFormat(glm::vec3(-0.1, -0.5, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0.1, -0.5, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(-0.1, 0.5, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0.1, 0.5, 0), glm::vec3(1, 1, 1)),

			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indProjectile =
		{
			0, 1, 2,	// indices for first triangle
			2, 1, 3,	// indices for second triangle
						// TODO: Complete indices data
		};

	

		vector<VertexFormat> verEnemy
		{
			VertexFormat(glm::vec3(-0.5, -0.5, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-0.5, 0.5, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0.5, 0.5, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0.5, -0.5, 0), glm::vec3(1, 0, 0))
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indEnemy =
		{
			0, 1, 2,	// indices for first triangle
			2, 3, 4,
			// TODO: Complete indices data
		};

		vector<VertexFormat> verEnemy2
		{
			VertexFormat(glm::vec3(-0.5, -0.5, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(-0.5, 0.5, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0.5, 0.5, 0), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3(0.5, -0.5, 0), glm::vec3(1, 1, 0))
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indEnemy2 =
		{
			0, 1, 2,	// indices for first triangle
			2, 3, 4,
			// TODO: Complete indices data
		};


		vector<VertexFormat> verEnemy3
		{
			VertexFormat(glm::vec3(-0.5, -0.5, 0), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(-0.5, 0.5, 0), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(0.5, 0.5, 0), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(0.5, -0.5, 0), glm::vec3(0, 0, 1))
			// TODO: Complete the information for the cube
		};

		vector<unsigned short> indLife =
		{
			0, 1, 2,	// indices for first triangle
			2, 0 ,3,
			4, 7 ,6,
			5, 6, 7,

			// TODO: Complete indices data
		};


		vector<VertexFormat> verLife
		{
			VertexFormat(glm::vec3(-1, -0.25, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-1, 0.25, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1, 0.25, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(1, -0.25, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-0.5, 1, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0.5, 1, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(-0.5, -1, 0), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3(0.5, -1, 0), glm::vec3(1, 0, 0))
			// TODO: Complete the information for the cube
		};


		vector<unsigned short> indEnemy3 =
		{
			0, 1, 2,	// indices for first triangle
			2, 3, 4,
			// TODO: Complete indices data
		};


		Mesh *ship = CreateMesh("ship", vertices, indices);
		Mesh *projectile = CreateMesh("projectile", verProjectile, indProjectile);
		Mesh *enemy = CreateMesh("enemy", verEnemy, indEnemy);
		Mesh *enemyYellow = CreateMesh("enemyYellow", verEnemy2, indEnemy2);
		Mesh *enemyBlue = CreateMesh("enemyBlue", verEnemy3, indEnemy3);
		Mesh *cross = CreateMesh("cross", verLife, indLife);

	}

	//Adaugarea celor 3 vieti in vectorul de vieti corespunzator
	Life* p1 = new Life();
	p1->cooX = 1240;
	p1->cooY = 680;

	Life* p2 = new Life();
	p2->cooX = 1180;
	p2->cooY = 680;

	Life* p3 = new Life();
	p3->cooX = 1120;
	p3->cooY = 680;

	lF.push_back(p1);
	lF.push_back(p2);
	lF.push_back(p3);


}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}



void Laborator3::Update(float deltaTimeSeconds)
{
	// Creare nava utilizator

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(640, 360);
	modelMatrix *= Transform2D::Translate(translateX, translateY);

	modelMatrix *= Transform2D::Scale(45, 45);
	modelMatrix *= Transform2D::Rotate(270 * PI / 180);
	modelMatrix *= Transform2D::Rotate(angle);

	// Randare nava utilizator

	RenderMesh2D(meshes["ship"], shaders["VertexColor"], modelMatrix);
	
	// Pozitionarea si randarea vietilor in coltul din dreapta sus
	for each(Life* lives in lF) {
		lives->matrix = glm::mat3(1);
		lives->matrix *= Transform2D::Translate(lives->cooX, lives->cooY);
		lives->matrix *= Transform2D::Rotate( PI );
		lives->matrix *= Transform2D::Scale(20, 20);
		RenderMesh2D(meshes["cross"], shaders["VertexColor"], lives->matrix);
	}

	//Crearea proiectilelor
	for each(ProjectileX* proj in vP) {
		proj->matrix = glm::mat3(1);

		proj->matrix *= Transform2D::Translate(proj->cooX, proj->cooY);
		proj->matrix *= Transform2D::Scale(12, 12);
		proj->matrix *= Transform2D::Rotate(90 * PI / 180);
		proj->matrix *= Transform2D::Rotate(proj->angle);
		proj->cooX = proj->cooX + cos(proj->angle) * 30;
		proj->cooY = proj->cooY + sin(proj->angle) * 30;
		RenderMesh2D(meshes["projectile"], shaders["VertexColor"], proj->matrix);
			
	}
	
	//Aparitia navelor inamice la un inverval de timp (2s - 0.5s)
	if (i % spawnTime == 0) {
		enemyShip* eShip = new enemyShip();
		eShip->angle = angle + RandomFloat(0, 2 * PI);
		eShip->cooX = coordX + cos(eShip->angle) * 450;
		eShip->cooY = coordY - sin(eShip->angle) * 300;
		eShip->type = (int)rand() % 2;
		eShip->culoare = 2;
		eS.push_back(eShip);

	}

	//Pozitionare si randare inamici + coliziune inamic - proiectil , inamic - nava utilizator
	for (vector <enemyShip*>::iterator iter = eS.begin(); iter != eS.end(); ) {

				int check = 0;
				int check1 = 0;
				enemyShip* it = *iter;

				if (it->type == 1 && it->culoare == 1) it->speed = 6;
				it->matrix = glm::mat3(1);
				it->angle2 = atan2(coordY - (it->cooY) ,coordX - (it->cooX));
				it->matrix *= Transform2D::Translate(it->cooX, it->cooY);
				it->cooX = it->cooX + cos(it->angle2) * it->speed;
				it->cooY = it->cooY + sin(it->angle2) * it->speed;
				
				it->matrix *= Transform2D::Rotate(it->angle2+PI/2);
				if (it->type == 0) {
					it->matrix *= Transform2D::Scale(45, 45);
					RenderMesh2D(meshes["enemyBlue"], shaders["VertexColor"], it->matrix);
				}
				else {
					if (it->culoare == 2) {
						it->matrix *= Transform2D::Scale(45, 45);
						RenderMesh2D(meshes["enemyYellow"], shaders["VertexColor"], it->matrix);
					}
					else {
						if (it->scale > 30) it->scale -= 0.4;
						it->matrix *= Transform2D::Scale(it->scale, it->scale);
						RenderMesh2D(meshes["enemy"], shaders["VertexColor"], it->matrix);
					}
				}
				for (vector <ProjectileX*>::iterator iter2 = vP.begin(); iter2 != vP.end();) {
					ProjectileX* it1 =*iter2;
					if ((it->cooX - it1->cooX < 25) && (it->cooX - it1->cooX > -25) && (it->cooY - it1->cooY < 25) && (it->cooY - it1->cooY > -25)) {
						iter2 = vP.erase(iter2);
						check = 1;
						break;
					}
					else { iter2++; }
				}
	
						if ((it->cooX - coordX < 35) && (it->cooX - coordX > -35) && (it->cooY - coordY < 35) && (it->cooY - coordY > -35)) {
							if (lives == 1) {
								lF.erase(lF.end() - 1);
								//Animatie culoare ecran la finalul jocului
								for (float i = 0; i < 1; i = i + 0.0125) {
									glClearColor(i, 0, 0, 1);
									glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
									Sleep(25);
								
									modelMatrix = glm::mat3(1);
									modelMatrix *= Transform2D::Translate(coordX, coordY);
									modelMatrix *= Transform2D::Scale(45, 45);
									modelMatrix *= Transform2D::Rotate(270 * PI / 180);
									modelMatrix *= Transform2D::Rotate(angle);
									RenderMesh2D(meshes["ship"], shaders["VertexColor"], modelMatrix);
									for each(ProjectileX* proj in vP) {
										proj->matrix = glm::mat3(1);

										proj->matrix *= Transform2D::Translate(proj->cooX, proj->cooY);
										proj->matrix *= Transform2D::Scale(12, 12);
										proj->matrix *= Transform2D::Rotate(90 * PI / 180);
										proj->matrix *= Transform2D::Rotate(proj->angle);
										RenderMesh2D(meshes["projectile"], shaders["VertexColor"], proj->matrix);

									}
									for each(enemyShip* it in eS) {
										it->matrix = glm::mat3(1);
										it->angle2 = atan2(coordY - (it->cooY), coordX - (it->cooX));
										it->matrix *= Transform2D::Translate(it->cooX, it->cooY);
										it->matrix *= Transform2D::Rotate(it->angle2 + PI / 2);
										if (it->type == 0) {
											it->matrix *= Transform2D::Scale(45, 45);
											RenderMesh2D(meshes["enemyBlue"], shaders["VertexColor"], it->matrix);
										}
										else {
											if (it->culoare == 2) {
												it->matrix *= Transform2D::Scale(45, 45);
												RenderMesh2D(meshes["enemyYellow"], shaders["VertexColor"], it->matrix);
											}
											else {
												if (it->scale > 30) it->scale -= 0.4;
												it->matrix *= Transform2D::Scale(it->scale, it->scale);
												RenderMesh2D(meshes["enemy"], shaders["VertexColor"], it->matrix);
											}
										}
									}
									window->SwapBuffers();
									}
									Sleep(2000);
									exit(1);
								}

							else { lives--; lF.erase(lF.end() - 1); check1 = 1;	}
						}

				//folosesc check si check1 pentru a verifica coliziunea dintre proiectil - inamic si respectiv inamic - nava utilizator
				if (check == 0 && check1==0) {
					iter++;
				}
				else if (check1 == 1) { iter = eS.erase(iter); }
				else if (check == 1 && it->type == 1 && it->culoare == 2) { it->culoare = 1; }
				else { iter = eS.erase(iter); }
				

				
		}

		i++;

		if (i == 300) {
			i = 0;
			if (spawnTime > 25) spawnTime-=25;
		}
				
}

void Laborator3::FrameEnd()
{

	
}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	//miscarea navei prin taste
	if (window->KeyHold(GLFW_KEY_S)) {
		coordY -= 300 * deltaTime;
		translateY -= 300 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		coordY += 300 * deltaTime;
		translateY += 300 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		coordX += 300 * deltaTime;
		translateX += 300 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		coordX -= 300 * deltaTime;
		translateX -= 300 * deltaTime;
	}
}

void Laborator3::OnKeyPress(int key, int mods)
{

	// add key press event
};

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	//setarea unghiului de rotire la miscarea mouse-ului
	angle = atan2(abs(720 - mouseY) - coordY, mouseX - coordX);
};

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	//In cazul apasarii Left-click setarea coordonatelor si unghiului proiectilului
	if (button == 1) {
		ProjectileX* p = new ProjectileX();
		p->cooX = coordX;
		p->cooY = coordY;
		p->angle = angle;
		vP.push_back(p);
		shootProjectile = 1;

	}

	// add mouse button press event
};

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

