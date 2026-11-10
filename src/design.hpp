#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <iterator>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace design {
namespace fs=std::filesystem;
inline fs::path utf8path(const std::string& text){return fs::path(std::u8string(text.begin(),text.end()));}
inline std::array<int,3> rgb(const std::string& text){
 if(text.size()!=7||text[0]!='#'||!std::all_of(text.begin()+1,text.end(),[](unsigned char c){return std::isxdigit(c)!=0;}))throw std::invalid_argument("Usa colores #RRGGBB.");
 return {std::stoi(text.substr(1,2),nullptr,16),std::stoi(text.substr(3,2),nullptr,16),std::stoi(text.substr(5,2),nullptr,16)};
}
inline std::string escape(const std::string& text){std::string out;for(char c:text){switch(c){case '&':out+="&amp;";break;case '<':out+="&lt;";break;case '>':out+="&gt;";break;case '"':out+="&quot;";break;case '\'':out+="&#39;";break;default:if(static_cast<unsigned char>(c)>=32||c=='\n'||c=='\t')out+=c;}}return out;}
inline std::string start(int width=1920,int height=1080){return "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\""+std::to_string(width)+"\" height=\""+std::to_string(height)+"\" viewBox=\"0 0 "+std::to_string(width)+" "+std::to_string(height)+"\">\n";}
inline std::string gradient(const std::string&a,const std::string&b){rgb(a);rgb(b);return start()+"<defs><linearGradient id=\"g\" x2=\"1\" y2=\"1\"><stop stop-color=\""+a+"\"/><stop offset=\"1\" stop-color=\""+b+"\"/></linearGradient></defs><path fill=\"url(#g)\" d=\"M0 0h1920v1080H0z\"/></svg>\n";}
inline std::string mosaic(const std::string&a,const std::string&b,unsigned seed){rgb(a);rgb(b);std::mt19937 rng(seed);std::ostringstream out;out<<start()<<"<path fill=\""<<a<<"\" d=\"M0 0h1920v1080H0z\"/>";for(int y=0;y<1080;y+=120)for(int x=0;x<1920;x+=120){const bool flip=(rng()%2)==0;out<<"<path d=\"M"<<x<<' '<<y<<"h120"<<(flip?"l-120 120z":"v120z")<<"\" fill=\""<<b<<"\" opacity=\""<<(.06+(rng()%29)/100.)<<"\"/>";}return out.str()+"</svg>\n";}
inline std::string dots(const std::string&a,const std::string&b){rgb(a);rgb(b);return start()+"<defs><pattern id=\"p\" width=\"40\" height=\"40\" patternUnits=\"userSpaceOnUse\"><circle cx=\"20\" cy=\"20\" r=\"1.5\" fill=\""+b+"\"/></pattern></defs><path fill=\""+a+"\" d=\"M0 0h1920v1080H0z\"/><path fill=\"url(#p)\" d=\"M0 0h1920v1080H0z\"/></svg>\n";}
inline std::string monogram(const std::string&text,const std::string&a,const std::string&b){rgb(a);rgb(b);if(text.empty()||text.size()>32)throw std::invalid_argument("Usa una etiqueta corta (máximo 32 bytes UTF-8). Para iniciales, de 1 a 3 caracteres.");return start(512,512)+"<rect width=\"512\" height=\"512\" rx=\"112\" fill=\""+a+"\"/><text x=\"256\" y=\"320\" text-anchor=\"middle\" font-family=\"system-ui,sans-serif\" font-size=\"160\" font-weight=\"600\" fill=\""+b+"\">"+escape(text)+"</text></svg>\n";}
inline double luminance(const std::string&text){auto c=rgb(text);std::array<double,3> v{};for(int i=0;i<3;i++){double n=c[i]/255.;v[i]=n<=.04045?n/12.92:std::pow((n+.055)/1.055,2.4);}return v[0]*.2126+v[1]*.7152+v[2]*.0722;}
inline double contrast(const std::string&a,const std::string&b){double x=luminance(a),y=luminance(b);return (std::max(x,y)+.05)/(std::min(x,y)+.05);}
inline std::string palette(const std::string&color){auto base=rgb(color);std::ostringstream out;out<<":root {\n";for(int i=0;i<9;i++){double factor=(i-4)/5.;out<<"  --tono-"<<i+1<<": #";for(int c:base){int value=static_cast<int>(std::round(factor<0?c*(1+factor):c+(255-c)*factor));out<<std::uppercase<<std::hex<<std::setw(2)<<std::setfill('0')<<value;}out<<";\n"<<std::dec;}out<<"}\n";return out.str();}
inline std::string spacing(){std::ostringstream out;out<<start(1000,800)<<"<path fill=\"#10152E\" d=\"M0 0h1000v800H0z\"/><text x=\"50\" y=\"65\" font-family=\"sans-serif\" font-size=\"28\" fill=\"#E6EFDA\">ESCALA DE ESPACIADO / BASE 4</text>";int y=120;for(int size:{4,8,12,16,24,32,48,64}){out<<"<text x=\"50\" y=\""<<y+24<<"\" font-family=\"monospace\" font-size=\"20\" fill=\"#E6EFDA\">"<<size<<" px</text><rect x=\"170\" y=\""<<y<<"\" width=\""<<size*10<<"\" height=\"38\" rx=\"6\" fill=\"#56DCC6\"/>";y+=76;}return out.str()+"</svg>\n";}
inline std::string base64(const std::vector<unsigned char>&data){static const char chars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";std::string out;out.reserve((data.size()+2)/3*4);for(std::size_t i=0;i<data.size();i+=3){unsigned value=static_cast<unsigned>(data[i])<<16;bool b=i+1<data.size(),c=i+2<data.size();if(b)value|=static_cast<unsigned>(data[i+1])<<8;if(c)value|=data[i+2];out+=chars[(value>>18)&63];out+=chars[(value>>12)&63];out+=b?chars[(value>>6)&63]:'=';out+=c?chars[value&63]:'=';}return out;}
inline std::string gallery(const fs::path&folder){
 if(!fs::is_directory(folder))throw std::invalid_argument("Indica una carpeta de imágenes.");
 std::vector<fs::path> paths;for(const auto&e:fs::directory_iterator(folder)){if(!e.is_regular_file()||e.is_symlink())continue;auto ext=e.path().extension().string();std::transform(ext.begin(),ext.end(),ext.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});if(ext==".png"||ext==".jpg"||ext==".jpeg"||ext==".gif"||ext==".webp")paths.push_back(e.path());}std::sort(paths.begin(),paths.end());if(paths.empty())throw std::invalid_argument("No hay imágenes compatibles.");if(paths.size()>24)paths.resize(24);
 std::ostringstream out;out<<"<!doctype html><meta charset=\"utf-8\"><title>Galería local</title><style>body{font:16px system-ui;background:#10152e;color:#e6efda;margin:40px}main{display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:24px}figure{margin:0;background:#ffffff08;padding:16px;border-radius:12px}img{width:100%;height:230px;object-fit:contain}figcaption{overflow-wrap:anywhere;margin-top:12px}</style><h1>Galería local</h1><p>Imágenes integradas en este documento.</p><main>";std::uintmax_t total=0;
 for(const auto&p:paths){auto size=fs::file_size(p);total+=size;if(size>8*1024*1024||total>32*1024*1024)throw std::invalid_argument("Máximo 8 MiB por imagen y 32 MiB en total.");std::ifstream input(p,std::ios::binary);if(!input)throw std::runtime_error("No se pudo leer una imagen.");std::vector<unsigned char> data((std::istreambuf_iterator<char>(input)),{});if(input.bad())throw std::runtime_error("Error al leer la imagen.");auto ext=p.extension().string();std::transform(ext.begin(),ext.end(),ext.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});auto mime=ext==".png"?"image/png":ext==".gif"?"image/gif":ext==".webp"?"image/webp":"image/jpeg";auto name=p.filename().u8string();std::string label(name.begin(),name.end());out<<"<figure><img alt=\""<<escape(label)<<"\" src=\"data:"<<mime<<";base64,"<<base64(data)<<"\"><figcaption>"<<escape(label)<<"</figcaption></figure>";}return out.str()+"</main>\n";
}
inline void write_new(const fs::path&path,const std::string&content){
#ifdef _WIN32
 auto* file=_wfopen(path.c_str(),L"wbx");
#else
 auto* file=std::fopen(path.c_str(),"wbx");
#endif
 if(!file)throw std::runtime_error("No se pudo crear el destino: usa un nombre nuevo y una carpeta existente.");
 bool bad=std::fwrite(content.data(),1,content.size(),file)!=content.size();if(std::fclose(file)!=0)bad=true;if(bad){std::error_code error;fs::remove(path,error);throw std::runtime_error("Error de escritura; se intentó retirar la salida parcial.");}
}
}
