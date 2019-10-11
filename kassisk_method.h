#ifndef KASSISK_METHOD_H
#define KASSISK_METHOD_H

#include <QString>
#include <QList>
#include <QFile>

class kassisk_method
{
private:
    QFile output;
    int krok;
    QString sifrovany_text;
    QList<int> * dlzky;

    bool init_outputFstream();
    void close_outputFstream();
    void calc_nasobky_dlzky_hesla();
public:
    kassisk_method(QString pa_sifrovany_test,QString pathFor_bruteForce_oFile,int pa_krok=3);
    kassisk_method(QFile& file,int pa_krok=3);
    QString rozdel_na_casti(int dlzka_hesla,int part);
    QString toString_nasobkyHesla();
    QString cezar(int* heslo,int dlzka_hesla);
    void brute_force_kassisk(int dlzka_hesla,int pom=0);

    virtual ~kassisk_method();
    QList<int> *getDlzky() const;
};

#endif // KASSISK_METHOD_H
