//
//  DataConverter.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 05/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "DataConverter.h"
#include "macrologger.h"
#include <string>
using std::string;
using std::to_string;

uint64_t ConverterString::serialize(const IGameMap &map, void * output){
    if (output == nullptr) {
        LOG_ERROR("Invalid data");
        return 0; 
    }
    uint64_t width = map.getWidth();
    uint64_t height = map.getHeight();
    uint8_t ** data = (uint8_t **)map.getMap();
    if (data != nullptr){
        //add header - type and version should be covered in comms header
        (*(string *)output) += to_string(width) + ",";
        (*(string *)output) += to_string(height) + ",";
        //add body
        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; j++){
                (*(string *)output) += to_string((int)(data[i][j]));
            }
        }
    }
    else {
        LOG_ERROR("Error: map not created! ");
    }
    
    return 0;
}

void ConverterString::deserialize(void * data, IGameMap & map, const uint64_t & size){
    if (data == nullptr) {
        LOG_ERROR("Invalid data");
        return;
    }
    
    string * temp = (string *)(data);
    //extract height & width
    LOG_DEBUG("string to do: %s", temp->c_str());
    size_t pos = temp->find(",");
    if (pos == string::npos){
        LOG_ERROR("Invalid data");
        return;
    }
    uint64_t height = std::stoi(temp->substr(0,pos));
    LOG_DEBUG("height: %d",(int)height);
    size_t pos2 = temp->find(",", pos+1);
    if (pos == string::npos){
        LOG_ERROR("Invalid data");
        return;
    }
    uint64_t width = std::stoi(temp->substr(pos+1,pos2));;
    LOG_DEBUG("width: %d", (int)width);
    
    map.setHeight(height);
    map.setWidth(width);
    //create 2D data
    uint8_t ** newMap = new uint8_t*[height];
    for (int i = 0; i < height; ++i){
        newMap[i] = new uint8_t[width];
    }
    size_t index = pos2+1;
    for (int i = 0; i < height && index < temp->size(); ++i){
        for (int j = 0; j < width && index < temp->size(); ++j){
            newMap[i][j] = (uint8_t)std::stoi(temp->substr(index++, 1));
        }
    }
    
    map.setMap((void *)newMap);
}

uint64_t ConverterCompressedString::serialize(const IGameMap &map, void * output){
    if (output == nullptr) {
        LOG_ERROR("Invalid data");
        return 0;
    }
    uint64_t width = map.getWidth();
    uint64_t height = map.getHeight();
    uint8_t ** data = (uint8_t **)map.getMap();
    if (data != nullptr){
        //add header - type and version should be covered in comms header
        (*(string *)output) += to_string(width) + ",";
        (*(string *)output) += to_string(height) + ",";
        //add body
        int count = 0;
        int prev = -1;
        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; j++){
                if (count == 0){
                    (*(string *)output) += to_string((int)(data[i][j]));
                    count++;
                    prev = (int)(data[i][j]);
                }
                else if (prev == (int)(data[i][j])){
                    count++;
                }
                else if (prev != (int)(data[i][j])){
                    (*(string *)output) += "(" + to_string(count) + ")";
                    (*(string *)output) += to_string((int)(data[i][j]));
                    count = 1;
                    prev = (int)(data[i][j]);
                }
            }
        }
        if (count != 0){
            (*(string *)output) += "(" + to_string(count) + ")";
        }
        LOG_DEBUG("%s", ((string *)output)->c_str());
    }
    else {
        LOG_ERROR("Error: map not created! ");
    }
    
    return 0;
}


bool ConverterCompressedString::getNext(int & value, int & size, string * temp, size_t & index){
    if (temp == nullptr) {
        LOG_ERROR("Invalid data");
        return false;
    }
    value = std::stoi(temp->substr(index++,1));
    size_t bracket1 = temp->find("(", index);
    if (bracket1 == string::npos) return false;
    size_t bracket2 = temp->find(")", bracket1+1);
    if (bracket2 == string::npos) return false;
    size = std::stoi(temp->substr(bracket1+1,bracket2-bracket1-1));
    index = bracket2+1;
    return true;
}

