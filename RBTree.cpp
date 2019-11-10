#define RED true
#define BLACK false

using namespace std;

template <class keytype, class valuetype>
class RBTree
{
public:
	RBTree()
	{
		root = nullptr;
		tree_size = 0;
	}

	RBTree(keytype keys[], valuetype values[], int s)
	{
		//printf("STARTED CONSTRUCTOR\n");
		tree_size = 0;
		root = nullptr;

		for(int i = 0; i < s; i++)
		{
			//cout << "Inserting node" << i+1 << ", with key: " << keys[i] << " and value: " << values[i] << endl;
			insert(keys[i], values[i]);
		}
	}

	valuetype * search(keytype k)
	{
		Node *x = root;

		while(x != nullptr) //NULL ACTS AS FALSE, IF X IS NULL END LOOP
		{
			int cmp = compareKey(k, x->key);
			if(cmp == 0) return &x->val;
			else if(cmp < 0) x = x->left;
			else if(cmp > 0) x = x->right;
		}

		return nullptr;
	}

	void insert(keytype k, valuetype v)
	{
		root = _insert(root, k, v);
		root->color = BLACK;
		tree_size++;
	}

	int remove(keytype k)
	{
		if(search(k) == nullptr)
		{
			return 0;
		}

		if(!isRed(root->left) && !isRed(root->right))
		{
			root->color = RED;
		}

		root = _remove(root, k);

		if(root != nullptr)
		{
			root->color = BLACK;
		}

		return 1;
	}

	int rank(keytype key)
	{
		return _rank(root, key) + 1;
	}

	int size()
	{
		return size(root);
	}

	void preorder()
	{
		_preorder(root);
		cout << endl;
	}

	void inorder()
	{
		_inorder(root);
		cout << endl;
	}

	void postorder()
	{
		_postorder(root);
		cout << endl;
	}

	keytype select(int order)
	{
		Node *x = _select(root, order - 1);
		return x->key;
	}


private:
	struct Node
	{
		keytype key;
		valuetype val;
		Node *left, *right;
		int size;
		bool color;

		Node(keytype _key, valuetype _val)
		{
			key = _key;
			val = _val;
			color = RED;
			left = nullptr;
			right = nullptr;
			size = 1;
		}
	};

	Node* root;
	int tree_size;

	void _inorder(Node* node)
	{
		if(node == nullptr)
		{
			//cout << "NuLL" << endl;
			return;
		}

		_inorder(node->left);

		cout << node->key << " ";

		_inorder(node->right);

	}

	void _preorder(Node* node)
	{
		if(node == nullptr)
		{
			return;
		}

		cout << node->key << " ";

		_preorder(node->left);

		_preorder(node->right);
	}

	void _postorder(Node* node)
	{
		if(node == nullptr)
		{
			return;
		}

		_postorder(node->left);

		_postorder(node->right);

		cout << node->key << " ";
	}

	int _rank(Node* n, keytype key)
	{
		if(n == nullptr)
		{
			return 0;
		}

		int cmp  = compareKey(key, n->key);
		if(cmp < 0)
		{
			return _rank(n->left, key);
		}
		else if(cmp > 0)
		{
			return 1 + size(n->left) + _rank(n->right, key);
		}
		else
		{
			return size(n->left);
		}
	}


	int size(Node* n)
	{
		if(n == nullptr)
		{
			return 0;
		}

		return n->size;
	}

	Node* moveRedLeft(Node *n)
	{
		colorFlip(n);

		if(isRed(n->right->left))
		{
			n->right = rotateRight(n->right);
			n = rotateLeft(n);
			colorFlip(n);
		}

		return n;
	}

	Node* moveRedRight(Node *n)
	{
		colorFlip(n);

		if(isRed(n->left->left))
		{
			n = rotateRight(n);
			colorFlip(n);
		}

		return n;
	}

	Node* _remove(Node *n, keytype k)
	{
		if(compareKey(k, n->key) < 0)
		{
			if(!isRed(n->left) && !isRed(n->left->left))
			{
				n = moveRedLeft(n);
			}

			n->left = _remove(n->left, k);
		}
		else
		{
			if(isRed(n->left))
			{
				n = rotateRight(n);
			}
			if(compareKey(k, n->key) == 0 && (n->right == nullptr))
			{
				return nullptr;
			}
			//POTENTIAL SEGFAULT HERE
			if(!isRed(n->right) && !isRed(n->right->left))
			{
				n = moveRedRight(n);
			}
			if(compareKey(k, n->key) == 0)
			{
				Node *x = min(n->right);
				n->key = x->key;
				n->val = x->val;

				n->right = deleteMin(n->right);

			}
			else
			{
				n->right = _remove(n->right, k);
			}
		}

		return balance(n);
	}

