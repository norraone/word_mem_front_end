#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>

class FunctionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FunctionWidget(QWidget *parent = nullptr);
    ~FunctionWidget();

private slots:
    void onFunctionSelected(int index);
    void onCalculateClicked();

private:
    void setupUi();
    void createBasicMathPage();
    void createAdvancedMathPage();
    void createStatisticsPage();

    QVBoxLayout *mainLayout;
    QComboBox *functionSelector;
    QStackedWidget *functionPages;
    QPushButton *calculateButton;
    QLabel *resultLabel;

    // Input fields for different functions
    QLineEdit *basicNum1;
    QLineEdit *basicNum2;
    QComboBox *basicOperator;

    QLineEdit *advancedNum;
    QComboBox *advancedFunction;

    QLineEdit *statsNumbers;
    QComboBox *statsFunction;
};

#endif // FUNCTIONWIDGET_H
