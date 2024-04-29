#include "Drawer_Game_Interface.h"


Drawer_Game_Interface::Drawer_Game_Interface(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	updateTimer = new QTimer(this);
	updateTimer->start(10000);
	getPLayers();
	// Connect signals and slots
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updatePlayerList()));
	QTimer* chatUpdateTimer = new QTimer(this);
	connect(chatUpdateTimer, SIGNAL(timeout()), this, SLOT(updateChat()));
	chatUpdateTimer->start(3000);

	QTimer* runTimer = new QTimer(this);
	connect(runTimer, SIGNAL(timeout()), this, SLOT(runGame()));
	runTimer->start(60000);

	
	seconds = 60;
	connect(&timer, SIGNAL(timeout()), this, SLOT(watch()));
	timer.start(1000);

	QTimer* autoSaveTimer = new QTimer(this);
	connect(autoSaveTimer, SIGNAL(timeout()), this, SLOT(autoSaveImage()));
	autoSaveTimer->start(5000);  // Setare interval de 2 secunde pentru salvare automat?

	QTimer* sendImg = new QTimer(this);
	connect(sendImg, SIGNAL(timeout()), this, SLOT(sendImage())); 
	sendImg->start(5000); 
}

Drawer_Game_Interface::~Drawer_Game_Interface()
{}

void Drawer_Game_Interface::runGame()
{
	/*Transition* lobby = new Transition(this);
	lobby->setName(this->getName());
	lobby->show();*/
	this->close();
	delete this;
	//closeAndOpenGuesser();

}

void Drawer_Game_Interface::setName(std::string name)
{

	this->name = name;

}

std::string Drawer_Game_Interface::getName()
{
	return name;
}

bool Drawer_Game_Interface::openImage(const QString& fileName)
{
	//aici primim o imagine din fisier si verificam sa fie minim pe marimile widget-ului
	QImage loadedImage;
	if (!loadedImage.load(fileName))
		return false;

	QSize newSize = loadedImage.size().expandedTo(size());
	resizeImage(&loadedImage, newSize);//redimensionare
	image = loadedImage;
	modified = false;
	update();
	return true;
}

bool Drawer_Game_Interface::saveImage(const QString& fileName, const char* fileFormat)
{
	///Creaza un obiect de tip QImage si salveaza ce este vizibil din imaginea importata si o salveaza
	//Daca totul este ok, setam modified ca si false
	QImage visibleImage = image;
	resizeImage(&visibleImage, size());

	if (visibleImage.save(fileName, fileFormat))
	{
		modified = false;
		return true;
	}
	return false;
}

void Drawer_Game_Interface::autoSaveImage()
{
	// Define?te un nume de fi?ier unic, de exemplu, bazat pe data ?i or?
	QString fileName = "auto_save.png";

	// Salveaz? imaginea cu numele de fi?ier generat
	saveImage(fileName, "PNG");
}

void Drawer_Game_Interface::sendImage()
{
	// Convert the QImage to QByteArray for sending
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, "PNG"); // Save the image to the buffer in PNG format

	// Make a POST request to send the image data to the server
	cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/uploadImage" },
		cpr::Body{ byteArray.toStdString() });

	// Check the response status
	if (response.status_code == 200) {
		qDebug() << "Image sent successfully!";
	}
	else {
		qDebug() << "Failed to send image. Status code: " << response.status_code;
	}
}

void Drawer_Game_Interface::setPenColor(const QColor& newColor)
{
	//seteaza culoarea
	myPenColor = newColor;
}

void Drawer_Game_Interface::setPenWidth(int newWidth)
{
	//seteaza grosimea
	myPenWidth = newWidth;
}

void Drawer_Game_Interface::clearImage()
{
	//setam toata imaginea cu alb(255,255,255) pentru a sterge desenul curent
	image.fill(qRgb(255, 255, 255));
	modified = true;
	update();
}

