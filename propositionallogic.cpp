/*
    propositionallogic.cpp
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

#include "propositionallogic.h"
#include <QTime>
#include <QStack>
#include <QMap>
#include <QStringList>

PropositionalLogic::PropositionalLogic(QObject *parent) :
    QObject(parent)
{
    QTime time = QTime::currentTime();
    qsrand(time.msec());
    operations = "&v>~"; //¬
}

QString PropositionalLogic::generateProposition()
{
    QString proposition;
    qint8 amount = qrand() % 3 + 4;
    qint8 keepStatement = amount - 3;
    QString stats = "XYZ";
    for (int i = 0; i < amount; ++i) {
        QChar s = stats.at(qrand() % stats.count());
        if ((keepStatement && qrand() % 2) || (keepStatement && stats.count() == 1))
            --keepStatement;
        else
            stats.remove(s);
        if (!i) {
            proposition.append(QString(qrand() % 4 ? "" : "¬") + s);
        } else {
            if (qrand() % 2) {
                proposition.append(operations.at(qrand() % operations.count())
                                   + QString(qrand() % 4 ? "" : "¬") + s);
            } else {
                proposition.prepend(QString(qrand() % 4 ? "" : "¬") + s
                                    + operations.at(qrand() % operations.count()));
            }
            if (qrand() % 2) {
                proposition.append(")");
                proposition.prepend("(");
            }
        }
    }
    return proposition;
}

bool PropositionalLogic::calculateProposition(QString proposition)
{
    QString poliz;
    QStack<QChar> stack;
    foreach (QChar el, proposition) {
        if (operations.contains(el)) {
            if (stack.isEmpty()) {
                stack.push(el);
            } else {
                if (operations.indexOf(stack.top()) < operations.indexOf(el)) {
                    stack.push(el);
                } else {
                    do {
                        poliz.append(stack.pop());
                        if (stack.isEmpty())
                            break;
                    } while (operations.indexOf(stack.top()) >= operations.indexOf(el));
                    stack.push(el);
                }
            }
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
    QMap<QChar, bool> statements;
    statements.insert('X', true);
    statements.insert('Y', false);
    statements.insert('Z', false);
    QStack<bool> operands;
    bool invert = false;
    foreach (QChar el, poliz) {
        if (statements.contains(el)) {
            operands.push(statements.value(el) + invert);
            invert = false;
        } else if (el == QString("¬").at(0)) {
            invert = true;
        } else if (operations.contains(el)) {
            if (el == '&') {
                operands.push(operands.pop() && operands.pop());
            } else if (el == 'v') {
                bool s1 = operands.pop();
                operands.push(operands.pop() || s1);
            } else if (el == '>') {
                bool s1 = operands.pop();
                bool s2 = operands.pop();
                if (s1)
                    operands.push(true);
                else if (!s2)
                    operands.push(true);
                else
                    operands.push(false);
            } else if (el == '~') {
                if (operands.pop() == operands.pop())
                    operands.push(true);
                else
                    operands.push(false);
            }
        }
    }
    return operands.last();
}

QStringList PropositionalLogic::generatePack(bool withAnswers)
{
    QStringList pack;
    pack.append(QString());
    if (withAnswers)
        pack.append(QString());
    for (int i = 0; i < pack.count(); ++i)
        pack[i].append("1) Вычислите значение истинности следующих формул логики высказываний, если X=И, Y=Л, Z=Л:");
    for (int i = 0; i < 4; ++i) {
        QString prop = generateProposition();
        for (int j = 0; j < pack.count(); ++j)
            pack[j].append("\r\n\r\n" + prop + "\r\nОтвет:");
        if (withAnswers)
            pack.last().append(";" + QString(calculateProposition(prop) ? "И" : "Л"));
    }
    return pack;
}
