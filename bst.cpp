/* bst.cpp
 * Maxwell Benefield
 * COP3415
 * Last Modified: March 27, 2025
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <thread>

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

/* output_and_write
 * Description: Prints output to console and writes to a file "example_out.txt"
 * Parameter: const string& output: The output
 * Return: nothing
 */
inline void output_and_write(const string& output, bool endLine) {
    ofstream file("out.txt", ios_base::app);
    if (!output.empty()) {
        cout << output; file << output;
    }
    if (endLine == true) {
        cout << endl; file<<endl;
    }
}


/* nextLine
 * Description: Each time the function is called, the next line of "in.txt" is read.
 * Parameters: none
 * Return: The string from the given line in the .txt file.
 */
inline string nextLine() {
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
    if (getline(file, line))
        return line;
    file.close();
    fileOpened = false;
    return "EOF"; // end of file
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
    treeNameNode *root = nullptr;
    for (int i = 1; i <= numTreeNames; i++) {
        string element = nextLine();
        root = insertToName(root, new treeNameNode(element));
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
 * Description: Inserts an item into the respective location in the item tree
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

/* insertToItem
 * Description: Inserts nodes into Item trees found in the Name tree
 * Parameters:
 * - treeNameNode* root: The root of the Name tree where the Item trees within will get items added to it
 * - const int numItems: The number of lines from the input that have queries for inserting Item nodes
 * Return: nothing
 */
void insertToItem(treeNameNode *root, const int numItems) {
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
            auto* tempNode = new itemNode (item_name,item_count);
            rootOfItem->theTree = insertItem(rootOfItem->theTree, tempNode);
        } else {
            cout << "The item: \"" << itemQuery << "\" could not be inserted because the tree:  " <<
                item_tree << "does not exist." << endl;
        }
    }
}



/* traverse_in_traverse
 * Description: Traverses the main Name tree with inorder traversal, prints each name,
 * then does the same for each name tree (looks at the items)
 * Parameter: treeNameNode* root (the address of the root of the name_tree)
 * Sub-functions:
 * - tint_names: Traverses Name tree. Setting a bool to true will also make it call tint_items
 * - tint_items: Traverses Item tree.
 * Return: nothing
 */
void tint_names (const treeNameNode* node, bool printItems);
void tint_items (const itemNode* node);
void traverse_in_traverse (const treeNameNode *root) {
    tint_names(root, false);
    output_and_write("",true);
    tint_names(root, true);
    cout << endl;
}
void tint_names (const treeNameNode* node, bool printItems) {
    if (!node)
        return;
    tint_names(node->left, printItems);
    if (printItems == false)
        output_and_write(node->treeName + " ", false);
    else {
        output_and_write("***" + node->treeName + "***", true);
        tint_items(node->theTree);
        output_and_write("",true);
    }
    tint_names(node->right, printItems);
}
void tint_items(const itemNode* node) {
    if (!node)
        return;
    tint_items(node->left);
    output_and_write(node->name + " ",false);
    tint_items(node->right);
}

/* Query Functions */

/* search
 * Description: Searches for an item in a given tree.
 * Parameters:
 *  - itemNode* node: The root of the Item tree which the item should be in
 * - string tree_name: The tree in which the item should be in
 * - string item_name: The item to find
 * Return: The function can return one of two strings:
 * - Case 1: If the item is found in the tree name, it returns: "(count of item) (name of item) found in (name of tree)"
 * - Case 2: If the tree exists and the item cannot be found, it returns: "(name of item) not found in (name of tree)"
 */
string search(const itemNode* node, const string &tree_name, const string &item_name) {
    while(node) {
        if(node->name == item_name)
            return to_string(node->count) + " " + item_name + " found in " + tree_name;
        if (item_name < node->name)
            node = node->left;
        else if (item_name > node->name)
            node = node->right;
    }
    return item_name + " not found in " + tree_name;
}

/* item_before
 * Description: Counts the number of items before a node.
 * Parameters:
 * - itemNode* node: The root of the Item tree which the item should be in
 * - string tree_name: The tree in which the item should be in
 * - string item_name: The item to find
 * Return: The function can return one of two strings:
 * - Case 1: If the item is found in the tree name, it returns: "items before (name of item): (# of items before it)"
 * - Case 2: If the tree exists and the item cannot be found, it returns: "(name of item) not found in (name of tree)"
 */
string item_before(const itemNode* node, const string &tree_name, const string &item_name) {
    int count_before = 1;
    // Traverses the tree and counts any items that is not the specified item
    while(node) {
        count_before++;
        if(node->name == item_name)
            return "items before " + item_name + ": " + to_string(count_before);
        if (item_name < node->name) {
            node = node->left;
        } else if (item_name > node->name) {
            node = node->right;
        }
    }
    return item_name + " not found in " + tree_name;
}

