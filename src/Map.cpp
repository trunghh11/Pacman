#include "../Source/Map/Map.h"
#include <queue>

typedef std::pair<int, int> II;

bool firstInit = true;
int premanMap[2][31][28];
int color[31][28];

Map::Map() {
    if (firstInit) {
        for (int mapID = 0; mapID <2; mapID++) {
            std::ifstream file(mapFile[mapID].c_str());
            if (file) {
                for (int i = 0; i < MAP_HEIGHT; ++i) {
                    for (int j = 0; j < MAP_WIDTH; ++j) {
                        file >> premanMap[mapID][i][j];
                        if (premanMap[mapID][i][j] > 31 || premanMap[mapID][i][j] < 0) {
                            Console->Status("Wrong Tile!");
                            return;
                        }
                    }
                }

                Console->Status("Map read successfully!");
            }
            else Console->Status("Error reading file!");
            file.close();
        }
    }

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            tile[MAP_1][i][j] = premanMap[0][i][j];
            tile[MAP_2][i][j] = premanMap[1][i][j];
        }
    }

    findingCrossRoad();
    NextCrossTileID();
    calculateDistance();
}

int Map::getTileID(int x, int y, int mapID) {
    return tile[mapID][y][x];
}

std::pair<int, int> Map::getnextCrossID(int mapID, int x, int y, int dir) {
    return nextCrossID[mapID][y][x][dir];
}

bool Map::isWall(int mapID, std::pair<int, int> tileID) {
    if (tileID == II(0, 14) || tileID == II(27, 14)) return false;
    if (tileID.first < 1 || tileID.first > 26) return true;
    if (tileID.second < 1 || tileID.second > 29) return true;
    return tile[mapID][tileID.second][tileID.first] != 26 && tile[mapID][tileID.second][tileID.first] != 30 && tile[mapID][tileID.second][tileID.first] != 27;
}

bool Map::iscrossRoad(int mapID, int x, int y) {
    int cnt = 0;
    if (markCross[mapID][y][x][UP]) ++cnt;
    if (markCross[mapID][y][x][RIGHT]) ++cnt;
    if (markCross[mapID][y][x][DOWN]) ++cnt;
    if (markCross[mapID][y][x][LEFT]) ++cnt;
    if (cnt >= 3) return true;
    if (cnt == 2) {
        if (markCross[mapID][y][x][UP] && markCross[mapID][y][x][DOWN]) return false;
        if (markCross[mapID][y][x][LEFT] && markCross[mapID][y][x][RIGHT]) return false;
        return true;
    }
    return false;
}

bool Map::canChangeDir(int mapID, int x, int y, int newDir) {
    return markCross[mapID][y][x][newDir];
}

bool Map::besideCrossIsWall(int mapID, std::pair<int, int> Cross, int newDir) {
    if (newDir == UP) Cross.second -= 1;
    else if (newDir == DOWN) Cross.second += 1;
    else if (newDir == RIGHT) Cross.first += 1;
    else Cross.first -= 1;
    return isWall(mapID, Cross);
}

void Map::findingCrossRoad() {
    for (int mapID = 0; mapID < 2; mapID++) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                for (int dir = 0; dir < 4; ++dir) markCross[mapID][y][x][dir] = false;

                if (tile[mapID][y][x] != 26 && tile[mapID][y][x] != 27 && tile[mapID][y][x] != 30) continue;

                if (y > 0  && (tile[mapID][y - 1][x] == 26 || tile[mapID][y - 1][x] == 27 || tile[mapID][y - 1][x] == 30)) markCross[mapID][y][x][UP] = true;
                if (x < 27 && (tile[mapID][y][x + 1] == 26 || tile[mapID][y][x + 1] == 27 || tile[mapID][y][x + 1] == 30)) markCross[mapID][y][x][RIGHT] = true;
                if (y < 30 && (tile[mapID][y + 1][x] == 26 || tile[mapID][y + 1][x] == 27 || tile[mapID][y + 1][x] == 30)) markCross[mapID][y][x][DOWN] = true;
                if (x > 0  && (tile[mapID][y][x - 1] == 26 || tile[mapID][y][x - 1] == 27 || tile[mapID][y][x - 1] == 30)) markCross[mapID][y][x][LEFT] = true;
            }
        }
    }
}

