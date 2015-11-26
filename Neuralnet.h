#ifndef NEURALNET
#define NEURALNET
#include<vector>
#include<math.h>
#include"Data.h"

double RandomClamped(){
    return -1+2*(rand()/((double)RAND_MAX+1));
}

struct Neuron{
    int NumInputs;//神经元的输入量
    std::vector<double> vecWeight;//权重
    double Activation;//这里是根据输入，通过某个线性函数确定，相当于神经元的输出
    double Error;//误差值
    Neuron(int inputs){
        NumInputs=inputs+1;
        Activation=0;
        Error=0;
        for(int i=0;i<NumInputs+1;i++)
            vecWeight.push_back(RandomClamped());//初始化权重
    }
};

struct NeuronLayer{
    int	NumNeurons;//每层拥有的神经元数
    std::vector<Neuron>	vecNeurons;
    NeuronLayer(int neurons, int perNeuron):NumNeurons(neurons){
        for(int i=0;i<NumNeurons;i++)
            vecNeurons.push_back(Neuron(perNeuron));
    }
};

typedef std::vector<double> iovector;

class NeuralNet{
public:
    int NumInputs;//输入量
    int NumOutputs;//输出量
    int NumHiddenLayers;//隐藏层数
    int NeuronsPerHiddenLayer;//隐藏层拥有的神经元
    double LearningRate;//学习率
    double ErrorSum;//误差总值
    bool Trained;//是否已经训练过
    int NumEpochs;//代数
    std::vector<NeuronLayer> vecLayers;//层数
    bool NetworkTrainingEpoch(std::vector<iovector > &SetIn,std::vector<iovector > &SetOut);//训练神经网络
    void CreateNet();//生成网络
    void InitializeNetwork();//初始化
    inline double Sigmoid(double activation, double response);
    NeuralNet(int inputs,int outputs,int hiddenneurons,double learningRate);
    std::vector<double> Update(std::vector<double> inputs);
    bool Train(Data* data);

};

bool NeuralNet::NetworkTrainingEpoch(std::vector<iovector> &SetIn, std::vector<iovector> &SetOut){
    std::vector<double>::iterator curWeight;//指向某个权重
    std::vector<Neuron>::iterator curNrnOut,curNrnHid;//指向输出神经元和某个隐藏神经元
    ErrorSum=0;//置零
    //对每一个输入集合调整权值
    for(unsigned int vec=0;vec<SetIn.size();vec++){
        std::vector<double> outputs=Update(SetIn[vec]);//通过神经网络获得输出
        //根据每一个输出神经元的输出调整权值
        for(int op=0;op<NumOutputs;op++){
            double err=(SetOut[vec][op]-outputs[op])*outputs[op]*(1-outputs[op]);//误差的平方
            ErrorSum+=(SetOut[vec][op]-outputs[op])*(SetOut[vec][op]-outputs[op]);//计算误差总和，用于暂停训练
            vecLayers[1].vecNeurons[op].Error=err;//更新误差（输出层）
            curWeight=vecLayers[1].vecNeurons[op].vecWeight.begin();//标记第一个权重
            curNrnHid=vecLayers[0].vecNeurons.begin();//标记隐藏层第一个神经元
            //对该神经元的每一个权重进行调整
            while(curWeight!=vecLayers[1].vecNeurons[op].vecWeight.end()-1){
                *curWeight+=err*LearningRate*curNrnHid->Activation;//根据误差和学习率和阈值调整权重
                curWeight++;//指向下一个权重
                curNrnHid++;//指向下一个隐藏层神经元
            }
            *curWeight+=err*LearningRate*(-1);//偏移值
        }
        curNrnHid=vecLayers[0].vecNeurons.begin();//重新指向隐藏层第一个神经元
        int n=0;
        //对每一个隐藏层神经元
        while(curNrnHid!=vecLayers[0].vecNeurons.end()-1){
            double err=0;
            curNrnOut=vecLayers[1].vecNeurons.begin();//指向第一个输出神经元
            //对每一个输出神经元的第一个权重
            while(curNrnOut!=vecLayers[1].vecNeurons.end()){
                err+=curNrnOut->Error*curNrnOut->vecWeight[n];//某种计算误差的方法(BP)
                curNrnOut++;
            }
            err*=curNrnHid->Activation*(1-curNrnHid->Activation);//某种计算误差的方法(BP)
            for(int w=0;w<NumInputs;w++)
                curNrnHid->vecWeight[w]+=err*LearningRate*SetIn[vec][w];//根据误差更新隐藏层的权重
            curNrnHid->vecWeight[NumInputs]+=err*LearningRate*(-1);//偏移值
            curNrnHid++;//下一个隐藏层神经元
            n++;//下一个权重
        }
    }
    return 1;
}

