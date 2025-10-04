#include <iostream>
#include <vector>
#include <cmath>
#include "raylib.h"
#include "../include/BlurFisicas.h"
using namespace std;

//Variables
const int screenWidth = 800;//fija Largo de la ventana
const int screenHeight = 600;//fija Alto de la ventana 
Vec2 mouse; //se guarada la posicion del mouse
Vec2 mortero = {32,512}; //Aqui definiremos desde donde se dispara



//Inicio del programa
int main() {
    InitWindow(screenWidth, screenHeight, "Cuadrado Rojo");//crea la ventana perce
    SetTargetFPS(30);// tasa de fotogramas

//Plantilla de proyectil
ObjetoFisico proyectil(mortero.x,mortero.y);
proyectil.masa=1;
proyectil.elasticidad=0.8;
proyectil.friccion=0.2;
ObjetoFisico simulado = proyectil;

//Arreglo donde se gardaran los disparos
std::vector<ObjetoFisico> disparos;
    

    while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime();
		BeginDrawing();//Inicia el proceso de dibujo en pantalla. Todo lo que dibujes debe estar entre BeginDrawing() y EndDrawing()
		ClearBackground(WHITE);//Limpia el fondo
		mouse.x = GetMousePosition().x;//toma la posicion X del mouse
		mouse.y = GetMousePosition().y;//toma la posicion X del mouse


//Mortero y datos del angulo
float anguloMotero = positionToAngulo(mouse,mortero); //Usamos Blur para calcular el angulo[resultado en Radian]
float angulo = anguloMotero;//Creamos una copia que utilizaremos mas adelante
anguloMotero = radianToGrados(anguloMotero); //Usamos Blur para convertirlo a Grados[valor que utiliza Raylib]
Rectangle rect = { mortero.x, mortero.y, 64, 8}; //Definimos un rectangulo usando Raylib
DrawRectanglePro(rect, {0,4}, anguloMotero, BLACK); //Lo dibujamos y usamos el angulo que calculamos para rotarlo

// Parametros para la simulacion
float pasoTiempo = 0.06f; // paso fijo para la simulación de trayectoria
int pasos = 60; // cantidad de puntos
float g = 90.0f; // “gravedad” en unidades de tu mundo (la Y positiva en Raylib)
float fuerza = positionToRadio(mortero, mouse) * 8.0f; // velocidad inicial escala con distancia

//Actualizando posicion original del proyevtil para coincidir con el mortero
proyectil.estado.posicion.x = polarToCartesiano(mortero,angulo,64).x;
proyectil.estado.posicion.y = polarToCartesiano(mortero,angulo,64).y;

// Preparar proyectil simulado
simulado.angulo = proyectil.angulo;
simulado.impulso = proyectil.impulso;
simulado.estado = proyectil.estado;

// Darle fuerza inicial
simulado.ejercerImpulso( angulo, fuerza, pasoTiempo);

//Dibujar camino de la simulacion
for (int t=0; t<pasos ;t++){
	//Se genera un impulso de gravedad hacia abajo[En Raylib el sistema esta invertido asi que apuntamos arriba]
	simulado.ejercerImpulso( gradosToRadian(90), g, pasoTiempo);
	simulado.continuarImpulso(pasoTiempo, 9000.0f);
	
	if( simulado.estado.posicion.x > screenWidth || simulado.estado.posicion.x < 0 ){ 
		simulado.estado.posicion.x = round( simulado.estado.posicion.x/screenWidth )*screenWidth;
		simulado.mirrorW(0); }
	
	if( simulado.estado.posicion.y > screenHeight || simulado.estado.posicion.y < 0 ){ 
		simulado.estado.posicion.y = round( simulado.estado.posicion.y/screenHeight )*screenHeight;
		simulado.mirrorH(0); }
	
	//Dibuja el punto donde se encuentra la trayectoria simulada
	DrawCircle(simulado.estado.posicion.x, simulado.estado.posicion.y , 4, PINK);
	}

//Creacion de bala real
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
	ObjetoFisico bala( polarToCartesiano(mortero,angulo,64).x , polarToCartesiano(mortero,angulo,64).y );
	bala.masa = proyectil.masa;
	bala.elasticidad = proyectil.elasticidad;
	bala.friccion = proyectil.friccion;
	bala.ejercerImpulso(angulo, fuerza, pasoTiempo);
	disparos.push_back(bala);
}

//Recorremos la lista de las balas dibujarlas y actualizarlas
for (auto& id : disparos){
	// Aplicar gravedad
	id.ejercerImpulso(gradosToRadian(90), g, deltaTime);
	id.continuarImpulso(deltaTime, 9000.0f);
	
	// Rebotes
	if (id.estado.posicion.x < 0 || id.estado.posicion.x > screenWidth) {
		id.estado.posicion.x = round( id.estado.posicion.x/screenWidth )*screenWidth;
		id.mirrorW(0);}
		
	if (id.estado.posicion.y < 0 || id.estado.posicion.y > screenHeight) {
		id.estado.posicion.y = round( id.estado.posicion.y/screenHeight )*screenHeight;
		id.mirrorH(0);}
	
	// Dibujar proyectil
	DrawCircle(id.estado.posicion.x, id.estado.posicion.y, 6, BLACK);
	}


		EndDrawing();//Finaliza el dibujo y actualiza la ventana con lo que se dibujo.
	}
	CloseWindow();
	return 0;
}
