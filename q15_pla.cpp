/*
�����������һ��4ά����x1~x4��һά��ǩy,��֤���������Ϳɷֵģ���������ʹ��PLA�㷨���ж��֡�
*/

#include<fstream>
#include<iostream>
#include<vector>
using namespace std;

#define DEMENSION 5 //5ά���ݣ�����Ϊ�������ж���һά��w0�������ж���һά��x0 = 1;

double weight[DEMENSION];
int step = 0;
int n = 0;
char *file = "C:/Users/Administrator/Desktop/̨�����������ѧϰ��ʯ��Դ����/work/work_1/data/data.txt";

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
		cur_record.input[0] = 1;//��ֵthreshold = 1
		int i;
		for(i = 1;i < DEMENSION;i++)//��x1����x4���������ʵ�ʵ�����ֵ���Ǳ�ǩֵ
		{
			datafile >> cur_record.input[i];
		}
		datafile >> cur_record.output;//�����ı�ǩֵ����
		training_set.push_back(cur_record);
	}
	datafile.close();
	n = training_set.size();
}

int sign(double x)//���ź���
{
	if(x <= 0)
		return -1;
	else
		return 1;
}

void add(double *v1,double *v2,int demension)//����ͣ�����Ȩ�ص�ʱ��ʹ�ã�w(i+1) = w(i) + x*y��
{
	int i;
	for(i = 0;i < demension;i++)
	{
		v1[i] += v2[i];
	}
}

double v_v_multiply(double *v1,double *v2,int demension)//����������ˣ�����÷�ֵ��ʱ��ʹ�ã�w*x��������Ƕ�ά���������Ƕ�Ӧ�������֮�������
{
	double temp = 0.0;
	int i;
	for(i = 0;i < demension;i++)
	{
		temp += v1[i] * v2[i];//��ӦԪ�����֮�����ۼ���Ϊ�������ֵ���������ĵ���ͣ�
	}
	return temp;
}

void num_v_multiply(double *result,double *v,int demension,int num)//�����������ˣ�������ֵ���ǩ�˻���ʱ��ʹ�ã�y*x��
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
		//Ȩ�غ����ݳ˻�����з����жϣ��������õķ��ź�ԭ���ݼ��������ķ��š�.output�����бȽϣ����ҷ�����ȷ�����¼�õ�ķ�������ȷ��
		if(training_set[index].output == sign(v_v_multiply(weight,training_set[index].input,DEMENSION)))
			correct_num++;
		//���಻��ȷ����ʼ������ƽ��
		else
		{
			double temp[DEMENSION];
			//�ȼ���x��������Ӧ�ı�ǩֵy��.output���ĳ˻�
			num_v_multiply(temp,training_set[index].input,DEMENSION,training_set[index].output);
			//�ټ���w + x*y
			add(weight,temp,DEMENSION);
			step++;
			correct_num = 0;
			cout << "step" << step << ":" << endl << "index " << index << " is wrong" << endl;
		}
		if(index == n - 1)//���㵽���һ��������Ȼ�����±���
			index = 0;
		else 
			index++;
		if(correct_num == n)//����������еĵ�֮������ȷ���ݵĸ������ܵ���������һ�£���˵�����еĵ��������ȷ���㷨����
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