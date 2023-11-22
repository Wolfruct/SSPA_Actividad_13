#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <iostream>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QtMath>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame_3->hide();
    ventana.setScene(&escena1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->generar_texto();
}


void MainWindow::generar_texto()
{
    ui->plainTextEdit->clear();

    if(lista.get_lista_beg() != NULL)
    {
        neurona *aux = lista.get_lista_beg();
        while(aux != NULL)
        {
            QString aux_qstr = QString("ID: %1\nVoltaje: %2\n Posicion x: %3\nPosicion y: %4\nRed: %5\nGreen: %6\nBlue: %7\n---------------").arg(aux->get_id()).arg(aux->get_voltaje()).arg(aux->get_posicion_x()).arg(aux->get_posicion_y()).arg(aux->get_red()).arg(aux->get_green()).arg(aux->get_blue());
            ui->plainTextEdit->appendPlainText(aux_qstr);
            aux = aux->get_sig_neurona();
        }
    }
}

void MainWindow::obtener_datos(int indicador)
{
    neurona *aux = new neurona();

    aux->set_all(ui->spinBox->value(),ui->doubleSpinBox->value(),ui->spinBox_2->value(),ui->spinBox_3->value(),ui->spinBox_4->value(),ui->spinBox_5->value(),ui->spinBox_6->value());

    if(indicador == 0)
        lista.agregar_inicio(aux);
    if(indicador == 1)
        lista.agregar_final(aux);

}


void MainWindow::on_pushButton_2_clicked()
{
    obtener_datos(0);
}


void MainWindow::on_pushButton_3_clicked()
{
    obtener_datos(1);
}


void MainWindow::on_actionGuardar_triggered()
{
    QString direccion = QFileDialog::getSaveFileName(this, tr("Guardar Neuronas"),"",tr("Text files (*.txt)"));

    if (!direccion.isEmpty())
    {
        QFile archivo_guardar(direccion);

        if (archivo_guardar.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream imprimir(&archivo_guardar);
            if(lista.get_lista_beg() != NULL)
            {
                neurona *aux = lista.get_lista_beg();
                while(aux != NULL)
                {
                    QString aux_qstr = QString("%1:%2:%3:%4:%5:%6:%7").arg(aux->get_id()).arg(aux->get_voltaje()).arg(aux->get_posicion_x()).arg(aux->get_posicion_y()).arg(aux->get_red()).arg(aux->get_green()).arg(aux->get_blue());
                    imprimir << aux_qstr << '\n';
                    aux = aux->get_sig_neurona();
                }
            }
            archivo_guardar.close();
        }
    }
}


void MainWindow::on_actionRecuperar_triggered()
{
    QString direccion = QFileDialog::getOpenFileName(this, tr("Abrir Neuronas"),"",tr("Text files (*.txt)"));

    if (!direccion.isEmpty())
    {
        QFile archivo_leer(direccion);

        if (archivo_leer.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream leer(&archivo_leer);

            while (!leer.atEnd())
            {
                QString linea = leer.readLine();
                QStringList lista_valores =  linea.split(":");
                int id = lista_valores[0].toInt();
                float voltaje = lista_valores[1].toFloat();
                int posicionX = lista_valores[2].toInt();
                int posicionY = lista_valores[3].toInt();
                int red = lista_valores[4].toInt();
                int green = lista_valores[5].toInt();
                int blue = lista_valores[6].toInt();

                neurona *aux = new neurona();
                aux->set_all(id,voltaje,posicionX,posicionY,red,green,blue);
                lista.agregar_final(aux);
            }

            archivo_leer.close();
        }

    }
}


