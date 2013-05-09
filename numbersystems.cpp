/*
    numbersystems.cpp
    Copyright (C) 2011 Mikhalevich Yurij <count@ymilij.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "numbersystems.h"
#include <QStringList>
#include <qmath.h>
#include <QTime>
#include <QMap>

NumberSystems::NumberSystems(QObject *parent) :
    QObject(parent)
{
    QTime time = QTime::currentTime();
    qsrand(time.msec());
}

QStringList NumberSystems::generatePack(bool withAnswers)
{
    QStringList pack;
    pack.append(QString());
    if (withAnswers)
        pack.append(QString());
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("1) Представьте следующие числа в полиномиальной записи:\r\n");
    quint8 radixes[4] = { 2, 8, 10, 16};
    for (quint8 radixInd = 0; radixInd < 4; ++radixInd) {
        QString number = convert(generateDecNumber(), 10, radixes[radixInd]);
        for (int i = 0; i < pack.count(); ++i)
            pack[i].append("\r\n" + number + "(" + QString::number(radixes[radixInd]) +")\r\nОтвет:");
        if (withAnswers)
            pack.last().append(";" + calculatePolinom(number, radixes[radixInd]));
        for (int i = 0; i < pack.count(); ++i)
            pack[i].append("\r\n");
    }
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n2) Посчитайте, какое число представлено в виде полиномиальной записи:\r\n");
    for (quint8 radixInd = 0; radixInd < 4; ++radixInd) {
        QString number = convert(generateDecNumber(), 10, radixes[radixInd]);
        for (int i = 0; i < pack.count(); ++i)
            pack[i].append("\r\n" + calculatePolinom(number, radixes[radixInd]) + "\r\nОтвет:");
        if (withAnswers)
            pack.last().append(";" + number + "(" + QString::number(radixes[radixInd]) +")");
        for (int i = 0; i < pack.count(); ++i)
            pack[i].append("\r\n");
    }
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n3) Переведите в десятичную систему счисления следующие числа:\r\n");
    for (quint8 radixInd = 0; radixInd < 4; ++radixInd) {
        QString number = generateDecNumber();
        for (int i = 0; i < pack.count(); ++i) {
            pack[i].append("\r\n" + convert(number, 10, radixes[radixInd])
                           + "(" + QString::number(radixes[radixInd]) +")\r\nОтвет:");
        }
        if (withAnswers) {
            if (radixes[radixInd] == 2)
                number = convert(convert(number, 10, 2), 2, 10);
            pack.last().append(";" + number);
        }
        for (int i = 0; i < pack.count(); ++i)
            pack[i].append("\r\n");
        if (radixInd == 1)
            ++radixInd;
    }
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n4) Переведите следующие числа из десятичной системы счисления в:\r\n");
    QMap<quint8, QString> radixesDict;
    radixesDict.insert(2, "двоичную");
    radixesDict.insert(8, "восьмеричную");
    radixesDict.insert(10, "десятиричную");
    radixesDict.insert(16, "шестнадцатеричную");
    for (quint8 radixInd = 0; radixInd < 4; ++radixInd) {
        QString number = generateDecNumber();
        for (int i = 0; i < pack.count(); ++i) {
            pack[i].append("\r\n" + radixesDict.value(radixes[radixInd]) + ";" + number + "\r\nОтвет:");
        }
        if (withAnswers) {
            pack.last().append(";" + convert(number, 10, radixes[radixInd])
                               + "(" + QString::number(radixes[radixInd]) +")");
        }
        for (int i = 0; i < pack.count(); ++i)
            pack[i].append("\r\n");
        if (radixInd == 1)
            ++radixInd;
    }
    return pack;
}

QString NumberSystems::generateDecNumber()
{
    QString number = QString::number(qrand() % (9999 - 101) + 100) + '.' + QString::number(qrand() % (999 - 101) + 100);
    while (!number.endsWith(".0")) {
        if (number.endsWith('0'))
            number.chop(1);
        else
            break;
    }
    return number;
}

QString NumberSystems::convert(QString number, quint8 from, quint8 to)
{
    QStringList splittedNumber = number.split(".");
    QString result = QString::number(splittedNumber.first().toInt(0, from), to).toUpper();
    result.append('.');
    if (splittedNumber.count() != 1) {
        qreal afterDotDec = 0;
        if (from != 10) {
            for (quint8 i = 1; i <= splittedNumber.last().count(); ++i) {
                QChar c = splittedNumber.last().at(i - 1);
                if (c != '0')
                   afterDotDec += QString(c).toInt(0, from) * qPow(from, (-i));
            }
        } else {
            afterDotDec = QString("0." + splittedNumber.last()).toDouble();
        }
        if (to != 10) {
            for (quint8 i = 0; i < 10; ++i) {
                afterDotDec *= to;
                QString c = QString::number(afterDotDec).split('.').first();
                if (c.toInt() > 9)
                    c = QString::number(c.toInt(), to);
                result.append(c);
                if (QString::number(afterDotDec).split('.').count() != 1) {
                    afterDotDec = QString("0." + QString::number(afterDotDec).split('.').last()).toDouble();
                } else {
                    afterDotDec = 0;
                    break;
                }
            }
            while (!result.endsWith(".0")) {
                if (result.endsWith('0'))
                    result.chop(1);
                else
                    break;
            }
        } else {
            result.append(QString::number(afterDotDec).split('.').last());
        }
    } else {
        result.append('0');
    }
    return result.toUpper();
}

QString NumberSystems::convert(qreal number, quint8 to)
{
    return convert(QString::number(number), 10, to);
}

QString NumberSystems::calculatePolinom(QString number, quint8 order)
{
    QString polinom = "";
    QStringList splittedNumber = number.split(".");
    QMap<QChar, QString> hexDict;
    hexDict.insert('A', "10");
    hexDict.insert('B', "11");
    hexDict.insert('C', "12");
    hexDict.insert('D', "13");
    hexDict.insert('E', "14");
    hexDict.insert('F', "15");
    for (quint8 i = splittedNumber.first().count(); i > 0; --i) {
        QChar c = splittedNumber.first().at(splittedNumber.first().count() - i);
        if (c != '0') {
            if (polinom.length() != 0)
                polinom.append("+");
            polinom.append(hexDict.value(c, QString(c)) + "*" + QString::number(order) + "^(" + QString::number(i + 1) + ")");
        }
    }
    if (splittedNumber.count() != 1) {
        for (quint8 i = 1; i <= splittedNumber.last().count(); ++i) {
            QChar c = splittedNumber.last().at(i - 1);
            if (c != '0') {
                if (polinom.length() != 0)
                    polinom.append("+");
                polinom.append(hexDict.value(c, QString(c)) + "*" + QString::number(order) + "^(" + QString::number(i) + ")");
            }
        }
    }
    return polinom;
}

QString NumberSystems::calculatePolinom(qreal number)
{
    return calculatePolinom(QString::number(number), 10);
}
