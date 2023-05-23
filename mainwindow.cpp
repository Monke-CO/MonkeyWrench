#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Benchmarks/include/benchmark/hdd/hddReadWriteBenchmark.h"
#include "Benchmarks/include/timer/Timer.h"
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    button = new QPushButton("Update", this);
    lineEdit = new QLineEdit(this);

    connect(button, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    auto hdd = new benchmark::hdd::HDDWriteSpeedBenchmark;
    auto timer = new timer::Timer;

    hdd->setMaxIndex(2);
    hdd->setMinIndex(0);
    hdd->setPrefix("data-files/data");
    hdd->setSuffix(".dat");
    timer->start();
    QFuture<void> future1 = QtConcurrent::run([&hdd]() {hdd->run("fs");});
    future1.waitForFinished();
    double test = timer->stop().count();
    lineEdit->setText(QString::number(test));
}

