#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QStandardItemModel>

#include "../../../../pugixml/src/pugixml.hpp"
#include "common.h"

class XMLhandler
{
public:
    static void init();
    static void addRansacInfo(int a_ransacNbPoint, int a_picNbPoint, float a_percentTh, float a_rho, int a_J, float a_time);
    static QStandardItemModel *getItemModel();

private:
    static pugi::xml_document doc;
    static pugi::xml_parse_result result;
    static std::string filename;
};

#endif // XMLHANDLER_H
