#ifndef SERVER_H
#define SERVER_H


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "userserver.h"
#include "plabel.h"
#include "avl.h"
#include "client.h"
#include "friend.h"

using std::string;
using std::ofstream;
using std::ifstream;

typedef Avl<UserServer, string>			avlSUser;
typedef Avl<UserServer, int>			avlIUser;
typedef Avl<Publication, int>			AvlPInt;

class Server
{
private:
    string			dataBase;				//Nombre del archivo
    avlSUser*		avlUEmail;				//Arbol de emails
    avlIUser*		avlUCellPhone;			//Arbol de telefono
    avlSUser*		avlUIDFilter;			//Arbol de filtro

public:
    Server() {
        avlUEmail =		new avlSUser([](UserServer user)	{ return user.getEmail(); });
        avlUCellPhone = new avlIUser([](UserServer user)	{ return user.getCellPhone(); });
        avlUIDFilter =	new avlSUser([](UserServer user)	{ return user.getIDFilter(); });
        dataBase = "db__RegisterUser.txt";
        readFromDataBase();
    }
    ~Server() {}

    avlSUser* getAVLUEmail()	{ return avlUEmail;			}
    avlIUser* getAVLCellPhone() { return avlUCellPhone;		}
    avlSUser* getAVLIDFilter()	{ return avlUIDFilter;		}

    bool existThisCellPhone(unsigned int cellPhone) {
        UserServer userExist = avlUCellPhone->find(cellPhone);
        if (userExist.getCellPhone() == cellPhone) return true;
        return false;
    }
    bool existThisEmail(string email) {
        UserServer userExist = avlUEmail->find(email);
        if (userExist.getEmail() == email) return true;
        return false;
    }

    bool logInWithEmail(string email, string password, Client& myClient) {
        UserServer logUser = avlUEmail->find(email);
        if (logUser.getEmail() == email && logUser.getPassword() == password) {
            myClient.setUserData(logUser);
            myClient.setlistFeed(GenerateListFeed(logUser));
            GenerateTreeFriend(myClient);
            return true;
        }
        return false;
    }
    bool logInWithCellPhone(unsigned int cellPhone, string password, Client& myClient) {
        UserServer logUser = avlUCellPhone->find(cellPhone);
        if (logUser.getCellPhone() == cellPhone && logUser.getPassword() == password) {
            myClient.setUserData(logUser);
            myClient.setlistFeed(GenerateListFeed(logUser));
            GenerateTreeFriend(myClient);
            return true;
        }
        return false;
    }

