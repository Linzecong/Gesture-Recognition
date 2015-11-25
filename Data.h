#ifndef DATA
#define DATA
#include<vector>
#include<QStringList>

class Data{
public:
    std::vector<std::vector<double> > SetIn;
    std::vector<std::vector<double> > SetOut;
    QStringList Name;
    std::vector<std::vector<double> > vecPatterns;
    int NumPatterns;
    int VectorSize;
    void Init();
public:
    Data(int numStartPatterns,int vectorSize):NumPatterns(numStartPatterns),VectorSize(vectorSize){
        Init();
        CreateTrainingSetFromData();
    }
    QString PatternName(int val);

    bool AddData(std::vector<double> data, QString NewName);
    void CreateTrainingSetFromData();

    std::vector<std::vector<double> > GetInputSet() {return SetIn;}
    std::vector<std::vector<double> > GetOutputSet(){return SetOut;}
};
const double InputVectors[NUM_PATTERNS][NUM_VECTORS*2]=
{
    1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0,

    -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0,

    0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1, 0,1,

    0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1, 0,-1,

    1,0, 1,0, 1,0, 0,1, 0,1, 0,1, -1,0, -1,0, -1,0, 0,-1, 0,-1, 0,-1,

    -1,0, -1,0, -1,0, 0,1, 0,1, 0,1, 1,0, 1,0, 1,0, 0,-1, 0,-1, 0,-1,

    1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, 1,0, -0.45,0.9, -0.9, 0.45, -0.9,0.45,

    -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, -1,0, 0.45,0.9, 0.9, 0.45, 0.9,0.45,

    -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7,
    -0.7,0.7, -0.7,0.7, -0.7,0.7, -0.7,0.7,

    0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7, 0.7,0.7,
    0.7,0.7, 0.7,0.7, 0.7,0.7,

    1,0, 1,0, 1,0, 1,0, -0.72,0.69,-0.7,0.72,0.59,0.81, 1,0, 1,0, 1,0, 1,0, 1,0
};

#endif // DATA

void Data::Init(){
    for(int ptn=0;ptn<NumPatterns;++ptn){
        std::vector<double> temp;
        for(int v=0;v<VectorSize*2; ++v)
            temp.push_back(InputVectors[ptn][v]);
        vecPatterns.push_back(temp);
    }
    Name.append("向右");
    Name.append("向左");
    Name.append("向下");
    Name.append("向上");
    Name.append("顺时针");
    Name.append("逆时针");
    Name.append("右下拐");
    Name.append("左下拐");
    Name.append("西南");
    Name.append("东南");
    Name.append("Z");
}

QString Data::PatternName(int val){
    if(Name.size()>0){
        return Name[val];
    }
    else{
        return "";
    }
}

bool Data::AddData(std::vector<double> data, QString NewName){
    Name.append(NewName);
    vecPatterns.push_back(data);
    ++NumPatterns;
    CreateTrainingSetFromData();
    return true;
}

void Data::CreateTrainingSetFromData(){
    SetIn.clear();
    SetOut.clear();
    for (int ptn=0; ptn<NumPatterns; ++ptn){
        SetIn.push_back(vecPatterns[ptn]);
        std::vector<double> outputs(NumPatterns,0);
        outputs[ptn] = 1;
        SetOut.push_back(outputs);
    }
}
