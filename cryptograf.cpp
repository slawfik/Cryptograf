#include "cryptograf.h"
#include "ui_cryptograf.h"
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include <stdio.h>
#include "bruteforceset.h"

Cryptograf::Cryptograf(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Cryptograf)
{
    ui->setupUi(this);
    ui->spinBox_passwdLen->setValue(4);
    ui->spinBox_pasLenCrypted->setValue(4);
    ui->pushButton_2->hide();
}

Cryptograf::~Cryptograf()
{
    if(kass != nullptr)
        delete  kass;
    if(coint != nullptr)
        delete coint;
    if(chr_list != nullptr){
        for (int i = 0;i<chr_list->size();i++) {
            delete chr_list->operator[](i);
        }
        delete chr_list;
        qDebug() << "del chrList";
    }
    for (int i = 0;i< najpocetnejsie_znaky.size();i++) {
        delete najpocetnejsie_znaky.operator[](i);
    }
    delete ui;
}

void Cryptograf::on_button_openCryptFile_clicked()
{
    QString pa_path;

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
    cryptedlist.clear();
    cryptedlist.push_back(kass->getSifrovany_text());
    crypted_model.setStringList(cryptedlist);
    ui->listView_Crypted->setModel(&crypted_model);
}

/*TLACIDLO PRE NSATAVENIE UTOKU*/
void Cryptograf::on_button_startAttack_clicked()
{
    QString pom;
    bruteforcePass_len = ui->spinBox_passwdLen->value();

    if(kass == nullptr){
        qDebug() << "kass is not initialized on_button_startAttack_clicked()";
        QMessageBox::critical(this,"Bruteforce", "Please select .txt file with crypted data.");
        return;
    } else if(bruteforcePass_len == 0){
        qDebug() << "passwd length is 0 on_button_startAttack_clicked()";
        ui->statusbar->showMessage("zlá dĺžka hesla");
        return;
    } else {
        bruteForceSet *sett = new bruteForceSet(bruteforcePass_len);

        if(sett->exec()) {
            chr_list = sett->getList();
            ui->pushButton_2->show();
        }
    }
}


/*TLACIDLO INDEX COENCIDENCIE*/
void Cryptograf::on_pushButton_clicked()
{
    QString part;
    QString poctyZnakov;
    QString print = "\n";

    int dlzka_hesla = ui->spinBox_pasLenCrypted->value();

    if(kass == nullptr){
        qDebug() << "kass is not initialized on_pushButton_clicked()";
        return;
    }
    for (int i =0;i<dlzka_hesla;i++) {
        najpocetnejsie_znaky.push_back(new QList<QChar>);
    }
    cryptedlist.clear();

    cryptedlist.push_back(kass->getSifrovany_text());
    cryptedlist.push_back(kass->toString_nasobkyHesla());
    for (int i = 1;i<=dlzka_hesla;i++) {
        part = kass->rozdel_na_casti(dlzka_hesla,i);
        poctyZnakov = "PART_" + QString::number(i)+":  ";// + part+"\n\n";
        poctyZnakov += kass->sum_4_najpocetnejsieZnaky(part);
        najpocetnejsie_znaky[i-1]->push_back(poctyZnakov.split('-')[0].back());
        najpocetnejsie_znaky[i-1]->push_back(poctyZnakov.split('-')[1].back());
        najpocetnejsie_znaky[i-1]->push_back(poctyZnakov.split('-')[2].back());
        najpocetnejsie_znaky[i-1]->push_back(poctyZnakov.split('-')[3].back());
        cryptedlist.push_back(poctyZnakov);

        coicident_index co(part);
        print += "Časť " + QString::number(i) + " : " + co.toQString() + "\n";
    }
    cryptedlist.push_back(print);

    crypted_model.setStringList(cryptedlist);
}

void Cryptograf::on_button_desifruj_clicked()
{
    int i_heslo[40] = {0,0,0};
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

}

/*TLACIDLO NA SPUSTENIE BRUTEFORCE UTOKU*/
void Cryptograf::on_pushButton_2_clicked()
{
    kass->bruteForce(chr_list,bruteforcePass_len);
    ui->statusbar->showMessage("attack is done!"+QString::number(kass->getPoced_desifrovanych()),5000);
}

void Cryptograf::on_btn_zistiHeslo_clicked()
{
    QString heslo = kass->break_passwd(ui->spinBox_pasLenCrypted->value());
    cryptedlist.push_front(heslo);
    crypted_model.setStringList(cryptedlist);
    qDebug().noquote() << heslo;
}
