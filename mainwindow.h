#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QPixmap>
#include <windows.h>
#include <QProcess>
#include <QComboBox>
#include <QListView>
#include <QPainter>
#include <QProxyStyle>
#include <QStylePainter>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QTimer>
#include <QMovie>
#include <QDialog>
#include "cpuworker.h"
#include "hddworker.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void setTabBackground();
    void setIcons();
    void setSystemInformationLabel();
    QString getStorageDevices(const QString&);
    QString getCPUInfo();
    QString getRamInfo();
    void doConnections();
    void setupComboBox();
    void updateHddLabel(int);
    QString getOsInfo();
    QString getDetailedStorageInfo(const QString&);
    void setShadowEffect(QTabWidget*);
    void waitAsync(int);
    void cpuBenchmarkClicked();
    void hddBenchmarkClicked();
    void invisibleButtonClicked();
};

#endif // MAINWINDOW_H
