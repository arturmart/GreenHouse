#include "ChartDrawing.h"


#include <cstdio> 
#include <algorithm>




ChartDrawing::ChartDrawing(const std::vector<std::unordered_map<std::string, std::string>>& dataMapIn):dataMap(dataMapIn){

}

std::vector<long long> ChartDrawing::getVectorLongLong(const std::string& logType) {
    std::vector<long long> vector;
    for (auto dataLine : dataMap) {
      // for (auto tp : dataLine) {
      //       std::cout<<tp.first<<":"<<tp.second<<" ";
      //  }
        if (auto search = dataLine.find(logType); search != dataLine.end()) {
            //std::cout<<search->first<<":"<<search->second<<"\n";
            vector.push_back(std::stoll(search->second));
            
        }
        //else std::cout<<"df"<<std::endl;
        
    }
    return vector;  // Return by value instead of by reference
}

std::vector<double> ChartDrawing::getVectorDouble(const std::string& logType){
   std::vector<double> vector;

   for(auto line: dataMap){
       if (auto search = line.find(logType); search != line.end()) {
         //std::cout<<search->first<<":"<<search->second<<"\n";
         vector.push_back(std::stod(search->second));
      }
   }
   return vector;
}
std::vector<std::pair<long long, double>> ChartDrawing::getVectorPair(const std::string& logType1,const std::string& logType2){
   std::vector<std::pair<long long, double>> vector;

   for(auto line: dataMap){
      if (auto search = line.find(logType2); search != line.end()) {
         //std::cout<<std::stoll(line[logType1])<<":"<<std::stod(line[logType2])<<"\n";
         vector.push_back(std::pair<long long, double>(std::stoll(line[logType1]),std::stod(line[logType2])));
         //long long timestamp = std::stod(line[logType]);
      }
   }
   return vector;
}


template <typename T>
std::pair<T, T> ChartDrawing::getSmallAndLarge(const std::vector<T>& vec) {
    if (vec.empty()) {
        throw std::invalid_argument("Vector is empty");
    }
    T min = *std::min_element(vec.begin(), vec.end());
    T max = *std::max_element(vec.begin(), vec.end());
    std::cout<<min<<">"<<max<<"\n";
    return std::pair<T, T>(min, max);
}




