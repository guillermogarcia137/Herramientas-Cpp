#include "design.hpp"
#include <iostream>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <cwchar>
#endif
const char* help=R"(Herramientas-Cpp · ocho utilidades de diseño
Uso: diseno COMANDO ARGUMENTOS
  degradado NUEVO.svg COLOR_A COLOR_B
  mosaico NUEVO.svg COLOR_A COLOR_B SEMILLA
  puntos NUEVO.svg FONDO COLOR_PUNTOS
  monograma NUEVO.svg INICIALES FONDO TEXTO
  contraste COLOR_TEXTO COLOR_FONDO
  paleta NUEVO.css COLOR_BASE
  espaciado NUEVO.svg
  galeria CARPETA NUEVO.html
Colores #RRGGBB entre comillas. Destinos nuevos, sin sobrescritura.
)";
int run(const std::vector<std::string>&a){using namespace design;try{
 if(a.empty()||a[0]=="--help"){std::cout<<help;return 0;}
 std::string content,target;const auto&command=a[0];
 if(command=="contraste"&&a.size()==3){double ratio=contrast(a[1],a[2]);std::cout<<std::fixed<<std::setprecision(3)<<"Ratio: "<<ratio<<"\nAA normal: "<<(ratio>=4.5?"SI":"NO")<<"\nAA grande: "<<(ratio>=3?"SI":"NO")<<"\nAAA normal: "<<(ratio>=7?"SI":"NO")<<'\n';return 0;}
 if(command=="degradado"&&a.size()==4){target=a[1];content=gradient(a[2],a[3]);}
 else if(command=="mosaico"&&a.size()==5){if(a[4].empty()||!std::all_of(a[4].begin(),a[4].end(),[](unsigned char c){return std::isdigit(c)!=0;}))throw std::invalid_argument("Semilla entera no negativa.");std::size_t parsed=0;auto seed=std::stoul(a[4],&parsed);if(parsed!=a[4].size()||seed>0xFFFFFFFFul)throw std::invalid_argument("Semilla entera entre 0 y 4294967295.");target=a[1];content=mosaic(a[2],a[3],static_cast<unsigned>(seed));}
 else if(command=="puntos"&&a.size()==4){target=a[1];content=dots(a[2],a[3]);}
 else if(command=="monograma"&&a.size()==5){target=a[1];content=monogram(a[2],a[3],a[4]);}
 else if(command=="paleta"&&a.size()==3){target=a[1];content=palette(a[2]);}
 else if(command=="espaciado"&&a.size()==2){target=a[1];content=spacing();}
 else if(command=="galeria"&&a.size()==3){target=a[2];content=gallery(utf8path(a[1]));}
 else throw std::invalid_argument("Comando o argumentos incorrectos; usa --help.");
 auto extension=utf8path(target).extension().string();const std::string expected=command=="galeria"?".html":command=="paleta"?".css":".svg";if(extension!=expected)throw std::invalid_argument("El destino debe terminar en "+expected);
 write_new(utf8path(target),content);std::cout<<"Creado: "<<target<<'\n';return 0;
 }catch(const std::exception&error){std::cerr<<"Error: "<<error.what()<<'\n';return 1;}}
#ifdef _WIN32
int wmain(int argc,wchar_t**argv){std::vector<std::string>a;for(int i=1;i<argc;i++){int length=static_cast<int>(std::wcslen(argv[i]));int count=WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,argv[i],length,nullptr,0,nullptr,nullptr);if(count==0&&length!=0)return 1;std::string value(count,0);WideCharToMultiByte(CP_UTF8,WC_ERR_INVALID_CHARS,argv[i],length,value.data(),count,nullptr,nullptr);a.push_back(value);}return run(a);}
#else
int main(int argc,char**argv){return run({argv+1,argv+argc});}
#endif
