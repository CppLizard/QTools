﻿#include "XmlData.h"

QList<QString>           XmlData::lSoftType;  // 软件分类
QList<QList<QString> >   XmlData::llSoftName; // 软件名称
QList<QList<QString> >   XmlData::llSoftPath; // 软件路径
QList<QList<QString> >   XmlData::llSoftIcon; // 软件图标

XmlData::XmlData()
{
    // 读取XML文件
    readXml();
}

// 读取XML文件
bool XmlData::readXml()
{
    // 清除已保存的数据
    lSoftType.clear();
    llSoftName.clear();
    llSoftPath.clear();
    llSoftIcon.clear();
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    // 读取根节点
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        // 创建软件分类文件夹
        QString dirPath = "./" + QString(type.attribute("name").value());
        createFolder(dirPath);
        // 软件名、路径、图标
        pugi::xml_node soft = type.child("soft");
        QStringList name;
        QStringList path;
        QStringList icon;
        while (soft)
        {
            name.append(soft.attribute("name").value());
            path.append(soft.attribute("path").value());
            icon.append(soft.attribute("icon").value());
            soft = soft.next_sibling();
        }
        // 软件分类
        lSoftType.append(type.attribute("name").value());
        // 软件信息
        llSoftName.append(name);
        llSoftPath.append(path);
        llSoftIcon.append(icon);
        type = type.next_sibling();
    }
    return true;
}

// 创建文件夹
bool XmlData::createFolder(QString dirPath)
{
    QDir dir;
    // 判断文件夹是否存在
    if (!dir.exists(dirPath))
    {
        // 不存在创建文件夹
        dir.mkdir(dirPath);
        return true;
    }
    return false;
}

// 判断软件分类是否存在
bool XmlData::isType(QString softType)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        if (QString(type.attribute("name").value()) == softType)
        {
            return true;
        }
        type = type.next_sibling();
    }
    return false;
}

// 判断软件名是否存在
bool XmlData::isSoft(QString softName)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        // 查找软件名
        pugi::xml_node soft = type.child("soft");
        while (soft)
        {
            if (QString(soft.attribute("name").value()) == softName)
            {
                return true;
            }
            soft = soft.next_sibling();
        }
        type = type.next_sibling();
    }
    return false;
}

// 添加软件分类到XML文件中
bool XmlData::addTypeXml(QString softType)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 添加软件分类节点
    pugi::xml_node newNode = root.append_child("type");
    // 添加软件分类属性
    newNode.append_attribute("name").set_value(softType.toStdString().c_str());
    // 添加软件节点
    pugi::xml_node newChild = newNode.append_child("soft");
    // 添加软件属性
    newChild.append_attribute("name").set_value("添加");
    newChild.append_attribute("path").set_value("");
    newChild.append_attribute("icon").set_value("add.png");
    // 保存XML文件
    doc.save_file(xmlFilePath);
    return true;
}

// 添加软件到XML文件中
bool XmlData::addSoftXml(QString softType, QString softName, QString softPath, QString softIcon)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        // 查找软件分类
        if (QString(type.attribute("name").value()) == softType)
        {
            pugi::xml_node soft = type.child("soft");
            while (soft)
            {
                if (QString(soft.attribute("name").value()) == "添加")
                {
                    // 添加节点
                    pugi::xml_node newNode = type.insert_child_before("soft", soft);
                    // 添加属性
                    newNode.append_attribute("name").set_value(softName.toStdString().c_str());
                    newNode.append_attribute("path").set_value(softPath.toStdString().c_str());
                    newNode.append_attribute("icon").set_value(softIcon.toStdString().c_str());
                    // 保存XML文件
                    doc.save_file(xmlFilePath);
                    return true;
                }
                soft = soft.next_sibling();
            }
        }
        type = type.next_sibling();
    }
    return false;
}

// XML文件中删除软件分类
bool XmlData::removeTypeXml(QString name)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        if (QString(type.attribute("name").value()) == name)
        {
            // 删除软件分类
            root.remove_child(type);
            // 保存XML文件
            doc.save_file(xmlFilePath);
            return true;
        }
        type = type.next_sibling();
    }
    return false;
}

// XML文件中修改软件分类
bool XmlData::modifyTypeXml(QString oldType, QString newType)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        if (QString(type.attribute("name").value()) == oldType)
        {
            // 修改属性
            type.attribute("name").set_value(newType.toStdString().c_str());
            // 保存XML文件
            doc.save_file(xmlFilePath);
            return true;
        }
        type = type.next_sibling();
    }
    return false;
}

// XML文件中删除软件
bool XmlData::removeSoftXml(QString name)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        // 查找软件名
        pugi::xml_node soft = type.child("soft");
        while (soft)
        {
            if (QString(soft.attribute("name").value()) == name)
            {
                // 删除软件
                type.remove_child(soft);
                // 保存XML文件
                doc.save_file(xmlFilePath);
                return true;
            }
            soft = soft.next_sibling();
        }
        type = type.next_sibling();
    }
    return false;
}

// XML文件中修改软件名
bool XmlData::modifySoftXml(QString oldName, QString newName)
{
    // 打开XML文件
    pugi::xml_document doc;
    if(!doc.load_file(xmlFilePath, pugi::parse_default, pugi::encoding_utf8))
    {
        qDebug() << "打开XML文件失败";
        return false;
    }
    pugi::xml_node root = doc.child("root");
    // 读取第一个节点
    pugi::xml_node type = root.child("type");
    while (type)
    {
        // 查找软件名
        pugi::xml_node soft = type.child("soft");
        while (soft)
        {
            if (QString(soft.attribute("name").value()) == oldName)
            {
                // 修改属性
                soft.attribute("name").set_value(newName.toStdString().c_str());
                // 保存XML文件
                doc.save_file(xmlFilePath);
                return true;
            }
            soft = soft.next_sibling();
        }
        type = type.next_sibling();
    }
    return false;
}
