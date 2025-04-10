// #include "TDS.hpp"
// #include <algorithm>
// #include <iostream>
// #include <stack>
// #include <functional>
// using namespace std;

// TangoTree::TangoTree() : referenceRoot(nullptr), root(nullptr) {}

// TangoTree::~TangoTree() {
//     clearTree(referenceRoot);
//     // Note: auxTrees nodes are destroyed when their preferred nodes are destroyed
// }

// // Recursively builds a balanced reference tree
// TreeNode* TangoTree::buildReferenceTree(std::vector<Internship*>& interns, int low, int high, int depth) {
//     if (low > high) return nullptr;
//     int mid = (low + high) / 2;
//     TreeNode* node = new TreeNode(interns[mid], depth);
//     node->left = buildReferenceTree(interns, low, mid - 1, depth + 1);
//     node->right = buildReferenceTree(interns, mid + 1, high, depth + 1);
//     return node;
// }

// // Stores internships in-order for rebuilding the tree
// void TangoTree::storeInOrder(TreeNode* node, std::vector<Internship*>& interns) {
//     if (!node) return;
//     storeInOrder(node->left, interns);
//     interns.push_back(node->data);
//     storeInOrder(node->right, interns);
// }

// // Clears the tree and deallocates memory
// void TangoTree::clearTree(TreeNode* node) {
//     if (!node) return;
//     clearTree(node->left);
//     clearTree(node->right);
    
//     // Clear associated aux tree if exists
//     auto it = auxTrees.find(node);
//     if (it != auxTrees.end()) {
//         // Recursively delete aux tree nodes
//         std::stack<AuxNode*> auxStack;
//         auxStack.push(it->second);
//         while (!auxStack.empty()) {
//             AuxNode* current = auxStack.top();
//             auxStack.pop();
//             if (current->left) auxStack.push(current->left);
//             if (current->right) auxStack.push(current->right);
//             delete current;
//         }
//         auxTrees.erase(it);
//     }
    
//     delete node;
// }

// /**
//  * Performs a right rotation around the given node.
//  * Used for rebalancing the tree during splay operations.
//  * 
//  * @param y The node to rotate around (will become the right child)
//  * @return The new root of this subtree after rotation
//  */
// TreeNode* TangoTree::rotateRight(TreeNode* y) { //so this shifts whole auxillary trees instead of just children 
    
//     if (!y || !y->left) return y; // Can't rotate
    
//     TreeNode *T2;
//     TreeNode* x = y->left;
    
//     // Perform rotation
//     x->right = y;
//     y->left = T2;
    
//     // Update preferred child pointers
//     if (T2) {
//         y->preferredChild = T2;
//     } else {
//         y->preferredChild = nullptr;
//     }
    
//     // Maintain auxiliary tree relationships
//     if (auxTrees.count(y)) {
//         // Transfer auxiliary tree to x if y had one
//         auxTrees[x] = auxTrees[y];
//         auxTrees.erase(y);
//     }
    
//     return x; // New root
// }

// /**
//  * Performs a left rotation around the given node.
//  * Used for rebalancing the tree during splay operations.
//  * 
//  * @param x The node to rotate around (will become the left child)
//  * @return The new root of this subtree after rotation
//  */
// TreeNode* TangoTree::rotateLeft(TreeNode* x) {
//     if (!x || !x->right) return x; // Can't rotate
    
//     TreeNode* y = x->right;
//     TreeNode* T2 = y->left;
    
//     // Perform rotation
//     y->left = x;
//     x->right = T2;
    
//     // Update preferred child pointers
//     if (T2) {
//         x->preferredChild = T2;
//     } else {
//         x->preferredChild = nullptr;
//     }
    
//     // Maintain auxiliary tree relationships
//     if (auxTrees.count(x)) {
//         // Transfer auxiliary tree to y if x had one
//         auxTrees[y] = auxTrees[x];
//         auxTrees.erase(x);
//     }
    
//     return y; // New root
// }

// // Splay operation for bringing nodes to the root
// TreeNode* TangoTree::splay(TreeNode* node, int key) {
//     if (!node || node->data->relevanceScore == key) return node;
    
//     if (key < node->data->relevanceScore) {
//         if (!node->left) return node;
        
