

```cpp
  DataParsing parser;

   parser.setData("83,0,1;83,1,1;83,2,1;83,3,1/61204cfc");
   parser.parseLevel(DELIMITER1);
   parser.parseLevel(DELIMITER2);
   parser.parseLevel(DELIMITER3);
   parser.calculateLevelsPartCounts();
```


``` cpp
    std::cout<<std::endl;
    parser.printVectorPair();
```

```cpp
   std::cout<<parser.levelsPartCounts.size()<<std::endl;
   for(int i = 0; i < parser.levelsPartCounts.size();i++){
      
            std::cout << " + " << parser.levelsPartCounts[i]<<std::endl;
   }
   std::cout<<std::endl;
```

```cpp
   std::vector<std::string> strs2 (parser.getParsedList());
   
   for(std::string str : strs2){
      std::cout<<str<<std::endl;
   }

   std::cout<<std::endl;
```

```cpp
   std::vector<std::vector<std::string>> dt = parser.getParsedListLayared();
   for(int i = 0; i<3;i++){
      for(int j = 0; j < parser.levelsPartCounts[i];j++)
            std::cout<<dt[i][j]<<std::endl;
   }

```


```cpp
std::cout<<"level1 count: "<< parser.levelsPartCounts[0]<< std::endl;
   for(int i =0;i<parser.levelsPartCounts[0];i++){
      std::cout<< parser.getParsedPartLayered(i,0)<< std::endl;

   }
   std::cout<<"level2 count: "<< parser.levelsPartCounts[1]<< std::endl;
   for(int i =0;i<parser.levelsPartCounts[1];i++){
      std::cout<< parser.getParsedPartLayered(i,1)<< std::endl;

   }
   std::cout<<"level3 count: "<< parser.levelsPartCounts[2]<< std::endl;
   for(int i =0;i<parser.levelsPartCounts[2];i++){
      std::cout<< parser.getParsedPartLayered(i,2)<< std::endl;

   }
   std::cout<< std::endl;
```
