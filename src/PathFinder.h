//
//  PathFinder.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 05/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__PathFinder__
#define __BytePlay__PathFinder__

#include "Interface.h"
#include <vector>
using std::vector;
using std::pair;

class PathFinder : public IPathFinder {
private:
    vector<pair<int64_t, int64_t>> getMoveOptions(const IGameMap &, const int64_t &, const int64_t &, uint8_t **) const;
    bool checkType(const IGameMap &, const pair<int64_t, int64_t> &, const ROOM_TYPE &) const;
    int64_t getScore(const IGameMap &, const vector<pair<int64_t, int64_t> > & ) const;
    void getPathDepthFS_2dArray(const IGameMap &, vector<pair<int64_t, int64_t>> &, uint8_t **, vector<vector<pair<int64_t, int64_t>>> &) const;
    vector<pair<int64_t, int64_t>> getPathDepthFS_2dArray(const IGameMap &) const;
    vector<pair<int64_t, int64_t>> getPathDepthFS_Graph(const IGameMap &) const;
public:
    vector<pair<int64_t, int64_t>> getPathDepthFS(const IGameMap &map) const;
    vector<pair<int64_t, int64_t>> getPathBreathFS(const IGameMap &map) const;
};

#endif /* defined(__BytePlay__PathFinder__) */
