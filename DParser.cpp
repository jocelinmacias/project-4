#include "DocumentParser.h"

// Constructor
DocumentParser::DocumentParser() : indexHandler(nullptr) {}

// Set the IndexHandler instance
void DocumentParser::setIndexHandler(DocumentHandler* handler) {
    indexHandler = handler;
}

// Reads and loads stopwords from a file
void DocumentParser::loadStopWords() {
    ifstream stopWordsFile("stopwords.txt");
    if (!stopWordsFile.is_open()) {
        throw runtime_error("Failed to open stopwords file.");
    }

    string word;
    while (getline(stopWordsFile, word)) {
        stopwords.insert(word);
    }
    stopWordsFile.close();
}

// Processes all files in the given directory
void DocumentParser::processDirectory(const string& folderPath) {
    for (const auto& entry : filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            parseJsonFile(entry.path().string());
        }
    }
}

// Parses a JSON file to extract relevant document information
void DocumentParser::parseJsonFile(const string& jsonFilePath) {
    ifstream jsonFile(jsonFilePath);
    if (!jsonFile.is_open()) {
        throw runtime_error("Failed to open JSON file: " + jsonFilePath);
    }

    IStreamWrapper isw(jsonFile);
    Document document;
    document.ParseStream(isw);

    if (document.HasParseError()) {
        throw runtime_error("Error parsing JSON file: " + jsonFilePath);
    }

    ParsedDocument parsedDoc;

    // Extract document data if fields exist
    if (document.HasMember("title") && document["title"].IsString()) {
        parsedDoc.title = document["title"].GetString();
    }
    if (document.HasMember("publication") && document["publication"].IsString()) {
        parsedDoc.publication = document["publication"].GetString();
    }
    if (document.HasMember("date") && document["date"].IsString()) {
        parsedDoc.date = document["date"].GetString();
    }
    if (document.HasMember("text") && document["text"].IsString()) {
        parsedDoc.text = document["text"].GetString();
    }

    // Index document data if indexHandler is set
    if (indexHandler) {
        indexHandler->index(parsedDoc);
    }

    jsonFile.close();
}

// Collects essential document data based on file input
ParsedDocument DocumentParser::fetchDocumentData(const string& fileName) {
    ParsedDocument parsedDoc;

    ifstream jsonFile(fileName);
    if (!jsonFile.is_open()) {
        throw runtime_error("Failed to open file: " + fileName);
    }

    IStreamWrapper isw(jsonFile);
    Document document;
    document.ParseStream(isw);

    if (document.HasParseError()) {
        throw runtime_error("Error parsing JSON file: " + fileName);
    }

    // Populate ParsedDocument structure
    if (document.HasMember("title") && document["title"].IsString()) {
        parsedDoc.title = document["title"].GetString();
    }
    if (document.HasMember("publication") && document["publication"].IsString()) {
        parsedDoc.publication = document["publication"].GetString();
    }
    if (document.HasMember("date") && document["date"].IsString()) {
        parsedDoc.date = document["date"].GetString();
    }
    if (document.HasMember("text") && document["text"].IsString()) {
        parsedDoc.text = document["text"].GetString();
    }

    jsonFile.close();
    return parsedDoc;
}