void Drawer_Game_Interface::print()
{
	//Cream un obiect QPrinter HR pt output
	//In QDialog specificam dimensiunile dorite
	//Daca este acceptata dimensiunea incepem desenarea
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
	QPrinter printer(QPrinter::HighResolution);

	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec() == QDialog::Accepted) {
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = image.size();
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(image.rect());
		painter.drawImage(0, 0, image);
	}
#endif // QT_CONFIG(printdialog)
}

void Drawer_Game_Interface::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		// Verifica?i dac? punctul de ap?sare a butonului este în interiorul unei regiuni
		if (isPointInsideAllowedRegion(event->pos()))
		{
			lastPoint = event->pos();
			scribbling = true;
		}
	}
}

void Drawer_Game_Interface::mouseMoveEvent(QMouseEvent* event)
{
	// Desen?m linia doar dac? mi?carea mouse-ului are loc în interiorul regiunii permise
	if ((event->buttons() & Qt::LeftButton) && scribbling && isPointInsideAllowedRegion(event->pos()))
		drawLineTo(event->pos());
}

void Drawer_Game_Interface::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && scribbling && isPointInsideAllowedRegion(event->pos()))
	{
		drawLineTo(event->pos());
		scribbling = false;
	}
}

bool Drawer_Game_Interface::isPointInsideAllowedRegion(const QPoint& point)
{
	// Defini?i regiunea permis? (de exemplu, o margine de 20 de pixeli)
	QRect allowedRegion(100, 100, width() - 300, height() - 300);

	// Verifica?i dac? punctul este în interiorul regiunii permise
	return allowedRegion.contains(point);
}

void Drawer_Game_Interface::paintEvent(QPaintEvent* event)
{
	connect(ui.redButton, &QPushButton::clicked, this, &Drawer_Game_Interface::redButtonClicked);
	connect(ui.blueButton, &QPushButton::clicked, this, &Drawer_Game_Interface::blueButtonClicked);
	connect(ui.yellowButton, &QPushButton::clicked, this, &Drawer_Game_Interface::yellowButtonClicked);
	connect(ui.blackButton, &QPushButton::clicked, this, &Drawer_Game_Interface::blackButtonClicked);
	connect(ui.cyanButton, &QPushButton::clicked, this, &Drawer_Game_Interface::cyanButtonClicked);
	connect(ui.greenButton, &QPushButton::clicked, this, &Drawer_Game_Interface::greenButtonClicked);
	connect(ui.limeButton, &QPushButton::clicked, this, &Drawer_Game_Interface::limeButtonClicked);
	connect(ui.pinkButton, &QPushButton::clicked, this, &Drawer_Game_Interface::pinkButtonClicked);
	connect(ui.purpleButton, &QPushButton::clicked, this, &Drawer_Game_Interface::purpleButtonClicked);
	connect(ui.brownButton, &QPushButton::clicked, this, &Drawer_Game_Interface::brownButtonClicked);
	connect(ui.orangeButton, &QPushButton::clicked, this, &Drawer_Game_Interface::orangeButtonClicked);

	connect(ui.clearButton, &QPushButton::clicked, this, &Drawer_Game_Interface::clearButtonClicked);
	connect(ui.eraserButton, &QPushButton::clicked, this, &Drawer_Game_Interface::eraserButtonClicked);

	connect(ui.widthSlider, &QSlider::valueChanged, this, &Drawer_Game_Interface::setPenWidth);
	ui.widthSlider->setRange(1, 10); // Seteaz? intervalul grosimii penului


	// Crea?i un obiect QPainter pentru zona de desenare
	QPainter painter(this);

	// Seta?i culoarea pentru fundalul UI-ului la albastru
	painter.fillRect(rect(), Qt::gray);

	// Defini?i regiunea permis? (rama alb?)
	int margin = 100;
	QRect allowedRegion(margin + 100, margin, width() - 420, height() - 300);

	// Desena?i culoarea pentru regiunea permis? (fundalul alb)
	painter.fillRect(allowedRegion, Qt::white);

	// Excluziunea regiunii în care nu dori?i s? desena?i (culoarea albastr?)
	QRect exclusionRegion(200, 100, width() - 420, height() - 300);
	painter.fillRect(exclusionRegion, Qt::blue);

	// Desena?i imaginea în afara regiunii excluse (în restul zonei albe)
	QRect dirtyRect = event->rect();
	QRect imageRect = image.rect();
	QRect intersection = dirtyRect.intersected(exclusionRegion);

	if (!intersection.isEmpty()) {
		// Desena?i imaginea în regiunea alb? r?mas?
		QRect allowedRect = dirtyRect.intersected(allowedRegion);
		painter.drawImage(allowedRect, image, allowedRect);
	}
	const QString qword = QString::fromStdString(word);

	ui.GuessWord->setText(qword);
	ui.GuessWord->setAlignment(Qt::AlignCenter);

	ui.GuessWord->show();

}

