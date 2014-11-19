#include "xmlhandler.h"

// Static initialization
pugi::xml_document XMLhandler::doc;
pugi::xml_parse_result XMLhandler::result;
std::string XMLhandler::filename = "../../../Statistics/Stat.xml";

// ------------- init() -------------
void XMLhandler::init()
{
    result = doc.load_file(filename.c_str());
    DEBUG_MSG("Initialized XML handler");
}

void XMLhandler::addRansacInfo(int a_ransacNbPoint, int a_picNbPoint, float a_percentTh, float a_rho, int a_J, float a_time)
{
#ifdef DEBUG
    pugi::xml_node statNode = doc.child("Statistics");
    pugi::xml_node nodeRansac = statNode.append_child("RANSAC");

    nodeRansac.append_attribute("nb_point_curve") = a_ransacNbPoint;
    nodeRansac.append_attribute("nb_point_pic") = a_picNbPoint;
    nodeRansac.append_attribute("percent_threshold") = a_percentTh;
    nodeRansac.append_attribute("rho") = a_rho;
    nodeRansac.append_attribute("nb_iter") = a_J;
    nodeRansac.append_attribute("time") = a_time;

    doc.save_file(filename.c_str());
#endif
}

QStandardItemModel* XMLhandler::getItemModel()
{
    pugi::xml_node statNode = doc.child("Statistics");

    QStandardItemModel *model = new QStandardItemModel();

    // RANSAC PART
    model->setHorizontalHeaderItem(0,new QStandardItem("nb_point_curve"));
    model->setHorizontalHeaderItem(1,new QStandardItem("nb_point_pic"));
    model->setHorizontalHeaderItem(2,new QStandardItem("percent_threshold"));
    model->setHorizontalHeaderItem(3,new QStandardItem("rho"));
    model->setHorizontalHeaderItem(4,new QStandardItem("nb_iter"));
    model->setHorizontalHeaderItem(5,new QStandardItem("time"));

    QStandardItem *item;
    int indexCurrentRow = 0;
    foreach(pugi::xml_node ransacNode, statNode.children("RANSAC"))
    {
        int indexAttr = 0;
        foreach(pugi::xml_attribute attr, ransacNode.attributes())
        {
            item = new QStandardItem(attr.value());
            model->setItem(indexCurrentRow,indexAttr,item);
            indexAttr++;
        }
        indexCurrentRow++;
    }

    return model;
}
