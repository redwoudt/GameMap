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
/// An enum.
/// List of Room Types
///
enum ROOM_TYPE{
    eEMPTY = 0,     ///< enum value for empty room
    eENTRANCE = 1,  ///< enum value for entrance room
    eMONSTER = 2,   ///< enum value for room with monster
    eTREASURE = 3,  ///< enum value for room with treasure
    eEXIT = 4,      ///< enum value for exit room
    eUNDEFINED = 5, ///< enum value for undefined space
    eTOTAL = 6      ///< enum value for total number of room types
};

/// An enum.
///  List of Data Conversion Types
///
enum DATA_CONVERSION_TYPE {
    eBINARY,                        ///< enum value for binary data conversion
    eSTRING,                        ///< enum value for string data conversion
    eJSON,                          ///< enum value for JSON data conversion
    eCOMPRESSED_STRING_RUN_LENGTH,  ///< enum value for run-length compressed string data conversion
    eCOMPRESSED_STRING_POSITION,    ///< enum value for absolute position string data conversion
    eCOMPRESSED_BINARY_RUN_LENGTH,  ///< enum value for run-length compressed binary data conversion
    eCOMPRESSED_BINARY_POSITION     ///< enum value for absolute position binary data conversion
};

/// An enum.
///  List of Game Map Types
///
enum GAME_MAP_TYPE {
    e2D_ARRAY,      ///< enum value for 2D Array Map.
    eGRAPH          ///< enum value for Graph Map.
};

/// An enum.
///  List of Path Search Types
///
enum PATH_SEARCH{
    eDEPTH_FIRST_SEARCH,    ///< enum value for DFS.
    eBREADTH_FIRST_SEARCH,  ///< enum value for BFS.
    eOPTIMAL,               ///< enum value for Dynamic Programming.
    ePATH_SEARCH_MAX        ///< enum value for max number of types.
};

/// An Point struct.
/// Data structure for a Point
///
struct Point{
    ///
    /// A constructor.
    /// Default constructor as all values have default value.
    /// Used to initialize values on creation of Point
    /// @param xPos int64_t position
    /// @param yPos int64_t position
    ///
    Point(int64_t xPos = 0, int64_t yPos = 0) : x(xPos), y(yPos){}
    int64_t x;  ///< x position.
    int64_t y;      ///< y position.
}; // end of Point

/// A Game Map Interface.
/// Interface to ensure consistent implementation of game map types
///
class IGameMap{
protected:
    GAME_MAP_TYPE type; ///< Type of Game Map.
public:
    ///
    /// A constructor.
    /// @param t Game Map Type.
    ///
    IGameMap(GAME_MAP_TYPE t) : type(t){}
    ///
    /// A virtual destructor.
    ///
    virtual ~IGameMap(){};
    ///
    /// Get Type of Game Map.
    /// @return Returns Game Map type
    ///
    GAME_MAP_TYPE getType() const {return type;}
    ///
    /// Create a Random Game Map.
    ///   This will fill a predefined map size with an
    ///     entrance, 0 to 2 exits and a random number
    ///     of monster, treaure and empty rooms
    /// @return Returns Game Map type
    ///
    virtual void createRandomMap() = 0;
    ///
    /// Display Game Map.
    ///
    virtual void display() const = 0;
    ///
    /// Get Game Map Width.
    /// @return width of map
    ///
    virtual uint64_t getWidth() const = 0;
    ///
    /// Get Game Map Height.
    /// @return height of map
    ///
    virtual uint64_t getHeight() const = 0;
    ///
    /// Get Pointer to Game Map data.
    /// @return Pointer to Game Map data.
    ///
    virtual void * getMap() const = 0;
    ///
    /// Set Game Map Width.
    /// @param width to set map to.
    ///
    virtual void setWidth(const uint64_t & width) = 0;
    ///
    /// Set Game Map Width.
    /// @param height to set map to.
    ///
    virtual void setHeight(const uint64_t & height) = 0;
    ///
    /// Set Game Map data.
    /// @param map Game Map data.
    ///
    virtual void setMap(void * map) = 0;
    ///
    /// Get Entrance room location.
    /// @return entrance room location.
    ///
    virtual void * getEntrance() const = 0;
    ///
    /// Get Optimal Path from Entrance to Exit (if exists).
    /// @param type Type of search algorithm to apply.
    /// @return optimal path. Or empty vector if none exists.
    ///
    virtual vector<pair<int64_t, int64_t>> getPath(const PATH_SEARCH type = eBREADTH_FIRST_SEARCH) const = 0;
};

