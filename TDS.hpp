#include <unordered_map>
#include <string>
#include <vector>

struct Internship {
    int id;
    std::string title;
    std::string location;
    std::vector<std::string> requiredSkills;
    int relevanceScore; // used as the key in the Tango Tree

    Internship(int _id, std::string _title, std::string _location, std::vector<std::string> _skills, int score)
        : id(_id), title(_title), location(_location), requiredSkills(_skills), relevanceScore(score) {}
};


struct TreeNode {
    Internship* data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* preferredChild;

    TreeNode(Internship* _data)
        : data(_data), left(nullptr), right(nullptr), preferredChild(nullptr) {}
};

struct AuxNode {
    Internship* data;
    AuxNode* left;
    AuxNode* right;

    AuxNode(Internship* _data)
        : data(_data), left(nullptr), right(nullptr) {}
};



class TangoTree {
    private:
        TreeNode* referenceRoot;
        std::unordered_map<int, AuxNode*> auxTrees; // maps preferred path root to its aux tree
    
    public:
        TangoTree();
    
        void insertInternship(Internship* intern);
        void search(int relevanceScore); // core logic
        void deleteInternship(int relevanceScore);
    
        // internal helpers
        TreeNode* buildReferenceTree(std::vector<Internship*>& interns, int low, int high);
        TreeNode* searchReferenceTree(TreeNode* node, int key, std::vector<TreeNode*>& path);
        void updatePreferredPaths(int key);
    };
    