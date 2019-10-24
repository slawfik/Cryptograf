#ifndef KASSISK_METHOD_H
#define KASSISK_METHOD_H

#include <QString>
#include <QList>
#include <QFile>
#include <QMap>

class kassisk_method
{
private:
    int krok;
    int *pocty[4];      //4 najfrekventovanejšie znaky
    long long poced_desifrovanych = 0;
    QList<int> sifrovany_text;
    QString read_allText;
    QList<int> * dlzky;
    QHash<QChar,double> svk_pravdepodobnosti = { {'A',0.11160},{'B',0.01778},{'C',0.02463},{'D',0.03760},{'E',0.09316}
                                               , {'F',0.00165},{'G',0.00175},{'H',0.02482},{'I',0.05745},{'J',0.02158}
                                               , {'K',0.03961},{'L',0.04375},{'M',0.03578},{'N',0.05949},{'O',0.09540}
                                               , {'P',0.03007},{'Q',0.00000},{'R',0.04706},{'S',0.06121},{'T',0.05722}
                                               , {'U',0.03308},{'V',0.04604},{'W',0.00001},{'X',0.00028},{'Y',0.02674}
                                               , {'Z',0.03064}};

    bool init_outputFstream();
    void close_outputFstream();
    void calc_nasobky_dlzky_hesla();
    void cezar(int* heslo,int dlzka_hesla);
    void set_sifrovanyText();
    void swap(int row,int destRow);
    void zistiPravdepodobnostiZnakov(QString part,QList<double>& pravd_part);//iba pre čast
    double vzdialenost_vektorov(bool use_svk_pravd,QList<double>&  pravd_part);
    QString part_cezar(int* heslo,int dlzka_hesla,QString part);
public:
    QFile output;

    QString sum_4_najpocetnejsieZnaky(QString part_sifrovanehoTextu);
    kassisk_method(QString pa_sifrovany_test,QString pathFor_bruteForce_oFile,int pa_krok=3);
    kassisk_method(QFile& file,int pa_krok=3);
    QString rozdel_na_casti(int dlzka_hesla,int part);
    QString toString_nasobkyHesla();
    QString cezar_ret(int* heslo,int dlzka_hesla);
    int brute_force_kassisk(int dlzka_hesla,int pom=0);
    void bruteForce(QList<QList<QChar>*>* mnozin_znakov, int dlzka_hesla,int pom=0);
    QString break_passwd(int dlzka_hesla);

    virtual ~kassisk_method();
    QList<int> *getDlzky() const;
    QString getSifrovany_text() const;
    long long getPoced_desifrovanych() const;
};

#endif // KASSISK_METHOD_H
