#include "RadioOption.h"

RadioOption::RadioOption(int value) : Value(value)
{
	ValueChanged.Connect([this](bool) -> bool
	{
		if (Container != nullptr && (Container->GetValue() == this->Value) != IsChecked())
			Container->SetValue(IsChecked() ? this->Value : -1);

		return true;
	});
}

void RadioOption::SetValue(int newValue)
{
	Value = newValue;

	if (Container != nullptr)
		SetChecked(Container->GetValue() == Value);
}

void RadioOption::SetContainer(RadioContainer* newContainer)
{
	//if (Container != nullptr)
	//	Container->ValueChanged.Disconnect(&Listener);

	Container = newContainer;

	if (Container != nullptr)
	{
		Container->ValueChanged.Connect([this](int) -> bool
		{
			SetChecked(Container->GetValue() == Value);

			return true;
		});

		SetChecked(Container->GetValue() == Value);
	}
}

int RadioOption::GetValue() const
{
	return Value;
}

RadioContainer* RadioOption::GetContainer()
{
	return Container;
}
