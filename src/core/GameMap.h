#pragma once
#include <QObject>
#include <QVariantList>
#include <vector>

struct WallData {
    int x;
    int y;
    int w;
    int h;
};

class GameMap: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList walls READ walls CONSTANT)
    Q_PROPERTY(int gridSize READ gridSize CONSTANT)
    Q_PROPERTY(int mapSize READ mapSize CONSTANT)

public:
    explicit GameMap(int level = 1, QObject* parent = nullptr);

    QVariantList walls() const;
    int gridSize() const { return m_gridSize; }
    int mapSize() const { return m_mapSize; }

    bool isWallAt(int nextX, int nextY, int objSize) const;

private:
    const int m_gridSize = 50;
    const int m_mapSize = m_gridSize * 16;
    std::vector<WallData> m_wallsLayout;
};
