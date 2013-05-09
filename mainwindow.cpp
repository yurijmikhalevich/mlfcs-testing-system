/*
    mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "numbersystems.h"
#include "settheory.h"
#include "propositionallogic.h"
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QPainter>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <qmath.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_numbersSystems_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выбирите директорию для сохранения работ", QDir::homePath());
    if (!dir.isEmpty()) {
        QDir qdir;
        QString ta[2] = { "tasks", "answers" };
        if (ui->withAnswers->isChecked()) {
            qdir.mkdir(dir + QDir::separator() + ta[0]);
            qdir.mkdir(dir + QDir::separator() + ta[1]);
        }
        NumberSystems generator;
        for (int i = 0; i < ui->variants->value(); ++i) {
            QStringList pack = generator.generatePack(ui->withAnswers->isChecked());
            if (ui->withAnswers->isChecked()) {
                for (int j = 0; j < 2; ++j) {
                    QFile file(dir + QDir::separator() + ta[j] + QDir::separator() + "var" + QString::number(i) + ".csv");
                    file.open(QFile::WriteOnly);
                    QTextStream stream(&file);
                    stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                    stream << pack.at(j);
                    file.close();
                }
            } else {
                QFile file(dir + QDir::separator() + "var" + QString::number(i) + ".csv");
                file.open(QFile::WriteOnly);
                QTextStream stream(&file);
                stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                stream << pack.first();
                file.close();
            }
        }
    }
}

void MainWindow::on_binaryArithmetic_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выбирите директорию для сохранения работ", QDir::homePath());
    if (!dir.isEmpty()) {
        QDir qdir;
        QString ta[2] = { "tasks", "answers" };
        if (ui->withAnswers->isChecked()) {
            qdir.mkdir(dir + QDir::separator() + ta[0]);
            qdir.mkdir(dir + QDir::separator() + ta[1]);
        }
        NumberSystems generator;
        for (int var = 0; var < ui->variants->value(); ++var) {
            QStringList pack;
            pack.append(QString());
            if (ui->withAnswers->isChecked())
                pack.append(QString());
            for (int i = 0; i < pack.count(); ++i)
                pack[i].append("1) Решите следующие примеры:\r\n");
            for (int i = 0; i < 2; ++i) {
                QString bin1 = generator.convert(generator.generateDecNumber(), 10, 2);
                QString bin2 = generator.convert(generator.generateDecNumber(), 10, 2);
                for (int j = 0; j < pack.count(); ++j)
                    pack[j].append("\r\n" + bin1 + ";+;" + bin2 + ";=;");
                if (pack.count() != 1) {
                    pack.last().append(generator.convert(generator.convert(bin1, 2, 10).toDouble()
                                                         + generator.convert(bin2, 2, 10).toDouble(), 2));
                }
            }
            for (int i = 0; i < 2; ++i) {
                QString bin1 = generator.convert(generator.generateDecNumber(), 10, 2);
                QString bin2 = generator.convert(generator.generateDecNumber(), 10, 2);
                for (int j = 0; j < pack.count(); ++j)
                    pack[j].append("\r\n" + bin1 + ";-;" + bin2 + ";=;");
                if (pack.count() != 1) {
                    qreal result = generator.convert(bin1, 2, 10).toDouble() - generator.convert(bin2, 2, 10).toDouble();
                    if (result < 0)
                        pack.last().append("-");
                    pack.last().append(generator.convert(qAbs(result), 2));
                }
            }
            for (int i = 0; i < 2; ++i) {
                QString bin1 = generator.convert(generator.generateDecNumber(), 10, 2);
                QString bin2 = generator.convert(generator.generateDecNumber(), 10, 2);
                for (int j = 0; j < pack.count(); ++j)
                    pack[j].append("\r\n" + bin1 + ";*;" + bin2 + ";=;");
                if (pack.count() != 1) {
                    pack.last().append(generator.convert(generator.convert(bin1, 2, 10).toDouble()
                                                         * generator.convert(bin2, 2, 10).toDouble(), 2));
                }
            }
            for (int i = 0; i < 2; ++i) {
                QString bin1 = generator.convert(generator.generateDecNumber(), 10, 2);
                QString bin2 = generator.convert(generator.generateDecNumber(), 10, 2);
                for (int j = 0; j < pack.count(); ++j)
                    pack[j].append("\r\n" + bin1 + ";/;" + bin2 + ";=;");
                if (pack.count() != 1) {
                    pack.last().append(generator.convert(generator.convert(bin1, 2, 10).toDouble()
                                                         / generator.convert(bin2, 2, 10).toDouble(), 2));
                }
            }
            if (ui->withAnswers->isChecked()) {
                for (int j = 0; j < 2; ++j) {
                    QFile file(dir + QDir::separator() + ta[j] + QDir::separator() + "var" + QString::number(var) + ".csv");
                    file.open(QFile::WriteOnly);
                    QTextStream stream(&file);
                    stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                    stream << pack.at(j);
                    file.close();
                }
            } else {
                QFile file(dir + QDir::separator() + "var" + QString::number(var) + ".csv");
                file.open(QFile::WriteOnly);
                QTextStream stream(&file);
                stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                stream << pack.first();
                file.close();
            }
        }
    }
}

void MainWindow::on_setTheory_clicked()
{

    QString dir = QFileDialog::getExistingDirectory(this, "Выбирите директорию для сохранения работ", QDir::homePath());
    if (!dir.isEmpty()) {
        QDir qdir;
        QString ta[2] = { "tasks", "answers" };
        if (ui->withAnswers->isChecked()) {
            qdir.mkdir(dir + QDir::separator() + ta[0]);
            qdir.mkdir(dir + QDir::separator() + ta[1]);
        }
        SetTheory generator;
        for (int i = 0; i < ui->variants->value(); ++i) {
            QStringList pack = generator.generatePack(ui->withAnswers->isChecked());
            if (ui->withAnswers->isChecked()) {
                for (int j = 0; j < 2; ++j) {
                    QFile file(dir + QDir::separator() + ta[j] + QDir::separator() + "var" + QString::number(i) + ".csv");
                    file.open(QFile::WriteOnly);
                    QTextStream stream(&file);
                    stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                    stream << pack.at(j);
                    file.close();
                }
            } else {
                QFile file(dir + QDir::separator() + "var" + QString::number(i) + ".csv");
                file.open(QFile::WriteOnly);
                QTextStream stream(&file);
                stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                stream << pack.first();
                file.close();
            }
            if (ui->withAnswers->isChecked()) {
                QGraphicsScene *diagram = generator.drawEulerVennDiagram(pack.first().section("\r\n", -1));
                QImage image(diagram->width(), diagram->height(), QImage::Format_ARGB32_Premultiplied);
                QPainter painter(&image);
                diagram->render(&painter);
                image.save(dir + QDir::separator() + ta[1] + QDir::separator() + "var" + QString::number(i) + ".png");
            }
        }
    }
}

void MainWindow::on_propositionalLogic_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выбирите директорию для сохранения работ", QDir::homePath());
    if (!dir.isEmpty()) {
        QDir qdir;
        QString ta[2] = { "tasks", "answers" };
        if (ui->withAnswers->isChecked()) {
            qdir.mkdir(dir + QDir::separator() + ta[0]);
            qdir.mkdir(dir + QDir::separator() + ta[1]);
        }
        PropositionalLogic generator;
        for (int i = 0; i < ui->variants->value(); ++i) {
            QStringList pack = generator.generatePack(ui->withAnswers->isChecked());
            if (ui->withAnswers->isChecked()) {
                for (int j = 0; j < 2; ++j) {
                    QFile file(dir + QDir::separator() + ta[j] + QDir::separator() + "var" + QString::number(i) + ".csv");
                    file.open(QFile::WriteOnly);
                    QTextStream stream(&file);
                    stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                    stream << pack.at(j);
                    file.close();
                }
            } else {
                QFile file(dir + QDir::separator() + "var" + QString::number(i) + ".csv");
                file.open(QFile::WriteOnly);
                QTextStream stream(&file);
                stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                stream << pack.first();
                file.close();
            }
        }
    }
}

void MainWindow::on_worksChecking_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Укажите директорию с файлами выполненных работ", QDir::homePath());
    if (!dir.isEmpty()) {
        QDir qdir;
        qdir.setPath(dir);
        QStringList files = qdir.entryList(QStringList("var*.csv"), QDir::Files, QDir::Name);
        if (files.isEmpty()) {
            QMessageBox::warning(this, "Произошла ошибка", "Директория не содержит файлов работ. Убедитесь, что вы правильно указали директорию и файлы имеют имена вида «var*.csv» и повторите попытку.", QMessageBox::Ok, 0);
        } else {
            QPlainTextEdit *edit = new QPlainTextEdit;
            edit->setWindowTitle("Результаты проверки");
            edit->setReadOnly(true);
            edit->show();
            foreach (QString fileName, files) {
                QFile file(dir + QDir::separator() + fileName);
                file.open(QFile::ReadOnly);
                QTextStream stream(&file);
                stream.setCodec(QTextCodec::codecForName("Windows-1251"));
                QStringList lines = stream.readAll().split("\r\n");
                qint8 errorsCount = 0;
                if (lines.first() == "1) Представьте следующие числа в полиномиальной записи:") {
                    NumberSystems master;
                    for (int i = 2; i <= 11; i += 3)
                        if (master.calculatePolinom(lines.at(i).section("(", 0, 0), lines.at(i).section("(", 1, 1).section(")", 0, 0).toInt()) != lines.at(i + 1).section(";", 1, 1))
                            ++errorsCount;
                    for (int i = 17; i <= 26; i += 3)
                        if (master.calculatePolinom(lines.at(i).section(";", 1, 1).section("(", 0, 0), lines.at(i).section(";", 1, 1).section("(", 1, 1).section(")", 0, 0).toInt()) != lines.at(i - 1))
                            ++errorsCount;
                    for (int i = 30; i <= 36; i += 3)
                        if (master.convert(lines.at(i).section("(", 0, 0), lines.at(i).section("(", 1, 1).section(")", 0, 0).toInt(), 10) != lines.at(i + 1).section(";", 1, 1))
                            ++errorsCount;
                    for (int i = 41; i <= 47; i += 3)
                        if (master.convert(lines.at(i).section(";", 1, 1), 10, lines.at(i + 1).section(";", 1, 1).section("(", 1, 1).section(")", 0, 0).toInt()) != lines.at(i + 1).section(";", 1, 1).section("(", 0, 0))
                            ++errorsCount;
                    edit->appendPlainText(fileName + " - " + QString::number(errorsCount) + " ошибок в 14 заданиях");
                } else if (lines.first() == "1) Решите следующие примеры:") {
                    NumberSystems master;
                    for (int i = 2; i <= 3; ++i)
                        if (master.convert(master.convert(lines.at(i).section(";", 0, 0), 2, 10).toDouble()
                                           + master.convert(lines.at(i).section(";", 2, 2), 2, 10).toDouble(), 2)
                                != lines.at(i).section(";", 4, 4))
                            ++errorsCount;
                    for (int i = 4; i <= 5; ++i) {
                        qreal result = master.convert(lines.at(i).section(";", 0, 0), 2, 10).toDouble()
                                - master.convert(lines.at(i).section(";", 2, 2), 2, 10).toDouble();
                        QString res;
                        if (result < 0)
                            res.append("-");
                        res.append(master.convert(qAbs(result), 2));
                        if (res != lines.at(i).section(";", 4, 4))
                            ++errorsCount;
                    }
                    for (int i = 6; i <= 7; ++i)
                        if (master.convert(master.convert(lines.at(i).section(";", 0, 0), 2, 10).toDouble()
                                           * master.convert(lines.at(i).section(";", 2, 2), 2, 10).toDouble(), 2)
                                != lines.at(i).section(";", 4, 4))
                            ++errorsCount;
                    for (int i = 8; i <= 9; ++i)
                        if (master.convert(master.convert(lines.at(i).section(";", 0, 0), 2, 10).toDouble()
                                           / master.convert(lines.at(i).section(";", 2, 2), 2, 10).toDouble(), 2)
                                != lines.at(i).section(";", 4, 4))
                            ++errorsCount;
                    edit->appendPlainText(fileName + " - " + QString::number(errorsCount) + " ошибок в 8 заданиях");
                } else if (lines.first() == "1) Перечислите все подмножества множества:") {
                    SetTheory master;
                    for (int i = 2; i <= 4; i += 2) {
                        QString set = lines.at(i).section(";", 0, 0);
                        set.remove(" }");
                        set.remove("{ ");
                        set.remove(", ");
                        QString subsets = master.calculateSubsets(set);
                        QString answer = lines.at(i + 1).section(";", 1, 1);
                        if (subsets != answer)
                            ++errorsCount;
                    }
                    QString A = lines.at(9).section(";", 0, 0);
                    QString B = lines.at(10).section(";", 0, 0);
                    foreach (QChar el, A)
                        if (!el.isDigit())
                            A.remove(el);
                    foreach (QChar el, B)
                        if (!el.isDigit())
                            B.remove(el);
                    if (master.formalizeSet(master.calculateUnion(A, B)) != lines.at(11).section(";", 1, 1))
                        ++errorsCount;
                    A = lines.at(15).section(";", 0, 0);
                    B = lines.at(16).section(";", 0, 0);
                    foreach (QChar el, A)
                        if (!el.isDigit())
                            A.remove(el);
                    foreach (QChar el, B)
                        if (!el.isDigit())
                            B.remove(el);
                    if (master.formalizeSet(master.calculateIntersection(A, B)) != lines.at(17).section(";", 1, 1))
                        ++errorsCount;
                    A = lines.at(21).section(";", 0, 0);
                    B = lines.at(22).section(";", 0, 0);
                    foreach (QChar el, A)
                        if (!el.isDigit())
                            A.remove(el);
                    foreach (QChar el, B)
                        if (!el.isDigit())
                            B.remove(el);
                    if (master.formalizeSet(master.calculateDifference(A, B)) != lines.at(23).section(";", 1, 1))
                        ++errorsCount;
                    edit->appendPlainText(fileName + " - " + QString::number(errorsCount) + " ошибок в 5 заданиях. Диаграмма Эйлера-Венна не проверяется");
                } else if (lines.first() == "1) Вычислите значение истинности следующих формул логики высказываний, если X=И, Y=Л, Z=Л:") {
                    PropositionalLogic master;
                    for (int i = 2; i <= 11; i += 3)
                        if ((master.calculateProposition(lines.at(i).section(";", 0, 0)) ? "И" : "Л") != lines.at(i + 1).section(";", 1, 1))
                            ++errorsCount;
                    edit->appendPlainText(fileName + " - " + QString::number(errorsCount) + " ошибок в 4 заданиях");
                } else {
                    edit->appendPlainText(fileName + " - не корректный файл");
                }
                file.close();
                qApp->processEvents();
            }
        }
    }
}
