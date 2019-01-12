#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
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
	shared_ptr<Test> sp(new Test);
	shared_ptr<Test> sp1(sp);         //通过引用计数的方式来实现多个对象之间的资源共享
	cout << sp.use_count() << endl;
	cout << sp1.use_count() << endl;
}
template<class T>
class My_shared_ptr
{
public:
	My_shared_ptr(T* ptr = nullptr)
		:_ptr(ptr), _pcount(new int(1)), _pmutex(new mutex)
	{
		if (_ptr == nullptr)
			*_pcount = 0;
	}
	My_shared_ptr(const My_shared_ptr<T>& sp)
		:_ptr(sp._ptr), _pcount(sp._pcount), _pmutex(sp._pmutex)
	{
		if (_ptr)
			AddCount();
	}
	My_shared_ptr& operator=(const My_shared_ptr<T>& sp)
	{
		if (this != &sp)
		{
			//释放旧资源
			Release();  
			//共享新资源
			_ptr = sp._ptr;
			_pcount = sp._pcount;
			_pmutex = sp._pmutex;
			if (_ptr)
				AddCount();
		}
		return *this;
	}
	int UseCount()
	{
		return *_pcount;
	}
	T* Get()
	{
		return _ptr;
	}
	T& operator*(){ return *_ptr; }
	T* operator->(){ return _ptr; }
	~My_shared_ptr()
	{
		Release();
	}
private:
	void AddCount()
	{
		_pmutex->lock();
		(*_pcount)++;
		_pmutex->unlock();
	}
	int SubCount()
	{
		_pmutex->lock();
		(*_pcount)--;
		_pmutex->unlock();
		return *_pcount;
	}
	void Release()
	{
		if (_ptr && SubCount() == 0){  //若--为0,则释放; 若--不为0，则只减引用计数
			delete _ptr;
			delete _pcount;
			delete _pmutex;
		}
		if (_ptr == nullptr)
		{
			delete _pcount;
			delete _pmutex;
		}
	}
private:
	T* _ptr;
	int* _pcount;
	mutex* _pmutex;
};
void Func(My_shared_ptr<Test>& sp,const size_t n,mutex* mutex_)
{
	for (size_t i = 0; i < n; i++){
		//智能指针构造和析构是线程安全的
		My_shared_ptr<Test> copy(sp);
		//但智能指针管理的资源不是线程安全的
		mutex_->lock();
		copy->num++;
		mutex_->unlock();
	}
}
void Test2()
{
	My_shared_ptr<Test> sp1(new Test);
	//My_shared_ptr<Test> sp2(sp1);
	//My_shared_ptr<Test> sp3;
	//sp3 = sp1;
	//cout << sp2.UseCount() << endl;
	mutex mutex_;
	const size_t n = 10000;
	thread t1(Func, sp1, n, &mutex_);   //可变参数列表
	thread t2(Func, sp1, n, &mutex_);
	t1.join();   //必须等待,
	t2.join();
	cout << sp1->num << endl;
}
int main()
{
	//Test1();
	Test2();
	return 0;
}