void MainWindow::on_actionBuscar_triggered()
{
    ui->frame->hide();
    ui->frame_2->hide();
    ui->frame_3->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->label_11->setText("");
    neurona *aux = lista.get_lista_beg();
    if(aux != NULL)
    {
        bool exito;
        int id = ui->lineEdit->text().toInt(&exito);

        if(!exito)
        {
            qDebug() << "Ingrese un número entero";
        }

        exito = true;

        while(aux != NULL && exito)
        {
            cout << id <<endl;
            int id_aux = aux->get_id();
            cout << id_aux <<endl;
            if ( id_aux == id)
            {
                exito = false;
            }
            else
            {
                aux = aux->get_sig_neurona();
            }
        }

        if(!exito)
        {
            QString id_qs = QString("%1").arg(aux->get_id());
            QString voltaje_qs = QString("%1").arg(aux->get_voltaje());
            QString pos_x_qs = QString("%1").arg(aux->get_posicion_x());
            QString pos_y_qs = QString("%1").arg(aux->get_posicion_y());
            QString red_qs = QString("%1").arg(aux->get_red());
            QString green_qs = QString("%1").arg(aux->get_green());
            QString blue_qs = QString("%1").arg(aux->get_blue());

            QTableWidgetItem *id_f = new QTableWidgetItem(id_qs);
            QTableWidgetItem *voltaje_f = new QTableWidgetItem(voltaje_qs);
            QTableWidgetItem *pos_x_f = new QTableWidgetItem(pos_x_qs);
            QTableWidgetItem *pos_y_f = new QTableWidgetItem(pos_y_qs);
            QTableWidgetItem *red_f = new QTableWidgetItem(red_qs);
            QTableWidgetItem *green_f = new QTableWidgetItem(green_qs);
            QTableWidgetItem *blue_f = new QTableWidgetItem(blue_qs);

            ui->tableWidget->setItem(0,0,id_f);
            ui->tableWidget->setItem(1,0,voltaje_f);
            ui->tableWidget->setItem(2,0,pos_x_f);
            ui->tableWidget->setItem(3,0,pos_y_f);
            ui->tableWidget->setItem(4,0,red_f);
            ui->tableWidget->setItem(5,0,green_f);
            ui->tableWidget->setItem(6,0,blue_f);
        }
        else
        {
            ui->label_11->setText("ID no encontrada");
        }
    }
}


void MainWindow::on_actionCapturar_Mostrar_triggered()
{
    ui->frame->show();
    ui->frame_2->show();
    ui->frame_3->hide();
}


void MainWindow::on_actionEscenaNeuronas_triggered()
{
    ventana.setScene(&escena1);
    QPen borde;

    if(lista.get_lista_beg() != NULL)
    {
        neurona *aux = lista.get_lista_beg();
        while(aux != NULL)
        {
            QBrush relleno(QColor(aux->get_red()+100, aux->get_green()+100, aux->get_blue()+100));

            elipse = escena1.addEllipse(aux->get_posicion_x(), aux->get_posicion_y(), aux->get_voltaje(), aux->get_voltaje(),borde,relleno);

            aux = aux->get_sig_neurona();
        }
    }
    ventana.show();
}

void MainWindow::sort()
{
    if(lista.get_lista_beg() != NULL)
    {
        int cont = 0;
        neurona *aux_cont = lista.get_lista_beg();
        while(aux_cont->get_sig_neurona() != NULL)
        {
            cont++;

            /*if(lista.get_anterior(aux_cont)!=NULL){
                cout << aux_cont->get_id() << endl;
                cout << lista.get_anterior(aux_cont)->get_id() << endl;}*/
            aux_cont = aux_cont->get_sig_neurona();

        }

        int cont2 = 0;


        while(cont2 < cont)
        {

            neurona *aux_buscar = lista.get_lista_beg();
            int cont3 = 0;
            while(cont3 < cont)
            {
                if(aux_buscar->get_voltaje() > aux_buscar->get_sig_neurona()->get_voltaje())
                {
                    cout << "holamundo1" << endl;
                    if (cont3 == 0)
                    {
                        cout << "holamundo2" << endl;
                        neurona *aux = aux_buscar->get_sig_neurona();
                        aux_buscar->set_sig_neurona(aux_buscar->get_sig_neurona()->get_sig_neurona());
                        //aux->set_sig_neurona(aux_buscar);
                        lista.agregar_inicio(aux);
                    }
                    else
                    {
                        cout << "holamundo3" << endl;
                        neurona *aux = aux_buscar->get_sig_neurona();
                        aux_buscar->set_sig_neurona(aux_buscar->get_sig_neurona()->get_sig_neurona());
                        lista.get_anterior(aux_buscar)->set_sig_neurona(aux);
                        aux->set_sig_neurona(aux_buscar);
                    }

                }
                else
                {
                    cout << "holamundo4" << endl;
                    aux_buscar = aux_buscar->get_sig_neurona();
                }
                cont3++;
            }
            cout << "holamundo5" << endl;
            cont2++;
        }
    }
}

