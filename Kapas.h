#pragma once
#include <iostream>
#include "Afichmation.h"
#include <SFML/Graphics.hpp>
#include "Afichmation.h"

static const int MAX_ENEMIGOS = 4;
static const int N_PISOS = 6;

// Estructura pila (LIFO)
struct Pila {
    Afichmation* datos[MAX_ENEMIGOS];
    int top;
    Pila() : top(-1) {}
    bool push(Afichmation* e) {
        if (top + 1 < MAX_ENEMIGOS) {
            datos[++top] = e;
            return true;
        }
        return false;
    }
    Afichmation* at(int i) const { return datos[i]; }
    int size() const { return top + 1; }
};

// Estructura cola (FIFO)
struct Cola {
    Afichmation* datos[MAX_ENEMIGOS + 1];  // +1 para diferenciar full/empty
    int head, tail;
    Cola() : head(0), tail(0) {}
    bool enqueue(Afichmation* e) {
        int next = (tail + 1) % (MAX_ENEMIGOS + 1);
        if (next != head) {
            datos[tail] = e;
            tail = next;
            return true;
        }
        return false;
    }
    Afichmation* at(int i) const {
        return datos[(head + i) % (MAX_ENEMIGOS + 1)];
    }
    int size() const {
        if (tail >= head) return tail - head;
        return (MAX_ENEMIGOS + 1 - head) + tail;
    }
};

// Clase contenedora de enemigos por piso
class Kapas {
public:
    Kapas() {}
    // Agrega un enemigo al piso
    void addEnemy(int piso, Afichmation* e) {
        int idx = piso - 1;
        if (piso % 2 == 0) {
            colaEnemigos[idx].enqueue(e);
        }
        else {
            pilaEnemigos[idx].push(e);
        }
    }
    // Devuelve puntero al enemigo i-ésimo del piso
    Afichmation* get(int piso, int i) const {
        int idx = piso - 1;
        if (piso % 2 == 0)
            return colaEnemigos[idx].at(i);
        else
            return pilaEnemigos[idx].at(i);
    }
    // Cantidad de enemigos en piso
    int count(int piso) const {
        int idx = piso - 1;
        if (piso % 2 == 0)
            return colaEnemigos[idx].size();
        else
            return pilaEnemigos[idx].size();
    }
    Afichmation* popEnemy(int piso) {
        int idx = piso - 1;
        if (piso % 2 == 0) {
            // COLA: dequeue
            if (colaEnemigos[idx].size() > 0) {
                Afichmation* e = colaEnemigos[idx].at(0);
                colaEnemigos[idx].head = (colaEnemigos[idx].head + 1) % (MAX_ENEMIGOS + 1);
                return e;
            }
        }
        else {
            // PILA: pop
            if (pilaEnemigos[idx].size() > 0) {
                Afichmation* e = pilaEnemigos[idx].datos[pilaEnemigos[idx].top];
                pilaEnemigos[idx].top--;
                return e;
            }
        }
        return nullptr;
    }
    void reagrupar(int piso, Afichmation* e) {
        addEnemy(piso, e);
    }
private:
    Pila pilaEnemigos[N_PISOS];
    Cola colaEnemigos[N_PISOS];
};
