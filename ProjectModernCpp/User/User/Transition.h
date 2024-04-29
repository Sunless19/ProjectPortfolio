#pragma once

#include <QMainWindow>
#include "ui_Transition.h"
#include "Guess_Game_Interface.h"
#include "Drawer_Game_Interface.h"

#include <cpr/cpr.h>
#include <crow.h>
#include <fstream>


class Transition : public QMainWindow
{
	Q_OBJECT

public:
	Transition(QWidget *parent = nullptr);
	~Transition();

	std::string getName();
	void setName(std::string);

public slots:
	void UserType();

private:
	Ui::TransitionClass ui;
	std::string name;
	int noTransitions=0;
};
