#include <iostream>
#include <string>
#include "QueryEngine.h"
#include "IndexHandler.h"

void displayMenu() {
    std::cout << "Welcome to the Search Engine!" << std::endl;
    std::cout << "\nPlease choose an option (1-5):" << std::endl;
    std::cout << "1) Enter a query" << std::endl;
    std::cout << "2) Display search engine statistics" << std::endl;
    std::cout << "3) Save index to file" << std::endl;
    std::cout << "4) Load index from file" << std::endl;
    std::cout << "5) Quit" << std::endl;
}

int main() {
    QueryEngine queryEngine; // Initialize QueryEngine
    bool running = true;

    while (running) {
        displayMenu();

        // Get user input
        std::string input;
        std::getline(std::cin, input);

        try {
            int option = std::stoi(input); // Convert input to an integer

            switch (option) {
            case 1: { // Query
                std::cout << "Enter your search query:" << std::endl;
                std::string query;
                std::getline(std::cin, query);

                // Parse and display results
                std::vector<std::pair<double, std::string>> results = queryEngine.parseQuery(query);
                if (results.empty()) {
                    std::cout << "No results found for the query: " << query << std::endl;
                } else {
                    std::cout << "Search Results (sorted by relevance):" << std::endl;
                    int rank = 1;
                    for (const auto& result : results) {
                        std::cout << rank << ") Document: " << result.second << " | Score: " << result.first << std::endl;
                        rank++;
                    }
                }
                break;
            }
            case 2: { // Statistics
                std::cout << "Search Engine Statistics:" << std::endl;
                std::cout << "Total Documents Indexed: " << queryEngine.ih->getDocSize() << std::endl;
                std::cout << "Total Unique Words: " << queryEngine.ih->returnSize() << std::endl;
                break;
            }
            case 3: { // Save index
                std::cout << "Saving index to file..." << std::endl;
                queryEngine.ih->createPersistence();
                std::cout << "Index saved successfully!" << std::endl;
                break;
            }
            case 4: { // Load index
                std::cout << "Loading index from file..." << std::endl;
                queryEngine.ih->readPersistence();
                std::cout << "Index loaded successfully!" << std::endl;
                break;
            }
            case 5: { // Quit
                std::cout << "Thank you for using the search engine. Goodbye!" << std::endl;
                running = false; // Exit the loop
                break;
            }
            default:
                std::cout << "Invalid option. Please enter a number between 1 and 5." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Invalid input. Please enter a valid number between 1 and 5." << std::endl;
        }
    }

    return 0;
}
