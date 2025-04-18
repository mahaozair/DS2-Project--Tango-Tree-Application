#include "FinalTangi.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> allIndustries = {};
std::vector<std::string> allLocations = {};



// Function to print details of internships in a vector
void printInternships(const std::vector<Internship*>& internships) {
    if (internships.empty()) {
        std::cout << "No matching internships found." << std::endl;
        return;
    }
    
    std::cout << "\n===== Internship Details =====" << std::endl;
    int count = 1;
    for (const Internship* intern : internships) {
        std::cout << "\n[Internship " << count++ << "]" << std::endl;
        std::cout << "Title: " << intern->title << std::endl;
        std::cout << "Location: " << intern->allLocations[intern->location] << std::endl;
        std::cout << "Industry: " << intern->allIndustries[intern->industry] << std::endl;
        std::cout << "Remote: " << (intern->remote ? "Yes" : "No") << std::endl;
        std::cout << "Paid: " << (intern->paid ? "Yes" : "No") << std::endl;
        std::cout << "Experience Required: " << intern->yearsExperience << " years" << std::endl;
        std::cout << "Hands-on: " << (intern->handsOn ? "Yes" : "No") << std::endl;
        
        // Convert weeklyHours back to actual hours (1-4 scale to 10-40 hours)
        int actualHours = intern->weeklyHours * 10;
        std::cout << "Weekly Hours: " << actualHours << std::endl;
        std::cout << "Category ID: " << intern->CategoryId << std::endl;
        std::cout << "----------------------------";
    }
    std::cout << std::endl;
}

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
    std::cout << std::endl;
    
    // Initialize the TangoTree
    TangoTree tree;
    for (Internship* intern : internships) {
        tree.insertInternship(intern);
    }
    
    // Print the tree structure
    // tree.printTree();
    
    int rank = 10;
    int industry = 0;
    int location = 0;
    int handsOn = 0;
    int remote = 0;
    int paid = 0;
    int maxExperience = 0;
    int maxWeeklyHours = 0;
    bool hasprefindustry = false;
    bool haspreflocation = false;
    bool hasprefremote = false;
    bool hasprefpaid = false;
    bool hasprefhandsOn = false;

    for(int i = 0; i < allIndustries.size(); i++) {
        std::cout << i << " " << allIndustries[i] << std::endl;
    }
    std::cout << allIndustries.size() << " " << "any" << std::endl;

    std::cout << "Enter your preferred industry (0-" << allIndustries.size() << "): ";
    std::cin >> industry;
    if(industry == allIndustries.size()) {
        hasprefindustry = true;
    } else {
        hasprefindustry = false;
    }
    for(int i = 0; i < allLocations.size(); i++) {
        std::cout << i << " " << allLocations[i] << std::endl;
    }
    std::cout << allLocations.size() << " " << "any" << std::endl;
    std::cout << "Enter your preferred location (0-" << allLocations.size() << "): ";
    std::cin >> location;
    if(location == allLocations.size()) {
        haspreflocation = true;
    } else {
        haspreflocation = false;
    }
    std::cout << "Enter your maximum experience (0-4): ";
    std::cin >> maxExperience;
    std::cout << "Enter your maximum weekly hours in multiples of 10 (10-40): ";
    std::cin >> maxWeeklyHours;
    std::cout << "Do you have a preferrence regarding remote or on-site internships? (1 for Yes, 0 for No): ";
    std::cin >> hasprefremote;
    if(hasprefremote) {
        std::cout << "Do you prefer remote internships? (1 for Yes, 0 for No): ";
        std::cin >> remote;
    } else {
        remote = 0;
    }
    std::cout << "Do you have a preferrence regarding paid or unpaid internships? (1 for Yes, 0 for No): ";
    std::cin >> hasprefpaid;
    if(hasprefpaid) {
        std::cout << "Do you prefer paid internships? (1 for Yes, 0 for No): ";
        std::cin >> paid;
    } else {
        paid = 0;
    }
    std::cout << "Do you have a preferrence regarding hands-on or research-based internships? (1 for Yes, 0 for No): ";
    std::cin >> hasprefhandsOn;
    if(hasprefhandsOn) {
        std::cout << "Do you prefer hands-on internships? (1 for Yes, 0 for No): ";
        std::cin >> handsOn;
    } else {
        handsOn = 0;
    }
    std::cout << "How many most relevant internships do you want to see? : ";
    std::cin >> rank;
    if(rank > internships.size()) {
        rank = internships.size();
    }
    if(rank < 0) {
        rank = 0;
    }

    UserPreferences userPref(remote, paid, allIndustries[industry], maxExperience, handsOn, maxWeeklyHours, allIndustries, allLocations, allLocations[location], hasprefremote, hasprefpaid, hasprefindustry, haspreflocation, hasprefhandsOn, rank);

    std::vector<Internship*> result;
    for(int i = 0; i < userPref.CategoryId.size(); i++) {
        result.insert(result.end(), tree.search(userPref.CategoryId[i]).begin(), tree.search(userPref.CategoryId[i]).end());
    }

    printInternships(result);



    
}
