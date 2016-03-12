//
//  Warehouse.hpp
//  Assignment#2
//
//  Created by Farooq Khan on 2/27/16.
//  Copyright © 2016 Farooq Khan. All rights reserved.
//

#ifndef Warehouse_hpp
#define Warehouse_hpp

#include <iostream>

using namespace std;

class Warehouse {

public:
    
    string warehouseName;
    int quantities[3]= {0,0,0};
    
    void printCurrentStock() {
        cout << "\t" <<warehouseName << "\t\t" << quantities[0]
        << " " <<quantities[1] << " " << quantities[2]
        << endl << endl;
    }
    
};

#endif /* Warehouse_hpp */
