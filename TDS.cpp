// TangoTree.cpp
#include "TDS.hpp"
#include <algorithm>
#include <iostream>

// Constructor - dummy for now
TangoTree::TangoTree() {
    referenceRoot = nullptr;
}

// Recursively builds a balanced reference tree
TreeNode* TangoTree::buildReferenceTree(std::vector<Internship*>& interns, int low, int high) {
    if (low > high) return nullptr;
    int mid = (low + high) / 2;
    TreeNode* root = new TreeNode(interns[mid]);
    root->left = buildReferenceTree(interns, low, mid - 1);
    root->right = buildReferenceTree(interns, mid + 1, high);
    return root;
}

// Helper to search in the reference tree and store the path
TreeNode* TangoTree::searchReferenceTree(TreeNode* node, int key, std::vector<TreeNode*>& path) {
    if (!node) return nullptr;
    path.push_back(node);
    if (key == node->data->relevanceScore) return node;
    else if (key < node->data->relevanceScore) return searchReferenceTree(node->left, key, path);
    else return searchReferenceTree(node->right, key, path);
}

// Updates preferred path pointers based on search path
void TangoTree::updatePreferredPaths(int key) {
    std::vector<TreeNode*> path;
    TreeNode* result = searchReferenceTree(referenceRoot, key, path);
    if (!result) return;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        path[i]->preferredChild = path[i + 1];
    }
    path.back()->preferredChild = nullptr;
}

// High-level search function
void TangoTree::search(int relevanceScore) {
    std::vector<TreeNode*> path;
    TreeNode* result = searchReferenceTree(referenceRoot, relevanceScore, path);

    if (result) {
        std::cout << "Internship found: " << result->data->title << " at " << result->data->location << std::endl;
        updatePreferredPaths(relevanceScore);
    } else {
        std::cout << "Internship with score " << relevanceScore << " not found." << std::endl;
    }
}

// Placeholder for insert logic
void TangoTree::insertInternship(Internship* intern) {
    // TODO: Implement insert logic with rebalancing if necessary
}

// Placeholder for delete logic
void TangoTree::deleteInternship(int relevanceScore) {
    // TODO: Implement delete logic or lazy deletion
}
