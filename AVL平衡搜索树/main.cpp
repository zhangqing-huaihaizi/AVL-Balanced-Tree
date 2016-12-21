//AVL���ڵ���Ϣ
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
	void insertpri(TreeNode<T>* &node, T x);//����
	TreeNode<T>* findpri(TreeNode<T>* node, T x);
	void insubtree(TreeNode<T> *node);//�������
	void Deletepri(TreeNode<T>* &node, T x);
	int height(TreeNode<T> *node); //�����ĸ߶�
	void SingRotateLeft(TreeNode<T>* &k2);//LL����µ���ת
	void SingRotateRight(TreeNode<T>* &k2);//RR����µ���ת
	void DoubleRotateLR(TreeNode<T>* &k3);//LR����µ���ת
	void DoubleRotateRL(TreeNode<T>* &k3); //RL����µ���ת
	int Max(int cmpa, int cmpb); //�����ֵ
public:
	AVLTree:root(NULL){}
	void insert(T x); //����ӿ�
	TreeNode<T>* find(T x);//���ҽӿ�
	void Delete(T x); //ɾ���ӿ�
	void traversal();//�����ӿ�
};

//��ת�㷨��Ҫ�����������ܵĸ�����һ���������ĸ߶ȣ�һ�����������߶ȵ����ֵ
//�涨��һ�ÿ����ĸ߶�Ϊ-1��ֻ��һ�����ڵ�����ĸ߶�Ϊ0
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

//��ת
//(1)��������µ���ת
template<class T>
void AVLTree<T>::SingRotateLeft(TreeNode<T>* &k2){
	TreeNode<T>* k1;
	k1 = k2->lson;
	k2->lson = k1->rson;
	k1->rson = k2;

	//����k1,k2�ĸ߶�
	k2->hgt = Max(height(k2->lson), height(k2->rson)) + 1;
	k1->hgt = Max(height(k1->lson), height(k1->lson)) + 1;
}
//(2)��������µ���ת
template<class T>
void AVLTree<T>::SingRotateRight(TreeNode<T>* &k2){
	TreeNode<T>* k1;
	k1 = k2->rson;
	k2->rson = k1->lson;
	k1->lson = k2;

	//���¸߶�
	k2->hgt = Max(height(k2->lson), height(k2->rson)) + 1;
	k1->hgt = Max(height(k1->lson), height(k1->rson)) + 1;
}
//�����������ת
template<class T>
void AVLTree<T>::DoubleRotateLR(TreeNode<T>* &k3){
	SingRotateRight(k3->lson);
	SingRotateLeft(k3);
}
//�����������ת
template<class T>
void AVLTree<T>::DoubleRotateRL(TreeNode<T>* &k3){
	SingRotateLeft(k3->rson);
	SingRotateRight(k3);
}
//���룺������ɺ���Ҫ�Ӳ���Ľڵ㿪ʼά��һ�������ڵ��·����û����һ���ڵ㶼Ҫά��
//����ƽ�⡣ά������ƽ��Ҫ���ݸ߶Ȳ���ص�ѡ��ͬ����ת�㷨
template<class T>
void AVLTree<T>::insertpri(TreeNode<T>* &node, T x){
	if(node == NULL){ //����ڵ�Ϊ�գ����ڴ˽ڵ㴦����x��Ϣ
		node = new TreeNode<T>();
		node->data = x;
		return;
	}
	if(node->data > x){ //���xС�ڽڵ��ֵ���ͼ����ڽڵ���������в���x
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
//����ӿ�
template<class T>
void AVLTree<T>::insertpri(T x){
	insertpri(root, x);
}

//����
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
//���ҽӿ�
template<class T>
TreeNode<T>* AVLTree<T>::find(T x){
	return findpri(root, x);
}

//ɾ����ɾ���ķ���Ҳ�Ͷ����������һ�£������ǣ�ɾ����ɺ���Ҫ��ɾ���ڵ�
//�ĸ��׿�ʼ����ά������ƽ��һֱ�����ڵ�
template<class T>
void AVLTree<T>::Deletepri(TreeNode<T>* &node, T x){
	if(node == NULL) return;
	if(x < node->data){
		Deletepri(node->lson, x);
		//ɾ��������������Ҫ��������ұ��ǲ�����ȴ��������
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
	}else{ //�����ȣ��˽ڵ����Ҫɾ���Ľڵ�
		if(node->lson && node->rson){ //�˽ڵ�����������
			TreeNode<T>* temp = node->rson; //tempָ��ڵ���Ҷ���
			while(temp->lson != NULL) temp = temp->lson; //�ҵ���������ֵ��С�Ľڵ�
			
			//������������С�Ľڵ㸳�����ڵ�
			node->data = temp->data;
			node->freq = temp->freq;
			//ɾ������������Сֵ�Ľڵ�
			if(height(node->lson) - height(node->rson) == 2){
				if(node->lson->rson != NULL && (height(node->lson->rson) > height(node->lson->lson)))
					DoubleRotateLR(node);
				else
					SingRotateLeft(node);
			}
		}else{ //�˽ڵ���1�����ӻ���0������
			TreeNode<T>* temp = node;//�������Ҫ�У�Ҫ��Ȼ���Ҳ������ָ�룬ɾ��������
			if(node->lson == NULL)//���Ҷ��ӻ���û�ж���
				node = node->rson;
			else(node->rson == NULL)//�������
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
//ɾ���ӿ�
template<class T>
void AVLTree<T>::Delete(T x){
	Deletepri(root, x);
}

//�����������
template<class T>
void AVLTree<T>::insubtree(TreeNode<T> *node){
	if(node == NULL) return;
	insubtree(node->lson);
	cout<<node->data<<" ";
	insubtree(node->rson);
}
//��������ӿ�
template<class T>
void AVLTree<T>::traversal(){
	insubtree(root);
}