#pragma once

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

#define BASE64_IMPLEMENTATION
// RFC 4648

/// @brief Зашифровать некоторые данные в base64.
/// @param data указатель на данные.
/// @param size длина данных.
/// @return base64 этих данных.
char* base64_encode(char* data, size_t size);
/// @brief Расшифровать данные из base64. 
/// @param data зашифрованные данные
/// @param size длина зашифрованных данных
/// @return Расшифрованные данные
char* base64_decode(char* data, size_t size);
/// @brief Зашифровать файл в base64.
/// @param in_file название файла для шифрования
/// @param out_file название файла, в который записываются зашифрованные данные
/// @return true в случае успешной работы, иначе false
bool base64_encode_file(const char* in_file, const char* out_file);
/// @brief Расшифровать файл, заданный в base64
/// @param in_file название зашифрованного файла
/// @param out_file название файла, в который записываются расшифрованные данные
/// @return true в случае успешной работы, иначе false
bool base64_decode_file(const char* in_file, const char* out_file);

#ifdef BASE64_IMPLEMENTATION

const char* TABLE_INTO = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const int TABLE_FROM[256] = {
    [(int)'A'] = 0,
    [(int)'B'] = 1,
    [(int)'C'] = 2,
    [(int)'D'] = 3,
    [(int)'E'] = 4,
    [(int)'F'] = 5,
    [(int)'G'] = 6,
    [(int)'H'] = 7,
    [(int)'I'] = 8,
    [(int)'J'] = 9,
    [(int)'K'] = 10,
    [(int)'L'] = 11,
    [(int)'M'] = 12,
    [(int)'N'] = 13,
    [(int)'O'] = 14,
    [(int)'P'] = 15,
    [(int)'Q'] = 16,
    [(int)'R'] = 17,
    [(int)'S'] = 18,
    [(int)'T'] = 19,
    [(int)'U'] = 20,
    [(int)'V'] = 21,
    [(int)'W'] = 22,
    [(int)'X'] = 23,
    [(int)'Y'] = 24,
    [(int)'Z'] = 25,
    [(int)'a'] = 26,
    [(int)'b'] = 27,
    [(int)'c'] = 28,
    [(int)'d'] = 29,
    [(int)'e'] = 30,
    [(int)'f'] = 31,
    [(int)'g'] = 32,
    [(int)'h'] = 33,
    [(int)'i'] = 34,
    [(int)'j'] = 35,
    [(int)'k'] = 36,
    [(int)'l'] = 37,
    [(int)'m'] = 38,
    [(int)'n'] = 39,
    [(int)'o'] = 40,
    [(int)'p'] = 41,
    [(int)'q'] = 42,
    [(int)'r'] = 43,
    [(int)'s'] = 44,
    [(int)'t'] = 45,
    [(int)'u'] = 46,
    [(int)'v'] = 47,
    [(int)'w'] = 48,
    [(int)'x'] = 49,
    [(int)'y'] = 50,
    [(int)'z'] = 51,
    [(int)'0'] = 52,
    [(int)'1'] = 53,
    [(int)'2'] = 54,
    [(int)'3'] = 55,
    [(int)'4'] = 56,
    [(int)'5'] = 57,
    [(int)'6'] = 58,
    [(int)'7'] = 59,
    [(int)'8'] = 60,
    [(int)'9'] = 61,
    [(int)'+'] = 62,
    [(int)'/'] = 63,
};

const char PADDING = '=';

int concat_chars(char hi, char mid, char lo) {
    return ((int)(hi) << 16) | ((int)(mid) << 8) | lo;
}

char* into_chars(int integer) {
    char* result = malloc(3 * sizeof(char));
    result[0] = integer >> 16 & 0xFF;
    result[1] = integer >> 8 & 0xFF;
    result[2] = integer & 0xFF;
    return result;
}

void dbg(char block[], int i) {
    printf("block %i\n", i);
    printf("chars %c %c %c\n", block[0], block[1], block[2]);
    int block_int = concat_chars(block[0], block[1], block[2]);
    printf("integer ");
    for (int i = 0; i < 24; i++) {
        printf("%i", (block_int >> (24 - i)) & 1);
    } 
    printf("\n");
    int idx1 = (block_int >> 18) & 0x3F;
    int idx2 = (block_int >> 12) & 0x3F;
    int idx3 = (block_int >> 6) & 0x3F;
    int idx4 = block_int & 0x3F;
    printf("idxs %i %i %i %i\n", idx1, idx2, idx3, idx4);
}