void MainWindow::sort2()
{
    if(lista.get_lista_beg() != NULL)
    {
        int cont = 0;
        neurona *aux_cont = lista.get_lista_beg();
        while(aux_cont->get_sig_neurona() != NULL)
        {
            cont++;

            /*if(lista.get_anterior(aux_cont)!=NULL){
                cout << aux_cont->get_id() << endl;
                cout << lista.get_anterior(aux_cont)->get_id() << endl;}*/
            aux_cont = aux_cont->get_sig_neurona();

        }

        int cont2 = 0;


        while(cont2 < cont)
        {

            neurona *aux_buscar = lista.get_lista_beg();
            int cont3 = 0;
            while(cont3 < cont)
            {
                if(aux_buscar->get_id() > aux_buscar->get_sig_neurona()->get_id())
                {
                    cout << "holamundo1" << endl;
                    if (cont3 == 0)
                    {
                        cout << "holamundo2" << endl;
                        neurona *aux = aux_buscar->get_sig_neurona();
                        aux_buscar->set_sig_neurona(aux_buscar->get_sig_neurona()->get_sig_neurona());
                        //aux->set_sig_neurona(aux_buscar);
                        lista.agregar_inicio(aux);
                    }
                    else
                    {
                        cout << "holamundo3" << endl;
                        neurona *aux = aux_buscar->get_sig_neurona();
                        aux_buscar->set_sig_neurona(aux_buscar->get_sig_neurona()->get_sig_neurona());
                        lista.get_anterior(aux_buscar)->set_sig_neurona(aux);
                        aux->set_sig_neurona(aux_buscar);
                    }

                }
                else
                {
                    cout << "holamundo4" << endl;
                    aux_buscar = aux_buscar->get_sig_neurona();
                }
                cont3++;
            }
            cout << "holamundo5" << endl;
            cont2++;
        }
    }
}

void MainWindow::on_actionOrdenarVoltaje_triggered()
{
    this->sort();
}


void MainWindow::on_actionOrdenarID_triggered()
{
    this->sort2();
}


void MainWindow::on_actionEscenaGrafo_triggered()
{
    ventana.setScene(&escena2);

    if(lista.get_lista_beg() != NULL)
    {
        neurona *aux = lista.get_lista_beg();
        while(aux != NULL)
        {

            elipse = escena2.addEllipse(aux->get_posicion_x()-2, aux->get_posicion_y()-2, 4, 4);
            aux = aux->get_sig_neurona();
        }
    }

    //linea = escena2.addLine(aux->get_posicion_x(), aux->get_posicion_y(), aux->get_posicion_x()-50, aux->get_posicion_y()-50 );

    ventana.show();
}


