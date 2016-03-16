// FAROOQ KHAN
//
// CISC 3130 - Data Structures
//  Assignment # 2
//
//  main.cpp
//
//  Created by Farooq Khan on 2/11/16.
//  Copyright © 2016 Farooq Khan. All rights reserved.


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Warehouse.hpp"
#include "Card.hpp"

using namespace std;

//
// Function Prototypes
//

void readAndProcessAllCards(Warehouse []);
bool pricesFromStringToDouble(double *, string);
bool findLargestQuantity(Warehouse [], Warehouse, Card, int, vector<int> &);


//
//  Some Glabal Variables
//

const int numberOfWarehouses = 5;
string namesOfWarehouses[numberOfWarehouses] = {"New_York","Los_Angeles","Miami","Houston","Chicago"};


int main(){
    
    Warehouse warehouses [numberOfWarehouses];
    
    for (int i = 0; i < numberOfWarehouses; i++) {
        warehouses[i].warehouseName = namesOfWarehouses[i];
    }
    
    readAndProcessAllCards(warehouses);
    
    return 0;
}

void readAndProcessAllCards(Warehouse warehouses[]){
    
    ifstream recordFile;
    Card card;
    
    string pricesInString;
    double priceChart[3];
    
    //
    // vector - pricesOfAllItems
    // Holds the calculated price of each item processed
    // which are then added to calculate the total price of the order.
    //
    vector<double> pricesOfAllItems;
    vector <int> itemsFromOtherCities;
    
    bool itemFound;
    double tax = .1;
    
    recordFile.open("File");
    
    getline(recordFile, pricesInString);
    
    pricesFromStringToDouble(priceChart, pricesInString);
    
    while(!recordFile.eof()) {
        
        recordFile >> card.cardType;
        recordFile >> card.warehouseName;
        recordFile >> card.amounts[0];
        recordFile >> card.amounts[1];
        recordFile >> card.amounts[2];
        
        if (recordFile.eof())
            break;
        
        //
        // Print out the data of the current card being processed.
        //
        card.printAllData();
        
        if (card.cardType == 'S') {
            
            for (int i = 0; i < numberOfWarehouses; i++) {
                
                if (warehouses[i].warehouseName == card.warehouseName) {
                    
                    warehouses[i].quantities[0] += card.amounts[0];
                    warehouses[i].quantities[1] += card.amounts[1];
                    warehouses[i].quantities[2] += card.amounts[2];
                    
                    cout << "\t" << warehouses[i].warehouseName << "\t\t" << warehouses[i].quantities[0]
                    << " " << warehouses[i].quantities[1] << " " << warehouses[i].quantities[2] << endl << endl;
                }
                
            }
            
        } else if (card.cardType == 'O'){
            
            for (int warehouseNumber = 0; warehouseNumber < numberOfWarehouses; warehouseNumber++) {
                
                if (warehouses[warehouseNumber].warehouseName == card.warehouseName) {
                    
                    for (int itemNumber = 0; itemNumber < 3; itemNumber++) {
                        
                        if (warehouses[warehouseNumber].quantities[itemNumber] >= card.amounts[itemNumber])
                        {
                            warehouses[warehouseNumber].quantities[itemNumber] -= card.amounts[itemNumber];
                            pricesOfAllItems.push_back(priceChart[itemNumber] * card.amounts[itemNumber]);
                        }
                        else {
                            itemFound = findLargestQuantity(warehouses, warehouses[warehouseNumber], card, itemNumber, itemsFromOtherCities);
                            
                            if (itemFound) {
                                
                                double priceBeforeTax;
                                double priceAfterTax;
                                
                                priceBeforeTax = priceChart[itemNumber] * card.amounts[itemNumber];
                                priceAfterTax = (priceBeforeTax * tax) + priceBeforeTax;
                                
                                pricesOfAllItems.push_back(priceAfterTax);
                            
                            } else
                                pricesOfAllItems.push_back(0);
                            
                        }
                    }
                    
                    warehouses[warehouseNumber].printCurrentStock();
                    
                    //
                    // Calculate the total price of Order.
                    //
    
                        double totalPrice = 0.0;
                    
                        for (int q = 0; q < 3 ; q++) {
                            
                            totalPrice += pricesOfAllItems[q];
                        }
                    
                        cout << "Price: " << totalPrice << endl;
                    
                    pricesOfAllItems.clear(); // Clear the price vector before processing the next card.
                    
                }
        
            }
            
        }
    }
    
    recordFile.close();
}

//
//  This function looks for the warehouse which holds the largest amount
//  of a particular item and ships it to the warehouse, it was ordered.
//

bool findLargestQuantity(Warehouse warehouse[], Warehouse workingWarehouse, Card card, int item, vector<int> &itemsFromOtherCities) {

    int largestQuantityAvailable = 0;
    int warehouseWithLargestQuantity = 0;
    
    //
    // Finds the warehouse with the largest quantity of a particular item.
    //
    for ( int j = 0; j < numberOfWarehouses; j++) {
        
        if (largestQuantityAvailable < warehouse[j].quantities[item]) {
            
            largestQuantityAvailable = warehouse[j].quantities[item];
            warehouseWithLargestQuantity = j;
        }
    }
    //
    //  Checks if the Largest quantity found is greater than or equal to the desired amount.
    //
    if (largestQuantityAvailable >= card.amounts[item]) {
        
        warehouse[warehouseWithLargestQuantity].quantities[item] -= card.amounts[item];
        itemsFromOtherCities.push_back(card.amounts[item]);
        
        cout << endl << card.amounts[item] << " of Item "<< item+1 << " shipped from " << warehouse[warehouseWithLargestQuantity].warehouseName << " to " << workingWarehouse.warehouseName << endl;
        
        warehouse[warehouseWithLargestQuantity].printCurrentStock();
        
        return true;
    }
    else{
        cout <<"Item " << item+1 << ": Order Unfullfilled" << endl;
        return false;
    }

}

//
//  This function separates numbers (prices),
//  followed by a dollar sign, from a given string.
//

bool pricesFromStringToDouble(double *buffer, string stringline){
    
    int iterator = 0;
    size_t position = 0;
    
    while (position < stringline.length()-1) {
        
        size_t positionOfDollarSign = stringline.find("$", position);

        if (positionOfDollarSign == string::npos) {
            position = stringline.length();
            break;
        }
        
        size_t positionOfSpace = stringline.find(" ", positionOfDollarSign-1);
        
        if(positionOfSpace == string::npos){
           
            
            positionOfSpace = stringline.length()-1;
        }
        
        string priceInStringType;
        
        priceInStringType = stringline.substr(positionOfDollarSign+1, positionOfSpace-positionOfDollarSign);

        
        istringstream stringToDoubleConverter(priceInStringType);
        
        if(!(stringToDoubleConverter >> buffer[iterator])){
            return false;
        }
        
        position = positionOfSpace;
        
        iterator++;
    }
    
    return true;
}





