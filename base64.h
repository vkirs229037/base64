#pragma once

#include "stdlib.h"
#include "string.h"

#define BASE64_IMPLEMENTATION
// RFC 4648

/// @brief Зашифровать некоторые данные в base64.
/// @param data указатель на данные.
/// @param size длина данных.
/// @return base64 этих данных.
char* base64_encode(char* data, size_t size);
/// @brief Расшифровать данные из base64. 
/// @param data зашифрованные данные
/// @return Расшифрованные данные
char* base64_decode(char* data);
/// @brief Зашифровать файл в base64.
/// @param in_file название файла для шифрования
/// @param out_file название файла, в который записываются зашифрованные данные
void base64_encode_file(const char* in_file, const char* out_file);
/// @brief Расшифровать файл, заданный в base64
/// @param in_file название зашифрованного файла
/// @param out_file название файла, в который записываются расшифрованные данные
void base64_decode_file(const char* in_file, const char* out_file);

#ifdef BASE64_IMPLEMENTATION

const char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char padding = '=';

char* base64_encode(char* data, size_t size) {
    return "";
}

char* base64_decode(char* data) {
    return "";
}

void base64_encode_file(const char* in_file, const char* out_file) {
    return;
}

void base64_decode_file(const char* in_file, const char* out_file) {
    return;
}

#endif // BASE64_IMPLEMENTATION