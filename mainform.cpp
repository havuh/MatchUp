#include "mainform.h"
#include "ui_mainform.h"
#include "funtionsHelp.h"
#include <fstream>


using std::ifstream;
using std::ofstream;
using std::string;

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{

    ui->setupUi(this);

    addComboBoxDays(ui->cbFDay);       //Agregar dias al QComboBox
    addComboBoxYears(ui->cbFYear);     //Agregar años al QComboBox

    //BARRA DE BUSQUEDA CON CODIGO
    BarSearch = new searchBar(ui->gbBarSearch);
    BarSearch->show();
    connect(BarSearch->getLineEdit(), &QLineEdit::textChanged, [this]() { filterTextChange(); });

    //BARRA PARA AGREGAR PUBLICACION
    boxAddPublication = new TextPublication(ui->groupBox);
    ui->groupBox->layout()->addWidget(boxAddPublication);
    boxAddPublication->show();
    connect(boxAddPublication->getPlainEdit(), &QTextEdit::textChanged, [this](){ addPublication(); });

}

MainForm::~MainForm()
{
}

void MainForm::addPublication()
{
    if (boxAddPublication->getPlainEdit()->toPlainText().toStdString().size() > 0){
        string text = boxAddPublication->getPlainEdit()->toPlainText().toStdString();
        string title = QString(QString("Publication " + QString::number(client.getUserData().getPublications().size()))).toStdString();
        string desc = boxAddPublication->getPlainEdit()->toPlainText().toStdString();
        Date date;
        date.setDay(QDateTime::currentDateTime().date().day());
        date.setMonth(QDateTime::currentDateTime().date().month());
        date.setYear(QDateTime::currentDateTime().date().year());

        if (text.at(text.size() - 1) == '\n'){
            Publication newP(client.getUserData().getPublications().size(), title, desc, date, 0);
            server->addPublication(client.getUserData().getCellPhone(), newP);
            client.getUserData().addPublication(newP);
            boxAddPublication->getPlainEdit()->setText("");
        }
    }
}

void MainForm::filterTextChange(){
    QString text = BarSearch->getLineEdit()->text();
    if (text != ""){
        removeiFriends();
        server->FillListFriendData(text.toStdString(), client);
        addFriendToStruct(client.getListFriendData());
    }
    else {
        removeiFriends();
        addAllFriends();
        addFriendToStruct(client.getListFriendData());
    }
}

QString MainForm::searchPhoto(int codeUser)
{
    ifstream photo;
    QString address = QString(QString("../build-SocialNetwork_Algorithms-Desktop_Qt_5_15_1_MSVC2015_64bit-Debug/") +
                              QString::number(codeUser) + QString(".png"));
    photo.open(address.toStdString(), std::ios::in);
    if (photo.is_open())    return address;
    else                    return QString(":/img/Resource/img/default-user.png");

}

void MainForm::goToTheComments(unsigned int ID, unsigned int codeUser) //AQUI
{
    removeiComments();
    this->pubPos = ID;
    this->codeUser = codeUser;
    Publication pub = server->getAVLCellPhone()->find(codeUser).getPublications()[ID];
    uploatComments(pub.getComments());
    ui->ContenedorTinder->setCurrentIndex(4);
    ui->PageOfHelp->setCurrentIndex(1);

}

void MainForm::giveLike(unsigned int codeUser, unsigned int codePublication)
{
    vector<Publication> pub = server->getAVLCellPhone()->find(codeUser).getPublications();
    for (unsigned int i = 0; i < pub.size(); i++)
    {
        if (pub.at(i).getID() == codePublication){
            server->getAVLCellPhone()->find(codeUser).getPublications()[i].addLike();
            User user;
            client.getActually(user);
            user.getPublications()[i].addLike();
            break;
        }
    }

}

void MainForm::filterWithText(string data)
{
    if (data != ""){
        removeiFriends();
        server->FillListFriendData(data, client);
        addFriendToStruct(client.getListFriendData());
    }
    else {
        removeiFriends();
        addAllFriends();
        addFriendToStruct(client.getListFriendData());
    }
}

void MainForm::clearWidgetOfRegister(){
    ui->txtCorreo->setText("");
    ui->txtPassword->setText("");
    ui->txtTelefono->setText("");
    ui->txtUserName->setText("");
    ui->cbFDay->setCurrentIndex(0);
    ui->cbFMonth->setCurrentIndex(0);
    ui->cbFYear->setCurrentIndex(0);
    ui->cbbPrefijo->setCurrentIndex(0);
    ui->cbMostrarGenero->setCheckState(Qt::Unchecked);
    ui->cbCentroEstudios->setCurrentIndex(0);
}

void MainForm::addAllFriends()
{
    client.getListFriendData().clear();
    vector<Friend> f = client.getUserData().getFriends();
    for (unsigned int i = 0; i < f.size(); i++){
        User u = server->getAVLCellPhone()->find(f.at(i).getCode());
        client.getListFriendData().push_back(u);
    }
}
//AQUI
void MainForm::generatePerfilFriend(unsigned int code)
{
    User FriendUser = server->getAVLCellPhone()->find(code);
    this->codeUser = code;
    uploadPerfilAux(FriendUser);
    ui->ContenedorTinder->setCurrentIndex(4);
    ui->PageOfHelp->setCurrentIndex(0);
}

