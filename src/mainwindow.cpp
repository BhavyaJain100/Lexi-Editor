#include "mainwindow.h"
#include <QHeaderView>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *label = new QLabel("Enter C code:", this);
    layout->addWidget(label);

    codeEditor = new QPlainTextEdit(this);
    layout->addWidget(codeEditor);

    // Buttons layout: Analyze, Upload, Clear
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    analyzeButton = new QPushButton("Analyze", this);
    buttonLayout->addWidget(analyzeButton);

    QPushButton *uploadButton = new QPushButton("Upload Code", this);
    buttonLayout->addWidget(uploadButton);

    QPushButton *clearButton = new QPushButton("Clear", this);
    buttonLayout->addWidget(clearButton);

    layout->addLayout(buttonLayout);

    tokenTable = new QTableWidget(this);
    tokenTable->setColumnCount(3);
    tokenTable->setHorizontalHeaderLabels({"Token", "Category", "Subcategory"});
    tokenTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tokenTable);

    setCentralWidget(centralWidget);

    connect(analyzeButton, &QPushButton::clicked, this, &MainWindow::analyzeCode);
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadCode);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearCode);
}

void MainWindow::analyzeCode() {
    QString input = codeEditor->toPlainText();
    LexicalAnalyzer lexer;
    allTokens = lexer.analyze(input.toStdString());

    displayTokens(allTokens);
}

void MainWindow::displayTokens(const std::vector<std::pair<std::string, std::string>> &tokens) {
    tokenTable->setRowCount(0);

    for (const auto &[token, category] : tokens) {
        QString qToken = QString::fromStdString(token);
        QString qCategory = QString::fromStdString(category);

        int row = tokenTable->rowCount();
        tokenTable->insertRow(row);

        QTableWidgetItem *tokenItem = new QTableWidgetItem(qToken);
        QTableWidgetItem *categoryItem = new QTableWidgetItem;
        QTableWidgetItem *subCategoryItem = new QTableWidgetItem;

        // Split category string "MainCategory:Subcategory"
        QStringList parts = qCategory.split(":");
        QString mainCategory = parts.value(0).trimmed();
        QString subCategory = parts.size() > 1 ? parts.value(1).trimmed() : "";

        categoryItem->setText(mainCategory);
        subCategoryItem->setText(subCategory);

        tokenTable->setItem(row, 0, tokenItem);
        tokenTable->setItem(row, 1, categoryItem);
        tokenTable->setItem(row, 2, subCategoryItem);
    }
}

void MainWindow::uploadCode() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Code File", "", "Code Files (*.cpp *.h *.txt);;All Files (*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open file!");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    codeEditor->setPlainText(content);
}

void MainWindow::clearCode() {
    codeEditor->clear();
    tokenTable->clearContents();
    tokenTable->setRowCount(0);
}
