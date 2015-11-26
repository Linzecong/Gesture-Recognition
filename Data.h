#ifndef DATA
#define DATA
#include<vector>
#include<QStringList>

class Data{
public:
    std::vector<std::vector<double> > SetIn;//记录训练用的数据
    std::vector<std::vector<double> > SetOut;//记录训练用数据的目标输出

    QStringList Name;//保存数据所代表的手势
    std::vector<std::vector<double> > vecPatterns;//同SetIn
    int NumPatterns;//数据量
    int VectorSize;//数据大小
    void Init();
public:
    Data(int numStartPatterns,int vectorSize):NumPatterns(numStartPatterns),VectorSize(vectorSize){
        Init();
        CreateTrainingSetFromData();
    }

    bool AddData(std::vector<double> data, QString NewName);//新增学习数据
    void CreateTrainingSetFromData();//生成训练的输入和输出

    std::vector<std::vector<double> > GetInputSet() {return SetIn;}
    std::vector<std::vector<double> > GetOutputSet(){return SetOut;}
};
//定义默认手势的输入向量
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

//初始化默认手势
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


bool Data::AddData(std::vector<double> data, QString NewName){
    Name.append(NewName);
    vecPatterns.push_back(data);
    ++NumPatterns;
    CreateTrainingSetFromData();
    return true;
}

//生成训练用的输入和输出
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

#endif // DATA
