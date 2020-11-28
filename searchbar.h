#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

using std::string;

class searchBar : public QWidget
{
    Q_OBJECT

private:
    QLabel*     icon;
    QLineEdit*  bar;
public:
    explicit searchBar(QWidget *parent = nullptr) :  QWidget(parent)
    {
        this->setParent(parent);
        this->setGeometry(24,20, 661, 51);

        QVBoxLayout* ly = new QVBoxLayout;
        bar = new QLineEdit;
        icon = new QLabel(parent);

        icon->setMinimumSize(21, 21);
        icon->setMaximumSize(21, 21);
        icon->setGeometry(43, 35, 21, 21);

        bar->setMinimumSize(661, 51);
        bar->setMaximumSize(661, 51);

        ly->setContentsMargins(0,0,0,0);
        ly->addWidget(bar);
        this->setLayout(ly);
        this->setContentsMargins(0,0,0,0);

        setStyle();
    }

    QLineEdit*& getLineEdit() { return bar; }

    void setStyle()
    {
        icon->setScaledContents(true);
        icon->setPixmap(QPixmap(":/img/Resource/img/lupa-gris.png"));
        icon->setStyleSheet("border: 1px black solid; ");
        bar->setStyleSheet("border-radius: 20px; border: 1px solid gray; background-color: white; padding: 10px 60px 10px 60px; font: 10pt 'MS Shell Dlg 2';");
        bar->setPlaceholderText("Buscar...");
    }

    void keyPressEvent(QKeyEvent *event)
    {
        string data = bar->text().toStdString();
        emit searchThisData(data);
    }

signals:
    void searchThisData(string data);
};

#endif // SEARCHBAR_H
