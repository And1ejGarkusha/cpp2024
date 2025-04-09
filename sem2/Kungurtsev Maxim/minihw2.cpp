﻿#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <ctime>
#include <iostream>

// (V) 1. M x N двумерное поле с тайлами(в примере 10 x 10)
// (V) 2. Скрытые тайлы -> открытые с эвентами
// (X) 3. эвенты должны отображаться, + Логика
// (X) 4. Под каждым тайлом с вероятность в 10% есть консервы,
// при вскрытии тайла -> восст. сытость
// (V) 5. Открыть M x N тайлов (все тайлы) для победы
// (V) 6. Экран победы + экран поражения
// (V) 7. За каждое открытие тайла теряем 1 сытость. Сытость равно 0,
// тогда поражение, + отображать сытость
// (V) 8. Начальное значени сытости - 25

#define CELLSIZE_M 10
#define CELLSIZE_N 10
#define CHANCE_FOOD 10
#define CHANCE_ENEMY 10
#define HP_MAX 5
#define ENERGY_MAX 100

#define CELLSIZE_SCREEN 100.f

enum CellType {
    Type_Grass,
    Type_Hill,
    Type_Forest,
    Type_Stone,
    Type_Sand,
    Type_Snow,
    Type_Water,
    Type_End
};
enum TextureType {
    Texture_Grass,
    Texture_Hill,
    Texture_Forest,
    Texture_Stone,
    Texture_Sand,
    Texture_Snow,
    Texture_Water,
    Texture_Hide,
    Texture_Soup,
    Texture_Enemy,
    Texture_End
};

struct Cell {
    bool isHidden;
    bool isFood;
    bool isEnemy;
    CellType cellType;
};

bool isWinOfGame(const Cell cells[CELLSIZE_M][CELLSIZE_N]) {
    bool winCondition = true;
    for (int x = 0; x < CELLSIZE_M; x++) {
        for (int y = 0; y < CELLSIZE_N; y++) {
            if (cells[x][y].isHidden) {
                winCondition = false;
                break;
            }
        }
    }
    return winCondition;
}

bool isLoseOfGame(const int& energy, const int& hp) {
    return (energy <= 0 or hp<=0);
}

void clickTile(int& energy, int& hp,
    const sf::Vector2i& mouseCoord,
    Cell cells[CELLSIZE_M][CELLSIZE_N],
    sf::RectangleShape shapes[CELLSIZE_M][CELLSIZE_N],
    sf::Texture textures[TextureType::Texture_End], const sf::Vector2u& windowSize) {
    const int x = mouseCoord.x < 0 ?
        0 :
        mouseCoord.x > CELLSIZE_SCREEN * CELLSIZE_M ?
        CELLSIZE_M :
        mouseCoord.x/(windowSize.x/20);

    const int y = mouseCoord.y < 0 ?
        0 :
        mouseCoord.y > CELLSIZE_SCREEN * CELLSIZE_N ?
        CELLSIZE_N :
        mouseCoord.y / (windowSize.y / 20);
    if (cells[x][y].isHidden){ 
        cells[x][y].isHidden = false;

        if (cells[x][y].isFood) {
            shapes[x][y].setTexture(&textures[TextureType::Texture_Soup]);
            energy = ENERGY_MAX;
            hp = HP_MAX;
        }
        else if (cells[x][y].isEnemy){
            shapes[x][y].setTexture(&textures[TextureType::Texture_Enemy]);
            hp-=1;
        }
        else{
            shapes[x][y].setTexture(&textures[cells[x][y].cellType]);
            energy--;
        }
    }
    
}

