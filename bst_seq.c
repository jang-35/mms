/*
  CSC A48 - Assignment 2 - BSTs, Traversals, a tiny Sequencer
*/

#include<stdio.h>
#include<stdlib.h>
#include"NoteSynth.c"

typedef struct BST_Node_Struct
{

    double key;
    double freq;
    int bar;
    double index;
    /*** TO DO:
     * Complete the definition of the BST_Node_Struct
     ***/

    struct BST_Node_Struct *left;
    struct BST_Node_Struct *right;
  
} BST_Node;

BST_Node *newBST_Node(double freq, int bar, double index)
{

    BST_Node *new = NULL;
    new = (BST_Node *)calloc(1, sizeof(BST_Node));
    if(new == NULL) {
        return NULL;
    }
    new->freq = freq;
    new->bar = bar;
    new->index = index;
    new->key = (10.0*bar) + index;
    new->left = NULL;
    new->right = NULL;
    return new;
}

BST_Node *BST_insert(BST_Node *root, BST_Node *new_node)
{

    double epsilon = 0.000000001;
    if(root == NULL) {
        return new_node;
    }
    if(fabs(root->key - new_node->key) <= epsilon) {
        printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n",root->bar, root->index);
        return root;
    }
    if(new_node->key < root->key) {
        root->left = BST_insert(root->left, new_node);
    }
    else {
        root->right = BST_insert(root->right, new_node);
    }
    return root;
}

BST_Node *BST_search(BST_Node *root, int bar, double index)
{

    double epsilon = 0.000000001;
    double key = 10.0 * bar + index;
    if(root == NULL) {
        return NULL;
    }
    if(fabs(root->key - key) <= epsilon) {
        return root;
    }
    if(key < root->key) {
        return BST_search(root->left, bar, index);
    }
    return BST_search(root->right, bar, index);
}

BST_Node *find_successor(BST_Node *right_child_node)
{

    if(right_child_node == NULL) {
        return NULL; //delete will never reach this b/c it has 2 children
    }
    if(right_child_node->left == NULL) {
        return right_child_node;
    }
    return(find_successor(right_child_node->left));
    
}

BST_Node *BST_delete(BST_Node *root, int bar, double index)
{

    double epsilon = 0.000000001;
    if(root == NULL) {//empty tree, or we reach bottom and do nothing (exit)
        return NULL;
    }
    double key = 10.0 * bar + index;
    BST_Node *tmp = NULL;
    if(fabs(root->key - key) <= epsilon) {//found node to delete
        //3 cases (note return root @every case)
        //case 1 delete at leaf
        if(root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        //case 2a delete (child on left only)
        else if(root->left != NULL && root->right == NULL) {
            tmp = root->left;
            free(root);
            return tmp;//connect parent of root to root->left
        }

        //case 2b delete (child on right only)
        else if(root->left == NULL && root->right != NULL) {
            tmp = root->right;
            free(root);
            return tmp;//connect parent of root to root->right
        }

        //case 3 delete (2 children)
        else {
            //copy value of succ into root
            //delete succ
            tmp = find_successor(root->right);
            root->bar = tmp->bar;
            root->freq = tmp->freq;
            root->index = tmp->index;
            root->key = tmp->key;
            root->right = BST_delete(root->right, tmp->bar, tmp->index); //delete at right subtree where succ is located
            return root;
        }

    }

    else {//update deletion at left or right
        if(key < root->key) {
            root->left = BST_delete(root->left, bar, index);
        }
        else {
            root->right = BST_delete(root->right, bar, index);
        }
    }
    return root;

}

void BST_makePlayList(BST_Node *root)
{

    if(root == NULL) { //come back up to rt
        return;
    }
    BST_makePlayList(root->left); //recurse down to bottom left
    playlist_head = playlist_insert(playlist_head, root->freq, root->bar, root->index);
    BST_makePlayList(root->right); //now recurse down to bottom right, note we don't 'reprint' this roots value
}

void BST_inOrder(BST_Node *root, int depth)
{

    if(root == NULL) {
        return;
    }
    BST_inOrder(root->left, depth + 1); //recurse to left, make sure to specify depth
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
    BST_inOrder(root->right, depth + 1);
} 

void BST_preOrder(BST_Node *root, int depth)
{

    if(root == NULL) {
        return;
    }
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
    BST_preOrder(root->left, depth + 1);
    BST_preOrder(root->right, depth + 1);
}

void BST_postOrder(BST_Node *root,int depth)
{

    if(root == NULL) {
        return;
    }
    BST_postOrder(root->left, depth + 1);
    BST_postOrder(root->right, depth + 1);
    printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);
} 

void delete_BST(BST_Node *root)
{

    if(root == NULL) {
        return;
    }
    delete_BST(root->left);
    delete_BST(root->right);
    free(root);
}

int findMaxBar(BST_Node *root) {
    if(root == NULL) {
        return -1;//empty tree
    }
    if(root->right == NULL) {//rightmost node
        return root->bar;
    }
    return findMaxBar(root->right);
}

