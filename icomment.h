#ifndef ICOMMENT_H
#define ICOMMENT_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QString>
#include "ilabelfullname.h"

class iComment : public QWidget
{
    Q_OBJECT

private:
    QGroupBox*          gbComment;
    iLabelFullName*     lblFullName;
    QLabel*             lblDate;
    QLabel*             lblComment;

public:
    explicit iComment(QWidget *parent = nullptr) : QWidget(parent) {
        lblFullName = new iLabelFullName;
        lblDate = new QLabel;
        lblComment = new QLabel;
        gbComment = new QGroupBox;
        QVBoxLayout* lyv = new QVBoxLayout;
        QHBoxLayout* lyh = new QHBoxLayout;
        lyh->addWidget(lblFullName);
        lyh->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

        lyv->addLayout(lyh);
        lyv->addWidget(lblDate);
        lyv->addWidget(lblComment);

        gbComment->setLayout(lyv);
        QVBoxLayout* ml = new QVBoxLayout;
        setLayout(ml);
        layout()->addWidget(gbComment);

        setStyle();
    }

    void setData(QString fullName, QString date, QString comment, int userCode)
    {
        lblFullName->setFullName(fullName);
        lblFullName->setCodeUser(userCode);
        lblDate->setText(date);
        lblComment->setWordWrap(true);
        lblComment->setText(comment);
    }
    void setStyle()
    {
        gbComment->setStyleSheet("border: 1px solid gray; border-radius: 15px; background-color: white; padding: 2px 10px 2px 10px; ");
        lblFullName->setStyleSheet("font: 10pt 'MS Shell Dlg 2'; border: none;");
        lblDate->setStyleSheet("border: none; ");
        lblComment->setStyleSheet("border: none; ");
        //setStyleSheet("border: 1px solid gray; ");
    }

    iLabelFullName* getFullName() { return lblFullName; }

signals:

};

#endif // ICOMMENT_H
