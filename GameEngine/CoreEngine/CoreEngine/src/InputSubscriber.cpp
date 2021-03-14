#include "InputSubscriber.h"

namespace GraphicsEngine
{
	InputSubscription::~InputSubscription()
	{
		if (Input != nullptr)
		{
			if (BeganConnection != -1)
				Connection().Disconnect(Input->Began, BeganConnection);

			if (BeganConnection != -1)
				Connection().Disconnect(Input->Began, BeganConnection);

			if (BeganConnection != -1)
				Connection().Disconnect(Input->Began, BeganConnection);
		}
	}

	bool InputSubscription::HasFocus(Enum::BoundDevice device) const
	{
		std::shared_ptr<InputSubscriber> subscriber = GetComponent<InputSubscriber>();

		if (subscriber != nullptr)
			return subscriber->Focused[device];

		return false;
	}

	void InputSubscription::Update(float)
	{
		UpdateState();
		std::cout << GetStateEnum(Enum::BoundDevice::Mouse1) << std::endl;
	}

	void InputSubscription::Subscribe(const std::shared_ptr<Engine::InputObject>& input)
	{
		if (Input != nullptr)
			throw std::string("Attempt to subscribe with InputSubscription that is already bound");

		Input = input;

		if (input->GetType() != Enum::InputType::Point)
		{
			BeganConnection = input->Began.Connect([this](const std::shared_ptr<InputObject>& object)
			{
				UpdateState();

				return true;
			});

			EndedConnection = input->Ended.Connect([this](const std::shared_ptr<InputObject>& object)
			{
				UpdateState();
				
				return true;
			});
		}
		else if (input->GetType() != Enum::InputType::Button)
			ChangedConnection = input->Ended.Connect([this](const std::shared_ptr<InputObject>& object)
			{
				UpdateState();

				return true;
			});
	}

	void InputSubscription::UpdateState()
	{
		std::shared_ptr<InputSubscriber> subscriber = GetComponent<InputSubscriber>();

		for (int i = 0; i < Enum::BoundDevice::Count; ++i)
		{
			Enum::BoundDevice device = Enum::BoundDevice(i);
			bool hasFocus = false;
			bool focusedLast = false;

			if (subscriber != nullptr)
			{
				hasFocus = subscriber->Focused[device];
				focusedLast = subscriber->FocusedLast[device];
			}

			if (!hasFocus)
			{
				SetState(false, device, true);
				SetPosition(Vector3(), device, true);
			}
			else
			{
				SetState(Input->GetState(device), device, false);
				SetPosition(Input->GetPosition(device), device, false);
			}
		}
	}

	std::shared_ptr<InputSubscription> InputSubscriber::Subscribe(const std::shared_ptr<Engine::InputObject>& input)
	{
		std::shared_ptr<InputSubscriber> subscriber = This.lock()->Cast<InputSubscriber>();

		std::shared_ptr<InputSubscription> subscription = Engine::Create<InputSubscription>();

		subscription->Name = input->Name + "Subscription";
		subscription->Subscribe(input);
		subscription->SetParent(subscriber);

		return subscription;
	}

	void InputSubscriber::Update(float)
	{
		for (int i = 0; i < Enum::BoundDevice::Count; ++i)
			FocusedLast[i] = Focused[i];
	}

	void InputSubscriber::SetFocus(Enum::BoundDevice device, bool focus)
	{
		Focused[device] = focus;
		
		for (int i = 1; focus && i < Enum::BoundDevice::Count; ++i)
			focus |= Focused[i];

		Focused[Enum::BoundDevice::Any] = focus;
	}
}