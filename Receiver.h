//
//  Receiver.h
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 08/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Receiver__
#define __BytePlay__Receiver__

class Receiver {
public:
    static Receiver *sm_instance;
    static Receiver * getInstance();
    enum RECEIVER_EVENT{
        eCREATE_2D_ARRAY_RANDOM_MAP,
        eCREATE_GRAPH_RANDOM_MAP,
        eSERIALIZE_TO_STRING_AND_WRITE_TO_FILE,
        eSERIALIZE_TO_BINARY_AND_WRITE_TO_FILE,
        eSERIALIZE_TO_STRING_COMPRESSED_AND_WRITE_TO_FILE,
        eSERIALIZE_TO_BINARY_COMPRESSED_AND_WRITE_TO_FILE,
        eSERIALIZE_TO_STRING_ABSOLUTE_POSITIONS_AND_WRITE_TO_FILE,
        eSERIALIZE_TO_BINARY_ABSOLUTE_POSITIONS_AND_WRITE_TO_FILE,
        eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING,
        eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY,
        eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING_COMPRESSED,
        eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY_COMPRESSED,
        eREAD_FROM_FILE_AND_DESERIALIZE_TO_STRING_ABSOLUTE_POSITIONS,
        eREAD_FROM_FILE_AND_DESERIALIZE_TO_BINARY_ABSOLUTE_POSITIONS,
        eFIND_OPTIMAL_PATH,
        eDISPLAY_MAP,
        eRUN_ALL_TESTS,
        eEVENT_MAX
    };
    void sendEvent(const RECEIVER_EVENT &, void * = nullptr) const;
};

#endif /* defined(__BytePlay__Receiver__) */
