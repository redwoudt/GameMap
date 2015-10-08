//
//  Interface.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 05/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef BytePlay_Interface_h
#define BytePlay_Interface_h
#include <string>
#include <vector>
using std::vector;
using std::pair;
using std::string;
/* ROOM_TYPE: define list of possible room types */
enum ROOM_TYPE{
    eEMPTY = 0,
    eENTRANCE = 1,
    eMONSTER = 2,
    eTREASURE = 3,
    eEXIT = 4,
    eUNDEFINED = 5,
    eTOTAL = 6
};

enum DATA_CONVERSION_TYPE {
    eBINARY,
    eSTRING,
    eJSON,
    eCOMPRESSED_STRING_RUN_LENGTH,
    eCOMPRESSED_STRING_POSITION,
    eCOMPRESSED_BINARY_RUN_LENGTH,
    eCOMPRESSED_BINARY_POSITION
};

enum GAME_MAP_TYPE {
    e2D_ARRAY,
    eGRAPH
};

enum PATH_SEARCH{
    eDEPTH_FIRST_SEARCH,
    eBREADTH_FIRST_SEARCH,
    eOPTIMAL,
    ePATH_SEARCH_MAX
};

struct Point{ 
    Point(int64_t xPos = 0, int64_t yPos = 0) : x(xPos), y(yPos){}
    int64_t x;
    int64_t y;
};

/*  */
class IGameMap{
protected:
    GAME_MAP_TYPE type;
public:
    IGameMap(GAME_MAP_TYPE t) : type(t){}
    virtual ~IGameMap(){};
    GAME_MAP_TYPE getType() const {return type;}
    virtual void createRandomMap() = 0;
    virtual void display() const = 0;
    virtual uint64_t getWidth() const = 0;
    virtual uint64_t getHeight() const = 0;
    virtual void * getMap() const = 0;
    virtual void setWidth(const uint64_t &) = 0;
    virtual void setHeight(const uint64_t &) = 0;
    virtual void setMap(void *) = 0;
    virtual void * getEntrance() const = 0;
    virtual vector<pair<int64_t, int64_t>> getPath(const PATH_SEARCH = eBREADTH_FIRST_SEARCH) const = 0;
};

struct ConverterData {
    ConverterData(const string & p="", IGameMap * m=nullptr, const DATA_CONVERSION_TYPE & type = DATA_CONVERSION_TYPE::eBINARY) : m_path(p), m_map(m), m_type(type) {}
    void setValues(const string & p="", IGameMap * m=nullptr, const DATA_CONVERSION_TYPE & type = DATA_CONVERSION_TYPE::eBINARY) {m_path = p; m_map = m; m_type = type;}
    string m_path;
    IGameMap *m_map;
    DATA_CONVERSION_TYPE m_type;
};

/* DataConverter: abstract to define interface for different converters */
class IDataConverter{
protected:
    DATA_CONVERSION_TYPE type;
public:
    IDataConverter(DATA_CONVERSION_TYPE t) : type(t){}
    /* serialize GameMap object to byte map */
    /* string can be URL or file name */
    virtual uint64_t serialize(const IGameMap &, void *) = 0;
    /* deserialize byte map into GameMap object */
    /* string can be URL or file name */
    virtual void deserialize(void *, IGameMap &, const uint64_t & = 0) = 0;
};

class IDataIO{
public:
    virtual void read(const string &, string &) const = 0;
    virtual void read(const string &, uint8_t **, uint64_t & ) const = 0;
    virtual void write(const string &, const string &) = 0;
    virtual void write(const string &, const uint64_t *, uint64_t ) = 0;
};

class IPathFinder{
public:
    virtual ~IPathFinder() {};
    virtual vector<pair<int64_t, int64_t>> getPathDepthFS(const IGameMap &) const = 0;
    virtual vector<pair<int64_t, int64_t>> getPathBreathFS(const IGameMap &) const = 0;
};


#endif
