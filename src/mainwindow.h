#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QVBoxLayout>

#include "lexicalanalyzer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void analyzeCode();
    void uploadCode();
    void clearCode();

private:
    QPlainTextEdit *codeEditor;
    QPushButton *analyzeButton;
    QPushButton *uploadButton;      // new button
    QPushButton *clearButton;       // new button
    QTableWidget *tokenTable;

    std::vector<std::pair<std::string, std::string>> allTokens;

    void setupUI();
    void displayTokens(const std::vector<std::pair<std::string, std::string>> &tokens);

};

#endif // MAINWINDOW_H
