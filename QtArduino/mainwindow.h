#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Written by Chris Konstad 7/26/2012
//Released into the public domain

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void onPacketReceived();
    void sendPacket();

private:
    Ui::MainWindow *ui;
    QUdpSocket *socket;
    QTimer *packetTimer;
};

#endif // MAINWINDOW_H
