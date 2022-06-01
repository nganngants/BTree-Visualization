#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1500,800);
    ui->textbox->setMaxLength(4);
    ui->textbox->setText(QString());

    ui->forPrintTree->setWordWrap(true);
    QFont f( "Nirmala UI", 16);
    ui->forPrintTree->setFont(f);
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(ui->enter);
    buttonLayout->addWidget(ui->textbox);
    buttonLayout->addWidget(ui->insButton);
    buttonLayout->addWidget(ui->revButton);
    buttonLayout->addWidget(ui->searchButton);
    buttonLayout->addSpacing(105);
    buttonLayout->addWidget(ui->printButton);
    buttonLayout->addWidget(ui->clrButton);
    buttonLayout->addSpacing(450);
    buttonLayout->addWidget(ui->maxdeg);
    buttonLayout->addWidget(ui->maxDegOpt);

    renderArea = new RenderArea(this);
    renderArea->setVisible(true);
    renderArea->setMinimumWidth(5000);
    renderArea->setMinimumHeight(5000);
    renderArea->setUpdatesEnabled(true);
    renderArea->SetX(50 + renderArea->geometry().x());
    renderArea->SetY(renderArea->geometry().y() + 10);


    ui->maxDegOpt->addItem("3");
    ui->maxDegOpt->addItem("5");
    ui->maxDegOpt->addItem("7");
    ui->maxDegOpt->setCurrentIndex(0);

    ui->scrollArea->setWidget(renderArea);
    ui->scrollArea->installEventFilter(renderArea);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(ui->scrollArea);
    mainLayout->addWidget(ui->forPrintTree);
    mainLayout->addLayout(buttonLayout,200);
    mainLayout->addWidget(ui->history);

    centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete renderArea;
    delete buttonLayout;
    delete centralWidget;
}


void MainWindow::on_searchButton_clicked()
{
    QString num = ui->textbox->text();
    if (num.size() == 0) return;
    ui->textbox->setText("");
    bool isValid = isNum(num);
    if (!isValid)
    {
        QMessageBox::critical(this,"Invalid Input","You must enter a non-negative integer less than 10000",
                              QMessageBox::StandardButton::Ok);
        return;
    }
    ui->forPrintTree->setText("");
    ui->history->setText(">>>Searching key " + num + "...");
    QApplication::processEvents();
    bool succ = renderArea->Search(convertToNum(num));
    if (succ) ui->history->setText(">>>Key " + num + " was found");
    else ui->history->setText(">>>Key " + num + " does not exist in the tree");
}


void MainWindow::on_insButton_clicked()
{
    QString num = ui->textbox->text();
    if (num.size() == 0) return;
    bool isValid = isNum(num);
    ui->textbox->setText("");
    if (!isValid)
    {
        QMessageBox::critical(this,"Invalid Input","You must enter a non-negative integer less than 10000",
                              QMessageBox::StandardButton::Ok);
        return;
    }
    ui->forPrintTree->setText("");
    ui->history->setText(">>>Inserting key " + num + "...");

    QApplication::processEvents();
    renderArea->Insert(convertToNum(num));
    ui->history->setText(">>>Key " + num + " was inserted");
}


void MainWindow::on_revButton_clicked()
{
    QString num = ui->textbox->text();
    if (num.size() == 0) return;
    ui->textbox->setText("");
    bool isValid = isNum(num);
    if (!isValid)
    {
        QMessageBox::critical(this,"Invalid Input","You must enter a non-negative integer less than 10000",
                              QMessageBox::StandardButton::Ok);
        return;
    }
    ui->forPrintTree->setText("");
    ui->history->setText(">>>Removing key " + num + "...");
    QApplication::processEvents();
    bool succ = renderArea->Remove(convertToNum(num));
    if (succ) ui->history->setText(">>>Key " + num + " was removed");
    else ui->history->setText(">>>Key " + num + " does not exist");
}


void MainWindow::on_clrButton_clicked()
{
    ui->forPrintTree->setText("");
    renderArea->Clear();
    ui->history->setText(">>>Screen Cleared");
}

bool MainWindow::isNum(const QString& st)
{
    for (int i = 0;i < sz(st); ++i)
        if (!st[i].isDigit()) return false;
    return true;
}

int MainWindow::convertToNum(QString st)
{
    int res = 0;
    for (int i=0;i<sz(st);++i)
        res = res * 10 + (st[i].digitValue());
    return res;
}

QString MainWindow::convertToString(int x)
{
    QString res = "";
    while (x)
    {
        res = char(x % 10 + '0') + res;
        x /= 10;
    }
    return res;
}


void MainWindow::on_printButton_clicked()
{
    QString printTree = renderArea->Print();
    if (printTree.size() == 0)
        ui->history->setText(">>>The tree is empty. Nothing to print");
    else
    {
        ui->history->setText(">>>B-Tree was printed");
        ui->forPrintTree->setText(printTree);
    }
}

void MainWindow::on_maxDegOpt_currentTextChanged(const QString &arg1)
{
    if (renderArea == NULL) return;
    int curMaxDeg = convertToNum(ui->maxDegOpt->currentText());
    ui->history->setText(">>>Initialize B-Tree with max degree " + ui->maxDegOpt->currentText());
    renderArea->SetMaxDeg(curMaxDeg);
    renderArea->repaint();
}
