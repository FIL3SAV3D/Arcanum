#pragma once

namespace ArcEngine
{
	class ICommand
	{
	public:
		virtual ~ICommand() {};
		virtual void Execute() const = 0;
		virtual void Undo() const = 0;
	};
}