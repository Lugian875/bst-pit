/* bst.cpp
 * Maxwell Benefield
 * COP3415
 * Last Modified: March 18, 2025
 */

#include <iostream>
#include <utility>
// #include <string.h>
using namespace std;

// Item Node Implementation
// Stores data for each item of each tree
class itemNode {
public:
    string name;
    int count;
    itemNode *left, *right; // pointers for left and right something

    // Default constructor
    itemNode() {
        name = '\0';
        count = 0;
        left = nullptr;
        right = nullptr;
    }

    // Constructor with user-defined parameters
    itemNode(const string &itemName, int population) {
        name = itemName;
        count = population;
        left = nullptr;
        right = nullptr;
    }
};

// Tree Node Implementation
// Stores data for each node in the Name tree
class treeNameNode {
public:
    string treeName;
    treeNameNode *left, *right;
    itemNode *theTree;

    // Default constructor
    treeNameNode() {
        treeName = '\0';
        theTree = nullptr;
        left = nullptr;
        right = nullptr;
    }

    // Constructor with user-defined parameters
    explicit treeNameNode(const string &name) {
        treeName = name;
        theTree = nullptr;
        left = nullptr;
        right = nullptr;
    }
};









int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
