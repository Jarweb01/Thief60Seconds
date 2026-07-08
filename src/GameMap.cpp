#include "GameMap.h"
#include <QVariantMap>

constexpr int operator"" _grid(unsigned long long int cells) {
    return cells * 50;
}

GameMap::GameMap(QObject *parent): QObject(parent) {
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
