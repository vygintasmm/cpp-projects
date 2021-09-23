// virtuali_masina.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<iomanip>
#include<map>
#include<bitset>
#include<cstdlib>
#include<cstdint>

using namespace std;


vector<string>prg;
bool e_o_f = false;

void read_bin(string bin_name) {
	fstream binary_file;
	binary_file.open(bin_name, ios::in | ios::out | ios::binary);
	char* buffer = new char[1];

	while (!binary_file.eof()) {
		binary_file.read(buffer, 1);
		stringstream ss;
		for (size_t i = 0; i < 1; ++i) {
			ss << hex << setw(2) << setfill('0') << static_cast<int>(buffer[i]);
		}
		
		string rez(ss.str());
		if (rez.length() > 2) {
			string save = rez;
			rez = save[save.length() - 2];
			rez+= save[save.length() - 1];
		}
		prg.push_back(rez);
	}
	
}


void handle_regs(string fl_name) {
	map<char, int>registers;
	int flag = 0;
	ifstream line(fl_name);
	string param;
	char reg_x;
	char reg_y;
	int index = 0;


	while (true) {
		if (prg[index] == "01") {
			param = prg[index + 1];
			reg_x = param[1];
			registers[reg_x] += 1;
			index += 2;
		}
		else if (prg[index] == "02") {
			param = prg[index + 1];
			reg_x = param[1];
			registers[reg_x] -= 1;
			index += 2;
		}
		else if (prg[index] == "03") {
			param = prg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_y];
			index += 2;
		}
		else if (prg[index] == "04") {
			param = prg[index + 1];
			stringstream ss;
			ss << param;
			unsigned int constant;
			ss >> hex >> constant;
			registers['0'] = constant;
			index += 2;
		}
		else if (prg[index] == "05") {
			param = prg[index + 1];
			reg_x = param[1];
			int value = registers[reg_x];
			string binary_of_value = bitset<8>(value).to_string();
			//shift
			binary_of_value += '0';
			binary_of_value = binary_of_value.substr(1);
			//back to int
			int ats = stoi(binary_of_value, nullptr, 2);
			registers[reg_x] = ats;
			index += 2;

		}
		else if (prg[index] == "06") {
			param = prg[index + 1];
			reg_x = param[1];
			int value = registers[reg_x];
			string binary_of_value = bitset<8>(value).to_string();
			//shift
			string att = "0";
			att += binary_of_value;
			string ans=att.substr(0, att.size() - 1);
			//back to int
			int ats = stoi(ans, nullptr, 2);
			registers[reg_x] = ats;
			index += 2;
		}
		else if (prg[index] == "07") {
			param = prg[index + 1];
			
			unsigned int x;
			stringstream ss;
			ss << param;
			ss >> hex >> x;
			int8_t jump = (short)x;
			index += jump;

		}
		else if (prg[index] == "08") {
			index += 2;
		}
		else if (prg[index] == "09") {
			index += 2;
		}
		else if (prg[index] == "0a") {
			param = prg[index + 1];
			stringstream ss;
			ss << param;
			signed int sk;
			ss >> hex >> sk;
			if (flag == 1)index += sk;
			else {
				index += 2;
			}
		}
		else if (prg[index] == "0b") {
			break; //???
		}
		else if (prg[index] == "0c") {
			param = prg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_y] + registers[reg_x];
			index += 2;
		}
		else if (prg[index] == "0d") {
			param = prg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_x] - registers[reg_y];
			index += 2;
		}
		else if (prg[index] == "0e") {
			//xor
			param = prg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_x] ^ registers[reg_y];
			index += 2;
		}
		else if (prg[index] == "0f") {
			//or
			param = prg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_x] | registers[reg_y];
			index += 2;
		}
		else if (prg[index] == "10") {
			if (line.eof())flag=1;
			param = prg[index + 1];
			reg_x = param[1];
			char input;
			line >> input;
			int inp = (int)input;
			registers[reg_x] = inp;
			index += 2;
		}
		else if (prg[index] == "11") {
			param = prg[index + 1];
			reg_x = param[1];
			cout << (char)registers[reg_x];
			index += 2;
		}
	}



}




