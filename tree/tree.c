#include "tree.h"

struct private_node
{
    private_node* prev_;
    private_node* left_;
    private_node* right_;
    node* data_;
    int key_;
};

struct tree
{
    struct private_node* root_;
};

void* mymalloc(int size)
{
    static int counter = 0;
    if (counter % 5 == 3)
    {
        counter++;
        return NULL;
    }

    counter++;
    return calloc(1, size);
}

tree* new_tree()
{
    tree* new = (tree*)mymalloc(sizeof(tree));
    if (new == NULL)
    {
        return NULL;
    }
    new->root_ = NULL;
    return new;
}

int tree_insert(tree* tree, int key, node* data_node)
{
    if (tree == NULL)
        return -1;

    private_node* new = (private_node*)mymalloc(sizeof(private_node));
    node* tmp = (node*)mymalloc(sizeof(node));
    if (new == NULL || tmp == NULL)
    {
        free(new);
        free(tmp);
        return -1;
    }
    tmp->data_ = data_node->data_;
    new->key_ = key;
    new->data_ = tmp;

    private_node* prev = NULL;
    private_node** rubish;
    private_node* cur = tree_search_private(tree, key, &prev, &rubish);
    if (cur != NULL)
    {
        free(new);
        free(tmp);
        return -1;
    }


    new->prev_ = prev;
    if (prev == NULL)
    {
        tree->root_ = new;
    }
    else if (new->key_ < prev->key_)
    {
        prev->left_ = new;
    }
    else
    {
        prev->right_ = new;
    }

    return 0;
}

int tree_delete(tree* tree, int key)
{
    if (tree == NULL)
        return -1;

    private_node** prev_pos = NULL;
    private_node* rubish;
    private_node* cur = tree_search_private(tree, key, &rubish, &prev_pos);
    if (cur == NULL)
    {
        return -1;
    }

    if (cur->left_ == NULL && cur->right_ == NULL)
    {
        *prev_pos = NULL;
        free(cur->data_);
        free(cur);
    }
    else if (cur->left_ != NULL && cur->right_ == NULL)
    {
        *prev_pos = cur->left_;
        free(cur->data_);
        free(cur);
    }
    else if (cur->left_ == NULL && cur->right_ != NULL)
    {
        *prev_pos = cur->right_;
        free(cur->data_);
        free(cur);
    }
    else
    {
        private_node* next = search_next_private(tree, key);
        if (next == cur->right_)
        {
            *prev_pos = next;
            next->left_ = cur->left_;
            free(cur->data_);
            free(cur);
        }
        else
        {
            next->prev_->left_ = next->right_;
            if (prev_pos != NULL)
            {
                *prev_pos = next;
            }
            else
            {
                tree->root_ = next;
            }

            next->left_ = cur->left_;
            next->right_ = cur->right_;
            free(cur->data_);
            free(cur);
        }
    }

    return 0;
}

node* tree_search(tree* tree, int key)
{
    if (tree == NULL)
        return NULL;

    private_node* rubish1;
    private_node** rubish2;
    private_node* result = tree_search_private(tree, key, &rubish1, &rubish2);
    if (result == NULL)
    {
        return NULL;
    }

    return result->data_;
}

private_node* tree_search_private(tree* tree, int key, private_node** prev, private_node*** prev_pos)
{
    private_node* cur = tree->root_;
    while (cur != NULL && cur->key_ != key)
    {
        *prev = cur;
        if (key < cur->key_)
        {
            *prev_pos = &(cur->left_);
            cur = cur->left_;
        }
        else
        {
            *prev_pos = &(cur->right_);
            cur = cur->right_;
        }
    }

    return cur;
}

int search_next(tree* tree, int key)
{
    if (tree == NULL)
        return -1;

    private_node* result = search_next_private(tree, key);
    if (result == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    return result->key_;
}

private_node* search_next_private(tree* tree, int key)
{
    private_node* rubish1;
    private_node** rubish2;
    private_node* cur = tree_search_private(tree, key, &rubish1, &rubish2);
    if (cur == NULL)
    {
        return NULL;
    }

    if (cur->right_ != NULL)
    {
        return tree_min_private(cur->right_);
    }

    private_node* prev = cur->prev_;
    while (prev != NULL && cur == prev->right_)
    {
        cur = prev;
        prev = prev->prev_;
    }

    return prev;
}

int tree_min(tree* tree)
{
    if (tree == NULL)
        return -1;

    private_node* result = tree_min_private(tree->root_);
    if (result == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    return result->key_;
}

private_node* tree_min_private(private_node* root)
{
    if (root == NULL)
    {
        return NULL;
    }

    private_node* cur = root;
    while (cur->left_ != NULL)
    {
        cur = cur->left_;
    }

    return cur;
}

int tree_max(tree* tree)
{
    if (tree == NULL)
        return -1;

    private_node* result = tree_max_private(tree->root_);
    if (result == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    return result->key_;
}

private_node* tree_max_private(private_node* root)
{
    if (root == NULL)
    {
        return NULL;
    }

    private_node* cur = root;
    while (cur->right_ != NULL)
    {
        cur = cur->right_;
    }

    return cur;
}

void print(tree* tree)
{
    if (tree != NULL)
    {
        print_private(tree->root_);
    }
}

void print_private(private_node* root)
{
    private_node* cur = root;
    if (cur != NULL)
    {
        print_private(cur->left_);
        printf("{%d, %c} ", cur->key_, cur->data_->data_);
        print_private(cur->right_);
    }
}

void tree_destroy(tree** tree)
{
    if (tree != NULL && (*tree)->root_ != NULL)
    {
        tree_destroy_private((*tree)->root_);
        (*tree)->root_ = NULL;
    }

    free(*tree);
    *tree = NULL;
}

void tree_destroy_private(private_node* root)
{
    if (root->left_ != NULL)
    {
        tree_destroy_private(root->left_);
    }

    if (root->right_ != NULL)
    {
        tree_destroy_private(root->right_);
    }

    free(root->data_);
    free(root);
}