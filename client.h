#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "avl.h"
#include "publicationuser.h"

class Client
{
private:
    User                        userData;				//All user information (register)
    unsigned int                pos;					//List feed position 
    vector<User>                listFeed;				//List of add persons (pendents first, then compatibles)
    Avl<User, string>*          treefriendData;			//Avl of friends (UserClients)
    vector<User>                listfriendsData;
    vector<PublicationUser>     AllPubl;				//Vector of all publications

public:
    Client() {
        treefriendData = new Avl<User, string> ([](User user) {return user.getFullName(); });
        pos = 0;
    }
    ~Client() {
    }
    void	setUserData			(User newUser)						{ userData = newUser;       }
    void	setlistFeed			(vector<User> newList)				{ listFeed = newList;       }
    void	setlistFriend		(vector<User> newList)              { listfriendsData = newList;}
    void	setAllPubl			(vector<PublicationUser> newAllPub)	{ AllPubl = newAllPub;      }
    void	setTreeFriendData	(Avl<User, string>* newTree)		{ newTree = treefriendData; }
    void	addAllPubl			(PublicationUser r)					{ AllPubl.push_back(r);     }
    void	addTreeFriendData	(User r)							{ treefriendData->add(r);   }
    void	addListFeed			(User r)							{ listFeed.push_back(r);    }

    User&						getUserData()						{ return userData;          }
    User&                       getFeedActually()                   { return listFeed[pos];     }
    vector<User>&				getlistFeed()						{ return listFeed;          }
    Avl<User, string>*&         getTreeFriendData()                 { return treefriendData;    }
    vector<User>&               getListFriendData()                 { return listfriendsData;   }
    vector<PublicationUser>&	getAllPub()							{ return AllPubl;           }

    vector<Publication> generateMyPListRecent()
    {
        auto key = [](Publication user) {
            return -1 * (user.getPubDate().getYear()*10000 + user.getPubDate().getMonth()*100 + user.getPubDate().getDay());
        };
        Avl<Publication, int> treeDate(key);
        for (unsigned int i = 0; i < userData.getPublications().size(); i++) {
            treeDate.add(userData.getPublications()[i]);
        }
        treeDate.inOrderPersonalized(50);
        return treeDate.getPeople();
    }

    vector<Publication> generateMyPListLikes()
    {
        Avl<Publication, int> treeLikes([](Publication user) {return (-1*user.getNumLikes()); });;
        for (unsigned int i = 0; i < userData.getPublications().size(); i++) {
            treeLikes.add(userData.getPublications()[i]);
        }
        treeLikes.inOrderPersonalized(50);
        return treeLikes.getPeople();
    }

    vector<Publication> generateMyPListRelev()
    {
        Avl<Publication, int> treeRelev([](Publication user) {return -1*(user.getComments().size() + user.getNumLikes()); });
        for (unsigned int i = 0; i < userData.getPublications().size(); i++) {
            treeRelev.add(userData.getPublications()[i]);
        }
        treeRelev.inOrderPersonalized(50);
        return treeRelev.getPeople();
    }

    bool MoveNext(User& user) {
        if (pos >= 0 && pos < listFeed.size() - 1 && listFeed.size() > 0) {
            pos++; user = listFeed[pos];
            return true;
        }
        return false;
    }

    bool MoveBack(User& user) {
        if (pos > 0 && pos < listFeed.size()) {
            pos--; user = listFeed[pos];
            return true;
        }
        return false;
    }

    bool getActually(User& user) {
         if (listFeed.size() > 0) {
             user = listFeed[pos];
             return true;
         }
         return false;
     }

    void removeActually() {
        if (listFeed.size() > 0) {
            listFeed.erase(listFeed.begin() + pos); if (pos == listFeed.size() && pos > 0) pos--;
        }
    }

};

#endif // CLIENT_H
