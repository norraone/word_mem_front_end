#include "wordwidget.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>

WordWidget::WordWidget(QWidget *parent)
    : QWidget(parent)
    , currentWordIndex(-1)
    , answerShown(false)
    , totalReviews(0)
    , knownCount(0)
    , unknownCount(0)
{
    setMinimumSize(800, 600);  // Set minimum window size
    resize(1024, 768);  // Set default window size
    setupUi();
    loadWords();
}

WordWidget::~WordWidget()
{
    saveWords();
}

void WordWidget::setupUi()
{
    mainLayout = new QVBoxLayout(this);
    
    // Create mode selector
    modeSelector = new QComboBox(this);
    modeSelector->addItem("Add New Words");
    modeSelector->addItem("Review Words");
    mainLayout->addWidget(modeSelector);

    // Create stacked widget
    pages = new QStackedWidget(this);
    mainLayout->addWidget(pages);

    // Create pages
    createAddPage();
    createReviewPage();

    // Connect signals
    connect(modeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WordWidget::onModeChanged);

    // Set initial page
    pages->setCurrentIndex(0);
}

void WordWidget::createAddPage()
{
    addPage = new QWidget(this);
    QGridLayout *layout = new QGridLayout(addPage);

    wordInput = new QLineEdit(addPage);
    meaningInput = new QLineEdit(addPage);
    noteInput = new QLineEdit(addPage);
    addWordButton = new QPushButton("Add Word", addPage);

    layout->addWidget(new QLabel("Word:"), 0, 0);
    layout->addWidget(wordInput, 0, 1);
    layout->addWidget(new QLabel("Meaning:"), 1, 0);
    layout->addWidget(meaningInput, 1, 1);
    layout->addWidget(new QLabel("Note:"), 2, 0);
    layout->addWidget(noteInput, 2, 1);
    layout->addWidget(addWordButton, 3, 0, 1, 2);

    connect(addWordButton, &QPushButton::clicked, this, &WordWidget::onAddWordClicked);

    pages->addWidget(addPage);
}

void WordWidget::createReviewPage()
{
    reviewPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(reviewPage);

    wordLabel = new QLabel(reviewPage);
    meaningLabel = new QLabel(reviewPage);
    noteLabel = new QLabel(reviewPage);
    showAnswerButton = new QPushButton("Show Answer", reviewPage);
    knowButton = new QPushButton("I Know This", reviewPage);
    dontKnowButton = new QPushButton("Don't Know", reviewPage);
    statsLabel = new QLabel(reviewPage);

    // Style the labels
    QFont font;
    font.setPointSize(16);
    wordLabel->setFont(font);
    wordLabel->setAlignment(Qt::AlignCenter);
    meaningLabel->setFont(font);
    meaningLabel->setAlignment(Qt::AlignCenter);
    noteLabel->setAlignment(Qt::AlignCenter);
    statsLabel->setAlignment(Qt::AlignCenter);

    // Hide answer-related widgets initially
    meaningLabel->hide();
    noteLabel->hide();
    knowButton->hide();
    dontKnowButton->hide();

    layout->addWidget(wordLabel);
    layout->addWidget(meaningLabel);
    layout->addWidget(noteLabel);
    layout->addWidget(showAnswerButton);
    layout->addWidget(knowButton);
    layout->addWidget(dontKnowButton);
    layout->addSpacing(20);
    layout->addWidget(statsLabel);

    connect(showAnswerButton, &QPushButton::clicked, this, &WordWidget::onShowAnswerClicked);
    connect(knowButton, &QPushButton::clicked, this, &WordWidget::onKnowClicked);
    connect(dontKnowButton, &QPushButton::clicked, this, &WordWidget::onDontKnowClicked);

    pages->addWidget(reviewPage);
}

void WordWidget::onAddWordClicked()
{
    QString word = wordInput->text().trimmed();
    QString meaning = meaningInput->text().trimmed();
    QString note = noteInput->text().trimmed();

    if (word.isEmpty() || meaning.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both word and meaning");
        return;
    }

    WordCard newCard;
    newCard.word = word;
    newCard.meaning = meaning;
    newCard.note = note;
    newCard.level = 0;
    newCard.lastReview = QDate::currentDate();

    words.append(newCard);
    saveWords();

    // Clear inputs
    wordInput->clear();
    meaningInput->clear();
    noteInput->clear();
    wordInput->setFocus();

    QMessageBox::information(this, "Success", "Word added successfully!");
}

