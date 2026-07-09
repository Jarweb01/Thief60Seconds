#include "GameMap.h"
#include <QVariantMap>

constexpr int operator"" _grid(unsigned long long int cells) {
    return cells * 50;
}

GameMap::GameMap(int level, QObject *parent): QObject(parent) {
    if (level == 1) {
        m_wallsLayout = {
            // Внешние стены здания (комната)
            { 4_grid, 4_grid,  3_grid, 30 },     // Верх
            { 8_grid, 4_grid,  4_grid, 30 },     // Верх
            { 4_grid, 4_grid,  30,     7_grid }, // Лево
            { 12_grid, 4_grid, 30,     7_grid + 30}, // Право
            { 4_grid, 11_grid, 8_grid, 30 },     // Низ

            // ВНУТРЕННИЕ ПЕРЕГОРОДКИ (Добавили одной строчкой!)
            { 8_grid, 8_grid,  4_grid,    15 }  // Внутренняя стена посреди комнаты
        };
    } else {
        m_wallsLayout = {
            // Внешний периметр здания
            { 2_grid, 2_grid,  12_grid, 30 },        // Верхняя внешняя стена
            { 2_grid, 2_grid,  30,      10_grid },     // Левая внешняя стена
            { 14_grid, 2_grid, 30,      10_grid + 30}, // Правая внешняя стена

            // ИСПРАВЛЕНО: Возвращаем проход по центру нижней стены (на клетках 7 и 8)
            { 2_grid, 12_grid, 5_grid,  30 },        // Левая часть нижней стены
            { 9_grid, 12_grid, 5_grid,  30 },        // Правая часть нижней стены

            // Внутренние горизонтальные перегородки (выровненные по сетке)
            { 2_grid,  7_grid, 3_grid, 30 }, // Левый кусок плотно прилегает к стене
            { 6_grid,  7_grid, 4_grid, 30 }, // Центральный кусок
            { 11_grid, 7_grid, 3_grid, 30 }, // Правый кусок

            // Внутренняя вертикальная стена
            { 8_grid, 2_grid,  30,     5_grid }
        };
    }
}

QVariantList GameMap::walls() const {
    QVariantList list;
    for (const auto& wall: m_wallsLayout) {
        QVariantMap map;
        map["x"] = wall.x;
        map["y"] = wall.y;
        map["w"] = wall.w;
        map["h"] = wall.h;
        list.append(map);
    }
    return list;
}

// МАТЕМАТИКА КОЛЛИЗИЙ (AABB столкновение прямоугольников)
bool GameMap::isWallAt(int nextX, int nextY, int objSize) const {
    for (const auto& wall : m_wallsLayout) {
        if (nextX < wall.x + wall.w &&
            nextX + objSize > wall.x &&
            nextY < wall.y + wall.h &&
            nextY + objSize > wall.y) {
            return true; // Столкновение со стеной обнаружено
        }
    }
    return false;
}
