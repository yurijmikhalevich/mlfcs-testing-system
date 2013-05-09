/*
    settheory.cpp
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

#include "settheory.h"
#include <QStringList>
#include <QTime>
#include <QStack>
#include <QGraphicsTextItem>

SetTheory::SetTheory(QObject *parent) :
    QObject(parent)
{
    QTime time = QTime::currentTime();
    qsrand(time.msec());
    operations = "UП\\"; //¬
}

QString SetTheory::generateSet()
{
    QString set;
    qint32 amount = (qrand() % 4) + 3;
    for (int i = 0; i < amount; ++i) {
        int num = qrand() % 10;
        if (set.contains(QString::number(num)))
            --i;
        else
            set.append(QString::number(num));
    }
    return set;
}

QString SetTheory::calculateSubsets(QString set)
{
    QString subsets;
    QString subset;
    qint32 steps = set.length() - 1;
    for (int i = 0; i < steps; ++i) {
        subset.clear();
        subset.append(set.at(i));
        if (i)
            subsets.append(", ");
        subsets.append(formalizeSet(subset));
    }
    qint32 k = 1;
    do {
        for (int i = 0; i < steps; ++i) {
            subset.clear();
            subset = set.mid(i, k);
            for (int j = i + k; j < set.length(); ++j) {
                QString bufSubset = subset;
                bufSubset.append(set.at(j));
                subsets.append(", ");
                subsets.append(formalizeSet(bufSubset));
            }
        }
        --steps;
        ++k;
    } while (steps);
    return subsets;
}

QString SetTheory::formalizeSet(QString set)
{
    QString formalized = "{ ";
    foreach (QChar el, set) {
        formalized.append(el);
        if (el != set.at(set.count() - 1))
            formalized.append(", ");
    }
    formalized.append(" }");
    return formalized;
}

QString SetTheory::calculateUnion(QString A, QString B)
{
    foreach (QChar el, B)
        if (!A.contains(el))
            A.append(el);
    return A;
}

QString SetTheory::calculateIntersection(QString A, QString B)
{
    QString intersection;
    foreach (QChar el, B)
        if (A.contains(el))
            intersection.append(el);
    return intersection;
}

QString SetTheory::calculateDifference(QString A, QString B)
{
    foreach (QChar el, B)
        A.remove(el);
    return A;
}

QString SetTheory::generateSetRelation()
{
    QString setRelation;
    QString sets = "ABC";
    bool keepSet = true;
    QChar set1 = sets.at(qrand() % sets.count());
    if (keepSet && qrand() % 2) keepSet = false; else sets.remove(set1);
    QChar set2;
    do set2 = sets.at(qrand() % sets.count()); while (set2 == set1);
    if (keepSet && qrand() % 2) keepSet = false; else sets.remove(set2);
    setRelation.append("(" + QString(qrand() % 3 ? "" : "¬") + set1
                                           + operations.at(qrand() % operations.count())
                                           + QString(qrand() % 3 ? "" : "¬") + set2 + ")");
    QChar set3 = sets.at(qrand() % sets.count());
    if (keepSet) keepSet = false; else sets.remove(set3);
    qrand() % 2
            ? setRelation.prepend(QString(qrand() % 3 ? "" : "¬") + set3 + operations.at(qrand() % operations.count()))
            : setRelation.append(operations.at(qrand() % operations.count()) + QString(qrand() % 3 ? "" : "¬") + set3);
    if (qrand() % 2) {
        setRelation.prepend("(");
        setRelation.append(")");
    }
    qrand() % 2
            ? setRelation.prepend(QString(qrand() % 3 ? "" : "¬") + sets.at(0) + operations.at(qrand() % operations.count()))
            : setRelation.append(operations.at(qrand() % operations.count()) + QString(qrand() % 3 ? "" : "¬") + sets.at(0));
    return setRelation;
}

QGraphicsScene *SetTheory::drawEulerVennDiagram(QString setRelation)
{
    QGraphicsScene *scene = new QGraphicsScene;
    QRect U = QRect(0, 0, 400, 400);
    scene->addRect(U, QPen(Qt::white), QBrush(Qt::white, Qt::SolidPattern));
    QRect A = QRect(50, 50, 200, 200);
    QRect B = QRect(150, 50, 200, 200);
    QRect C = QRect(100, 150, 200, 200);
    scene->addEllipse(A, QPen(Qt::green));
    scene->addText("A")->setPos(100, 100);
    scene->addEllipse(B, QPen(Qt::red));
    scene->addText("B")->setPos(300, 100);
    scene->addEllipse(C, QPen(Qt::black));
    scene->addText("C")->setPos(200, 300);
    QPainterPath path;
    QString poliz;
    QStack<QChar> stack;
    foreach (QChar el, setRelation) {
        if (operations.contains(el)) {
            stack.push(el);
        } else if (el == '(') {
            stack.push('#');
        } else if (el == ')') {
            while (stack.top() != '#')
                poliz.append(stack.pop());
            stack.pop();
        } else {
            poliz.append(el);
        }
    }
    while (!stack.isEmpty())
        poliz.append(stack.pop());
    bool complement = false;
    foreach (QChar el, poliz) {
        if (operations.contains(el)) {
            QList<QPainterPath> paths;
            paths.append(QPainterPath());
            paths.append(QPainterPath());
            for (int i = 0; i < 2; ++i) {
                QChar popped = stack.pop();
                if (popped.toUpper() == 'A') {
                    paths[i].addEllipse(A);
                    if (popped == 'a') {
                        QPainterPath u;
                        u.addRect(U);
                        paths[i] = u.subtracted(paths[i]);
                    }
                } else if (popped.toUpper() == 'B') {
                    paths[i].addEllipse(B);
                    if (popped == 'b') {
                        QPainterPath u;
                        u.addRect(U);
                        paths[i] = u.subtracted(paths[i]);
                    }
                } else if (popped.toUpper() == 'C') {
                    paths[i].addEllipse(C);
                    if (popped == 'c') {
                        QPainterPath u;
                        u.addRect(U);
                        paths[i] = u.subtracted(paths[i]);
                    }
                } else if (popped == 'P') {
                    paths[i] = path;
                }
            }
            if (el == 'U') {
                path = paths.last().united(paths.first());
            } else if (el == QString("П").at(0)) {
                path = paths.last().intersected(paths.first());
            } else {
                path = paths.last().subtracted(paths.first());
            }
            stack.push('P');
        } else if (el == QString("¬").at(0)) {
            complement = true;
        } else {
            if (complement) {
                el = el.toLower();
                complement = false;
            }
            stack.push(el);
        }
    }
    scene->addPath(path, QPen(Qt::black), QBrush(Qt::black, Qt::FDiagPattern));
    return scene;
}

QStringList SetTheory::generatePack(bool withAnswers)
{
    QStringList pack;
    pack.append(QString());
    if (withAnswers)
        pack.append(QString());
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("1) Перечислите все подмножества множества:\r\n");
    for (int i = 0; i < 2; ++i) {
        QString set = generateSet();
        for (int j = 0; j < pack.count(); ++j)
            pack[j].append("\r\n" + formalizeSet(set) + "\r\nОтвет:");
        if (withAnswers)
            pack.last().append(";" + calculateSubsets(set));
    }
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n\r\n2) Найдите объединение следующих множеств A и B:\r\n");
    QString A = generateSet();
    QString B = generateSet();
    for (int i = 0; i < pack.count(); ++i) {
        pack[i].append("\r\nA = " + formalizeSet(A));
        pack[i].append("\r\nB = " + formalizeSet(B));
        pack[i].append("\r\nОтвет:");
    }
    if (withAnswers)
        pack.last().append(";" + formalizeSet(calculateUnion(A, B)));
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n\r\n3) Найдите пересечение следующих множеств A и B:\r\n");
    A = generateSet();
    B = generateSet();
    for (int i = 0; i < pack.count(); ++i) {
        pack[i].append("\r\nA = " + formalizeSet(A));
        pack[i].append("\r\nB = " + formalizeSet(B));
        pack[i].append("\r\nОтвет:");
    }
    if (withAnswers)
        pack.last().append(";" + formalizeSet(calculateIntersection(A, B)));
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n\r\n4) Найдите разность следующих множеств A и B (A\\B):\r\n");
    A = generateSet();
    B = generateSet();
    for (int i = 0; i < pack.count(); ++i) {
        pack[i].append("\r\nA = " + formalizeSet(A));
        pack[i].append("\r\nB = " + formalizeSet(B));
        pack[i].append("\r\nОтвет:");
    }
    if (withAnswers)
        pack.last().append(";" + formalizeSet(calculateDifference(A, B)));
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n\r\n5) Изобразите с помощью диаграммы Эйлера-Венна множество:\r\n");
    QString setRelation = generateSetRelation();
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("\r\n" + setRelation);
    if (withAnswers)
        pack.last().append("\r\nДиаграмма находится в директории с файлом");
    return pack;
}
