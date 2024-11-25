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
std::vector<std::pair<long long, double>> ChartDrawing::getVectorPairDouble(const std::string& logType1,const std::string& logType2){
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
std::vector<std::pair<long long, bool>> ChartDrawing::getVectorPairBool(const std::string& logType1,const std::string& logType2){
   std::vector<std::pair<long long, bool>> vector;
   std::string lowerStr;
   bool boolFromStr;

   for(auto line: dataMap){
      if (auto search = line.find(logType2); search != line.end()) {
         //std::cout<<std::stoll(line[logType1])<<":"<<line[logType2]<<"\n";
         lowerStr = line[logType2];
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

            if (lowerStr == "1" || lowerStr == "true"|| lowerStr == "on"|| lowerStr == "high") {
                boolFromStr = true;
                //std::cout<<"true"<<std::endl;
            } else if (lowerStr == "0" || lowerStr == "false"|| lowerStr == "off"|| lowerStr == "low") {
                boolFromStr = false;
                //std::cout<<"false"<<std::endl;
            } else {
                throw std::invalid_argument("Invalid boolean string: " + line[logType2]);
            }
         vector.push_back(std::pair<long long, bool>(std::stoll(line[logType1]),boolFromStr));
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




bool ChartDrawing::createImageFromJSON(const std::string& title, const std::string& time, const std::vector<std::string> charts){

   std::vector<long long> timeVector = getVectorLongLong(time);
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
    int marginChartTop = 200, marginChartBottom = 80, marginChartLeft = 100, marginChartRight = 60;
    int XlinesChart = 20,YlinesChart = 10;

    // Создаем поверхность для изображения
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(surface);

    // Устанавливаем белый фон
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // Белый цвет
    cairo_paint(cr);

   cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
   DrawGrid(cr,width,height,marginChartTop, marginChartBottom, marginChartLeft,marginChartRight,XlinesChart,YlinesChart);
   
   cairo_set_source_rgb(cr, 0, 0.7, 0);
   DraWBoolChart(cr, getVectorPairBool(time, "R1"),minX,maxX,8,0,width,height,40, (height-marginChartTop-marginChartBottom)+marginChartBottom+10, marginChartLeft);
   cairo_set_source_rgb(cr, 0.1, 0.3, 0.8);
   DraWBoolChart(cr, getVectorPairBool(time, "R2"),minX,maxX,8,1,width,height,40, (height-marginChartTop-marginChartBottom)+marginChartBottom+10, marginChartLeft);
   
   cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
   DrawGrid(cr,width,height,40, (height-marginChartTop-marginChartBottom)+marginChartBottom+10, marginChartLeft,marginChartRight,XlinesChart,8);

   cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);  
   DraWChart(cr,getVectorPairDouble(time, "temp"),minX, maxX,minY, maxY,width,height,marginChartTop, marginChartBottom, marginChartLeft,marginChartRight);
   
   cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);  
   DraWChart(cr,getVectorPairDouble(time, "tempOut"),minX, maxX,minY, maxY,width,height,marginChartTop, marginChartBottom, marginChartLeft,marginChartRight);

   cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); 

  

    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size(cr, 16);
   DrawChartXLabels(cr,minX, maxX, minY, maxY,XlinesChart, width ,height, marginChartBottom, marginChartLeft,marginChartRight);
   DrawChartYLabels(cr,minY, maxY,YlinesChart, width,height,marginChartTop, marginChartBottom, marginChartLeft);
   DrawYLabels(cr,{"R1","R2","R3","R4","R5","R6","R7","R8"},8, width,height,40, (height-marginChartTop-marginChartBottom)+marginChartBottom+10, marginChartLeft);
   
    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 20);
    cairo_move_to(cr, width / 2 - 100, 30); // Позиция заголовка
    cairo_show_text(cr, title.c_str());
    

    // Добавляем подпись для оси X
    cairo_move_to(cr, 40, height- marginChartBottom+75); // Позиция X
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

   void ChartDrawing::DrawGrid(
    cairo_t* cr,int width,int height,
    int marginTop,int marginBottom,int marginLeft,int marginRight,
    int Xlines, int Ylines){
    

    //box

    cairo_move_to(cr, 0+marginLeft, 0+marginTop);
    cairo_line_to(cr, 0+marginLeft, height-marginBottom);
    cairo_line_to(cr, width-marginRight, height-marginBottom);
    cairo_line_to(cr, width-marginRight, 0+marginTop);
    cairo_line_to(cr, 0+marginLeft, 0+marginTop);
    cairo_stroke(cr); // Рисуем линии


    //lines

    for(int i = 1; i< Xlines;i++){
        cairo_move_to(cr, (i * (width-(marginRight+marginLeft)))/Xlines+marginLeft, 0+marginTop);
        cairo_line_to(cr, (i * (width-(marginRight+marginLeft)))/Xlines+marginLeft, height-marginBottom);
        cairo_stroke(cr); // Рисуем линии
    }

    for(int i = 0; i< Ylines;i++){
        cairo_move_to(cr, 0+marginLeft, (  i * (height-(marginTop+marginBottom)))/Ylines+marginTop);
        cairo_line_to(cr, width-marginRight, ( i * (height-(marginTop+marginBottom)))/Ylines+marginTop);
        cairo_stroke(cr); // Рисуем линии

    }
   }

   void ChartDrawing::DraWChart(
        cairo_t* cr, const std::vector<std::pair<long long, double>>& vec,
        long long minX, long long maxX, double minY, double maxY,
        int width, int height,
        int marginTop,int marginBottom,int marginLeft,int marginRight)
    {
      //cairo_move_to(cr, ((vec[0].first - minX) / (maxX - minX) * (width-2*margin))+margin, (height - (vec[0].second - minY) / (maxY - minY) * (height-2*margin))-margin);
      for (size_t i = 0; i < vec.size(); ++i) {
            //std::cout<<((vec[i].first - minX) / (double)(maxX - minX) * (width-2*margin))+margin<<" "<<(height - (vec[i].second - minY) / (maxY - minY) * (height-2*margin))<<"\n";
            cairo_line_to(
                cr, 
                ((vec[i].first - minX) / (double)(maxX - minX) * (width-(marginRight+marginLeft)))+marginLeft, 
                (height - (vec[i].second - minY) / (maxY - minY) * (height-(marginTop+marginBottom)))-marginBottom);
      }
      cairo_stroke(cr); // Рисуем линии
   }

    void ChartDrawing::DraWBoolChart(
        cairo_t* cr, const std::vector<std::pair<long long, bool>>& vec,
        long long minX, long long maxX,
        int Ylines, int Yid,
        int width, int height,
        int marginTop,int marginBottom,int marginLeft,int marginRight){

        for (size_t i = 0; i < vec.size(); ++i) {
            //std::cout<<vec[i].first<<" "<<vec[i].second<<std::endl;
            long long firstPoint;
            long long secondPoint;
            if(vec[i].second == true){
                    firstPoint=vec[i].first; 
            }
            else continue;

            i++;
            while(vec[i].second == true && i < vec.size()-1){
                i++;
            }
            secondPoint=vec[i].first;

            double x1 = ((firstPoint - minX) / (double)(maxX - minX) * (width-(marginRight+marginLeft)))+marginLeft;
            double x2 = ((secondPoint - minX) / (double)(maxX - minX) * (width-(marginRight+marginLeft)))+marginLeft;

            double y1 = ((  Yid * (height-(marginTop+marginBottom)))/Ylines+marginTop);
            double y2 = ((  (Yid+1) * (height-(marginTop+marginBottom)))/Ylines+marginTop);
            

            cairo_rectangle(cr, x1, y1, x2-x1, y2-y1); // Координаты и размеры (x, y, ширина, высота)
            cairo_fill(cr); // Заливаем прямоугольник
            cairo_stroke(cr);
             
               
                

        }

        }
   void ChartDrawing::DrawChartXLabels(
        cairo_t* cr,
        long long minX, long long maxX, double minY, double maxY,
        int Xlines,
        int width, int height,
        int marginBottom,int marginLeft, int marginRight){
      for(int i = 0; i< Xlines+1;i++){



        cairo_save(cr);
        

    
        std::string timeString = unixTimestampToTimeString(((maxX - minX) *  i) / 10 + minX);//?

        cairo_text_extents_t extents;
        cairo_text_extents(cr, timeString.c_str(), &extents);

        double textX = (i * (width - (marginRight+marginLeft))) / Xlines + marginLeft;
        double textY = height - marginBottom+10;

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

   void ChartDrawing::DrawChartYLabels(
    cairo_t* cr,
    double minY, double maxY,
    int Ylines,
    int width, int height, 
    int marginTop,int marginBottom,int marginLeft){

      for(int i = 0; i< Ylines+1;i++){
        
        //cairo_move_to(cr, ((width-margin)*i)/10, ( i * (height-2*margin))/10+margin);

        char buf[50];
        sprintf(buf, "%.1f", ((maxY - minY)*(Ylines-i))/Ylines + minY);

        cairo_move_to(cr, marginLeft-40, ( i * (height-(marginTop+marginBottom)))/Ylines+marginTop+10);
        cairo_show_text(cr, buf);
        cairo_stroke(cr); // Рисуем линии

    }
   }

   void ChartDrawing::DrawYLabels(
    cairo_t* cr,
    const std::vector<std::string>& strs,
    int Ylines,
    int width, int height, 
    int marginTop,int marginBottom,int marginLeft){

      for(int i = 0; i< Ylines;i++){
        
        //cairo_move_to(cr, ((width-margin)*i)/10, ( i * (height-2*margin))/10+margin);

        cairo_move_to(cr, marginLeft-40, ( i * (height-(marginTop+marginBottom)))/Ylines+marginTop+10);
        cairo_show_text(cr, strs[i].c_str());
        cairo_stroke(cr); // Рисуем линии

    }
   }

   

   std::vector<std::unordered_map<std::string,std::string>> dataMap;
