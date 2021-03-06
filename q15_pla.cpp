/*
读入的数据是一组4维数据x1~x4和一维标签y,保证数据是线型可分的，本程序是使用PLA算法进行二分。
*/

#include<fstream>
#include<iostream>
#include<vector>
using namespace std;

#define DEMENSION 5 //5维数据，是因为在运算中多了一维的w0，数据中多了一维的x0 = 1;

double weight[DEMENSION];
int step = 0;
int n = 0;
char *file = "C:/Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_1/data/data.txt";

struct record
{
	double input[DEMENSION];
	int output;
};

vector<record> training_set;

void get_data(ifstream &datafile)
{
	while(!datafile.eof())
	{
		record cur_record;
		cur_record.input[0] = 1;//阈值threshold = 1
		int i;
		for(i = 1;i < DEMENSION;i++)//从x1读到x4，读入的是实际的数据值，非标签值
		{
			datafile >> cur_record.input[i];
		}
		datafile >> cur_record.output;//把最后的标签值读入
		training_set.push_back(cur_record);
	}
	datafile.close();
	n = training_set.size();
}

int sign(double x)//符号函数
{
	if(x <= 0)
		return -1;
	else
		return 1;
}

void add(double *v1,double *v2,int demension)//计算和：更新权重的时候使用（w(i+1) = w(i) + x*y）
{
	int i;
	for(i = 0;i < demension;i++)
	{
		v1[i] += v2[i];
	}
}

double v_v_multiply(double *v1,double *v2,int demension)//计算向量点乘：计算得分值的时候使用（w*x），如果是多维向量，则是对应分量相乘之后再相加
{
	double temp = 0.0;
	int i;
	for(i = 0;i < demension;i++)
	{
		temp += v1[i] * v2[i];//对应元素相乘之后再累加作为最终输出值（即向量的点积和）
	}
	return temp;
}

void num_v_multiply(double *result,double *v,int demension,int num)//计算向量数乘：计算数值与标签乘积的时候使用（y*x）
{
	int i;
	for(i = 0;i < demension;i++)
	{
		result[i] = num * v[i];
	}
}

void pla()
{
	int correct_num = 0;
	int index = 0;
	bool is_finished = 0;
	while(!is_finished)
	{
		//权重和数据乘积后进行符号判断（计算所得的符号和原数据集中所给的符号【.output】进行比较），且符号正确，则记录该点的分类是正确的
		if(training_set[index].output == sign(v_v_multiply(weight,training_set[index].input,DEMENSION)))
			correct_num++;
		//分类不正确，开始修正超平面
		else
		{
			double temp[DEMENSION];
			//先计算x和其所对应的标签值y【.output】的乘积
			num_v_multiply(temp,training_set[index].input,DEMENSION,training_set[index].output);
			//再计算w + x*y
			add(weight,temp,DEMENSION);
			step++;
			correct_num = 0;
			cout << "step" << step << ":" << endl << "index " << index << " is wrong" << endl;
		}
		if(index == n - 1)//计算到最后一个样本，然后重新遍历
			index = 0;
		else 
			index++;
		if(correct_num == n)//如果遍历所有的点之后发现正确数据的个数和总的样本个数一致，则说明所有的点均分类正确，算法收敛
			is_finished = 1;
	}
	cout <<"total step:" << step << endl;
}

void main()
{
	ifstream data_file(file);
	if(data_file.is_open())
	{
		get_data(data_file);
	}
	else
	{
		cout << "the file is error!" << endl;
		exit(1);
	}

	int i;
	for(i = 0;i < DEMENSION;i++)
		weight[i] = 0.0;
	pla();

	while(1);
}