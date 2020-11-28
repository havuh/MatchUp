#ifndef FUNTIONSHELP_H
#define FUNTIONSHELP_H

#include <QComboBox>
#include <QDateTime>

void addComboBoxDays(QComboBox* cb){
    for(int i = 1; i <= 31; i++){
        cb->addItem(QString::number(i));
    }
}
void addComboBoxYears(QComboBox* cb){
    int yearNow = QDateTime::currentDateTime().date().year();
    for (int i = yearNow; i >= 1970; --i)
        cb->addItem(QString::number(i));
}
bool validateExtensionMail(QString mail){
    QVector<QString> extMail = { "@gmail.com", "@hotmail.com", "@outlook.es" };

    for (int i = 0; i < extMail.size(); i++){
        if (mail.indexOf(extMail[i]) != -1){
            return true;
        }
    }
    return false;
}
void addUserHobbie();


#endif // FUNTIONSHELP_H
