//
//  DataIO.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 05/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__DataIO__
#define __BytePlay__DataIO__

#include "Interface.h"
#include <string>
using std::string;
    
class DataIOFile : public IDataIO{
public:
    void read(const string &, string &) const;
    void read(const string &, uint8_t **, uint64_t & ) const;
    void write(const string &, const string &);
    void write(const string &, const uint64_t *, uint64_t );
};

#endif /* defined(__BytePlay__DataIO__) */
