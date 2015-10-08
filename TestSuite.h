//
//  TestSuite.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 08/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__TestSuite__
#define __BytePlay__TestSuite__
#include "Interface.h"

class TestSuite {
private:
    void serializeAndWriteToFile(ConverterData *data) const;
    void deserializeAndReadFromFile(ConverterData *data) const;
public:
    static TestSuite *sm_instance;
    static TestSuite * getInstance();
    void runAll() const;
    void createRandomMap(void *) const;
    void serializeAndWriteToFile(void *data) const;
    void deserializeAndReadFromFile(void *data) const;
    void displayMap(void * data) const;
    void findPath(void * data) const;
};
#endif /* defined(__BytePlay__TestSuite__) */
