//
//  main.cpp
//  Game Map assignment
//
//  Created by Redelinghuys, Ferdinand on 04/10/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//
#include "Receiver.h"
#include "Settings.h"
#include "macrologger.h"

int main(int argv, char* argc[]){
    
    LOG_DEBUG("Starting program");
    LOG_DEBUG("Execute Tests");
    Receiver * receiver = Receiver::getInstance();
    receiver->sendEvent(Receiver::RECEIVER_EVENT::eRUN_ALL_TESTS);
    return 0;
}