bool ChartDrawing::createImageFromJSON(const std::string& title){

   std::vector<long long> timeVector = getVectorLongLong("data");
    if (timeVector.empty()) {
        std::cerr << "Error: Time vector is empty!" << std::endl;
        return false; // Return false if there's no data to plot
    }

    std::pair<long long,long long> X= getSmallAndLarge(timeVector);
    long long minX = X.first;
    long long maxX = X.second;

     std::vector<double> dataVector = getVectorDouble("temp");
    if (dataVector.empty()) {
        std::cerr << "Error: Data vector is empty!" << std::endl;
        return false; // Return false if there's no data to plot
    }
    std::vector<double> dataVector2 = getVectorDouble("tempOut");
    if (dataVector.empty()) {
        std::cerr << "Error: Data vector is empty!" << std::endl;
        return false; // Return false if there's no data to plot
    }

    std::pair<double,double> Y= getSmallAndLarge(dataVector);
    std::pair<double,double> Y2= getSmallAndLarge(dataVector2);
    double minY = std::min(Y.first,Y2.first);
    double maxY = std::max(Y.second,Y2.second);
 
 
    // Размер изображения
    int width = 1280;
    int height = 640;
    int margin = 100;

    // Создаем поверхность для изображения
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(surface);

    // Устанавливаем белый фон
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // Белый цвет
    cairo_paint(cr);


   cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
   DrawGrid(cr);

   cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);  
   DraWChart(cr,getVectorPair("data", "temp"),minX, maxX,minY, maxY);
   
   cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);  
   DraWChart(cr,getVectorPair("data", "tempOut"),minX, maxX,minY, maxY);

   cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); 
   DrawXLabels(cr,minX, maxX,minY, maxY);
   DrawYLabels(cr,minY, maxY);

    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 20);
    cairo_move_to(cr, width / 2 - 100, 30); // Позиция заголовка
    cairo_show_text(cr, title.c_str());

    // Добавляем подпись для оси X
    cairo_move_to(cr, width / 2 - 40, 80); // Позиция X
    cairo_show_text(cr, "time");

    // Добавляем подпись для оси Y
    cairo_save(cr);
    cairo_rotate(cr, -M_PI / 2);  // Поворот текста для оси Y
    cairo_move_to(cr, -height / 2 - 40, 20); // Позиция Y
    cairo_show_text(cr, "data");
    cairo_restore(cr);

    // Сохраняем изображение в файл
    cairo_surface_write_to_png(surface, (title+".png").c_str());

    // Освобождаем ресурсы
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    std::cout << "Image created and saved to " << title << std::endl;

    return true;
}



   void ChartDrawing::DrawGrid(cairo_t* cr,int width,int height,int margin, int Xlines, int Ylines){
    

    //box

    cairo_move_to(cr, 0+margin, 0+margin);
    cairo_line_to(cr, 0+margin, height-margin);
    cairo_line_to(cr, width-margin, height-margin);
    cairo_line_to(cr, width-margin, 0+margin);
    cairo_line_to(cr, 0+margin, 0+margin);
    cairo_stroke(cr); // Рисуем линии


    //lines

    for(int i = 1; i< Xlines;i++){
        cairo_move_to(cr, (i * (width-2*margin))/Xlines+margin, 0+margin);
        cairo_line_to(cr, (i * (width-2*margin))/Xlines+margin, height-margin);
        cairo_stroke(cr); // Рисуем линии
    }

    for(int i = 0; i< Ylines;i++){
        cairo_move_to(cr, 0+margin, (  i * (height-2*margin))/Ylines+margin);
        cairo_line_to(cr, width-margin, ( i * (height-2*margin))/Ylines+margin);
        cairo_stroke(cr); // Рисуем линии

    }
   }

   void ChartDrawing::DraWChart(cairo_t* cr,const std::vector<std::pair<long long, double>>& vec,long long minX,long long maxX,double minY, double maxY,int width,int height,int margin){
      //cairo_move_to(cr, ((vec[0].first - minX) / (maxX - minX) * (width-2*margin))+margin, (height - (vec[0].second - minY) / (maxY - minY) * (height-2*margin))-margin);
      for (size_t i = 0; i < vec.size(); ++i) {
            //std::cout<<((vec[i].first - minX) / (double)(maxX - minX) * (width-2*margin))+margin<<" "<<(height - (vec[i].second - minY) / (maxY - minY) * (height-2*margin))<<"\n";
            cairo_line_to(cr, ((vec[i].first - minX) / (double)(maxX - minX) * (width-2*margin))+margin, (height - (vec[i].second - minY) / (maxY - minY) * (height-2*margin))-margin);
      }
      cairo_stroke(cr); // Рисуем линии
   }

   void ChartDrawing::DrawXLabels(cairo_t* cr,long long minX,long long maxX,double minY, double maxY, int Xlines,int width,int height,int margin){
      for(int i = 0; i< Xlines+1;i++){



        cairo_save(cr);
        

    
        std::string timeString = unixTimestampToTimeString(((maxX - minX) *  i) / 10 + minX);//?

        cairo_text_extents_t extents;
        cairo_text_extents(cr, timeString.c_str(), &extents);

        double textX = (i * (width - 2 * margin)) / Xlines + margin;
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
   }

   void ChartDrawing::DrawYLabels(cairo_t* cr, double minY, double maxY, int Ylines,int width,int height,int margin){
      for(int i = 0; i< Ylines;i++){
        
        //cairo_move_to(cr, ((width-margin)*i)/10, ( i * (height-2*margin))/10+margin);

        char buf[50];
        sprintf(buf, "%.1f", ((maxY - minY)*(10-i))/10 + minY);

        cairo_move_to(cr, margin-40, ( i * (height-2*margin))/10+margin);
        cairo_show_text(cr, buf);
        cairo_stroke(cr); // Рисуем линии

    }
   }

   std::vector<std::unordered_map<std::string,std::string>> dataMap;