void WordWidget::onModeChanged(int index)
{
    pages->setCurrentIndex(index);
    if (index == 1) { // Review mode
        reviewQueue.clear();
        QDate today = QDate::currentDate();
        
        // Add words due for review to queue
        for (int i = 0; i < words.size(); ++i) {
            int daysUntilReview;
            switch (words[i].level) {
                case 0: daysUntilReview = 0; break;
                case 1: daysUntilReview = 1; break;
                case 2: daysUntilReview = 3; break;
                case 3: daysUntilReview = 7; break;
                case 4: daysUntilReview = 14; break;
                default: daysUntilReview = 30; break;
            }
            
            if (words[i].lastReview.addDays(daysUntilReview) <= today) {
                reviewQueue.append(&words[i]);
            }
        }

        if (reviewQueue.isEmpty()) {
            QMessageBox::information(this, "Review Complete", "No words to review today!");
            modeSelector->setCurrentIndex(0);
            return;
        }

        // Start review
        currentWordIndex = 0;
        nextWord();
    }
}

void WordWidget::onShowAnswerClicked()
{
    if (currentWordIndex >= 0 && currentWordIndex < reviewQueue.size()) {
        meaningLabel->show();
        noteLabel->show();
        knowButton->show();
        dontKnowButton->show();
        showAnswerButton->hide();
        answerShown = true;
    }
}

void WordWidget::onKnowClicked()
{
    if (currentWordIndex >= 0 && currentWordIndex < reviewQueue.size()) {
        WordCard *card = reviewQueue[currentWordIndex];
        card->level = qMin(card->level + 1, 5);
        card->lastReview = QDate::currentDate();
        knownCount++;
        totalReviews++;
        saveWords();
        nextWord();
    }
}

void WordWidget::onDontKnowClicked()
{
    if (currentWordIndex >= 0 && currentWordIndex < reviewQueue.size()) {
        WordCard *card = reviewQueue[currentWordIndex];
        card->level = qMax(card->level - 1, 0);
        card->lastReview = QDate::currentDate();
        unknownCount++;
        totalReviews++;
        saveWords();
        nextWord();
    }
}

void WordWidget::nextWord()
{
    currentWordIndex++;
    if (currentWordIndex >= reviewQueue.size()) {
        QMessageBox::information(this, "Review Complete", "You've completed all words for today!");
        modeSelector->setCurrentIndex(0);
        return;
    }

    WordCard *card = reviewQueue[currentWordIndex];
    wordLabel->setText(card->word);
    meaningLabel->setText(card->meaning);
    noteLabel->setText(card->note.isEmpty() ? "" : "Note: " + card->note);

    meaningLabel->hide();
    noteLabel->hide();
    knowButton->hide();
    dontKnowButton->hide();
    showAnswerButton->show();
    answerShown = false;

    updateStats();
}

void WordWidget::updateStats()
{
    if (totalReviews > 0) {
        double knownPercent = (knownCount * 100.0) / totalReviews;
        statsLabel->setText(QString("Total Reviews: %1 | Success Rate: %2%")
                          .arg(totalReviews)
                          .arg(knownPercent, 0, 'f', 1));
    } else {
        statsLabel->setText("No reviews yet");
    }
}

void WordWidget::loadWords()
{
    QFile file("words.json");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray array = doc.array();

    words.clear();
    for (const QJsonValue &value : array) {
        QJsonObject obj = value.toObject();
        WordCard card;
        card.word = obj["word"].toString();
        card.meaning = obj["meaning"].toString();
        card.note = obj["note"].toString();
        card.level = obj["level"].toInt();
        card.lastReview = QDate::fromString(obj["lastReview"].toString(), Qt::ISODate);
        words.append(card);
    }

    file.close();
}

void WordWidget::saveWords()
{
    QFile file("words.json");
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Could not save words to file");
        return;
    }

    QJsonArray array;
    for (const WordCard &card : words) {
        QJsonObject obj;
        obj["word"] = card.word;
        obj["meaning"] = card.meaning;
        obj["note"] = card.note;
        obj["level"] = card.level;
        obj["lastReview"] = card.lastReview.toString(Qt::ISODate);
        array.append(obj);
    }

    QJsonDocument doc(array);
    file.write(doc.toJson());
    file.close();
}
