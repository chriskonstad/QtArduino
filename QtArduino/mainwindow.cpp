#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QUdpSocket(this);  //Create an instance of the QUdpSocket class pointed to by socket
    socket->bind(51000, QUdpSocket::ShareAddress);  //make the socket listen on port 51000 and make it try to share the port with any other program the OS is running
    packetTimer = new QTimer(this); //Create an instance of the QTimer class pointed to by packetTimer
    packetTimer->setInterval(100);   //have it send a packet 10x a second
    connect(packetTimer, SIGNAL(timeout()), this, SLOT(sendPacket()));  //When the timer runs out, send a packet
    packetTimer->start();   //Start the timer
    connect(socket, SIGNAL(readyRead()), this, SLOT(onPacketReceived()));   //When a packet is received, call onPacketRecieved()
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPacketReceived()
{
    QString packet;
    QByteArray datagram;
    int digital;
    int analog;

    do
    {
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());
    }
    while(socket->hasPendingDatagrams());
    packet = (tr("\"%1\"").arg(datagram.data()));   //turn the datagram into a string
    packet.remove(QChar('"'), Qt::CaseInsensitive); //remove quotation marks

    QTextStream stream(&packet);    //create a textstream to parse the packet

    stream >> digital >> analog;    //parse the packet

    //Change the color of the QLineEdit
    if(digital%2 == 1)
        ui->leDigital->setStyleSheet("QLineEdit { background-color: rgb(136, 195, 240) }");
    else
        ui->leDigital->setStyleSheet("QLineEdit { background-color: none }");

    ui->leDigital->setText(QString::number(digital));

    if(digital <= 100)
        ui->barAnalog->setValue(digital); //slide the progress bar
    else
        QMessageBox::critical(0 ,
        "critical message" , "The value received is bigger than 100?",
        QMessageBox::Ok | QMessageBox::Default ,
        QMessageBox::Cancel | QMessageBox::Escape ,  0 );
}

void MainWindow::sendPacket()
{
    QString packet; //create the packet string to hold the values to send

    if(ui->pbDigital->isDown())
        packet.append(QString::number(1));
    else
        packet.append(QString::number(0));
    packet.append(" "); //deliminate the packet with whitespace
    packet.append(QString::number(ui->hsAnalog->value()));

    QByteArray datagram = packet.toAscii(); //convert into a datagram

    socket->writeDatagram(datagram, QHostAddress::Broadcast, 50000); //braodcast the packet on port 50000

    packetTimer->start();   //restart the timer
}
