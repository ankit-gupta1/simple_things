/*
 * bst.cpp
 *
 *  Created on: 06-Oct-2012
 *      Author: ankitgupta
 */

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "../include/bst.h"
#include "../include/stack.h"
#include "../include/queue.h"

using namespace std;

int test_array[NODE_COUNT];

typedef enum {
	LEFT,
	RIGHT,
	UP,
	SAME,
} state_t;

void create_tree(tree_t **tree)
{
	if (!*tree) {
		(*tree) = (tree_t *)malloc(sizeof(tree_t));
		(*tree)->root = NULL;
	}
}

void create_node(node_t **node, int val)
{
	*node = (node_t *)malloc(sizeof(node_t));
	(*node)->key = val;
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*node)->parent = NULL;
}

void inorder_tree_walk(node_t *root)
{
	if (root) {
		inorder_tree_walk(root->left);
		cout<<root->key<<" ";
		inorder_tree_walk(root->right);
	}
}

void preorder_tree_walk(node_t *root)
{
	if (root) {
		cout<<root->key<<" ";
		preorder_tree_walk(root->left);
		preorder_tree_walk(root->right);
	}
}

void postorder_tree_walk(node_t *root)
{
	if (root) {
		postorder_tree_walk(root->left);
		postorder_tree_walk(root->right);
		cout<<root->key<<" ";
	}
}

void show_tree_sorted(tree_t *tree)
{
	cout<<"\nTree in sorted order :\n";
	inorder_tree_walk(tree->root);
	cout<<"\n";
}

void show_tree_preorder(tree_t *tree)
{
	cout<<"\nTree in pre order :\n";
	preorder_tree_walk(tree->root);
	cout<<"\n";
}

void show_tree_postorder(tree_t *tree)
{
	cout<<"\nTree in post order :\n";
	postorder_tree_walk(tree->root);
	cout<<"\n";
}

node_t *tree_search(node_t *root, int val)
{
	if (!root)
		return root;

	if (root->key == val) {
		return root;
	} else {
		if (val < root->key) {
			return tree_search(root->left, val);
		} else {
			return tree_search(root->right, val);
		}
	}
}

node_t *tree_search_iterative(node_t *root, int val)
{
	node_t *node = NULL;
	node = root;
	while (node) {
		if (node->key == val) {
			break;
		}
		if (val < node->key)
			node = node->left;
		else
			node = node->right;
	}
	return node;
}

node_t *tree_minimum(node_t *root)
{
	node_t *node = NULL;
	node = root;
	while (node->left)
		node = node->left;

	return node;
}

node_t *tree_maximum(node_t *root)
{
	node_t *node = NULL;
	node = root;
	while (node->right)
		node = node->right;

	return node;
}

node_t *tree_successor(node_t *root)
{
	node_t *node = NULL, *node_p = NULL;
	node = root;
	if (!node)
		return node;

	if (node->right)
		return tree_minimum(node->right);

	node_p = node->parent;
	while (node_p) {
		if (node_p->right == node)
			break;

		node = node_p;
		node_p = node_p->parent;
	}
	return node_p;
}

node_t *tree_predecessor(node_t *root)
{
	node_t *node = NULL, *node_p = NULL;
	node = root;
	if (!node)
		return node;

	if (node->left)
		return tree_maximum(node->left);

	node_p = node->parent;
	while (node_p) {
		if (node_p->left == node)
			break;

		node = node_p;
		node_p = node_p->parent;
	}
	return node_p;
}

void tree_insertion(tree_t *tree, node_t *node_z)
{
	node_t *node_x = NULL;
	node_t *node_y = NULL;
	if (!tree || !node_z)
		return;

	node_x = tree->root;
	while (node_x) {
		node_y = node_x;
		if (node_z->key < node_x->key)
			node_x = node_x->left;
		else
			node_x = node_x->right;
	}

	node_z->parent = node_y;
	if (!node_y)
		tree->root = node_z;
	else if (node_z->key < node_y->key)
		node_y->left = node_z;
	else
		node_y->right = node_z;
}

void tree_deletion(tree_t *tree, node_t *node_z)
{
	node_t *node_x = NULL;
	node_t *node_y = NULL;
	if (!tree || !node_z)
		return;

	if (!node_z->left && !node_z->right) {
		node_y = node_z->parent;
		if (!node_y)
			tree->root = NULL;
		else if (node_y->left == node_z)
			node_y->left = NULL;
		else
			node_y->right = NULL;
		free(node_z);
	} else if (!node_z->left || !node_z->right) {
		node_y = node_z->parent;
		if (node_z->left)
			node_x = node_z->left;
		else
			node_x = node_z->right;

		if (!node_y) {
			node_x->parent = NULL;
			tree->root = node_x;
		} else {
			node_x->parent = node_y;
			if (node_y->left == node_z)
				node_y->left = node_x;
			else
				node_y->right = node_x;
		}
		free(node_z);
	} else {
		int key;
		node_y = tree_successor(node_z);
		key = node_y->key;
		tree_deletion(tree, node_y);
		node_z->key = key;
		free(node_y);
	}
}

