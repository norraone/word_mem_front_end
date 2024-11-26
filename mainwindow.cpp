//
// Created by OuYang XiangQian on 24-11-26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "userdatabase.h" // Include UserDatabase header

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    currentFontFamily = "OpenSans";  // Default font
    currentFontSize = 36;         // Changed default size to 48
    userDb = new UserDatabase(this); // Initialize database
    if (!userDb->initialize()) {
        QMessageBox::critical(this, "Error", "Failed to initialize database: " + userDb->lastError());
    }
    setupUi();
    createMenuBar();
    createLoginForm();
    createButtons();
    setupConnections();
    
    // Apply a soft blue theme
    updateFontStyle();
}

MainWindow::~MainWindow()
{
    delete usernameLabel;
    delete passwordLabel;
    delete usernameInput;
    delete passwordInput;
    delete loginButton;
    delete signUpButton;
    delete statusLabel;
    delete formLayout;
    delete mainLayout;
    delete menuBar;
    delete fileMenu;
    delete exitAction;
    delete editMenu;
    delete clearAction;
    delete helpMenu;
    delete aboutAction;
    delete fontMenu;
    delete fontSizeGroup;
    delete fontFamilyMenu;
    delete fontFamilyGroup;
    // Database is deleted automatically since it's a child of MainWindow
}

void MainWindow::setupUi()
{
    // Create main window layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    formLayout = new QGridLayout();
    
    // Set window properties
    setWindowTitle("Login System");
    resize(800, 600);  // Increased window size
    
    // Create status label
    statusLabel = new QLabel("Welcome! Please login or sign up.", centralWidget);
    statusLabel->setAlignment(Qt::AlignCenter);
    
    // Add status label to main layout
    mainLayout->addWidget(statusLabel);
    
    // Set central widget
    setCentralWidget(centralWidget);
}

void MainWindow::createMenuBar()
{
    // 创建菜单栏
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // 创建文件菜单
    fileMenu = menuBar->addMenu(tr("&File"));
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    fileMenu->addAction(exitAction);

    // 创建编辑菜单
    editMenu = menuBar->addMenu(tr("&Edit"));
    clearAction = new QAction(tr("&Clear Fields"), this);
    clearAction->setShortcut(tr("Ctrl+L"));
    editMenu->addAction(clearAction);

    // 创建字体菜单
    createFontMenu();
    createFontFamilyMenu();

    // 创建帮助菜单 (moved to last)
    helpMenu = menuBar->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAction);

    // 连接信号和槽
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExitTriggered);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearTriggered);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutTriggered);
}

void MainWindow::createFontMenu()
{
    fontMenu = menuBar->addMenu(tr("&Font Size"));
    fontSizeGroup = new QActionGroup(this);
    fontSizeGroup->setExclusive(true);

    QList<int> fontSizes = {8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 32, 36, 40, 48};
    for (int size : fontSizes) {
        QAction* sizeAction = new QAction(QString::number(size), this);
        sizeAction->setCheckable(true);
        sizeAction->setData(size);
        if (size == currentFontSize) sizeAction->setChecked(true);
        fontSizeGroup->addAction(sizeAction);
        fontMenu->addAction(sizeAction);
    }

    connect(fontSizeGroup, &QActionGroup::triggered, this, &MainWindow::changeFontSize);
}

void MainWindow::createFontFamilyMenu()
{
    fontFamilyMenu = menuBar->addMenu(tr("&Font Family"));
    fontFamilyGroup = new QActionGroup(this);
    fontFamilyGroup->setExclusive(true);

    QStringList fontFamilies = {
        "Arial", "Helvetica", "Times New Roman", "Courier New",
        "Verdana", "Georgia", "Calibri", "Tahoma", "Segoe UI",
        "Roboto", "Open Sans", "Lato", "Ubuntu"
    };

    for (const QString& family : fontFamilies) {
        QAction* familyAction = new QAction(family, this);
        familyAction->setCheckable(true);
        familyAction->setData(family);
        if (family == currentFontFamily) familyAction->setChecked(true);
        fontFamilyGroup->addAction(familyAction);
        fontFamilyMenu->addAction(familyAction);
    }

    connect(fontFamilyGroup, &QActionGroup::triggered, this, &MainWindow::changeFontFamily);
}

void MainWindow::changeFontSize(QAction* action)
{
    currentFontSize = action->data().toInt();
    updateFontStyle();
}