void MainWindow::on_actionAplicarFB_triggered()
{
    neurona *aux2 = lista.get_lista_beg();

    while(aux2 != NULL)
    {
        int pos_x = 0;
        int pos_y = 0;
        int t_selec = 999;
        int pos_fx = 0;
        int pos_fy = 0;

        neurona *aux_buscar = lista.get_lista_beg();

        while(aux_buscar != NULL)
        {
            pos_x = aux2->get_posicion_x() - aux_buscar->get_posicion_x();
            pos_y = aux2->get_posicion_y() - aux_buscar->get_posicion_y();

            if(pos_x + pos_y != 0)
            {
                if(pos_x < 0)
                    pos_x = pos_x * -1;
                if(pos_y < 0)
                    pos_y = pos_y * -1;

                if(pos_x + pos_y < t_selec)
                {
                    t_selec = pos_x + pos_y;
                    pos_fx = aux_buscar->get_posicion_x();
                    pos_fy = aux_buscar->get_posicion_y();

                }
            }
            aux_buscar = aux_buscar->get_sig_neurona();
        }
        linea = escena2.addLine(aux2->get_posicion_x(), aux2->get_posicion_y(),pos_fx,pos_fy );
        aux2 = aux2->get_sig_neurona();
    }
}


void MainWindow::on_actionGrafoCompleto_triggered()
{
    ventana.setScene(&escena3);
    neurona *aux2 = lista.get_lista_beg();

    while(aux2 != NULL)
    {
        int pos_x = 0;
        int pos_y = 0;
        int t_selec = 999;

        neurona *aux_buscar = lista.get_lista_beg();

        while(aux_buscar != NULL)
        {
            pos_x = aux2->get_posicion_x() - aux_buscar->get_posicion_x();
            pos_y = aux2->get_posicion_y() - aux_buscar->get_posicion_y();

            if(pos_x + pos_y != 0)
            {
                if(pos_x < 0)
                    pos_x = pos_x * -1;
                if(pos_y < 0)
                    pos_y = pos_y * -1;

                if(pos_x + pos_y < t_selec)
                {
                    t_selec = pos_x + pos_y;
                    int peso = qSqrt((qPow(pos_x,2) + qPow(pos_y,2)));
                    aux2->set_adyecente(aux_buscar);
                    aux2->set_peso_a(peso);
                }
            }
            aux_buscar = aux_buscar->get_sig_neurona();
        }
        aux2 = aux2->get_sig_neurona();
    }

    if(lista.get_lista_beg() != NULL)
    {
        neurona *aux = lista.get_lista_beg();
        while(aux != NULL)
        {

            QGraphicsEllipseItem *elip = new QGraphicsEllipseItem(aux->get_posicion_x(), aux->get_posicion_y(), 4, 4);
            escena3.addItem(elip);
            QString aux_qstr = QString("%1").arg(aux->get_id());
            QFont fuente;
            QGraphicsTextItem *tex = new QGraphicsTextItem(aux_qstr);
            fuente.setPointSize(7);
            QGraphicsLineItem *lin = new QGraphicsLineItem(aux->get_posicion_x(), aux->get_posicion_y(),aux->get_adyecente()->get_posicion_x(),aux->get_adyecente()->get_posicion_y());
            escena3.addItem(lin);
            tex->setPos(elip->boundingRect().center());
            tex->setFont(fuente);
            escena3.addItem(tex);
            QString aux_qstr2 = QString("%1").arg(aux->get_peso_a());
            QFont fuente2;
            QGraphicsTextItem *tex2 = new QGraphicsTextItem(aux_qstr2);
            fuente2.setPointSize(5);
            tex2->setPos(lin->boundingRect().center());
            tex2->setFont(fuente2);
            escena3.addItem(tex2);
            QGraphicsRectItem *punta = new QGraphicsRectItem(aux->get_adyecente()->get_posicion_x()-2,aux->get_adyecente()->get_posicion_y()-2, 2, 2);
            escena3.addItem(punta);


            //((aux->get_adyecente()->get_posicion_x()-aux->get_posicion_x())+aux->get_adyecente()->get_posicion_x())/2,((aux->get_adyecente()->get_posicion_y()-aux->get_posicion_y())+aux->get_adyecente()->get_posicion_y())/2
            aux = aux->get_sig_neurona();
        }
    }

    ventana.show();
}

