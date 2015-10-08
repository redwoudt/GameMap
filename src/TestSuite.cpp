//
//  TestSuite.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 08/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "TestSuite.h"
#include "Interface.h"
#include "Factory.h"
#include "Receiver.h"
#include "Settings.h"
#include "macrologger.h"
#include <iostream>
using std::cout;
using std::endl;

TestSuite *TestSuite::sm_instance = nullptr;

TestSuite * TestSuite::getInstance(){
    if (sm_instance == nullptr){
        sm_instance = new TestSuite;
    }
    return sm_instance;
}

void TestSuite::createRandomMap(void * data) const{
    IGameMap * map = (IGameMap *)data;
    if (map != nullptr){
        LOG_INFO(">create random map");
        map->createRandomMap();
    }
    else {
        LOG_ERROR("map is not defined");
    }
}

void TestSuite::displayMap(void * data) const{
    IGameMap * map = (IGameMap *)data;
    if (map != nullptr){
        LOG_INFO(">display map");
        map->display();
    }
    else {
        LOG_ERROR("map is not defined");
    }
}

void TestSuite::serializeAndWriteToFile(ConverterData *data) const{
    if (data != nullptr && data->m_map != nullptr){
        LOG_DEBUG(">serialize and write to file");
        DATA_CONVERSION_TYPE type = data->m_type;
        std::unique_ptr<ADataIOFactory> ioDataFactory(ADataIOFactory::createFactory(ADataIOFactory::DATAIO_FACTORIES::FILE));
        std::unique_ptr<IDataIO> ioData (ioDataFactory->getDataIO());
        
        //convert map to export string data format
        std::unique_ptr<ADataConverterFactory> dataConverterFactory(ADataConverterFactory::createFactory(type));
        std::unique_ptr<IDataConverter> convertedData (dataConverterFactory->getDataConverter());
        if (type == DATA_CONVERSION_TYPE::eSTRING || type == DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_RUN_LENGTH || type == DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_POSITION){
            string stringData;
            convertedData->serialize(*data->m_map, (void *)&stringData);
            ioData->write(data->m_path,stringData);
        }
        else if (type == DATA_CONVERSION_TYPE::eBINARY || type == DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_RUN_LENGTH || type == DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_POSITION){
            uint64_t * binaryData = new uint64_t[3];
            uint64_t size;
            size = convertedData->serialize(*data->m_map, (void *) binaryData);
            ioData->write(data->m_path, (uint64_t *)binaryData, size);
            delete [] (uint8_t *)binaryData[2];
            delete [] binaryData;
        }
        else {
            LOG_ERROR("invalid type %d", type);
        }
        
    }
    else {
        LOG_ERROR("invalid data");
    }
}

void TestSuite::serializeAndWriteToFile(void *data) const{
    serializeAndWriteToFile((ConverterData *)data);
}

void TestSuite::deserializeAndReadFromFile(ConverterData *data) const{
    if (data != nullptr && data->m_map != nullptr){
        LOG_DEBUG("> read from file  and deserialize");
        DATA_CONVERSION_TYPE type = data->m_type;
        std::unique_ptr<ADataIOFactory> ioDataFactory(ADataIOFactory::createFactory(ADataIOFactory::DATAIO_FACTORIES::FILE));
        std::unique_ptr<IDataIO> ioData (ioDataFactory->getDataIO());
        
        //convert map to export string data format
        std::unique_ptr<ADataConverterFactory> dataConverterFactory(ADataConverterFactory::createFactory(type));
        std::unique_ptr<IDataConverter> convertedData (dataConverterFactory->getDataConverter());
        if (type == DATA_CONVERSION_TYPE::eSTRING || type == DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_RUN_LENGTH || type == DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_POSITION){
            string stringData;
            ioData->read(data->m_path,stringData);
            convertedData->deserialize((void *)&stringData, *data->m_map);
        }
        else if (type == DATA_CONVERSION_TYPE::eBINARY || type == DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_RUN_LENGTH || type == DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_POSITION){
            uint8_t * binaryReadData;
            uint64_t dataSize = 0;
            ioData->read(data->m_path, &binaryReadData, dataSize);
            convertedData->deserialize((void *)binaryReadData, *data->m_map, dataSize);
            delete [] binaryReadData;
        }
        else {
            LOG_ERROR("invalid type %d", type);
        }
        
    }
    else {
        LOG_ERROR("invalid data");
    }
}

void TestSuite::deserializeAndReadFromFile(void *data) const{
    deserializeAndReadFromFile((ConverterData *)data);
}

