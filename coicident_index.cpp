#include "coicident_index.h"

coicident_index::coicident_index(int pa_char_count,int pa_first_char)
{
    source_char_count = pa_char_count;
    prvy_znak_vASCI = pa_first_char;
    najdene_znaky = new int [source_char_count];
    memset(najdene_znaky,0,sizeof (int)* static_cast<unsigned long>(source_char_count));
    pravdepodobnosti_znakov = new double [source_char_count];
}

coicident_index::coicident_index(QString pa_text,int pa_char_count,int pa_first_char)
{
    source_char_count = pa_char_count;
    prvy_znak_vASCI = pa_first_char;
    sifrovany_text = pa_text;
    najdene_znaky = new int [source_char_count];
    memset(najdene_znaky,0,sizeof (int)* static_cast<unsigned long>(source_char_count));
    pravdepodobnosti_znakov = new double [source_char_count];
    parse_sif_text();
    calculate_index();
}

coicident_index::coicident_index(QFile& pa_file, int pa_char_count, int pa_first_char)
{
    source_char_count = pa_char_count;
    prvy_znak_vASCI = pa_first_char;
    najdene_znaky = new int [source_char_count];
    memset(najdene_znaky,0,sizeof (int)* static_cast<unsigned long>(source_char_count));
    pravdepodobnosti_znakov = new double [source_char_count];

    pa_file.open(QFile::ReadOnly);
    if(pa_file.isOpen())  {
        QTextStream in(&pa_file);
        sifrovany_text = in.readAll();
        in.flush();
        pa_file.close();
    }
    parse_sif_text();
    calculate_index();
}

coicident_index::~coicident_index()
{
    delete najdene_znaky;
    //delete[] najdene_znaky;
    delete pravdepodobnosti_znakov;
    //delete[] pravdepodobnosti_znakov;
}

QString coicident_index::toQString() const
{
    return QString("Index_coincident: " + QString::number(index_coincidencie,'f',4));
}

void coicident_index::calculate_index()
{
    for (int i = 0; i < source_char_count;i++) {
        pravdepodobnosti_znakov[i] = static_cast<double>(najdene_znaky[i])/static_cast<double>(pocet_platnych_znakov);
        index_coincidencie += pravdepodobnosti_znakov[i] * static_cast<double>(najdene_znaky[i]-1) / static_cast<double>(pocet_platnych_znakov-1);
    }
}

void coicident_index::parse_sif_text()
{
    for(int i = 0;i < sifrovany_text.length();i++ )   {
        int pom = sifrovany_text[i].toLatin1() - prvy_znak_vASCI;
        if(pom < source_char_count && pom >-1) {
            najdene_znaky[pom]++;
            pocet_platnych_znakov++;
        }
    }
}

/*###_GET___SET_###*/

QString coicident_index::getSifrovany_text() const
{
    return sifrovany_text;
}

void coicident_index::setSifrovany_text(const QString &value)
{
    sifrovany_text = value;
    parse_sif_text();
    calculate_index();
}

int coicident_index::getPrvy_znak_vASCI() const
{
    return prvy_znak_vASCI;
}

void coicident_index::setPrvy_znak_vASCI(int value)
{
    prvy_znak_vASCI = value;
}

int coicident_index::getSource_char_count() const
{
    return source_char_count;
}

void coicident_index::setSource_char_count(int value)
{
    source_char_count = value;
}

double coicident_index::getIndex_coincidencie() const
{
    return index_coincidencie;
}

void coicident_index::setIndex_coincidencie(double value)
{
    index_coincidencie = value;
}