void MainForm::removeiFriends()
{
    for (unsigned int i = 0; i < vecFriends.size(); i++){
        vecFriends.at(i)->hide();
        ui->saFriends->widget()->layout()->removeWidget(vecFriends.at(i));
    }

    if (verticalSpacer != nullptr){
        ui->saFriends->widget()->layout()->removeItem(verticalSpacer);
    }
    vecFriends.clear();
}
void MainForm::removeiPublications(QWidget* wid){
    for (unsigned int i = 0; i < vecPublications.size(); i++){
        vecPublications.at(i)->hide();
        wid->layout()->removeWidget(vecPublications.at(i));
    }
    if (verticalSpacer3 != nullptr){
        wid->layout()->removeItem(verticalSpacer3);
    }
    if (verticalSpacer2 != nullptr){
        wid->layout()->removeItem(verticalSpacer2);
    }
    vecPublications.clear();
}
void MainForm::removeiComments()
{
    for (unsigned int i = 0; i < vecComments.size(); i++){
        vecComments.at(i)->hide();
        ui->scrollArea_5->widget()->layout()->removeWidget(vecComments.at(i));
    }

    if (verticalSpacer4 != nullptr){
        ui->scrollArea_5->widget()->layout()->removeItem(verticalSpacer4);
    }
    vecComments.clear();
}
void MainForm::addFriendToStruct(vector<User> f)
{
    for (unsigned int i = 0; i < f.size(); i++)
    {
       iFriend* x = new iFriend(this);
       x->setText(QString(f.at(i).getFullName().c_str()), "", f.at(i).getCellPhone());
       x->setPhoto(searchPhoto(f.at(i).getCellPhone()));
       vecFriends.push_back(x); ui->saFriends->widget()->layout()->addWidget(x);
       connect(x, SIGNAL(clicked(unsigned int)), this, SLOT(generatePerfilFriend(unsigned int)));
    }
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->saFriends->widget()->layout()->addItem(verticalSpacer);
}
void MainForm::addPublicationToStruct(vector<Publication> p, QScrollArea* widget, User user)
{
    QGridLayout* ly = new QGridLayout;
    widget->widget()->setLayout(ly);
    for (unsigned int i = 0; i < p.size(); i++)
    {
        iPublication* pub = new iPublication(widget);
        QString date = QString::number(p.at(i).getPubDate().getDay()) + QString(" - ") + QString::number(p.at(i).getPubDate().getMonth()) +
                        QString(" - ") + QString::number(p.at(i).getPubDate().getYear());

        pub->setData(QString(user.getFullName().c_str()), date, QString(p.at(i).getTitle().c_str()), QString(p.at(i).getDesc().c_str()),
                     i, user.getCellPhone(), QString::number(p.at(i).getNumLikes()), QString::number(p.at(i).getComments().size()));
        widget->widget()->layout()->addWidget(pub);
        vecPublications.push_back(pub);
        connect(pub->getBtnLike(), SIGNAL(MoreLikes(unsigned int, unsigned int)), this, SLOT(giveLike(unsigned int, unsigned int)));
        connect(pub->getBtnComments(), SIGNAL(clicked(unsigned int, unsigned int)), this, SLOT(goToTheComments(unsigned int, unsigned int)));
        connect(pub->getBtnShared(), SIGNAL(clicked(int, int)), this, SLOT(slotButtonShared(int, int)));
    }
    verticalSpacer3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    widget->widget()->layout()->addItem(verticalSpacer3);
}

void MainForm::PublicationAllUserStruct(vector<PublicationUser> vecUser)
{
    boxAddPublication->setStyleSheet(QString(client.getUserData().getFullName().c_str()),
                                     searchPhoto(client.getUserData().getCellPhone()));
    for (unsigned int i = 0; i < vecUser.size(); i++)
    {
        Date date = vecUser.at(i).getPubDate();
        QString d = QString(QString::number(date.getDay()) + " - " + QString::number(date.getMonth()) + " - " + QString::number(date.getYear()));
        iPublication* pub = new iPublication(ui->scrollArea_3);
        pub->setData(QString(vecUser.at(i).fullName.c_str()), d, QString(vecUser.at(i).getTitle().c_str()), QString(vecUser.at(i).getDesc().c_str()),
                     vecUser.at(i).getID(), vecUser.at(i).codePhone, QString::number(vecUser.at(i).getNumLikes()), QString::number(vecUser.at(i).getComments().size()));

        ui->scrollArea_3->widget()->layout()->addWidget(pub);
        vecPublications.push_back(pub);
        connect(pub->getFullName(), SIGNAL(clicked(unsigned int)),this, SLOT(generatePerfilFriend(unsigned int)));
        connect(pub->getBtnLike(), SIGNAL(MoreLikes(unsigned int, unsigned int)), this, SLOT(giveLike(unsigned int, unsigned int)));
        connect(pub->getBtnComments(), SIGNAL(clicked(unsigned int, unsigned int)), this, SLOT(goToTheComments(unsigned int, unsigned int)));
        connect(pub->getBtnShared(), SIGNAL(clicked(int, int)), this, SLOT(slotButtonShared(int, int)));
    }
    verticalSpacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->scrollArea_3->widget()->layout()->addItem(verticalSpacer2);
}

void MainForm::selectHobbie(QWidget*& widget, User::Hobbies hobbie){
    int pos = -1;
    for (unsigned int i = 0; i < otherData->vecHobbies.size(); i++){
        if (otherData->vecHobbies.at(i) == hobbie){
            pos = i; break;
        }
    }
    if (pos != -1){
        otherData->vecHobbies.erase(otherData->vecHobbies.begin() + pos);
        widget->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    }
    else {
        if (otherData->vecHobbies.size() < 5){
            otherData->vecHobbies.push_back(hobbie);
            widget->setStyleSheet("background-color: white; color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); border-radius: 15px; border: 3px solid; border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1));");
        }
    }
}

void MainForm::savePhotoUser(QString address, QString codePhone){

    ifstream origin;
    origin.open(address.toStdString(), std::ios::in | std::ios::binary);

    ofstream destination;
    QString namePhoto = QString("../build-SocialNetwork_Algorithms-Desktop_Qt_5_15_1_MSVC2015_64bit-Debug/") + codePhone + QString(".png");
    destination.open(namePhoto.toStdString(), std::ios::out | std::ios::binary);

    char byte;
    while(!origin.eof() && !origin.fail()){
        origin.read((char *)&byte, sizeof(char));
        if (origin.good()){
            destination.write((char *)&byte, sizeof(char));
        }
    }
    destination.close();
    origin.close();
}