void MainWindow::changeFontFamily(QAction* action)
{
    currentFontFamily = action->data().toString();
    updateFontStyle();
}

void MainWindow::updateFontStyle()
{
    QString newStyle = QString(
        "QWidget {\n"
        "    background-color: #F0F8FF;\n"
        "} \n"
        "QPushButton {\n"
        "    background-color: #B0E0E6;\n"
        "    color: #2F4F4F;\n"
        "    border-radius: 8px;\n"
        "    padding: 8px 15px;\n"
        "    border: 1px solid #ADD8E6;\n"
        "    font-family: '%1';\n"
        "    font-size: %2px;\n"
        "} \n"
        "QPushButton:hover {\n"
        "    background-color: #ADD8E6;\n"
        "} \n"
        "QLineEdit {\n"
        "    background-color: white;\n"
        "    border: 2px solid #B0E0E6;\n"
        "    border-radius: 6px;\n"
        "    padding: 5px;\n"
        "    font-family: '%1';\n"
        "    font-size: %2px;\n"
        "}\n"
        "QLabel {\n"
        "    color: #4682B4;\n"
        "    font-family: '%1';\n"
        "    font-size: %2px;\n"
        "}\n"
        "QMenuBar {\n"
        "    background-color: #F0F8FF;\n"
        "    border-bottom: 1px solid #B0E0E6;\n"
        "    font-family: '%1';\n"
        "    font-size: %2px;\n"
        "}\n"
        "QMenuBar::item:selected {\n"
        "    background-color: #B0E0E6;\n"
        "}\n"
        "QMenu {\n"
        "    font-family: '%1';\n"
        "    font-size: %2px;\n"
        "}")
        .arg(currentFontFamily)
        .arg(currentFontSize);

    setStyleSheet(newStyle);
}

void MainWindow::createLoginForm()
{
    // Create form labels
    usernameLabel = new QLabel("Username:", centralWidget);
    passwordLabel = new QLabel("Password:", centralWidget);
    
    // Create form inputs
    usernameInput = new QLineEdit(centralWidget);
    passwordInput = new QLineEdit(centralWidget);
    passwordInput->setEchoMode(QLineEdit::Password);
    
    // Add to form layout
    formLayout->addWidget(usernameLabel, 0, 0);
    formLayout->addWidget(usernameInput, 0, 1);
    formLayout->addWidget(passwordLabel, 1, 0);
    formLayout->addWidget(passwordInput, 1, 1);
    
    // Add form layout to main layout
    mainLayout->addLayout(formLayout);
}

void MainWindow::createButtons()
{
    // Create button container
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    // Create buttons
    loginButton = new QPushButton("Login", centralWidget);
    signUpButton = new QPushButton("Sign Up", centralWidget);
    
    // Add buttons to layout
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(signUpButton);
    
    // Add button layout to main layout
    mainLayout->addLayout(buttonLayout);
}

void MainWindow::setupConnections()
{
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(signUpButton, &QPushButton::clicked, this, &MainWindow::onSignUpClicked);
}

void MainWindow::onLoginClicked()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter both username and password.");
        return;
    }

    if (userDb->validateUser(username, password)) {
        statusLabel->setText("Login successful!");
        statusLabel->setStyleSheet("color: green;");
    } else {
        statusLabel->setText("Invalid username or password.");
        statusLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::onSignUpClicked()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter both username and password.");
        return;
    }

    if (password.length() < 6) {
        statusLabel->setText("Password must be at least 6 characters long.");
        return;
    }

    if (userDb->addUser(username, password)) {
        statusLabel->setText("Account created successfully!");
        statusLabel->setStyleSheet("color: green;");
    } else {
        statusLabel->setText(userDb->lastError());
        statusLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::onExitTriggered()
{
    QApplication::quit();
}

void MainWindow::onClearTriggered()
{
    usernameInput->clear();
    passwordInput->clear();
    statusLabel->setText("Fields cleared");
}

void MainWindow::onAboutTriggered()
{
    QString aboutText = tr(
        "Login System\n\n"
        "Version: 1.0\n"
        "Author: OuYang XiangQian\n"
        "Copyright 2024\n\n"
        "This is a Qt-based login system with the following features:\n"
        "• User authentication\n"
        "• Account creation\n"
        "• Password handling\n\n"
        "For more information, please contact: norra1@zoho.com.cn"
    );
    
    QMessageBox::about(this, tr("About Login System"), aboutText);
}
