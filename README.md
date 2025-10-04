# Blur Engine (v0.03)
### "Mi laptop es una patata y queria hacer juegos."

## Indice
1. **Introducción**
	- Información General
2. **Tipos de Datos**
	- Vec2
	- Impulso
	- Estado
3. **Funciones**
	- gradosToRadian();
	- radianToGrados();
	- positionToAngulo();
	- positionToRadio();
	- polarToCartesiano();
	- lerp();
	- aplicarImpulso();
4. **ObjetoFisico**
	1. **Variables**
		- impulso;
		- estado;
		- masa;
		- angulo;
		- elasticidad;
		- friccion;
	2. **Funciones**
		- ejercerImpulso();
		- limitarVelocidad();
		- continuarImpulso(); 
		- stop();
		- normalizarRadianes();
		- mirrorH();
		- mirrorW();
		- corregirAngulo();
		- aplicarFriccion();
		- esCasiCero();
5. **Ejemplos**
6. **Integración**
7. **¿Por qué?**


## Introducción
### Información General
**Blur Engine** busca ser un kit de herramientas básicas para el desarrollo de juegos, diseñado de forma modular.
Actualmente incluye un módulo de físicas simple. El apartado gráfico y de colisiones corre de la mano de la librería [Raylib](https://www.raylib.com/), aunque un futuro módulo propio de colisiones esta los planes cercanos.

>El módulo de físicas **no depende de Raylib**, por lo que puede integrarse con otras librerías o entornos, siempre que el usuario se encargue de la parte gráfica y de colisiones por su cuenta.

>La documentación no esta terminada, se ira actualizando progresivamente conforme avanza el desarrollo, sin embargo todas las funciones nombradas en el indice son reales y funcionales.

## Tipo de Datos

- ### Vec2
`Vec2` es una estructura que representa un vector en dos dimensiones. Almacena dos componentes numéricos: `x` y `y`, que se utilizan para describir posiciones, desplazamientos, velocidades, fuerzas, o cualquier sistema que requiera al menos dos valores tipo `float` para operar en el plano 2D.
**Implementación basicá:**
```
// Declaración
Vec2 posicion = {0,0};

// Actualizar información
posicion = {32,64};
posicion.x = 16; // Actualizar unicamente el valor x
posicion.y = 3; // Actualizar unicamente el valor y

// Obtención de Información
Vec2 nuevaPosicion = posicion; // Crear un nuevo Vec2 con exactamente la misma información.
float posicionX = posicion.x; // Obtener unicamente la informacion de x
float posicionY = posicion.y; // Obtener unicamente la informacion de y
```

- ### Impulso
`Impulso` es una estructura que representa una dirección(`Vec2`) y una magnitud(`float`). Podemos entenderla simplemente como una fuerza aplicada en `x` y `y`. `Impulso{ direccion{x,y} , magnitud }`
**Implementación basicá:**
```
// Declaración #1: Podemos declararla en su totalidad
Impulso ejemplo1 = { {1,0} , 1.0f }; //Esto crearia un impulso a la derecha

// Declaración #2: Podemos crear un Vec2 que contenga la direccion y luego cargarla al Impulso
Vec2 direccion = {0,1};
Impulso ejemplo2 = { direccion , 1.0f };
```


## ¿Por qué?
- **¿Por qué esto existe?**
Me interesa mucho la lógica detrás de cómo se hacen las cosas desde pequeño. Entre los 9 y los 15 años estuve bastante interesado específicamente en el desarrollo de videojuegos. Nada muy complicado: Game Jams, muchos juegos sin terminar, prototipos, etc. No aprendí mucho realmente. Eventualmente me centré en otras cosas y me alejé de ese mundo.
Hace un par de días desde que estoy escribiendo esto, decidí retomar el desarrollo de videojuegos. Tuve esto en mente todo el año, vi algunos videos, me puse algo al día y... el panorama es feo, para mi. Tengo una tostadora por PC, así que Unity y Unreal estaban descartados desde el inicio. Game Maker está raro con sus suscripciones y también me anda mal. Godot funciona, pero no me gusta nada. Top peores motores de la galaxia. Considero a sus usuarios gente muy inteligente por saber usarlo.
Así que bueno, decidí programar juegos en C++. Tampoco quería reinventar la rueda: instalar una librería para gráficos y otra para físicas, y enfocarme solo en los juegos. Eso salió mal. No tenía experiencia real con C++, ni con GitHub, ni con todo este flujo de trabajo. Milagrosamente instalé Raylib sin mayores problemas, pero mi pared fue la parte de físicas. Me frustré, decidí hacer un pequeño sistema por mi cuenta y aquí estamos.

- **¿Por qué está en español?**
Se hablar ingles, pero no pienso en ingles y estuve improvisando muchas cosas durante todo el proceso asi que necesitaba claridad para entender cada paso, mientras no encuentre una razón de peso todo se mantendra en español. Y bueno hay mucho contenido en ingles, no creo que haga daño un motor en español... Eh quizas deba cambiar el nombre despues, problemas a futuro!

- **Conclusión**
Solo queria programar un poco y recuperar esta interes del pasado, aunque le estoy agarrando el gusto a esto de hacer herramientas. No se si algo de esto le sea util ha alguien realmente pero yo seguire trabajando en el. Sientanse libres de decirme que está bien, que está mal, que esta feo, etc. Yo solo estoy aprendiendo y estoy seguro que habrá mil formas de hacer esto mejor.
 
 
