#ifndef TEXTPUBLICATION_H
#define TEXTPUBLICATION_H

#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QLabel>

class TextPublication : public QWidget
{
    Q_OBJECT

private:
    QTextEdit*          text;
    QLabel              photo;
public:
    explicit TextPublication(QWidget *parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout* lh = new QHBoxLayout;
        text = new QTextEdit;
        lh->setContentsMargins(0,0,0,0);

        lh->addWidget(&photo);
        lh->addWidget(text);

        this->setLayout(lh);
    }
    void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == QKeyEvent::Enter){
            emit pressEnter(text->toPlainText());
        }
        emit pressEnter(text->toPlainText());
    }
    QTextEdit*& getPlainEdit() { return text; }
    void clearPlainText() { text->setText(""); }
    void setStyleSheet(QString fullname, QString filePhoto){
        photo.setMinimumSize(60,60);
        photo.setMaximumSize(60,60);
        photo.setStyleSheet("border: none; ");
        photo.setPixmap(QPixmap(filePhoto));
        photo.setScaledContents(true);
        text->setStyleSheet("background-color: gray; border-radius: 30px; padding: 10 15 10 15; color: white; font: 10pt 'MS Shell Dlg 2';");
        text->setPlaceholderText(fullname + QString(", ¿Qué estas pensando?"));
    }

signals:
    void pressEnter(QString);
};

#endif // TEXTPUBLICATION_H
