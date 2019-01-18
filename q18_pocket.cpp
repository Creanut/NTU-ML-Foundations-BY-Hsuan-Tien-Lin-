#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

#define DEMENSION 5 //��������ά�ȣ�4ά����+1ά����ֵ��1����

struct record //�����ṹ�壨xΪ5ά������������ԭʼ���ݺ�һά������ֵ��yΪ��ǩֵ��
{
	double x[DEMENSION];
	int y;
};

void get_data(fstream &data_file,vector<record> &data_set)
{
	while(!data_file.eof())
	{
		record cur_record;
		cur_record.x[0] = 1;//����ֵ����ֵΪ1����Ϊx0��
		int i;
		for(i = 1;i < DEMENSION;i++)
			data_file >> cur_record.x[i];//�������е�x0~x4���������ļ�data_file��
		data_file >> cur_record.y;//�����ݱ�ǩ����data_file��
		data_set.push_back(cur_record);//��record���͵�����ת��Ϊ�������͵�data_set
	}
	data_file.close();
}

int sign(double x)//���ź���
{
	if(x <= 0)
		return -1;
	else
		return 1;
}

double v_v_multiply(double *v1,double *v2)//�����ڻ������������ؼ�����
{
	int i;
	double temp = 0.0;
	for(i = 0;i < DEMENSION;i++)
		temp += v1[i] * v2[i];
	return temp;
}

void num_v_multiply(double *result,double *v,int num)//��������������������1Ϊ������
{
	int i;
	for(i = 0;i < DEMENSION;i++)
		result[i] = num * v[i];
}

void add(double *result,double *v1,double *v2)//�����ӣ���Ӧά�ȵ�������Ӽ��ɣ�����1Ϊ������
{
	int i;
	for(i = 0;i < DEMENSION;i++)
		result[i] = v1[i] + v2[i];
}

//���������
//���㷽ʽ�ǣ�����Ӧ��Ȩ�����������ݼ��е�ÿ���������ڻ�������
//�������ݵ�ʵ�ʱ�ǩ���Աȣ�������߲���ȣ���˵�����ݷ������
//���������������������������������ݺ���ô�������������ռ��
//���ɻ�����յĴ����ʣ��ô����ʱ���������һ��ѵ��������Ȩ�ص�
//��ѵ��������������̣����Լ��ϣ���׼ȷ�ʡ�
double get_error_rate(double *weight,vector<record> data_set)
{
	int n = data_set.size();//������ݼ��е������������������������ĸ���
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
		//�������Ȩ��ֵ���㲻��ȷ�����������
		//PLA��������ʽ��w(i+1) = w(i) + x*y
		if(sign(v_v_multiply(training_set[index].x,weights)) != training_set[index].y)
		{
			double temp[DEMENSION];
			num_v_multiply(temp,training_set[index].x,training_set[index].y);
			int i;
			for(i = 0;i < DEMENSION;i++)
				weights[i] += temp[i];//w(i+1) = w(i) + x*y
			//pocket�㷨�ĺ��ģ�������PLA�㷨��˼�����Ȩ��ϵ����Ȼ����㵱ǰȨ�ص�weights��
			//֮ǰ����pocket���weights�ļ�������ʣ������ǰ����weights�Ĵ����ʱ�֮ǰpocket
			//��Ĵ�����С�Ļ��������pocket���Ȩ�أ����򲻸��¡�
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
	//��������
	fstream data_file1("C:/Users/Administrator/Desktop/̨�����������ѧϰ��ʯ��Դ����/work/work_1/data/data_for18_train.txt");
	fstream data_file2("C:/Users/Administrator/Desktop/̨�����������ѧϰ��ʯ��Դ����/work/work_1/data/data_for18_test.txt");
	/*char *data_file1 = "C:/Users/Administrator/Desktop/̨�����������ѧϰ��ʯ��Դ����/work/work_1/data/data_for18_train.txt";
	char *data_file2 = "C:/Users/Administrator/Desktop/̨�����������ѧϰ��ʯ��Դ����/work/work_1/data/data_for18_test.txt";*/
	//������ת��Ϊ����
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
	for(j = 0;j < 2000;j++)//���д���ѭ��
	{
		random_shuffle(training_set.begin(),training_set.end());

		int i;
		for(i = 0;i < DEMENSION;i++)//ÿ�ε�������Ҫ�������ݱ�������ʼ��Ȩ��
		{
			weights[i] = 0.0;
			pocket_weights[i] = 0.0;
		}
		//ÿ�ε���ֻ����50�ε�����
		pocket_pla(pocket_weights,weights,training_set,100);
		double training_error = get_error_rate(pocket_weights,training_set);
		double test_error = get_error_rate(pocket_weights,test_set);
		average_error += test_error;
		cout << "the " << j << " time " << "---training_error: " << training_error << " test error: " << test_error << endl;
	}
	cout << "the average error rate: " << average_error / 2000 << endl;
	while(1);
}

