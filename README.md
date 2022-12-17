# Siv4: Visualisation of  2D wave propagation
Требования к программному обеспечению
Операционная система: Linux или Unix-подобная система
Используется библиотека SFML (version 2.5.1)
Стандарт C++ 17

Управление
Для запуска нужно собрать проект с помощью CMake
Нажатие любой кнопки мыши -- небольшое возмущение около курсора (для правильной работы необходимо, чтобы окно не было развернуто)
Space -- пауза
's' -- остановка источников волн

Описание проекта
Целью проекта является визуализация распространения волн на двумерной поверхности. В проекте реализованы два типа стен: отражающие и поглощающие. Также есть возможность добавлять источники создающие гармоническое возмущение или периодечские импульсы.