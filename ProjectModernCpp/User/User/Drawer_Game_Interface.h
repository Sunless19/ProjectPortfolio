#pragma once

#include <QMainWindow>
#include "ui_Drawer_Game_Interface.h"
#include <qevent.h>
#include <qpainter.h>
#include<QScreen>
#include<qrect.h>

#include <qurl.h>
#include <cpr/cpr.h>
#include <crow.h>

#include <QTimer>
#include <QApplication>
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QBuffer>
#include<QJsonObject>
#include<QJsonDocument>
#include "StopWatch.h"
#include "Transition.h"

class Drawer_Game_Interface : public QMainWindow
{
	Q_OBJECT

public:
	Drawer_Game_Interface(QWidget* parent = nullptr);
	~Drawer_Game_Interface();
	bool openImage(const QString& fileName);                         //incarca o imagine din fisier, poate fi modificata
	bool saveImage(const QString& fileName, const char* fileFormat); //salveaza imaginea curenta intr-un fisier
	void setPenColor(const QColor& newColor);
	void setPenWidth(int newWidth);

	bool isModified() const { return modified; }    // verifica daca s-a modificat ceva fata de starea anterioara
	QColor penColor() const { return myPenColor; }  // tine minte culoarea selectata
	int penWidth() const { return myPenWidth; }     // tine minte grosimea selectata
	// void closeAndOpenGuesser();


public slots:
	void clearImage();  //sterge imaginea curenta
	void print();       //afisarea imaginii curente

	void redButtonClicked();
	void blueButtonClicked();
	void yellowButtonClicked();
	void blackButtonClicked();
	void cyanButtonClicked();
	void greenButtonClicked();
	void limeButtonClicked();
	void pinkButtonClicked();
	void purpleButtonClicked();
	void brownButtonClicked();
	void orangeButtonClicked();
	void clearButtonClicked();
	void eraserButtonClicked();
	void updateChat();
	void DeleteChatMessage(const std::string& contentToDelete);
	void getChatAndDelete();
	void updatePlayerList();
	void getPLayers();


	void runGame();
	void setName(std::string);
	std::string getName();
	void setWord(std::string);              //functie de setword pentru fiecare joc .

	void watch();
	void autoSaveImage();
	void sendImage();

protected:
	void mousePressEvent(QMouseEvent* event) override;  //
	void mouseMoveEvent(QMouseEvent* event) override;   // aceste 3 functii au rolul de a implementa desenul
	void mouseReleaseEvent(QMouseEvent* event) override;//
	bool isPointInsideAllowedRegion(const QPoint& point);
	void paintEvent(QPaintEvent* event) override;       // pt. updatarea desenului
	void resizeEvent(QResizeEvent* event) override;     // verif. ca QImaginea pe care desenam sa fie minim de dimensiunea widgetului

private:
	Ui::Drawer_Game_InterfaceClass ui;
	Ui::Drawer_Game_InterfaceClass drawdrawAreaUi;  // Adaugati aceasta linie
	void drawLineTo(const QPoint& endPoint);                // excuta actiunea efectiva de desenare
	void resizeImage(QImage* image, const QSize& newSize);  // schimba marimile imaginii

	bool modified = false;
	bool scribbling = false;        // este true daca se tine apasat click in zona de desenare
	int myPenWidth = 1;
	QColor myPenColor = Qt::black;
	QImage image;                   // contine imaginea desenata
	QPoint lastPoint;               // tine minte ultima pozitie a unei actiuni a mouse-ului

	QTimer* updateTimer;
	std::string word;
	StopWatch stopwatch;
	std::string name;

	short int seconds;
	QTimer timer;
};
