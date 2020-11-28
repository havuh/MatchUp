#ifndef IBUTTONPUBLICATION_H
#define IBUTTONPUBLICATION_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QLabel>
#include <QGroupBox>

class iButtonPublication : public QWidget
{
    Q_OBJECT

private:
    unsigned int    idUser;
    unsigned int    idPublication;
    QLabel*         lblName;
    QLabel*         lblIcon;
    QGroupBox*      gbButton;
public:
    explicit iButtonPublication(QWidget *parent = nullptr) : QWidget(parent)
    {
        lblName = new QLabel;
        lblIcon = new QLabel;
        gbButton = new QGroupBox;

        lblName->setAlignment(Qt::AlignCenter);

        QHBoxLayout* lyh = new QHBoxLayout;
        lyh->addWidget(lblIcon);
        lyh->addWidget(lblName);
        lyh->setContentsMargins(10,2,15,2);

        lblIcon->setMaximumSize(30,30);
        lblIcon->setMinimumSize(30,30);

        gbButton->setLayout(lyh);

        QVBoxLayout* lyv = new QVBoxLayout;
        lyv->addWidget(gbButton);
        lyv->setContentsMargins(0,0,0,0);

        this->setLayout(lyv);

        setStyle();
    }
    void mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton){
            emit clicked(idUser, idPublication);
            emit MoreLikes(idUser, idPublication);
        }
    }
    void setIDUser(unsigned int idUser) { this->idUser = idUser; }
    void setIDPublication(unsigned int idPublication) { this->idPublication = idPublication; }
    void setText(QString text) { lblName->setText(text); }
    void setStyle()
    {
        lblName->setStyleSheet("");
        gbButton->setStyleSheet("border-radius: 18px; border: 1px solid gray; padding: 0px 10px 0px 10px; ");
        lblName->setStyleSheet("border: 0px solid none; ");
        lblIcon->setStyleSheet("border-radius: 0px; border: none; ");
    }
public slots:


signals:
    void clicked(unsigned int, unsigned int);
    void MoreLikes(unsigned int, unsigned int);

};

#endif // IBUTTONPUBLICATION_H