void build_random_tree(tree_t *tree, unsigned int node_count)
{
	unsigned int i;
	node_t *node;
	if (!tree)
		return;

	for (i = 0; i < node_count; i++) {
		srand(time(NULL) * i + i * i + 7);
		test_array[i] = (rand() % 1000 + 1);
		create_node(&node, test_array[i]);
		tree_insertion(tree, node);
		node = NULL;
		free(node);
	}
}

int tree_height(node_t *node)
{
	int lh = 0, rh = 0;

	if(!node)
		return 0;

	lh += tree_height(node->left);
	rh += tree_height(node->right);

	return lh > rh ? (lh + 1) : (rh + 1);
}

int tree_height_iterative(tree_t *tree)
{
	int height = 0;

	queue_t *q1 = NULL;
	queue_t *q2 = NULL;
	queue_t *qt = NULL;

	node_t *node = NULL;
	if (!tree)
		return 0;

	node = tree->root;
	create_queue(&q1);
	create_queue(&q2);
	create_queue(&qt);

	enqueue(q1, node);

	while (q1->front) {
		node = dequeue(q1);

		while (node) {
			if (node->left)
				enqueue(q2, node->left);

			if (node->right)
				enqueue(q2, node->right);

			node = dequeue(q1);
		}

		/*Swap the queues. */
		qt = q1;
		q1 = q2;
		q2 = qt;

		height++;
	}

	free_queue(&q1);
	free_queue(&q2);

	return height;
}

void inorder_tree_walk_iterative(tree_t* tree)
{
	stack_t *stack = NULL;
	node_t *node = NULL;
	if (!tree)
		return;

	create_stack(&stack);
	cout<<"\nIterative sorted order\n";
	node = tree->root;

	while (stack->head || node) {
		if (node) {
			push(stack, node);
			node = node->left;
		} else {
			node = pop(stack);
			cout<<node->key<<" ";
			node = node->right;
		}
	}

	cout<<"\n";
	free_stack(&stack);
}

void morris_traversal(tree_t *tree)
{
  struct node_t *curr = NULL;
  struct node_t *pred = NULL;

  if (!tree->root)
     return;

  curr = tree->root;
  cout<<"\nMorris Traversal order w/o stack\n";
  while (curr) {
    if (!curr->left) {
      cout<<curr->key<<" ";
      curr = curr->right;
    } else {
      /* Find the in order predecessor of current */

      pred = curr->left;
      while (pred->right && (pred->right != curr))
        pred = pred->right;

      /* Make current as right child of its in order predecessor */
      if (!pred->right) {
        pred->right = curr;
        curr = curr->left;
      }

      /* Revert the changes made in if part to restore the original
        tree i.e., fix the right child of predecssor */
      else {
        pred->right = NULL;
        cout<<curr->key<<" ";
        curr = curr->right;
      }
    }
  }

  cout<<endl;
}

void inorder_tree_walk_iterative_wo_stack(tree_t *tree)
{
	node_t *curr_node = NULL;
	node_t *prev_node = NULL;
	if (!tree)
		return;

	curr_node = tree->root;
	cout<<"\nIterative sorted order w/o stack\n";
	while (curr_node) {
		if (prev_node == curr_node->parent) {
			prev_node = curr_node;
			if (curr_node->left)
				curr_node = curr_node->left;
			else {
				cout<<curr_node->key<<" ";
				if (curr_node->right)
					curr_node = curr_node->right;
				else
					curr_node = curr_node->parent;
			}
		} else if (prev_node == curr_node->left) {
			cout<<curr_node->key<<" ";
			prev_node = curr_node;
			if (curr_node->right)
				curr_node = curr_node->right;
			else
				curr_node = curr_node->parent;
		} else if (prev_node == curr_node->right) {
			prev_node = curr_node;
			curr_node = curr_node->parent;
		}
	}

	cout<<endl;
}

void preorder_tree_walk_iterative(tree_t* tree)
{
	stack_t *stack = NULL;
	node_t *node = NULL;
	if (!tree)
		return;

	create_stack(&stack);
	cout<<"\nIterative preorder\n";
	node = tree->root;

	while (stack->head || node) {
		if (node) {
			cout<<node->key<<" ";
			push(stack, node);
			node = node->left;
		} else {
			node = pop(stack);
			node = node->right;
		}
	}

	cout<<"\n";
	free_stack(&stack);
}



