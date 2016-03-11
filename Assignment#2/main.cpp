//
//  main.cpp
//  Assignment#2
//
//  Created by Farooq Khan on 2/11/16.
//  Copyright © 2016 Farooq Khan. All rights reserved.
// $$$$$
// NOTE!!! CHANGE NewYork's Amount3 from 11 to 1

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Warehouse.hpp"
#include "Card.hpp"

using namespace std;

void readInCards(Warehouse []);
bool pricesFromStringToDouble(double *, string);
bool findLargestQuantity(Warehouse [], Warehouse, Card, int, vector<int> &);

const int numberOfWarehouses = 5;
string namesOfWarehouses[numberOfWarehouses] = {"New_York","Los_Angeles","Miami","Houston","Chicago"};


int main(){
    
    Warehouse warehouses [numberOfWarehouses];
    
    for (int i = 0; i < numberOfWarehouses; i++) {
        warehouses[i].warehouseName = namesOfWarehouses[i];
    }
    
    readInCards(warehouses);
    
    //printAllCards(warehouses);
    
    
    return 0;
}

void readInCards(Warehouse warehouses[]){
    ifstream recordFile;
    Card card;
    
    string pricesInString;
    double priceChart[3];
    vector<double> pricesOfAllItems;
    vector <int> itemsFromOtherCities;
    bool itemFound;
    double tax = .1;
    
    recordFile.open("File");
    
    getline(recordFile, pricesInString);
    
    pricesFromStringToDouble(priceChart, pricesInString);
    
    while(!recordFile.eof())
    {
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
                            pricesOfAllItems.push_back(priceChart[itemNumber]*card.amounts[itemNumber]);
                        }
                        else {
                            itemFound = findLargestQuantity(warehouses, warehouses[warehouseNumber], card, itemNumber, itemsFromOtherCities); // insert bool here
                            if (itemFound) {
                                double priceBeforeTax;
                                double priceAfterTax;
                                
                                priceBeforeTax = priceChart[itemNumber] * card.amounts[itemNumber];
                                priceAfterTax = ((priceBeforeTax * tax) + priceBeforeTax);
                                cout << "Price of Item: " << priceAfterTax <<"    " << priceChart[itemNumber] <<" "<< card.amounts[itemNumber]<< endl;
                                pricesOfAllItems.push_back(priceAfterTax);
                            
                            } else{
                                cout << "Item not found in other warehouses... " << endl;
                                pricesOfAllItems.push_back(0);
                            }
                        }
                    }
                    
                    cout << "\t" << warehouses[warehouseNumber].warehouseName << "\t\t" << warehouses[warehouseNumber].quantities[0]
                    << " " << warehouses[warehouseNumber].quantities[1] << " " << warehouses[warehouseNumber].quantities[2]
                    << endl << endl;
                    
                    //
                    // Calculate the total price of Order.
                    //
    
                        double totalPrice = 0.0;
                        for (int q = 0; q < 3 ; q++) {
                            totalPrice += pricesOfAllItems[q];
                            cout << "Price for Item No " << q << " is  ** " << pricesOfAllItems[q] << endl;
                        }
                        cout << "Price: " << totalPrice << endl;
                    
                    
                    pricesOfAllItems.clear(); // Clear the container for next cards prices.
                    
                }
        
            }
            
        }
    }
    
    for (int i =0; i < 3; i++) {
        cout << "Price: " << i << " " << priceChart[i] << endl;
    }
    
    recordFile.close();
}

//
//  This function looks for the warehouse which holds the largest amount
//  of a particular item and ships it to the warehouse from it was ordered.
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
    //  Checks if Largest quantity found is greater than or equal to the desired amount.
    //
    if (largestQuantityAvailable >= card.amounts[item]) {
        
        warehouse[warehouseWithLargestQuantity].quantities[item] -= card.amounts[item];
        itemsFromOtherCities.push_back(card.amounts[item]);
        
        cout << card.amounts[item] << " of Item "<< item+1 << " shipped from " << warehouse[warehouseWithLargestQuantity].warehouseName << " to " << workingWarehouse.warehouseName << endl;
        return true;
    }
    else{
        cout << "Order Unfullfilled" << endl;
        return false;
    }

}

//$$$$$
// FUNCTION NOT FINALIZED!!! CHECK BEFORE SUBMISSION
//

bool pricesFromStringToDouble(double *buffer, string input){
    
    size_t position = 0;
    
    int indexForBuffer = 0;
    
    while (position < input.length()-1) {
        
        size_t positionOfDollarSign = input.find("$", position);
        
        //
        //  If there are no dollar signs, just
        //  set the position to out of range
        //  of the while loop and it will exit.
        //
        
        if (positionOfDollarSign == string::npos) {
            position = input.length();
            break;
        }
        
        size_t positionOfTrailingSpace = input.find(" ", positionOfDollarSign-1);
        
        //
        //  We found the last dollar sign,
        //  so take the substring from the
        //  dollar sign until the end.
        //
        
        if(positionOfTrailingSpace == string::npos){
            
            //
            //  Get the index of the last character
            //
            
            positionOfTrailingSpace = input.length()-1;
        }
        
        
        //
        //  By this point, we have to indices to use
        //  to grab a substring from the input.
        //
        
        string priceValueAsString;
        
        priceValueAsString = input.substr(positionOfDollarSign+1, positionOfTrailingSpace-positionOfDollarSign);
        
        //
        //  Convert the string into a double via a stream.
        //
        //  If for some reason the conversion fails, then
        //  return false
        //
        
        istringstream stringToDoubleConverter(priceValueAsString);
        
        if(!(stringToDoubleConverter >> buffer[indexForBuffer])){
            return false;
        }
        
        //
        //  Move the "header" of the loop to
        //  the next part of the string.
        //
        
        position = positionOfTrailingSpace;
        
        //
        //  Don't forget to update the buffer index
        //
        
        indexForBuffer++;
    }
    
    return true;
}

void manageOrder() {}





