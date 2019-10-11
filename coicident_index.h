#ifndef COICIDENT_INDEX_H
#define COICIDENT_INDEX_H
#include <QFile>
#include <QDebug>
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <QList>


class coicident_index
{
private:
   int source_char_count;
   int prvy_znak_vASCI;
   double index_coincidencie = 0.0;

   QString sifrovany_text;
   int pocet_platnych_znakov = 0;
   int * najdene_znaky;
   double * pravdepodobnosti_znakov;

   void calculate_index();
   void parse_sif_text();
public:
    coicident_index(int pa_char_count = 26,int pa_first_char = 'A');
    coicident_index(QString pa_text,int pa_char_count = 26,int pa_first_char = 'A');
    coicident_index(QFile& pa_file,int pa_char_count = 26,int pa_first_char = 'A');

    virtual ~coicident_index();

    QString toQString() const;

    int getSource_char_count() const;
    void setSource_char_count(int value);
    double getIndex_coincidencie() const;
    void setIndex_coincidencie(double value);
    int getPrvy_znak_vASCI() const;
    void setPrvy_znak_vASCI(int value);
    QString getSifrovany_text() const;
    void setSifrovany_text(const QString &value);
};

#endif // COICIDENT_INDEX_H
