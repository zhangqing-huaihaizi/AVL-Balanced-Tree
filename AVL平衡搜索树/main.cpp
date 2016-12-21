//AVL树节点信息
template<class T>
class TreeNode{
public:
	TreeNode():lson(NULL),rson(NULL),freq(1),hgt(0){}
	T data;
	int hgt;
	unsigned int freq;
	TreeNode* lson;
	TreeNode* rson;
};

template<class T>
class AVLTree{
private:
	TreeNode<T>* root;
	void insertpri(TreeNode<T>* &node, T x);//插入
	TreeNode<T>* findpri(TreeNode<T>* node, T x);
	void insubtree(TreeNode<T> *node);//中序遍历
	void Deletepri(TreeNode<T>* &node, T x);
	int height(TreeNode<T> *node); //求树的高度
	void SingRotateLeft(TreeNode<T>* &k2);//LL情况下的旋转
	void SingRotateRight(TreeNode<T>* &k2);//RR情况下的旋转
	void DoubleRotateLR(TreeNode<T>* &k3);//LR情况下的旋转
	void DoubleRotateRL(TreeNode<T>* &k3); //RL情况下的旋转
	int Max(int cmpa, int cmpb); //求最大值
public:
	AVLTree:root(NULL){}
	void insert(T x); //插入接口
	TreeNode<T>* find(T x);//查找接口
	void Delete(T x); //删除接口
	void traversal();//遍历接口
};

//旋转算法需要借助两个功能的辅助：一个是求树的高度，一个是求两个高度的最大值
//规定：一棵空树的高度为-1，只有一个根节点的树的高度为0
template<class T>
int AVLTree::height(TreeNode<T>* node){
	if(node != NULL)
		return node->hgt;
	return -1;
}
template<class T>
int AVLTree<T>::Max(int cmpa, int cmpb){
	return cmpa > cmpb ? cmpa : cmpb;
}

//旋转
//(1)左左情况下的旋转
template<class T>
void AVLTree<T>::SingRotateLeft(TreeNode<T>* &k2){
	TreeNode<T>* k1;
	k1 = k2->lson;
	k2->lson = k1->rson;
	k1->rson = k2;

	//更新k1,k2的高度
	k2->hgt = Max(height(k2->lson), height(k2->rson)) + 1;
	k1->hgt = Max(height(k1->lson), height(k1->lson)) + 1;
}
//(2)右右情况下的旋转
template<class T>
void AVLTree<T>::SingRotateRight(TreeNode<T>* &k2){
	TreeNode<T>* k1;
	k1 = k2->rson;
	k2->rson = k1->lson;
	k1->lson = k2;

	//更新高度
	k2->hgt = Max(height(k2->lson), height(k2->rson)) + 1;
	k1->hgt = Max(height(k1->lson), height(k1->rson)) + 1;
}
//左右情况的旋转
template<class T>
void AVLTree<T>::DoubleRotateLR(TreeNode<T>* &k3){
	SingRotateRight(k3->lson);
	SingRotateLeft(k3);
}
//右左情况的旋转
template<class T>
void AVLTree<T>::DoubleRotateRL(TreeNode<T>* &k3){
	SingRotateLeft(k3->rson);
	SingRotateRight(k3);
}
//插入：插入完成后，需要从插入的节点开始维护一个到根节点的路径，没经过一个节点都要维持
//树的平衡。维持树的平衡要根据高度差的特点选择不同的旋转算法
template<class T>
void AVLTree<T>::insertpri(TreeNode<T>* &node, T x){
	if(node == NULL){ //如果节点为空，就在此节点处加入x信息
		node = new TreeNode<T>();
		node->data = x;
		return;
	}
	if(node->data > x){ //如果x小于节点的值，就继续在节点的左子树中插入x
		insertpri(node->lson, x);
		if(height(node->lson) - height(node->rson) == 2)
			if(x < node->lson->data)
				SingRotateLeft(node);
			else
				DoubleRotateLR(node);
	}else if(node->data < x){
		insertpri(node->rson, x);
		if(height(node->rson) - height(node->lson)== 2)
			if(x > node->rson->data)
				SingRotateRight(node);
			else
				DoubleRotateRL(node);
	}
	else ++(node->freq);
	node->hgt = Max(height(node->lson), height(node->rson));
}
//插入接口
template<class T>
void AVLTree<T>::insertpri(T x){
	insertpri(root, x);
}

//查找
template<class T>
TreeNode<T> AVLTree<T>::findpri(TreeNode<T>* node, T x){
	if(node == NULL){
		return NULL;
	}
	if(node->data > x){
		return findpri(node->lson, x);
	}else if(node->data < x){
		return findpri(node->rson, x);
	}
	else return node;
}
//查找接口
template<class T>
TreeNode<T>* AVLTree<T>::find(T x){
	return findpri(root, x);
}

//删除：删除的方法也和二叉查找树的一致，区别是，删除完成后，需要从删除节点
//的父亲开始向上维护树的平衡一直到根节点
template<class T>
void AVLTree<T>::Deletepri(TreeNode<T>* &node, T x){
	if(node == NULL) return;
	if(x < node->data){
		Deletepri(node->lson, x);
		//删除了左子树，就要看这个点右边是不是深度大于了左边
		if(height(node->rson) - height(node->lson) == 2)
			if(node->rson->lson != NULL && (height(node->rson->lson) > height(node->rson->rson)))
				DoubleRotateRL(node);
			else
				SingRotateRight(node);
	}else if(x > node->data){
		Deletepri(node->rson, x);
		if(height(node->lson) - height(node->rson) == 2)
			if(node->lson->rson != NULL && (height(node->lson->rson) > height(node->lson->lson)))
				DoubleRotateLR(node);
			else
				SingRotateLeft(node);
	}else{ //如果相等，此节点就是要删除的节点
		if(node->lson && node->rson){ //此节点有两个儿子
			TreeNode<T>* temp = node->rson; //temp指向节点的右儿子
			while(temp->lson != NULL) temp = temp->lson; //找到右子树中值最小的节点
			
			//把右子树中最小的节点赋给本节点
			node->data = temp->data;
			node->freq = temp->freq;
			//删除右子树中最小值的节点
			if(height(node->lson) - height(node->rson) == 2){
				if(node->lson->rson != NULL && (height(node->lson->rson) > height(node->lson->lson)))
					DoubleRotateLR(node);
				else
					SingRotateLeft(node);
			}
		}else{ //此节点有1个儿子或者0个儿子
			TreeNode<T>* temp = node;//这个必须要有，要不然就找不到这个指针，删除不了了
			if(node->lson == NULL)//有右儿子或者没有儿子
				node = node->rson;
			else(node->rson == NULL)//有左儿子
				node = node->lson;
			delete(temp); 
			temp = NULL;
		}
		if(node == NULL)
			return;
		node->hgt = Max(height(node->lson), height(node->rson)) + 1;
		return;
	}
}
//删除接口
template<class T>
void AVLTree<T>::Delete(T x){
	Deletepri(root, x);
}

//中序遍历函数
template<class T>
void AVLTree<T>::insubtree(TreeNode<T> *node){
	if(node == NULL) return;
	insubtree(node->lson);
	cout<<node->data<<" ";
	insubtree(node->rson);
}
//中序遍历接口
template<class T>
void AVLTree<T>::traversal(){
	insubtree(root);
}