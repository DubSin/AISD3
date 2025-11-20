#ifndef BINARYTREE_H
#define BINARYTREE_H

using namespace std;

#include "stdafx.h"
#include "Queue.h"

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    TreeNode* root;

public:
    BinaryTree() : root(nullptr) {}
    

    void clear() {
        clear_tree(root);
    }
    
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;
    
    // (произвольное расположение)
    void add(int value) {
        TreeNode* new_node = new TreeNode(value);
        
        if (!root) {
            root = new_node;
            return;
        }
        
        // Используем очередь для поиска первого свободного места
        Queue<TreeNode*> queue;
        queue.push(root);
        
        while (!queue.empty()) {
            TreeNode* current = queue.front();
            queue.pop();

            if (!current->left) {
                current->left = new_node;
                return;
            } else {
                queue.push(current->left);
            }

            if (!current->right) {
                current->right = new_node;
                return;
            } else {
                queue.push(current->right);
            }
        }
    }

    void dfs_preorder() {
        cout << "Pre-order: ";
        dfs_preorder_recursive(root);
        cout << endl;
    }

    void dfs_inorder() {
        cout << "In-order: ";
        dfs_inorder_recursive(root);
        cout << endl;
    }

    void dfs_postorder() {
        cout << "Post-order: ";
        dfs_postorder_recursive(root);
        cout << endl;
    }
    
    void print_tree() {
        if (!root) {
            cout << "Tree empty" << endl;
            return;
        }
        print_tree_recursive(root, 0, "Root: ");
    }

private:
    
    void dfs_preorder_recursive(TreeNode* node) {
        if (!node) return;
        
        cout << node->value << " ";  
        dfs_preorder_recursive(node->left);   
        dfs_preorder_recursive(node->right); 
    }
    
    void dfs_inorder_recursive(TreeNode* node) {
        if (!node) return;
        
        dfs_inorder_recursive(node->left);    
        cout << node->value << " ";  
        dfs_inorder_recursive(node->right);  
    }
    
    void dfs_postorder_recursive(TreeNode* node) {
        if (!node) return;
        
        dfs_postorder_recursive(node->left);  
        dfs_postorder_recursive(node->right); 
        cout << node->value << " ";  
    }
    
    void print_tree_recursive(TreeNode* node, int level, const string& prefix) {
        if (!node) return;
        
        cout << string(level * 4, ' ') << prefix << node->value << endl;
        
        if (node->left) {
            print_tree_recursive(node->left, level + 1, "L--- ");
        }
        if (node->right) {
            print_tree_recursive(node->right, level + 1, "R--- ");
        }
    }
    
    void clear_tree(TreeNode* node) {
        if (!node) return;
        
        clear_tree(node->left);
        clear_tree(node->right);
        delete node;
    }
};

#endif