void Drawer_Game_Interface::resizeEvent(QResizeEvent* event)
{
	// Redimensiona?i imaginea numai dac? widget-ul devine mai mare decât dimensiunea imaginii
	if (width() > image.width() || height() > image.height()) {
		int newWidth = qMax(width(), image.width());
		int newHeight = qMax(height(), image.height());
		resizeImage(&image, QSize(newWidth, newHeight));

		// Calcula?i pozi?ia central? în widget
		int x = (width() - image.width()) / 2;
		int y = (height() - image.height()) / 2;

		// Actualiza?i pozi?ia zonei de desenare
		setGeometry(x + 70, y + 70, image.width() - 10, image.height());

		update();
	}
	//setGeometry(20,20, image.width(), image.height());
	update();
	QWidget::resizeEvent(event);
}

void Drawer_Game_Interface::setWord(std::string guessword)
{
	
	this->word = guessword;
	qDebug() << word;
}

void Drawer_Game_Interface::drawLineTo(const QPoint& endPoint)
{
	//Desenam o linie de la ultima pozitie pana la pozitia curenta
	//Setam modificat ca true
	//Resetam ultima pozitie pentru a continua din punctul in care ne-am oprit
	//Updatam imaginea
	QPainter painter(&image);
	painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
		Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);
	modified = true;

	int rad = (myPenWidth / 2) + 2;
	update(QRect(lastPoint, endPoint).normalized()
		.adjusted(-rad, -rad, +rad, +rad));
	lastPoint = endPoint;
}

void Drawer_Game_Interface::resizeImage(QImage* image, const QSize& newSize)
{
	//Pentru redimensionarea imaginii cream o noua imagine alba cu dimensiunea dorita si uploadam vechea imagine 
	if (image->size() == newSize)
		return;

	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}

void Drawer_Game_Interface::redButtonClicked()
{

	setPenColor(Qt::red);
}

void Drawer_Game_Interface::blueButtonClicked()
{
	setPenColor(Qt::blue);

}

void Drawer_Game_Interface::yellowButtonClicked()
{
	setPenColor(Qt::yellow);

}

void Drawer_Game_Interface::blackButtonClicked()
{
	setPenColor(Qt::black);
}

void Drawer_Game_Interface::cyanButtonClicked()
{
	setPenColor(Qt::cyan);

}

void Drawer_Game_Interface::greenButtonClicked()
{

	setPenColor(Qt::darkGreen);
}

void Drawer_Game_Interface::limeButtonClicked()
{
	setPenColor(Qt::green);
}

void Drawer_Game_Interface::pinkButtonClicked()
{
	setPenColor(Qt::magenta);


}

void Drawer_Game_Interface::purpleButtonClicked()
{
	setPenColor(QColor(148, 0, 211)); // RGB values for pink color

}

void Drawer_Game_Interface::brownButtonClicked()
{
	setPenColor(QColor(165, 42, 42)); // RGB values for pink color

}

void Drawer_Game_Interface::orangeButtonClicked()
{

	setPenColor(QColor(255, 165, 0));
}

void Drawer_Game_Interface::clearButtonClicked()
{
	//setam toata imaginea cu alb(255,255,255) pentru a sterge desenul curent
	image.fill(qRgb(255, 255, 255));
	modified = true;
	update();

}

