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
    QString getDetailedStorageInfo(const QString&);
};
#endif // MAINWINDOW_H