//         // Zig-Zig (Left Left)
//         if (key < node->left->data->relevanceScore) {
//             node->left->left = splay(node->left->left, key);
//             node = rotateRight(node);
//         }
//         // Zig-Zag (Left Right)
//         else if (key > node->left->data->relevanceScore) {
//             node->left->right = splay(node->left->right, key);
//             if (node->left->right)
//                 node->left = rotateLeft(node->left);
//         }
//         return node->left ? rotateRight(node) : node;
//     }
//     else {
//         if (!node->right) return node;
        
//         // Zag-Zag (Right Right)
//         if (key > node->right->data->relevanceScore) {
//             node->right->right = splay(node->right->right, key);
//             node = rotateLeft(node);
//         }
//         // Zag-Zig (Right Left)
//         else if (key < node->right->data->relevanceScore) {
//             node->right->left = splay(node->right->left, key);
//             if (node->right->left)
//                 node->right = rotateRight(node->right);
//         }
//         return node->right ? rotateLeft(node) : node;
//     }
// }

// // // Rotates the tree right around the given node
// // TreeNode* TangoTree::rotateRight(TreeNode* y) {
// //     TreeNode* x = y->left;
// //     y->left = x->right;
// //     x->right = y;
// //     return x;
// // }

// // // Rotates the tree left around the given node
// // TreeNode* TangoTree::rotateLeft(TreeNode* x) {
// //     TreeNode* y = x->right;
// //     x->right = y->left;
// //     y->left = x;
// //     return y;
// // }

// // Cuts the tree at the node with the given key
// TreeNode* TangoTree::cut(TreeNode* node, int key) {
//     if (!node) return nullptr;
    
//     node = splay(node, key);
//     if (node->data->relevanceScore != key) return node;
    
//     // The right subtree becomes an auxiliary tree
//     TreeNode* aux = node->right;
//     node->right = nullptr;
    
//     // Build and store the auxiliary tree
//     auxTrees[node] = buildAuxTree(aux);
    
//     return node;
// }

// // // Joins a main tree with an auxiliary tree
// TreeNode* TangoTree::join(TreeNode* mainTree, TreeNode* auxTree) {
//     if (!mainTree) return auxTree;
    
//     // Bring the maximum element to the root
//     mainTree = splay(mainTree, INT_MAX);
//     mainTree->right = auxTree;
    
//     // Remove the auxiliary tree from our map
//     auto it = auxTrees.find(mainTree);
//     if (it != auxTrees.end()) {
//         delete it->second; // Clean up the old aux tree
//         auxTrees.erase(it);
//     }
    
//     return mainTree;
// }

// // TreeNode *TangoTree::cut(TreeNode *node, int key)
// // {
// //     if (!node)
// //         return nullptr;

// //     node = splay(node, key);

// //     if (node->data->relevanceScore != key)
// //         return node;

// //     TreeNode *rightSub = node->right;
// //     node->right = nullptr;

// //     // ✅ Traverse from root to find actual pointer to this node in the main tree
// //     TreeNode *knownPtr = root;
// //     while (knownPtr && knownPtr->data->relevanceScore != key)
// //     {
// //         if (key < knownPtr->data->relevanceScore)
// //             knownPtr = knownPtr->left;
// //         else
// //             knownPtr = knownPtr->right;
// //     }

// //     if (knownPtr && rightSub)
// //     {
// //         auxTrees[knownPtr] = buildAuxTree(rightSub);
// //         std::cout << "[DEBUG] Aux tree stored under: " << knownPtr->data->title << " (" << knownPtr << ")\n";
// //     }

// //     return node;
// // }


// // // Joins a main tree with an auxiliary tree
// // TreeNode *TangoTree::join(TreeNode *mainTree, TreeNode *auxTree)
// // {
// //     if (!mainTree)
// //         return auxTree;

// //     mainTree = splay(mainTree, INT_MAX);
// //     mainTree->right = auxTree;

// //     // Clear the auxiliary tree if it exists
// //     // auto it = auxTrees.find(mainTree);
// //     // if (it != auxTrees.end())
// //     // {
// //     //     delete it->second;
// //     //     auxTrees.erase(it);
// //     // }
// //     std::cout << "[DEBUG] join(): Checking if we should erase aux tree for " 
// //           << mainTree->data->title << "\n";
// //     return mainTree;
// // }

// // Builds an auxiliary tree from a subtree
// AuxNode* TangoTree::buildAuxTree(TreeNode* node) {
//     if (!node) return nullptr;
    
//     std::vector<Internship*> interns;
//     storeInOrder(node, interns);
    
