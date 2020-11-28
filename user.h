#ifndef USER_H
#define USER_H


#include <iostream>
#include <vector>
#include "date.h"
#include "friend.h"
#include "comment.h"
#include "interaction.h"
#include "publication.h"

using std::string;
using std::vector;

class User
{
public:
public:
    enum Gender {Male, Female, None};
    enum pGender {M, F, Both, N};							//MALE, FEMALE, BOTH, NONE
    enum Hobbies {
        Musica = 'A', Leer, Vino, SalirATomarAlgo,
        Bailar, Escritura, Caminar, Manualidades, Cocinar,
        Senderismo, Futbol, Reposteria, Entrenamiento,
        Golf, Deportes, Moda, Trivia, Museo, AireLibre,
        Cafe, Viajar, Fotografia, Voluntariado, Te,
        Gamer, SalirDeCompras, Disney = 'a', Nadar, Bloquear,
        Karaoke, Peliculas, AmanteDeLosGatos
    };
    enum University {
        UPC, UDA, UPCH, UP, PUCP, UTEC, UCAL, UNIFE, URP, UDEP,
        UNALM, UNAH, UARM, USMP, UNTRM, UNAJMA, USIL, UMCH, UCSP,
        UPN, UCH, UNI, UPT, UAC, UNAP, UCSM, UNJ, NONE
    };

protected:
    string						email;
    string						fullName;					//Nombre completo
    Gender						gender;						//Genero
    pGender						prefGender;					//Lo que le gusta (MH, MM, MA, HM, HA, HH)
    bool						showGender;					//Mostrar genero
    unsigned short				age;						//Edad
    unsigned int				cellPhone;					//Numero de telefono
    University					studyCenter;				//Centro de estudio
    Date						registerDate;				//Dia del registro
    vector<Hobbies>				hobbies;					//Pasatiempos
    vector<Friend>				friends;					//Amigos del usuario
    vector<Publication>			publications;				//Publicaciones del usuario (ID)

public:
    User() { }
    User(string email, string fullName, Gender gender, pGender prefGender, bool showGender, unsigned short age,
        unsigned int cellPhone, University studyCenter, Date registerDate, vector<Hobbies> hobbies, vector<Friend> friends,
        vector<Publication> publicationes) : email(email), fullName(fullName), gender(gender), prefGender(prefGender), showGender(showGender), age(age),
        cellPhone(cellPhone), studyCenter(studyCenter), registerDate(registerDate), hobbies(hobbies), friends(friends), publications(publicationes) {}
    ~User() { }
    void setEmail			(string email)						{ this->email = email                   ; }
    void setFullName		(string fullName)					{ this->fullName = fullName             ; }
    void setGender			(Gender gender)						{ this->gender = gender                 ; }
    void setPrefGender		(pGender prefGender)				{ this->prefGender = prefGender         ; }
    void setShowGender		(bool showGender)					{ this->showGender = showGender         ; }
    void setAge				(unsigned short age)				{ this->age = age                       ; }
    void setCellPhone		(unsigned int cellPhone)			{ this->cellPhone = cellPhone           ; }
    void setStudyCenter		(University studyCenter)			{ this->studyCenter = studyCenter       ; }
    void setRegisterDate	(Date registerDate)					{ this->registerDate = registerDate     ; }
    void setHobbies			(vector<Hobbies> hobbies)			{ this->hobbies = hobbies               ; }
    void setFriends			(vector<Friend> friends)			{ this->friends = friends               ; }
    void setPublications	(vector<Publication> publications)	{ this->publications = publications     ; }

    void addPublication		(Publication newPublication)		{ publications.push_back(newPublication); }
    void addFriend			(Friend newFriend)					{ friends.push_back(newFriend)          ; }
    void addHobbie			(Hobbies newHobbie)					{ hobbies.push_back(newHobbie)			; }

    void setFriendStatus(unsigned int code, Friend::Condition newCondition) {
        for (unsigned int i = 0; i < friends.size(); i++) {
            if (friends[i].getCode() == code)
                friends[i].setIsAdd(newCondition);
        }
    }
    void removeFriend(unsigned int code) {
        for (unsigned int i = 0; i < friends.size(); i++)
            if (friends[i].getCode() == code)
                friends.erase(friends.begin() + i);
    }
    bool searchFriendPendient(unsigned int code) {
        for (unsigned int i = 0; i < friends.size(); i++)
            if (friends[i].getCode() == code && friends[i].getIsAdd() == Friend::Condition::pendient)
                return true;
        return false;
    }

    bool searchFriendExist(int code) {
            for (int i = 0; i < friends.size(); i++)
                if (int(friends[i].getCode()) == code) return true;
            return false;
        }

    string					getEmail()			{ return email          ; }
    string					getFullName()		{ return fullName       ; }
    Gender					getGender()			{ return gender         ; }
    pGender					getPrefGender()		{ return prefGender     ; }
    bool					getShowGender()		{ return showGender     ; }
    unsigned short			getAge()			{ return age            ; }
    unsigned int			getCellPhone()		{ return cellPhone      ; }
    University				getStudyCenter()	{ return studyCenter    ; }
    Date					getRegisterDate()	{ return registerDate   ; }
    vector<Hobbies>			getHobbies()		{ return hobbies        ; }
    vector<Friend>&			getFriends()		{ return friends        ; }
    vector<Publication>&	getPublications()	{ return publications   ; }

};

#endif // USER_H
