#pragma once

#include "BCBase.h"
#include<functional>
#include<chrono>
#include<thread>
#include<atomic>
#include<memory>
#include<mutex>
#include<condition_variable>

using namespace std;



//start ���warning C4251
//���ڵ�����Timer���õ���������ģ��������࣬Ҳ��Ҫ��������

/*
#pragma warning(push)
#pragma warning(disable:4251)
//your declarations that cause 4251
#pragma warning(pop)
*/

//template class DLLImportExportMacro std::vector

template struct BCSAMPLE_EXPORT atomic<bool>;
class BCSAMPLE_EXPORT std::mutex;
class BCSAMPLE_EXPORT std::condition_variable;
//end  ���warning C4251



class BCSAMPLE_EXPORT Timer 
{
public:
	Timer() :expired_(true), try_to_expire_(false) 	{	}

	Timer(const Timer& t)
	{
		expired_ = t.expired_.load();
		try_to_expire_ = t.try_to_expire_.load();
	}
	~Timer() 
	{
		Expire();
		//cout << "timer destructed!" << endl;
	}

	void StartTimer(int interval, function<void()> task) {
		if (expired_ == false) {
			//cout << "timer is currently running, please expire it first..." << endl;
			return;
		}
		expired_ = false;
		thread([this, interval, task]() {
			while (!try_to_expire_) {
				this_thread::sleep_for(chrono::milliseconds(interval));
				task();
			}
			//          cout << "stop task..." << endl;
			{
				lock_guard<mutex> locker(mutex_);
				expired_ = true;
				expired_cond_.notify_one();
			}
		}).detach();
	}

	void Expire() {
		if (expired_) {
			return;
		}

		if (try_to_expire_) {
			// cout << "timer is trying to expire, please wait..." << endl;
			return;
		}
		try_to_expire_ = true;
		{
			unique_lock<mutex> locker(mutex_);
			expired_cond_.wait(locker, [this] {return expired_ == true; });
			if (expired_ == true) {
				//              cout << "timer expired!" << endl;
				try_to_expire_ = false;
			}
		}
	}

	template<typename callable, class... arguments>
	void SyncWait(int after, callable&& f, arguments&&... args) {

		function<typename result_of<callable(arguments...)>::type()> task
		(bind(forward<callable>(f), forward<arguments>(args)...));
		this_thread::sleep_for(chrono::milliseconds(after));
		task();
	}
	template<typename callable, class... arguments>
	void AsyncWait(int after, callable&& f, arguments&&... args) {
		function<typename result_of<callable(arguments...)>::type()> task
		(bind(forward<callable>(f), forward<arguments>(args)...));

		thread([after, task]() {
			this_thread::sleep_for(chrono::milliseconds(after));
			task();
		}).detach();
	}

private:
	atomic<bool> expired_;
	atomic<bool> try_to_expire_;
	mutex mutex_;
	condition_variable expired_cond_;
};


