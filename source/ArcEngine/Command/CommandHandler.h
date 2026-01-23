#pragma once
#include <vector>
#include "ICommand.h"

class CommandHandler
{
public:
	void AddCommand(ICommand& command)
	{
		if (index < m_CommandList.size())
		{
			m_CommandList.erase(m_CommandList.begin() + index, m_CommandList.end() - index);
		}

		m_CommandList.push_back(command);
		command.Execute();
		index++;
	}

	void UndoCommand(ICommand& command)
	{
		if (m_CommandList.size() == 0)
		{
			return;
		}
		if (index > 0)
		{
			m_CommandList.at(index - 1).Undo();
			index--;
		}
	}

	void RedoCommand(ICommand& command)
	{
		if (m_CommandList.size() == 0)
		{
			return;
		}
		if (index < m_CommandList.size())
		{
			index++;
			m_CommandList.at(index - 1).Execute();
		}
	}

private:
	int index = 0;
	std::vector<ICommand> m_CommandList = {};
	
};