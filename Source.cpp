
#include <iostream>

#include "Header.h"

Node* queue = NULL;
bool verbose_output = false;
BPlusTree::BPlusTree() {
    this->root = nullptr;
    this->order = 1;
}
BPlusTree::BPlusTree(int order) {
    this->root = nullptr;
    this->order = order;
}
Node* BPlusTree::getRoot() {
    return this->root;
}
void BPlusTree::setRoot(Node* node) {
    this->root = node;
}
void BPlusTree::enqueue(Node* new_Node) {
    Node* c;
    if (queue == NULL) {
        queue = new_Node;
        queue->next = NULL;
    }
    else {
        c = queue;
        while (c->next != NULL) {
            c = c->next;
        }
        c->next = new_Node;
        new_Node->next = NULL;
    }
}

Node* BPlusTree::dequeue(void) {
    Node* n = queue;
    queue = queue->next;
    n->next = NULL;
    return n;
}

void BPlusTree::print_leaves(Node* const root) {
    if (root == NULL) {
        std::cout << "Empty tree.\n";
        return;
    }
    int i;
    Node* c = root;
    while (!c->is_leaf)
        c = (Node*)c->pointers[0];
    while (true) {
        for (i = 0; i < c->num_keys; i++) {
            if (verbose_output)
                std::cout << c->pointers[i];
            std::cout << c->keys[i];
        }
        if (verbose_output)
            std::cout << c->pointers[order - 1];
        if (c->pointers[order - 1] != NULL) {
            std::cout << " | ";
            c = (Node*)c->pointers[order - 1];
        }
        else
            break;
    }
    std::cout << "\n";
}

int BPlusTree::height(Node* const root) {
    int h = 0;
    Node* c = root;
    while (!c->is_leaf) {
        c = (Node*)c->pointers[0];
        h++;
    }
    return h;
}
int BPlusTree::path_to_root(Node* const root, Node* child) {
    int length = 0;
    Node* c = child;
    while (c != root) {
        c = c->parent;
        length++;
    }
    return length;
}

void BPlusTree::print_tree() {
    Node* n = NULL;
    int i = 0;
    int rank = 0;
    int new_rank = 0;

    if (root == NULL) {
        std::cout << "Empty tree.\n";
        return;
    }
    queue = NULL;
    enqueue(root);
    while (queue != NULL) {
        n = dequeue();
        if (n->parent != NULL && n == n->parent->pointers[0]) {
            new_rank = path_to_root(root, n);
            if (new_rank != rank) {
                rank = new_rank;
                std::cout << "\n";
            }
        }
        if (verbose_output)
            std::cout << n;
        for (i = 0; i < n->num_keys; i++) {
            if (verbose_output)
                std::cout << n->pointers[i];
            std::cout << n->keys[i] << " ";
        }
        if (!n->is_leaf)
            for (i = 0; i <= n->num_keys; i++)
                enqueue((Node*)n->pointers[i]);
        if (verbose_output) {
            if (n->is_leaf)
                std::cout << n->pointers[order - 1];
            else
                std::cout << n->pointers[n->num_keys];
        }
        std::cout << " | ";
    }
    std::cout << "\n";
}

void BPlusTree::find_and_print(Node* const root, int key, bool verbose) {
    Node* leaf = NULL;
    Record* r = find(root, key, verbose, NULL);
    if (r == NULL)
        std::cout << "Record not found under key.\n", key;
    else
        std::cout << "Record at -- key, value.\n",
        r, key, r->value;
}

void BPlusTree::find_and_print_range(Node* const root, int key_start, int key_end,
    bool verbose) {
    int i;
    int array_size = key_end - key_start + 1;
    int* returned_keys = new int[array_size];
    void** returned_pointers = new void* [array_size];
    int num_found = find_range(root, key_start, key_end, verbose,
        returned_keys, returned_pointers);
    if (!num_found)
        std::cout << "None found.\n";
    else {
        for (i = 0; i < num_found; i++)
            std::cout << "Key: " << returned_keys[i] <<
            "Location: " << returned_pointers[i] <<
            "Value: " << ((Record*)returned_pointers[i])->value << "\n";
    }
}

