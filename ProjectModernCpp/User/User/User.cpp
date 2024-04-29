#include "User.h"
#include <QMessageBox>

User::User(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	Lobby = nullptr;
	QPalette palette;
	palette.setColor(QPalette::Window, Qt::cyan);
	setPalette(palette);
}

User::~User()
{}
std::string User::getName() const
{
	return this->name;
}

void User::on_Loggin_clicked()
{
	this->name = ui.lineEdit->text().toStdString();
	auto usersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/users" });
	std::string json_data = this->name;

	crow::json::rvalue usersJson = crow::json::load(usersResponse.text);
	auto response = cpr::Put(cpr::Url{ "http://localhost:18080/activeUsers" }, cpr::Parameters{ { "name", name} });

	auto found = usersResponse.text.find(this->name);

	if (found == std::string::npos)
	{
		QMessageBox::warning(this, "Error", "The name has been either not registered or typed uncorrectly");
	}
	else {
	Lobby = new Lobby_Interface(this);
	Lobby->setName(this->getName());
	this->close();
	Lobby->show();
	}
}

void User::on_Register_clicked()
{
	this->name = ui.lineEdit->text().toStdString();
	auto usersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/users" });
	std::string json_data = this->name;

	crow::json::rvalue usersJson = crow::json::load(usersResponse.text);

	auto found = usersResponse.text.find(this->name);
	if (found != std::string::npos)
	{
		QMessageBox::warning(this, "Error", "The name already exists and is taken. Please choose another name.");
	}
	else {
		auto response = cpr::Put(cpr::Url{ "http://localhost:18080/adduser" }, cpr::Parameters{ { "name", name} });
	
		this->close();
		Lobby = new Lobby_Interface(this);
		Lobby->show();
	}
}