#include"ComM.h"

template <typename states_T>
size_t Automata::State_state_pair<states_T>::hash() const
{
	if (static_cast<size_t>(state1) > static_cast<size_t>(state2))
	{
		return static_cast<size_t>(state1) * static_cast<size_t>(state1) + static_cast<size_t>(state1) + static_cast<size_t>(state2);
	}
	else
	{
		return static_cast<size_t>(state2) * static_cast<size_t>(state2) + static_cast<size_t>(state1);
	}

}