/*
    numbersystems.h
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

#ifndef NUMBERSYSTEMS_H
#define NUMBERSYSTEMS_H

#include <QObject>

class NumberSystems : public QObject
{
    Q_OBJECT
public:
    explicit NumberSystems(QObject *parent = 0);
    QStringList generatePack(bool withAnswers = false);
    QString calculatePolinom(QString number, quint8 order);
    QString calculatePolinom(qreal number);
    QString convert(QString number, quint8 from, quint8 to);
    QString convert(qreal number, quint8 to);
    QString generateDecNumber();

signals:

public slots:

};

#endif // NUMBERSYSTEMS_H