QImage MainForm::uploadImages()
{
    QImage image;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Buscar Foto de Perfil"),"", tr("Images (*.jpg *.jpeg *.png *.bmp)"));

    if (QString::compare(fileName, QString()) != 0){
        image.load(fileName);
        otherData->photo = fileName;
        return image;
    }
    else return QImage();
}
//AQUI
void MainForm::uploadPerfilAux(User user)
{
    ui->lblAFoto->setPixmap(QPixmap(searchPhoto(user.getCellPhone())));
    ui->lblAFoto->setScaledContents(true);
    ui->lblAFullName->setText(QString(user.getFullName().c_str()));
    ui->lblAEdad->setText(QString::number(user.getAge()));
    ui->lblAGenero->setText(getGenderInText(user.getGender()));
    ui->lblACellNumber->setText(QString::number(user.getCellPhone()));
    ui->lblACentroEstudio->setText(getUniversityInText(user.getStudyCenter()));

    QString date = QString(QString::number(user.getRegisterDate().getDay()) + " - " +
            QString::number(user.getRegisterDate().getMonth()) + " - " +
            QString::number(user.getRegisterDate().getYear()));
    ui->lblAFechaRegistro->setText(date);


    ui->lblAP1->setText(getHobbieInText(user.getHobbies()[0]));
    ui->lblAP2->setText(getHobbieInText(user.getHobbies()[1]));
    ui->lblAP3->setText(getHobbieInText(user.getHobbies()[2]));
    ui->lblAP4->setText(getHobbieInText(user.getHobbies()[3]));
    ui->lblAP5->setText(getHobbieInText(user.getHobbies()[4]));

    removeiPublications(ui->scrollArea_2->widget());
    removeiPublications(ui->scrollArea_4->widget());
    addPublicationToStruct(user.getPublications(), ui->scrollArea_4, user);
}

void MainForm::uploadPerfilPri(User user)
{
    ui->lblPhoto_2->setPixmap(QPixmap(searchPhoto(user.getCellPhone())));
    ui->lblPhoto_2->setScaledContents(true);
    ui->lblPFullName->setText(QString(user.getFullName().c_str()));
    ui->lblPEdad->setText(QString::number(user.getAge()));
    ui->lblPGenero->setText(getGenderInText(user.getGender()));
    ui->lblPCellPhone->setText(QString::number(user.getCellPhone()));
    ui->lblPCentroEstudio->setText(getUniversityInText(user.getStudyCenter()));
    ui->lblPFechaRegistro->setText(QString::number(user.getRegisterDate().getDay()) + " - " +
                                   QString::number(user.getRegisterDate().getMonth()) + " - " +
                                   QString::number(user.getRegisterDate().getYear()));

    ui->lblPP1->setText(getHobbieInText(user.getHobbies()[0]));
    ui->lblPP2->setText(getHobbieInText(user.getHobbies()[1]));
    ui->lblPP3->setText(getHobbieInText(user.getHobbies()[2]));
    ui->lblPP4->setText(getHobbieInText(user.getHobbies()[3]));
    ui->lblPP5->setText(getHobbieInText(user.getHobbies()[4]));

    removeiPublications(ui->scrollArea_4->widget());
    removeiPublications(ui->scrollArea_2->widget());
    vector<Publication> pub = client.generateMyPListRecent();
    addPublicationToStruct(pub, ui->scrollArea_2, user);
}

void MainForm::uploadDataHome()
{
    User UserToShow;
    if (client.getActually(UserToShow)){
        ui->lblFullName->setText(QString(UserToShow.getFullName().c_str()));
        ui->lblAge->setText(QString::number(UserToShow.getAge()));

        ui->lblIcon->setPixmap(QPixmap(":/img/Resource/img/graduacion-gray.png"));
        ui->lblCenterStudy->setText(getUniversityInText(UserToShow.getStudyCenter()));

        QString address = QString("../build-SocialNetwork_Algorithms-Desktop_Qt_5_15_1_MSVC2015_64bit-Debug/" +
                              QString::number(UserToShow.getCellPhone()) + QString(".png"));
        ifstream photo;
        photo.open(address.toStdString(), std::ios::in);

        if (photo.is_open()){
            ui->lblPhoto->setScaledContents(true);
            ui->lblPhoto->setPixmap(address);
        }
        else {
            QImage defauld = QImage(":/img/Resource/img/default-user.png");
            ui->lblPhoto->setScaledContents(false);
            ui->lblPhoto->setPixmap(QPixmap::fromImage(defauld));
        }
    }
    else {
        ui->lblPhoto->setText("NO HAY USUARIOS PARA MOSTRAR");
        ui->lblPhoto->setStyleSheet("background-color: #E0E0E0;");
        ui->lblFullName->setText("");
        ui->lblCenterStudy->setText("");
        ui->lblIcon->setPixmap(QPixmap());
        ui->lblAge->setText("");

    }
}

void MainForm::uploatComments(vector<Comment> vec)
{
    QGridLayout* ly = new QGridLayout;
    ui->scrollArea_5->widget()->setLayout(ly);
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        iComment* comm = new iComment;
        QString date = QString(QString::number(vec.at(i).getDate().getDay()) + " - " + QString::number(vec.at(i).getDate().getMonth()))
                +  " - " + QString::number(vec.at(i).getDate().getYear());
        comm->setData(QString(vec.at(i).getAutor().c_str()), date, QString(vec.at(i).getText().c_str()), i);
        comm->show();
        vecComments.push_back(comm);

        ui->scrollArea_5->widget()->layout()->addWidget(comm); //THIS LINE
    }
    verticalSpacer4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->scrollArea_5->widget()->layout()->addItem(verticalSpacer4);
}

void MainForm::processRegister() {

    //FECHA DE REGISTRO
    QDateTime now = QDateTime::currentDateTime();
    Date fRegister;
    fRegister.setDay(now.date().day());
    fRegister.setMonth(now.date().month());
    fRegister.setYear(now.date().year());

    //GUARDAR DATA
    server->addUser(otherData->fullName, otherData->email, otherData->password, otherData->gender, otherData->pGender,
                    otherData->showgender, otherData->age, otherData->cellPhone, otherData->centerStudy, fRegister, otherData->vecHobbies);
    savePhotoUser(otherData->photo, ui->txtTelefono->text());

    //COMPLETAR MENSAJE DE USUARIO CREADO
    ui->lblSUFullName->setText(QString(otherData->fullName.c_str()));

    //LIMPIAR CAJAS
    clearWidgetOfRegister();

    //PASAR A LA SIGUIENTE INTERFAS
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(8);

}