void print_bin(int x, int size) {
    for (int i = 0; i < size; i++) {
        printf("%i", (x >> (size - i)) & 1);
    } 
    printf("\n");
}

char* base64_encode(char* data, size_t size) {
    // Число блоков по 3 байта в начальных данных.
    // Не всегда данные можно поделить на такие блоки.
    // В таком случае добавим еще один блок, в котором будет 
    // только 1 или 2 ненулевых байта. 
    size_t n_blocks = (int)(size / 3) + (int)(size % 3 != 0);
    // Копия данных округленного размера
    char* padded_data = (char*)malloc(n_blocks * 3 * sizeof(char));
    memset(padded_data, 0, n_blocks * 3);
    memcpy(padded_data, data, size);
    // Массив для результата
    // + 1 нулевой байт
    char* result = (char*)malloc(n_blocks * 4 * sizeof(char) + 1);
    // Итерация по блокам по 3 байта
    for (int i = 0; i < n_blocks; i++) {
        char block[3] = {0};
        // Копируем в объединение блок 3 байта
        memcpy(block, padded_data + i*3, 3);
        int block_int = concat_chars(block[0], block[1], block[2]);
        // dbg(block, i);
        char c1;
        char c2;
        char c3;
        char c4;
        // Если непоследний блок
        if (i != n_blocks - 1) {
            // Извлекаем из объединения индексы для таблицы
            // 0x3F = 0b111111
            int idx1 = (block_int >> 18) & 0x3F;
            int idx2 = (block_int >> 12) & 0x3F;
            int idx3 = (block_int >> 6) & 0x3F;
            int idx4 = block_int & 0x3F;
            // и по ним получаем 4 буквы base64
            c1 = TABLE_INTO[idx1];
            c2 = TABLE_INTO[idx2];
            c3 = TABLE_INTO[idx3];
            c4 = TABLE_INTO[idx4];
        }
        // Если последний блок
        // Возможно, будет необходимо добавить padding
        else {
            // Если нулевые 2 последних байта
            if (block[1] == 0 && block[2] == 0) {
                // Извлекаем только 2 индекса
                // 0x3F = 0b111111
                int idx1 = (block_int >> 18) & 0x3F;
                int idx2 = (block_int >> 12) & 0x3F;
                // Получим только 2 буквы base64, все остальное - padding 
                c1 = TABLE_INTO[idx1];
                c2 = TABLE_INTO[idx2];
                c3 = PADDING;
                c4 = PADDING;
            }
            // Если нулевой только последний байт
            else if (block[2] == 0) {
                // Извлекаем 3 индекса
                // 0x3F = 0b111111
                int idx1 = (block_int >> 18) & 0x3F;
                int idx2 = (block_int >> 12) & 0x3F;
                int idx3 = (block_int >> 6) & 0x3F;
                // Получим 3 буквы base64, все остальное - padding 
                c1 = TABLE_INTO[idx1];
                c2 = TABLE_INTO[idx2];
                c3 = TABLE_INTO[idx3];
                c4 = PADDING;
            }
            // Все как обычно, все 3 байта ненулевые
            else {
                // Извлекаем из объединения индексы для таблицы
                // 0x3F = 0b111111
                int idx1 = (block_int >> 18) & 0x3F;
                int idx2 = (block_int >> 12) & 0x3F;
                int idx3 = (block_int >> 6) & 0x3F;
                int idx4 = block_int & 0x3F;
                // и по ним получаем 4 буквы base64
                c1 = TABLE_INTO[idx1];
                c2 = TABLE_INTO[idx2];
                c3 = TABLE_INTO[idx3];
                c4 = TABLE_INTO[idx4];
            }
        }
        // Полученные буквы записываются в буфер для результата
        result[4*i + 0] = c1;
        result[4*i + 1] = c2;
        result[4*i + 2] = c3;
        result[4*i + 3] = c4;
    }
    return result;
}