    void addNewComment(Client& client, int IDPub, Comment comm)
    {
        UserServer user = avlUCellPhone->find(client.getUserData().getCellPhone());
        avlUCellPhone->findOriginal(user.getCellPhone()).getPublications()[IDPub].addComment(comm);
        avlUEmail->findOriginal(user.getEmail()).getPublications()[IDPub].addComment(comm);
    }
    void LoadTXT(){
        ifstream data;
                data.open("users.txt", std::ios::in);

                if (data.is_open()) {

                    string					line;

                    getline(data, line);
                    while (!data.eof()) {

                        UserServer			user;
                        string				joinDate;
                        Date				date;

                        int pos_ini = 0, pos_final = 0;
                        getline(data, line);

                        if (line.size() > 0) {

                            pos_ini = 0;
                            pos_final = line.find('\t');
                            user.setCellPhone(900000000 + stoi(line.substr(pos_ini, pos_final - pos_ini)));

                            pos_ini = pos_final + 1;
                            pos_final = line.find('\t', pos_ini);
                            user.setEmail(line.substr(pos_ini, pos_final - pos_ini));

                            pos_ini = pos_final + 1;
                            pos_final = line.find('\t', pos_ini);
                            user.setFullName(line.substr(pos_ini, pos_final - pos_ini));

                            pos_ini = pos_final + 1;
                            pos_final = line.length();
                            joinDate = line.substr(pos_ini, pos_final - pos_ini);

                            pos_ini = pos_final = 0;
                            pos_final = joinDate.find("-");
                            date.setYear(stoi(joinDate.substr(pos_ini, pos_final - pos_ini)));

                            pos_ini = pos_final + 1;
                            pos_final = joinDate.find("-", pos_ini);
                            date.setMonth(stoi(joinDate.substr(pos_ini, pos_final - pos_ini)));

                            pos_ini = pos_final + 1;
                            pos_final = joinDate.length();
                            date.setDay(stoi(joinDate.substr(pos_ini, pos_final - pos_ini)));

                            user.setRegisterDate(date);

                            user.setPassword(std::to_string(100000 + rand() % 1500));
                            user.setAge(18 + rand() % 40);
                            user.setGender(User::Gender(rand() % 2));
                            user.setShowGender(rand() % 2);
                            user.setPrefGender(User::pGender(rand() % 3));
                            user.setStudyCenter(User::University(1 + rand() % 4));
                            for (int i = 0; i < 5; i++)
                                user.addHobbie(User::Hobbies(65 + rand() % 10));

                            user.generateIDFilter();

                            addUser(user);
                        }
                    }
                }

                ifstream publ;
                publ.open("publications.txt", std::ios::in);

                if (publ.is_open()) {

                    string		line;
                    getline(publ, line);
                    while (!publ.eof()) {

                        Publication		newPubl;
                        string			fecha;
                        Date			date;
                        int				cellPhone;
                        int pos_ini = 0, pos_final = 0;

                        getline(publ, line);

                        pos_ini = line.find('\t') + 1;
                        pos_final = line.find('\t', pos_ini);
                        cellPhone = (900000000 + stoi(line.substr(pos_ini, pos_final - pos_ini)));

                        pos_ini = pos_final + 1;
                        pos_final = line.find('\t', pos_ini);
                        newPubl.setTitle(line.substr(pos_ini, pos_final - pos_ini));

                        pos_ini = pos_final + 1;
                        pos_final = line.find('\t', pos_ini);
                        newPubl.setDesc(line.substr(pos_ini, pos_final - pos_ini));

                        pos_ini = pos_final + 1;
                        pos_final = line.find('\t', pos_ini);
                        fecha = line.substr(pos_ini, pos_final - pos_ini);

                        pos_ini = pos_final + 1;
                        pos_final = line.length();
                        newPubl.setNumLikes(stoi(line.substr(pos_ini, pos_final - pos_ini)));

                        pos_ini = pos_final = 0;
                        pos_final = fecha.find("-");
                        date.setYear(stoi(fecha.substr(pos_ini, pos_final - pos_ini)));

                        pos_ini = pos_final + 1;
                        pos_final = fecha.find("-", pos_ini);
                        date.setMonth(stoi(fecha.substr(pos_ini, pos_final - pos_ini)));

                        pos_ini = pos_final + 1;
                        pos_final = fecha.length();
                        date.setDay(stoi(fecha.substr(pos_ini, pos_final - pos_ini)));

                        newPubl.setPubDate(date);

                        UserServer& original = avlUCellPhone->findOriginal(cellPhone);
                        newPubl.setID(original.getPublications().size());

                        avlUEmail->findOriginal(original.getEmail()).addPublication(newPubl);
                        original.addPublication(newPubl);
                        avlUIDFilter->findOriginal(original.getIDFilter()).addPublication(newPubl);
                    }
                }

                ifstream friends;
                friends.open("followers.txt", std::ios::in);

                if (friends.is_open()) {

                    string line;
                    getline(friends, line);
                    while (!friends.eof()) {

                        int			idU;
                        int			idF;
                        string		emailU;
                        string		emailF;
                        string		codeU;
                        string		codeF;
                        int			pos_ini = 0, pos_final = 0;

                        getline(friends, line);
                        pos_final = line.find(",");
                        idU = 900000000 + stoi(line.substr(pos_ini, pos_final - pos_ini));

                        pos_ini = pos_final + 1;
                        idF = 900000000 + stoi(line.substr(pos_ini, line.length() - pos_ini));

                        UserServer &u = avlUCellPhone->findOriginal(idU);
                        UserServer &f = avlUCellPhone->findOriginal(idF);

                        if (!u.searchFriendExist(idF)) {

                            Friend ini;
                            ini.setCode(idF);
                            ini.setIsAdd(Friend::Condition::confirmed);

                            Friend fin;
                            fin.setCode(idU);
                            fin.setIsAdd(Friend::Condition::confirmed);

                            emailU = u.getEmail();
                            emailF = f.getEmail();
                            codeU = u.getIDFilter();
                            codeF = f.getIDFilter();

                            avlUEmail->findOriginal(emailU).addFriend(ini);
                            avlUEmail->findOriginal(emailF).addFriend(fin);
                            u.addFriend(ini);
                            f.addFriend(fin);
                            avlUIDFilter->findOriginal(codeU).addFriend(ini);
                            avlUIDFilter->findOriginal(codeF).addFriend(fin);
                        }
                    }
                }

                ifstream comments;
                comments.open("comment.txt", std::ios::in);

                if (comments.is_open()) {

                    string line;
                    getline(comments, line);
                    while (!comments.eof()) {

                        int			idPublic;		//PUBLICACION DONDE SE COMENTÓ
                        int			idComment;		//PERSONA QUIEN COMENTÓ
                        string		fecha;
                        Date		date;
                        Comment		comment;
                        int			pos_ini = 0, pos_final = 0;

                        getline(comments, line);

                        pos_final = line.find('\t');
                        idPublic = 900000000 + stoi(line.substr(pos_ini, pos_final - pos_ini));

                        UserServer& i = avlUCellPhone->findOriginal(idPublic);

                        if (i.getPublications().size() != 0) {

                            pos_ini = pos_final + 1;
                            pos_final = line.find('\t', pos_ini);
                            idComment = 900000000 + stoi(line.substr(pos_ini, pos_final - pos_ini));

                            pos_ini = pos_final + 1;
                            pos_final = line.find('\t', pos_ini);
                            fecha = line.substr(pos_ini, pos_final - 1);

                            pos_ini = pos_final + 1;
                            comment.setText(line.substr(pos_ini, line.length() - pos_ini));

                            pos_ini = pos_final = 0;
                            pos_final = fecha.find("-");
                            date.setYear(stoi(fecha.substr(pos_ini, pos_final - pos_ini)));

                            pos_ini = pos_final + 1;
                            pos_final = fecha.find("-", pos_ini);
                            date.setMonth(stoi(fecha.substr(pos_ini, pos_final - pos_ini)));

                            pos_ini = pos_final + 1;
                            pos_final = fecha.length();
                            date.setDay(stoi(fecha.substr(pos_ini, pos_final - pos_ini)));

                            comment.setDate(date);
                            comment.setAutor(avlUCellPhone->find(idComment).getFullName());
                            int num_publ = rand() % i.getPublications().size();
                            i.getPublications()[num_publ].addComment(comment);
                            avlUEmail->findOriginal(i.getEmail()).getPublications()[num_publ].addComment(comment);
                            avlUIDFilter->findOriginal(i.getIDFilter()).getPublications()[num_publ].addComment(comment);

                        }
                    }
                }
    }

