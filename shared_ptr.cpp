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
	shared_ptr<Test> sp1(sp);         //ͨ�����ü����ķ�ʽ��ʵ�ֶ������֮�����Դ����
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
			//�ͷž���Դ
			Release();  
			//��������Դ
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
		if (_ptr && SubCount() == 0){  //��--Ϊ0,���ͷ�; ��--��Ϊ0����ֻ�����ü���
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
		//����ָ�빹����������̰߳�ȫ��
		My_shared_ptr<Test> copy(sp);
		//������ָ��������Դ�����̰߳�ȫ��
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
	thread t1(Func, sp1, n, &mutex_);   //�ɱ�����б�
	thread t2(Func, sp1, n, &mutex_);
	t1.join();   //����ȴ�,
	t2.join();
	cout << sp1->num << endl;
}
int main()
{
	//Test1();
	Test2();
	return 0;
}