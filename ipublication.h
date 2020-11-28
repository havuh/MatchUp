#ifndef IPUBLICATION_H
#define IPUBLICATION_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <ilabelfullname.h>
#include <ibuttonpublication.h>
#include "icomment.h"

class iPublication : public QWidget
{
    Q_OBJECT

private:
    iLabelFullName*     lblFullName;
    QLabel*             lblTitle;
    QLabel*             lblDate;
    QLabel*             lblPublication;
    iButtonPublication* btnLike;
    iButtonPublication* btnComments;
    iButtonPublication* btnShared;
    QGroupBox           gbPublication;
    QLabel              lblLikes;
    QLabel              lblComments;
    QVector<iComment>*  vecComments;
    QGridLayout*        lyP;
    int                 numLikes;

public:
    iPublication(QWidget *parent = nullptr) : QWidget(parent)
    {

        //asignacion de memoria a componentes importantes
        btnLike = new iButtonPublication;
        btnShared = new iButtonPublication;
        btnComments = new iButtonPublication;
        lblFullName = new iLabelFullName;
        lblDate = new QLabel;
        lblPublication = new QLabel;
        lblTitle = new QLabel;

        //Fila del Nombre
        QHBoxLayout* lyF = new QHBoxLayout;
        lyF->addWidget(lblFullName);
        lyF->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

        //Fila de muestra de likes y comentarios
        QHBoxLayout* lyM = new QHBoxLayout;
        lyM->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
        lyM->addWidget(&lblLikes);
        lyM->addWidget(&lblComments);
        lyM->setContentsMargins(0,0,0,0);

        //Fila de botones
        QHBoxLayout* lyB = new QHBoxLayout;
        lyB->addWidget(btnLike);
        lyB->addWidget(btnComments);
        lyB->addWidget(btnShared);

        //SEPARADOR
        QFrame* lineH1 = new QFrame;
        lineH1->setFrameShape(QFrame::HLine);
        lineH1->setFrameShadow(QFrame::Sunken);

        QFrame* lineH2 = new QFrame;
        lineH2->setFrameShape(QFrame::HLine);
        lineH2->setFrameShadow(QFrame::Sunken);

        //Generar el layout de gbPublication;
        QVBoxLayout* lyG = new QVBoxLayout;
        lyG->addLayout(lyF);
        lyG->addWidget(lblDate);
        lyG->addWidget(lblTitle);
        lyG->addWidget(lblPublication);
        lyG->addLayout(lyM);
        lyG->addWidget(lineH1);
        lyG->addLayout(lyB);
        lyG->addWidget(lineH2);

        gbPublication.setLayout(lyG);
        setMaximumWidth(parent->width() - 30);

        //Agregar el layout principal
        lyP = new QGridLayout;
        lyP->setContentsMargins(0,0,0,0);

        //Pasar el layout principal al widget
        setLayout(lyP);

        //Agregar Widget
        layout()->addWidget(&gbPublication);

        lblLikes.setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        lblComments.setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        lblPublication->setWordWrap(true);

        //conectar
        connect(btnLike, SIGNAL(clicked(unsigned int, unsigned int)), this, SLOT(addLikes()));

        //CAMBIAR ESTILO
        setStyle();

    }

    void setData(QString FullName, QString Date, QString Title, QString Publication, unsigned int codeUser,
                       unsigned int idPublication, QString numLikes, QString numComments)
    {
        //Informacion de etiquetas

        lblFullName->setFullName(FullName);
        lblFullName->setCodeUser(idPublication);
        lblDate->setText(Date);
        lblPublication->setText(Publication);
        lblComments.setText(numComments + " comentarios");
        lblLikes.setText(numLikes + " likes");
        lblTitle->setText(Title);

        //data a los botones

        btnLike->setText("Me gusta");
        btnLike->setIDPublication(idPublication);
        btnLike->setIDUser(codeUser);

        btnComments->setText("Comentar");
        btnComments->setIDPublication(idPublication);
        btnComments->setIDUser(codeUser);

        btnShared->setText("Compartir");
        btnShared->setIDPublication(idPublication);
        btnShared->setIDUser(codeUser);

        this->numLikes = numLikes.toUInt();

        connect(btnLike, SIGNAL(MoreLike(int)), this, SLOT(MoreLike(int)));

        lblLikes.setAlignment(Qt::AlignRight);
        lblComments.setAlignment(Qt::AlignRight);


    }
    void addComment(iComment*& comment) {
        gbPublication.layout()->addWidget(comment);
    }
    void setStyle()
    {
        gbPublication.setStyleSheet     ("border: 1px solid #DCD6D6; margin: 7px 0px 7px 0px;");
        lblFullName->setStyleSheet      ("border: none; font: 10pt 'MS Shell Dlg 2'; margin: 0px 0px 0px 0px;");
        lblDate->setStyleSheet          ("border: none; margin: 0px 0px 0px 0px;");
        lblTitle->setStyleSheet         ("border: none; margin: 0px 0px 0px 0px;");
        lblPublication->setStyleSheet   ("border: none; margin: 0px 0px 0px 0px;");
        lblLikes.setStyleSheet          ("border: none; margin: 0px 0px 0px 0px;");
        lblComments.setStyleSheet       ("border: none; margin: 0px 0px 0px 0px;");
    }

    iButtonPublication* getBtnLike()        { return btnLike;       }
    iButtonPublication* getBtnComments()    { return btnComments;   }
    iButtonPublication* getBtnShared()      { return btnShared;     }
    iLabelFullName*     getFullName()       { return lblFullName;   }



public slots:
    void addLikes()
    {
        numLikes++;
        lblLikes.setText(QString::number(numLikes) + QString(" likes"));
    }

signals:

};

#endif // IPUBLICATION_H
