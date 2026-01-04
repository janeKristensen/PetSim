#pragma once


class Command{
public:
	virtual ~Command() {};
	virtual void execute() const = 0;
};


class SaveCommand : public Command{
public:
	SaveCommand(){}
	void execute() const override 
	{
		
	}

private:
	
};