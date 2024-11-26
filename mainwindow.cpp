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
    applyTheme("blue");
    blueThemeAct->setChecked(true);
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
    delete lightThemeAct;
    delete blueThemeAct;
    // Database is deleted automatically since it's a child of MainWindow
}

void MainWindow::setupUi()
{
    // Create main window layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(16);  // Material Design spacing
    mainLayout->setContentsMargins(24, 24, 24, 24);  // Material Design margins

    // Create a card-like container for the form
    QWidget* formCard = new QWidget(this);
    formCard->setObjectName("formCard");
    formCard->setStyleSheet(
        "QWidget#formCard {"
        "   background-color: white;"
        "   border-radius: 8px;"
        "   padding: 24px;"
        "   margin: 8px;"
        "}"
    );

    formLayout = new QGridLayout(formCard);
    formLayout->setSpacing(16);
    formLayout->setContentsMargins(24, 24, 24, 24);

    // Set window properties
    setWindowTitle("Vocabulary Learning App");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    resize(800, 600);  // Set a good default size

    // Create status label
    statusLabel = new QLabel("Welcome! Please login or sign up.", centralWidget);
    statusLabel->setAlignment(Qt::AlignCenter);
    
    // Add status label to main layout
    mainLayout->addWidget(statusLabel);
    
    // Add form layout to main layout
    mainLayout->addWidget(formCard);
    
    // Set central widget
    setCentralWidget(centralWidget);
}

void MainWindow::createMenuBar()
{
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // File Menu
    fileMenu = menuBar->addMenu(tr("&File"));
    exitAction = new QAction(QIcon(":/icons/exit.png"), tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    fileMenu->addAction(exitAction);

    // Theme Menu
    themeMenu = menuBar->addMenu(tr("&Theme"));
    lightThemeAct = new QAction(tr("&Light"), this);
    lightThemeAct->setCheckable(true);
    connect(lightThemeAct, &QAction::triggered, this, &MainWindow::setLightTheme);

    blueThemeAct = new QAction(tr("&Blue"), this);
    blueThemeAct->setCheckable(true);
    connect(blueThemeAct, &QAction::triggered, this, &MainWindow::setBlueTheme);

    QActionGroup *themeGroup = new QActionGroup(this);
    themeGroup->addAction(lightThemeAct);
    themeGroup->addAction(blueThemeAct);
    blueThemeAct->setChecked(true);  // Default theme

    themeMenu->addAction(lightThemeAct);
    themeMenu->addAction(blueThemeAct);

    // Edit Menu
    editMenu = menuBar->addMenu(tr("&Edit"));
    clearAction = new QAction(QIcon(":/icons/clear.png"), tr("&Clear Fields"), this);
    clearAction->setShortcut(tr("Ctrl+L"));
    editMenu->addAction(clearAction);

    // Help Menu
    helpMenu = menuBar->addMenu(tr("&Help"));
    aboutAction = new QAction(QIcon(":/icons/help.png"), tr("&About"), this);
    helpMenu->addAction(aboutAction);

    // Connect signals and slots
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExitTriggered);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearTriggered);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAboutTriggered);
}

void MainWindow::setLightTheme() {
    applyTheme("light");
    lightThemeAct->setChecked(true);
    blueThemeAct->setChecked(false);
}

void MainWindow::setBlueTheme() {
    applyTheme("blue");
    blueThemeAct->setChecked(true);
    lightThemeAct->setChecked(false);
}

