#define RED true
#define BLACK false

using namespace std;

template <class keytype>
template <class valuetype>
class RBTree
{
public:
	RBTree()
	{
		root = NULL;
	}

	valuetype * search(keytype k)
	{
		Node x = root;

		while(x != NULL)
		{
			int cmp = compareKey(k, x.key);
			if(cmp == 0) return &x.val;
			else if(cmp < 0) x = x.left;
			else if(cmp > 0) x = x.right;
		}

		return NULL;
	}

	void insert(keytype k, valuetype v)
	{
		root = _insert(root, k, v);
		root.color = BLACK;
	}

private:
	struct Node
	{
		keytype key;
		valuetype val;
		Node left, right;
		bool color;

		Node(keytype _key, valuetype _val)
		{
			key = _key;
			val = _val;
			color = RED;
		}
	}

	Node root;

	int compareKey(keytype k1, keytype k2)
	{
		if(k1 < k2)	return -1;
		if(k1 > k2)	return 1;
		if(k1 == k2) return 0;
	}

	Node _insert(Node n, keytype k, valuetype v)
	{
		if(n == NULL)	return Node(k, v);

		if(n.left.color && n.right.color)	colorFlip(n);

		int cmp = compareKey(k, n.key);
		if(cmp == 0)	n.val = v;
		else if (cmp < 0)	n.left = _insert(n.left, k, v);
		else 				n.right = _insert(n.right, k, v);

		if(n.right.color && !n.left.color)	n = rotateLeft(n);
		if(n.left.color && !n.left.left.color)	n = rotateRight(n);

		return n;
	}

	Node rotateLeft(Node n)
	{
		Node x = n.right;
		n.right = x.left;
		x.left = n;
		x.color = n.color;
		n.color = RED;
		return x;
	}

	Node rotateRight(Node n)
	{
		Node x = n.left;
		n.left = x.right;
		x.left = n;
		x.color = n.color;
		n.color = RED;
		return x;
	}

	void colorFlip(Node n)
	{
		n.color = !n.color;
		n.left.color = !n.left.color;
		n.right.color = !n.right.color;
	}

};