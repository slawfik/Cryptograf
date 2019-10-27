#ifndef CRYPTOGRAF_H
#define CRYPTOGRAF_H

#include <QMainWindow>
#include "coicident_index.h"
#include "kassisk_method.h"
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Cryptograf; }
QT_END_NAMESPACE

class Cryptograf : public QMainWindow
{
    Q_OBJECT

public:
    Cryptograf(QWidget *parent = nullptr);
    virtual ~Cryptograf();

private slots:
    void on_button_openCryptFile_clicked();

    void on_button_startAttack_clicked();

    void on_pushButton_clicked();

    void on_button_desifruj_clicked();

    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_btn_zistiHeslo_clicked();

private:
    Ui::Cryptograf *ui;
    kassisk_method *kass = nullptr;
    coicident_index *coint = nullptr;
    QList<QList<QChar>*>* chr_list = nullptr;

    int bruteforcePass_len;
    QList<QList<QChar>*> najpocetnejsie_znaky;
    QStringList cryptedlist;
    QStringList list;
    QFile file;
    QFile encryptedFile;
    QStringListModel crypted_model;
    QStringListModel encrypted_model;

    //void show_encrypted_results();
};
#endif // CRYPTOGRAF_H
