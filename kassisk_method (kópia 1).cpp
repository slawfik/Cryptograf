#include "kassisk_method.h"
#include <QTextStream>
#include <QDebug>
#include <string>

QString kassisk_method::getSifrovany_text() const
{
    return sifrovany_text;
}

bool kassisk_method::init_outputFstream()
{
    output.open(QFile::WriteOnly);
    if(!output.isOpen())    {
        qDebug() << "Problém s otvorením výstupného súboru";
        return false;
    }
    return true;
}

void kassisk_method::calc_nasobky_dlzky_hesla()
{
    QString substr;
    size_t poz2 = 0;
    for(int poz = 0;poz < sifrovany_text.length()-3;poz++)   {
        substr = sifrovany_text.mid(poz,3);
        poz2 = sifrovany_text.toLatin1().toStdString().find(substr.toLatin1().toStdString().c_str(),
                                                     static_cast<size_t>(poz)+1,
                                                     static_cast<size_t>(krok));
        if(poz2 != std::string::npos)
            dlzky->append(static_cast<int>(poz2)-poz);
    }

}

kassisk_method::kassisk_method(QString pa_sifrovany_test,QString pathFor_bruteForce_oFile, int pa_krok)
{
    dlzky = new QList<int>;
    read_allText = pa_sifrovany_test;
    set_sifrovanyText();    //premaze neplatne znaky
    krok = pa_krok;
    output.setFileName(pathFor_bruteForce_oFile);

    calc_nasobky_dlzky_hesla();
}

kassisk_method::kassisk_method(QFile& file, int pa_krok)
{
    dlzky = new QList<int>;
    krok = pa_krok;

    output.setFileName(file.fileName().split('.')[0]+"_bruteForce.txt");

    file.open(QFile::ReadOnly);
    if(file.isOpen())  {
        QTextStream in(&file);
        read_allText = in.readAll();
        set_sifrovanyText();    //premaze neplatne znaky
        in.flush();
        file.close();
        calc_nasobky_dlzky_hesla();
    }
}

QString kassisk_method::rozdel_na_casti(int dlzka_hesla, int part)
{
    QString ret("");
    if(part < 1 || part > dlzka_hesla)
        return QString("Zle parametre pre funkciu \"rozdel_na_casti\"");

    for(int i = 0;i<sifrovany_text.length();i++)    {
        if(i%dlzka_hesla == part-1) {
            ret.append(sifrovany_text.operator[](i));
        }
    }
    return ret;
}

QString kassisk_method::toString_nasobkyHesla()
{
    QString ret = "Kassiského metóda(3) nasobky hesla: ";
    for(int i = 0;i<dlzky->length();i++)    {
        if(!(i%10))
            ret += '\n';
        ret += QString::number(dlzky->operator[](i)) + " ,";
    }
    ret += '\n';
    return ret;
}

void kassisk_method::cezar(int *heslo, int dlzka_hesla)
{
    int pop,ps=0;
    QString S_heslo = "PASSWD: ";
    QString ret;
    for(int i=0;i<sifrovany_text.length();i++) {
        pop = sifrovany_text.operator[](i).unicode()-'A';
        if(pop >= 0 && pop <= 26)   {
            ret[i] = (((pop+26 - heslo[ps%dlzka_hesla])%26)+'A');
            ps++;
        }
    }
    for(int i=0;i<dlzka_hesla;i++)  {
        S_heslo.append(heslo[i]+'A');
    }
    size_t a = ret.toStdString().find('W');
    size_t b = ret.toStdString().find('Q');
    if(a == std::string::npos && b == std::string::npos){
        output.write(S_heslo.toLatin1()+'\n'+ret.toLatin1()+'\n');
        poced_desifrovanych++;
    }
}

void kassisk_method::set_sifrovanyText()
{

}

QString kassisk_method::sum_4_najpocetnejsieZnaky(QString part_sifrovanehoTextu)
{
    QString freqPrav;
    //int arr[4] = {0,0,0,0};
    for (char i = 65;i<91;i++)   {
        freqPrav += QString(QChar(i)+QString("->") + QString::number(part_sifrovanehoTextu.count(QChar(i))) + QString(" ,"));
        if(i==75)
            freqPrav += '\n';
        else if(i==85)
            freqPrav += '\n';
    }
    return freqPrav;
}

QString kassisk_method::cezar_ret(int *heslo, int dlzka_hesla)
{
    int pop;
    int ps = 0;
    QString ret;
    for(int i=0;i<sifrovany_text.length();i++) {
        pop = sifrovany_text.operator[](i).unicode()-'A';
        if(pop >= 0 && pop <= 26)   {
            ret[i] = (((pop+26 - heslo[ps%dlzka_hesla])%26)+'A');
            ps++;
        }
    }
    return ret;
}

int kassisk_method::brute_force_kassisk(int dlzka_hesla,int pom)
{
    static int heslo[10];
    //prve volanie funkcie pom=0 inicializuje fstream pre výsledky
    if(pom == 0)    {
        bool fs = init_outputFstream();
        if(!fs)
            return 0;
    }
    if(dlzka_hesla >= 10)
        return 0;

    for(int i = 0;i<26;i++) {
        heslo[pom] = i;
        cezar(&heslo[0],dlzka_hesla);
        if(pom != dlzka_hesla-1)   {
            brute_force_kassisk(dlzka_hesla,pom+1);
        }
    }

    // prve volanie funkcie pom=0 otvorilo fstream aj si ho zatvorí
    if(pom == 0)    {
        output.close();
        return poced_desifrovanych;
    }
    return 0;
}

QList<int> *kassisk_method::getDlzky() const
{
    return dlzky;
}

kassisk_method::~kassisk_method()
{
    delete dlzky;
}
