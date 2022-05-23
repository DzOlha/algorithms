#pragma once
#ifndef HEADER_H
#define HEADER_H

struct Record {
	double value;
};
struct Node {
    void** pointers;
    int* keys;
    Node* parent;
    bool is_leaf;
    int num_keys;
    Node* next;
};
class BPlusTree {
private:
    Node* root;
    int order;
public:
    BPlusTree();
    BPlusTree(int order);
    Node* getRoot();
    void setRoot(Node* node);
    void enqueue(Node* new_Node);
    Node* dequeue(void);
    int height(Node* const root);
    int path_to_root(Node* const root, Node* child);
    void print_leaves(Node* const root);
    void print_tree();
    void find_and_print(Node* const root, int key, bool verbose);
    int find_range(Node* const root, int key_start, int key_end, bool verbose,
        int returned_keys[], void* returned_pointers[]);
    void find_and_print_range(Node* const root, int range1, int range2, bool verbose);
    Node* find_leaf(Node* const root, int key, bool verbose);
    Record* find(Node* root, int key, bool verbose, Node** leaf_out);
    int cut(int length);

    Record* make_Record(double value);
    Node* make_Node(void);
    Node* make_leaf(void);
    int get_left_index(Node* parent, Node* left);
    Node* insert_into_leaf(Node* leaf, int key, Record* pointer);
    Node* insert_into_leaf_after_splitting(Node* root, Node* leaf, int key,
        Record* pointer);
    Node* insert_into_Node(Node* root, Node* parent,
        int left_index, int key, Node* right);
    Node* insert_into_Node_after_splitting(Node* root, Node* parent,
        int left_index,
        int key, Node* right);
    Node* insert_into_parent(Node* root, Node* left, int key, Node* right);
    Node* insert_into_new_root(Node* left, int key, Node* right);
    Node* start_new_tree(int key, Record* pointer);
    Node* INSERT(Node* root, int key, double value);
    Node* remove_entry_from_Node(Node* n, int key, Node* pointer);
    int get_neighbor_index(Node* n);
    Node* adjust_root(Node* root);
    Node* coalesce_Nodes(Node* root, Node* n, Node* neighbor,
        int neighbor_index, int k_prime);
    Node* redistribute_Nodes(Node* root, Node* n, Node* neighbor,
        int neighbor_index,
        int k_prime_index, int k_prime);
    Node* delete_entry(Node* root, Node* n, int key, void* pointer);
    Node* DELETE(Node* root, int key);
    void destroy_tree_Nodes(Node* root);
    Node* destroy_tree(Node* root);
};
#endif
