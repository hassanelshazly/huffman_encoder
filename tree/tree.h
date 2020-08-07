#ifndef _TREE_H_
#define _TREE_H_

class tree_node
{
public:
    /**
     * Default constructor
     */
    tree_node();

    /**
     * construct a leaf node with just data
     */
    tree_node(char data);

    /**
     * construct node with left, right and data
     */
    tree_node(tree_node *left, tree_node *right, char data);

    /**
     * @return data
     */
    char get_data();

    /**
     * @return left child
     */
    tree_node *get_left();

    /**
     * @return right child
     */
    tree_node *get_right();

    /**
     * overwite the data
     */
    void set_data(char new_data);

    /**
     * overwite left child
     */
    void set_left(tree_node * new_left);

    /**
     * overwite right child
     */
    void set_right(tree_node * new_right);

    /**
     * @return ture if leaf node
     */
    bool is_leaf();

    /**
     * friend method to delete a given tree
     * @param node root of the tree
     */
    friend void del_tree(tree_node *node);

private:
    tree_node *left;
    tree_node *right;
    char data;
};

// include implemtation
#include "tree_impl.h"

#endif // End of the file