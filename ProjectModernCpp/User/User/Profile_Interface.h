#pragma once

#include <QMainWindow>
#include "ui_Profile_Interface.h"

#include <cpr/cpr.h>
#include <crow.h>
#include <fstream>
#include <QFileDialog>


class Profile_Interface : public QMainWindow
{
	Q_OBJECT

public slots:
	void on_imageButton_clicked();

public:
	Profile_Interface(QWidget* parent = nullptr);
	~Profile_Interface();
	void setName(std::string);

	void saveImagePath(const QString& imagePath);
	QString loadImagePath();

private:
	Ui::Profile_InterfaceClass ui;
	std::string name;
	/*
	void setScores();
	std::vector<std::string> userScores*/;
};
