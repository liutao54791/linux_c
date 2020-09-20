
//Node* delsame(Node* head)
//{
//	Node* p,q,r;
//	while((p = head->next) != NULL)
//	{
//		q = p;
//		while(q->next != NULL)
//		{
//			if (q->next->data == p->data)
//			{
//				r = q->next;
//				q->next = q->next->next;
//				free(r);
//			}
//			else
//			{
//				q = q->next;
//			}
//		}
//		p = p->next;
//	}
//}
//
//Mode* insert(Node* list, Node* p, int pos)
//{
//	for (int i = 0; (i < pos) && (list->next != NULL); ++i)
//	{
//		list = list->next;
//	}
//	p->next = list->next;
//	list->next = p;
//
//	return list;
//}
//
//#include<iostream>
//#include<string>
//#include<cstdio>
//
//
//using namespace std;
//#define MAX 1010  
//
//
//int main()
//{
//	string std1, std2;
//	cin>>std1>>std2;
//	int length1 = std1.length();
//	int length2 = std2.length();
//	int a[MAX] = {0};
//	int b[MAX] = {0};
//	int result[2 * MAX] = {0};
//	int i = 0, j = 0;
//	for(i = length1 - 1, j = 0; i >= 0; i--, j++)
//	{
//		a[j] = std1[i] - '0';
//	}
//	for(i = length2 - 1, j = 0; i >= 0; i--, j++)
//	{
//		b[j] = std2[i] - '0';
//	}
//	//将结果储存在 resullt中，result[i + j] = a[i] * b[j]是关键算法 
//	for(i = 0; i < length1; i++)
//	{
//		for(j = 0; j < length2; j++)
//		{
//			result[i + j] += a[i] * b[j];
//		}
//	}
//	
//	//从低位到高位进行进位
//	
//	for(i = 0; i < (length1+length2); i++)
//	{
//		if(result[i] > 9)
//		{
//			result[i+1] += result[i]/10;
//			result[i] %= 10;
//		}
//	}
//	//将前导0全部剔掉，比如我们结果是236，在result中
//	//是这样存储的63200……我们需要定位到第一个不为零的数，它的位置也就是i ，两数相乘，位数最多是两数位数之和
//	
//	for(i = length1 + lengrh2; i >= 0 ; i--)
//	{
//		if(result[i] == 0) continue;
//		else break;
//	}
//	//接着i继续输出，就是我们的结果 
//	for(; i >=0; i--)
//		cout<<result[i];
//	return 0;
//}
//
//
//using namespace std;
//int main()
//{
//    //定义变量	
//    char x1[200]={'\0'};//两个不超过200位的加数
//    char x2[200]={'\0'};
//    int carry[201]={0};//进位
//    int result[201]={0};//和
//    int i;//循环变量
//    //输入
//    cin>>x1>>x2;
//    //求两char数组长度
//    int len1=strlen(x1);
//    int len2=strlen(x2);
//    //反序
//    strrev(x1);
//    strrev(x2);
//    //确定两序列长度的最大值
//    int lenmax=len1*(len1>=len2)+len2*(len1<len2);
//    //把char类型的加数数组转化为int
//    int x11[200]={0};
//    for(i=0;i<=len1-1;i++)
//    {
//        x11[i]=x1[i]-'0';
//    }
//    int x22[200]={0};
//    for(i=0;i<=len2-1;i++)
//    {
//        x22[i]=x2[i]-'0';
//    }
//    //不考虑进位对应位相加
//    for(i=0;i<=lenmax-1;i++)
//    {
//        result[i]=x11[i]+x22[i];
//        result[i+1]=0;//用于防止超过lenmax后的一位
//    }
//    //考虑进位 完成最终求和
//    for(i=0;i<=lenmax+1;i++)
//    {
//        carry[i+1]=result[i]/10;
//        result[i]=result[i]%10+carry[i];
//    }
//    //若最高位为0,则不输出最高位
//    if(result[lenmax]==0)
//        i=lenmax-1;
//    else
//        i=lenmax;
//    for(;i>=0;i--)
//        cout<<result[i];
//    cout<<endl;
//return 0;
//}
//
///*
// * 暴力法：用于字符串匹配
// * string t：文本串
// * string p：模式串
// * 返回值：返回首次匹配（完全匹配）位置（失败返回-1）
//*/
//int stringMatch(string t, string p)
//{
//	int lenT = t.size();
//	int lenP = p.size();
//	int i, j;
//	for (i = 0; i <= lenT - lenP; ++i)
//	{
//		for (j = 0; j < lenP; ++j)
//		{
//			if (t[i + j] != p[j])
//			{
//				break;
//			}
//		}
//		if (j == lenP)
//		{
//			return i;
//		}
//	}
//	return -1;
//}
//
//
//#include <iostream>
//using namespace std;
//
//int solve(int * a, int N)
//{
//    int min=100;
//    int i,j;
//    for(i=1;i<N/2;i++)
//    {
//        int path=0;
//        for(j=i;j<N;j+=a[j])
//        {
//            path++;
//            if(j==N-1)
//            {
//                min=min<path?min:path;
//                break;
//            }
//        }
//    }
//    if(min==100) return -1;
//        return min;
//}
//int main()
//{
//    int a[100];
//    int tmp;
//    int i=0;
//    while(cin.peek()!='\n')
//    {
//        cin>>a[i++];
//    }
//    cout<<solve(a,i);
//
//    return 0;
//}
//
//
//int jump(std::vector<int>& nums)
//{
// 	if (nums.size() < 2) 
// 	{
// 	 	return 0;
// 	}
//
// 	unsigned int current_max_index = nums[0];
// 	unsigned int pre_max_index = nums[0];
// 	int jump_min = 1;
//
// 	for (unsigned int i = 0; i < nums.size(); i++)
// 	{
// 	 	if (i>current_max_index)
// 	 	{
// 	 	 	jump_min++;
// 	 	 	current_max_index = pre_max_index;
// 	 	}
// 	 	if (pre_max_index<nums[i]+i)
// 	 	{
// 	 	 	pre_max_index = nums[i] + i;
// 	 	}
// 	}
// 	return jump_min;
//}
//