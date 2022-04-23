
#include<memory>
#include<iostream>
#include<queue>
using namespace std;
enum Status{FALSE = 0,TRUE};
enum {RH = -1,EH,LH};

typedef struct BTNode
{
	int data;
	int BF;  /* Balance Factor */
	struct BTNode* lchild, * rchild;
	BTNode(int x):data(x),BF(0),lchild(nullptr),rchild(nullptr){}
} *BTree


/************************** 
递归查找二叉排序树T中是否存在key 
指针f指向T的父节点，其初始调用值为NULL
若查找成功，则指针p指向该数据元素节点，并返回TRUE
否则指针指向查找路径上最后一个节点并返回FALSE
***************************/
Status SearchBST(BTree cur, int key, BTree curFather, BTree* ret)
{
	if (!cur) {
		*ret = curFather;
		return FALSE;
	}
	else if (key == cur->data) {
		*ret = cur;
		return TRUE;
	}
	else if (key < cur->data)
		return SearchBST(cur->lchild, key, cur, ret);
	else
		return SearchBST(cur->lchild, key, cur, ret);
}

/**************************
当二叉排序树T中不存在关键字等于key的数据元素时
插入key并返回TRUE，否则返回FLASE
***************************/
Status InsertBST(BTree* cur, int key) {
	BTree ret, temp;
	if (!SearchBST(*cur, key, nullptr, &ret)) { //树中无相同元素
		temp = (BTree)malloc(sizeof(BTNode));
		temp->data = key;
		temp->lchild = temp->rchild = nullptr;
		if (!ret)
			*cur = temp;           /* 插入s为新节点 */
		else if (key < ret->data)
			ret->lchild = temp;    /* 插入s为左孩子 */
		else
			ret->rchild = temp;    /* 插入s为右孩子 */
		return TRUE;
	}
	else
		return FALSE;  //树中已含有值相同元素
}

/**************************
从二叉排序树中删除节点p，并重接其左或右子树
***************************/
Status Delete(BTree* cur) {
	BTree temp, child;
	temp = *cur;
	if ((*cur)->rchild == nullptr) { //若右子树为空，则只需重接其左子树 
		*cur = (*cur)->lchild;
		free(temp);
	}
	else if ((*cur)->lchild == nullptr) { //若左子树为空，则只需重接其右子树
		*cur = (*cur)->rchild;
		free(temp);
	}
	else                            //左右子树皆不为空
	{
		child = (*cur)->lchild;
		while (child->rchild)           //寻找和删除目标相差最小的前驱
		{
			temp = child;
			child = child->rchild;
		}
		(*cur)->data = child->data;     //s指向被删节点的直接前驱
		if (temp != *cur)
			temp->rchild = child->lchild;//重接q的右子树
		else
			temp->lchild = child->lchild;//重接q的左子树
		free(child);
	}
	return TRUE;
}

/**************************
若二叉排序树T中存在关键字等于key的数据元素时，则删除该数据元素的节点
删除成功则返回TRUE，否则返回FALSE
***************************/
Status DeleteBST(BTree* cur, int key) {
	if (!*cur) //无删除元素
		return FALSE;
	else {
		if (key == (*cur)->data)
			return Delete(cur);
		else if (key < (*cur)->data)
			return DeleteBST(&(*cur)->lchild, key);
		else
			return DeleteBST(&(*cur)->rchild, key);
	}	
}

/**************************
以cur为根的二叉排序树作右旋操作
处理之后cur指向新的根节点，即旋转处理之前的左子树根节点
***************************/
void R_Rotate(BTree& cur)
{
	BTree temp;
	temp = cur->lchild;        // temp赋值为cur的左子树
	cur->lchild = temp->rchild;// cur的左子树赋值为temp的右子树
	temp->rchild = cur;        // temp的右子树赋值为cur
	cur = temp;                // cur赋值为temp
}

/**************************
以cur为根的二叉排序树作左旋操作
处理之后cur指向新的根节点，即旋转处理之前的右子树根节点
***************************/
void L_Rotate(BTree& cur)
{
	BTree temp;
	temp = cur->rchild;        // temp赋值为cur的右子树
	cur->rchild = temp->lchild;// cur的右子树赋值为temp的左子树
	temp->lchild = cur;        // temp的左子树赋值为cur
	cur = temp;                // cur赋值为temp
}

