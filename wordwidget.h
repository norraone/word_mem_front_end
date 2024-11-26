#ifndef WORDWIDGET_H
#define WORDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QComboBox>
#include <QTimer>
#include <QVector>
#include <QPair>
#include <QDate>

class WordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WordWidget(QWidget *parent = nullptr);
    ~WordWidget();

private slots:
    void onAddWordClicked();
    void onShowAnswerClicked();
    void onKnowClicked();
    void onDontKnowClicked();
    void onModeChanged(int index);

private:
    void setupUi();
    void createAddPage();
    void createReviewPage();
    void loadWords();
    void saveWords();
    void nextWord();
    void updateStats();

    // UI Elements
    QVBoxLayout *mainLayout;
    QComboBox *modeSelector;
    QStackedWidget *pages;

    // Add Word Page
    QWidget *addPage;
    QLineEdit *wordInput;
    QLineEdit *meaningInput;
    QLineEdit *noteInput;
    QPushButton *addWordButton;

    // Review Page
    QWidget *reviewPage;
    QLabel *wordLabel;
    QLabel *meaningLabel;
    QLabel *noteLabel;
    QPushButton *showAnswerButton;
    QPushButton *knowButton;
    QPushButton *dontKnowButton;
    QLabel *statsLabel;

    // Data
    struct WordCard {
        QString word;
        QString meaning;
        QString note;
        int level;      // 0-5, higher means better known
        QDate lastReview;
    };
    QVector<WordCard> words;
    QVector<WordCard*> reviewQueue;
    int currentWordIndex;
    bool answerShown;

    // Statistics
    int totalReviews;
    int knownCount;
    int unknownCount;
};

#endif // WORDWIDGET_H