    void FillVectorPubl(Client& newClient) {
            newClient.getAllPub().clear();
            for (unsigned int i = 0; i < newClient.getUserData().getFriends().size(); i++) {
                if (newClient.getUserData().getFriends()[i].getIsAdd() == Friend::confirmed) {
                    for (unsigned int j = 0; j < avlUCellPhone->find(newClient.getUserData().getFriends()[i].getCode()).getPublications().size(); j++) {

                        Publication n = avlUCellPhone->find(newClient.getUserData().getFriends()[i].getCode()).getPublications()[j];
                        PublicationUser newPUser(n.getID(), avlUCellPhone->find(newClient.getUserData().getFriends()[i].getCode()).getFullName(),
                            newClient.getUserData().getFriends()[i].getCode(), n.getTitle(), n.getDesc(), n.getPubDate(), n.getNumLikes());
                        newPUser.setComments(n.getComments());

                        newClient.addAllPubl(newPUser);
                    }
                }
            }
        }

    vector<User> GenerateListFeed(User& newUser) {

        string phase;
        string idFilter = avlUCellPhone->find(newUser.getCellPhone()).getIDFilter();

        if (idFilter.substr(0, 2) == "HH") {
            phase = idFilter;
            avlUIDFilter->preOrderPersonalized(phase, 100);
        }
        else if (idFilter.substr(0, 2) == "HM") {
            phase = "MH." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
            phase = "MA." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
        }
        else if (idFilter.substr(0, 2) == "HA") {
            phase = "MH." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
            phase = "HH." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
        }
        else if (idFilter.substr(0, 2) == "MH") {
            phase = "HM." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
            phase = "HA." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
        }
        else if (idFilter.substr(0, 2) == "MM") {
            phase = idFilter;
            avlUIDFilter->preOrderPersonalized(phase, 100);
        }
        else if (idFilter.substr(0, 2) == "MA") {
            phase = "HM." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
            phase = "MM." + idFilter.substr(3, 6);
            avlUIDFilter->preOrderPersonalized(phase, 100);
        }

        vector<UserServer>		Feed = avlUIDFilter->getPeople();
        vector<User>			destino;								//SE COMPARA UN ENTERO

        for (unsigned int i = 0; i < Feed.size(); i++) {
            destino.push_back(Feed[i]);
        }
        avlUIDFilter->clearList();										//LIMPIAR VECTOR

        for (unsigned int i = 0; i < newUser.getFriends().size(); i++) {
            if (newUser.getFriends()[i].getIsAdd() == Friend::Condition::pendient) {
                UserServer aux = avlUCellPhone->find(newUser.getFriends()[i].getCode());
                for (unsigned int i = 0; i < destino.size(); i++) {
                    if (destino[i].getCellPhone() == aux.getCellPhone()) {
                        destino.erase(destino.begin() + i);
                        destino.insert(destino.begin(), aux);
                    }
                }
            }
            else if (newUser.getFriends()[i].getIsAdd() == Friend::Condition::no_confirmed) {
                UserServer aux = avlUCellPhone->find(newUser.getFriends()[i].getCode());
                for (unsigned int i = 0; i < destino.size(); i++) {
                    if (destino[i].getCellPhone() == aux.getCellPhone())
                        destino.erase(destino.begin() + i);
                }
            }
            else if (newUser.getFriends()[i].getIsAdd() == Friend::Condition::confirmed) {
                UserServer aux = avlUCellPhone->find(newUser.getFriends()[i].getCode());
                for (unsigned int i = 0; i < destino.size(); i++) {
                    if (destino[i].getCellPhone() == aux.getCellPhone())
                        destino.erase(destino.begin() + i);
                }
            }
        }

        for (unsigned int i = 0; i < destino.size(); i++) {
            if (destino[i].getCellPhone() == newUser.getCellPhone())
                destino.erase(destino.begin() + i);
        }

        return destino;
    }

