#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <map>
#include "csv.h"
#include <locale>
#include <codecvt>
#include<Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
//using namespace std;
//#pragma warning(disable : 4996)

class Row {
public:
	std::vector<double> data;
	Row() = default;
	~Row() = default;
	Row(std::vector<double> _data) {
		data = _data;
	}
};
std::map<int, Row> mapData;

std::wstring string2wstring(std::string str) {
	std::wstring result;
	//��ȡ��������С��������ռ䣬��������С���ַ�����  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	wchar_t* buffer = new wchar_t[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β  
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}
std::string wstring2string(std::wstring wstr) {
	std::string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}
std::vector<std::string> split(std::string str, std::string pattern) {
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();
	for (int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
void Wchar_tToString(std::string& szDst, wchar_t* wchar) {
	wchar_t* wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
	char* psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����//CP_OEMCP//CP_ACP
	szDst = psText;// std::string��ֵ
	delete[]psText;// psText�����
}

void ReadCSVUTF16LE(const std::wstring& fileName) {
	FILE* fp;//, ccs=UTF-16LE
	auto err = _wfopen_s(&fp, fileName.c_str(), L"r,ccs=UTF-16LE");
	if (fp == NULL) {
		perror("���ļ�ʱ��������");
		return;
	}
	wchar_t str[1024] = { 0 };
	int CurrentLine = 1;
	int firstdataLine = INT_MAX;
	while (fgetws(str, 1024, fp) != NULL) {

		std::string value;
		Wchar_tToString(value, str);
		//value.erase(0, value.find_first_not_of("\n\r"));
		//value.erase(value.find_last_not_of("\n\r") + 1);
		if (value.empty()) { continue; }

		if (value.find("ɨ�����:") != std::string::npos) {
			//printf("value.find %d %d : %s\n", CurrentLine, dataLine, value.c_str());
			std::cout << "Line:" << firstdataLine << std::endl;
			fgetws(str, 1024, fp);
			fgetws(str, 1024, fp);
			Wchar_tToString(value, str);
			CurrentLine = firstdataLine;
			//firstdataLine = CurrentLine + 2;
		} else if (CurrentLine >= firstdataLine) {
			auto result = split(value, "\t");

		} else {
			continue;
		}



		//printf("i == %d, dataLine == %d\n", i, dataLine);
		//if () {
		//
		//} else if (i >= dataLine) {
		//	printf("i >= dataLine : %s\n", value.c_str());
		//	std::vector<double> data;
		//	data.insert(data.end(), std::stod("0.222222256"));
		//	Row table(data);
		//	mapData.insert(std::pair<int, Row>(i, table));
		//}



		//std::stringstream ss(value);
		//while (std::getline(ss, value, '\t')) {
		//	arrValue.push_back(value);
		//}
		//if (arrValue.size() < 2) {
		//	continue;
		//}
		//MultiMap.insert(std::pair<int, std::string>(std::stoi(arrValue[0]), arrValue[1]));//!!!!!!!!!!!!!!!!s
		CurrentLine++;
	}
	fclose(fp);
	return;
}

/*���ַ���д�뵽���а�*/
BOOL ���Ƶ����а�(const char* data) {
	int contentSize = strlen(data) + 1;
	HGLOBAL hMemory;
	LPTSTR lpMemory;
	if (!OpenClipboard(NULL)) return false;  //�򿪼��а�
	if (!EmptyClipboard()) return false;     //��ռ��а�
	if (!(hMemory = GlobalAlloc(GMEM_MOVEABLE, contentSize))) return false;  //�Լ��а�����ڴ�
	if (!(lpMemory = (LPTSTR)GlobalLock(hMemory))) return false;  //�����ڴ�����
	memcpy_s(lpMemory, contentSize, data, contentSize);  //�������ݵ��ڴ�����
	GlobalUnlock(hMemory);                               //����ڴ�����
	if (!SetClipboardData(CF_TEXT, hMemory)) return false;  //���ü��а�����
	CloseClipboard();                                       //�رռ��а�
	printf("�Ѹ��ƽ����\n");
	return true;
}

//����ļ��Ƿ����
BOOL �ļ����(LPCSTR lpszFileName) {
	if (!PathFileExistsA(lpszFileName)) {
		OutputDebugStringA("File Not Exist��");
		return false;
	}
	DWORD dwAttributes = ::GetFileAttributesA(lpszFileName);//��ͼȡ���ļ�����
	if (dwAttributes == INVALID_FILE_ATTRIBUTES) {//�ж��Ƿ�Ϊ��
		return false;
	}
	printf("�ļ�����:%d\n", dwAttributes);
	if (dwAttributes != 32) {
		return false;
	}
	return true;
}



//�����ļ���������
std::string ����(std::string file) {
	if (!�ļ����(file.c_str())) {
		printf("%s �����ļ��򲻴��ڣ�\n", file.c_str());
		return "";
	}
	//std::wstring _file(file.begin(), file.end());
	ReadCSVUTF16LE(string2wstring(file));
	for (int i = 0; i < (int)mapData.size(); i++) {
		std::cout << "mymap:" << mapData[i].data.at(i) << "\n";
	}

	return file;
}

int main(int argc, char** argv) {
	std::string ���� = "";
	���� = ����("1-01-��.csv");
	//if (argc <= 1) {
	//	return 0;
	//}
	//if (argc == 2) {
	//	���� = ����(argv[1]);
	//}
	//if (argc > 2) {
	//	for (int i = 1; i < argc; i++) {
	//		���� = ���� + ����(argv[i]) + "\n";
	//	}
	//}

	if (���� != "") {
		if (!���Ƶ����а�(����.c_str())) {
			printf("�������ʧ�ܣ��������....\n");
			printf("%s\n", ����.c_str());
		}
	}
	return 0;
}