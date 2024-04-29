#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_User.h"
#include "Lobby_Interface.h"
#include <cpr/cpr.h>
#include <crow.h>
class User : public QMainWindow
{
    Q_OBJECT

public:
    User(QWidget *parent = nullptr);
    ~User();
    std::string getName() const;
public slots:
    void on_Loggin_clicked();
    void on_Register_clicked();
private:
    Ui::UserClass ui;
    Lobby_Interface* Lobby;
    std::string name;
};