void Drawer_Game_Interface::eraserButtonClicked()
{
	setPenColor(QColor(255, 255, 255));
}

void Drawer_Game_Interface::DeleteChatMessage(const std::string& contentToDelete)
{
	cpr::Response deleteResponse = cpr::Delete(cpr::Url{ "http://localhost:18080/chat" }, cpr::Parameters{ {"Message", contentToDelete} });
}

void Drawer_Game_Interface::getChatAndDelete()
{
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/get_chat" });
	auto chat = crow::json::load(response.text);

	// Itera?i prin mesaje ?i face?i cereri DELETE pentru fiecare
	for (const auto& message : chat)
	{
		std::string content = message["content"].s();
		DeleteChatMessage(content);
	}
}

void Drawer_Game_Interface::updatePlayerList()
{
	this->getPLayers();
}

void Drawer_Game_Interface::getPLayers()
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

void Drawer_Game_Interface::updateChat()
{
	auto responseMessages = cpr::Get(cpr::Url{ "http://localhost:18080/get_chat" });
	auto responseRows = crow::json::load(responseMessages.text);
	ui.chatDisplayDraw->clear();
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
		ui.chatDisplayDraw->append(qstrMessage);
	}
}

void Drawer_Game_Interface::watch()
{
	seconds = seconds - 1;
	if (seconds >= 0)
	{
		ui.stopWatch->display(seconds);
	}
	if (seconds == 0)this->close();
}




//void Drawer_Game_Interface::closeAndOpenGuesser()
//{
//	std::string start1 = "true";
//	auto response = cpr::Put(cpr::Url{ "http://localhost:18080/startGame" }, cpr::Parameters{ { "start", start1} });
//
//	cpr::Response response1 = cpr::Get(cpr::Url{ "http://localhost:18080/getUserType" });
//	auto interfaceTypes = crow::json::load(response1.text);
//
//	if (interfaceTypes) {
//		std::string userIsDrawer;  // Seteaz? la true dac? utilizatorul curent este desenatorul
//
//		for (const auto& interfaceType : interfaceTypes) {
//			std::string playerName = interfaceType["name"].s();
//			std::string boolValue = interfaceType["guesser"].s();
//
//			if (playerName == this->getName()) {
//				userIsDrawer = boolValue;
//				break;
//			}
//		}
//
//		QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
//		for (int i = 0; i < topLevelWidgets.size(); ++i) {
//			QWidget* widget = topLevelWidgets.at(i);
//			if (widget->objectName() == "Drawer_Game_Interface" || widget->objectName() == "Guess_Game_Interface") {
//				widget->close();  // Închide widget-ul
//				widget->deleteLater();  // Amân? ?tergerea widget-ului
//			}
//		}
//
//		if (userIsDrawer == "true") {
//			Drawer_Game_Interface* draw = new Drawer_Game_Interface(this);
//			draw->setName(this->getName());
//			draw->show();
//		}
//		else if (userIsDrawer == "false") {
//			Guess_Game_Interface* guesser = new Guess_Game_Interface(this);
//			guesser->setName(this->getName());
//			guesser->show();
//		}
//		else if (userIsDrawer == "end") {
//			Lobby_Interface* lobby = new Lobby_Interface(this);
//			lobby->setName(this->getName());
//			lobby->show();
//		}
//	}
//}

//void Drawer_Game_Interface::sendImage()
//{
//	// Convert the QImage to a QByteArray
//	QByteArray byteArray;
//	QBuffer buffer(&byteArray);
//	buffer.open(QIODevice::WriteOnly);
//	image.save(&buffer, "PNG");
//
//	// Make a POST request to send the image data to the server
//	cpr::Response response = cpr::Post(cpr::Url{ "http://localhost:18080/uploadImage" },
//		cpr::Body{ byteArray.toStdString() });
//
//	// Check the response status
//	if (response.status_code == 200) {
//		qDebug() << "Image sent successfully!";
//	}
//	else {
//		qDebug() << "Failed to send image. Status code: " << response.status_code;
//	}
//}