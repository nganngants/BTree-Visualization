#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include "renderarea.h"
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static bool isNum(const QString& st);
    static int convertToNum (QString st);
    static QString convertToString(int num);
private slots:
    void on_searchButton_clicked();

    void on_insButton_clicked();

    void on_revButton_clicked();

    void on_clrButton_clicked();

    void on_printButton_clicked();

    void on_maxDegOpt_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    //QLabel* forPrintTree;
    QVBoxLayout* mainLayout;
    RenderArea* renderArea;
    QWidget* centralWidget;
    QHBoxLayout* buttonLayout;
};
#endif // MAINWINDOW_H