QString MainForm::getHobbieInText(User::Hobbies hob){
    if (hob == User::Hobbies::AireLibre)                return "Aire Libre";
    else if (hob == User::Hobbies::AmanteDeLosGatos)    return "Amante de los Gatos";
    else if (hob == User::Hobbies::Bailar)              return "Bailar";
    else if (hob == User::Hobbies::Bloquear)            return "Bloquear";
    else if (hob == User::Hobbies::Cafe)                return "Cafe";
    else if (hob == User::Hobbies::Caminar)             return "Caminar";
    else if (hob == User::Hobbies::Cocinar)             return "Cocinar";
    else if (hob == User::Hobbies::Deportes)            return "Deportes";
    else if (hob == User::Hobbies::Disney)              return "Disney";
    else if (hob == User::Hobbies::Entrenamiento)       return "Entretenimiento";
    else if (hob == User::Hobbies::Escritura)           return "Escritura";
    else if (hob == User::Hobbies::Fotografia)          return "Fotografia";
    else if (hob == User::Hobbies::Futbol)              return "Futbol";
    else if (hob == User::Hobbies::Gamer)               return "Gamer";
    else if (hob == User::Hobbies::Golf)                return "Golf";
    else if (hob == User::Hobbies::Karaoke)             return "Karaoke";
    else if (hob == User::Hobbies::Leer)                return "Leer";
    else if (hob == User::Hobbies::Manualidades)        return "Manualidades";
    else if (hob == User::Hobbies::Moda)                return "Moda";
    else if (hob == User::Hobbies::Museo)               return "Museo";
    else if (hob == User::Hobbies::Musica)              return "Musica";
    else if (hob == User::Hobbies::Nadar)               return "Nadar";
    else if (hob == User::Hobbies::Peliculas)           return "Peliculas";
    else if (hob == User::Hobbies::Reposteria)          return "Reposteria";
    else if (hob == User::Hobbies::SalirATomarAlgo)     return "Salir a tomar Algo";
    else if (hob == User::Hobbies::SalirDeCompras)      return "Salir de compras";
    else if (hob == User::Hobbies::Senderismo)          return "Senderismo";
    else if (hob == User::Hobbies::Te)                  return "Te";
    else if (hob == User::Hobbies::Trivia)              return "Trivia";
    else if (hob == User::Hobbies::Viajar)              return "Viajar";
    else if (hob == User::Hobbies::Vino)                return "Vino";
    else if (hob == User::Hobbies::Voluntariado)        return "Voluntariado";
    else                                                return "";
}

QString MainForm::getUniversityInText(User::University uni){
    if (uni == User::University::PUCP)          return "Pontificia Universidad Católica del Perú";
    else if (uni == User::University::UAC)      return "Universidad Andina del Cusco";
    else if (uni == User::University::UARM)     return "Universidad Antonio Ruiz de Montoya";
    else if (uni == User::University::UCAL)     return "Universidad de Ciencias y Artes de América Latina";
    else if (uni == User::University::UCH)      return "Universidad de Ciencias y Humaninades";
    else if (uni == User::University::UCSM)     return "Universidad Católica de Santa María";
    else if (uni == User::University::UCSP)     return "Universidad Católica San Pablo";
    else if (uni == User::University::UDA)      return "Universidad para el Desarrollo Andino";
    else if (uni == User::University::UDEP)     return "Universidad de Piura";
    else if (uni == User::University::UMCH)     return "Universidad Marcelino Champgnat ";
    else if (uni == User::University::UNAH)     return "Universidad Nacional del Altiplano";
    else if (uni == User::University::UNAJMA)   return "Universidad Nacional José María Arguedas";
    else if (uni == User::University::UNALM)    return "Universidad Nacional Agraria La Molina";
    else if (uni == User::University::UNAP)     return "Universidad Nacional del Altiplano";
    else if (uni == User::University::UNI)      return "Universidad Nacional de Ingeniería";
    else if (uni == User::University::UNIFE)    return "Universidad Femenina del Sagrado Corazón";
    else if (uni == User::University::UNJ)      return "Universidad Nacional de Jaén";
    else if (uni == User::University::UNTRM)    return "Universidad Nacional Toribio Rodriguez de Mendoza de Amazonas";
    else if (uni == User::University::UP)       return "Universidad del Pacífico";
    else if (uni == User::University::UPC)      return "Universidad Peruana de Ciencias Aplicadas";
    else if (uni == User::University::UPCH)     return "Universidad Peruana Cayetano Heredia";
    else if (uni == User::University::UPN)      return "Universidad Privada del Norte";
    else if (uni == User::University::UPT)      return "Universidad Privada de Tacna";
    else if (uni == User::University::URP)      return "Universidad Ricardo Palma";
    else if (uni == User::University::USIL)     return "Universidad San Ignacio de Loyola";
    else if (uni == User::University::USMP)     return "Universidad de San Martín de Porres";
    else if (uni == User::University::UTEC)     return "Universidad de Ingeniería y Tecnología";
    else                                        return "";
}

QString MainForm::getGenderInText(User::Gender gen){
    if (gen == User::Gender::Female)    return "Femenino";
    else                                return "Masculino";
}

void MainForm::on_btnConCorreo_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(2);
    ui->ContainerKindLogIn->setCurrentIndex(0);
}

void MainForm::on_btnRegistrarse_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
}

void MainForm::on_btnContinuar01_clicked()
{
    if (ui->txtCorreo->text() != "" && ui->txtPassword->text() != "" && ui->txtTelefono->text() != ""){

        if (validateExtensionMail(ui->txtCorreo->text())){

            if (!server->existThisEmail(ui->txtCorreo->text().toStdString())){ //validar que el correo no se haya resgistrado aun

                if (ui->txtPassword->text().length() >= 6) {

                    unsigned int phoneNumber = ui->txtTelefono->text().toUInt();
                    if (!server->existThisCellPhone(phoneNumber)){ //validar que el numero de celular no se haya registrado aun

                        //PASAR DATA
                        otherData->email = ui->txtCorreo->text().toStdString();
                        otherData->password = ui->txtPassword->text().toStdString();
                        otherData->cellPhone = ui->txtTelefono->text().toUInt();

                        //PASAR DE PAGINA
                        ui->ContenedorTinder->setCurrentIndex(1);
                        ui->ContenedorRegistrar ->setCurrentIndex(1);

                    }
                    else {
                        QMessageBox::information(this, "Tinder - Alertas", "El numero de celular proporcionado ya se encuentra en uso!", QMessageBox::Ok);
                    }
                }
                else {
                    QMessageBox::information(this, "Tinder - Alertas", "La contraseña debe de tener mas de 6 caracteres!", QMessageBox::Ok);
                }
            }
            else {
                QMessageBox::information(this, "Tinder - Alertas", "El correo ya se encuentra registrado!", QMessageBox::Ok);
            }
        }
        else {
            QMessageBox::information(this, "Tinder - Alertas", "La extención del correo no es valido!", QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this, "Tinder - Alertas", "Ingrese todos los datos solicitados por favor!", QMessageBox::Ok);
    }
}

void MainForm::on_btnVolverAtras_3_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(0);
}

