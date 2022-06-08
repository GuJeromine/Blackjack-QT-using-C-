#include "listagrafica.h"
#include "janela.h"
#include "Baralho.h"
#include <QInputDialog>
#include <QPainter>
#include <QMessageBox>

int ListaGrafica::GanhadorDefinido;
int ListaGrafica::NumeroDeListas = 0;
int ListaGrafica::SomaLista1 = 0;
int ListaGrafica::SomaLista2 = 0;
int ListaGrafica::Turno = 0;
int ListaGrafica::AuxiliarDeDesistencia = 1;
ListaGrafica::ListaGrafica(QWidget *parent) : QFrame(parent){
    TemA = false;
    Desistiu = false;
    Ganhou = false;
    Soma = 0;
    NumeroDaLista = NumeroDeListas;
    NumeroDeListas++;
}
void ListaGrafica::mousePressEvent(QMouseEvent *event) {
    std::string sum = "Jogador1";
    std::string sdois = "Jogador2";
    std::string semp = "Empate";

    if(SomaLista1 > 21){
    resultMessage(sdois);
    GanhadorDefinido = true;
    }
    else if(SomaLista2 > 21){
     resultMessage(sum);
     GanhadorDefinido = true;
    }
    else if(SomaLista1 == 21){
     resultMessage(sum);
     GanhadorDefinido = true;
    }
    else if(SomaLista2 == 21){
     resultMessage(sdois);
     GanhadorDefinido = true;
    }
    else if(GanhadorDefinido == true){
        if(SomaLista1 < 21 and SomaLista1 > SomaLista2)
        resultMessage(sum);
        if(SomaLista2 < 21 and SomaLista2 > SomaLista1)
        resultMessage(sdois);
    }
    else if(AuxiliarDeDesistencia == 2 and Desistiu == false and SomaLista1 > SomaLista2 and (Turno % 2 == 0)){
        GanhadorDefinido = true;
        resultMessage(sum);
    }
    else if(AuxiliarDeDesistencia == 2 and Desistiu == false and SomaLista2 > SomaLista1 and (Turno % 2 == 1)){
        GanhadorDefinido = true;
        resultMessage(sdois);
    }
    if (event->button() == Qt::LeftButton and Baralho::getTam() > 0 and Desistiu == false and (Soma < 21) and GanhadorDefinido == 0 and
            (Turno % 2 == NumeroDaLista)){
        int NumeroSorteado = Baralho::SortearCarta();
        qDebug() << NumeroSorteado;
        int NaipeSorteada = NumeroSorteado / 13;
        NumeroSorteado = NumeroSorteado % 13;
        if(NumeroSorteado == 0){
            NumeroSorteado = 13;
            NaipeSorteada--;
        }
        if(NumeroSorteado == 1 and Soma < 12){
            TemA = true;
            Soma = Soma + 10;
        }
        Carta CartaSorteada;
        CartaSorteada.NumDaCarta = NumeroSorteado;
        CartaSorteada.NaipeDaCarta = NaipeSorteada;
        if(Elementos.size() > 0){
        Elementos.pop_back();
        }
        Elementos.push_back(CartaSorteada);
        Carta CartaSoma;
        if(NumeroSorteado > 10){
            Soma = Soma + 10;
        }
        else{
            Soma = Soma + NumeroSorteado;
        }

        if(Soma == 21){
            GanhadorDefinido = 1;
            Ganhou = true;
        }
        if(Soma > 21 and TemA == true){
            TemA = false;
            Soma = Soma - 10;
        }
        if(Soma > 21){
            GanhadorDefinido = true;           
        }
        if(Turno % 2 == 0){
            SomaLista1 = Soma;
        }
        if(Turno % 2 == 1){
            SomaLista2 = Soma;
        }
        CartaSoma.NaipeDaCarta = 4;
        CartaSoma.NumDaCarta = Soma;
        Elementos.push_back(CartaSoma);
        this->update();
        Turno = Turno + AuxiliarDeDesistencia;
    }
   else if(event->button() == Qt::RightButton and Desistiu == false and (Soma < 21) and GanhadorDefinido == 0 and
           (Turno % 2 == NumeroDaLista)){
    if(AuxiliarDeDesistencia == 2 and SomaLista1 == SomaLista2)
    {
        resultMessage(semp);
        GanhadorDefinido = true;
    }
    Desistiu = true;
    if(NumeroDaLista == 0 and SomaLista1 < SomaLista2){
        GanhadorDefinido = true;
    }
    if(NumeroDaLista == 1 and SomaLista2 < SomaLista1){
        GanhadorDefinido = true;
    }
    AuxiliarDeDesistencia = 2;
    Turno++;
    }
}