//     // Recursively build a balanced aux tree
//     std::function<AuxNode*(int, int, int)> build = [&](int low, int high, int depth) -> AuxNode* {
//         if (low > high) return nullptr;
//         int mid = (low + high) / 2;
//         AuxNode* auxNode = new AuxNode(interns[mid], depth);
//         auxNode->left = build(low, mid - 1, depth + 1);
//         auxNode->right = build(mid + 1, high, depth + 1);
//         return auxNode;
//     };
    
//     return build(0, interns.size() - 1, node->depth);
// }

// // Updates the auxiliary tree for a preferred node
// void TangoTree::updateAuxTree(TreeNode* preferredNode) {
//     if (!preferredNode) return;
    
//     auto it = auxTrees.find(preferredNode);
//     if (it != auxTrees.end()) {
//         delete it->second; // Clean up the old aux tree
//     }
    
//     auxTrees[preferredNode] = buildAuxTree(preferredNode->right);
//     preferredNode->right = nullptr;
// }

// // Public insert function
// void TangoTree::insertInternship(Internship* intern) {
//     if (!referenceRoot) {
//         referenceRoot = new TreeNode(intern, 0);
//         root = referenceRoot;
//         return;
//     }
    
//     // Insert into the reference tree
//     std::vector<Internship*> interns;
//     storeInOrder(referenceRoot, interns);
//     interns.push_back(intern);
//     std::sort(interns.begin(), interns.end(), 
//         [](Internship* a, Internship* b) { return a->relevanceScore < b->relevanceScore; });
    
//     clearTree(referenceRoot);
//     referenceRoot = buildReferenceTree(interns, 0, interns.size() - 1, 0);
//     root = referenceRoot;
// };

// std::string TangoTree::search(int relevanceScore) {
//     if (!root) return "Not Found!";

//     // Search preferred path
//     TreeNode* current = root;
//     std::vector<TreeNode*> path;

//     while (current) {
//         path.push_back(current);
//         if (relevanceScore == current->data->relevanceScore) {
//             // Update preferred child pointers
//             for (size_t i = 0; i + 1 < path.size(); ++i) {
//                 path[i]->preferredChild = path[i + 1];
//             }

//             TreeNode* preferredRoot = path[0];
//             TreeNode* newRoot = splay(preferredRoot, relevanceScore);
//             if (preferredRoot == root) root = newRoot;

//             Internship* i = current->data;
//             return "✅ Found: " + i->title + " at " + i->location + " | Score: " + std::to_string(i->relevanceScore);
//         }
//         current = (relevanceScore < current->data->relevanceScore) ? current->left : current->right;
//     }

//     // Search auxiliary trees
//     for (auto& entry : auxTrees) {
//         std::stack<AuxNode*> auxStack;
//         auxStack.push(entry.second);

//         while (!auxStack.empty()) {
//             AuxNode* auxNode = auxStack.top();
//             auxStack.pop();

//             if (auxNode->data->relevanceScore == relevanceScore) {
//                 TreeNode* preferredRoot = entry.first;
//                 preferredRoot = cut(preferredRoot, relevanceScore);
//                 preferredRoot = join(preferredRoot, preferredRoot->right);
//                 if (preferredRoot == root) root = preferredRoot;

//                 Internship* i = auxNode->data;
//                 return "✅ Found (aux): " + i->title + " at " + i->location + " | Score: " + std::to_string(i->relevanceScore);
//             }

//             if (auxNode->left) auxStack.push(auxNode->left);
//             if (auxNode->right) auxStack.push(auxNode->right);
//         }
//     }

//     return "❌ Not Found!";
// }


// // Public delete function
// void TangoTree::deleteInternship(int relevanceScore) {
//     if (!root) return;
    
//     // First try to find and remove from preferred path
//     TreeNode* current = root;
//     TreeNode* parent = nullptr;
//     bool found = false;
    
//     while (current) {
//         if (relevanceScore == current->data->relevanceScore) {
//             found = true;
//             break;
//         }
        
//         parent = current;
//         if (relevanceScore < current->data->relevanceScore) {
//             current = current->left;
//         }
//         else {
//             current = current->right;
//         }
//     }
    
