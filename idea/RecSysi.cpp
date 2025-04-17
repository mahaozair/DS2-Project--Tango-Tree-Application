#include "FinalTangi.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> allIndustries = {};
std::vector<std::string> allLocations = {};
// Function to read internships from a CSV file
std::vector<Internship*> readInternshipsFromCSV(const std::string& filename, 
                                                std::vector<std::string>& allIndustries, 
                                                std::vector<std::string>& allLocations) {
    std::vector<Internship*> internships;
    std::ifstream file(filename);
    
    // Define industry and location lists
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return internships;
    }
    
    std::string line;
    // Skip header line
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string title, location, industry;
        std::string remoteStr, paidStr, yearsExpStr, handsOnStr, weeklyHrsStr;
        
        // Parse CSV line
        std::getline(ss, title, ',');
        std::getline(ss, location, ',');
        std::getline(ss, remoteStr, ',');
        std::getline(ss, paidStr, ',');
        std::getline(ss, industry, ',');
        std::getline(ss, yearsExpStr, ',');
        std::getline(ss, handsOnStr, ',');
        std::getline(ss, weeklyHrsStr, ',');
        
        // Convert string values to appropriate types
        bool remote = (remoteStr == "1" || remoteStr == "true");
        bool paid = (paidStr == "1" || paidStr == "true");
        int yearsExp = std::stoi(yearsExpStr);
        bool handsOn = (handsOnStr == "1" || handsOnStr == "true");
        int weeklyHrs = std::stoi(weeklyHrsStr);
        
        // Create and add the internship
        Internship* intern = new Internship(title, location, remote, paid, industry, 
                                           yearsExp, handsOn, weeklyHrs, 
                                           allIndustries, allLocations);
        internships.push_back(intern);
    }
    
    file.close();
    return internships;
}

int main() {
    // Read internships from CSV file
    std::string csvFilename = "C:\\Users\\haris\\project_ds2\\DS2-Project--Tango-Tree-Application\\idea\\internship.csv";
    std::vector<Internship*> internships = readInternshipsFromCSV(csvFilename, allIndustries, allLocations);
    
    // Check if internships were loaded successfully
    if (internships.empty()) {
        std::cerr << "No internships loaded from CSV file." << std::endl;
        return 1;
    }
    
    std::cout << "Loaded " << internships.size() << " internships from CSV file." << std::endl;
    
    // Initialize the TangoTree
    TangoTree tree;
    for (Internship* intern : internships) {
        tree.insertInternship(intern);
    }
    
    // Print the tree structure
    // tree.printTree();
    for(int i = 0; i < allIndustries.size(); i++) {
        std::cout << i << " " << allIndustries[i] << std::endl;
    }
    
    // Example: Search for a specific category ID
    int categoryId = internships[0]->CategoryId;
    std::cout << "\nSearching for CategoryId: " << categoryId << std::endl;
    std::vector<Internship*> results = tree.search(categoryId);
    
    std::cout << "Found " << results.size() << " internships:" << std::endl;
    for (Internship* intern : results) {
        std::cout << "- " << intern->title << " (Industry: " << allIndustries[intern->industry] << ")" << std::endl;
    }
    
    // Clean up
    for (Internship* intern : internships) {
        delete intern;
    }
    
    return 0;
}
