#include "Command.h"


Command::Command()
{
}
Command::~Command()
{
}
JumpCommand::JumpCommand(Point * point, int x, int y)
	: point_(point), x_(x), y_(y)
{
}
void JumpCommand::execute()
{
	point_->jump();
}
GoRightCommand::GoRightCommand(Point * point, int x, int y)
	:point_(point), x_(x), y_(y)
{
}
void GoRightCommand::execute()
{
	point_->GoRight();
}
GoLeftCommand::GoLeftCommand(Point * point, int x, int y)
	: point_(point), x_(x), y_(y)
{
}
void GoLeftCommand::execute()
{
	point_->GoLeft();
}
DownCommand::DownCommand(Point * point, int x, int y)
	: point_(point), x_(x), y_(y)
{
}
void DownCommand::execute()
{
	point_->GoDown();
}

EnterCommand::EnterCommand(Point * point, int x, int y)
	: point_(point), x_(x), y_(y)
{
}
void EnterCommand::execute()
{
	point_->champ_->champLocationSettingX(x_);
	point_->champ_->champLocationSettingY(y_);
}
