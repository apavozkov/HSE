#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int main() {
  DIR *dir; // Определяем указатели на структуры для сбора инфы
  struct dirent *entry;
  struct stat fileStat;

  dir = opendir("."); // Открываем текущую директорию
  if (dir == NULL) {
    printf("Не удалось открыть директорию"); // Если не удалось открыть, то выбить с ошибкой
    return 1;
  }

  // Делаем счётчики
  int regular_count = 0;
  int dir_count = 0;
  int chardevice_count = 0;
  int blockdevice_count = 0;
  int namedpipe_count = 0;
  int socket_count = 0;
  int symlink_count = 0;
  int unknown_count = 0;
  
  while ((entry = readdir(dir)) != NULL) { // Перебираем все записи в директории, пока они есть
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { // Игнорируем текущую и родительскую директории
      continue;
    }

    if (stat(entry->d_name, &fileStat) < 0) { // Получаем информацию об указанной записи
      printf("Не удалось получить информацию о файле"); 
      continue;
    }

    // Классифицируем содержимое
    if (S_ISREG(fileStat.st_mode)) { // Проверяем, является ли обычным файлом
      regular_count++;
    } else if (S_ISDIR(fileStat.st_mode)) { // Проверяем, является ли директорией
      dir_count++;
    } else if (S_ISCHR(fileStat.st_mode)) { // Проверяем, является ли символьным файлом устройства
      chardevice_count++;
    } else if (S_ISBLK(fileStat.st_mode)) { // Проверяем, является ли блочным файлом устройства
      blockdevice_count++;
    } else if (S_ISFIFO(fileStat.st_mode)) { // Проверяем, является ли именованным каналом
      namedpipe_count++;
    } else if (S_ISSOCK(fileStat.st_mode)) { // Проверяем, является ли сокетом
      socket_count++;
    } else if (S_ISLNK(fileStat.st_mode)) { // Проверяем, является ли символической ссылкой
      symlink_count++;
    } else { // Записываем, если не удалось определить тип
      unknown_count++;
    }
  }

  closedir(dir); 

  // Выводим результат
if (regular_count > 0) {
	printf("Обычные файлы: %d\n", regular_count);
}
if (dir_count > 0) {
	printf("Директории: %d\n", dir_count);
}
if (chardevice_count > 0) {
	printf("Символьные устройства: %d\n", chardevice_count);
}
if (blockdevice_count > 0) {
	printf("Блочные устройства: %d\n", blockdevice_count);
}
if (namedpipe_count > 0) {
	printf("Именованные каналы: %d\n", namedpipe_count);
}
if (socket_count > 0) {
	printf("Сокеты: %d\n", socket_count);
}
if (symlink_count > 0) {
	printf("Символические ссылки: %d\n", symlink_count);
}
if (unknown_count > 0) {
	printf("Неопознанные: %d\n", unknown_count);
}

  return 0;
}
