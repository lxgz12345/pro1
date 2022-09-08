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
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	wchar_t* buffer = new wchar_t[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}
std::string wstring2string(std::wstring wstr) {
	std::string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}
std::vector<std::string> split(std::string str, std::string pattern) {
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
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
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char* psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_ACP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用//CP_OEMCP//CP_ACP
	szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
}

void ReadCSVUTF16LE(const std::wstring& fileName) {
	FILE* fp;//, ccs=UTF-16LE
	auto err = _wfopen_s(&fp, fileName.c_str(), L"r,ccs=UTF-16LE");
	if (fp == NULL) {
		perror("打开文件时发生错误");
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

		if (value.find("扫描控制:") != std::string::npos) {
			//printf("value.find %d %d : %s\n", CurrentLine, dataLine, value.c_str());
			std::cout << "Line:" << firstdataLine << std::endl;
			fgetws(str, 1024, fp);
			fgetws(str, 1024, fp);
			Wchar_tToString(value, str);
			CurrentLine = firstdataLine;
			//firstdataLine = CurrentLine + 2;
		} else if (CurrentLine >= firstdataLine) {
			auto result = split(value, "\t");
			//ok
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

/*将字符串写入到剪切板*/
BOOL 复制到剪切板(const char* data) {
	int contentSize = strlen(data) + 1;
	HGLOBAL hMemory;
	LPTSTR lpMemory;
	if (!OpenClipboard(NULL)) return false;  //打开剪切板
	if (!EmptyClipboard()) return false;     //清空剪切板
	if (!(hMemory = GlobalAlloc(GMEM_MOVEABLE, contentSize))) return false;  //对剪切板分配内存
	if (!(lpMemory = (LPTSTR)GlobalLock(hMemory))) return false;  //锁定内存区域
	memcpy_s(lpMemory, contentSize, data, contentSize);  //复制数据到内存区域
	GlobalUnlock(hMemory);                               //解除内存锁定
	if (!SetClipboardData(CF_TEXT, hMemory)) return false;  //设置剪切板数据
	CloseClipboard();                                       //关闭剪切板
	printf("已复制结果！\n");
	return true;
}

//检测文件是否存在
BOOL 文件检查(LPCSTR lpszFileName) {
	if (!PathFileExistsA(lpszFileName)) {
		OutputDebugStringA("File Not Exist！");
		return false;
	}
	DWORD dwAttributes = ::GetFileAttributesA(lpszFileName);//试图取得文件属性
	if (dwAttributes == INVALID_FILE_ATTRIBUTES) {//判断是否为空
		return false;
	}
	printf("文件类型:%d\n", dwAttributes);
	if (dwAttributes != 32) {
		return false;
	}
	return true;
}



//处理文件的主函数
std::string 处理(std::string file) {
	if (!文件检查(file.c_str())) {
		printf("%s 不是文件或不存在！\n", file.c_str());
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
	std::string 返回 = "";
	返回 = 处理("1-01-反.csv");
	//if (argc <= 1) {
	//	return 0;
	//}
	//if (argc == 2) {
	//	返回 = 处理(argv[1]);
	//}
	//if (argc > 2) {
	//	for (int i = 1; i < argc; i++) {
	//		返回 = 返回 + 处理(argv[i]) + "\n";
	//	}
	//}

	if (返回 != "") {
		if (!复制到剪切板(返回.c_str())) {
			printf("结果复制失败！正在输出....\n");
			printf("%s\n", 返回.c_str());
		}
	}
	return 0;
}