#include "TangoTree.hpp"
#include <iostream>
#include <vector>
#include <iomanip>

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
        new Internship(10, "Game Development Intern", "Islamabad", {"Unity", "C#"}, 92)
    };
}

// Print internship details
void printInternship(Internship* intern) {
    std::cout << "ID: " << std::setw(2) << intern->id 
              << " | Title: " << std::setw(25) << std::left << intern->title 
              << " | Location: " << std::setw(10) << std::left << intern->location 
              << " | Score: " << std::setw(3) << std::left << intern->relevanceScore
              << " | Skills: ";
    
    for (size_t i = 0; i < intern->requiredSkills.size(); ++i) {
        std::cout << (i > 0 ? ", " : "") << intern->requiredSkills[i];
    }
    std::cout << std::endl;
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
    
    // Search for internships with relevance score 85 (should return 3 internships)
    std::cout << "===== Search Results for Score 85 =====" << std::endl;
    std::vector<Internship*> results85 = treeManager.search(85);
    std::cout << "Found " << results85.size() << " internships with score 85:" << std::endl;
    for (Internship* intern : results85) {
        printInternship(intern);
    }
    std::cout << std::endl;
    
    // Search for internships with relevance score 90 (should return 2 internships)
    std::cout << "===== Search Results for Score 90 =====" << std::endl;
    std::vector<Internship*> results90 = treeManager.search(90);
    std::cout << "Found " << results90.size() << " internships with score 90:" << std::endl;
    for (Internship* intern : results90) {
        printInternship(intern);
    }
    std::cout << std::endl;
    
    // Search for non-existent score
    std::cout << "===== Search Results for Score 100 =====" << std::endl;
    std::vector<Internship*> results100 = treeManager.search(100);
    std::cout << "Found " << results100.size() << " internships with score 100" << std::endl;
    std::cout << std::endl;
    
    // Delete internships with score 85
    std::cout << "===== Deleting Internships with Score 85 =====" << std::endl;
    treeManager.deleteInternship(85);
    
    // Check if deletion was successful
    std::vector<Internship*> resultsAfterDelete = treeManager.search(85);
    std::cout << "After deletion: Found " << resultsAfterDelete.size() 
              << " internships with score 85" << std::endl;
    std::cout << std::endl;
    
    // Print tree structure after deletion
    std::cout << "===== Tree Structure After Deletion =====" << std::endl;
    treeManager.printTree();
    std::cout << std::endl;
    
    // Insert a new internship with score 85
    std::cout << "===== Inserting New Internship with Score 85 =====" << std::endl;
    Internship* newIntern = new Internship(11, "Cloud Computing Intern", "Lahore", 
                                        {"AWS", "Azure", "GCP"}, 85);
    treeManager.insertInternship(newIntern);
    
    // Verify insertion
    std::vector<Internship*> resultsAfterInsert = treeManager.search(85);
    std::cout << "After insertion: Found " << resultsAfterInsert.size() 
              << " internship(s) with score 85:" << std::endl;
    for (Internship* intern : resultsAfterInsert) {
        printInternship(intern);
    }
    std::cout << std::endl;
    
    // Print final tree structure
    std::cout << "===== Final Tree Structure =====" << std::endl;
    treeManager.printTree();
    
    // Clean up internship memory (in a real application)
    for (Internship* intern : internships) {
        delete intern;
    }
    delete newIntern;
    
    return 0;
}