void Map::NextCrossTileID() {
    for (int mapID = 0; mapID < 2; mapID ++) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            nextCrossID[mapID][y][0][LEFT] = II(-1, -1);
            for (int x = 1; x < MAP_WIDTH; ++x) {
                nextCrossID[mapID][y][x][LEFT] = II(-1, -1);

                if ( !isWall(mapID, std::pair<int, int> (x, y)) ) {
                    if (nextCrossID[mapID][y][x - 1][LEFT] != II(-1, -1)) nextCrossID[mapID][y][x][LEFT] = nextCrossID[mapID][y][x - 1][LEFT];
                    if (iscrossRoad(mapID, x - 1, y)) nextCrossID[mapID][y][x][LEFT] = II(x - 1, y);
                }
            }

            nextCrossID[mapID][y][MAP_WIDTH - 1][RIGHT] = II(-1, -1);
            for (int x = MAP_WIDTH - 2; x >= 0; --x) {
                nextCrossID[mapID][y][x][RIGHT] = II(-1, -1);

                if ( !isWall(mapID, std::pair<int, int> (x, y)) ) {
                    if (nextCrossID[mapID][y][x + 1][RIGHT] != II(-1, -1)) nextCrossID[mapID][y][x][RIGHT] = nextCrossID[mapID][y][x + 1][RIGHT];
                    if (iscrossRoad(mapID, x + 1, y)) nextCrossID[mapID][y][x][RIGHT] = II(x + 1, y);
                }
            }
        }

        for (int x = 0; x < MAP_WIDTH; ++x) {
            nextCrossID[mapID][0][x][UP] = II(-1, -1);
            for (int y = 1; y < MAP_HEIGHT; ++y) {
                nextCrossID[mapID][y][x][UP] = II(-1, -1);

                if ( !isWall(mapID, std::pair<int, int> (x, y)) ) {
                    if (nextCrossID[mapID][y - 1][x][UP] != II(-1, -1)) nextCrossID[mapID][y][x][UP] = nextCrossID[mapID][y - 1][x][UP];
                    if (iscrossRoad(mapID, x, y - 1)) nextCrossID[mapID][y][x][UP] = II(x, y - 1);
                }
            }

            nextCrossID[mapID][MAP_HEIGHT - 1][x][DOWN] = II(-1, -1);
            for (int y = MAP_HEIGHT - 2; y >= 0; --y) {
                nextCrossID[mapID][y][x][DOWN] = II(-1, -1);

                if ( !isWall(mapID, std::pair<int, int> (x, y)) ) {
                    if (nextCrossID[mapID][y + 1][x][DOWN] != II(-1, -1)) nextCrossID[mapID][y][x][DOWN] = nextCrossID[mapID][y + 1][x][DOWN];
                    if (iscrossRoad(mapID, x, y + 1)) nextCrossID[mapID][y][x][DOWN] = II(x, y + 1);
                }
            }
        }
    }
}

