#ifndef IFRIEND_H
#define IFRIEND_H

#include <QWidget>
#include <QMouseEvent>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>

class iFriend : public QWidget
{
    Q_OBJECT

private:
    int             codeUser;
    QGroupBox*      gbContainer;
    QHBoxLayout*    lyHorizontal;
    QLabel*         lblPhoto;
    QLabel*         lblFullName;

public:
    explicit iFriend(QWidget *parent = nullptr) : QWidget(parent)
    {
        setGeometry(0,0, parent->width() - 100, 60);
        setMinimumSize(parent->width() - 100, 60);
        setMaximumSize(parent->width() - 100, 60);

        gbContainer = new QGroupBox(this);
        gbContainer->setGeometry(this->rect());
        gbContainer->setMaximumSize(this->size());

        lblPhoto = new QLabel(this);
        lblPhoto->setMinimumSize(40,40);
        lblPhoto->setMaximumSize(40,40);
        lblFullName = new QLabel(this);
        lblFullName->setMinimumSize(this->width() - 150,40);
        lblFullName->setMinimumSize(this->width() - 150,40);

        lyHorizontal = new QHBoxLayout();
        lyHorizontal->addSpacerItem(new QSpacerItem(20,40));
        lyHorizontal->addWidget(lblPhoto);
        lyHorizontal->addSpacerItem(new QSpacerItem(25,40));
        lyHorizontal->addWidget(lblFullName);
        lyHorizontal->addSpacerItem(new QSpacerItem(20,40));

        gbContainer->setLayout(lyHorizontal);

        setStyleMyWidget();
    }
    void setText(QString fullname, QString photo, int codeUser)
    {
        lblFullName->setText(fullname);
        lblPhoto->setText(photo);
        this->codeUser = codeUser;
    }
    void setStyleMyWidget()
    {
        setStyleSheet("background-color: gray; border-radius: 30px; ");
        gbContainer->setStyleSheet("border-radius: 30px; background-color: gray; ");
        lblPhoto->setStyleSheet("background-color: white; border-radius: 20px; ");
        lblFullName->setStyleSheet("color: white; font: 12pt 'MS Shell Dlg 2'; ");
    }
    void setPhoto(QString photo)
    {
        lblPhoto->setPixmap(QPixmap(photo));
        lblPhoto->setScaledContents(true);
        lblPhoto->setStyleSheet("border: none; background: none;");
    }
    void mousePressEvent(QMouseEvent * event)
    {
        emit clicked(codeUser);
    }

signals:
    void clicked(unsigned int);
};

#endif // IFRIEND_H
