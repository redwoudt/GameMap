//
//  PathFinder.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 05/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "PathFinder.h"
#include "macrologger.h"

vector<pair<int64_t, int64_t>> PathFinder::getMoveOptions(const IGameMap &map, const int64_t & startX, const int64_t & startY, uint8_t ** visited) const {
    vector<pair<int64_t, int64_t>> options;
    
    uint8_t ** map2D = (uint8_t **)map.getMap();
    if (map2D == nullptr || visited == nullptr) return options;
    
    if (startX + 1 < map.getWidth() && map2D[startY][startX + 1] != eUNDEFINED && !visited[startY][startX + 1]){
        options.push_back(pair<int64_t, int64_t>(startX + 1, startY));
    }
    if (startX - 1 >= 0 && map2D[startY][startX - 1] != eUNDEFINED && !visited[startY][startX - 1]){
        options.push_back(pair<int64_t, int64_t>(startX - 1, startY));
    }
    if (startY + 1 < map.getHeight() && map2D[startY + 1][startX] != eUNDEFINED && !visited[startY + 1][startX]){
        options.push_back(pair<int64_t, int64_t>(startX, startY + 1));
    }
    if (startY - 1 >= 0 && map2D[startY - 1][startX] != eUNDEFINED && !visited[startY - 1][startX]){
        options.push_back(pair<int64_t, int64_t>(startX, startY - 1));
    }
    
    return options;
}

bool PathFinder::checkType(const IGameMap &map, const pair<int64_t, int64_t> & pos, const ROOM_TYPE & eType) const{
    uint8_t ** map2D = (uint8_t **)map.getMap();
    if (map2D == nullptr || pos.first >= map.getWidth() || pos.second >= map.getHeight() || pos.first < 0 || pos.second < 0) return false;
    
    return (int)map2D[pos.second][pos.first] == (int)eType;
}

void PathFinder::getPathDepthFS_2dArray(const IGameMap &map, vector<pair<int64_t, int64_t> > &path, uint8_t ** visited, vector<vector<pair<int64_t, int64_t>>> &completedPaths) const {
    //find options from last position in path
    pair<int64_t, int64_t> lastPoint = path.back();
    vector<pair<int64_t, int64_t>> options = getMoveOptions(map, lastPoint.first, lastPoint.second, visited);
    
    //check if any are the exit, if so add to path and return full path
    // else add each seperately and call this function recursively
    for (const auto & option : options){
        if (checkType(map, option, ROOM_TYPE::eEXIT)){
            //found exit
            path.push_back(option);
            completedPaths.push_back(path);
            path.pop_back();
        }
        else{
            path.push_back(option);
            visited[option.second][option.first] = 1;
            getPathDepthFS_2dArray(map, path, visited, completedPaths);
            path.pop_back();
        }
    }
}

int64_t PathFinder::getScore(const IGameMap &map, const vector<pair<int64_t, int64_t>> & path) const{
    int64_t score = 0;
    for (const auto & pos : path){
        if (checkType(map, pos, ROOM_TYPE::eTREASURE)){
            score += 1;
        }
        else if (checkType(map, pos, ROOM_TYPE::eMONSTER)){
            score -= 1;
        }
    }
    return score;
}

vector<pair<int64_t, int64_t>> PathFinder::getPathDepthFS_2dArray(const IGameMap &map) const{
    vector<pair<int64_t, int64_t>> path;
    
    //find entrance & number of exists
    pair<int64_t, int64_t> entrance(-1, -1);
    uint64_t numberOfExists = 0;
    
    uint8_t ** map2D = (uint8_t **)map.getMap();
    if (map2D == nullptr) return path;
    
    for (int i = 0; i < map.getHeight(); ++i){
        for (int j = 0; j < map.getWidth(); ++j){
            if (map2D[i][j] == (int)eENTRANCE){
                entrance.first = j;
                entrance.second = i;
            }
            else if (map2D[i][j] == (int)eEXIT){
                numberOfExists++;
            }
        }
    }
    
    if (numberOfExists == 0 || entrance.first == -1) return path;
    
    path.push_back(entrance);
    
    //setup visited array
    uint8_t ** visited = new uint8_t*[ map.getHeight()];
    for (int i = 0; i < map.getHeight(); ++i){
        visited[i] = new uint8_t[map.getWidth()];
    }
    for (int i = 0; i < map.getHeight() ; ++i){
        for (int j = 0; j < map.getWidth(); ++j){
            visited[i][j] = 0;
        }
    }
    visited[entrance.second][entrance.first] = 1;
    vector<vector<pair<int64_t, int64_t>>> completedPaths;
    getPathDepthFS_2dArray(map, path, visited, completedPaths);
   
    vector<pair<int64_t, int64_t>> bestPath;
    int64_t scoreMax = INT64_MIN;
    LOG_DEBUG("number of solutions: %d", (int)completedPaths.size());
    for (const auto & completedPath : completedPaths){
#if LOG_LEVEL >= DEBUG_LEVEL
        for (int i = 0; i < completedPath.size(); ++i){
            LOG_DEBUG("(%d, %d) ", (int)completedPath[i].first, (int)completedPath[i].second);
        }
#endif
        
        int64_t score = getScore(map, completedPath);
        if (score > scoreMax){
            bestPath = completedPath;
        }
    }
    
#if LOG_LEVEL >= DEBUG_LEVEL
    LOG_DEBUG("best path: ");
    for (int i = 0; i < bestPath.size(); ++i){
        LOG_DEBUG("(%d, %d) ", (int)bestPath[i].first, (int)bestPath[i].second);
    }
#endif
    
    return bestPath;
}

vector<pair<int64_t, int64_t>> PathFinder::getPathDepthFS_Graph(const IGameMap &map) const{
    LOG_ERROR("not supported ");
    return vector<pair<int64_t, int64_t>>();
}

vector<pair<int64_t, int64_t>> PathFinder::getPathDepthFS(const IGameMap &map) const{
    if (map.getType() == e2D_ARRAY){
        return getPathDepthFS_2dArray(map);
    }
    else if (map.getType() == eGRAPH){
        return getPathDepthFS_Graph(map);
    }
    else{
        LOG_ERROR("not supported ");
    }
    
    return vector<pair<int64_t, int64_t>>();
}

vector<pair<int64_t, int64_t>> PathFinder::getPathBreathFS(const IGameMap &map) const{
    LOG_ERROR("not supported ");
    return vector<pair<int64_t, int64_t>>();
}
