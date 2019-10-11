#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "coicident_index.h"
#include "kassisk_method.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile text1("/home/tomas/Dokumenty/Škola/Crypto/text1.txt");
    coicident_index ind(text1);
    kassisk_method kas(text1);

    /*QString part_sifrovaneho_TXT = kas.rozdel_na_casti(8,1);
    coicident_index part(part_sifrovaneho_TXT);
*/
    kas.brute_force_kassisk(4);

    //qDebug().noquote() << "1 cast " << part_sifrovaneho_TXT << "\n\n index= " <<  part.toString_nasobkyHesla();
    qDebug().noquote() << " Obsahuje: \n" + ind.getSifrovany_text() + "\n\n" + ind.toQString();
    //qDebug().noquote() << "\n\nKassiskeho metoda dala dlzky:\n" << kas.toString();
    return a.exec();
}
