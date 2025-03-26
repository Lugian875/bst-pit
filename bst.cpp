/* bst.cpp
 * Maxwell Benefield
 * COP3415
 * Last Modified: March 25, 2025
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/* Classes */

/* Item Node Implementation
 * Stores data for each node in each Item tree
 */
class itemNode {
public:
    string name; 
    int count;
    itemNode *left, *right; 

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

/* Name Tree Functions */

/* insertToName
 * Description: Inserts nodes into the Name tree
 * Parameters:
 * - treeNameNode *root: The address of the current root of the Name tree
 * - treeNameNode *element: The element to be inserted into the Name tree
 * Return: The updated root of the Name tree
 */
treeNameNode *insertToName(treeNameNode *root, treeNameNode *element) {
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
 * Description: Builds a name tree using queries from an input file
 * Parameters:
 * - const int numTreeNames: the number of  lines from the input that has queries for inserting Name nodes
 * Return: the root of the treeNameNode after all nodes have been added
 */
treeNameNode *buildNameTree(const int numTreeNames) {
    treeNameNode *tempNode, *root = nullptr;
    for (int i = 1; i <= numTreeNames; i++) {
        string element = nextLine();
        tempNode = new treeNameNode(element);
        root = insertToName(root, tempNode);
    }
    return root;
}

/* searchNameNode
 * Description: Searches the Name tree for a given name and returns the node for that name
 * Parameters:
 * - treeNameNode* root: The root of the name tree
 * - const string &treeName : The name to be searched for
 * Return: The name for the specific tree, or nullptr if it couldn't be found
 */
treeNameNode* searchNameNode(treeNameNode* root, const string &treeName) {
    treeNameNode* node = root;
    while(node) {
        if(node->treeName == treeName)
            return node;
        if (treeName < node->treeName)
            node = node->left;
        else if (treeName > node->treeName)
            node = node->right;
    }
    return nullptr;
}

/* Item Tree Functions */
/* insertItem
 * Description: Inserts each individual Item into their respective location in their Item tree.
 * Parameters:
 * - itemNode* root: The root of the item tree
 * - itemNode* element: The element to be added, containing the name and count of Item
 * Return: Updated root of the Item tree
 */
itemNode *insertItem(itemNode* root, itemNode* element) {
    if (root == nullptr) {
        return element;
    } else {
        // Condition for when the element should be inserted to the right
        if (element->name > root->name) {
            // If there is a right subtree, insert it there
            if (root-> right != nullptr)
                root->right = insertItem(root->right,element);
            // Else, just insert it to the right of the root
            else
                root->right = element;
        }
        // Condition for when the element should be inserted to the left
        else {
            // If there is a left subtree, insert it there
            if (root->left != nullptr)
                root->left = insertItem(root->left, element);
            // Else, just insert it to the left of the root
            else
                root->left = element;
        }
        return root; // returns root pointer of the updated tree
    }
}

/* insertToItems
 * Description: Inserts nodes into Item trees found in the Name tree
 * Parameters:
 * - treeNameNode* root: The root of the Name tree where the Item trees within will get items added to it
 * - const int numItems: The number of lines from the input that have queries for inserting Item nodes
 * Return: nothing
 */
void insertToItems(treeNameNode *root, const int numItems) {
    int item_count = 0; // Stores "count" of item specified in each line

    for (int i = 1; i <= numItems; i++) {
        string item_tree, item_name; // Stores Item name and Item tree where it is from
        string itemQuery = nextLine(), trimString = itemQuery;
        int pos = 0;

        // Extracts the Item information from the query
        while (pos != -1) {
            pos = trimString.find(' ');
            if (item_tree.empty())
                item_tree = trimString.substr(0, pos);
            else if (item_name.empty())
                item_name = trimString.substr(0, pos);
            else
                item_count = stoi(trimString.substr(0));
            trimString = trimString.substr(pos + 1);
        }

        treeNameNode* rootOfItem = searchNameNode(root, item_tree); // Node of root of the given Item tree
        if (rootOfItem != nullptr) {
            itemNode* tempNode = new itemNode (item_name,item_count);
            rootOfItem->theTree = insertItem(rootOfItem->theTree, tempNode);
        } else {
            cout << "The item: \"" << itemQuery << "\" could not be inserted because the tree:  " <<
                item_tree << "does not exist." << endl;
        }
        item_tree = "", item_name = "";
    }
}


void tint_names (const treeNameNode* node, bool printItems);
void tint_items (const itemNode* node);
/* traverse_in_traverse
 * Description: Traverses the main Name tree with inorder traversal, prints each name,
 * then does the same for each name tree (looks at the items)
 * Parameter: treeNameNode* root (the address of the root of the name_tree)
 * Sub-functions:
 * - tint_names: Traverses Name tree. Setting a bool to true will also make it call tint_items
 * - tint_items: Traverses Item tree.
 * Return: nothing
 */
void traverse_in_traverse (const treeNameNode *root) {
    tint_names(root, false);
    cout << endl;
    tint_names(root, true);
    cout << endl;
}
void tint_names (const treeNameNode* node, bool printItems) {
    if (!node)
        return;
    tint_names(node->left, printItems);
    if (printItems == false)
        cout << node->treeName << " ";
    else {
        cout << "***" << node->treeName << "***" << endl;
        tint_items(node->theTree);
        cout << endl;
    }
    tint_names(node->right, printItems);
}
void tint_items(const itemNode* node) {
    if (!node)
        return;
    tint_items(node->left);
    cout << node->name << " ";
    tint_items(node->right);
}


int main() {
    int numTreeNames = 0, numItems = 0, numQuery = 0;
    treeNameNode* name_tree= nullptr;

    // First line int extraction
    stringstream firstLine;
    firstLine << nextLine();
    string temp; int is_Int;
    while (!firstLine.eof()) {
        firstLine >> temp;
        if (stringstream(temp) >> is_Int) {
            if (numTreeNames == 0)
                numTreeNames = is_Int;
            else if (numItems == 0)
                numItems = is_Int;
            else if (numQuery == 0)
                numQuery = is_Int;
            else
                throw ("First line of in.txt is not correctly formatted\n "
                       "It should be three integers in sequence like \"N I Q\"");

        }
    }

    cout << numQuery << endl;
    name_tree = buildNameTree(numTreeNames);
    insertToItems(name_tree, numItems);
    traverse_in_traverse(name_tree);

    delete name_tree;
    return 0;
}
