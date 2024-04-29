#include "Guess_Game_Interface.h"

Guess_Game_Interface::Guess_Game_Interface(QWidget* parent)
	: QMainWindow(parent),
	wordGuessed(false)
{
	ui.setupUi(this);
	setWindowTitle("Main Game");

	//IMAGINE
	image = QPixmap("auto_save.png");

	if (image.isNull()) {
		qDebug() << "Eroare la incarcarea imaginii!";
	}
	else {
		ui.label->setPixmap(image);
	}

	QTimer* imageTimer = new QTimer(this);
	connect(imageTimer, SIGNAL(timeout()), this, SLOT(updateImage()));
	imageTimer->start(1000);

	updateTimer = new QTimer(this); 
	getPLayers(); 
	// Connect signals and slots
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updatePlayerList()));
	connect(ui.sendButton, SIGNAL(clicked()), this, SLOT(sendMessage())); 

	updateTimer->start(10000);

	QTimer* chatUpdateTimer = new QTimer(this); 
	connect(chatUpdateTimer, SIGNAL(timeout()), this, SLOT(updateChat())); 
	chatUpdateTimer->start(3000);

	
	
	QTimer* runTimer = new QTimer(this);
	connect(runTimer, SIGNAL(timeout()), this, SLOT(closeWindow()));
	runTimer->start(60000);

	seconds = 60;
	connect(&timer, SIGNAL(timeout()), this, SLOT(watch()));
	timer.start(1000);
}

void Guess_Game_Interface::closeWindow()
{
	/*Transition* lobby = new Transition(this);
	lobby->setName(this->getName());
	lobby->show();
	this->close();*/
	delete this;

	//closeAndOpenDrawer();
}

void Guess_Game_Interface::updateImage() 
{
	// Make a GET request to fetch the image data from the server
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/downloadImage" }); 

	// Check the response status
	if (response.status_code == 200) {
		// Convert the image data to QPixmap and set it to the label
		QByteArray byteArray(response.text.c_str(), response.text.length());
		QPixmap newImage;
		newImage.loadFromData(byteArray);

		if (newImage.isNull()) {
			qDebug() << "Error loading image from server!";
		}
		else {
			image = newImage;
			ui.label->setPixmap(image);
		}
	}
	else {
		qDebug() << "Failed to fetch image from server. Status code: " << response.status_code;
	}
}

Guess_Game_Interface::~Guess_Game_Interface()
{}

void Guess_Game_Interface::setName(std::string name)
{
	this->name = name;
}

std::string Guess_Game_Interface::getName()
{
	return this->name;
}

void Guess_Game_Interface::updatePlayerList()
{
	this->getPLayers();
}

void Guess_Game_Interface::getPLayers()
{

	QStandardItemModel* model = new QStandardItemModel();
	QStandardItem* item1 = new QStandardItem("Player 1");

	std::ofstream f("Text.txt");
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/getUsers" });
	std::cout << "Users:\n";
	auto users = crow::json::load(response.text);
	for (const auto& user : users)
	{
		QStandardItem* item1 = new QStandardItem(QString::fromStdString(user["name"].s()));
		item1->setFlags(item1->flags() ^ Qt::ItemIsEditable);
		model->appendRow(item1);

	}

	ui.playerList->setModel(model);


}

void Guess_Game_Interface::updateChat()
{
	auto responseMessages = cpr::Get(cpr::Url{ "http://localhost:18080/get_chat" });
	auto responseRows = crow::json::load(responseMessages.text);
	ui.chatDisplay->clear();
	for (const auto& responseRow : responseRows)
	{
		std::string message;
		std::string mess = std::string(responseRow["Message"]);
		message = message + std::string(responseRow["Username"]);
		message += ": ";
		message += mess;
		QString qstrMessage;

		for (auto c : message)
			qstrMessage.push_back(c);
		ui.chatDisplay->append(qstrMessage);
	}
	
}

void Guess_Game_Interface::sendMessage()
{
	QString message = ui.messageInput->text();
	if (!message.isEmpty())
	{
		ui.messageInput->clear();

		if (message.toLower() == word.c_str())
		{
			// Calcula?i punctajul în func?ie de formula dat?
			

			// Ad?uga?i puncte suplimentare dac? s-a ghicit cu 30 de secunde înainte
			if (seconds > 30)
			{
				score += 100;
			}
			else float score = ((60 - seconds) * 100) / 60;

			// Ad?uga?i punctajul juc?torului în func?ia de scorul calculat
			this->score = score;

			wordGuessed = true; // Seteaz? c? cuvântul a fost ghicit
		}

		// Trimite mesajul c?tre server
		cpr::Response sendMessageResponse = cpr::Put(cpr::Url{ "http://localhost:18080/chat" }, cpr::Parameters{ { "Message" ,message.toStdString()},
		{"Username",this->getName()} });
	}
}

void Guess_Game_Interface::setWord(std::string guessword)
{
	

	this->word = guessword;
	qDebug() << word;
	for (int i = 0; i < word.size(); i++)
	{
		if (i < word.size() - 1)
			secretword += "_ ";
		else
			secretword += "_";
	}
	const QString qword = QString::fromStdString(secretword);

	ui.GuessWord->setText(qword);
	ui.GuessWord->setAlignment(Qt::AlignCenter);

	ui.GuessWord->show();
}

void Guess_Game_Interface::watch()
{
	seconds = seconds - 1;
	if (seconds >= 0)
	{
		ui.stopWatch->display(seconds);


		if (seconds == 30 && !wordGuessed)
		{

			secretword[0] = word[0];
			const QString qword = QString::fromStdString(secretword);
			ui.GuessWord->setText(qword);
		}
		if (seconds == 20 && !wordGuessed)
		{

			secretword[1] = word[1];
			const QString qword = QString::fromStdString(secretword);
			ui.GuessWord->setText(qword);
		}
		if (seconds == 10 && !wordGuessed)
		{

			secretword[2] = word[2];
			const QString qword = QString::fromStdString(secretword);
			ui.GuessWord->setText(qword);
		}
		ui.GuessWord->setAlignment(Qt::AlignCenter);
	}

	if (seconds == 0)
		this->close();
}

//void Guess_Game_Interface::updateImage()
//{
//	QPixmap newImage("auto_save.png");
//
//	if (newImage.isNull()) {
//		qDebug() << "Eroare la incarcarea imaginii!";
//	}
//	else {
//		image = newImage;
//		ui.label->setPixmap(image);
//	}
//}