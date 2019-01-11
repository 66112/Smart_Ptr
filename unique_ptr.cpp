#include <iostream>
#include <memory>
using namespace std;
class Test
{
public:
	Test(){
		cout << "Test()" << endl;
	}
	~Test(){
		cout << "~Test()" << endl;
	}
public:
	int num = 0;
};
void Test1()
{
	unique_ptr<Test> up(new Test);
	//unique_ptr<Test> up1(up);   //û�п�������͸�ֵ����
	unique_ptr<Test> up2;
	up2 = move(up);     //���up�ͷ�
	//up->num = 1;
	//up2->num = 2;
}
template<class T>
class My_unique_ptr
{
public:
	My_unique_ptr(const T* p) :_ptr(p)
	{}
	T& operator*(){ return *_ptr; }
	T* operator->(){ return _ptr; }
	~My_unique_ptr()
	{
		if (_ptr){
			delete _ptr;
		}
	}
	//My_unique_ptr(My_unique_ptr<T>& p) = delete;           //C++11�ķ�������ʽ
	//My_unique_ptr& operator=(My_unique_ptr<T>& p) = delete;
private:
	My_unique_ptr(My_unique_ptr<T>& p);           //C++98�ķ�������ʽ
	My_unique_ptr& operator=(My_unique_ptr<T>& p);

private:
	T* _ptr;
};
void Test2()
{
	My_unique_ptr<Test> up(new Test);
}
int main()
{
	//Test1();
	Test2();
	return 0;
}