//     if (found) {
//         // Case 1: Node has no children
//         if (!current->left && !current->right) {
//             if (!parent) {
//                 root = nullptr;
//             }
//             else if (parent->left == current) {
//                 parent->left = nullptr;
//             }
//             else {
//                 parent->right = nullptr;
//             }
//         }
//         // Case 2: Node has one child
//         else if (!current->left || !current->right) {
//             TreeNode* child = current->left ? current->left : current->right;
//             if (!parent) {
//                 root = child;
//             }
//             else if (parent->left == current) {
//                 parent->left = child;
//             }
//             else {
//                 parent->right = child;
//             }
//         }
//         // Case 3: Node has two children
//         else {
//             // Find in-order successor (leftmost in right subtree)
//             TreeNode* successor = current->right;
//             TreeNode* successorParent = current;
            
//             while (successor->left) {
//                 successorParent = successor;
//                 successor = successor->left;
//             }
            
//             // Replace data with successor's data
//             Internship* temp = current->data;
//             current->data = successor->data;
//             successor->data = temp;
            
//             // Delete the successor (now guaranteed to have 0 or 1 child)
//             if (successorParent == current) {
//                 successorParent->right = successor->right;
//             }
//             else {
//                 successorParent->left = successor->right;
//             }
//         }
        
//         // Rebuild reference tree
//         std::vector<Internship*> interns;
//         storeInOrder(referenceRoot, interns);
//         interns.erase(std::remove_if(interns.begin(), interns.end(),
//             [relevanceScore](Internship* i) { return i->relevanceScore == relevanceScore; }),
//             interns.end());
        
//         clearTree(referenceRoot);
//         referenceRoot = buildReferenceTree(interns, 0, interns.size() - 1, 0);
        
//         // Update root if it was deleted
//         if (root && root->data->relevanceScore == relevanceScore) {
//             root = referenceRoot;
//         }
        
//         return;
//     }
    
//     // If not found in preferred path, search auxiliary trees
//     for (auto& entry : auxTrees) {
//         std::stack<AuxNode*> auxStack;
//         auxStack.push(entry.second);
        
//         while (!auxStack.empty()) {
//             AuxNode* current = auxStack.top();
//             auxStack.pop();
            
//             if (current->data->relevanceScore == relevanceScore) {
//                 // Found in auxiliary tree - remove it
//                 // (For simplicity, we'll just rebuild the aux tree)
//                 updateAuxTree(entry.first);
//                 return;
//             }
            
//             if (current->left) auxStack.push(current->left);
//             if (current->right) auxStack.push(current->right);
//         }
//     }
// }

// /////////////////////////////for debugging
// // Helper to print TreeNode with indentation
// void TangoTree::printTreeNode(TreeNode *node, int depth, bool isPreferred)
// {
//     if (!node)
//         return;

//     // Print indentation
//     for (int i = 0; i < depth; ++i)
//     {
//         std::cout << "    ";
//     }

//     // Print node info
//     std::cout << (isPreferred ? "★ " : "  ")
//               << node->data->title
//               << " (Score: " << node->data->relevanceScore << ")"
//               << " [Depth: " << node->depth << "]";

//     // Mark preferred child
//     if (node->preferredChild)
//     {
//         std::cout << " → " << node->preferredChild->data->title;
//     }
//     std::cout << "\n";

//     // Recursively print children
//     printTreeNode(node->left, depth + 1, node->preferredChild == node->left);
//     printTreeNode(node->right, depth + 1, node->preferredChild == node->right);
// }

// // Helper to print AuxNode with indentation
// void TangoTree::printAuxNode(AuxNode *node, int depth)
// {
//     if (!node)
//         return;

//     // Print indentation
//     for (int i = 0; i < depth; ++i)
//     {
//         std::cout << "    ";
//     }

//     // Print node info
//     std::cout << "• " << node->data->title
//               << " (Score: " << node->data->relevanceScore << ")"
//               << " [Depth: " << node->depth << "]\n";

//     // Recursively print children
//     printAuxNode(node->left, depth + 1);
//     printAuxNode(node->right, depth + 1);
// }

// void TangoTree::printTree()
// {
//     std::cout << "\n=== Tango Tree Structure ===\n";