void ConverterCompressedString::deserialize(void * data, IGameMap & map, const uint64_t & size){
    if (data == nullptr) {
        LOG_ERROR("Invalid data");
        return;
    }

    string * temp = (string *)(data);
    //extract height & width
    LOG_DEBUG("string to do: %s",temp->c_str());
    size_t pos = temp->find(",");
    if (pos == string::npos){
        LOG_ERROR("Error: map data invalid ");
    }
    uint64_t height = std::stoi(temp->substr(0,pos));
    LOG_DEBUG("height: %d",(int)height);
    size_t pos2 = temp->find(",", pos+1);
    if (pos == string::npos){
       LOG_ERROR("Error: map data invalid ");
    }
    uint64_t width = std::stoi(temp->substr(pos+1,pos2));;
    LOG_DEBUG("width: %d",(int)width);
    
    map.setHeight(height);
    map.setWidth(width);
    //create 2D data
    uint8_t ** newMap = new uint8_t*[height];
    for (int i = 0; i < height; ++i){
        newMap[i] = new uint8_t[width];
    }
    size_t index = pos2+1;
    int value;
    int numSize;
    int x = 0;
    int y = 0;
    while (index < temp->size()){
        if (getNext(value, numSize, temp, index)){
            for(int i = 0; i < numSize; ++i){
                newMap[y][x++] = value;
                if (x >= width){
                    x = 0;
                    ++y;
                }
                if (y >= height){
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    map.setMap((void *)newMap);
}

uint64_t ConvertedAbsoluteString::serialize(const IGameMap &map, void * output){
    if (output == nullptr) {
        LOG_ERROR("Invalid data");
        return 0;
    }
    uint64_t width = map.getWidth();
    uint64_t height = map.getHeight();
    uint8_t ** data = (uint8_t **)map.getMap();
    if (data != nullptr){
        //add header - type and version should be covered in comms header
        (*(string *)output) += to_string(width) + ",";
        (*(string *)output) += to_string(height) + ",";
        //add body
        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; j++){
                if (data[i][j]!= (uint8_t)eUNDEFINED)
                {
                    (*(string *)output) += to_string((int)(data[i][j]));
                    (*(string *)output) += "(" + to_string(j) + "," + to_string(i) + ")";
                }
            }
        }
        LOG_DEBUG("%s", ((string *)output)->c_str());
    }
    else {
        LOG_ERROR("Error: map not created! ");
    }
    
    return 0;
}

bool ConvertedAbsoluteString::getNext(int & value, int & xPos, int &yPos, string * temp, size_t & index){
    if (temp == nullptr) {
        LOG_ERROR("Invalid data");
        return false;
    }

    value = std::stoi(temp->substr(index++,1));
    size_t bracket1 = temp->find("(", index);
    if (bracket1 == string::npos) {
        LOG_ERROR("Map data invalid");
        return false;
    }
    size_t comma = temp->find(",", bracket1+1);
    size_t bracket2 = temp->find(")", comma+1);
    if (bracket2 == string::npos){
        LOG_ERROR("Map data invalid");
        return false;
    }
    xPos = std::stoi(temp->substr(bracket1+1,comma-bracket1-1));
    yPos = std::stoi(temp->substr(comma+1,bracket2-comma-1));
    index = bracket2+1;
    return true;
}

void ConvertedAbsoluteString::deserialize(void * data, IGameMap & map, const uint64_t & size){
    if (data == nullptr) {
        LOG_ERROR("Invalid data");
        return;
    }

    string * temp = (string *)(data);
    //extract height & width
    LOG_DEBUG("string to do: %s", temp->c_str());
    size_t pos = temp->find(",");
    if (pos == string::npos){
        LOG_ERROR("Error: map data invalid ");
    }
    uint64_t height = std::stoi(temp->substr(0,pos));
    LOG_DEBUG("height: %d",(int)height);
    size_t pos2 = temp->find(",", pos+1);
    if (pos == string::npos){
        LOG_ERROR("Error: map data invalid ");
    }
    uint64_t width = std::stoi(temp->substr(pos+1,pos2));;
    LOG_DEBUG("width: %d",(int)width);
    
    map.setHeight(height);
    map.setWidth(width);
    //create 2D data
    uint8_t ** newMap = new uint8_t*[height];
    for (int i = 0; i < height; ++i){
        newMap[i] = new uint8_t[width];
    }
    size_t index = pos2+1;
    int value;
    int x = 0;
    int y = 0;
    //init 2D array
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            newMap[i][j] = (int)eUNDEFINED;
        }
    }
    //fill-in specific coordinates
    while (index < temp->size()){
        if (getNext(value, x, y, temp, index)){
            newMap[y][x] = value;
        }
        else{
            break;
        }
    }
    map.setMap((void *)newMap);
}

