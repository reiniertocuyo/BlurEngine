#ifndef BLURFISICAS_H
#define BLURFISICAS_H

constexpr float pi = 3.14159265f; //Constante de Pi

struct Vec2 { //Almacena dos variables de tipo float en {x,y}
	float x;
	float y;
};

struct Impulso { //Almacena la direccion de la fuerza{x,y} y su magnitud
    Vec2 direccion;
    float magnitud;
};

struct Estado {  //Almacena la posicion en {x,y} y su velocidad {0,0}
    Vec2 posicion;
    Vec2 velocidad;
};

float gradosToRadian(float grados);//Convierte grados en radiante
float radianToGrados(float radian);//Convierte radiante en grados
float positionToAngulo(Vec2 punto, Vec2 centro);//Convierte el angulo 
float positionToRadio(Vec2 punto, Vec2 centro);//Sirve para conseguir una distancia tambien
Vec2 polarToCartesiano(Vec2 zero,float a, float r);//Polar a cartesiano
float lerp( float a, float b, float t);//Interpolacion 0.0 a 1.0


Estado aplicarImpulso(Estado estado, Impulso impulso, float masa, float deltaTime); //Genera los impulsos //NOTA: No recuerdo bien su funcionamiento, dedicar mas tiempo a su investigacion

//Plantilla de objeto con fisicas
struct ObjetoFisico {
	Impulso impulso;
	Estado estado; //Contiene la velocidad y la posicion
	float masa;
	float angulo; //Valores en radian
	float elasticidad; //Valores de 0.0 a 1.0
	float friccion; // Valores entre 0.0 y 1.0
	const float epsilon = 0.01f;//Limite antes de convertir a cero
	
	ObjetoFisico(float x, float y);
	
	void ejercerImpulso(float nuevoAngulo, float magnitud, float deltaTime); //ejerce una fuerza
	void limitarVelocidad(float velocidadMaxima); //Limita la velocidad maxima //NOTA: considerar convertir velocidadMaxima en una variable aparte para futuras versiones
	void continuarImpulso(float deltaTime, float limite); // Continua el impulso y añade una velocidad maxima de paso
	void stop(); //Reduce la velocidad y magnitud a cero
	float normalizarRadianes(float a); //Mantiene los radianes dentro de un limite claro
	void mirrorH(); //Invierte direccion vertical
	void mirrorW(); //Invierte direccion Horizontal
	void corregirAngulo(); //Mantiene el angulo actualizado
	void aplicarFriccion(float deltaTime); //Friccion dependiendo de deltatime
	void esCasiCero(); //evita numeros infinitamente enanos

};

#endif


