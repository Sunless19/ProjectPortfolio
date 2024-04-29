#include "Lobby_Interface.h"

Lobby_Interface::Lobby_Interface(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	getPLayers();

	runTime->start(1000);
	connect(runTime, SIGNAL(timeout()), this, SLOT(verifyStarted()));


	//connect(openTime, SIGNAL(timeout()), this, SLOT(Aux()));
	QPalette palette;
	palette.setColor(QPalette::Window, Qt::cyan);
	setPalette(palette);


}
void Lobby_Interface::Aux()
{

	cpr::Response response10 = cpr::Get(cpr::Url{ "http://localhost:18080/getUsers" });
	auto users = crow::json::load(response10.text);
	if (iterator != (users.size() * 4) - 1)
		openInterface(iterator);
	else
	{
		openTime->stop();
	}
}
Lobby_Interface::~Lobby_Interface()
{}

void Lobby_Interface::setName(std::string name)
{
	this->name = name;
}

std::string Lobby_Interface::getName()
{
	return this->name;
}

void Lobby_Interface::getPLayers()
{

	QStandardItemModel* model = new QStandardItemModel();
	QStandardItem* item1 = new QStandardItem("Player 1");

	std::ofstream f("Text.txt");
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/users" });
	std::cout << "Users:\n";
	auto users = crow::json::load(response.text);
	for (const auto& user : users)
	{
		QStandardItem* item1 = new QStandardItem(QString::fromStdString(user["name"].s()));

		model->appendRow(item1);

	}

	ui.playerList->setModel(model);


}

void Lobby_Interface::openInterface(int& it)
{
	std::string word;
	/*std::string start1 = "true";
	auto response3 = cpr::Put(cpr::Url{ "http://localhost:18080/startGame" }, cpr::Parameters{ { "start", start1} });*/

	/*cpr::Response response7 = cpr::Get(cpr::Url{ "http://localhost:18080/getUserType" });
	auto interfaceTypes = crow::json::load(response7.text);

	cpr::Response response8 = cpr::Get(cpr::Url{ "http://localhost:18080/getUsers" });
	auto users = crow::json::load(response8.text);*/


	if(interfaceTypes.size()==users.size()*4)
	if (it == (users.size() * 4) - 1)
	{
		delete this;
	}
	else
	{

		if (interfaceTypes[it + users.size() - 1]) {
			// Preia?i valoarea boolean? pentru utilizatorul curent
			std::string userIsDrawer = "false";  // Seteaz? la true dac? utilizatorul curent este desenatorul

			/*for (const auto& interfaceType : interfaceTypes)*/
			for (int i = it; i < it + users.size(); i++)
			{
				std::string playerName = interfaceTypes[i]["name"].s();
				std::string boolValue = interfaceTypes[i]["guesser"].s();
				word = interfaceTypes[i]["word"].s();
				qDebug() << playerName << " " << boolValue;

				if (playerName == this->getName()) {
					userIsDrawer = boolValue;
					break;
				}
			}
			if (userIsDrawer == "true") {
				it += users.size();
				Drawer_Game_Interface* draw = new Drawer_Game_Interface(this);
				draw->setName(this->getName());
				draw->setWord(word);
				//this->close();
				this->hide();
				draw->show();

			}
			else if (userIsDrawer == "false") {
				it += users.size();
				Guess_Game_Interface* guesser = new Guess_Game_Interface(this);
				guesser->setName(this->getName());
				guesser->setWord(word);

				//this->close();
				this->hide();
				guesser->show();
			}



			qDebug() << "open interface end\n";



		}

	}

}

void Lobby_Interface::verifyStarted()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/checkStarted" });
	auto check = crow::json::load(response.text);
	for (const auto& checked : check)
	{
		if (checked["start"].s() == "true" && ok == 0)
		{
			ok = 1;
			openInterface(iterator);
		}

	}
}

void Lobby_Interface::on_start_game_clicked()
{
	std::string start1 = "true";
	auto response = cpr::Put(cpr::Url{ "http://localhost:18080/startGame" }, cpr::Parameters{ { "start", start1} });
	cpr::Response response7 = cpr::Get(cpr::Url{ "http://localhost:18080/getUserType" });
	interfaceTypes = crow::json::load(response7.text);

	cpr::Response response8 = cpr::Get(cpr::Url{ "http://localhost:18080/getUsers" });
	users = crow::json::load(response8.text);
	openInterface(iterator);
	runTime->stop();

	openTime->start(62000);
	connect(openTime, SIGNAL(timeout()), this, SLOT(Aux()));
	qDebug() << "open timer start\n";

}
void Lobby_Interface::on_profileButton_clicked()
{
	Profile_Interface * profile = new Profile_Interface(this);
	profile->setName(this->name);
	profile->show();
}