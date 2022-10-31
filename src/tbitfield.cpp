// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
	{
		throw exception("len must be positive");
	}
	else
	{
		BitLen = len;
		MemLen = BitLen / (sizeof(TELEM) * 8);
		if (MemLen * sizeof(TELEM) * 8 < len)
		{
			MemLen++;
		}
		pMem = new TELEM[MemLen];
		memset(pMem, 0, MemLen * sizeof(TELEM));
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen)
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
		throw exception("wrong index");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < BitLen)
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	else
		throw exception("wrong index");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n < BitLen)
	{
		if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == 0)
			return 0;

		return 1;
	}
	else
		throw exception("wrong index");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));

	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (int indMem = 0; indMem < MemLen; indMem++)
	{
		if (pMem[indMem] != bf.pMem[indMem])
			return 0;
	}

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (*this == bf)
		return 0;

	return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField tmp(max(BitLen, bf.BitLen));

	for (int i = 0; i < MemLen; i++)
	{
		tmp.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++)
	{
		tmp.pMem[i] |= bf.pMem[i];
	}
	return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	TBitField tmp(max(BitLen, bf.BitLen));
	for (int indMem = 0; indMem < min(MemLen, bf.MemLen); indMem++)
	{
		tmp.pMem[indMem] = pMem[indMem] & bf.pMem[indMem];
	}
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int indMem = 0; indMem < MemLen; indMem++)
	{
		tmp.pMem[indMem] = ~pMem[indMem];
	}
	tmp.pMem[MemLen - 1] &= GetMemMask(BitLen) - 1;

	return tmp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int max_number = bf.GetLength();
	int valInp;
	while (true)
	{
		istr >> valInp;

		if (valInp >= 0 && valInp < max_number)
			bf.SetBit(valInp);
		else
			break;
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int indOut = 0; indOut < bf.GetLength(); indOut++)
	{
		if (bf.GetBit(indOut))
			ostr << indOut << " ";
	}
	return ostr;
}
