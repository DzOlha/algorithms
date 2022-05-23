
#include <iostream>
#include "Header.h"
int main() {
    BPlusTree tree(3);
    const int size = 10;
    int arr[size] = { 15, 23, 45, 12, 347, 237, 896, 24, 78, 12 };
    for (int i = 0; i < size; i++) {
        tree.setRoot(tree.INSERT(tree.getRoot(), i, arr[i]));
    }
    tree.print_tree();

    tree.DELETE(tree.getRoot(), 5);
    std::cout << "\n";
    tree.print_tree();
}