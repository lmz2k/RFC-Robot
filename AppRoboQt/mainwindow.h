#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <string>
#include "accelhandler.h"

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
    void enviarDados();
    void lerSensores();
    void conectar(QString ip, QString porta);
    void connectionSucessful();

    void on_btnLigar_clicked();

    void on_btnAttack_clicked();

    void on_Slider_valueChanged(int value);

    void on_btnSettings_clicked();

    void on_btnVoltar_clicked();

    void on_btnConectar_clicked();

private:
    Ui::MainWindow *ui;
    Client *wifiClient;
    AccelHandler *ac;
    QTimer *timer;

};

#endif // MAINWINDOW_H
