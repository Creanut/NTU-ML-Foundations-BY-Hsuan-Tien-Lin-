#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

#define DEMENSION 5

double weight[DEMENSION];
int step = 0;
int total_step = 0;
int n = 0;
char *file = "C:/Users/Administrator/Desktop/台大林轩田机器学习基石资源汇总/work/work_1/data/data.txt";

struct record
{
	double input[DEMENSION];
	int output;
};

vector<record> training_set;

void get_data(ifstream &data_file)
{
	while(!data_file.eof())
	{
		record cur_record;
		cur_record.input[0] = 1;
		int i;
		for(i = 1;i < DEMENSION;i++)
		{
			data_file >> cur_record.input[i];
		}
		data_file >> cur_record.output;
		training_set.push_back(cur_record);
	}
	data_file.close();
	n = training_set.size();
}

int sign(double x)
{
	if(x <= 0)
		return -1;
	else
		return 1;
}

void add(double *v1,double *v2,int demension)
{
	int i;
	for(i = 0;i < demension;i++)
	{
		v1[i] += v2[i];
	}
}

double v_v_multiply(double *v1,double *v2,int demension)
{
	double temp = 0.0;
	int i;
	for(i = 0;i < demension;i++)
	{
		temp += v1[i]*v2[i];
	}
	return temp;
}

void num_v_multiply(double *result,double *v,int demension,double num)
{
	int i;
	for(i = 0;i < demension;i++)
	{
		//在这里更改算法的学习率，直接在num之前添加学习率系数，计算起来比较简单
		result[i] = 0.5 * num * v[i];
	}
}

void pla()
{
	int correct_num = 0;
	int index = 0;
	bool is_finished = 0;
	while(!is_finished)
	{
		if(training_set[index].output == sign(v_v_multiply(weight,training_set[index].input,DEMENSION)))
			correct_num++;
		else
		{
			double temp[DEMENSION];
			double temp_temp[DEMENSION];
			num_v_multiply(temp,training_set[index].input,DEMENSION,training_set[index].output);
			add(weight,temp,DEMENSION);
			//上述默认学习率，也就是x*y这一项前的系数为1，如果改变学习率为0.5，则算法变为：
			//num_v_multiply(temp,training_set[index].input,DEMENSION,training_set[index].output);
			//add(weight,num_v_multiply(temp_temp[index],temp,DEMENSION,0.5),DEMENSION);
			step++;
			correct_num = 0;
		}
		if(index == n - 1)
			index = 0;
		else
			index++;
		if(correct_num == n)
			is_finished = 1;
	}
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
	for(i = 0;i < 2000;i++)
	{
		random_shuffle(training_set.begin(),training_set.end());
		
		int j;
		for(j = 0;j < DEMENSION;j++)
			weight[j] = 0.0;
		pla();
		total_step += step;
		cout << "this is the " << i << " time " << ",and the step is:" << step << endl;
		step = 0;
	}
	cout << "the average step is:" <<total_step/2000 << endl;
	while(1);
}

