#define  _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

struct node 
{
	int data;
	node* next;
};

int SList_Create01(node** head)//尾插法 需要三个辅助指针 phead, pnew, pcur
{
	node	* phead = NULL, * pnew = NULL, * pcur = NULL;
	int		each = 0;
	phead = (node*)malloc(sizeof(node));
	if (phead == NULL)
		return -1;

	phead->data = 0;
	phead->next = NULL;
	pcur = phead;

	printf("请输入data值");
	scanf("%d", &each);

	while (each != -1)
	{
		pnew = (node*)malloc(sizeof(node));
		if (pnew == NULL)
			return -1;
		pnew->data = each;
		pnew->next = NULL;

		pcur->next = pnew;
		pcur = pnew;

		printf("请输入data值");
		scanf("%d", &each);
	}
	*head = phead;
	return 0;
}

node* SList_Create02()//头插法  需要两个辅助指针 phead, pnew
{
	node* phead = NULL, * pnew = NULL;
	int		each = 0;
	phead = (node*)malloc(sizeof(node));
	if (phead == NULL)
		return NULL;

	phead->data = 0;
	phead->next = NULL;

	while (scanf("%d", &each) != -1)
	{
		pnew = (node*)malloc(sizeof(node));
		if (pnew == NULL)
			return NULL;
		pnew->data = each;
		pnew->next = NULL;

		pnew->next = phead->next;
		phead->next = pnew;
	}
	return phead;
}

int SList_Print(node* phead) //需要一个辅助指针 pcur
{
	if (phead == NULL)
		return -1;

	node* pcur = NULL;
	pcur = phead->next;
	while (pcur != NULL)
	{
		printf("%d ", pcur->data);
		pcur = pcur->next;
	}
	return 0;
}

int SList_NodeInsert(node* phead, int x, int y) //三个辅助指针 pnew, ppre, pcur
{
	if (phead == NULL)
		return -1;
	node* pcur = NULL, * ppre = NULL, * pnew = NULL;
	int exist = 0;

	pnew = (node*)malloc(sizeof(node));//创建新结点
	pnew->data = y;
	pnew->next = NULL;

	ppre = phead;
	pcur = phead->next;
	while (pcur != NULL)
	{
		if (pcur->data == x)
			break;

		ppre = pcur;
		pcur = pcur->next;
	}

	pnew->next = pcur;
	ppre->next = pnew;
	return 0;
}

int SList_NodeDel(node* phead, int x) //需要两个辅助指针 pre cur
{
	if (phead == NULL)
		return -1;
	node* ppre = NULL, * pcur = NULL;
	ppre = phead;
	pcur = phead->next;
	while (pcur != NULL)
	{
		if (pcur->data == x)
		{
			break;
		}
		ppre = pcur;
		pcur = pcur->next;
	}
	if (pcur == NULL)
	{
		printf("没有结点data为%d的结点", x);
		return 0;
	}
	ppre->next = pcur->next;
	if (pcur != NULL)
		free(pcur);
	return 0;
}

int SList_Destory(node* phead)
{
	if (phead == NULL)
		return -1;
	node* pcur = NULL;
	while (phead != NULL)
	{
		pcur = phead->next;
		free(phead);
		phead = pcur;
	}
	return 0;
}

int SList_Reverse(node* phead)
{
	if(phead == NULL || phead->next == NULL || phead->next->next == NULL)
		return 0;
	node* ppre = NULL, * pcur = NULL, * pnxt = NULL;
	pcur = phead->next;
	while (pcur != NULL)
	{
		pnxt = pcur->next;
		pcur->next = ppre;
		ppre = pcur;
		pcur = pnxt;
	}
	phead->next = ppre;
	return 0;
}
int create_list(node** head, int num) //带头结点
{
	if (head == NULL)
		return -1;
	int i = 0, ret = 0;
	//node* phead = (node*)malloc(sizeof(node));
	//node* pcur = phead;
	node* phead = NULL;
	node* pcur = NULL;
	for (i = 0; i < num; i++)
	{
		node* pnew = (node*)malloc(sizeof(node));
		pnew->next = NULL;
		pnew->data = i + 1;
		//pcur->next = pnew;
		//pcur = pnew;
		if (i == 0)
		{
			phead = pnew;
			pcur = phead;
		}
		else
		{
			pcur->next = pnew;
			pcur = pnew;
		}
	}
	//*head = phead;
	*head = phead;
	return ret;
}

void print_list(node* head)
{
	/*if (head->next == NULL)
		return;
	node* cur = head->next;
	while (cur != NULL)
	{
		printf("%d ", cur->data);
		cur = cur->next;
	}*/
	if (head == NULL)
	{
		return;
	}
	node* pcur = head;
	while (pcur != NULL)
	{
		printf("%d ", pcur->data);
		pcur = pcur->next;
	}
	return;
}
int main()
{
	node* head = NULL;
	int ret = 0;
	SList_Create01(&head);
	ret = SList_Print(head);
	printf("\n");
	ret = SList_NodeInsert(head, 15, 19);
	ret = SList_NodeDel(head, 15);
	ret = SList_Print(head);

	SList_Reverse(head);
	printf("\n");
	SList_Print(head);

	SList_Destory(head);
	return 0;
}
