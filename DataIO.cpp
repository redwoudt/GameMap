//
//  DataIO.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 05/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "DataIO.h"
#include <fstream>
using std::ofstream;
using std::ios;
using std::ifstream;
#include <string>
using std::string;
#include "macrologger.h"

void DataIOFile::read(const string & path, string & data) const{
    ifstream inputFile(path);
    if (inputFile.is_open()){
        LOG_DEBUG("File %s opened ", path.c_str());
        string line;
        while (getline(inputFile, line)){
            data += line;
        }
    }
    else {
        LOG_ERROR("Error: file (%s) not opened", path.c_str());
    }
    inputFile.close();
}

void DataIOFile::read(const string & path, uint8_t ** data, uint64_t & size) const{
    ifstream inputFile;
    inputFile.open(path, ios::binary | ios::out);
    if (inputFile.is_open()){
        LOG_DEBUG("File %s opened ", path.c_str());
        inputFile.seekg(0, inputFile.end);
        uint64_t len = inputFile.tellg();
        inputFile.seekg(0, inputFile.beg);
        
        *data = (uint8_t *)new char [len];
        
        inputFile.read((char *)*data, len);
        
        size = len;
        
#if LOG_LEVEL >= DEBUG_LEVEL
        LOG_DEBUG("Buffer: ");
        for (int i = 0; i < len; ++i){
            LOG_DEBUG("%d ", (int)(*data)[i]);
        }
#endif
    }
    else {
        LOG_ERROR("Error: file (%s) not opened", path.c_str());
    }
    inputFile.close();
    
}

void DataIOFile::write(const string & path, const string & data){
    ofstream outputFile(path);
    if (outputFile.is_open()){
        LOG_DEBUG("File %s opened ", path.c_str());
        outputFile << data;
        LOG_DEBUG("Data written to: %s", path.c_str());
    }
    else {
        LOG_ERROR("Error: file (%s) not opened", path.c_str());
    }
    outputFile.close();
}

void DataIOFile::write(const string & path, const uint64_t * data, uint64_t size){
    ofstream outputFile;
    outputFile.open(path, ios::binary | ios::out);
    if (outputFile.is_open()){
        LOG_DEBUG("File %s opened ", path.c_str());
        uint64_t height = data[0];
        uint64_t width = data[1];
        uint64_t data_ptr = data[2];
        uint64_t dimensionBufferSize = sizeof(uint64_t) / sizeof(char);
        char heightBuffer[dimensionBufferSize];
        char widthBuffer[dimensionBufferSize];

#if LOG_LEVEL >= DEBUG_LEVEL
        LOG_DEBUG("data: %d", (int)height);
        LOG_DEBUG("data: %d", (int)width);
        LOG_DEBUG("data: %d", (int)data_ptr);
#endif
        
        //move 64-bit numbers into 8-bit equavalent
        //height
        for (int i = 0; i < dimensionBufferSize; ++i){
            heightBuffer[i] = 0;
        }
        int64_t index = dimensionBufferSize-1;
        while (height && index >= 0){
            heightBuffer[index] = height & 0XFF;
            index--;
            height = height >> 8;
        }

#if LOG_LEVEL >= DEBUG_LEVEL
        for (int i = 0; i < dimensionBufferSize; ++i){
            LOG_DEBUG("%d:",(int)heightBuffer[i]);
        }
#endif
        //width
        for (int i = 0; i < dimensionBufferSize; ++i){
            widthBuffer[i] = 0;
        }
        index = dimensionBufferSize-1;
        while (width && index >= 0){
            widthBuffer[index] = width & 0XFF;
            index--;
            width = width >> 8;
        }
        
#if LOG_LEVEL >= DEBUG_LEVEL
        for (int i = 0; i < dimensionBufferSize; ++i){
            //if (i%(width/2) == 0) cout << endl;
            LOG_DEBUG("%d:", (int)widthBuffer[i]);
        }
#endif
        
#if LOG_LEVEL >= DEBUG_LEVEL
        for (int i = 0; i < width*height/2; ++i){
            //if (i%(width/2) == 0) cout << endl;
            LOG_DEBUG("%d ", (int)((char *)data_ptr)[i]);
        }
#endif
        outputFile.write(heightBuffer, sizeof(heightBuffer));
        outputFile.write(widthBuffer, sizeof(widthBuffer));
        outputFile.write((char *)data_ptr, size);
        LOG_DEBUG("Data written to: %s", path.c_str());
    }
    else {
        LOG_ERROR("Error: file (%s) not opened", path.c_str());
    }
    outputFile.close();
}