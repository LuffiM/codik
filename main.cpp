#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

struct { // массив вершин квадратов, из которых состоит поле
    float Point1;
    float Point2;
    float Point3;
} vert[] = {1,1,0,  1,-1,0,  -1,-1,0,  -1,1,0};

struct {
    float x, y, z;
    float Xrot, Zrot;
} camera = {0,0,2.7, 70, 0}; // параметры камера

void Camera_Apply() // управление камерой
{
    glRotatef(-camera.Xrot, 1, 0, 0);
    glRotatef(-camera.Zrot, 0, 0, 1);
    glTranslatef(-camera.x, -camera.y, -camera.z);
}

void CameraRotation(float xAngle, float zAngle) // поворот камеры вдоль осей x и z
{
    camera.Zrot += zAngle;
    if (camera.Zrot < 0) { camera.Zrot += 360; }
    if (camera.Zrot > 360) { camera.Zrot -= 360; }
    camera.Xrot += xAngle;
    if (camera.Xrot < 0) { camera.Xrot = 0; }
    if (camera.Xrot > 180) { camera.Xrot = 180; }
}

void ShowWorld() // генерация поля
{
    glPushMatrix();
    Camera_Apply();
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vert);
        for (int i = -5; i < 5; i++)
            for (int j = -5; j < 5; j++)
            {
                glPushMatrix();
                    if ((i + j) % 2 == 0) glColor3f(0, 0.5, 0);
                    else glColor3f(1, 1, 1);
                    glTranslatef(i*2, j*2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                glPopMatrix();
            }
        glColor3f(0, 0.5, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void MoveCamera() // передвижение по кнопкам
{
    float speed = 0;
    float ugol = -camera.Zrot / 180 * M_PI;
    if (Keyboard::isKeyPressed(Keyboard::W)) { speed = 0.002; camera.x += sin(ugol) * speed; camera.y += cos(ugol) * speed; }
    if (Keyboard::isKeyPressed(Keyboard::S)) { speed = +0.002; camera.x -= sin(ugol) * speed; camera.y -= cos(ugol) * speed; }
    if (Keyboard::isKeyPressed(Keyboard::A)) { speed = 0.002; ugol += M_PI * 0.5; camera.x -= sin(ugol) * speed; camera.y -= cos(ugol) * speed; }
    if (Keyboard::isKeyPressed(Keyboard::D)) { speed = 0.002; ugol += M_PI * 0.5; camera.x += sin(ugol) * speed; camera.y += cos(ugol) * speed; }
    if (Keyboard::isKeyPressed(Keyboard::Escape)) { exit(0); }
}

int main()
{
    int i = 1000;
    int j = 720;
	sf::RenderWindow window(sf::VideoMode(i, j), "Hello World");

    window.setActive(true);

    glFrustum(-1,1, -1,1, 2,80);
    window.setMouseCursorVisible(false); // убирает курсор мыши

    bool running = true;
    Vector2i cur;
    Vector2i Pos = { i / 2, j / 2 }; // центр окна
    Mouse::setPosition(Pos, window); // устанавливаем курсор мыши в центр
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glPushMatrix();
            ShowWorld();
            MoveCamera();
            cur = Mouse::getPosition(window);
            CameraRotation((Pos.y - cur.y) / 2.5, (Pos.x - cur.x) / 2.5); // управление мышью
            Mouse::setPosition(Pos, window);       
        glPopMatrix();

		window.display();
	}
	return 0;
}

