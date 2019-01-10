//#include <iostream>
//#include <memory>
//using namespace std;
//
//class Test
//{
//public:
//	Test(){
//		cout << "Test()" << endl;
//	}
//	~Test(){
//		cout << "~Test()" << endl;
//	}
//public:
//	int num;
//};
//void Test1()
//{
//	auto_ptr<Test> ap(new Test);    //可以释放单个连续空间 
//	//auto_ptr<Test> ap1(new Test[10]);  //析构错误,其源码不能释放连续对象空间
//	auto_ptr<Test> ap2(ap);
//	auto_ptr<Test> ap3 = ap2;
//	//ap->num = 1;    //错误！管理权转移,之前的指针会悬空
//}
//template<class T>
//class My_auto_ptr
//{
//public:
//	My_auto_ptr(T* ptr = nullptr) :_ptr(ptr)
//	{}
//	My_auto_ptr(My_auto_ptr<T>& ap)
//	{
//		_ptr = ap._ptr;
//		ap._ptr = nullptr;
//	}
//	T& operator*()
//	{
//		return *_ptr;
//	}
//	T* operator->()
//	{
//		return _ptr;
//	}
//	My_auto_ptr<T>& operator=(My_auto_ptr<T>& ap)
//	{
//		if (this != &ap)
//		{
//			if (_ptr)
//				delete _ptr;
//			_ptr = ap._ptr;
//			ap._ptr = nullptr;
//		}
//		return *this;
//	}
//	~My_auto_ptr()
//	{
//		if (_ptr)
//			delete _ptr;
//	}
//private:
//	T* _ptr;
//};
//void Test2()
//{
//	My_auto_ptr<Test> ap(new Test);    //可以释放单个连续空间 
//	//My_auto_ptr<Test> ap1(new Test[10]);  //析构错误,其源码不能释放连续对象空间
//	//My_auto_ptr<Test> ap2(ap);
//	My_auto_ptr<Test> ap3 = ap;    //调的是拷贝构造函数
//	ap3 = ap;
//	//ap->num = 1;    //错误！管理权转移,之前的指针会悬空
//}
//int main()
//{
//	//Test1();
//	Test2();
//	return 0;
//}