void Map::calculateDistance() {
    for (int mapID = 0; mapID < 2; mapID ++) {
        for (int x = 0; x < MAP_WIDTH; ++x)
            for (int y = 0; y < MAP_HEIGHT; ++y)
                for (int u = 0; u < MAP_WIDTH; ++u)
                    for (int v = 0; v < MAP_HEIGHT; ++v)
                        for (int dir = 0; dir < 4; ++dir)
                            dist[mapID][x * MAP_HEIGHT + y][u * MAP_HEIGHT + v][dir] = -1;
        //std::cout << 1;
        int id = 0;
        int dh[4] = { 0, 1, 0, -1};
        int dc[4] = {-1, 0, 1,  0};
        int dis[MAP_WIDTH * MAP_HEIGHT];
        std::queue< std::pair<int, int> > visitNode;
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                if (isWall(mapID, std::pair<int, int> (x, y))) continue;
                if (y == 14 && (x == 0 || x == 27)) continue;

                for (int startDir = 0; startDir < 4; ++ startDir) {
                    int xn = x + dh[startDir], yn = y + dc[startDir];
                    if (isWall(mapID, std::pair<int, int> (xn, yn))) continue;
                    for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; ++i) dis[i] = -1;
                    ++id;
                    color[yn][xn] = id;
                    dis[xn * MAP_HEIGHT + yn] = 0;
                    visitNode.push(std::pair<int, int> (yn * MAP_WIDTH + xn, startDir));
                    while (!visitNode.empty()) {
                        int curx = visitNode.front().first % MAP_WIDTH,
                            cury = visitNode.front().first / MAP_WIDTH,
                            lasDir = visitNode.front().second;
                        visitNode.pop();
                        if (cury == 14 && (curx == 0 || curx == 27)) continue;
                        for (int dir = 0; dir < 4; ++dir) {
                            int u = curx + dh[dir], v = cury + dc[dir];
                            if (lasDir % 2 == dir % 2 && dir != lasDir) continue;
                            if (isWall(mapID, std::pair<int, int> (u, v))) continue;
                            if (color[v][u] != id) {
                                color[v][u] = id;
                                dis[u * MAP_HEIGHT + v] = dis[curx * MAP_HEIGHT + cury] + 1;
                                visitNode.push(std::pair<int, int> (v * MAP_WIDTH + u, dir));
                            }
                        }
                    }
                    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i)
                        dist[mapID][xn * MAP_HEIGHT + yn][i][startDir] = dis[i];
                }
            }
        }
    }
    std::cout << "Done!" << std::endl;
}

int Map::eatCoins(int mapID, const int &pacmanTileX, const int &pacmanTileY) {
    if (tile[mapID][pacmanTileY][pacmanTileX] == 26) {
        tile[mapID][pacmanTileY][pacmanTileX] = 30;
        return 26;
    }
    if (tile[mapID][pacmanTileY][pacmanTileX] == 27) {
        tile[mapID][pacmanTileY][pacmanTileX] = 30;
        return 27;
    }
    return 0;
}

int Map::getDist(int mapID, std::pair<int, int> start, std::pair<int, int> end, int startDir) {
    if (isWall(mapID, end)) return (start.first - end.first) * (start.first - end.first) + (start.second - end.second) * (start.second - end.second);
    else {
        if (dist[mapID][start.first * MAP_HEIGHT + start.second][end.first * MAP_HEIGHT + end.second][startDir] == -1)
            return (start.first - end.first) * (start.first - end.first) + (start.second - end.second) * (start.second - end.second);
        else return dist[mapID][start.first * MAP_HEIGHT + start.second][end.first * MAP_HEIGHT + end.second][startDir];
    }
}

void Map::reset(int mapID) {
    COINS = 40;
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            tile[mapID][i][j] = premanMap[mapID][i][j];
            if (tile[mapID][i][j] == 26) tile[mapID][i][j] = 30;
        }
    }
}

void Map::respawn(int mapID) {
    if (COINS != 240) {
        srand(time(NULL));
        int i,j;
        for (int k = 0; k < COINS ; k++) {
            i = rand() % MAP_HEIGHT;
            j = rand() % MAP_WIDTH;
            if (tile[mapID][i][j] == 30 && premanMap[mapID][i][j] == 26) {
                tile[mapID][i][j] = 26;
            }
            else k--;
            // std::cout << k << ": " << tile[i][j] << std::endl;
        }
    }
    else {
        for (int i = 0; i < MAP_HEIGHT; ++i) {
            for (int j = 0; j < MAP_WIDTH; ++j) {
                tile[mapID][i][j] = premanMap[mapID][i][j];
            }
        }
    }
    // std::cout << "map respawned!" << std::endl;

}

