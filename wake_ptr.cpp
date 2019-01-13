#include <iostream>
#include <memory>
using namespace std;
struct ListNode
{
	int _data;
	weak_ptr<ListNode> _prev;       //解决智能指针的循环引用问题
	weak_ptr<ListNode> _next;       //weak_ptr不会对引用计数++
	~ListNode(){ cout << "~ListNode()" << endl; }
};
int main()
{
	shared_ptr<ListNode> node1(new ListNode);
	shared_ptr<ListNode> node2(new ListNode);
	shared_ptr<ListNode> node3(node1);
	cout << node1.use_count() << endl;
	cout << node2.use_count() << endl;
	node1->_next = node2;
	node2->_prev = node1;
	cout << node1.use_count() << endl;
	cout << node2.use_count() << endl;
	return 0;
}