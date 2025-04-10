#include "TDS.hpp"
#include <iostream>
#include <vector>
std::vector<Internship*> generateMockInternships() {
    return {
        new Internship(1, "Web Development Intern", "Lahore", {"HTML", "CSS", "JavaScript"}, 85),
        new Internship(2, "Data Science Intern", "Karachi", {"Python", "Pandas", "ML"}, 90),
        new Internship(3, "Marketing Intern", "Lahore", {"SEO", "Social Media"}, 70),
        new Internship(4, "AI Research Intern", "Islamabad", {"Python", "TensorFlow"}, 95),
        new Internship(5, "Frontend Developer Intern", "Lahore", {"React", "CSS"}, 88),
        new Internship(6, "Backend Developer Intern", "Karachi", {"Node.js", "MongoDB"}, 82)
    };
};
int main() {
    // Generate and sort internships by relevance score
    std::vector<Internship*> internships = generateMockInternships();

    // Build TangoTree
    TangoTree recommender;
    for (Internship* i : internships) {
        recommender.insertInternship(i);
    }

    std::cout <<recommender.search(90);

    std::cout <<recommender.search(95);

    std::cout <<recommender.search(60);

    std::cout << "\n Deleting internship with score 90...\n";
    recommender.deleteInternship(90);

    std::cout <<recommender.search(90);
    recommender.printTree();
    return 0;
};

