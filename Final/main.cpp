#include "FinalTang.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>  // For timing measurements

std::vector<Internship*> generateSampleInternships() {
    std::vector<Internship*> internships = {
        new Internship(1, "Web Development Intern", "Lahore", {"HTML", "CSS", "JavaScript"}, 85),
        new Internship(2, "Data Science Intern", "Karachi", {"Python", "Pandas", "ML"}, 90),
        new Internship(3, "Marketing Intern", "Lahore", {"SEO", "Social Media"}, 70),
        new Internship(4, "AI Research Intern", "Islamabad", {"Python", "TensorFlow"}, 95),
        new Internship(5, "Frontend Developer Intern", "Lahore", {"React", "CSS"}, 85),
        new Internship(6, "Backend Developer Intern", "Karachi", {"Node.js", "MongoDB"}, 90),
        new Internship(7, "Mobile App Intern", "Islamabad", {"Flutter", "Dart"}, 80),
        new Internship(8, "DevOps Intern", "Karachi", {"Docker", "Kubernetes"}, 88),
        new Internship(9, "UX Design Intern", "Lahore", {"Figma", "User Research"}, 85),
        new Internship(10, "Game Development Intern", "Islamabad", {"Unity", "C#"}, 92),
        new Internship(11, "System Admin Intern", "Karachi", {"Linux", "Bash"}, 75),
        new Internship(12, "Network Engineer Intern", "Lahore", {"Cisco", "Networking"}, 82),
        new Internship(13, "Database Admin Intern", "Islamabad", {"SQL", "Oracle"}, 87),
        new Internship(14, "QA Testing Intern", "Karachi", {"Selenium", "Testing"}, 78),
        new Internship(15, "Cybersecurity Intern", "Lahore", {"Security", "Penetration Testing"}, 92),
        new Internship(16, "Technical Writer Intern", "Islamabad", {"Documentation", "Writing"}, 65),
        new Internship(17, "UI Designer Intern", "Karachi", {"UI/UX", "Adobe XD"}, 85),
        new Internship(18, "Machine Learning Intern", "Lahore", {"PyTorch", "Scikit-learn"}, 94),
        new Internship(19, "Cloud Engineer Intern", "Islamabad", {"AWS", "Azure"}, 91),
        new Internship(20, "Data Analyst Intern", "Karachi", {"Excel", "Power BI"}, 83)
    };
    
    // Set additional attributes for each internship
    
    // Tech industry internships
    internships[0]->paid = true;
    internships[0]->years_of_experience = 1;
    internships[0]->hands_on = true;
    internships[0]->industry = "Tech";
    internships[0]->weekly_hours = 30;
    
    internships[1]->paid = true;
    internships[1]->years_of_experience = 2;
    internships[1]->hands_on = true;
    internships[1]->industry = "Tech";
    internships[1]->weekly_hours = 40;
    
    internships[4]->paid = true;
    internships[4]->years_of_experience = 1;
    internships[4]->hands_on = true;
    internships[4]->industry = "Tech";
    internships[4]->weekly_hours = 25;
    
    internships[5]->paid = true;
    internships[5]->years_of_experience = 2;
    internships[5]->hands_on = true;
    internships[5]->industry = "Tech";
    internships[5]->weekly_hours = 35;
    
    internships[6]->paid = true;
    internships[6]->years_of_experience = 1;
    internships[6]->hands_on = true;
    internships[6]->industry = "Tech";
    internships[6]->weekly_hours = 30;
    
    internships[7]->paid = true;
    internships[7]->years_of_experience = 3;
    internships[7]->hands_on = true;
    internships[7]->industry = "Tech";
    internships[7]->weekly_hours = 40;
    
    internships[9]->paid = true;
    internships[9]->years_of_experience = 2;
    internships[9]->hands_on = true;
    internships[9]->industry = "Tech";
    internships[9]->weekly_hours = 35;
    
    // Business/Marketing industry internships
    internships[2]->paid = false;
    internships[2]->years_of_experience = 0;
    internships[2]->hands_on = false;
    internships[2]->industry = "Business";
    internships[2]->weekly_hours = 20;
    
    internships[8]->paid = true;
    internships[8]->years_of_experience = 1;
    internships[8]->hands_on = true;
    internships[8]->industry = "Business";
    internships[8]->weekly_hours = 25;
    
    internships[16]->paid = true;
    internships[16]->years_of_experience = 1;
    internships[16]->hands_on = true;
    internships[16]->industry = "Business";
    internships[16]->weekly_hours = 20;
    
    internships[19]->paid = true;
    internships[19]->years_of_experience = 2;
    internships[19]->hands_on = true;
    internships[19]->industry = "Business";
    internships[19]->weekly_hours = 35;
    
    // AI/ML focused internships
    internships[3]->paid = true;
    internships[3]->years_of_experience = 3;
    internships[3]->hands_on = true;
    internships[3]->industry = "Tech";
    internships[3]->weekly_hours = 40;
    
    internships[17]->paid = true;
    internships[17]->years_of_experience = 3;
    internships[17]->hands_on = true;
    internships[17]->industry = "Tech";
    internships[17]->weekly_hours = 35;
    
    // IT/Infrastructure internships
    internships[10]->paid = true;
    internships[10]->years_of_experience = 1;
    internships[10]->hands_on = true;
    internships[10]->industry = "Tech";
    internships[10]->weekly_hours = 30;
    
    internships[11]->paid = true;
    internships[11]->years_of_experience = 2;
    internships[11]->hands_on = true;
    internships[11]->industry = "Tech";
    internships[11]->weekly_hours = 40;
    
    internships[12]->paid = true;
    internships[12]->years_of_experience = 2;
    internships[12]->hands_on = true;
    internships[12]->industry = "Tech";
    internships[12]->weekly_hours = 35;
    
    internships[14]->paid = true;
    internships[14]->years_of_experience = 3;
    internships[14]->hands_on = true;
    internships[14]->industry = "Tech";
    internships[14]->weekly_hours = 40;
    
    internships[18]->paid = true;
    internships[18]->years_of_experience = 2;
    internships[18]->hands_on = true;
    internships[18]->industry = "Tech";
    internships[18]->weekly_hours = 35;
    
    // Non-technical internships
    internships[13]->paid = false;
    internships[13]->years_of_experience = 0;
    internships[13]->hands_on = false;
    internships[13]->industry = "Tech";
    internships[13]->weekly_hours = 25;
    
    internships[15]->paid = false;
    internships[15]->years_of_experience = 0;
    internships[15]->hands_on = false;
    internships[15]->industry = "Business";
    internships[15]->weekly_hours = 15;
    
    return internships;
}

 
void clearInputBuffer(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int calculateRelevanceScore(Internship* intern,
    const bool preferPaid,
    const int minYearsExp,
    const bool preferHandsOn,
    const std::string& preferredIndustry,
    const int preferredMinHours) {
    int score = 0;
    
    // Adjust score based on attributes
    if (intern->paid == preferPaid) score += 5;
    
    if (intern->years_of_experience >= minYearsExp) {
        score += 5;
    }
    
    if (intern->hands_on == preferHandsOn) score += 5;
    
    if (intern->industry == preferredIndustry) {
        score += 5;
    }
    
    if (intern->weekly_hours >= preferredMinHours) {
        score += 5;
    }
    
    return score*4;
}

int main(){
    
 
 
    TangoTree tree;
    std::vector<Internship*> internships = generateSampleInternships();
   
   
    // User preferences
    bool wantPaid, wantHandsOn;
    int minYearsExp, minWeeklyHours;
    std::string preferredIndustry;
   
    // Get user preferences
    std::cout << "\n=== Internship Search Preferences ===\n";
   
    // Paid preference
    std::cout << "Search for paid internships? (1 for Yes, 0 for No): ";
    while (!(std::cin >> wantPaid) || (wantPaid != 0 && wantPaid != 1)) {
        std::cout << "Invalid input. Please enter 1 for Yes or 0 for No: ";
        clearInputBuffer();
    }
   
    // Years of experience
    std::cout << "Minimum years of experience required (0-5): ";
    while (!(std::cin >> minYearsExp) || minYearsExp < 0 || minYearsExp > 5) {
        std::cout << "Invalid input. Please enter a number between 0-5: ";
        clearInputBuffer();
    }
   
   
    // Hands-on preference
    std::cout << "Prefer hands-on internships? (1 for Yes, 0 for No): ";
    while (!(std::cin >> wantHandsOn) || (wantHandsOn != 0 && wantHandsOn != 1)) {
        std::cout << "Invalid input. Please enter 1 for Yes or 0 for No: ";
        clearInputBuffer();
    }
   
    // Industry preference
    clearInputBuffer();
    std::cout << "Preferred industry (Tech/Business/Healthcare/Any): ";
    std::getline(std::cin, preferredIndustry);
   
   
    // Weekly hours
    std::cout << "Minimum weekly hours you can commit (10-40): ";
    while (!(std::cin >> minWeeklyHours) || minWeeklyHours < 10 || minWeeklyHours > 40) {
        std::cout << "Invalid input. Please enter a number between 10-40: ";
        clearInputBuffer();
    }
   
   
    for (int i = 0;i < internships.size(); i++)
    {
        internships[i]->relevanceScore = calculateRelevanceScore(internships[i], wantPaid, minYearsExp, wantHandsOn, preferredIndustry, minWeeklyHours);
    }
    for (int i = 0;i < internships.size(); i++)
    {
        tree.insertInternship(internships[i]);
    }
 
    std::vector<Internship*> results;
 
    for (int i = 80; i < 100; i += 5) {
        std::vector<Internship*> found = tree.search(i);
        results.insert(results.end(), found.begin(), found.end());
    }
        // Display results
        std::cout << "\n=== Matching Internships ===\n";
    if (results.empty()) {
        std::cout << "No internships match all your criteria.\n";
    } else {
        for (Internship* intern : results) {
            std::cout << "\nID: " << intern->id
                      << "\nTitle: " << intern->title
                      << "\nLocation: " << intern->location
                      << "\nRelevance Score: " << intern->relevanceScore
                      << "\nPaid: " << (intern->paid ? "Yes" : "No")
                      << "\nYears of Experience: " << intern->years_of_experience
                      << "\nHands-on: " << (intern->hands_on ? "Yes" : "No")
                      << "\nIndustry: " << intern->industry
                      << "\nWeekly Hours: " << intern->weekly_hours
                      << "\nRequired Skills: ";
            for (const auto& skill : intern->requiredSkills) {
                std::cout << skill << ", ";
            }
            std::cout << "\n";
        }
    }
   
    // Clean up
    for (Internship* intern : internships) {
        delete intern;
    }
    return 0;
}
 

























// Time a search operation and return the duration in microseconds
// long long timeSearch(TangoTree& treeManager, int score) {
//     auto start = std::chrono::high_resolution_clock::now();
//     std::vector<Internship*> results = treeManager.search(score);
//     auto end = std::chrono::high_resolution_clock::now();
    
//     std::chrono::duration<double, std::micro> duration = end - start;
//     return duration.count();
// }

// int main() {
//     // Generate sample internships
//     std::vector<Internship*> internships = generateSampleInternships();
    
//     // Print all internships
//     std::cout << "===== All Internships =====" << std::endl;
//     for (Internship* intern : internships) {
//         printInternship(intern);
//     }
//     std::cout << std::endl;
    
//     // Build Tango Tree
//     TangoTree treeManager;
//     for (Internship* intern : internships) {
//         treeManager.insertInternship(intern);
//     }
    
//     // Print initial tree structure
//     std::cout << "===== Initial Tree Structure =====" << std::endl;
//     treeManager.printTree();
//     std::cout << std::endl;
    
//     // First search for 85 - should be the slowest as no aux trees have been built yet
//     std::cout << "===== First Search for Score 85 =====" << std::endl;
//     long long duration1 = timeSearch(treeManager, 85);
//     std::vector<Internship*> results85 = treeManager.search(85);
//     std::cout << "Found " << results85.size() << " internships with score 85:" << std::endl;
//     for (Internship* intern : results85) {
//         printInternship(intern);
//     }
//     std::cout << "First search time: " << duration1 << " microseconds" << std::endl;
//     std::cout << std::endl;
    
//     // Second search for 85 - should be faster due to aux trees and preferred path
//     std::cout << "===== Second Search for Score 85 =====" << std::endl;
//     long long duration2 = timeSearch(treeManager, 85);
//     results85 = treeManager.search(85);
//     std::cout << "Found " << results85.size() << " internships with score 85:" << std::endl;
//     for (Internship* intern : results85) {
//         printInternship(intern);
//     }
//     std::cout << "Second search time: " << duration2 << " microseconds" << std::endl;
//     std::cout << "Speed improvement: " << (duration1 / (duration2 > 0 ? duration2 : 1)) << "x faster" << std::endl;
//     std::cout << std::endl;
    
//     // Third search for 85 - should be very fast now
//     std::cout << "===== Third Search for Score 85 =====" << std::endl;
//     long long duration3 = timeSearch(treeManager, 85);
//     results85 = treeManager.search(85);
//     std::cout << "Found " << results85.size() << " internships with score 85:" << std::endl;
//     for (Internship* intern : results85) {
//         printInternship(intern);
//     }
//     std::cout << "Third search time: " << duration3 << " microseconds" << std::endl;
//     std::cout << "Speed improvement from first search: " << (duration1 / (duration3 > 0 ? duration3 : 1)) << "x faster" << std::endl;
//     std::cout << std::endl;
    
//     // Search for internships with relevance score 90
//     std::cout << "===== Search Results for Score 90 =====" << std::endl;
//     long long duration90 = timeSearch(treeManager, 90);
//     std::vector<Internship*> results90 = treeManager.search(90);
//     std::cout << "Found " << results90.size() << " internships with score 90:" << std::endl;
//     for (Internship* intern : results90) {
//         printInternship(intern);
//     }
//     std::cout << "Search time: " << duration90 << " microseconds" << std::endl;
//     std::cout << std::endl;
    
//     // Search for non-existent score
//     std::cout << "===== Search Results for Score 100 =====" << std::endl;
//     long long duration100 = timeSearch(treeManager, 100);
//     std::vector<Internship*> results100 = treeManager.search(100);
//     std::cout << "Found " << results100.size() << " internships with score 100" << std::endl;
//     std::cout << "Search time: " << duration100 << " microseconds" << std::endl;
//     std::cout << std::endl;
    
//     // Delete internships with score 85
//     std::cout << "===== Deleting Internships with Score 85 =====" << std::endl;
//     treeManager.deleteInternship(85, "Cloud Engineer Intern");
    
//     // Check if deletion was successful
//     long long durationAfterDelete = timeSearch(treeManager, 85);
//     std::vector<Internship*> resultsAfterDelete = treeManager.search(85);
//     std::cout << "After deletion: Found " << resultsAfterDelete.size() 
//               << " internships with score 85" << std::endl;
//     std::cout << "Search time after deletion: " << durationAfterDelete << " microseconds" << std::endl;
//     std::cout << std::endl;
    
//     // Print tree structure after deletion
//     std::cout << "===== Tree Structure After Deletion =====" << std::endl;
//     treeManager.printTree();
//     std::cout << std::endl;
    
//     // Insert a new internship with score 85
//     std::cout << "===== Inserting New Internship with Score 85 =====" << std::endl;
//     Internship* newIntern = new Internship(21, "Cloud Computing Intern", "Lahore", 
//                                         {"AWS", "Azure", "GCP"}, 85);
//     treeManager.insertInternship(newIntern);
    
//     // Verify insertion and check timing
//     long long durationAfterInsert = timeSearch(treeManager, 85);
//     std::vector<Internship*> resultsAfterInsert = treeManager.search(85);
//     std::cout << "After insertion: Found " << resultsAfterInsert.size() 
//               << " internship(s) with score 85:" << std::endl;
//     for (Internship* intern : resultsAfterInsert) {
//         printInternship(intern);
//     }
//     std::cout << "Search time after insertion: " << durationAfterInsert << " microseconds" << std::endl;
//     std::cout << std::endl;
    
//     // Print final tree structure
//     std::cout << "===== Final Tree Structure =====" << std::endl;
//     treeManager.printTree();
    
//     // Performance summary
//     std::cout << "\n===== Tango Tree Performance Summary =====" << std::endl;
//     std::cout << "First search:  " << duration1 << " microseconds" << std::endl;
//     std::cout << "Second search: " << duration2 << " microseconds" << std::endl;
//     std::cout << "Third search:  " << duration3 << " microseconds" << std::endl;
//     std::cout << "Improvement from first to third search: " 
//               << ((duration1 - duration3) / static_cast<double>(duration1) * 100) << "%" << std::endl;
    
//     // Clean up internship memory
//     for (Internship* intern : internships) {
//         delete intern;
//     }
//     delete newIntern;
    
//     return 0;
// }