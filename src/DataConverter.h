//
//  DataConverter.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 05/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__DataConverter__
#define __BytePlay__DataConverter__

#include "Interface.h"
#include "GameMap.h"

using std::string;

class ConverterString : public IDataConverter{
public:
    ConverterString(DATA_CONVERSION_TYPE t = eSTRING) : IDataConverter(t){}
    uint64_t serialize(const IGameMap &, void *);
    void deserialize(void *, IGameMap &, const uint64_t & = 0);
};

class ConverterCompressedString : public IDataConverter{
private:
    bool getNext(int & , int & , string * , size_t & );
public:
    ConverterCompressedString(DATA_CONVERSION_TYPE t = eCOMPRESSED_STRING_RUN_LENGTH) : IDataConverter(t){}
    uint64_t serialize(const IGameMap &, void *);
    void deserialize(void *, IGameMap &, const uint64_t & = 0);
};

class ConvertedAbsoluteString : public IDataConverter{
private:
    bool getNext(int & , int & , int &, string * , size_t & );
public:
    ConvertedAbsoluteString(DATA_CONVERSION_TYPE t = eCOMPRESSED_STRING_POSITION) : IDataConverter(t){}
    uint64_t serialize(const IGameMap &, void *);
    void deserialize(void *, IGameMap &, const uint64_t & = 0);
};

class ConverterBinary : public IDataConverter{
public:
    ConverterBinary(DATA_CONVERSION_TYPE t = eBINARY) : IDataConverter(t){}
    uint64_t serialize(const IGameMap &, void *);
    void deserialize(void *, IGameMap &, const uint64_t & = 0);
};

class ConverterCompressedBinary : public IDataConverter{
private:
    bool getNext(int & , int & , uint8_t * , size_t & );
public:
    ConverterCompressedBinary(DATA_CONVERSION_TYPE t = eCOMPRESSED_BINARY_RUN_LENGTH) : IDataConverter(t){}
    uint64_t serialize(const IGameMap &, void *);
    void deserialize(void *, IGameMap &, const uint64_t & = 0);
};

class ConverterAbsoluteBinary : public IDataConverter{
private:
    bool getNext(int & , int & , int &, uint8_t * , size_t & );
public:
    ConverterAbsoluteBinary(DATA_CONVERSION_TYPE t = eCOMPRESSED_BINARY_RUN_LENGTH) : IDataConverter(t){}
    uint64_t serialize(const IGameMap &, void *);
    void deserialize(void *, IGameMap &, const uint64_t & = 0);
};
#endif /* defined(__BytePlay__DataConverter__) */
