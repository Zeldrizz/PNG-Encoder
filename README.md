# Проект "PNG Encoder"

## Задача

На вход приложения подается изображение в сыром виде (RAW): трехмерный массив размера HxWx3, где для каждого пикселя определены компоненты RGB. Требуется сжать изображение в формат PNG и сохранить его. Проект включает в себя следующие этапы:

- Анализ и понимание структуры формата PNG.
- Реализация фильтра PAETH первичной обработки RAW файла.
- Реализация цветовых фильтров.
- Использование алгоритма сжатия данных DEFLATE, используемых в PNG.
- Обработка и оптимизация цветовой информации.
- Формирование корректного файла PNG.
- Тестирование с различными изображениями для проверки корректности и эффективности сжатия.

## Сколько человек

Проект рассчитан на 1 человека.

## Цели проекта

- Изучить структуру формата PNG и понять принципы его работы.
- Реализовать алгоритм PAETH для первичной фильтрации изображения.
- Реализовать несколько цветовых фильтров для изменения изображений.
- Использовать алгоритм сжатия DEFLATE на языке C/C++ из ZLIB.
- Создать полноценное приложение, способное конвертировать сырые изображения в формат PNG.
- Обеспечить корректную работу приложения на различных тестовых изображениях.
- Получить практический опыт разработки программного обеспечения, взаимодействующего с системными ресурсами и файловой системой.

## Функционал

1. **Загрузка RAW-изображения**
   `ImageLoader::LoadRawImage(const std::string &path, uint64_t width, uint64_t height)` — читает файл `.raw` и заполняет `RawImage::data` длиной `width * height * 3`.

2. **Цветовые фильтры**
   - `NegativeFilter::Apply(std::vector<uint8_t> &rgb_data)` — инверсия значений (255 − v)
   - `GrayscaleFilter::Apply(std::vector<uint8_t> &rgb_data)` — преобразование по формуле Y = 0.299 × R + 0.587 × G + 0.114 × B
   - `PerlinNoiseFilter::Apply(std::vector<uint8_t> &rgb_data, uint64_t width, uint64_t height, float percent)` — шум Перлина с интенсивностью percent (0–100)

3. **PNG-фильтрация**  
   `PNGFilter::Apply(const std::vector<uint8_t> &rgb_data, uint64_t width, uint64_t height)` — возвращает вектор скан-лайнов, где каждая строка начинается с байта фильтра Paeth.

4. **Сжатие**
   `DeflateCompressor::Compress(const std::vector<uint8_t> &data)` — сжимает переданные скан-лайны с помощью ZLIB (режим Z_BEST_COMPRESSION).

5. **Формирование PNG**
   `PNGWriter::WritePNG(const std::string &filename, uint64_t width, uint64_t height, const std::vector<uint8_t> &compressed_data)` — пишет сигнатуру, чанки IHDR, IDAT, IEND и рассчитывает CRC.

6. **Тесты**
   - `test_image_loader.cpp`
   - `test_filter.cpp`
   - `test_png_writer.cpp`
   - `test_color_filters.cpp`  
   Запуск: `ctest --output-on-failure`

7. **Утилиты**
   - `generate_raw_from_png.py` — конвертация PNG -> RAW
   - `micro-benchmark.py` — сравнение скорости конвертации и размера выходного файла с Pillow/OpenCV

## Зависимости

- **C++20** (GCC 10+ или Clang 10+)
- **CMake ≥ 3.14**
- **ZLIB** (dev-пакет)
- **Python 3.6+** (для утилит):
  ```bash
  pip install Pillow
  # опционально для бенчмарка:
  pip install opencv-python
   ```

## Сборка
```bash
git clone <repo_url>
cd PNG-Encoder
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

## Использование
```bash
cd build

# базовая конвертация RAW -> PNG
./png_encoder input.raw output.png width height

# с цветовым фильтром: none | negative | grayscale
./png_encoder input.raw output.png width height negative

# с шумом Перлина (0-100)
./png_encoder input.raw output.png width height perlin 75
```

## Генерация RAW из PNG
```bash
python3 generate_raw_from_png.py examples/png/test5-1280x720.png examples/raw/test5-1280x720.raw
```

## Бенчмарки
```bash
python3 micro-benchmark.py
```

## Источники

- [PNG: Filtering](https://en.wikipedia.org/wiki/PNG#Filtering)
- [Глубокое погружение в PNG (Habr)](https://habr.com/ru/articles/130472/)
- [Подробности о PNG: фильтрация и компрессия (Habr)](https://habr.com/ru/companies/ruvds/articles/787302/)
- [ZLIB: Документация](https://www.zlib.net/manual.html)
- [DEFLATE: Алгоритм сжатия](https://en.wikipedia.org/wiki/Deflate)
- [GoogleTest: Библиотека тестирования](https://github.com/google/googletest.git)