int BPlusTree::find_range(Node* const root, int key_start, int key_end, bool verbose,
    int returned_keys[], void* returned_pointers[]) {
    int i, num_found;
    num_found = 0;
    Node* n = find_leaf(root, key_start, verbose);
    if (n == NULL)
        return 0;
    for (i = 0; i < n->num_keys && n->keys[i] < key_start; i++)
        ;
    if (i == n->num_keys)
        return 0;
    while (n != NULL) {
        for (; i < n->num_keys && n->keys[i] <= key_end; i++) {
            returned_keys[num_found] = n->keys[i];
            returned_pointers[num_found] = n->pointers[i];
            num_found++;
        }
        n = (Node*)n->pointers[order - 1];
        i = 0;
    }
    return num_found;
}

Node* BPlusTree::find_leaf(Node* const root, int key, bool verbose) {
    if (root == NULL) {
        if (verbose)
            std::cout << "Empty tree.\n";
        return root;
    }
    int i = 0;
    Node* c = root;
    while (!c->is_leaf) {
        if (verbose) {
            std::cout << "[";
            for (i = 0; i < c->num_keys - 1; i++)
                std::cout << c->keys[i];
            std::cout << c->keys[i];
        }
        i = 0;
        while (i < c->num_keys) {
            if (key >= c->keys[i])
                i++;
            else
                break;
        }
        if (verbose)
            std::cout << "->\n", i;
        c = (Node*)c->pointers[i];
    }
    if (verbose) {
        std::cout << "Leaf [";
        for (i = 0; i < c->num_keys - 1; i++)
            std::cout << c->keys[i];
        std::cout << "\n", c->keys[i];
    }
    return c;
}

Record* BPlusTree::find(Node* root, int key, bool verbose, Node** leaf_out) {
    if (root == NULL) {
        if (leaf_out != NULL) {
            *leaf_out = NULL;
        }
        return NULL;
    }

    int i = 0;
    Node* leaf = NULL;

    leaf = find_leaf(root, key, verbose);

    for (i = 0; i < leaf->num_keys; i++)
        if (leaf->keys[i] == key)
            break;
    if (leaf_out != NULL) {
        *leaf_out = leaf;
    }
    if (i == leaf->num_keys)
        return NULL;
    else
        return (Record*)leaf->pointers[i];
}

int BPlusTree::cut(int length) {
    if (length % 2 == 0)
        return length / 2;
    else
        return length / 2 + 1;
}

Record* BPlusTree::make_Record(double value) {
    Record* new_Record = new Record;
    if (new_Record == NULL) {
        perror("Record creation.");
        exit(EXIT_FAILURE);
    }
    else {
        new_Record->value = value;
    }
    return new_Record;
}

Node* BPlusTree::make_Node(void) {
    Node* new_Node;
    new_Node = new Node;
    if (new_Node == NULL) {
        perror("Node creation.");
        exit(EXIT_FAILURE);
    }
    new_Node->keys = new int[order - 1];
    if (new_Node->keys == NULL) {
        perror("New Node keys array.");
        exit(EXIT_FAILURE);
    }
    new_Node->pointers = new void* [order];
    if (new_Node->pointers == NULL) {
        perror("New Node pointers array.");
        exit(EXIT_FAILURE);
    }
    new_Node->is_leaf = false;
    new_Node->num_keys = 0;
    new_Node->parent = NULL;
    new_Node->next = NULL;
    return new_Node;
}

Node* BPlusTree::make_leaf(void) {
    Node* leaf = make_Node();
    leaf->is_leaf = true;
    return leaf;
}

int BPlusTree::get_left_index(Node* parent, Node* left) {
    int left_index = 0;
    while (left_index <= parent->num_keys &&
        parent->pointers[left_index] != left)
        left_index++;
    return left_index;
}

Node* BPlusTree::insert_into_leaf(Node* leaf, int key, Record* pointer) {
    int i, insertion_point;

    insertion_point = 0;
    while (insertion_point < leaf->num_keys && leaf->keys[insertion_point] < key)
        insertion_point++;

    for (i = leaf->num_keys; i > insertion_point; i--) {
        leaf->keys[i] = leaf->keys[i - 1];
        leaf->pointers[i] = leaf->pointers[i - 1];
    }
    leaf->keys[insertion_point] = key;
    leaf->pointers[insertion_point] = pointer;
    leaf->num_keys++;
    return leaf;
}

