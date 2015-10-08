//
//  Factory.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 07/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Factory__
#define __BytePlay__Factory__
#include "DataIO.h"
#include "DataConverter.h"

class ADataIOFactory{
public:
    enum DATAIO_FACTORIES{
        FILE,
        NETWORK //for future implementation
    };
    
    virtual IDataIO* getDataIO() = 0;
    static ADataIOFactory* createFactory(DATAIO_FACTORIES factory);
};

class DataIOFileFactory : public ADataIOFactory{
public:
    IDataIO* getDataIO() { return new DataIOFile();}
};

class ADataConverterFactory{
public:
    virtual IDataConverter* getDataConverter() = 0;
    static ADataConverterFactory* createFactory(DATA_CONVERSION_TYPE factory);
};

class DataStringConverterFactory : public ADataConverterFactory{
public:
    IDataConverter* getDataConverter() { return new ConverterString();}
};

class DataBinaryConverterFactory : public ADataConverterFactory{
public:
    IDataConverter* getDataConverter() { return new ConverterBinary();}
};

class DataStringCompressedConverterFactory : public ADataConverterFactory{
public:
    IDataConverter* getDataConverter() { return new ConverterCompressedString();}
};

class DataBinaryCompressedConverterFactory : public ADataConverterFactory{
public:
    IDataConverter* getDataConverter() { return new ConverterCompressedBinary();}
};

class DataStringAbsoluteFactory : public ADataConverterFactory{
public:
    IDataConverter* getDataConverter() { return new ConvertedAbsoluteString();}
};

class DataBinaryAbsoluteFactory : public ADataConverterFactory{
public:
    IDataConverter* getDataConverter() { return new ConverterAbsoluteBinary();}
};

#endif /* defined(__BytePlay__Factory__) */