uint64_t ConverterBinary::serialize(const IGameMap &map, void *output){
    if (output == nullptr) {
        LOG_ERROR("Invalid data");
        return 0;
    }

    uint64_t width = map.getWidth();
    uint64_t height = map.getHeight();
    uint64_t size = (width*height/2)*sizeof(uint8_t);
    uint64_t * data = (uint64_t *) output;
    uint8_t * data_ptr = new uint8_t[height*width/2];
    
    //compress values to 4-bit sections
    for (int i = 0; i < height; ++i){
        int ref = 0;
        for (int j = 0; j < width; j += 2){
            uint8_t ** map_temp =(uint8_t **)map.getMap();
            data_ptr[i*height/2 + ref++] = (0xF&map_temp[i][j]) << 4 | (0xF&map_temp[i][j+1]);
        }
    }
#if LOG_LEVEL >= DEBUG_LEVEL
    for (int i = 0; i < height*width/2; ++i){
        //if (i%(width/2) == 0) cout << endl;
        LOG_DEBUG("%d ", (int)data_ptr[i]);
    }
#endif
    
    data[0] = width;
    data[1] = height;
    data[2] = (uint64_t)(data_ptr);
    
#if LOG_LEVEL >= DEBUG_LEVEL
    LOG_DEBUG("data: %d", (int)data[0]);
    LOG_DEBUG("data 2: %d", (int)data[1]);
    LOG_DEBUG("data ptr: %d",(int)data[2]);
    uint8_t * data_test_ptr = (uint8_t *)(data[2]);
    
    for (int i = 0; i < 100; ++i){
        //if (i%10 == 0) cout << endl;
        LOG_DEBUG("%d ", (int)data_test_ptr[i]);
    }
#endif
    
    return size;
}

void ConverterBinary::deserialize(void * data, IGameMap & map, const uint64_t & size){
    uint8_t * temp = (uint8_t *)data;
    //get height
    uint64_t height = 0;
    int index = 0;
    for (;index < sizeof(uint64_t)/sizeof(uint8_t); ++index){
        height <<= 8;
        height += temp[index];
    }
    LOG_DEBUG("height: %d",(int)height);
    
    //get width
    uint64_t width = 0;
    for (;index < 2*sizeof(uint64_t)/sizeof(uint8_t); ++index){
        width <<= 8;
        width += temp[index];
    }
    LOG_DEBUG("width: %d",(int)width);
    
    //create map and fill it in
    map.setHeight(height);
    map.setWidth(width);
    //create 2D data
    uint8_t ** newMap = new uint8_t*[height];
    for (int i = 0; i < height; ++i){
        newMap[i] = new uint8_t[width];
    }
    for (int i = 0; i < height && index < size; ++i){
        for (int j = 0; j < width-1 && index < size; j+=2){
            uint8_t curr = temp[index++];
            newMap[i][j] = (int)(0xF&(curr>>4));
            newMap[i][j+1] = (int)(0xF&curr);
        }
    }
    
    map.setMap((void *)newMap);
#if LOG_LEVEL >= DEBUG_LEVEL
    LOG_DEBUG("Data to export: ");
    for (int i = 0; i < size; ++i){
        LOG_DEBUG("%d ", (int)temp[i]);
    }
#endif
}

