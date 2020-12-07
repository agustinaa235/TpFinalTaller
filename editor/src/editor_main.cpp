#include <QApplication>
#include "editor.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPalette>
#include <string>
#include <iostream>
#include <fstream>
#include "map.h"
#include "map_translator.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QWidget ventana;

    ventana.setGeometry(50,50,500,500);

    ventana.setStyleSheet("background-color: blue");
    QLabel label("configuracion");

    QPushButton colores("editar colores");
    QPushButton personajes("editar personajes");

    QHBoxLayout configuracion;
    QVBoxLayout editar;
    // Agrego los widgets al layout vertical y horiontal;
    configuracion.addWidget(&label);
    editar.addWidget(&colores);
    editar.addWidget(&personajes);
    //configuracion.addH(&editar);
  //  QLabel label("El mapa a editar es\n");
    //ventana.setLayout(&configuracion);
    ventana.setLayout(&editar);
    ventana.show();
    //Editor editor;
    //std::string nombre_archivo("mapa.txt");
    //editor.CrearMapa(nombre_archivo);
    //return app.exec();
    Map m = MapTranslator::yamlToMap(YAML::LoadFile("../src/config.yaml"));
    std::cout << m.getColSize() << std::endl;
    std::cout << m.getRowSize() << std::endl;


    for (std::size_t i = 0; i < m.getRowSize(); i++) {
        for (std::size_t j = 0; j < m.getColSize(); j++) {
            m.setValue(i, j, m(i,j));
        }
    }

    YAML::Node node = MapTranslator::mapToYaml(m);
    std::ofstream file("confignew.yaml");
    file << node;
    file.close();
    return 0;
}
