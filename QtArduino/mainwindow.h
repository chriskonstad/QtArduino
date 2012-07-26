#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
