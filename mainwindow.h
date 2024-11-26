//
// Created by OuYang XiangQian on 24-11-26.
//
/**
 * @brief MainWindow class represents the main window of the application.
 * 
 * This class inherits from QMainWindow and provides the primary user interface
 * for the application. It includes functionality for setting up the UI and
 * handling user interactions.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#include "userdatabase.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();
    void onSignUpClicked();
    void onAboutTriggered();
    void onExitTriggered();
    void onClearTriggered();
    void changeFontSize(QAction* action);
    void changeFontFamily(QAction* action);

private:
    void setupUi();
    void createLoginForm();
    void createButtons();
    void setupConnections();
    void createMenuBar();
    void createFontMenu();
    void createFontFamilyMenu();
    void updateFontStyle();

    // UI Elements
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QGridLayout *formLayout;
    
    // Form fields
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    
    // Buttons
    QPushButton *loginButton;
    QPushButton *signUpButton;
    
    // Status
    QLabel *statusLabel;

    // 菜单相关成员
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *fontMenu;
    QMenu *fontFamilyMenu;
    QAction *exitAction;
    QAction *clearAction;
    QAction *aboutAction;
    QActionGroup *fontSizeGroup;
    QActionGroup *fontFamilyGroup;
    QString currentFontFamily;
    int currentFontSize;

    // Database
    UserDatabase *userDb;
};

#endif //MAINWINDOW_H
