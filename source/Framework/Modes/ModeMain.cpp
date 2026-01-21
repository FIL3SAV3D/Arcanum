#include "ModeMain.h"

ModeMain::ModeMain(const char* _name, std::shared_ptr<Window> _Window):
	window{_Window},
	IMode{ _name }
{
	OnCreate();
}

ModeMain::~ModeMain()
{
}

void ModeMain::OnResize(const uint32_t _width, const uint32_t _height)
{
}

void ModeMain::OnCreate()
{
	//Pushlayer(std::static_pointer_cast<ILayer>(std::make_shared<GameLayer>(window)));
}

void ModeMain::OnDestroy()
{
}

void ModeMain::OnEnter()
{
}

void ModeMain::OnExit()
{
}

void ModeMain::Update(const float& _delta_time)
{
	//UpdateLayer(_delta_time);
}
