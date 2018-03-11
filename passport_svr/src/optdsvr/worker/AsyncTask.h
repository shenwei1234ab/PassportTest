//
//  BSForceGuide.h
//  optd_srv
//
//  Created by Cloverz on 10/23/14.
//  Copyright (c) 2014 longsgoo.com. All rights reserved.
//

#ifndef __optd_srv__AsyncTask
#define __optd_srv__AsyncTask
#include "vnRefCounted.h"
using namespace vn;
class AsyncTask:public RefCounted
{
public:

	void Init(int taskSize)
	{
		m_Task.reserve(taskSize);
	}

	void FinishTask(int taskValue)
	{
		m_Task.push_back(taskValue);
	}

	bool IsFinished()const
	{
		return m_Task.size()==m_Task.capacity();
	}

	/*int operator[](int index)
	{
		return myTask[index];
	}*/
	int at(int index)
	{
		return m_Task[index];
	}

	int TaskSize()
	{
		return m_Task.size();
	}
	//要变化的钻石数量
		s32 m_diamondCount=0;
/*	struct Parameter
	{
		vn::u64 timeInterval;
	}m_parameters;*/

private:

	std::vector<int> m_Task;
};
typedef vn::RefCountedPtr<AsyncTask> AsyncTaskPtr;


#endif /* defined(__optd_srv__BSForceGuide__) */
