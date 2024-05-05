// Creator: Sivoshenko Aleksandr Yurievich
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Node {
    struct Node *left, *right;
    int key;
} Node;


Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

void freeTree(Node* root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

Node* splay(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (root->key > key) {
        if (root->left == NULL) return root;

        if (root->left->key > key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        else if (root->left->key < key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return (root->left == NULL)? root: rightRotate(root);
    }
    else {
        if (root->right == NULL) return root;

        if (root->right->key > key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (root->right->key < key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        return (root->right == NULL)? root: leftRotate(root);
    }
}

//для первичного добавления в дерево, без учета продвижения элемента в корень
Node* insert(Node* root, int k) {
    if (root == NULL) return newNode(k);

    root = splay(root, k);

    if (root->key == k) return root;

    Node* newnode = newNode(k);

    if (root->key > k) {
        newnode->right = root;
        newnode->left = root->left;
        root->left = NULL;
    }
    else {
        newnode->left = root;
        newnode->right = root->right;
        root->right = NULL;
    }

    return newnode;
}

void print(Node* root, int space) {
    if (root == NULL) return;
    space += 10;
    print(root->right, space);
    printf("\n");
    for (int i = 10; i < space; i++) printf(" ");
    printf("%d\n", root->key);
    print(root->left, space);
}

Node* merge(Node* left, Node* right) {
    if (left == NULL) return right;
    if (right == NULL) return left;

    left = splay(left, right->key);
    left->right = right;
    return left;
}

typedef struct SplitResult {
    Node* left;
    Node* right;
} SplitResult;

SplitResult split(Node* root, int key) {
    SplitResult result;
    if (root == NULL) {
        result.left = NULL;
        result.right = NULL;
        return result;
    }

    root = splay(root, key);

    if (root->key < key) {
        result.left = root;
        result.right = root->right;
        if (result.right != NULL) {
            result.left->right = NULL;
        }
    } else {
        result.right = root;
        result.left = root->left;
        if (result.left != NULL) {
            result.right->left = NULL;
        }
    }

    return result;
}

Node* add(Node* root, Node* x) {
    if (root != NULL) {
        root = splay(root, x->key);
        if (root->key == x->key) {
            printf("Warn: Key already exists in the tree\n");
            return root;
        }
    }

    SplitResult result = split(root, x->key);
    x->left = result.left;
    x->right = result.right;
    
    return x;
}


void test_search1() {
    Node *root = newNode(100);
    root->left = newNode(50);
    root->right = newNode(200);
    root->left->left = newNode(40);
    root->left->right = newNode(60);
    root = splay(root, 60);
    assert(root->key == 60);
    assert(root->left->key == 50);
    assert(root->left->left->key == 40);

    printf("test_search1 passed\n");
    freeTree(root);
}

void test_search2() {
    Node *root = newNode(100);
    root->left = newNode(50);
    root->right = newNode(200);
    root->left->left = newNode(40);
    root->left->right = newNode(60);
    root = splay(root, 23);
    assert(root->key == 40);

    printf("test_search2 passed\n");
    freeTree(root);
}

void test_merge1() {
    Node *root1 = newNode(100);
    root1 = insert(root1, 50);
    root1 = insert(root1, 200);
    root1 = insert(root1, 40);
    root1 = insert(root1, 60);

    Node *root2 = newNode(150);
    root2 = insert(root2, 120);
    root2 = insert(root2, 250);
    root2 = insert(root2, 110);
    root2 = insert(root2, 130);

    Node *root = merge(root1, root2);

    assert(root->key == 200);
    assert(root->left->key == 100);

    printf("test_merge1 passed\n");
    freeTree(root);
}

void test_merge2() {
    Node *root1 = newNode(200);
    root1 = insert(root1, 150);

    Node *root = newNode(200);
    root = insert(root, 150);
    root = insert(root, 250);
    root = insert(root, 100);
    root = insert(root, 175);

    root = merge(root, root1);

    assert(root->key == 150);
    assert(root->left->key == 100);

    printf("test_merge2 passed\n");
    freeTree(root);
}

void test_split1() {
    Node *root = newNode(100);
    root = insert(root, 50);
    root = insert(root, 200);
    root = insert(root, 40);
    root = insert(root, 60);

    SplitResult result = split(root, 60);
    Node *left = result.left;
    Node *right = result.right;

    assert(left->key == 50);
    assert(right->key == 60);

    printf("test_split1 passed\n");
    freeTree(left);
    freeTree(right);
}

void test_split2() {
    Node *root = newNode(100);
    root = insert(root, 50);
    root = insert(root, 200);
    root = insert(root, 40);
    root = insert(root, 60);

    SplitResult result = split(root, 70);
    Node *left = result.left;
    Node *right = result.right;

    assert(left->key == 60);
    assert(right->key == 100);

    printf("test_split2 passed\n");
    freeTree(left);
    freeTree(right);
}

void test_add1() {
    Node *root = newNode(100);
    root = insert(root, 50);
    root = insert(root, 200);
    root = insert(root, 40);
    root = insert(root, 60);

    Node *x = newNode(70);
    root = add(root, x);

    assert(root->key == 70);
    assert(root->left->key == 60);
    assert(root->right->key == 100);

    printf("test_add1 passed\n");
    freeTree(root);
}

void test_add2() {
    Node *root = newNode(100);
    root = insert(root, 50);
    root = insert(root, 200);
    root = insert(root, 40);
    root = insert(root, 60);

    Node *x = newNode(60);
    root = add(root, x);

    assert(root->key == 60);
    assert(root->left->key == 50);
    assert(root->right->key == 100);

    printf("test_add2 passed\n");
    freeTree(root);
}


int main() {
    test_search1();
    test_search2();
    test_merge1();
    test_merge2();
    test_split1();
    test_split2();
    test_add1();
    test_add2();
    return 0;
}