void MainForm::on_btnAceptarCondiciones_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(2);
}

void MainForm::on_btnVolverAtras_4_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(0);
}

void MainForm::on_btnContinuar02_clicked()
{
    if (ui->txtUserName->text() != ""){
        if (ui->txtUserName->text().length() >= 3){

            int year = ui->cbFYear->currentText().toInt();
            if (QDateTime::currentDateTime().date().year() - year >= 18){

                otherData->fullName = ui->txtUserName->text().toStdString();
                otherData->age = QDateTime::currentDateTime().date().year() - year;

                ui->ContenedorTinder->setCurrentIndex(1);
                ui->ContenedorRegistrar->setCurrentIndex(3);
            }
            else {
                QMessageBox::information(this, "Tinder - Alertas", "Debe ser mayor de edad para registrarse!", QMessageBox::Ok);
            }
        }
        else {
            QMessageBox::information(this, "Tinder - Alertas", "Por favor ingrese un nombre de usuario valido!", QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this, "Tinder - Alertas", "Por favor ingrese un nombre de usuario!", QMessageBox::Ok);
    }
}

void MainForm::on_btnVolverAtras_5_clicked()
{

}

void MainForm::on_btnContinuar03_clicked()
{
    if (otherData->gender != User::Gender::None){
        ui->ContenedorTinder->setCurrentIndex(1);
        ui->ContenedorRegistrar->setCurrentIndex(4);
    }
    else {
        QMessageBox::information(this, "Tinder - Alertas", "Por favor seleccione su genero!", QMessageBox::Ok);
    }
}

void MainForm::on_btnSelectNombre_clicked()
{
    ui->btnSelectMujer->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    ui->btnSelectNombre->setStyleSheet("background-color: white; color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); border-radius: 15px; border: 3px solid; border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1));");
    otherData->gender = User::Gender::Male;
}

void MainForm::on_btnSelectMujer_clicked()
{
    ui->btnSelectMujer->setStyleSheet("background-color: white; color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); border-radius: 15px; border: 3px solid; border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1));");
    ui->btnSelectNombre->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    otherData->gender = User::Gender::Female;
}

void MainForm::on_btnContinuar04_clicked()
{
    if (otherData->pGender != User::pGender::N){
        ui->ContenedorTinder->setCurrentIndex(1);
        ui->ContenedorRegistrar->setCurrentIndex(5);
    }
    else {
        QMessageBox::information(this, "Tinder - Alertas", "Por favor seleccione una de las opciones!", QMessageBox::Ok);
    }
}

void MainForm::on_btnVolverAtras_7_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(3);
}

void MainForm::on_btnContinuar05_clicked()
{
    QMessageBox::StandardButton opcion;
    opcion = QMessageBox::question(this, "Tinder - Alertas", "Esta seguro que la " + ui->cbCentroEstudios->currentText() +
                                   " es su centro de estudios", QMessageBox::Ok | QMessageBox::Cancel);

    if (opcion == QMessageBox::Ok){
        //obteniendo el centro de estudios
        otherData->centerStudy = User::University(ui->cbCentroEstudios->currentIndex());

        //pasar a la siguiente pag
        ui->ContenedorTinder->setCurrentIndex(1);
        ui->ContenedorRegistrar->setCurrentIndex(6);
    }
}

void MainForm::on_btnVolverAtras_8_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(4);
}

void MainForm::on_btnPreferMujer_clicked()
{
    ui->btnPreferMujer->setStyleSheet("background-color: white; color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); border-radius: 15px; border: 3px solid; border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1));");
    ui->btnPreferHombres->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    ui->btnPreferTodos->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    otherData->pGender = User::pGender::F;
}

void MainForm::on_btnPreferHombres_clicked()
{
    ui->btnPreferMujer->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    ui->btnPreferHombres->setStyleSheet("background-color: white; color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); border-radius: 15px; border: 3px solid; border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1));");
    ui->btnPreferTodos->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    otherData->pGender = User::pGender::M;
}

void MainForm::on_btnPreferTodos_clicked()
{
    ui->btnPreferMujer->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    ui->btnPreferHombres->setStyleSheet("background-color: white; color: rgb(199, 199, 199); border-radius: 15px; border: 3px solid; border-color: rgb(221, 225, 228);");
    ui->btnPreferTodos->setStyleSheet("background-color: white; color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); border-radius: 15px; border: 3px solid; border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1));");
    otherData->pGender = User::pGender::Both;
}

void MainForm::on_btnHA_clicked()
{
    QWidget* MyWidged = ui->btnHA;
    selectHobbie(MyWidged, User::Hobbies::Musica);
}

void MainForm::on_btnHB_clicked()
{
    QWidget* MyWidged = ui->btnHB;
    selectHobbie(MyWidged, User::Hobbies::Leer);
}

void MainForm::on_btnHC_clicked()
{
    QWidget* MyWidged = ui->btnHC;
    selectHobbie(MyWidged, User::Hobbies::Vino);
}

void MainForm::on_btnHD_clicked()
{
    QWidget* MyWidged = ui->btnHD;
    selectHobbie(MyWidged, User::Hobbies::SalirATomarAlgo);
}

void MainForm::on_btnHE_clicked()
{
    QWidget* MyWidged = ui->btnHE;
    selectHobbie(MyWidged, User::Hobbies::Bailar);
}

void MainForm::on_btnHF_clicked()
{
    QWidget* MyWidged = ui->btnHF;
    selectHobbie(MyWidged, User::Hobbies::Escritura);
}

void MainForm::on_btnHG_clicked()
{
    QWidget* MyWidged = ui->btnHG;
    selectHobbie(MyWidged, User::Hobbies::Caminar);
}

