#include "TangoTree.hpp"
#include <iostream>
#include <stack>
#include <algorithm>
#include <functional>
#include <map>  // Add this include for std::map

using namespace std;

TangoTree::TangoTree() : referenceRoot(nullptr), root(nullptr) {}

TangoTree::~TangoTree() {
    clearTree(referenceRoot);
    // Note: auxTrees nodes are destroyed when their preferred nodes are destroyed
}

int TangoTree::height(TreeNode* node) {
    return node ? node->height : 0;
}

int TangoTree::balanceFactor(TreeNode* node) {
    return height(node->left) - height(node->right);
}

TreeNode* TangoTree::rotateRight(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x; // New root
}

TreeNode* TangoTree::rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y; // New root
}

TreeNode* TangoTree::insert(TreeNode* node, Internship* intern, int depth) {
    if (!node) return new TreeNode(intern, depth);

    // If we found a node with the same relevance score, add the internship to it
    if (intern->relevanceScore == node->key) {
        node->data.push_back(intern);
        return node;
    }
    
    if (intern->relevanceScore < node->key) {
        node->left = insert(node->left, intern, depth + 1);
    } else {
        node->right = insert(node->right, intern, depth + 1);
    }

    // Update the height of this ancestor node
    node->height = max(height(node->left), height(node->right)) + 1;

    // Balance the node
    int balance = balanceFactor(node);

    // Left heavy (left-left)
    if (balance > 1 && intern->relevanceScore < node->left->key)
        return rotateRight(node);

    // Right heavy (right-right)
    if (balance < -1 && intern->relevanceScore > node->right->key)
        return rotateLeft(node);

    // Left-Right (left-heavy right child)
    if (balance > 1 && intern->relevanceScore > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Left (right-heavy left child)
    if (balance < -1 && intern->relevanceScore < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

TreeNode* TangoTree::remove(TreeNode* node, int relevanceScore) {
    if (!node) return node;

    if (relevanceScore < node->key) {
        node->left = remove(node->left, relevanceScore);
    } else if (relevanceScore > node->key) {
        node->right = remove(node->right, relevanceScore);
    } else {
        // Found the node with the matching relevance score
        // If this is the last internship in this node, remove the node
        if (node->data.size() <= 1) {
            if (!node->left || !node->right) {
                TreeNode* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            }

            // Find in-order successor
            TreeNode* temp = node->right;
            while (temp && temp->left) temp = temp->left;

            // Copy successor's key and data
            node->key = temp->key;
            node->data = temp->data;
            
            // Delete the successor
            node->right = remove(node->right, temp->key);
        } else {
            // Remove just one internship (last one for simplicity)
            // In a real application, you might want to specify which one to remove
            node->data.pop_back();
        }
    }

    if (!node) return node;

    // Update height
    node->height = max(height(node->left), height(node->right)) + 1;

    // Balance the node
    int balance = balanceFactor(node);

    // Left heavy
    if (balance > 1 && balanceFactor(node->left) >= 0)
        return rotateRight(node);

    // Right heavy
    if (balance < -1 && balanceFactor(node->right) <= 0)
        return rotateLeft(node);

    // Left-right case
    if (balance > 1 && balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-left case
    if (balance < -1 && balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void TangoTree::storeInOrder(TreeNode* node, std::vector<Internship*>& interns) {
    if (!node) return;
    storeInOrder(node->left, interns);
    
    // Add all internships from this node
    for (Internship* intern : node->data) {
        interns.push_back(intern);
    }
    
    storeInOrder(node->right, interns);
}

AuxNode* TangoTree::buildAuxTree(TreeNode* node) {
    if (!node) return nullptr;
    
    std::vector<Internship*> interns;
    storeInOrder(node, interns);
    
    // Group internships by relevance score
    std::map<int, std::vector<Internship*>> scoreGroups;
    for (Internship* intern : interns) {
        scoreGroups[intern->relevanceScore].push_back(intern);
    }
    
    // Create a vector of unique scores ordered
    std::vector<int> scores;
    for (const auto& group : scoreGroups) {
        scores.push_back(group.first);
    }
    
    // Recursively build a balanced aux tree
    std::function<AuxNode*(int, int, int)> build = [&](int low, int high, int depth) -> AuxNode* {
        if (low > high) return nullptr;
        int mid = (low + high) / 2;
        int score = scores[mid];
        
        // Create node with first internship, then add the rest
        AuxNode* auxNode = new AuxNode(scoreGroups[score][0], depth);
        
        // Add remaining internships with the same score
        for (size_t i = 1; i < scoreGroups[score].size(); ++i) {
            auxNode->data.push_back(scoreGroups[score][i]);
        }
        
        auxNode->left = build(low, mid - 1, depth + 1);
        auxNode->right = build(mid + 1, high, depth + 1);
        return auxNode;
    };
    
    return !scores.empty() ? build(0, scores.size() - 1, node->depth) : nullptr;
}

void TangoTree::clearTree(TreeNode* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    
    auto it = auxTrees.find(node);
    if (it != auxTrees.end()) {
        std::stack<AuxNode*> auxStack;
        auxStack.push(it->second);
        while (!auxStack.empty()) {
            AuxNode* current = auxStack.top();
            auxStack.pop();
            if (current->left) auxStack.push(current->left);
            if (current->right) auxStack.push(current->right);
            delete current;
        }
        auxTrees.erase(it);
    }
    
    delete node;
}

void TangoTree::insertInternship(Internship* intern) {
    if (!referenceRoot) {
        referenceRoot = new TreeNode(intern, 0);
        root = referenceRoot;
        return;
    }

    referenceRoot = insert(referenceRoot, intern, 0);
    root = referenceRoot;
}

std::vector<Internship*> TangoTree::search(int relevanceScore) {
    std::vector<Internship*> result;
    if (!root) return result;
    
    TreeNode* current = root;
    while (current) {
        if (relevanceScore == current->key) {
            return current->data; // Return all internships with matching score
        }
        current = (relevanceScore < current->key) ? current->left : current->right;
    }

    return result; // Empty vector if not found
}

void TangoTree::deleteInternship(int relevanceScore) {
    if (!root) return;
    root = remove(root, relevanceScore);
}

void TangoTree::printTree() {
    std::cout << "\nTree Structure:\n";
    printTreeNode(root, 0, true);
}

void TangoTree::printTreeNode(TreeNode* node, int depth, bool isPreferred) {
    if (!node) return;

    for (int i = 0; i < depth; ++i) std::cout << "    ";
    std::cout << (isPreferred ? "- " : "  ") << "Node with Score: " << node->key 
              << " (Internships: " << node->data.size() << ")\n";
              
    // Print all internships in this node
    for (int i = 0; i < depth + 1; ++i) std::cout << "    ";
    for (size_t i = 0; i < node->data.size(); ++i) {
        std::cout << (i == 0 ? "Contains: " : ", ") << node->data[i]->title;
    }
    std::cout << "\n";
    
    printTreeNode(node->left, depth + 1, node->preferredChild == node->left);
    printTreeNode(node->right, depth + 1, node->preferredChild == node->right);
}

void TangoTree::printAuxNode(AuxNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; ++i) std::cout << "    ";
    std::cout << "Node with Score: " << node->key 
              << " (Internships: " << node->data.size() << ")\n";
              
    // Print all internships in this node
    for (int i = 0; i < depth + 1; ++i) std::cout << "    ";
    for (size_t i = 0; i < node->data.size(); ++i) {
        std::cout << (i == 0 ? "Contains: " : ", ") << node->data[i]->title;
    }
    std::cout << "\n";
    
    printAuxNode(node->left, depth + 1);
    printAuxNode(node->right, depth + 1);
}
