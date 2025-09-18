#include <iostream>
#include "raylib.h"
#include <cmath>
using namespace std;

//Variables
const int screenWidth = 800;//fija Largo de la ventana
const int screenHeight = 600;//fija Alto de la ventana 
Vector2 mouse;
int crecen=0;
Vector2 posi = {screenWidth/2, screenHeight/2};


//Aqui inicia el Blur Engine
//Funciones matematicas
const float pi = 3.14159265f;

//Convierte grados en radiante
float gradosToRadian(float grados){
	return grados*(pi/180.0f);
	}

//Convierte radiante en grados, muy util
float radianToGrados(float radian){
	return radian*(180.0f/pi);
	}

//Convierte el angulo 
float positionToAngulo(Vector2 punto, Vector2 centro){
	float delta_x = punto.x-centro.x;
	float delta_y = punto.y-centro.y;
	return atan2(delta_y, delta_x);//Regresa en radian
	}

//Convierte a Radian
float positionToRadio(Vector2 punto, Vector2 centro){
	float delta_x = punto.x-centro.x;
	float delta_y = punto.y-centro.y;
	return sqrt( pow(delta_x,2) + pow(delta_y,2) );
	}

//Polar a cartesiano
Vector2 polarToCartesiano(Vector2 zero,float a, float r){
	float x = cos(a)*r;
	float y = sin(a)*r;
	x=x+zero.x;
	y=y+zero.y;
	Vector2 result = {x,y};
	return result;
	}

//Interpolacion 0.0 a 1.0
float lerp( float a, float b, float t){
	return  a + (b - a) * t;
	}
	
//Aplicar fuerza
struct Impulso{
    Vector2 direccion;
    float magnitud;
};

//Estado del objeto, entender como velocidad o algo asi
struct Estado{
    Vector2 posicion;
    Vector2 velocidad;
};

//Genera los impulsos
Estado aplicarImpulso(Estado estado, Impulso impulso, float masa, float deltaTime) {
	// Convertir impulso en fuerza vectorial
	Vector2 fuerza = {
		impulso.direccion.x * impulso.magnitud,
		impulso.direccion.y * impulso.magnitud
		};
		
	// Calcular aceleración
	Vector2 aceleracion = {
		fuerza.x / masa,
		fuerza.y / masa
		};
	//actualizar velocidad
	estado.velocidad.x += aceleracion.x * deltaTime;
	estado.velocidad.y += aceleracion.y * deltaTime;
	//actualizar posicion
	estado.posicion.x += estado.velocidad.x * deltaTime;
	estado.posicion.y += estado.velocidad.y * deltaTime;
	
	return estado;
}


//Plantilla de objeto con fisicas
struct ObjetoFisico {
	Impulso impulso;
	Estado estado; //Contiene la velocidad y la posicion
	float masa;
	float angulo; //Valores en radian
	float elasticidad; //Valores de 0.0 a 1.0
	float friccion; // Valores entre 0.0 y 1.0
	const float epsilon = 0.01f;//Limite antes de convertir a cero
	
	ObjetoFisico(float x, float y, float m = 1.0f) {
		estado.posicion = {x, y};
		estado.velocidad = {0, 0};
		impulso.direccion = {0, 0};
		impulso.magnitud = 0.0f;
		masa = m;
		angulo = 0.0f;
		elasticidad = 0.8f;//por defecto a 0.8f
		friccion = 0.1f;//por defecto a 0.1f
	}
	
	void ejercerImpulso(float nuevoAngulo, float magnitud, float deltaTime) {
		angulo = nuevoAngulo;
		impulso.direccion = polarToCartesiano({0, 0}, angulo, 1.0f); // dirección unitaria
		impulso.magnitud = magnitud;
		estado = aplicarImpulso(estado, impulso, masa, deltaTime);
		//corregirAngulo();
		esCasiCero();
		}
		
	void limitarVelocidad(float velocidadMaxima) {
		float mag = sqrt(estado.velocidad.x * estado.velocidad.x + estado.velocidad.y * estado.velocidad.y);
		if (mag > velocidadMaxima) {
			estado.velocidad.x = (estado.velocidad.x / mag) * velocidadMaxima;
			estado.velocidad.y = (estado.velocidad.y / mag) * velocidadMaxima;
			}}
		
