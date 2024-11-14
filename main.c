#include "stdio.h"
#define BASE64_IMPLEMENTATION
#include "base64.h"

const char* VERSION = "0.0.3";

typedef enum Command {
    COM_ENCODE = 0x0100,
    COM_DECODE = 0x1000,
    ARG_TEXT = 0x0001,
    ARG_FILE = 0x0010
} Command;

typedef struct Data {
    char* text;
    size_t num;
    Command command[2];
} Data;

void help() {
    printf("Справка\n");
    printf("base64 <режим_работы> <аргумент> [аргументы...]\n");
    printf("Режим работы\n");
    printf("    -e                   Закодировать\n");
    printf("    -d                   Раскодировать\n");
    printf("Аргумент для закодирования");
    printf("    -t <text> <len>      Текст text длиной len");
    printf("    -f <filename>        Файл");
    printf("Аргумент для раскодирования");
    printf("    -t <text>            Текст text (в base64)");
    printf("    -f <filename>        Входной файл");
    printf("Помощь");
    printf("    -h                   Вывести эту справку");
    printf("    -v                   Версия");
}

void parse_argv(int argc, char** argv, Data* data) {
    if (strcmp(argv[1], "-e") == 0) {
        if (argc < 2) {
            printf("Ошибка: не задан аргумент для кодирования\n");
            help();
            exit(1);
        }

        if (strcmp(argv[2], "-t") == 0) {
            if (argc < 5) {
                printf("Ошибка: не задан текст для кодирования или его длина\n");
                help();
                exit(1);
            }
            data->text = argv[3];
            char* p;
            size_t num = strtol(argv[4], &p, 10);
            if (p == argv[4]) {
                printf("Ошибка: %s не является десятичным числом\n", argv[4]);
                help();
                exit(1);
            }
            data->num = num;
            data->command[0] = COM_ENCODE;
            data->command[1] = ARG_TEXT;
        }
        else if (strcmp(argv[2], "-f") == 0) {
            if (argc < 4) {
                printf("Ошибка: не задан файл для кодирования\n");
                help();
                exit(1);
            }
            data->text = argv[3];
            data->num = strlen(argv[3]);
            data->command[0] = COM_ENCODE;
            data->command[1] = ARG_FILE;
        }
        else {
            printf("Ошибка: неизвестный аргумент %s\n", argv[3]);
        }
    } 
    else if (strcmp(argv[1], "-d") == 0) {
        if (argc < 2) {
            printf("Ошибка: не задан аргумент для раскодирования\n");
            help();
            exit(1);
        }
        
        if (strcmp(argv[2], "-t") == 0) {
            if (argc < 4) {
                printf("Ошибка: не задан текст для раскодирования\n");
                help();
                exit(1);
            }
            data->text = argv[3];
            data->num = strlen(argv[3]);
            data->command[0] = COM_DECODE;
            data->command[1] = ARG_TEXT;
        }
        else if (strcmp(argv[2], "-f") == 0) {
            if (argc < 4) {
                printf("Ошибка: не задан файл для раскодирования\n");
                help();
                exit(1);
            }
            data->text = argv[3];
            data->num = strlen(argv[3]);
            data->command[0] = COM_DECODE;
            data->command[1] = ARG_FILE;
        }
        else {
            printf("Ошибка: неизвестный аргумент %s\n", argv[3]);
        }
    } 
    else if (strcmp(argv[1], "-h") == 0) {
        help();
        exit(0);
    }
    else if (strcmp(argv[1], "-v") == 0) {
        printf("%s\n", VERSION);
        exit(0);
    }
    else {
        printf("Ошибка: Не задан режим работы\n");
        help();
        exit(1);
    }
}

int main(int argc, char** argv) {
    Data data = {0};
    parse_argv(argc, argv, &data);
    int command = data.command[0] | data.command[1];
    switch (command) {
        // COM_ENCODE ARG_TEXT
        case 0x0101: {
            char* result = base64_encode(data.text, data.num);
            printf("%s\n", result);
            break;
        }
        // COM_ENCODE ARG_FILE
        case 0x0110: {
            char* out_file_name = malloc((data.num + 8) * sizeof(char));
            memset(out_file_name, 0, data.num + 8);
            memcpy(out_file_name, data.text, data.num);
            memcpy(out_file_name + data.num, ".base64", 7);
            base64_encode_file(data.text, out_file_name);
            break;
        }
        // COM_DECODE ARG_TEXT
        case 0x1001: {
            char* result = base64_decode(data.text);
            printf("%s\n", result);
            break;
        }
        // COM_DECODE ARG_FILE
        case 0x1010: {
            char* out_file_name = malloc((data.num + 9) * sizeof(char));
            memset(out_file_name, 0, data.num + 9);
            memcpy(out_file_name, data.text, data.num);
            memcpy(out_file_name + data.num, ".decoded", 8);
            base64_decode_file(data.text, out_file_name);
            break;
        }
    }
    return 0;
}
