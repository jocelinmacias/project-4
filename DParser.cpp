#include "DParser.h"

DocumentParser::DocumentParser() {
    loadStopWords();
    indexHandler = nullptr;
}

void DocumentParser::setIndexHandler(IndexHandler* handler) {
    indexHandler = handler;
}

void DocumentParser::loadStopWords() {
    ifstream inputFile("../stopwords.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open stopwords file" << endl;
        exit(EXIT_FAILURE);
    }

    stopwords.clear();
    string word;
    while (getline(inputFile, word)) {
        if (word.empty()) continue;
        trim(word); 
        stem(word); 
        stopwords.insert(word);
    }
    inputFile.close();
}

void DocumentParser::processDirectory(const string& directoryPath) {
    for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            parseJsonFile(entry.path().string());
        }
    }
}

void DocumentParser::parseJsonFile(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        exit(EXIT_FAILURE);
    }

    IStreamWrapper jsonStream(inputFile);
    Document jsonDoc;
    jsonDoc.ParseStream(jsonStream);

    if (jsonDoc.HasParseError()) {
        cerr << "JSON parsing error in file " << filePath << ": " 
             << jsonDoc.GetParseError() << " at offset " 
             << jsonDoc.GetErrorOffset() << endl;
        inputFile.close();
        return;
    }

    string textContent = jsonDoc["text"].GetString();
    regex pattern("[A-Za-z]+"); 

    sregex_token_iterator wordIterator(textContent.begin(), textContent.end(), pattern);
    sregex_token_iterator wordEnd;

    while (wordIterator != wordEnd) {
        string token = *wordIterator++;
        trim(token);
        stem(token);

        if (token.length() > 2 && stopwords.find(token) == stopwords.end()) {
            if (indexHandler) {
                indexHandler->addPhrase(token, filePath);
            } else {
                cerr << "Error: IndexHandler is not set." << endl;
            }
        }
    }

    for (const auto& person : jsonDoc["entities"]["persons"].GetArray()) {
        stringstream nameStream(person["name"].GetString());
        string name;

        while (getline(nameStream, name, ' ')) {
            trim(name);
            stem(name);
            if (!name.empty() && indexHandler) {
                indexHandler->addPerson(name, filePath);
            }
        }
    }

    for (const auto& organization : jsonDoc["entities"]["organizations"].GetArray()) {
        stringstream orgStream(organization["name"].GetString());
        string orgName;

        while (getline(orgStream, orgName, ' ')) {
            trim(orgName);
            stem(orgName);
            if (!orgName.empty() && indexHandler) {
                indexHandler->addOrganization(orgName, filePath);
            }
        }
    }

    inputFile.close();
}

ParsedDocument DocumentParser::fetchDocumentData(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        exit(EXIT_FAILURE);
    }

    IStreamWrapper jsonStream(inputFile);
    Document jsonDoc;
    jsonDoc.ParseStream(jsonStream);

    if (jsonDoc.HasParseError()) {
        cerr << "JSON parsing error in file " << filePath << ": " 
             << jsonDoc.GetParseError() << " at offset " 
             << jsonDoc.GetErrorOffset() << endl;
        inputFile.close();
        exit(EXIT_FAILURE);
    }

    ParsedDocument docData;
    if (jsonDoc.HasMember("title") && jsonDoc["title"].IsString()) {
        docData.title = jsonDoc["title"].GetString();
    }
    if (jsonDoc.HasMember("thread") && jsonDoc["thread"]["site"].IsString()) {
        docData.publication = jsonDoc["thread"]["site"].GetString();
    }
    if (jsonDoc.HasMember("text") && jsonDoc["text"].IsString()) {
        docData.text = jsonDoc["text"].GetString();
    }
    if (jsonDoc.HasMember("published") && jsonDoc["published"].IsString()) {
        docData.date = jsonDoc["published"].GetString();
    }

    inputFile.close();
    return docData;
}
