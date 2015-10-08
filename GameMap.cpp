//
//  GameMap.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 04/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "GameMap.h"
#include "PathFinder.h"
#include <cstdlib>
#include <ctime>
#include "macrologger.h"
#include <iostream>
using std::cout;
using std::endl;

GameMap2DArray::GameMap2DArray(int32_t width, int32_t height)
: IGameMap(e2D_ARRAY), m_width(width), m_height(height), m_numberOfExistsAvailable(0), m_numberOfOtherRoomsAvailable(0){
    if (width > 0 && height > 0){
        createMapArray(width, height);
    }
    else {
        m_map = nullptr;
    }
    m_PathFinder = new PathFinder();
}

GameMap2DArray::~GameMap2DArray(){
    if (m_map != nullptr){
        for(uint32_t i = 0; i < m_height; ++i){
            delete[] m_map[i];
        }
        delete m_map;
        m_map = nullptr;
    }
    delete m_PathFinder;
}

void GameMap2DArray::createMapArray(const int32_t & width, const int32_t & height){
    if (width > 0 && height > 0){
        m_map = new uint8_t*[height];
        for (int i = 0; i < height; ++i){
            m_map[i] = new uint8_t[width];
        }
        m_width = width;
        m_height = height;
    }
    fillMapArray();
}

void GameMap2DArray::fillMapArray(){
    for (int i = 0; i < m_height ; ++i){
        for (int j = 0; j < m_width; ++j){
            m_map[i][j] = eUNDEFINED;
        }
    }
}

void GameMap2DArray::createRandomMap(){
    //need one entrance, 0 to 2 exits, and set of monsters, treasure and empty rooms
    if (m_map != nullptr) delete m_map;
    
    createMapArray(SIMPLE_MAP_WIDTH, SIMPLE_MAP_HEIGHT);
    
    srand((uint32_t)time(NULL)*2);
    
    //determine number of rooms
    m_numberOfExistsAvailable = rand()%(MAX_NUMBER_OF_EXIT_ROOMS+1);
    m_numberOfOtherRoomsAvailable = rand()%MAX_NUMBER_OF_OTHER_ROOMS;
    
    LOG_DEBUG("number of exists: %d ", (int) m_numberOfExistsAvailable);
    LOG_DEBUG("number of rooms: %d ", (int) m_numberOfOtherRoomsAvailable);
    
    //determine entrance location
    int entranceX = rand()%SIMPLE_MAP_WIDTH;
    int entranceY = rand()%SIMPLE_MAP_HEIGHT;
    
    if (m_map == nullptr){
        cout << "m_map == nullptr " << endl;
        return;
    }
    m_map[entranceY][entranceX] = eENTRANCE;
    
    //add neighbours recursively
    addNeighbours(entranceX,entranceY);
}

vector<pair<int32_t, int32_t>> GameMap2DArray::getOpenSpots(const int32_t & startX, const int32_t & startY) const {
    vector<pair<int32_t, int32_t>> slots;
    
    if (startX + 1 < m_width && m_map[startY][startX + 1] == eUNDEFINED){
        slots.push_back(pair<int32_t, int32_t>(startX + 1, startY));
    }
    if (startX - 1 >= 0 && m_map[startY][startX - 1] == eUNDEFINED){
        slots.push_back(pair<int32_t, int32_t>(startX - 1, startY));
    }
    if (startY + 1 < m_height && m_map[startY + 1][startX] == eUNDEFINED){
        slots.push_back(pair<int32_t, int32_t>(startX, startY + 1));
    }
    if (startY - 1 >= 0 && m_map[startY - 1][startX] == eUNDEFINED){
        slots.push_back(pair<int32_t, int32_t>(startX, startY - 1));
    }
    
    return slots;
}

void GameMap2DArray::addNeighbours(int32_t startX, int32_t startY){
    if (m_numberOfOtherRoomsAvailable == 0 && m_numberOfExistsAvailable == 0) return;
    
    //count available neighbours
    vector<pair<int32_t, int32_t>> availableNeighbours = getOpenSpots(startX, startY);
    
    if (availableNeighbours.empty()) return;
    
    //get number of neighbours
    int numberOfNeighbours = rand()%availableNeighbours.size()+1;
    numberOfNeighbours = std::min(numberOfNeighbours, m_numberOfExistsAvailable+m_numberOfOtherRoomsAvailable);
    
    for (int i = 0; i < numberOfNeighbours; ++i){
        //for now just do equal selection of rooms
        ROOM_TYPE type;
        if (m_numberOfOtherRoomsAvailable == 0){
            type = eEXIT;
        }
        else {
            do{
                type = static_cast<ROOM_TYPE>(rand()%eTOTAL);
            }while (type == eENTRANCE || type == eUNDEFINED || (type == eEXIT && m_numberOfExistsAvailable==0));
            
        }
        
        m_map[availableNeighbours[i].second][availableNeighbours[i].first] = type;
        
        if (type == eEXIT && m_numberOfExistsAvailable > 0) --m_numberOfExistsAvailable;
        else if (m_numberOfOtherRoomsAvailable > 0) --m_numberOfOtherRoomsAvailable;
    }
    for (int i = 0; i < numberOfNeighbours; ++i){
        addNeighbours(availableNeighbours[i].first, availableNeighbours[i].second);
    }
    
}