int main() {
    int energy = ENERGY_MAX;
    int hp = HP_MAX;
    srand(time(0));

    Cell cells[CELLSIZE_M][CELLSIZE_N];
    for (int x = 0; x < CELLSIZE_M; x++) {
        for (int y = 0; y < CELLSIZE_N; y++) {
            cells[x][y].isHidden = true;
            cells[x][y].isFood = rand() % 100 < CHANCE_FOOD;
            if (!cells[x][y].isFood) {
                cells[x][y].isEnemy = rand() % 100 < CHANCE_ENEMY;
            }
            cells[x][y].cellType = static_cast<CellType>
                (rand() % (CellType::Type_End - 1));
        }
    }

    sf::RenderWindow window(
        sf::VideoMode({ static_cast <unsigned int> (CELLSIZE_SCREEN * CELLSIZE_M),
            static_cast <unsigned int> (CELLSIZE_SCREEN * CELLSIZE_N) }),
        "NOT MINESWEEPER GAME", sf::State::Windowed);
    sf::Vector2i mouseCoord;
    sf::Font font("Arial.ttf");
    sf::Text textEnergy(font);
    sf::Text RealtextEnergy(font);
    RealtextEnergy.setString("Energy:");
    sf::Text RealtextHp(font);
    RealtextHp.setString("Hp:");
    sf::Text textHp(font);
    sf::Text textCondition(font);
    textEnergy.setCharacterSize(CELLSIZE_SCREEN / 2);
    textEnergy.setFillColor(sf::Color::Red);
    textEnergy.setStyle(sf::Text::Bold | sf::Text::Underlined);
    RealtextEnergy.setCharacterSize(CELLSIZE_SCREEN / 2);
    RealtextEnergy.setFillColor(sf::Color::Red);
    RealtextEnergy.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textHp.setCharacterSize(CELLSIZE_SCREEN / 2);
    textHp.setFillColor(sf::Color::Red);
    textHp.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textCondition.setStyle(sf::Text::Bold | sf::Text::Underlined);
    RealtextHp.setCharacterSize(CELLSIZE_SCREEN / 2);
    RealtextHp.setFillColor(sf::Color::Red);
    RealtextHp.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textCondition.setStyle(sf::Text::Bold | sf::Text::Underlined);
    textCondition.setCharacterSize(CELLSIZE_SCREEN);
    textCondition.setFillColor(sf::Color::Red);
    RealtextEnergy.setPosition({0,10* CELLSIZE_SCREEN * CELLSIZE_N /20});
    textEnergy.setPosition({ 0,11 * CELLSIZE_SCREEN * CELLSIZE_N / 20 });
    RealtextHp.setPosition({ 0,12 * CELLSIZE_SCREEN * CELLSIZE_N / 20 });
    textHp.setPosition({ 0,13 * CELLSIZE_SCREEN * CELLSIZE_N / 20 });
    textCondition.setPosition({ 0,11 * CELLSIZE_SCREEN * CELLSIZE_N / 20 });
    sf::RectangleShape shapes[CELLSIZE_M][CELLSIZE_N];
    sf::Texture textures[TextureType::Texture_End];
    textures[TextureType::Texture_Grass] = sf::Texture("grass.png");
    textures[TextureType::Texture_Hill] = sf::Texture("hill.png");
    textures[TextureType::Texture_Forest] = sf::Texture("forest.png");
    textures[TextureType::Texture_Stone] = sf::Texture("stone.png");
    textures[TextureType::Texture_Sand] = sf::Texture("sand.png");
    textures[TextureType::Texture_Snow] = sf::Texture("snow.png");
    textures[TextureType::Texture_Water] = sf::Texture("water.png");
    textures[TextureType::Texture_Hide] = sf::Texture("hide.png");
    textures[TextureType::Texture_Soup] = sf::Texture("soup.png");
    textures[TextureType::Texture_Enemy] = sf::Texture("enemy.png");
    for (int x = 0; x < CELLSIZE_M; x++) {
        for (int y = 0; y < CELLSIZE_N; y++) {

            // TODO функцию а не напрямую
            if (cells[x][y].isHidden)
                shapes[x][y].setTexture(&textures[TextureType::Texture_Hide]);
            else
                shapes[x][y].setTexture(&textures[cells[x][y].cellType]);

            shapes[x][y].setPosition(
                sf::Vector2f(x * CELLSIZE_SCREEN/2, y * CELLSIZE_SCREEN/2));
            shapes[x][y].setSize({ CELLSIZE_SCREEN/2, CELLSIZE_SCREEN/2 });
        }
    }

    bool mousepressed = false;
    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }

            if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                mousepressed = false;
            }

            if (!mousepressed && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                mousepressed = true;
                mouseCoord = sf::Mouse::getPosition(window);
                sf::Vector2u windowSize= window.getSize();
                // т.к. уменьшил размер клеток надо проверять в нужном ли мышь месте
                if (mouseCoord.x <= windowSize.x / 2 and mouseCoord.y <= windowSize.y / 2) {
                    clickTile(energy,hp, mouseCoord, cells, shapes, textures,windowSize);
                }
                textEnergy.setString(std::to_wstring(energy));
                textHp.setString(std::to_wstring(hp));

                if (isWinOfGame(cells))
                    textCondition.setString("WINNER!!!");
                if (isLoseOfGame(energy,hp))
                    textCondition.setString("LOSER!!!");
            }
        }
        window.clear();
        for (int x = 0; x < CELLSIZE_M; x++) {
            for (int y = 0; y < CELLSIZE_N; y++) {
                window.draw(shapes[x][y]);
            }
        }
        if (textCondition.getString().toAnsiString() == "") {
            window.draw(RealtextEnergy);
            window.draw(RealtextHp);
            window.draw(textEnergy);
            window.draw(textHp);
        }
        window.draw(textCondition);
        window.display();
    }
}