/// A Converter Data structure.
/// Data passed through events to allow for agnostic data conversion.
///
struct ConverterData {
    ///
    /// A constructor.
    /// @param p Path to file/data location.
    /// @param m Pointer to Game Map instance.
    /// @param type Data conversion type.
    ///
    ConverterData(const string & p="", IGameMap * m=nullptr, const DATA_CONVERSION_TYPE & type = DATA_CONVERSION_TYPE::eBINARY) : m_path(p), m_map(m), m_type(type) {}
    ///
    /// Set values of structure.
    /// @param p Path to file/data location.
    /// @param m Pointer to Game Map instance.
    /// @param type Data conversion type.
    ///
    void setValues(const string & p="", IGameMap * m=nullptr, const DATA_CONVERSION_TYPE & type = DATA_CONVERSION_TYPE::eBINARY) {m_path = p; m_map = m; m_type = type;}
    string m_path;  ///<location of data
    IGameMap *m_map; ///<Game Map instance
    DATA_CONVERSION_TYPE m_type; ///<type of converstion
};

///
/// A interface to define different converters.
///
class IDataConverter{
protected:
    DATA_CONVERSION_TYPE type; ///<type of Data Conversion
public:
    ///
    /// A constructor.
    /// @param t Data conversion type.
    ///
    IDataConverter(DATA_CONVERSION_TYPE t) : type(t){}
    ///
    /// serialize GameMap object to byte map.
    /// @param map Game Map instance to serialize.
    /// @param path Can be URL or file name.
    /// @return size of serialized data
    ///
    virtual uint64_t serialize(const IGameMap & map, void * path) = 0;
    ///
    /// deserialize byte map into GameMap object.
    /// @param map Game Map that will receive deserialize.
    /// @param path Can be URL or file name to deserialize
    /// @param size Size of data
    ///
    virtual void deserialize(void * path, IGameMap & map, const uint64_t & size = 0) = 0;
};

///
/// An interface to define different data input/output mechanisms.
///
class IDataIO{
public:
    ///
    /// Read from given location into given string.
    /// @param path Path to read from.
    /// @param str String to read data into.
    ///
    virtual void read(const string & path, string & str) const = 0;
    ///
    /// Read from given location into given string.
    /// @param path Path to read from.
    /// @param data pointer to returned data.
    /// @param size size of data read.
    ///
    virtual void read(const string & path, uint8_t ** data, uint64_t & size) const = 0;
    ///
    /// Write to given location from given string.
    /// @param path Path to write to.
    /// @param data pointer to data to write.
    ///
    virtual void write(const string & path, const string & data) = 0;
    ///
    /// Write to given location from given string.
    /// @param path Path to write to.
    /// @param data pointer to data to write.
    /// @param size size of data to write.
    ///
    virtual void write(const string & path, const uint64_t * data, uint64_t size) = 0;
};

///
/// An interface for path finder algorithms.
///
class IPathFinder{
public:
    ///
    /// Virtual Destructor.
    ///
    virtual ~IPathFinder() {};
    ///
    /// Get Optimal Path from Entrance to Exit (if exists) using DFS.
    /// @param map Game Map to search.
    /// @return optimal path. Or empty vector if none exists.
    ///
    virtual vector<pair<int64_t, int64_t>> getPathDepthFS(const IGameMap &map) const = 0;
    ///
    /// Get Optimal Path from Entrance to Exit (if exists) using BFS.
    /// @param map Game Map to search.
    /// @return optimal path. Or empty vector if none exists.
    ///
    virtual vector<pair<int64_t, int64_t>> getPathBreathFS(const IGameMap &) const = 0;
};

#endif
