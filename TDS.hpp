// #ifndef TDS_HPP
// #define TDS_HPP

// #include <unordered_map>
// #include <string>
// #include <vector>
// #include <memory>

// struct Internship
// {
//     int id;
//     std::string title;
//     std::string location;
//     std::vector<std::string> requiredSkills;
//     int relevanceScore; // used as the key in the Tango Tree
//     //these things will be assigned at each insertion
//     bool paid;
//     int years_of_experience;
//     bool hands_on;
//     std::string industry;
//     bool remote;
//     int weekly_hours;

//     Internship(int _id, std::string _title, std::string _location,
//                std::vector<std::string> _skills, int score)
//         : id(_id), title(_title), location(_location),
//           requiredSkills(_skills), relevanceScore(score) {}
// };

// struct TreeNode
// {
//     Internship *data;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode *preferredChild;
//     int depth; // Depth in the reference tree

//     TreeNode(Internship *_data, int _depth = 0)
//         : data(_data), left(nullptr), right(nullptr),
//           preferredChild(nullptr), depth(_depth) {}
// };

// struct AuxNode
// {
//     Internship *data;
//     AuxNode *left;
//     AuxNode *right;
//     int depth; // Depth in the reference tree

//     AuxNode(Internship *_data, int _depth = 0)
//         : data(_data), left(nullptr), right(nullptr), depth(_depth) {}
// };

// class TangoTree
// {
// private:
//     TreeNode *referenceRoot;
//     TreeNode *root;                                     // Current root of the Tango tree
//     std::unordered_map<TreeNode *, AuxNode *> auxTrees; // Maps preferred nodes to their aux trees

//     // Helper functions
//     TreeNode *buildReferenceTree(std::vector<Internship *> &interns, int low, int high, int depth);
//     void storeInOrder(TreeNode *node, std::vector<Internship *> &interns);
//     void clearTree(TreeNode *node);

//     // Tango-specific operations
//     TreeNode *cut(TreeNode *node, int key);
//     TreeNode *join(TreeNode *mainTree, TreeNode *auxTree);
//     TreeNode *splay(TreeNode *node, int key);
//     TreeNode *rotateRight(TreeNode *y);
//     TreeNode *rotateLeft(TreeNode *x);
//     AuxNode *buildAuxTree(TreeNode *node);
//     void updateAuxTree(TreeNode *preferredNode);

// public:
//     //////for debugging may remove later
//     void printTreeNode(TreeNode *node, int depth, bool isPreferred);
//     void printAuxNode(AuxNode *node, int depth);
//     void printTree();
//     TangoTree();
//     ~TangoTree();

//     // Public interface
//     void insertInternship(Internship *intern);
//     std::string search(int relevanceScore);
//     void deleteInternship(int relevanceScore);

//     // Utility functions
//     void printTree() const;
// };

// #endif // TDS_HPP

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
    Internship(int _id, std::string _title, std::string _location,
               std::vector<std::string> _skills, int score)
        : id(_id), title(_title), location(_location),
          requiredSkills(_skills), relevanceScore(score) {}
};

class TreeNode {
public:
    Internship* data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* preferredChild;
    int height;
    int depth;

    TreeNode(Internship* intern, int d)
        : data(intern), left(nullptr), right(nullptr), preferredChild(nullptr), height(1), depth(d) {}
};

class AuxNode {
public:
    Internship* data;
    AuxNode* left;
    AuxNode* right;
    int depth;

    AuxNode(Internship* intern, int d)
        : data(intern), left(nullptr), right(nullptr), depth(d) {}
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
    void clearTree(TreeNode* node);

public:
    TangoTree();
    ~TangoTree();

    // Public functions
    void insertInternship(Internship* intern);
    void deleteInternship(int relevanceScore);
    std::string search(int relevanceScore);
    void printTree();
    void printTreeNode(TreeNode* node, int depth, bool isPreferred);
    void printAuxNode(AuxNode* node, int depth);
};

#endif // TANGOTREE_HPP
