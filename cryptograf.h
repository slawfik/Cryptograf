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

    void on_button_textORnasobky_clicked();

    void on_button_startAttack_clicked();

    void on_pushButton_clicked();

    void on_button_desifruj_clicked();

    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_spinBox_site_valueChanged(int arg1);

private:
    Ui::Cryptograf *ui;
    kassisk_method *kass = nullptr;
    coicident_index *coint = nullptr;

    int count_result_view = 0;
    int site = 1;
    QStringList list;
    QFile file;
    QFile encryptedFile;
    QStringListModel crypted_model;
    QStringListModel encrypted_model;

    void show_encrypted_results();
};
#endif // CRYPTOGRAF_H