	valuetype _search(Node *x, keytype k)
	{

		while(x != nullptr) //NULL ACTS AS FALSE, IF X IS NULL END LOOP
		{
			int cmp = compareKey(k, x->key);
			if(cmp == 0) return x->val;
			else if(cmp < 0) x = x->left;
			else if(cmp > 0) x = x->right;
		}

		return NULL;
	}

	int compareKey(keytype k1, keytype k2)
	{
		if(k1 < k2)	return -1;
		if(k1 > k2)	return 1;
		if(k1 == k2) return 0;
	}

	bool isRed(Node *n)
	{
		if(n == nullptr)
		{
			return BLACK;
		}

		return n->color;
	}

	Node* balance(Node *h)
	{
		if(isRed(h->right))
		{
			h = rotateLeft(h);
		}

		if(isRed(h->left) && isRed(h->left->left))
		{
			h = rotateRight(h);
		}

		if(isRed(h->left) && isRed(h->right))
		{
			colorFlip(h);
		}

		h->size = size(h->left) + size(h->right) + 1;
		return h;
	}

	Node* min(Node *x)
	{
		if(x->left == nullptr)
		{
			return x;
		}
		else
		{
			return min(x->left);
		}
	}

	Node* deleteMin(Node *h)
	{
		if(h->left == nullptr)
		{
			return nullptr;
		}

		if(!isRed(h->left) && !isRed(h->left->left))
		{
			h = moveRedLeft(h);
		}

		h->left = deleteMin(h->left);
		return balance(h);
	}

	Node* _insert(Node* n, keytype k, valuetype v)
	{
		if(n == nullptr)
		{
			//printf("ADDED NEW NODE\n");
			return new Node(k, v);
		}	


		bool left_clr, right_clr;
		if(n->left == nullptr)
		{
			left_clr = BLACK;
		}
		else
		{
			left_clr = n->left->color;
		}

		if(n->right == nullptr)
		{
			right_clr = BLACK;
		}
		else
		{
			right_clr = n->right->color;
		}


		if(left_clr && right_clr)
		{
			//printf("COLOR FLIP\n");
			colorFlip(n);
		}

		int cmp = compareKey(k, n->key);
		if(cmp == 0)
		{
			n->val = v;
		}	
		else if (cmp < 0)
		{
			//printf("PAGE LEFT\n");
			//n->size_left++;
			n->left = _insert(n->left, k, v);
		}	
		else
		{
			//printf("PAGE RIGHT\n");
			//n->size_right++;
			n->right = _insert(n->right, k, v);
		}

		if(n->left == nullptr)
		{
			left_clr = BLACK;
		}
		else
		{
			left_clr = n->left->color;
		}

		if(n->right == nullptr)
		{
			right_clr = BLACK;
		}
		else
		{
			right_clr = n->right->color;
		}

		if(right_clr && !left_clr)
		{
			//cout << "ROTATE LEFT" << endl;
			n = rotateLeft(n);
		}

		bool left2_clr;
		if(n->left->left == nullptr)
		{
			left2_clr = BLACK;
		}
		else
		{
			left2_clr = n->left->left->color;
		}

		if(left_clr && left2_clr)
		{
			//cout << "ROTATE RIGHT" << endl;
			n = rotateRight(n);
		}	

		n->size = size(n->left) + size(n->right) + 1;

		return n;
	}

	Node* rotateRight(Node *n)
	{
		Node *x = n->left;

		n->left = x->right;
		x->right = n;
		
		x->color = x->right->color;
		x->right->color = RED;

		x->size = n->size;
		n->size = size(n->left) + size(n->right) + 1;

		return x;
	}

	Node* rotateLeft(Node *n)
	{
		Node *x = n->right;
		n->right = x->left;
		x->left = n;

		x->color = x->left->color;
		x->left->color = RED;

		x->size = n->size;
		n->size = size(n->left) + size(n->right) + 1;
		return x;
	}

	void colorFlip(Node *n)
	{
		n->color = !n->color;
		n->left->color = !n->left->color;
		n->right->color = !n->right->color;
	}

	Node* _select(Node *x, int order)
	{
		int l_sze = size(x->left);
		if(l_sze > order)
		{
			return _select(x->left, order);
		}
		else if(l_sze < order)
		{
			return _select(x->right, order - l_sze - 1);
		}
		else
		{
			return x;
		}
	}

};