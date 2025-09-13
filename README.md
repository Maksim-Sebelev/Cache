# ARC-кеш от студентов 2 курса РТ
## Авторы: Лобачев Артем, Себелев Максим

## Зависимости:
У проекта **нет обязательных зависимостей**, но для удобной работы могут понадобиться:\
[git](https://git-scm.com/ "ссылка на официальный сайт git")\
[make](https://www.gnu.org/software/make/ "ссылка на оффициальный сайт make") \
[cmake](https://cmake.org/ "с\сылка на официальный сайт cmake")


## Установка:
```bash
git clone https://github.com/Maksim-Sebelev/Cache
```
## Начало работы:
```bash
cd Cache/
mkdir build/
cd build
cmake ../Src
```

## Комплиляция:
```bash
make
```

# Тестирование

## !!!Перед запуском тестов!!!:
```bash
make ready_for_tests # сборка программы для тестирования
```
## запуск тестов:
```bash
make test 
```
или для более подробной информации:
```bash
ctest --verbose
```

# Структура проекта
```txt
.
├── build # рабочая директория (создается Вами)
│   └── <some files created with cmake>
├── README.md
├── .gitignore
├── project_struct
│   └── project_struct.txt
├── Src
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── cache
│   │   │   ├── arc_cache.hpp
│   │   │   └── optimal_cache.hpp
│   │   └── test
│   │       ├── cache_test.hpp
│   │       ├── check_args.hpp
│   │       ├── read_answer.hpp
│   │       ├── read_test_and_answer.hpp
│   │       ├── read_test.hpp
│   │       └── test_files.hpp
│   └── src
│       ├── cache
│       │   ├── arc_cache.cpp
│       │   ├── main.cpp
│       │   └── optimal_cache.cpp
│       └── test
│           ├── cache_test.cpp
│           ├── check_args.cpp
│           ├── read_answer.cpp
│           ├── read_test_and_answer.cpp
│           ├── read_test.cpp
│           └── test_files.cpp
└── tests
    ├── 1.ans
    ├── 1.dat
    ├── 2.ans
    └── 2.dat

10 directories, 24 files
```


## Почему тестирования такое мудренное?
Нам хотелось потренироваться в синтаксисе на нашем первом проекте на С++, чего не позволило бы простое написание кеша и легеньких тестов.

## Почему авторов двое?
В силу ограниченности времени обязанности написания проекта были распределены: Максим писал тестироавание + Cmake, Артем писал алгоритмы кеширования (arc + belady).\
Но это не значит, что каждый делал только свое дело, не разбираясь в деле сокомандника. По итогу каждый получил опыт написания кеша, опыт работы с Cmake и опыт написания тестов (самое сложно было заставить cmake эти тесты делать)

## Понимаем ли мы, что если так дальше париться над каждым проектом, то выгорание наступит через пару недель?
Да.


# Проблемы проекта:
## 1
Нет разделения на дебажную и релизную версии проекта. Причина - отсутствие времени
