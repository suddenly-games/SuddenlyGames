#pragma once

#include "TaskScheduler.h"
#include "IdentifierHeap.h"

namespace Engine
{
	class EventQueueBlock
	{
	public:
		int BlockSize = -1;
	};

	class EventQueue
	{
	public:

	private:
		typedef std::vector<char> DataStream;

		DataStream Queue;
	};

	class LuaConnection : public Connection
	{
	public:
		void Died();

	private:

		LuaConnection();
	};
}