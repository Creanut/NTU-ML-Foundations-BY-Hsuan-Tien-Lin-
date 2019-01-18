#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

#define DEMENSION 5 //定义数据维度（4维数据+1维门限值【1】）

struct record //样本结构体（x为5维的向量，包含原始数据和一维的门限值，y为标签值）
{
	double x[DEMENSION];
	int y;
};

void get_data(fstream &data_file,vector<record> &data_set)
{
	while(!data_file.eof())
	{
		record cur_record;
		cur_record.x[0] = 1;//门限值均赋值为1，即为x0项
		int i;
		for(i = 1;i < DEMENSION;i++)
			data_file >> cur_record.x[i];//将数据中的x0~x4读入数据文件data_file内
		data_file >> cur_record.y;//将数据标签读入data_file内
		data_set.push_back(cur_record);//将record类型的数据转换为向量类型的data_set
	}
	data_file.close();
}

int sign(double x)//符号函数
{
	if(x <= 0)
		return -1;
	else
		return 1;
}

double v_v_multiply(double *v1,double *v2)//向量内积函数，并返回计算结果
{
	int i;
	double temp = 0.0;
	for(i = 0;i < DEMENSION;i++)
		temp += v1[i] * v2[i];
	return temp;
}

void num_v_multiply(double *result,double *v,int num)//向量数量积函数，参数1为计算结果
{
	int i;
	for(i = 0;i < DEMENSION;i++)
		result[i] = num * v[i];
}

void add(double *result,double *v1,double *v2)//向量加，对应维度的数据相加即可，参数1为计算结果
{
	int i;
	for(i = 0;i < DEMENSION;i++)
		result[i] = v1[i] + v2[i];
}

//计算错误率
//计算方式是，将对应的权重向量和数据集中的每组数据做内积，并与
//该组数据的实际标签作对比，如果二者不相等，则说明数据分类错误，
//错误数量计数器自增，遍历完所有数据后，求得错误数据组数的占比
//即可获得最终的错误率，该错误率表征的是这一组训练出来的权重的
//在训练集或者推理过程（测试集上）的准确率。
double get_error_rate(double *weight,vector<record> data_set)
{
	int n = data_set.size();//获得数据集中的向量的数量，即数据向量的个数
	double error_rate = 0.0;
	int i;
	for(i = 0;i < n;i++)
		if(sign(v_v_multiply(weight,data_set[i].x)) != data_set[i].y)
			error_rate++;
	return error_rate / n;
}

void pocket_pla(double *pocket_weights,double *weights,vector<record> training_set,int iteration)
{
	int index = 0;
	int iter = 1;
	int n = training_set.size();
	while(iter < iteration)
	{
		//如果该组权重值计算不正确，则进行修正
		//PLA的修正公式是w(i+1) = w(i) + x*y
		if(sign(v_v_multiply(training_set[index].x,weights)) != training_set[index].y)
		{
			double temp[DEMENSION];
			num_v_multiply(temp,training_set[index].x,training_set[index].y);
			int i;
			for(i = 0;i < DEMENSION;i++)
				weights[i] += temp[i];//w(i+1) = w(i) + x*y
			//pocket算法的核心：先利用PLA算法的思想更新权重系数，然后计算当前权重的weights与
			//之前放在pocket里的weights的计算错误率，如果当前这组weights的错误率比之前pocket
			//里的错误率小的话，则更新pocket里的权重，否则不更新。
			if(get_error_rate(weights,training_set) < get_error_rate(pocket_weights,training_set))
			{
				int j;
				for(j = 0;j < DEMENSION;j++)
					pocket_weights[j] = weights[j];
			}
			iter++;
		}
		if(index == n - 1)
			index = 0;
		else
			index++;
	}
}

void main()
{
	vector<record> training_set;
	vector<record> test_set;
	//读入数据
	fstream data_file1("C:/Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_1/data/data_for18_train.txt");
	fstream data_file2("C:/Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_1/data/data_for18_test.txt");
	/*char *data_file1 = "C:/Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_1/data/data_for18_train.txt";
	char *data_file2 = "C:/Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_1/data/data_for18_test.txt";*/
	//将数据转化为向量
	if(data_file1.is_open() && data_file2.is_open())
	{
		get_data(data_file1,training_set);
		get_data(data_file2,test_set);
	}
	else
	{
		cout << "can not open the file!" << endl;
		exit(1);
	}

	double weights[DEMENSION],pocket_weights[DEMENSION];
	double average_error = 0.0;

	int j;
	for(j = 0;j < 2000;j++)//运行次数循环
	{
		random_shuffle(training_set.begin(),training_set.end());

		int i;
		for(i = 0;i < DEMENSION;i++)//每次迭代都需要进行数据遍历，初始化权重
		{
			weights[i] = 0.0;
			pocket_weights[i] = 0.0;
		}
		//每次迭代只进行50次的修正
		pocket_pla(pocket_weights,weights,training_set,100);
		double training_error = get_error_rate(pocket_weights,training_set);
		double test_error = get_error_rate(pocket_weights,test_set);
		average_error += test_error;
		cout << "the " << j << " time " << "---training_error: " << training_error << " test error: " << test_error << endl;
	}
	cout << "the average error rate: " << average_error / 2000 << endl;
	while(1);
}