int findMinBar(BST_Node *root) {
    if(root == NULL) {
        return -1;//empty tree
    }
    if(root->left == NULL) {//rightmost node
        return root->bar;
    }
    return findMinBar(root->left);
}

void updateInfo(BST_Node *root, int maxbar, int minbar) {
    if(root == NULL || maxbar == -1) {
        return;
    }
    updateInfo(root->left, maxbar, minbar);
    updateInfo(root->right, maxbar, minbar);
    //update freq
    BST_Node *tmp = root->left;
    root->left = root->right;
    root->right = tmp;

    //update other info
    if(root->left != NULL) {
        root->left->index = 1.0 - root->left->index; //reverse index
        root->left->bar = maxbar + minbar - root->left->bar; 
        root->left->key = 10.0 * root->left->bar + root->left->index; //update key
    }
    if(root->right != NULL) {
        root->right->index = 1.0 - root->right->index; //reverse index
        root->right->bar = maxbar + minbar - root->right->bar; 
        root->right->key = 10.0 * root->right->bar + root->right->index; //update key
    }
}

BST_Node *reverseSong(BST_Node *root)
{

    //need helper b/c function defn does not have max bar to recurse
    if(root == NULL) {
        return NULL;
    }
    int maxbar = findMaxBar(root);
    int minbar = findMinBar(root);
    updateInfo(root, maxbar, minbar);
    //messy solution, update root manually. root freq does not need to change (we pivot the song around it)
    //but we need to update the position
    root->index = 1.0 - root->index; //reverse index
    root->bar = maxbar + minbar - root->bar; //assuming bar starts @0
    root->key = 10.0 * root->bar + root->index; //update key

    return root; //root does not change after inversion
}

/********************************************************************************************
 *      Add any helper functions you need for implementing BST_harmonize() just below this
 * comment block, and above BST_Harmonize itself!
 * ******************************************************************************************/
typedef struct Note_List_Struct {
    BST_Node *node;
    struct Note_List_Struct *next;
}Note;

Note *newListNode(BST_Node *note) {
    Note *new = (Note*)calloc(1, sizeof(Note));
    if(new == NULL) {
        return NULL;
    }
    new->node = note;
    new->next = NULL;
    return new;
}

Note *list_insert(Note *head, Note* new) {
    //insert at head
    if(head == NULL) {
        return new;
    }
    new->next = head;
    return new;
}
BST_Node *note_harmonize(BST_Node *note, int semitones, double time_shift) {
    /*Given a note, create and return a harmonized version of the note*/
    double epsilon = 0.000000001;
    BST_Node *new = newBST_Node(note->freq, note->bar, note->index);
    int j = -1;//flag if freq not found                                                                
    for(int i = 0; i < 100; i++) {
        if(fabs(note->freq - note_freq[i]) < epsilon) {
            j = i;
            break;
        }
    }
    if(j != -1) {
        if(j + semitones < 100 && j + semitones >= 0) {
            j = j + semitones;
            new->freq = note_freq[j];
        }
        else {
            new->freq = -1;
        }
    }
    else {
        new->freq = -1;
    }

    if(note->index + time_shift >= 0 && note->index + time_shift < 1) {
        new->index = note->index + time_shift;
    }
    else {
        new->index = -1;
    }
    new->key = 10.0*new->bar + new->index;
    return new;
}


Note *createHarmList(BST_Node *root, Note *head, int semitones, double time_shift) {
    /* given a BST, create and return a list of harmonized notes for the BST*/

    if(root == NULL) {//base case, make sure ret head and not null
        return head;
    }
        BST_Node *new = newBST_Node(root->freq, root->bar, root->index);
        new = note_harmonize(new, semitones, time_shift);
        Note *new_h = newListNode(new);
        head = createHarmList(root->left, head, semitones, time_shift);//inorder
        head = list_insert(head, new_h);//need heads here as we are inserting at each line, do not remove head = ..
        head = createHarmList(root->right, head, semitones, time_shift);
    //only need return root harmonized b/c we insert @head
    return head;
}

void delete_list(Note *head) {
    Note *p = head;
    Note *q = head;
    while(p != NULL) {
        q = p->next; //this is ok b/c we do not use NULL p in this loop
        free(p); //free node pointed by p
        p = q; //point p to p->next
    }
}

BST_Node *BST_harmonize(BST_Node *root, int semitones, double time_shift)
{

    if(root == NULL) {
        return NULL; 
    }
    Note *head = NULL;
    head = createHarmList(root, head, semitones, time_shift);
    
    Note *p = head;
    while(p != NULL) {
        while(BST_search(root, p->node->bar, p->node->index) != NULL) {
            p->node->key += 0.0000001;
            p->node->index += 0.0000001;
        } //prevent duplicates
        if(p->node->freq != -1 && p->node->index != -1) {
            root = BST_insert(root, p->node);
        }   
        p = p->next;
    }
    delete_list(head);
    return root;
}