uint64_t ConverterCompressedBinary::serialize(const IGameMap &map, void *output){
    if (output == nullptr) {
        LOG_ERROR("Invalid data");
        return 0;
    }

    uint64_t width = map.getWidth();
    uint64_t height = map.getHeight();
    uint64_t * data = (uint64_t *) output;
    uint8_t * data_ptr = new uint8_t[height*width/2];
    
    //compress values to 4-bit section
    // and do run-length encoding
    uint8_t count = 0;
    uint8_t prev = 0;
    int len = 0;
    int i = 0;
    for (; i < height; ++i){
        for (int j = 0; j < width; j += 2){
            uint8_t ** map_temp = (uint8_t **)map.getMap();
            uint8_t curr = (0xF&map_temp[i][j]) << 4 | (0xF&map_temp[i][j+1]);
            if (count == 0){
                data_ptr[len++] = curr;
                count++;
                prev = curr;
            }
            else if (prev == curr){
                count++;
            }
            else if (prev != curr){
                data_ptr[len++] = count;
                data_ptr[len++] = curr;
                count = 1;
                prev = curr;
            }
        }
    }
    if (count != 0){
        data_ptr[len++] = count;
    }


#if LOG_LEVEL >= DEBUG_LEVEL
    for (int i = 0; i < len ; ++i){
        LOG_DEBUG("%d ", (int)data_ptr[i]);
    }
#endif

    data[0] = width;
    data[1] = height;
    data[2] = (uint64_t)(data_ptr);
    
#if LOG_LEVEL >= DEBUG_LEVEL
     LOG_DEBUG("data: %d", (int)data[0]);
     LOG_DEBUG("data 2: %d", (int)data[1]);
     LOG_DEBUG("data ptr: %d", (int)data[2]);
     uint8_t * data_test_ptr = (uint8_t *)(data[2]);
     
     for (int i = 0; i < 100; ++i){
     //if (i%10 == 0) cout << endl;
         LOG_DEBUG("%d ", (int)data_test_ptr[i]);
     }
#endif
    
    return len;
}

bool ConverterCompressedBinary::getNext(int & value, int & size, uint8_t * data, size_t & index){
    if (data == nullptr) {
        LOG_ERROR("Invalid data");
        return false;
    }

    value = data[index++];
    size = data[index++];
    return true;
}

void ConverterCompressedBinary::deserialize(void * data, IGameMap & map, const uint64_t & size){
    uint8_t * temp = (uint8_t *)data;
    //get height
    uint64_t height = 0;
    size_t index = 0;
    for (;index < sizeof(uint64_t)/sizeof(uint8_t); ++index){
        height <<= 8;
        height += temp[index];
    }
    LOG_DEBUG("height: %d",(int)height);
    
    //get width
    uint64_t width = 0;
    for (;index < 2*sizeof(uint64_t)/sizeof(uint8_t); ++index){
        width <<= 8;
        width += temp[index];
    }
    LOG_DEBUG("width: %d",(int)width);
    
    //create map and fill it in
    map.setHeight(height);
    map.setWidth(width);
    //create 2D data
    uint8_t ** newMap = new uint8_t*[height];
    for (int i = 0; i < height; ++i){
        newMap[i] = new uint8_t[width];
    }
    
#if LOG_LEVEL >= DEBUG_LEVEL
    LOG_DEBUG("Data to export: ");
    for (int i = 0; i < size; ++i){
        LOG_DEBUG("%d ", (int)temp[i]);
    }
#endif
              
    int value;
    int numSize;
    int x = 0;
    int y = 0;
    while (index < size){
        if (getNext(value, numSize, temp, index)){
            for(int i = 0; i < numSize; ++i){
                newMap[y][x++] = (int)(0xF&(value>>4));
                newMap[y][x++] = (int)(0xF&value);
                if (x >= width-1){
                    x = 0;
                    ++y;
                }
                if (y >= height){
                    break;
                }
            }
        }
        else{
            break;
        }
    }
    map.setMap((void *)newMap);
    
}