	void continuarImpulso(float deltaTime, float lv) {
		impulso.direccion = polarToCartesiano({0, 0}, angulo, 1.0f);
		estado = aplicarImpulso(estado, impulso, masa, deltaTime);
		limitarVelocidad(lv);
		corregirAngulo();
		aplicarFriccion(deltaTime);
		esCasiCero();
		}
	
	void stop() {
		estado.velocidad = {0, 0};
		impulso.magnitud = 0.0f;
		}

	float normalizarRadianes(float a) {
		while (a < 0) a += 2 * pi;
		while (a >= 2 * pi) a -= 2 * pi;
		return a;
		}

	void mirrorH() {
		estado.velocidad.y *= -elasticidad;
		impulso.direccion.y *= -elasticidad;
		angulo = -angulo; // reflejo vertical
		angulo = normalizarRadianes(angulo);
		
		estado.posicion.y += polarToCartesiano( {0,0} , angulo, 4).y;
		}
		
	void mirrorW() {
		estado.velocidad.x *= -elasticidad;
		impulso.direccion.x *= -elasticidad;
		angulo = pi - angulo; // reflejo horizontal
		angulo = normalizarRadianes(angulo);
		
		estado.posicion.x += polarToCartesiano( {0,0} , angulo, 4).x;
		}
		
	void corregirAngulo(){
		angulo = positionToAngulo( estado.velocidad, {0,0} );
		angulo = normalizarRadianes(angulo);
		}
		
	void aplicarFriccion(float deltaTime) {
		float factor = 1.0f - friccion * deltaTime;
		if (factor < 0.0f) factor = 0.0f; // evita que se invierta la velocidad
		estado.velocidad.x *= factor;
		estado.velocidad.y *= factor;
		}
		
	void esCasiCero(){
		if (fabs(estado.velocidad.x) < epsilon) estado.velocidad.x = 0.0f;
		if (fabs(estado.velocidad.y) < epsilon) estado.velocidad.y = 0.0f;
		if (impulso.magnitud < epsilon) impulso.magnitud = 0.0f;
		}

};

//ejemplo
//ObjetoFisico circulo(400,300);




//Inicio del programa
int main() {
    InitWindow(screenWidth, screenHeight, "Cuadrado Rojo");//crea la ventana perce
    SetTargetFPS(60);// tasa de fotogramas
    
    ObjetoFisico circulo(400,300);
    circulo.masa=1.0;
    circulo.elasticidad=1;
    circulo.friccion=0;
    

    while (!WindowShouldClose()) {
		float deltaTime = GetFrameTime();
        BeginDrawing();//Inicia el proceso de dibujo en pantalla. Todo lo que dibujes debe estar entre BeginDrawing() y EndDrawing()
        ClearBackground(RAYWHITE);//Limpia el fondo
        mouse = GetMousePosition();//toma la posicion del mouse
        
        
        DrawCircle(circulo.estado.posicion.x,circulo.estado.posicion.y, 16, SKYBLUE);
        Rectangle rect = { circulo.estado.posicion.x, circulo.estado.posicion.y, 32, 4};
		DrawRectanglePro(rect, {0,0},radianToGrados(circulo.angulo), BLACK);
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			float mouseDirection = positionToAngulo( mouse , circulo.estado.posicion);
			circulo.stop();
			circulo.ejercerImpulso( mouseDirection, 4000, deltaTime);
			}
		circulo.ejercerImpulso( gradosToRadian(90), 95.5f, deltaTime);//Debe definirse primero un impulso
		circulo.continuarImpulso(deltaTime, 9000.0f);
		if( circulo.estado.posicion.x > screenWidth || circulo.estado.posicion.x < 0 ){ circulo.mirrorW(); }
		if( circulo.estado.posicion.y > screenHeight || circulo.estado.posicion.y < 0 ){ circulo.mirrorH(); }
		

        EndDrawing();//Finaliza el dibujo y actualiza la ventana con lo que se dibujó. Es como decir “mostrar lo que acabo de pintar”.<>
    }

    CloseWindow();
    return 0;
}
