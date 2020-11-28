#ifndef ILABELFULLNAME_H
#define ILABELFULLNAME_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>

class iLabelFullName : public QWidget
{
    Q_OBJECT

private:
    QLabel*         lblFullname;
    unsigned int    codeUser;

public:
    explicit iLabelFullName(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout* lyh = new QVBoxLayout;
        lyh->setContentsMargins(0,0,0,0);

        lblFullname = new QLabel();
        lyh->addWidget(lblFullname);

        this->setLayout(lyh);
        setStyle();
    }

    void mousePressEvent(QMouseEvent*) {
        emit clicked(codeUser);
    }

    void setCodeUser(unsigned int codeUser) { this->codeUser = codeUser; }
    void setFullName(QString fullname) { lblFullname->setText(fullname); }

    void setStyle() {
        lblFullname->setStyleSheet("font: 10pt 'MS Shell Dlg 2';");
    }

signals:
    void clicked(unsigned int);

};

#endif // ILABELFULLNAME_H
