#ifndef USERSERVER_H
#define USERSERVER_H

#include "user.h"
#include <algorithm>
#include <string>

using namespace std;

class UserServer : public User
{
private:
    string			password;
    string			idFilter;

public:
    UserServer() {}
    UserServer(string fullName, string email, string password, Gender gender, pGender prefGender,
        bool showGender, unsigned short age, unsigned int cellPhone, University studyCenter, Date registerDate,
        vector<Hobbies> hobbies, vector<Friend> friends, vector<Publication> publications) :
        User(email, fullName, gender, prefGender, showGender, age, cellPhone, studyCenter, registerDate, hobbies, friends, publications),
        password(password) {
        GenerateIDFilter();
    }
    ~UserServer() { }

    void		setPassword(string password)	{ this->password = password	; }
    void		setIDFilter(string idFilter)	{ this->idFilter = idFilter	; }
    string		getPassword()					{ return password			; }
    string		getIDFilter()					{ return idFilter			; }
    void		generateIDFilter()				{ GenerateIDFilter()		; }

private:
    void GenerateIDFilter() {
        string ID;
        (gender == 0) ?				ID =  "H" : ID = "M";
        if (prefGender == 0)		ID += "H.";
        else if (prefGender == 1)	ID += "M.";
        else						ID += "A.";
        ID += (to_string(studyCenter) + '.');
        sort(hobbies.begin(), hobbies.end());
        ID += (char)hobbies[0];
        idFilter = ID;
    }
};

#endif // USERSERVER_H