void TestSuite::findPath(void * data) const{
    IGameMap * map = (IGameMap *) data;
    if (map == nullptr) return;
    
    vector<pair<int64_t, int64_t>> path = map->getPath(PATH_SEARCH::eDEPTH_FIRST_SEARCH);
    
    cout<<"Best Path for 2D Array Map: "<<endl;
    for (int i = 0; i < path.size(); ++i){
        cout<< "("<< (int)path[i].first << "," << (int)path[i].second << ")";
    }
    if(path.size() == 0){
        cout<< "There was no valid exit ";
    }
    cout << endl;
}


void TestSuite::runAll() const{
    LOG_INFO("Starting Tests...");
    IGameMap * myGame = new GameMap2DArray();
    IGameMap * myGraphGame = new GameMapGraph();
    Receiver * receiver = Receiver::getInstance();
    //create random map
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eCREATE_2D_ARRAY_RANDOM_MAP, (void *)myGame);
    //display map
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGame);
    
    //serialize map to plain string and save to file
    ConverterData newData(FILE_ROOT_PATH+"mapString.dat", myGame, DATA_CONVERSION_TYPE::eSTRING);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eSERIALIZE_TO_STRING_AND_WRITE_TO_FILE, (void *)&newData);
    //serialize map to plain binary and save to file
    newData.setValues(FILE_ROOT_PATH+"mapBinary.dat", myGame, DATA_CONVERSION_TYPE::eBINARY);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eSERIALIZE_TO_BINARY_AND_WRITE_TO_FILE, (void *)&newData);
    //serialize map to plain string and save to file
    newData.setValues(FILE_ROOT_PATH+"mapStringCompressed.txt", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_RUN_LENGTH);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eSERIALIZE_TO_STRING_AND_WRITE_TO_FILE, (void *)&newData);
    //serialize map to plain binary and save to file
    newData.setValues(FILE_ROOT_PATH+"mapCompressedBinary.dat", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_RUN_LENGTH);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eSERIALIZE_TO_BINARY_AND_WRITE_TO_FILE, (void *)&newData);
    //serialize map to plain string and save to file
    newData.setValues(FILE_ROOT_PATH+"mapStringPosition.txt", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_POSITION);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eSERIALIZE_TO_STRING_AND_WRITE_TO_FILE, (void *)&newData);
    //serialize map to plain binary and save to file
    newData.setValues(FILE_ROOT_PATH+"mapBinaryPosition.dat", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_POSITION);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eSERIALIZE_TO_BINARY_AND_WRITE_TO_FILE, (void *)&newData);
    
    //read-in string file and convert back to map
    newData.setValues(FILE_ROOT_PATH+"mapString.dat", myGame, DATA_CONVERSION_TYPE::eSTRING);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING, (void *)&newData);
    //receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGame);
    //read-in binary file and convert back to map
    newData.setValues(FILE_ROOT_PATH+"mapBinary.dat", myGame, DATA_CONVERSION_TYPE::eBINARY);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY, (void *)&newData);
    //receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGame);
    //read-in run-length encoding string file and convert back to map
    newData.setValues(FILE_ROOT_PATH+"mapStringCompressed.txt", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_RUN_LENGTH);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING_COMPRESSED, (void *)&newData);
    //receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGame);
    //read-in binary run-length file and convert back to map
    newData.setValues(FILE_ROOT_PATH+"mapCompressedBinary.dat", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_RUN_LENGTH);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY_COMPRESSED, (void *)&newData);
    //receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGame);
    //read-in absolute point string file and convert back to map
    newData.setValues(FILE_ROOT_PATH+"mapStringPosition.txt", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_STRING_POSITION);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING_ABSOLUTE_POSITIONS, (void *)&newData);
    //receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGame);
    //read-in absolute binary file and convert back to map
    newData.setValues(FILE_ROOT_PATH+"mapBinaryPosition.dat", myGame, DATA_CONVERSION_TYPE::eCOMPRESSED_BINARY_POSITION);
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY_ABSOLUTE_POSITIONS, (void *)&newData);
    //receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGame);
    
    //let's find a way home using DFS
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eFIND_OPTIMAL_PATH, (void *)myGame);
    
    //create random graph map
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eCREATE_GRAPH_RANDOM_MAP, (void *)myGraphGame);
    //display map
    //receiver->sendEvent(Receiver::RECEIVER_EVENT::eDISPLAY_MAP, (void *)myGraphGame);
    delete myGame;
    delete myGraphGame;
    
    LOG_INFO("... Tests Completed!");
}