#ifndef PUBLICATIONUSER_H
#define PUBLICATIONUSER_H

#include <publication.h>

class PublicationUser : public Publication
{
public:
    string			fullName;			//Nombre del publicante
    unsigned int	codePhone;
    PublicationUser(){}
    PublicationUser(int ID, string fullName, unsigned int codePhone, string title,
                    string desc, Date pubDate, unsigned int numLikes): Publication(ID, title, desc, pubDate, numLikes){
        this->fullName = fullName;
        this->codePhone = codePhone;
    }
    ~PublicationUser() {}
    void setFullName(string fullName) { this->fullName = fullName; }
    void setCodePhone(unsigned int codePhone) { this->codePhone = codePhone; }
    string			getFullName() { return fullName; }
    unsigned int	getCodePhone() { return codePhone; }
};

#endif // PUBLICATIONUSER_H
