#include "design.hpp"
#include <chrono>
#include <iostream>
void check(bool ok,const char*message){if(!ok)throw std::runtime_error(message);std::cout<<"OK: "<<message<<'\n';}
int main(){using namespace design;auto root=fs::temp_directory_path()/("diseno-cpp-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));fs::create_directory(root);try{
 check(contrast("#000000","#FFFFFF")==21,"Contraste blanco/negro");check(contrast("#123456","#123456")==1,"Contraste idéntico");bool invalid=false;try{rgb("red");}catch(const std::invalid_argument&){invalid=true;}check(invalid,"Rechazo de colores inválidos");
 check(monogram("<&>","#123456","#ABCDEF").find("&lt;&amp;&gt;")!=std::string::npos,"Texto SVG escapado");check(mosaic("#123456","#ABCDEF",11)==mosaic("#123456","#ABCDEF",11),"Semilla reproducible");check(base64({'h','o','l','a'})=="aG9sYQ==","Base64");check(palette("#123456").find("--tono-9")!=std::string::npos,"Nueve tonos CSS");
 auto path=root/"new.txt";write_new(path,"original");bool collision=false;try{write_new(path,"otro");}catch(const std::runtime_error&){collision=true;}std::ifstream input(path);std::string value;input>>value;input.close();check(collision&&value=="original","Preservación de destino");
 write_new(root/"image.png",std::string("PNG-DE-PRUEBA"));check(gallery(root).find("data:image/png;base64,")!=std::string::npos,"Galería integra archivos");check(spacing().find("64 px")!=std::string::npos,"Escala de espaciado");fs::remove_all(root);return 0;
 }catch(const std::exception&error){fs::remove_all(root);std::cerr<<error.what()<<'\n';return 1;}}
