#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

//Variáveis globais:
int RD = 0;
int RE = 0;
QString ATK = "";
int PWR = 0;
int PWM = 0;

int eixoRobo = 5; // Determinar a distancia (em cm) do eixo entre as duas rodas.
int raioEsquerdo = 2 ; // Raio da roda esquerda.
int raioDireito = 2; // Raio da roda direita.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enviarDados()
{


    if(wifiClient->status() == "Socket is not connected"){
         PWR = 0;
         ui->txtStatus->setText("Status: Desconectado.");
         ui->btnLigar->setText("Ligar");
         ui->btnLigar->setStyleSheet("QPushButton {background-color: #121212; border: 1px solid black;border-radius: 1px;color: green;}");

    }else{
           ui->txtStatus->setText("Status: Conectado.");
           if(PWR == 1){
               lerSensores();
           }else{
               RD = 0;
               RE = 0;
               ATK = "";
               ui->btnAttack->setStyleSheet("QPushButton {background-color: #121212; border: 1px solid black; border-radius: 1px; color: blue;}");
           }

           QString mensagem = (QString::number(RD) + ";" + QString::number(RE) + ";" +  ATK + "\n");
           this->wifiClient->writeData(mensagem.toStdString().c_str());
    }





}

void MainWindow::lerSensores()
{
     int acX = ac->getX()*100/9;
     int acY = ac->getY()*100/9;

     double Wd = ((2 * acX) + (acY * eixoRobo))/(2 * raioDireito);
     double We = ((2 * acX) - (acY * eixoRobo))/(2 * raioEsquerdo);

     RD = (Wd *  PWM)/100;
     RE = (We *  PWM)/100;

}

void MainWindow::conectar(QString ip, QString porta)
{
    qInfo()<<"Comecou";
    this->wifiClient = new Client(ip, porta.toInt());
    connect(this->wifiClient, SIGNAL(connectionSuccessful()),this, SLOT(connectionSucessful()));
}

void MainWindow::connectionSucessful()
{
        ui->txtStatus->setText("Status: Conectado.");
        ac = new AccelHandler(QAccelerometer::Combined, 200);
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()),this, SLOT(enviarDados()));
        timer->start(300);

}

void MainWindow::on_btnLigar_clicked()
{
    if(PWR == 1){
        PWR = 0;
        ui->btnLigar->setText("Ligar");
        ui->btnLigar->setStyleSheet("QPushButton {background-color: #121212; border: 1px solid black;border-radius: 1px;color: green;}");

    }else if(PWR == 0){
        PWR = 1;
        ui->btnLigar->setText("Desligar");
        ui->btnLigar->setStyleSheet("QPushButton {background-color: #121212;border: 1px solid black;border-radius: 1px;color: red;}");
    }

}

void MainWindow::on_btnAttack_clicked()
{
    if(ATK == "ligado"){
        ATK = "";
        ui->btnAttack->setStyleSheet("QPushButton {background-color: #121212; border: 1px solid black; border-radius: 1px; color: blue;}");
    }else{
        ATK = "ligado";
        ui->btnAttack->setStyleSheet("QPushButton {background-color: #121212; border: 1px solid black; border-radius: 1px; color: yellow;}");
    }
}

void MainWindow::on_Slider_valueChanged(int value)
{
    int val = ui->Slider->value();
    PWM = val;
    ui->labelPWM->setText(QString::number(val)+"%");
}

void MainWindow::on_btnSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btnVoltar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnConectar_clicked()
{
    conectar(ui->txtIP->text(), ui->txtPorta->text());
}
