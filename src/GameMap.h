//
//  GameMap.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 04/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__GameMap__
#define __BytePlay__GameMap__

#include "Interface.h"
#include "Settings.h"

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ostream;
#include <vector>
using std::vector;
using std::pair;
#include <unordered_map>
using std::unordered_map;
    
class GameMap2DArray : public IGameMap {
private:
    uint8_t ** m_map;
    uint64_t m_width;
    uint64_t m_height;
    int32_t m_numberOfExistsAvailable;
    int32_t m_numberOfOtherRoomsAvailable;
    IPathFinder * m_PathFinder;
    void addNeighbours(int32_t startX, int32_t startY);
    vector<pair<int32_t, int32_t>> getOpenSpots(const int32_t & startX, const int32_t & startY) const;
    void fillMapArray();
    void createMapArray(const int32_t & width, const int32_t & height);
public:
    GameMap2DArray(int32_t width = 0, int32_t height = 0);
    ~GameMap2DArray();
    /* create a room, randomly */
    void createRandomMap();
    void display() const;
    uint64_t getWidth() const;
    uint64_t getHeight() const;
    void * getMap() const;
    void setWidth(const uint64_t &);
    void setHeight(const uint64_t &);
    void setMap(void *);
    vector<pair<int64_t, int64_t>> getPath(const PATH_SEARCH = eBREADTH_FIRST_SEARCH) const;
    void * getEntrance() const;
    
};


//
/* MapRoom: define room class to contain room characteristics plus pointers to neighbours
 * Notes: In future this can be extend to include other Room characteritics, like background picture, etc
 */
class MapRoom{
private:
    ROOM_TYPE m_type;
    int64_t m_xPos;
    int64_t m_yPos;
    MapRoom * m_top;
    MapRoom * m_bottom;
    MapRoom * m_left;
    MapRoom * m_right;
public:
    MapRoom(ROOM_TYPE type = eEMPTY, int64_t x = 0, int64_t y = 0, MapRoom * t = nullptr, MapRoom * b = nullptr, MapRoom * l = nullptr, MapRoom * r = nullptr ) : m_type(type), m_xPos(x),m_yPos(y),m_top(t), m_bottom(b), m_left(l), m_right(r){}
    ROOM_TYPE getType() const {return m_type;}
    int64_t getX() const {return m_xPos;}
    int64_t getY() const {return m_yPos;}
    MapRoom * getTop() const {return m_top;}
    MapRoom * getBottom() const {return m_bottom;}
    MapRoom * getRight() const {return m_left;}
    MapRoom * getLeft() const {return m_right;}
    void setTop(MapRoom * top) {m_top = top;}
    void setBottom(MapRoom * bottom) {m_bottom = bottom;}
    void setLeft(MapRoom * left) {m_left = left;}
    void setRight(MapRoom * right) {m_right = right;}
    vector<MapRoom *> getNeighbours() const;
};

/* GameMapAdvanced: define Game's map structure that contains rooms and allows for serializaton and deserialization
 * Notes: The map is a series of rooms, each of which has one entrance and zero to two exits. Each room can either be empty, contain a monster, contain a treasure, or be the exit from the map. One room is marked as the entrance to the map, and this would be where the player starts
 */
class GameMapGraph : public IGameMap {
private:
    MapRoom *m_entrance;
    uint64_t m_width;
    uint64_t m_height;
    int32_t m_numberOfExistsAvailable;
    int32_t m_numberOfOtherRoomsAvailable;
    IPathFinder * m_PathFinder;
    unordered_map<uint64_t, MapRoom *> m_positions; 
    void clearMap();
    void addNeighbours(MapRoom * );
    vector<pair<int64_t, int64_t>> getOpenSpots(uint64_t, uint64_t ) const;
    void addNode(MapRoom * node);
public:
    GameMapGraph(int32_t width = 0, int32_t height = 0);
    ~GameMapGraph();
    /* return path from entrance to exit */
    vector<MapRoom *> getPath(bool optimal = false) const;
    /* create a room, randomly */
    void createRandomMap();
    void display() const;
    uint64_t getWidth() const;
    uint64_t getHeight() const;
    void * getMap() const;
    void setWidth(const uint64_t & );
    void setHeight(const uint64_t & );
    void setMap(void * );
    vector<pair<int64_t, int64_t>> getPath(const PATH_SEARCH = eBREADTH_FIRST_SEARCH) const;
    void * getEntrance() const;
};

#endif /* defined(__BytePlay__GameMap__) */
