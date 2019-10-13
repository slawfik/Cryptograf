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

private:
    Ui::Cryptograf *ui;
    kassisk_method *kass = nullptr;
    coicident_index *coint = nullptr;

    QFile file;
    QStringListModel crypted_model;
    QStringListModel encrypted_model;
};
#endif // CRYPTOGRAF_H