/**************************
对以cur所指结点为根的二叉树作左平衡旋转处理
本算法结束时，cur指向新的根结点
***************************/
void LeftBalance(BTree& cur)
{
	BTree temp, tempr;
	temp = cur->lchild;
	switch (temp->BF)
	{//检查cur的左子树平衡度，并作相应平衡处理
	case LH://新节点插入在cur的左孩子的左子树上，对cur做单右旋处理。
		cur->BF = temp->BF = EH;
		R_Rotate(cur);
		break;
	case RH://新节点插入在cur的左孩子的右子树上，对cur做双旋处理。
		tempr = temp->rchild;
		switch (tempr->BF)//修改cur及其左孩子的BF
		{
		case LH:
			cur->BF = RH;
			temp->BF = EH;
			break;
		case EH:
			cur->BF = temp->BF = EH;
			break;
		case RH:
			cur->BF = EH;
			temp->BF = LH;
			break;
		}
		tempr->BF = EH;
		L_Rotate(cur->lchild);
		R_Rotate(cur);
		break;
	default:
		break;
	}
}

/**************************
对以cur所指结点为根的二叉树作左平衡旋转处理
本算法结束时，cur指向新的根结点
***************************/
void RightBalance(BTree& cur)
{
	BTree temp, templ;
	temp = cur->rchild;
	switch (temp->BF)
	{//检查cur的右子树平衡度，并作相应平衡处理
	case RH://新节点插入在cur的左孩子的右子树上，对cur做单左旋处理。
		cur->BF = temp->BF = EH;
		L_Rotate(cur);
		break;
	case LH://新节点插入在cur的右孩子的左子树上，对cur做双旋处理。
		templ = temp->lchild;
		switch (templ->BF)//修改cur及其右孩子的BF
		{
		case RH:
			cur->BF = LH;
			temp->BF = EH;
			break;
		case EH:
			cur->BF = temp->BF = EH;
			break;
		case LH:
			cur->BF = EH;
			temp->BF = RH;
			break;
		}
		templ->BF = EH;
		R_Rotate(cur->rchild);
		L_Rotate(cur);
		break;
	default:
		break;
	}
}

/**************************
若在平衡的二叉排序树T中不存在和data_in有同关键字的节点，则插入一个
数据元素为data_in的新节点并返回1，否则返回0.
若因插入而使二叉排序树失衡，则作平衡旋转处理，
布尔变量taller反映T长高与否
***************************/
Status InsertAVL(BTree& T, int data_in, Status& taller) 
{
	if (!T) {/* 插入新结点，树“长高”，置taller为TRUE */
		T = (BTree)malloc(sizeof(BTNode));
		T->data = data_in;
		T->lchild = T->rchild= nullptr;
		T->BF = EH;
		taller = TRUE;
	}
	else {
		if (data_in == T->data) {//树中存在和data_in相同关键字的结点则不再插入
			taller = FALSE;     //不插入则树 不长高
			return FALSE;
		}
		if (data_in < T->data) {//继续在T的左子树中查找
			if (!InsertAVL(T->lchild, data_in, taller))//插入失败
				return FALSE;
			if (taller) {       //插入成功且左子树长高
				switch (T->BF) {//检查T的平衡
				case LH:        //原树左倾，进行左平衡
					LeftBalance(T);
					taller = FALSE;
					break;
				case EH:        //原树平衡，左子树增高导致树左倾
					T->BF = LH;
					taller = TRUE;
					break;
				case RH:        //原树右倾，左子树增高导致树平衡
					T->BF = EH;
					taller = FALSE;
					break;
				}
			}
		}
		else {                      //继续在T的右子树中查找
			if (!InsertAVL(T->rchild, data_in, taller))//插入失败
				return FALSE;
			if (taller) {       //插入成功且右子树长高
				switch (T->BF) {//检查T的平衡
				case RH:        //原树右倾，进行右平衡
					RightBalance(T);
					taller = FALSE;
					break;
				case EH:        //原树平衡，右子树增高导致树右倾
					T->BF = RH;
					taller = TRUE;
					break;
				case LH:        //原树左倾，右子树增高导致树平衡
					T->BF = EH;
					taller = FALSE;
					break;
				}
			}
		}
	}
	return TRUE;
}

