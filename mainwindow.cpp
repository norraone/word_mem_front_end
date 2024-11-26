//
// Created by OuYang XiangQian on 24-11-26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    createMenuBar();
    createLoginForm();
    createButtons();
    setupConnections();
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
}

void MainWindow::setupUi()
{
    // Create main window layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    formLayout = new QGridLayout();
    
    // Set window properties
    setWindowTitle("Login System");
    resize(400, 250);
    
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

    // 创建帮助菜单
    helpMenu = menuBar->addMenu(tr("&Help"));
    aboutAction = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAction);

    // 连接信号和槽
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExitTriggered);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearTriggered);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutTriggered);
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
        statusLabel->setText("Please enter both username and password!");
        return;
    }
    
    // Here you would normally validate against a database
    statusLabel->setText("Login attempted with username: " + username);
    
    // Clear password field for security
    passwordInput->clear();
}

void MainWindow::onSignUpClicked()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter both username and password to sign up!");
        return;
    }
    
    // Here you would normally add the user to a database
    statusLabel->setText("Sign up attempted with username: " + username);
    
    // Clear both fields after sign up attempt
    usernameInput->clear();
    passwordInput->clear();
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
        "Version: 2.0\n"
        "Author: Your Name\n"
        "Copyright 2023\n\n"
        "This is a Qt-based login system with the following features:\n"
        "• User authentication\n"
        "• Account creation\n"
        "• Secure password handling\n\n"
        "For more information, please contact: your.email@example.com"
    );
    
    QMessageBox::about(this, tr("About Login System"), aboutText);
}