void GameMap2DArray::display() const{
    if (m_map == nullptr) return;
    for (int i = 0; i < m_height; ++i){
        for (int j = 0; j < m_width; ++j){
            cout << (int)m_map[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

uint64_t GameMap2DArray::getWidth() const{
    return m_width;
}

uint64_t GameMap2DArray::getHeight() const{
    return m_height;
}

void * GameMap2DArray::getMap() const{
    return (void *)m_map;
}

void GameMap2DArray::setWidth(const uint64_t & width){
    m_width = width;
}

void GameMap2DArray::setHeight(const uint64_t & height){
    m_height = height;
}

void GameMap2DArray::setMap(void * newMap){
    uint8_t ** nMap = (uint8_t **) newMap;
    if (nMap == nullptr) return;
    if (m_map != nullptr) delete m_map;
    m_map = nMap;
}

vector<pair<int64_t, int64_t>> GameMap2DArray::getPath(const PATH_SEARCH type) const{
    switch ((int)type){
        case eDEPTH_FIRST_SEARCH:
            return m_PathFinder->getPathDepthFS(*this);
        case eBREADTH_FIRST_SEARCH:
            LOG_ERROR("Not yet implemented");
       default:
            LOG_ERROR("Not supported");
            break;
    }
    return vector<pair<int64_t, int64_t>>();
}

void * GameMap2DArray::getEntrance() const{
    pair<int64_t, int64_t> * coords = new pair<int64_t, int64_t>(-1, -1);
    
    if (m_map == nullptr || m_height == 0 || m_width == 0) return coords;
    
    for (int i =  0; i < m_height; ++i){
        for (int j = 0; j < m_width; ++j){
            if (m_map[i][j] == (int)eENTRANCE){
                coords->first = j;
                coords->second = i;
                return (void *)coords;
            }
        }
    }
    
    return (void *)coords;
}

GameMapGraph::GameMapGraph(int32_t width, int32_t height)
: IGameMap(eGRAPH), m_entrance(nullptr), m_width(width), m_height(height), m_numberOfExistsAvailable(0), m_numberOfOtherRoomsAvailable(0){
    m_PathFinder = new PathFinder();
}

GameMapGraph::~GameMapGraph(){
    clearMap();
    delete m_PathFinder;
}


vector<MapRoom *> MapRoom::getNeighbours() const{
    vector<MapRoom *> neighbours;
    if (getBottom()!=nullptr){
        neighbours.push_back(getBottom());
    }
    if (getTop()!=nullptr){
        neighbours.push_back(getTop());
    }
    if (getLeft()!=nullptr){
        neighbours.push_back(getLeft());
    }
    if (getRight()!=nullptr){
        neighbours.push_back(getRight());
    }
    
    return neighbours;
}


uint64_t GameMapGraph::getWidth() const {
    return m_width;
}

uint64_t GameMapGraph::getHeight() const {
    return m_height;
}

void * GameMapGraph::getMap() const {
    return (void *)m_entrance;
}

void GameMapGraph::setWidth(const uint64_t & width) {
    m_width = width;
}

void GameMapGraph::setHeight(const uint64_t & height) {
    m_height = height;
}

void GameMapGraph::setMap(void * map) {
    m_entrance = (MapRoom *)map;
}

void * GameMapGraph::getEntrance() const {
    return (void *)m_entrance;
}

void GameMapGraph::clearMap(){
    if (m_entrance == nullptr) return;
    
    m_positions.clear();
}

void GameMapGraph::createRandomMap(){
    if (m_entrance != nullptr) clearMap();
    
    setWidth(SIMPLE_MAP_WIDTH);
    setHeight(SIMPLE_MAP_HEIGHT);
    
    srand((uint32_t)time(NULL));
    
    //determine number of rooms
    m_numberOfExistsAvailable = rand()%(MAX_NUMBER_OF_EXIT_ROOMS+1);
    m_numberOfOtherRoomsAvailable = rand()%MAX_NUMBER_OF_OTHER_ROOMS;
    
    //determine entrance location
    uint64_t entranceX = rand()%SIMPLE_MAP_WIDTH;
    uint64_t entranceY = rand()%SIMPLE_MAP_HEIGHT;
    
    m_entrance = new MapRoom(ROOM_TYPE::eENTRANCE, entranceX,entranceY);
    
    addNode(m_entrance);
    
    //add neighbours recursively
    addNeighbours(m_entrance);
}

vector<pair<int64_t, int64_t>> GameMapGraph::getOpenSpots(uint64_t startX, uint64_t startY) const {
    vector<pair<int64_t, int64_t>> slots;
    uint64_t index = (startX + 1)<<32 | startY;
    if (startX + 1 < m_width && m_positions.find(index)==m_positions.end()){
        slots.push_back(pair<int64_t, int64_t>(startX + 1, startY));
    }
    index = (startX - 1)<<32 | startY;
    if ((int)(startX - 1) >= 0 && m_positions.find(index)==m_positions.end()){
        slots.push_back(pair<int64_t, int64_t>(startX - 1, startY));
    }
    index = (startX)<<32 | (startY+1);
    if (startY + 1 < m_height && m_positions.find(index)==m_positions.end()){
        slots.push_back(pair<int64_t, int64_t>(startX, startY + 1));
    }
    index = (startX)<<32 | (startY-1);
    if ((int)(startY - 1) >= 0 && m_positions.find(index)==m_positions.end()){
        slots.push_back(pair<int64_t, int64_t>(startX, startY - 1));
    }
    
    return slots;
}

void GameMapGraph::addNode(MapRoom * node){
    if (node == nullptr) return;
    int64_t x = node->getX();
    int64_t y = node->getY();
    
    m_positions[x<<32 | y] = node;
    //now add neighbours
    if (x +1 < m_width && m_positions.find((x+1)<<32 | y)!= m_positions.end()){
        m_positions[(x+1)<<32 | y]->setLeft(node);
        node->setRight(m_positions[(x+1)<<32 | y]);
    }
    if (x - 1 >= 0 && m_positions.find((x-1)<<32 | y)!= m_positions.end()){
        m_positions[(x-1)<<32 | y]->setRight(node);
        node->setLeft(m_positions[(x-1)<<32 | y]);
    }
    if (y +1 < m_height && m_positions.find(x<<32 | (y+1))!= m_positions.end()){
        m_positions[x<<32 | (y+1)]->setBottom(node);
        node->setTop(m_positions[x<<32 | (y+1)]);
    }
    if (y - 1 >= 0 && m_positions.find(x<<32 | (y-1))!= m_positions.end()){
        m_positions[x<<32 | (y-1)]->setTop(node);
        node->setBottom(m_positions[x<<32 | (y-1)]);
    }
}

void GameMapGraph::addNeighbours(MapRoom * root){
    if (root == nullptr) return;
    if (m_numberOfOtherRoomsAvailable == 0 && m_numberOfExistsAvailable == 0) return;

    //count available neighbours
    vector<pair<int64_t, int64_t>> availableNeighbours = getOpenSpots(root->getX(), root->getY());
    if (availableNeighbours.empty()) return;
    //get number of neighbours
    int numberOfNeighbours = rand()%availableNeighbours.size()+1;
    numberOfNeighbours = std::min(numberOfNeighbours, m_numberOfExistsAvailable+m_numberOfOtherRoomsAvailable);
    
    for (int i = 0; i < numberOfNeighbours; ++i){
        //for now just do equal selection of rooms
        ROOM_TYPE type;
        if (m_numberOfOtherRoomsAvailable == 0){
            type = eEXIT;
        }
        else {
            do{
                type = static_cast<ROOM_TYPE>(rand()%eTOTAL);
            }while (type == eENTRANCE || type == eUNDEFINED || (type == eEXIT && m_numberOfExistsAvailable==0));
            
        }
        
        addNode(new MapRoom(type, availableNeighbours[i].first,availableNeighbours[i].second));
        if (type == eEXIT && m_numberOfExistsAvailable > 0) --m_numberOfExistsAvailable;
        else if (m_numberOfOtherRoomsAvailable > 0) --m_numberOfOtherRoomsAvailable;
    }
    for (int i = 0; i < numberOfNeighbours; ++i){
        addNeighbours(m_positions[availableNeighbours[i].first<<32 | availableNeighbours[i].second]);
    }
}

void GameMapGraph::display() const{
    ROOM_TYPE ** map = new ROOM_TYPE*[m_height];
    for (int i = 0; i < m_height; ++i){
        map[i] = new ROOM_TYPE[m_width];
    }
    for (int i = 0; i < m_height ; ++i){
        for (int j = 0; j < m_width; ++j){
            map[i][j] = eUNDEFINED;
        }
    }
    
    for(const auto & position : m_positions){
        cout << " type: " << position.second->getType() << "(" << position.second->getX() << ", " << position.second->getY()<< ")"  << endl;
        map[position.second->getY()][position.second->getX()] = position.second->getType();
    }
    
    for (int i = 0; i < m_height; ++i){
        for (int j = 0; j < m_width; ++j){
            cout << (int)map[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

vector<pair<int64_t, int64_t>> GameMapGraph::getPath(const PATH_SEARCH type) const{
    switch ((int)type){
        case eDEPTH_FIRST_SEARCH:
            LOG_ERROR("Not yet implemented");
            break;
        case eBREADTH_FIRST_SEARCH:
            LOG_ERROR("Not yet implemented");
            break;
        default:
            LOG_ERROR("Not supported");
            break;
    }
    return vector<pair<int64_t, int64_t>>();
}


