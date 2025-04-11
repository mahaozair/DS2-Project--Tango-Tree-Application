#include "FinalTang.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>  // For timing measurements

// Generate sample internships with duplicate relevance scores to test the vector functionality
std::vector<Internship*> generateSampleInternships() {
    return {
        // Multiple internships with same relevance scores
        new Internship(1, "Web Development Intern", "Lahore", {"HTML", "CSS", "JavaScript"}, 85),
        new Internship(2, "Data Science Intern", "Karachi", {"Python", "Pandas", "ML"}, 90),
        new Internship(3, "Marketing Intern", "Lahore", {"SEO", "Social Media"}, 70),
        new Internship(4, "AI Research Intern", "Islamabad", {"Python", "TensorFlow"}, 95),
        new Internship(5, "Frontend Developer Intern", "Lahore", {"React", "CSS"}, 85), // Same score as #1
        new Internship(6, "Backend Developer Intern", "Karachi", {"Node.js", "MongoDB"}, 90), // Same score as #2
        new Internship(7, "Mobile App Intern", "Islamabad", {"Flutter", "Dart"}, 80),
        new Internship(8, "DevOps Intern", "Karachi", {"Docker", "Kubernetes"}, 88),
        new Internship(9, "UX Design Intern", "Lahore", {"Figma", "User Research"}, 85), // Same score as #1, #5
        new Internship(10, "Game Development Intern", "Islamabad", {"Unity", "C#"}, 92),
        // Adding more internships for better demonstration of aux tree benefits
        new Internship(11, "System Admin Intern", "Karachi", {"Linux", "Bash"}, 75),
        new Internship(12, "Network Engineer Intern", "Lahore", {"Cisco", "Networking"}, 82),
        new Internship(13, "Database Admin Intern", "Islamabad", {"SQL", "Oracle"}, 87),
        new Internship(14, "QA Testing Intern", "Karachi", {"Selenium", "Testing"}, 78),
        new Internship(15, "Cybersecurity Intern", "Lahore", {"Security", "Penetration Testing"}, 92), // Same as #10
        new Internship(16, "Technical Writer Intern", "Islamabad", {"Documentation", "Writing"}, 65),
        new Internship(17, "UI Designer Intern", "Karachi", {"UI/UX", "Adobe XD"}, 85), // Same as #1, #5, #9
        new Internship(18, "Machine Learning Intern", "Lahore", {"PyTorch", "Scikit-learn"}, 94),
        new Internship(19, "Cloud Engineer Intern", "Islamabad", {"AWS", "Azure"}, 91),
        new Internship(20, "Data Analyst Intern", "Karachi", {"Excel", "Power BI"}, 83)
    };
}

// Print internship details
void printInternship(Internship* intern) {
    std::cout << "ID: " << std::setw(2) << intern->id 
              << " | Title: " << std::setw(28) << std::left << intern->title 
              << " | Location: " << std::setw(10) << std::left << intern->location 
              << " | Score: " << std::setw(3) << std::left << intern->relevanceScore
              << " | Skills: ";
    
    for (size_t i = 0; i < intern->requiredSkills.size(); ++i) {
        std::cout << (i > 0 ? ", " : "") << intern->requiredSkills[i];
    }
    std::cout << std::endl;
}

// Time a search operation and return the duration in microseconds
long long timeSearch(TangoTree& treeManager, int score) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Internship*> results = treeManager.search(score);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::micro> duration = end - start;
    return duration.count();
}

