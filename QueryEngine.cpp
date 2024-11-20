//
// Created by MNGal on 11/14/2024.
//

#include "QueryEngine.h"

vector<pair<double, string> > QueryEngine::parseQuery(string query) {
    //return vector<pair<double, string>>();

    stringstream ss(query);
    //all terms we need to search for in the tree
    vector <string> terms;
    while(!ss.eof()){
        string temp;
        getline(ss,temp, ' ');
        terms.push_back(temp);
    }

    //library using streammer
    trim(terms[0]);
    stem(terms[0]);

    //getter form teh text tree and calling it
    map <string,int> results = ih->getWord(terms[0]);

    //for the documents
    for (int i = 0; i < terms.size();i++ ){
        if (terms[i][0] == '-'){
            //grabs everything from the front except the '-' in the front
            string word = terms[i].substr(1, terms[i].length()-1);

            trim(word);
            stem(word);

            //create a new map only for the results of that one word
            map<string, int> docs = ih->getWord(word);

            //iterates throufh the map
            for(const pair<string, int> & itr: docs){
                //if it does exist then remove it
                if(results.find(itr.first) != results.end()){
                    results.erase((itr.first));
                }
            }

        }
            //for the organizations

            //will check if there is a colon and org that will differnetiate and idnetify it as an organization
        else if(terms [i].length() >4 && terms[i].substr(0,4) == "ORG:"){
            string org = terms[i] .substr(4, terms[i].length()-4);
            trim(org);
            stem(org);

            //getter from the tree in the index handler
            map<string, int> docs = ih->getOrg(org);


            //iterate through the results
            for(const pair<string,int> itr: results){

                //if this is not in the previous or current
                if(docs.find(itr.first)==docs.end()){
                    results.erase(itr.first);
                }else{
                    //if it is in the intersection, we must increase its frequency from the past and the new
                    //current one (sum of the two frequencies)
                    results[itr.first] +=docs[itr.first];
                }
            }
        }
            //copying the same function as before with the parameter of PERSON
        else if(terms [i].length() >7 && terms[i].substr(0,7) == "PERSON:"){
            string person = terms[i] .substr(7, terms[i].length()-7);
            trim(person);
            stem(person);

            //getter from the tree in the index handler
            map<string, int> docs = ih->getPeo(person);


            //iterate through the results
            for(const pair<string,int> itr: results){

                //if this is not in the previous or current
                if(docs.find(itr.first)==docs.end()){
                    results.erase(itr.first);
                }else{
                    //if it is in the intersection, we must increase its frequency from the past and the new
                    //current one (sum of the two frequencies)
                    results[itr.first] +=docs[itr.first];
                }
            }

            //This is the one for regular words
        } else {
            string word = terms[i];
            trim(word);
            stem(word);

            //getter from the tree in the index handler
            map<string, int> docs = ih->getWord(word);


            //iterate through the results
            for(const pair<string,int> itr: results){

                //if this is not in the previous or current
                if(docs.find(itr.first)==docs.end()){
                    results.erase(itr.first);
                }else{
                    //if it is in the intersection, we must increase its frequency from the past and the new
                    //current one (sum of the two frequencies)
                    results[itr.first] +=docs[itr.first];
                }
            }
        }
    }

    return sortRelevency(results);
}


//
//Sort scores by the order of ascending and
vector<pair<double, string> > QueryEngine::sortRelevency(map<string, int> & results) {
    //this will be ouputting everything in
    vector< pair<double,string>> sortedResults;

    //iterating through all the results
    for( const pair<string,int>& itr: results){

        //declaring it and establishing the value by formulating the equation using DocCount
        double relevancy = (double )(itr.second)/ih->getDocCount((itr.first));
        sortedResults.push_back(make_pair(relevancy, itr.first));
    }
    //retunr the merge sort of sorted results
    return mergeSort(sortedResults);


    //return vector<pair<double, string>>();
}

QueryEngine::QueryEngine() {
    ih = nullptr;
}

vector<pair<double, string>> QueryEngine::merge(vector<pair<double, string>> & left, vector<pair<double, string>> & right) {
    //return vector<pair<double, string>>();

    //left//1
    int traLeft = 0;

    //right//2
    int traRight= 0;

    vector<pair<double, string>> vec;

    while(traLeft < left.size()&& traRight <right.size()){
        //want to find the larger of the two items
        if(left[traLeft] > right[traRight]){
            vec.push_back(left[traRight]);
            traLeft++;

        }else{
            vec.push_back((right[traRight]));
            traRight++;
        }


    }

    while(traLeft < left.size()){
        vec.push_back(left[traLeft]);
        traLeft++;

    }
    while(traLeft<right.size()){
        vec.push_back(right[traRight]);
        traRight++;
    }
    return vec;


}

vector<pair<double, string>> QueryEngine::mergeSort(vector<pair<double, string>> & vec) {
    if(vec.size()<=1){
        return vec;
    }
    //to find the middle of the array to split
    int mid = vec.size()/2;

    //extracting the first half from teh first point to the middle
    vector <pair<double, string>> left (vec.begin(), vec.begin() +mid);

    //extract the second half
    vector <pair<double, string>> right (vec.begin()+mid, vec.end());

    left = mergeSort(left);

    right = mergeSort(right);

    //returns the new sorted array
    return merge(left, right);







    //return vector<pair<double, string>>();
}
