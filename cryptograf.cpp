#include "cryptograf.h"
#include "ui_cryptograf.h"
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>

Cryptograf::Cryptograf(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cryptograf)
{
    ui->setupUi(this);
    ui->spinBox_site->setRange(1,200);
    ui->spinBox_passwdLen->setValue(4);
    ui->spinBox_pasLenCrypted->setValue(4);
}

Cryptograf::~Cryptograf()
{
    if(kass != nullptr)
        delete  kass;
    if(coint != nullptr)
        delete coint;

    delete ui;
}

void Cryptograf::on_button_openCryptFile_clicked()
{
    QString pa_path;
    QStringList list;

    pa_path = QFileDialog::getOpenFileName(this,tr("Open .txt file"),".","All files (*.txt*)");
    if(pa_path == ""){
        ui->statusbar->showMessage("No selected file!",5000);
        return;
    }
    file.setFileName(pa_path);
    ui->statusbar->showMessage(pa_path);
    if(kass == nullptr){
        kass = new kassisk_method(file);
    } else{
        qDebug() << "delete kass on_button_openCryptFile_clicked()";
        delete kass;
        kass = new kassisk_method(file);
    }
    list.push_back(kass->getSifrovany_text());
    crypted_model.setStringList(list);
    ui->listView_Crypted->setModel(&crypted_model);
}

void Cryptograf::on_button_textORnasobky_clicked()
{
    QStringList list;

    if(kass == nullptr){
        qDebug() << "kass is not initialized on_button_textORnasobky_clicked()";
        return;
    }

    list.push_back(kass->getSifrovany_text());
    list.push_back(kass->toString_nasobkyHesla());
    crypted_model.setStringList(list);
}

void Cryptograf::on_button_startAttack_clicked()
{
    QString pom;
    int pas_len = ui->spinBox_passwdLen->value();

    if(kass == nullptr){
        qDebug() << "kass is not initialized on_button_startAttack_clicked()";
        QMessageBox::critical(this,"Bruteforce", "Please select .txt file with crypted data.");
        return;
    } else if(pas_len == 0){
        qDebug() << "passwd length is 0 on_button_startAttack_clicked()";
        ui->statusbar->showMessage("zlá dĺžka hesla");
        return;
    } else {
        list.clear();
        int poc = kass->brute_force_kassisk(pas_len);
        //qDebug() << "desifrovane" << QString::number(poc);
        ui->spinBox_site->setRange(1,(poc/1000)+1);
        show_encrypted_results();
        /*QFile encryptedFile(kass->output.fileName());
        encryptedFile.open(QFile::ReadOnly);
        if(encryptedFile.isOpen()){
            while (!encryptedFile.atEnd()) {
                memset(&buf[0],0,800*sizeof (char));
                pom = "";
                encryptedFile.readLine(buf,800);
                pom += QString(buf);
                encryptedFile.readLine(buf,800);
                pom += QString(buf);
                list.append(pom);
            }
            encryptedFile.close();*/
            ui->statusbar->showMessage("is done!");
        /*} else{
            ui->statusbar->showMessage("error opening "+encryptedFile.fileName());
            qDebug() << "error opening " + encryptedFile.fileName() +" on_button_startAttack_clicked()";

        }*/

    }
}

void Cryptograf::show_encrypted_results()
{
    static qint64 posun=0;
    static int last_site=0,countMsg = 0;
    qint64 pos_s=0,s_Pos=0;
    char buf[800];
    QString pom;

    if(encryptedFile.fileName() != kass->output.fileName()){
        if(encryptedFile.isOpen()){
            site = 1;
            count_result_view = 0;
            encryptedFile.close();
            encryptedFile.setFileName(kass->output.fileName());
            encryptedFile.open(QFile::ReadOnly);
        } else{
            encryptedFile.setFileName(kass->output.fileName());
            encryptedFile.open(QFile::ReadOnly);
        }
    }

    if(encryptedFile.isOpen()){
       /* if(encryptedFile.atEnd() && last_site < site){ //ak presiahne dlzku súboru
            site = last_site;
            ui->spinBox_site->setValue(last_site);
            return;
        }*/

        list.clear();
        if(last_site > site){           //ak sa vráti o stránku nižšie
            encryptedFile.seek(site*posun-1000);
            if(countMsg > 0)
                count_result_view = count_result_view -1000 - countMsg;
            else
                count_result_view = count_result_view-2000;
        }

        s_Pos = encryptedFile.pos();
        last_site = site;
        while (!encryptedFile.atEnd() && count_result_view < site*1000) {
            memset(&buf[0],0,800*sizeof (char));
            pom = ""+QString::number(count_result_view)+".  ";
            encryptedFile.readLine(buf,800);
            pom += QString(buf);
            encryptedFile.readLine(buf,800);
            pom += QString(buf);
            list.append(pom);
            count_result_view++;
        }
        pos_s = encryptedFile.pos();
        countMsg = (count_result_view+1)%1000;
        if(!encryptedFile.atEnd()){
            posun = pos_s - s_Pos;
        }

        qDebug() << QString::number(posun) + "; ";
    }
    encrypted_model.setStringList(list);
    if(ui->listView_Encrypted->model() == nullptr)
        ui->listView_Encrypted->setModel(&encrypted_model);

}

void Cryptograf::on_pushButton_clicked()
{
    QStringList list;
    QString part;
    QString print = "\n\n";
    int dlzka_hesla = ui->spinBox_pasLenCrypted->value();

    if(kass == nullptr){
        qDebug() << "kass is not initialized on_pushButton_clicked()";
        return;
    }

    list.push_back(kass->getSifrovany_text());
    list.push_back(kass->toString_nasobkyHesla());
    for (int i = 1;i<=dlzka_hesla;i++) {
        part = kass->rozdel_na_casti(dlzka_hesla,i);
        coicident_index co(part);
        print += "Časť " + QString::number(i) + " : " + co.toQString() + "\n";
    }
    list.push_back(print);
    crypted_model.setStringList(list);
}

void Cryptograf::on_button_desifruj_clicked()
{
    int i_heslo[10] = {0,0,0};
    QString desifrovany;
    QString heslo = ui->lineEdit_znameHeslo->text();

    if(kass == nullptr){
        qDebug() << "kass is not initialized on_button_desifruj_clicked()";
        return;
    } else if(heslo.length()==0){
        qDebug() << "password length is 0! on_button_desifruj_clicked()";
        ui->statusbar->showMessage("Heslo nebolo zadané!",5000);
        return;
    }
    list.clear();
    for (int i =0;i<heslo.length();i++) {
        i_heslo[i] = heslo[i].toLatin1()-'A';
    }
    desifrovany = "PASSWD: "+heslo+"\n";
    desifrovany += kass->cezar_ret(i_heslo,heslo.length());
    list.append(desifrovany);
    encrypted_model.setStringList(list);
    if(ui->listView_Encrypted->model() == nullptr)
        qDebug() << "setting model on_button_desifruj_clicked()";
        ui->listView_Encrypted->setModel(&encrypted_model);
}

void Cryptograf::on_lineEdit_search_textChanged(const QString &arg1)
{
    QStringList filtered = list.filter(arg1,Qt::CaseInsensitive);
    encrypted_model.setStringList(filtered);

    //list.
    //ui->listView_Encrypted.g
}


void Cryptograf::on_spinBox_site_valueChanged(int arg1)
{
    if(kass != nullptr){
            site = arg1;
            show_encrypted_results();
    }


}
