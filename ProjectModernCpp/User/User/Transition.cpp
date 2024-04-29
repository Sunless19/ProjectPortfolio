#include "Transition.h"
#include <QDebug>

Transition::Transition(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QTimer* runTimer = new QTimer(this);
	connect(runTimer, SIGNAL(timeout()), this, SLOT(UserType()));
	runTimer->start(5000);
}

Transition::~Transition()
{}

std::string Transition::getName()
{
	return name;
}

void Transition::setName(std::string name)
{
	this->name=name;
}

void Transition::UserType()
{
	std::string start1 = "true";
	auto response3 = cpr::Put(cpr::Url{ "http://localhost:18080/startGame" }, cpr::Parameters{ { "start", start1} });

	cpr::Response response7 = cpr::Get(cpr::Url{ "http://localhost:18080/getUserType" });
	auto interfaceTypes = crow::json::load(response7.text);

	if (interfaceTypes) {
		// Preia?i valoarea boolean? pentru utilizatorul curent
		std::string userIsDrawer = "false";  // Seteaz? la true dac? utilizatorul curent este desenatorul

		for (const auto& interfaceType : interfaceTypes) 
		{
			std::string playerName = interfaceType["name"].s();
			std::string boolValue = interfaceType["guesser"].s();
			qDebug() << playerName<<" "<<boolValue;

			if (playerName == this->getName()) {
				userIsDrawer = boolValue;
			}
		}

		if (userIsDrawer == "true") {
			Drawer_Game_Interface* draw = new Drawer_Game_Interface(this);
			draw->setName(this->getName());
			//this->close();
			draw->show();
		}
		else if (userIsDrawer == "false") {
			Guess_Game_Interface* guesser = new Guess_Game_Interface(this);
			guesser->setName(this->getName());
			//this->close();
			guesser->show();
		}
		else if (userIsDrawer == "end")
		{
			delete this;
		}
	}


}