void NeuralNet::CreateNet(){
    if(NumHiddenLayers>0){
        vecLayers.push_back(NeuronLayer(NeuronsPerHiddenLayer,NumInputs));
        for(int i=0;i<NumHiddenLayers-1;++i)
            vecLayers.push_back(NeuronLayer(NeuronsPerHiddenLayer,NeuronsPerHiddenLayer));
        vecLayers.push_back(NeuronLayer(NumOutputs,NeuronsPerHiddenLayer));
    }
    else{
        vecLayers.push_back(NeuronLayer(NumOutputs,NumInputs));
    }
}

void NeuralNet::InitializeNetwork(){
    for(int i=0;i<NumHiddenLayers+1;++i)
        for(int n=0;n<vecLayers[i].NumNeurons;++n)
            for(int k=0;k<vecLayers[i].vecNeurons[n].NumInputs;++k)
                vecLayers[i].vecNeurons[n].vecWeight[k]=RandomClamped();//随机生成权重
    ErrorSum=9999;
    NumEpochs=0;
}

double NeuralNet::Sigmoid(double activation, double response){
    return ( 1 / ( 1 + exp(-activation / response)));
}

NeuralNet::NeuralNet(int inputs, int outputs, int hiddenneurons, double learningRate):
    NumInputs(inputs),
    NumOutputs(outputs),
    NumHiddenLayers(1),
    NeuronsPerHiddenLayer(hiddenneurons),
    LearningRate(learningRate),
    ErrorSum(9999),
    Trained(false),
    NumEpochs(0){
    CreateNet();
}

std::vector<double> NeuralNet::Update(std::vector<double> inputs){
    std::vector<double> outputs;
    int cWeight = 0;
    if (inputs.size()!=NumInputs)
        return outputs;
    for(int i=0;i<NumHiddenLayers+1;++i){
        if(i>0)
            inputs=outputs;
        outputs.clear();
        cWeight = 0;
        for(int n=0;n<vecLayers[i].NumNeurons;++n){
            double netinput=0;
            int	numInputs=vecLayers[i].vecNeurons[n].NumInputs;
            for (int k=0;k<numInputs-1;++k)
                netinput+=vecLayers[i].vecNeurons[n].vecWeight[k]*inputs[cWeight++];
            netinput+=vecLayers[i].vecNeurons[n].vecWeight[numInputs-1]*(-1);
            vecLayers[i].vecNeurons[n].Activation=Sigmoid(netinput,ACTIVATION_RESPONSE);
            outputs.push_back(vecLayers[i].vecNeurons[n].Activation);//即输出
            cWeight = 0;
        }
    }
    return outputs;
}

bool NeuralNet::Train(Data *data){
    std::vector<std::vector<double> > SetIn=data->GetInputSet();
    std::vector<std::vector<double> > SetOut=data->GetOutputSet();
    InitializeNetwork();
    while(ErrorSum>ERROR_THRESHOLD)//训练直到误差最低
        NetworkTrainingEpoch(SetIn,SetOut);
    Trained=true;
    return 1;
}

#endif // NEURALNET