    void FillListFriendData(string fullname, Client& newClient) {
        newClient.getListFriendData().clear();
        newClient.getTreeFriendData()->preOrderPersonalized(fullname, [](User user, int tmn) { return user.getFullName().substr(0, tmn); });
        newClient.setlistFriend(newClient.getTreeFriendData()->getPeople());
        newClient.getTreeFriendData()->clearList();
        }

    void addUser(string fullName, string email, string password, User::Gender gender, User::pGender prefGender,
        bool showGender, unsigned short age, unsigned int cellPhone, User::University studyCenter, Date registerDate, vector<User::Hobbies> hobbies) {

        UserServer newUser(fullName, email, password, gender, prefGender, showGender, age, cellPhone,
            studyCenter, registerDate, hobbies, vector<Friend>(), vector<Publication>());


        addUser(newUser);
    }

    void addPublication(unsigned int codeUser, Publication newP) {
        UserServer userIn = avlUCellPhone->find(codeUser);
        avlUCellPhone->findOriginal(codeUser).addPublication(newP);
        avlUEmail->findOriginal(userIn.getEmail()).addPublication(newP);
    }
    void addComment(unsigned int codeUser, unsigned int IDPub, Comment commnet) {
        UserServer userIn = avlUCellPhone->find(codeUser);
        avlUCellPhone->findOriginal(codeUser).getPublications()[IDPub].addComment(commnet);
        avlUEmail->findOriginal(userIn.getEmail()).getPublications()[IDPub].addComment(commnet);
    }

    void SaveAll() {
        avlUEmail->inOrderPersonalized(avlUEmail->size());
        vector<UserServer> save = avlUEmail->getPeople();
        avlUEmail->clearList();
        remove(dataBase.c_str());
        for (unsigned int i = 0; i < save.size(); i++)
            writeInDataBase(save[i]);
    }

