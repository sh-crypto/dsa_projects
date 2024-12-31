/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    // your code here
    Node* curr = root;
    Node*& toReturn = find(curr, key);
    return toReturn -> value;
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    // Your code here
    if (subtree == NULL || subtree -> key == key) {
        return subtree;
    }
    if (key < subtree-> key) {
        return find(subtree -> left, key);
    } else {
        return find(subtree -> right, key);
    }
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    // your code here
    Node* curr = root;
    insert(curr, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    Node* newNode = new Node(key, value);
    newNode -> right = NULL;
    newNode -> left = NULL;
    if (root == NULL) {
        root = newNode;
        return;
    }
    Node* & found = find(subtree, key);
    if (found) { return; }
    Node* curr = subtree;
    Node* parent = NULL;
    while (curr) {
        parent = curr;
        if (key > curr -> key) {
            curr = curr -> right;
        } else {
            curr = curr -> left;
        }
    }
    if (key > parent -> key) {
        parent -> right = newNode;
    } else {
        parent -> left = newNode;
    }
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    K key_first = first -> key;
    K key_second = second -> key;
    V value_first = first -> value;
    V value_second = second -> value;
    first -> key = key_second;
    first -> value = value_second;
    second -> key = key_first;
    second -> value = value_first;
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    // your code here
    Node* curr = root;
    remove(curr, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    Node*& found = find(subtree, key);
    if (found == NULL) { return; }
    if (found -> left == NULL && found -> right == NULL) {
        delete found;
        found = NULL;
        return;
    } else if (found -> left == NULL || found -> right == NULL) {
        Node* tmp = NULL;
        if (found -> left == NULL) { tmp = found -> right; }
        else if (found -> right == NULL) { tmp = found -> left; }
        delete found;
        found = tmp;
        return;
    } else {
        Node* tmp = found -> left;
        while (tmp -> right != NULL) {
            tmp = tmp -> right;
        }
        swap(tmp, found);
        remove (found -> left, key);
    }  
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K,V> new_tree;
    for (unsigned int i = 0; i < inList.size(); i++) {
        new_tree.insert(inList[i].first, inList[i].second);
    }
    return new_tree;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    std::vector<int> vect;
    std::sort(inList.begin(), inList.end());
    do {
        BST<K,V> new_tree = listBuild(inList);
        int height = new_tree.height();
        if (height >= (int) vect.size()) {
            vect.resize(height + 1);
            vect.at(height) += 1;
        } else {
            vect.at(height) += 1;
        }


    } while (std::next_permutation(inList.begin(), inList.end()));

    return vect;
}