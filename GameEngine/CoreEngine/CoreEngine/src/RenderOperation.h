#pragma once

#include "Object.h"

#include "IdentifierHeap.h"

namespace GraphicsEngine
{
	class RenderOperation : public Engine::Object
	{
	public:
		virtual ~RenderOperation();
		
		void Initialize();
		void Update(float delta) {}

		bool RenderAutomatically = false;

		virtual void Render() = 0;

		static void UpdateOperations(float delta);
		static void RenderOperations();

	private:
		typedef IDHeap<std::weak_ptr<RenderOperation>> OperationHeap;

		static OperationHeap& GetOperations();

		int OperationID = -1;

		NonInstantiable;

		Inherits_Class(Object);

		Reflected(RenderOperation);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::RenderOperation, Object);
}