    void addFriend(Client& actually)
    {
        User x; actually.getActually(x);
        UserServer _friend = avlUCellPhone->find(x.getCellPhone());
        UserServer yo = avlUCellPhone->find(actually.getUserData().getCellPhone());

        if (actually.getUserData().searchFriendPendient(x.getCellPhone())) {

           int code = actually.getUserData().getCellPhone();

            //----OPERACION EN LA OTRA PERSONA-------
            avlUEmail->findOriginal(_friend.getEmail()).setFriendStatus(code, Friend::Condition::confirmed);
            avlUCellPhone->findOriginal(_friend.getCellPhone()).setFriendStatus(code, Friend::Condition::confirmed);
            avlUIDFilter->findOriginal(_friend.getIDFilter()).setFriendStatus(code, Friend::Condition::confirmed);

            code = x.getCellPhone();

            //----OPERACION EN MI PERFIL------
            actually.getUserData().setFriendStatus(code, Friend::Condition::confirmed);
            avlUEmail->findOriginal(actually.getUserData().getEmail()).setFriendStatus(code, Friend::Condition::confirmed);
            avlUCellPhone->findOriginal(actually.getUserData().getCellPhone()).setFriendStatus(code, Friend::Condition::confirmed);
            avlUIDFilter->findOriginal(yo.getIDFilter()).setFriendStatus(code, Friend::Condition::confirmed);

            actually.addTreeFriendData(avlUEmail->find(x.getEmail()));
        }
        else {
            //----OPERACION EN LA OTRA PERSONA-------
                Friend newFriend;
                newFriend.setCode(actually.getUserData().getCellPhone());
                newFriend.setIsAdd(Friend::Condition::pendient);
                x.addFriend(newFriend);
                avlUEmail->findOriginal(x.getEmail()).addFriend(newFriend);
                avlUCellPhone->findOriginal(x.getCellPhone()).addFriend(newFriend);
                avlUIDFilter->findOriginal(_friend.getIDFilter()).addFriend(newFriend);


                //----OPERACION EN MI PERFIL------
                newFriend.setCode(x.getCellPhone());
                newFriend.setIsAdd(Friend::Condition::no_confirmed);
                actually.getUserData().addFriend(newFriend);
                avlUEmail->findOriginal(actually.getUserData().getEmail()).addFriend(newFriend);
                avlUCellPhone->findOriginal(actually.getUserData().getCellPhone()).addFriend(newFriend);
                avlUIDFilter->findOriginal(_friend.getIDFilter()).addFriend(newFriend);
        }
    }

private:

    void GenerateTreeFriend(Client& newClient) {
        for (unsigned int i = 0; i < newClient.getUserData().getFriends().size(); i++) {
            if (newClient.getUserData().getFriends()[i].getIsAdd() == Friend::Condition::confirmed) {
                newClient.addTreeFriendData(avlUCellPhone->find(newClient.getUserData().getFriends()[i].getCode()));
            }
        }
    }