uint64_t ConverterAbsoluteBinary::serialize(const IGameMap &map, void *output){
    if (output == nullptr) {
        LOG_ERROR("Invalid data");
        return 0;
    }

    uint64_t width = map.getWidth();
    uint64_t height = map.getHeight();
    uint64_t * data = (uint64_t *) output;
    uint8_t * data_ptr = new uint8_t[height*width/2];
    
    //add value
    // and add absolute points
    int len = 0;
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; j ++){
            uint8_t ** temp_map = (uint8_t **)map.getMap();
            uint8_t curr = temp_map[i][j];
            if (curr != (uint8_t)eUNDEFINED){
                data_ptr[len++] = curr;
                data_ptr[len++] = (uint8_t)j; //TODO: note that position is limited in size here, need to up it at some point
                data_ptr[len++] = (uint8_t)i;
            }
        }
    }
    
#if LOG_LEVEL >= DEBUG_LEVEL
    LOG_DEBUG("Absolute:");
    for (int i = 0; i < len ; ++i){
        LOG_DEBUG("%d ", (int)data_ptr[i]);
    }
#endif
    
    data[0] = width;
    data[1] = height;
    data[2] = (uint64_t)(data_ptr);
    
#if LOG_LEVEL >= DEBUG_LEVEL
    LOG_DEBUG("data: %d", (int)data[0]);
    LOG_DEBUG("data 2: %d", (int)data[1]);
    LOG_DEBUG("data ptr: %d", (int)data[2]);
     uint8_t * data_test_ptr = (uint8_t *)(data[2]);
     
     for (int i = 0; i < 100; ++i){
     //if (i%10 == 0) cout << endl;
         LOG_DEBUG("%d ", (int)data_test_ptr[i]);
     }
#endif
    
    return len;
}

bool ConverterAbsoluteBinary::getNext(int & value, int & xPos, int &yPos, uint8_t * data, size_t & index){
    if (data == nullptr) {
        LOG_ERROR("Invalid data");
        return false;
    }

    value = data[index++];
    xPos = data[index++];
    yPos = data[index++];
    return true;
}

void ConverterAbsoluteBinary::deserialize(void * data, IGameMap & map, const uint64_t & size){
    uint8_t * temp = (uint8_t *)data;
    //get height
    uint64_t height = 0;
    size_t index = 0;
    for (;index < sizeof(uint64_t)/sizeof(uint8_t); ++index){
        height <<= 8;
        height += temp[index];
    }
    LOG_DEBUG("height: %d",(int)height);
    
    //get width
    uint64_t width = 0;
    for (;index < 2*sizeof(uint64_t)/sizeof(uint8_t); ++index){
        width <<= 8;
        width += temp[index];
    }
    LOG_DEBUG("width: %d",(int)width);
    //create map and fill it in
    map.setHeight(height);
    map.setWidth(width);
    //create 2D data
    uint8_t ** newMap = new uint8_t*[height];
    for (int i = 0; i < height; ++i){
        newMap[i] = new uint8_t[width];
    }
    //fill array with default
    //init 2D array
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j){
            newMap[i][j] = (int)eUNDEFINED;
        }
    }

#if LOG_LEVEL >= DEBUG_LEVEL
    LOG_DEBUG("Data to export: ");
    for (int i = 0; i < size; ++i){
        LOG_DEBUG("%d ", (int)temp[i]);
    }
#endif
    
    int value;
    int x = 0;
    int y = 0;
    while (index < size){
        if (getNext(value, x, y, temp, index)){
            newMap[y][x] = (int)(value);
        }
        else{
            LOG_ERROR("Map invalid data");
            break;
        }
    }
    map.setMap((void *)newMap);
}
