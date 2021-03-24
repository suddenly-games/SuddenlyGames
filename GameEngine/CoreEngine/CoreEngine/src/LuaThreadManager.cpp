#include "LuaThreadManager.h"

#include <iostream>
#include <chrono>

#include "IdentifierHeap.h"

namespace Engine
{
	namespace LuaThread
	{
		typedef std::vector<int> IntVector;
		typedef std::map<lua_State*, int> IDMap;

		struct Thread
		{
			std::string Name;
			std::string Source;
			lua_State* State = nullptr;
			LuaCallback InitializeCallback;
			int ID = -1;
			bool Alive = true;
			bool Running = false;
			std::chrono::steady_clock::time_point ThreadStarted;

			Thread()
			{
				ThreadStarted = std::chrono::high_resolution_clock::now();
			}
		};

		IDHeap<Thread> Threads;
		IDMap ThreadIDs;
		IntVector ThreadQueue;
		IntVector ThreadStack;

		bool CheckError(lua_State* lua, Thread& thread, int status)
		{
			if (!status)
			{
				if (lua_type(lua, -1) == LUA_TSTRING)
					std::cout << lua_tostring(lua, -1) << std::endl;
				else if (status == LUA_ERRSYNTAX)
					std::cout << "unknown syntax error loading script '" << thread.Name << "'" << std::endl;
				else if (status == LUA_ERRMEM)
					std::cout << "memory error loading script '" << thread.Name << "'" << std::endl;
				else if (status == LUA_ERRRUN)
					std::cout << "unknown runtime error loading script '" << thread.Name << "'" << std::endl;
				else if (status == LUA_ERRERR)
					std::cout << "unknown error handler error loading script '" << thread.Name << "'" << std::endl;
			}

			return !status;
		}

		void Initialize(lua_State* lua)
		{
			lua_pushstring(lua, "ThreadData");
			lua_createtable(lua, 0, 0);

			//lua_createtable(lua, 0, 1);
			//lua_pushstring(lua, "__mode");
			//lua_pushstring(lua, "v");
			//lua_settable(lua, -3);
			//
			//lua_setmetatable(lua, -2);

			lua_settable(lua, LUA_REGISTRYINDEX);

			const char coroutineWrapper[] =
				"	return function(coroutine, created, resumed, yielded, ended, traceback, pcallwrap)"
				"\n		local create = coroutine.create"
				"\n		local resume = coroutine.resume"
				"\n		local yield = coroutine.yield"
				"\n		local wrap = coroutine.wrap"
				"\n		"
				"\n		function coroutine.create(func)"
				"\n			local thread = create(function(...) return pcallwrap(xpcall(func, traceback, ...)) end)"
				"\n			"
				"\n			created(thread)"
				"\n			"
				"\n			return thread"
				"\n		end"
				"\n		"
				"\n		function coroutine.resume(thread, var1, var2, ...)"
				"\n			--resumed(thread)"
				"\n			"
				"\n			return resumed(resume, thread, var1, var2, ...)"
				"\n		end"
				"\n		"
				"\n		function coroutine.yield(var1, var2, ...)"
				"\n			yielded()"
				"\n			"
				"\n			return yield(var1, var2, ...)"
				"\n		end"
				"\n		"
				"\n		function coroutine.wrap(func)"
				"\n			local thread = coroutine.create(func)"
				"\n			"
				"\n			return function(...)"
				"\n				return coroutine.resume(thread, ...)"
				"\n			end"
				"\n		end"
				"\n		"
				"\n		local function wrapper(func)"
				"\n			local thread = create(func)"
				"\n			"
				"\n			created(thread)"
				"\n			"
				"\n			return function(...)"
				"\n				return coroutine.resume(thread, ...)"
				"\n			end"
				"\n		end"
				"\n		"
				"\n		return wrapper"
				"\n end";

			Lua::RunChunk(lua, coroutineWrapper, __FILE__, __LINE__, sizeof(coroutineWrapper) - 1);

			lua_call(lua, 0, 1);
			lua_getglobal(lua, "coroutine");
			lua_pushcfunction(lua, CoroutineCreate);
			lua_pushcfunction(lua, [] (lua_State* lua) -> int
			{
				lua_State* thread = lua_tothread(lua, 2);

				CoroutineResume(thread);

				lua_call(lua, lua_gettop(lua) - 1, LUA_MULTRET);

				ThreadEnded(GetThreadID(thread));

				return lua_gettop(lua);
			});
			lua_pushcfunction(lua, CoroutineYield);
			lua_pushcfunction(lua, [](lua_State* lua) -> int
			{
				lua_State* thread = lua_tothread(lua, 1);

				ThreadEnded(GetThreadID(thread));

				return 0;
			});
			lua_pushcfunction(lua, Lua::Traceback);
			lua_pushcfunction(lua, &PCall);
			lua_call(lua, 7, 1);

			lua_setglobal(lua, "coroutine.wrap");
		}