    void writeInDataBase(UserServer newUser) {
        ofstream db;
        db.open(dataBase.c_str(), std::ios::app);
        if (db.is_open()) {
            db << "<user>\n";
            db << "\t<idfilter>"	<< newUser.getIDFilter()	<< "</idfilter>\n";
            db << "\t<fullname>"	<< newUser.getFullName()	<< "</fullname>\n";
            db << "\t<email>"		<< newUser.getEmail()		<< "</email>\n";
            db << "\t<password>"	<< newUser.getPassword()	<< "</password>\n";
            db << "\t<gender>"		<< newUser.getGender()		<< "</gender>\n";
            db << "\t<prefgender>"	<< newUser.getPrefGender()	<< "</prefgender>\n";
            db << "\t<showgender>"	<< newUser.getShowGender()	<< "</showgender>\n";
            db << "\t<age>"			<< newUser.getAge()			<< "</age>\n";
            db << "\t<cellphone>"	<< newUser.getCellPhone()	<< "</cellphone>\n";
            db << "\t<studycenter>" << newUser.getStudyCenter() << "</studycenter>\n";
            db << "\t<registerdate>\n";
            db << "\t\t<day>"		<< newUser.getRegisterDate().getDay()	<< "</day>\n";
            db << "\t\t<month>"		<< newUser.getRegisterDate().getMonth() << "</month>\n";
            db << "\t\t<year>"		<< newUser.getRegisterDate().getYear()	<< "</year>\n";
            db << "\t</registerdate>\n";

            db << "\t<hobbies>\n";
            for (User::Hobbies hobbie : newUser.getHobbies()) {
                db << "\t\t<hobbie>" << hobbie << "</hobbie>\n";
            }
            db << "\t</hobbies>\n";

            db << "\t<friends>\n";
            for (Friend _friend : newUser.getFriends()) {
                db << "\t\t<friend>\n";
                db << "\t\t\t<code>" << _friend.getCode() << "</code>\n";
                db << "\t\t\t<isadd>" << _friend.getIsAdd() << "</isadd>\n";
                db << "\t\t</friend>\n";
            }
            db << "\t</friends>\n";

            db << "\t<publications>\n";
            for (Publication publication : newUser.getPublications()) {
                db << "\t\t<publication>\n";
                db << "\t\t\t<ID>"			<< publication.getID()					<< "</ID>\n";
                db << "\t\t\t<title>"		<< publication.getTitle()				<< "</title>\n";
                db << "\t\t\t<desc>"		<< publication.getDesc()				<< "</desc>\n";
                db << "\t\t\t<pubdate>\n";
                db << "\t\t\t\t<day>"		<< publication.getPubDate().getDay()	<< "</day>\n";
                db << "\t\t\t\t<month>"		<< publication.getPubDate().getMonth()	<< "</month>\n";
                db << "\t\t\t\t<year>"		<< publication.getPubDate().getYear()	<< "</year>\n";
                db << "\t\t\t</pubdate>\n";
                db << "\t\t\t<numlikes>"	<< publication.getNumLikes()			<< "</numlikes>\n";

                db << "\t\t\t<comments>\n";
                for (Comment comment : publication.getComments()) {
                    db << "\t\t\t\t<comment>\n";
                    db << "\t\t\t\t\t<autor>" << comment.getAutor() << "</autor>\n";
                    db << "\t\t\t\t\t<text>" << comment.getText() << "</text>\n";
                    db << "\t\t\t\t\t<commdate>\n";
                    db << "\t\t\t\t\t\t<day>" << comment.getDate().getDay() << "</day>\n";
                    db << "\t\t\t\t\t\t<month>" << comment.getDate().getMonth() << "</month>\n";
                    db << "\t\t\t\t\t\t<year>" << comment.getDate().getYear() << "</year>\n";
                    db << "\t\t\t\t\t</commdate>\n";
                    db << "\t\t\t\t</comment>\n";
                }
                db << "\t\t\t</comments>\n";

                db << "\t\t\t<interactions>\n";
                for (Interaction interaction : publication.getInteractions()) {
                    db << "\t\t\t\t<interaction>\n";
                    db << "\t\t\t\t\t<intdate>\n";
                    db << "\t\t\t\t\t\t<day>" << interaction.getDate().getDay() << "</day>\n";
                    db << "\t\t\t\t\t\t<month>" << interaction.getDate().getMonth() << "</month>\n";
                    db << "\t\t\t\t\t\t<year>" << interaction.getDate().getYear() << "</year>\n";
                    db << "\t\t\t\t\t</intdate>\n";
                    db << "\t\t\t\t\t<shared>" << interaction.getShared() << "</shared>\n";
                    db << "\t\t\t\t</interaction>\n";
                }
                db << "\t\t\t</interactions>\n";

                db << "\t\t</publication>\n";
            }
            db << "\t</publications>\n";
            db << "</user>\n";
        }
    } //Escribe usuarios nuevos

