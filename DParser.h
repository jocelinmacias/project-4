#ifndef PROJECT4_DOCUPAR_H
#define PROJECT4_DOCUPAR_H

#include "IndexHandler.h"
#include "porter2_stemmer.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;
using namespace rapidjson;
using namespace Porter2Stemmer;

struct ParsedDocument {
    // Storing information from documents
    string title;
    string publication;
    string date;
    string text;
};

class DocumentParser {
private:
    // pointer to IndexHandler for shared functionality
    IndexHandler* indexHandler;
    unordered_set<string> stopwords;

public:
    // Constructor
    DocumentParser();

    // Set the IndexHandler instance (no deep copy)
    void setIndexHandler(IndexHandler* handler);

    // Reads and loads stopwords from a file
    void loadStopWords();

    // Processes all files in the given directory
    void processDirectory(const string& folderPath);

    // Parses a JSON file to extract relevant document information
    void parseJsonFile(const string& jsonFilePath);

    // Collects essential document data based on file input
    ParsedDocument fetchDocumentData(const string& fileName);
};

#endif 