Node* BPlusTree::insert_into_leaf_after_splitting(Node* root, Node* leaf, int key, Record* pointer) {
    Node* new_leaf;
    int* temp_keys;
    void** temp_pointers;
    int insertion_index, split, new_key, i, j;

    new_leaf = make_leaf();

    temp_keys = new int[order];
    if (temp_keys == NULL) {
        std::cout << "Temporary keys array.";
        exit(EXIT_FAILURE);
    }

    temp_pointers = new void* [order];
    if (temp_pointers == NULL) {
        perror("Temporary pointers array.");
        exit(EXIT_FAILURE);
    }

    insertion_index = 0;
    while (insertion_index < order - 1 && leaf->keys[insertion_index] < key)
        insertion_index++;

    for (i = 0, j = 0; i < leaf->num_keys; i++, j++) {
        if (j == insertion_index)
            j++;
        temp_keys[j] = leaf->keys[i];
        temp_pointers[j] = leaf->pointers[i];
    }

    temp_keys[insertion_index] = key;
    temp_pointers[insertion_index] = pointer;

    leaf->num_keys = 0;

    split = cut(order - 1);

    for (i = 0; i < split; i++) {
        leaf->pointers[i] = temp_pointers[i];
        leaf->keys[i] = temp_keys[i];
        leaf->num_keys++;
    }

    for (i = split, j = 0; i < order; i++, j++) {
        new_leaf->pointers[j] = temp_pointers[i];
        new_leaf->keys[j] = temp_keys[i];
        new_leaf->num_keys++;
    }

    delete[] temp_pointers;
    delete[] temp_keys;

    new_leaf->pointers[order - 1] = leaf->pointers[order - 1];
    leaf->pointers[order - 1] = new_leaf;

    for (i = leaf->num_keys; i < order - 1; i++)
        leaf->pointers[i] = NULL;
    for (i = new_leaf->num_keys; i < order - 1; i++)
        new_leaf->pointers[i] = NULL;

    new_leaf->parent = leaf->parent;
    new_key = new_leaf->keys[0];

    return insert_into_parent(root, leaf, new_key, new_leaf);
}

Node* BPlusTree::insert_into_Node(Node* root, Node* n,
    int left_index, int key, Node* right) {
    int i;

    for (i = n->num_keys; i > left_index; i--) {
        n->pointers[i + 1] = n->pointers[i];
        n->keys[i] = n->keys[i - 1];
    }
    n->pointers[left_index + 1] = right;
    n->keys[left_index] = key;
    n->num_keys++;
    return root;
}

Node* BPlusTree::insert_into_Node_after_splitting(Node* root, Node* old_Node, int left_index,
    int key, Node* right) {
    int i, j, split, k_prime;
    Node* new_Node, * child;
    int* temp_keys;
    Node** temp_pointers;

    temp_pointers = new Node * [order + 1];
    if (temp_pointers == NULL) {
        perror("Temporary pointers array for splitting Nodes.");
        exit(EXIT_FAILURE);
    }
    temp_keys = new int[order];
    if (temp_keys == NULL) {
        perror("Temporary keys array for splitting Nodes.");
        exit(EXIT_FAILURE);
    }

    for (i = 0, j = 0; i < old_Node->num_keys + 1; i++, j++) {
        if (j == left_index + 1)
            j++;
        temp_pointers[j] = (Node*)old_Node->pointers[i];
    }

    for (i = 0, j = 0; i < old_Node->num_keys; i++, j++) {
        if (j == left_index)
            j++;
        temp_keys[j] = old_Node->keys[i];
    }

    temp_pointers[left_index + 1] = right;
    temp_keys[left_index] = key;

    split = cut(order);
    new_Node = make_Node();
    old_Node->num_keys = 0;
    for (i = 0; i < split - 1; i++) {
        old_Node->pointers[i] = temp_pointers[i];
        old_Node->keys[i] = temp_keys[i];
        old_Node->num_keys++;
    }
    old_Node->pointers[i] = temp_pointers[i];
    k_prime = temp_keys[split - 1];
    for (++i, j = 0; i < order; i++, j++) {
        new_Node->pointers[j] = temp_pointers[i];
        new_Node->keys[j] = temp_keys[i];
        new_Node->num_keys++;
    }
    new_Node->pointers[j] = temp_pointers[i];
    delete[] temp_pointers;
    delete[] temp_keys;
    new_Node->parent = old_Node->parent;
    for (i = 0; i <= new_Node->num_keys; i++) {
        child = (Node*)new_Node->pointers[i];
        child->parent = new_Node;
    }

    return insert_into_parent(root, old_Node, k_prime, new_Node);
}

