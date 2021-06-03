
#include <iostream>
#include <utility>
#include <stdint.h>


/// <summary>
/// 2021.06.03
/// 标准二岔搜索树。暂不考虑数据域
/// </summary>
template <typename K>
class BST {
public:
	struct BSTNode {
		K key;
		BSTNode* left, * right;
		BSTNode(const K& k) :key(k), left(nullptr), right(nullptr) {}
	};

	BST() :root(nullptr) {}
	BST(const BST& t);
	BST(BST&& t);
	~BST();

	BST& operator=(const BST& t);
	BST& operator=(BST&& t);

	const BSTNode* get_insert_parent(const K& key)const;
	const BSTNode* find_parent(const K& key)const;
	BSTNode* insert(const K& key);
	BSTNode* find(const K& key);
	inline const BSTNode* find(const K& key)const {
		return const_cast<BST*>(this)->find(key);
	}

	BSTNode* copy()const;

	inline const BSTNode* get_root()const {
		return root;
	}

private:
	BSTNode* root;
	BSTNode* copy_nodes(const BSTNode* r)const;
	void destruct_node(BSTNode* r);
	const BSTNode* get_insert_parent(const K& key, const BSTNode* n)const;
	const BSTNode* find_parent(const K& key, const BSTNode* n)const;
	BSTNode* insert(const K& key, BSTNode* n);
	BSTNode* find(const K& key, BSTNode* n);

};

template<typename K>
BST<K>::BST(const BST<K>& t) :root(t.copy())
{
}

template<typename K>
BST<K>::BST(BST&& t) : root(t.root)
{
	t.root = nullptr;
}

template<typename K>
BST<K>::~BST()
{
	destruct_node(root);
	root = nullptr;
}

template<typename K>
BST<K>& BST<K>::operator=(const BST& t)
{
	root = t.copy();
	return *this;
}

template<typename K>
BST<K>& BST<K>::operator=(BST&& t)
{
	root = t.root;
	t.root = nullptr;
}

template<typename K>
const typename BST<K>::BSTNode* BST<K>::get_insert_parent(const K& key) const
{
	return get_insert_parent(key, root);
}

template<typename K>
const typename BST<K>::BSTNode* BST<K>::find_parent(const K& key) const
{
	return find_parent(key, root);
}

template<typename K>
typename BST<K>::BSTNode* BST<K>::insert(const K& key)
{
	if (!root) {
		root = new BSTNode(key);
		return root;
	}
	return insert(key, root);
}

template<typename K>
typename BST<K>::BSTNode* BST<K>::find(const K& key)
{
	return find(key, root);
}

template<typename K>
typename BST<K>::BSTNode* BST<K>::copy() const
{
	return copy_nodes(root);
}

template<typename K>
typename BST<K>::BSTNode* BST<K>::copy_nodes(const BSTNode* r) const
{
	if (!r)
		return nullptr;
	auto* n = new BSTNode(r->key);
	n->left = copy_nodes(r->left);
	n->right = copy_nodes(r->right);
	return n;
}

template<typename K>
void BST<K>::destruct_node(BSTNode* r)
{
	if (!r)
		return;
	destruct_node(r->left);
	destruct_node(r->right);
	delete r;
}