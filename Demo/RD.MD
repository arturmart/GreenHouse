
------------------------------
![a1](https://github.com/user-attachments/assets/84ef1484-870c-4aa5-b08d-51eba9875b16)
------------------------------
![a2](https://github.com/user-attachments/assets/907340d8-3d34-496b-908d-426da02b4d07)
------------------------------
![a3](https://github.com/user-attachments/assets/4616ceee-0492-42f3-92c7-af153ed2d1dc)
------------------------------

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

```cpp
TreeParsing tp("83,0,1;83,1,1;83,2,1;83,3,1/61204cfc");
tp.addChild("83,0,1;83,1,1;83,2,1;83,3,1");
tp.addChild("61204cfc");

tp.goToChildren(0);
tp.addChild("83,0,1");
tp.addChild("83,1,1");
tp.addChild("83,2,1");
tp.addChild("83,3,1");

tp.goToChildren(0);
tp.addChild("83");
tp.addChild("0");
tp.addChild("1");
tp.goToParent();




tp.goToChildren(1);
tp.addChild("83");
tp.addChild("1");
tp.addChild("1");
tp.goToParent();

tp.goToChildren(2);
tp.addChild("83");
tp.addChild("2");
tp.addChild("1");
tp.goToParent();

tp.goToChildren(3);
tp.addChild("83");
tp.addChild("3");
tp.addChild("1");

tp.goToRoot();

tp.print();
```
```
0xaaaac7d8af10  83,0,1;83,1,1;83,2,1;83,3,1/61204cfc
0xaaaac7d8af90  - 83,0,1;83,1,1;83,2,1;83,3,1
0xaaaac7d8b0a0  - - 83,0,1
0xaaaac7d8b200  - - - 83
0xaaaac7d8b250  - - - 0
0xaaaac7d8b2a0  - - - 1
0xaaaac7d8b0f0  - - 83,1,1
0xaaaac7d8b2f0  - - - 83
0xaaaac7d8b340  - - - 1
0xaaaac7d8b390  - - - 1
0xaaaac7d8b160  - - 83,2,1
0xaaaac7d8b410  - - - 83
0xaaaac7d8b460  - - - 2
0xaaaac7d8b4b0  - - - 1
0xaaaac7d8b1b0  - - 83,3,1
0xaaaac7d8b530  - - - 83
0xaaaac7d8b580  - - - 3
0xaaaac7d8b5d0  - - - 1
0xaaaac7d8b030  - 61204cfc

```
```cpp
tp.getLeafPtr({1})->setValue("qaq");
```

```cpp
std::cout<< tp.getLeafPtr({})       <<" "<<tp.getLeaf({})<<std::endl;
std::cout<< tp.getLeafPtr({0})       <<" "<<tp.getLeaf({0})<<std::endl;
std::cout<< tp.getLeafPtr({1})       <<" "<<tp.getLeaf({1})<<std::endl;
std::cout<< tp.getLeafPtr({0,0})       <<" "<<tp.getLeaf({0,0})<<std::endl;
std::cout<< tp.getLeafPtr({0,0,0})       <<" "<<tp.getLeaf({0,0,0})<<std::endl;
```

```cpp

//TreeParsing local data has  been public
std::cout<<"root "<< tp.root <<" "<< tp.root <<std::endl;
std::cout<<"cur  "<< tp.cursor <<" "<< tp.cursor <<std::endl;
tp.goToChildren(0);
std::cout<<"curC "<< tp.cursor <<" "<< tp.cursor <<std::endl;

```


```cpp
const char delimiters[] = {'/',';',','};

int main(){

   DataParsing parser;

   std::string str = "83,0,1;83,1,1;83,2,1;83,3,1/61204cfc";
   

   TreeParsing tp = parser.parse(str, delimiters ,3);
   tp.print();
   std::cout<<std::endl;


}
```

```cpp
   std::cout<<DC.combinePair(DC.combineListOfLists(strs,',',';'), "crc",'/') << std::endl;
   std::cout<<DC.combinePair(DC.combineListOfLists({strs1,strs2,strs3,strs4},',',';'), "crc",'/') << std::endl;
   std::cout<<DC.combinePair(DC.combineList({DC.combineList({"a","b","c"},','),
                                             DC.combineList({"a","b","c"},','),
                                             DC.combineList({"a","b","c"},','),
                                             DC.combineList({"a","b","c"},',')},';'), "crc",'/') << std::endl;
```
