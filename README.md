# ARC-кеш (+ Belady-кеш) от студентов 2 курса РТ
## Авторы: Лобачев Артем, Себелев Максим

## Зависимости:
У проекта *нет обязательныз зависимостей*, но для удобной работы могут понадобиться:\
[git](https://git-scm.com/ "ссылка на официальный сайт git")\
[make](https://www.gnu.org/software/make/ "ссылка на оффициальный сайт make") \
[cmake](https://cmake.org/ "с\сылка на официальный сайт cmake")

## Объяснение алгоритма ARC(Adaptive replacement cache):
https://miro.com/app/board/uXjVJIsDL80=/?focusWidget=3458764640319423359 \
UPD: Грязные капиталюги не дали сделать pdf/хорошего качество фото 

## Установка:
```bash
git clone https://github.com/Maksim-Sebelev/Cache
```
## Начало работы:
```bash
cd Cache/
mkdir build/
cd build
cmake -S ../Src -DCMAKE_BUILD_TYPE=Release # или =Debug для дебажной версии
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
├── .gitignore
├── README.md
├── project_struct
│   └── project_struct.txt
├── Src
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── cache
│   │   │   ├── arc
│   │   │   │   └── arc_cache.hpp
│   │   │   └── optimal
│   │   │       └── optimal_cache.hpp
│   │   └── test
│   │       ├── check_args.hpp
│   │       ├── read_answer.hpp
│   │       ├── read_test_and_answer.hpp
│   │       ├── read_test.hpp
│   │       └── test_files.hpp
│   └── src
│       ├── cache
│       │   ├── arc
│       │   │   ├── arc_cache.cpp
│       │   │   └── arc_main.cpp
│       │   └── optimal
│       │       ├── optimal_cache.cpp
│       │       └── optimal_main.cpp
│       └── test
│           ├── arc_cache_test.cpp
│           ├── check_args.cpp
│           └── optimal_cache_test.cpp
└── tests
    ├── ans
    │   ├── arc
    │   │   ├── 1.ans
    │   │   ├── 2.ans
    │   │   └── 3.ans
    │   └── optimal
    │       ├── 1.ans
    │       ├── 2.ans
    │       └── 3.ans
    └── dat
        ├── 1.dat
        ├── 2.dat
        └── 3.dat

19 directories, 28 files

```

## Как добавить тесты?
Тестирование происходит сразу для двух алгоритмов - arc и belady. \
Вот так на момент написани README.md выглядит структура папки tests:
```text
tests
    ├── ans
    │   ├── arc       # ответы для arc cache
    │   │   ├── 1.ans
    │   │   ├── 2.ans
    │   │   └── 3.ans
    │   └── optimal   # ответы для optimal cache
    │       ├── 1.ans
    │       ├── 2.ans
    │       └── 3.ans
    └── dat           # общие тестовые данные
        ├── 1.dat
        ├── 2.dat
        └── 3.dat
```
Положим `.` корневой папкой проекта.
Если Вы хотите добавить тесты для `arc cache`, то добавьте в папку `./tests/dat/` файлы с соответсвующим содержимым и названиями `<n>.dat`.
Для каждого вашего теста добавьте в папку `./tests/ans/arc/` соотвествующий файл с тем же названием, что и его тестовый файл `<n>.ans`.\
Аналогично для `optimal cache`.\
**Обратите внимание**, что тестовые данные одинаковы для обоих кешей.\
После просто выполните
```bash
cmake ../Src
make test
```
из папки `./build`. Ваши тесты будут включеные в тестирование и Вы увидете всю необходимую информацию о ваших тестах.


## Почему авторов двое?
В силу ограниченности времени обязанности написания проекта были распределены: Максим писал тестироавание + Cmake, Артем писал алгоритмы кеширования (arc + belady).\
Но это не значит, что каждый делал только свое дело, не разбираясь в деле сокомандника. По итогу каждый получил опыт написания кеша, опыт работы с Cmake и опыт написания тестов (самое сложно было заставить cmake эти тесты делать)

# Проблемы проекта:
Мы прекрасно знаем, что Cmake лучше знает какие флаги для релизной и дебажной версии нужны.
О чем говорил Константин Владимиров в своей лекции по системам сборкам.
Но мы не удержались, чтобы насильно не воткнуть так называемые "флаги Деда" (Дед = Дединский Илья Рудольфович), которые служили нам верой и правдой весь 1 курс. И здесь они себя неплохо проявили, показав множество неточностей и неэффективностей в коде.\
Особое внимание заслужил флаг `-Weffc++`. Он единолично заставлял сиять наши экраны от бесконечных warning

![Warning from -Weffc++](./assets/weffc++.png)

поэтому в целях экономии времени и неполного понимания его претензий к коду он был позорно занесен под комментарий (но не удален).

```cmake
set(DEBUG_FLAGS
	-g -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall
	-Wextra -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported
	-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations
	-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op
	-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith
	-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral
	-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192
	-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel
	-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types
	-Wsuggest-override -Wswitch-default -Wsync-nand -Wundef -Wunreachable-code -Wunused
	-Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing
	-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector
	-pie -fPIE -Werror=vla 				
	# -Weffc++
)
```

