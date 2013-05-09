/*
    settheory.h
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

#ifndef SETTHEORY_H
#define SETTHEORY_H

#include <QObject>
#include <QGraphicsScene>

class SetTheory : public QObject
{
    Q_OBJECT
public:
    explicit SetTheory(QObject *parent = 0);
    QStringList generatePack(bool withAnswers = false);
    QString calculateSubsets(QString set);
    QString calculateUnion(QString A, QString B);
    QString calculateIntersection(QString A, QString B);
    QString calculateDifference(QString A, QString B);
    QGraphicsScene *drawEulerVennDiagram(QString setRelation);
    QString formalizeSet(QString set);

private:
    QString generateSet();
    QString operations;
    QString generateSetRelation();

signals:

public slots:

};

#endif // SETTHEORY_H
