#pragma once

#include "Point.h"

class Command
{
public:
	Command();
	virtual ~Command();
	virtual void execute() = 0;
//	virtual void undo() = 0;
};



class JumpCommand : public Command
{
public:
	JumpCommand(Point* point, int x, int y);
	virtual void execute();	
	//virtual void undo();
private:

	Point * point_;
	int x_;
	int y_;
};

class DownCommand : public Command
{
public:
	DownCommand(Point* point, int x, int y);
	virtual void execute();
	//virtual void undo();

private:
	Point * point_;
	int x_;
	int y_;
};
class GoRightCommand : public Command
{
public:
	GoRightCommand(Point* point, int x, int y);
	virtual void execute();
	//virtual void undo();

private:
	Point * point_;
	int x_;
	int y_;
};

class GoLeftCommand : public Command
{
public:
	GoLeftCommand(Point* point, int x, int y);
	virtual void execute();
//	virtual void undo();

private:
	Point * point_;
	int x_;
	int y_;
};


class EnterCommand : public Command
{
public:
	EnterCommand(Point* point, int x, int y);
	virtual void execute();
	//	virtual void undo();

private:
	Point * point_;
	int x_;
	int y_;
};