void preorder_tree_walk_iterative_wo_stack(tree_t *tree)
{
	node_t *curr_node = NULL;
	node_t *prev_node = NULL;
	if (!tree)
		return;

	curr_node = tree->root;
	cout<<"\nPre-order walk w/o stack\n";
	while (curr_node) {
		if (prev_node == curr_node->parent) {
			cout<<curr_node->key<<" ";
			prev_node = curr_node;
			if (curr_node->left) {
				curr_node = curr_node->left;
			} else {
				if (curr_node->right)
					curr_node = curr_node->right;
				else
					curr_node = curr_node->parent;
			}
		} else if (prev_node == curr_node->left) {
			prev_node = curr_node;
			if (curr_node->right)
				curr_node = curr_node->right;
			else
				curr_node = curr_node->parent;
		} else if (prev_node == curr_node->right) {
			prev_node = curr_node;
			curr_node = curr_node->parent;
		}
	}

	cout<<endl;
}

void switch_state(node_t **node, stack_t **stack,
		state_t &state)
{
	switch (state) {
		case LEFT :
			*node = (*node)->left;
			break;
		case RIGHT :
			*node = (*node)->right;
			break;
		case UP :
			*node = top(*stack);
			break;
		case SAME :
			break;
	}
	state = SAME;
}

void postorder_tree_walk_iterative(tree_t* tree)
{
	stack_t *stack_trav = NULL;
	stack_t *stack_out = NULL;
	node_t *node = NULL;

	if (!tree)
		return;

	create_stack(&stack_trav);
	create_stack(&stack_out);

	cout<<"\nIterative post order\n";
	node = tree->root;

	if (!node)
		return;

	push(stack_trav, node);
	while (stack_trav->head) {
		node = pop(stack_trav);
		push(stack_out, node);
		if (node->left)
			push(stack_trav, node->left);
		if (node->right)
			push(stack_trav, node->right);
	}

	while (stack_out->head)
		cout<<pop(stack_out)->key<<" ";

	cout<<"\n";
	free_stack(&stack_trav);
	free_stack(&stack_out);
}

void postorder_tree_walk_iterative2(tree_t* tree)
{
	stack_t *stack_trav = NULL;
	node_t *curr_node = NULL;
	node_t *prev_node = NULL;
	state_t state;

	if (!tree)
		return;

	create_stack(&stack_trav);
	cout<<"\nIterative post order (O(log n) space complexity)\n";
	curr_node = tree->root;
	if (!curr_node)
		return;

	state = SAME;
	push(stack_trav, curr_node);

	if (curr_node->left) {
		push(stack_trav, curr_node->left);
		state = LEFT;
	} else if (curr_node->right) {
		push(stack_trav, curr_node->right);
		state = RIGHT;
	}

	prev_node = curr_node;
	switch_state(&curr_node, &stack_trav, state);

	while (stack_trav->head) {
		if ((prev_node->left == curr_node) ||
			(prev_node->right == curr_node)) {
			if (curr_node->left) {
				push(stack_trav, curr_node->left);
				state = LEFT;
			}
			else if (curr_node->right) {
				push(stack_trav, curr_node->right);
				state = RIGHT;
			}
		} else if (curr_node->left == prev_node) {
			if (curr_node->right) {
				push(stack_trav, curr_node->right);
				state = RIGHT;
			}
		} else {
			cout<<pop(stack_trav)->key<<" ";
			state = UP;
		}
		prev_node = curr_node;
		switch_state(&curr_node, &stack_trav, state);
	}

	cout<<"\n";
	free_stack(&stack_trav);
}

void update_info(node_t *node, stack_t **stack,
				stack_t **stack_n1, stack_t **stack_n2,
				int n1, int n2)
{
	if (node->key == n1)
		copy_stack(&(*stack), &(*stack_n1));

	if (node->key == n2)
		copy_stack(&(*stack), &(*stack_n2));
}

