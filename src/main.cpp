#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("LexiEditor - Lexical Analyzer");
    w.show();
    return a.exec();
}