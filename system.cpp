#include<iostream>
#include<Python.h>
using namespace std;
int main()
{
	Py_Initialize();//ʹ��python֮ǰ��Ҫ����Py_Initialize();����������г�ʼ��
	if (!Py_IsInitialized())
	{
 		printf("��ʼ��ʧ�ܣ�");
 		return 0;
 	}
 	PyRun_SimpleString("import sys");
 	PyRun_SimpleString("sys.path.append('./')");//��һ������Ҫ���޸�Python·��
 	PyObject* pModule = NULL;//��������
 	PyObject* pFunc = NULL;// ��������
 	pModule = PyImport_ImportModule("health_up");//������Ҫ���õ��ļ���health_up.py
 	if (pModule == NULL)
 	{
 		cout << "û�ҵ�" << endl;
 	}
 	pFunc = PyObject_GetAttrString(pModule, "run");//������Ҫ���õĺ�����
 	//�����ַ�������
 	PyObject* pParams = Py_BuildValue("()");
 	PyObject* pRet = PyObject_CallObject(pFunc, pParams);//���ú���
 	Py_Finalize();//����Py_Finalize�������Py_Initialize���Ӧ�ġ�
 	return 0;
}