void lowest_common_ancestor(tree_t *tree, int n1, int n2)
{
	stack_t *stack_trav = NULL;
	stack_t *stack_n1 = NULL;
	stack_t *stack_n2 = NULL;

	node_stack_t *node_n1 = NULL;
	node_stack_t *node_n2 = NULL;
	node_t *curr_node = NULL;
	node_t *prev_node = NULL;

	int least_key = ~(1 << 31);
	int nearest_key = 0;
	int distance_n1_n2 = 0;
	bool have_ancestor = false;
	state_t state;

	cout<<"\nLCA algorithm\n";

	if (!tree)
		return;

	create_stack(&stack_trav);
	create_stack(&stack_n1);
	create_stack(&stack_n2);
	curr_node = tree->root;
	if (!curr_node)
		return;

	state = SAME;

	update_info(curr_node, &stack_trav, &stack_n1,
				&stack_n2, n1, n2);
	push(stack_trav, curr_node);

	if (curr_node->left) {
		update_info(curr_node->left, &stack_trav, &stack_n1,
					&stack_n2, n1, n2);
		push(stack_trav, curr_node->left);
		state = LEFT;
	} else if (curr_node->right) {
		update_info(curr_node->right, &stack_trav, &stack_n1,
					&stack_n2, n1, n2);
		push(stack_trav, curr_node->right);
		state = RIGHT;
	}

	prev_node = curr_node;
	switch_state(&curr_node, &stack_trav, state);

	while (stack_trav->head) {
		if ((prev_node->left == curr_node) ||
			(prev_node->right == curr_node)) {
			if (curr_node->left) {
				update_info(curr_node->left, &stack_trav, &stack_n1,
							&stack_n2, n1, n2);
				push(stack_trav, curr_node->left);
				state = LEFT;
			}
			else if (curr_node->right) {
				update_info(curr_node->right, &stack_trav, &stack_n1,
							&stack_n2, n1, n2);
				push(stack_trav, curr_node->right);
				state = RIGHT;
			}
		} else if (curr_node->left == prev_node) {
			if (curr_node->right) {
				update_info(curr_node->right, &stack_trav, &stack_n1,
							&stack_n2, n1, n2);
				push(stack_trav, curr_node->right);
				state = RIGHT;
			}
		} else {
			pop(stack_trav);
			state = UP;
		}

		prev_node = curr_node;
		switch_state(&curr_node, &stack_trav, state);
	}

	if (!stack_n1)
		return;

	if (!stack_n2)
		return;

	node_n1 = stack_n1->head;
	node_n2 = stack_n2->head;

	while (node_n1 || node_n2) {
		if (node_n1 && node_n2) {
			if (node_n1->node->key == node_n2->node->key) {
				if (least_key > node_n1->node->key)
					least_key = node_n1->node->key;

				nearest_key = node_n1->node->key;
				have_ancestor = true;
			}
		} else {
			if (node_n1)
				distance_n1_n2++;

			if (node_n2)
				distance_n1_n2++;
		}

		if (node_n1)
			node_n1 = node_n1->next;

		if (node_n2)
			node_n2 = node_n2->next;
	}

	if (have_ancestor) {
		cout<<"\n N1 = "<<n1<<" N2 = "<<n2<<endl;
		cout<<"\nLowest common ancestor : "<<least_key<<endl;
		cout<<"\nNearest common ancestor : "<<nearest_key<<endl;
		cout<<"\nDistance between n1 and n2 : "<<distance_n1_n2 + 2<<endl;
	} else
		cout<<"\nDo not have common ancestor\n";

	free_stack(&stack_trav);
	free_stack(&stack_n1);
	free_stack(&stack_n2);
}

void postorder_tree_walk_iterative_wo_stack(tree_t *tree)
{
	node_t *curr_node = NULL;
	node_t *prev_node = NULL;
	if (!tree)
		return;

	curr_node = tree->root;
	cout<<"\nPost-order walk w/o stack\n";
	while (curr_node) {
		if (prev_node == curr_node->parent) {
			prev_node = curr_node;
			if (curr_node->left)
				curr_node = curr_node->left;
			else {
				if (curr_node->right)
					curr_node = curr_node->right;
				else {
					cout<<curr_node->key<<" ";
					curr_node = curr_node->parent;
				}
			}
		} else if (prev_node == curr_node->left) {
			prev_node = curr_node;
			if (curr_node->right)
				curr_node = curr_node->right;
			else {
				cout<<curr_node->key<<" ";
				curr_node = curr_node->parent;
			}
		} else if (prev_node == curr_node->right) {
			prev_node = curr_node;
			cout<<curr_node->key<<" ";
			curr_node = curr_node->parent;
		}
	}

	cout<<endl;
}

void level_order_tree_traversal	(tree_t* tree)
{
	queue_t *q1 = NULL;
	node_t *node = NULL;
	if (!tree)
		return;

	node = tree->root;
	create_queue(&q1);
	enqueue(q1, node);

	cout<<"\nLevel order traversal\n";
	while (node && q1->front) {
		node = dequeue(q1);
		cout<<node->key<<" ";
		if (node->left)
			enqueue(q1, node->left);

		if (node->right)
			enqueue(q1, node->right);
	}
	cout<<"\n";
	free_queue(&q1);
}

void free_tree(tree_t **tree)
{
	queue_t *q1 = NULL;
	node_t *node = NULL;
	if (!tree)
		return;

	node = (*tree)->root;
	create_queue(&q1);
	enqueue(q1, node);

	while (q1->front) {
		node = dequeue(q1);
		if (node->left)
			enqueue(q1, node->left);
		if (node->right)
			enqueue(q1, node->right);

		free(node);
	}

	free_queue(&q1);
	free(*tree);
	*tree = NULL;
}
