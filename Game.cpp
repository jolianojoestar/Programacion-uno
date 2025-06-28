#include "Game.h"

Game::Game() {
    window = new RenderWindow(VideoMode(ancho, alto), "TP 2");
    window->setFramerateLimit(60);
    cargarTodo();
}

Game::~Game() {
    delete window;
}

void Game::Run() {
    while (window->isOpen()) {
        DoEvents();
        Update();
        Drawing();
    }
}

void Game::DoEvents() {
    Event e;
    while (window->pollEvent(e)) {
        if (e.type == Event::Closed)
            window->close();

        if (e.type == Event::KeyPressed) {
            if (e.key.code == Keyboard::A) {
                jugador->Play("Run");
                jugador->FlipX(false);
                jugador->move(-8, 0);
            }
            if (e.key.code == Keyboard::D) {
                jugador->Play("Run");
                jugador->FlipX(true);
                jugador->move(8, 0);
            }
            if (e.key.code == Keyboard::Space && !isJumping) {
                jugador->Play("Jump");
                velocidad = Salto;
                isJumping = true;
            }
        }

        if (e.type == Event::KeyReleased) {
            if (e.key.code == Keyboard::A || e.key.code == Keyboard::D) {
                jugador->Play("Reposo");
            }
        }
    }
}

void Game::Drawing() {
    window->clear(Color(255, 255, 255, 255));
    window->draw(fondo);
    window->draw(Puerta);
    window->draw(*jugador);

    for (int piso = 1; piso <= N_PISOS; ++piso) {
        int cnt = enemigos.count(piso);
        for (int i = 0; i < cnt; ++i)
            window->draw(*enemigos.get(piso, i));
    }

    window->draw(Cronometro);
    window->draw(Finaldejuego);
    window->display();
}

void Game::Checkcollitions() {
    if (jugador->getGlobalBounds().intersects(Puerta.getGlobalBounds()) && (!Pierde)) {
        Gana = true;
    }
    for (int piso = 1; piso <= N_PISOS; ++piso) {
        int cnt = enemigos.count(piso);
        for (int i = 0; i < cnt; ++i) {
            if (jugador->getGlobalBounds().intersects(enemigos.get(piso, i)->getGlobalBounds())) {
                Alturapiso = ALTURA_BASE;
                isJumping = true;
                caePorEnemigo = true;
                velocidad = 0.f;
                jugador->Play("Cae");
                return;
            }
        }
    }
}

void Game::generarNumero() {
}

void Game::cargarTodo() {
    if (!Texturafondo.loadFromFile("assets/fondo_plataformas.png")) {
        cout << "No se carga Textura Fondo";
    }
    if (!TexPuerta.loadFromFile("assets/puerta.png")) {
        cout << "No se carga Textura Objeto";
    }
    if (!fuente.loadFromFile("J:/UNL/Programación uno/TP 2/assets/Ortega.otf")) {
        cout << "No se carga Fuente";
    }

    fondo.setTexture(Texturafondo);
    generarNumero();
    Puerta.setTexture(TexPuerta);
    Puerta.setPosition(280.0f, 25);

    Cronometro.setFont(fuente);
    Cronometro.setCharacterSize(20);
    Cronometro.setFillColor(Color::Green);
    Cronometro.setPosition(ancho - 200, 20);
    Cronometro.setString("Tiempo restante: " + to_string(Reloj));

    Finaldejuego.setFont(fuente);
    Finaldejuego.setCharacterSize(20);
    Finaldejuego.setPosition(250, 200);
    Finaldejuego.setFillColor(Color::Green);

    jugador = new Afichmation("assets/spritesheet.png", true, 104, 125);
    jugador->setScale(0.5, 0.5);
    jugador->Add("Reposo", { 1,2,3,3,2,1 }, 8, false);
    jugador->Add("Jump", { 12,13,14,13,12 }, 8, false);
    jugador->Add("Run", { 3,4,5,6,7,8 }, 8, true);
    jugador->Add("Cae", { 15 }, 8, false);
    jugador->setPosition(400, Alturapiso);
    jugador->Play("Reposo");

    srand(static_cast<unsigned>(time(nullptr)));

    for (int piso = 1; piso <= N_PISOS; ++piso) {
        float xBase = 25.0f;
        float yBase = ALTURA_BASE - 74 * piso;

        for (int i = 0; i < MAX_ENEMIGOS; ++i) {
            int color = rand() % 4;
            Afichmation* e = nullptr;

            switch (color) {
            case 0:
                e = new Afichmation("assets/shell_amarillo.png", true, 30, 24);
                velocidad = 100.f;
                break;
            case 1:
                e = new Afichmation("assets/shell_azul.png", true, 30, 24);
                velocidad = 125.f;
                break;
            case 2:
                e = new Afichmation("assets/shell_rojo.png", true, 30, 24);
                velocidad = 150.f;
                break;
            case 3:
            default:
                e = new Afichmation("assets/shell_verde.png", true, 30, 24);
                velocidad = 175.f;
                break;
            }

            velocidadesPorEnemigo[piso - 1][i] = velocidad;
            e->setScale(1.0f, 1.0f);
            e->Add("Idle", { 0 }, 6, true);
            e->Play("Idle");
            e->setPosition(xBase + i * (30.0f * e->getScale().x), yBase + 10);

            enemigos.addEnemy(piso, e);
        }
    }

    const float SEPARACION_Y = 74.f;
    const float X_LEFT_LIMIT = 50.f;
    const float X_RIGHT_LIMIT = ancho - 50.f;

    for (int piso = 1; piso <= N_PISOS; ++piso) {
        int idx = piso - 1;
        limiteIzq[idx] = X_LEFT_LIMIT;
        limiteDer[idx] = X_RIGHT_LIMIT;
        direccionEnemy[idx] = +1;
        activeIndex[idx] = 0;

        Afichmation* e = enemigos.get(piso, activeIndex[idx]);
        if (e) {
            float yBase = ALTURA_BASE - SEPARACION_Y * piso;
            float halfH = e->getGlobalBounds().height * 0.5f;
            e->setPosition(limiteIzq[idx], yBase);
        }
    }

    relojReal.restart();
}

