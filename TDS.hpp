#ifndef TDS_HPP
#define TDS_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

struct Internship
{
    int id;
    std::string title;
    std::string location;
    std::vector<std::string> requiredSkills;
    int relevanceScore; // used as the key in the Tango Tree

    Internship(int _id, std::string _title, std::string _location,
               std::vector<std::string> _skills, int score)
        : id(_id), title(_title), location(_location),
          requiredSkills(_skills), relevanceScore(score) {}
};

struct TreeNode
{
    Internship *data;
    TreeNode *left;
    TreeNode *right;
    TreeNode *preferredChild;
    int depth; // Depth in the reference tree

    TreeNode(Internship *_data, int _depth = 0)
        : data(_data), left(nullptr), right(nullptr),
          preferredChild(nullptr), depth(_depth) {}
};

struct AuxNode
{
    Internship *data;
    AuxNode *left;
    AuxNode *right;
    int depth; // Depth in the reference tree

    AuxNode(Internship *_data, int _depth = 0)
        : data(_data), left(nullptr), right(nullptr), depth(_depth) {}
};

class TangoTree
{
private:
    TreeNode *referenceRoot;
    TreeNode *root;                                     // Current root of the Tango tree
    std::unordered_map<TreeNode *, AuxNode *> auxTrees; // Maps preferred nodes to their aux trees

    // Helper functions
    TreeNode *buildReferenceTree(std::vector<Internship *> &interns, int low, int high, int depth);
    void storeInOrder(TreeNode *node, std::vector<Internship *> &interns);
    void clearTree(TreeNode *node);

    // Tango-specific operations
    TreeNode *cut(TreeNode *node, int key);
    TreeNode *join(TreeNode *mainTree, TreeNode *auxTree);
    TreeNode *splay(TreeNode *node, int key);
    TreeNode *rotateRight(TreeNode *y);
    TreeNode *rotateLeft(TreeNode *x);
    AuxNode *buildAuxTree(TreeNode *node);
    void updateAuxTree(TreeNode *preferredNode);

public:
    TangoTree();
    ~TangoTree();

    // Public interface
    void insertInternship(Internship *intern);
    bool search(int relevanceScore);
    void deleteInternship(int relevanceScore);

    // Utility functions
    void printTree() const;
};

#endif // TDS_HPP