    void readFromDataBase() {
        ifstream db;
        db.open(dataBase.c_str(), std::ios::in);

        if (db.is_open()) {

            pLabel					label;
            UserServer				user;
            Date					date;
            vector<User::Hobbies>	vecHobbies;
            vector<Friend>			vecFriends;
            vector<Publication>		vecPublications;
            Friend					_friend;
            Publication				publication;
            vector<Comment>			vecComments;
            Comment					comment;
            vector<Interaction>		vecInteractions;
            Interaction				interaction;

            while (!db.eof()) {

                string line;
                getline(db, line);

                if (line.size() > 0) {

                    label.process(line);

                    if (label.getName() == "user") {
                        if (label.is_open()) {
                            user;
                        }
                        else if (label.is_close()) {
                            avlUEmail->add(user);
                            avlUCellPhone->add(user);
                            avlUIDFilter->add(user);
                        }
                    }
                    else if (label.getName() == "idfilter") {
                        label.getValue();
                        user.setIDFilter(label.getValue());
                    }
                    else if (label.getName() == "fullname") {
                        user.setFullName(label.getValue());
                    }
                    else if (label.getName() == "email") {
                        user.setEmail(label.getValue());
                    }
                    else if (label.getName() == "password") {
                        user.setPassword(label.getValue());
                    }
                    else if (label.getName() == "gender") {
                        user.setGender(User::Gender(atoi(label.getValue().c_str())));
                    }
                    else if (label.getName() == "prefgender") {
                        user.setPrefGender(User::pGender(atoi(label.getValue().c_str())));
                    }
                    else if (label.getName() == "age") {
                        user.setAge(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "cellphone") {
                        user.setCellPhone(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "studycenter") {
                        user.setStudyCenter(User::University(atoi(label.getValue().c_str())));
                    }
                    else if (label.getName() == "registerdate") {
                        if (label.is_close()) {
                            user.setRegisterDate(date);
                        }
                    }
                    else if (label.getName() == "day") {
                        date.setDay(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "month") {
                        date.setMonth(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "year") {
                        date.setYear(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "hobbies") {
                        if (label.is_open()) {
                            vecHobbies.clear();
                        }
                        else if (label.is_close()) {
                            user.setHobbies(vecHobbies);
                        }
                    }
                    else if (label.getName() == "hobbie") {
                        vecHobbies.push_back(User::Hobbies(atoi(label.getValue().c_str())));
                    }
                    else if (label.getName() == "friends") {
                        if (label.is_open()) {
                            vecFriends.clear();
                        }
                        else if (label.is_close()) {
                            user.setFriends(vecFriends);
                        }
                    }
                    else if (label.getName() == "friend") {
                        if (label.is_close()) {
                            vecFriends.push_back(_friend);
                        }
                    }
                    else if (label.getName() == "code") {
                        _friend.setCode(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "isadd") {
                        _friend.setIsAdd(Friend::Condition(atoi(label.getValue().c_str())));
                    }
                    else if (label.getName() == "publications") {
                        if (label.is_open()) {
                            vecPublications.clear();
                        }
                        else if (label.is_close()) {
                            user.setPublications(vecPublications);
                        }
                    }
                    else if (label.getName() == "publication") {
                        if (label.is_close()) {
                            vecPublications.push_back(publication);
                        }
                    }
                    else if (label.getName() == "ID") {
                        publication.setID(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "title"){
                        publication.setTitle(label.getValue());
                    }
                    else if (label.getName() == "desc") {
                        publication.setDesc(label.getValue());
                    }
                    else if (label.getName() == "pubdate") {
                        if (label.is_close()) {
                            publication.setPubDate(date);
                        }
                    }
                    else if (label.getName() == "numlikes") {
                        publication.setNumLikes(atoi(label.getValue().c_str()));
                    }
                    else if (label.getName() == "comments") {
                        if (label.is_open()) {
                            vecComments.clear();
                        }
                        else if (label.is_close()) {
                            publication.setComments(vecComments);
                        }
                    }
                    else if (label.getName() == "comment") {
                        if (label.is_close()) {
                            vecComments.push_back(comment);
                        }
                    }
                    else if (label.getName() == "autor") {
                        comment.setAutor(label.getValue());
                    }
                    else if (label.getName() == "text") {
                        comment.setText(label.getValue());
                    }
                    else if (label.getName() == "commdate") {
                        if (label.is_close()) {
                            comment.setDate(date);
                        }
                    }
                    else if (label.getName() == "interactions") {
                        if (label.is_open()) {
                            vecInteractions.clear();
                        }
                        else if (label.is_close()) {
                            publication.setInteractions(vecInteractions);
                        }
                    }
                    else if (label.getName() == "interaction") {
                        if (label.is_close()) {
                            vecInteractions.push_back(interaction);
                        }
                    }
                    else if (label.getName() == "intdate") {
                        if (label.is_close()) {
                            interaction.setDate(date);
                        }
                    }
                    else if (label.getName() == "shared") {
                        interaction.setShared(atoi(label.getValue().c_str()));
                    }

                    label.clear();

                }
            }
        }
    } //Lee todos

    void addUser(UserServer newUser) {
        writeInDataBase(newUser);
        avlUEmail->add(newUser);
        avlUCellPhone->add(newUser);
        avlUIDFilter->add(newUser);
    }
};

#endif // SERVER_H