void Game::Update() {
    if (isJumping) {
        velocidad += gravity;
        jugador->move(0, velocidad);

        if (!caePorEnemigo)
            checkeaPiso();

        if (jugador->getPosition().y >= Alturapiso) {
            jugador->setPosition(jugador->getPosition().x, Alturapiso - 2);

            // Solo frenamos la caída si ya está en el piso base
            if (Alturapiso == ALTURA_BASE || !caePorEnemigo) {
                isJumping = false;
                velocidad = 0.f;
                jugador->Play("Reposo");
                caePorEnemigo = false;
            }
        }
    }
    if(!caePorEnemigo)checkeaPiso();
    

    float dt = deltaClock.restart().asSeconds();

    for (int piso = 1; piso <= N_PISOS; ++piso) {
        int idx = piso - 1;
        int cnt = enemigos.count(piso);
        if (cnt == 0) continue;

        Afichmation* e = enemigos.get(piso, activeIndex[idx]);
        if (!e) continue;
        float vel = velocidadesPorEnemigo[idx][activeIndex[idx]];
        e->move(direccionEnemy[idx] * vel * dt, 0);

        float x = e->getPosition().x;
        if ((direccionEnemy[idx] > 0 && x >= limiteDer[idx]) ||
            (direccionEnemy[idx] < 0 && x <= limiteIzq[idx])) {

            float offset = e->getGlobalBounds().width + 2;
            float yStack = ALTURA_BASE - 74 * piso + 10;

            //Posicionamiento al finalizar recorrido
            if (direccionEnemy[idx] > 0) {
                // Pila: se acumulan a la derecha, último en llegar primero en salir
                e->setPosition(limiteDer[idx] - (cnt - activeIndex[idx] - 1) * offset, yStack);
                activeIndex[idx] = (activeIndex[idx] - 1 + cnt) % cnt;
            }
            else {
                // Cola: se acumulan a la derecha, primero en llegar primero en salir
                e->setPosition(limiteDer[idx] - activeIndex[idx] * offset, yStack);
                activeIndex[idx] = (activeIndex[idx] + 1) % cnt;
            }
            //

            Afichmation* siguiente = enemigos.get(piso, activeIndex[idx]);
            if (siguiente) {
                float yBase = ALTURA_BASE - 74 * piso;
                float halfH = siguiente->getGlobalBounds().height * 0.5f;
                siguiente->setPosition(limiteIzq[idx], yBase + 10);
            }
        }
    }

    jugador->Update();

    for (int piso = 1; piso <= N_PISOS; ++piso) {
        int cnt = enemigos.count(piso);
        for (int i = 0; i < cnt; ++i)
            enemigos.get(piso, i)->Update();
    }

    if (Reloj >= 1) {
        if (relojReal.getElapsedTime().asSeconds() >= 1.0f) {
            Reloj--;
            relojReal.restart();
            Cronometro.setString("Tiempo restante: " + std::to_string(Reloj));

            if (Reloj <= 0) {
                Pierde = true;
            }
        }
    }

    Checkcollitions();

    if (Pierde) {
        Finaldejuego.setFillColor(Color::Red);
        Finaldejuego.setString("Juego finalizado ! Perdiste");
    }
    else if (Gana) {
        Finaldejuego.setString("Juego finalizado ! Ganaste");
    }
}

void Game::checkeaPiso() {
    if (jugador->getPosition().y < Alturapiso - 74) {
        piso1 = true;
        Alturapiso = Alturapiso - 74;
    }
    if (jugador->getPosition().y < Puerta.getPosition().y) {
        Alturapiso = 53;
    }
    if (jugador->getPosition().x < Extremoizq) {
        jugador->setPosition(Extremoizq, jugador->getPosition().y);
    }
    if (jugador->getPosition().x > ExtremoDer) {
        jugador->setPosition(ExtremoDer, jugador->getPosition().y);
    }
}

void Game::generarEnemigo() {
    int color = 1 + rand() % (5 - 1);
    switch (color) {
    case 1:
        break;
    }
}