Node* BPlusTree::insert_into_parent(Node* root, Node* left, int key, Node* right) {
    int left_index;
    Node* parent;

    parent = left->parent;

    if (parent == NULL)
        return insert_into_new_root(left, key, right);

    left_index = get_left_index(parent, left);

    if (parent->num_keys < order - 1)
        return insert_into_Node(root, parent, left_index, key, right);

    return insert_into_Node_after_splitting(root, parent, left_index, key, right);
}

Node* BPlusTree::insert_into_new_root(Node* left, int key, Node* right) {
    Node* root = make_Node();
    root->keys[0] = key;
    root->pointers[0] = left;
    root->pointers[1] = right;
    root->num_keys++;
    root->parent = NULL;
    left->parent = root;
    right->parent = root;
    return root;
}

Node* BPlusTree::start_new_tree(int key, Record* pointer) {
    Node* root = make_leaf();
    root->keys[0] = key;
    root->pointers[0] = pointer;
    root->pointers[order - 1] = NULL;
    root->parent = NULL;
    root->num_keys++;
    return root;
}

Node* BPlusTree::INSERT(Node* root, int key, double value) {
    Record* Record_pointer = NULL;
    Node* leaf = NULL;

    Record_pointer = find(root, key, false, NULL);
    if (Record_pointer != NULL) {
        Record_pointer->value = value;
        return root;
    }

    Record_pointer = make_Record(value);

    if (root == NULL)
        return start_new_tree(key, Record_pointer);

    leaf = find_leaf(root, key, false);

    if (leaf->num_keys < order - 1) {
        leaf = insert_into_leaf(leaf, key, Record_pointer);
        return root;
    }

    return insert_into_leaf_after_splitting(root, leaf, key, Record_pointer);
}

int BPlusTree::get_neighbor_index(Node* n) {
    int i;
    for (i = 0; i <= n->parent->num_keys; i++)
        if (n->parent->pointers[i] == n)
            return i - 1;

    std::cout << "Search for nonexistent pointer to Node in parent.\n";
    std::cout << "Node:  \n", (unsigned long)n;
    exit(EXIT_FAILURE);
}

Node* BPlusTree::remove_entry_from_Node(Node* n, int key, Node* pointer) {
    int i, num_pointers;
    i = 0;
    while (n->keys[i] != key)
        i++;
    for (++i; i < n->num_keys; i++)
        n->keys[i - 1] = n->keys[i];

    num_pointers = n->is_leaf ? n->num_keys : n->num_keys + 1;
    i = 0;
    while (n->pointers[i] != pointer)
        i++;
    for (++i; i < num_pointers; i++)
        n->pointers[i - 1] = n->pointers[i];

    n->num_keys--;

    if (n->is_leaf)
        for (i = n->num_keys; i < order - 1; i++)
            n->pointers[i] = NULL;
    else
        for (i = n->num_keys + 1; i < order; i++)
            n->pointers[i] = NULL;

    return n;
}

Node* BPlusTree::adjust_root(Node* root) {
    Node* new_root;

    if (root->num_keys > 0)
        return root;

    if (!root->is_leaf) {
        new_root = (Node*)root->pointers[0];
        new_root->parent = NULL;
    }

    else
        new_root = NULL;

    free(root->keys);
    free(root->pointers);
    free(root);

    return new_root;
}

Node* BPlusTree::coalesce_Nodes(Node* root, Node* n, Node* neighbor, int neighbor_index, int k_prime) {
    int i, j, neighbor_insertion_index, n_end;
    Node* tmp;

    if (neighbor_index == -1) {
        tmp = n;
        n = neighbor;
        neighbor = tmp;
    }

    neighbor_insertion_index = neighbor->num_keys;

    if (!n->is_leaf) {
        neighbor->keys[neighbor_insertion_index] = k_prime;
        neighbor->num_keys++;

        n_end = n->num_keys;

        for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
            neighbor->keys[i] = n->keys[j];
            neighbor->pointers[i] = n->pointers[j];
            neighbor->num_keys++;
            n->num_keys--;
        }

        neighbor->pointers[i] = n->pointers[j];

        for (i = 0; i < neighbor->num_keys + 1; i++) {
            tmp = (Node*)neighbor->pointers[i];
            tmp->parent = neighbor;
        }
    }

    else {
        for (i = neighbor_insertion_index, j = 0; j < n->num_keys; i++, j++) {
            neighbor->keys[i] = n->keys[j];
            neighbor->pointers[i] = n->pointers[j];
            neighbor->num_keys++;
        }
        neighbor->pointers[order - 1] = n->pointers[order - 1];
    }

    root = delete_entry(root, n->parent, k_prime, n);
    free(n->keys);
    free(n->pointers);
    free(n);
    return root;
}