void MainForm::on_btnHH_clicked()
{
    QWidget* MyWidged = ui->btnHH;
    selectHobbie(MyWidged, User::Hobbies::Manualidades);
}

void MainForm::on_btnHI_clicked()
{
    QWidget* MyWidged = ui->btnHI;
    selectHobbie(MyWidged, User::Hobbies::Cocinar);
}

void MainForm::on_btnHJ_clicked()
{
    QWidget* MyWidged = ui->btnHJ;
    selectHobbie(MyWidged, User::Hobbies::Senderismo);
}

void MainForm::on_btnHK_clicked()
{
    QWidget* MyWidged = ui->btnHK;
    selectHobbie(MyWidged, User::Hobbies::Futbol);
}

void MainForm::on_btnHL_clicked()
{
    QWidget* MyWidged = ui->btnHL;
    selectHobbie(MyWidged, User::Hobbies::Reposteria);
}

void MainForm::on_btnHM_clicked()
{
    QWidget* MyWidged = ui->btnHM;
    selectHobbie(MyWidged, User::Hobbies::Entrenamiento);
}

void MainForm::on_btnHN_clicked()
{
    QWidget* MyWidged = ui->btnHN;
    selectHobbie(MyWidged, User::Hobbies::Golf);
}

void MainForm::on_btnHNN_clicked()
{
    QWidget* MyWidged = ui->btnHNN;
    selectHobbie(MyWidged, User::Hobbies::Deportes);
}

void MainForm::on_btnHO_clicked()
{
    QWidget* MyWidged = ui->btnHO;
    selectHobbie(MyWidged, User::Hobbies::Moda);
}

void MainForm::on_btnHP_clicked()
{
    QWidget* MyWidged = ui->btnHP;
    selectHobbie(MyWidged, User::Hobbies::Trivia);
}

void MainForm::on_btnHQ_clicked()
{
    QWidget* MyWidged = ui->btnHQ;
    selectHobbie(MyWidged, User::Hobbies::Museo);
}

void MainForm::on_btnHR_clicked()
{
    QWidget* MyWidged = ui->btnHR;
    selectHobbie(MyWidged, User::Hobbies::AireLibre);
}

void MainForm::on_btnHS_clicked()
{
    QWidget* MyWidged = ui->btnHS;
    selectHobbie(MyWidged, User::Hobbies::Cafe);
}

void MainForm::on_btnHT_clicked()
{
    QWidget* MyWidged = ui->btnHT;
    selectHobbie(MyWidged, User::Hobbies::Viajar);
}

void MainForm::on_btnHU_clicked()
{
    QWidget* MyWidged = ui->btnHU;
    selectHobbie(MyWidged, User::Hobbies::Fotografia);
}

void MainForm::on_btnHV_clicked()
{
    QWidget* MyWidged = ui->btnHV;
    selectHobbie(MyWidged, User::Hobbies::Voluntariado);
}

void MainForm::on_btnHW_clicked()
{
    QWidget* MyWidged = ui->btnHW;
    selectHobbie(MyWidged, User::Hobbies::Te);
}

void MainForm::on_btnHX_clicked()
{
    QWidget* MyWidged = ui->btnHX;
    selectHobbie(MyWidged, User::Hobbies::Gamer);
}

void MainForm::on_btnHY_clicked()
{
    QWidget* MyWidged = ui->btnHY;
    selectHobbie(MyWidged, User::Hobbies::SalirDeCompras);
}

void MainForm::on_btnHZ_clicked()
{
    QWidget* MyWidged = ui->btnHZ;
    selectHobbie(MyWidged, User::Hobbies::Disney);
}

void MainForm::on_btnHAA_clicked()
{
    QWidget* MyWidged = ui->btnHAA;
    selectHobbie(MyWidged, User::Hobbies::Nadar);
}

void MainForm::on_btnHAB_clicked()
{
    QWidget* MyWidged = ui->btnHAB;
    selectHobbie(MyWidged, User::Hobbies::Bloquear);
}

void MainForm::on_btnHAC_clicked()
{
    QWidget* MyWidged = ui->btnHAC;
    selectHobbie(MyWidged, User::Hobbies::Karaoke);
}

void MainForm::on_btnHAD_clicked()
{
    QWidget* MyWidged = ui->btnHAD;
    selectHobbie(MyWidged, User::Hobbies::Peliculas);
}

void MainForm::on_btnHAE_clicked()
{
    QWidget* MyWidged = ui->btnHAE;
    selectHobbie(MyWidged, User::Hobbies::AmanteDeLosGatos);
}

void MainForm::on_btnVolverAtras_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(0);
}

void MainForm::on_btnConCorreo_2_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(0);
}

void MainForm::on_btnContinuar06_clicked()
{
    if (otherData->vecHobbies.size() == 5){
        ui->ContenedorTinder->setCurrentIndex(1);
        ui->ContenedorRegistrar->setCurrentIndex(7);
    }
    else {
        QMessageBox::information(this, "Tinder - Alertas", "Por favor ingresar cinco hobbies!", QMessageBox::Ok);
    }
}

void MainForm::on_btnVolverAtras_9_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(5);
}

void MainForm::on_btnVolverAtras_10_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(6);
}

void MainForm::on_btnContinuar07_2_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(0);
}

void MainForm::on_btnContinuar07_3_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(2);
}

void MainForm::on_btnContinuarPrincipal02_clicked()
{
    string correo = ui->txtCCorreo->text().toStdString();
    string password = ui->txtCPassword->text().toStdString();

    if (server->logInWithEmail(correo, password, client))
    {
        //LIMPIO LAS CAJAS
        ui->txtCCorreo->setText("");
        ui->txtCPassword->setText("");

        //CARGAR DATA INICIAL
        uploadDataHome();

        //PASAR A LA SIGUIENTE PAGINA
        ui->ContenedorTinder->setCurrentIndex(3);
        ui->UserOn->setCurrentIndex(0);
    }
    else {
        QMessageBox::information(this, "Tinder - Alertas", "Correo y/o contraseña invalida, intente de nuevo!", QMessageBox::Ok);
    }
}