		void Update(lua_State* lua, float delta)
		{
			lua_pushstring(lua, "ThreadData");
			lua_gettable(lua, LUA_REGISTRYINDEX);

			for (int i = 0; i < int(ThreadQueue.size()); ++i)
			{
				if (!Threads.NodeAllocated(i))
					continue;

				{
					int top = lua_gettop(lua);
					top += 0;
				}

				Thread& thread = Threads.GetNode(ThreadQueue[i]).GetData();

				if (thread.Alive && thread.State == nullptr)
				{
					thread.State = lua_newthread(lua);
					{
						int top = lua_gettop(lua);
						top += 0;
					}

					lua_pushnumber(lua, lua_Number(thread.ID));
					lua_pushvalue(lua, -2);
					lua_settable(lua, -4);

					lua_getglobal(lua, "print");

					int print = lua_gettop(lua);

					{
						int top = lua_gettop(lua);
						top += 0;
					}

					lua_pushcfunction(lua, Lua::Traceback);

					int traceback = lua_gettop(lua);


					std::string message;
					int error = luaL_loadbuffer(lua, thread.Source.c_str(), thread.Source.size(), thread.Name.c_str());
					{
						int top = lua_gettop(lua);
						top += 0;
					}

					if (!error)
					{
						lua_createtable(lua, 0, 0);
						thread.InitializeCallback(lua);
						thread.Running = true;
						{
							int top = lua_gettop(lua);
							top += 0;
						}

						lua_createtable(lua, 0, 1);
						lua_pushstring(lua, "__index");
						lua_getglobal(lua, "_G");
						lua_settable(lua, -3);

						lua_setmetatable(lua, -2);

						lua_setupvalue(lua, -2, 1);
						{
							int top = lua_gettop(lua);
							top += 0;
						}

						lua_getglobal(lua, "coroutine.wrap");

						const char threadWrapper[] = "return function(chunk, traceback, pcallwrap) return pcallwrap(xpcall(chunk, traceback)) end";

						Lua::RunChunk(lua, threadWrapper, __FILE__, __LINE__, sizeof(threadWrapper) - 1);

						lua_call(lua, 0, 1);

						lua_call(lua, 1, 1);
						{
							int top = lua_gettop(lua);
							top += 0;
						}

						lua_pushvalue(lua, -2);
						lua_pushcfunction(lua, Lua::Traceback);
						lua_pushcfunction(lua, &PCall);

						error = lua_pcall(lua, 3, 1, traceback);

						if (error)
						{
							if (lua_isstring(lua, -1))
							std::cout << lua_tostring(lua, -1);
						}

						ThreadEnded(i);
						{
							int top = lua_gettop(lua);
							top += 0;
						}

						lua_pop(lua, 6);
					}
					else
					{
						CheckError(lua, thread, error);

						lua_pop(lua, 4);
					}

					//lua_pop(lua, 1);

					{
						int top = lua_gettop(lua);
						top += 0;
					}
				}
			}

			int top = lua_gettop(lua);

			lua_pop(lua, 1);
		}

		int PCall(lua_State* lua)
		{
			if (lua_toboolean(lua, 1))
				return lua_gettop(lua) - 1;
			else
			{
				std::cout << lua_tostring(lua, 2);

				return 0;
			}
		}

		int Spawn(const std::string& source, const std::string& name, LuaCallback initializeCallback)
		{
			int threadID = Threads.RequestID();

			Thread& thread = Threads.GetNode(threadID).GetData();

			ThreadIDs[thread.State] = threadID;

			thread.Name = name;
			thread.ID = threadID;
			thread.Source = source;
			thread.InitializeCallback = initializeCallback;

			ThreadQueue.push_back(threadID);

			return threadID;
		}

