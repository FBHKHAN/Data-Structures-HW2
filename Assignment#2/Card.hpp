//
//  Card.hpp
//  Assignment#2
//
//  Created by Farooq Khan on 3/2/16.
//  Copyright © 2016 Farooq Khan. All rights reserved.
//

#ifndef Card_hpp
#define Card_hpp

#include <iostream>

using namespace std;

class Card {

public:
    
    char cardType;
    string warehouseName;
    int amounts[3];
    
    void printAllData() {
        
        if (cardType == 'O')
            cout << "########| ORDER |########" << endl;
        else
            cout << "********| SHIPMENT |********" << endl;
        
        cout << "\t" << warehouseName << "\t\t" << amounts[0]
        << " " << amounts[1] << " " << amounts[2] << endl;
        
    }
};

#endif /* Card_hpp */
