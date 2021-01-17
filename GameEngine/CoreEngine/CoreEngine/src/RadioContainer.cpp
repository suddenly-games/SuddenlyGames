#include "RadioContainer.h"

void RadioContainer::SetValue(int newValue)
{
	Value = newValue;

	ValueChanged.Fire(newValue + 0);
}

int RadioContainer::GetValue() const
{
	return Value;
}
