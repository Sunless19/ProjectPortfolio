#include "Profile_Interface.h"
#include <QSettings>

Profile_Interface::Profile_Interface(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.imageButton, &QPushButton::clicked, this, &Profile_Interface::on_imageButton_clicked);

    // Înc?rca?i calea imaginii salvate
    QString savedImagePath = loadImagePath();
    if (!savedImagePath.isEmpty()) {
        QPixmap pixmap(savedImagePath);
        if (!pixmap.isNull()) {
            ui.imagelabel->setPixmap(pixmap.scaledToWidth(300, Qt::SmoothTransformation));
        }
    }
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::cyan);
    setPalette(palette);
}

Profile_Interface::~Profile_Interface()
{}
void Profile_Interface::setName(std::string nam)
{
	name = nam;
	const QString qname = QString::fromStdString(name);
	ui.username->setText(qname);
}

void Profile_Interface::on_imageButton_clicked()
{
    // Utilizatorul a ap?sat butonul pentru a înc?rca/salva imaginea
    QString imagePath = QFileDialog::getOpenFileName(this, "Alege?i o imagine", "", "Imagini (*.png *.jpg *.bmp)");

    if (!imagePath.isEmpty()) {
        // Salva?i calea imaginii
        saveImagePath(imagePath);

        // Cre?m o instan?? QPixmap cu calea imaginii
        QPixmap pixmap(imagePath);

        if (!pixmap.isNull()) {
            // Afi??m imaginea în QLabel
            ui.imagelabel->setPixmap(pixmap.scaledToWidth(300, Qt::SmoothTransformation));
        }
    }
}


void Profile_Interface::saveImagePath(const QString& imagePath)
{
	QSettings settings("NumeleFirmei", "NumeleAplicatiei");
	settings.setValue("ImagePath", imagePath);
}

QString Profile_Interface::loadImagePath()
{
	QSettings settings("NumeleFirmei", "NumeleAplicatiei");
	return settings.value("ImagePath").toString();
}

//void Profile_Interface::setScores() {
//    // Make the HTTP GET request
//    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/get_score" });
//
//    if (response.status_code == 200) {
//        // Parse the JSON response
//        auto scores = crow::json::load(response.text);
//
//        // Assuming scores is a crow::json::wvalue
//        // You can access the JSON elements as needed
//        for (const auto& score : scores) {
//            // Accessing key and value from the JSON object
//            std::string playerName = score.key();
//
//            // Check if the key "second" exists in the JSON object
//            if (score.has("second") && score["second"].t() == crow::json::type::Number) {
//                float playerScore = score["second"].as<float>(); // Use as<float>() for type conversion
//                // Now you can do something with playerName and playerScore
//                userScores.push_back(std::to_string(playerScore));  // Convert the float to string before pushing into vector
//            }
//            else {
//                // Handle the case where the score is not a float
//                std::cerr << "Error: Score is not a float for player " << playerName << std::endl;
//            }
//        }
//
//    }
//    else {
//        // Handle the case where the server did not return a 200 OK status
//        std::cerr << "HTTP GET request failed. Status code: " << response.status_code << std::endl;
//    }
//}