int GetTreeDepth(BTree& T)
{
	if (!T)
	{
		return 0;
	}

	int left = GetTreeDepth(T->lchild);
	int right = GetTreeDepth(T->rchild);

	return (left >= right ? left : right) + 1;
}
int GetTreeCount(BTree& T) {
	if (!T)
	{
		return 0;
	}

	int left = GetTreeCount(T->lchild);
	int right = GetTreeCount(T->rchild);

	return left+right+1;
}

int exnum(const int& num, const int& exponent)
{
	int x=1;
	for (int i = 0; i < exponent; i++)
		x *= num;
	return x;
}
void coutw(int x,const char * s)
{
	for (int i = 0; i < x; i++)
		cout << s;
}

void PreTraverse(BTree& T)
{
	if(T) {
		cout << T->data << " ";
		PreTraverse(T->lchild);
		PreTraverse(T->rchild);
	}
}
void InTraverse(BTree& T)
{
	if (T) {
		InTraverse(T->lchild);
		cout << T->data << " ";
		InTraverse(T->rchild);
	}
}
void PostTraverse(BTree& T)
{
	if (T) {
		PostTraverse(T->lchild);
		PostTraverse(T->rchild);
		cout << T->data << " ";
	}
}
void LevelTraverse(BTree& T)
{
	int depth = GetTreeDepth(T);
	BTNode* flag =new BTNode(-1);
	// 1.初始化
	queue<BTree> q;
	if (T == nullptr) { // 二叉树为空
		return;
	}
	q.push(T);
	// 2.遍历整棵树
	while (1) {
		coutw(exnum(2, depth)," ");
		int cnt = q.size(); // 要处理层的节点个数
		int flag2=0;

		for (int i = 0; i < cnt; i++)
		{
			if (q.front()->data != -1)
				flag2 = 1;
			q.push(q.front());
			q.pop();
		}

		if (!flag2)break;	
				
		// 3.遍历该层
		while (cnt--) {
			BTree cur = q.front();
			q.pop();

			// 4.对 cur 的操作，根据题意更改
			if (cur->data == -1)
				cout << " ";
			else
				cout << cur->data;
				coutw(exnum(2, depth+1) - 1, " ");
			// 5.将左右孩子放入队列
				if (cur->lchild) q.push(cur->lchild);
				else q.push(flag);
				if (cur->rchild) q.push(cur->rchild);
				else q.push(flag);
		}
		cout << endl;
		depth--;
	}
	delete flag;
}

void LevelTraversecpy(BTree & T)
	{
		// 1.初始化
		queue<BTree> q;
		if (T == nullptr) { // 二叉树为空
			return;
		}
		q.push(T);
		// 2.遍历整棵树
		while (1) {
			int cnt = q.size(); // 要处理层的节点个数
			if (cnt == 0) break; // 已经遍历完二叉树
			// 3.遍历该层
			while (cnt--) {
				BTree cur = q.front();
				q.pop();

				// 4.对 cur 的操作，根据题意更改
				cout << cur->data << " ";




				// 5.将左右孩子放入队列
				if (cur->lchild) q.push(cur->lchild);
				if (cur->rchild) q.push(cur->rchild);
			}
		}
	}


int main()
{

	int a[] = {9,7,6,5,8,1,3,4,2,13,24,56,78,90,54,66,32,12,22,79};
	BTree T =nullptr;
	Status taller;
	for (auto x:a) {
		InsertAVL(T, x, taller);
		LevelTraverse(T);
		coutw(50, "*");
		cout << endl;
	}
	/*PreTraverse(T);
	cout << endl;
	InTraverse(T);
	cout << endl;
	PostTraverse(T);
	cout << endl;
	LevelTraverse(T);
	cout << endl;*/
	
	system("pause");
	return 0;
}