char* base64_decode(char* data, size_t size) {
    // Количество квартетов
    // Длина строки base64 всегда делится на 4
    size_t n_blocks = size / 4;
    // Массив для результата
    // Из каждого квартета получим 3 байта
    char* result = (char*)malloc(n_blocks * 3 * sizeof(char));
    for (int i = 0; i < n_blocks; i++) {
        // Блок из 4 знаков base64
        char block[4];
        memcpy(block, data + 4*i, 4);
        // Число, которое получится после соединения 4-х индексов
        int integer;
        // Байты результата
        char bytes[3];
        // Если последние 2 знака - =
        if (block[2] == '=' && block[3] == '=') {
            int idx1 = TABLE_FROM[(int)block[0]];
            int idx2 = TABLE_FROM[(int)block[1]];
            integer = (idx1 << 18) | (idx2 << 12);
            char* chars = into_chars(integer);
            memcpy(bytes, chars, 1);
            free(chars);
        } 
        // Если только последний знак - =
        else if (block[3] == '=') {
            printf("1 =\n");
            int idx1 = TABLE_FROM[(int)block[0]];
            int idx2 = TABLE_FROM[(int)block[1]];
            int idx3 = TABLE_FROM[(int)block[2]];
            integer = (idx1 << 18) | (idx2 << 12) | (idx3 << 6);
            char* chars = into_chars(integer);
            memcpy(bytes, chars, 2);
            free(chars);
        }
        else {
            int idx1 = TABLE_FROM[(int)block[0]];
            int idx2 = TABLE_FROM[(int)block[1]];
            int idx3 = TABLE_FROM[(int)block[2]];
            int idx4 = TABLE_FROM[(int)block[3]];
            integer = (idx1 << 18) | (idx2 << 12) | (idx3 << 6) | idx4;
            char* chars = into_chars(integer);
            memcpy(bytes, chars, 3);
            free(chars);
        }
        memcpy(result + 3*i, bytes, 3);
        memset(bytes, 0, 3);
    }
    return result;
}

bool base64_encode_file(const char* in_file, const char* out_file) {
    FILE* in_fstream;
    in_fstream = fopen(in_file, "rb");
    if (in_fstream == NULL) {
        printf("Ошибка: файл %s не мог быть открыт\n", in_file);
        return false;
    }

    FILE* out_fstream;
    out_fstream = fopen(out_file, "wb+");
    if (out_fstream == NULL) {
        printf("Ошибка: файл %s не мог быть открыт\n", in_file);
        return false;
    }

    size_t block_size = 512; // байт
    char in_buffer[block_size];
    char* base64_buffer;
    size_t read_bytes;
    while ((read_bytes = fread(in_buffer, 1, block_size, in_fstream)) == block_size) {
        base64_buffer = base64_encode(in_buffer, block_size);
        fwrite(base64_buffer, 1, block_size, out_fstream);
        free(base64_buffer);
        memset(in_buffer, 0, read_bytes);
    }

    if (!feof(in_fstream)) {
        printf("Произошла ошибка при чтении файла %s\n", in_file);
        fclose(in_fstream);
        fclose(out_fstream);
        return false;
    }

    base64_buffer = base64_encode(in_buffer, read_bytes);
    fwrite(base64_buffer, 1, read_bytes, out_fstream);
    free(base64_buffer);

    fclose(in_fstream);
    fclose(out_fstream);

    return true;
}

bool base64_decode_file(const char* in_file, const char* out_file) {
    FILE* in_fstream;
    in_fstream = fopen(in_file, "rb");
    if (in_fstream == NULL) {
        printf("Ошибка: файл %s не мог быть открыт\n", in_file);
        return false;
    }

    FILE* out_fstream;
    out_fstream = fopen(out_file, "wb+");
    if (out_fstream == NULL) {
        printf("Ошибка: файл %s не мог быть открыт\n", in_file);
        return false;
    }

    size_t block_size = 512; // байт
    char in_buffer[block_size];
    char* decoded_buffer;
    size_t read_bytes;
    while ((read_bytes = fread(in_buffer, 1, block_size, in_fstream)) == block_size) {
        decoded_buffer = base64_decode(in_buffer, block_size);
        fwrite(decoded_buffer, 1, block_size, out_fstream);
        free(decoded_buffer);
        memset(in_buffer, 0, read_bytes);
    }

    if (!feof(in_fstream)) {
        printf("Произошла ошибка при чтении файла %s\n", in_file);
        fclose(in_fstream);
        fclose(out_fstream);
        return false;
    }

    decoded_buffer = base64_decode(in_buffer, read_bytes);
    fwrite(decoded_buffer, 1, read_bytes, out_fstream);
    free(decoded_buffer);

    fclose(in_fstream);
    fclose(out_fstream);

    return true;
}

#endif // BASE64_IMPLEMENTATION