
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
�ݹ���Ҷ���������T���Ƿ����key 
ָ��fָ��T�ĸ��ڵ㣬���ʼ����ֵΪNULL
�����ҳɹ�����ָ��pָ�������Ԫ�ؽڵ㣬������TRUE
����ָ��ָ�����·�������һ���ڵ㲢����FALSE
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
������������T�в����ڹؼ��ֵ���key������Ԫ��ʱ
����key������TRUE�����򷵻�FLASE
***************************/
Status InsertBST(BTree* cur, int key) {
	BTree ret, temp;
	if (!SearchBST(*cur, key, nullptr, &ret)) { //��������ͬԪ��
		temp = (BTree)malloc(sizeof(BTNode));
		temp->data = key;
		temp->lchild = temp->rchild = nullptr;
		if (!ret)
			*cur = temp;           /* ����sΪ�½ڵ� */
		else if (key < ret->data)
			ret->lchild = temp;    /* ����sΪ���� */
		else
			ret->rchild = temp;    /* ����sΪ�Һ��� */
		return TRUE;
	}
	else
		return FALSE;  //�����Ѻ���ֵ��ͬԪ��
}

/**************************
�Ӷ�����������ɾ���ڵ�p�����ؽ������������
***************************/
Status Delete(BTree* cur) {
	BTree temp, child;
	temp = *cur;
	if ((*cur)->rchild == nullptr) { //��������Ϊ�գ���ֻ���ؽ��������� 
		*cur = (*cur)->lchild;
		free(temp);
	}
	else if ((*cur)->lchild == nullptr) { //��������Ϊ�գ���ֻ���ؽ���������
		*cur = (*cur)->rchild;
		free(temp);
	}
	else                            //���������Բ�Ϊ��
	{
		child = (*cur)->lchild;
		while (child->rchild)           //Ѱ�Һ�ɾ��Ŀ�������С��ǰ��
		{
			temp = child;
			child = child->rchild;
		}
		(*cur)->data = child->data;     //sָ��ɾ�ڵ��ֱ��ǰ��
		if (temp != *cur)
			temp->rchild = child->lchild;//�ؽ�q��������
		else
			temp->lchild = child->lchild;//�ؽ�q��������
		free(child);
	}
	return TRUE;
}

