// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    TBitField converted(BitField);
    return converted;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;

    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TSet combination(max(MaxPower, s.MaxPower));
    combination.BitField = BitField | s.BitField;

    return combination;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet insertion(*this);
    insertion.BitField.SetBit(Elem);

    return insertion;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet removal(*this);
    removal.BitField.ClrBit(Elem);

    return removal;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet intersection(max(MaxPower, s.MaxPower));
    intersection.BitField = BitField & s.BitField;

    return intersection;
}

TSet TSet::operator~(void) // дополнение
{
    TSet negation(*this);
    negation.BitField = ~BitField;

    return negation;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int max_number = s.GetMaxPower();
    int valInp;
    cout << "Enter -1 to stop\n" << endl;
    while (true)
    {
        cout << "Enter the number: ";
        istr >> valInp;

        if (valInp == -1)
            break;

        if (valInp < 0 || valInp >= max_number)
        {
            cout << "Bad number, try again" << endl;
            continue;
        }
        else if (s.IsMember(valInp))
        {
            cout << "This number is already included" << endl;
            continue;
        }
        else
        {
            s.InsElem(valInp);
            cout << "\n";
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    for (int indOut = 0; indOut < s.GetMaxPower(); indOut++)
    {
        if (s.IsMember(indOut))
            ostr << indOut << " ";
    }
    return ostr;
}
