#ifndef TANGOTREE_HPP
#define TANGOTREE_HPP
#include <vector>
#include <unordered_map>
#include <string>

class Internship {
public:
    int id;
    int relevanceScore;
    std::string title;
    std::string location;
    std::vector<std::string> requiredSkills;
    
    Internship(int id, std::string title, std::string location,
               std::vector<std::string> skills, int score)
        : id(id), title(title), location(location),
          requiredSkills(skills), relevanceScore(score) {}
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
        : key(intern->relevanceScore), left(nullptr), right(nullptr), 
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
        : key(intern->relevanceScore), left(nullptr), right(nullptr), depth(d) {
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
    void deleteInternship(int relevanceScore);
    std::vector<Internship*> search(int relevanceScore); // Return vector of internships
    void printTree();
    void printTreeNode(TreeNode* node, int depth, bool isPreferred);
    void printAuxNode(AuxNode* node, int depth);
};

#endif // TANGOTREE_HPP