void ListaGrafica::resultMessage(std::string &s) const
{
    QMessageBox::information(nullptr, "Vencedor", s.c_str());
}

void ListaGrafica::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::darkRed);
    QBrush brush;
    brush.setColor(Qt::darkGreen);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    pen.setWidth(6);
    painter.setPen(pen);
    painter.drawRect(this->rect().x()+1,
                     this->rect().y()+1,
                     this->rect().width()-1,
                     this->rect().height()-2);

    int Margem = 35;
    int LarguraDaCarta = 100;
    int AlturaDaCarta = 175;
    pen.setWidth(3);
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    painter.setPen(pen);
    for (size_t i=0; i<Elementos.size(); i++) {
        pen.setWidth(2);
        pen.setColor(Qt::black);
        if(Elementos.at(i).NaipeDaCarta == 4)
            pen.setColor(Qt::darkYellow);
        painter.setPen(pen);
        painter.drawRect(Margem*(i+1) + LarguraDaCarta*i, 50,
                 LarguraDaCarta, AlturaDaCarta);
        if(Elementos.at(i).NaipeDaCarta == 0){
            QString Naipe = "\3";
            pen.setColor(Qt::red);
            painter.setPen(pen);
            switch(Elementos.at(i).NumDaCarta){
            case 1:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("A"));
                break;
            case 11:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("J"));
                break;
            case 12:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("Q"));
                break;
            case 13:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("K"));
                break;
            default:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::number(Elementos.at(i).NumDaCarta));
            }
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i + 10 , 55,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignTop,
                 Naipe);
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i - 10 , 200,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignRight, Naipe);
        }
        if(Elementos.at(i).NaipeDaCarta == 1){
            pen.setColor(Qt::red);
            painter.setPen(pen);
            QString Naipe = "\4";
            switch(Elementos.at(i).NumDaCarta){
            case 11:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("J"));
                break;
            case 12:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("Q"));
                break;
            case 13:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("K"));
                break;
            default:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::number(Elementos.at(i).NumDaCarta));
            }
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i + 10 , 55,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignTop, Naipe);
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i - 10 , 200,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignRight, Naipe);
        }
        if(Elementos.at(i).NaipeDaCarta == 2){
            pen.setColor(Qt::black);
            painter.setPen(pen);
            QString Naipe = "\5";
            switch(Elementos.at(i).NumDaCarta){
            case 11:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("J"));
                break;
            case 12:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("Q"));
                break;
            case 13:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("K"));
                break;
            default:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::number(Elementos.at(i).NumDaCarta));
            }
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i + 10 , 55,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignTop, Naipe);
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i - 10 , 200,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignRight, Naipe);
        }
        if(Elementos.at(i).NaipeDaCarta == 3){
            pen.setColor(Qt::black);
            painter.setPen(pen);
            QString Naipe = "\6";
            switch(Elementos.at(i).NumDaCarta){
            case 11:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("J"));
                break;
            case 12:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("Q"));
                break;
            case 13:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::fromStdString("K"));
                break;
            default:
                painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                                 LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                         QString::number(Elementos.at(i).NumDaCarta));
            }
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i + 10 , 55,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignTop, Naipe);
        painter.drawText(Margem*(i+1) + LarguraDaCarta*i - 10 , 200,
                         LarguraDaCarta, AlturaDaCarta, Qt::AlignRight, Naipe);
        }
        if(Elementos.at(i).NaipeDaCarta == 4){
            //QString Naipe = "\3";
            pen.setColor(Qt::darkYellow);
            painter.setPen(pen);
            painter.drawText(Margem*(i+1) + LarguraDaCarta*i, 50,
                             LarguraDaCarta, AlturaDaCarta, Qt::AlignCenter,
                     QString::number(Elementos.at(i).NumDaCarta));    
        }

    }
    pen.setColor(Qt::white);
    painter.setPen(pen);
    if(NumeroDaLista == 0)
    painter.drawText(20,5,60,50, Qt::AlignCenter, "Jogador 1");
    else if(NumeroDaLista == 1)
    painter.drawText(20,5,60,50, Qt::AlignCenter, "Jogador 2");
}