//     // Print main tree
//     std::cout << "\nMain Tree (Preferred Paths):\n";
//     // printTreeNode(referenceRoot, 0, true);
//     printTreeNode(root, 0, true);
//     std::cout << "[DEBUG] Total auxiliary trees stored: " << auxTrees.size() << "\n";
//     // Print auxiliary trees
//     std::cout << "\nAuxiliary Trees:\n";
//     if (auxTrees.empty())
//     {
//         std::cout << "No auxiliary trees\n";
//     }
//     else
//     {
//         int auxId = 1;
//         for (const auto &entry : auxTrees)
//         {
//             std::cout << "\nAux Tree #" << auxId++
//                       << " for node [" << entry.first->data->title
//                       << "] at " << entry.first << ":\n"; // Print pointer address
//             printAuxNode(entry.second, 0);
//         }
//     }
//     std::cout << "==========================\n\n";
// }











#include "TDS.hpp"
#include <iostream>
#include <stack>
#include <algorithm>
#include <functional>

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

    if (intern->relevanceScore < node->data->relevanceScore) {
        node->left = insert(node->left, intern, depth + 1);
    } else {
        node->right = insert(node->right, intern, depth + 1);
    }

    // Update the height of this ancestor node
    node->height = max(height(node->left), height(node->right)) + 1;

    // Balance the node
    int balance = balanceFactor(node);

    // Left heavy (left-left)
    if (balance > 1 && intern->relevanceScore < node->left->data->relevanceScore)
        return rotateRight(node);

    // Right heavy (right-right)
    if (balance < -1 && intern->relevanceScore > node->right->data->relevanceScore)
        return rotateLeft(node);

    // Left-Right (left-heavy right child)
    if (balance > 1 && intern->relevanceScore > node->left->data->relevanceScore) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Left (right-heavy left child)
    if (balance < -1 && intern->relevanceScore < node->right->data->relevanceScore) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

TreeNode* TangoTree::remove(TreeNode* node, int relevanceScore) {
    if (!node) return node;

    if (relevanceScore < node->data->relevanceScore) {
        node->left = remove(node->left, relevanceScore);
    } else if (relevanceScore > node->data->relevanceScore) {
        node->right = remove(node->right, relevanceScore);
    } else {
        if (!node->left || !node->right) {
            TreeNode* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }

        // Find in-order successor
        TreeNode* temp = node->right;
        while (temp && temp->left) temp = temp->left;

        node->data = temp->data;
        node->right = remove(node->right, temp->data->relevanceScore);
    }

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
    interns.push_back(node->data);
    storeInOrder(node->right, interns);
}

AuxNode* TangoTree::buildAuxTree(TreeNode* node) {
    if (!node) return nullptr;
    
    std::vector<Internship*> interns;
    storeInOrder(node, interns);
    
    // Recursively build a balanced aux tree
    std::function<AuxNode*(int, int, int)> build = [&](int low, int high, int depth) -> AuxNode* {
        if (low > high) return nullptr;
        int mid = (low + high) / 2;
        AuxNode* auxNode = new AuxNode(interns[mid], depth);
        auxNode->left = build(low, mid - 1, depth + 1);
        auxNode->right = build(mid + 1, high, depth + 1);
        return auxNode;
    };
    
    return build(0, interns.size() - 1, node->depth);
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

std::string TangoTree::search(int relevanceScore) {
    if (!root) return "Not Found!";
    
    TreeNode* current = root;
    while (current) {
        if (relevanceScore == current->data->relevanceScore) {
            return "Found: " + current->data->title + " | Score: " + std::to_string(current->data->relevanceScore);
        }
        current = (relevanceScore < current->data->relevanceScore) ? current->left : current->right;
    }

    return "Not Found!";
}

void TangoTree::deleteInternship(int relevanceScore) {
    if (!root) return;
    root = remove(root, relevanceScore);
}

void TangoTree::printTree() {
    // This function prints the tree. You can implement it according to your requirements
    std::cout << "\nTree Structure:\n";
    printTreeNode(root, 0, true);
}

void TangoTree::printTreeNode(TreeNode* node, int depth, bool isPreferred) {
    if (!node) return;

    for (int i = 0; i < depth; ++i) std::cout << "    ";
    std::cout << (isPreferred ? "- " : "  ") << node->data->title << " (Score: " << node->data->relevanceScore << ")\n";
    printTreeNode(node->left, depth + 1, node->preferredChild == node->left);
    printTreeNode(node->right, depth + 1, node->preferredChild == node->right);
}

void TangoTree::printAuxNode(AuxNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; ++i) std::cout << "    ";
    std::cout << node->data->title << " (Score: " << node->data->relevanceScore << ")\n";
    printAuxNode(node->left, depth + 1);
    printAuxNode(node->right, depth + 1);
}
