#pragma once

#ifndef _MAP_H_
#define _MAP_H_

#include "../Manager/LogStatus.h"
#include <fstream>
#include <iostream>
#include <algorithm>

class Map {
    private:
        static const int MAP_WIDTH = 28;
        static const int MAP_HEIGHT = 31;
        static const int TOTAL_MAP = 2;
        const std::string mapFile[TOTAL_MAP] = {"Source/Map/map1.txt", "Source/Map/map2.txt"};
        // const std::string mapFile2 = "Source/Map/map2.txt";
        LogStatus* Console = new LogStatus("Map");

        int tile[TOTAL_MAP][MAP_HEIGHT][MAP_WIDTH];
        int dist[TOTAL_MAP][MAP_WIDTH * MAP_HEIGHT][MAP_WIDTH * MAP_HEIGHT][4];
        std::pair<int, int> nextCrossID[TOTAL_MAP][MAP_HEIGHT][MAP_WIDTH][4];
        bool markCross[TOTAL_MAP][MAP_HEIGHT][MAP_WIDTH][4];

        void findingCrossRoad();

        void calculateDistance();

        void NextCrossTileID();
    public:
        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;
        int COINS = 0;
        enum {
            MAP_1 = 0,
            MAP_2 = 1,
        };

        Map();

        ~Map() {
            Console = nullptr;
            delete Console;
        }

        int getTileID(int x, int y, int mapID);

        std::pair<int, int> getnextCrossID(int mapID, int x, int y, int dir);

        bool isWall(int mapID, std::pair<int, int> tileID);

        bool iscrossRoad(int mapID, int y, int x);

        bool canChangeDir(int mapID, int x, int y, int newDir);

        bool besideCrossIsWall(int mapID, std::pair<int, int> Cross, int newDir);

        int eatCoins(int mapID, const int &pacmanTileX, const int &pacmanTileY);

        int getDist(int mapID, std::pair<int, int> start, std::pair<int, int> end, int startDir);

        void reset(int mapID);
        void respawn(int mapID);
};

#endif // _MAP_H_
