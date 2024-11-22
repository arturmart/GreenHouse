#include "jsonManager.h"
#include "TimeManager.h"

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>



//тест



// Упростим доступ к пространству имен библиотеки

jsonManager::jsonManager(const std::string& filename): filename(filename){}

// Пример функции чтения JSON из файла

json jsonManager::read_json_from_file(const std::string &otherfilename) {
    std::ifstream file(otherfilename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + otherfilename);
        return NULL;
    }
    json j;
    file >> j;
    return j;
}

json jsonManager::read_json_from_file() {
    return read_json_from_file(filename);
}

    // Пример функции записи JSON в файл
void jsonManager::write_json_to_file(const json& j) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }
    file << j.dump(4); // Запись с отступом в 4 пробела
}

bool jsonManager::appendToJsonArray( const nlohmann::json& new_element) {
    // Открываем файл в режиме чтения и записи
    std::ifstream input_file(filename);
    nlohmann::json json_array;

    // Читаем файл
    if (input_file.is_open()) {
        try {
            input_file >> json_array;

            // Если структура неправильная, исправляем её
            if (!json_array.is_array()) {
                std::cerr << "Ошибка: JSON не является массивом. Исправляю..." << std::endl;
                json_array = nlohmann::json::array();
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка парсинга: " << e.what() << ". Создаю новый массив..." << std::endl;
            json_array = nlohmann::json::array();
        }
    } else {
        std::cerr << "Файл не найден. Создаю новый массив..." << std::endl;
        json_array = nlohmann::json::array();
    }
    input_file.close();

    // Добавляем новый элемент
    json_array.push_back(new_element);

    // Записываем обратно в файл
    std::ofstream output_file(filename);
    if (output_file.is_open()) {
        output_file << json_array.dump(4); // Запись с форматированием
    } else {
        std::cerr << "Ошибка записи в файл!" << std::endl;
        return false;
    }
    output_file.close();
    return true;
}

void jsonManager::changeName(const std::string& newfilename){
        filename = newfilename;
}

bool jsonManager::createImageFromJSON(
    const std::string& jsonData,
    const std::string& outputFile,
    const std::string& title = "Chart",
    const std::string& xlabel = "X",
    const std::string& ylabel = "Y") {

    // Парсим JSON данные с использованием nlohmann::json
    json root=read_json_from_file(jsonData);

    // Извлекаем данные точек (x, y)
     std::vector<double> x, y;

    // Преобразуем данные времени в секунды и температуры
    for (const auto& point : root) {
        // Преобразование времени из строки в число (timestamp)
        long long timestamp = std::stoll(point[xlabel].get<std::string>());
        x.push_back(static_cast<double>(timestamp));  // Сохраняем временные метки как X
        y.push_back(std::stod(point[ylabel].get<std::string>()));  // Сохраняем температуру как Y
    }

    // Печатаем отладочную информацию о данных
    /*
    std::cout << "Extracted data:" << std::endl;
    for (size_t i = 0; i < x.size(); ++i) {
        std::cout << "Timestamp: " << x[i] << ", Temp: " << y[i] << std::endl;
    }
    */

    // Определяем диапазоны для масштабирования
    double minX = *std::min_element(x.begin(), x.end());
    double maxX = *std::max_element(x.begin(), x.end());
    double minY = *std::min_element(y.begin(), y.end());
    double maxY = *std::max_element(y.begin(), y.end());

    // Размер изображения
    int width = 1280;
    int height = 640;

    // Создаем поверхность для изображения
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(surface);

    // Устанавливаем белый фон
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // Белый цвет
    cairo_paint(cr);

    //resuem setku------------------------------------

    cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);  // Черный цвет
    int margin = 100;

    //box

    cairo_move_to(cr, 0+margin, 0+margin);
    cairo_line_to(cr, 0+margin, height-margin);
    cairo_line_to(cr, width-margin, height-margin);
    cairo_line_to(cr, width-margin, 0+margin);
    cairo_line_to(cr, 0+margin, 0+margin);
    cairo_stroke(cr); // Рисуем линии


    //lines

    for(int i = 1; i< 20;i++){
        cairo_move_to(cr, (i * (width-2*margin))/20+margin, 0+margin);
        cairo_line_to(cr, (i * (width-2*margin))/20+margin, height-margin);
        cairo_stroke(cr); // Рисуем линии
    }

    for(int i = 0; i< 10;i++){
        cairo_move_to(cr, 0+margin, (  i * (height-2*margin))/10+margin);
        cairo_line_to(cr, width-margin, ( i * (height-2*margin))/10+margin);
        cairo_stroke(cr); // Рисуем линии

    }


    // Рисуем график-----------------------------------------------------------------------------------
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);  // Черный цвет

    

    // Начальная точка графика
    cairo_move_to(cr, ((x[0] - minX) / (maxX - minX) * (width-2*margin))+margin, (height - (y[0] - minY) / (maxY - minY) * (height-2*margin))-margin);

    // Рисуем линии
    for (size_t i = 1; i < x.size(); ++i) {
        cairo_line_to(cr, ((x[i] - minX) / (maxX - minX) * (width-2*margin))+margin, (height - (y[i] - minY) / (maxY - minY) * (height-2*margin))-margin);
    }
    cairo_stroke(cr); // Рисуем линии

    cairo_set_source_rgb(cr, 0, 0, 0);

    // Добавляем заголовок-----------------------------------------------------------------------

    cairo_set_source_rgb(cr, 0, 0, 0);

    //for box

    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 20);


   for(int i = 0; i< 21;i++){



        cairo_save(cr);
        

    
        std::string timeString = unixTimestampToTimeString(((maxX - minX) *  i) / 10 + minX);

        cairo_text_extents_t extents;
        cairo_text_extents(cr, timeString.c_str(), &extents);

        double textX = (i * (width - 2 * margin)) / 20 + margin;
        double textY = height - margin+10;

        cairo_translate(cr,textX, textY);
        cairo_rotate(cr, M_PI / 4);
        //cairo_move_to(cr, textX - extents.width / 2, textY);
        cairo_move_to(cr, 0, 0); 

        cairo_show_text(cr, timeString.c_str());

        // Отрисовываем текст
        cairo_stroke(cr);
        cairo_restore(cr);
        
    }
   

    
    
    
    
    for(int i = 0; i< 11;i++){
        
        //cairo_move_to(cr, ((width-margin)*i)/10, ( i * (height-2*margin))/10+margin);

        char buf[50];
        sprintf(buf, "%.1f", ((maxY - minY)*(10-i))/10 + minY);

        cairo_move_to(cr, margin-40, ( i * (height-2*margin))/10+margin);
        cairo_show_text(cr, buf);
        cairo_stroke(cr); // Рисуем линии

    }

    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 20);
    cairo_move_to(cr, width / 2 - 100, 30); // Позиция заголовка
    cairo_show_text(cr, title.c_str());

    // Добавляем подпись для оси X
    cairo_move_to(cr, width / 2 - 40, 80); // Позиция X
    cairo_show_text(cr, xlabel.c_str());

    // Добавляем подпись для оси Y
    cairo_save(cr);
    cairo_rotate(cr, -M_PI / 2);  // Поворот текста для оси Y
    cairo_move_to(cr, -height / 2 - 40, 20); // Позиция Y
    cairo_show_text(cr, ylabel.c_str());
    cairo_restore(cr);

    // Сохраняем изображение в файл
    cairo_surface_write_to_png(surface, outputFile.c_str());

    // Освобождаем ресурсы
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    std::cout << "Image created and saved to " << outputFile << std::endl;

    return true;
}




/*
int main() {

    jsonManager LogJson("example.json");

    try {
        // Создаем JSON объект
        json j = {
            {"name", "Иван"},
            {"age", 30},
            {"languages", {"C++", "Python", "JavaScript"}},
            {"is_developer", true},
            {"arr", {1,2,3,4,5,6,7,8,9,10}},
            {"mapArr", {{"a",1},{"b",2}}}
        };

        // Сохраняем JSON в файл

        LogJson.write_json_to_file(j);

        std::cout << "JSON успешно записан в файл: " << std::endl;

        // Читаем JSON из файла
        json loaded_json = LogJson.read_json_from_file();
        std::cout << "Прочитанный JSON:\n" << loaded_json.dump(4) << std::endl;

        // Доступ к элементам JSON
        std::string name = loaded_json["name"];
        int age = loaded_json["age"];
        bool is_developer = loaded_json["is_developer"];
        std::cout << "Имя: " << name << "\nВозраст: " << age
                  << "\nРазработчик: " << (is_developer ? "Да" : "Нет") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}

*/

