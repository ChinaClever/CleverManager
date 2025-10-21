/*
 * ds_lcdnumwid.cpp
 * 设备数据显示LCD定制
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "ds_lcdnumwid.h"
#include "ui_ds_lcdnumwid.h"
#include "common.h"

DS_LcdNumWid::DS_LcdNumWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DS_LcdNumWid)
{
    ui->setupUi(this);
}

DS_LcdNumWid::~DS_LcdNumWid()
{
    delete ui;
}

/**
 * @brief 设置单位
 * @param str
 */
void DS_LcdNumWid::setUnit(const QString &title,const QString &str)
{
    ui->titleLab->setText(title);
    ui->label->setText(str);
}

/**
 * @brief 显示数字
 * @param data
 */
void DS_LcdNumWid::display(double data)
{
    ui->lcdNumber->display(data);
}

void DS_LcdNumWid::display(const QString &str)
{
    ui->lcdNumber->display(str);
}

/**
 * @brief 显示位数
 * @param value
 */
void DS_LcdNumWid::setDigitCount(int value)
{
    ui->lcdNumber->setDigitCount(value);
    this->setMaximumWidth(value *70); // 宽度限制
}

void DS_LcdNumWid::setStyleSheet(const QString &str)
{
    ui->lcdNumber->setStyleSheet(str);
    ui->label->setStyleSheet(str);
}

void DS_LcdNumWid::setTitle(const QString &str)
{
    ui->titleLab->setText(str);
}

