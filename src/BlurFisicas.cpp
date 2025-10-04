#include "BlurFisicas.h"
#include <cmath>

//Convierte grados en radiante
float gradosToRadian(float grados){ return grados*(pi/180.0f);}

//Convierte radiante en grados
float radianToGrados(float radian){	return radian*(180.0f/pi);}

//Interpolacion 0.0 a 1.0
float lerp( float a, float b, float t){ return  a + (b - a) * t; }

//Calcula en angulo
float positionToAngulo(Vec2 punto, Vec2 centro){
	float delta_x = punto.x-centro.x;
	float delta_y = punto.y-centro.y;
	return std::atan2(delta_y, delta_x);//Regresa en radian
	}

//Calcula el radio //ACLARACION: sirve para distancias tambien
float positionToRadio(Vec2 puntoA, Vec2 puntoB){
	float delta_x = puntoA.x-puntoB.x;
	float delta_y = puntoA.y-puntoB.y;
	return std::sqrt( std::pow(delta_x,2) + std::pow(delta_y,2) ); 
	}

//Polar a Cartesiano //ACLARACION: convierte un centro, angulo y radio a coordenadas en {x, y}
Vec2 polarToCartesiano(Vec2 centro,float a, float r){
	float x = std::cos(a)*r;
	float y = std::sin(a)*r;
	x=x+centro.x;
	y=y+centro.y;
	Vec2 result = {x,y};
	return result;
	}

//Genera los impulsos //ACLARION: No usar en solitario, se debe contar como minimo con un dato tipo "estado" para funcionar. //NOTA: Funcionamiento basado en razonamiento rudimentario del autor[no soy bueno con las matematicas hice lo que pude, si hay errores notifiquese porfavor]
Estado aplicarImpulso(Estado estado, Impulso impulso, float masa, float deltaTime) {
	// Convertir impulso en fuerza vectorial
	Vec2 fuerza = {
		impulso.direccion.x * impulso.magnitud,
		impulso.direccion.y * impulso.magnitud
		};
	// Calcular aceleración
	Vec2 aceleracion = {
		fuerza.x / masa,
		fuerza.y / masa
		};
	//actualizar velocidad
	estado.velocidad.x += aceleracion.x * deltaTime;
	estado.velocidad.y += aceleracion.y * deltaTime;
	//actualizar posicion
	estado.posicion.x += estado.velocidad.x * deltaTime;
	estado.posicion.y += estado.velocidad.y * deltaTime;
	return estado; //Regresa un estado[tipo de dato que contiene 2 vec2: posicion{x,y} velocidad{0,0}]
}


//Plantilla de objeto con fisicas //NOTA: Consultarla guia para tener una referencia sobre como los valores afectan a un objeto
ObjetoFisico::ObjetoFisico(float x, float y) {
	estado.posicion = {x, y};
	estado.velocidad = {0, 0};
	impulso.direccion = {0, 0};
	impulso.magnitud = 0.0f;
	masa = 1.0f;
	angulo = 0.0f;
	elasticidad = 0.8f;
	friccion = 0.1f;
	}

void ObjetoFisico::ejercerImpulso(float nuevoAngulo, float magnitud, float deltaTime) {
	angulo = nuevoAngulo;
	impulso.direccion = polarToCartesiano({0, 0}, angulo, 1.0f); // dirección unitaria
	impulso.magnitud = magnitud;
	estado = aplicarImpulso(estado, impulso, masa, deltaTime);
	esCasiCero();
	}

void ObjetoFisico::limitarVelocidad(float velocidadMaxima) {
	float mag = std::sqrt(estado.velocidad.x * estado.velocidad.x + estado.velocidad.y * estado.velocidad.y);
	if (mag > velocidadMaxima) {
		estado.velocidad.x = (estado.velocidad.x / mag) * velocidadMaxima;
		estado.velocidad.y = (estado.velocidad.y / mag) * velocidadMaxima;
		}}

void ObjetoFisico::continuarImpulso(float deltaTime, float lv) {
	impulso.direccion = polarToCartesiano({0, 0}, angulo, 1.0f);
	estado = aplicarImpulso(estado, impulso, masa, deltaTime);
	limitarVelocidad(lv);
	corregirAngulo();
	aplicarFriccion(deltaTime);
	esCasiCero();
	}

void ObjetoFisico::stop() {
	estado.velocidad = {0, 0};
	impulso.magnitud = 0.0f;
	}

float ObjetoFisico::normalizarRadianes(float a) {
	while (a < 0) a += 2 * pi;
	while (a >= 2 * pi) a -= 2 * pi;
	return a;
	}

void ObjetoFisico::mirrorH(float m) {
	estado.velocidad.y *= -elasticidad;
	impulso.direccion.y *= -elasticidad;
	angulo = -angulo; // reflejo vertical
	angulo = normalizarRadianes(angulo);
	estado.posicion.y += polarToCartesiano( {0,0} , angulo, m).y;
	}
		
void ObjetoFisico::mirrorW(float m) {
	estado.velocidad.x *= -elasticidad;
	impulso.direccion.x *= -elasticidad;
	angulo = pi - angulo; // reflejo horizontal
	angulo = normalizarRadianes(angulo);
	estado.posicion.x += polarToCartesiano( {0,0} , angulo, m).x;
	}

void ObjetoFisico::corregirAngulo(){
	angulo = positionToAngulo( estado.velocidad, {0,0} );
	angulo = normalizarRadianes(angulo);
	}

void ObjetoFisico::aplicarFriccion(float deltaTime) {
	float factor = 1.0f - friccion * deltaTime;
	if (factor < 0.0f) factor = 0.0f; // evita que se invierta la velocidad
	estado.velocidad.x *= factor;
	estado.velocidad.y *= factor;
	}

void ObjetoFisico::esCasiCero(){
	if (std::fabs(estado.velocidad.x) < epsilon) estado.velocidad.x = 0.0f;
	if (std::fabs(estado.velocidad.y) < epsilon) estado.velocidad.y = 0.0f;
	if (impulso.magnitud < epsilon) impulso.magnitud = 0.0f;
	}

//ejemplo
//ObjetoFisico circulo(400,300);