/* height_balance
 * Description: Finds the height of the left and right subtree and prints them. Also prints if the tree is balanced
 * Parameters:
 * - itemNode* node: The root of the Item tree to be checked
 * - string tree_name: The name of said item tree
 * Return: nothing
 */
void height_balance(itemNode* node, const string& tree_name) {
    int left_height = 0, right_height = 0;
    // A separate node is created to store the root of the item tree since it needs to be reset
    itemNode* item_node = node;
    // For measuring the height of the left subtree
    while(item_node) {
        // Catch for if the left subtree has no nodes
        if (left_height == 0 && item_node->left == nullptr) {
            break;
        }

        if (item_node->left != nullptr)
            left_height++;
        else if (item_node->right != nullptr) {
            item_node = item_node->right;
            left_height++;
            continue;
        }
        item_node = item_node->left;
    }

    item_node = node; // Resets item_node to the root of the item tree for a second traversal

    // For measuring the height of the right subtree
    while(item_node) {
        // Catch for if the right subtree has no nodes
        if (right_height == 0 && item_node->right == nullptr) {
            break;
        }
        if (item_node->right != nullptr)
            right_height++;
        else if (item_node->left != nullptr) {
            item_node = item_node->left;
            right_height++;
            continue;
        }
        item_node = item_node->right;
    }

    // The final output for a height_balance query
    string output_hb;
    output_hb.append(tree_name + ": left height " + to_string(left_height) + ", right height " +
        to_string(right_height) + ", difference " + to_string(right_height-left_height));
    // Checking if the BST is balanced
    if (abs(right_height - left_height) > 1)
        output_hb.append(", not balanced");
    else
        output_hb.append(", balanced");
    output_and_write(output_hb, true);
}

/* count
 * Description: Counts the total number of items in an Item tree. Not each Item node, but the count value that each item
 * has.
 * Parameters:
 * - itemNode* node: The root of the Item tree of which its nodes are counted
 * Sub-function:
 *  - count_more: Recursively looks through the tree and adds the count of each item to count, which is passed by
 *  reference into this function.
 * Return: count (int), which is the total count of every item in the tree
 */
void count_more(const itemNode* node, int* count);
int count(const itemNode* node) {
    int count = 0;
    count_more(node, &count);
    return count;
}
void count_more(const itemNode* node, int* count) {
    if (!node)
        return;
    count_more(node->left, count);
    *count += node->count;
    count_more(node->right, count);
}

/* querySelector
 * Description: Extracts queries from the input then either calls the query function or prints that said tree does
 * not exist (if needed).
 * Parameters:
 * - treeNamenode* root: The root of the Name tree
 * - int numQuery: The number of queries in the input file
 * Return: nothing
 */
void querySelector(treeNameNode* root, int numQuery) {
    for (int i = 1; i <= numQuery; i++) {
        // Extracts next line from input for processing
        string fullQuery = nextLine(); int pos = fullQuery.find(' ');
        string theQuery = fullQuery.substr(0, pos); // Extracts query term
        fullQuery = fullQuery.substr(pos+1); pos = fullQuery.find(' ');
        string tree_name = fullQuery.substr(0,pos); // Extracts tree name for where the query will execute on
        treeNameNode* node = searchNameNode(root, tree_name); // Attempts to find the node in the Name tree

        if (node!= nullptr) {
            if (theQuery == "search") {
                fullQuery = fullQuery.substr(pos+1); //item_name
                output_and_write(search(node->theTree,tree_name,fullQuery),true);
            } else if (theQuery == "item_before") {
                fullQuery = fullQuery.substr(pos+1); //item_name
                output_and_write(item_before(node->theTree,tree_name,fullQuery),true);
            } else if (theQuery == "height_balance") {
                height_balance(node->theTree,tree_name);
            } else if (theQuery == "count") {
                output_and_write(tree_name + " count: " + to_string(count(node->theTree)),true);
            } else {
                cout << "Invalid query" << endl;
            }
        } else {
            // If it can find the node in the Name tree
            output_and_write(tree_name + " does not exist", true);
        }
    }
}


int main() {
    // A small little check for example_out.txt files
    if(filesystem::exists("example_out.txt")) {
        cout << "example_out.txt exists, would you like to delete it? (Y/N)" << endl <<
            "If not deleting it, then the new output will be written below the "
            "previous outputs." << endl;
        string user_input; cin >> user_input;
        switch (user_input[0]) {
            case 'Y': case 'y':
                cout << "Deleting example_out.txt..." << endl;
                remove("example_out.txt");
                this_thread::sleep_for(2s);
                break;
            default:
                cout << "Ok then" << endl;
                break;
        }
    }

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
        }
    }

    name_tree = buildNameTree(numTreeNames);
    insertToItem(name_tree, numItems);
    traverse_in_traverse(name_tree);
    querySelector(name_tree,numQuery);

    delete name_tree;
    return 0;
}
