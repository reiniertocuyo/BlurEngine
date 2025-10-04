#include <iostream>
#include "raylib.h"
#include "../include/BlurFisicas.h"
using namespace std;

//Variables
const int screenWidth = 800;//fija Largo de la ventana
const int screenHeight = 600;//fija Alto de la ventana 
Vec2 mouse;

//Inicio del programa
int main() {
    InitWindow(screenWidth, screenHeight, "Cuadrado Rojo");//crea la ventana perce
    SetTargetFPS(60);// tasa de fotogramas
    
    ObjetoFisico circulo(400,300);
    circulo.masa=1.0;
    circulo.elasticidad=0.8;
    circulo.friccion=0.1;
    

    while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime();
        BeginDrawing();//Inicia el proceso de dibujo en pantalla. Todo lo que dibujes debe estar entre BeginDrawing() y EndDrawing()
        ClearBackground(RAYWHITE);//Limpia el fondo
        mouse.x = GetMousePosition().x;//toma la posicion X del mouse
        mouse.y = GetMousePosition().y;//toma la posicion X del mouse
        
        
		DrawCircle(circulo.estado.posicion.x,circulo.estado.posicion.y, 16, SKYBLUE);
		Rectangle rect = { circulo.estado.posicion.x, circulo.estado.posicion.y, 32, 4};
		DrawRectanglePro(rect, {0,0},radianToGrados(circulo.angulo), BLACK);

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			float mouseDirection = positionToAngulo( mouse , circulo.estado.posicion);
			circulo.ejercerImpulso( mouseDirection, 4000, deltaTime);
			}

		circulo.ejercerImpulso( gradosToRadian(90), 95.5f, deltaTime);//Debe definirse primero un impulso
		circulo.continuarImpulso(deltaTime, 9000.0f);
		if( circulo.estado.posicion.x > screenWidth || circulo.estado.posicion.x < 0 ){ circulo.mirrorW(); }
		if( circulo.estado.posicion.y > screenHeight || circulo.estado.posicion.y < 0 ){ circulo.mirrorH(); }
		

        EndDrawing();//Finaliza el dibujo y actualiza la ventana con lo que se dibujo.
    }

    CloseWindow();
    return 0;
}
