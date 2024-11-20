//
// Created by MNGal on 11/14/2024.
//

#ifndef PROJECT4_QUERYENGINE_H
#define PROJECT4_QUERYENGINE_H

#include "InHan.h"
#include "porter2_stemmer.h"


using namespace Porter2Stemmer;




class QueryEngine {
private:
    InHan* ih;
public:

    //do something for this
    QueryEngine();
    void setIndex(InHan *i) {
        //shallow copy
        ih = i;
    }



    //Like the search box in google (what is inside the string)
    vector<pair<double,string> >parseQuery(string);

    //take the unsorted list and converted it to a sorted one
    vector <pair<double,string> >sortRelevency(map<string, int>&);

    //sorting outer(display the result and the number of frequencies
    vector <pair<double,string>> mergeSort(vector <pair<double, string>>&);
    vector<pair<double, string>> merge(vector <pair<double, string>>&, vector <pair<double, string>>&);



};


#endif //PROJECT4_QUERYENGINE_H