void MainWindow::applyTheme(const QString& theme) {
    // Common Material Design styles
    QString commonStyles = 
        "QWidget { font-family: 'Roboto', sans-serif; }"
        "QPushButton { "
        "   border-radius: 20px; "
        "   min-width: 88px; "
        "   min-height: 36px; "
        "   padding: 8px 16px; "
        "   font-weight: 500; "
        "   text-transform: uppercase; "
        "   border: none;"
        "}"
        "QPushButton:hover { "
        "   margin-top: -2px; "
        "   margin-bottom: 2px; "
        "}"
        "QPushButton:pressed { "
        "   margin-top: 2px; "
        "   margin-bottom: -2px; "
        "}"
        "QLineEdit { "
        "   border-radius: 4px; "
        "   padding: 8px 12px; "
        "   min-height: 36px; "
        "   border: none; "
        "   margin: 8px 0; "
        "   background: transparent; "
        "}"
        "QMenuBar { "
        "   border: none; "
        "   min-height: 48px; "
        "   padding: 8px; "
        "   spacing: 12px; "
        "}"
        "QMenuBar::item { "
        "   padding: 12px 20px; "
        "   margin: 4px 8px; "
        "   border-radius: 6px; "
        "   font-size: 14px; "
        "}"
        "QMenu { "
        "   border: 1px solid rgba(0, 0, 0, 0.1); "
        "   padding: 12px 0; "
        "   margin: 8px 0; "
        "   min-width: 250px; "
        "}"
        "QMenu::item { "
        "   padding: 16px 32px; "
        "   font-size: 14px; "
        "   min-height: 32px; "
        "   margin: 8px 12px; "
        "   border-radius: 6px; "
        "}"
        "QMenu::separator { "
        "   height: 1px; "
        "   margin: 12px 0; "
        "}"
        "QMenu::indicator { "
        "   width: 18px; "
        "   height: 18px; "
        "   margin-left: 8px; "
        "}"
        "QMenu::right-arrow { "
        "   margin: 8px; "
        "   width: 16px; "
        "   height: 16px; "
        "}";

    QString styleSheet;
    
    if (theme == "light") {
        styleSheet = commonStyles +
            "QMainWindow, QWidget { background-color: #fafafa; color: #212121; }"
            "QPushButton { "
            "   background-color: #2196F3; "
            "   color: white; "
            "   border: 2px solid #1976D2; "
            "}"
            "QPushButton:hover { "
            "   background-color: #1976D2; "
            "}"
            "QPushButton:pressed { "
            "   background-color: #1565C0; "
            "}"
            "QLineEdit { "
            "   background-color: white; "
            "   color: #212121; "
            "   border-bottom: 2px solid #BDBDBD; "
            "}"
            "QLineEdit:focus { "
            "   border-bottom: 2px solid #2196F3; "
            "}"
            "QMenuBar { "
            "   background-color: white; "
            "   color: #212121; "
            "   border-bottom: 1px solid #E0E0E0; "
            "}"
            "QMenuBar::item:selected { "
            "   background-color: #E3F2FD; "
            "}"
            "QMenu { "
            "   background-color: white; "
            "   border: 1px solid #E0E0E0; "
            "}"
            "QMenu::item:selected { "
            "   background-color: #E3F2FD; "
            "}"
            "QMenu::separator { "
            "   background-color: #EEEEEE; "
            "}";
    }
    else if (theme == "blue") {
        styleSheet = commonStyles +
            "QMainWindow, QWidget { background-color: #E3F2FD; color: #212121; }"
            "QPushButton { "
            "   background-color: #1E88E5; "
            "   color: white; "
            "   border: 2px solid #1976D2; "
            "}"
            "QPushButton:hover { "
            "   background-color: #1976D2; "
            "}"
            "QPushButton:pressed { "
            "   background-color: #1565C0; "
            "}"
            "QLineEdit { "
            "   background-color: white; "
            "   color: #212121; "
            "   border-bottom: 2px solid #90CAF9; "
            "}"
            "QLineEdit:focus { "
            "   border-bottom: 2px solid #1E88E5; "
            "}"
            "QMenuBar { "
            "   background-color: #1E88E5; "
            "   color: white; "
            "   border-bottom: 1px solid #1976D2; "
            "}"
            "QMenuBar::item:selected { "
            "   background-color: #1976D2; "
            "}"
            "QMenu { "
            "   background-color: white; "
            "   border: 1px solid #90CAF9; "
            "}"
            "QMenu::item:selected { "
            "   background-color: #E3F2FD; "
            "}"
            "QMenu::separator { "
            "   background-color: #E3F2FD; "
            "}";
    }

    setStyleSheet(styleSheet);
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
}

void MainWindow::createButtons()
{
    // Create button container
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    // Create buttons
    loginButton = new QPushButton(QIcon(":/icons/login.png"), tr("Login"), centralWidget);
    signUpButton = new QPushButton(QIcon(":/icons/register.png"), tr("Sign Up"), centralWidget);
    
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
