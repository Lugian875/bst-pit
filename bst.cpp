/* bst.cpp
 * Maxwell Benefield
 * COP3415
 * Last Modified: March 20, 2025
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class itemNode;
class treeNameNode;

string nextLine();
void tint_names (const treeNameNode* node);

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


/* insertToName
 * Description: This function inserts nodes into the Name tree recursively.
 * Parameters:
 * - treeNameNode *root: The address of the current root of the Name tree.
 * - treeNameNode *element: The element to be inserted into the Name tree.
 * Return: The updated root of the Name tree
 */
treeNameNode* insertToName(treeNameNode *root, treeNameNode *element) {
    // When tree is empty, just insert the element and make it the root
    if (root == nullptr) {
        return element;
    } else {
        // condition for when the element should be inserted to the right
        if (element->treeName > root->treeName) {
            // If there is a right subtree, insert it there
            if (root->right != nullptr)
                root->right = insertToName(root->right, element);
            // Else, just insert it to the right of the root
            else
                root->right = element;
        }
        // condition for when the element should be inserted to the left
        else {
            // If there is a left subtree, insert it there
            if (root->left != nullptr)
                root->left = insertToName(root->left, element);
            // Else, just insert it to the left of the root
            else
                root->left = element;
        }
        return root; // Returns root pointer of the updated tree
    }
}

/* buildNameTree
     * Description: This builds a name tree using queries from an input file
     * Parameters:
     * - int treeNameCount (the number of lines from input that has queries for inserting nodes into Name tree)
     * Return: the root of the given treeNameNode
     */
treeNameNode* buildNameTree(const int treeNameCount) {
    treeNameNode* tempNode, *root = nullptr;
    for (int i = 1; i <= treeNameCount; i++) {
        string element = nextLine();
        tempNode = new treeNameNode(element);
        root = insertToName(root, tempNode);
    }
    return root;
}

/* traverse_in_traverse
 * Description: Traverses the Name tree, printing all the trees' names and the items within them in
 * a designated format.
 * Parameter: treeNameNode* root (the address of the root of the name_tree)
 * Return: nothing
 */
void traverse_in_traverse (const treeNameNode *root) {
    tint_names(root);
    cout << endl;
}
void tint_names (const treeNameNode* node) {
    if (!node)
        return;
    tint_names(node->left);
    cout << node->treeName << " ";
    tint_names(node->right);
}



/* nextLine
 * Description: Each time the function is called, the next line of "in.txt" is read.
 * Parameters: none
 * Return: The string from the given line in the .txt file.
 */
string nextLine() {
    static ifstream file;
    static bool fileOpened = false;

    // The first call opens the file
    if(!fileOpened) {
        file.open("in.txt");
        if (!file.is_open()) {
            throw ("Error: File not found");
        }
        fileOpened = true;
    }

    string line;
    if (getline(file, line)) {
        return line;
    } else {
        file.close();
        fileOpened = false;
        return "EOF"; // end of file
    }
}

int main() {
    int treeNameCount = 0, itemCount = 0, queryCount = 0;
    treeNameNode* name_tree= nullptr;

    // First line int extraction
    stringstream firstLine;
    firstLine << nextLine();
    string temp; int is_Int;
    while (!firstLine.eof()) {
        firstLine >> temp;
        if (stringstream(temp) >> is_Int) {
            if (treeNameCount == 0)
                treeNameCount = is_Int;
            else if (itemCount == 0)
                itemCount = is_Int;
            else if (queryCount == 0)
                queryCount = is_Int;
            else
                throw ("First line of in.txt is not correctly formatted\n "
                       "It should be three integers in sequence like \"N I Q\"");

        }
    }

    cout << queryCount << endl;
    name_tree = buildNameTree(treeNameCount);
    traverse_in_traverse(name_tree);

    delete name_tree;
    return 0;
}
