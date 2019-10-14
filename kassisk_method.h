#ifndef KASSISK_METHOD_H
#define KASSISK_METHOD_H

#include <QString>
#include <QList>
#include <QFile>

class kassisk_method
{
private:
    int krok;
    int poced_desifrovanych = 0;
    QString sifrovany_text;
    QList<int> * dlzky;

    bool init_outputFstream();
    void close_outputFstream();
    void calc_nasobky_dlzky_hesla();
    void cezar(int* heslo,int dlzka_hesla);
public:
    QFile output;

    kassisk_method(QString pa_sifrovany_test,QString pathFor_bruteForce_oFile,int pa_krok=3);
    kassisk_method(QFile& file,int pa_krok=3);
    QString rozdel_na_casti(int dlzka_hesla,int part);
    QString toString_nasobkyHesla();
    QString cezar_ret(int* heslo,int dlzka_hesla);
    int brute_force_kassisk(int dlzka_hesla,int pom=0);

    virtual ~kassisk_method();
    QList<int> *getDlzky() const;
    QString getSifrovany_text() const;
};

#endif // KASSISK_METHOD_H
