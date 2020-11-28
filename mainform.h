#ifndef MAINFORM_H
#define MAINFORM_H


#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFileDialog>
#include <QWidget>
#include <QKeyEvent>
#include <QScrollArea>
#include "searchbar.h"
#include "server.h"
#include "client.h"
#include "ifriend.h"
#include "icomment.h"
#include "ipublication.h"
#include "textpublication.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainForm; }
QT_END_NAMESPACE

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    struct DataUser {
        QString                 photo;
        string                  fullName;
        string                  email;
        string                  password;
        User::Gender            gender;
        bool                    showgender;
        User::pGender           pGender;
        vector<User::Hobbies>   vecHobbies;
        User::University        centerStudy;
        unsigned short          age;
        unsigned int            cellPhone;
        DataUser() {
            gender = User::Gender::None;
            showgender = false;
            pGender = User::pGender::N;
            centerStudy = User::University::NONE;
        }
    };

private:
    Server*                     server = new Server();
    Client                      client;
    DataUser*                   otherData = new DataUser();
    searchBar*                  BarSearch;
    unsigned int                lastPage = 0;
    unsigned int                pubPos;
    unsigned int                codeUser;
    vector<iFriend*>            vecFriends;
    vector<iPublication*>       vecPublications;
    vector<iComment*>           vecComments;
    QLayoutItem*                verticalSpacer = nullptr;
    QLayoutItem*                verticalSpacer2 = nullptr;
    QLayoutItem*                verticalSpacer3 = nullptr;
    QLayoutItem*                verticalSpacer4 = nullptr;
    TextPublication*            boxAddPublication;


public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

    void clearWidgetOfRegister();
    void selectHobbie(QWidget*& widget, User::Hobbies hobbie);
    void savePhotoUser(QString address, QString codePhone);
    void processRegister();
    QString searchPhoto(int codeUser);

    void uploatComments(vector<Comment> vec);
    void uploadDataHome();
    void uploadPerfilAux(User user);
    void uploadPerfilPri(User user);

    QString getHobbieInText(User::Hobbies hob);
    QString getUniversityInText(User::University uni);
    QString getGenderInText(User::Gender gen);

    void removeiFriends();
    void removeiPublications(QWidget* wid);
    void removeiComments();

    void addFriendToStruct(vector<User> f);
    void addPublicationToStruct(vector<Publication> f, QScrollArea* widget, User user);
    void addAllFriends();
    void PublicationAllUserStruct(vector<PublicationUser> vecUser);

    QImage uploadImages();

signals:

private slots:

    void addPublication();

    void filterTextChange();

    void goToTheComments(unsigned int,unsigned int);

    void giveLike(unsigned int, unsigned int);

    void filterWithText(string data);

    void generatePerfilFriend(unsigned int);

    void generateIntefaceNews();

    void on_btnConCorreo_clicked();

    void on_btnRegistrarse_clicked();

    void on_btnContinuar01_clicked();

    void on_btnVolverAtras_3_clicked();

    void on_btnAceptarCondiciones_clicked();

    void on_btnVolverAtras_4_clicked();

    void on_btnContinuar02_clicked();

    void on_btnVolverAtras_5_clicked();

    void on_btnContinuar03_clicked();

    void on_btnSelectNombre_clicked();

    void on_btnSelectMujer_clicked();

    void on_btnContinuar04_clicked();

    void on_btnVolverAtras_7_clicked();

    void on_btnContinuar05_clicked();

    void on_btnVolverAtras_8_clicked();

    void on_btnPreferMujer_clicked();

    void on_btnPreferHombres_clicked();

    void on_btnPreferTodos_clicked();

    void on_btnHA_clicked();

    void on_btnHB_clicked();

    void on_btnHC_clicked();

    void on_btnHD_clicked();

    void on_btnHE_clicked();

    void on_btnHF_clicked();

    void on_btnHH_clicked();

    void on_btnHG_clicked();

    void on_btnHI_clicked();

    void on_btnHJ_clicked();

    void on_btnHK_clicked();

    void on_btnHL_clicked();

    void on_btnHM_clicked();

    void on_btnHN_clicked();

    void on_btnHNN_clicked();

    void on_btnHO_clicked();

    void on_btnHP_clicked();

    void on_btnHQ_clicked();

    void on_btnHR_clicked();

    void on_btnHS_clicked();

    void on_btnHT_clicked();

    void on_btnHU_clicked();

    void on_btnHV_clicked();

    void on_btnHW_clicked();

    void on_btnHX_clicked();

    void on_btnHY_clicked();

    void on_btnHZ_clicked();

    void on_btnHAA_clicked();

    void on_btnHAB_clicked();

    void on_btnHAC_clicked();

    void on_btnHAD_clicked();

    void on_btnHAE_clicked();

    void on_btnVolverAtras_clicked();

    void on_btnConCorreo_2_clicked();

    void on_btnContinuar06_clicked();

    void on_btnVolverAtras_9_clicked();

    void on_btnVolverAtras_10_clicked();

    void on_btnContinuar07_2_clicked();

    void on_btnContinuar07_3_clicked();

    void on_btnContinuarPrincipal02_clicked();

    void on_btnHome_clicked();

    void on_btnFriends_clicked();

    void on_btnPerfil_clicked();

    void on_btnSettings_clicked();

    void on_btnNew_clicked();

    void on_btnRegisterUser_clicked();

    void on_btnVolverAtras_6_clicked();

    void on_btnConTelefono_clicked();

    void on_btnVolverAtras_2_clicked();

    void on_btnLogInWithCellPhone_clicked();

    void on_cbMostrarGenero_stateChanged(int arg1);

    void on_btnSelectFoto01_clicked();

    void on_btnNext_clicked();

    void on_btnBack_clicked();

    void on_btnPNext_clicked();

    void on_btnPBack_clicked();

    void on_btnSee_clicked();

    void on_pushVolveAtras_clicked();

    void on_btnMostRecent_clicked();

    void on_btnMostLikes_clicked();

    void on_btnMostRelevant_clicked();

    void on_btnMostRecent_2_clicked();

    void on_btnMostLikes_2_clicked();

    void on_btnMostRelevant_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainForm *ui;
};
#endif // MAINFORM_H
