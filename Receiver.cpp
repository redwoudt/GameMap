//
//  Receiver.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 08/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "Receiver.h"
#include "TestSuite.h"
#include "Interface.h"
#include "macrologger.h"

Receiver *Receiver::sm_instance = nullptr;

Receiver * Receiver::getInstance(){
    if (sm_instance == nullptr){
        sm_instance = new Receiver;
    }
    return sm_instance;
}

void Receiver::sendEvent(const RECEIVER_EVENT &event, void * data) const{
    LOG_DEBUG("Event received: %d", event);
    switch(event){
        case eCREATE_2D_ARRAY_RANDOM_MAP:{
            TestSuite * testSuite = TestSuite::getInstance();
            if (testSuite != nullptr){
                testSuite->createRandomMap(data);
            }
        }
            break;
        case eCREATE_GRAPH_RANDOM_MAP:{
            TestSuite * testSuite = TestSuite::getInstance();
            if (testSuite != nullptr){
                testSuite->createRandomMap(data);
            }
        }
            break;
        case eSERIALIZE_TO_STRING_AND_WRITE_TO_FILE: //fall-through
        case eSERIALIZE_TO_BINARY_AND_WRITE_TO_FILE: //fall-through
        case eSERIALIZE_TO_STRING_COMPRESSED_AND_WRITE_TO_FILE: //fall-through
        case eSERIALIZE_TO_BINARY_COMPRESSED_AND_WRITE_TO_FILE: //fall-through
        case eSERIALIZE_TO_STRING_ABSOLUTE_POSITIONS_AND_WRITE_TO_FILE: //fall-through
        case eSERIALIZE_TO_BINARY_ABSOLUTE_POSITIONS_AND_WRITE_TO_FILE: //fall-through
        {
            TestSuite * testSuite = TestSuite::getInstance();
            if (testSuite != nullptr){
                testSuite->serializeAndWriteToFile(data);
            }
        }
            break;
        case eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING: //fall-through
        case eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY: //fall-through
        case eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING_COMPRESSED: //fall-through
        case eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY_COMPRESSED: //fall-through
        case eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING_ABSOLUTE_POSITIONS: //fall-through
        case eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY_ABSOLUTE_POSITIONS: //fall-through
        {
            TestSuite * testSuite = TestSuite::getInstance();
            if (testSuite != nullptr){
                testSuite->deserializeAndReadFromFile(data);
            }
        }
            break;
        case eDISPLAY_MAP:
        {
            TestSuite * testSuite = TestSuite::getInstance();
            if (testSuite != nullptr){
                testSuite->displayMap(data);
            }
        }
            break;
        case eFIND_OPTIMAL_PATH:
        {
            TestSuite * testSuite = TestSuite::getInstance();
            if (testSuite != nullptr){
                testSuite->findPath(data);
            }
        }
            break;
        case eRUN_ALL_TESTS:
        {
            //cout << "eRUN_ALL_TESTS " << endl;
            TestSuite * testSuite = TestSuite::getInstance();
            testSuite->runAll();
        }
            break;
        case eEVENT_MAX: //fall-through
        default:
            LOG_ERROR("invalid event %d", event);
            break;
    }
}