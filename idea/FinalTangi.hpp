#ifndef TANGOTREE_HPP
#define TANGOTREE_HPP
#include <vector>
#include <unordered_map>
#include <string>

struct UserPreferences {
    bool preferRemote;
    bool requirePaid;
    std::string preferredIndustry;
    int maxExperience;
    bool preferHandsOn;
    int maxWeeklyHours;
    
    UserPreferences(bool remote = false, bool paid = true,
                   std::string industry = "", int maxExp = 0,
                   bool handsOn = false, int maxHrs = 40)
        : preferRemote(remote), 
          requirePaid(paid), 
          preferredIndustry(industry), 
          maxExperience(maxExp),
          preferHandsOn(handsOn), 
          maxWeeklyHours(maxHrs) {}
};
class Internship {
    private:
        int CalculateCategoryId();
    public:
        int CategoryId;
        std::string title;
        int location;
        // std::vector<std::string> requiredSkills;
        bool remote;
        bool paid;
        int industry;
        int yearsExperience;
        bool handsOn;
        int weeklyHours;
        
        Internship(std::string title, std::string location,
                   bool remote, bool paid = true,
                   std::string industry, int yearsExp ,
                   bool handsOn, int weeklyHrs);
           
    };

class TreeNode {
public:
    std::vector<Internship*> data; // Vector of internships
    int key; // Relevance score for all internships in this node
    TreeNode* left;
    TreeNode* right;
    TreeNode* preferredChild;
    int height;
    int depth;
    
    TreeNode(Internship* intern, int d)
        : key(intern->CategoryId), left(nullptr), right(nullptr), 
          preferredChild(nullptr), height(1), depth(d) {
        data.push_back(intern);
    }
};

class AuxNode {
public:
    std::vector<Internship*> data; // Vector of internships
    int key; // Relevance score for all internships in this node
    AuxNode* left;
    AuxNode* right;
    int depth;
    
    AuxNode(Internship* intern, int d)
        : key(intern->CategoryId), left(nullptr), right(nullptr), depth(d) {
        data.push_back(intern);
    }
};

class TangoTree {
private:
    TreeNode* root;
    TreeNode* referenceRoot;
    std::unordered_map<TreeNode*, AuxNode*> auxTrees;
    
    // Helper functions for AVL tree
    int height(TreeNode* node);
    int balanceFactor(TreeNode* node);
    TreeNode* rotateRight(TreeNode* y);
    TreeNode* rotateLeft(TreeNode* x);
    TreeNode* splay(TreeNode* node, int key);
    TreeNode* insert(TreeNode* node, Internship* intern, int depth);
    TreeNode* remove(TreeNode* node, int relevanceScore);
    
    // Auxiliary tree management
    void storeInOrder(TreeNode* node, std::vector<Internship*>& interns);
    AuxNode* buildAuxTree(TreeNode* node);
    void updateAuxTree(TreeNode* preferredNode);
    void updatePreferredPath(TreeNode* node, int key);
    void clearTree(TreeNode* node);
    void clearAuxTree(AuxNode* node);
    
public:
    TangoTree();
    ~TangoTree();
    
    // Public functions
    void insertInternship(Internship* intern);
    void deleteInternship(int CategoryId);
    std::vector<Internship*> search(int CategoryID); // Return vector of internships
    void printTree();
    void printTreeNode(TreeNode* node, int depth, bool isPreferred);
    void printAuxNode(AuxNode* node, int depth);
};

#endif // TANGOTREE_HPP