void MainForm::on_btnHome_clicked()
{
    //Diseñor de los botones ( ACTIVAR - DESACTIVAR )
    ui->ContenedorTinder->setCurrentIndex(3);
    ui->UserOn->setCurrentIndex(0);
    QIcon onHome(":/img/Resource/img/hogar-color.png");
    QIcon offFriends(":/img/Resource/img/apoyar-gris.png");
    QIcon offPerfil(":/img/Resource/img/usuario2-gris.png");
    QIcon offSetting(":/icon/Resource/img/engranaje_gris.png");
    QIcon offNew(":/img/Resource/img/noticias-gris.png");
    ui->btnHome->setIcon(onHome);
    ui->btnFriends->setIcon(offFriends);
    ui->btnPerfil->setIcon(offPerfil);
    ui->btnSettings->setIcon(offSetting);
    ui->btnNew->setIcon(offNew);
    lastPage = 0;

    //REMOVER OTRAS VISTAS
    removeiFriends();
}

void MainForm::on_btnFriends_clicked()
{
    //Diseñor de los botones ( ACTIVAR - DESACTIVAR )
    ui->ContenedorTinder->setCurrentIndex(3);
    ui->UserOn->setCurrentIndex(1);
    QIcon onFriends(":/img/Resource/img/apoyar-color.png");
    QIcon offHome(":/img/Resource/img/hogar-gris.png");
    QIcon offPerfil(":/img/Resource/img/usuario2-gris.png");
    QIcon offSetting(":/icon/Resource/img/engranaje_gris.png");
    QIcon offNew(":/img/Resource/img/noticias-gris.png");
    ui->btnHome->setIcon(offHome);
    ui->btnFriends->setIcon(onFriends);
    ui->btnPerfil->setIcon(offPerfil);
    ui->btnSettings->setIcon(offSetting);
    ui->btnNew->setIcon(offNew);
    lastPage = 1;

    //GENERAR AMIGOS
    QGridLayout* ly = new QGridLayout(ui->saFriends);
    ui->saFriends->widget()->setLayout(ly);
    ui->saFriends->setWidgetResizable(true);

    removeiFriends();
    addAllFriends();
    addFriendToStruct(client.getListFriendData());

}

void MainForm::on_btnPerfil_clicked()
{
    //Diseñor de los botones ( ACTIVAR - DESACTIVAR )
    ui->ContenedorTinder->setCurrentIndex(3);
    ui->UserOn->setCurrentIndex(2);
    QIcon offFriends(":/img/Resource/img/apoyar-gris.png");
    QIcon offHome(":/img/Resource/img/hogar-gris.png");
    QIcon onPerfil(":/img/Resource/img/usuario-color.png");
    QIcon offSetting(":/icon/Resource/img/engranaje_gris.png");
    QIcon offNew(":/img/Resource/img/noticias-gris.png");
    ui->btnHome->setIcon(offHome);
    ui->btnFriends->setIcon(offFriends);
    ui->btnPerfil->setIcon(onPerfil);
    ui->btnSettings->setIcon(offSetting);
    ui->btnNew->setIcon(offNew);
    lastPage = 2;

    uploadPerfilPri(client.getUserData());

    //REMOVER OTRAS VISTAS
    removeiFriends();
}

void MainForm::on_btnSettings_clicked()
{
    //Diseñor de los botones ( ACTIVAR - DESACTIVAR )
    ui->ContenedorTinder->setCurrentIndex(3);
    ui->UserOn->setCurrentIndex(3);
    QIcon offFriends(":/img/Resource/img/apoyar-gris.png");
    QIcon offHome(":/img/Resource/img/hogar-gris.png");
    QIcon offPerfil(":/img/Resource/img/usuario2-gris.png");
    QIcon onSetting(":/img/Resource/img/engranaje-color.png");
    QIcon offNew(":/img/Resource/img/noticias-gris.png");
    ui->btnHome->setIcon(offHome);
    ui->btnFriends->setIcon(offFriends);
    ui->btnPerfil->setIcon(offPerfil);
    ui->btnSettings->setIcon(onSetting);
    ui->btnNew->setIcon(offNew);
    lastPage = 3;

    //REMOVER OTRAS VISTAS
    removeiFriends();
}

void MainForm::generateIntefaceNews()
{
    //Diseñor de los botones ( ACTIVAR - DESACTIVAR )
    ui->ContenedorTinder->setCurrentIndex(3);
    ui->UserOn->setCurrentIndex(4);
    QIcon offFriends(":/img/Resource/img/apoyar-gris.png");
    QIcon offHome(":/img/Resource/img/hogar-gris.png");
    QIcon offPerfil(":/img/Resource/img/usuario2-gris.png");
    QIcon offSetting(":/icon/Resource/img/engranaje_gris.png");
    QIcon onNew(":/img/Resource/img/noticias-color.png");
    ui->btnHome->setIcon(offHome);
    ui->btnFriends->setIcon(offFriends);
    ui->btnPerfil->setIcon(offPerfil);
    ui->btnSettings->setIcon(offSetting);
    ui->btnNew->setIcon(onNew);
    lastPage = 4;

    //REMOVER OTRAS VISTAS
    removeiFriends();


    removeiPublications(ui->scrollArea_3->widget());
    server->FillVectorPubl(client);
    PublicationAllUserStruct(client.getAllPub());
}

void MainForm::on_btnNew_clicked()
{
    generateIntefaceNews();
}

void MainForm::on_btnRegisterUser_clicked()
{
    if (otherData->photo == QString()){

        QMessageBox::StandardButton opcion;
        opcion = QMessageBox::question(this, "Tinder - Alertas", "¿Esta seguro de continuar sin agregar una foto de perfil?",
                                       QMessageBox::Ok | QMessageBox::Cancel);
        if (opcion == QMessageBox::Ok) {
            processRegister();
        }
    }
    else {
        processRegister();
    }
}

void MainForm::on_btnVolverAtras_6_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(1);
    ui->ContenedorRegistrar->setCurrentIndex(1);
}

void MainForm::on_btnConTelefono_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(2);
    ui->ContainerKindLogIn->setCurrentIndex(1);
}

void MainForm::on_btnVolverAtras_2_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(0);
}

