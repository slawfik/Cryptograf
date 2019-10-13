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
    file.setFileName(pa_path);
    ui->statusbar->showMessage(pa_path);
    if(kass == nullptr){
        kass = new kassisk_method(file);
    } else{
        qDebug() << "delete kass getOpenFileName()";
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
    QStringList list;
    QString pom;
    char buf[800];

    if(kass == nullptr){
        qDebug() << "kass is not initialized on_button_startAttack_clicked()";
        QMessageBox::critical(this,"Bruteforce", "Please select .txt file with crypted data.");
        return;
    } else {
        kass->brute_force_kassisk(ui->spinBox_passwdLen->value());
        QFile encryptedFile(kass->output.fileName());
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
        }
        encryptedFile.close();
        encrypted_model.setStringList(list);
        ui->listView_Encrypted->setModel(&encrypted_model);
    }
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
