#ifndef NORMALIZADOR_TEXTO_H
#define NORMALIZADOR_TEXTO_H

#include <string>
#include <map>
#include <cctype> // para tolower

// Elimina acentos comunes y convierte caracteres especiales
inline std::string quitar_acentos(const std::string& texto) {
    static const std::map<char, char> mapa_acentos = {
        {'á','a'}, {'é','e'}, {'í','i'}, {'ó','o'}, {'ú','u'},
        {'Á','a'}, {'É','e'}, {'Í','i'}, {'Ó','o'}, {'Ú','u'},
        {'ñ','n'}, {'Ñ','n'}
    };

    std::string resultado;
    for (char c : texto) {
        auto it = mapa_acentos.find(c);
        if (it != mapa_acentos.end()) {
            resultado += it->second;
        } else {
            resultado += c;
        }
    }
    return resultado;
}

// Convierte todos los caracteres a minúsculas usando tolower
inline std::string a_minusculas(const std::string& texto) {
    std::string resultado;
    for (char c : texto) {
        resultado += std::tolower(static_cast<unsigned char>(c));
    }
    return resultado;
}

// Aplica ambas funciones: acentos + minúsculas
inline std::string normalizar_texto(const std::string& texto) {
    return a_minusculas(quitar_acentos(texto));
}

#endif // NORMALIZADOR_TEXTO_H


