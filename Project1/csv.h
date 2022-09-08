#pragma once
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <stdlib.h>
#include <codecvt>

//std::u16string to_utf16(std::string str) {
//	return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.from_bytes(str);
//}
//std::string to_utf8(std::u16string str16) {
//	return std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(str16);
//}
//std::u32string to_utf32(std::string str) {
//	return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.from_bytes(str);
//}
//std::string to_utf8(std::u32string str32) {
//	return std::wstring_convert< std::codecvt_utf8<char32_t>, char32_t >{}.to_bytes(str32);
//}
//std::wstring to_wchar_t(std::string str) {
//	return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.from_bytes(str);
//}
//std::string to_utf8(std::wstring wstr) {
//	return std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t >{}.to_bytes(wstr);
//}

class csv {
public:
	csv();
	static std::vector<std::vector<std::string>> get_string_data(std::string name, char sc = ',');
	static void save_data(std::vector<std::vector<std::string>> all_data, std::string name, char sc);
	static void print_data(std::vector<std::vector<std::string>> data);

	static std::vector<std::vector<double>> get_double_data(std::string name, char sc = ',');
	static void save_data(std::vector<std::vector<double>> all_data, std::string name, char sc);
	static void print_data(std::vector<std::vector<double>> data);
	static std::vector<std::vector<double>> transpose(std::vector<std::vector<double>> data);
};


csv::csv() {
}

std::vector<std::vector<std::string>> csv::get_string_data(std::string name, char sc) {
	std::ifstream file;
	std::vector<std::vector<std::string> > all_data;
	file.open(name.c_str());
	if (file.is_open()) {
		// 初始化变量
		std::string file_line_data;
		std::string file_line_data_word;
		std::vector<std::string> line_data;

		while (std::getline(file, file_line_data)) {
			for (int i = 0; i < (int)file_line_data.size(); i++) {
				if (file_line_data[i] != sc) {
					file_line_data_word.push_back(file_line_data[i]);
				} else {
					line_data.push_back(file_line_data_word);
					file_line_data_word.clear();
				}
			}
			line_data.push_back(file_line_data_word);
			file_line_data_word.clear();

			all_data.push_back(line_data);
			line_data.clear();
		}
		file.close();
	} else {
		std::cout << "错误:无法打开文件!" << std::endl;
	}
	return all_data;
}

std::vector<std::vector<double>> csv::get_double_data(std::string name, char sc) {
	std::ifstream file;
	std::vector<std::vector<double> > all_data;
	file.open(name.c_str());
	if (file.is_open()) {
		// 初始化变量
		std::string file_line_data;
		std::string file_line_data_word;
		std::vector<double> line_data;
		double double_data_temp;

		while (std::getline(file, file_line_data)) {
			for (int i = 0; i < (int)file_line_data.size(); i++) {
				if (file_line_data[i] != sc) {
					file_line_data_word.push_back(file_line_data[i]);
				} else {
					if ((int)file_line_data_word.size() > 0) {
						std::stringstream ss(file_line_data_word);
						ss >> double_data_temp;
						line_data.push_back(double_data_temp);
						file_line_data_word.clear();
					}
				}
			}
			if ((int)file_line_data_word.size() > 0) {
				std::stringstream ss(file_line_data_word);
				ss >> double_data_temp;
				line_data.push_back(double_data_temp);
				file_line_data_word.clear();
			}

			all_data.push_back(line_data);
			line_data.clear();
		}
		file.close();
	} else {
		std::cout << "错误:无法打开文件!" << std::endl;
	}
	return all_data;
}

void csv::save_data(std::vector<std::vector<std::string>> all_data, std::string name, char sc) {
	std::ofstream file;
	file.open(name.c_str());
	if (file.is_open()) {
		for (int i = 0; i < (int)all_data.size(); i++) {
			for (int j = 0; j < (int)all_data[i].size(); j++) {
				file << all_data[i][j];
				if (j < (int)all_data[i].size() - 1) file << sc;
			}
			if (i + 1 < (int)all_data.size()) file << "\n";
		}
		file.close();
	} else {
		std::cout << "错误:无法打开文件!" << std::endl;
	}
}

void csv::save_data(std::vector<std::vector<double>> all_data, std::string name, char sc) {
	std::ofstream file;
	file.open(name.c_str());
	if (file.is_open()) {
		for (int i = 0; i < (int)all_data.size(); i++) {
			for (int j = 0; j < (int)all_data[i].size(); j++) {
				file << all_data[i][j];
				if (j < (int)all_data[i].size() - 1) file << sc;
			}
			if (i + 1 < (int)all_data.size()) file << "\n";
		}
		file.close();
	} else {
		std::cout << "错误:无法打开文件!" << std::endl;
	}
}

void csv::print_data(std::vector<std::vector<std::string>> data) {
	for (int i = 0; i < (int)data.size(); i++) {
		for (int j = 0; j < (int)data[i].size(); j++) {
			std::cout << data[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void csv::print_data(std::vector<std::vector<double>> data) {
	for (int i = 0; i < (int)data.size(); i++) {
		for (int j = 0; j < (int)data[i].size(); j++) {
			std::cout << data[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

std::vector<std::vector<double>> csv::transpose(std::vector<std::vector<double>> data) {
	// 检查数据是否为方形，是否可以进行换位
	for (int i = 0, s = (int)data[0].size(); i < (int)data.size(); i++) {
		if ((int)data[i].size() != s) {
			std::cout << "这个函数不能转换数据!!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	std::vector<std::vector<double>> outtrans(data[0].size(), std::vector<double>(data.size()));
	for (size_t i = 0; i < data.size(); ++i)
		for (size_t j = 0; j < data[0].size(); ++j)
			outtrans[j][i] = data[i][j];
	return outtrans;
}