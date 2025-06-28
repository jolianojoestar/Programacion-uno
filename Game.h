#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Afichmation.h"
#include <cstdlib>
#include <string>
#include "Kapas.h"

using namespace std;
using namespace sf;
class Game {
private:
	// Variables sumadas para controlar todo 
	const int ALTURA_BASE = 500; // piso base
	const int DIF_PLATAFORMA = 72;
	const int N_Pisos = 6;
	int alturasPiso[7] = { 500,428,356,284,212,140,68 };
	int activeIndex[N_PISOS]{ 0,0,0,0,0,0 };
	//movimiento
	float velocidadesPorEnemigo[N_PISOS][MAX_ENEMIGOS];
	const float ENEMIGO_SPEED = 100.f;
	//limites de cada piso
	float limiteIzq[N_PISOS], limiteDer[N_PISOS];
	int direccionEnemy[N_PISOS]{ 1,1,1,1,1,1 };
	//Variables y auxiliares
	int ancho = 800;
	int alto = 600;
	float Alturapiso = 500.0f;
	float Extremoizq = 26;
	float ExtremoDer = 774;
	float velocidad = 0.0f;
	const float gravity = 0.5f;
	const float Salto = -10.0f;
	int Reloj = 60;
	//Aux
	int pisoActual = 0;
	int siguienteIndice = 0;
	int contadorCorrect;

	//Banderas
	bool piso1 = false;
	bool piso2 = false;
	bool piso3 = false;
	bool piso4 = false;
	bool piso5 = false;
	bool piso6 = false;
	bool isJumping = false;
	bool Pierde = false;
	bool Gana = false;
	bool Repite = false;
	bool caePorEnemigo = false;
	//SFML
	Vector2f dim_plataforma{ 100,72 };
	RenderWindow* window;
	Sprite pisos[6];
	Sprite Puerta;
	Sprite fondo;
	Texture Texturafondo;
	Texture TexPuerta;
	Font fuente;
	Text Cronometro;
	Text Finaldejuego;
	Afichmation* jugador = nullptr;
	Kapas enemigos;
	Clock relojReal;
	Clock deltaClock;
	Clock frameClock;
	//Puntero a cual enemigo se mueve en cada piso
	Afichmation* enemigoactual[N_PISOS];
public:
	Game();
	~Game();
	void Run();
	void Update();
	void DoEvents();
	void Drawing();
	void Checkcollitions();
	void generarNumero();
	void cargarTodo();
	void Quicksort(int* arreglo, int izquierda, int derecha);
	void generarEnemigo();
	void checkeaPiso();
};