/**************************
������������T�д��ڹؼ��ֵ���key������Ԫ��ʱ����ɾ��������Ԫ�صĽڵ�
ɾ���ɹ��򷵻�TRUE�����򷵻�FALSE
***************************/
Status DeleteBST(BTree* cur, int key) {
	if (!*cur) //��ɾ��Ԫ��
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
��curΪ���Ķ�������������������
����֮��curָ���µĸ��ڵ㣬����ת����֮ǰ�����������ڵ�
***************************/
void R_Rotate(BTree& cur)
{
	BTree temp;
	temp = cur->lchild;        // temp��ֵΪcur��������
	cur->lchild = temp->rchild;// cur����������ֵΪtemp��������
	temp->rchild = cur;        // temp����������ֵΪcur
	cur = temp;                // cur��ֵΪtemp
}

/**************************
��curΪ���Ķ�������������������
����֮��curָ���µĸ��ڵ㣬����ת����֮ǰ�����������ڵ�
***************************/
void L_Rotate(BTree& cur)
{
	BTree temp;
	temp = cur->rchild;        // temp��ֵΪcur��������
	cur->rchild = temp->lchild;// cur����������ֵΪtemp��������
	temp->lchild = cur;        // temp����������ֵΪcur
	cur = temp;                // cur��ֵΪtemp
}

/**************************
����cur��ָ���Ϊ���Ķ���������ƽ����ת����
���㷨����ʱ��curָ���µĸ����
***************************/
void LeftBalance(BTree& cur)
{
	BTree temp, tempr;
	temp = cur->lchild;
	switch (temp->BF)
	{//���cur��������ƽ��ȣ�������Ӧƽ�⴦��
	case LH://�½ڵ������cur�����ӵ��������ϣ���cur������������
		cur->BF = temp->BF = EH;
		R_Rotate(cur);
		break;
	case RH://�½ڵ������cur�����ӵ��������ϣ���cur��˫������
		tempr = temp->rchild;
		switch (tempr->BF)//�޸�cur�������ӵ�BF
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
����cur��ָ���Ϊ���Ķ���������ƽ����ת����
���㷨����ʱ��curָ���µĸ����
***************************/
void RightBalance(BTree& cur)
{
	BTree temp, templ;
	temp = cur->rchild;
	switch (temp->BF)
	{//���cur��������ƽ��ȣ�������Ӧƽ�⴦��
	case RH://�½ڵ������cur�����ӵ��������ϣ���cur������������
		cur->BF = temp->BF = EH;
		L_Rotate(cur);
		break;
	case LH://�½ڵ������cur���Һ��ӵ��������ϣ���cur��˫������
		templ = temp->lchild;
		switch (templ->BF)//�޸�cur�����Һ��ӵ�BF
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
����ƽ��Ķ���������T�в����ں�data_in��ͬ�ؼ��ֵĽڵ㣬�����һ��
����Ԫ��Ϊdata_in���½ڵ㲢����1�����򷵻�0.
��������ʹ����������ʧ�⣬����ƽ����ת����
��������taller��ӳT�������
***************************/
Status InsertAVL(BTree& T, int data_in, Status& taller) 
{
	if (!T) {/* �����½�㣬�������ߡ�����tallerΪTRUE */
		T = (BTree)malloc(sizeof(BTNode));
		T->data = data_in;
		T->lchild = T->rchild= nullptr;
		T->BF = EH;
		taller = TRUE;
	}
	else {
		if (data_in == T->data) {//���д��ں�data_in��ͬ�ؼ��ֵĽ�����ٲ���
			taller = FALSE;     //���������� ������
			return FALSE;
		}
		if (data_in < T->data) {//������T���������в���
			if (!InsertAVL(T->lchild, data_in, taller))//����ʧ��
				return FALSE;
			if (taller) {       //����ɹ�������������
				switch (T->BF) {//���T��ƽ��
				case LH:        //ԭ�����㣬������ƽ��
					LeftBalance(T);
					taller = FALSE;
					break;
				case EH:        //ԭ��ƽ�⣬���������ߵ���������
					T->BF = LH;
					taller = TRUE;
					break;
				case RH:        //ԭ�����㣬���������ߵ�����ƽ��
					T->BF = EH;
					taller = FALSE;
					break;
				}
			}
		}
		else {                      //������T���������в���
			if (!InsertAVL(T->rchild, data_in, taller))//����ʧ��
				return FALSE;
			if (taller) {       //����ɹ�������������
				switch (T->BF) {//���T��ƽ��
				case RH:        //ԭ�����㣬������ƽ��
					RightBalance(T);
					taller = FALSE;
					break;
				case EH:        //ԭ��ƽ�⣬���������ߵ���������
					T->BF = RH;
					taller = TRUE;
					break;
				case LH:        //ԭ�����㣬���������ߵ�����ƽ��
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
	// 1.��ʼ��
	queue<BTree> q;
	if (T == nullptr) { // ������Ϊ��
		return;
	}
	q.push(T);
	// 2.����������
	while (1) {
		coutw(exnum(2, depth)," ");
		int cnt = q.size(); // Ҫ�����Ľڵ����
		int flag2=0;

		for (int i = 0; i < cnt; i++)
		{
			if (q.front()->data != -1)
				flag2 = 1;
			q.push(q.front());
			q.pop();
		}

		if (!flag2)break;	
				
		// 3.�����ò�
		while (cnt--) {
			BTree cur = q.front();
			q.pop();

			// 4.�� cur �Ĳ����������������
			if (cur->data == -1)
				cout << " ";
			else
				cout << cur->data;
				coutw(exnum(2, depth+1) - 1, " ");
			// 5.�����Һ��ӷ������
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
		// 1.��ʼ��
		queue<BTree> q;
		if (T == nullptr) { // ������Ϊ��
			return;
		}
		q.push(T);
		// 2.����������
		while (1) {
			int cnt = q.size(); // Ҫ�����Ľڵ����
			if (cnt == 0) break; // �Ѿ������������
			// 3.�����ò�
			while (cnt--) {
				BTree cur = q.front();
				q.pop();

				// 4.�� cur �Ĳ����������������
				cout << cur->data << " ";




				// 5.�����Һ��ӷ������
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