void code_sentence(string sentence) {
	int size_of_sentence = sentence.length();
	vector<int>numbers_to_encrypt;
	for (int i = 0; i < size_of_sentence; ++i) {
		char ch = sentence[i];
		int a = (int)ch;
		int vn = a / 2;
		int dd = a - vn;
		numbers_to_encrypt.push_back(vn);
		numbers_to_encrypt.push_back(dd);
	}



	vector<string>rprg;
	map<char, int>rev_regs;


	fstream binary_file;
	binary_file.open("decriptor.bin", ios::in | ios::out | ios::binary);
	char* buffer = new char[1];

	while (!binary_file.eof()) {
		binary_file.read(buffer, 1);
		stringstream ss;
		for (size_t i = 0; i < 1; ++i) {
			ss << hex << setw(2) << setfill('0') << static_cast<int>(buffer[i]);
		}

		string rez(ss.str());
		if (rez.length() > 2) {
			string save = rez;
			rez = save[save.length() - 2];
			rez += save[save.length() - 1];
		}
		rprg.push_back(rez);
	}

	ofstream rex("encc.txt");

	
	char cx;
	char cy;

	map<char, int>registers;
	string param;
	int flag = 0;
	int check_ind = 0;
	char reg_x;
	char reg_y;
	int index = 0;

	while (true) {
		if (rprg[index] == "01") {
			param = rprg[index + 1];
			reg_x = param[1];
			registers[reg_x] -= 1;
			index += 2;
		}
		else if (rprg[index] == "02") {
			param = rprg[index + 1];
			reg_x = param[1];
			registers[reg_x] += 1;
			index += 2;
		}
		else if (rprg[index] == "03") {
			param = rprg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_y];
			index += 2;
		}
		else if (rprg[index] == "04") {
			param = rprg[index + 1];
			stringstream ss;
			ss << param;
			unsigned int constant;
			ss >> hex >> constant;
			registers['0'] = constant;
			index += 2;
		}
		else if (rprg[index] == "06") {
			param = rprg[index + 1];
			reg_x = param[1];
			int value = registers[reg_x];
			string binary_of_value = bitset<8>(value).to_string();
			//shift
			binary_of_value += '0';
			binary_of_value = binary_of_value.substr(1);
			//back to int
			int ats = stoi(binary_of_value, nullptr, 2);
			registers[reg_x] = ats;
			index += 2;

		}
		else if (rprg[index] == "05") {
			param = rprg[index + 1];
			reg_x = param[1];
			int value = registers[reg_x];
			string binary_of_value = bitset<8>(value).to_string();
			//shift
			string att = "0";
			att += binary_of_value;
			string ans = att.substr(0, att.size() - 1);
			//back to int
			int ats = stoi(ans, nullptr, 2);
			registers[reg_x] = ats;
			index += 2;
		}
		else if (rprg[index] == "07") {
			param = rprg[index + 1];

			unsigned int x;
			stringstream ss;
			ss << param;
			ss >> hex >> x;
			int8_t jump = (short)x;
			index += jump;

		}
		else if (rprg[index] == "08") {
			index += 2;
		}
		else if (rprg[index] == "09") {
			index += 2;
		}
		else if (rprg[index] == "0a") {
			param = rprg[index + 1];
			stringstream ss;
			ss << param;
			signed int sk;
			ss >> hex >> sk;
			if (flag == 1)index += sk;
			else {
				index += 2;
			}
		}
		else if (rprg[index] == "0b") {
			break; //???
		}
		else if (rprg[index] == "0c") {
			param = rprg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			cx = reg_x;
			cy = reg_y;
			index += 2;
		}
		else if (rprg[index] == "0d") {
			param = rprg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			cx = reg_x;
			cy = reg_y;
			index += 2;
		}
		else if (rprg[index] == "0f") {
			//xor
			param = rprg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_x] ^ registers[reg_y];
			index += 2;
		}
		else if (rprg[index] == "0e") {
			//or
			param = rprg[index + 1];
			reg_x = param[1];
			reg_y = param[0];
			registers[reg_x] = registers[reg_x] | registers[reg_y];
			index += 2;
		}
		else if (rprg[index] == "10") {
			if (check_ind==numbers_to_encrypt.size()-1)flag = 1;
			param = rprg[index + 1];
			reg_x = param[1];
			int input;
			input = numbers_to_encrypt[check_ind];
			registers[reg_x] = input;
			check_ind += 1;
			index += 2;
		}
		else if (rprg[index] == "11") {
		rex << (char)registers[cx];
		rex << (char)registers[cy];
			index += 2;
		}
	}
}



int main()
{
	bool repeat = true;

	while (repeat) {
		cout << "iveskite instrukciju failo pavadinima" << endl;
		string bin_file_name;
		cin >> bin_file_name;
		cout << "iveskite atkoduoti norimo failo pavadinima" << endl;
		string decode_file_name;
		cin >> decode_file_name;

		read_bin(bin_file_name);
		handle_regs(decode_file_name);
		cout << " atkoduota!" << endl;
		prg.clear();


		cout << "iveskite faila su sakiniu atkodavimui" << endl;
		string sentence;
		cin >> sentence;
		fstream fin(sentence, fstream::in);
		char temp;
		string enc_str = "";
		while (fin >> noskipws >> temp) {
			enc_str += temp;
		}

		code_sentence(enc_str);

		cout << "noredami kartoti iveskite 'taip', bet kokia kita ivestis bus traktuojama kaip 'ne'" << endl;
		string ans;
		cin >> ans;
		if (ans == "taip")repeat = true;
		else { repeat = false; };
	}

	

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
