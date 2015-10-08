//
//  Factory.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 07/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "Factory.h"
#include "macrologger.h"

ADataIOFactory* ADataIOFactory::createFactory(DATAIO_FACTORIES factory){
    if (factory == FILE){
        return new DataIOFileFactory();
    }
    else if (factory == NETWORK){
        LOG_ERROR("Not implemented ");
    }
    else {
        LOG_ERROR("Unknown factory ");
    }
    return nullptr;
}

ADataConverterFactory* ADataConverterFactory::createFactory(DATA_CONVERSION_TYPE factory){
    if (factory == eSTRING){
        return new DataStringConverterFactory();
    }
    else if (factory == eBINARY){
        return new DataBinaryConverterFactory();
    }
    else if (factory == eJSON){
        LOG_ERROR("Not implemented ");
    }
    else if (factory == eCOMPRESSED_STRING_RUN_LENGTH){
        return new DataStringCompressedConverterFactory();
    }
    else if (factory == eCOMPRESSED_STRING_POSITION){
        return new DataStringAbsoluteFactory();
    }
    else if (factory == eCOMPRESSED_BINARY_RUN_LENGTH){
        return new DataBinaryCompressedConverterFactory();
    }
    else if (factory == eCOMPRESSED_BINARY_POSITION){
        return new DataBinaryAbsoluteFactory();
    }
    else {
        LOG_ERROR("Unknown factory ");
    }
    return nullptr;
}