void MainForm::on_btnLogInWithCellPhone_clicked()
{
    unsigned int telefono = ui->txtTTelefono->text().toUInt();
    string password = ui->txtTPassword->text().toStdString();
    if (server->logInWithCellPhone(telefono, password, client))
    {
        //LIMPIO LAS CAJAS
        ui->txtTTelefono->setText("");
        ui->txtTPassword->setText("");

        //CARGAR DATA INCIAL DE HOME
        uploadDataHome();

        //PASAR A LA SIGUIENTE PAGINA
        ui->ContenedorTinder->setCurrentIndex(3);
        ui->UserOn->setCurrentIndex(0);
    }
    else {
        QMessageBox::information(this, "Tinder - Alertas", "Numero de celular y/o contraseña invalida, intente de nuevo!", QMessageBox::Ok);
    }
}

void MainForm::on_cbMostrarGenero_stateChanged(int arg1)
{
    otherData->showgender = arg1;
}

void MainForm::on_btnSelectFoto01_clicked()
{
    QImage photo = uploadImages();

    if (photo != QImage()){
        ui->lblFoto01->setPixmap(QPixmap::fromImage(photo));
    }
}

void MainForm::on_btnNext_clicked()
{
    User UserShow;
    if (client.MoveNext(UserShow)){
        uploadDataHome();
    }

}

void MainForm::on_btnBack_clicked()
{
    User UserShow;
    if (client.MoveBack(UserShow)){
        uploadDataHome();
    }
}

void MainForm::on_btnPNext_clicked()
{
    client.removeActually();
    uploadDataHome();
}

void MainForm::on_btnPBack_clicked()
{
    User UserShow;
    if (client.MoveBack(UserShow)){
        uploadDataHome();
    }
}

void MainForm::on_btnSee_clicked()
{

    ui->ContenedorTinder->setCurrentIndex(4);
    ui->PageOfHelp->setCurrentIndex(0);

    User UserActually;

    if (client.getActually(UserActually)) uploadPerfilAux(UserActually);
    codeUser = UserActually.getCellPhone();
}

void MainForm::on_pushVolveAtras_clicked()
{
    if (lastPage == 4){
        generateIntefaceNews();
    }
    ui->ContenedorTinder->setCurrentIndex(3);
    ui->UserOn->setCurrentIndex(lastPage);
}

void MainForm::on_btnMostRecent_clicked()
{
    ui->btnMostRecent->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->btnMostRelevant->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->btnMostLikes->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    removeiPublications(ui->scrollArea_4->widget());
    removeiPublications(ui->scrollArea_2->widget());
    vector<Publication> pub = client.generateMyPListRecent();
    addPublicationToStruct(pub, ui->scrollArea_2, client.getUserData());

}

void MainForm::on_btnMostLikes_clicked()
{
    ui->btnMostLikes->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->btnMostRecent->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->btnMostRelevant->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    removeiPublications(ui->scrollArea_2->widget());
    vector<Publication> pub = client.generateMyPListLikes();
    addPublicationToStruct(pub, ui->scrollArea_2, client.getUserData());
}

void MainForm::on_btnMostRelevant_clicked()
{
    ui->btnMostRelevant->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->btnMostRecent->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->btnMostLikes->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    removeiPublications(ui->scrollArea_2->widget());
    vector<Publication> pub = client.generateMyPListRelev();
    addPublicationToStruct(pub, ui->scrollArea_2, client.getUserData());
}

void MainForm::on_btnMostRecent_2_clicked()
{
    ui->btnMostRecent_2->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->btnMostRelevant_2->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->btnMostLikes_2->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
}

void MainForm::on_btnMostLikes_2_clicked()
{
    ui->btnMostLikes_2->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->btnMostRecent_2->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->btnMostRelevant_2->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
}

void MainForm::on_btnMostRelevant_2_clicked()
{
    ui->btnMostRelevant_2->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->btnMostRecent_2->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->btnMostLikes_2->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
}

void MainForm::on_pushButton_5_clicked()
{
    server->addFriend(client);
    client.removeActually();
    uploadDataHome();
}

void MainForm::on_pushButton_4_clicked()
{
    ui->ContenedorTinder->setCurrentIndex(3);
    ui->UserOn->setCurrentIndex(lastPage);
}

void MainForm::on_pushButton_2_clicked()
{
    string autor = client.getUserData().getFullName();
    string text =  ui->textEdit->toPlainText().toStdString();
    Date date;
    date.setDay(QDateTime::currentDateTime().date().day());
    date.setMonth(QDateTime::currentDateTime().date().month());
    date.setYear(QDateTime::currentDateTime().date().year());
    Comment comm(autor, text, date);

    User user = server->getAVLCellPhone()->find(codeUser);

    server->addComment(user.getCellPhone(), pubPos, comm);


    if (client.getUserData().getCellPhone() == user.getCellPhone()){
        client.getUserData().getPublications()[pubPos].addComment(comm);
        removeiComments();
        uploatComments(client.getUserData().getPublications()[pubPos].getComments());
    }
    else {
        client.getFeedActually().getPublications().at(pubPos).addComment(comm);
        removeiComments();
        server->FillVectorPubl(client);
        User u = server->getAVLCellPhone()->find(codeUser);
        uploatComments(u.getPublications()[pubPos].getComments());
    }
    ui->textEdit->setText("");
}

void MainForm::on_pushButton_clicked()
{
    server->LoadTXT();
    QMessageBox::information(this, "Datos cargados", "Sus datos han sido cargador satisfactoriamente!!", QMessageBox::Ok);
}

void MainForm::on_pushButton_6_clicked()
{
    server->SaveAll();
    QMessageBox::information(this, "Cerrar Sesión", "Se ha cerrado sesión vuelva pronto :'v", QMessageBox::Ok);
    ui->ContenedorTinder->setCurrentIndex(0);
}

void MainForm::on_pushButton_8_clicked()
{
    ui->pushButton_8->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->pushButton_9->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->pushButton_10->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");

}

void MainForm::on_pushButton_9_clicked()
{
    ui->pushButton_9->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->pushButton_8->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->pushButton_10->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
}

void MainForm::on_pushButton_10_clicked()
{
    ui->pushButton_10->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(243, 21, 122, 1), stop:1 rgba(248, 128, 89, 1)); color: white; border-radius: 10px; font-weight: bold");
    ui->pushButton_8->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
    ui->pushButton_9->setStyleSheet("background-color: #DCDCDC; border-radius: 10px; color: white; font-weight: bold");
}