Node* BPlusTree::redistribute_Nodes(Node* root, Node* n, Node* neighbor, int neighbor_index,
    int k_prime_index, int k_prime) {
    int i;
    Node* tmp;

    if (neighbor_index != -1) {
        if (!n->is_leaf)
            n->pointers[n->num_keys + 1] = n->pointers[n->num_keys];
        for (i = n->num_keys; i > 0; i--) {
            n->keys[i] = n->keys[i - 1];
            n->pointers[i] = n->pointers[i - 1];
        }
        if (!n->is_leaf) {
            n->pointers[0] = neighbor->pointers[neighbor->num_keys];
            tmp = (Node*)n->pointers[0];
            tmp->parent = n;
            neighbor->pointers[neighbor->num_keys] = NULL;
            n->keys[0] = k_prime;
            n->parent->keys[k_prime_index] = neighbor->keys[neighbor->num_keys - 1];
        }
        else {
            n->pointers[0] = neighbor->pointers[neighbor->num_keys - 1];
            neighbor->pointers[neighbor->num_keys - 1] = NULL;
            n->keys[0] = neighbor->keys[neighbor->num_keys - 1];
            n->parent->keys[k_prime_index] = n->keys[0];
        }
    }

    else {
        if (n->is_leaf) {
            n->keys[n->num_keys] = neighbor->keys[0];
            n->pointers[n->num_keys] = neighbor->pointers[0];
            n->parent->keys[k_prime_index] = neighbor->keys[1];
        }
        else {
            n->keys[n->num_keys] = k_prime;
            n->pointers[n->num_keys + 1] = neighbor->pointers[0];
            tmp = (Node*)n->pointers[n->num_keys + 1];
            tmp->parent = n;
            n->parent->keys[k_prime_index] = neighbor->keys[0];
        }
        for (i = 0; i < neighbor->num_keys - 1; i++) {
            neighbor->keys[i] = neighbor->keys[i + 1];
            neighbor->pointers[i] = neighbor->pointers[i + 1];
        }
        if (!n->is_leaf)
            neighbor->pointers[i] = neighbor->pointers[i + 1];
    }

    n->num_keys++;
    neighbor->num_keys--;

    return root;
}

Node* BPlusTree::delete_entry(Node* root, Node* n, int key, void* pointer) {
    int min_keys;
    Node* neighbor;
    int neighbor_index;
    int k_prime_index, k_prime;
    int capacity;

    n = remove_entry_from_Node(n, key, (Node*)pointer);

    if (n == root)
        return adjust_root(root);

    min_keys = n->is_leaf ? cut(order - 1) : cut(order) - 1;

    if (n->num_keys >= min_keys)
        return root;

    neighbor_index = get_neighbor_index(n);
    k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
    k_prime = n->parent->keys[k_prime_index];
    neighbor = ((Node*)(neighbor_index == -1 ? n->parent->pointers[1] : n->parent->pointers[neighbor_index]));

    capacity = n->is_leaf ? order : order - 1;

    if (neighbor->num_keys + n->num_keys < capacity)
        return coalesce_Nodes(root, n, neighbor, neighbor_index, k_prime);
    else
        return redistribute_Nodes(root, n, neighbor, neighbor_index, k_prime_index, k_prime);
}

Node* BPlusTree::DELETE(Node* root, int key) {
    Node* key_leaf = NULL;
    Record* key_Record = NULL;

    key_Record = find(root, key, false, &key_leaf);

    if (key_Record != NULL && key_leaf != NULL) {
        root = delete_entry(root, key_leaf, key, key_Record);
        free(key_Record);
    }
    return root;
}

void BPlusTree::destroy_tree_Nodes(Node* root) {
    int i;
    if (root->is_leaf)
        for (i = 0; i < root->num_keys; i++)
            free(root->pointers[i]);
    else
        for (i = 0; i < root->num_keys + 1; i++)
            destroy_tree_Nodes((Node*)root->pointers[i]);
    free(root->pointers);
    free(root->keys);
    free(root);
}

Node* BPlusTree::destroy_tree(Node* root) {
    destroy_tree_Nodes(root);
    return NULL;
}


