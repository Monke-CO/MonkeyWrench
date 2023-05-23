#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setSystemInformationLabel();
    setupComboBox();
    doConnections();
    updateHddLabel(0);
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doConnections()
{
    connect(ui->comboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this, &MainWindow::updateHddLabel);
}

void MainWindow::updateHddLabel(int index)
{
    ui->hddLabel->setText(getStorageDevices(QString::number(index)));
}
void MainWindow::setSystemInformationLabel()
{
    QString cpuInformation = getCPUInfo();
    ui->cpuInfoLabel->setText(cpuInformation);
    QString ramInfo = getRamInfo();
    QString cDrive = getStorageDevices("0");
    QString dDrive = getStorageDevices("1");

    QString result = cpuInformation +"<br>" + ramInfo + "<br>" + cDrive + "<br>" + dDrive;
    ui->systemInfoLabel->setText(result);
}

void MainWindow::setupComboBox()
{
    auto comboBox = ui->comboBox;
    auto listView = new QListView(comboBox);
    comboBox->setView(listView);
    comboBox->addItem(getStorageDevices("0"));
    comboBox->addItem(getStorageDevices("1"));
}

QString MainWindow::getRamInfo()
{
    QProcess process;
    QString command = "wmic ComputerSystem get TotalPhysicalMemory";

    process.start("cmd.exe", QStringList() << "/C" << command);
    process.waitForFinished();

    QByteArray output = process.readAllStandardOutput();
    QString ramInfo = QString::fromLocal8Bit(output);

    // Extract the RAM size from the output
    QStringList lines = ramInfo.split('\n', Qt::SkipEmptyParts);
    if (lines.size() >= 2) {
        QString ramSizeStr = lines[1].trimmed(); // Extract the second line

        // Convert RAM size from bytes to gigabytes
        qlonglong ramSizeBytes = ramSizeStr.toLongLong();
        double ramSizeGB = ramSizeBytes / (1024.0 * 1024.0 * 1024.0);

        ramInfo = "Total RAM Size: " + QString::number(ramSizeGB, 'f', 2) + " GB";
    } else {
        ramInfo = "Failed to retrieve RAM information.";
    }

    return ramInfo;
}

QString MainWindow::getCPUInfo()
{
    QString cpuInformation;

    QProcess process;
    QString command = "wmic cpu get Name, Manufacturer, MaxClockSpeed, NumberOfCores, NumberOfLogicalProcessors /format:list";

    process.start("cmd.exe", QStringList() << "/C" << command);
    process.waitForFinished(-1);

    if (process.exitCode() == QProcess::NormalExit) {
        QByteArray output = process.readAllStandardOutput();
        output.replace("\n","<br>");
        cpuInformation = QString::fromLocal8Bit(output).trimmed();
    } else {
        cpuInformation = "Failed to retrieve CPU information.";
    }

    return cpuInformation;
}

QString MainWindow::getStorageDevices(const QString& driveLetter) {
    QProcess process;
    QString command = "wmic diskdrive where Index='" + driveLetter + "' get Model,Size /value";

    process.start("cmd.exe", QStringList() << "/C" << command);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();

    // Extract the model and size from the output
    QString deviceModel;
    QString deviceSize;
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        QStringList parts = line.split("=");
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            if (key == "Model")
                deviceModel = value;
            else if (key == "Size") {
                // Convert the size to GB
                double sizeBytes = value.toDouble();
                double sizeGB = sizeBytes / (1024 * 1024 * 1024);
                deviceSize = QString::number(sizeGB, 'f', 2) + " GB";
            }
        }
    }

    QString deviceInfo = deviceModel + " (" + deviceSize + ")";
    return deviceInfo;
}