int main() {
    // Generate sample internships
    std::vector<Internship*> internships = generateSampleInternships();
    
    // Print all internships
    std::cout << "===== All Internships =====" << std::endl;
    for (Internship* intern : internships) {
        printInternship(intern);
    }
    std::cout << std::endl;
    
    // Build Tango Tree
    TangoTree treeManager;
    for (Internship* intern : internships) {
        treeManager.insertInternship(intern);
    }
    
    // Print initial tree structure
    std::cout << "===== Initial Tree Structure =====" << std::endl;
    treeManager.printTree();
    std::cout << std::endl;
    
    // First search for 85 - should be the slowest as no aux trees have been built yet
    std::cout << "===== First Search for Score 85 =====" << std::endl;
    long long duration1 = timeSearch(treeManager, 85);
    std::vector<Internship*> results85 = treeManager.search(85);
    std::cout << "Found " << results85.size() << " internships with score 85:" << std::endl;
    for (Internship* intern : results85) {
        printInternship(intern);
    }
    std::cout << "First search time: " << duration1 << " microseconds" << std::endl;
    std::cout << std::endl;
    
    // Second search for 85 - should be faster due to aux trees and preferred path
    std::cout << "===== Second Search for Score 85 =====" << std::endl;
    long long duration2 = timeSearch(treeManager, 85);
    results85 = treeManager.search(85);
    std::cout << "Found " << results85.size() << " internships with score 85:" << std::endl;
    for (Internship* intern : results85) {
        printInternship(intern);
    }
    std::cout << "Second search time: " << duration2 << " microseconds" << std::endl;
    std::cout << "Speed improvement: " << (duration1 / (duration2 > 0 ? duration2 : 1)) << "x faster" << std::endl;
    std::cout << std::endl;
    
    // Third search for 85 - should be very fast now
    std::cout << "===== Third Search for Score 85 =====" << std::endl;
    long long duration3 = timeSearch(treeManager, 85);
    results85 = treeManager.search(85);
    std::cout << "Found " << results85.size() << " internships with score 85:" << std::endl;
    for (Internship* intern : results85) {
        printInternship(intern);
    }
    std::cout << "Third search time: " << duration3 << " microseconds" << std::endl;
    std::cout << "Speed improvement from first search: " << (duration1 / (duration3 > 0 ? duration3 : 1)) << "x faster" << std::endl;
    std::cout << std::endl;
    
    // Search for internships with relevance score 90
    std::cout << "===== Search Results for Score 90 =====" << std::endl;
    long long duration90 = timeSearch(treeManager, 90);
    std::vector<Internship*> results90 = treeManager.search(90);
    std::cout << "Found " << results90.size() << " internships with score 90:" << std::endl;
    for (Internship* intern : results90) {
        printInternship(intern);
    }
    std::cout << "Search time: " << duration90 << " microseconds" << std::endl;
    std::cout << std::endl;
    
    // Search for non-existent score
    std::cout << "===== Search Results for Score 100 =====" << std::endl;
    long long duration100 = timeSearch(treeManager, 100);
    std::vector<Internship*> results100 = treeManager.search(100);
    std::cout << "Found " << results100.size() << " internships with score 100" << std::endl;
    std::cout << "Search time: " << duration100 << " microseconds" << std::endl;
    std::cout << std::endl;
    
    // Delete internships with score 85
    std::cout << "===== Deleting Internships with Score 85 =====" << std::endl;
    treeManager.deleteInternship(85);
    
    // Check if deletion was successful
    long long durationAfterDelete = timeSearch(treeManager, 85);
    std::vector<Internship*> resultsAfterDelete = treeManager.search(85);
    std::cout << "After deletion: Found " << resultsAfterDelete.size() 
              << " internships with score 85" << std::endl;
    std::cout << "Search time after deletion: " << durationAfterDelete << " microseconds" << std::endl;
    std::cout << std::endl;
    
    // Print tree structure after deletion
    std::cout << "===== Tree Structure After Deletion =====" << std::endl;
    treeManager.printTree();
    std::cout << std::endl;
    
    // Insert a new internship with score 85
    std::cout << "===== Inserting New Internship with Score 85 =====" << std::endl;
    Internship* newIntern = new Internship(21, "Cloud Computing Intern", "Lahore", 
                                        {"AWS", "Azure", "GCP"}, 85);
    treeManager.insertInternship(newIntern);
    
    // Verify insertion and check timing
    long long durationAfterInsert = timeSearch(treeManager, 85);
    std::vector<Internship*> resultsAfterInsert = treeManager.search(85);
    std::cout << "After insertion: Found " << resultsAfterInsert.size() 
              << " internship(s) with score 85:" << std::endl;
    for (Internship* intern : resultsAfterInsert) {
        printInternship(intern);
    }
    std::cout << "Search time after insertion: " << durationAfterInsert << " microseconds" << std::endl;
    std::cout << std::endl;
    
    // Print final tree structure
    std::cout << "===== Final Tree Structure =====" << std::endl;
    treeManager.printTree();
    
    // Performance summary
    std::cout << "\n===== Tango Tree Performance Summary =====" << std::endl;
    std::cout << "First search:  " << duration1 << " microseconds" << std::endl;
    std::cout << "Second search: " << duration2 << " microseconds" << std::endl;
    std::cout << "Third search:  " << duration3 << " microseconds" << std::endl;
    std::cout << "Improvement from first to third search: " 
              << ((duration1 - duration3) / static_cast<double>(duration1) * 100) << "%" << std::endl;
    
    // Clean up internship memory
    for (Internship* intern : internships) {
        delete intern;
    }
    delete newIntern;
    
    return 0;
}