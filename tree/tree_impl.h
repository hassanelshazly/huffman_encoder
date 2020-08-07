#include "tree.h"

tree_node::tree_node()
    : left(nullptr), right(nullptr), data(0)
{
    // do nothing
}
tree_node::tree_node(char data)
    : left(nullptr), right(nullptr), data(data)
{
    // do nothing
}

tree_node::tree_node(tree_node *left, tree_node *right, char data)
    : left(left), right(right), data(data)
{
    // do nothing
}

inline char tree_node::get_data()
{
    return data;
}

inline tree_node *tree_node::get_left()
{
    return left;
}

inline tree_node *tree_node::get_right()
{
    return right;
}

inline void set_data(char new_data)
{
    data = new_data;
}
    
inline void set_left(tree_node * new_left)
{
    left = new_left;
}

inline void set_right(tree_node * new_right)
{
    right = new_right;
}

inline bool tree_node::is_leaf()
{
    return left == nullptr && right == nullptr;
}

// friend function
void del_tree(tree_node *node)
{
    if (node == nullptr)
        return;
    del_tree(node->get_left());
    del_tree(node->get_right());
    delete node;
}