		void Kill(int threadID)
		{
			if (!Threads.NodeAllocated(threadID))
				return;

			Threads.GetNode(threadID).GetData().Alive = false;
		}

		bool Running(int threadID)
		{
			if (!Threads.NodeAllocated(threadID))
				return false;

			return Threads.GetNode(threadID).GetData().Running;
		}

		bool Yielded(int threadID)
		{
			if (!Threads.NodeAllocated(threadID))
				return false;

			Thread& thread = Threads.GetNode(threadID).GetData();

			return thread.Alive && !thread.Running;
		}

		bool Dead(int threadID)
		{
			if (!Threads.NodeAllocated(threadID))
				return false;

			return !Threads.GetNode(threadID).GetData().Alive;
		}

		int GetData(lua_State* lua, int threadID)
		{
			if (!Threads.NodeAllocated(threadID))
				return 0;

			lua_pushstring(lua, "ThreadData");
			lua_gettable(lua, LUA_REGISTRYINDEX);

			lua_pushnumber(lua, lua_Number(threadID));
			lua_gettable(lua, -2);

			lua_replace(lua, -2);
			lua_pop(lua, 1);

			return 1;
		}

		int GetThreadID(lua_State* lua)
		{
			IDMap::iterator i = ThreadIDs.find(lua);

			if (i != ThreadIDs.end())
				return i->second;

			return -1;
		}

		int CoroutineCreate(lua_State* lua)
		{
			int threadID = Threads.RequestID();

			Thread& thread = Threads.GetNode(threadID).GetData();

			thread.ID = threadID;
			thread.State = lua_tothread(lua, 1);

			ThreadIDs[thread.State] = threadID;

			lua_pushstring(lua, "ThreadData");
			lua_gettable(lua, LUA_REGISTRYINDEX);

			lua_pushnumber(lua, lua_Number(threadID));
			lua_pushvalue(lua, 1);
			lua_settable(lua, -3);

			return 0;
		}

		bool CompareThreads(lua_State* thread1, lua_State* thread2)
		{
			const auto id1 = ThreadIDs.find(thread1);
			const auto id2 = ThreadIDs.find(thread2);

			if (id1 == ThreadIDs.end() || id2 == ThreadIDs.end())
			{
				std::cout << "warning: untracked thread registered" << std::endl;
				
				return false;
			}

			Thread& threadData1 = Threads.GetNode(id1->second).GetData();
			Thread& threadData2 = Threads.GetNode(id2->second).GetData();

			return threadData1.ThreadStarted > threadData2.ThreadStarted;
		}

		int CoroutineResume(lua_State* lua)
		{
			int threadID = GetThreadID(lua);

			if (threadID >= 0 && threadID <= Threads.Size() && Threads.NodeAllocated(threadID))
			{
				Thread& thread = Threads.GetNode(threadID).GetData();

				thread.Running = true;

				ThreadStack.push_back(threadID);

				//std::cout << "Entering thread [" << ThreadStack.size() << "]" << std::endl;
			}

			return 0;
		}

		int CoroutineYield(lua_State* lua)
		{
			int threadID = GetThreadID(lua);

			if (threadID >= 0 && threadID <= Threads.Size() && Threads.NodeAllocated(threadID))
			{
				Thread& thread = Threads.GetNode(threadID).GetData();

				thread.Running = false;

				//std::cout << "Exiting thread [" << ThreadStack.size() << "]" << std::endl;

				if (ThreadStack.size() > 0 && ThreadStack[ThreadStack.size() - 1] == threadID)
					ThreadStack.pop_back();
			}

			return 0;
		}

		void ThreadEnded(int id)
		{
			if (ThreadStack.size() > 0 && ThreadStack[ThreadStack.size() - 1] == id)
			{
				Thread& thread = Threads.GetNode(id).GetData();

				thread.Running = false;
				thread.Alive = false;

				//std::cout << "Exiting thread [" << ThreadStack.size() << "]" << std::endl;

				ThreadStack.pop_back();
			}
		}
	}
}
