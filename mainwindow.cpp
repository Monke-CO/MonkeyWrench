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
    ui->comboBox->hide();
    updateHddLabel(0);
    //setShadowEffect(ui->tabWidget);
    ui->cpuGifLabel->hide();
    ui->hddGifLabel->hide();
    ui->cpuLoading->hide();
    ui->hddLoading->hide();
    ui->tabWidget->setCurrentIndex(0);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doConnections()
{
    connect(ui->comboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this, &MainWindow::updateHddLabel);
    connect(ui->cpuButton,&QPushButton::clicked,this,&MainWindow::cpuBenchmarkClicked);
    connect(ui->hddButton,&QPushButton::clicked,this,&MainWindow::hddBenchmarkClicked);
    connect(ui->invisibleButton,&QPushButton::clicked,this,&MainWindow::invisibleButtonClicked);
}

void MainWindow::updateHddLabel(int index)
{
    ui->hddLabel->setText(getDetailedStorageInfo(QString::number(index)));
}

void MainWindow::invisibleButtonClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("NEVER FORGET");

    QVBoxLayout layout(&dialog);
    QLabel label(&dialog);
    layout.addWidget(&label);
    QPixmap pixmap(":/resources/harambe.jpg");
    label.setStyleSheet("font-size: 18px; "
                        "font-weight: bold;"
                        "color: #333333;"
                        "background-color: #EBD68E;"
                        "border: 2px solid #CCCCCC;"
                        "border-radius: 10px;"
                        "padding: 10px; ");
    label.setPixmap(pixmap);
    dialog.exec();
}

void MainWindow::cpuBenchmarkClicked()
{
    QMovie* gif = new QMovie(":/resources/monkey-spinning-holding-hands.gif");
    ui->cpuGifLabel->setMovie(gif);
    ui->cpuGifLabel->show();
    ui->cpuLoading->show();
    gif->start();

    auto* worker = new Benchmarking::cpuWorker();
    QThread *thread = new QThread();

    QObject::connect(thread, &QThread::started,[this, &worker](){
        QString result = worker->runBenchmark();
        QMetaObject::invokeMethod(ui->cpuScore,"setText",Qt::QueuedConnection,Q_ARG(QString,result));
    });

    gif->stop();
    ui->cpuGifLabel->hide();
    ui->cpuResult->show();
    ui->cpuLoading->hide();
}

void MainWindow::hddBenchmarkClicked()
{
    QMovie* gif = new QMovie(":/resources/monkey-spinning-holding-hands.gif");
    ui->hddGifLabel->setMovie(gif);
    ui->hddGifLabel->show();
    ui->hddLoading->show();
    gif->start();

    //simulate benchmark
    waitAsync(2000);
    gif->stop();
    ui->hddGifLabel->hide();
    ui->hddResult->show();
    ui->hddLoading->hide();
}
void MainWindow::setSystemInformationLabel()
{
    QString osInfo = getOsInfo();
    QString cpuInformation = getCPUInfo();
    ui->cpuInfoLabel->setText(cpuInformation);
    QString ramInfo = getRamInfo();
    QString cDrive = getStorageDevices("0");
    QString dDrive = getStorageDevices("1");

    QString result = osInfo + cpuInformation + ramInfo + "<br>" + cDrive + "<br>" + dDrive;
    ui->systemInfoLabel->setText(result);
}

void MainWindow::waitAsync(int milliseconds)
{
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow::setupComboBox()
{
    auto comboBox = ui->comboBox;
    auto listView = new QListView(comboBox);
    comboBox->setView(listView);
    comboBox->addItem(getStorageDevices("0"));
    comboBox->addItem(getStorageDevices("1"));
}

void MainWindow::setShadowEffect(QTabWidget* tabWidget)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(100);
    effect->setXOffset(100);
    effect->setYOffset(100);
    effect->setColor(Qt::black);

    tabWidget->setGraphicsEffect(effect);
}

QString MainWindow::getOsInfo()
{
    QString result;
    result += QSysInfo::prettyProductName();
    return result;
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
    QString command = "wmic cpu get Name, Manufacturer, NumberOfCores, NumberOfLogicalProcessors /format:list";

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

QString MainWindow::getDetailedStorageInfo(const QString& driveIndex) {
    QProcess process;
    QString command = "wmic diskdrive where Index=" + driveIndex + " get Caption,Description,Model,Size /value";

    process.start("cmd.exe", QStringList() << "/C" << command);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split("\n", Qt::SkipEmptyParts);

    QString deviceInfo;
    for (const QString& line : lines) {
        QStringList parts = line.split("=");
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();

            if (key == "Caption") {
                //deviceInfo += "Drive: " + value + "<br>";
            } else if (key == "Description") {
                deviceInfo += "Description: " + value + "<br>";
            } else if (key == "Model") {
                deviceInfo += "Model: " + value + "<br>";
            } else if (key == "Size") {
                auto bytes = value.toULongLong();
                bytes /= 1024*1024*1024;
                deviceInfo += "Size: " + QString::number(bytes) + " GB" + "<br>";
            }
        }
    }

    return deviceInfo;
}
