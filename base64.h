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

/// @brief Объединение, представляющее собой блок начальных данных. 
typedef union Block {
    char bytes[3];
    unsigned int integer;
} Block;

#ifdef BASE64_IMPLEMENTATION

const char* TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char PADDING = '=';

char* base64_encode(char* data, size_t size) {
    // Число блоков по 3 байта в начальных данных.
    // Не всегда данные можно поделить на такие блоки.
    // В таком случае добавим еще один блок, в котором будет 
    // только 1 или 2 ненулевых байта. 
    size_t n_blocks = (int)(size / 3) + (int)(size % 3 == 0);
    // Копия данных округленного размера
    char* padded_data = (char*)malloc(n_blocks * 3 * sizeof(char));
    memset(padded_data, 0, n_blocks * 3);
    memcpy(padded_data, data, size);
    // Массив для результата
    // + 1 нулевой байт
    char* result = (char*)malloc(n_blocks * 4 * sizeof(char) + 1);
    // Итерация по блокам по 3 байта
    for (int i = 0; i < n_blocks; i++) {
        Block block = {0};
        // Копируем в объединение блок 3 байта
        memcpy(block.bytes, padded_data + i, 3);
        char c1;
        char c2;
        char c3;
        char c4;
        // Если непоследний блок
        if (i != n_blocks - 1) {
            // Извлекаем из объединения индексы для таблицы
            int idx1 = block.integer & 0b111111;
            int idx2 = (block.integer >> 6) & 0b111111;
            int idx3 = (block.integer >> 12) & 0b111111;
            int idx4 = (block.integer >> 18) & 0b111111;
            // и по ним получаем 4 буквы base64
            c1 = TABLE[idx1];
            c2 = TABLE[idx2];
            c3 = TABLE[idx3];
            c4 = TABLE[idx4];
        }
        // Если последний блок
        // Возможно, будет необходимо добавить padding
        else {
            // Если нулевые 2 последних байта
            if (block.bytes[1] == 0 && block.bytes[2] == 0) {
                // Извлекаем только 2 индекса
                int idx1 = block.integer & 0b111111;
                int idx2 = (block.integer >> 6) & 0b111111;
                // Получим только 2 буквы base64, все остальное - padding 
                c1 = TABLE[idx1];
                c2 = TABLE[idx2];
                c3 = PADDING;
                c4 = PADDING;
            }
            // Если нулевой только последний байт
            else if (block.bytes[2] == 0) {
                // Извлекаем 3 индекса
                int idx1 = block.integer & 0b111111;
                int idx2 = (block.integer >> 6) & 0b111111;
                int idx3 = (block.integer >> 12) & 0b111111;
                // Получим 3 буквы base64, все остальное - padding 
                c1 = TABLE[idx1];
                c2 = TABLE[idx2];
                c3 = TABLE[idx3];
                c4 = PADDING;
            }
            // Все как обычно, все 3 байта ненулевые
            else {
                // Извлекаем из объединения индексы для таблицы
                // 0x3F = 0b111111
                int idx1 = block.integer & 0x3F;
                int idx2 = (block.integer >> 6) & 0x3F;
                int idx3 = (block.integer >> 12) & 0x3F;
                int idx4 = (block.integer >> 18) & 0x3F;
                // и по ним получаем 4 буквы base64
                c1 = TABLE[idx1];
                c2 = TABLE[idx2];
                c3 = TABLE[idx3];
                c4 = TABLE[idx4];
            }
        }
        // Полученные буквы записываются в буфер для результата
        result[i] = c1;
        result[i+1] = c2;
        result[i+2] = c3;
        result[i+3] = c4;
    }
    return result;
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