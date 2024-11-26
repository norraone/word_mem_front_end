#include "functionwidget.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QMessageBox>
#include <cmath>
#include <QStringList>

FunctionWidget::FunctionWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

FunctionWidget::~FunctionWidget()
{
}

void FunctionWidget::setupUi()
{
    mainLayout = new QVBoxLayout(this);
    
    // Create function selector
    functionSelector = new QComboBox(this);
    functionSelector->addItem("Basic Math");
    functionSelector->addItem("Advanced Math");
    functionSelector->addItem("Statistics");
    mainLayout->addWidget(functionSelector);

    // Create stacked widget for different function pages
    functionPages = new QStackedWidget(this);
    mainLayout->addWidget(functionPages);

    // Create different pages
    createBasicMathPage();
    createAdvancedMathPage();
    createStatisticsPage();

    // Create calculate button
    calculateButton = new QPushButton("Calculate", this);
    mainLayout->addWidget(calculateButton);

    // Create result label
    resultLabel = new QLabel(this);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet("font-size: 16px; color: #4682B4;");
    mainLayout->addWidget(resultLabel);

    // Connect signals
    connect(functionSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FunctionWidget::onFunctionSelected);
    connect(calculateButton, &QPushButton::clicked,
            this, &FunctionWidget::onCalculateClicked);

    // Set initial page
    functionPages->setCurrentIndex(0);
}

void FunctionWidget::createBasicMathPage()
{
    QWidget *page = new QWidget(this);
    QGridLayout *layout = new QGridLayout(page);

    basicNum1 = new QLineEdit(page);
    basicNum2 = new QLineEdit(page);
    basicOperator = new QComboBox(page);

    basicOperator->addItems(QStringList() << "+" << "-" << "*" << "/");

    layout->addWidget(new QLabel("Number 1:"), 0, 0);
    layout->addWidget(basicNum1, 0, 1);
    layout->addWidget(new QLabel("Operation:"), 1, 0);
    layout->addWidget(basicOperator, 1, 1);
    layout->addWidget(new QLabel("Number 2:"), 2, 0);
    layout->addWidget(basicNum2, 2, 1);

    functionPages->addWidget(page);
}

void FunctionWidget::createAdvancedMathPage()
{
    QWidget *page = new QWidget(this);
    QGridLayout *layout = new QGridLayout(page);

    advancedNum = new QLineEdit(page);
    advancedFunction = new QComboBox(page);

    advancedFunction->addItems(QStringList() << "Square" << "Square Root" << "Cube" << "Sine" << "Cosine");

    layout->addWidget(new QLabel("Number:"), 0, 0);
    layout->addWidget(advancedNum, 0, 1);
    layout->addWidget(new QLabel("Function:"), 1, 0);
    layout->addWidget(advancedFunction, 1, 1);

    functionPages->addWidget(page);
}

void FunctionWidget::createStatisticsPage()
{
    QWidget *page = new QWidget(this);
    QGridLayout *layout = new QGridLayout(page);

    statsNumbers = new QLineEdit(page);
    statsFunction = new QComboBox(page);

    statsFunction->addItems(QStringList() << "Average" << "Maximum" << "Minimum" << "Sum");

    layout->addWidget(new QLabel("Numbers (comma-separated):"), 0, 0);
    layout->addWidget(statsNumbers, 0, 1);
    layout->addWidget(new QLabel("Function:"), 1, 0);
    layout->addWidget(statsFunction, 1, 1);

    functionPages->addWidget(page);
}

void FunctionWidget::onFunctionSelected(int index)
{
    functionPages->setCurrentIndex(index);
    resultLabel->clear();
}

void FunctionWidget::onCalculateClicked()
{
    bool ok1, ok2;
    QString result;

    switch (functionPages->currentIndex()) {
        case 0: { // Basic Math
            double num1 = basicNum1->text().toDouble(&ok1);
            double num2 = basicNum2->text().toDouble(&ok2);

            if (!ok1 || !ok2) {
                QMessageBox::warning(this, "Error", "Please enter valid numbers");
                return;
            }

            switch (basicOperator->currentIndex()) {
                case 0: result = QString::number(num1 + num2); break;
                case 1: result = QString::number(num1 - num2); break;
                case 2: result = QString::number(num1 * num2); break;
                case 3: 
                    if (num2 == 0) {
                        QMessageBox::warning(this, "Error", "Cannot divide by zero");
                        return;
                    }
                    result = QString::number(num1 / num2);
                    break;
            }
            break;
        }

        case 1: { // Advanced Math
            double num = advancedNum->text().toDouble(&ok1);
            if (!ok1) {
                QMessageBox::warning(this, "Error", "Please enter a valid number");
                return;
            }

            switch (advancedFunction->currentIndex()) {
                case 0: result = QString::number(num * num); break;
                case 1: 
                    if (num < 0) {
                        QMessageBox::warning(this, "Error", "Cannot calculate square root of negative number");
                        return;
                    }
                    result = QString::number(sqrt(num));
                    break;
                case 2: result = QString::number(num * num * num); break;
                case 3: result = QString::number(sin(num)); break;
                case 4: result = QString::number(cos(num)); break;
            }
            break;
        }

        case 2: { // Statistics
            QStringList numbers = statsNumbers->text().split(',', Qt::SkipEmptyParts);
            QVector<double> nums;

            for (const QString &str : numbers) {
                double num = str.trimmed().toDouble(&ok1);
                if (!ok1) {
                    QMessageBox::warning(this, "Error", "Please enter valid numbers separated by commas");
                    return;
                }
                nums.append(num);
            }

            if (nums.isEmpty()) {
                QMessageBox::warning(this, "Error", "Please enter at least one number");
                return;
            }

            switch (statsFunction->currentIndex()) {
                case 0: { // Average
                    double sum = 0;
                    for (double num : nums) sum += num;
                    result = QString::number(sum / nums.size());
                    break;
                }
                case 1: { // Maximum
                    double max = nums[0];
                    for (double num : nums) if (num > max) max = num;
                    result = QString::number(max);
                    break;
                }
                case 2: { // Minimum
                    double min = nums[0];
                    for (double num : nums) if (num < min) min = num;
                    result = QString::number(min);
                    break;
                }
                case 3: { // Sum
                    double sum = 0;
                    for (double num : nums) sum += num;
                    result = QString::number(sum);
                    break;
                }
            }
            break;
        }
    }

    resultLabel->setText("Result: " + result);
}
