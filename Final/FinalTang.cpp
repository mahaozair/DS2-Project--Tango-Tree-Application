#include "FinalTang.hpp"
#include <iostream>
#include <stack>
#include <algorithm>
#include <functional>
#include <map>
#include <queue>

using namespace std;

TangoTree::TangoTree() : referenceRoot(nullptr), root(nullptr) {}

TangoTree::~TangoTree() {
    clearTree(referenceRoot);
}

int TangoTree::height(TreeNode* node) {
    return node ? node->height : 0;
}

int TangoTree::balanceFactor(TreeNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

TreeNode* TangoTree::rotateRight(TreeNode* y) {
    if (!y || !y->left) return y;
    
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Maintain preferred child relationships
    if (y->preferredChild == x) {
        y->preferredChild = T2;
    }

    return x; // New root
}

TreeNode* TangoTree::rotateLeft(TreeNode* x) {
    if (!x || !x->right) return x;
    
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Maintain preferred child relationships
    if (x->preferredChild == y) {
        x->preferredChild = T2;
    }

    return y; // New root
}

TreeNode* TangoTree::splay(TreeNode* root, int key) {
    // Base cases: root is NULL or key is present at root
    if (root == nullptr || root->key == key)
        return root;

    // Mark the path we're traversing as preferred
    if (key < root->key) {
        if (root->left == nullptr) return root;
        
        root->preferredChild = root->left;
        
        // Zig-Zig (Left Left)
        if (key < root->left->key) {
            // Recursively bring the key as root of left-left
            root->left->left = splay(root->left->left, key);
            
            // First rotation for root
            root = rotateRight(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key) {
            // Recursively bring the key as root of left-right
            root->left->right = splay(root->left->right, key);
            
            // First rotation for root->left
            if (root->left->right != nullptr)
                root->left = rotateLeft(root->left);
        }
        
        // Second rotation for root (if left child exists)
        return (root->left == nullptr) ? root : rotateRight(root);
    }
    else { // key > root->key
        if (root->right == nullptr) return root;
        
        root->preferredChild = root->right;
        
        // Zig-Zag (Right Left)
        if (key < root->right->key) {
            // Recursively bring the key as root of right-left
            root->right->left = splay(root->right->left, key);
            
            // First rotation for root->right
            if (root->right->left != nullptr)
                root->right = rotateRight(root->right);
        }
        // Zig-Zig (Right Right)
        else if (key > root->right->key) {
            // Recursively bring the key as root of right-right
            root->right->right = splay(root->right->right, key);
            
            // First rotation for root
            root = rotateLeft(root);
        }
        
        // Second rotation for root (if right child exists)
        return (root->right == nullptr) ? root : rotateLeft(root);
    }
}

TreeNode* TangoTree::insert(TreeNode* node, Internship* intern, int depth) {
    // Standard BST insertion
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

    // Balance the node (AVL property)
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
                
                // Ensure we cleanup the aux tree associated with this node first
                auto it = auxTrees.find(node);
                if (it != auxTrees.end()) {
                    clearAuxTree(it->second);
                    auxTrees.erase(it);
                }
                
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

// Helper method to clear auxiliary tree
void TangoTree::clearAuxTree(AuxNode* node) {
    if (!node) return;
    
    std::stack<AuxNode*> auxStack;
    auxStack.push(node);
    
    while (!auxStack.empty()) {
        AuxNode* current = auxStack.top();
        auxStack.pop();
        
        if (current->left) auxStack.push(current->left);
        if (current->right) auxStack.push(current->right);
        
        delete current;
    }
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

void TangoTree::updateAuxTree(TreeNode* preferredNode) {
    if (!preferredNode) return;
    
    // Check if an aux tree already exists for this node
    auto it = auxTrees.find(preferredNode);
    if (it != auxTrees.end()) {
        // Clean up existing aux tree
        clearAuxTree(it->second);
    }
    
    // Build a new aux tree for this preferred node
    auxTrees[preferredNode] = buildAuxTree(preferredNode);
}

void TangoTree::updatePreferredPath(TreeNode* node, int key) {
    if (!node) return;
    
    // Set up the preferred path
    TreeNode* current = node;
    
    while (current) {
        if (key < current->key) {
            if (current->left) {
                current->preferredChild = current->left;
                // Update the auxiliary tree for this non-preferred node
                updateAuxTree(current);
                current = current->left;
            } else {
                break;
            }
        } else if (key > current->key) {
            if (current->right) {
                current->preferredChild = current->right;
                // Update the auxiliary tree for this non-preferred node
                updateAuxTree(current);
                current = current->right;
            } else {
                break;
            }
        } else {
            // Found the key
            break;
        }
    }
}

void TangoTree::clearTree(TreeNode* node) {
    if (!node) return;
    
    // Use iterative approach for tree deletion to avoid stack overflow
    std::stack<TreeNode*> nodeStack;
    nodeStack.push(node);
    
    while (!nodeStack.empty()) {
        TreeNode* current = nodeStack.top();
        nodeStack.pop();
        
        if (current->right) nodeStack.push(current->right);
        if (current->left) nodeStack.push(current->left);
        
        // Clean up auxiliary tree associated with this node
        auto it = auxTrees.find(current);
        if (it != auxTrees.end()) {
            clearAuxTree(it->second);
            auxTrees.erase(it);
        }
        
        delete current;
    }
}

void TangoTree::insertInternship(Internship* intern) {
    if (!referenceRoot) {
        referenceRoot = new TreeNode(intern, 0);
        root = referenceRoot;
        return;
    }

    referenceRoot = insert(referenceRoot, intern, 0);
    root = referenceRoot;
    
    // Update preferred path to this inserted node
    updatePreferredPath(root, intern->relevanceScore);
}

std::vector<Internship*> TangoTree::search(int relevanceScore) {
    std::vector<Internship*> result;
    if (!root) return result;
    
    // First, update the preferred path for this search
    updatePreferredPath(root, relevanceScore);
    
    // Now splay the tree to bring the searched node (or closest node) to the top
    root = splay(root, relevanceScore);
    
    // If we found the exact node
    if (root && root->key == relevanceScore) {
        return root->data;
    }
    
    // Otherwise, search in auxiliary trees along the preferred path
    TreeNode* current = root;
    while (current) {
        // Check if this node has an auxiliary tree
        auto it = auxTrees.find(current);
        if (it != auxTrees.end()) {
            // Search in the auxiliary tree
            AuxNode* auxRoot = it->second;
            AuxNode* auxCurrent = auxRoot;
            
            while (auxCurrent) {
                if (relevanceScore == auxCurrent->key) {
                    return auxCurrent->data;
                }
                auxCurrent = (relevanceScore < auxCurrent->key) ? auxCurrent->left : auxCurrent->right;
            }
        }
        
        // Continue down the preferred path
        if (relevanceScore < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return result; // Empty vector if not found
}

void TangoTree::deleteInternship(int relevanceScore) {
    if (!root) return;
    
    // First, update the preferred path for this deletion
    updatePreferredPath(root, relevanceScore);
    
    // Splay the tree to bring the target node (or closest) to the top
    root = splay(root, relevanceScore);
    
    // Now remove the node
    root = remove(root, relevanceScore);
    
    // Update reference root
    referenceRoot = root;
}

void TangoTree::printTree() {
    std::cout << "\nTree Structure:\n";
    printTreeNode(root, 0, true);
    
    std::cout << "\nAuxiliary Trees:\n";
    for (const auto& pair : auxTrees) {
        std::cout << "Aux Tree for Node with Score: " << pair.first->key << "\n";
        printAuxNode(pair.second, 1);
    }
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